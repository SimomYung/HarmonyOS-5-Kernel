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


class ElfProject(HMProject):

    def __init__(self, target="test.elf", elfType=constants.EXEC_DYNAMIC_TYPE):
        super().__init__()
        if elfType not in constants.ELF_TYPE:
            raise HMParamError(elfType)
        self.target = target
        self.__elfType = elfType
        self.__main_file_name = "main.c"
        self.__main_includes = []
        self.__main_call_function = []
        self.__alrelady_proccess_file = False
        self.__main_conditional_compilation = None
        self.__install_dir = constants.ELF_INSTALL_DIR

    def __add_main_file(self):
        srcFile = self.add_project_src(self.__main_file_name, [
            "main"], self.__main_includes, target=self.target)
        for f in self.__main_call_function:
            srcFile.add_call_function("main", f)
        if self.__main_conditional_compilation:
            srcFile.add_function_compile_macro(
                "main", self.__main_conditional_compilation)

    def __proccess_project_hmbuild(self):
        self.rootHMbuild.add_exec(self.__elfType, self.target)
        self.rootHMbuild.add_install(self.target, self.__install_dir)

    def set_main_conditional_compilation(self, macroName):
        if not isinstance(macroName, str):
            raise HMParamTypeError("macroName", "str")
        self.__main_conditional_compilation = macroName

    def set_elf_type(self, elfType):
        if elfType not in constants.ELF_TYPE:
            raise HMParamError(elfType)
        self.__elfType = elfType

    def set_main_file_name(self, name):
        if not isinstance(name, str):
            raise HMParamTypeError("name", "str")
        self.__main_file_name = name

    def set_target_install_dir(self, installpath):
        if not isinstance(installpath, str):
            raise HMParamTypeError("installpath", "str")
        self.__install_dir = installpath

    def add_main_call_function(self, functionName):
        if not isinstance(functionName, str):
            raise HMParamTypeError("functionName", "str")
        self.__main_call_function.append(functionName)

    def add_main_includes(self, includeName):
        if not isinstance(includeName, str):
            raise HMParamTypeError("includeName", "str")
        self.__main_includes.append(includeName)

    def before_write_to_disk(self):
        # only want to deal with it once.
        if self.__alrelady_proccess_file:
            return
        self.__add_main_file()
        self.__proccess_project_hmbuild()
        self.__alrelady_proccess_file = True


if __name__ == '__main__':
    standLoneElf = ElfProject()
    standLoneElf.before_write_to_disk()
    standLoneElf.write_to_disk(os.path.dirname(__file__))
