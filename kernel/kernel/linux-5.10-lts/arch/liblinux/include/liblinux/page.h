/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_PAGE_H__
#define __LIBLINUX_PAGE_H__

#include <linux/gfp.h>
#include <liblinux/pal.h>

static inline unsigned int __gfp_mask_to_page_pool_flag(gfp_t gfp_mask)
{
	unsigned int flags = 0;

	if (gfp_mask & __GFP_DMA)
		flags |= LIBLINUX_PAL_GFP_DMA;
	if (gfp_mask & __GFP_NORETRY)
		flags |= LIBLINUX_PAL_GFP_NORETRY;
	if ((gfp_mask & __GFP_NORETRY) || (gfp_mask & __GFP_RETRY_MAYFAIL) || (gfp_mask & __GFP_NOWARN))
		flags |= LIBLINUX_PAL_GFP_NO_RESVMEM;
	if (gfp_mask & __GFP_KSWAPD_SELF)
		flags |= LIBLINUX_PAL_GFP_KSWAPD_SELF;

	return flags;
}

#endif /* __LIBLINUX_PAGE_H__ */
