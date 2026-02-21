#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
from tools.projectmodule.base.hmproject import HMProject
from tools.filemodule.cmakefile import HMCmakeFile


class NoLangProject(HMProject):

    def __init__(self):
        super().__init__()
        self.rootCmake.set_content(HMCmakeFile.NO_LANG_CMAKE_CONTENT)
        self.__alrelady_proccess_file = False

    def __proccess_project_hmbuild(self):
        return

    def before_write_to_disk(self):
        # only want to deal with it once.
        if self.__alrelady_proccess_file:
            return
        self.__proccess_project_hmbuild()
        self.__alrelady_proccess_file = True


if __name__ == '__main__':
    standLoneElf = NoLangProject()
    standLoneElf.before_write_to_disk()
    standLoneElf.write_to_disk(os.path.dirname(__file__))
