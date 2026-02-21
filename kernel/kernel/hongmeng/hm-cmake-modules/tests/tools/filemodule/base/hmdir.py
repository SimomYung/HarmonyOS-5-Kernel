#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools for create elf HMbuild project
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
from pathlib import Path
from tools.filemodule.base.hmfile import HMFile
from tools.error.hmerror import HMParamTypeError
from tools.error.hmerror import HMDirNotExitsError

__all__ = ["HMDir"]


class HMDir(object):

    def __init__(self, dirname):
        self.name = dirname
        self.dirs = {}
        self.files = {}

    def add_dir(self, subdir):
        if not isinstance(subdir, HMDir):
            raise HMParamTypeError("subdir", "HMDir")
        self.dirs[subdir.name] = subdir

    def remove_dir(self, subdirname):
        if not isinstance(subdirname, str):
            raise HMParamTypeError("subdirname", "HMDir")
        del self.dirs[subdirname]

    def get_dir(self, subdirname):
        if not isinstance(subdirname, str):
            raise HMParamTypeError("subdirname", "str")
        if subdirname not in self.dirs:
            return None
        return self.dirs[subdirname]

    def add_file(self, subfile):
        if not isinstance(subfile, HMFile):
            raise HMParamTypeError("subfile", "HMFile")
        self.files[subfile.filename] = subfile

    def remove_file(self, filename):
        if not isinstance(filename, str):
            raise HMParamTypeError("filename", "HMDir")
        del self.files[filename]

    def get_file(self, filename):
        if not isinstance(filename, str):
            raise HMParamTypeError("filename", "str")
        if filename not in self.files:
            return None
        return self.files[filename]

    def write_to_disk(self, rootpath="."):
        if not isinstance(rootpath, str):
            raise HMParamTypeError("rootpath", "str")
        rootpath = os.path.realpath(rootpath)
        # this will create all dirs and files
        check = Path(rootpath)
        if not check.is_dir():
            raise HMDirNotExitsError(rootpath)

        dirpath = os.path.join(rootpath, self.name)
        os.makedirs(dirpath)

        for fkey in self.files:
            self.files[fkey].write_to_disk(dirpath)

        for subkey in self.dirs:
            self.dirs[subkey].write_to_disk(dirpath)
