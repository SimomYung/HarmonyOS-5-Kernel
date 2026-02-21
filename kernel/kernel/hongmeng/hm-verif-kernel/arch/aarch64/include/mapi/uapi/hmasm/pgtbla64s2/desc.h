/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Arm64 stage2 page table desc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 23 22:14:37 2023
 */
#ifndef MAPI_UAPI_HMASM_PGTBLA64S2_DESC_H
#define MAPI_UAPI_HMASM_PGTBLA64S2_DESC_H

/* undefine guard to mute code scanner */
#undef MAPI_UAPI_HMASM_PGTBLA64S2_DESC_H

#ifdef __PGTBLHDLR_CODE

/* Real guard in __PGTBLHDLR_CODE block */
#ifndef MAPI_UAPI_HMASM_PGTBLA64S2_DESC_H__CODE
#define MAPI_UAPI_HMASM_PGTBLA64S2_DESC_H__CODE

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/strict.h>
#include <hmasm/pgtbla64s2/attr.h>
#include <hmasm/pgtbla64s2/pgtblattr/all_noguard.h>

typedef __u64 __pgtblhdlr_a64s2_rawfmt_base_t;
#define __PGTBLHDLR_A64S2_RAWFMT_BASE_T_INVAL 0xffffffffffffffffull

struct __pgtblhdlr_rawucfg_a64s2_s {
	unsigned long ipa_bits;
	__bool is_hisi;
	struct {
		__u64 a64s2ttbrreg;
		__u64 a64s2tbl512g;
		__u64 a64s2tbl1g;
		__u64 a64s2tbl2m;
		__u64 a64s2blk1g;
		__u64 a64s2blk2m;
		__u64 a64s2page4k;
	} attrbits;
};

#define pga64s2cfg_of_attr(bits, hisi, A) \
{ \
	.ipa_bits = bits, \
	.is_hisi = hisi, \
	.attrbits = A, \
}

struct __pgtblhdlr_rawfmt_a64s2ttbrreg_s {
	__u64 v;
};

#define __PGTBL_A64S2_COMMON_TBL_SET_LEVEL(sz)	\
static __pgtblhdlr_inline __pgtblhdlr_a64s2_rawfmt_base_t \
__pgtblhdlr_a64s2tbl##sz##_prepare_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __paddr_t pa) \
{ \
	return cfg->attrbits.a64s2tbl##sz | (pa & __UL(0xfffffffff000)); \
} \

__PGTBL_A64S2_COMMON_TBL_SET_LEVEL(512g)
__PGTBL_A64S2_COMMON_TBL_SET_LEVEL(1g)
__PGTBL_A64S2_COMMON_TBL_SET_LEVEL(2m)

static __pgtblhdlr_inline __pgtblhdlr_a64s2_rawfmt_base_t
__pgtblhdlr_a64s2ttbrreg_prepare_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64s2ttbrreg | (pa & __UL(0xfffffffffffe));
}

