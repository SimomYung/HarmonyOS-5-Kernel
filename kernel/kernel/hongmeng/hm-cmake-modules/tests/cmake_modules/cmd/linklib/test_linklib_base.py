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


class TestLinkLibCmdBase(HmBuildTestCase):
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

        self.elfProject.add_inclib(
            self.elfProject.rootHMbuild, "TestPkg::TestPkg_include",
            target=self.elfProject.target)
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

    def test_no_target_should_cmake_fail(self):
        self.elfProject.before_write_to_disk()
        # replace Hmbuild linklib-target-y: xxx to linklib-y: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace(self.elfProject.target, "")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertCMaKeFail(self.buildProject)

    def test_target_error_should_make_fail(self):
        self.elfProject.before_write_to_disk()
        # replace Hmbuild linklib-target-y: xxx to linklib-y: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace(self.elfProject.target, "error")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)

    def test_no_condition_should_make_success(self):
        self.elfProject.before_write_to_disk()
        # replace Hmbuild linklib-target-y: xxx to linklib-target: xxx
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-y", "")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)

    def test_with_condition_disable_should_make_fail(self):
        self.elfProject.before_write_to_disk()
        self.elfProject.add_bool_kconfig("TEST", defaultvalue="n")

        # replace Hmbuild linklib-target-y: xxx to linklib-target-CONFIG_TEST
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-y", "-CONFIG_TEST")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)

    def test_with_condition_enable_should_make_success(self):
        self.elfProject.before_write_to_disk()
        self.elfProject.add_bool_kconfig("TEST", defaultvalue="y")

        # replace Hmbuild linklib-target-y: xxx to linklib-target-CONFIG_TEST
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("-y", "-CONFIG_TEST")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeSuccess(self.buildProject)

    def test_no_lib_should_make_fail(self):
        self.elfProject.before_write_to_disk()

        # replace Hmbuild linklib-target-y: xxx to linklib-target-CONFIG_TEST
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("TestPkg::TestPkg_shared", "")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)

    def test_lib_error_should_make_fail(self):
        self.elfProject.before_write_to_disk()

        # replace Hmbuild linklib-target-y: xxx to linklib-target-CONFIG_TEST
        projectHmbuild = self.elfProject.rootHMbuild
        regrex = r"linklib\-.*:.*"
        findstr = projectHmbuild.search_content(regrex)
        self.assertIsNotNone(findstr)
        replacestr = findstr.replace("TestPkg::TestPkg_shared", "error")
        projectHmbuild.replace_content(regrex, replacestr)

        self.buildProject.build()

        self.assertMaKeFail(self.buildProject)


if __name__ == '__main__':
    unittest.main()
