# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Unittest testcases of compiler-independent normalization
Author: Huawei OS Kernel Lab
Create: Thu 14 Jan 2021 12:30:00 AM CST
"""

from hmdsl.common.c_biast.biast import CStatement, CNullStatement, CCompoundStatement
from hmdsl.common.c_biast.normalize import CNormalizer
from unittest import TestCase


class TestDeadCode(TestCase):

    def test_dead_if_1(self):
        normalizer = CNormalizer()
        stmt = CNormalizer().normalize(CStatement.of("""
        if (0) {
            a++;
        }
        """))

        self.assertEqual(stmt, CNullStatement())

    def test_dead_if_2(self):
        normalizer = CNormalizer()
        stmt = CNormalizer().normalize(CStatement.of("""
        if (1) {
            a++;
        }
        """))

        self.assertEqual(stmt, CStatement.of("{ a++; }"))

    def test_dead_if_3(self):
        normalizer = CNormalizer()
        stmt = CNormalizer().normalize(CStatement.of("""
        if (0) {
            a++;
        } else {}
        """))

        self.assertEqual(stmt, CCompoundStatement([]))
