#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import re
from tools.error.hmerror import HMParamTypeError
from tools.error.hmerror import HMParamForamtError
from tools.error.hmerror import HMDirNotExitsError


from tools.filemodule.base.hmdir import HMDir
from tools.filemodule.cmakefile import HMCmakeFile
from tools.filemodule.hmkconfig import HMKconfigFile
from tools.filemodule.hmbuild import HmbuildFile
from tools.filemodule.srcfile import SrcFile
from tools.filemodule.headerfile import HeaderFile

import tools.constants as constants


__all__ = ["HMProject"]


class HMProject(object):

    def __init__(self, projectName="sample"):
        if not isinstance(projectName, str):
            raise HMParamTypeError("projectName", "str")
        self.projectName = projectName
        self.Kconfig = HMKconfigFile()
        self.rootCmake = HMCmakeFile()
        self.rootHMbuild = HmbuildFile()
        self.__projectDir = HMDir(projectName)
        self.__projectDir.add_file(self.Kconfig)
        self.__projectDir.add_file(self.rootCmake)
        self.__projectDir.add_file(self.rootHMbuild)
        self.projectpath = None

    def get_file_from_project(self, filepath):
        dirs = filepath.split("/")
        filename = dirs[len(dirs)-1]
        dirs = dirs[0:len(dirs)-1]
        parentDir = self.__projectDir
        for d in dirs:
            parentDir = parentDir.get_dir(d)
            if not parentDir:
                raise HMDirNotExitsError(d)

        return parentDir.get_file(filename)

    def get_dir_from_project(self, dirpath):
        dirs = dirpath.split("/")
        parentDir = self.__projectDir
        for d in dirs:
            parentDir = parentDir.get_dir(d)
            if not parentDir:
                raise HMDirNotExitsError(dirpath)
        return parentDir

    def __add_dir(self, parentDir, dirNmae):
        if not isinstance(parentDir, HMDir):
            raise HMParamTypeError("parentDir", "HMDir")

        if parentDir.get_dir(dirNmae):
            return parentDir.get_dir(dirNmae)

        newDir = HMDir(dirNmae)
        hmbuild = HmbuildFile()
        cmake = HMCmakeFile(HMCmakeFile.DEF_ONLY_HMBUILD_CONTENT)
        newDir.add_file(hmbuild)
        newDir.add_file(cmake)
        parentDir.add_dir(newDir)

        parentHmbuildFile = parentDir.get_file(constants.HMBUILD_FILENAME)
        if not isinstance(parentHmbuildFile, HmbuildFile):
            raise HMParamTypeError("parentHmbuildFile", "HmbuildFile")

        parentHmbuildFile.add_sub_dir(dirNmae)

        return newDir

    def __add_src(self, parentDir, filename, functions, includes=None,
                  target=None, condition="y"):
        if not isinstance(parentDir, HMDir):
            raise HMParamTypeError("parentDir", "HMDir")

        if target:
            if not isinstance(target, str):
                raise HMParamTypeError("target", "str")

        if not isinstance(condition, str):
            raise HMParamTypeError("condition", "str")

        if parentDir.get_file(filename):
            return parentDir.get_file(filename)
        newSrc = SrcFile(filename, functions, includes)
        parentDir.add_file(newSrc)

        # parent dir's hmbuild add :src-target-y: filename
        parentHmbuildFile = parentDir.get_file(constants.HMBUILD_FILENAME)
        if not isinstance(parentHmbuildFile, HmbuildFile):
            raise HMParamTypeError("parentHmbuildFile", "HmbuildFile")
        parentHmbuildFile.add_src(filename, target, condition)
        return newSrc

    def __del_src(self, parentDir, filename):
        if not isinstance(parentDir, HMDir):
            raise HMParamTypeError("parentDir", "HMDir")

        parentDir.remove_file(filename)

        # parent dir's hmbuild del :src-*: filename
        parentHmbuildFile = parentDir.get_file(constants.HMBUILD_FILENAME)
        if not isinstance(parentHmbuildFile, HmbuildFile):
            raise HMParamTypeError("parentHmbuildFile", "HmbuildFile")
        parentHmbuildFile.del_src(filename)

    def __add_header(self, parentDir, filename,
                     functions, needInstall, installDir=None):
        if not isinstance(parentDir, HMDir):
            raise HMParamTypeError("parentDir", "HMDir")

        if parentDir.get_file(filename):
            return parentDir.get_file(filename)
        newheader = HeaderFile(filename, functions)
        parentDir.add_file(newheader)
        if not needInstall:
            return

        # parent dir's hmbuild add :installinc: filename
        parentHmbuildFile = parentDir.get_file(constants.HMBUILD_FILENAME)
        if not isinstance(parentHmbuildFile, HmbuildFile):
            raise HMParamTypeError("parentHmbuildFile", "HmbuildFile")
        parentHmbuildFile.add_installinc(filename, installDir)

    def add_project_dir(self, dirpath):
        path_re = re.compile("[a-zA-Z0-9-_.]+")
        if not isinstance(dirpath, str):
            raise HMParamTypeError("dirpath", "str")
        dirs = dirpath.split("/")
        parentDir = self.__projectDir
        for d in dirs:
            if not path_re.match(d):
                raise HMParamForamtError(d)
            parentDir = self.__add_dir(parentDir, d)

    def add_project_src(self, fileName, functions,
                        includes=None, dirpath=None,
                        target=None, condition="y"):
        src_re = re.compile(r"[a-zA-Z0-9-_.]+\.c")
        if not src_re.match(fileName):
            raise HMParamForamtError(fileName)

        if not dirpath:
            parentDir = self.__projectDir
        else:
            parentDir = self.get_dir_from_project(dirpath)
        return self.__add_src(parentDir, fileName, functions,
                              includes, target, condition)

    def del_project_src(self, fileName, dirpath=None):
        src_re = re.compile(r"[a-zA-Z0-9-_.]+\.c")
        if not src_re.match(fileName):
            raise HMParamForamtError(fileName)

        if not dirpath:
            parentDir = self.__projectDir
        else:
            parentDir = self.get_dir_from_project(dirpath)
        return self.__del_src(parentDir, fileName)

    def add_project_header(self, fileName, functions,
                           dirpath=None, needInstall=False,
                           installsubdir=None):
        header_re = re.compile(r"[a-zA-Z0-9-_.]+\.h")
        if not header_re.match(fileName):
            raise HMParamForamtError(fileName)

        if not dirpath:
            parentDir = self.__projectDir
        else:
            parentDir = self.get_dir_from_project(dirpath)
        self.__add_header(parentDir, fileName,
                          functions, needInstall,
                          installsubdir)

    def before_write_to_disk(self):
        return

    def write_to_disk(self, rootpath):
        self.projectpath = os.path.join(rootpath, self.projectName)
        self.__projectDir.write_to_disk(rootpath)

    def add_flag(self, hmbuild, flags, target=None, condition="y",
                 noNeedCondition=False):
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_cflags(flags, target, condition, noNeedCondition)

    def add_include(self, hmbuild, dirpath, target=None, condition="y",
                    noNeedCondition=False):
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_include(dirpath, target, condition, noNeedCondition)

    def add_linklib(self, hmbuild, target, libinfo, condition="y"):
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_linklib(target, libinfo, condition)

    def add_findpkg(self, hmbuild, pkg=""):
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_findpkg(pkg)

    def add_bool_kconfig(self, name, defaultvalue="y", desc="sample"):
        self.Kconfig.add_bool_config(name, defaultvalue, desc)

    def add_inclib(self, hmbuild, libinfo, target=None):
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_inclib(libinfo, target=target)

    def add_installinc(self, hmbuild=None, filename="", installDir=None):
        if hmbuild is None:
            hmbuild = self.rootHMbuild
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_installinc(filename, installDir)

    def add_output(self, hmbuild=None, target="", output=""):
        if hmbuild is None:
            hmbuild = self.rootHMbuild
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")
        hmbuild.add_output(target=target, output=output)

    def add_file(self, fileinstance, dirpath=None):
        if not dirpath:
            parentDir = self.__projectDir
        else:
            parentDir = self.get_dir_from_project(dirpath)
        return parentDir.add_file(fileinstance)

    def add_install_find_module(self, hmbuild=None, filename=""):
        if hmbuild is None:
            hmbuild = self.rootHMbuild
        if not isinstance(hmbuild, HmbuildFile):
            raise HMParamTypeError("hmbuild", "HmbuildFile")

        return hmbuild.add_install_findmodule(filename)
