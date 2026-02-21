/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Add linux/filter.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 10 20:47:18 2022
 */
#ifndef _LINUX_FILTER_H
#define _LINUX_FILTER_H

#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/bpf_common.h>

struct sock_filter {	/* Filter block */
	__u16	code;   /* Actual filter code */
	__u8	jt;	/* Jump true */
	__u8	jf;	/* Jump false */
	__u32	k;      /* Generic multiuse field */
};

struct sock_fprog {	/* Required for SO_ATTACH_FILTER. */
	unsigned short		len;	/* Number of filter blocks */
	struct sock_filter __user *filter;
};

#define 		BPF_TAX 		0x00

#ifndef BPF_STMT
#define BPF_STMT(code, k) { (unsigned short)(code), 0, 0, k }
#endif
#ifndef BPF_JUMP
#define BPF_JUMP(code, k, jt, jf) { (unsigned short)(code), jt, jf, k }
#endif

#endif
