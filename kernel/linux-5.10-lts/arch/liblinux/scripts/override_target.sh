#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-only

set -e

nm=$1
all=$(cat $2)

$nm --defined-only -A ${all} | sed 's/:.* /:/g' | grep -f override-syms.txt | grep -v -f override-objs.txt
