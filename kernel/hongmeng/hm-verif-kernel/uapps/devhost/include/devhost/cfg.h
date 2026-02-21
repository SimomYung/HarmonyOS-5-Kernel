/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Parse config for devhost
 * Author: Huawei OS Kernel Lab
 * Create: Tues May 23 10:45:12 2022
 */

#ifndef __DEVHOST_CFG_H__
#define __DEVHOST_CFG_H__

#define MAX_POOL_NUM 8
#define POOL_NAME_LEN 16

enum devhost_pool_type {
	DEVHOST_POOL_TYPE_NONE,
	DEVHOST_POOL_TYPE_MEM,
	DEVHOST_POOL_TYPE_CMA,
	DEVHOST_POOL_TYPE_DISCRETE,
	DEVHOST_POOL_TYPE_MAX,
};

enum devhost_rsv_type {
	DEVHOST_RSV_TYPE_NONE,
	DEVHOST_RSV_TYPE_ST,
	DEVHOST_RSV_TYPE_DYN,
};

struct pool_info {
	char name[POOL_NAME_LEN];
	unsigned int type;
	unsigned int sizeorder;
	unsigned int reclaimorder;
	unsigned int rsv;
	unsigned int high_watermark;
	unsigned int low_watermark;
};

/*
 * Devhost get page pool number.
 *
 * @return page pool number.
 */
unsigned int devhost_get_pool_num(void);

/*
 * Devhost get page pool info struct.
 *
 * @param pool_idx	[I] index of pool in pool group
 *
 * @return pool_info pointers.
 */
struct pool_info *devhost_get_pool_info(unsigned int pool_idx);

/*
 * Devhost get page pool type.
 *
 * @param pool_idx	[I] index of pool in pool group
 *
 * @return pool type.
 */
enum devhost_pool_type devhost_get_pool_type(unsigned int pool_idx);

/*
 * Devhost parse page pool config.
 *
 * @param buf		[I] buf of config
 *
 * @return void.
 */
void devhost_parse_pools(char* buf);

/*
 * Devhost get pool total watermark.
 *
 * @return pool total watermark.
 */
unsigned int devhost_get_poolcfg_total_watermark(void);

/*
 * Devhost parse pool config.
 *
 * @param buf		[I] buf of config
 *
 * @return void.
 */
void devhost_parse_poolcfg(char* buf);

#endif /* __DEVHOST_API_BACKEND_H__ */
