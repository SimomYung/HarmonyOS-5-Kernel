#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest

if __name__ == "__main__":
    curdir = os.path.dirname(__file__)
    discover = unittest.defaultTestLoader.discover(curdir, pattern="test_*.py")
    runner = unittest.TextTestRunner()
    runner.run(discover)
