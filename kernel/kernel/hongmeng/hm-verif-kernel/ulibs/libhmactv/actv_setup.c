/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: actv setup
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 05 15:44:20 2021
 */
#include <hmkernel/trigger.h>
#include <hmkernel/sched/sched.h>
#include <libsched/priority.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libsysif/utils_server.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhwsecurec/securec.h>
#include <libmem/utils.h>
#include <sched.h>

int hm_apconfig_private_init(struct actvpool_configs *configs)
{
	int err = E_HM_OK;

	if (configs == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		configs->p = (struct actvpool_private *)malloc(sizeof(struct actvpool_private));
		if (configs->p == NULL) {
			hm_error("alloc actvpool private mem failed\n");
			err = E_HM_NOMEM;
		} else {
			mem_zero_s(*configs->p);
		}
	}

	if (err == E_HM_OK) {
		configs->p->p_actv_attr = (struct actv_pure_attr *)malloc(sizeof(struct actv_pure_attr));
		if (configs->p->p_actv_attr == NULL) {
			hm_error("alloc actv attr mem failed\n");
			free(configs->p);
			configs->p = NULL;
			err = E_HM_NOMEM;
		} else {
			mem_zero_s(*configs->p->p_actv_attr);
		}
	}

	if (err == E_HM_OK) {
		configs->p->p_emrg_attr = (struct actv_emrg_attr *)malloc(sizeof(struct actv_emrg_attr));
		if (configs->p->p_emrg_attr == NULL) {
			hm_error("alloc emrg attr mem failed\n");
			free(configs->p->p_actv_attr);
			free(configs->p);
			configs->p = NULL;
			err = E_HM_NOMEM;
		} else {
			mem_zero_s(*configs->p->p_emrg_attr);
		}
	}

	return err;
}

void hm_apconfig_private_destroy(struct actvpool_configs *configs)
{
	if (configs != NULL && configs->p != NULL) {
		/* free p_actv_attr */
		if (configs->p->p_actv_attr != NULL) {
			if (configs->p->share_trig_wq != NULL) {
				configs->p->p_actv_attr->trig_wq = NULL;
				configs->p->share_trig_wq = NULL;
			}

			/* free p_actv_attr->trig_wq */
			if (configs->p->p_actv_attr->trig_wq != NULL) {
				free(configs->p->p_actv_attr->trig_wq);
				configs->p->p_actv_attr->trig_wq = NULL;
			}
			free(configs->p->p_actv_attr);
			configs->p->p_actv_attr = NULL;
		}
		/* free p_emrg_attr */
		if (configs->p->p_emrg_attr != NULL) {
			free(configs->p->p_emrg_attr);
			configs->p->p_emrg_attr = NULL;
		}

		if (configs->p->sched_info != NULL) {
			free(configs->p->sched_info);
			configs->p->sched_info = NULL;
		}
		/* free actvpool private */
		free(configs->p);
		configs->p = NULL;
	}
}

int hm_apconfig_private_enable_alak(const struct actvpool_configs *configs)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL) {
		err = E_HM_INVAL;
	} else {
		configs->p->use_actv_local_area_k = true;
	}

	return err;
}

int hm_apconfig_private_enable_preinitentry(const struct actvpool_configs *configs)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL) {
		err = E_HM_INVAL;
	} else {
		configs->p->use_pre_init_entry = true;
	}

	return err;
}

int hm_apconfig_private_enable_kcall_ret(struct actvpool_configs *configs)
{
	int err = E_HM_OK;

	if (configs == NULL) {
		err = E_HM_INVAL;
	} else if (configs->actvret_size < __ACTV_UPCALL_MAX_RET_ARG_SZ) {
		/* Reuse the ret buffer to pass the upcall rlambda args */
		configs->actvret_size = __ACTV_UPCALL_MAX_RET_ARG_SZ;
	} else {
		/* do nothing */
	}

	return err;
}

