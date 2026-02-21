#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Template class
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from dataclasses import dataclass

from .. import utils
from .node import Node


@dataclass
class Template():
    """
    Template is the base class of templates. It contains the root of a tree of
    nodes.
    """
    root: Node


_template_builder = utils.Builder()


def make_template(name):
    """
    Make a template by a name
    """
    return _template_builder.make(name)


def register_template(name, cls):
    """
    Register a template class with a name
    """
    _template_builder.register(name, cls)


def all_templates():
    """
    Get all registered templates
    """
    return _template_builder.all_templates()


# vim: ts=4:sw=4:expandtab
