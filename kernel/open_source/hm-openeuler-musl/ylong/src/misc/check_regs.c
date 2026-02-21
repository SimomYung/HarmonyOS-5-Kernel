/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: check regs
 */

#include <sys/procfs.h>
#include <errno.h>
#include "syscall.h"

int SysCheckRegs(hpe_proc_check_regs_t regs, void *addr, size_t len)
{
    int ret = __syscall(SYS_check_regs, regs, addr, len);
    if (ret < 0) {
        return __syscall_ret(ret);
    }
    return ret;
}

int HpeProcCheckRegs(hpe_proc_check_regs_t regs, void *addr, size_t len)
{
    return SysCheckRegs(regs, addr, len);
}