int hm_apconfig_private_set_emrgnocow(const struct actvpool_configs *configs)
{
	if (configs == NULL || configs->p == NULL || configs->p->p_emrg_attr == NULL) {
		return E_HM_INVAL;
	}

	/* Lock the memory of p_emrg_attr to avoid COW/swap */
	return (int)hm_mem_mlock(configs->p->p_emrg_attr, sizeof(struct actv_emrg_attr));
}

int hm_apconfig_private_set_actvinitfunc(const struct actvpool_configs *configs,
					 actv_init_func_t func)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL) {
		err = E_HM_INVAL;
	} else {
		configs->p->actv_init = func;
	}

	return err;
}

int hm_apconfig_private_set_memfunc(const struct actvpool_configs *configs,
				    mem_alloc_func_t mem_alloc,
				    mem_free_func_t mem_free)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL ||
	    (mem_alloc != NULL && mem_free == NULL) ||
	    (mem_alloc == NULL && mem_free != NULL)) {
		err = E_HM_INVAL;
	} else {
		configs->p->mem_alloc	= mem_alloc;
		configs->p->mem_free	= mem_free;
	}

	return err;
}

int hm_apconfig_private_set_poolid(const struct actvpool_configs *configs,
				   unsigned int resv_actv_idpool)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL) {
		err = E_HM_INVAL;
	} else {
		configs->p->resv_actv_idpool = resv_actv_idpool;
	}

	return err;
}

int hm_apconfig_private_set_bindfunc(const struct actvpool_configs *configs,
				     actvpool_bind_func_t func)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL) {
		err = E_HM_INVAL;
	} else {
		configs->p->bind_func = func;
	}

	return err;
}

int hm_apconfig_private_set_pathparam(const struct actvpool_configs *configs,
				      const mode_t *mode,
				      const unsigned long *grant_info,
				      bool grant2sysmgr)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL) {
		err = E_HM_INVAL;
	} else {
		if (mode != NULL) {
			configs->p->path_add_with_mode = true;
			configs->p->path_mode = *mode;
		}
		if (grant_info != NULL) {
			configs->p->path_set_grant_info = true;
			configs->p->path_grant_info = *grant_info;
		}
		configs->p->path_grant2sysmgr = grant2sysmgr;
	}

	return err;
}

int hm_apconfig_private_set_elasticparam(const struct actvpool_configs *configs,
					 bool main_thread_elastic,
					 unsigned int add_water,
					 unsigned int del_water,
					 const char *elastic_prefix)
{
	int err = E_HM_OK;
	int ret;

	if (configs == NULL || configs->p == NULL || add_water >= del_water) {
		err = E_HM_INVAL;
	} else {
		configs->p->main_thread_elastic = main_thread_elastic;
		configs->p->add_water = add_water;
		configs->p->del_water = del_water;
		if (elastic_prefix != NULL) {
			ret = strncpy_s(configs->p->elastic_prefix, ACTVPOOL_NAME_LENGTH_MAX,
					elastic_prefix, strlen(elastic_prefix));
			if (ret != 0) {
				hm_error("copy elastic prefix failed, ret=%d\n", ret);
				err = E_HM_RESFAULT;
			}
		}
	}

	return err;
}

int hm_apconfig_private_set_schedinfo(struct actvpool_configs *configs,
				      const struct __actvpool_sched_info_s *sched_info,
				      unsigned int flags)
{
	int err = E_HM_OK;

	if (configs != NULL && configs->p != NULL) {
		if (configs->p->sched_info == NULL) {
			configs->p->sched_info = (struct __actvpool_sched_info_s *)malloc(sizeof(struct __actvpool_sched_info_s));
			if (configs->p->sched_info == NULL) {
				hm_error("alloc sched_info mem failed\n");
				err = E_HM_NOMEM;
			}
		}
		if (err == E_HM_OK) {
			NOFAIL(memcpy_s(configs->p->sched_info, sizeof(struct __actvpool_sched_info_s),
			       sched_info, sizeof(struct __actvpool_sched_info_s)));
			configs->p->flags |= flags;
		}
	}

	return err;
}

