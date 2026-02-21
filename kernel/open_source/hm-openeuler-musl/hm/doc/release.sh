#!/bin/bash
# Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Fri Apr 10 10:34:11 2020

set -e

TMP_DIR=$(mktemp -d)
cp api.md $TMP_DIR/

sed -i "s/hm\/doc\/man\//#/g" ${TMP_DIR}/api.md
sed -i "s/\.[1-9]\(\|p\)\.md//g" ${TMP_DIR}/api.md

commit=$(git rev-parse --short HEAD)

pandoc -f markdown_github -t latex --latex-engine=xelatex ${TMP_DIR}/api.md $(find man/ -name "*.md" | sort -n) -o libhmc.commit-${commit}.pdf

[ -n "${TMP_DIR}" ] && rm -rf ${TMP_DIR}
