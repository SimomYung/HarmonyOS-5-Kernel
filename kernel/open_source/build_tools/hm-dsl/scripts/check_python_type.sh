#!/usr/bin/env bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
# Description: Perform Static Type Checking in hm-dsl scripts
# Author: Huawei OS Kernel Lab
# Create: Mon 07 Dec 2020 08:19:00 PM CST
set -e

BASEDIR=$(dirname "$0")
PROJDIR=$(dirname $BASEDIR)
cd $PROJDIR

exec mypy ./hmdsl --ignore-missing-imports
