#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

# ret
FAIL = -1
CMAKE_SUCCESS = 0
MAKE_SUCCESS = 0

# File Default Name
HMBUILD_FILENAME = "HMbuild"
CMAKE_FILENAME = "CMakeLists.txt"
KCONFIG_FILENAME = "Kconfig"

# exec
EXEC_CMD = "exec"
EXEC_DYNAMIC_TYPE = "dynamic"
EXEC_STATIC_TYPE = "static"
ELF_TYPE = ["dynamic", "static"]
ELF_INSTALL_DIR = "BINDIR"

# subdir
SUBDIR_CMD = "subdir"

# src
SRC_CMD = "src"

# lib
LIB_CMD = "lib"
LIB_DYNAMIC_TYPE = "dynamic"
LIB_STATIC_TYPE = "static"
LIB_TYPE = ["dynamic", "static"]
LIB_INSTALL_DIR = "LIBDIR"

# libversion
LIBVERSION_CMD = "libversion"

# installinc
INSTALLINC_CMD = "installinc"

# install
INSTALL_CMD = "install"

# cflags
CFLAGS_CMD = "cflags"

# include
INCLUDE_CMD = "include"

# linklib
LINKLIB_CMD = "linklib"

# inclib
INCLIB_CMD = "inclib"

# findpkg
FINDPKG_CMD = "findpkg"

# inclib
OUTPUT_CMD = "output"

# installfindmodule
INSTALL_FIND_MODULE_CMD = "installfindmodule"
