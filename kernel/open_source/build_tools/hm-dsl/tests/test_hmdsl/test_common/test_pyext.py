# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Provide optional type
Author: Huawei OS Kernel Lab
Create: Mon 28 Dec 2020 12:40:00 PM CST
"""

from unittest import TestCase
from hmdsl.common.pyext import EOptional


class TestEOptional(TestCase):

    def test_invalid_get_1(self):
        with self.assertRaises(ValueError):
            EOptional.get(None)

    def test_get_1(self):
        self.assertEqual(EOptional.get(1), 1)

    def test_invalid_merge_1(self):
        with self.assertRaises(ValueError):
            EOptional.merge(1, 2)

    def test_merge_1(self):
        self.assertEqual(EOptional.merge(1, None), 1)

    def test_merge_2(self):
        self.assertEqual(EOptional.merge(None, 1), 1)

    def test_merge_3(self):
        self.assertEqual(EOptional.merge(None, None), None)

    def test_merge_4(self):
        self.assertEqual(EOptional.merge(1, 1), 1)
