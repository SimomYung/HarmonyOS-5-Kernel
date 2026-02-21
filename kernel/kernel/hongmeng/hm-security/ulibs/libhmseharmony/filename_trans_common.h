/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for filename trans common
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_FILENAME_TRANS_COMMON_H
#define HMSEL_FILENAME_TRANS_COMMON_H

struct hmsel_fname_trans_key {
	uint32_t ttype;
	uint32_t tclass;
	const char *name;
};

struct hmsel_fname_trans_datum;

#endif
