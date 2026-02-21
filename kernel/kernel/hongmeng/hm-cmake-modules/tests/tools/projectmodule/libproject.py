#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import tools.constants as constants
from tools.projectmodule.base.hmproject import HMProject
from tools.error.hmerror import HMParamError
from tools.error.hmerror import HMParamTypeError


class LibProject(HMProject):

    def __init__(self, target="sample", libType=constants.LIB_STATIC_TYPE):
        super().__init__()
        if libType not in constants.LIB_TYPE:
            raise HMParamError(libType)
        self.target = target
        self.__libType = libType
        self.__alrelady_proccess_file = False
        self.__lib_condition = "y"
        self.__lib_version = "0.1"
        self.__install_dir = "/usr/lib"

    def __proccess_project_hmbuild(self):
        self.rootHMbuild.add_lib(
            self.__libType, self.target, condition=self.__lib_condition)
        self.rootHMbuild.add_install(self.target, constants.LIB_INSTALL_DIR)
        self.rootHMbuild.add_libversion(self.target, self.__lib_version)

    def set_lib_type(self, libType):
        if libType not in constants.LIB_TYPE:
            raise HMParamError(libType)
        self.__libType = libType

    def set_lib_condition(self, condition):
        if not isinstance(condition, str):
            raise HMParamTypeError("condition", "str")
        self.__lib_condition = condition

    def set_lib_version(self, version):
        if not isinstance(version, str):
            raise HMParamTypeError("version", "str")
        self.__lib_version = version

    def set_target_install_dir(self, installpath):
        '''
        set value of install dir to `installpath`
        '''
        if not isinstance(installpath, str):
            raise HMParamTypeError("installpath", "str")
        self.__install_dir = installpath

    def before_write_to_disk(self):
        # only want to deal with it once.
        if self.__alrelady_proccess_file:
            return
        self.__proccess_project_hmbuild()
        self.__alrelady_proccess_file = True


if __name__ == '__main__':
    standLoneElf = LibProject()
    standLoneElf.before_write_to_disk()
    standLoneElf.write_to_disk(os.path.dirname(__file__))
