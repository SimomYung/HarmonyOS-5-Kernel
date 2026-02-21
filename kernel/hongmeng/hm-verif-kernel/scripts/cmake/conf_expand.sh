#!/usr/bin/env bash
# Config expand file.
# Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.

CONFIG_FILE=$1
CONFIG_CMAKE=$2
AUTOCONF=$3

#cat ${CONFIG_FILE} | awk -F '=' 'BEGIN { print "#ifndef CONFIG_H\n#define CONFIG_H\n"} \
#		NF == 2 { print "#define " $1" "$2 } \
#		    END { print "\n#endif" }' > ${AUTOCONF}
cat ${CONFIG_FILE} | awk -F '=' 'NF == 2 { print "#define " $1" "$2 }' > ${AUTOCONF}

cat ${CONFIG_FILE} | awk -F '=' 'NF == 2 { print "set(" $1" "$2")" }' > ${CONFIG_CMAKE}
