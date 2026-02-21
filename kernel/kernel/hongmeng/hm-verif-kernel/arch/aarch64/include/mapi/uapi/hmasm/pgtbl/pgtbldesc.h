/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Description of aarch64 page table
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 02 18:02:39 2021
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBLDESC_H
#define AARCH64_MAPI_UAPI_ASM_PGTBLDESC_H

/* undefine guard to mute code scanner */
#undef AARCH64_MAPI_UAPI_ASM_PGTBLDESC_H

#ifdef __PGTBLHDLR_CODE

/* Real guard in __PGTBLHDLR_CODE block */
#ifndef AARCH64_MAPI_UAPI_ASM_PGTBLDESC_H__CODE
#define AARCH64_MAPI_UAPI_ASM_PGTBLDESC_H__CODE

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/macro.h>
#include <hmkernel/strict.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hmasm/pgtbl/pgtblattr.h>

typedef __u64 __pgtblhdlr_rawfmt_base_t;
#define __PGTBLHDLR_RAWFMT_BASE_T_INVAL 0xffffffffffffffffull

/*
 * According to 'Table D5-28 Data access permissions for stage 1 translations
 * that applies to EL0 and a higher Exception level'.
 * ap2 controls read-only, ap1 controls el0 accessability.
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ap21_is_uwritable(__u64 ap2, __u64 ap1)
{
	return (ap2 == 0UL) && (ap1 != 0UL);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ap21_is_ureadable(__u64 ap2, __u64 ap1)
{
	return (ap1 != 0UL);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ap21_is_kwritable(__u64 ap2, __u64 ap1)
{
	return (ap2 == 0UL);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ap21_is_kreadable(__u64 ap2, __u64 ap1)
{
	return __true;
}

static __pgtblhdlr_inline __u64
__pgtblhdlr_a64ap21_set_rdonly(__u64 orig_v, __u64 ap2, __u64 ap1)
{
	return (orig_v | ap2);
}

#define __pgtblhdlr_a64ap21_is_writable(ap2, ap1) \
	(__pgtblhdlr_a64ap21_is_uwritable((ap2), (ap1)) || \
	 __pgtblhdlr_a64ap21_is_kwritable((ap2), (ap1)))

#define __pgtblhdlr_a64ap21_is_readable(ap2, ap1) \
	(__pgtblhdlr_a64ap21_is_ureadable((ap2), (ap1)) || \
	 __pgtblhdlr_a64ap21_is_kreadable((ap2), (ap1)))

#define __PGTBLHDLR_A64_AP21_EXTRACTABLE_ATTR_LIST uwritable, ureadable, kwritable, kreadable, \
						   writable, readable
#define __PGTBLHDLR_A64_OTHER_EXTRACTABLE_ATTR_LIST uexecutable, kexecutable, executable, fragile, afset
#define __PGTBLHDLR_A64_EXTRACTABLE_ATTR_LIST __PGTBLHDLR_A64_AP21_EXTRACTABLE_ATTR_LIST, \
					      __PGTBLHDLR_A64_OTHER_EXTRACTABLE_ATTR_LIST

#define __PGTBLHDLR_A64_AP21_SETABLE_ATTR_LIST rdonly
#define __PGTBLHDLR_A64_OTHER_SETABLE_ATTR_LIST noaf, fragile, nofragile
#define __PGTBLHDLR_A64_SETABLE_ATTR_LIST __PGTBLHDLR_A64_AP21_SETABLE_ATTR_LIST, \
					  __PGTBLHDLR_A64_OTHER_SETABLE_ATTR_LIST

struct __pgtblhdlr_rawucfg_a64_s {
	unsigned int va_bits;
};

struct __pgtblhdlr_rawfmt_a64ttbrreg_s {
	__u64 v;
};

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64ttbrreg_is_zero(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				      const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw)
{
	return ____NOT(raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64ttbrreg_is_valid(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				       const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw)
{
	return ____IS(raw->v);
}

struct __pgtblhdlr_rawfmt_a64pgentry_s {
	__u64 v;
};

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64pgentry_is_zero(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				      const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return ____NOT(raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_rawfmt_a64pgentry_is_valid(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				       const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return ____IS((raw->v & __UL(1)));
}

/*
 * Define arm64 common page table entry operations
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_val_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return ((v & __UL(3)) == __UL(1));
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_entry_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
					   const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return __pgtblhdlr_a64blk_common_val_is_present(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_val_is_fragile(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return (v & __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING) != 0UL;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_val_is_executable(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	/*
	 * Check if all of uxn/xn/pxn are 1.
	 * If one of them is 0, it can execute.
	 */
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	/*
	 * Check if both uxn/xn are 1 (they are on same bit).
	 * If one of them is 0 (the bit is 0), it can execute in userspace.
	 */
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 0, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	/*
	 * Check if pxn is 1. It can execute in kernel when pxn is 0.
	 */
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 0, 0, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_val_is_afset(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return ((v & __PGTBLATTR_A64LOWERBLKATTR_AF_MASK) != 0) ? __true : __false;
}

