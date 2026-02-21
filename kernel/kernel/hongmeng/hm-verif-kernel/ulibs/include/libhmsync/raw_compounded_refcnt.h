/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: compounded refcnt ops
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 10 17:05:22 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_COMPOUNDED_REFCNT_H
#define ULIBS_INCLUDE_LIBHMSYNC_COMPOUNDED_REFCNT_H

#include <vsync/atomic.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>

#define REFCNT_SHIFT	40
#define RPCCNT_MASK	0x000000ffffffffff
#define REFCNT_MASK	0xffffff0000000000
#define REFCNT_MAX	0x7fffff
#define RPCCNT_MIN	0x8000000000
#define RPCCNT_MAX	0x7fffffffff
#define RELEASED_MAGIC	0xffffffffffffffff

struct raw_compounded_refcnt {
	/*
	 * XXX:
	 * Now, use lower 40bits to store rpccnt,
	 * higher 24bits to store refcnt,
	 * both of them are signed number.
	 * If 24bits or 40bits is not enough to cover their range,
	 * we will fixup this issue.
	 */
	vatomic64_t v;
};

static inline void
raw_compounded_refcnt_init(struct raw_compounded_refcnt *refcnt, uint64_t v)
{
	uint64_t _v = ((v << REFCNT_SHIFT) & REFCNT_MASK);

	BUG_ON(v >= (uint64_t)REFCNT_MAX);
	vatomic64_init(&refcnt->v, _v);
}

static inline uint64_t __raw_compounded_v2refcnt(uint64_t v)
{
	return ((v & REFCNT_MASK) >> REFCNT_SHIFT);
}
static inline int
raw_compounded_refcnt_of(struct raw_compounded_refcnt *refcnt, uint64_t *res)
{
	uint64_t v = vatomic64_read(&refcnt->v);
	if (unlikely(v == RELEASED_MAGIC)) {
		return E_HM_POSIX_NOENT;
	}

	if (res != NULL) {
		*res = __raw_compounded_v2refcnt(v);
	}

	return E_HM_OK;
}

static inline int
raw_compounded_rpccnt_put(struct raw_compounded_refcnt *refcnt, uint64_t *res)
{
	uint64_t oldv, newv, rpccnt;

	do {
		oldv = vatomic64_read(&refcnt->v);
		if (unlikely(oldv == RELEASED_MAGIC)) {
			return E_HM_POSIX_NOENT;
		}
		rpccnt = (oldv & RPCCNT_MASK);
		if (unlikely(rpccnt == RPCCNT_MIN)) {
			if (res != NULL) {
				*res = oldv;
			}
			return E_HM_OVERFLOW;
		}
		rpccnt--;
		newv = (oldv & REFCNT_MASK) | (rpccnt & RPCCNT_MASK);
		newv = (newv == 0) ? RELEASED_MAGIC : newv;
	} while (vatomic64_cmpxchg(&refcnt->v, oldv, newv) != oldv);

	if (res != NULL) {
		*res = (newv == RELEASED_MAGIC) ? 0ULL : newv;
	}
	return E_HM_OK;
}

static inline int
raw_compounded_rpccnt_balance(struct raw_compounded_refcnt *refcnt,
			      uint64_t cnt, uint64_t *res)
{
	uint64_t oldv, newv, rpccnt;

	/*
	 * cnt should be a non-negative number,
	 * and shoud not be bigger than RPCCNT_MAX.
	 */
	BUG_ON(cnt > RPCCNT_MAX);
	do {
		oldv = vatomic64_read(&refcnt->v);
		if (unlikely(oldv == RELEASED_MAGIC)) {
			return E_HM_POSIX_NOENT;
		}
		rpccnt = (oldv & RPCCNT_MASK);
		/*
		 * rpccnt is almost always a non-positive number,
		 * if it is positive number(rpccnt < RPCCNT_MIN),
		 * and (rpccnt + cnt) is bigger than RPCCNT_MAX,
		 * somthing is wrong.
		 */
		if (unlikely(rpccnt < RPCCNT_MIN) &&
		    unlikely(rpccnt + cnt > RPCCNT_MAX)) {
			if (res != NULL) {
				*res = oldv;
			}
			return E_HM_OVERFLOW;
		}
		rpccnt += cnt;
		newv = (oldv & REFCNT_MASK) | (rpccnt & RPCCNT_MASK);
		newv = (newv == 0) ? RELEASED_MAGIC : newv;
	} while (vatomic64_cmpxchg(&refcnt->v, oldv, newv) != oldv);

	if (res != NULL) {
		*res = (newv == RELEASED_MAGIC) ? 0ULL : newv;
	}
	return E_HM_OK;
}

static inline int
raw_compounded_refcnt_get(struct raw_compounded_refcnt *refcnt, uint64_t *res)
{
	uint64_t oldv, newv, cnt;

	do {
		oldv = vatomic64_read(&refcnt->v);
		if (unlikely(oldv == RELEASED_MAGIC)) {
			return E_HM_POSIX_NOENT;
		}
		cnt = ((oldv & REFCNT_MASK) >> REFCNT_SHIFT);
		if (unlikely(cnt == (uint64_t)REFCNT_MAX)) {
			if (res != NULL) {
				*res = oldv;
			}
			return E_HM_OVERFLOW;
		}
		cnt++;
		newv = (oldv & RPCCNT_MASK) | (cnt << REFCNT_SHIFT);
	} while (vatomic64_cmpxchg(&refcnt->v, oldv, newv) != oldv);

	if (res != NULL) {
		*res = newv;
	}
	return E_HM_OK;
}

static inline int
raw_compounded_refcnt_put(struct raw_compounded_refcnt *refcnt, uint64_t *res)
{
	uint64_t oldv, newv, cnt;

	do {
		oldv = vatomic64_read(&refcnt->v);
		if (unlikely(oldv == RELEASED_MAGIC)) {
			return E_HM_POSIX_NOENT;
		}
		cnt = ((oldv & REFCNT_MASK) >> REFCNT_SHIFT);
		if (unlikely(cnt == 0ULL)) {
			if (res != NULL) {
				*res = oldv;
			}
			return E_HM_OVERFLOW;
		}
		cnt--;
		newv = (oldv & RPCCNT_MASK) | (cnt << REFCNT_SHIFT);
		newv = (newv == 0) ? RELEASED_MAGIC : newv;
	} while (vatomic64_cmpxchg(&refcnt->v, oldv, newv) != oldv);

	if (res != NULL) {
		*res = (newv == RELEASED_MAGIC) ? 0ULL : newv;
	}
	return E_HM_OK;
}
#endif
