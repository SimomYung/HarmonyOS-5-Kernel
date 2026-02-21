#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

from tools.filemodule.base.hmfile import HMFile
import tools.constants as constants

__all__ = ["HMCmakeFile"]


class HMCmakeFile(HMFile):
    DEF_CMAKE_CONTENT = "cmake_minimum_required(VERSION 3.16 FATAL_ERROR)\nset(CMAKE_SYSTEM_NAME Hongmeng)\nproject(tests C ASM)\ninclude(HongmengCMakeNG/Kconfig)\ninclude(HongmengCMakeNG/HMbuild)\n"

    DEF_NO_LANG_CMAKE_CONTENT = "cmake_minimum_required(VERSION 3.16 FATAL_ERROR)\nset(CMAKE_SYSTEM_NAME Hongmeng)\nproject(tests LANGUAGES NONE)\ninclude(HongmengCMakeNG/Kconfig)\ninclude(HongmengCMakeNG/HMbuild)\n"

    DEF_ONLY_HMBUILD_CONTENT = "include(HongmengCMakeNG/HMbuild)\n"

    NO_LANG_CMAKE_CONTENT = "cmake_minimum_required(VERSION 3.16 FATAL_ERROR)\nset(CMAKE_SYSTEM_NAME Hongmeng)\nproject(tests LANGUAGES NONE)\ninclude(HongmengCMakeNG/Kconfig)\ninclude(HongmengCMakeNG/HMbuild)\n"


    def __init__(self, content=DEF_CMAKE_CONTENT):
        super().__init__(constants.CMAKE_FILENAME, content)


if __name__ == "__main__":
    import os
    s = HMCmakeFile()
    print("content: \n" + s.content)
    s.write_to_disk(os.path.dirname(__file__))
