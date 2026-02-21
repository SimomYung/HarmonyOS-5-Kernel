#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
Description: Provide code generators
Author: Huawei OS Kernel Lab
Create: Thu Jul 12 15:22:42 2023
"""

from hmdsl.common.c_biast.biast import CParameterDeclaration, CType, \
    CPointerType, CExpression, CTypeQualifier
from .c_convert import become_passed
from ..utils import get_command_options


def gen_expr_expected_result(hook):
    """
    Generate an expression of the expected result of the hook.
    """
    return CExpression.of(hook.result.expected_value)


def gen_params_hook(hook):
    """
    Generate a list of parameters of the hook.
    """
    return [CParameterDeclaration(a.name, CType.of(a.type))
            for a in hook.arguments]


def gen_args_call_method(hook):
    """
    Generate arguments to call methods
    """
    params = []
    if hook.credential.enabled:
        params.append(CExpression.of('&cred'))
    params.extend(become_passed(gen_params_hook(hook)))
    return params


def gen_expr_default_result(hook):
    """
    Generate an expression of the default result of the hook.
    """
    return CExpression.of(hook.result.default_value)


def gen_params_method(hook):
    """
    Generate a list of parameters of the hook method.
    """
    params = []
    if hook.credential.enabled:
        cred_type = gen_type_cred(hook)
        if not (hook.credential.active or hook.credential.transaction):
            cred_type.add_qualifier(CTypeQualifier.CONST)
        cred_type = CPointerType.create(cred_type, 1)
        params += [CParameterDeclaration('cred', cred_type)]
    params += gen_params_hook(hook)
    return params


def gen_type_cred(hook):
    """
    Generate a type of the credential of the hook.
    """
    return CType.of('struct sec_chk_cred_xact' if
                    hook.credential.transaction else
                    'struct sec_chk_cred')


def gen_type_hook_result(hook):
    """
    Generate a type of the result of the hook.
    """
    return CType.of(hook.result.type)


def generation_trend(hook):
    """
    Generation trend of a hook
    """
    trend = hook.generation.trend
    if trend != 'auto':
        return trend
    trend = get_command_options().trend
    if (trend == 'performance' and
            (not hook.generation.performance_compatibility)):
        trend = 'auto'
    return trend


def generation_disabled(hook):
    """
    Generation disabled of a hook
    """
    if not hook.generation.enabled:
        return True
    configs = get_command_options().configs.split(' ')
    if (generation_trend(hook) == 'performance' and hook.condition.ifdef is not None):
        for cond in hook.condition.ifdef:
            if cond not in configs:
                return True
    return False
# vim: ts=4:sw=4:expandtab
