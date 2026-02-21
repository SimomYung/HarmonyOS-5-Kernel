/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Activation infrastructure
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include "devhost.h"

#include <syscall.h>
#include <hmasm/page.h>
#include <sys/mman.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/server.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libmem/utils.h>
#include <libstrict/strict.h>
#include <libpreempt/preempt.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_proc_affinity.h>
#ifdef CONFIG_AUDIT
#include <libhmaudit/audit.h>
#endif

#include "devhost_backend.h"

#include <devhost/log.h>
#include <devhost/file.h>
#ifdef CONFIG_NET_FUSION
#include <devhost/workqueue.h>
#endif
#include "utils/utils.h"

#define DEVHOST_ACTV_STACK_ORDER	2UL /* 16K */
#define DEVHOST_ACTV_STACK_SIZE		(1UL << (DEVHOST_ACTV_STACK_ORDER + PAGE_SHIFT))
#define DEVHOST_ACTV_INIT_NUM		8
#define DEVHOST_ACTV_BUF_SIZE		2048UL
#define DEVHOST_ACTV_POOL_ADD_WATER	8u
#define DEVHOST_ACTV_POOL_DEL_WATER	12800u
#define DEVHOST_ACTV_TSD_SIZE		128u
#define DEVHOST_ACTV_POOL_NAME		"devhost_actv"
#define DEVHOST_ACTV_MAX_NUM 5120U /* 1024 + 4096(Default maximum for ptys) */
#define DEVHOST_ELASTIC_PREFIX "devhost"

static cref_t g_devhost_ap;
static struct actvpool_configs devhost_ap_cfg = {
	.ap_name		= DEVHOST_ACTV_POOL_NAME,
	.path_name		= "", /* set empty string, means do not add path */

	.actv_init_num		= 0, /* should init actv when crate it */
	.actv_max_num		= DEVHOST_ACTV_MAX_NUM,
	.recvbuf_size		= DEVHOST_ACTV_BUF_SIZE,
	.tsd_size		= DEVHOST_ACTV_TSD_SIZE,
	.stack_size		= DEVHOST_ACTV_STACK_SIZE,
	.actvret_size		= sizeof(union __actvret_drvcall_devhost),
	.action_table		= sysif_devhost_actvcapcallhdlr_table,
	.action_table_size	= sizeof(void *) * (size_t)__devhost_method_MAX,

	.p			= NULL
};

static int devhost_actv_init(void)
{
#ifdef CONFIG_AUDIT
	audit_server_num_set(__DCOMB(server, devhost));
#endif
	return backend_ctx_setup();
}

