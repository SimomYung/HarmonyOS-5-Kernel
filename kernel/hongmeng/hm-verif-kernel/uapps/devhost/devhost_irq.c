/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interrupt support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 14 14:39:12 2019
 */
#include "devhost.h"

#include <sched.h>
#include <malloc.h>
#include <sched.h>
#include <hmasm/page.h>
#include <libalgo/idr.h>
#include <hmkernel/rq.h>
#include <hmkernel/types.h>
#include <hmkernel/drivers/virq.h>
#include <hmkernel/capability.h>
#include <hmkernel/sched/sched.h>
#include <hmkernel/sched/priority.h>
#include <hmkernel/sched_module.h>
#include <hmkernel/interrupt/kvic.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libhmirq/irq.h>
#include <libhmirq/intr.h>
#include <libsysif/utils.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/uspinlock.h>
#include <libhmucap/ucap.h>
#include <libhmactv/actv.h>
#include <libentropy/entropy.h>
#include <libentropy/health_test.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libsysif/devhost/server.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhwsecurec/securec.h>
#include <libdevhost/uobject.h>
#include <libpreempt/preempt.h>

#include "devhost_api.h"
#include "devhost_irq.h"
#include <devhost/log.h>
#include "utils/utils.h"
#include <devhost/interrupt.h>

#define DEVHOST_IRQMAP_SIZE	4096
#define DEVHOST_VIRQMAP_SIZE	64

#define IRQ_TYPE_VIRT		1U
#define IRQ_TYPE(logic_irq)	((logic_irq) >> 28)
#define IRQ_ID(logic_irq)	((logic_irq) & 0xFFFFFFFU)
#define IRQ_VIRT(logic_id)	((IRQ_TYPE_VIRT << 28) | (logic_id))

#define IRQ_IPI_NAME		"dh-ipi"

/*
 * kvic irq range linux irq 1-15
 * Current only uart use kvic linux irq = 1
 */
#define KVIC_LINUX_IRQ_RANGE		16

struct devhost_irq_info {
	int linux_irq;		/* linux irq */
	/*
	 * kvic irq (linux_irq 1-15) logic_irq = kvic source_id,
	 * normal irq (linux >=16) means hm_logic_irq
	 * virt irq logic_irq equals to virt_irq
	 */
	unsigned int logic_irq;
	union {
		rref_t irq_rref;		/* rref of normal irq (not kvic) */
		bool registered;		/* kvic or virt irq registered */
	} data;
	devhost_irq_handler_t handler;	/* irq handler */
	void *dev_id;		/* dev_id is private data for handler */
	char name[IRQCTRL_NAME_LEN]; /* name of this dev */
	struct uobject obj;

	__cpuset_t thread_affinity;  /* irq thread affinity */
};

static struct uspinlock_s g_idr_spinlock = USPINLOCK_INITIALIZER;

static void irq_info_uobject_release(struct uobject *obj)
{
	struct devhost_irq_info *irq_info = container_of(obj, struct devhost_irq_info, obj);
	free(irq_info);
}

static raw_thread_t *default_irq_server = NULL;

static raw_thread_t *affinity_irq_server = NULL;
static unsigned long config_cpu_nr = 0UL;

static cref_t kvic_server;

static DEFINE_IDR(linux_irqs);

static devhost_irq_handler_t sgi_handler[IRQ_SGI_NR] = {NULL};
static rref_t irq_sgi_rref[IRQ_PERCPU_NR * IRQ_MAX_CPU_NR] = {0ULL};

static struct irq_fastpath_res_s irq_fastpath_res = {
	.irq_actv_pool = (cref_t)0UL,
};

static struct actvpool_configs irq_ap_cfg = {
	.ap_name		= IRQ_ACTV_NAME,
	.path_name		= "",
	.actv_init_num		= 0, 		// use config cpu nr (even)
	.actv_max_num		= 0, 		// use config cpu nr (even)
	.tsd_size		= IRQ_ACTV_TSD_SIZE,
	.stack_size		= IRQ_ACTV_STACK_SIZE,
	.actvret_size		= 0,
	.recvbuf_size		= 0,
	.action_table		= sysif_devhost_actvcapcallhdlr_table,
	.action_table_size	= sizeof(void *) * __devhost_method_MAX,
	.p			= NULL
};

/* map linux irq and logic irq, no need to unmap
 * currently reserve DEVHOST_IRQMAP_SIZE slots to map
 * maybe use idr in future.
*/
static int irq_map[DEVHOST_IRQMAP_SIZE];
static int virq_map[DEVHOST_VIRQMAP_SIZE];

static inline bool is_kvic_linux_irq(unsigned int linux_irq)
{
	/* kvic linux irq range 1-15 */
	return (linux_irq != 0 && linux_irq < KVIC_LINUX_IRQ_RANGE);
}

static bool is_virt_linux_irq(unsigned int linux_irq);

static inline bool is_virt_irq(unsigned int logic_irq)
{
	return IRQ_TYPE(logic_irq) == IRQ_TYPE_VIRT;
}

static inline bool is_kvic_or_virt(const struct devhost_irq_info *irq_info)
{
	return is_kvic_linux_irq((unsigned int )irq_info->linux_irq) ||
	       is_virt_irq(irq_info->logic_irq);
}

static inline void irq_map_set(unsigned int logic_irq, int linux_irq)
{
	unsigned int irq_type = IRQ_TYPE(logic_irq);
	unsigned int logic_id = IRQ_ID(logic_irq);

	if (irq_type == IRQ_TYPE_VIRT) {
		BUG_ON(logic_id >= DEVHOST_VIRQMAP_SIZE);
		virq_map[logic_id] = linux_irq;
	} else {
		BUG_ON(logic_id >= DEVHOST_IRQMAP_SIZE);
		irq_map[logic_id] = linux_irq;
	}
}

static inline int irq_map_get(unsigned int logic_irq)
{
	int linux_irq;
	unsigned int irq_type = IRQ_TYPE(logic_irq);
	unsigned int logic_id = IRQ_ID(logic_irq);

	if (irq_type == IRQ_TYPE_VIRT) {
		BUG_ON(logic_id >= DEVHOST_VIRQMAP_SIZE);
		linux_irq = virq_map[logic_id];
	} else {
		BUG_ON(logic_id >= DEVHOST_IRQMAP_SIZE);
		linux_irq = irq_map[logic_id];
	}
	return linux_irq;
}

static void devhost_irq_handler_set_name(raw_thread_t thread, const char *thread_name, unsigned int idx)
{
	int err = E_HM_OK;
	char dh_irq_tname[DH_IRQ_TNAME_MAX];

	mem_zero_a(dh_irq_tname);

	err = snprintf_s(dh_irq_tname, DH_IRQ_TNAME_MAX, DH_IRQ_TNAME_MAX - 1UL,
			 "%s-%u", thread_name, idx);
	err = (err < 0) ? E_HM_INVAL : E_HM_OK;

	if (err == E_HM_OK) {
		err = hm_thread_setname(raw_thread_cref_of(thread), dh_irq_tname, NULL);
		if (err != E_HM_OK) {
			dh_info("irq handler set name failed: %s\n", hmstrerror(err));
		}
	}

	return;
}

static int devhost_irq_idr_set(struct devhost_irq_info *irq_info, unsigned int linux_irq)
{
	int err = 0;

	uobject_init(&irq_info->obj, 1, irq_info_uobject_release);
	uspinlock_lock(&g_idr_spinlock);
	err = idr_set_nolock(&linux_irqs, irq_info, linux_irq);
	uspinlock_unlock(&g_idr_spinlock);
	return err;
}

static struct devhost_irq_info *devhost_irq_idr_find_get(unsigned int linux_irq)
{
	struct devhost_irq_info *irq_info = NULL;
	int ret = 0;

	uspinlock_lock(&g_idr_spinlock);
	irq_info = idr_find_nolock(&linux_irqs, linux_irq);
	if (irq_info == NULL) {
		uspinlock_unlock(&g_idr_spinlock);
		return NULL;
	}
	ret = uobject_get(&irq_info->obj);
	if (ret <= 0) {
		uspinlock_unlock(&g_idr_spinlock);
		return NULL;
	}
	uspinlock_unlock(&g_idr_spinlock);
	return irq_info;
}

