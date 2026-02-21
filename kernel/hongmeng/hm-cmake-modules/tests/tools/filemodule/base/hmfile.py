#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import os
import re
from pathlib import Path
from tools.error.hmerror import HMParamTypeError
from tools.error.hmerror import HMDirNotExitsError

__all__ = ["HMFile"]


class HMFile(object):

    def __init__(self, filename, content=""):
        if not isinstance(filename, str):
            raise HMParamTypeError("filename", "str")
        if not isinstance(content, str):
            raise HMParamTypeError("content", "str")
        self.filename = filename
        self.content = content

    def set_content(self, content):
        if not isinstance(content, str):
            raise HMParamTypeError("content", "str")
        self.content = content

    def append_content(self, content):
        if not isinstance(content, str):
            raise HMParamTypeError("content", "str")
        self.content += content

    def search_content(self, regexp):
        if not isinstance(regexp, str):
            raise HMParamTypeError("regexp", "str")
        searchre = re.compile(regexp)
        find = searchre.search(self.content)
        if find:
            return find.group()
        return None

    def replace_content(self, regexp, replacestr):
        if not isinstance(regexp, str):
            raise HMParamTypeError("regexp", "str")
        if not isinstance(replacestr, str):
            raise HMParamTypeError("replacestr", "str")

        findstr = self.search_content(regexp)
        if findstr:
            replacere = re.compile(regexp)
            newContent = replacere.sub(replacestr, self.content)
            self.set_content(newContent)

    def write_to_disk(self, path="."):
        if not isinstance(path, str):
            raise HMParamTypeError("path", "str")
        checkPath = Path(path)
        if not checkPath.is_dir():
            raise HMDirNotExitsError(path)
        filepath = os.path.join(path, self.filename)
        with open(filepath, 'w') as f:
            f.write(self.content)
