#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Wrap up C codes with macros
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:39:08 PM CST
"""

from __future__ import annotations, absolute_import

from typing import Callable, Dict, FrozenSet, List, Tuple

from hmdsl.sysif.model import CCode, Element


Category = Tuple[FrozenSet[str], FrozenSet[str]]


def categorize_elements(elts: List[Element]) -> Dict[Category, List[Element]]:
    """
    Categorize C #ifdef and #ifndef directives

    Args:
        elts (List[Element]): A list of #ifdef and #ifndef directives

    Returns:
        Dict[Category, List[Element]]: Categorize the directives
    """
    categorized: Dict[Category, List[Element]] = {}
    for elt in elts:
        category = elt.get_category()
        lis = categorized.get(category)
        if lis is not None:
            lis.append(elt)
        else:
            categorized[category] = [elt]
    return categorized


def print_category(category: Dict[Category, List[Element]], print_element: Callable[..., str]) -> \
        str:
    """
    To print #ifdef and #ifndef directives

    Returns:
        str: C code of #ifdef and #ifndef
    """
    res = ['\n']
    for (ifdefs, ifndefs) in category:
        res.append("".join(f"#ifdef {condition}\n" for condition in ifdefs))
        res.append("".join(f"#ifndef {condition}\n" for condition in ifndefs))
        res.append("".join(print_element(elt) for elt in category[(ifdefs, ifndefs)]))
        res.append("\n%s" % ("#endif\n" * (len(ifdefs) + len(ifndefs))))

    return ''.join(res)


def print_c_header(header: CCode) -> str:
    """
    Print C code headers

    Args:
        header (CCode): C headers

    Returns:
        str: `#include <>` kind C headers
    """
    return f"#include <{header.content}>\n"


def print_datatypes(datatypes: CCode) -> str:
    """
    Print C data types declarations

    Args:
        datatypes (CCode): C data types declarations

    Returns:
        str: data types declarations in C code
    """
    return f'{datatypes.content}\n'
