/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Entry point for devhost
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <dlfcn.h>
#include <unistd.h>
#include <getopt.h>
#include <hmasm/page.h>
#include <sys/types.h>
#include <libmem/umem.h>
#include <libcrt/hmcrt.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/utils.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/server.h>
#include <libhmsync/raw_thread.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_init.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_event.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_proc_affinity.h>
#include <libhmsrv_sys/hm_hkip.h>
#include <api/devhost_api.h>
#include <libdevhost/sysfs.h>
#include <fs_server.h>
#include <fd.h>
#include <vfs.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/pac.h>
#include <libhmsrv_sys/hm_timer.h>
#endif

#include "devhost.h"
#include <devhost/log.h>
#include <devhost/cfg.h>
#include "devhost_backend.h"
#include "utils/utils.h"
#include "plugins/plugin.h"

#ifdef CONFIG_HKIP
#include <libroafterinit/roafterinit.h>
#endif

static int __dh_log_ident = 0;
unsigned int g_dh_self_cnode_idx;

static DLIST_HEAD(g_barrier_list);
static struct raw_mutex g_barrier_lock = RAW_MUTEX_INITIALIZER;
static struct devhost_backend_args backend_args;

static __cpuset_t irq_default_affinity = __cpuset_full;
static int __is_root_dh = 0; /* 0: defualt not root devhost */

#define MIN_ARGS_NUM 2
static const struct option long_options[] = {
	{"irqthread_affinity", required_argument, NULL, (int)'i'},
	{"total_poolcfg", required_argument, NULL, (int)'m'},
	{"pools", required_argument, NULL, (int)'p'},
	{"isolate", required_argument, NULL, (int)'s'},
	{"extensions", required_argument, NULL, (int)'e'},
	{"args", required_argument, NULL, (int)'a'},
	{NULL, no_argument, NULL, 0}
};

int dh_log_ident(void)
{
	return __dh_log_ident;
}

bool dh_is_root(void)
{
	return !!__is_root_dh;
}

#ifdef CONFIG_HKIP
static void devhost_enable_hkip_protect(void)
{
	hm_hkip_protect_proc();
}
#else
static void devhost_enable_hkip_protect(void){}
#endif

int devhost_register_barrier(int (*barrier_fn)(void *arg), void *arg,
			     enum devhost_barrier_level level)
{
	struct devhost_barrier *barrier = NULL;

	if ((barrier_fn == NULL) || (level >= DEVHOST_BARRIER_LEVEL_MAX)) {
		return E_HM_INVAL;
	}

	barrier = (struct devhost_barrier *)malloc(sizeof(struct devhost_barrier));
	if (barrier == NULL) {
		return E_HM_NOMEM;
	}

	dlist_init(&barrier->node);
	barrier->barrier_fn = barrier_fn;
	barrier->arg = arg;
	barrier->level = level;

	raw_mutex_lock(&g_barrier_lock);
	dlist_insert_tail(&g_barrier_list, &barrier->node);
	raw_mutex_unlock(&g_barrier_lock);

	return E_HM_OK;
}

static int devhost_pre_init_barrier(void)
{
	int ret = E_HM_OK;
	struct devhost_barrier *barrier = NULL;
	struct devhost_barrier *tmp = NULL;
	int level;

	raw_mutex_lock(&g_barrier_lock);
	for (level = (int)DEVHOST_BARRIER_LEVEL_PRE;
	     level < (int)DEVHOST_BARRIER_LEVEL_MAX; level++) {
		dlist_for_each_entry_safe(barrier, tmp, &g_barrier_list,
					  struct devhost_barrier, node) {
			if ((int)barrier->level != level) {
				continue;
			}
			if (ret == E_HM_OK) {
				ret = barrier->barrier_fn(barrier->arg);
			}
			dlist_delete(&barrier->node);
			free(barrier);
		}
	}
	raw_mutex_unlock(&g_barrier_lock);

	return ret;
}

