/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of atomic bitops.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 18:10:40 2023
 */
#ifndef ULIBS_LIBALGO_BITOPS_ATOMIC_H
#define ULIBS_LIBALGO_BITOPS_ATOMIC_H

#include <libhmsync/atomic.h>

/*
 * Set the bit mask in memory
 *
 * @mask	[I] Bit mask to set
 *
 * @p		[I] Address
 */
static inline void atomic_set_bit(unsigned int mask, raw_atomic_int_t *p)
{
	int old_val, new_val;

	old_val = raw_atomic_int_read(p);
	if (!(old_val & mask)) {
		new_val = old_val | mask;
		(void)raw_atomic_int_set(p, new_val);
	}
}

/*
 * Clear the bit mask in memory
 *
 * @mask	[I] Bit mask to set
 *
 * @p		[I] Address
 */
static inline void atomic_clear_bit(unsigned int mask, raw_atomic_int_t *p)
{
	int old_val, new_val;

	old_val = raw_atomic_int_read(p);
	if ((old_val & mask) != 0) {
		new_val = old_val & ~mask;
		(void)raw_atomic_int_set(p, new_val);
	}
}

static inline void atomic_clear_bit_long(unsigned long mask, raw_atomic_long_t *p)
{
	long old_val, new_val;

	old_val = raw_atomic_long_read(p);
	if ((old_val & mask) != 0) {
		new_val = old_val & ~mask;
		(void)raw_atomic_long_set(p, new_val);
	}
}

/*
 * Set the bit mask by CAS
 *
 * @mask	[I] Bit mask to set
 *
 * @p		[I] Address
 *
 * @return	Return true if the bit mask is set successful
 */
static inline __always_inline bool atomic_test_and_set_bit(unsigned int mask, raw_atomic_int_t *p)
{
	int old_val, new_val, ret_val;
	bool is_succ = true;

	old_val = raw_atomic_int_read(p);
	if ((old_val & mask) == 0) {
		new_val = old_val | mask;
		ret_val = raw_atomic_int_cas(p, old_val, new_val);
		if (ret_val != old_val) {
			is_succ = false;
		}
	}

	return is_succ;
}

static inline __always_inline bool atomic_test_and_set_bit_long(unsigned long mask, raw_atomic_long_t *p)
{
	long old_val, new_val, ret_val;
	bool is_succ = true;

	old_val = raw_atomic_long_read(p);
	if ((old_val & mask) == 0) {
		new_val = old_val | mask;
		ret_val = raw_atomic_long_cas(p, old_val, new_val);
		if (ret_val != old_val) {
			is_succ = false;
		}
	}

	return is_succ;
}

/*
 * Set the bit mask by CAS
 *
 * @mask	[I] Bit mask to set
 *
 * @p		[I] Address
 *
 * @return	Return true if the bit mask is set successful
 */
static inline __always_inline bool atomic_test_and_clear_bit(unsigned int mask, raw_atomic_int_t *p)
{
	int old_val, new_val, ret_val;
	bool is_succ = true;

	old_val = raw_atomic_int_read(p);
	if ((old_val & mask) != 0) {
		new_val = old_val & ~mask;
		ret_val = raw_atomic_int_cas(p, old_val, new_val);
		if (ret_val != old_val) {
			is_succ = false;
		}
	}

	return is_succ;
}

static inline __always_inline bool atomic_test_and_clear_bit_long(unsigned long mask, raw_atomic_long_t *p)
{
	long old_val, new_val, ret_val;
	bool is_succ = true;

	old_val = raw_atomic_long_read(p);
	if ((old_val & mask) != 0) {
		new_val = old_val & ~mask;
		ret_val = raw_atomic_long_cas(p, old_val, new_val);
		if (ret_val != old_val) {
			is_succ = false;
		}
	}

	return is_succ;
}

/*
 * Determine whether the bit mask is set
 *
 * @mask	[I] Bit mask to set
 *
 * @p		[I] Address
 *
 * Return	return true if the test bit is set
 */
static inline bool atomic_test_bit(unsigned int mask, raw_atomic_int_t *p)
{
	int old_val;

	old_val = raw_atomic_int_read(p);

	return (old_val & mask) != 0;
}

static inline bool atomic_test_bit_long(unsigned long mask, raw_atomic_long_t *p)
{
	long old_val;

	old_val = raw_atomic_long_read(p);

	return (old_val & mask) != 0;
}

#endif /* ULIBS_LIBALGO_BITOPS_ATOMIC_H */
