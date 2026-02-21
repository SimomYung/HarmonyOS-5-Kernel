#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: To install this code generator
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:22:21 PM CST
"""

from __future__ import absolute_import

from os.path import abspath, dirname, join
import setuptools


repo = dirname(abspath(__file__))

readme = join(repo, "README.md")

install_requires = ["antlr4-python3-runtime==4.13.0"]


with open(readme, "r") as f:
    long_description = f.read()


setuptools.setup(
    name='hm-dsl',
    version='0.0.1',
    scripts=[join(repo, 'hm-dsl')],
    author=['Huawei OS Kernel Lab'],
    author_email=[''],
    description='DSL-based code-generator used in Hongmeng OS Kernel',
    long_description=long_description,
    long_description_content_type='text/markdown',
    packages=setuptools.find_packages(exclude=["tests.*", "tests"]),
    package_data={},
    classifiers=[
        "Programming Language :: Python :: 3",
        "Operating System :: Linux",
    ],
    python_requires=">=3.7",
    install_requires=install_requires
)
