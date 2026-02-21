/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Handler for devhost process sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 04 14:26:54 2023
 */

#include <hmasm/ucontext.h>
#include <hongmeng/errno.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/export.h>
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <libmem/umem.h>
#include <libhmactv/actv.h>
#include <libsysif/devhost/server.h>

#include "probe_api.h"

#define min(x, y) ((x) > (typeof(x))(y) ? (typeof(x))(y) : (x))

#define REGS_PSTATE (31)
#define REGS_PC (32)
#define REGS_SP (33)

static const struct devhost_plat_probe_ops *g_probe_ops = NULL;

/* Used in module */
PUBLIC_SYMBOL
int devhost_probe_init_ops(const struct devhost_plat_probe_ops *ops)
{
	BUG_ON(ops == NULL);

	g_probe_ops = ops;
	return 0;
}
EXPORT_SYMBOL(devhost_probe_init_ops);

static void copy_to_hmpt_regs(const struct arch_uctx *uctx, struct hmpt_regs *hm_regs)
{
	unsigned int size = min(sizeof(hm_regs->regs), sizeof(uctx->regs.__regs));

	memcpy_s(&(hm_regs->regs[0]), (size), &(uctx->regs.__regs[0]), (size));
	hm_regs->pc = uctx->regs.__regs[REGS_PC];
	hm_regs->sp = uctx->regs.__regs[REGS_SP];
	hm_regs->pstate = uctx->regs.__regs[REGS_PSTATE];
	hm_regs->orig_x0 = uctx->orig_r0;
}

static void copy_to_uctx(const struct hmpt_regs *hm_regs, struct arch_uctx *uctx)
{
	unsigned int size = min(sizeof(hm_regs->regs), sizeof(uctx->regs.__regs));

	memcpy_s(&(uctx->regs.__regs[0]), size, &(hm_regs->regs[0]), size);
	uctx->regs.__regs[REGS_PC] = hm_regs->pc;
	uctx->regs.__regs[REGS_SP] = hm_regs->sp;
	uctx->regs.__regs[REGS_PSTATE] = hm_regs->pstate;
	uctx->orig_r0 = hm_regs->orig_x0;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_probe_handle_brk, dh_probe_handle_brk_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct arch_uctx *, __uctx)
{
	UNUSED(sender, credential);
	int err = E_HM_NOSYS;
	struct hmpt_regs hm_regs;
	struct arch_uctx uctx = {0};

	if (g_probe_ops == NULL) {
		return err;
	}

	err = hm_copy_from_user(&uctx, __uctx, sizeof(struct arch_uctx));
	if (err != 0) {
		hm_warn("copy from user failed, err:%d, regs:%p\n", err, &hm_regs);
		return err;
	}

	copy_to_hmpt_regs(&uctx, &hm_regs);
	err = g_probe_ops->handle_breakpoint(&hm_regs);
	err = posix2hmerrno(-err);
	if (err == 0) {
		copy_to_uctx(&hm_regs, &uctx);
		err = hm_copy_to_user(__uctx, &uctx, sizeof(struct arch_uctx));
	}

	if (err != 0) {
		hm_warn("fail to handle breakpoint: err=%d\n", err);
	}
	return err;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_probe_handle_ss_brk, dh_probe_handle_ss_brk_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct arch_uctx *, __uctx)
{
	UNUSED(sender, credential);
	int err = E_HM_NOSYS;
	struct hmpt_regs hm_regs;
	struct arch_uctx uctx = {0};

	if (g_probe_ops == NULL) {
		return err;
	}

	err = hm_copy_from_user(&uctx, __uctx, sizeof(struct arch_uctx));
	if (err != 0) {
		hm_warn("copy from user failed, err:%d, regs:%p\n", err, &hm_regs);
		return err;
	}

	copy_to_hmpt_regs(&uctx, &hm_regs);
	err = g_probe_ops->handle_ss_breakpoint(&hm_regs);
	err = posix2hmerrno(-err);
	if (err == 0) {
		copy_to_uctx(&hm_regs, &uctx);
		err = hm_copy_to_user(__uctx, &uctx, sizeof(struct arch_uctx));
	}

	if (err != 0) {
		hm_warn("fail to handle breakpoint: err=%d\n", err);
	}

	return err;
}

/* load devhost_probe module */
static int probe_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("devhost_probe.ko");
	if (ret < 0) {
		hm_error("lnxbase: load devhost_probe.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, probe_load_module, NULL);

/*
 * devhost handles devhost_probe brk exception
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_probe_handle_brk`
 * export symbol: NULL
 *
 * dependence: NULL
 */
DEFINE_DSO_PLUGIN(probe, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_probe_handle_brk, dh_probe_handle_brk_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_probe_handle_ss_brk, dh_probe_handle_ss_brk_ext),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
