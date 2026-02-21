/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost backend APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#include <lnxbase/lnxbase.h>

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libdevhost/devhost.h>
#include <devhost/memory.h>
#include <devhost/backend.h>
#include <devhost/netlink.h>
#include <lnxbase/ksymtab.h>
#include <liblinux/pal.h>
#include <libsec/cred.h>
#include <devhost/log.h>
#include "internal.h"

static const struct lnxbase_ctx *g_default_ctx = NULL;
/*
 * declare lnxbase syminfo
 */
static const DECLARE_LIBLINUX_SYMINFO(__lnxbase_si);

static int load_extensions(struct lnxbase_ctx *ctx)
{
	int ret;
	char *exts = lnxbase_backend_args()->extensions;
	char *ext = NULL;
	char *buf = NULL;

	if ((exts == NULL) || (exts[0] == '\0')) {
		return E_HM_OK;
	}

	buf = strdup(exts);
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	exts = buf;
	while ((ext = strsep(&exts, ":")) != NULL) {
		ret = lnxbase_load_extension(ctx, ext);
		if (ret < 0) {
			dh_info("load extension %s failed\n", ext);
		}
	}

	free(buf);
	return E_HM_OK;
}

static int __load_extra_modules(struct lnxbase_ctx *ctx)
{
	int ret;

	/* load lnxbase/devhost symbols */
	ret = lnxbase_load_symbols(ctx, "libdh-lnxbase.so", &__lnxbase_si);
	if (ret < 0) {
		dh_error("lnxbase: load symbols to backend failed, ret=%d\n", ret);
		return posix2hmerrno(-ret);
	}

	/* load ksecurec.ko */
	ret = lnxbase_load_module_chk(ctx, "ksecurec.ko", "GetHwSecureCVersion");
	if (ret < 0) {
		dh_error("lnxbase: load ksecurec.ko failed, ret=%d\n", ret);
		return posix2hmerrno(-ret);
	}

	/* load devhost.ko */
	ret = lnxbase_load_module(ctx, "devhost.ko");
	if (ret < 0) {
		dh_error("lnxbase: load devhost.ko failed, ret=%d\n", ret);
		return posix2hmerrno(-ret);
	}

	/* load all backend dso extensions */
	ret = load_extensions(ctx);
	if (ret < 0) {
		dh_error("lnxbase: load extension failed, ret = %d\n", ret);
		return ret;
	}

#if defined(CONFIG_LNXBASE_MEMSTAT)
	(void)liblinux_pal_request_module("ldk_memstat.ko");
#endif
	return E_HM_OK;
}

int lnxbase_load_extra_modules(struct lnxbase_ctx *ctx)
{
	int ret;

	if (ctx == NULL) {
		return -EINVAL;
	}
	if (ctx->mod_loaded != 0) {
		return 0;
	}

	ctx->mod_loaded = 1;
	ret = __load_extra_modules(ctx);
	return -hmerrno2posix(ret);
}

static void lnxbase_rtc_init(void)
{
	if (lnxbase_hmkernel_is_support_rtc() != 0) {
		dh_debug("lnxbase: rtc init read time failed\n");
	}
}

static int __lnxbase_postinit(void *args)
{
	struct lnxbase_ctx *ctx = (struct lnxbase_ctx *)args;
	int ret = E_HM_OK;
	BUG_ON(ctx == NULL);

	/* call backend postinit second when has bootfdt */
	if (ctx->postinit_fn != NULL) {
		ret = ctx->postinit_fn(ctx->postinit_args);
		if (ret < 0) {
			dh_error("lnxbase: backend post init failed, ret=%d\n", ret);
			return posix2hmerrno(-ret);
		}
	}

	ret = lnxbase_load_extra_modules(ctx);
	if (ret != 0) {
		return ret;
	}

	lnxbase_rtc_init();

	if (lnxbase_vmallocinfo_enabled()) {
		ret = lnxbase_vmallocinfo_init();
		if (ret < 0) {
			return ret;
		}
	}

	if (ctx->time_sync_hdlr != NULL &&
	    (ctx->features & LNXBASE_FEATURE_SKIP_FIRST_TIMESYNC) == 0U) {
		(ctx->time_sync_hdlr)();
	}

	return E_HM_OK;
}

