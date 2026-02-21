/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Zero page deduplicate
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 13 10:15:08 2023
 */
#ifndef SYSMGR_MEM_ZEROPG_DEDUP_H
#define SYSMGR_MEM_ZEROPG_DEDUP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * Initialize the zero page deduplicate thread.
 * Note the thread will block after this, call
 * zpd_set_enabled to enable it manually.
 *
 * @return void
 */
void zpd_thread_init(void);

/*
 * Update the sleep interval between two run
 *
 * @param interval	[I] interval in milliseconds
 * @return E_HM_OK on success, E_HM_INVAL if interval less than MIN_INTERVAL
 */
int zpd_write_interval(uint32_t interval);

uint32_t zpd_read_interval(void);

/*
 * Update the number of pages to scan each time
 *
 * @param step	[I] number of pages to scan
 * @return E_HM_OK on success, E_HM_INVAL if step larger than MAX_STEP
 */
int zpd_write_step(uint32_t step);

uint32_t zpd_read_step(void);

void zpd_set_enabled(bool enable);

bool zpd_is_enabled(void);

/*
 * Write statistics info of mapping and pages deduped into data
 *
 * @return size of data written if success, E_HM_FBIG if buf too small
 */
int zpd_stat(char *buf, size_t len);

#endif
