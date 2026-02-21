#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2010-2018. All rights reserved.
#Buildmc检查抽取部分函�?set -e

CUR_DIR=$(cd "$(dirname "$0")";pwd)
ROOT_DIR="$(dirname ${CUR_DIR})"

function prepare_simpo_tools()
{  
    mkdir -p ${ROOT_DIR}/output/simpo_tools
    rm -rf ${ROOT_DIR}/output/simpo_tools/*
    pushd ${ROOT_DIR}/output/simpo_tools
    if [ "${TARGET_PLATFORM}" = "hm" ];then
        ysd d dophi-devel-tools -a $(arch)
        rpm2cpio dophi-devel-tools*.rpm | cpio -div
    else
        ysd d hdophi-devel-tools -a $(arch)
        rpm2cpio hdophi-devel-tools*.rpm | cpio -div
        ln -s ${ROOT_DIR}/output/simpo_tools/bin/simpo_xdophi ${ROOT_DIR}/output/simpo_tools/bin/simpo
    fi
    chmod +x * -R
    export PATH=${ROOT_DIR}/output/simpo_tools/bin/:${PATH}
    popd
}

function genereate_cros_code()
{
    src_simpo_dir=${ROOT_DIR}/simpo
    cd ${src_simpo_dir}/cros/
    cros_output_dir=${ROOT_DIR}/src/extend/cli
    mkdir -p ${cros_output_dir}

    for file in $(ls *simpo || true)
    do
        simpo_file_name=${file##*/}
        simpo_file_name=${file%.simpo}
        common_cmd="simpo --simpo ${file} -o ${cros_output_dir}"

        special_gen_arg=${specical_cros_simpo[$simpo_file_name]}
        if [ ! -z "$special_gen_arg" ];then
            local_arg="${special_gen_arg}"
        else
            grep_result=$(grep -E "^[ \t]*uService[ \ta-zA-Z0-9_]*\{" ${file}) || true
            if [ -z "$grep_result" ];then
                local_arg="-a -t"
            else
                local_arg="--rpc --free"
            fi
        fi
        echo "Generating simpo file '$file' with command '$common_cmd $local_arg'."
        ${common_cmd} ${local_arg}
    done

    rm -rf ${cros_output_dir}/*simpo-stub.c
    rm -rf ${cros_output_dir}/*simpo-free.c
    echo "Generated all cros simpo file success."
    cd ..
}

function download_gtest_framework()
{
    local url
    mkdir ${OUTPUT_DIR}/test_frame
    export TEST_FRAME_DIR=${OUTPUT_DIR}/test_frame
    echo "download test framework for platform $PLATFORM"

    url="https://prod-rpm.cloudartifact.szv.dragon.tools.huawei.com/artifactory/cmc-rpm-release-YunShan-dtest-0/1.0.1"
    wget -P $TEST_FRAME_DIR ${url}/gtest-hpk-1.11.0-1.aarch64.rpm

    pushd $TEST_FRAME_DIR >/dev/null
        rpm2cpio gtest*.rpm | cpio --quiet -idm 2>/dev/null
    popd >/dev/null
    rm -rf $TEST_FRAME_DIR/*.rpm
}

function download_hmtest_framework()
{
    cd ${OUTPUT_DIR}/test_frame
    hm_url="http://100.95.111.143/repo_hpp_DevelopBranch/hpe2022/hm_testcase"
    wget ${hm_url}/hm_hal_irq_t.tar.gz
 
    tar -zxvf hm_hal_irq_t.tar.gz >/dev/null
    rm -rf ${OUTPUT_DIR}/hm_hal_irq_t.tar.gz
}
