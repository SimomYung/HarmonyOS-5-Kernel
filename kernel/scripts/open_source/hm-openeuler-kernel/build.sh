#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.
set -e
LOCAL_DIR=$(dirname $(readlink -f "$0"))
echo "LOCAL_DIR: ${LOCAL_DIR}"
if [ -e "$TOPDIR/set_env.sh" ]; then
	source $TOPDIR/set_env.sh
	export CLANG_PREBUILTS_PATH="${PROJ_PATH}"/prebuilts/clang/ohos/linux-x86_64/llvm/
else
	export PROJ_PATH=${TOPDIR}
	export CLANG_PREBUILTS_PATH="${TOPDIR}"/output/tools/
	
	export PRODUCT="$(grep CONFIG_VENDOR_CHIP_PREFIX -rn ${TOPDIR}/scripts/open_source/hm-openeuler-kernel/config)"
	HW_PRODUCT=${PRODUCT#*=}
	export HW_PRODUCT=${HW_PRODUCT//\"/}

	CONFIG_VENDOR_CHIP_SUFFIX="$(grep CONFIG_VENDOR_CHIP_SUFFIX -rn ${TOPDIR}/scripts/open_source/hm-openeuler-kernel/config)"
	CHIP_TYPE=${CONFIG_VENDOR_CHIP_SUFFIX#*=}
	export chip_type=${CHIP_TYPE//\"/}

	sed -i "s|CLANG_PREBUILTS_PATH ?=.*|CLANG_PREBUILTS_PATH ?= ${TOPDIR}/output/tools/|g" ${TOPDIR}/open_source/hm-openeuler-kernel/Makefile
fi
source $TOPDIR/scripts/common.sh
if [ -e "$TOPDIR/open_source/hm-openeuler-kernel/build" ];then
    rm -rf $TOPDIR/open_source/hm-openeuler-kernel/build
fi
mkdir -p $TOPDIR/open_source/hm-openeuler-kernel/build
cd $TOPDIR/open_source/hm-openeuler-kernel/build
export LD_LIBRARY_PATH="${CLANG_PREBUILTS_PATH}"/bin
PLAT=${HW_PRODUCT}
export PLAT=${PLAT}
export USE_HM_KERNEL=true

if [ ! -d "$TOPDIR/open_source/hm-openeuler-kernel/build/include" ]; then
    mkdir -p $TOPDIR/open_source/hm-openeuler-kernel/build/include/
fi
unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS MACHINE
cp "${TOPDIR}"/scripts/open_source/hm-openeuler-kernel/config "${TOPDIR}"/open_source/hm-openeuler-kernel/build/.config
make -C $TOPDIR/open_source/hm-openeuler-kernel  O=$TOPDIR/open_source/hm-openeuler-kernel/build ARCH=arm64 olddefconfig USE_HM_KERNEL=true
make -j 64  pac_path="${CLANG_PREBUILTS_PATH}"/plugins/PAC -C $TOPDIR/open_source/hm-openeuler-kernel O=$TOPDIR/open_source/hm-openeuler-kernel/build ARCH=arm64 \
        LLVM=1 LLVM_IAS=1 \
        V=1 CCACHE=ccache \
        TARGET_BOARD_PLATFORM=$PLAT \
        OBB_PRODUCT_NAME=$PLAT \
        KBUILD_LIBLINUX_FILTER_OBJS=1 \
        KBUILD_MODPOST_WARN=1 \
        CONFIG_KCOV_ENABLE= \
        CROSS_COMPILE=aarch64-linux-ohos- \
        HMSDKSYSROOTPATH=$TOPDIR/output/aarch64 LOCALVERSION="" \
        LIBLINUX_FILTER_OBJS=gen_liblinux_objs.txt \
        LIBLINUX_EXTRA_FILTER_OBJS= \
        chip_type=$chip_type \
        modem_build_with_log=false \
        TARGET_BUILD_VARIANT=user \
        cust_config=cust_modem_user \
	CC="ccache ${LD_LIBRARY_PATH}/clang  -mlittle-endian -Wa,-march=armv8.4-a  --target=aarch64-linux-ohos     -Wno-unused-command-line-argument     --ld-path=${LD_LIBRARY_PATH}/ld.lld           -L$TOPDIR/output/aarch64/lib     -Dhongmeng=1     -D__hongmeng=1     -D__hongmeng__=1     -Ulinux     -U__linux     -U__linux__     -U__gnu_linux__     -fno-emulated-tls     -fno-builtin-bcmp     -Wl,--apply-dynamic-relocs     -Wl,--dynamic-linker=/lib/hmld.so.elf          -mno-outline-atomics          -std=gnu99      --sysroot=$TOPDIR/output/aarch64" LD="${LD_LIBRARY_PATH}/ld.lld  --sysroot=$TOPDIR/output/aarch64 " NM="${LD_LIBRARY_PATH}/llvm-nm" HMSDKSYSROOTPATH="$TOPDIR/output/aarch64" \
        vmlinux modules dynlibs
unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS MACHINE
 # First install the modules
if (grep -q -i -e '^CONFIG_MODULES=y$' .config); then
    make -j 32 ARCH=arm64 DEPMOD=echo MODLIB=$TOPDIR/output/aarch64/lib/modules/linux-5.10.97-oh modules_install
    rm "$TOPDIR/output/aarch64/lib/modules/linux-5.10.97-oh/build"
    rm "$TOPDIR/output/aarch64/lib/modules/linux-5.10.97-oh/source"
    # If the kernel/ directory is empty remove it to prevent QA issues
    rmdir --ignore-fail-on-non-empty "$TOPDIR/output/aarch64/lib/modules/linux-5.10.97-oh/kernel"
else
    echo "no modules to install"
fi

# Then install liblinux.a
install -m 0755 -d $TOPDIR/output/aarch64/lib/modules/linux-5.10.97-oh
install -m 0755 -d $TOPDIR/output/aarch64/boot/linux
install -m 0644 .config $TOPDIR/output/aarch64/boot/linux/config-linux-5.10.97-oh
[ -e Module.symvers ] && install -m 0644 Module.symvers $TOPDIR/output/aarch64/boot/linux/Module.symvers-linux-5.10.97-oh

# install ldk backend library if exists
[ -e samples/ldk/libdh-linux.so.5.10.97-oh ] && install -m 0644 samples/ldk/libdh-linux.so.5.10.97-oh $TOPDIR/output/aarch64/lib