static int devhost_actvpool_set_schedinfo(void)
{
	int err;
	unsigned long online_mask;
	__cpuset_t online_set;
	struct __actvpool_sched_info_s sched_info;
	struct proc_affinity_info_s proc_affinity_info;

	mem_zero_s(sched_info);
	mem_zero_s(proc_affinity_info);
	err = hm_sysctrl_read_conf(__SYSCONF_ONLINE_CPU_MASK, (void *)&online_mask,
				  sizeof(online_mask));
	if (err < 0) {
		dh_error("devhost: get online mask failed, err=%s\n", hmstrerror(err));
	} else {
		err = E_HM_OK;
	}
	if (err == E_HM_OK) {
		err = hm_proc_affinity_get(0, &proc_affinity_info);
		if (err != E_HM_OK) {
			dh_error("devhost: get proc_affinity failed, err=%s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		if (proc_affinity_info.privileged == 0) {
			online_set.__bits = (unsigned int)online_mask;
			if (__cpuset_contain(proc_affinity_info.default_affinity, online_set) == 0) {
				sched_info.affinity = proc_affinity_info.default_affinity;
				NOFAIL(hm_apconfig_private_set_schedinfo(&devhost_ap_cfg, &sched_info,
							 __ACTV_POOL_SCHED_INFO_FLAG_AFFINITY));
			}
		}
	}

	return err;
}

static cref_t devhost_setup_actvpool(void)
{
	int err;
	cref_t actv_pool = ERR_TO_REF(E_HM_POSIX_FAULT);

	/* create actv-pool only. don't crate actv */
	NOFAIL(hm_apconfig_private_init(&devhost_ap_cfg));
	NOFAIL(hm_apconfig_private_set_actvinitfunc(&devhost_ap_cfg, devhost_actv_init));
	NOFAIL(hm_apconfig_private_enable_preinitentry(&devhost_ap_cfg));
	NOFAIL(hm_apconfig_private_set_elasticparam(&devhost_ap_cfg, true /* main_thread_elastic */,
						    DEVHOST_ACTV_POOL_ADD_WATER,
						    DEVHOST_ACTV_POOL_DEL_WATER,
						    DEVHOST_ELASTIC_PREFIX));
	if (preempt_process_feature_enabled()) {
		NOFAIL(hm_apconfig_private_enable_alak(&devhost_ap_cfg));
	}
	err = devhost_actvpool_set_schedinfo();
	if (err == E_HM_OK) {
		err = hm_setup_actvpool(&devhost_ap_cfg, &actv_pool);
		if (err < 0) {
			dh_error("devhost setup actvpool failed, err=%s\n", hmstrerror(err));
			actv_pool = ERR_TO_REF(err);
		}
	}

	return actv_pool;
}

#define DEVHOST_UPCALL_HANDLER_INIT(callno, hdlr)						\
	do {											\
		sysif_devhost_actvcapcallhdlr_table[callno].p_sysif_handle_kcall_entry = hdlr;	\
	} while (0)

static void devhost_ex_actvhandler_init_offset(unsigned int offset)
{
	DEVHOST_UPCALL_HANDLER_INIT(__ACTV_UPCALL_RPCCNT_PUT + offset, handle_rpccnt_put);
}

static void devhost_actvhandler_init(void)
{
	for(unsigned int times = 0U; times < __devhost_method_TIMES; times++) {
		devhost_ex_actvhandler_init_offset(times * __devhost_method_MAX);
	}
}

int devhost_ap_init(cref_t *ap_cref)
{
	int err = E_HM_OK;
	cref_t ap;

	if (ap_cref == NULL) {
		return E_HM_INVAL;
	}

	devhost_actvhandler_init();

	/* create actv pool */
	ap = devhost_setup_actvpool();
	if (IS_REF_ERR(ap)) {
		return REF_TO_ERR(ap);
	}

	*ap_cref = ap;
	g_devhost_ap = ap;

	if (dh_is_root()) {
		err = hm_sysctrl_register_lsyscall_server(__LSYSCALL_SVR_DEVHOST, ap);
		if (err < 0) {
			dh_error("devhost register lsyscall server failed, err=%s\n", hmstrerror(err));
		}
	}

	return err;
}

void devhost_init_actvs_create(void)
{
	int index = 0;
	int err = E_HM_OK;

	if (devhost_ap_cfg.p == NULL || devhost_ap_cfg.p->p_actv_attr == NULL) {
		dh_panic("devhost_ap_cfg actvpool_private is abnormal\n");
	}

	while (index < DEVHOST_ACTV_INIT_NUM) {
		err = hm_actv_pure_create_pre_init(devhost_ap_cfg.p->p_actv_attr);
		if (err != E_HM_OK) {
			dh_panic("dh init actv create failed: i: %d, err: %s\n",
				 index, hmstrerror(err));
		}

		index++;
	}
}

int devhost_actvpool_elastic(void)
{
	int err;

	/* set thread name to dh-actv-elastic */
	err = hm_thread_setname(0, DH_ACTV_ELASTIC_TNAME, NULL);
	if (err < 0) {
		dh_warn("set elastic thread name failed, err=%s\n", hmstrerror(err));
	}

#ifdef CONFIG_NET_FUSION
	struct raw_workqueue *actvpool_wq = devhost_get_actvpool_wq();
	NOFAIL(hm_apconfig_private_set_workqueue(&devhost_ap_cfg, actvpool_wq, NULL));
#endif
	if (err == E_HM_OK) {
		err = hm_actvpool_setup_elastic(&devhost_ap_cfg);
		if (err < 0) {
			dh_error("devhost setup actvpool elastic failed, err=%s\n", hmstrerror(err));
		}
	}

	return err;
}
