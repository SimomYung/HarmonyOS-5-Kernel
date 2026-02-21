#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Fri Feb 11 14:30:53 2022

from __future__ import absolute_import
import os
import unittest
from tools.projectmodule.libproject import LibProject
from tools.projectmodule.elfproject import ElfProject
from tools.buildmodule.buildproject import HmBuildProject
from tools.hmtest import HmBuildTestCase
import tools.constants as constants


class TestInstallElfPermission(HmBuildTestCase):
    '''
    Class for testing permission of libs.
    '''
    def setUp(self):
        '''
        The setUp() method allow you to define instructions that will be executed before each test method.
        '''
        super().setUp()
        self.elfProject = ElfProject()

        self.buildElfProject = HmBuildProject(
             self.elfProject, self.workPath, self.sysrootPath,
             self.insatllCmakeModlulePath)

    def tearDown(self):
        '''
        The tearDown() method allow you to define instructions that will be executed after each test method.
        '''
        super().tearDown()
        self.buildElfProject.clean_all()

    def test_elf_permission(self):
        '''
        test permission of elf file
        '''
        self.elfProject.set_target_install_dir(constants.ELF_INSTALL_DIR)

        self.buildElfProject.build()
        self.assertMaKeSuccess(self.buildElfProject)
        self.assert_permission_check_success(os.path.join(
            self.buildElfProject.installPath+"/usr/bin",
            self.elfProject.target), "755")


class TestInstallLibPermission(HmBuildTestCase):
    '''
    Class for testing permission of libs.
    '''
    def setUp(self):
        super().setUp()
        self.libProject = LibProject()
        self.libProject.add_project_header("test.h", ["test"])
        self.libProject.add_project_src(
            "test.c", ["test"], includes=["test.h"],
            target=self.libProject.target)

        self.libProject.add_include(self.libProject.rootHMbuild, "./",
                                    target=self.libProject.target)
        self.libProject.set_target_install_dir(constants.LIB_INSTALL_DIR)

        self.buildLibProject = HmBuildProject(
            self.libProject, self.workPath, self.sysrootPath,
            self.insatllCmakeModlulePath)

    def tearDown(self):
        super().tearDown()
        self.buildLibProject.clean_all()

    def test_dynamic_lib_permission(self):
        '''
        test permission of dynamic lib file
        '''
        self.libProject.set_lib_type(constants.LIB_DYNAMIC_TYPE)
        self.buildLibProject.build()

        self.assertMaKeSuccess(self.buildLibProject)
        self.assert_permission_check_success(os.path.join(
            self.buildLibProject.installPath+"/usr/lib",
            "libsample.so.0.1"), "644")

    def test_static_lib_permission(self):
        '''
        test permission of static lib file
        '''
        self.libProject.set_lib_type(constants.LIB_STATIC_TYPE)
        self.buildLibProject.build()

        self.assertMaKeSuccess(self.buildLibProject)
        self.assert_permission_check_success(os.path.join(
            self.buildLibProject.installPath+"/usr/lib",
            "libsample.a"), "644")

if __name__ == '__main__':
    unittest.main()
