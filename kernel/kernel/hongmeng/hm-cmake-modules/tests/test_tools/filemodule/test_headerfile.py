#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path

from tools.filemodule.headerfile import HeaderFile
from tools.tooltest import ToolTestCase


class TestHMHeaderFile(ToolTestCase):

    def test_write_to_disk(self):
        test = HeaderFile("test.h")
        test.write_to_disk(self.tmpdir)
        check = Path(os.path.join(self.tmpdir, "test.h"))
        self.assertTrue(check.exists())


if __name__ == '__main__':
    unittest.main()
