#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path

from tools.filemodule.base.hmdir import HMDir
from tools.filemodule.base.hmfile import HMFile
from tools.tooltest import ToolTestCase


class TestHMDir(ToolTestCase):

    def test_file_interface(self):
        testdir = HMDir("test")
        testdir.add_file(HMFile("hello", "hello"))
        self.assertIsNotNone(testdir.get_file("hello"))
        testdir.remove_file("hello")
        self.assertIsNone(testdir.get_file("hello"))

    def test_dir_interface(self):
        testdir = HMDir("test")
        subdir = "subdir"
        testdir.add_dir(HMDir(subdir))
        self.assertIsNotNone(testdir.get_dir(subdir))
        self.assertIsInstance(testdir.get_dir(subdir), HMDir)
        testdir.remove_dir(subdir)
        self.assertIsNone(testdir.get_dir(subdir))

    def test_write_to_disk(self):
        testdir = HMDir("test")
        subdir = "subdir"
        testdir.add_dir(HMDir(subdir))
        testdir.add_file(HMFile("hello", "hello"))
        testdir.write_to_disk(self.tmpdir)

        check = Path(os.path.join(self.tmpdir, "test"))
        self.assertTrue(check.exists())
        self.assertTrue(check.is_dir())

        check = Path(os.path.join(self.tmpdir, "test/subdir"))
        self.assertTrue(check.exists())
        self.assertTrue(check.is_dir())

        check = Path(os.path.join(self.tmpdir, "test/hello"))
        self.assertTrue(check.exists())


if __name__ == '__main__':
    unittest.main()
