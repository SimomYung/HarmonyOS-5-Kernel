#!/bin/bash
# Copyright (c) 2024 Huawei Device Co., Ltd.
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

set -e

# 定义 JSON 文件路径
JSON_FILE="build-tex.json"
TEX_SOURCE_DIR="../../hyph-utf8/tex/generic/hyph-utf8/patterns/tex"
HPB_OUT_DIR="./out_hpb"

# 检查 jq 是否安装
if ! command -v jq &> /dev/null
then
    echo "jq could not be found, please install jq to proceed."
    exit
fi

# 编译可执行文件
g++ -g -Wall ../src/hyphen-build/hyphen_pattern_processor.cpp -o transform

# 读取 JSON 文件并遍历数组，获取每个对象的 language 字段
jq -c '.[]' "$JSON_FILE" | while read -r item; do
    FILENAME=$(echo "$item" | jq -r '.filename')
    echo "filename: $FILENAME"
    ./transform "$TEX_SOURCE_DIR/$FILENAME" "$HPB_OUT_DIR"
done