unsigned int lnxbase_current_cnode_idx(void)
{
	cref_t tcb_ref = hm_actv_local_thread_cref_of();
	if (tcb_ref != 0) {
		return __REF_CNODE_IDX_OF(tcb_ref);
	}
	return __REF_CNODE_IDX_OF(hm_thread_effective_cref());
}

void lnxbase_get_dac(struct lnxbase_cred_dac *lnxbase_cred_ids)
{
	int err;
	__u32 cnode_idx;
	struct sec_chk_cred cred_chk = {0};
	cnode_idx = lnxbase_current_cnode_idx();

	__u64 idx = 0;
	err = sysfast_miscdata_read(&idx, __MISCDATA_TCB_CRED,
			__MISCACCESS_SELF);
	if (err != E_HM_OK) {
		dh_error("failed to read tcb cred misc data\n");
		return;
	}

	struct base_cred *b_cred = (struct base_cred *)sec_cred_idx_to_ptr(idx);
	if (b_cred == NULL) {
		dh_error("failed to get base cred pointer\n");
		return;
	}
	struct shared_cred *sh_cred = (struct shared_cred *)sec_cred_idx_to_ptr(b_cred->sh_idx);
	if (sh_cred == NULL) {
		dh_error("failed to get shared cred pointer\n");
		return;
	}
	cred_chk.cnode_idx = cnode_idx;
	cred_chk.cred = sh_cred;
	cred_chk.base_cred = b_cred;
	cred_chk.cred_idx = idx;
	struct sec_cred_dac *cred_dac = dac_cred_of_sec_cred(&cred_chk);
	mode_t umask = UMASK_OF_SH_CRED(sh_cred);
	lnxbase_cred_ids->cnode_idx = cnode_idx;
	lnxbase_cred_ids->uid = cred_dac->ruid;
	lnxbase_cred_ids->gid = cred_dac->rgid;
	lnxbase_cred_ids->suid = cred_dac->suid;
	lnxbase_cred_ids->sgid = cred_dac->sgid;
	lnxbase_cred_ids->euid = cred_dac->euid;
	lnxbase_cred_ids->egid = cred_dac->egid;
	lnxbase_cred_ids->fsuid = cred_dac->fsuid;
	lnxbase_cred_ids->fsgid = cred_dac->fsgid;
	lnxbase_cred_ids->umask = (int)umask;
}

struct lnxbase_ctx *lnxbase_newctx(struct dh_backend_ctor *ctor)
{
	struct lnxbase_ctx *ctx = NULL;

	ctx = (struct lnxbase_ctx *)malloc(sizeof(struct lnxbase_ctx));
	if (ctx == NULL) {
		return NULL;
	}
	mem_zero_s(*ctx);
	dlist_init(&ctx->extensions);

	ctx->ctor = ctor;

	return ctx;
}

void lnxbase_destroy(struct lnxbase_ctx *ctx)
{
	free(ctx);
}

