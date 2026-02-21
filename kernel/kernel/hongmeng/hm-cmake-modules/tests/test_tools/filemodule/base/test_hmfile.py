#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path

from tools.filemodule.base.hmfile import HMFile
from tools.tooltest import ToolTestCase


class TestHMFile(ToolTestCase):

    def test_set_content(self):
        test = HMFile("test")
        test.set_content("test")
        self.assertEqual("test", test.content)

    def test_append_content(self):
        test = HMFile("test")
        test.append_content("test")
        self.assertEqual("test", test.content)

    def test_search_content(self):
        test = HMFile("test", "exec-static-y: test.elf\nsubdir-y: test")
        regrex = r"exec\-.*\-y:.*"
        findstr = test.search_content(regrex)
        self.assertIsNotNone(findstr)
        self.assertEqual(findstr, "exec-static-y: test.elf")

    def test_replace_content(self):
        test = HMFile("test", "exec-static-y: test.elf\nsubdir-y: test")
        regrex = r"exec\-.*\-y:.*"
        findstr = test.search_content(regrex)
        replacestr = findstr.replace("-y", "")
        test.replace_content(regrex, replacestr)
        regrex = r"exec\-.*:.*"
        findstr = test.search_content(regrex)
        self.assertIsNotNone(findstr)
        self.assertEqual(findstr, "exec-static: test.elf")

    def test_write_to_disk(self):
        test = HMFile("test", "exec-static-y: test.elf\nsubdir-y: test")
        test.write_to_disk(self.tmpdir)
        check = Path(os.path.join(self.tmpdir, "test"))
        self.assertTrue(check.exists())


if __name__ == '__main__':
    unittest.main()
