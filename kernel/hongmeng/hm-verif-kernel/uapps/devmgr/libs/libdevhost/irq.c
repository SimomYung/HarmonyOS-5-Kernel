/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of libdh irq
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 17 17:31:59 2020
 */
#include <libdevhost/irq.h>

#include <string.h>
#include <lib/utils.h>
#include <libalgo/idr.h>
#include <hmkernel/sched/sched.h>
#include <hmkernel/sched/priority.h>
#include <hongmeng/errno.h>
#include <libhmirq/irq.h>
#include <libhmirq/intr.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_scopedptr.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_hmsignal.h>
#include <libdevhost/devhost.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/utils_server.h>
#include <libsysif/devhost/server.h>

struct irq_info {
	unsigned int logic_irq;
	libdh_irq_handler_t handler;
	void *args;
	char name[IRQCTRL_NAME_LEN];
	struct raw_mutex lock;
};

/* record irq mapping in libdh for irq fastpath */
static DEFINE_IDR(linux_irqs);

#define MAX_SERVER_NAME_SIZE 16
struct libdh_irq_server {
	cref_t cref;
	int repeat;
	char *name;
	raw_thread_t thread;
	__cpuset_t affinity;
	struct idr irqs;
	struct raw_mutex lock; /* lock for irq_info lock */
};

static void *__libdh_handle_irq(void *args)
{
	int err = 0;
	unsigned int logic_irq = 0;
	struct irq_info *info = NULL;
	libdh_irq_server_t server = (libdh_irq_server_t)args;

	if (!__cpuset_is_empty(server->affinity)) {
		/* create config_cpu_nr irq-threads. if cpu offline, set affinity allow failed */
		(void)hm_thread_sched_setaffinity(0, sizeof(__cpuset_t), &server->affinity);
	}
	while (server->repeat == 1) {
		err = hm_irq_wait_for_irq();
		if (err < 0) {
			hm_debug("libdh: wait for irq return %s\n", hmstrerror(err));
			continue;
		}

		logic_irq = (unsigned int)err;

		raw_mutex_lock(&server->lock);
		info = idr_find(&server->irqs, logic_irq);
		if (info == NULL) {
			raw_mutex_unlock(&server->lock);
			hm_warn("libdh: irq handler not found for irq=%u\n",
				logic_irq);
			continue;
		}

		/* pass global lock to per entry lock */
		RAW_MUTEX_GUARD(_, &info->lock);
		raw_mutex_unlock(&server->lock);

		BUG_ON(info->handler == NULL);
		info->handler((int)info->logic_irq, info->args);
	}
	/*
	 * return is reachable only if destroy server is called
	 * we need to unlock server, so destroy server will continue
	 */
	hm_info("libdh: irq server exit, name=%s\n", server->name);
	return NULL;
}

static libdh_irq_server_t __libdh_irq_alloc_server(const char *server_name, __cpuset_t affinity)
{
	libdh_irq_server_t irq_server = (libdh_irq_server_t)malloc(sizeof(*irq_server));
	if (irq_server == NULL) {
		return NULL;
	}
	irq_server->repeat = 1;
	irq_server->name = strndup(server_name, MAX_SERVER_NAME_SIZE - 1);
	irq_server->affinity = affinity;
	if (irq_server->name == NULL) {
		free(irq_server);
		return NULL;
	}

	idr_init(&irq_server->irqs);
	raw_mutex_init(&irq_server->lock);
	return irq_server;
}

static void __libdh_irq_free_server(libdh_irq_server_t server)
{
	idr_free(&server->irqs);
	free(server->name);
	free(server);
}

static int prepare_irq_server_attr(raw_thread_attr_t *attr, int prio)
{
	int err = E_HM_OK;
	struct raw_sched_param param;

	err = raw_thread_attr_init(attr);
	if (err < 0) {
		hm_error("libdh: failed to init thread attr, err=%s\n",
			 hmstrerror(err));
		return err;
	}
	err = raw_thread_attr_setinheritsched(attr, RAW_THREAD_EXPLICIT_SCHED);
	if (err < 0) {
		hm_error("libdh: failed to set thread attr inherit, err=%s\n",
			 hmstrerror(err));
		return err;
	}
	err = raw_thread_attr_setschedpolicy(attr, SCHED_RR);
	if (err < 0) {
		hm_error("libdh: failed to set thread attr policy, err=%s\n",
			 hmstrerror(err));
		return err;
	}
	mem_zero_s(param);
	param.priority = (unsigned int)prio;
	err = raw_thread_attr_setschedparam(attr, &param);
	if (err < 0) {
		hm_error("libdh: failed to set thread param, err=%s\n",
			 hmstrerror(err));
		return err;
	}

	return err;
}