int hm_apconfig_private_set_workqueue(struct actvpool_configs *configs,
				      struct raw_workqueue *trig_wq,
				      const char *workqueue_prefix)
{
	int err = E_HM_OK;
	int ret;

	if (configs != NULL && configs->p != NULL && trig_wq != NULL) {
		if (workqueue_prefix != NULL) {
			ret = snprintf_s(&configs->p->share_trig_name[0], sizeof(configs->p->share_trig_name),
					 sizeof(configs->p->share_trig_name) - 1U, "%s-t-wq", workqueue_prefix);
			if (ret <= 0) {
				hm_error("build trigger wq name failed, ret=%d, ap=%s\n",
					 ret, configs->ap_name);
				err = E_HM_OVERFLOW;
			}
		}
		configs->p->share_trig_wq = trig_wq;
	}

	return err;
}

static int actv_pure_attr_init(const struct actvpool_configs *configs, cref_t ap_cref)
{
	unsigned long membuf_size;
	void *actv_kobj_pool = NULL;
	int err = E_HM_OK;
	struct actvpool_private *p = configs->p;
	struct actv_pure_attr *attr = p->p_actv_attr;

	/* membuf contains actv_pure, recv buf, stack buf and one page for stack protect. */
	membuf_size = sizeof(struct actv_pure) + configs->recvbuf_size +
		      configs->stack_size + __U(PAGE_SIZE);
	membuf_size = ALIGN_UP(membuf_size, PAGE_SIZE);

	actv_kobj_pool = actv_kobj_pool_init(configs->actv_max_num);
	if (actv_kobj_pool == NULL) {
		err = E_HM_NOMEM;
	}

	if (err == E_HM_OK) {
		attr->tsd_size		= configs->tsd_size;
		attr->stack_size	= configs->stack_size;
		attr->actvret_size	= configs->actvret_size;
		attr->recvbuf_size	= configs->recvbuf_size;
		attr->action_table	= configs->action_table;
		attr->action_table_size	= configs->action_table_size;

		attr->resv_actv_idpool	= p->resv_actv_idpool;
		attr->actv_init		= p->actv_init;
		attr->mem_alloc		= NULL;
		attr->mem_free		= NULL;

		attr->self_init_hdlr	= actv_pure_self_init_hdlr;
		attr->comb_kobj_hdlr	= actv_pure_stack_comb_kobj;
		attr->link_thread	= raw_thread_self();
		attr->membuf_size	= membuf_size;
		attr->actv_kobj_pool	= actv_kobj_pool;
		attr->actv_pool		= ap_cref;
	}

	return err;
}

