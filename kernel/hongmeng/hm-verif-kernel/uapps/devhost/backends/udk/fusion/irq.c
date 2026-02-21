/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK irq server management
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 25 15:50:54 2023
 */

#include <hmkernel/sched/priority.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libalgo/idr.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libsysif/devmgr/types.h>
#include <libdevhost/devhost.h>
#include <libsysif/devmgr/api.h>
#include <udk/log.h>

#include "irq.h"

#define IRQ_SERVER_NAME		"udk-irq-"
#define IRQ_CELLS_NUM		3

/* to simplify irq mapping logic, suppose there is only one irqctrl */
unsigned int default_irqctrl_id;
static unsigned long config_cpu_nr;
static libdh_irq_server_t *g_irq_sever;
static unsigned int g_irq_sever_cnt = 0U;
static DEFINE_IDR(g_irqctx_idr);
static DEFINE_IDR(g_irqmap_idr);

static int __irq_server_init(void)
{
	int err;

	err = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &config_cpu_nr, sizeof(config_cpu_nr));
	if (err < 0) {
		udk_warn("get config cpu number failed\n");
		return E_HM_INVAL;
	}

	g_irq_sever_cnt = config_cpu_nr;
	g_irq_sever = (libdh_irq_server_t *)malloc(sizeof(libdh_irq_server_t) * config_cpu_nr);
	if (g_irq_sever == NULL) {
		return E_HM_NOMEM;
	}

	udk_info("udk fusion irq: init %u server success\n", config_cpu_nr);

	return 0;
}

int udk_fusion_irq_ap_table_init(void)
{
	return libdh_irq_ap_table_init();
}

int udk_fusion_irq_server_init(void)
{
	int err = 0;
	unsigned int si;
	char name[HMCAP_THREAD_NAME_SIZE];

	if ((err = __irq_server_init()) < 0) {
		return err;
	}

	for (si = 0U; si < g_irq_sever_cnt; ++si) {
		mem_zero_a(name);
		err = snprintf_s(name, HMCAP_THREAD_NAME_SIZE,
				 HMCAP_THREAD_NAME_SIZE - 1, IRQ_SERVER_NAME"%u", si);
		BUG_ON(err < 0);
		err = libdh_irq_create_server(name, HM_PRIORITY_DH_KVIC,
					      __cpuset_one(si), &g_irq_sever[si]);
		BUG_ON(err != 0);
	}

	err = libdh_irq_create_fastpath();
	BUG_ON(err < 0);

	return 0;
}

