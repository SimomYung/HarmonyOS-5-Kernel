/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: PAC instructions
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 26 10:57:32 2023
 */
#ifndef KLIBS_EXT_PAC_H
#define KLIBS_EXT_PAC_H
#include "pac.h"
#ifdef PAC_FWCFI_SYSSERVICE
static void __PAC_INIT_SECTION ext_hdlr_init(uintptr_t begin, uintptr_t end)
{
        uintptr_t reg_global_func = begin;
        while (reg_global_func < end) {
                void (*func)(void);
                func = *(void (**)(void))reg_global_func;
                PAC_FECFI_XPAC_SIGN(func, void(*)(void));
                *(void (**)(void))reg_global_func = func;
                reg_global_func += sizeof(void(*)(void));
        }
        return;
}
#endif
#endif