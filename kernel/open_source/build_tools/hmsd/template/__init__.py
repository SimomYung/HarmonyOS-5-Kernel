#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Template module
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .template import Template, make_template, register_template, all_templates
from .iterator import TemplateIterator, ObjectIterator, index_to_name
from .template_printer import TemplatePrinter
from .object_standardizer import ObjectStandardizer, standardize_object
from .sysif import make_sysif_template
from .hook import make_hook_template
from .object import make_object_template

register_template('sysif', make_sysif_template)
register_template('hook', make_hook_template)
register_template('object', make_object_template)