static __pgtblhdlr_inline __u64
__pgtblhdlr_a64blk_common_val_set_noaf(__u64 v)
{
	return v & (~(__u64)__PGTBLATTR_A64LOWERBLKATTR_AF_MASK);
}

static __pgtblhdlr_inline __u64
__pgtblhdlr_a64blk_common_val_set_fragile(__u64 v)
{
	return v | __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING;
}

static __pgtblhdlr_inline __u64
__pgtblhdlr_a64blk_common_val_set_nofragile(__u64 v)
{
	return v & ~((__u64)__PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64tbl_common_val_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return ((v & __UL(3)) == __UL(3));
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64tbl_common_entry_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
					   const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return __pgtblhdlr_a64tbl_common_val_is_present(cfg, raw->v);
}

#define __pgtblhdlr_a64tbl_define_ap21_attr(sz, n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __true; \
}

#define __pgtblhdlr_a64tbl_define_other_attr(sz, n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __false; \
}

#define __pgtblhdlr_a64tbl_define_entry_attr(sz, n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_entry_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				      const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64tbl##sz##_val_is_##n(cfg, raw->v); \
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64tbl_common_is_lastlv(const struct __pgtblhdlr_rawucfg_a64_s *cfg)
{
	return __false;
}

#define __pgtblhdlr_a64tbl_define_setable_attr(sz, n) \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64tbl##sz##_val_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				     __u64 v) \
{ \
	return v; \
} \
static __pgtblhdlr_inline void \
__pgtblhdlr_a64tbl##sz##_entry_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				       struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	raw->v = __pgtblhdlr_a64tbl##sz##_val_set_##n(cfg, raw->v); \
}

#define __pgtblhdlr_a64blk_define_ap21_attr(sz, n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64blk##sz##_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64ap21_is_##n(v & __PGTBLATTR_A64LOWERBLKATTR_AP21(0x2), \
					  v & __PGTBLATTR_A64LOWERBLKATTR_AP21(0x1)); \
}

#define __pgtblhdlr_a64blk_define_other_attr(sz, n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64blk##sz##_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64blk_common_val_is_##n(cfg, v); \
}

#define __pgtblhdlr_a64blk_define_entry_attr(sz, n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64blk##sz##_entry_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				      const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64blk##sz##_val_is_##n(cfg, raw->v); \
}

#define __pgtblhdlr_a64blk_define_ap21_setable_attr(sz, n) \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64blk##sz##_val_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				     __u64 orig_v) \
{ \
	return __pgtblhdlr_a64ap21_set_##n(orig_v, __PGTBLATTR_A64LOWERBLKATTR_AP21(0x2), \
						   __PGTBLATTR_A64LOWERBLKATTR_AP21(0x1)); \
}

