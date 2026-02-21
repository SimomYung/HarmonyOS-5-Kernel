#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Configuration of the C Bi-Ast Styler
Author: Huawei OS Kernel Lab
Create: Thu 14 Jan 2021 14:30:00 PM CST
"""

from __future__ import absolute_import

from dataclasses import dataclass, field
from typing import Optional, Set, Tuple
from enum import Enum, auto
from hmdsl.common.c_biast.biast import CBinaryOperator, CBiAst, CBinaryExpression
from hmdsl.common.logging import get_logger


logger = get_logger("common.c_biast.styler.config")


@dataclass
class CWparenthesesStyle:
    """ GCC compile command: -Wparentheses (or -Wall) warns
    when operators are nested whose precedence
    often get confused about. We record such opeartor pairs in a set to put parentheses
    around operands of such operators while rendring C codes to silence the warnings.
    """

    wparentheses_pairs: Set[Tuple[str, str]] = field(default_factory=set)

    def __post_init__(self):
        self.wparentheses_pairs = {
            ('||', '&&'), ('|', '^'), ('|', '&'), ('|', '+'), ('|', '-'), ('^', '&'),
            ('^', '+'), ('^', '-'), ('&', '+'), ('&', '-'), ('==', '<='), ('==', '>='),
            ('==', '<'), ('==', '>'), ('!=', '<='), ('!=', '>='), ('!=', '<'), ('!=', '>'),
            ('<<', '+'), ('<<', '-'), ('>>', '+'), ('>>', '-')
        }

    def append(self, op1: str, op2: str):
        """
        Add a pair of binary operations which should put parentheses around
        """
        if CBinaryOperator(op1).precedence < CBinaryOperator(op2).precedence:
            self.wparentheses_pairs.add((op2, op1))
        elif CBinaryOperator(op1).precedence > CBinaryOperator(op2).precedence:
            self.wparentheses_pairs.add((op1, op2))
        else:
            logger.info(f"ignore operators with same precedences.")

    def update(self, *op_pairs):
        """
        Add more pairs of binary operations which should put parentheses around
        """
        for (op1, op2) in op_pairs:
            self.append(op1, op2)

    def wparentheses_nested_expression(self, ast: CBiAst, parent: Optional[CBiAst]) -> bool:
        """
        Decide whether the nested binary expression should put parentheses around
        """
        return isinstance(ast, CBinaryExpression) and isinstance(parent, CBinaryExpression) and \
            ast.precedence < parent.precedence and \
            (parent.operator.value, ast.operator.value) in self.wparentheses_pairs


class BraceStyle(Enum):
    """
    Enum of different types of the way to put braces
    """
    ATTACHED = auto()
    BROKEN = auto()
    LINUX = auto()


@dataclass
class CStylerConfig:
    """ A large number of configurable items in this styler are inspired by
    the open-source styling tool *astyle*.
    """

    brace_style: BraceStyle = BraceStyle.LINUX
    indent: str = '\t'
    width: int = 80
    parentheses_style: CWparenthesesStyle = field(default_factory=CWparenthesesStyle)


class CStylerPresets:
    """
    Preset a HONGMENG style config
    """

    @staticmethod
    def export_hongmeng_config():
        """ Return a HongMeng config """
        return CStylerConfig()

    @staticmethod
    def rsvd_api_two():
        """ reserve a public interface """
        return

    @classmethod
    def rsvd_api_one(cls):
        """ reserve a public interface """
        return