static void devhost_irq_idr_put(struct devhost_irq_info *irq_info)
{
	if (irq_info != NULL) {
		(void)uobject_put(&irq_info->obj);
	}
}

static struct devhost_irq_info *devhost_irq_idr_remove(unsigned int linux_irq)
{
	struct devhost_irq_info *irq_info = NULL;

	uspinlock_lock(&g_idr_spinlock);
	irq_info = (struct devhost_irq_info *)idr_remove_nolock(&linux_irqs, linux_irq);
	uspinlock_unlock(&g_idr_spinlock);

	return irq_info;
}

static void __noreturn __devhost_irq_common_handler(void)
{
	int err = E_HM_OK;
	int linux_irq;
	unsigned int logic_irq;
	struct devhost_irq_info *irq_info = NULL;
	int backend_inited = 0;

	while (__true) {
		err = hm_irq_wait_for_irq();
		if (err < 0) {
			dh_info("%s: common handler err=%s\n", DH_IRQ_TNAME, hmstrerror(err));
			continue;
		}
		if (unlikely(!backend_inited)) {
			backend_ctx_setup();
			backend_inited = 1;
		}
		logic_irq = (unsigned int)err;
		if (logic_irq < IRQ_PERCPU_NR * config_cpu_nr) {
			linux_irq = logic_irq % IRQ_PERCPU_NR;
			if (linux_irq < (int)IRQ_SGI_NR && sgi_handler[linux_irq] != NULL) {
				sgi_handler[linux_irq](linux_irq, NULL);
				continue;
			} else {
				dh_info("devhost, can not handler per_cpu irq:%d\n", linux_irq);
				continue;
			}
		}
		linux_irq = irq_map_get(logic_irq);
		irq_info = devhost_irq_idr_find_get((unsigned int)linux_irq);
		if (irq_info == NULL || irq_info->handler == NULL) {
			devhost_irq_idr_put(irq_info);
			dh_warn("%s: received linux_irq without info, "
				"irq=%d logic_irq=%u\n", DH_IRQ_TNAME, linux_irq, logic_irq);
			continue;
		}
		/* add random func hold mutex_lock, only slowpath could revoke */
		devhost_add_randomness(irq_info,
				       (unsigned int)sizeof(struct devhost_irq_info));

		irq_info->handler(irq_info->linux_irq, irq_info->dev_id);
		devhost_irq_idr_put(irq_info);
	}
}

static void *devhost_irq_common_handler(void *args)
{
	UNUSED(args);

	__devhost_irq_common_handler();

	/* thread dh-irq can not exit */
	__builtin_unreachable();
	return NULL;
}

static void __noreturn __devhost_kvic_input_handler(void)
{
	int err = E_HM_OK;
	int virq;
	struct devhost_irq_info *irq_info = NULL;
	struct __kvic_uart_read_data data_buff;
	int backend_inited = 0;

	err = hm_thread_setname(0, DH_KVIC_TNAME, NULL);
	if (err < 0) {
		dh_info("%s: set thread name failed, err=%s\n", DH_KVIC_TNAME, hmstrerror(err));
	}

	mem_zero_s(data_buff);

	while (__true) {
		err = hm_irq_wait_for_irq();
		if (err < 0) {
			dh_info("%s: input handler was abort, err=%d\n", DH_KVIC_TNAME, err);
			continue;
		}
		if (unlikely(!backend_inited)) {
			backend_ctx_setup();
			backend_inited = 1;
		}

		(void)sysfast_trigger_read((void *)&data_buff, sizeof(data_buff));
		virq = irq_map_get((unsigned int)err);
		irq_info = devhost_irq_idr_find_get((unsigned int)virq);
		if (irq_info == NULL) {
			dh_warn("%s: received linux_irq without info, irq=%d\n", DH_KVIC_TNAME, virq);
			continue;
		}
		if (irq_info->handler == NULL) {
			dh_warn("%s: handler of linux_irq is NULL, irq=%d, irq_name=%s\n",
				DH_KVIC_TNAME, virq, irq_info->name);
				devhost_irq_idr_put(irq_info);
			continue;
		}
		/* kvic know how to handle */
		devhost_kvic_getinput(&data_buff);
		devhost_add_randomness(irq_info, (unsigned int)sizeof(struct devhost_irq_info));

		/* dispatch this irq */
		irq_info->handler(virq, irq_info->dev_id);
		devhost_irq_idr_put(irq_info);
	}
}

static void *devhost_kvic_input_handler(void *args)
{
	UNUSED(args);

	__devhost_kvic_input_handler();

	/* thread dh-kvic can not exit */
	__builtin_unreachable();

	return NULL;
}

/*
 * Use idx to set irq-thread affinity
 */
static int devhost_irq_handler_set_affinity(raw_thread_t thread, unsigned int idx)
{
	unsigned int target_cpu = idx;
	struct drvcall_devmgr_irq_thread_arg arg;

	/*
	 * if there exist isolate cpus, and devhost may have no privilege to set affinity,
	 * here we borrow privilege from devmgr.
	 */
	mem_zero_s(arg);
	arg.tid = raw_thread_tid_of(thread);
	arg.cpu_mask = __cpuset_one(target_cpu);

	/*
	 * The CPU may be offline during interrupt registration. CONFIG_NR_CPUS is used
	 * to prepare resource.
	 * When the CPU is offline, irq-thread(dh-irq) setaffinity can be allowed to fail.
	 */
	(void)actvcapcall_drvcall_devmgr_irq_thread_setaffinity(libdh_get_devmgr_ap(), arg);

	return E_HM_OK;
}

DECLARE_UPCALL_ACTVHDLR(handle_irq_fastpath);
DEFINE_UPCALL_ACTVHDLR(handle_irq_fastpath)
{
	UNUSED(credential);
	unsigned int logic_irq = args.irq.logic_irq;
	int linux_irq;
	struct devhost_irq_info *irq_info = NULL;

	linux_irq = irq_map_get(logic_irq);
	irq_info = devhost_irq_idr_find_get((unsigned int)linux_irq);
	if (irq_info == NULL || irq_info->handler == NULL) {
		devhost_irq_idr_put(irq_info);
		dh_warn("%s: received linux_irq without info, "
			"irq=%d logic_irq=%u\n", DH_IRQ_TNAME, linux_irq, logic_irq);
	} else {
		irq_info->handler(irq_info->linux_irq, irq_info->dev_id);
	}
	devhost_irq_idr_put(irq_info);
	/* upcall finish should do eoi, irq number should used for retval, not support 0 */
	return (int)logic_irq;
}

static void devhost_ex_actvhandler_init_offset(unsigned int offset)
{
	union devhost_hdlrs_union *dh_table = &sysif_devhost_actvcapcallhdlr_table[__ACTV_UPCALL_IRQ_FASTPATH + offset];
	dh_table->p_sysif_handle_kcall_entry = handle_irq_fastpath;
}

static void devhost_ex_actvhandler_init(void)
{
	for (unsigned int i = 0; i < __devhost_method_TIMES; i++) {
		devhost_ex_actvhandler_init_offset(i * __devhost_method_MAX);
	}
}

/* create for irq_fastpath resource(e.g. irq-actv) */
static int irq_fastpath_init(void)
{
	return backend_ctx_setup();
}

/* irq-actvs are required resources for irq fathpath, use dh_panic when failed */
static int devhost_create_irq_fastpath(void)
{
	/* Only an even nr of actvs are supported */
	unsigned long irq_actv_nr	= (((config_cpu_nr + 1UL) >> 1) << 1);
	irq_ap_cfg.actv_init_num	= irq_actv_nr;
	irq_ap_cfg.actv_max_num		= irq_actv_nr;

	mem_zero_s(irq_fastpath_res);
	return hm_irq_create_fastpath(&irq_ap_cfg, &irq_fastpath_res,
				      irq_fastpath_init);
}

