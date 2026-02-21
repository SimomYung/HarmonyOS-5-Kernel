#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path

from tools.projectmodule.base.hmproject import HMProject
from tools.tooltest import ToolTestCase
import tools.constants as constants


class TestHMBuildFile(ToolTestCase):

    def test_init(self):
        project = HMProject("sample")
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath))
        self.assertTrue(check.is_dir())

        check = Path(os.path.join(project.projectpath,
                     constants.HMBUILD_FILENAME))
        self.assertTrue(check.exists())

        check = Path(os.path.join(project.projectpath,
                     constants.KCONFIG_FILENAME))
        self.assertTrue(check.exists())

        check = Path(os.path.join(project.projectpath,
                     constants.CMAKE_FILENAME))
        self.assertTrue(check.exists())

    def test_add_project_dir(self):
        project = HMProject("sample")
        project.add_project_dir("subdir1/subdir2")
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "subdir1"))
        self.assertTrue(check.is_dir())
        check = Path(os.path.join(project.projectpath, "subdir1/HMbuild"))
        self.assertTrue(check.is_file())

        check = Path(os.path.join(project.projectpath, "subdir1/subdir2"))
        self.assertTrue(check.is_dir())
        check = Path(os.path.join(
            project.projectpath, "subdir1/subdir2/HMbuild"))
        self.assertTrue(check.is_file())

    def test_add_project_src(self):
        project = HMProject("sample")
        project.add_project_src("test.c", ["test"])
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "test.c"))
        self.assertTrue(check.is_file())

        regex = "src-y: test.c"
        findstr = project.rootHMbuild.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_project_src_with_target(self):
        project = HMProject("sample")
        project.add_project_src("test.c", ["test"], target="test.elf")
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath,
                     constants.HMBUILD_FILENAME))
        self.assertTrue(check.is_file())

        regex = "src-test.elf-y: test.c"
        findstr = project.rootHMbuild.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_project_src_with_includes(self):
        project = HMProject("sample")
        project.add_project_src(
            "test.c", ["test"], target="test.elf", includes=["test.h"])
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "test.c"))
        self.assertTrue(check.is_file())

        srcfile = project.get_file_from_project("test.c")
        regex = "#include <test.h>"
        findstr = srcfile.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_project_src_with_dir(self):
        project = HMProject("sample")
        project.add_project_dir("test")

        project.add_project_src(
            "test.c", ["test"], dirpath="test")
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "test/test.c"))
        self.assertTrue(check.is_file())

    def test_add_project_src_with_condition(self):
        project = HMProject("sample")
        project.add_project_src(
            "test.c", ["test"], condition="CONFIG_TEST")
        project.write_to_disk(self.tmpdir)

        regex = "src-CONFIG_TEST: test.c"
        findstr = project.rootHMbuild.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_del_project_src_with_dir(self):
        project = HMProject("sample")
        project.add_project_dir("test")

        project.add_project_src(
            "test.c", ["test"], dirpath="test")
        project.del_project_src("test.c", dirpath="test")
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "test/test.c"))
        self.assertFalse(check.exists())

    def test_add_project_header(self):
        project = HMProject("sample")
        project.add_project_header("test.h", ["test"])
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "test.h"))
        self.assertTrue(check.is_file())

        headerfile = project.get_file_from_project("test.h")
        regex = r"int test\(void\);"
        findstr = headerfile.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_project_header_with_dir(self):
        project = HMProject("sample")
        project.add_project_dir("test")

        project.add_project_header("test.h", ["test"], dirpath="test")
        project.write_to_disk(self.tmpdir)

        check = Path(os.path.join(project.projectpath, "test/test.h"))
        self.assertTrue(check.is_file())

    def test_add_project_header_with_install(self):
        project = HMProject("sample")
        project.add_project_header("test.h", ["test"], needInstall=True)
        project.write_to_disk(self.tmpdir)

        regex = "installinc: test.h"
        findstr = project.rootHMbuild.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_project_header_with_subdir_install(self):
        project = HMProject("sample")
        project.add_project_header(
            "test.h", ["test"], needInstall=True, installsubdir="fs")
        project.write_to_disk(self.tmpdir)

        regex = "installinc-fs: test.h"
        findstr = project.rootHMbuild.search_content(regex)
        self.assertIsNotNone(findstr)

    def test_add_bool_kconfig(self):
        project = HMProject("sample")
        project.add_bool_kconfig("TEST")
        project.write_to_disk(self.tmpdir)

        regex = "config TEST"
        findstr = project.Kconfig.search_content(regex)
        self.assertIsNotNone(findstr)


if __name__ == '__main__':
    unittest.main()
