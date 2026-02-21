/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 00:16:08 2018
 */
#ifndef A64_ASM_MM_VSPACE_H
#define A64_ASM_MM_VSPACE_H

#include <hmasm/memory.h>
#include <hmasm/vspace.h>
#include <hmkernel/strict.h>
#include <hmkernel/compiler.h>
#include <hmkernel/mm/pgtable.h>
#include <hmkernel/ctrlmem/vspace.h>

static inline u64 atomic_val(vatomic64_t id)
{
	return vatomic64_read_rlx(&id);
}

struct hyp_vm_s;
struct capability_s;

struct arch_vspace_cap_s {
	union {
#ifdef CONFIG_HYPERVISOR
		/*
		 * For stage2 vspace, don't need ASID at all, but vm pointer.
		 * when flush TLB, needs to hvc to EL2 to switch to VM to flush TLB.
		 */
		__sec_data(kernel, ptr_hdlr) struct hyp_vm_s *vm;
#endif
		u64 v;
	} u;
};

extern int
arch_vspace_cap_init(struct capability_s *vspace,
		     const struct sysarg_vspace_config *sys_conf);

extern int
arch_vspace_cap_destroy(const struct arch_vspace_cap_s *arch_cap);

JAM_INLINE void
arch_vspace_switch_unprivileged(const struct capability_s *vspace_cap);

JAM_INLINE void
arch_vspace_switch_unprivileged_dummy(void);

JAM_INLINE void
arch_vspace_switch_privileged(const struct capability_s *vspace_cap);

JAM_INLINE void
arch_vspace_switch_privileged_default(void);

/* TTBR0 or TTBR1 */
struct arch_vspace_state_s {
	u64 ttbr;
};

/* No JAM_INLINE: not in hostpath. Not very important */
void arch_vspace_switch_unprivileged_atomic(struct arch_vspace_state_s *orig,
					    const struct capability_s *vspace_cap);

void arch_vspace_switch_unprivileged_atomic_restore(const struct arch_vspace_state_s *orig);

extern void
arch_vspace_unmap_page(const struct capability_s *vspace_cap,
		       vaddr_t va, size_t size);

extern void
arch_vspace_unmap_page_nobarrier(const struct capability_s *vspace_cap,
				 vaddr_t va, size_t size);

extern void
arch_vspace_unmap_page_all_asid(vaddr_t va, size_t size);

#ifdef CONFIG_HYPERVISOR
extern void
arch_vspace_set_vm(struct capability_s *cap, struct hyp_vm_s *vm);
#endif

/* On aarch64, we flush TLB page by page, and do nothing when finish unmapping */
static inline void
arch_vspace_unmap_done(const struct capability_s *vspace_cap,
		       vaddr_t __va_start, vaddr_t __va_end)
{
	UNUSED(vspace_cap, __va_start, __va_end);
}

vaddr_t arch_vspace_user_limit(void);

static inline bool arch_vspace_hint_need_icache_sync(u64 hint)
{
	return (((hint & PGTABLE_ATTR_UEXEC) != 0UL) &&
		((hint & PGTABLE_ATTR_DEVICE) == 0UL) &&
		!((hint & PGTABLE_ATTR_A64_STAGE2) != 0UL));
}

int arch_sync_icache_kernel_page(vaddr_t vstart);
int arch_sync_icache_kernel_range(vaddr_t vstart, size_t size);

int arch_sync_icache_page(paddr_t pstart);

int arch_sync_icache_range(paddr_t pstart, paddr_t pend);

void arch_sync_icache_done(void);

bool arch_vspace_is_icache_aliasing(void);

#endif
