#!/usr/bin/env bash
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
# Description: Perform Static Codestyle Checking in hm-dsl scripts
# Author: Huawei OS Kernel Lab
# Create: Mon 07 Dec 2020 08:19:00 PM CST
set -e

BASEDIR=$(dirname "$0")
PROJDIR=$(dirname $BASEDIR)
cd $PROJDIR

pycodestyle --ignore=W605,E123,E126,E226,W504,E121,E701 \
    --max-line-length=120 \
    --exclude=$PROJDIR/experiments,$PROJDIR/externals \
    $PROJDIR

pylint $(find $PROJDIR/hmdsl | grep .py$) --disable R,C,W,E --enable W0611,R1719,R1714,C0116,R0914,C0123