/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2024. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 14 16:32:33 2024
 */
#ifndef HMASM_REC_SPINLOCK_H
#define HMASM_REC_SPINLOCK_H

#include <hmkernel/types.h>
#include <hmkernel/reclock_struct.h>

#define REC_SPIN_TRIES 10

static inline u32 __do_rec_spinlock_trylock(struct rec_spinlock_s *lock, unsigned int ownerid)
{
	u32 owner, val, tmp, is_owned;
	asm volatile("        prfm    pstl1strm, [%[lock_val]]\n"           /* prepare */
				 "        ldr     %w[owner], [%[ownerid]]\n"
				 "        mvn     %w[is_owned], wzr\n"

				 "1:      ldaxr   %w[val], [%[lock_val]]\n"             /* try with load-acquire */
				 "        sub     %w[tmp], %w[val], %w[is_owned]\n"
				 "        cbnz    %w[tmp], 2f\n"                        /* locked */
				 "        stxr    %w[tmp], %w[owner], [%[lock_val]]\n"
				 "        cbz     %w[tmp], 4f\n"                        /* successful lock ==> done */
				 "        mov     %w[is_owned], wzr\n"                  /* failed to lock ==> rv_value <- false */
				 "        b       4f\n"                                 /* done */

				 "2:      sub     %w[tmp], %w[val], %w[owner]\n"        /* check_recursive */
				 "        cbz     %w[tmp], 3f\n"                        /* is recursive ==> update recursive_cnt */
				 "        mov     %w[is_owned], wzr\n"                  /* failed to lock  ==> rv_value <- false */
				 "        b       4f\n"                                 /* done */

				 "3:      ldr     %w[tmp], [%[lock_cnt]]\n"             /* recursive_cnt++ */
				 "        add     %w[tmp], %w[tmp], #1\n"
				 "        str     %w[tmp], [%[lock_cnt]]\n"             /* done */
				 "4:"

				 : [owner] "=&r"(owner), [val] "=&r"(val), [tmp] "=&r"(tmp), [is_owned] "=&r"(is_owned)
				 : [lock_val] "r"(&lock->owner), [ownerid] "r"(&ownerid), [lock_cnt] "r"(&lock->cnt)
				 : "memory");
	return is_owned;
}

static inline __always_inline void __do_rec_spinlock_lock_wfe(struct rec_spinlock_s *lock, unsigned int ownerid)
{
	u32 owner, val, tmp, avail;
	asm volatile("        prfm    pstl1strm, [%[lock_val]]\n"           /*  prepare */
				 "        ldr     %w[owner], [%[ownerid]]\n"
				 "        mvn     %w[avail], wzr\n"

				 "        sevl\n"                                       /* signal local prevents wait */
				 "0:      wfe\n"                                        /* wait */

				 "1:      ldaxr   %w[val], [%[lock_val]]\n"             /* try with load-acquire */
				 "        sub     %w[tmp], %w[val], %w[avail]\n"
				 "        cbnz    %w[tmp], 2f\n"                        /* in_use */
				 "        stxr    %w[tmp], %w[owner], [%[lock_val]]\n"
				 "        cbnz    %w[tmp], 1b\n"                        /* retry */
				 "        b       3f\n"                                 /* done */

				 "2:      sub     %w[tmp], %w[val], %w[owner]\n"        /* check_recursive */
				 "        cbnz    %w[tmp], 0b\n"                        /* not recursive ==> wait */

				 "        ldr     %w[tmp], [%[lock_cnt]]\n"             /* recursive_cnt++ */
				 "        add     %w[tmp], %w[tmp], #1\n"
				 "        str     %w[tmp], [%[lock_cnt]]\n"             /* done */
				 "3:"

				 : [owner] "=&r"(owner), [val] "=&r"(val), [tmp] "=&r"(tmp), [avail] "=&r"(avail)
				 : [lock_val] "r"(&lock->owner), [ownerid] "r"(&ownerid), [lock_cnt] "r"(&lock->cnt)
				 : "memory");
}

static inline __always_inline void __do_rec_spinlock_spin_lock_wfe(struct rec_spinlock_s *lock, unsigned int ownerid)
{
	u32 spin_count = REC_SPIN_TRIES;

	/* spin */
	while (spin_count > 0) {
		if (__do_rec_spinlock_trylock(lock, ownerid)) {
			return;
		}
		spin_count--;
	}

	/* lock using WFE */
	__do_rec_spinlock_lock_wfe(lock, ownerid);
}

static inline __always_inline void __do_rec_spinlock_unlock_wfe(struct rec_spinlock_s *lock)
{
	u32 val, cnt;
	asm volatile("        prfm    pstl1strm, [%[lock_val]]\n"           /* prepare */
				 "        ldr     %w[cnt], [%[lock_cnt]]\n"
				 "        cbnz    %w[cnt], 1f\n"                        /* recursive_cnt > 0 ==> dec cnt */
				 "        mvn     %w[val], wzr\n"                       /* release lock */
				 "        stlr    %w[val], [%[lock_val]]\n"
				 "        b       2f\n"                                 /* done */

				 "1:      sub     %w[cnt], %w[cnt], #1\n"               /* recursive_cnt-- */
				 "        str     %w[cnt], [%[lock_cnt]]\n"

				 "2:"                                                   /* done */

				 : [val] "=&r"(val), [cnt] "=&r"(cnt)
				 : [lock_val] "r"(&lock->owner), [lock_cnt] "r"(&lock->cnt)
				 : "memory");
}
#endif