static struct irq_fastpath_res_s irq_fastpath_res = {
	.irq_actv_pool = (cref_t)0ULL,
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

DECLARE_UPCALL_ACTVHDLR(handle_irq_fastpath);
DEFINE_UPCALL_ACTVHDLR(handle_irq_fastpath)
{
	UNUSED(credential);
	unsigned int logic_irq = args.irq.logic_irq;
	struct irq_info *info = NULL;

	info = idr_find(&linux_irqs, logic_irq);
	if (info == NULL || info->handler == NULL) {
		hm_warn("libdh: irq handler not found for irq=%u\n",
			logic_irq);
	} else {
		info->handler((int)info->logic_irq, info->args);
	}

	return logic_irq;
}

static int irq_fastpath_init(void)
{
	return E_HM_OK;
}

static int __irq_create_fastpath(void)
{
	int err = E_HM_OK;
	unsigned long config_cpu_nr = 0UL;

	err = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &config_cpu_nr,
				   sizeof(config_cpu_nr));
	if (err < 0) {
		hm_info("libdh get config cpu nr failed: %s\n", hmstrerror(err));
	} else {
		/* Only an even nr of actvs are supported */
		unsigned long irq_actv_nr	= (((config_cpu_nr + 1UL) >> 1) << 1);
		irq_ap_cfg.actv_init_num	= irq_actv_nr;
		irq_ap_cfg.actv_max_num		= irq_actv_nr;

		mem_zero_s(irq_fastpath_res);
		err = hm_irq_create_fastpath(&irq_ap_cfg, &irq_fastpath_res,
					     irq_fastpath_init);
	}

	return err;
}

static void libdh_ex_actvhandler_init_offset(unsigned int offset)
{
	union devhost_hdlrs_union *dh_table = &sysif_devhost_actvcapcallhdlr_table[__ACTV_UPCALL_IRQ_FASTPATH + offset];
	dh_table->p_sysif_handle_kcall_entry = handle_irq_fastpath;
}

static void libdh_ex_actvhandler_init(void)
{
	for (unsigned int i = 0; i < __devhost_method_TIMES; i++) {
		libdh_ex_actvhandler_init_offset(i * __devhost_method_MAX);
	}
}

int libdh_irq_ap_table_init(void)
{
	libdh_ex_actvhandler_init();
	return 0;
}

int libdh_irq_create_fastpath(void)
{
	int err = E_HM_OK;

	err = __irq_create_fastpath();
	if (err < 0) {
		hm_info("libdh irq create irq fastpath failed: %s\n", hmstrerror(err));
	}

	return err;
}

int libdh_irq_create_server(const char *server_name, int priority,
			    __cpuset_t affinity, libdh_irq_server_t *server /* out */)
{
	raw_thread_attr_t attr;
	raw_thread_t thread = NULL;
	libdh_irq_server_t irq_server = NULL;
	int err = 0;
	bool ref_err;

	if (server == NULL || server_name == NULL) {
		return E_HM_INVAL;
	}

	err = prepare_irq_server_attr(&attr, priority);
	if (err < 0) {
		return err;
	}

	irq_server = __libdh_irq_alloc_server(server_name, affinity);
	if (irq_server == NULL) {
		return E_HM_NOMEM;
	}

	/* setup irq server */
	err = raw_thread_create(&thread, &attr, __libdh_handle_irq, irq_server);
	if (err < 0) {
		hm_error("libdh: failed to create irq server, err=%s\n",
			 hmstrerror(err));
		__libdh_irq_free_server(irq_server);
		return err;
	}

	irq_server->thread = thread;
	irq_server->cref = raw_thread_cref_of(thread);
	ref_err = IS_REF_ERR(irq_server->cref);
	BUG_ON(ref_err);

	/* set thread name */
	err = hm_thread_setname(irq_server->cref, server_name, NULL);
	if (err < 0) {
		hm_warn("libdh: set irq thread name failed, err=%s\n",
			hmstrerror(err));
	}

	*server = irq_server;

	return E_HM_OK;
}

