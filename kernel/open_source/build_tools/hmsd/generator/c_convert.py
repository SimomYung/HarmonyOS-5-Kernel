#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Some classes and function to convert AST
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import copy

from hmdsl.common.c_biast.biast import CFunction, CVariableDeclaration, \
    CStorageQualifier, CParameterDeclaration, CIdentifierExpression, \
    CDeclaration, CAliasType, CUnionType, CEnumType, CEnumItem, CStructType


def is_ast_instance(ast, ast_types):
    """
    Check whether the AST is an instance of one of AST types.
    """
    for ast_type in ast_types:
        if isinstance(ast, ast_type):
            return True
    return False


def _convert(ast, func):
    """
    Use the function to convert the AST.
    """
    return [func(a) for a in ast] if isinstance(ast, list) else func(ast)


def _export(ast):
    """
    Get the declaration of AST.
    """
    ast = copy.deepcopy(ast)
    if isinstance(ast, CFunction):
        ast.body = None
    elif isinstance(ast, CVariableDeclaration):
        ast.initializer = None
        ast.storage_qualifiers = {CStorageQualifier.EXTERN}
    else:
        raise ValueError('Unknown Biast subclass', type(ast))
    return ast


def become_exported(ast):
    """
    Get the declaration of AST.
    """
    return _convert(ast, _export)


def _pass(ast):
    """
    Get the AST to be passed to functions.
    """
    if isinstance(ast, CParameterDeclaration):
        return CIdentifierExpression(ast.name)
    raise ValueError('Unknown Biast subclass', type(ast))


def become_passed(ast):
    """
    Get the AST to be passed to functions.
    """
    return _convert(ast, _pass)


def name_of(ast):
    """
    Get the name of the AST.
    """
    if isinstance(ast, CDeclaration):
        ast = ast.get_ident()
    if is_ast_instance(ast, [
            CIdentifierExpression,
            CFunction,
            CAliasType,
            CUnionType,
            CStructType,
            CEnumType,
            CEnumItem,
    ]):
        return ast.name
    raise ValueError('Unknown Biast subclass', type(ast))


def _become_identifier(ast):
    """
    Get the indentifier of the AST.
    """
    return CIdentifierExpression(name_of(ast))


def become_identifier(ast):
    """
    Get the indentifier of the AST.
    """
    return _convert(ast, _become_identifier)

# vim: ts=4:sw=4:expandtab
