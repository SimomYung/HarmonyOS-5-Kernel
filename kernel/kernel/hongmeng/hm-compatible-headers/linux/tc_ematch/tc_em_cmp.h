/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/tc_ematch/tc_em_cmp.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 24 09:41:13 2022
 */

#ifndef LINUX_TC_EM_CMP_H
#define LINUX_TC_EM_CMP_H

#include <linux/types.h>

struct tcf_em_cmp {
	__u32		val;
	__u32		mask;
	__u16		off;
	__u8		align:4;
	__u8		flags:4;
	__u8		layer:4;
	__u8		opnd:4;
};
enum {
	TCF_EM_ALIGN_U8  = 1,
	TCF_EM_ALIGN_U16 = 2,
	TCF_EM_ALIGN_U32 = 4
};

#define TCF_EM_CMP_TRANS	1

#endif