static int devhost_serverpool_create(raw_thread_t *thread,
				     const raw_thread_attr_t *attr)
{
	int err = E_HM_OK;

	for (unsigned int i = 0; i < config_cpu_nr; i++) {
		/*
		 * setup nomal irq server
		 * irq handler threads are required resources and
		 * can not exit, so use dh_panic when failed
		 */
		err = raw_thread_create(thread, attr, devhost_irq_common_handler, NULL);
		if (err != E_HM_OK) {
			dh_panic("%s: failed to create irq common "
				 "handler, cpu_nr: %lu\n", DH_IRQ_TNAME, config_cpu_nr);
			return err;
		}

		default_irq_server[i] = *thread;

		/* irq handler setting name is not a necessary resource */
		devhost_irq_handler_set_name(default_irq_server[i], DH_IRQ_TNAME, i);

		/*
		 * The interrupt handling depends on the thread affinity.
		 * If the thread affinity fails to be set,
		 * the interrupts may be triggered without being processed by the thread.
		 * As a result, the subsequent interrupt processing will be blocked.
		 * So, here use dh_panic when failed
		 */
		err = devhost_irq_handler_set_affinity(default_irq_server[i], i);
		if (err != E_HM_OK) {
			dh_panic("set irq handler affinity failed: %s, idx: %u, cpu_nr: %lu\n",
				 hmstrerror(err), i, config_cpu_nr);
		}
		err = hm_thread_set_lt_group(raw_thread_tid_of(thread), SCHED_LT_POWER_GROUP);
		if (err != E_HM_OK) {
			dh_error("hm_thread_set_lt_group failed, tid=%d, ret=%s\n",
				 raw_thread_tid_of(thread), hmstrerror(err));
		}
	}

	/*
	 * setup nomal kvic server
	 * kvic handler thread is required resource and
	 * can not exit, so use dh_panic when failed
	 */
	err = raw_thread_create(thread, attr, devhost_kvic_input_handler, NULL);
	if (err != E_HM_OK) {
		dh_panic("%s: failed to create kvic handler: %s\n",
			 DH_KVIC_TNAME, hmstrerror(err));
		return err;
	}

	kvic_server = raw_thread_cref_of(*thread);

	return err;
}

