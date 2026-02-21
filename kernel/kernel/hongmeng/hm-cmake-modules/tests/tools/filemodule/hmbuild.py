#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

from tools.error.hmerror import HMParamTypeError
from tools.error.hmerror import HMParamError

from tools.filemodule.base.hmfile import HMFile
import tools.constants as constants

__all__ = ["HmbuildFile"]


class HmbuildFile(HMFile):

    def __init__(self):
        super().__init__(constants.HMBUILD_FILENAME)

    def add_cflags(self, flags, target=None, condition="y",
                   noNeedCondition=False):
        if not isinstance(flags, str):
            raise HMParamTypeError("flags", "str")
        if target:
            if not isinstance(target, str):
                raise HMParamTypeError("target", "str")
            if noNeedCondition:
                appendContent = ("%s-%s: %s\n" %
                                 (constants.CFLAGS_CMD, target,
                                  flags))
            else:
                appendContent = ("%s-%s-%s: %s\n" %
                                 (constants.CFLAGS_CMD, target,
                                  condition, flags))
        else:
            if noNeedCondition:
                appendContent = ("%s: %s\n" %
                                 (constants.CFLAGS_CMD, condition, flags))
            else:
                appendContent = ("%s-%s: %s\n" %
                                 (constants.CFLAGS_CMD, condition, flags))

        self.append_content(appendContent)

    def add_sub_dir(self, dirNmae, condition="y"):
        if not isinstance(dirNmae, str):
            raise HMParamTypeError("dirNmae", "str")

        content = ("%s-%s: %s\n" % (constants.SUBDIR_CMD, condition, dirNmae))
        self.append_content(content)

    def add_src(self, filename="", target="", condition="y"):
        if target:
            if condition:
                content = ("%s-%s-%s: %s\n" %
                           (constants.SRC_CMD, target, condition, filename))
            else:
                content = ("%s-%s: %s\n" %
                           (constants.SRC_CMD, target, filename))
        else:
            if condition:
                content = ("%s-%s: %s\n" %
                           (constants.SRC_CMD, condition, filename))
            else:
                content = ("%s: %s\n" %
                           (constants.SRC_CMD, filename))
        self.append_content(content)

    def del_src(self, filename):
        regrex = ((r"src\-.*: %s") % filename)
        self.replace_content(regrex, "")

    def add_installinc(self, filename="", installDir=None):
        if installDir:
            if not isinstance(installDir, str):
                raise HMParamTypeError("installDir", "str")
            content = ("%s-%s: %s\n" %
                       (constants.INSTALLINC_CMD, installDir, filename))
        else:
            content = ("%s: %s\n" %
                       (constants.INSTALLINC_CMD, filename))
        self.append_content(content)

    def add_exec(self, elfType, target, condition="y"):
        if elfType not in constants.ELF_TYPE:
            raise HMParamError(elfType)
        if not isinstance(target, str):
            raise HMParamTypeError("target", "str")

        content = ("%s-%s-%s: %s\n") % (constants.EXEC_CMD,
                                        elfType, condition, target)
        self.append_content(content)

    def add_install(self, target, installDir):
        if not isinstance(target, str):
            raise HMParamTypeError("target", "str")
        if not isinstance(installDir, str):
            raise HMParamTypeError("installDir", "str")

        content = ("%s-%s-y: %s\n") % (constants.INSTALL_CMD,
                                       target,
                                       installDir)
        self.append_content(content)

    def add_include(self, dirpath, target=None, condition="y",
                    noNeedCondition=False):
        if not isinstance(dirpath, str):
            raise HMParamTypeError("dirpath", "str")
        if target:
            if not isinstance(target, str):
                raise HMParamTypeError("target", "str")
            if noNeedCondition:
                appendContent = ("%s-%s: %s\n" %
                                 (constants.INCLUDE_CMD, target,
                                  dirpath))
            else:
                appendContent = ("%s-%s-%s: %s\n" %
                                 (constants.INCLUDE_CMD, target,
                                  condition, dirpath))
        else:
            if noNeedCondition:
                appendContent = ("%s: %s\n" %
                                 (constants.INCLUDE_CMD, condition, dirpath))
            else:
                appendContent = ("%s-%s: %s\n" %
                                 (constants.INCLUDE_CMD, condition, dirpath))

        self.append_content(appendContent)

    def add_lib(self, libType, target, condition="y"):
        if libType not in constants.LIB_TYPE:
            raise HMParamError(libType)
        if not isinstance(target, str):
            raise HMParamTypeError("target", "str")

        content = ("%s-%s-%s: %s\n") % (constants.LIB_CMD,
                                        libType, condition, target)
        self.append_content(content)

    def add_libversion(self, target, version):
        if not isinstance(target, str):
            raise HMParamTypeError("target", "str")
        if not isinstance(version, str):
            raise HMParamTypeError("version", "str")

        content = ("%s-%s: %s\n") % (constants.LIBVERSION_CMD,
                                     target,
                                     version)
        self.append_content(content)

    def add_linklib(self, target, libinfo, condition="y"):
        if not isinstance(target, str):
            raise HMParamTypeError("target", "str")
        if not isinstance(condition, str):
            raise HMParamTypeError("condition", "str")
        if not isinstance(libinfo, str):
            raise HMParamTypeError("libinfo", "str")

        content = ("%s-%s-%s: %s\n") % (constants.LINKLIB_CMD,
                                        target, condition, libinfo)
        self.append_content(content)

    def add_findpkg(self, pkg=""):
        if not isinstance(pkg, str):
            raise HMParamTypeError("pkg", "str")

        content = ("%s: %s\n") % (constants.FINDPKG_CMD, pkg)
        self.append_content(content)

    def add_inclib(self, libinfo, target=None):
        if not isinstance(libinfo, str):
            raise HMParamTypeError("libinfo", "str")

        if target:
            if not isinstance(target, str):
                raise HMParamTypeError("target", "str")
            content = ("%s-%s: %s\n") % (constants.INCLIB_CMD,
                                         target, libinfo)
        else:
            content = ("%s: %s\n") % (constants.INCLIB_CMD,
                                      libinfo)
        self.append_content(content)

    def add_output(self, target="", output=""):
        if not isinstance(target, str):
            raise HMParamTypeError("target", "str")
        if not isinstance(output, str):
            raise HMParamTypeError("output", "str")
        if target:
            content = ("%s-%s: %s\n") % (constants.OUTPUT_CMD, target, output)
        else:
            content = ("%s: %s\n") % (constants.OUTPUT_CMD, output)

        self.append_content(content)

    def add_install_findmodule(self, filename=""):
        if not isinstance(filename, str):
            raise HMParamTypeError("filename", "str")

        content = ("%s: %s\n") % (constants.INSTALL_FIND_MODULE_CMD, filename)
        self.append_content(content)
