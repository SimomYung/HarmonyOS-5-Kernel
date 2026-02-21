#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2024-2033. All rights reserved.

set -e

export TOPDIR=$(cd $(dirname $0); pwd)

if [ -e "$TOPDIR/output/tools/bin/clang" ];then
    export TOOLCHAIN_FILE='aarch64-euler-elf-toolchain.cmake'
    export TOOLCHAIN_CC='clang'
    dot_config='.config_llvm'
elif [ -e "$TOPDIR/output/tools/bin/aarch64-hongmeng-musl-gcc" ];then
    export TOOLCHAIN_FILE='aarch64-hongmeng-musl-toolchain.cmake'
    export TOOLCHAIN_CC='aarch64-hongmeng-musl-gcc'
    dot_config='.config_gcc'
else
    echo "Please execute build_tools.sh first"
    exit 1
fi

source ./environment-setup-aarch64-euler-elf
source scripts/common.sh

build_all() {
	cd $TOPDIR
	local modules=$(readconf modules)
	for mod in ${modules[@]}; do
		pushd ./scripts/$(mod_path ${mod})
		./build.sh
		popd
	done
}

clean_all() {
	cd $TOPDIR
	local modules=$(readconf prepare)
	for mod in ${modules[@]}; do
		rm -rf ${TOPDIR}/$(mod_path ${mod})/build
	done
}

