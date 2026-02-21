#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Normalization of C-BiAst
Author: Huawei OS Kernel Lab
Create: Thu 14 Jan 2021 14:30:00 PM CST
"""

from __future__ import annotations, absolute_import

from dataclasses import dataclass, field
from typing import Optional, List

from hmdsl.common.c_biast.biast import CStatement, CIfStatement, CConstant, CNullStatement, \
    CExpression, CBiAst
from hmdsl.common.pyext import PathHelper


@dataclass
class CCompilerDesc:
    """
    Infos of C compilers

    Raises:
        ValueError: Invalid path
    """

    path: str
    args: List[str] = field(default_factory=list)

    def __post_init__(self):
        full_path = PathHelper.find_executable(self.path)

        self.path = full_path


@dataclass
class CCompilerConfig:
    """
    Get C compiler configs from C compiler infos
    """

    @classmethod
    def from_compiler_desc(cls, _: CCompilerDesc) -> CCompilerConfig:
        """
        To generate render configs from C compiler infos

        Args:
            compiler_desc (CCompilerDesc): A given C compiler

        Returns:
            CCompilerConfig: Configs to render C codes
        """
        return CCompilerConfig()


@dataclass
class CNormalizer:
    """ C normalizer to normalize some simple codes """

    # When the compiler configuration is present, we can do more semantics-level
    # normalization, e.g. evaluate the ``sizeof`` macro, generate the struct layout,
    # etc.
    compiler_config: Optional[CCompilerConfig] = None

    @staticmethod
    def __normalize_expression(expr: CExpression) -> CExpression:
        return expr

    def normalize(self, ast: CBiAst) -> CBiAst:
        """
        Normalize C Ast

        Args:
            ast (CBiAst): A given C BiAst

        Raises:
            NotImplementedError: Open to extend for more normalizable cases

        Returns:
            CBiAst: A normalized C BiAst
        """
        if isinstance(ast, CStatement):
            return self.__normalize_statement(ast)
        if isinstance(ast, CExpression):
            return self.__normalize_expression(ast)
        raise NotImplementedError(f"don't know how to normalize {ast}")

    def __normalize_statement(self, stmt: CStatement) -> CStatement:
        if isinstance(stmt, CIfStatement):
            stmt.then_statement = self.__normalize_statement(stmt.then_statement)
            if stmt.else_statement is not None:
                stmt.else_statement = self.__normalize_statement(stmt.else_statement)

            if isinstance(stmt.condition, CConstant):
                if bool(stmt.condition):
                    return stmt.then_statement
                if stmt.else_statement is not None:
                    return stmt.else_statement
                return CNullStatement()

        return stmt
