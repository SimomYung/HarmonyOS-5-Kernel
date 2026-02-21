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
import tools.constants as constants


class TestInstallCmdBase(HmBuildTestCase):
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
        self.elfProject.set_target_install_dir(constants.ELF_INSTALL_DIR)

        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)
        self.assertFileExits(os.path.join(
            self.buildProject.installPath+"/usr/bin",
            self.elfProject.target))

    def test_no_target_should_cmake_fail(self):
        self.elfProject.before_write_to_disk()

        # replace Hmbuild install-*-y: xxx to install-y: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"install\-.*\-y:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-" + self.elfProject.target, "")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertCMaKeFail(self.buildProject)

    def test_error_target_should_cmake_fail(self):
        self.elfProject.before_write_to_disk()

        # replace Hmbuild install-elf-y: xxx to install-error-y: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"install\-.*\-y:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace(self.elfProject.target, "error")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertCMaKeFail(self.buildProject)

    def test_no_condition_should_cmake_fail(self):
        self.elfProject.before_write_to_disk()

        # replace Hmbuild install-elf-y: xxx to install-elf: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"install\-.*\-y:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-y", "")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertCMaKeFail(self.buildProject)

    def test_with_condtion_disable_shoud_make_fail(self):
        self.elfProject.add_bool_kconfig("TEST", defaultvalue="n")
        self.elfProject.before_write_to_disk()

        # replace Hmbuild install-elf-y: xxx to install-elf-CONFIG_TEST: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"install\-.*\-y:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-y", "-CONFIG_TEST")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)

    def test_with_condtion_enable_shoud_success(self):
        self.elfProject.add_bool_kconfig("TEST", defaultvalue="y")
        self.elfProject.before_write_to_disk()

        # replace Hmbuild install-elf-y: xxx to install-elf-CONFIG_TEST: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"install\-.*\-y:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-y", "-CONFIG_TEST")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)


if __name__ == '__main__':
    unittest.main()
