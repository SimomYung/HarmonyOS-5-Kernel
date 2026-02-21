#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

echo " ==============build world init ===================="
#function make_who
case "$1" in       
    all)
        MKWHO="all"
        ;;                 
    x86_64|aarch64)
        case "$2" in
            bin)
                MKWHO="bin"
                ;;
            patch)
                MKWHO="patch"
                ;;
            module)
                MKWHO="module"
                ;;
            test)
                MKWHO="test"
                ;;
            *)
                exit -1
                ;;
        esac
        ;;
    *)
        exit -1
        ;;
esac