static int setup_pure_actvs(const struct actvpool_configs *configs, cref_t actv_pool)
{
	int err;
	struct actvpool_private *p = configs->p;

	err = actv_pure_attr_init(configs, actv_pool);
	if (err == E_HM_OK) {
		err = hm_actv_pure_assist_mem_init(p->p_actv_attr);
		if (err < 0) {
			hm_error("init actv assist mem failed, err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
		}
	}

	if (err == E_HM_OK) {
		for (unsigned int i = 0U; i < configs->actv_init_num; i++) {
			if (p->use_pre_init_entry) {
				err = hm_actv_pure_create_pre_init(p->p_actv_attr);
			} else {
				err = hm_actv_pure_create(p->p_actv_attr);
			}
			if (err < 0) {
				/*
				 * Should not free the assist mem here, as it might
				 * be used by the first few actvs which were created
				 * successfully.
				 */
				hm_error("create actv pure failed, idx=%u, err=%s, ap=%s\n",
					 i, hmstrerror(err), configs->ap_name);
				break;
			}
		}
	}

	return err;
}

static cref_t setup_actvpool(struct actvpool_configs *configs)
{
	int err = E_HM_OK;
	cref_t actv_pool;
	struct actv_bind_ret_rref sysmgr_bind_ret;
	struct actvpool_private *p = configs->p;
	char *name = NULL;

	if (configs->ap_name[0] != '\0') {
		name = &configs->ap_name[0];
	}

	actv_pool = hm_ucap_new_actvpool(name);
	if (IS_REF_ERR(actv_pool)) {
		hm_error("create activation pool failed, ap=%s, err=%s\n",
			 configs->ap_name, hmstrerror(REF_TO_ERR(actv_pool)));
	} else {
		if (p->bind_func != NULL) {
			p->bind_func(actv_pool);
		} else {
			err = hm_actv_pool_bind_sysmgr(actv_pool, 0, &sysmgr_bind_ret);
			if (err < 0) {
				hm_error("bind activation pool to sysmgr failed, ap=%s, err=%s\n",
					 configs->ap_name, hmstrerror(err));
			}
		}

		if (err == E_HM_OK) {
			err = hm_actv_pool_emrg_init(actv_pool, p->p_emrg_attr);
			if (err < 0) {
				hm_error("emrg init failed, err=%s, ap=%s\n",
					 hmstrerror(err), configs->ap_name);
			}
		}
#ifdef CONFIG_PAC
		if (err == E_HM_OK) {
			err = hm_actv_pool_pac_update(actv_pool);
			if (err < 0) {
				hm_error("pac update failed, err=%s, ap=%s\n",
					 hmstrerror(err), configs->ap_name);
			}
		}
#endif
		if (err == E_HM_OK && p->sched_info != NULL) {
			err = hm_actv_pool_set_schedinfo(actv_pool, p->sched_info, p->flags);
			if (err < 0) {
				hm_error("actv_pool set affinity failed, err = %s\n", hmstrerror(err));
			}
		}
	}

	if (err < 0) {
		(void)hm_ucap_delete_actvpool(actv_pool);
	}

	return ((err < 0) ? ERR_TO_REF(err) : actv_pool);
}

static int actvpool_path_add_grant(const struct actvpool_configs *configs, cref_t actvpool)
{
	int err = E_HM_OK;
	struct actvpool_private *p = configs->p;
	unsigned long grant_info;

	if (configs->path_name[0] != '\0') {
		if (p->path_set_grant_info) {
			grant_info = p->path_grant_info;
		} else {
			grant_info = 0UL;
		}

		if (p->path_add_with_mode) {
			err = hm_path_add_with_mode(actvpool, grant_info, &configs->path_name[0],
						    p->path_mode);
		} else {
			err = hm_path_add(actvpool, grant_info, &configs->path_name[0]);
		}

		if (err < 0) {
			hm_error("add path failed, err=%s, path=%s\n",
				 hmstrerror(err), configs->path_name);
		}

		if (err == E_HM_OK && p->path_grant2sysmgr) {
			err = hm_path_grant(&configs->path_name[0]);
			if (err < 0) {
				hm_error("grant path failed, err=%s, path=%s\n",
					 hmstrerror(err), configs->path_name);
				(void)hm_path_delete(&configs->path_name[0]);
			}
		}
	}

	return err;
}

static int check_actvpool_configs(const struct actvpool_configs *configs)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL ||
	    configs->p->p_actv_attr == NULL ||
	    configs->p->p_emrg_attr == NULL ||
	    configs->actv_init_num > configs->actv_max_num) {
		err = E_HM_INVAL;
	}

	return err;
}

static void update_actvpool_configs(struct actvpool_configs *configs)
{
	int err;
	struct __sysctrl_actv_pool_config conf;

	err = hm_sysctrl_actv_pool_conf_of(&configs->ap_name[0], (unsigned int)strlen(configs->ap_name), &conf);
	if ((err < 0) && (err != E_HM_POSIX_NOENT)) {
		hm_error("get actv pool %s config failed. err:%s\n", configs->ap_name,
			 hmstrerror(err));
	}

	if (err == E_HM_OK) {
		configs->actv_init_num	= conf.init_num;
		configs->actv_max_num	= conf.max_num;
		configs->p->add_water	= conf.add_water;
		configs->p->del_water	= conf.del_water;
		hm_info("update actv pool %s config succuss. (init:%u max:%u add:%u del:%u)\n",
			conf.pool_name, conf.init_num, conf.max_num, conf.add_water, conf.del_water);
	}
}

