/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost log output
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 12 14:31:35 2019
 */
#ifndef __DEVHOST_API_LOG_H__
#define __DEVHOST_API_LOG_H__

#ifdef log_fmt
#undef log_fmt
#endif
#define log_fmt(fmt) "DEVHOST/%d: " fmt, dh_log_ident()

#include <libhmlog/hmlog.h>

extern int dh_log_ident(void);

#define dh_debug(fmt, ...) \
	hm_debug(fmt, ##__VA_ARGS__)
#define dh_info(fmt, ...) \
	hm_info(fmt, ##__VA_ARGS__)
#define dh_warn(fmt, ...) \
	hm_warn(fmt, ##__VA_ARGS__)
#define dh_error(fmt, ...) \
	hm_error(fmt, ##__VA_ARGS__)
#define dh_panic(fmt, ...) \
	hm_panic(fmt, ##__VA_ARGS__)

#endif /* __DEVHOST_API_LOG_H__ */