#define __pgtblhdlr_a64blk_define_other_setable_attr(sz, n) \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64blk##sz##_val_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				     __u64 orig_v) \
{ \
	return __pgtblhdlr_a64blk_common_val_set_##n(orig_v); \
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk_common_is_lastlv(const struct __pgtblhdlr_rawucfg_a64_s *cfg)
{
	return __true;
}

#define __pgtblhdlr_a64blk_define_entry_setable_attr(sz, n) \
static __pgtblhdlr_inline void \
__pgtblhdlr_a64blk##sz##_entry_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				       struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	raw->v = __pgtblhdlr_a64blk##sz##_val_set_##n(cfg, raw->v); \
}

#define __PGTBL_A64_COMMON_TBL_LEVEL(sz)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_val_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64tbl_common_val_is_present(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_entry_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				    const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64tbl_common_entry_is_present(cfg, raw); \
} \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64tbl_define_ap21_attr, sz, \
				 __PGTBLHDLR_A64_AP21_EXTRACTABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64tbl_define_other_attr, sz, \
				 __PGTBLHDLR_A64_OTHER_EXTRACTABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64tbl_define_entry_attr, sz, \
				 __PGTBLHDLR_A64_EXTRACTABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64tbl_define_setable_attr, sz, \
				 __PGTBLHDLR_A64_SETABLE_ATTR_LIST) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_is_lastlv(const struct __pgtblhdlr_rawucfg_a64_s *cfg) \
{ \
	return __false; \
} \
static __pgtblhdlr_inline __paddr_t \
__pgtblhdlr_a64tbl##sz##_decode_val(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				    __pgtblhdlr_rawfmt_base_t v) \
{ \
	return v & __UL(0xfffffffff000); \
} \
static __pgtblhdlr_inline __paddr_t \
__pgtblhdlr_a64tbl##sz##_decode_entry(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				      const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64tbl##sz##_decode_val(cfg, raw->v & __UL(0xfffffffff000)); \
} \
static __pgtblhdlr_inline unsigned long \
__pgtblhdlr_a64tbl##sz##_tblentry_size(const struct __pgtblhdlr_rawucfg_a64_s *cfg) \
{ \
	return sizeof(__u64); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64tbl##sz##_key_is_aligned(__vaddr_t va_start, __vaddr_t va_end) \
{ \
	return __true; \
}

#define __PGTBL_A64_COMMON_BLK_LEVEL(sz)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64blk##sz##_val_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64blk_common_val_is_present(cfg, v); \
} \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64blk##sz##_entry_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				    const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64blk_common_entry_is_present(cfg, raw); \
} \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64blk_define_ap21_attr, sz, \
				 __PGTBLHDLR_A64_AP21_EXTRACTABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64blk_define_other_attr, sz, \
				 __PGTBLHDLR_A64_OTHER_EXTRACTABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64blk_define_entry_attr, sz, \
				 __PGTBLHDLR_A64_EXTRACTABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64blk_define_ap21_setable_attr, sz, \
				 __PGTBLHDLR_A64_AP21_SETABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64blk_define_other_setable_attr, sz, \
				 __PGTBLHDLR_A64_OTHER_SETABLE_ATTR_LIST) \
__hmkernel_macro_exarg1_map_list(__pgtblhdlr_a64blk_define_entry_setable_attr, sz, \
				 __PGTBLHDLR_A64_SETABLE_ATTR_LIST) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64blk##sz##_is_lastlv(const struct __pgtblhdlr_rawucfg_a64_s *cfg) \
{ \
	return __true; \
} \
static __pgtblhdlr_inline unsigned long \
__pgtblhdlr_a64blk##sz##_tblentry_size(const struct __pgtblhdlr_rawucfg_a64_s *cfg) \
{ \
	return __UL(1); \
}

