#!/bin/bash
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation version 2.1
# of the License.
#
# Copyright(c) 2023 Huawei Device Co., Ltd.

set -e
cd $1
if [ -d "libnl-3.7.0" ];then
    rm -rf libnl-3.7.0
fi
tar xvf libnl-3.7.0.tar.gz
cd $1/libnl-3.7.0
./configure
patch -p1 < $1/slove-oh-update-sp3.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/solve-oh-compile-problem.patch --fuzz=0 --no-backup-if-mismatch
patch -p1 < $1/slove-oh-bug-fix.patch --fuzz=0 --no-backup-if-mismatch
exit 0
