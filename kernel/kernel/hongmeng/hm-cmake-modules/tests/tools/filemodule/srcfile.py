#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

from tools.filemodule.base.hmfile import HMFile
from tools.error.hmerror import HMParamTypeError
from tools.error.hmerror import HMFunctionNotExitsError

__all__ = ["SrcFile"]


class SrcFile(HMFile):

    def __init__(self, filename, functions, includes=None):
        super().__init__(filename)
        if not isinstance(functions, list):
            raise HMParamTypeError("functions", "list")
        self.__functions = functions

        if includes:
            if not isinstance(includes, list):
                raise HMParamTypeError("includes", "list")
            self.__includes = includes
        else:
            self.__includes = []

        self.__call_func_info = {}
        self.__func_macro = {}

    def __generate_content(self):
        content = ""
        for inc in self.__includes:
            content += "#include <" + inc + ">\n"
        # now only support int fun(void){ return 0};
        for func in self.__functions:
            if func in self.__func_macro:
                macro = self.__func_macro[func]
                content += ("#ifdef %s\n" % macro)
            content += "int " + func + "(void) {\n"

            if func in self.__call_func_info:
                calls = self.__call_func_info[func]
                for c in calls:
                    content += ("    %s();\n" % (c))
            content += "    return 0; \n}\n"

            if func in self.__func_macro:
                content += ("#endif\n")
        self.set_content(content)
        return content

    def add_call_function(self, srcFuc, destFunc):
        if not isinstance(srcFuc, str):
            raise HMParamTypeError("srcFuc", "str")
        if not isinstance(destFunc, str):
            raise HMParamTypeError("destFunc", "str")
        if srcFuc not in self.__functions:
            raise HMFunctionNotExitsError(srcFuc)
        if srcFuc not in self.__call_func_info:
            self.__call_func_info[srcFuc] = []
        self.__call_func_info[srcFuc].append(destFunc)

    def add_function_compile_macro(self, func, macroName):
        if not isinstance(func, str):
            raise HMParamTypeError("func", "str")
        if not isinstance(macroName, str):
            raise HMParamTypeError("macroName", "str")
        if func not in self.__functions:
            raise HMFunctionNotExitsError(func)

        self.__func_macro[func] = macroName

    def add_func(self, func):
        if not isinstance(func, str):
            raise HMParamTypeError("func", "str")
        self.__functions.append(func)

    def add_include(self, include):
        if not isinstance(include, str):
            raise HMParamTypeError("include", "str")
        self.__includes.append(include)

    def write_to_disk(self, path):
        self.__generate_content()
        super().write_to_disk(path)