static int devhost_irqthread_attr_init(raw_thread_attr_t *attr)
{
	int err = E_HM_OK;
	struct raw_sched_param param;

	err = raw_thread_attr_init(attr);
	if (err != E_HM_OK) {
		dh_error("raw_thread_attr_init failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = raw_thread_attr_setinheritsched(attr, RAW_THREAD_EXPLICIT_SCHED);
	if (err != E_HM_OK) {
		dh_error("raw_thread_attr_setinheritsched failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = raw_thread_attr_setschedpolicy(attr, SCHED_RR);
	if (err != E_HM_OK) {
		dh_error("raw_thread_attr_setschedpolicy failed, err=%s\n", hmstrerror(err));
		return err;
	}
	mem_zero_s(param);
#ifdef CONFIG_MODULE_SCHED_CLASS
	param.priority = HM_PRIORITY_DH_KVIC;
	err = raw_thread_attr_setschedparam(attr, &param);
#else
	param.priority = DEVHOST_IRQ_RUNQUEUE_KVIC;
	err = raw_thread_attr_setschedparam(attr, &param);
#endif
	if (err != E_HM_OK) {
		dh_error("raw_thread_attr_setschedparam failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return err;
}

void devhost_irq_ap_table_init(void)
{
	devhost_ex_actvhandler_init();
}

static int devhost_irq_setup_serverpool(void)
{
	int err = E_HM_OK;
	raw_thread_t thread;
	raw_thread_attr_t attr;

	err = devhost_irqthread_attr_init(&attr);
	if (err != E_HM_OK) {
		return err;
	}

	err = devhost_serverpool_create(&thread, &attr);
	if (err != E_HM_OK) {
		dh_panic("devhost serverpool create failed err=%s\n", hmstrerror(err));
		return err;
	}

	return 0;
}

static int devhost_irq_get_cpu_info(void)
{
	int err = E_HM_OK;

	err = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &config_cpu_nr,
				   sizeof(config_cpu_nr));
	if (err < 0) {
		dh_debug("get cpu info failed: %s\n", hmstrerror(err));
	} else {
		err = E_HM_OK;
	}

	return err;
}

static int devhost_irq_server_init(void)
{
	int err = E_HM_OK;

	default_irq_server = (raw_thread_t *)malloc(config_cpu_nr * sizeof(raw_thread_t));
	if (default_irq_server == NULL) {
		dh_error("alloc memory for default irq server failed, "
			 "config_cpu_nr:%lu, size:%lu\n",
			 config_cpu_nr, config_cpu_nr * sizeof(raw_thread_t));
		return E_HM_NOMEM;
	}
	mem_zero_b(default_irq_server, config_cpu_nr * sizeof(raw_thread_t));

	affinity_irq_server = (raw_thread_t *)malloc(config_cpu_nr * sizeof(raw_thread_t));
	if (affinity_irq_server == NULL) {
		dh_error("alloc memory for affinity irq server failed, "
			 "config_cpu_nr:%lu, size:%lu\n",
			 config_cpu_nr, config_cpu_nr * sizeof(raw_thread_t));
			return E_HM_NOMEM;
	}
	mem_zero_b(affinity_irq_server, config_cpu_nr * sizeof(raw_thread_t));

	return err;
}

int devhost_irq_setup(void)
{
	int err = E_HM_OK;

	/* get max number of supported CPUs */
	if (config_cpu_nr == 0UL) {
		err = devhost_irq_get_cpu_info();
	}

	if (err == E_HM_OK) {
		/*
		 * devhost create some irq handlers for irq register.
		 * These resources do not need to be reclaimed if the
		 * system running properly.
		 */
		err = devhost_irq_server_init();
	}

	if (err == E_HM_OK) {
		err = devhost_irq_setup_serverpool();
	}

	if (err == E_HM_OK) {
		/* init devhost irq entropy health data */
		entropy_init_health_data(HEALTH_RC_MAX_COUNT, HEALTH_DEVHOST_AP_MAX_COUNT);
	} else {
		dh_error("devhost setup irq failed: %s\n", hmstrerror(err));
		if (default_irq_server != NULL) {
			free(default_irq_server);
			default_irq_server = NULL;
		}
		if (affinity_irq_server != NULL) {
			free(affinity_irq_server);
			affinity_irq_server = NULL;
		}
	}

	return err;
}

static struct devhost_irq_info *
devhost_alloc_irq_info(unsigned int linux_irq, rref_t rref, unsigned int logic_irq)
{
	struct devhost_irq_info *info = malloc(sizeof(struct devhost_irq_info));

	if (info == NULL) {
		dh_error("alloc memory for linux_irq failed, size=%zu, irq=%u\n",
			 sizeof(struct devhost_irq_info), linux_irq);
	} else {
		info->logic_irq = logic_irq;
		info->linux_irq = (int)linux_irq;
		info->thread_affinity = devhost_get_default_affinity();

		if (is_kvic_or_virt(info)) {
			info->data.registered = false;
		} else {
			info->data.irq_rref = rref;
		}
	}

	return info;
}

static int devhost_kvic_irq_fetch(unsigned int linux_irq,
				  struct __actvret_drvcall_devmgr_irq_fetch *ret)
{
	struct drvcall_devmgr_irq_fetch_arg arg;
	int err = E_HM_OK;

	mem_zero_s(arg);
	arg.virt_irq = linux_irq;
	arg.is_sgi = false;
	arg.cpu = 0U;

	/* fetch info of linux_irq from devmgr to get logic_irq/source_id */
	err = actvcapcall_drvcall_devmgr_irq_fetch(libdh_get_devmgr_ap(), arg, ret);
	if (err < 0) {
		dh_error("actv capcall linux irq: %u, fetch failed: %s\n",
			 linux_irq, hmstrerror(err));
	}

	return err;
}

static void devhost_kvic_irq_return(unsigned int linux_irq)
{
	int err = E_HM_OK;
	struct drvcall_devmgr_irq_return_arg arg;

	mem_zero_s(arg);
	arg.virt_irq = linux_irq;
	arg.is_sgi = false;
	arg.cpu = 0;

	err = actvcapcall_drvcall_devmgr_irq_return(libdh_get_devmgr_ap(), arg);
	if (err < 0) {
		dh_error("return linux irq failed, linux_irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
	}
}

static int devhost_new_kvic_irq_info(unsigned int linux_irq, bool is_virq)
{
	struct __actvret_drvcall_devmgr_irq_fetch ret;
	struct devhost_irq_info *info = NULL;
	int err = E_HM_OK;
	unsigned int logic_irq;

	mem_zero_s(ret);
	/* fetch info of linux_irq from devmgr to get logic_irq/source_id */
	err = devhost_kvic_irq_fetch(linux_irq, &ret);
	if (err < 0) {
		return err;
	}

	if (is_virq) {
		logic_irq = IRQ_VIRT(ret.rlt.logic_irqs[0]);
	} else {
		logic_irq = ret.rlt.logic_irqs[0];
	}

	/* allocate new info to record this, ref = 0 because not used */
	info = devhost_alloc_irq_info(linux_irq, 0, logic_irq);
	if (info == NULL) {
		devhost_kvic_irq_return(linux_irq);
		err = E_HM_NOMEM;
		return err;
	}
	err = devhost_irq_idr_set(info, linux_irq);
	if (err < 0) {
		devhost_irq_idr_put(info);
		devhost_kvic_irq_return(linux_irq);
		if (err == E_HM_OBJEXIST) {
			err = E_HM_POSIX_EXIST;
		}
		return err;
	}

	irq_map_set(info->logic_irq, info->linux_irq);

	return 0;
}

static int devhost_normal_irq_fetch(unsigned int linux_irq,
				    struct __actvret_drvcall_devmgr_irq_fetch *ret,
				    rref_t *rref_ptr)
{
	struct drvcall_devmgr_irq_fetch_arg arg;
	rref_t rref;
	int err = E_HM_OK;

	rref = hm_ucap_require(0, libdh_get_devmgr_cnode_idx(), 0);
	if (IS_REF_ERR(rref)) {
		dh_error("create rref failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(rref)));
		return REF_TO_ERR(rref);
	}

	mem_zero_s(arg);
	/* fetch info of linux_irq from devmgr */
	arg.rrefs_nr = 1;
	arg.virt_irq = linux_irq;
	arg.is_sgi = false;
	arg.cpu = 0U;
	arg.irqctrl_rrefs[0] = rref;

	/* fetch info of linux_irq from devmgr to get logic_irq/source_id */
	err = actvcapcall_drvcall_devmgr_irq_fetch(libdh_get_devmgr_ap(), arg, ret);
	if (err < 0) {
		/* ucap require success, ignore return value */
		(void)hm_ucap_reject(0, rref);
		return err;
	}
	*rref_ptr = rref;

	return err;
}

static void devhost_irq_set_affinity(struct devhost_irq_info *info, unsigned int affinity)
{
	if (affinity != UINT_MAX) {
		info->thread_affinity = __cpuset_one(affinity);
	}
}

static void devhost_normal_irq_return(unsigned int linux_irq, rref_t irq_rref);

static int devhost_new_normal_irq_info(unsigned int linux_irq)
{
	struct __actvret_drvcall_devmgr_irq_fetch ret;
	struct devhost_irq_info *info = NULL;
	rref_t rref = 0;
	int err = E_HM_OK;

	/* only one tcb could successfully fetch rref, don't need lock */
	mem_zero_s(ret);
	err = devhost_normal_irq_fetch(linux_irq, &ret, &rref);
	if (err < 0) {
		return err;
	}

		/* since this is first time, setup irq */
	err = syscap_IrqCtrlConfig(rref, ret.rlt.logic_irqs[0],
				ret.rlt.hwconf, ret.rlt.hwconf_len);
	if (err < 0) {
		devhost_normal_irq_return(linux_irq, rref);
		dh_error("setup irq failed, err=%s\n", hmstrerror(err));
		return err;
	}

	/* allocate new info to record this */
	info = devhost_alloc_irq_info(linux_irq, rref,
				      ret.rlt.logic_irqs[0]);
	if (info == NULL) {
		devhost_normal_irq_return(linux_irq, rref);
		err = E_HM_NOMEM;
		return err;
	}

	devhost_irq_set_affinity(info, ret.rlt.affinity);
	err = devhost_irq_idr_set(info, linux_irq);
	if (err < 0) {
		devhost_irq_idr_put(info);
		devhost_normal_irq_return(linux_irq, rref);
		if (err == E_HM_OBJEXIST) {
			err = E_HM_POSIX_EXIST;
		}
		return err;
	}

	irq_map_set(info->logic_irq, info->linux_irq);

	return 0;
}

/* only support 1 core now, create new info and setup it */
static int devhost_new_irq_info(unsigned int linux_irq)
{
	int err = E_HM_OK;

	if ( is_kvic_linux_irq(linux_irq)) {
		err = devhost_new_kvic_irq_info(linux_irq, false);
	} else if (is_virt_linux_irq(linux_irq)) {
		err = devhost_new_kvic_irq_info(linux_irq, true);
	} else {
		err = devhost_new_normal_irq_info(linux_irq);
	}

	if (err < 0) {
		dh_debug("devhost new irq info linux_irq: %u, failed: %s\n",
			 linux_irq, hmstrerror(err));
	}

	return err;
}

static int devhost_hack_compatible_irq(unsigned int linux_irq)
{
	/* for v3mrru, it using hw_irq without map it.
	 * we need to create a fake mapping for it here */
	int err = E_HM_OK;
	struct drvcall_devmgr_irq_map_arg arg = {0};
	struct __actvret_drvcall_devmgr_irq_map ret;
	int irqctrl_id = hm_irq_default_ctrl_id(HM_IRQ_CTRL_TYPE_PHYSICAL);
	if (irqctrl_id < 0) {
		dh_error("get physical irqctrl_id failed, err=%s\n",
			 hmstrerror(irqctrl_id));
		return irqctrl_id;
	}

	arg.hwconf[HW_CONF_IRQ_ID_IDX] = linux_irq;
	arg.hwconf[HW_CONF_IRQ_CONF_IDX] = HW_CONF_DEFAULT_PRIO;
	arg.hwconf[HW_CONF_IRQ_TARGET_IDX] = HW_CONF_TARGET_CPU;
	arg.hwconf_len = HW_CONF_LENGTH;
	arg.oirq_offset = (unsigned int)irqctrl_id;

	mem_zero_s(ret);
	err = actvcapcall_drvcall_devmgr_irq_map(libdh_get_devmgr_ap(), arg, &ret);
	if (err < 0) {
		dh_error("map compatible linux_irq failed, irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
		return err;
	}

	if (ret.virt_irq != linux_irq) {
		dh_error("can't map linux_irq to virt_irq, irq=%u, virt_irq=%u\n",
			 linux_irq, ret.virt_irq);
		return E_HM_INVAL;
	}

	dh_warn("using default compatible setting for linux_irq, irq=%u\n",
		linux_irq);
	/* retry */
	err = devhost_new_irq_info(linux_irq);

	return err;
}

static int devhost_irq_register_with_mask(struct devhost_irq_info *info,
					  raw_thread_t *server,
					  struct hm_irq_register_setting *setting,
					  __cpuset_t cpu_mask)
{
	UNUSED(cpu_mask);
	int err = E_HM_OK;

	for (unsigned int i = 0U; i < config_cpu_nr; i++) {
		setting->cpu_mask = __cpuset_one(i);

		err = hm_irq_register(info->data.irq_rref, info->logic_irq,
				      setting, raw_thread_cref_of(server[i]));
		if (err != E_HM_OK) {
			dh_warn("irq register idx: %lu, setting: 0x%x, failed: %s\n",
				i, setting->cpu_mask.__bits, hmstrerror(err));
			break;
		}
	}
	if (err != E_HM_OK) {
		(void)hm_irq_unregister(info->data.irq_rref, info->logic_irq);
	}

	return err;
}

static int devhost_irq_register_default(struct devhost_irq_info *info,
					raw_thread_t *server,
					struct hm_irq_register_setting *setting)
{
	return devhost_irq_register_with_mask(info, server, setting, __cpuset_full);
}

static cref_t devhost_irq_fetch_affinity_cref(const struct devhost_irq_info *info,
					      raw_thread_t *server, unsigned long server_len)
{
	int ret;
	raw_thread_t thread;
	raw_thread_attr_t attr;
	cref_t cref_out = 0ULL;
	unsigned int idx = (unsigned int)__cpuset_first_cpu(info->thread_affinity);
	if (idx >= server_len) {
		dh_error("irq%d config server%u affinity conflict.\n", info->linux_irq, idx);
		return cref_out;
	}

	if (server[idx] == NULL) {
		ret = devhost_irqthread_attr_init(&attr);
		if (ret != E_HM_OK) {
			dh_error("failed to init irq#%d thread attr\n", info->linux_irq);
			return cref_out;
		}

		/*
		 * irq handler threads are necessary resources and
		 * can not exit, so use dh_panic when failed
		 */
		ret = raw_thread_create(&thread, &attr, devhost_irq_common_handler, NULL);
		if (ret != E_HM_OK) {
			dh_panic("%s: failed to create irq#%d "
				 "server%u affinity handler\n", DH_IRQ_AFF_TNAME, info->linux_irq, idx);
			return cref_out;
		}
		cpu_set_t affinity = { };
		affinity.__bits[0] = (unsigned long)info->thread_affinity.__bits;
		ret = hm_thread_sched_setaffinity(raw_thread_tid_of(thread),
						  sizeof(affinity), &affinity);
		if (ret < 0) {
			dh_panic("%s: failed to sched irq#%d server%u affinity\n",
				 DH_IRQ_AFF_TNAME, info->linux_irq, idx);
		}
		server[idx] = thread;

		/* irq handler setting name is not a required resource */
		devhost_irq_handler_set_name(server[idx], DH_IRQ_AFF_TNAME, idx);

		dh_info("create irq thread %s-%u(affinity 0x%x)\n", DH_IRQ_AFF_TNAME, idx,
			info->thread_affinity.__bits);
	}

	dh_debug("assign irq#%d to thread %s-%u(affinity 0x%x)\n", info->linux_irq,
		 DH_IRQ_AFF_TNAME, idx, info->thread_affinity.__bits);
	cref_out = raw_thread_cref_of(server[idx]);
	return cref_out;
}

static rref_t default_virqctrl(void)
{
	static rref_t rref = 0ULL;

	if (rref == 0ULL) {
		rref = hm_module_open("virq");
	}
	return rref;
}

static bool is_virt_linux_irq(unsigned int linux_irq)
{
	int err = E_HM_OK;
	rref_t rref;
	struct __virq_translate_s data;

	rref = default_virqctrl();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}
	if (err == E_HM_OK) {
		data.hw_irq = linux_irq;
		err = hm_module_ioctl(rref, __VIRQ_CMD_TRANSLATE, &data, sizeof(data), NULL, 0);
	}
	return err >= 0;
}

static cref_t devhost_virq_server_of(const struct devhost_irq_info *info)
{
	cref_t cref;
	__cpuset_t default_affinity;

	default_affinity = devhost_get_default_affinity();
	if (__cpuset_equal(info->thread_affinity, default_affinity)) {
		cref = raw_thread_cref_of(default_irq_server[0U]);
	} else {
		cref = devhost_irq_fetch_affinity_cref(info, affinity_irq_server, config_cpu_nr);
	}
	return cref;
}

static int devhost_virt_irq_enable(const struct devhost_irq_info *info)
{
	int err = E_HM_OK;
	rref_t rref;
	struct __virq_bind_s data;

	rref = default_virqctrl();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}
	if (err == E_HM_OK) {
		data.cref = devhost_virq_server_of(info);
		data.retval = (int)info->logic_irq;
		data.virt_irq = IRQ_ID(info->logic_irq);
		err = hm_module_ioctl(default_virqctrl(), __VIRQ_CMD_BIND, &data, sizeof(data), NULL, 0);
	}
	if (err < 0) {
		dh_warn("virt irq %d bind failed: %s\n", info->linux_irq, hmstrerror(err));
	}
	return err;
}

static int devhost_virt_irq_disable(const struct devhost_irq_info *info)
{
	int err = E_HM_OK;
	rref_t rref;
	struct __virq_unbind_s data;

	rref = default_virqctrl();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}
	if (err == E_HM_OK) {
		data.virt_irq = IRQ_ID(info->logic_irq);
		err = hm_module_ioctl(default_virqctrl(), __VIRQ_CMD_UNBIND, &data, sizeof(data), NULL, 0);
	}
	if (err < 0) {
		dh_warn("virt irq %d bind failed: %s\n", info->linux_irq, hmstrerror(err));
	}
	return err;
}

static int devhost_virt_irq_unregister(struct devhost_irq_info *irq_info)
{
	(void)devhost_virt_irq_disable(irq_info);
	irq_info->data.registered = false;
	return E_HM_OK;
}

static int devhost_irq_register(struct devhost_irq_info *info, int is_kvic,
				struct hm_irq_register_setting *setting)
{
	int err = E_HM_OK;
	cref_t irq_handler_cref = 0;
	__cpuset_t default_affinity = devhost_get_default_affinity();

	UNUSED(is_kvic);

	if (is_kvic_or_virt(info)) {
		info->data.registered = true;
	} else {
		if (__cpuset_equal(info->thread_affinity, default_affinity) != 0) {
			err = devhost_irq_register_default(info, default_irq_server, setting);
		} else {
			irq_handler_cref = devhost_irq_fetch_affinity_cref(info, affinity_irq_server, config_cpu_nr);
			/* Interrupts from any CPU are processed by affinity threads */
			setting->cpu_mask = __cpuset_full;
			err = hm_irq_register(info->data.irq_rref, info->logic_irq,
					      setting, irq_handler_cref);
		}
	}

	if (err < 0) {
		dh_error("irq register failed, linux_irq=%u, logic_irq=%u, err=%s\n",
			 (unsigned int)info->linux_irq, info->logic_irq,
			 hmstrerror(err));
	}

	return err;
}

static int devhost_setup_request_irq(struct devhost_irq_info *info,
				     devhost_irq_handler_t handler,
				     int is_kvic,
				     const char *devname,
				     void *dev_id)
{
	int err = E_HM_OK;
	struct hm_irq_register_setting setting;
	unsigned int linux_irq = (unsigned int)info->linux_irq;

	mem_zero_s(setting);
	setting.name = devname;
	/*
	 * 1. some driver must do something before enable irq.
	 * 2. mark irq as staywake when suspend in order to do not
	 *    disable when call irqctrl_suspend, beacuse we disable
	 *    the irq in liblinux directly.
	 */
	setting.flags = IRQCTRL_REGISTER_STAYWAKE;
	/* allow invalid cref_t */
	setting.irq_fastpath_ap = irq_fastpath_res.irq_actv_pool;
	setting.is_fp_bypass = 0U;

	err = devhost_irq_register(info, is_kvic, &setting);
	if (err < 0) {
		dh_error("devhost irq register failed:"
			 "linux_irq=%u, logic_irq=%u, err=%s\n",
			 linux_irq, info->logic_irq, hmstrerror(err));
	}

	/* save handler to irqinfo */
	info->dev_id = dev_id;
	info->handler = handler;
	NOFAIL(strcpy_s(info->name, sizeof(info->name), devname));

	dh_debug("irq request successfully, linux_irq=%u, logic_irq=%u\n",
		 linux_irq, info->logic_irq);

	return err;
}

static void devhost_irq_return(const struct devhost_irq_info *info,
			       unsigned int linux_irq)
{
	if (is_kvic_or_virt(info)) {
		devhost_kvic_irq_return(linux_irq);
	} else {
		devhost_normal_irq_return(linux_irq, info->data.irq_rref);
	}
}

int devhost_request_threaded_irq(unsigned int linux_irq,
				 devhost_irq_handler_t handler,
				 int is_kvic,
				 const char *devname,
				 void *dev_id)
{
	int err = E_HM_OK;
	struct devhost_irq_info *info = NULL;

	/* irq-actv init call backend_ctx_setup should after init_backend_dllib */
	if (irq_fastpath_res.irq_actv_pool == 0UL) {
		err = devhost_create_irq_fastpath();
		if (err != E_HM_OK) {
			dh_info("create irq actvs failed: %s\n", hmstrerror(err));
			return err;
		}
	}

	if (devname == NULL) {
		dh_error("invalid NULL devname\n");
		return E_HM_INVAL;
	}

	/* sanity check */
	if (linux_irq == 0) {
		dh_error("invalid linux_irq, irq=%u\n", linux_irq);
		return E_HM_INVAL;
	}
	if (handler == NULL) {
		dh_error("handler of linux_irq is NULL, irq=%u\n", linux_irq);
		return E_HM_INVAL;
	}
	info = devhost_irq_idr_find_get((unsigned int)linux_irq);
	if (info != NULL) {
		devhost_irq_idr_put(info);
		/* linux support request multiple times if irqflags is SHARED */
		dh_error("doesn't support multiple request, irq=%u\n", linux_irq);
		return E_HM_OBJEXIST;
	}
	/* save linux_irq info and setup */
	err = devhost_new_irq_info(linux_irq);
	if (err == E_HM_NOOBJ) { /* no mapping, create one */
		err = devhost_hack_compatible_irq(linux_irq);
	}

	if (err < 0) {
		dh_error("new irq info failed, linux_irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
		return err;
	}

	/* linux_irq, rref and logic_irq have been filled */
	info = devhost_irq_idr_find_get((unsigned int)linux_irq);
	BUG_ON(info == NULL);
	err = devhost_setup_request_irq(info, handler, is_kvic,
					devname, dev_id);
	if (err < 0) {
		/*
		 * Irq registration is not complete, the interrupt is not triggered
		 * and does not need to add lock
		 */
		devhost_irq_return(info, linux_irq);
		devhost_irq_idr_put(info);
		(void)devhost_irq_idr_remove(linux_irq);
		devhost_irq_idr_put(info);
		return err;
	}
	devhost_irq_idr_put(info);
	return err;
}

static void devhost_normal_irq_return(unsigned int linux_irq, rref_t irq_rref)
{
	int err = E_HM_OK;
	struct drvcall_devmgr_irq_return_arg arg;

	mem_zero_s(arg);
	arg.virt_irq = linux_irq;
	arg.rrefs_nr = 1;
	arg.irqctrl_rrefs[0] = irq_rref;
	arg.is_sgi = false;
	arg.cpu = 0;

	/* don't need check return code, if return failed, this rref
	 * still can be used and we will directly free it */
	(void)hm_ucap_export_move(0, irq_rref, libdh_get_devmgr_cnode_idx());
	err = actvcapcall_drvcall_devmgr_irq_return(libdh_get_devmgr_ap(), arg);
	if (err < 0) {
		dh_error("return linux_irq failed, free it directly, irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
	}

	/* rref is useless now, ignore ret value */
	(void)hm_ucap_reject(0, irq_rref);
}

static int devhost_kvic_irq_unregister(struct devhost_irq_info *irq_info)
{
	int err = E_HM_OK;
	unsigned int source_id;

	/* kvic irq (1-15) use logic_id as kvic source id */
	source_id = (unsigned int)(irq_info->logic_irq);

	/* only register not enable, kvic source unbind may failed */
	err = hm_thread_kvic_source_unbind(kvic_server, source_id);
	if (err < 0) {
		dh_debug("kvic source unbind failed, cref: %" PRIxref
			 "source_id=%u, err=%s\n",
			 kvic_server, source_id, hmstrerror(err));
		/* ignore return value */
		err = E_HM_OK;
	}

	irq_info->data.registered = false;

	return err;
}

/*
 * The user does not invoke free_irq when irq_info is being used,
 * no need to add lock for performance consideration
 */
void devhost_free_irq(unsigned int linux_irq)
{
	int err = E_HM_OK;
	struct devhost_irq_info *irq_info = devhost_irq_idr_remove(linux_irq);

	if (irq_info == NULL) {
		return ;
	}

	if (is_kvic_linux_irq(linux_irq)) {
		err = devhost_kvic_irq_unregister(irq_info);
	} else if (is_virt_irq(irq_info->logic_irq)) {
		err = devhost_virt_irq_unregister(irq_info);
	} else {
		/* only one could unregister and revoke ctrl successfully,
		 * so we don't need lock here */
		err = hm_irq_unregister(irq_info->data.irq_rref, irq_info->logic_irq);
	}

	if (err < 0) {
		dh_error("devhost free irq failed, linux_irq=%u, dev=%s, err=%s\n",
			 linux_irq, irq_info->name, hmstrerror(err));
		devhost_irq_idr_put(irq_info);
		return;
	}

	/* clean and free resources */
	devhost_irq_return(irq_info, linux_irq);
	devhost_irq_idr_put(irq_info);
	dh_debug("free linux_irq successfully, linux_irq=%u\n", linux_irq);
}

static int devhost_kvic_irq_enable(const struct devhost_irq_info *irq_info)
{
	int err = E_HM_OK;
	unsigned int source_id;

	/* kvic irq (1-15) use logic_id as kvic source id */
	source_id = (unsigned int)(irq_info->logic_irq);

	if (irq_info->data.registered == true) {
		/* arg flag = 0 means can not drop pending fire sig */
		err = hm_thread_kvic_source_bind(kvic_server, source_id, 0);
	} else {
		dh_warn("kvic source_id:%u, enable failed because unregister\n",
			source_id);
	}

	if (err < 0) {
		dh_warn("kvic source bind failed, cref: %" PRIxref
			"source_id=%u, err=%s\n",
			kvic_server, source_id, hmstrerror(err));
	}

	return err;
}

void devhost_enable_irq(unsigned int linux_irq)
{
	int err = E_HM_OK;
	struct devhost_irq_info *irq_info = devhost_irq_idr_find_get(linux_irq);
	if (irq_info == NULL) {
		dh_error("no irqinfo, linux_irq=%u\n", linux_irq);
		return;
	}

	if (is_kvic_linux_irq(linux_irq)) {
		err = devhost_kvic_irq_enable(irq_info);
	} else if (is_virt_irq(irq_info->logic_irq)) {
		err = devhost_virt_irq_enable(irq_info);
	} else {
		err = hm_irq_enable(irq_info->data.irq_rref, irq_info->logic_irq);
	}

	if (err < 0) {
		dh_warn("enable irq failed, linux_irq=%u, err=%s\n",
			linux_irq, hmstrerror(err));
	}
	devhost_irq_idr_put(irq_info);

	return;
}

static int devhost_kvic_irq_disable(const struct devhost_irq_info *irq_info)
{
	int err = E_HM_OK;
	unsigned int source_id;

	/* kvic irq (1-15) use logic_id as kvic source id */
	source_id = (unsigned int)(irq_info->logic_irq);

	if (irq_info->data.registered == true) {
		err = hm_thread_kvic_source_unbind(kvic_server, source_id);
	} else {
		dh_warn("enable kvic source failed because unregister\n");
	}

	if (err < 0) {
		dh_warn("kvic source unbind failed, cref: %" PRIxref
			"source_id=%u, err=%s\n",
			kvic_server, source_id, hmstrerror(err));
	}

	return err;
}

void devhost_disable_irq(unsigned int linux_irq)
{
	int err = E_HM_OK;
	struct devhost_irq_info *irq_info = devhost_irq_idr_find_get(linux_irq);
	if (irq_info == NULL) {
		dh_error("no irqinfo, linux_irq=%u\n", linux_irq);
		return ;
	}

	if (is_kvic_linux_irq(linux_irq)) {
		err = devhost_kvic_irq_disable(irq_info);
	}  else if (is_virt_irq(irq_info->logic_irq)) {
		err = devhost_virt_irq_disable(irq_info);
	} else {
		err = hm_irq_disable(irq_info->data.irq_rref, irq_info->logic_irq);
	}

	if (err < 0) {
		dh_error("disable irq failed, linux_irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
	}
	devhost_irq_idr_put(irq_info);

	return;
}

#define IRQ_CAN_WAKEUP		1U

int devhost_set_wake_irq(unsigned int linux_irq, unsigned int can_wakeup)
{
	int err = E_HM_OK;
	struct devhost_irq_info *irq_info = devhost_irq_idr_find_get(linux_irq);
	unsigned int flag = 0U;

	if (irq_info == NULL) {
		dh_error("no irqinfo, linux_irq=%u\n", linux_irq);
		return -hmerrno2posix(E_HM_INVAL);
	}

	if (is_kvic_or_virt(irq_info)) {
		dh_debug("set wake irq failed, logic: %u\n", irq_info->logic_irq);
		devhost_irq_idr_put(irq_info);
		return -hmerrno2posix(E_HM_INVAL);
	}

	if (can_wakeup == IRQ_CAN_WAKEUP) {
		flag = IRQCTRL_SET_WAKE;
	}

	err = hm_irq_set_wake(irq_info->data.irq_rref, irq_info->logic_irq, flag);
	if (err < 0) {
		devhost_irq_idr_put(irq_info);
		dh_error("irq set wake failed, linux_irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
		return -hmerrno2posix(err);
	}
	devhost_irq_idr_put(irq_info);
	return 0;
}

void devhost_disable_local_irq(void)
{
	struct sched_param param_user;
#ifdef CONFIG_MODULE_SCHED_CLASS
	param_user.sched_priority = HM_PRIORITY_DH_IRQ_OFF;
	(void)hm_thread_sched_setparam(0, &param_user);
#else
	param_user.sched_priority = DEVHOST_IRQ_RUNQUEUE_OFF;
	(void)hm_thread_sched_setparam(0, &param_user);
#endif
}

void devhost_enable_local_irq(void)
{
	struct sched_param param_user;
#ifdef CONFIG_MODULE_SCHED_CLASS
	param_user.sched_priority = HM_PRIORITY_USER_DEFAULT;
	(void)hm_thread_sched_setparam(0, &param_user);
#else
	param_user.sched_priority = HM_RUNQUEUE_USER_DEFAULT;
	(void)hm_thread_sched_setparam(0, &param_user);
#endif
}

/* Migrate IRQs away from specified CPU to another online CPU */
void devhost_migrate_all_irq_of_cpu(unsigned int cpu)
{
	return;
}

void devhost_restore_local_irq(unsigned long flags)
{
	int priority;
	struct sched_param param_user;

	if (flags == 0UL) {
		dh_info("zero flags will be ignored in restore local irq\n");
		return;
	}

	/*
	 * In Hongmeng's current implementation, flags is equal to priority.
	 */
	priority = (int)flags;
	param_user.sched_priority = priority;
	(void)hm_thread_sched_setparam(0, &param_user);
}

unsigned long devhost_save_local_irq(void)
{
	int ret = E_HM_OK;
	unsigned int priority;
	struct sched_param param;

	mem_zero_s(param);
	ret = hm_thread_sched_getparam(0, &param);
	if (ret < 0) {
		dh_info("acquire priority failed\n");
		return 0;
	}

	/* ret must be a legal priority here, so forced it to unsigned int */
	priority = (unsigned int)param.sched_priority;
#ifdef CONFIG_MODULE_SCHED_CLASS
	if (priority < HM_PRIORITY_DH_IRQ_OFF) {
		/* mask irq */
		param.sched_priority = HM_PRIORITY_DH_IRQ_OFF;
		(void)hm_thread_sched_setparam(0, &param);
	}
#else
	if (priority < DEVHOST_IRQ_RUNQUEUE_OFF) {
		/* mask irq */
		param.sched_priority = DEVHOST_IRQ_RUNQUEUE_OFF;
		(void)hm_thread_sched_setparam(0, &param);
	}
#endif
	/*
	 * In design, priority is represented by some bits in the flags of
	 * type unsigned long, but in the current implementation priority
	 * is equal to flags, so the priority type is cast to unsigned long here.
	 */
	return (unsigned long)priority;
}

unsigned long devhost_save_local_flags(void)
{
	int ret = E_HM_OK;
	unsigned int priority;
	struct sched_param param;

	mem_zero_s(param);
	ret = hm_thread_sched_getparam(0, &param);
	if (ret < 0) {
		dh_info("acquire priority failed\n");
		return 0;
	}
	/* ret must be a legal priority here, so forced it to unsigned int */
	priority = (unsigned int)param.sched_priority;

	/*
	 * In design, priority is represented by some bits in the flags of
	 * type unsigned long, but in the current implementation priority
	 * is equal to flags, so the priority type is cast to unsigned long here.
	 */
	return (unsigned long)priority;
}

/* returns linux_irq on success, 0: means mapping failure */
unsigned int devhost_create_irq_mapping(unsigned int oirq_offset,
					const unsigned int *args,
					unsigned int args_count)
{
	struct drvcall_devmgr_irq_map_arg arg;
	struct __actvret_drvcall_devmgr_irq_map ret;

	if ((args == NULL) || (args_count > DEVMGR_IRQ_HWCONF_LEN_MAX)) {
		return 0;
	}

	mem_zero_s(arg);
	NOFAIL(memcpy_s(&arg.hwconf[0], sizeof(arg.hwconf),
			args, (size_t)args_count * sizeof(uint32_t)));

	arg.oirq_offset = oirq_offset;
	arg.hwconf_len = args_count;

	mem_zero_s(ret);
	if (actvcapcall_drvcall_devmgr_irq_map(libdh_get_devmgr_ap(), arg, &ret) < 0) {
		return 0;
	}
	return ret.virt_irq;
}

unsigned int devhost_irq_translate(unsigned int oirq_offset,
				   const unsigned int *args,
				   unsigned int args_count)
{
	struct drvcall_devmgr_irq_map_arg arg;
	struct __actvret_drvcall_devmgr_irq_translate ret;

	if ((args == NULL) || (args_count > DEVMGR_IRQ_HWCONF_LEN_MAX)) {
		return 0;
	}

	mem_zero_s(arg);
	NOFAIL(memcpy_s(&arg.hwconf[0], sizeof(arg.hwconf),
			args, (size_t)args_count * sizeof(uint32_t)));

	arg.oirq_offset = oirq_offset;
	arg.hwconf_len = args_count;

	mem_zero_s(ret);
	if (actvcapcall_drvcall_devmgr_irq_translate(libdh_get_devmgr_ap(), arg, &ret) < 0) {
		return 0;
	}
	return ret.virt_irq;
}

int devhost_irq_set_type(unsigned int linux_irq, unsigned int irq_type)
{
	int err = E_HM_OK;
	struct devhost_irq_info *irq_info = devhost_irq_idr_find_get(linux_irq);
	struct hm_irq_attribute irq_attr = {0};

	if (irq_info == NULL) {
		dh_error("no irqinfo, linux_irq=%u\n", linux_irq);
		return -hmerrno2posix(E_HM_INVAL);
	}

	/* virt_irq does not support set_type. */
	if (is_virt_irq(irq_info->logic_irq)) {
		dh_debug("irq set type for virq %u\n", irq_info->logic_irq);
		devhost_irq_idr_put(irq_info);
		return 0;
	}

	if (irq_type == DEVHOST_IRQ_ATTR_LEVEL_TYPE) {
		irq_attr.attr_type = IRQ_ATTR_LEVEL_TYPE;
	} else {
		irq_attr.attr_type = IRQ_ATTR_EDGE_TYPE;
	}

	err = hm_irq_set_attribute(irq_info->data.irq_rref, irq_info->logic_irq, &irq_attr, IRQ_SET_ATTRIBUTE_TYPE);
	if (err < 0) {
		devhost_irq_idr_put(irq_info);
		dh_error("irq set type failed, linux_irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
		return -hmerrno2posix(err);
	}
	devhost_irq_idr_put(irq_info);
	return 0;
}

int devhost_irq_setaffinity(unsigned int linux_irq, unsigned long affinity)
{
	int err = E_HM_OK;
	struct devhost_irq_info *info = devhost_irq_idr_find_get(linux_irq);
	struct hm_irq_attribute irq_attr = {0};
	if (info == NULL) {
		dh_info("no irqinfo, linux_irq=%u\n", linux_irq);
		return -hmerrno2posix(E_HM_INVAL);
	}

	if (is_kvic_or_virt(info)) {
		dh_info("irq set affinity failed, logic: %u\n", info->logic_irq);
		devhost_irq_idr_put(info);
		return -hmerrno2posix(E_HM_INVAL);
	}

	irq_attr.attr_cpu = (uint32_t)affinity;
	err = hm_irq_set_attribute(info->data.irq_rref, info->logic_irq, &irq_attr,
							IRQ_SET_ATTRIBUTE_CPU);
	if (err < 0) {
		devhost_irq_idr_put(info);
		dh_debug("set irq attribute failed, linux_irq=%u, affinity=0x%lx, err=%s\n",
			 linux_irq, affinity, hmstrerror(err));
		return -hmerrno2posix(err);
	}
	devhost_irq_idr_put(info);
	return err;
}

int devhost_irq_setaffinity_hint(unsigned int linux_irq, unsigned long affinity_hint)
{
	int err = E_HM_OK;
	struct devhost_irq_info *info = devhost_irq_idr_find_get(linux_irq);
	struct hm_irq_attribute irq_attr = {0};
	if (info == NULL) {
		dh_info("no irqinfo, linux_irq=%u\n", linux_irq);
		return -hmerrno2posix(E_HM_INVAL);
	}

	if (is_kvic_or_virt(info)) {
		dh_info("irq set affinity_hint failed, logic: %u\n", info->logic_irq);
		devhost_irq_idr_put(info);
		return -hmerrno2posix(E_HM_INVAL);
	}

	irq_attr.attr_affi_hint = (uint32_t)affinity_hint;
	err = hm_irq_set_attribute(info->data.irq_rref, info->logic_irq, &irq_attr,
							IRQ_SET_ATTRIBUTE_AFFI_HINT);
	if (err < 0) {
		devhost_irq_idr_put(info);
		dh_info("set irq affinity_hint failed, linux_irq=%u, affinity_hint=0x%lx, err=%s\n",
			 linux_irq, affinity_hint, hmstrerror(err));
		return -hmerrno2posix(err);
	}
	devhost_irq_idr_put(info);
	return err;
}

static int devhost_fetch_sgi_ref(int irq, rref_t *rref_ptr,
				 struct drvcall_devmgr_irq_fetch_arg *arg_ptr,
				 struct __actvret_drvcall_devmgr_irq_fetch *ret_ptr)
{
	int err = E_HM_OK;
	struct drvcall_devmgr_irq_fetch_arg arg;
	struct __actvret_drvcall_devmgr_irq_fetch ret;
	rref_t rref;

	rref = hm_ucap_require(0, libdh_get_devmgr_cnode_idx(), 0);
	if (IS_REF_ERR(rref)) {
		dh_error("create rref failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(rref)));
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		mem_zero_s(arg);
		arg.rrefs_nr = 1;
		arg.virt_irq = irq;
		arg.is_sgi = true;
		arg.cpu = (unsigned int)preempt_process_cpu_get();
		arg.irqctrl_rrefs[0] = rref;

		/* fetch info of linux_irq from devmgr to get logic_irq/source_id */
		err = actvcapcall_drvcall_devmgr_irq_fetch(libdh_get_devmgr_ap(), arg, &ret);
		if (err < 0) {
			/* ucap require success, ignore return value */
			(void)hm_ucap_reject(0, rref);
		}
	}

	if (err == E_HM_OK) {
		*rref_ptr = rref;
		*arg_ptr = arg;
		*ret_ptr = ret;
	}

	return err;
}

static int devhost_register_sgi(rref_t rref, unsigned int logic_irq,
				unsigned int *hwconf, unsigned int hwconf_len,
				unsigned int sgi_cpu)
{
	int err = E_HM_OK;
	cref_t tcb_cref = (cref_t)0ULL;

	err = syscap_IrqCtrlConfig(rref, logic_irq, hwconf, hwconf_len);
	if (err == E_HM_OK) {
		for (unsigned int i = 0U; i < config_cpu_nr; i++) {
			if (i == sgi_cpu) {
				tcb_cref = raw_thread_cref_of(default_irq_server[i]);
				break;
			}
		}
		if (IS_REF_ERR(tcb_cref)) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		struct hm_irq_register_setting setting;

		mem_zero_s(setting);
		setting.name = IRQ_IPI_NAME;
		setting.flags = IRQCTRL_REGISTER_STAYWAKE;
		setting.irq_fastpath_ap = 0U;
		setting.is_fp_bypass = 0U;
		setting.cpu_mask = __cpuset_full;
		err = hm_irq_register(rref, logic_irq, &setting, tcb_cref);
	}

	return err;
}

static void devhost_sgi_irq_return(unsigned int linux_irq, rref_t irq_rref)
{
	int err = E_HM_OK;
	struct drvcall_devmgr_irq_return_arg arg;
	unsigned int sgi_cpu = (unsigned int)preempt_process_cpu_get();

	mem_zero_s(arg);
	arg.virt_irq = linux_irq;
	arg.rrefs_nr = 1;
	arg.irqctrl_rrefs[0] = irq_rref;
	arg.is_sgi = true;
	arg.cpu = sgi_cpu;

	/* don't need check return code, if return failed, this rref
	 * still can be used and we will directly free it */
	(void)hm_ucap_export_move(0, irq_rref, libdh_get_devmgr_cnode_idx());
	err = actvcapcall_drvcall_devmgr_irq_return(libdh_get_devmgr_ap(), arg);
	if (err < 0) {
		dh_error("return linux_irq failed, free it directly, irq=%u, err=%s\n",
			 linux_irq, hmstrerror(err));
	}

	/* rref is useless now, ignore ret value */
	(void)hm_ucap_reject(0, irq_rref);
	irq_sgi_rref[linux_irq + IRQ_PERCPU_NR * sgi_cpu] = ERR_TO_REF(E_HM_INVAL);
}

int devhost_register_ipi_handler(int irq, devhost_irq_handler_t handler)
{
	struct drvcall_devmgr_irq_fetch_arg arg;
	struct __actvret_drvcall_devmgr_irq_fetch ret = {0};
	rref_t rref;
	int err = E_HM_OK;
	unsigned int sgi_cpu = (unsigned int)preempt_process_cpu_get();
	unsigned int logic_irq = 0U;

	if (irq >= (int)IRQ_SGI_NR || handler == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = devhost_fetch_sgi_ref(irq, &rref, &arg, &ret);
	}

	if (err == E_HM_OK) {
		logic_irq = ret.rlt.logic_irqs[0];
		irq_sgi_rref[logic_irq] = rref;

		err = devhost_register_sgi(rref, logic_irq,
					   ret.rlt.hwconf, ret.rlt.hwconf_len, sgi_cpu);
		if (err < 0) {
			devhost_sgi_irq_return(irq, rref);
		}
	}

	if (err == E_HM_OK) {
		if (sgi_handler[irq] != handler) {
			sgi_handler[irq] = handler;
		}
		err = hm_irq_enable(rref, logic_irq);
		if (err < 0) {
			sgi_handler[irq] = NULL;
			devhost_sgi_irq_return(irq, rref);
		}
	}

	return err;
}

int devhost_unregister_ipi(int irq)
{
	int err = E_HM_OK;
	unsigned int logic_irq = 0U;

	if (irq >= (int)IRQ_SGI_NR) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		logic_irq = irq + IRQ_PERCPU_NR * (unsigned int)preempt_process_cpu_get();
		if (IS_REF_VALID(irq_sgi_rref[logic_irq])) {
			devhost_sgi_irq_return(irq, irq_sgi_rref[logic_irq]);
		}
	}

	return err;
}

int devhost_check_irq_state(unsigned int linux_irq, int state, int *val)
{
	int err = E_HM_OK;
	struct sysarg_irqctrl_stat stat;
	struct devhost_irq_info *info = devhost_irq_idr_find_get(linux_irq);
	if (info == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	if (is_kvic_or_virt(info)) {
		devhost_irq_idr_put(info);
		return -hmerrno2posix(E_HM_INVAL);
	}

	err = syscap_IrqCtrlStat(info->data.irq_rref, info->logic_irq, &stat);
	if (err < 0) {
		devhost_irq_idr_put(info);
		return -hmerrno2posix(err);
	}
	devhost_irq_idr_put(info);

	*val = 0;
	switch (state) {
	case DEVHOST_IRQ_PENDING:
		if (stat.hw_pending) {
			*val = 1;
		}
		break;
	case DEVHOST_IRQ_ACTIVE:
		if (stat.hw_active) {
			*val = 1;
		}
		break;
	case DEVHOST_IRQ_MASKED:
		if (!stat.hw_enable) {
			*val = 1;
		}
		break;
	default:
		err = -hmerrno2posix(E_HM_INVAL);
		break;
	}

	return err;
}
