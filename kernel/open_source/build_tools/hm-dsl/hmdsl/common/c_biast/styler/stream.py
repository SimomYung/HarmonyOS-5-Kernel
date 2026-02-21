#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: A Rendering Stream of the C Bi-Ast Styler
Author: Huawei OS Kernel Lab
Create: Thu 14 Jan 2021 14:30:00 PM CST
"""

from __future__ import absolute_import

from dataclasses import dataclass, field
from typing import Union, List
from enum import Enum, auto
from io import StringIO


class ControlToken(Enum):
    """
    Enum for space, end of line and indent
    """
    SPACE = auto()
    ENDL = auto()
    SPACE_ENDL = auto()
    INDENT = auto()


@dataclass
class CStylerStream:
    """ print C code """
    tokens: List[Union[str, ControlToken]] = field(default_factory=list)
    indent_level: int = 0
    width: int = 80

    def endl(self):
        """ Print endl character """
        self.tokens.append(ControlToken.ENDL)

    def space(self):
        """ Print space character """
        self.tokens.append(ControlToken.SPACE)

    def space_or_endl(self):
        """ Print space or endl character """
        self.tokens.append(ControlToken.SPACE_ENDL)

    def write(self, *contents: str):
        """ write a block of content """
        for content in contents:
            for line in content.split("\n"):
                self.__write(line)
                self.endl()

            # drop the last ENDL
            self.tokens.pop()

    def writeln(self, *contents: str):
        """ write content with endl """
        self.write(*contents)
        self.endl()

    def clear(self):
        """ clear tokens and reset indent level """
        self.tokens.clear()
        self.indent_level = 0

    def getvalue(self, indent: str) -> str:
        """ get tokens' value """
        ret = StringIO()
        for token in self.tokens:
            if isinstance(token, str):
                ret.write(token)
            elif token is ControlToken.SPACE:
                ret.write(' ')
            elif token is ControlToken.SPACE_ENDL:
                # In the future this should be treated differently
                # with ControlToken.SPACE for line-length limitation
                ret.write(' ')
            elif token is ControlToken.ENDL:
                ret.write('\n')
            elif token is ControlToken.INDENT:
                ret.write(indent)
            else:
                raise NotImplementedError(f"unknown control token {token}")

        return ret.getvalue()

    def __write(self, content: str):
        """ content shall not contains the endl character ``\\n`` """

        if self.indent_level > 0 and \
                (len(self.tokens) == 0 or self.tokens[-1] == ControlToken.ENDL):
            self.tokens += [ControlToken.INDENT] * self.indent_level

        self.tokens.append(content)
