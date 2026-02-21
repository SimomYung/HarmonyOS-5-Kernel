#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path

from tools.filemodule.hmbuild import HmbuildFile
from tools.tooltest import ToolTestCase
import tools.constants as constants
from tools.error.hmerror import HMParamError


class TestHMBuildFile(ToolTestCase):

    def test_write_to_disk(self):
        hmbuild = HmbuildFile()
        hmbuild.write_to_disk(self.tmpdir)
        check = Path(os.path.join(self.tmpdir, constants.HMBUILD_FILENAME))
        self.assertTrue(check.exists())

    def test_add_cflag_default(self):
        hmbuild = HmbuildFile()
        hmbuild.add_cflags("-DTEST")
        expect = "cflags-y: -DTEST\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_cflag_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_cflags("-DTEST", target="test")
        expect = "cflags-test-y: -DTEST\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_cflag_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_cflags("-DTEST", target="test", condition="CONFIG_TEST")
        expect = "cflags-test-CONFIG_TEST: -DTEST\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_cflag_NOcondition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_cflags("-DTEST", target="test",
                           condition="CONFIG_TEST", noNeedCondition=True)
        expect = "cflags-test: -DTEST\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_subdir(self):
        hmbuild = HmbuildFile()
        hmbuild.add_sub_dir("subdir")
        expect = "subdir-y: subdir\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_subdir_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_sub_dir("subdir", "CONFIG_TEST")
        expect = "subdir-CONFIG_TEST: subdir\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_src_default(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c")
        expect = "src-y: test.c\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_src_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", target="test")
        expect = "src-test-y: test.c\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_src_with_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", target="test", condition="CONFIG_TEST")
        expect = "src-test-CONFIG_TEST: test.c\n"
        self.assertEqual(hmbuild.content, expect)

        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", condition="CONFIG_TEST")
        expect = "src-CONFIG_TEST: test.c\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_src_no_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", target="test", condition="")
        expect = "src-test: test.c\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_src_no_src(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src(filename="", target="test", condition="")
        expect = "src-test: \n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_src_no_src_target_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src(filename="", target="", condition="")
        expect = "src: \n"
        self.assertEqual(hmbuild.content, expect)

    def test_del_src_default(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c")
        expect = "src-y: test.c\n"
        self.assertEqual(hmbuild.content, expect)

        hmbuild.del_src("test.c")
        expect = "\n"
        self.assertEqual(hmbuild.content, expect)

    def test_del_src_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", target="test")
        expect = "src-test-y: test.c\n"
        self.assertEqual(hmbuild.content, expect)

        hmbuild.del_src("test.c")
        expect = "\n"
        self.assertEqual(hmbuild.content, expect)

    def test_del_src_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", target="test", condition="CONFIG_TEST")
        expect = "src-test-CONFIG_TEST: test.c\n"
        self.assertEqual(hmbuild.content, expect)

        hmbuild = HmbuildFile()
        hmbuild.add_src("test.c", condition="CONFIG_TEST")
        expect = "src-CONFIG_TEST: test.c\n"
        self.assertEqual(hmbuild.content, expect)

        hmbuild.del_src("test.c")
        expect = "\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_installinc_with_filename_no_dir(self):
        hmbuild = HmbuildFile()
        hmbuild.add_installinc("test.h")
        expect = "installinc: test.h\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_installinc_with_filename_with_dir(self):
        hmbuild = HmbuildFile()
        hmbuild.add_installinc("test.h", "fs")
        expect = "installinc-fs: test.h\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_installinc_no_filename_with_dir(self):
        hmbuild = HmbuildFile()
        hmbuild.add_installinc(installDir="fs")
        expect = "installinc-fs: \n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_installinc_no_filename_no_dir(self):
        hmbuild = HmbuildFile()
        hmbuild.add_installinc()
        expect = "installinc: \n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_dynamic_exec(self):
        hmbuild = HmbuildFile()
        hmbuild.add_exec(constants.EXEC_DYNAMIC_TYPE, "test.elf")
        expect = "exec-dynamic-y: test.elf\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_error_exec(self):
        hmbuild = HmbuildFile()
        with self.assertRaises(HMParamError):
            hmbuild.add_exec("error", "test.elf")

    def test_add_install(self):
        hmbuild = HmbuildFile()
        hmbuild.add_install("test", "BINDIR")
        expect = "install-test-y: BINDIR\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_include_default(self):
        hmbuild = HmbuildFile()
        hmbuild.add_include("test")
        expect = "include-y: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_include_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_include("test", target="test.elf")
        expect = "include-test.elf-y: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_include_no_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_include("test", target="test.elf", noNeedCondition=True)
        expect = "include-test.elf: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_include_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_include("test", target="test.elf", condition="CONFIG_TEST")
        expect = "include-test.elf-CONFIG_TEST: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_lib(self):
        hmbuild = HmbuildFile()
        hmbuild.add_lib(constants.LIB_DYNAMIC_TYPE, target="sample")
        expect = "lib-dynamic-y: sample\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_lib_with_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_lib(constants.LIB_STATIC_TYPE,
                        target="sample", condition="CONFIG_TEST")
        expect = "lib-static-CONFIG_TEST: sample\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_linklib(self):
        hmbuild = HmbuildFile()
        hmbuild.add_linklib("sample", "libinfo")
        expect = "linklib-sample-y: libinfo\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_linklib_with_condition(self):
        hmbuild = HmbuildFile()
        hmbuild.add_linklib("sample", "libinfo", condition="CONFIG_TEST")
        expect = "linklib-sample-CONFIG_TEST: libinfo\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_inclib(self):
        hmbuild = HmbuildFile()
        hmbuild.add_inclib("libinfo", target="sample")
        expect = "inclib-sample: libinfo\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_inclib_no_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_inclib("libinfo")
        expect = "inclib: libinfo\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_output_no_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_output(output="test")
        expect = "output: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_output_with_target(self):
        hmbuild = HmbuildFile()
        hmbuild.add_output(target="target", output="test")
        expect = "output-target: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_output_no_output(self):
        hmbuild = HmbuildFile()
        hmbuild.add_output(target="target")
        expect = "output-target: \n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_findpkg_with_pkg(self):
        hmbuild = HmbuildFile()
        hmbuild.add_findpkg(pkg="target")
        expect = "findpkg: target\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_findpkg_no_pkg(self):
        hmbuild = HmbuildFile()
        hmbuild.add_findpkg(pkg="")
        expect = "findpkg: \n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_install_find_with_file(self):
        hmbuild = HmbuildFile()
        hmbuild.add_install_findmodule(filename="test")
        expect = "installfindmodule: test\n"
        self.assertEqual(hmbuild.content, expect)

    def test_add_install_find_no_file(self):
        hmbuild = HmbuildFile()
        hmbuild.add_install_findmodule(filename="")
        expect = "installfindmodule: \n"
        self.assertEqual(hmbuild.content, expect)


if __name__ == '__main__':
    unittest.main()
