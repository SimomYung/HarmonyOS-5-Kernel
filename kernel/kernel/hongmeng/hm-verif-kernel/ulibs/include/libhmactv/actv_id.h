/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Actv ID
 * Author: Huawei OS Kernel Lab
 * Create: Thi Nov 13 11:43:11 2019
 */

#ifndef ULIBS_INCLUDE_LIBHMACTV_ACTV_ID_H
#define ULIBS_INCLUDE_LIBHMACTV_ACTV_ID_H

#include <stdbool.h>

#define ACTV_MAX_ID_COUNT (65536U)
#define ACTV_IDPOOL_OFFSET (ACTV_MAX_ID_COUNT + 128U)
#define actv_bitmapid_to_id(bitmapid) (ACTV_IDPOOL_OFFSET + (bitmapid))
#define actv_id_to_bitmapid(id) ((id) - ACTV_IDPOOL_OFFSET)

/*
 *  resv actv id is used for actv which needs self initialization.
 *
 *  resv actv id, actv id and thread id should be separated and their
 *  allocation as follows.
 *
 *  +---------------------+  <-- 0
 *  |    thread idpool    |
 *  +---------------------+  <-- ACTV_IDPOOL_OFFSET
 *  |     actv idpool     |
 *  +---------------------+  <-- ACTV_IDPOOL_OFFSET + ACTV_MAX_ID_COUNT = RESV_ACTV_ID_OFFSET
 *  | resv actv idpool 0  |
 *  +---------------------+  <-- RESV_ACTV_ID_OFFSET + (1 * RESV_ACTV_MAX_ID_COUNT)
 *  | resv actv idpool 1  |
 *  +---------------------+  <-- RESV_ACTV_ID_OFFSET + (2 * RESV_ACTV_MAX_ID_COUNT)
 *  |       ......        |
 *  +---------------------+  <-- RESV_ACTV_ID_OFFSET + (15 * RESV_ACTV_MAX_ID_COUNT)
 *  | resv actv idpool 15 |
 *  +---------------------+  <-- RESC_ACTV_ID_OFFSET + (16 * RESV_ACTV_MAX_ID_COUNT)
 */
#define RESV_ACTV_ID_OFFSET (ACTV_IDPOOL_OFFSET + ACTV_MAX_ID_COUNT)
/* Now support max 16 resv idpool for each process */
#define RESV_ACTV_IDPOOL_MAX_NUM 16
/* max num of resv actv id in each pool */
#define RESV_ACTV_MAX_ID_COUNT 4096
#define resv_actv_bitmapid_to_id(poolid, bitmapid)	\
	(RESV_ACTV_ID_OFFSET + ((poolid) * RESV_ACTV_MAX_ID_COUNT) + (bitmapid))
#define resv_actv_id_to_bitmapid(actvid)		\
	(((actvid) - RESV_ACTV_ID_OFFSET) % RESV_ACTV_MAX_ID_COUNT)
#define resv_actv_id_to_poolid(actvid)			\
	(((actvid) - RESV_ACTV_ID_OFFSET) / RESV_ACTV_MAX_ID_COUNT)

unsigned int actv_id_alloc(void);
void actv_id_free(unsigned int actv_id);
unsigned int actv_id_min(void);

bool resv_actv_idpool_is_inited(void);
void resv_actv_idpool_init(void);
int actv_pure_id_alloc(unsigned int pool_id, unsigned int *actv_id);
int actv_pure_id_free(unsigned int actv_id);

#endif