static __pgtblhdlr_inline __pgtblhdlr_a64s2_rawfmt_base_t
__pgtblhdlr_a64s2page4k_prepare_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64s2page4k | (pa & __UL(0xfffffffff000));
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64s2ttbrreg_is_zero(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					const struct __pgtblhdlr_rawfmt_a64s2ttbrreg_s *raw)
{
	return ____NOT(raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64s2ttbrreg_val_is_valid(__u64 v)
{
	return ____IS(v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64s2ttbrreg_is_valid(const struct __pgtblhdlr_rawfmt_a64s2ttbrreg_s *raw)
{
	return __pgtblhdlr_rawfmt_a64s2ttbrreg_val_is_valid(raw->v);
}

struct __pgtblhdlr_rawfmt_a64s2pgentry_s {
	__u64 v;
};

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64s2pgentry_is_zero(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return ____NOT(raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64s2pgentry_val_is_valid(__u64 v)
{
	return ____IS((v & __UL(1)));
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64s2pgentry_is_valid(const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_rawfmt_a64s2pgentry_val_is_valid(raw->v);
}

/*
 * Define arm64 common page table entry operations
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk_common_val_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return ((v & __UL(3)) == __UL(1));
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk_common_entry_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					     const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_a64s2blk_common_val_is_present(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk_common_val_is_executable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	/*
	 * Check if all of uxn/xn/pxn are 1.
	 * If one of them is 0, it can execute.
	 */
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk_common_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	/*
	 * Check if both uxn/xn are 1 (they are on same bit).
	 * If one of them is 0 (the bit is 0), it can execute in userspace.
	 */
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 0, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk_common_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	/*
	 * Check if pxn is 1. It can execute in kernel when pxn is 0.
	 */
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 0, 0, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2tbl_common_val_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return ((v & __UL(3)) == __UL(3));
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2tbl_common_entry_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					     const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_a64s2tbl_common_val_is_present(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2tbl_common_val_is_executable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return __false;
}
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2tbl_common_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return __false;
}
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2tbl_common_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return __false;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2tbl_common_is_lastlv(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg)
{
	return __false;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk_common_is_lastlv(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg)
{
	return __true;
}

#define __PGTBL_A64S2_COMMON_TBL_LEVEL(sz)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_val_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2tbl_common_val_is_present(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_entry_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, \
					    const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64s2tbl_common_entry_is_present(cfg, raw); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_val_is_executable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2tbl_common_val_is_executable(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2tbl_common_val_is_uexecutable(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2tbl_common_val_is_kexecutable(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_is_lastlv(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg) \
{ \
	return __false; \
} \
static __pgtblhdlr_inline __paddr_t \
__pgtblhdlr_a64s2tbl##sz##_decode_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, \
				      __pgtblhdlr_a64s2_rawfmt_base_t v) \
{ \
	return v & __UL(0xfffffffff000); \
} \
static __pgtblhdlr_inline __paddr_t \
__pgtblhdlr_a64s2tbl##sz##_decode_entry(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, \
					const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64s2tbl##sz##_decode_val(cfg, raw->v & __UL(0xfffffffff000)); \
} \
static __pgtblhdlr_inline unsigned long \
__pgtblhdlr_a64s2tbl##sz##_tblentry_size(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg) \
{ \
	return sizeof(__u64); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2tbl##sz##_key_is_aligned(__paddr_t ipa_start, __paddr_t ipa_end) \
{ \
	return __true; \
}

#define __PGTBL_A64S2_COMMON_BLK_LEVEL(sz)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2blk##sz##_val_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2blk_common_val_is_present(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2blk##sz##_entry_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, \
					    const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64s2blk_common_entry_is_present(cfg, raw); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2blk##sz##_val_is_executable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2blk_common_val_is_executable(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2blk##sz##_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2blk_common_val_is_uexecutable(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2blk##sz##_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64s2blk_common_val_is_kexecutable(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2blk##sz##_is_lastlv(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg) \
{ \
	return __true; \
} \
static __pgtblhdlr_inline unsigned long \
__pgtblhdlr_a64s2blk##sz##_tblentry_size(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg) \
{ \
	return __UL(1); \
}

/*
 * Register in ttbr (logical)
 * Address 0 is keylid in some devices, in this case ttbr should not point to address 0
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_val_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return ____IS(v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_entry_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					  const struct __pgtblhdlr_rawfmt_a64s2ttbrreg_s *raw)
{
	return __pgtblhdlr_a64s2ttbrreg_val_is_present(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_val_is_executable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return __false;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return __false;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return __false;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_is_lastlv(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg)
{
	return __false;
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2ttbrreg_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					 __paddr_t ipa)
{
	unsigned int ipa_bits = cfg->ipa_bits;
	unsigned long result;

	if (!cfg->is_hisi) {
		if (ipa_bits > __U(39)) {
			result = (ipa >> __U(39)) & ((__UL(1) << (ipa_bits - __U(39))) - __UL(1));
		} else if (ipa_bits > __U(30)) {
			result = (ipa >> __U(30)) & ((__UL(1) << (ipa_bits - __U(30))) - __UL(1));
		} else if (ipa_bits > __U(21)) {
			result = (ipa >> __U(21)) & ((__UL(1) << (ipa_bits - __U(21))) - __UL(1));
		} else {
			result = (ipa >> __U(12)) & ((__UL(1) << (ipa_bits - __U(12))) - __UL(1));
		}
	} else {
		/* Hisi smmu is fixed length(39 + 4) ipa which has 13 bits index of ttbrreg */
		result = (ipa >> __U(30)) & ((__UL(1) << (ipa_bits - __U(30))) - __UL(1));
	}

	return result;
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2ttbrreg_tblentry_size(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg)
{
	return sizeof(__u64);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2ttbrreg_decode_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				    __pgtblhdlr_a64s2_rawfmt_base_t v)
{
	return v & __UL(0xfffffffffffe);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2ttbrreg_decode_entry(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				      const struct __pgtblhdlr_rawfmt_a64s2ttbrreg_s *raw)
{
	return __pgtblhdlr_a64s2ttbrreg_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2ttbrreg_key_is_aligned(__paddr_t ipa_start, __paddr_t ipa_end)
{
	return __true;
}

/*
 * Only table is allowed at level 1 (controls bit 48~39)
 */
__PGTBL_A64S2_COMMON_TBL_LEVEL(512g)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2tbl512g_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					 __paddr_t ipa)
{
	return (ipa >> __U(30)) & (__UL(0x1ff));
}

/*
 * decode_entry for table has been auto generated using __PGTBL_A64S2_COMMON_LEVELxxx because
 * for all table entry, pa points to a 4k aligned table.
 */
__PGTBL_A64S2_COMMON_TBL_LEVEL(1g)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2tbl1g_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				       __paddr_t ipa)
{
	return (ipa >> __U(21)) & (__UL(0x1ff));
}

__PGTBL_A64S2_COMMON_BLK_LEVEL(1g)

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2blk1g_decode_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				  __pgtblhdlr_a64s2_rawfmt_base_t v)
{
	return v & __UL(0xffffc0000000);
}

static __pgtblhdlr_inline __pgtblhdlr_a64s2_rawfmt_base_t
__pgtblhdlr_a64s2blk1g_prepare_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64s2blk1g | (pa & __UL(0xffffc0000000));
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2blk1g_decode_entry(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				    const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_a64s2blk1g_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2blk1g_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				       __paddr_t ipa)
{
	/* all 30 bites */
	return ipa & __UL(0x3fffffff);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk1g_key_is_aligned(__paddr_t ipa_start, __paddr_t ipa_end)
{
	return (ipa_start % 0x40000000 == 0) && (ipa_end % 0x40000000 == 0);
}

__PGTBL_A64S2_COMMON_TBL_LEVEL(2m)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2tbl2m_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				       __paddr_t ipa)
{
	return (ipa >> __U(12)) & (__UL(0x1ff));
}

__PGTBL_A64S2_COMMON_BLK_LEVEL(2m)

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2blk2m_decode_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				  __pgtblhdlr_a64s2_rawfmt_base_t v)
{
	return v & __UL(0xffffffe00000);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2blk2m_decode_entry(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				    const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_a64s2blk2m_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline __pgtblhdlr_a64s2_rawfmt_base_t
__pgtblhdlr_a64s2blk2m_prepare_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64s2blk2m | (pa & __UL(0xffffffe00000));
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2blk2m_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				       __paddr_t ipa)
{
	/* all 21 bits */
	return ipa & __UL(0x1fffff);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2blk2m_key_is_aligned(__paddr_t ipa_start, __paddr_t ipa_end)
{
	return (ipa_start % 0x200000 == 0) && (ipa_end % 0x200000 == 0);
}

/*
 * Last level 4k page
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_val_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	return (v & __UL(3)) == __UL(3);
}
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_entry_is_present(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					 const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_a64s2page4k_val_is_present(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_val_is_executable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 0, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg, __u64 v)
{
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 0, 0, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_is_lastlv(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg)
{
	return __true;
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2page4k_decode_val(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				   __pgtblhdlr_a64s2_rawfmt_base_t v)
{
	return v & __UL(0xfffffffff000);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64s2page4k_decode_entry(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				     const struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr_a64s2page4k_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2page4k_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
					__paddr_t ipa)
{
	return ipa & __UL(0xfff);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64s2page4k_tblentry_size(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg)
{
	return __U(1);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64s2page4k_key_is_aligned(__paddr_t ipa_start, __paddr_t ipa_end)
{
	return __true;
}

#define __PGTBL_A64S2_CONN_ACCEPT_UNCOND(n, raw, nx, rawx)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_##n##_accept_##nx(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg) \
{ \
	return __true; \
}

__PGTBL_A64S2_CONN_ACCEPT_UNCOND(a64s2tbl512g,	a64s2pgentry,	a64s2tbl1g,	a64s2pgentry)
__PGTBL_A64S2_CONN_ACCEPT_UNCOND(a64s2tbl512g,	a64s2pgentry,	a64s2blk1g,	a64s2pgentry)
__PGTBL_A64S2_CONN_ACCEPT_UNCOND(a64s2tbl1g,	a64s2pgentry,	a64s2tbl2m,	a64s2pgentry)
__PGTBL_A64S2_CONN_ACCEPT_UNCOND(a64s2tbl1g,	a64s2pgentry,	a64s2blk2m,	a64s2pgentry)
__PGTBL_A64S2_CONN_ACCEPT_UNCOND(a64s2tbl2m,	a64s2pgentry,	a64s2page4k,	a64s2pgentry)

#define __PGTBL_A64S2_CONN_ACCEPT_TTBRREG(nx, b)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64s2ttbrreg_accept_##nx(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg) \
{ \
	unsigned int ipa_bits = cfg->ipa_bits; \
	__bool res; \
	if ((ipa_bits > __U(b)) && (ipa_bits <= (__U(b) + __U(9)))) { \
		if (cfg->is_hisi && __U(b) == 39) { \
			res = __false; \
		} else { \
			res = __true; \
		} \
	} else { \
		if (cfg->is_hisi && __U(b) == 30) { \
			res = __true; \
		} else { \
			res = __false; \
		} \
	} \
	return res; \
}

__PGTBL_A64S2_CONN_ACCEPT_TTBRREG(a64s2tbl512g,	39)
__PGTBL_A64S2_CONN_ACCEPT_TTBRREG(a64s2tbl1g,	30)
__PGTBL_A64S2_CONN_ACCEPT_TTBRREG(a64s2blk1g,	30)
__PGTBL_A64S2_CONN_ACCEPT_TTBRREG(a64s2tbl2m,	21)
__PGTBL_A64S2_CONN_ACCEPT_TTBRREG(a64s2blk2m,	21)
__PGTBL_A64S2_CONN_ACCEPT_TTBRREG(a64s2page4k,	12)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_pgtbla64s2_key_align(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				 unsigned long reference)
{
	unsigned long res = reference;

	if (reference == 0ul) {
		res = __UL(1) << cfg->ipa_bits;
	}
	return res;
}

/*
 * Alignment and range are always same for aarch64
 */
static __pgtblhdlr_inline unsigned long
__pgtblhdlr_pgtbla64s2_key_range(const struct __pgtblhdlr_rawucfg_a64s2_s *cfg,
				 unsigned long reference)
{
	return __pgtblhdlr_pgtbla64s2_key_align(cfg, reference);
}

#endif /* real guard */

#else  /* !__PGTBLHDLR_CODE */

PGTBLHDLR_RAWCFG(a64s2)
PGTBLHDLR_RAWFMT(a64s2ttbrreg)
PGTBLHDLR_RAWFMT(a64s2pgentry)

PGTBLHDLR_PGENTRY(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, 0,			0)
PGTBLHDLR_PGENTRY(a64s2tbl512g, A64S2TBL512G, a64s2pgentry, 0x8000000000,	0x8000000000)
PGTBLHDLR_PGENTRY(a64s2blk1g,   A64S2BLK1G,   a64s2pgentry, 0x40000000,		0x40000000)
PGTBLHDLR_PGENTRY(a64s2tbl1g,   A64S2TBL1G,   a64s2pgentry, 0x40000000,		0x40000000)
PGTBLHDLR_PGENTRY(a64s2blk2m,   A64S2BLK2M,   a64s2pgentry, 0x200000,		0x200000)
PGTBLHDLR_PGENTRY(a64s2tbl2m,   A64S2TBL2M,   a64s2pgentry, 0x200000,		0x200000)
PGTBLHDLR_PGENTRY(a64s2page4k,  A64S2PAGE4K,  a64s2pgentry, 0x1000,		0x1000)

PGTBLHDLR_CONN(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, a64s2tbl512g, A64S2TBL512G, a64s2pgentry)
PGTBLHDLR_CONN(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, a64s2blk1g,   A64S2BLK1G,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, a64s2tbl1g,   A64S2TBL1G,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, a64s2blk2m,   A64S2BLK2M,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, a64s2tbl2m,   A64S2TBL2M,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2ttbrreg, A64S2TTBRREG, a64s2ttbrreg, a64s2page4k,  A64S2PAGE4K,  a64s2pgentry)

PGTBLHDLR_CONN(a64s2tbl512g, A64S2TBL512G, a64s2pgentry, a64s2blk1g,   A64S2BLK1G,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2tbl512g, A64S2TBL512G, a64s2pgentry, a64s2tbl1g,   A64S2TBL1G,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2tbl1g,   A64S2TBL1G,   a64s2pgentry, a64s2blk2m,   A64S2BLK2M,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2tbl1g,   A64S2TBL1G,   a64s2pgentry, a64s2tbl2m,   A64S2TBL2M,   a64s2pgentry)
PGTBLHDLR_CONN(a64s2tbl2m,   A64S2TBL2M,   a64s2pgentry, a64s2page4k,  A64S2PAGE4K,  a64s2pgentry)

#endif  /* __PGTBLHDLR_CODE */

#endif /* fake guard */
