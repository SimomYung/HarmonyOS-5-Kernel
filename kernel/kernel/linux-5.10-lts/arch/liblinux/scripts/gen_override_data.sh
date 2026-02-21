#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only

set -e

NM=$1
obj=$2
all=$(cat $3)

if [ "$obj"x = "obj"x ]
then
    ${NM} --defined-only -A -g -f sysv $all \
                               | grep '.text.override' | cut -d':' -f1 \
                               | uniq | tee override-objs.txt
    sed -i 's/[ \t]*$/:/g' override-objs.txt
else
    ${NM} --defined-only -A -g -f sysv $all \
                               | grep '.text.override' | cut -d'|' -f1 | cut -d':' -f2 \
                               | uniq | tee override-syms.txt
    sed -i 's/^[ \t]*/:/' override-syms.txt
    sed -i 's/[ \t]*$/$/g' override-syms.txt
fi
