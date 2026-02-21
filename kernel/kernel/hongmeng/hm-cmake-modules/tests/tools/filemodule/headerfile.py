#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

from tools.filemodule.base.hmfile import HMFile
from tools.error.hmerror import HMParamTypeError

__all__ = ["HeaderFile"]


class HeaderFile(HMFile):

    def __init__(self, filename, functions=None):
        super().__init__(filename)
        if functions:
            if not isinstance(functions, list):
                raise HMParamTypeError("functions", "list")
            self.__functions = functions
        else:
            self.__functions = []

    def __create_file_mocro(self):
        return "__"+self.filename.strip().replace(".", "_").upper()

    def __generate_content(self):
        content = "#ifndef " + self.__create_file_mocro() + "\n"
        content += "#define " + self.__create_file_mocro() + "\n"
        # now only support int fun(void);
        for func in self.__functions:
            content += "int " + func + "(void);\n"
        content += "#endif"
        self.set_content(content)
        return content

    def add_function(self, func):
        if not isinstance(func, str):
            raise HMParamTypeError("func", "list")
        self.__functions.append(func)

    def write_to_disk(self, path):
        self.__generate_content()
        super().write_to_disk(path)
