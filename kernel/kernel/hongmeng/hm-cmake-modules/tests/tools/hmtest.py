#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: This is Hmbuild Base test class
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import unittest
from pathlib import Path
import tools.constants as constants


class HmBuildTestCase(unittest.TestCase):
    def assertFileExits(self, filePath, msg=None):
        checkPath = Path(filePath)
        if not checkPath.exists():
            msg = self._formatMessage(msg, "%s is not exits" % (filePath))
            raise self.failureException(msg)

    def assertFileNotExits(self, filePath, msg=None):
        checkPath = Path(filePath)
        if checkPath.exists():
            msg = self._formatMessage(msg, "%s is exits" % (filePath))
            raise self.failureException(msg)

    def assert_permission_check_success(self, file_path, permission, msg=None):
        '''
        assert permission of `file_path` is `permission`
        '''
        if not oct(os.stat(file_path).st_mode)[-3:] == permission:
            msg = self._formatMessage(msg, "Permission of %s check failed" % (file_path))
            raise self.failureException(msg)

    def assert_permission_check_fail(self, file_path, permission, msg=None):
        '''
        assert permission of `file_path` isn't `permission`
        '''
        if oct(os.stat(file_path).st_mode)[-3:] == permission:
            msg = self._formatMessage(msg, "Permission of %s check success" % (file_path))
            raise self.failureException(msg)

    def assertCMaKeSuccess(self, buildproject, msg=None):
        if buildproject.cmakeRet != constants.CMAKE_SUCCESS:
            msg = self._formatMessage(msg, "Failed to cmake")
            raise self.failureException(msg)

    def assertCMaKeFail(self, buildproject, msg=None):
        if buildproject.cmakeRet == constants.CMAKE_SUCCESS:
            msg = self._formatMessage(msg, "Cmake success")
            raise self.failureException(msg)

    def assertMaKeSuccess(self, buildproject, msg=None):
        if buildproject.cmakeRet != constants.CMAKE_SUCCESS:
            msg = self._formatMessage(
                msg, "Hope to make success, but cmake has failed")
            raise self.failureException(msg)

        if buildproject.makeRet != constants.MAKE_SUCCESS:
            msg = self._formatMessage(
                msg, "Failed to make")
            raise self.failureException(msg)

    def assertMaKeFail(self, buildproject, msg=None):
        if buildproject.cmakeRet != constants.CMAKE_SUCCESS:
            msg = self._formatMessage(
                msg, "Hope to make fail, but cmake has failed")
            raise self.failureException(msg)

        if buildproject.makeRet == constants.MAKE_SUCCESS:
            msg = self._formatMessage(
                msg, "Make success")
            raise self.failureException(msg)

    def setUp(self):
        self.workPath = os.getenv("TEST_WORK_PATH")
        self.sysrootPath = os.getenv("TEST_SYS_ROOT_PATH")
        self.insatllCmakeModlulePath = os.getenv(
            "TEST_CMAKE_MODULE_INSTALL_PATH")

    def tearDown(self):
        return
