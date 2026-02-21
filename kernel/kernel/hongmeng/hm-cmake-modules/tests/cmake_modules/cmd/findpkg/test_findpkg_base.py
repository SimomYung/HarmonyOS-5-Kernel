#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from tools.projectmodule.elfproject import ElfProject
from tools.buildmodule.buildproject import HmBuildProject
from tools.hmtest import HmBuildTestCase


class TestFindpkgCmdBase(HmBuildTestCase):
    def setUp(self):
        super().setUp()
        self.elfProject = ElfProject()

        self.buildProject = HmBuildProject(
            self.elfProject, self.workPath, self.sysrootPath,
            self.insatllCmakeModlulePath)

    def tearDown(self):
        super().tearDown()
        self.buildProject.clean_all()

    def test_base(self):
        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)

    def test_have_right_pkg_should_make_success(self):
        self.elfProject.add_findpkg(self.elfProject.rootHMbuild, "TestPkg")

        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)
        self.assertFileExits(os.path.join(self.buildProject.buildPath,
                                          self.elfProject.target))

    def test_have_error_pkg_should_cmake_fail(self):
        self.elfProject.add_findpkg(self.elfProject.rootHMbuild, "error")

        self.buildProject.build()
        self.assertCMaKeSuccess(self.buildProject)

    def test_no_pkg_should_make_success(self):
        self.elfProject.add_findpkg(self.elfProject.rootHMbuild, "")

        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)


if __name__ == '__main__':
    unittest.main()
