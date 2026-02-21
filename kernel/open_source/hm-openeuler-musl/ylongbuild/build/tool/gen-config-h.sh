#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

echo "#ifndef __CAP_CONFIG_H"
echo "#define __CAP_CONFIG_H"
grep CONFIG_ "$1" |
grep -v '^[ \t]*#' |
sed 's,CONFIG_\(.*\)=y.*$,#undef CONFIG_\1\
#define CONFIG_\1 1,' |
sed 's,CONFIG_\(.*\)=n.*$,#undef CONFIG_\1,' |
sed 's,CONFIG_\(.*\)=\(.*\)$,#undef CONFIG_\1\
#define CONFIG_\1 \2,' |
sed 's,CONFIG_BOARD_AR\(.*\)$,BOARD_AR\1,' |
sed 's,\# CONFIG_\(.*\) is not set$,#undef CONFIG_\1,'
echo "#endif /* __CAP_CONFIG_H */"
