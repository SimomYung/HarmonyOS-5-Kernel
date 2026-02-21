#!/usr/bin/env bash
# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: The shell script to generate unittest coverage info
# Author: Huawei OS Kernel Lab
# Create: Thu Feb 04 13:50:00 2021

set -e

# Description:
# 1. copy this shell to the directory of hmut_xxx.elf
# 2. run this shell,you can get the follow output:
#    1) the coverage report of text format
#    2) the coverage report of html format

# 1. run hmut_dh_test01.elf
qemu-armeb hmut_musl_string.elf

# 2. generate coverage info
lcov --capture --directory ./ --output-file coverage.info

# 3. show coverage info
lcov -l coverage.info

# 4. generate html report
genhtml coverage.info --output-directory lcov_out1

# musl-string coverage info
lcov -c -d musl/string/CMakeFiles/hmut-musl-string.dir/src/ -o output.info
lcov --remove output.info '*.h' '*.template' -o output.info
lcov -l output.info
genhtml output.info --output-directory lcov_out2
