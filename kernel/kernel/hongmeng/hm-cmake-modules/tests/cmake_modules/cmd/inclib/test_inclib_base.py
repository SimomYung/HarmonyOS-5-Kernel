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


class TestIncLibCmdBase(HmBuildTestCase):
    def setUp(self):
        super().setUp()
        self.elfProject = ElfProject()

        # main need call pkg and include test_pkg
        self.elfProject.add_main_call_function("pkg")
        self.elfProject.add_main_includes("test_pkg.h")

        self.elfProject.add_findpkg(self.elfProject.rootHMbuild, "TestPkg")
        self.elfProject.add_linklib(
            self.elfProject.rootHMbuild,
            self.elfProject.target, "TestPkg::TestPkg_shared")

        self.buildProject = HmBuildProject(
            self.elfProject, self.workPath, self.sysrootPath,
            self.insatllCmakeModlulePath)

    def tearDown(self):
        super().tearDown()
        self.buildProject.clean_all()

    def test_elf_target_should_make_success(self):
        self.elfProject.add_inclib(self.elfProject.rootHMbuild,
                                   "TestPkg::TestPkg_include",
                                   target=self.elfProject.target)
        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)
        self.assertFileExits(os.path.join(self.buildProject.buildPath,
                                          self.elfProject.target))

    def test_dir_target_should_make_success(self):
        self.elfProject.add_inclib(self.elfProject.rootHMbuild,
                                   "TestPkg::TestPkg_include",
                                   target=".")
        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)

    def test_no_target_should_make_success(self):
        self.elfProject.add_inclib(self.elfProject.rootHMbuild,
                                   "TestPkg::TestPkg_include")
        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)

    def test_src_target_should_make_success(self):
        self.elfProject.add_inclib(self.elfProject.rootHMbuild,
                                   "TestPkg::TestPkg_include",
                                   target="main.c")
        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)

    def test_no_inclib_should_make_fail(self):
        self.buildProject.build()
        self.assertMaKeFail(self.buildProject)

    def test_error_include_should_cmake_fail(self):
        self.elfProject.add_inclib(self.elfProject.rootHMbuild,
                                   "-Ierror",
                                   target="main.c")
        self.buildProject.build()

        self.assertCMaKeFail(self.buildProject)

    def test_empty_include_should_cmake_fail(self):
        self.elfProject.add_inclib(self.elfProject.rootHMbuild,
                                   "")

        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)


if __name__ == '__main__':
    unittest.main()
