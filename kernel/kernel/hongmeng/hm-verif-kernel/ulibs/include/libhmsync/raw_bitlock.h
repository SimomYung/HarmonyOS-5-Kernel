/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: implement the raw bitlock which will use only two bits to implement the lock in userspace.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 14:08:30 2023
 */

#ifndef ULIBS_RAW_BITLOCK_H
#define ULIBS_RAW_BITLOCK_H

/* the max bit of an int num is 32 */
#define INVALID_BITS_NUM		33
int raw_bitlock_trylock(int *addr, int lock_bit, int wait_bit);
int raw_bitlock_lock(int *addr, int lock_bit, int wait_bit, unsigned long long *timeout);
int raw_bitlock_unlock(int *addr, int lock_bit, int wait_bit);

int raw_onebit_trylock(int *addr, int lock_bit);
int raw_onebit_lock(int *addr, int lock_bit);
int raw_onebit_unlock(int *addr, int lock_bit);
int raw_bitlock_locked(int *addr, int lock_bit, int wait_bit);
int raw_onebit_locked(int *addr, int lock_bit);

void disable_raw_bitlock(void);
void enable_raw_bitlock(void);
_Bool is_raw_bitlock_disabled(void);

/* raw bit-based spinlock is for performance which disables preempt
 * unless you really need it.
 * Note *bitlock_spin_[un]lock* is independent of normal *bitlock_[un]lock*.
 * Do not make misused.
 */
int raw_bitlock_spin_lock(int *addr, int lock_bit);
int raw_bitlock_spin_unlock(int *addr, int lock_bit);

#endif
