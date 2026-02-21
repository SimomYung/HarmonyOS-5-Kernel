/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: als para table ucs116e0 header file
 * Author: wangmenghui 30062392
 * Create: 2024-10-23
 */
 
#ifndef __ALS_PARA_TABLE_UCS116E0_H__
#define __ALS_PARA_TABLE_UCS116E0_H__
 
#include "als_para_table.h"
 
ucs116e0_als_para_table *als_get_ucs116e0_table_by_id(uint32_t id);
ucs116e0_als_para_table *als_get_ucs116e0_first_table(void);
uint32_t als_get_ucs116e0_table_count(void);
 
#endif /* __ALS_PARA_TABLE_UCS116E0_H__ */