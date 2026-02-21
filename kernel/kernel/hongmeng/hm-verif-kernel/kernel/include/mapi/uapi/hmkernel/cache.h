/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of cache
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 29 13:58:42 2019
 */
#ifndef UAPI_HMKERNEL_CACHE_H
#define UAPI_HMKERNEL_CACHE_H

#define FLUSH_CACHE_MAX_SIZE (1UL << 21)

/* for sysfast_flush_cache */
#define __CACHE_FLUSH_RANGE	0U
#define __DCACHE_FLUSH_RANGE	1U
#define __ICACHE_FLUSH_RANGE	2U
#define __DCACHE_CLEAN_RANGE	3U
#define __DCACHE_INVAL_RANGE	4U

/*
 * for sysfast_ext_flush_cache_all
 * NOTE: dcache inval all will invalidate stack push operation,
 * so it is not supported.
 */
#define __CACHE_FLUSH_ALL	5U
#define __DCACHE_FLUSH_ALL	6U
#define __ICACHE_FLUSH_ALL	7U
#define __DCACHE_CLEAN_ALL	8U

#endif
