#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Test tools
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

from tools.filemodule.base.hmfile import HMFile
import tools.constants as constants
from tools.error.hmerror import HMParamTypeError

__all__ = ["HMKconfigFile"]


class HMKconfigFile(HMFile):
    DEF_KCONFIG_CONTENT = "source \"$(HMCMAKENG_KCONFIG)\"\n"

    def __init__(self):
        super().__init__(constants.KCONFIG_FILENAME, self.DEF_KCONFIG_CONTENT)

    def add_bool_config(self, name, defaultvalue="y", desc="sample"):
        if not isinstance(name, str):
            raise HMParamTypeError("name", "str")
        if not isinstance(defaultvalue, str):
            raise HMParamTypeError("defaultvalue", "str")
        if not isinstance(desc, str):
            raise HMParamTypeError("desc", "str")

        appendcontent = ("config %s\n") % (name)
        self.append_content(appendcontent)
        appendcontent = ("  bool \"%s\"\n") % (desc)
        self.append_content(appendcontent)
        appendcontent = ("  default \"%s\"\n") % (defaultvalue)
        self.append_content(appendcontent)


if __name__ == "__main__":
    import os
    kconfig = HMKconfigFile()
    kconfig.write_file_content(os.path.dirname(__file__))
