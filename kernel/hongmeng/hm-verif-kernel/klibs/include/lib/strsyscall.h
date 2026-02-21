/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strsyscall
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 07:45:05 2018
 */
#ifndef KLIBS_STRSYSCALL_H
#define KLIBS_STRSYSCALL_H

const char *
strsyscall_bits(unsigned int sysno);

const char *
strsyscall(unsigned int sysno);

const char *
strtailcall(unsigned int tcno);

#endif
