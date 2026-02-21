#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Substitution of CBiAst Macros
Author: Huawei OS Kernel Lab
Create: Thu 4 Mar 2021 16:30:00 PM CST
"""
from __future__ import absolute_import

from typing import TypeVar, Union, cast, Mapping, Dict, Any, Sequence
from dataclasses import dataclass

from hmdsl.common.c_biast.biast import CMacro, CBiAst


T = TypeVar('T')

SubstContext = Mapping[str, Union[CBiAst, Sequence[CBiAst]]]


@dataclass
class SubstitutionError(Exception):
    """
    Specific exceptions for substitution errors
    """
    macro_name: str
    message: str


def __expect(obj_t, item, cls, parent):
    if isinstance(item, list) and not isinstance(parent, list):
        raise SubstitutionError(obj_t, f"cannot unfold as a list")

    for obj_v in item if isinstance(item, list) else (item,):
        if not isinstance(obj_v, cls):
            raise SubstitutionError(obj_t, f"{obj_v} is supposed to be a {cls.__name__}")

    return item


def __subst_dict(obj_t, _, context, inplace):
    dret: Dict[str, Any] = {k: __subst(v, obj_t, context, inplace) for k, v in obj_t.items()}

    if not inplace:
        return dret

    obj_t.update(dret)
    return obj_t


def __subst_list(obj_t, _, context, inplace):
    lret = []
    for item in obj_t:
        updated_item = __subst(item, obj_t, context, inplace)
        if isinstance(updated_item, list):
            lret += updated_item
        else:
            lret.append(updated_item)

    if not inplace:
        return lret

    obj_t.clear()
    obj_t += lret
    return obj_t


def __subst_tuple(obj_t, _, context, inplace):
    return tuple(__subst(item, obj_t, context, inplace) for item in obj_t)


def __subst_biast(obj_t, _, context, inplace):
    cls = type(obj_t)
    dic = __subst(obj_t.__dict__, obj_t, context, inplace)
    if dic is obj_t.__dict__:
        return obj_t
    if inplace:
        obj_t.__dict__ = dic
        return obj_t

    ret = cls.__new__(cls)
    ret.__dict__ = dic
    return ret


def __subst(obj_t, parent, context, inplace):
    oprs = {
        dict: __subst_dict,
        list: __subst_list,
        tuple: __subst_tuple
    }

    if type(obj_t) in oprs:
        return oprs.get(type(obj_t))(obj_t, parent, context, inplace)
    if issubclass(type(obj_t), CMacro) and obj_t.macro_name in context:
        return __expect(obj_t.macro_name, context[obj_t.macro_name], obj_t.get_base_class(), parent)
    if isinstance(obj_t, CBiAst):
        return __subst_biast(obj_t, parent, context, inplace)

    return obj_t


def subst(obj_t: T, context: SubstContext, inplace: bool = False) -> T:
    """ This is a generic function aims to perform substitution
    on the macros.

    To bypass type-checking, which does not properly works in this function, we
    use some ``cast`` function here. Please be careful when you want to change
    this function. And always be noticed that type-checker does not help with
    avoiding errors in ``subst``.
    """

    return cast(T, __subst(obj_t, None, context, inplace))