int libdh_irq_destroy_server(libdh_irq_server_t server)
{
	int ret = 0;

	if (server == NULL) {
		return E_HM_INVAL;
	}

	server->repeat = 0;
	/*
	 * irq handler thread is blocked by hm_irq_wait_for_irq,
	 * kill the thread so hm_irq_wait_for_irq return E_HM_HMSIGINTR
	 * and the thread will finish
	 */
	ret = syscap_ThreadHMSignalKill(server->cref, (unsigned int)HMSIGNAL_NONMASKABLE);
	if (ret < 0) {
		hm_error("libdh: kill irq server failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
	/* wait for server thread exit */
	ret = raw_thread_join(server->thread, NULL);
	if (ret < 0) {
		return ret;
	}
	__libdh_irq_free_server(server);

	return E_HM_OK;
}

/*
 * Only support single-core irq mapping.
 * When smp is enabled, we need allocate per-cpu memory to
 * save logic_irq and rref, each virt_irq may be mapped with
 * more than one logic_irqs.
 */
#define CPUID_0		0

struct libdh_irq_ctx {
	rref_t rref;
	unsigned int logic_irq;
	unsigned int affinity;
};

int libdh_irq_xlate_ex(int irqctrl_id, const unsigned int *hwconf,
		       unsigned int hwconf_len, unsigned int *logic_irq)
{
	int ret = 0;

	if (logic_irq == NULL) {
		return E_HM_INVAL;
	}

	/* hwconf and hwconf_len is validated inside */
	ret = hm_irq_conf_to_irq(irqctrl_id, hwconf, hwconf_len, CPUID_0);
	if (ret < 0) {
		hm_error("libdh: xlate to logic irq failed, err=%s\n",
			 hmstrerror(ret));
		return ret;
	}

	*logic_irq = (unsigned int)ret;

	return E_HM_OK;
}

static int __register_irq_server(const struct libdh_irq_ctx *ctx, const char *name,
				 const struct libdh_irq_server *server)
{
	/*
	 * 1. some driver must do something before enable irq.
	 * 2. mark irq as staywake when suspend in order to do not
	 *    disable when call irqctrl_suspend, beacuse we disbale
	 *    the irq in liblinux directly.
	 */
	struct hm_irq_register_setting setting = {
		.flags = IRQCTRL_REGISTER_STAYWAKE,
		.name = name,
		/* register to all cpus */
		.cpu_mask = __cpuset_full,
		.irq_fastpath_ap = irq_fastpath_res.irq_actv_pool,
		.is_fp_bypass = 1U,
	};
	/*
	 * If we specify server affinity, it means we want to setup percpu
	 * dh-irq thread, global no-bind server is default.
	 */
	if (!__cpuset_is_empty(server->affinity)) {
		setting.cpu_mask = server->affinity;
	}

	return hm_irq_register(ctx->rref, ctx->logic_irq, &setting,
			       server->cref);
}

static struct irq_info *__alloc_irq_info(unsigned int logic_irq)
{
	struct irq_info *info = NULL;

	info = (struct irq_info *)malloc(sizeof(*info));
	if (info == NULL) {
		hm_error("libdh: alloc irq_info failed, size=%zu, logic_irq=%u\n",
			 sizeof(struct irq_info), logic_irq);
	} else {
		info->logic_irq = logic_irq;
		raw_mutex_init(&info->lock);
	}

	return info;
}

static int __set_irq_affinity(const struct libdh_irq_ctx *ctx, unsigned int affinity)
{
	/* no need to set affinity */
	if (affinity == 0U) {
		return 0;
	}
	struct hm_irq_attribute irq_attr = {0};
	irq_attr.attr_cpu = affinity;

	return hm_irq_set_attribute(ctx->rref, ctx->logic_irq, &irq_attr,
				    IRQ_SET_ATTRIBUTE_CPU);
}

static int __create_irq_info(const struct libdh_irq_ctx *ctx,
			     libdh_irq_server_t server,
			     const struct libdh_irq_setup_args *arg)
{
	int ret = 0;
	struct irq_info *info = NULL;

	/* allocate new info to record this */
	info = __alloc_irq_info(ctx->logic_irq);
	if (info == NULL) {
		return E_HM_INVAL;
	}

	info->args = arg->args;
	info->handler = arg->handler;
	NOFAIL(strcpy_s(info->name, sizeof(info->name), arg->name));

	/*
	 * to prevent register after remove we can lock info only
	 * however, to prevent double free, we must lock server
	 */
	RAW_MUTEX_GUARD(_, &server->lock);
	ret = idr_set(&server->irqs, info, ctx->logic_irq);
	if (ret < 0) {
		free(info);
		if (ret == E_HM_OBJEXIST) {
			ret = E_HM_POSIX_EXIST;
		}
		return ret;
	}
	ret = __set_irq_affinity(ctx, arg->affinity);
	if (ret < 0) {
		hm_error("libdh: irq affinity set failed, logic_irq=%u, err=%s\n",
			 ctx->logic_irq, hmstrerror(ret));
		(void)idr_remove(&server->irqs, ctx->logic_irq);
		free(info);
		return ret;
	}

	ret = __register_irq_server(ctx, arg->name, server);
	if (ret < 0) {
		hm_error("libdh: irq register failed, logic_irq=%u, err=%s\n",
			 ctx->logic_irq, hmstrerror(ret));
		(void)idr_remove(&server->irqs, ctx->logic_irq);
		free(info);
		return ret;
	}

	if (ret == E_HM_OK) {
		(void)idr_set(&linux_irqs, info, ctx->logic_irq);
	}

	return ret;
}

static rref_t libdh_irq_fetch(unsigned int linux_irq)
{
	struct drvcall_devmgr_irq_fetch_arg arg;
	struct __actvret_drvcall_devmgr_irq_fetch ret;
	rref_t rref;
	int err = E_HM_OK;

	rref = hm_ucap_require(0, libdh_get_devmgr_cnode_idx(), 0);
	if (IS_REF_ERR(rref)) {
		hm_error("create rref failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(rref)));
		return rref;
	}

	mem_zero_s(arg);
	mem_zero_s(ret);
	/* fetch info of linux_irq from devmgr */
	arg.rrefs_nr = 1;
	arg.virt_irq = linux_irq;
	arg.irqctrl_rrefs[0] = rref;
	arg.is_sgi = false;
	arg.cpu = 0U;

	err = actvcapcall_drvcall_devmgr_irq_fetch(libdh_get_devmgr_ap(), arg, &ret);
	if (err < 0) {
		(void)hm_ucap_reject(0, rref);
		return ERR_TO_REF(err);
	}

	return rref;
}

static libdh_irq_ctx_t __create_irq_ctx(const struct libdh_irq_setup_args *arg)
{
	libdh_irq_ctx_t ctx = NULL;

	ctx = (libdh_irq_ctx_t)malloc(sizeof(*ctx));
	if (ctx == NULL) {
		return NULL;
	}

	ctx->rref = hm_irq_require_grant(arg->irqctrl_id, arg->logic_irq);
	if (IS_REF_ERR(ctx->rref) && (arg->linux_irq != 0U)) {
		/* fallback to devmgr */
		ctx->rref = libdh_irq_fetch(arg->linux_irq);
	}
	if (IS_REF_ERR(ctx->rref)) {
		hm_error("libdh: require grant failed, logic_irq=%u\n", arg->logic_irq);
		free(ctx);
		return NULL;
	}

	ctx->logic_irq = arg->logic_irq;
	ctx->affinity = arg->affinity;

	return ctx;
}

static int __libdh_irq_setup(libdh_irq_server_t *servers, unsigned int server_cnt,
		      const struct libdh_irq_setup_args *arg,
		      libdh_irq_ctx_t *ctx_out)
{
	int ret = 0;
	unsigned int si = 0;
	libdh_irq_ctx_t ctx = NULL;

	if ((servers == NULL) || (arg == NULL) || (ctx_out == NULL) ||
	    (arg->name == NULL) || (arg->handler == NULL)) {
		return E_HM_INVAL;
	}

	if (strnlen(arg->name, IRQCTRL_NAME_LEN) == IRQCTRL_NAME_LEN) {
		hm_error("libdh: name is invalid\n");
		return E_HM_INVAL;
	}

	for (si = 0; si < server_cnt; ++si) {
		if ((servers[si] == NULL) || (IS_REF_ERR(servers[si]->cref))) {
			hm_error("libdh: irq_server is invalid\n");
			return E_HM_INVAL;
		}
	}
	ctx = __create_irq_ctx(arg);
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	for (si = 0U; si < server_cnt; ++si) {
		ret = __create_irq_info(ctx, servers[si], arg);
		if (ret < 0) {
			(void)libdh_irq_close_bind(servers, server_cnt, ctx);
			return ret;
		}
	}

	/* return ctx */
	*ctx_out = ctx;

	return E_HM_OK;
}

/*
 * Register irq to global irq thread
 */
int libdh_irq_setup(libdh_irq_server_t server,
		    const struct libdh_irq_setup_args *arg,
		    libdh_irq_ctx_t *ctx_out)
{
	libdh_irq_server_t servers[] = { server };

	return __libdh_irq_setup(servers, (unsigned int)ARRAY_SIZE(servers), arg, ctx_out);
}

/*
 * Register irq to percpu irq thread,
 * caller needs to make sure that the interface is called only once
 * and make sure sum of servers affinity is equals to config cpu.
 */
int libdh_irq_setup_bind(libdh_irq_server_t *server, unsigned int server_cnt,
			 const struct libdh_irq_setup_args *arg,
			 libdh_irq_ctx_t *ctx_out)
{
	return __libdh_irq_setup(server, server_cnt, arg, ctx_out);
}

static int __libdh_irq_close(libdh_irq_server_t *servers, unsigned int server_cnt,
		      libdh_irq_ctx_t ctx)
{
	int ret = 0;
	unsigned int si;
	libdh_irq_server_t server;
	struct irq_info *info = NULL;

	if (servers == NULL || ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_irq_unregister(ctx->rref, ctx->logic_irq);
	if (ret < 0) {
		hm_error("libdh: unregister irq failed, logic_irq=%u, err=%s\n",
			 ctx->logic_irq, hmstrerror(ret));
		free(ctx);
		return ret;
	}

	ret = hm_irq_reject_grant(ctx->rref);
	if (ret < 0) {
		hm_error("libdh: reject grant failed, logic_irq=%u, err=%s\n",
			 ctx->logic_irq, hmstrerror(ret));
		free(ctx);
		return ret;
	}

	for (si = 0U; si < server_cnt; ++si) {
		server = servers[si];
		if (server == NULL) {
			continue;
		}
		/* remove ctx from idr first, then do cleanup */
		raw_mutex_lock(&server->lock);
		info = idr_remove(&server->irqs, ctx->logic_irq);
		if (info == NULL) {
			hm_error("libdh: no irqinfo, logic_irq=%u\n", ctx->logic_irq);
			raw_mutex_unlock(&server->lock);
			continue;
		}
		/* pass global lock to per entry lock */
		raw_mutex_lock(&info->lock);
		raw_mutex_unlock(&server->lock);

		/* clean and free resources */
		info->handler = NULL; /* not mendatory */
		raw_mutex_unlock(&info->lock);
		free(info);
	}

	if (ret == E_HM_OK) {
		(void)idr_remove(&linux_irqs, ctx->logic_irq);
	}

	free(ctx);

	return E_HM_OK;
}

int libdh_irq_close(libdh_irq_server_t server, libdh_irq_ctx_t ctx)
{
	libdh_irq_server_t servers[] = { server };

	return __libdh_irq_close(servers, (unsigned int)ARRAY_SIZE(servers), ctx);
}

/*
 * Close irq, caller needs to the interface is called only once.
 * 1. reject irq grant
 * 2. recycle servers idr resources.
 */
int libdh_irq_close_bind(libdh_irq_server_t *server, unsigned int server_cnt,
			 libdh_irq_ctx_t ctx)
{
	return __libdh_irq_close(server, server_cnt, ctx);
}

int libdh_irq_enable(const struct libdh_irq_ctx *ctx)
{
	int ret = 0;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_irq_enable(ctx->rref, ctx->logic_irq);
	if (ret < 0) {
		hm_error("libdh: enable irq failed, logic_irq=%u, err=%s\n",
			 ctx->logic_irq, hmstrerror(ret));
	}
	return ret;
}

int libdh_irq_disable(const struct libdh_irq_ctx *ctx)
{
	int ret = 0;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_irq_disable(ctx->rref, ctx->logic_irq);
	if (ret < 0) {
		hm_error("libdh: disable irq failed, logic_irq=%u, err=%s\n",
			 ctx->logic_irq, hmstrerror(ret));
	}
	return ret;
}

int libdh_irq_setaffinity(struct libdh_irq_ctx *ctx, unsigned long affinity)
{
	int ret = 0;

	ret = __set_irq_affinity(ctx, affinity);
	if (ret < 0) {
		hm_debug("libdh: irq setaffinity failed, logic_irq=%u, affinity=0x%lx, err=%s\n",
			 ctx->logic_irq, affinity, hmstrerror(ret));
		return ret;
	}
	ctx->affinity = (unsigned int)affinity;

	return ret;
}

int libdh_irq_getaffinity(const struct libdh_irq_ctx *ctx, unsigned long *affinity)
{
	if (ctx == NULL || affinity == NULL) {
		return E_HM_INVAL;
	}
	*affinity = ctx->affinity;

	return 0;
}
