#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2024 Huawei Device Co., Ltd.

set -e
if uname -a | grep -iq linux
then
	DEFAULT_PREFIX=/opt
else
	DEFAULT_PREFIX=/usr/local
fi

echo ${prefix:=$DEFAULT_PREFIX/openposix_testsuite}
