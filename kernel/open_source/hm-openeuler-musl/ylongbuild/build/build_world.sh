#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

#check arch
function check_arch()
{
    for i in ${CPU_ARCH[@]}; do
        if [ "$i" == "$ARCHI" ];then
            return
        fi
    done

    echo "error: cpu arch '$ARCHI' is not valid!"
    exit 1
}

function copy_target()
{
    OUTFILE="$1"
    echo "===== $OUTFILE ====="
    files=$(ls "$OUTFILE" 2> /dev/null | wc -l)
    if [ "$files" != "0" ]; then
        cp -rfL "$OUTFILE"  "$ROOT_DIR"/output/"$ARCHI"
    fi
}

function set_target()
{
    ARCHI="$1"
} 

function main()
{
    CUR_DIR=$(cd "$(dirname ""$0"")";pwd)
    ROOT_DIR="$CUR_DIR"/..
    CPU_ARCH=$(ls "$ROOT_DIR"/config/compile | grep defconfig | sed "s/defconfig_\(.*\)/\1/g")
    APP_NAME="hlibc"
    MKWHO="none"
    NEED_COMPILE=""
    START_TIME=$(date --date='0 days ago' "+%Y-%m-%d %H:%M:%S")
    source "$ROOT_DIR"/build/build_world_init.sh "$1" "$2"
    COMPILE_MODE=$3
    case "${MKWHO}" in
        bin)
            set_target "$1"
            check_arch
            source "$ROOT_DIR"/build/build_world_env.sh
            source "$ROOT_DIR"/build/build_world_musl.sh "$COMPILE_MODE"
            ;;
        patch)
            set_target "$1"
            check_arch
            source "$ROOT_DIR"/build/build_world_env.sh
            source "$ROOT_DIR"/build/build_world_patch.sh
            ;;
        module)
            set_target "$1"
            check_arch
            source "$ROOT_DIR"/build/build_world_env.sh
            source "$ROOT_DIR"/build/build_world_module.sh
            ;;
        test)
            set_target "$1"
            check_arch
            source "$ROOT_DIR"/build/build_world_env.sh
            source "$ROOT_DIR"/build/build_world_test.sh
            ;;


        *)
            echo "Unsupport make target!"
            exit -1
            ;;
    esac    

    if command -v tree >/dev/null 2>&1; then
        tree "$ROOT_DIR"/output -L 4 --charset ASCII
    fi

    END_TIME=$(date --date='0 days ago' "+%Y-%m-%d %H:%M:%S")
    ESCAPE_TIME=$(($(($(date +%s -d "$END_TIME")-$(date +%s -d "$START_TIME")))))
    echo "Compile time escape:  ${ESCAPE_TIME}s"
    echo "Info: $APP_NAME $MKWHO compiling Success!"
    exit 0
}

main "$1" "$2" "$3"
