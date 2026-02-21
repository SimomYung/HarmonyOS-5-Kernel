#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Common classes and function to generate C language codes
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import re

from hmdsl.common.c_biast.subst import subst
from hmdsl.common.c_biast.styler.styler import CStyler
from hmdsl.common.c_biast.biast import CTranslationUnit


def subst_string(string, snippets):
    """
    Substitue a string with snippets.
    """
    snippets = {s.name: str(s.eval()) for s in snippets}
    return re.sub(r'@{[\w_][\w\d_]*}',
                  lambda m: snippets[string[m.start() + 2 : m.end() - 1]],
                  string)


def subst_ast(ast, snippets):
    """
    Substitue an abstract syntax tree with snippets.
    """
    snippets = {s.name: s.eval() for s in snippets}
    return subst(ast, snippets)


def subst_code(code, snippets):
    """
    Substitue codes with snippets.
    """
    if isinstance(code, str):
        return subst_string(code, snippets)
    return subst_ast(code, snippets)


def code_to_str(code):
    """
    Convert a code object to a string.
    """
    if isinstance(code, str):
        return code
    content = code if isinstance(code, list) else [code]
    return str(CStyler().render(CTranslationUnit(content)))


def code_to_text(code):
    """
    Convert a list of code objects to a string.
    """
    return ''.join(code_to_str(item) if not isinstance(item, str) else
                   item for item in code)


class Snippet():
    """
    A lazy-execution snippet of codes.
    """
    def __init__(self, function, *args, **kargs):
        self.function = function
        self.args = args
        self.kargs = kargs
        self._name = None


    @property
    def name(self):
        """
        Get the name of the snippet.
        """
        if self._name is not None:
            return self._name
        if not self.function.__name__.startswith('gen_'):
            raise ValueError('The function name does not start with gen_',
                             self.function.__name__)
        return self.function.__name__.replace('gen_', '', 1)


    def add_name(self, name):
        """
        Set the name of the snippet.
        """
        self._name = name
        return self


    def eval(self):
        """
        Execute the function to generate codes.
        """
        return self.function(*self.args, **self.kargs)


placeholder_snippet = Snippet(lambda: None).add_name('_unused_')
# vim: ts=4:sw=4:expandtab
