/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Header for kcov
 * Author: Huawei OS Kernel Lab
 * Create: Sun May 26 15:26:00 2024
 */
#ifndef MAPI_UAPI_KCOV_H
#define MAPI_UAPI_KCOV_H

#define __KCOV_STATE_LEN 16
#define __KCOV_STATE_MASK ((1U << __KCOV_STATE_LEN) - 1U)
#define __KCOV_IDX_STATE(idx, state) (((idx) << __KCOV_STATE_LEN) | (state))
#define __KCOV_IDX_OF(idx_state) ((idx_state) >> __KCOV_STATE_LEN)
#define __KCOV_STATE_OF(idx_state) ((idx_state) & __KCOV_STATE_MASK)

#define __KCOV_IDX_DISABLED 0

enum __kcov_state {
	__KCOV_STATE_INVALID = 0,
	__KCOV_STATE_CREATED, /* after kcov_open */
	__KCOV_STATE_INITED, /* after ioctl of KCOV_INIT_TRACE */
	__KCOV_STATE_TRACE_PC,
	__KCOV_STATE_TRACE_CMP
};

#endif
