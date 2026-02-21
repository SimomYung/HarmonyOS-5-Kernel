#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Description: Parsing Error
Author: Huawei OS Kernel Lab
Create: Mon Jul 18 15:04:50 2022
"""

from __future__ import absolute_import

from dataclasses import dataclass

from antlr4.error.ErrorListener import ErrorListener


@dataclass
class ParsingError(Exception):
    """ Parsing error """
    typ: str
    msg: str


class DSLErrorListener(ErrorListener):
    """ A customized error listener """

    @staticmethod
    def syntaxError(__recognizer, __offending_symbol, line, column, msg, e):
        """ customized error message """
        msg = f"line {line}:{column} {msg}"
        raise ParsingError((type(e)).__name__, msg) from None

    @staticmethod
    def rsvd_api_one():
        " reserve a public interface "
        return
