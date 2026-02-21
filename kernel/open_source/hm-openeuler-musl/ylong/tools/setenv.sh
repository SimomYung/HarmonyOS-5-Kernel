#!/bin/bash
# sdk toochains init scripts
# Copyright Huawei Technologies Co., Ltd. 2010-2019. All rights reserved.

if [[ "$0" = "bash" ]] || [[ "$0" = "-su" ]]; then
        cd "$(dirname "$BASH_SOURCE")"
        CUR_FILE=$(pwd)/$(basename "$BASH_SOURCE")
        ORIGIN_DIR=$(dirname "$CUR_FILE")
        cd - > /dev/null
else
        CUR_FILE="$BASH_SOURCE"
        ORIGIN_DIR=$(dirname "$CUR_FILE")
fi

ORIGIN_DIR="/opt/pdt-toolchains/"
export HW_SEC_CFLAGS=" -fstack-protector-strong -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack "
export HW_SEC_LDFLAGS=" -z relro -z now -z noexecstack "



extra_cflag_cmd_arm='[[ x$UNSET_HW_SEC_FLAGS == "x1" ]] ||  DL_TARGET_CFLAGS=" $DL_TARGET_CFLAGS $HW_SEC_CFLAGS "'
extra_ldflag_cmd_arm='[[ x$UNSET_HW_SEC_FLAGS == "x1" ]] ||  DL_TARGET_LD_FLAGS=" $DL_TARGET_LD_FLAGS $HW_SEC_LDFLAGS "'

extra_cflag_cmd_x86='[[ x$UNSET_HW_SEC_FLAGS == "x1" ]] ||  RTOS_TARGET_CFLAGS=" $RTOS_TARGET_CFLAGS $HW_SEC_CFLAGS "'
extra_ldflag_cmd_x86='[[ x$UNSET_HW_SEC_FLAGS == "x1" ]] ||  RTOS_TARGET_LD_FLAGS=" $RTOS_TARGET_LD_FLAGS $HW_SEC_LDFLAGS "'

RTOS_RELASE_VERSION=`cat /opt/RTOS/RTOS_VERSION.log`

AARCH_WRAPPER=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/arm64le_5.10_ek_preempt_pro/bin/aarch64-linux-gnu-wrapper.sh
ARMEL_WRAPPER=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/arm64le_5.10_ek_preempt_pro/bin/arm-linux-gnueabi-wrapper.sh
ARM32_WRAPPER=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/arm32A15le_5.10_ek_preempt_pro/bin/arm32-linux-gnueabi-wrapper.sh
X86_64_WRAPPER=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/x86_64_5.10_ek_pro/bin/x86_64-rtos-linux-gnu-wrapper.sh
X86_32_WRAPPER=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/x86_64_5.10_ek_pro/bin/x86_32-rtos-linux-gnu-wrapper.sh

function usage(){
    echo -e "usage :\n  source ${ORIGIN_DIR}/setenv.sh [aarch64|x86_64]"
    echo "examples:"
    echo "  source ${ORIGIN_DIR}/setenv.sh arm32   ---> enable arm32";set +e
    echo "  source ${ORIGIN_DIR}/setenv.sh aarch64 ---> enable aarch64";set +e
    echo "  source ${ORIGIN_DIR}/setenv.sh x86_64  ---> enable x86_64";set +e
}

function add_extra_cmd(){
    match_line=$(sed -n '/DL_TARGET_LD_FLAGS/=' $1 | head -n1 )
    if [ "x$match_line" == "x" ];then
        match_line=$(sed -n '/RTOS_TARGET_LD_FLAGS/=' $1 | head -n1 )
        if [ "x$match_line" == "x" ];then
            echo "[Error] cannot add hw sec flags!"
            exit -1
        fi
    fi
    if [ "x$2" == "xx86_64" ];then
        sed -i "${match_line} a$extra_cflag_cmd_x86" $1
        sed -i "${match_line} a$extra_ldflag_cmd_x86" $1
    else
        sed -i "${match_line} a$extra_cflag_cmd_arm" $1
        sed -i "${match_line} a$extra_ldflag_cmd_arm" $1
    fi

}

function modify_arm64_wrapper(){
    if grep -q PNF_CFLAGS "$1"; then
        echo "arm64 wrapper already edited"
    else
        cp $1 $1-bak
    sed -i "s/DL_TARGET_AS_FLAGS=''/DL_TARGET_AS_FLAGS=\" \$PNF_AS_FLAGS \"/" $1
    sed -i "s/DL_TARGET_CFLAGS=''/DL_TARGET_CFLAGS=\" \$PNF_CFLAGS \"/" $1
    sed -i "s/DL_TARGET_LD_FLAGS=''/DL_TARGET_LD_FLAGS=\" \$PNF_LD_FLAGS \"/" $1
    add_extra_cmd $1
    fi
}

