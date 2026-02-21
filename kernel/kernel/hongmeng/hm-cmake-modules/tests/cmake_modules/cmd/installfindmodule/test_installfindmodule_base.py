#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from tools.filemodule.base.hmfile import HMFile
from tools.projectmodule.elfproject import ElfProject
from tools.buildmodule.buildproject import HmBuildProject
from tools.hmtest import HmBuildTestCase


class TestInstallFindModulecCmd(HmBuildTestCase):
    def setUp(self):
        super().setUp()
        self.elfProject = ElfProject()
        findFile = HMFile("FindTest.cmake")
        self.elfProject.add_file(findFile)
        self.buildProject = HmBuildProject(
            self.elfProject, self.workPath, self.sysrootPath,
            self.insatllCmakeModlulePath)

    def tearDown(self):
        super().tearDown()
        self.buildProject.clean_all()

    def test_base(self):
        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)
        self.assertFileExits(os.path.join(self.buildProject.buildPath,
                                          self.elfProject.target))

    def test_right_file_should_install_success(self):
        self.elfProject.add_install_find_module(filename="FindTest.cmake")
        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)
        checkfile = os.path.join(
            self.buildProject.installPath+"/usr/share/cmake/Modules",
            "FindTest.cmake")
        self.assertFileExits(checkfile)

    def test_error_file_should_make_fail(self):
        self.elfProject.add_install_find_module(filename="error.cmake")
        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)

    def test_no_file_should_install_success(self):
        self.elfProject.add_install_find_module(filename="")
        self.buildProject.build()

        self.assertCMaKeFail(self.buildProject)


if __name__ == '__main__':
    unittest.main()
