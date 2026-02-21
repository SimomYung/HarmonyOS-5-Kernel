/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Seqlock
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 9 09:49:33 2019
 */
#ifndef UAPI_HMKERNEL_SEQLOCK_H
#define UAPI_HMKERNEL_SEQLOCK_H
#include <hmkernel/barrier.h>
#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmasm/processor.h>
#include <hmasm/types.h>

struct __seqcnt_s {
	__u32 __sequence;
};

#define raw_read_seqcnt(sc)				\
({							\
	unsigned seq = __read_once((sc)->__sequence);	\
	__smp_rmb();					\
	seq;						\
})							\

/*
 * raw_seqcnt_begin() - begin a seqcnt read critical section.
 *
 * Unlike __seqcnt_start_read, raw_seqcnt_begin will not wait
 * the seqcnt until it is stabled. Which means, if a writer is
 * active when the macro is called, the caller would fail the
 * seqcnt retry check at the end of the read critical section.
 *
 * Use only in code where performance is a matter of great concern,
 * the read section should be small and not likely to fail.
 */
#define raw_seqcnt_begin(sc)				\
({							\
	raw_read_seqcnt(sc) & ~1;			\
})							\

static inline void __seqcnt_init(struct __seqcnt_s *sc)
{
	sc->__sequence = 0;
}

static inline __u32 ____seqcnt_start_read(const struct __seqcnt_s *sc)
{
	__u32 seq;
	do {
		seq = __read_once(sc->__sequence);
		if ((seq & 1U) == 0U) {
			break;
		}
		cpu_relax();
	} while (__true);
	return seq;
}

static inline __u32 __seqcnt_start_read(const struct __seqcnt_s *sc)
{
	__u32 seq;

	seq = ____seqcnt_start_read(sc);
	__smp_rmb();
	return seq;
}

/*
 * ____seqcnt_retry_read() - end a seqcnt read section barrier.
 *
 * This fundtion is like __seqcnt_retry_read, but without smp_rmb()
 * barrier. Caller should make sure that the correct ordering is
 * followed before loading the variables that are protected with this
 * critical section.
 *
 * Use carefully only on critical code sensitive to performance.
 */
static inline __s32 ____seqcnt_retry_read(const struct __seqcnt_s *sc, __u32 seq)
{
	return __unlikely(sc->__sequence != seq);
}

static inline __s32 __seqcnt_retry_read(const struct __seqcnt_s *sc, __u32 seq)
{
	__smp_rmb();
	return ____seqcnt_retry_read(sc, seq);
}

static inline void __seqcnt_begin_write(struct __seqcnt_s *sc)
{
	sc->__sequence++;
	__smp_wmb();
}

static inline void __seqcnt_end_write(struct __seqcnt_s *sc)
{
	__smp_wmb();
	sc->__sequence++;
}
#endif