int hm_setup_actvpool(struct actvpool_configs *configs, cref_t *actvpool)
{
	int err;
	cref_t ap = 0ULL;

	err = check_actvpool_configs(configs);
	if (err == E_HM_OK && actvpool == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		/*
		 * (1) Currently, only the kirin9000 scene needs to fetch and
		 *     update the actv pool config;
		 * (2) Fetch the actv pool config from the static storage data
		 *     temporarily. After the subsequent functions of ctrlmem
		 *     sysconf and sysmgr analysis dtb are completed, consider
		 *     reading the actv pool config from dtb.
		 *
		 * The configs include:
		 *     actv_init_num/actv_max_num/add_water/del_water.
		 */
		update_actvpool_configs(configs);

		ap = setup_actvpool(configs);
		if (IS_REF_ERR(ap)) {
			err = REF_TO_ERR(ap);
		} else {
			/*
			 * Set the value to the output args before
			 * add/grant path. Otherwise, invalid golal
			 * actvpool cref would be used.
			 */
			*actvpool = ap;
		}
	}

	if (err == E_HM_OK) {
		err = setup_pure_actvs(configs, ap);
		if (err < 0) {
			hm_error("setup pure actvs failed, err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
			(void)hm_ucap_delete_actvpool(ap);
		}
	}

	if (err == E_HM_OK) {
		err = actvpool_path_add_grant(configs, ap);
		if (err < 0) {
			hm_error("path add or grant failed, err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
			(void)hm_ucap_delete_actvpool(ap);
		}
	}

	if (err == E_HM_OK) {
		hm_info("setup actvpool [%s] success\n", configs->ap_name);
	}

	return err;
}

static void handle_elastic_event(const struct actvpool_private *p)
{
	int err;

	err = sysif_actv_pool_handle_elastic_pure(p->p_actv_attr);
	if (err < 0) {
		hm_warn_ratelimited("handle elastic event failed, err=%s\n", hmstrerror(err));
	}
}

static void *listen_actvpool(void *args)
{
	int err;
	char thread_name[HMCAP_THREAD_NAME_SIZE] = {0};
	struct actvpool_private *p = ptr_to_type(args, struct actvpool_private *);
	struct actv_pure_attr *p_actv_attr = p->p_actv_attr;

	/* update the link thread be raw thread of trigger thread */
	p_actv_attr->link_thread = raw_thread_self();

	err = hm_actv_pool_listen_water(p_actv_attr->actv_pool);
	if (err < 0) {
		hm_error("listen water failed, err=%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = snprintf_s(thread_name, HMCAP_THREAD_NAME_SIZE, HMCAP_THREAD_NAME_SIZE - 1,
				 "%s-trig", p->elastic_prefix);
		BUG_ON(err < 0);
		err = hm_thread_setname(hm_thread_self_cref(), &thread_name[0], NULL);
		BUG_ON(err != E_HM_OK);
		hm_info("start listen thread %s success\n", thread_name);
	}

	if (err == E_HM_OK) {
		do {
			err = hm_trigger_wait(__TRIGGER_WAIT_DONE, NULL);
			if (err == ACTV_POOL_WATER_TRIGGER) {
				handle_elastic_event(p);
			}
		/*
		 * when ret is non negative or is E_HM_HMSIGINTR, which
		 * means block state may be interrupted by hmsignal, continue
		 * to trigger wait for next elastic event.
		 */
		} while ((err >= 0) || (err == E_HM_HMSIGINTR));
	}

	hm_panic("cannot handle elastic event, err=%s, thread=%s-trig\n",
		  hmstrerror(err), p->elastic_prefix);

	return NULL;
}

static int
setup_actvpool_elastic_new_thread(const struct actvpool_configs *configs)
{
	int err;
	unsigned int priority = (unsigned int)hm_thread_sched_getdefaultpriority();
	int policy = hm_thread_sched_getdefaultscheduler();
	raw_thread_attr_t listen_thread_attr;
	raw_thread_t listen_thread = NULL;
	struct raw_sched_param param;
	struct actvpool_private *p = configs->p;
	struct __actv_pool_water actv_water = {
		.ready_add_water = p->add_water,
		.ready_del_water = p->del_water,
	};

	if (policy == SCHED_RR) {
		priority = HM_PRIORITY_ELASTIC;
	}
	err = raw_thread_attr_init(&listen_thread_attr);
	BUG_ON(err != 0);
	err = raw_thread_attr_setflags(&listen_thread_attr, THREAD_CONFIG_FLAGS_MASK_CREATE_PRE_ACTV);
	BUG_ON(err != 0);
	err = raw_thread_attr_setinheritsched(&listen_thread_attr, RAW_THREAD_EXPLICIT_SCHED);
	BUG_ON(err != 0);
	err = raw_thread_attr_setschedpolicy(&listen_thread_attr, policy);
	BUG_ON(err != 0);
	mem_zero_s(param);
	param.priority = priority;
	err = raw_thread_attr_setschedparam(&listen_thread_attr, &param);
	BUG_ON(err != 0);

	/* set trigger water */
	err = hm_actv_pool_set_water(p->p_actv_attr->actv_pool, &actv_water);
	if (err < 0) {
		hm_error("actv pool set water failed, err=%s, ap=%s\n",
			  hmstrerror(err), configs->ap_name);
	}

	/* created listen thread */
	if (err == E_HM_OK) {
		err = raw_thread_create(&listen_thread, &listen_thread_attr, listen_actvpool, (void *)p);
		if (err < 0) {
			hm_error("create listen thread failed, err=%s, ap=%s\n",
				  hmstrerror(err), configs->ap_name);
		}
	}

	return err;
}

static int setup_actvpool_elastic(const struct actvpool_configs *configs)
{
	int err;
	int priority = hm_thread_sched_getdefaultpriority();
	int policy = hm_thread_sched_getdefaultscheduler();
	struct actvpool_private *p = configs->p;
	struct actv_pure_attr *p_actv_attr = p->p_actv_attr;
	struct __actv_pool_water actv_water = {
		.ready_add_water = p->add_water,
		.ready_del_water = p->del_water,
	};
	struct sched_param param_user;

	if (policy == SCHED_RR) {
		priority = HM_PRIORITY_ELASTIC;
	}
	/* update the link thread be raw thread of trigger thread */
	p_actv_attr->link_thread = raw_thread_self();

	err = hm_actv_pool_set_water(p_actv_attr->actv_pool, &actv_water);
	if (err < 0) {
		hm_error("actv pool set water failed, err=%s, ap=%s\n",
			 hmstrerror(err), configs->ap_name);
	}

	if (err == E_HM_OK) {
		err = hm_actv_pool_listen_water(p_actv_attr->actv_pool);
		if (err < 0) {
			hm_error("actv pool listen water failed, err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
		}
	}

	/* set priority be HM_PRIORITY_ELASTIC for the listen thread */
	if (err == E_HM_OK) {
		param_user.sched_priority = priority;
		err = hm_thread_sched_setscheduler(0, policy, &param_user);
		if (err < 0) {
			hm_error("set priority failed, err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
		}
	}

	if (err == E_HM_OK) {
		do {
			err = hm_trigger_wait(__TRIGGER_WAIT_DONE, NULL);
			if (err == ACTV_POOL_WATER_TRIGGER) {
				handle_elastic_event(p);
			}
		/*
		 * when err is non negative or is E_HM_HMSIGINTR, which
		 * means block state may be interrupted by hmsignal, continue
		 * to trigger wait for next elastic event.
		 */
		} while ((err >= 0) || (err == E_HM_HMSIGINTR));
	}

	hm_panic("actvpool [%s] cannot handle elastic event, err=%s\n",
		 configs->ap_name, hmstrerror(err));

	return err;
}

static int setup_share_trigger_workqueue(const struct actvpool_configs *configs)
{
	int err = E_HM_OK;
	struct actvpool_private *p = configs->p;
	struct actv_pure_attr *p_actv_attr = p->p_actv_attr;

	if (p->share_trig_name[0] != '\0') {
		mem_zero_s(*p->share_trig_wq);
		err = hm_actv_pure_trig_workqueue_init(p->share_trig_wq, &p->share_trig_name[0]);
		if (err < 0) {
			hm_error("trig workqueue init failed: err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
		}
	}
	p_actv_attr->trig_wq = p->share_trig_wq;

	return err;
}

static int setup_trigger_workqueue(const struct actvpool_configs *configs)
{
	int ret;
	int err = E_HM_OK;
	struct actvpool_private *p = configs->p;
	struct actv_pure_attr *p_actv_attr = p->p_actv_attr;
	char trig_wq_name[ACTVPOOL_NAME_LENGTH_MAX];

	/* build trigger wq name */
	mem_zero_a(trig_wq_name);
	ret = snprintf_s(&trig_wq_name[0], sizeof(trig_wq_name), sizeof(trig_wq_name) - 1U,
			 "%s-t-wq", p->elastic_prefix);
	if (ret <= 0) {
		hm_error("build trigger wq name failed, ret=%d, ap=%s\n",
			 ret, configs->ap_name);
		err = E_HM_OVERFLOW;
	}

	if (err == E_HM_OK) {
		p_actv_attr->trig_wq = (struct raw_workqueue *)malloc(sizeof(struct raw_workqueue));
		if (p_actv_attr->trig_wq == NULL) {
			hm_error("alloc trig workqueue mem failed\n");
			err = E_HM_NOMEM;
		} else {
			mem_zero_s(*p_actv_attr->trig_wq);
		}
	}

	if (err == E_HM_OK) {
		err = hm_actv_pure_trig_workqueue_init(p_actv_attr->trig_wq, &trig_wq_name[0]);
		if (err < 0) {
			hm_error("trig workqueue init failed: err=%s, ap=%s\n",
				 hmstrerror(err), configs->ap_name);
		}
	}

	return err;
}

static int
check_actvpool_elastic_configs(const struct actvpool_configs *configs)
{
	int err = E_HM_OK;

	if (configs == NULL || configs->p == NULL ||
	    configs->p->p_actv_attr == NULL ||
	    configs->p->elastic_prefix[0] == '\0' ||
	    configs->p->add_water >= configs->p->del_water) {
		err = E_HM_INVAL;
	}

	return err;
}

static void
free_trigger_workqueue_mem(const struct actvpool_configs *configs)
{
	if (configs != NULL && configs->p != NULL &&
	    configs->p->p_actv_attr != NULL &&
	    configs->p->p_actv_attr->trig_wq != NULL) {
		if (configs->p->share_trig_wq == NULL) {
			free(configs->p->p_actv_attr->trig_wq);
		}
		configs->p->p_actv_attr->trig_wq = NULL;
	}
}

int hm_actvpool_setup_elastic(const struct actvpool_configs *configs)
{
	int err;

	err = check_actvpool_elastic_configs(configs);
	if (err == E_HM_OK) {
		if (configs->p->share_trig_wq != NULL) {
			err = setup_share_trigger_workqueue(configs);
		} else {
			err = setup_trigger_workqueue(configs);
		}
	}

	if (err == E_HM_OK) {
		if (configs->p->main_thread_elastic) {
			err = setup_actvpool_elastic(configs);
		} else {
			err = setup_actvpool_elastic_new_thread(configs);
		}
	}

	if (err < 0) {
		/* release trig_wq resource alloced in this function */
		free_trigger_workqueue_mem(configs);
	}

	return err;
}
