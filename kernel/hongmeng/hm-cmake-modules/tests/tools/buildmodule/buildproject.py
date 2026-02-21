#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests Build
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import shutil
from pathlib import Path
from tools.projectmodule.base.hmproject import HMProject
from tools.error.hmerror import HMParamTypeError
import tools.constants as constants


class HmBuildProject(object):
    TMP_OUTPUT = "tmp_output"

    def __init__(self, project, workPath, sysrootPath, insatllCmakeModlulePath,
                 installPath=None, buildDir="build"):
        if not isinstance(project, HMProject):
            raise HMParamTypeError("projectPath", "HMProject")
        workPath = os.path.realpath(workPath)
        sysrootPath = os.path.realpath(sysrootPath)

        self.insatllCmakeModlulePath = os.path.realpath(
            insatllCmakeModlulePath)
        self.project = project
        self.projectPath = os.path.join(workPath, self.project.projectName)
        self.workPath = workPath
        if not installPath:
            self.installPath = os.path.join(workPath, self.TMP_OUTPUT)
            self.isTmpInstall = True
        else:
            self.installPath = os.path.realpath(installPath)

        self.buildPath = os.path.join(workPath, buildDir)
        self.sysrootPath = sysrootPath
        self.cmakeRet = constants.FAIL
        self.makeRet = constants.FAIL
        self.__alreadyCreateProject = False
        self.__dot_config = ""

    def clean_all(self):
        shutil.rmtree(self.buildPath)
        if self.isTmpInstall:
            shutil.rmtree(self.installPath)
        shutil.rmtree(self.projectPath)

    def create_project(self):
        if self.__alreadyCreateProject:
            return

        checkProjectPath = Path(self.projectPath)
        if checkProjectPath.exists():
            shutil.rmtree(self.projectPath)
        self.project.before_write_to_disk()
        self.project.write_to_disk(self.workPath)
        self.__alreadyCreateProject = True

    def set_make_header(self, enable="y"):
        content = "CONFIG_HMBUILD_MAKE_HEADERS=%s\n" % (enable)
        self.append_dot_config(content)

    def set_make_binary(self, enable="y"):
        content = "CONFIG_HMBUILD_MAKE_BINARIES=%s\n" % (enable)
        self.append_dot_config(content)

    def append_dot_config(self, content):
        if not isinstance(content, str):
            raise HMParamTypeError("content", "str")
        self.__dot_config += content

    def write_dot_config(self):
        if self.__dot_config:
            filepath = os.path.join(self.buildPath, ".config")
            with open(filepath, 'w') as f:
                f.write(self.__dot_config)

    def build(self):
        self.create_project()
        checkBuildPath = Path(self.buildPath)
        if checkBuildPath.exists():
            shutil.rmtree(self.buildPath)
        os.makedirs(self.buildPath)

        checkInstallPath = Path(self.installPath)
        if not checkInstallPath.exists():
            os.makedirs(self.installPath)

        self.write_dot_config()
        cmakeCmdStr = ("cd  %s; cmake -DCMAKE_MODULE_PATH=%s -DCMAKE_SYSROOT=%s \
                        -Wno-dev %s" % (self.buildPath,
                                        self.insatllCmakeModlulePath,
                                        self.sysrootPath, self.projectPath))
        self.cmakeRet = os.system(cmakeCmdStr)
        if self.cmakeRet:
            return False

        makedCmdStr = ("cd  %s;DESTDIR=%s make install" %
                       (self.buildPath, self.installPath))
        self.makeRet = os.system(makedCmdStr)
        if self.makeRet:
            return False


if __name__ == "__main__":
    from tools.projectmodule.elfproject import ElfProject
    standLoneElf = ElfProject()
