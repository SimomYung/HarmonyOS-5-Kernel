#!/usr/bin/env python3
# coding: utf-8
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
import sys
import subprocess
import os


def run_command(command):
    result = subprocess.run(command, shell=False, capture_output=True)
    return result.stdout, result.stderr, result.returncode


def main():
    if len(sys.argv) != 4:
        print("Usage: python generate_hpb.py <hpb_transform_exe> <tex_file_path> <output_hpb_file>")
        sys.exit(1)

    hpb_transform_exe = sys.argv[1]
    tex_file_path = sys.argv[2]
    output_hpb_file = sys.argv[3]

    # 检查并创建输出文件夹
    output_dir = os.path.dirname(output_hpb_file)
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Created directory: {output_dir}")

    # 创建命令
    command_str = f"{hpb_transform_exe} {tex_file_path} {output_hpb_file}"
    command = [hpb_transform_exe, tex_file_path, output_hpb_file]
    print(f"hpy_command: {command_str}")

    # 执行命令
    stdout, stderr, returncode = run_command(command)
    if returncode == 0:
        print("Command executed successfully.")
    else:
        print(f"Command failed with return code {returncode}")
        print(f"Error output: {stderr.decode('utf-8')}")


if __name__ == "__main__":
    main()
