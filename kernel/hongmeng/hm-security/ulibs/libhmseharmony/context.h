/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for context
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_CONTEXT_H
#define HMSEL_CONTEXT_H

#include <libhmseharmony/services.h>
#include <libhmseharmony/pac.h>

struct hmsel_chk_context {
	__sec_data(seharmony, ptr) struct hmsel_context *scon;
	__sec_data(seharmony, ptr) struct hmsel_context *tcon;
	hmsel_class_t tclass;
};

#endif
