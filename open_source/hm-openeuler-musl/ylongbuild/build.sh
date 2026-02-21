#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

function init_env() {
    CUR_DIR=$(cd "$(dirname ""$0"")";pwd)
    ROOT_DIR="$CUR_DIR"
    OUTPUT_DIR="${ROOT_DIR}/output"
    export HLIBC_FEATURE=""
    export CPU_TYPE="hi5651"
}

function help() {
    echo "       Usage: $0 -a {arch} -c {cpu_type} -r {repo-url} [ -f cov] [-M compile_mode]"
    echo ""

    echo "     example:"
    echo "      (*)    $0 -a aarch64 -C hi5651 -r http://100.95.111.143/repo_hpp_DevelopBranch/hpe2022/hpe2022.repo"
    echo "      (*)    $0 -a aarch64 -C hi5651 -M release -r http://100.95.111.143/repo_hpp_DevelopBranch/hpe2022/hpe2022.repo"
    echo "      (*)    $0 -a aarch64 -C qemu -r http://100.95.111.143/repo_hpp_DevelopBranch/hpe2022/hpe2022.repo"
    echo "      (*)    $0 -a aarch64 -C hi1213 -r http://100.95.111.143/repo_hpp_DevelopBranch/hpe2022/hpe2022.repo"
    echo "      (*)    $0 -a aarch64 -r http://100.95.111.143/repo_hpp_DevelopBranch/hpe2022/hpe2022.repo -m test"
}

function decide_repo() {
    rm -rf /etc/yum.repos.d/*
    [ -f "$1" ] && { echo "Setup local repo file $1"; cp -f "$1" /etc/yum.repos.d/; }
    [ -d "$1" ] && { echo "Setup local repo file path $1"; find "$1" -name *.repo -exec cp -f {} /etc/yum.repos.d/ \;; }
    [ -e "$1" ] || { echo "Setup remote repo file $1"; wget -P /etc/yum.repos.d/ "$1"; }
}

function clean_output() {
    if [ -d "$OUTPUT_DIR" ]; then
        rm -rf $OUTPUT_DIR/*
    fi
    echo "Info: Clean output directory."
}

function parse_arguments() {
    clean_output
    if [ "$1" == "clean" ];then
        exit 0
    fi

    while getopts ":a:r:o:m:f:C:T:M:" opt
    do
        case "$opt" in
            a)
            arch="$OPTARG"
            ;;
            r)
            repo_path="$OPTARG"
            ;;
            o)
            export TOOLCHAIN="$OPTARG"
            ;;
            m)
            module="$OPTARG"
            ;;
            f)
            export HLIBC_FEATURE="$OPTARG"
            ;;
            C)
            export CPU_TYPE="$OPTARG"
            ;;
            T)
            export OS="$OPTARG"
            ;;
            M)
            mode="$OPTARG"
            ;;
            ?)
            echo "Warning: Unknown parameters, Usage: ./build.sh -a [aarch64] -C [cputype:hi5651/hi1382/hi1213/hi1280] -r [repo-url] [-f {feature}] -m [test] -M [debug/release]"
            ;;
        esac
    done
}

function check_arguments() {
    if [[ -z "$arch" ]]; then
        echo "Error: Parameters missing, you should provide at least two parameters -a -r"
        help
        exit 1
    fi

    if [ "$arch" != 'aarch64' ] && [ "$arch" != 'x86_64' ]; then
        echo "Error: Incorrect platform architecture, only [aarch64] | [x86_64] are supported now."
        help
        exit 1
    fi

    if [ "$module" != '' -a "$module" != 'all' -a "$module" != 'patch'  -a "$module" != 'test' ]; then
        echo "Error: Incorrect module, only [all, patch] is supported now."
        help
        exit 1
    fi

    if [ "$mode" == "debug" ];then
        compile_mode="_ylong_debug"
    elif [ "$mode" == "release" ];then
        compile_mode="_ylong_release"
    elif [[ -z "$mode" ]];then
        compile_mode="_ylong_debug"
    else
        echo "Error: Incorrect compile mode, only [release, debug] is supported now."
        help
        exit 1
    fi
}


function main() {
    init_env
    parse_arguments "$@"
    check_arguments
    echo "Building $repo_path in $arch ..."
    
     if [ "$module" == "test" ];then
        decide_repo "$repo_path"
    fi
    
    $ROOT_DIR/build/build_rpm.sh "$arch" "$module" "$compile_mode"
}

main "$@"