static int devhost_pre_init(void)
{
	int ret = E_HM_OK;
	rref_t atfd_rref;
	cref_t ap;

	/* set spin mode */
	hm_setup_spin_flags();

	/* acquire atfd module */
	atfd_rref = hm_module_open("atfd");
	if (IS_REF_ERR(atfd_rref)) {
		dh_info("no atfd module support, ignore\n");
	} else {
		/* atfd only support bound modif call */
		ret = hm_module_bind_modif(atfd_rref);
		if (ret < 0) {
			dh_error("bind atfd module failed, err=%s\n",
				 hmstrerror(ret));
			goto err_exit;
		}
	}

	/* initialize actv pool */
	ret = devhost_ap_init(&ap);
	if (ret < 0) {
		dh_error("create actv pool failed, err=%s\n", hmstrerror(ret));
		goto err_exit;
	}
	/* initialize libdh */
	ret = libdh_init_ap("devhost", ap, DH_FLAGS_STANDALONE);
	if (ret != 0) {
		dh_error("libdh initialization failed, err=%s\n",
			 hmstrerror(ret));
		goto err_exit;
	}
	/* initialize devhost irq core after ap_init */
	ret = devhost_irq_setup();
	if (ret < 0) {
		dh_error("set up irq failed, err=%s\n", hmstrerror(ret));
		goto err_exit;
	}

	/* sysfs init */
	ret = libdh_sysfs_init();
	if (ret < 0) {
		dh_error("sysfs init failed, err=%s\n", hmstrerror(ret));
		goto err_exit;
	}

	ret = devhost_pre_init_barrier();
	if (ret < 0) {
		goto err_exit;
	}

	return E_HM_OK;

err_exit:
	hm_module_close(atfd_rref);
	return ret;
}

static int devhost_register_to_fsmgr(void)
{
	int ret;
	uref_t dh_uref;
	rref_t fsmgr_rref = 0ULL;
	struct sysarg_cnode_statrref stat = { 0 };

	fsmgr_rref = acquire_cache_fsmgr_rref();
	if (unlikely(fsmgr_rref == FS_INVALID_CNODE_RREF)) {
		dh_error("acquire fsmgr rref failed\n");
		return E_HM_POSIX_FAULT;
	}

	ret = sysfast_cnode_stat_rref(&stat, fsmgr_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret != E_HM_OK) {
		hm_error("stat fsmgr rref failed: %s\n", hmstrerror(ret));
		return ret;
	}
	dh_uref = libdh_ap_export(stat.src_cnode_idx);
	if (IS_REF_ERR(dh_uref)) {
		ret = REF_TO_ERR(dh_uref);
		hm_error("export actv pool to fsmgr failed: %s\n", hmstrerror(ret));
		return ret;
	}
	ret = actvxactcapcall_fscall_vfs_register_devhost(false, true, fsmgr_rref, dh_uref);
	if (ret != E_HM_OK) {
		hm_error("export uref to fsmgr failed: %s\n", hmstrerror(ret));
		(void)hm_ucap_revoke(0, dh_uref);
		return ret;
	}

	return E_HM_OK;
}

