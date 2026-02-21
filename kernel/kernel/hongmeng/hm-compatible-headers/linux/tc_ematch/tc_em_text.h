/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/tc_ematch/tc_em_text.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 24 09:56:24 2022
 */

#ifndef LINUX_TC_EM_TEXT_H
#define LINUX_TC_EM_TEXT_H

#include <linux/types.h>

#define TC_EM_TEXT_ALGOSIZ	16

struct tcf_em_text {
	char		algo[TC_EM_TEXT_ALGOSIZ];
	__u16		from_offset;
	__u16		to_offset;
	__u16		pattern_len;
	__u8		from_layer:4;
	__u8		to_layer:4;
	__u8		pad;
};

#endif