int lnxbase_init(struct lnxbase_ctx *ctx,
		 lnxbase_postinit_fn_t fn, void *args)
{
	int ret;

	/* check required module ops */
	if (ctx->mod_ops == NULL ||
	    ctx->mod_ops->load_module == NULL ||
	    ctx->mod_ops->symbol_get == NULL ||
	    ctx->mod_ops->load_symbol_module == NULL) {
		return -EINVAL;
	}

	ret = lnxbase_args_init();
	if (ret < 0) {
		dh_error("lnxbase: args init failed, ret = %s\n", hmstrerror(ret));
		return ret;
	}

	/* save backend postinit fn, and register devhost postinit callback */
	ctx->postinit_fn = fn;
	ctx->postinit_args = args;
	ret = devhost_postinit(__lnxbase_postinit, (void *)ctx);
	if (ret < 0) {
		dh_error("register liblinux post-init hook failed, err=%s\n",
			 hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	if ((ctx->features & LNXBASE_FEATURE_TIMESYNC) != 0U) {
		ret = devhost_time_sync_init(ctx->time_sync_hdlr);
		if (ret < 0) {
			dh_error("init settime server failed, err=%s\n", hmstrerror(ret));
			return -hmerrno2posix(ret);
		}
	}

	ret = lnxbase_cache_init();
	if (ret < 0) {
		return ret;
	}

	return 0;
}

#ifdef CONFIG_DEVHOST_NETLINK
static int __netlink_register_hdr(struct liblinux_nl_info *nl)
{
	if (nl->type == CUST_NETLINK) {
		return devhost_netlink_register(nl->cust_nl.unit, nl->cust_nl.nr_grp);
	}

	return -ENOSYS;
}

static int __netlink_unregister_hdr(struct liblinux_nl_info *nl)
{
	if (nl->type == CUST_NETLINK) {
		return devhost_netlink_unregister(nl->cust_nl.unit);
	}

	return -ENOSYS;
}
#endif

static struct netlink_pal_handler nl_pal_handler = {
#ifdef CONFIG_DEVHOST_NETLINK
	.multicast_hdr = devhost_netlink_multicast,
	.unicast_hdr = devhost_netlink_unicast,
	.genl_reg_hdr = devhost_netlink_register_genl,
	.genl_unreg_hdr = devhost_netlink_unregister_genl,
	.nl_reg_hdr = __netlink_register_hdr,
	.nl_unreg_hdr = __netlink_unregister_hdr,
#else
	.multicast_hdr = NULL,
	.unicast_hdr = NULL,
	.genl_reg_hdr = NULL,
	.genl_unreg_hdr = NULL,
	.nl_reg_hdr = NULL,
	.nl_unreg_hdr = NULL,
#endif
};

int lnxbase_enable_netlink(struct lnxbase_ctx *ctx,
			   int (*netlink_init_fn)(struct netlink_pal_handler *))
{
	int ret;

	if ((ctx == NULL) || (netlink_init_fn == NULL)) {
		return -EINVAL;
	}

	ret = netlink_init_fn(&nl_pal_handler);
	if (ret < 0) {
		return ret;
	}

	ctx->features |= LNXBASE_FEATURE_NETLINK;

	return 0;
}

int lnxbase_enable_timesync(struct lnxbase_ctx *ctx, void (*sync_hdlr)(void))
{
	if (ctx == NULL) {
		return -EINVAL;
	}

	ctx->time_sync_hdlr = (void (*)(void))sync_hdlr;
	ctx->features |= LNXBASE_FEATURE_TIMESYNC;

	return 0;
}

int lnxbase_enable_post_dev_populate(struct lnxbase_ctx *ctx,
				     lnxbase_post_populate_fn_t fn)
{
	if (ctx->post_dev_populate_fn != NULL) {
		return -EBUSY;
	}
	ctx->post_dev_populate_fn = fn;
	return 0;
}

int lnxbase_root_notify(struct lnxbase_ctx *ctx)
{
	int ret;

	if (ctx->post_dev_populate_fn != NULL) {
		ret = (ctx->post_dev_populate_fn)();
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
		/* done with this hook */
		ctx->post_dev_populate_fn = NULL;
	}

	return E_HM_OK;
}

void lnxbase_enable_device_autoprobe(struct lnxbase_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}
	ctx->features |= LNXBASE_FEATURE_DEV_AUTOPROBE;
}

int lnxbase_set_default_ctx(const struct lnxbase_ctx *ctx)
{
	if (g_default_ctx == NULL) {
		g_default_ctx = ctx;
		return 0;
	} else {
		return -hmerrno2posix(E_HM_POSIX_EXIST);
	}
}

/* get default ctx feature */
int lnxbase_support_device_autoprobe(void)
{
	if (g_default_ctx == NULL) {
		return 0;
	}
	if ((g_default_ctx->features & LNXBASE_FEATURE_DEV_AUTOPROBE) != 0) {
		return 1;
	}
	return 0;
}

void lnxbase_enable_bootfdt(struct lnxbase_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}
	ctx->features |= LNXBASE_FEATURE_BOOTFDT;
	return;
}

void *__lnxbase_acquire_bootfdt(unsigned long *size, unsigned int fwtype_mask)
{
	void *dtb = NULL;

	(void)libdh_acquire_bootfdt(&dtb, size, fwtype_mask);
	return dtb;
}

void lnxbase_enable_skip_first_timesync(struct lnxbase_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}
	ctx->features |= LNXBASE_FEATURE_SKIP_FIRST_TIMESYNC;
	return;
}
