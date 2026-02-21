/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Sysmgr vregion stacktrace
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 28 15:18:08 2023
 */
#ifndef SYSMGR_MEM_SYSMGR_PSS_TRIGGER_H
#define SYSMGR_MEM_SYSMGR_PSS_TRIGGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * Loop each vr in sysmgr and print its stacktrace if pss is not zero.
 *
 * @return size of data written if success, E_HM_FBIG if buf too small
 */
int sysmgr_pss_trigger_read(char *buf, size_t len);

#endif
