/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Compatible header file for linux/bpf_common.h
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 25 16:02:47 2022
 */

#ifndef __LINUX_BPF_COMMON_H__
#define __LINUX_BPF_COMMON_H__

#define BPF_CLASS(code)	((code) & 0x07)
#define BPF_LD		0x00
#define BPF_LDX		0x01
#define BPF_ALU		0x04

#define BPF_JMP		0x05
#define BPF_RET		0x06
#define BPF_MISC	0x07

#define BPF_W		0x00
#define BPF_H		0x08
#define BPF_B		0x10

#define BPF_ABS		0x20
#define BPF_IND		0x40

#define BPF_ADD		0x00
#define BPF_OR		0x40
#define BPF_AND		0x50
#define BPF_LSH		0x60
#define BPF_RSH		0x70

#define BPF_LEN		0x80
#define BPF_MSH		0xa0

#define BPF_JA		0x00
#define BPF_JEQ		0x10
#define BPF_JSET	0x40
#define BPF_K		0x00
#define BPF_X		0x08

#endif
