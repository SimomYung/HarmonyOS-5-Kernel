/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/tc_ematch/tc_em_nbyte.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 24 09:50:25 2022
 */

#ifndef LINUX_TC_EM_NBYTE_H
#define LINUX_TC_EM_NBYTE_H

#include <linux/types.h>

struct tcf_em_nbyte {
	__u16			off;
	__u16			len:12;
	__u8			layer:4;
};

#endif
