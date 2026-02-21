/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Memory compaction headers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oac 16 11:13:20 2023
 */
#ifndef SYSMGR_INTAPI_MEM_COMPACTION_H
#define SYSMGR_INTAPI_MEM_COMPACTION_H

int compact_memory(void);
int fetch_compact_timeout_threshold(void);
int set_compact_timeout_threshold(int value);

int compact_extfrag_threshold_read(void);
int compact_extfrag_threshold_write(int val);

int compact_enable_read(void);
int compact_enable_write(int value);
#endif /* SYSMGR_INTAPI_MEM_COMPACTION_H */
