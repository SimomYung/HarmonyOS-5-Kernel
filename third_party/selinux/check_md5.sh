#! /bin/bash

# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

output_file=$1
input_file=$2
md5sum_file=$3

if [ ! -f ${input_file} ] || [ ! -f ${output_file} ];then
    echo "some files doesn't exit."
    exit
fi

if [ -f ${md5sum_file} ];then
    last_md5=$(cat ${md5sum_file})
    this_md5=$(md5sum ${input_file} | awk '{print $1}')
    if [ $last_md5 != $this_md5 ];then
        # update md5 file
        md5sum ${input_file} | awk '{print $1}' > ${md5sum_file}
        echo "update md5sum file"
        exit       
    fi
else
    md5sum ${input_file} | awk '{print $1}' > ${md5sum_file}
    echo "md5sum file doesn't exit."
    exit
fi
