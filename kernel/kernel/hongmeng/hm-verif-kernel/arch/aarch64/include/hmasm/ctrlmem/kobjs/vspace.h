/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Arch specific ctrlmem kobj vspace related functions
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 17 01:06:39 2023
 */

#ifndef ARCH64_INCLUDE_HMASM_CTRLMEM_KOBJS_VSPACE_H
#define ARCH64_INCLUDE_HMASM_CTRLMEM_KOBJS_VSPACE_H

#include <hmasm/tlb.h>
#include <hmkernel/strict.h>

#include <hmkernel/ctrlmem/kobjs/vspace.h>
#include <hmkernel/ctrlmem/vspace/supervsidx.h>
#include <hmkernel/ctrlmem/vspace/tlbflush.h>

#include <hmkernel/capability/cap_vspace.h>

struct ctrlmem_kobj_vspace_s;

struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s {
	struct ctrlmem_vspace_tlbflush_dropidx_ctx_s dropidx_ctx;
	unsigned int asid;
	bool dropall;
	bool hyp;
};

#define CTRLMEM_KOBJ_VSPACE_RANGEOP_TLBFLUSH_CTX_INIT(_kobjvs) { \
	.dropidx_ctx = __CTRLMEM_VSPACE_TLBFLUSH_DROPIDX_CTX_INIT(_kobjvs, \
			ctrlmem_kobj_vspace_busy_idx_of(_kobjvs), \
			(_kobjvs)->supervsidx, \
			ctrlmem_kobj_vspace_is_privileged(_kobjvs)), \
	.asid = 0U, \
	.dropall = false, \
	.hyp = false, \
}

#ifdef CONFIG_HYPERVISOR
static inline
bool ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_is_hyp(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx)
{
	return ctx->hyp;
}
#else
static inline
bool ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_is_hyp(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx)
{
	return false;
}
#endif

/*
 *  Usage:
 *
 *  struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s ctx;              |
 *  err = ctrlmem_kobj_vspace_rangeop_tlbflush_start(&ctx, ...);        |
 *  if (err == 0) {                                                     |
 *      foreach(page) {                                                 |
 *          ctrlmem_kobj_vspace_rangeop_tlbflush_onepage(&ctx, pageva); |
 *      }                                                               |
 *      ctrlmem_kobj_vspace_rangeop_tlbflush_finish(&ctx);              |
 *  } else if (err == E_HM_NOENT) {                                     |
 *      err = 0;                                                        |
 *  }                                                                   |
 */
static inline int __ctrlmem_kobj_vspace_rangeop_tlbflush_start(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx)
{
	int err = 0;

	unsigned int busy_idx = ctx->dropidx_ctx.busy_idx;
	if (!ctx->dropidx_ctx.has_busy_idx && !ctx->dropidx_ctx.has_supervsidx) {
		/*
		 * Not an error: no valid busy idx, no need to do tlb flush.
		 * Even it has busy idx before we do unmap or mprotect, since
		 * we have observed no-valid busy idx *after* modifying page table,
		 * there must have a tlb-flush-all-by-asid during page table
		 * updating.
		 *
		 * If it lost its asid during following tlbi loop, there is harmless
		 * for kernel and functionality. The only drawback is it flush tlb
		 * for wrong vspace.
		 *
		 * If it get a new asid or switch to a new asid during flushing, it is
		 * also no harm because in new asid space the TLB is empty.
		 *
		 * If asid switching is frequent, we can change the above tlbi loop
		 * to:
		 *      foreach(page) {                                                 |
		 *          ctrlmem_kobj_vspace_rangeop_tlbflush_onepage(&ctx, pageva); |
		 *          new_asid = asid_of(ctx);                                    |
		 *          if (new_asid != old_asid) {                                 |
		 *              break;                                                  |
		 *          }                                                           |
		 *      }                                                               |
		 *
		 */
		err = E_HM_NOENT;
	} else {
		err = ctrlmem_vspace_tlbflush_dropidx_start(&ctx->dropidx_ctx);
		BUG_ON(err != 0);
		if (!ctx->dropidx_ctx.is_privileged) {
			if (ctx->dropidx_ctx.has_busy_idx) {
				ctx->asid = a64_asid_from_busy_idx(busy_idx);
			}
		}
	}
	/*
	 * DSB is required unconditionally. If its asid is changing during unmap/mprotect,
	 * we must make sure the new ptes are written.
	 */
	dsb(ishst);
	ctx->dropidx_ctx.dsb_done = true;
	return err;
}

static inline int ctrlmem_kobj_vspace_rangeop_tlbflush_start(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx)
{
	int err = 0;
	if (!tlb_flush_is_hyp(ctx->dropidx_ctx.kobjvs)) {
		ctx->hyp = false;
		err = __ctrlmem_kobj_vspace_rangeop_tlbflush_start(ctx);
	} else {
		ctx->hyp = true;
		dsb(ishst);
		ctx->dropidx_ctx.dsb_done = true;
	}
	return err;
}

#define MAX_TLBI_RANGE_PAGES	512

static inline
void __ctrlmem_kobj_vspace_rangeop_tlbflush_onepage(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx,
						    const void __user *va, unsigned long size)
{
	if (!ctx->dropall) {
		if (!ctx->dropidx_ctx.is_privileged) {
			if (ctx->dropidx_ctx.has_busy_idx) {
				tlb_invalidate_by_asid(ptr_to_vaddr(va), ctx->asid, size);
			}
			if (ctx->dropidx_ctx.has_supervsidx) {
				__arch_ctrlmem_vspace_supervs_tlbflush_fixup(
						ctx->dropidx_ctx.supervsidx, ptr_to_vaddr(va), size);
			}
		} else {
			tlb_invalidate_all_asid(ptr_to_vaddr(va), size);
		}
	}
}

static inline void ctrlmem_kobj_vspace_rangeop_tlbflush_onepage(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx,
								const void __user *va, unsigned long size)
{
	if (unlikely(ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_is_hyp(ctx))) {
		(void)tlb_flush_try_hyp(ctx->dropidx_ctx.kobjvs, va, size);
	} else {
		__ctrlmem_kobj_vspace_rangeop_tlbflush_onepage(ctx, va, size);
	}
}

static inline bool ctrlmem_kobj_vspace_rangeop_tlbflush_try_dropall(
		struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx,
		unsigned int nr_pages)
{
	if (nr_pages >= MAX_TLBI_RANGE_PAGES) {
		ctx->dropall = true;
	} else {
		ctx->dropall = false;
	}
	return ctx->dropall;
}

static inline void __ctrlmem_kobj_vspace_rangeop_tlbflush_finish(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx)
{
	if (!ctx->dropall) {
		dsb(ish);
		if (ctx->dropidx_ctx.is_privileged || ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_is_hyp(ctx)) {
			isb();
		}
	} else {
		if (unlikely(ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_is_hyp(ctx))) {
			tlb_flush_hyp_done(ctx->dropidx_ctx.kobjvs);
		} else {
			ctrlmem_vspace_tlbflush_dropidx_done(&ctx->dropidx_ctx);
		}
	}
}

static inline void ctrlmem_kobj_vspace_rangeop_tlbflush_finish(struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx)
{
	__ctrlmem_kobj_vspace_rangeop_tlbflush_finish(ctx);
}

#endif