function modify_armel_wrapper(){
    if grep -q PNF_CFLAGS "$1"; then
        echo "armel wrapper already edited"
    else
        cp $1 $1-bak
    sed -i "s/DL_TARGET_AS_FLAGS=''/DL_TARGET_AS_FLAGS=\" \$PNF_AS_FLAGS \"/" $1
    sed -i "s/DL_TARGET_CFLAGS=''/DL_TARGET_CFLAGS=\" \$PNF_CFLAGS \"/" $1
    sed -i "s/DL_TARGET_LD_FLAGS=''/DL_TARGET_LD_FLAGS=\" \$PNF_LD_FLAGS \"/" $1
    add_extra_cmd $1
    fi
}

function modify_arm32_wrapper(){
    if grep -q PNF_CFLAGS "$1"; then
        echo "arm32 wrapper already edited"
    else
        cp $1 $1-bak
    sed -i "s/DL_TARGET_AS_FLAGS=''/DL_TARGET_AS_FLAGS=\" \$PNF_AS_FLAGS \"/" $1
    sed -i "s/DL_TARGET_CFLAGS=''/DL_TARGET_CFLAGS=\" \$PNF_CFLAGS \"/" $1
    sed -i "s/DL_TARGET_LD_FLAGS=''/DL_TARGET_LD_FLAGS=\" \$PNF_LD_FLAGS \"/" $1
    add_extra_cmd $1
    fi
}


function modify_x86_32_wrapper(){
    if grep -q PNF_CFLAGS "$1"; then
        echo "x86 wrapper already edited"
    else
        cp $1 $1-bak
    sed -i "s/RTOS_TARGET_AS_FLAGS='--32'/RTOS_TARGET_AS_FLAGS=\" --32 \$PNF_AS_FLAGS \"/" $1
    sed -i "s/RTOS_TARGET_CFLAGS='-m32 '/RTOS_TARGET_CFLAGS=\" -m32 \$PNF_CFLAGS \"/" $1
    sed -i "s/RTOS_TARGET_LD_FLAGS=' -m elf_i386 '/RTOS_TARGET_LD_FLAGS=\" -m elf_i386  \$PNF_LD_FLAGS \"/" $1
    add_extra_cmd $1
    fi
}

function modify_x86_64_wrapper(){
    if grep -q PNF_CFLAGS "$1"; then
        echo "x86 wrapper already edited"
    else
        cp $1 $1-bak
    sed -i "s/RTOS_TARGET_AS_FLAGS='--64'/RTOS_TARGET_AS_FLAGS=\" --64 \$PNF_AS_FLAGS \"/" $1
    sed -i "s/RTOS_TARGET_CFLAGS=' -m64'/RTOS_TARGET_CFLAGS=\" -m64 \$PNF_CFLAGS \"/" $1
    sed -i "s/RTOS_TARGET_LD_FLAGS='-m elf_x86_64'/RTOS_TARGET_LD_FLAGS=\" -m elf_x86_64 \$PNF_LD_FLAGS \"/" $1
    arrch="x86_64"
    add_extra_cmd $1 $arrch
    fi
}

function tips(){
    echo "======================================================================================="
    echo "HW_SEC_CFLAGS  = $HW_SEC_CFLAGS"
    echo "HW_SEC_LDFLAGS = $HW_SEC_LDFLAGS"
    echo -e "\n"
    echo "Default HW_SEC_CFLAGS HW_SEC_LDFLAGS enabled !"
    echo "Use UNSET_HW_SEC_FLAGS=1 to disable in case of compiling kernel modules."
    echo "Example:"
    echo "UNSET_HW_SEC_FLAGS=1 make -j\$NJOB KERNELDIR=\$KERNEL_DIR_PATH"
    echo "======================================================================================="
}


case $1 in
    x86_64)
            modify_x86_64_wrapper $X86_64_WRAPPER
            source ${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/dlsetenv.sh -p x86_64_5.10_ek_pro --sdk-path=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/x86_64_5.10_ek_pro/hcc_x86/
            ;;
    aarch64)
            modify_arm64_wrapper $AARCH_WRAPPER
            modify_armel_wrapper $ARMEL_WRAPPER
            source ${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/dlsetenv.sh -p arm64le_5.10_ek_preempt_pro --sdk-path=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/arm64le_5.10_ek_preempt_pro/hcc_arm64le/,${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/arm64le_5.10_ek_preempt_pro/hcc_arm32le/
            export DL_SDK_STD_TOOLCHAIN=aarch64-linux-gnu
            export DL_SDK_APP_TOOLCHAIN=aarch64-linux-gnu
            export CROSS_COMPILE=aarch64-linux-gnu-
            export PATH=$PATH:/opt/buildtools/ylongc/hpe/aarch64/bin

            ;;
    arm32)
            modify_arm32_wrapper $ARM32_WRAPPER
            source ${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/dlsetenv.sh -p arm32A15le_5.10_ek_preempt_pro --sdk-path=${ORIGIN_DIR}/${RTOS_RELASE_VERSION}/arm32A15le_5.10_ek_preempt_pro/hcc_arm32le/
            ;;
        *)
            usage
esac

tips
