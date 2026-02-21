#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

BUILD_DIR="$ROOT_DIR"/output/"$ARCHI"
OUTPUT_DIR="$BUILD_DIR"
MODLUE_OUTPUT_DIR="$ROOT_DIR"/output/"$ARCHI"
CMAKE_BUILD_DIR="$ROOT_DIR"/"cmake_build"
mkdir -p "$BUILD_DIR"

function install_ylong_c_toolchain()
{
    ysd download ylong_c-toolchain
    modify_x86_64_wrapper $X86_64_WRAPPER
    rm -f ylong_c-toolchain*.rpm
}

if [ "$MODULE" == "test" ];then
    #install_ylong_c_toolchain
    source /opt/buildtools/ylongc/hpe/setenv.sh "$ARCHI" hpe > /dev/null
    GCC_VERSION=$("$ARCHI"-linux-gnu-gcc -dumpversion)
    export GCCLIB_PATH="$DLVERSIONSET/$CURRENT_RTOS_PLATFORM/gnu64/lib64/gcc/"$ARCHI"-target-linux-gnu/$GCC_VERSION"
    export STDCXXLIB_PATH="$DLVERSIONSET/$CURRENT_RTOS_PLATFORM/gnu64//"$ARCHI"-target-linux-gnu/lib64"
fi

source /opt/RTOS/setenv.sh "$ARCHI"> /dev/null

function get_commitid() {
    if [ -f "$ROOT_DIR"/.git/HEAD ];then
        GIT_HEAD=$(cat "$ROOT_DIR"/.git/HEAD)
        COMMITID_DIR=${GIT_HEAD#*ref: }
    else
        COMMITID_DIR="unknown"
    fi

    if [ -f "$ROOT_DIR"/.git/"$COMMITID_DIR" ];then
        HLIBC_COMPILE_COMMIT_ID=$(cat "$ROOT_DIR"/.git/"$COMMITID_DIR")
    else
        HLIBC_COMPILE_COMMIT_ID="$COMMITID_DIR"
    fi

    echo "HLIBC_COMPILE_COMMIT_ID=$HLIBC_COMPILE_COMMIT_ID"
    export HLIBC_COMPILE_COMMIT_ID
}

function gen_config() {
    if [ "aarch64" == "$ARCHI" ];then
        CAP_CONFIG_TEMPLATE=$ROOT_DIR/config/compile/defconfig_aarch64
    fi

    if [ "x86_64" == "$ARCHI" ];then
        CAP_CONFIG_TEMPLATE=$ROOT_DIR/config/compile/defconfig_x86_64
    fi

    cpp -undef -P -x assembler-with-cpp -ffreestanding -o "$BUILD_DIR"/.config "$CAP_CONFIG_TEMPLATE"
    config=$(cat "$BUILD_DIR"/.config)
    echo "$config" | awk -F '=' 'BEGIN {i = 1} \
        /^#/ {pos[i++] = $0} \
        !/^#/ {if (!s[$1]) {pos[i] = $0; s[$1] = i++} \
            else {pos[s[$1]] = $0}} END \
        {for (j = 1; j < i; j++) print pos[j]}' \
        > "$BUILD_DIR"/.config

    #因为削减了pub目录，因此此处需要注释掉，但其实这整个函数，应该都不影响hlibc的rpm包构建。
    #"$ROOT_DIR"/build/tool/gen-config-h.sh "$BUILD_DIR"/.config > "$ROOT_DIR"/pub/include/cap_config.h
}

function down_deps()
{
    cd "$ROOT_DIR"/build/

    if [ -d $(pwd)/deps ]; then
        rm -r $(pwd)/deps
    fi

    #下载依赖包
    if [ "$MODULE" == "test" ];then
        DEPS_FILE=hpetest.spec
    elif [ "$CPU_TYPE" == "hi5651" ];then
        DEPS_FILE=libhlibc_ysx.spec
    elif [ "$CPU_TYPE" == "hi1213" ];then
        DEPS_FILE=libhlibc_ysl.spec
    elif [ "$CPU_TYPE" == "qemu" ];then
        DEPS_FILE=libhlibc_qemu.spec
    fi

    pnftool build-dep -a "$ARCHI" -s "$CUR_DIR"/pkg/"$DEPS_FILE" -d true
    source $(pwd)/deps/export_compile_flags.sh || true
}

get_commitid
gen_config
#down_deps

if [ "$MODULE" == "test" ];then
    export PNF_LD_FLAGS="$PNF_LD_FLAGS -L$(pwd)/deps/lib/$ARCHI-linux-gnu/hpe-hlibc-qemu/hpe -L$(pwd)/deps/lib/$ARCHI-linux-gnu/hsecurec/hpe -L$(pwd)/deps/lib/$ARCHI-linux-gnu/hdophi-xdophi-libs/hpe"
    export PNF_CFLAGS="$PNF_CFLAGS -I$(pwd)/deps/include/$ARCHI-linux-gnu/hpe-hlibc/dmod -I$(pwd)/deps/include/$ARCHI-linux-gnu/hpe-hlibc/common -I$(pwd)/deps/lib/$ARCHI-linux-gnu/hdophi-xdophi-libs/hpe"
else
    export PNF_CFLAGS="$PNF_CFLAGS -L$(pwd)/deps/lib/$ARCHI-linux-gnu/hsecurec/hpe -L$(pwd)/deps/lib/$ARCHI-linux-gnu/hdophi-xdophi-libs/hpe"
fi
if [ "$CPU_TYPE" == "hi5651" ];then
     export PNF_CFLAGS="$PNF_CFLAGS -L$(pwd)/deps/lib/$ARCHI-linux-gnu/starry-bsp-pie-sd5151t-hpk/hpe"
fi
#cd ~-
