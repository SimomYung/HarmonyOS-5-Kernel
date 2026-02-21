#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path

from tools.filemodule.srcfile import SrcFile
from tools.tooltest import ToolTestCase
from tools.error.hmerror import HMFunctionNotExitsError


class TestHMBuildFile(ToolTestCase):

    def test_write_to_disk(self):
        src = SrcFile("test.c", ["test"])
        src.write_to_disk(self.tmpdir)
        check = Path(os.path.join(self.tmpdir, "test.c"))
        self.assertTrue(check.exists())

    def test_add_func(self):
        src = SrcFile("test.c", ["foo"])
        src.add_func("test")
        src.write_to_disk(self.tmpdir)

        regex = r"int test\(void\)"
        findstr = src.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_include(self):
        src = SrcFile("test.c", ["foo"])
        src.add_include("test.h")
        src.write_to_disk(self.tmpdir)

        regex = r"#include <test.h>"
        findstr = src.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_compile_macro_must_have_func(self):
        src = SrcFile("test.c", ["foo"])
        with self.assertRaises(HMFunctionNotExitsError):
            src.add_function_compile_macro("error", "TEST")

    def test_add_compile_macro(self):
        src = SrcFile("test.c", ["foo"])
        src.add_function_compile_macro("foo", "TEST")
        src.write_to_disk(self.tmpdir)

        regex = r"#ifdef TEST"
        findstr = src.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_call_func(self):
        src = SrcFile("test.c", ["foo"])
        src.add_call_function("foo", "foo1")
        src.write_to_disk(self.tmpdir)

        regex = r"foo1\(\);"
        findstr = src.search_content(regex)
        self.assertIsNotNone(findstr)


if __name__ == '__main__':
    unittest.main()
