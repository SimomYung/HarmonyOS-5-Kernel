#!/bin/bash
# Copyright (C) 2023 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e
{
if [ "$(uname)" != "Darwin" ];then
    flock -x 100
fi
if [ -d "$1/libusb-1.0.26" ];then
    rm -rf $1/libusb-1.0.26
fi
tar jxvf $2/libusb-1.0.26.tar.bz2 -C $1
cp $2/fix-init-fail.patch $1/
cd $1/libusb-1.0.26/
patch -p1 < ../fix-init-fail.patch
if [ "$(uname)" != "Darwin" ];then
    flock -u 100
fi
} 100<>$2/lock_file.lock
exit 0
