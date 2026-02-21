
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: als para table ams tcs3743 header file
 * Author: tianyunhang 30013707
 * Create: 2025-3-10
 */

#ifndef __ALS_PARA_TABLE_AMS_TCS3743_H__
#define __ALS_PARA_TABLE_AMS_TCS3743_H__

#include "als_para_table.h"

ams_tcs3743_als_para_table *als_get_ams_tcs3743_table_by_id(uint32_t id);
ams_tcs3743_als_para_table *als_get_ams_tcs3743_first_table(void);
uint32_t als_get_ams_tcs3743_table_count(void);

#endif
