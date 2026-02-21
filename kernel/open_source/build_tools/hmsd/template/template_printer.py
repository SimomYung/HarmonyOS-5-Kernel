#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Template printer
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import sys
from .iterator import TemplateIterator, index_to_name


class TemplatePrinter(TemplateIterator):
    """
    Print a template when iterate it.
    """

    def __init__(self, template, output=sys.stdout):
        super().__init__(template)
        self._output = output


    def _print(self, node, index):
        indent = '\t' * self._stack_depth()
        index = index_to_name(index)
        type_name = type(node).__name__
        self._output.write(f'{indent}{index}: {type_name}\n')


    def _traverse_basic_node(self, node, index):
        self._print(node, index)


    def _traverse_list_node(self, node, index):
        self._print(node, index)
        super()._traverse_list_node(node, index)


    def _traverse_dict_node(self, node, index):
        self._print(node, index)
        super()._traverse_dict_node(node, index)


# vim: ts=4:sw=4:expandtab
