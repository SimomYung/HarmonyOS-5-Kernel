/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_MMU_H
#define __LIBLINUX_ASM_MMU_H


#ifndef __ASSEMBLY__

#include <linux/refcount.h>

typedef struct {
	atomic64_t	id;
#ifdef CONFIG_COMPAT
	void		*sigpage;
#endif
	refcount_t	pinned;
	void		*vdso;
	unsigned long	flags;
	unsigned long long	vspace_key;
} mm_context_t;


#define ASID(mm)	(atomic64_read(&(mm)->context.id) & 0xffff)
#define VSPACE_KEY(mm)	((mm)->context.vspace_key)
#endif

#endif
