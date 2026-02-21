#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: This is Hmbuild Base test class
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
import shutil


class ToolTestCase(unittest.TestCase):
    def setUp(self):
        curdir = os.path.dirname(__file__)
        self.tmpdir = os.path.join(curdir, "tmp")
        os.makedirs(self.tmpdir)

    def tearDown(self):
        shutil.rmtree(self.tmpdir)