prepare() {
	cd $TOPDIR
	mkdir -p ${TOPDIR}/output/tools
	mkdir -p ${TOPDIR}/output/aarch64/usr/include
#install headers
	local modules=$(readconf prepare)
	for mod in ${modules[@]}; do
		cd $TOPDIR
		mod_path=$(mod_path ${mod})
		mkdir ${TOPDIR}/${mod_path}/build
		if [ "$mod" == "hm-compatible-headers" ];then
			cd ${TOPDIR}/${mod_path}/build
			cp ${TOPDIR}/scripts/self_source/$mod/.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-compatible-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build
		fi
		if [ "$mod" == "hm-filesystems-libs" ];then
			cp ${TOPDIR}/scripts/self_source/$mod/$dot_config ${TOPDIR}/${mod_path}/.config
			cp ${TOPDIR}/scripts/self_source/$mod/.config_headers ${TOPDIR}/${mod_path}/
			cd ${TOPDIR}/${mod_path}/build
			cp ../.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}/fslibs
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-filesystems-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build
		fi
		if [ "$mod" == "hm-network-libs" ];then
			cp ${TOPDIR}/scripts/self_source/$mod/$dot_config ${TOPDIR}/${mod_path}/.config
			cp ${TOPDIR}/scripts/self_source/$mod/.config_headers ${TOPDIR}/${mod_path}/
			cd ${TOPDIR}/${mod_path}/build
			cp ../.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}/ulibs/stub
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-network-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build
		fi
		if [ "$mod" == "hm-security-libs" ];then
			cp ${TOPDIR}/scripts/self_source/$mod/$dot_config ${TOPDIR}/${mod_path}/.config
			cp ${TOPDIR}/scripts/self_source/$mod/.config_headers ${TOPDIR}/${mod_path}/
			cd ${TOPDIR}/${mod_path}/build
			cp ../.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}/include
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-security-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build
			cp -rf ${TOPDIR}/platform/hitls/include/* ${TOPDIR}/output/aarch64/usr/include/
			cp -rf ${TOPDIR}/platform/hitls/hm-hitls-libs/* ${TOPDIR}/output/aarch64/usr/lib/
			cp -rf ${TOPDIR}/scripts/self_source/hm-libsepol-headers/usr/include/* ${TOPDIR}/output/aarch64/usr/include/
		fi
		if [ "$mod" == "hm-libvsync" ];then
			cd ${TOPDIR}/${mod_path}/build
			cp ${TOPDIR}/scripts/self_source/$mod/.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-libvsync-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build
		fi
		if [ "$mod" == "hm-ulibs" ];then
#kernel-headers
			mkdir ${TOPDIR}/${mod_path}/build/kernel
			install -d ${TOPDIR}/output/aarch64/usr/include/hmkernel
			cp -a ${TOPDIR}/${mod_path}/kernel/* ${TOPDIR}/${mod_path}/build/kernel/
			cp -a ${TOPDIR}/${mod_path}/build/kernel/include/mapi/uapi/hmkernel/* ${TOPDIR}/output/aarch64/usr/include/hmkernel
			cp -a ${TOPDIR}/${mod_path}/build/kernel/modules/include/mapi/uapi/hmkernel/* ${TOPDIR}/output/aarch64/usr/include/hmkernel
			cp -a ${TOPDIR}/${mod_path}/build/kernel/modules/include/mapi/uapi/misc ${TOPDIR}/output/aarch64/usr/include
			cp -a ${TOPDIR}/${mod_path}/build/kernel/arch/aarch64/include/mapi/uapi/hmasm ${TOPDIR}/output/aarch64/usr/include

			cp -af --remove-destination ${TOPDIR}/${mod_path}/build/kernel/arch/arm/include/mapi/uapi/hmasm/current.h ${TOPDIR}/output/aarch64/usr/include/hmasm/current_compat.h
#ulibs-headers
			mkdir ${TOPDIR}/${mod_path}/build/ulibs
			cd ${TOPDIR}/${mod_path}/build/ulibs
			cp ${TOPDIR}/scripts/${mod_path}/ulibs/$dot_config ${TOPDIR}/${mod_path}/ulibs/.config
			cp ${TOPDIR}/scripts/${mod_path}/ulibs/.config_headers ${TOPDIR}/${mod_path}/ulibs/
			cp ../../ulibs/.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}/ulibs
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build/ulibs --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-ulibs-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build/ulibs
#devmgr-headers
			mkdir ${TOPDIR}/${mod_path}/build/devmgr
			cd ${TOPDIR}/${mod_path}/build/devmgr
			cp ${TOPDIR}/scripts/${mod_path}/devmgr/$dot_config ${TOPDIR}/${mod_path}/devmgr/.config
			cp ${TOPDIR}/scripts/${mod_path}/devmgr/.config_headers ${TOPDIR}/${mod_path}/devmgr/
			cp ../../devmgr/.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}/devmgr/libs
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build/devmgr --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-devmgr-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build/devmgr
#klibs-headers
			mkdir ${TOPDIR}/${mod_path}/build/klibs
			cd ${TOPDIR}/${mod_path}/build/klibs
			cp ${TOPDIR}/scripts/${mod_path}/klibs/.config_headers ${TOPDIR}/${mod_path}/klibs/
			cp -f ${TOPDIR}/scripts/${mod_path}/klibs/HMbuild ${TOPDIR}/${mod_path}/klibs/
			cp ../../klibs/.config_headers .config
			cmake -DCMAKE_MODULE_PATH:PATH=$TOPDIR/output/aarch64/usr/share/cmake/Modules \
				-DHMNATIVESDKPATH:PATH=$TOPDIR/output/tools \
				-DCMAKE_SYSROOT:PATH=$TOPDIR/output/aarch64 \
				-DCMAKE_TOOLCHAIN_FILE=$TOPDIR/output/aarch64/usr/share/cmake/Modules/$TOOLCHAIN_FILE \
				-DBUILD_USE_HMBUILD=TRUE \
				${TOPDIR}/${mod_path}/klibs
			export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(${TOOLCHAIN_CC} -print-file-name=plugin)
			cmake --build ${TOPDIR}/${mod_path}/build/klibs --verbose -- -j 32
			install -d ${TOPDIR}/output/aarch64/usr/share/info
			install .config ${TOPDIR}/output/aarch64/usr/share/info/config-hm-klibs-headers
			DESTDIR="${TOPDIR}/output/aarch64" cmake --install ${TOPDIR}/${mod_path}/build/klibs
#sysif-headers
			cp -r ${TOPDIR}/${mod_path}/sysif/generated ${TOPDIR}/output/aarch64/usr/include/
			cp -r ${TOPDIR}/${mod_path}/sec_headers/policy_def.h ${TOPDIR}/output/aarch64/usr/include/
			cp -r ${TOPDIR}/${mod_path}/sec_headers/libsecplc ${TOPDIR}/output/aarch64/usr/include/
#metalibs-headers
			cp -r ${TOPDIR}/${mod_path}/libs/metalibs/include/* ${TOPDIR}/output/aarch64/usr/include/
			cp -r ${TOPDIR}/scripts/${mod_path}/libs/metalibs/$dot_config ${TOPDIR}/${mod_path}/libs/metalibs/.config
			cp -r ${TOPDIR}/scripts/${mod_path}/libs/metalibs/.config_headers ${TOPDIR}/${mod_path}/libs/metalibs/
#sysmgr
			cp -r ${TOPDIR}/${mod_path}/sysmgr/include/intapi $TOPDIR/output/aarch64/usr/include/
		fi
	done
# hm-kernel-sysif-headers-symlinks
	mgrs="freqmgr \
          srvmgr \
          devmgr \
          devhost \
          pwrmgr \
          hguard \
          uvmm \
          auditmgr \
          keymgr \
          hmtracemgr \
          sysmgr \
	  eventcb\
          crypt"

    for mgr in ${mgrs}
    do
        install -d $TOPDIR/output/aarch64/usr/include/libsysif/${mgr}
        ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_client/${mgr}/libsysif/${mgr}/api.h \
            $TOPDIR/output/aarch64/usr/include/libsysif/${mgr}/api.h
        ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/${mgr}/libsysif/${mgr}/server.h \
            $TOPDIR/output/aarch64/usr/include/libsysif/${mgr}/server.h
    done

    # to be deleted
    mgr="crypt"
    sym_dir="libhmsrv_crypt/sysif"
    install -d $TOPDIR/output/aarch64/usr/include/${sym_dir}
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_client/${mgr}/libsysif/${mgr}/api.h \
        $TOPDIR/output/aarch64/usr/include/${sym_dir}/api.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/${mgr}/libsysif/${mgr}/server.h \
        $TOPDIR/output/aarch64/usr/include/${sym_dir}/server.h

    # install sysif templates place holder
    install -d $TOPDIR/output/aarch64/usr/include/libsysif/base
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_base/libsysif/base/client_template.h \
        $TOPDIR/output/aarch64/usr/include/libsysif/base/client_template.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_base/libsysif/base/server_template.h \
        $TOPDIR/output/aarch64/usr/include/libsysif/base/server_template.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_base/libsysif/base/server_actv_template.h \
        $TOPDIR/output/aarch64/usr/include/libsysif/base/server_actv_template.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_base/libsysif/base/api_common.h \
        $TOPDIR/output/aarch64/usr/include/libsysif/base/api_common.h

# hm-filesystmes-sysif-headers-symlinks
# prepare the target directory
    install -d $TOPDIR/output/aarch64/usr/include/libsysif/fs

    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_client/fs/libsysif/fs/api.h $TOPDIR/output/aarch64/usr/include/libsysif/fs/api.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/fs/libsysif/fs/server.h $TOPDIR/output/aarch64/usr/include/libsysif/fs/server.h

    # to be deleted
    install -d $TOPDIR/output/aarch64/usr/include/sysif
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/fs/libsysif/fs/server.h $TOPDIR/output/aarch64/usr/include/sysif/fs_server.h

# hm-network-sysif-headers-symlinks
    install -d $TOPDIR/output/aarch64/usr/include/libsysif_net

    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_client/net/libsysif_net/api.h $TOPDIR/output/aarch64/usr/include/libsysif_net/api.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/net/libsysif_net/service.h $TOPDIR/output/aarch64/usr/include/libsysif_net/service.h

# hm-security-sysif-headers-symlinks
   # prepare the target directory
    install -d $TOPDIR/output/aarch64/usr/include/libsysif/test_framework
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_client/test_framework/libsysif/test_framework/api.h \
        $TOPDIR/output/aarch64/usr/include/libsysif/test_framework/api.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/test_framework/libsysif/test_framework/server.h \
        $TOPDIR/output/aarch64/usr/include/libsysif/test_framework/server.h

    # to be deleted
    install -d $TOPDIR/output/aarch64/usr/include/sysif
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_client/test_framework/libsysif/test_framework/api.h \
        $TOPDIR/output/aarch64/usr/include/sysif/test_framework_api.h
    ln -sr $TOPDIR/output/aarch64/usr/include/generated/sysif_server/test_framework/libsysif/test_framework/server.h \
        $TOPDIR/output/aarch64/usr/include/sysif/test_framework_server.h

    mkdir -p $TOPDIR/output/aarch64/usr/include/libhmsrv_audit/
    cp -rf ${TOPDIR}/scripts/self_source/hm-audit-headers/server_audit_num.h  $TOPDIR/output/aarch64/usr/include/libhmsrv_audit/server_audit_num.h

}

clean_all
prepare
build_all
# clean_all
