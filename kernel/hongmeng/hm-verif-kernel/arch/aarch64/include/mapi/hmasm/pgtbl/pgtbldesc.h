/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: desc of arm64 page table
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 02 19:12:48 2021
 */

/* fake guard for code scanner */
#ifndef AARCH64_MAPI_ASM_PGTBLDESC_H
#define AARCH64_MAPI_ASM_PGTBLDESC_H

#undef AARCH64_MAPI_ASM_PGTBLDESC_H

#include <uapi/hmasm/pgtbl/pgtbldesc.h>

/* Real guard in __PGTBLHDLR_CODE block */
#ifndef AARCH64_MAPI_ASM_PGTBLDESC_H__CODE
#define AARCH64_MAPI_ASM_PGTBLDESC_H__CODE

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/errno.h>
#include <hmkernel/strict.h>

#define PGTBLHDLR_PGENTRY_TYPE_BASE		__PGTBLHDLR_PGENTRY_TYPE_BASE
#define PGTBLHDLR_PGENTRY_TYPE_SMALLEST_BLK	__PGTBLHDLR_PGENTRY_TYPE_SMALLEST_BLK
#define PGTBLHDLR_PGENTRY_TYPE_SMALLEST_TBL	__PGTBLHDLR_PGENTRY_TYPE_SMALLEST_TBL
#define PGTBLHDLR_PGENTRY_TYPE_SMALLEST_HUGE	__PGTBLHDLR_PGENTRY_TYPE_SMALLEST_HUGE

/*
 * HL means high level, different from rawcfg. hlattrs are not able
 * to directlly translated into page table bits when define scenario.
 * For example, va_bits and ttbcr_n, which is different between kernel
 * and user mapping, but programmer should not know the exact value
 * when define scenario like kdata / ucode.
 */
struct __pgtblhdlr_hlattr_a64_s {
	enum __pgtblhdlr_hlattr_a64_setting_e {
		__PGTBLHDLR_HLATTR_A64_SETTING_KERNEL,
		__PGTBLHDLR_HLATTR_A64_SETTING_USER,
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE,
		__PGTBLHDLR_HLATTR_A64_SETTING_ELFLOADER,
		__PGTBLHDLR_HLATTR_A64_SETTING_S2_NORMAL,
		__PGTBLHDLR_HLATTR_A64_SETTING_S2_DEV,
	} setting;
};

struct __pgtblhdlr_rawcfg_a64_s {
	/* Basic page table walk cfg */
	struct __pgtblhdlr_rawucfg_a64_s ucfg;
	struct __pgtblhdlr_hlattr_a64_s hlattr;
	struct {
		__u64 a64ttbrreg;
		__u64 a64tbl512g;
		__u64 a64tbl1g;
		__u64 a64tbl2m;
		__u64 a64blk1g;
		__u64 a64blk2m;
		__u64 a64page4k;
	} attrbits;
};

#define __PGTBL_A64_CFG_UPDATE(f) \
static __pgtblhdlr_inline void \
__pgtblhdlr_rawcfg_set_##f(struct __pgtblhdlr_rawcfg_a64_s *cfg, __u64 setbits, __u64 setval) \
{ \
	cfg->attrbits.f &= (~setbits);			\
	cfg->attrbits.f |= (setval);				\
}

__PGTBL_A64_CFG_UPDATE(a64ttbrreg)
__PGTBL_A64_CFG_UPDATE(a64tbl512g)
__PGTBL_A64_CFG_UPDATE(a64tbl1g)
__PGTBL_A64_CFG_UPDATE(a64tbl2m)
__PGTBL_A64_CFG_UPDATE(a64blk1g)
__PGTBL_A64_CFG_UPDATE(a64blk2m)
__PGTBL_A64_CFG_UPDATE(a64page4k)