/*
 * Register in ttbr (logical)
 * Address 0 is valid in some devices, in this case ttbr should not point to address 0
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ttbrreg_val_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return ____IS(v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ttbrreg_entry_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				  const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw)
{
	return __pgtblhdlr_a64ttbrreg_val_is_present(cfg, raw->v);
}

#define __pgtblhdlr_a64ttbrreg_define_ap21_attr(n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64ttbrreg_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __true; \
}

#define __pgtblhdlr_a64ttbrreg_define_other_attr(n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64ttbrreg_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __false; \
}

#define __pgtblhdlr_a64ttbrreg_define_entry_attr(n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64ttbrreg_entry_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				    const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw) \
{ \
	return __pgtblhdlr_a64ttbrreg_val_is_##n(cfg, raw->v); \
}

#define __pgtblhdlr_a64ttbrreg_define_setable_attr(n) \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64ttbrreg_val_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return v; \
} \
static __pgtblhdlr_inline void \
__pgtblhdlr_a64ttbrreg_entry_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				     struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw) \
{ \
	raw->v = __pgtblhdlr_a64ttbrreg_val_set_##n(cfg, raw->v); \
}

__hmkernel_macro_simple_map_list(__pgtblhdlr_a64ttbrreg_define_ap21_attr,
				 __PGTBLHDLR_A64_AP21_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64ttbrreg_define_other_attr,
				 __PGTBLHDLR_A64_OTHER_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64ttbrreg_define_entry_attr,
				 __PGTBLHDLR_A64_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64ttbrreg_define_setable_attr,
				 __PGTBLHDLR_A64_SETABLE_ATTR_LIST)

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ttbrreg_is_lastlv(const struct __pgtblhdlr_rawucfg_a64_s *cfg)
{
	return __false;
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64ttbrreg_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				       __vaddr_t va)
{
	unsigned int va_bits = cfg->va_bits;
	unsigned long result;

	if (va_bits > __U(39)) {
		result = (va >> __U(39)) & ((__UL(1) << (va_bits - __U(39))) - __UL(1));
	} else if (va_bits > __U(30)) {
		result = (va >> __U(30)) & ((__UL(1) << (va_bits - __U(30))) - __UL(1));
	} else if (va_bits > __U(21)) {
		result = (va >> __U(21)) & ((__UL(1) << (va_bits - __U(21))) - __UL(1));
	} else {
		result = (va >> __U(12)) & ((__UL(1) << (va_bits - __U(12))) - __UL(1));
	}
	return result;
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64ttbrreg_tblentry_size(const struct __pgtblhdlr_rawucfg_a64_s *cfg)
{
	return sizeof(__u64);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64ttbrreg_decode_val(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				  __pgtblhdlr_rawfmt_base_t v)
{
	return v & __UL(0xfffffffffffe);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64ttbrreg_decode_entry(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				    const struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw)
{
	return __pgtblhdlr_a64ttbrreg_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64ttbrreg_key_is_aligned(__vaddr_t va_start, __vaddr_t va_end)
{
	return __true;
}

/*
 * Only table is allowed at level 1 (controls bit 48~39)
 */
__PGTBL_A64_COMMON_TBL_LEVEL(512g)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64tbl512g_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				       __vaddr_t va)
{
	return (va >> __U(30)) & (__UL(0x1ff));
}

/*
 * decode_entry for table has been auto generated using __PGTBL_A64_COMMON_LEVELxxx because
 * for all table entry, pa points to a 4k aligned table.
 */
__PGTBL_A64_COMMON_TBL_LEVEL(1g)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64tbl1g_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				     __vaddr_t va)
{
	return (va >> __U(21)) & (__UL(0x1ff));
}

