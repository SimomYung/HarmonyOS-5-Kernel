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
import os
import subprocess
import re
import shutil
import json
import sys
import stat
import time


def run_command(command):
    result = subprocess.run(command, shell=False, capture_output=True, text=True, encoding='utf-8', errors='ignore')
    return result.stdout, result.returncode


def parse_log(log, tex_file, word, match_log_path, unmatch_log_path):
    result_size_pattern = re.compile(r'result size: (\d+) while expecting (\d+)')
    key_value_pattern = re.compile(r'([0-9a-fA-F]+): (\d+)')

    lines = log.split('\n')
    success = False
    for i, line in enumerate(lines):
        match = result_size_pattern.match(line)
        if match:
            result_size = int(match.group(1))
            expecting_size = int(match.group(2))
            if result_size == expecting_size:

                index = 0
                result_str = ""
                for j in range(i + 1, i + 1 + result_size):
                    key_value_match = key_value_pattern.match(lines[j])
                    if key_value_match:
                        value = int(key_value_match.group(2))
                        if value % 2 == 1:
                            success = True
                            result_str += f"{index}:{value} "

                        index += 1
                if not success:
                    with os.fdopen(os.open(unmatch_log_path, os.O_RDWR | os.O_CREAT, stat.S_IWUSR | stat.S_IRUSR),
                                   'a') as unmatch_log:
                        unmatch_log.write(f"{tex_file} {word}\n")
                else:
                    with os.fdopen(os.open(match_log_path, os.O_RDWR | os.O_CREAT, stat.S_IWUSR | stat.S_IRUSR),
                                   'a') as match_log:
                        match_log.write(f"{tex_file} {word} {result_str}\n")
                break


def main(config_file_name):
    with os.fdopen(os.open(config_file_name, os.O_RDONLY | os.O_CREAT, stat.S_IWUSR | stat.S_IRUSR), 'r') as f:
        config = json.load(f)

    file_path = config['file_path']
    tex_files = config['tex_files']
    out_dir = './out_hpb'

    # 创建 out_hpb 目录
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    # 创建 report 目录
    report_dir = './report'
    if not os.path.exists(report_dir):
        os.makedirs(report_dir)

    # 创建 hyphen_report_时间戳 目录
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    report_subdir = os.path.join(report_dir, f"hyphen_report_{timestamp}")
    os.makedirs(report_subdir)

    match_log_path = os.path.join(report_subdir, 'match.log')
    unmatch_log_path = os.path.join(report_subdir, 'unmatch.log')

    for tex_file in tex_files:
        tex_filename = tex_file['filename']
        words = tex_file['words']
        hpb_filename = f"{os.path.splitext(tex_filename)[0]}.hpb"
        hpb_filepath = os.path.join(out_dir, hpb_filename)

        # Step 1: Run the transform command
        transform_command = ["./transform", os.path.join(file_path, tex_filename), out_dir]
        print(f"Running transform command for {tex_filename}...")
        _, returncode = run_command(transform_command)
        if returncode != 0:
            print(f"Transform command failed for {tex_filename}. Skipping to next file.")
            continue

        for word in words:
            # Step 2: Run the reader command
            reader_command = ["./reader", hpb_filepath, word]
            print(f"Running reader command for {word}...")
            log_output, _ = run_command(reader_command)
            print("Reader command output:")
            print(log_output)

            # Step 3: Parse the log output
            print("Parsing log output...")
            parse_log(log_output, tex_filename, word, match_log_path, unmatch_log_path)

    # Step 4: Delete the out_hpb directory
    print("Deleting out_hpb directory...")
    shutil.rmtree(out_dir)
    print("out_hpb directory deleted.")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <config_file>")
        sys.exit(1)

    config_file = sys.argv[1]
    main(config_file)