#define __PGTBL_A64_COMMON_TBL_SET_LEVEL(sz)	\
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_a64tbl##sz##_prepare_val(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa) \
{ \
	return cfg->attrbits.a64tbl##sz | (pa & __UL(0xfffffffff000)); \
} \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_a64tbl##sz##_prepare_val_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa) \
{ \
	return __pgtblhdlr_a64tbl##sz##_prepare_val(cfg, pa); \
} \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_a64tbl##sz##_prepare_soft_val(__pgtblhdlr_rawfmt_base_t soft_val) \
{ \
	return soft_val & __UL(0xfffffffffffffffc); \
} \
static __pgtblhdlr_inline \
void __pgtblhdlr_a64tbl##sz##_populate_entry(const struct __pgtblhdlr_rawcfg_a64_s *cfg, \
					     struct __pgtblhdlr_rawfmt_a64pgentry_s *r, \
					     __paddr_t pa) \
{ \
	r->v = __pgtblhdlr_a64tbl##sz##_prepare_val(cfg, pa); \
} \
static __pgtblhdlr_inline \
void __pgtblhdlr_a64tbl##sz##_populate_entry_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg, \
						     struct __pgtblhdlr_rawfmt_a64pgentry_s *r, \
						     __paddr_t pa) \
{ \
	__pgtblhdlr_a64tbl##sz##_populate_entry(cfg, r, pa); \
} \

__PGTBL_A64_COMMON_TBL_SET_LEVEL(512g)
__PGTBL_A64_COMMON_TBL_SET_LEVEL(1g)
__PGTBL_A64_COMMON_TBL_SET_LEVEL(2m)

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64ttbrreg_prepare_val(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64ttbrreg | (pa & __UL(0xfffffffffffe));
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64ttbrreg_prepare_val_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return __pgtblhdlr_a64ttbrreg_prepare_val(cfg, pa);
}

