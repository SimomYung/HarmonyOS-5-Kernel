#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Generator module
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .generator import Generator, make_generator, register_generator
from .generator import register_options, standardize_options
from .template_document import TemplateDocumentGenerator
from .code import CodeGenerator, HeaderGenerator

register_generator('template_document', TemplateDocumentGenerator)
register_generator('code', CodeGenerator)
register_generator('header', HeaderGenerator)
