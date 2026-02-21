
#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Tests ERROR
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

class HMError(Exception):
    pass


class HMParamError(HMError):
    def __init__(self, param):
        HMError.__init__(self, ('param %s is invalid') % param)


class HMDirNotExitsError(HMError):
    def __init__(self, dirname):
        HMError.__init__(self, ('dir %s is not exits') % dirname)


class HMFileExitsError(HMError):
    def __init__(self, filename):
        HMError.__init__(self, ('file %s is not exits') % filename)


class HMParamTypeError(HMError):
    def __init__(self, paramName, paramtype):
        HMError.__init__(self, (' %s is not type %s ') %
                         (paramName, paramtype))


class HMParamForamtError(HMError):
    def __init__(self, paramName):
        HMError.__init__(self, (' %s foramt is error ') %
                         (paramName))


class HMFunctionNotExitsError(HMError):
    def __init__(self, paramName):
        HMError.__init__(self, (' func %s is not exits ') %
                         (paramName))


if __name__ == "__main__":
    error = HMParamError("test")
    error = HMDirNotExitsError("test")
    error = HMParamTypeError("test", "str")
