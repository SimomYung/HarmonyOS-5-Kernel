/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: als para table stk37670 header file
 * Author: tianyunhang2@huawei.com
 * Create: 2024-05-13
 */

#ifndef __ALS_PARA_TABLE_STK37670_H__
#define __ALS_PARA_TABLE_STK37670_H__

#include "als_para_table.h"

stk37670_als_para_table *als_get_pad_stk37670_table_by_id(uint32_t id);
stk37670_als_para_table *als_get_pad_stk37670_first_table(void);
uint32_t als_get_pad_stk37670_table_count(void);

#endif /* __ALS_PARA_TABLE_STK37670_H__ */
