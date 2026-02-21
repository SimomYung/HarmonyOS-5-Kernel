#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide template document generator
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .generator import Generator
from ..template import TemplateIterator, make_template, index_to_name, all_templates
from ..utils import cmd_print


class TemplateDocumentIterator(TemplateIterator):
    """
    TemplateDocumentIterator is an iterator to generate the documents of
    templates.
    """

    def _traverse_node(self, node, index):
        """
        Print this node.
        """
        cmd_print('{}:\n\t{}'''.format(self.__get_current_node_full_path(index),
            node.textualize('\n\t', '{:<8}: {}')
        ))


    def __get_current_node_full_path(self, last_index):
        """
        Get the full path of the current node.
        """
        indexes = [sf.index for sf in self._stack]
        indexes.append(last_index)
        return '.'.join([index_to_name(index, short_name=True) for index in indexes])


class TemplateDocumentGenerator(Generator):
    """
    TemplateDocumentGenerator is a generator which print documents of all
    templates.
    """

    def generate(self, _):
        """
        Generate documents of all templates.
        """
        for template in all_templates():
            cmd_print(f'Template {template}:')
            tmpl = make_template(template)
            tdit = TemplateDocumentIterator(tmpl)
            tdit.traverse()


# vim: ts=4:sw=4:expandtab
