#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from tools.projectmodule.elfproject import ElfProject
from tools.buildmodule.buildproject import HmBuildProject
from tools.hmtest import HmBuildTestCase


class TestMakeHeaderBinary(HmBuildTestCase):
    def setUp(self):
        super().setUp()
        self.elfProject = ElfProject()
        self.elfProject.add_project_header("header.h", ["test"])
        self.elfProject.add_installinc(filename="header.h")
        self.buildProject = HmBuildProject(
            self.elfProject, self.workPath, self.sysrootPath,
            self.insatllCmakeModlulePath)

    def tearDown(self):
        super().tearDown()
        self.buildProject.clean_all()

    def test_base(self):
        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)

    def test_enable_header_enable_binary(self):
        self.buildProject.set_make_binary("y")
        self.buildProject.set_make_header("y")

        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)
        self.assertFileExits(os.path.join(self.buildProject.buildPath,
                                          self.elfProject.target))
        self.assertFileExits(os.path.join(
            self.buildProject.installPath+"/usr/include",
            "header.h"))

    def test_disable_header_enable_binary(self):
        self.buildProject.set_make_binary("y")
        self.buildProject.set_make_header("n")

        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)
        self.assertFileExits(os.path.join(self.buildProject.buildPath,
                                          self.elfProject.target))
        self.assertFileNotExits(os.path.join(
            self.buildProject.installPath+"/usr/include",
            "header.h"))

    def test_enable_header_disable_binary(self):
        self.buildProject.set_make_binary("n")
        self.buildProject.set_make_header("y")

        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)
        self.assertFileNotExits(os.path.join(self.buildProject.buildPath,
                                             self.elfProject.target))
        self.assertFileExits(os.path.join(
            self.buildProject.installPath+"/usr/include",
            "header.h"))

    def test_disable_header_disable_binary(self):
        self.buildProject.set_make_binary("n")
        self.buildProject.set_make_header("n")

        self.buildProject.build()
        self.assertMaKeFail(self.buildProject)
        self.assertFileNotExits(os.path.join(self.buildProject.buildPath,
                                             self.elfProject.target))
        self.assertFileNotExits(os.path.join(
            self.buildProject.installPath+"/usr/include",
            "header.h"))


if __name__ == '__main__':
    unittest.main()
