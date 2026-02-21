/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: als para table hx32062secr header file
 * Author: huangjiaqing <huangjiaqing4@huawei.com>
 * Create: 2023-06-12
 */

#ifndef __ALS_PARA_TABLE_HX32062SECR_H__
#define __ALS_PARA_TABLE_HX32062SECR_H__

#include "als_para_table.h"

hx32062se_als_para_table *als_get_hx32062secr_table_by_id(uint32_t id);
hx32062se_als_para_table *als_get_hx32062secr_first_table(void);
uint32_t als_get_hx32062secr_table_count(void);

#endif /* __ALS_PARA_TABLE_HX32062SECR_H__ */
