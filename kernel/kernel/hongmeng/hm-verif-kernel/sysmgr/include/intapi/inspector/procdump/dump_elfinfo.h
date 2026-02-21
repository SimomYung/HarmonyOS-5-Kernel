/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Provide dump elfinfo interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 11 14:05:03 CST 2023
 */

#ifndef SYSMGR_INTAPI_INSPECTOR_PROCDUMP_DUMP_ELFINFO_H
#define SYSMGR_INTAPI_INSPECTOR_PROCDUMP_DUMP_ELFINFO_H

void elfinfo_dump_save_load_addr(const char* process_name, uint64_t load_addr);

#endif