static unsigned int __irq_translate(unsigned int oirq_offset,
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

unsigned int udk_fusion_irq_map(unsigned int oirq_offset,
				const unsigned int *args,
				unsigned int args_count)
{
	int ret;
	unsigned int logic_irq = 0U;
	unsigned int linux_irq = 0U;

	if (args == NULL) {
		return 0U;
	}

	linux_irq = __irq_translate(oirq_offset, args, args_count);

	ret = libdh_irq_xlate_ex(oirq_offset, args, args_count, &logic_irq);
	if (ret < 0) {
		return 0U;
	}
	if (default_irqctrl_id == 0U) {
		default_irqctrl_id = oirq_offset;
	}
	/* fusion udk only support one irqctrl */
	BUG_ON(default_irqctrl_id != oirq_offset);
	if(idr_find(&g_irqmap_idr, logic_irq) != NULL) {
		udk_warn("irq map exist, logic_irq=%u\n", logic_irq);
		return 0U;
	}
	ret = idr_set(&g_irqmap_idr, ulong_to_ptr((unsigned long)linux_irq, void), logic_irq);
	if (ret < 0) {
		return 0U;
	}
	udk_info("irq map success linux_irq=%u, logic_irq=%u\n", linux_irq, logic_irq);

	return logic_irq;
}

int udk_fusion_irq_setup(unsigned int logic_irq, devhost_irq_handler_t handler,
			 const char *devname, void *dev_id)
{
	int ret;
	libdh_irq_ctx_t ctx_out;
	struct libdh_irq_setup_args args = {
		.irqctrl_id = (int)default_irqctrl_id,
		.linux_irq = (unsigned int)ptr_to_ulong(idr_find(&g_irqmap_idr, logic_irq)),
		.logic_irq =  logic_irq,
		.affinity = 0x1U, /* set affinity with CPU0 */
		.handler = handler,
		.name = devname,
		.args = dev_id,
	};

	if ((handler == NULL) || (g_irq_sever == NULL)) {
		return E_HM_INVAL;
	}

	ret = libdh_irq_setup_bind(g_irq_sever, g_irq_sever_cnt, &args, &ctx_out);
	if (ret < 0) {
		return ret;
	}
	ret = idr_set(&g_irqctx_idr, ctx_out, logic_irq);
	if (ret < 0) {
		libdh_irq_close_bind(g_irq_sever, g_irq_sever_cnt, ctx_out);
		return ret;
	}

	udk_info("irq setup success, linux_irq=%u, logic_irq=%u\n", args.linux_irq, logic_irq);

	return 0;
}

int udk_fusion_irq_close(unsigned int logic_irq)
{
	libdh_irq_ctx_t ctx;

	ctx = (libdh_irq_ctx_t)idr_find(&g_irqctx_idr, logic_irq);

	return libdh_irq_close_bind(g_irq_sever, g_irq_sever_cnt, ctx);
}

int udk_fusion_irq_enable(unsigned int logic_irq)
{
	libdh_irq_ctx_t ctx;

	ctx = (libdh_irq_ctx_t)idr_find(&g_irqctx_idr, logic_irq);

	return libdh_irq_enable((const libdh_irq_ctx_t)ctx);
}

static int get_valid_affinity(unsigned long *affinity)
{
	int ret;
	unsigned long online_cpu;

	ret = hm_sysctrl_read_conf(__SYSCONF_ONLINE_CPU_MASK, (void *)&online_cpu,
				   sizeof(online_cpu));
	if (ret < 0) {
		udk_warn("get online cpu mask failed, ret = %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	if ((online_cpu & (*affinity)) == 0) {
		udk_warn("invalid affinity info, online_cpu=0x%lx, affinity=0x%lx",
			 online_cpu, *affinity);
		return -EINVAL;
	}
	*affinity = (*affinity & online_cpu);

	return 0;
}

int udk_fusion_irq_setaffinity(unsigned int logic_irq, unsigned long affinity)
{
	int err;
	unsigned long valid_affinity = affinity;
	libdh_irq_ctx_t ctx;

	err = get_valid_affinity(&valid_affinity);
	if (err < 0) {
		return err;
	}
	ctx = (libdh_irq_ctx_t)idr_find(&g_irqctx_idr, logic_irq);

	return libdh_irq_setaffinity((const libdh_irq_ctx_t)ctx, valid_affinity);
}

static int migrate_one_irq(unsigned int id, void *ptr, void *data)
{
	int err;
	unsigned int cpu;
	unsigned long valid_cpumask;
	unsigned long curr_affinity;
	unsigned long valid_affinity;
	libdh_irq_ctx_t ctx = NULL;

	UNUSED(id);
	if (ptr == NULL || data == NULL) {
		return -EINVAL;
	}

	ctx = (libdh_irq_ctx_t)ptr;
	cpu = *(void_to_ptr(data, unsigned int));
	if (libdh_irq_getaffinity(ctx, &curr_affinity) < 0) {
		return -EINVAL;
	}

	/* no need to migrate curr irq */
	if ((curr_affinity & (1UL << cpu)) == 0) {
		return 0;
	}

	/* fetch the online CPU mask */
	valid_cpumask = ~(1UL << cpu);
	err = get_valid_affinity(&valid_cpumask);
	if (err < 0) {
		return err;
	}

	valid_affinity = (valid_cpumask & curr_affinity);
	if (valid_affinity == 0UL) {
		valid_affinity = (1UL << udk_cpu_mask_to_valid_cpuid(valid_cpumask));
	}

	return libdh_irq_setaffinity((const libdh_irq_ctx_t)ctx, valid_affinity);
}

void udk_fusion_migrate_all_irq_of_cpu(unsigned int cpu)
{
	int err;

	err = idr_for_each(&g_irqctx_idr, migrate_one_irq, &cpu);
	if (err < 0) {
		udk_warn("migrate irq on cpu=%u failed, err: %s\n",
			 cpu, strerror(-err));
	}
}
