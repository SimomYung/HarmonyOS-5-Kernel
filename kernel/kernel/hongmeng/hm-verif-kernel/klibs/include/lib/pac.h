/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: PAC instructions
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 26 10:57:32 2023
 */
#ifndef KLIBS_PAC_H
#define KLIBS_PAC_H

#include <hmasm/types.h>

#ifdef CONFIG_PAC
#define DEFINE_PA_INST0(INST) \
static inline void pa_inst_##INST(void) \
{ \
	asm volatile (#INST); \
}

#define DEFINE_PA_INST1(INST) \
static inline __uptr_t pa_inst_##INST(__uptr_t ptr) \
{ \
	asm volatile (#INST " %0": "+r"(ptr)); \
	return ptr; \
}

#define DEFINE_PA_INST2(INST) \
static inline __uptr_t pa_inst_##INST(__uptr_t ptr, __u64 mod) \
{ \
	asm volatile (#INST " %0, %1": "+r"(ptr): "r"(mod)); \
	return ptr; \
}

#define DEFINE_PA_INST3(INST) \
static inline __u64 pa_inst_##INST(__uptr_t ptr, __u64 mod) \
{ \
	__u64 pac; \
	asm volatile (#INST " %0, %1, %2": "=r"(pac): "r"(ptr), "r"(mod)); \
	return pac; \
}

#define DEFINE_PA_INST_GRP_COMMON(GRP, REG) \
	/* x?, zero */ \
	DEFINE_PA_INST1(GRP##z##REG) \
	/* x?, x? */ \
	DEFINE_PA_INST2(GRP##REG)

#define DEFINE_PA_INST_GRP_I(GRP, REG) \
	/* lr, sp */ \
	DEFINE_PA_INST0(GRP##REG##sp) \
	/* lr, zero */ \
	DEFINE_PA_INST0(GRP##REG##z) \
	/* x17, x16 */ \
	DEFINE_PA_INST0(GRP##REG##1716) \
	DEFINE_PA_INST_GRP_COMMON(GRP, REG)

#define DEFINE_PA_INST_GRP_D(GRP, REG) DEFINE_PA_INST_GRP_COMMON(GRP, REG)
#define DEFINE_PA_INST_GRP(GRP, REG) \
	DEFINE_PA_INST_GRP_I(GRP##i, REG) \
	DEFINE_PA_INST_GRP_D(GRP##d, REG)

DEFINE_PA_INST_GRP(pac, a)
DEFINE_PA_INST_GRP(pac, b)
DEFINE_PA_INST_GRP(aut, a)
DEFINE_PA_INST_GRP(aut, b)
DEFINE_PA_INST3(pacga)
DEFINE_PA_INST1(xpaci)
DEFINE_PA_INST1(xpacd)
#endif

#ifdef PAC_DFI_SHARED
#define DFI_SHIFT_BITS 32
#define DFI_MASK_BITS 48
#define DFI_MASK_LOW ((1UL << DFI_MASK_BITS) - 1)
#define DFI_MASK_HIGH (~DFI_MASK_LOW)

static inline __u64 __hm_pacdb_default(__u64 data, __u64 context)
{
	__u64 res = data;
	asm volatile("pacdb  %0, %1" : "+r"(res) : "r"(context) :);
	return res >> DFI_SHIFT_BITS;
}
// Encrypt low bits and use high bits xor context as modifier
// to aovid missing protection of high bits.
static inline __u64 __hm_pacdb(__u64 data, __u64 context)
{
	return __hm_pacdb_default(data & DFI_MASK_LOW,
						(data & DFI_MASK_HIGH) ^ context);
}
static inline void hm_pacdb_data_chek(__u64 data, __u64 context, __u32 pac_value)
{
	__u64 pac = __hm_pacdb(data, context);
	if (pac != pac_value) {
		asm volatile("mov    x0, #1\n\t"
			"mov    w1, %w0\n\t"
			"mov    w2, %w1\n\t"
			"ldr    x0, [x0]"
			:
			: "r"(pac), "r"(pac_value)
			:);
	}
}
#else
static inline __u64 __hm_pacdb_default(__u64 data, __u64 context)
{
	(void)data;
	(void)context;
	return 0;
}
static inline __u64 __hm_pacdb(__u64 data, __u64 context)
{
	(void)data;
	(void)context;
	return 0;
}
static inline void hm_pacdb_data_chek(__u64 data, __u64 context, __u32 pac_value)
{
	(void)data;
	(void)context;
	(void)pac_value;
}
#endif

#if defined(PAC_FWCFI_SYSSERVICE) || defined(PAC_FWCFI_KERNEL)
#define PAC_FECFI_SIGN(_addr, _type) \
	asm volatile ("pacia %0, %1 \n": "=r" (_addr): "r"(__builtin_get_modifier_bytype(_type)), "0"(_addr))
#ifndef PAC_FECFI_XPAC
#define PAC_FECFI_XPAC(_addr) \
({ \
	const void *__addr = (_addr); \
\
	asm ("xpaci %0\n" : "+r" (__addr)); \
	(typeof (_addr))__addr; \
})
#endif
#define PAC_FECFI_XPAC_SIGN(_addr, _type) do { \
        _addr = PAC_FECFI_XPAC(_addr); \
        if ((_addr) != NULL) { \
                PAC_FECFI_SIGN((_addr), _type); \
        } \
} while (0)
#else
#define PAC_FECFI_SIGN(_addr, _type)
#define PAC_FECFI_XPAC(_addr)
#define PAC_FECFI_XPAC_SIGN(_addr, _type)
#endif

#ifdef CONFIG_PAC
#define __PAC_INIT_SECTION __attribute__((noinline,section(".pac_init_section")))
#else
#define __PAC_INIT_SECTION
#endif

#endif