__PGTBL_A64_COMMON_BLK_LEVEL(1g)

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64blk1g_decode_val(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				__pgtblhdlr_rawfmt_base_t v)
{
	return v & __UL(0xffffc0000000);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64blk1g_decode_entry(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				  const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return __pgtblhdlr_a64blk1g_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64blk1g_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				     __vaddr_t va)
{
	/* all 30 bites */
	return va & __UL(0x3fffffff);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk1g_key_is_aligned(__vaddr_t va_start, __vaddr_t va_end)
{
	return (va_start % 0x40000000 == 0) && (va_end % 0x40000000 == 0);
}

__PGTBL_A64_COMMON_TBL_LEVEL(2m)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64tbl2m_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				     __vaddr_t va)
{
	return (va >> __U(12)) & (__UL(0x1ff));
}

__PGTBL_A64_COMMON_BLK_LEVEL(2m)

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64blk2m_decode_val(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				__pgtblhdlr_rawfmt_base_t v)
{
	return v & __UL(0xffffffe00000);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64blk2m_decode_entry(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				  const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return __pgtblhdlr_a64blk2m_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64blk2m_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				     __vaddr_t va)
{
	/* all 21 bits */
	return va & __UL(0x1fffff);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64blk2m_key_is_aligned(__vaddr_t va_start, __vaddr_t va_end)
{
	return (va_start % 0x200000 == 0) && (va_end % 0x200000 == 0);
}

/*
 * Last level 4k page
 */
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_val_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return (v & __UL(3)) == __UL(3);
}
static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_entry_is_present(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				 const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return __pgtblhdlr_a64page4k_val_is_present(cfg, raw->v);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_val_is_fragile(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return (v & __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING) != 0UL;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_val_is_executable(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_val_is_uexecutable(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 1, 1, 0, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_val_is_kexecutable(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	__u64 xnmask = (__u64)(__PGTBLATTR_A64UPPERATTR(0, 0, 0, 1, 0, 0));

	return ((v & xnmask) == xnmask) ? __false : __true;
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_val_is_afset(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v)
{
	return ((v & __PGTBLATTR_A64LOWERBLKATTR_AF_MASK) != 0) ? __true : __false;
}

#define __pgtblhdlr_a64page4k_define_ap21_attr(n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64page4k_val_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64ap21_is_##n(v & __PGTBLATTR_A64LOWERBLKATTR_AP21(0x2), \
					  v & __PGTBLATTR_A64LOWERBLKATTR_AP21(0x1)); \
}

#define __pgtblhdlr_a64page4k_define_ap21_setable_attr(n) \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64page4k_val_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64ap21_set_##n(v, __PGTBLATTR_A64LOWERBLKATTR_AP21(0x2), \
					      __PGTBLATTR_A64LOWERBLKATTR_AP21(0x1)); \
}

#define __pgtblhdlr_a64page4k_define_other_setable_attr(n) \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64page4k_val_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, __u64 v) \
{ \
	return __pgtblhdlr_a64blk_common_val_set_##n(v); \
}

#define __pgtblhdlr_a64page4k_define_entry_attr(n) \
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64page4k_entry_is_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				   const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return __pgtblhdlr_a64page4k_val_is_##n(cfg, raw->v); \
}


#define __pgtblhdlr_a64page4k_define_entry_setable_attr(n) \
static __pgtblhdlr_inline void \
__pgtblhdlr_a64page4k_entry_set_##n(const struct __pgtblhdlr_rawucfg_a64_s *cfg, \
				    struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	raw->v = __pgtblhdlr_a64page4k_val_set_##n(cfg, raw->v); \
}

__hmkernel_macro_simple_map_list(__pgtblhdlr_a64page4k_define_ap21_attr,
				 __PGTBLHDLR_A64_AP21_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64page4k_define_entry_attr,
				 __PGTBLHDLR_A64_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64page4k_define_ap21_setable_attr,
				 __PGTBLHDLR_A64_AP21_SETABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64page4k_define_other_setable_attr,
				 __PGTBLHDLR_A64_OTHER_SETABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(__pgtblhdlr_a64page4k_define_entry_setable_attr,
				 __PGTBLHDLR_A64_SETABLE_ATTR_LIST)

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_is_lastlv(const struct __pgtblhdlr_rawucfg_a64_s *cfg)
{
	return __true;
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64page4k_decode_val(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				 __pgtblhdlr_rawfmt_base_t v)
{
	return v & __UL(0xfffffffff000);
}

static __pgtblhdlr_inline __paddr_t
__pgtblhdlr_a64page4k_decode_entry(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				   const struct __pgtblhdlr_rawfmt_a64pgentry_s *raw)
{
	return __pgtblhdlr_a64page4k_decode_val(cfg, raw->v);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64page4k_tblidx_from_key(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
				      __vaddr_t va)
{
	return va & __UL(0xfff);
}

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_a64page4k_tblentry_size(const struct __pgtblhdlr_rawucfg_a64_s *cfg)
{
	return __U(1);
}

static __pgtblhdlr_inline __bool
__pgtblhdlr_a64page4k_key_is_aligned(__vaddr_t va_start, __vaddr_t va_end)
{
	return __true;
}

#define __PGTBL_A64_CONN_ACCEPT_UNCOND(n, raw, nx, rawx)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_##n##_accept_##nx(const struct __pgtblhdlr_rawucfg_a64_s *cfg) \
{ \
	return __true; \
}

__PGTBL_A64_CONN_ACCEPT_UNCOND(a64tbl512g,	a64pgentry,	a64tbl1g,	a64pgentry)
__PGTBL_A64_CONN_ACCEPT_UNCOND(a64tbl512g,	a64pgentry,	a64blk1g,	a64pgentry)
__PGTBL_A64_CONN_ACCEPT_UNCOND(a64tbl1g,	a64pgentry,	a64tbl2m,	a64pgentry)
__PGTBL_A64_CONN_ACCEPT_UNCOND(a64tbl1g,	a64pgentry,	a64blk2m,	a64pgentry)
__PGTBL_A64_CONN_ACCEPT_UNCOND(a64tbl2m,	a64pgentry,	a64page4k,	a64pgentry)

#define __PGTBL_A64_CONN_ACCEPT_TTBRREG(nx, b)	\
static __pgtblhdlr_inline __bool \
__pgtblhdlr_a64ttbrreg_accept_##nx(const struct __pgtblhdlr_rawucfg_a64_s *cfg) \
{ \
	unsigned int va_bits = cfg->va_bits; \
	__bool res; \
	if ((va_bits > __U(b)) && (va_bits <= (__U(b) + __U(9)))) { \
		res = __true; \
	} else { \
		res = __false; \
	} \
	return res; \
}

__PGTBL_A64_CONN_ACCEPT_TTBRREG(a64tbl512g,	39)
__PGTBL_A64_CONN_ACCEPT_TTBRREG(a64tbl1g,	30)
__PGTBL_A64_CONN_ACCEPT_TTBRREG(a64blk1g,	30)
__PGTBL_A64_CONN_ACCEPT_TTBRREG(a64tbl2m,	21)
__PGTBL_A64_CONN_ACCEPT_TTBRREG(a64blk2m,	21)
__PGTBL_A64_CONN_ACCEPT_TTBRREG(a64page4k,	12)

static __pgtblhdlr_inline unsigned long
__pgtblhdlr_key_align(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
		      unsigned long reference)
{
	unsigned long res = reference;

	if (reference == 0ul) {
		res = __UL(1) << cfg->va_bits;
	}
	return res;
}

/*
 * Alignment and range are always same for aarch64
 */
static __pgtblhdlr_inline unsigned long
__pgtblhdlr_key_range(const struct __pgtblhdlr_rawucfg_a64_s *cfg,
		      unsigned long reference)
{
	return __pgtblhdlr_key_align(cfg, reference);
}

/*
 * On arm64, hardware support XN/UXN and PXN, which controls
 * U and K executable separated. Different from ARM.
 */
#define __PGTBLDESC_ARCH_HAS_IS_EXECUTABLE
#define __PGTBLDESC_ARCH_HAS_IS_UEXECUTABLE
#define __PGTBLDESC_ARCH_HAS_IS_KEXECUTABLE

/*
 * Provide some common concept so userspace apps (sysmgr) is able to build
 * its page table processing in a arch-independent way.
 */
#define __PGTBLHDLR_PGENTRY_TYPE_BASE		__PGTBLHDLR_PGENTRY_TYPE_A64TTBRREG
#define __PGTBLHDLR_PGENTRY_TYPE_SMALLEST_BLK	__PGTBLHDLR_PGENTRY_TYPE_A64PAGE4K
#define __PGTBLHDLR_PGENTRY_TYPE_SMALLEST_TBL	__PGTBLHDLR_PGENTRY_TYPE_A64TBL2M
#define __PGTBLHDLR_PGENTRY_TYPE_SMALLEST_HUGE	__PGTBLHDLR_PGENTRY_TYPE_A64BLK2M

#endif /* real guard */

#else  /* !__PGTBLHDLR_CODE */

PGTBLHDLR_RAWCFG(a64)
PGTBLHDLR_RAWFMT(a64ttbrreg)
PGTBLHDLR_RAWFMT(a64pgentry)

PGTBLHDLR_PGENTRY(a64ttbrreg, A64TTBRREG, a64ttbrreg, 0,		0)
PGTBLHDLR_PGENTRY(a64tbl512g, A64TBL512G, a64pgentry, 0x8000000000,	0x8000000000)
PGTBLHDLR_PGENTRY(a64blk1g,   A64BLK1G,   a64pgentry, 0x40000000,	0x40000000)
PGTBLHDLR_PGENTRY(a64tbl1g,   A64TBL1G,   a64pgentry, 0x40000000,	0x40000000)
PGTBLHDLR_PGENTRY(a64blk2m,   A64BLK2M,   a64pgentry, 0x200000,		0x200000)
PGTBLHDLR_PGENTRY(a64tbl2m,   A64TBL2M,   a64pgentry, 0x200000,		0x200000)
PGTBLHDLR_PGENTRY(a64page4k,  A64PAGE4K,  a64pgentry, 0x1000,		0x1000)

PGTBLHDLR_CONN(a64ttbrreg, A64TTBRREG, a64ttbrreg, a64tbl512g, A64TBL512G, a64pgentry)
PGTBLHDLR_CONN(a64ttbrreg, A64TTBRREG, a64ttbrreg, a64blk1g,   A64BLK1G,   a64pgentry)
PGTBLHDLR_CONN(a64ttbrreg, A64TTBRREG, a64ttbrreg, a64tbl1g,   A64TBL1G,   a64pgentry)
PGTBLHDLR_CONN(a64ttbrreg, A64TTBRREG, a64ttbrreg, a64blk2m,   A64BLK2M,   a64pgentry)
PGTBLHDLR_CONN(a64ttbrreg, A64TTBRREG, a64ttbrreg, a64tbl2m,   A64TBL2M,   a64pgentry)
PGTBLHDLR_CONN(a64ttbrreg, A64TTBRREG, a64ttbrreg, a64page4k,  A64PAGE4K,  a64pgentry)

PGTBLHDLR_CONN(a64tbl512g, A64TBL512G, a64pgentry, a64blk1g,   A64BLK1G,   a64pgentry)
PGTBLHDLR_CONN(a64tbl512g, A64TBL512G, a64pgentry, a64tbl1g,   A64TBL1G,   a64pgentry)
PGTBLHDLR_CONN(a64tbl1g,   A64TBL1G,   a64pgentry, a64blk2m,   A64BLK2M,   a64pgentry)
PGTBLHDLR_CONN(a64tbl1g,   A64TBL1G,   a64pgentry, a64tbl2m,   A64TBL2M,   a64pgentry)
PGTBLHDLR_CONN(a64tbl2m,   A64TBL2M,   a64pgentry, a64page4k,  A64PAGE4K,  a64pgentry)

#endif  /* __PGTBLHDLR_CODE */

#endif  /* fake guard */
