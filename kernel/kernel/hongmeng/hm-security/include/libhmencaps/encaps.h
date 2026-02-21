/*
* Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
* Description: ENCAPS check header file
* Author: Huawei OS Kernel Lab
* Create: Thu Apr 25 19:57:33 2024
*/
#ifndef LIBHMENCAPS_ENCAPS_H
#define LIBHMENCAPS_ENCAPS_H

#include <libsec/cred.h>
#include <libsec/pac.h>
#include <libsec/log.h>
#include <libsec/module.h>
#include <sys/types.h>

extern struct sec_mod sec_mod_encaps;

#define SEC_DEFINE_ENCAPS(name, arg)                                               \
static int _sec_chk_encaps_##name(const struct sec_chk_cred *cred,                 \
        const struct sec_cred_encaps *encaps_cred PARA(arg));                         \
static int sec_chk_encaps_##name(const struct sec_chk_cred *cred PARA(arg))        \
{                                                                               \
        struct sec_cred_encaps *encaps;                                               \
        int rc;                                                                 \
        encaps =  encaps_cred_of_sec_cred(cred);                                      \
        rc = _sec_chk_encaps_##name(cred, encaps VAR(arg));                   \
        return rc;                                                              \
}                                                                               \
static int _sec_chk_encaps_##name(const struct sec_chk_cred *cred,                 \
                               const struct sec_cred_encaps *encaps_cred PARA(arg))

#define sec_define_encaps_method(method)                                           \
        sec_define_chk_method(method, sec_chk_encaps_##method)

#endif
