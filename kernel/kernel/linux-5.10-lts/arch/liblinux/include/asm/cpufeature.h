/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_CPUFEATURE_H
#define __LIBLINUX_ASM_CPUFEATURE_H

#define MAX_CPU_FEATURES        64

#ifndef __ASSEMBLY__

#ifdef CONFIG_ARM64
/* required by "asm/module.h" */
static __always_inline bool cpus_have_const_cap(int num) { return false; }
/* required by "asm/virt.h" */
static __always_inline bool cpus_have_final_cap(int num) { return false; }
static __always_inline bool is_vhe_hyp_code(void) { return false; }
static __always_inline bool is_nvhe_hyp_code(void) { return false; }
static __always_inline bool this_cpu_has_cap(unsigned int cap) { return false; }
/* required by "asm/kvm_host.h" */
static __always_inline bool system_supports_sve(void) { return false; }
/* required by "asm/daifflags.h" */
static __always_inline bool system_has_prio_mask_debugging(void) { return false; }
static __always_inline bool system_uses_irq_prio_masking(void) { return false; }
/* required by "asm/elf.h" */
static inline bool system_supports_bti(void) { return false; }
/*required by "driver/base/cpu.h"*/
bool cpu_have_feature(unsigned int num);
#define cpu_feature(x) KERNEL_HWCAP_ ## x
#define cpu_set_named_feature(name) do {} while(0)
#define cpu_have_named_feature(name) cpu_have_feature(1)

static inline bool system_supports_address_auth(void)
{
    return IS_ENABLED(CONFIG_ARM64_PTR_AUTH);
}

static inline bool system_supports_generic_auth(void)
{
    return IS_ENABLED(CONFIG_ARM64_PTR_AUTH);
}
#endif /* CONFIG_ARM64 */
const struct cpumask *system_32bit_el0_cpumask(void);
static inline bool id_aa64pfr0_32bit_el0(u64 pfr0)
{
    return false;
}

#endif /* !__ASSEMBLY__ */
#endif /* __LIBLINUX_ASM_CPUFEATURE_H */
