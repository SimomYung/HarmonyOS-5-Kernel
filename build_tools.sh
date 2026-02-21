#!/bin/bash
# build_tools.sh
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e
export TOPDIR=$(cd $(dirname $0); pwd)

CODE_ROOT=${PROJ_PATH}
if [ -z "${CODE_ROOT}" ];then
	bash copy_src.sh $TOPDIR
	CODE_ROOT=${TOPDIR}   # ´úÂë¸ùÂ·¾¶
fi
echo "CODE_ROOT: ${CODE_ROOT}"
# clean workspace
rm -rf output
rm -rf "${TOPDIR}"/open_source/hm-openeuler-musl/build

# prepare
mkdir -p output/tools


if [ ! -z "${PROJ_PATH}" ];then
    if [ -d "${CODE_ROOT}/prebuilts/clang/ohos/linux-x86_64/llvm" ];then
        ln -s "${CODE_ROOT}"/prebuilts/clang/ohos/linux-x86_64/llvm/* "${TOPDIR}"/output/tools/
        TARGET_SYS="aarch64-euler-elf"
        export TOOLCHAIN_CC='clang'
    else
        echo "Please check prebuilts/clang/ohos/linux-x86_64/llvm"
        exit 1
    fi
else
    
    if [ -e "${CODE_ROOT}/commandline-tools-linux-x64-5.0.3.906.zip" ];then
	unzip commandline-tools-linux-x64-5.0.3.906.zip
	cp -r ${CODE_ROOT}/command-line-tools/sdk/default/hms/native/BiSheng/* "${TOPDIR}"/output/tools/ 
        TARGET_SYS="aarch64-euler-elf"
        export TOOLCHAIN_CC='clang'
    else
        echo "Please release commandline-tools-linux-x64-5.0.3.906.zip to $TOPDIR"
        exit 1
    fi
fi

source environment-setup-aarch64-euler-elf

mkdir -p "${TOPDIR}"/output/aarch64/usr

build_compilerlibs()
{
    if [ "${TOOLCHAIN_CC}" = "clang" ]; then
        mkdir -p "${TOPDIR}"/output/aarch64/lib/aarch64-linux-ohos
        mkdir -p "${TOPDIR}"/output/aarch64/usr/lib/aarch64-linux-ohos

        cp "${TOPDIR}"/output/tools/lib/clang/15.0.4/lib/aarch64-linux-ohos/* "${TOPDIR}"/output/aarch64/usr/lib/
        cp "${TOPDIR}"/output/tools/lib/aarch64-linux-ohos/libunwind.a "${TOPDIR}"/output/aarch64/usr/lib/

        cp "${TOPDIR}"/output/tools/lib/clang/15.0.4/lib/aarch64-linux-ohos/* "${TOPDIR}"/output/aarch64/lib/
        cp "${TOPDIR}"/output/tools/lib/aarch64-linux-ohos/libunwind.a "${TOPDIR}"/output/aarch64/lib/

    else
        mkdir -p "${TOPDIR}"/output/aarch64/lib
        cp -r "${TOPDIR}"/output/tools/lib/gcc/aarch64-hongmeng-musl "${TOPDIR}"/output/aarch64/usr/lib/
        cp "${TOPDIR}"/output/tools/aarch64-hongmeng-musl/lib64/libstdc++.so* "${TOPDIR}"/output/aarch64/usr/lib/
        cp "${TOPDIR}"/output/tools/aarch64-hongmeng-musl/lib64/libgcc_s.so* "${TOPDIR}"/output/aarch64/lib/
    fi
}

build_musl_headers()
{
    echo "build musl headers begin"
    mkdir "${TOPDIR}"/open_source/hm-openeuler-musl/build
    cd "${TOPDIR}"/open_source/hm-openeuler-musl/build
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}":$(${TOOLCHAIN_CC} -print-file-name=plugin)

    if [ "${TOOLCHAIN_CC}" = "clang" ]; then
        TARGET_MUSLHEADER_CC="clang  -mlittle-endian  --sysroot=${TOPDIR}/output/tools"
    else
        TARGET_MUSLHEADER_CC="aarch64-hongmeng-musl-gcc  -mlittle-endian --sysroot=${TOPDIR}/output/aarch64"
    fi

    chmod a+x "${TOPDIR}"/open_source/hm-openeuler-musl/hm/configure
    "${TOPDIR}"/open_source/hm-openeuler-musl/hm/configure CROSS_COMPILE="${TARGET_SYS}"- CC="${TARGET_MUSLHEADER_CC}" --target=aarch64 --enable-shadow-stack=no
    make install-headers OUTPUT="${TOPDIR}"/output/aarch64 ARCH=aarch64 includedir=/usr/include
    rm -rf "${TOPDIR}"/output/aarch64/obj
    mkdir "${TOPDIR}"/output/aarch64/usr/include/muslorigin
    for header in dirent.h fcntl.h features.h limits.h netinet/tcp.h netinet/udp.h pthread.h signal.h \
        stdlib.h string.h sys/param.h bits/alltypes.h
    do
            case "$header" in
                    *"/"*)
                            if [ ! -d "${TOPDIR}/output/aarch64/usr/include/muslorigin/${header%/*}" ]; then
                                    mkdir "${TOPDIR}"/output/aarch64/usr/include/muslorigin/"${header%/*}"
                            fi;;
                    *);;
            esac
            mv "${TOPDIR}"/output/aarch64/usr/include/"${header}" "${TOPDIR}"/output/aarch64/usr/include/muslorigin/"${header}"
    done

    echo "build musl headers done"
    if [ -e "${TOPDIR}/open_source/hm-openeuler-musl/build" ];then
        rm -rf "${TOPDIR}"/open_source/hm-openeuler-musl/build
    fi
}

build_musl()
{
    echo "build musl begin"

    mkdir "${TOPDIR}"/open_source/hm-openeuler-musl/build
    cd "${TOPDIR}"/open_source/hm-openeuler-musl/build
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}":$(${TOOLCHAIN_CC} -print-file-name=plugin)

    if [ "${TOOLCHAIN_CC}" = "clang" ]; then
        TARGET_MUSL_CC=" clang  -mlittle-endian --target=aarch64-linux-ohos \
            -Wno-unused-command-line-argument --ld-path=${TOPDIR}/output/tools/bin/ld.lld \
            -isystem ${TOPDIR}/output/aarch64/usr/include -L${TOPDIR}/output/tools/lib \
            -Dhongmeng=1 -D__hongmeng=1 -D__hongmeng__=1 -Ulinux -U__linux \
            -U__linux__ -U__gnu_linux__ -fno-emulated-tls -fno-builtin-bcmp \
            -Wl,--apply-dynamic-relocs -Wl,--dynamic-linker=/lib/hmld.so.elf \
            -mno-outline-atomics  -Wno-unsupported-floating-point-opt  -Wno-ignored-attributes \
            -Wno-ignored-pragmas -Wno-string-plus-int -Wno-typedef-redefinition \
            -Wno-unsupported-floating-point-opt  --sysroot=${TOPDIR}/output/tools   "
    else
        TARGET_MUSL_CC=" aarch64-hongmeng-musl-gcc  -mlittle-endian --sysroot=${TOPDIR}/output/aarch64 "
    fi

    chmod a+x "${TOPDIR}"/open_source/hm-openeuler-musl/hm/configure
    "${TOPDIR}"/open_source/hm-openeuler-musl/hm/configure CROSS_COMPILE="${TARGET_SYS}"- CC="${TARGET_MUSL_CC}" --target=aarch64 --disable-heapmgr --strip-opt="--strip-unneeded" --enable-fp-backtrace --debug-build=1
    make -j 32    SUBARCH="" EXTLIB_OUTPUT="${TOPDIR}"/open_source/hm-openeuler-musl/build \
    _HMSDKINCLUDE="${TOPDIR}"/output/aarch64/usr/include _HMSDKLIB="${TOPDIR}"/output/aarch64/usr/lib \
    _HMSDKSYSROOTPATH="${TOPDIR}"/output/aarch64 _HMNATIVESDKSYSROOTPATH="${TOPDIR}"/output/tools \
    LIBC_UBSAN_FLAGS="" LIBC_ENABLE_KASAN=n KASAN_CFLAGS="" V=1  "$@"

    make install DESTDIR="${TOPDIR}"/output/aarch64 LIBDIR="${TOPDIR}"/output/aarch64/lib \
    DOCDIR="${TOPDIR}"/output/aarch64/usr/share/doc
    rm -rvf "${TOPDIR}"/output/aarch64/usr/share/locale

    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libcrypt.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libdl.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libm.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libpthread.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/librt.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libutil.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libxnet.so
    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libresolv.so

    ln -sv libc.so "${TOPDIR}"/output/aarch64/lib/libc.so.6

    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/libcrypt.so.1
    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/libdl.so.2
    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/libm.so.6
    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/libpthread.so.0
    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/libresolv.so.2
    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/librt.so.1
    ln -sv libc.so.6 "${TOPDIR}"/output/aarch64/lib/libutil.so.1
    mkdir -p "${TOPDIR}"/output/aarch64/usr/lib
    mv "${TOPDIR}"/output/aarch64/lib/*.o "${TOPDIR}"/output/aarch64/usr/lib
    rm -rf "${TOPDIR}"/output/aarch64/lib/libc-sys.so
    ln -s libc.so "${TOPDIR}"/output/aarch64/lib/libc-sys.so
    echo "build musl done"
    rm -rf "${TOPDIR}"/open_source/hm-openeuler-musl/build
}

build_kbuild-standalone()
{
    rm -rf "${TOPDIR}"/kbuild-standalone_build
    mkdir "${TOPDIR}"/kbuild-standalone_build
    cd "${TOPDIR}"/kbuild-standalone_build

    make -C "${TOPDIR}"/open_source/build_tools/kbuild-standalone/ -f Makefile.sample O=$(pwd) -j
    mkdir -p "${TOPDIR}"/output/tools/usr/bin
    cp ./kconfig/conf "${TOPDIR}"/output/tools/usr/bin/kbuild-conf
    cp ./kconfig/mconf "${TOPDIR}"/output/tools/usr/bin/kbuild-mconf

    rm -rf "${TOPDIR}"/kbuild-standalone_build
}

build_cmakeng()
{
#cross
   cp -rf "${TOPDIR}"/scripts/self_source/hm-cmake-modules/cross "${TOPDIR}"/self_source/hm-cmake-modules
   cp -rf "${TOPDIR}"/scripts/self_source/hm-cmake-modules/Hongmeng-GNU-C.cmake "${TOPDIR}"/self_source/hm-cmake-modules/src/Platform/
   rm -rf "${TOPDIR}"/self_source/hm-cmake-modules/cross/build
   mkdir "${TOPDIR}"/self_source/hm-cmake-modules/cross/build
   cd "${TOPDIR}"/self_source/hm-cmake-modules/cross/build

   cmake -DTARGET_ARCH=aarch64  -DTARGET_PREFIX="${TARGET_SYS}"- \
          -DCMAKE_INSTALL_PREFIX:PATH="${TOPDIR}"/output/aarch64/usr \
          -DHMCMAKENG_MODULE_DIR:PATH="${TOPDIR}"/output/aarch64/usr/share/cmake/Modules \
          -DHMTOOLCHAIN_CLANG_BINDIR:PATH=/usr/local/bin/ \
          -DHMTOOLCHAIN_TARGET_SYS:STRING=aarch64-euler-elf \
          "${TOPDIR}"/self_source/hm-cmake-modules/cross
   cmake --install "${TOPDIR}"/self_source/hm-cmake-modules/cross/build
   rm -rf "${TOPDIR}"/self_source/hm-cmake-modules/cross/build

# native
   rm -rf "${TOPDIR}"/self_source/hm-cmake-modules/build
   mkdir "${TOPDIR}"/self_source/hm-cmake-modules/build
   cd "${TOPDIR}"/self_source/hm-cmake-modules/build

   cmake -DCMAKE_C_COMPILER_WORKS=TRUE \
         -DCMAKE_CXX_COMPILER_WORKS=TRUE \
         -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
         -DCMAKE_INSTALL_PREFIX:PATH="${TOPDIR}"/output/aarch64/usr \
         -DMODULE_INSTALL_DIR="${TOPDIR}"/output/aarch64/usr/share/cmake/Modules "${TOPDIR}"/self_source/hm-cmake-modules
   cmake --install "${TOPDIR}"/self_source/hm-cmake-modules/build

   rm -rf "${TOPDIR}"/self_source/hm-cmake-modules/build
}

build_hmsd()
{
    cp -rf "${TOPDIR}"/open_source/build_tools/hm-dsl "${TOPDIR}"/output/tools/usr/bin/
    cp -rf "${TOPDIR}"/open_source/build_tools/hmsd "${TOPDIR}"/output/tools/usr/bin/
    cp -rf "${TOPDIR}"/open_source/build_tools/hmsd/hmsdtool "${TOPDIR}"/output/tools/usr/bin/
    mkdir -p "${TOPDIR}"/output/tools/usr/lib
    cp -rf "${TOPDIR}"/open_source/build_tools/python3 "${TOPDIR}"/output/tools/usr/lib/
}

build_musl_headers
build_musl
build_kbuild-standalone
build_cmakeng
build_compilerlibs
build_hmsd

# vim:ts=4:sw=4:expandtab
