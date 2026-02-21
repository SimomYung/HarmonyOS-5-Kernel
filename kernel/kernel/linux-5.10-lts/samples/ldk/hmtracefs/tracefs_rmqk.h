/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_TRACEFS_RMQK_H__
#define __LIBDH_LINUX_TRACEFS_RMQK_H__
uintptr_t hmtracefs_record_shm_addr_of(void);

int hmtracefs_rmq_is_enabled(void);
uint64_t hmtracefs_rmq_enqueue_get_entry(uintptr_t rmq_addr, uint64_t len, void *ppe);
void hmtracefs_rmq_enqueue_confirm_entry(void *ppe);

void hmtracefs_rmq_dequeue_futex_wake(void);
#endif /* __LIBDH_LINUX_TRACEFS_RMQK_H__ */
