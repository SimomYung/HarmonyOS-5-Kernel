#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Generate sysif base common api
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:35:32 PM CST
"""

from __future__ import absolute_import

from typing import List, Union
from hmdsl.common.c_biast.biast import CTranslationUnit, CVariableDeclaration


def gen_api_common() -> List[Union[str, CTranslationUnit]]:
    """
    Generate common apis for all sysif servers

    Returns:
        List[Union[str, CTranslationUnit]]: A mixed C Macros string and C AST
    """
    res: List[Union[str, CTranslationUnit]] = ['#ifndef LIBSYSIF_BASE_API_COMMON_H\n']
    res.append('#define LIBSYSIF_BASE_API_COMMON_H\n')
    res.append('#include <stddef.h>\n')
    res.append('#include <stdint.h>\n')
    res.append('#include <hmkernel/capability.h>\n')

    res.append(CTranslationUnit(CVariableDeclaration.constant_of(
        """struct sysif_reply_header{
                int32_t retval;
                uint32_t msgsz;
        };
        """)))
    res.append('#define SYSIF_REPLY_LEN_MAX 256UL\n')

    res.append(CTranslationUnit(CVariableDeclaration.constant_of(
        """ extern void * hm_actvpool_prepare_ret(unsigned long long sender,
            unsigned long credential, unsigned long size);
        """)))
    res.append(CTranslationUnit(CVariableDeclaration.constant_of(
        """ extern struct arch_actv_local *hm_actv_current_arch_actv_local(void);
        """)))

    res.append('#endif\n')

    return res