/*
 * TTBR doesn't support softval: all values in TTBR has its meaning.
 */
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64ttbrreg_prepare_soft_val(__pgtblhdlr_rawfmt_base_t soft_val)
{
	return 0ul;
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64ttbrreg_populate_entry(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
					   struct __pgtblhdlr_rawfmt_a64ttbrreg_s *r,
					   __paddr_t pa)
{
	r->v = __pgtblhdlr_a64ttbrreg_prepare_val(cfg, pa);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64ttbrreg_populate_entry_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
						   struct __pgtblhdlr_rawfmt_a64ttbrreg_s *r,
						   __paddr_t pa)
{
	__pgtblhdlr_a64ttbrreg_populate_entry(cfg, r, pa);
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64blk1g_prepare_val(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64blk1g | (pa & __UL(0xffffc0000000));
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64blk1g_prepare_val_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return __pgtblhdlr_a64blk1g_prepare_val(cfg, pa) | __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING;
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64blk1g_prepare_soft_val(__pgtblhdlr_rawfmt_base_t soft_val)
{
	return soft_val & __UL(0xfffffffffffffffc);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64blk1g_populate_entry(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
				       struct __pgtblhdlr_rawfmt_a64pgentry_s *r,
				       __paddr_t pa)
{
	r->v = __pgtblhdlr_a64blk1g_prepare_val(cfg, pa);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64blk1g_populate_entry_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
						 struct __pgtblhdlr_rawfmt_a64pgentry_s *r,
						 __paddr_t pa)
{
	r->v = __pgtblhdlr_a64blk1g_prepare_val_fragile(cfg, pa);
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64blk2m_prepare_val(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64blk2m | (pa & __UL(0xffffffe00000));
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64blk2m_prepare_val_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return __pgtblhdlr_a64blk2m_prepare_val(cfg, pa) | __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING;
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64blk2m_prepare_soft_val(__pgtblhdlr_rawfmt_base_t soft_val)
{
	return soft_val & __UL(0xfffffffffffffffc);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64blk2m_populate_entry(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
				       struct __pgtblhdlr_rawfmt_a64pgentry_s *r,
				       __paddr_t pa)
{
	r->v = __pgtblhdlr_a64blk2m_prepare_val(cfg, pa);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64blk2m_populate_entry_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
						 struct __pgtblhdlr_rawfmt_a64pgentry_s *r,
						 __paddr_t pa)
{
	r->v = __pgtblhdlr_a64blk2m_prepare_val_fragile(cfg, pa);
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64page4k_prepare_val(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return cfg->attrbits.a64page4k | (pa & __UL(0xfffffffff000));
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64page4k_prepare_val_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg, __paddr_t pa)
{
	return __pgtblhdlr_a64page4k_prepare_val(cfg, pa) | __PGTBLATTR_A64UPPERATTR_SW_FRAGILE_MAPPING;
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__pgtblhdlr_a64page4k_prepare_soft_val(__pgtblhdlr_rawfmt_base_t soft_val)
{
	return soft_val & __UL(0xfffffffffffffffc);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64page4k_populate_entry(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
					struct __pgtblhdlr_rawfmt_a64pgentry_s *r,
					__paddr_t pa)
{
	r->v = __pgtblhdlr_a64page4k_prepare_val(cfg, pa);
}

static __pgtblhdlr_inline
void __pgtblhdlr_a64page4k_populate_entry_fragile(const struct __pgtblhdlr_rawcfg_a64_s *cfg,
						  struct __pgtblhdlr_rawfmt_a64pgentry_s *r,
						  __paddr_t pa)
{
	r->v = __pgtblhdlr_a64page4k_prepare_val_fragile(cfg, pa);
}

static __pgtblhdlr_inline
void __pgtblhdlr_rawfmt_a64ttbrreg_zero(struct __pgtblhdlr_rawfmt_a64ttbrreg_s *r)
{
	r->v = __UL(0);
}

static __pgtblhdlr_inline
void __pgtblhdlr_rawfmt_a64pgentry_zero(struct __pgtblhdlr_rawfmt_a64pgentry_s *r)
{
	r->v = __UL(0);
}

/*
 * It would be better to define these tbl_pgstr_type functions in PGTBLHDLR_PGENTRY table
 * howevet it require us change nearly 45 places which define PGTBLHDLR_PGENTRY.
 * In future we can chage most of PGTBLHDLR_PGENTRY macro definition to
 *
 * #define PGTBLHDLR_PGENTRY(x...)
 *
 */
static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64ttbrreg_tbl_pgstr_type(bool is_kmap)
{
	UNUSED(is_kmap);
	return PGSTR_TYPE_PGTBL_FL;
}

static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64tbl512g_tbl_pgstr_type(bool is_kmap)
{
	UNUSED(is_kmap);
	return PGSTR_TYPE_PGTBL_ML1;
}

static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64blk1g_tbl_pgstr_type(bool is_kmap)
{
	UNUSED(is_kmap);
	return PGSTR_TYPE_INVALID;
}

static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64tbl1g_tbl_pgstr_type(bool is_kmap)
{
	UNUSED(is_kmap);
	return PGSTR_TYPE_PGTBL_ML2;
}

static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64blk2m_tbl_pgstr_type(bool is_kmap)
{
	UNUSED(is_kmap);
	return PGSTR_TYPE_INVALID;
}

static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64tbl2m_tbl_pgstr_type(bool is_kmap)
{
	return is_kmap ? PGSTR_TYPE_PGTBL_KLL : PGSTR_TYPE_PGTBL_ULL;
}

static __pgtblhdlr_inline enum pgstr_type_e
__pgtblhdlr_a64page4k_tbl_pgstr_type(bool is_kmap)
{
	UNUSED(is_kmap);
	return PGSTR_TYPE_INVALID;
}

/*
 * Define atomic operations on page table entry (rawfmt):
 * atomic operation: (read, write, cmpxchg) X (_seq, _rlx, _acq, _rel)
 * page table entry: 2 types for arm64.
 *
 *                   read_seq read_rlx read_acq write_seq write_rel cmpxchg_seq
 * rawfmt_a64ttbrreg
 * rawfmt_a64pgentry
 *
 * Use macro to generate 12 functions
 */

static __pgtblhdlr_inline u64 atomic64_read_noatomic(atomic64_t *v)
{
	return v->_v;
}

static __pgtblhdlr_inline void atomic64_write_noatomic(atomic64_t *v, u64 _v)
{
	v->_v = _v;
}

static __pgtblhdlr_inline u64 atomic64_cmpxchg_noatomic(atomic64_t *v, u64 _oldv, u64 _newv)
{
	u64 retv = v->_v;
	if (retv == _oldv) {
		v->_v = _newv;
	}
	return retv;
}

/*
 * ttbrreg are always private to caller because they are usually virtually
 * created. No need to use real atomic instructions.
 */
#define define_rawfmt_a64ttbrreg_atomic_read(mo) \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_rawfmt_a64ttbrreg_atomic_read##mo(struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw) \
{ \
	return (__pgtblhdlr_rawfmt_base_t)(raw->v); \
}

#define define_rawfmt_a64ttbrreg_atomic_write(mo) \
static __pgtblhdlr_inline int \
__pgtblhdlr_rawfmt_a64ttbrreg_atomic_write##mo(struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw, \
					       __pgtblhdlr_rawfmt_base_t v) \
{ \
	raw->v = (u64)(v); \
	return 0; \
}

#define define_rawfmt_a64ttbrreg_atomic_cmpxchg(mo) \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_rawfmt_a64ttbrreg_atomic_cmpxchg##mo(struct __pgtblhdlr_rawfmt_a64ttbrreg_s *raw, \
						 __pgtblhdlr_rawfmt_base_t oldv, \
						 __pgtblhdlr_rawfmt_base_t newv) \
{ \
	__pgtblhdlr_rawfmt_base_t _oldv = (__pgtblhdlr_rawfmt_base_t)(raw->v); \
	if (_oldv == oldv) { \
		raw->v = (u64)newv; \
	} \
	return _oldv; \
}

define_rawfmt_a64ttbrreg_atomic_read(_seq)
define_rawfmt_a64ttbrreg_atomic_read(_rlx)
define_rawfmt_a64ttbrreg_atomic_read(_acq)
define_rawfmt_a64ttbrreg_atomic_read(_noatomic)
define_rawfmt_a64ttbrreg_atomic_write(_seq)
define_rawfmt_a64ttbrreg_atomic_write(_rlx)
define_rawfmt_a64ttbrreg_atomic_write(_rel)
define_rawfmt_a64ttbrreg_atomic_write(_noatomic)
define_rawfmt_a64ttbrreg_atomic_cmpxchg(_seq)
define_rawfmt_a64ttbrreg_atomic_cmpxchg(_noatomic)

#define define_rawfmt_a64pgentry_atomic_read(mo, amo) \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_rawfmt_a64pgentry_atomic_read##mo(struct __pgtblhdlr_rawfmt_a64pgentry_s *raw) \
{ \
	return atomic64_read##amo((atomic64_t *)(void *)(&raw->v)); \
}

#define define_rawfmt_a64pgentry_atomic_write(mo, amo) \
static __pgtblhdlr_inline int \
__pgtblhdlr_rawfmt_a64pgentry_atomic_write##mo(struct __pgtblhdlr_rawfmt_a64pgentry_s *raw, \
					       __pgtblhdlr_rawfmt_base_t v) \
{ \
	atomic64_write##amo((atomic64_t *)(void *)(&raw->v), (u64)(v)); \
	return 0; \
}

#define define_rawfmt_a64pgentry_atomic_cmpxchg(mo, amo) \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__pgtblhdlr_rawfmt_a64pgentry_atomic_cmpxchg##mo(struct __pgtblhdlr_rawfmt_a64pgentry_s *raw, \
						 __pgtblhdlr_rawfmt_base_t oldv, \
						 __pgtblhdlr_rawfmt_base_t newv) \
{ \
	return (__pgtblhdlr_rawfmt_base_t)atomic64_cmpxchg##amo((atomic64_t *)(void *)(&raw->v), \
								(u64)(oldv), (u64)(newv)); \
}

define_rawfmt_a64pgentry_atomic_read(_seq, )
define_rawfmt_a64pgentry_atomic_read(_rlx, _rlx)
define_rawfmt_a64pgentry_atomic_read(_acq, _acq)
define_rawfmt_a64pgentry_atomic_read(_noatomic, _noatomic)
define_rawfmt_a64pgentry_atomic_write(_seq, )
define_rawfmt_a64pgentry_atomic_write(_rlx, _rlx)
define_rawfmt_a64pgentry_atomic_write(_rel, _rel)
define_rawfmt_a64pgentry_atomic_write(_noatomic, _noatomic)
define_rawfmt_a64pgentry_atomic_cmpxchg(_seq, )
define_rawfmt_a64pgentry_atomic_cmpxchg(_noatomic, _noatomic)

#endif /* real guard */

#endif
