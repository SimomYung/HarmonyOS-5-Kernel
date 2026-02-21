#!/usr/bin/env bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
# Description: Run hm-dsl unittests
# Author: Huawei OS Kernel Lab
# Create: Mon 07 Dec 2020 08:19:00 PM CST
set -e

BASEDIR=$(dirname "$0")
PROJDIR=$(dirname $BASEDIR)
cd $PROJDIR

PYTHONPATH=$(pwd) python3 -m unittest discover -s tests -f
