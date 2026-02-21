# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Unit test cases for C_Biast subst
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:19:00 PM CST
"""

from hmdsl.common.c_biast.biast import CDeclaration, CIntegerConstant
from hmdsl.common.c_biast.subst import subst
from unittest import TestCase


class TestSubstDeclaration(TestCase):

    def test_1(self):
        decl0 = CDeclaration.of("T t = {@_wtf:expr};")
        decl1 = subst(decl0, {'_wtf': CIntegerConstant(0)})

        self.assertEqual(decl1, CDeclaration.of("T t = {0};"))