static int devhost_register_to_sysmgr(void)
{
	int ret = E_HM_OK;
	uref_t dh_uref = 0ULL;
	uref_t dh_uref_to_eventmgr = 0ULL;

	dh_uref = libdh_ap_export(hmsrv_sysmgr_cnode_idx());
	if (IS_REF_ERR(dh_uref)) {
		dh_error("export actv pool to sysmgr failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(dh_uref)));
		return REF_TO_ERR(dh_uref);
	}

	/* register devhost uref to sysmgr */
	ret = actvxactcall_hmcall_mem_register_devhost(false, false, dh_uref);
	if (ret < 0) {
		(void)hm_ucap_revoke(0, dh_uref);
		dh_error("export uref to sysmgr failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	/* register events to eventmgr */
	dh_uref_to_eventmgr = libdh_ap_export(hmsrv_sysmgr_cnode_idx());
	if (IS_REF_ERR(dh_uref_to_eventmgr)) {
		(void)hm_ucap_revoke(0, dh_uref);
		dh_error("export actv pool to sysmgr eventmgr failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(dh_uref_to_eventmgr)));
		return REF_TO_ERR(dh_uref_to_eventmgr);
	}

	ret = hm_event_register_event(dh_uref_to_eventmgr, EVTMGR_ETYPE_PROCESS_EXIT,
				      __devhost_method_process_revoke);
	if (ret == E_HM_NOSYS) {
		(void)hm_ucap_revoke(0, dh_uref_to_eventmgr);
		dh_info("system not support eventmgr!\n");
		ret = E_HM_OK;
	} else if (ret < 0) {
		(void)hm_ucap_revoke(0, dh_uref);
		(void)hm_ucap_revoke(0, dh_uref_to_eventmgr);
		dh_error("register to eventmgr failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

#ifdef CONFIG_SEC_SERVICE
	ret = devhost_reg_sec_mgmt();
	if (ret != 0) {
		dh_error("register security service to sysmgr failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
#endif

	return ret;
}

const struct devhost_backend_args *devhost_get_backend_args(void)
{
	return &backend_args;
}

__cpuset_t devhost_proc_default_affinity(void)
{
	int err;
	struct proc_affinity_info_s proc_affinity_info;

	mem_zero_s(proc_affinity_info);
	err = hm_proc_affinity_get(0, &proc_affinity_info);
	if (err < 0) {
		/* fail to get default affinity, default affinity 0 is illegal */
		dh_warn("devhost proc get default affinity failed, %s\n", hmstrerror(err));
		return __cpuset_empty;
	}

	return proc_affinity_info.default_affinity;
}

__cpuset_t devhost_get_default_affinity(void)
{
	return irq_default_affinity;
}

static void devhost_parse_irq_default_affinity(char* buf)
{
	long long num;
	char *str = NULL;
	char *next_token = NULL;
	unsigned int beg, end;
	__cpuset_t affinity_mask = __cpuset_empty;

	if (strcmp(buf, "") == 0) {
		return;
	}

	str = strtok_s(buf, ",", &next_token);
	BUG_ON(str == NULL);
	while(str != NULL) {
		if (strstr(str, "-")) {
			/* ensure that all 2 parameters are obtained successfully */
			if (sscanf_s(str, "%u-%u", &beg, &end) != 2) {
				dh_error("devhost irq sscanf_s affinity buf[%s] failed.\n", buf);
				break;
			}

			do {
				affinity_mask = __cpuset_add(affinity_mask, beg);
				beg++;
			} while (beg <= end);
		} else {
			num = strtoll(str, NULL, 0);
			if ((errno != 0) || (num < 0)) {
				dh_error("devhost irq strtoll affinity buf[%s] failed.\n", buf);
				break;
			}
			affinity_mask = __cpuset_add(affinity_mask, (unsigned int)num);
		}

		str = strtok_s(NULL, ",", &next_token);
	}

	irq_default_affinity = affinity_mask;
}

static void devhost_parse_isolate(char *buf)
{
	if (__is_root_dh != 0) {
		return;
	}

	if (buf == NULL || *buf == '\0') {
		__is_root_dh = 1;
	}
}

static int __parse_devhost_argv(int argc, char **argv)
{
	int opt;
	/* must have a backend args. extension is optional */
	if (argc <= 1 || argv[1] == NULL) {
		return E_HM_INVAL;
	}

	backend_args.argc = argc - 1;
	backend_args.argv = argv + 1;

	if (backend_args.argc < MIN_ARGS_NUM) {
		return E_HM_OK;
	}
	while ((opt = getopt_long(backend_args.argc, backend_args.argv, "", long_options, NULL)) != -1) {
		switch (opt) {
		case (int) 'i':
			devhost_parse_irq_default_affinity(optarg);
			break;
		case (int) 'm':
			devhost_parse_poolcfg(optarg);
			break;
		case (int) 'p':
			devhost_parse_pools(optarg);
			break;
		case (int) 's':
			devhost_parse_isolate(optarg);
			break;
		default:
			break;
		}
	}
	optind = 0;

	return E_HM_OK;
}

static void __dh_thread_ctx_setup(void)
{
	backend_ctx_setup();
}

/*
 * Define s strong symbol func `securec_is_fp_enabled` in elf will
 * override the weak symbol func `securec_is_fp_enabled` in libhwsecurec.so.
 * The return value indicates whether fpu is supported:
 * 0: not support
 * 1: support (default)
 * The `securec_is_fp_enabled` func is only used once during so reloc,
 * and will not be used where the securec func is actuall called.
 */
int securec_is_fp_enabled(void) { return 0; }

#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __dh_ro_after_init_hdlr_start []) (void);
extern void (*const __dh_ro_after_init_hdlr_end []) (void);
static void __PAC_INIT_SECTION devhost_hdlr_init(uintptr_t begin, uintptr_t end)
{
        uintptr_t reg_global_func = begin;
        while (reg_global_func < end) {
                void (*func)(void);
                func = *(void (**)(void))reg_global_func;
                PAC_FECFI_XPAC_SIGN(func, void(*)(void));
                *(void (**)(void))reg_global_func = func;
                reg_global_func += sizeof(void(*)(void));
        }
        return;
}
#endif

int main(int argc, char **argv)
{
	int ret = E_HM_OK;
#ifdef PAC_FWCFI_SYSSERVICE
	devhost_hdlr_init((uintptr_t)&__dh_ro_after_init_hdlr_start, (uintptr_t)(&__dh_ro_after_init_hdlr_end));
	(void)hm_clock_gettime_init();
#endif
	devhost_enable_hkip_protect();
	ret = __parse_devhost_argv(argc, argv);
	if (ret < 0) {
		return ret;
	}

	/* use pid for log identifier */
	__dh_log_ident = hm_getpid();
	g_dh_self_cnode_idx = hm_ucap_self_cnode_idx();

	/* show sysif methods */
	sysif_show_devhost_methods();

	/* pm ap table is protected by hkip, should init before security plugin */
	devhost_pm_ap_table_init();

	/* irq ap table is protected by hkip, do init before security plugin */
	devhost_irq_ap_table_init();

	ret = libdh_plugin_sys_init();
	if (ret < 0) {
		dh_error("plugin subsystem init failed\n");
		return ret;
	}

	/* devhost pre init, all backend dependencies should init first */
	ret = devhost_pre_init();
	if (ret < 0) {
		dh_error("pre-init failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

#ifdef PAC_FWCFI_SYSSERVICE
	devhost_hdlr_init((uintptr_t)&__dh_ro_after_init_hdlr_start, (uintptr_t)(&__dh_ro_after_init_hdlr_end));
#endif
	/*
	 * Note:
	 *   Devhost rref needs to be registered to fsmgr
	 *   after the devhost actv pool is registered
	 *   and before the LDK initialization starts.
	 */
	ret = devhost_register_to_fsmgr();
	if (ret != E_HM_OK) {
		return ret;
	}

	/*
	 * new wq-thread will do ctx_setup first after here
	 * wq-thread should do ctx_setup manually before here
	 */
	raw_workqueue_thread_ctx_setup_register(__dh_thread_ctx_setup);

	/* backend stage1: initialize backend after DEVHOST_REGISTER */
	ret = init_backend_dllib(argv[1]);
	if (ret < 0) {
		dh_error("backend initialization failed, err=%s\n",
			 hmstrerror(ret));
		return ret;
	}

	/* start: stage1.5 which depend on backend */
	/* init devhost workq: hm_user_futex_wait -> liblinux_preempt_disable */
	ret = devhost_workq_init();
	if (ret < 0) {
		dh_error("devhost workq init failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
	/* initialize devhost pm actv-pool. before devhost_do_postinit and after LDK loading */
	devhost_prepare_pm_actvs();
	devhost_init_actvs_create();
	/* end: stage1.5 */

	/* backend stage2: post-init callbacks */
	ret = devhost_do_postinit();
	if (ret < 0) {
		dh_error("post init failed, err=%s\n", hmstrerror(ret));
		/* unregister to devmgr before we exit */
		return ret;
	}

	(void)libdh_devhost_flush_async_kernfs();

	ret = devhost_register_to_sysmgr();
	if (ret != 0) {
		dh_error("register to sysmgr failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	/* bind main thread to default affinity after boot */
	hm_proc_set_default_affinity(0);

#ifdef CONFIG_HKIP
	roafterinit_register_ro();
#endif

	return devhost_actvpool_elastic();
}
