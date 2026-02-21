/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Pgtable layer
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 27 09:07:20 2018
 */
#ifndef HMKERNEL_PGTABLE_LAYER_H
#define HMKERNEL_PGTABLE_LAYER_H

#include <hmkernel/types.h>
#include <hmkernel/strict.h>
#include <hmkernel/compiler.h>

struct __pgtable_key_attr_s {
	__bool kreadable;
	__bool kwritable;
	__bool kexecutable;
	__bool ureadable;
	__bool uwritable;
	__bool uexecutable;
	__bool is_device;
	__bool is_nocache;
};

#endif
