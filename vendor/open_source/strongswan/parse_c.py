#!/usr/bin/env python
# -*- coding: utf-8 -*-
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

import argparse
import os
import subprocess
import sys


def parse_h_files(files, gen_dir):
    try:
        for file in files:
            if os.path.exists(os.path.join(gen_dir, file)):
                dirname = os.path.dirname(file)
                out_filename, _ = os.path.splitext(os.path.basename(file)) # xxx.h.in -> xxx.h
                filepath = os.path.join(dirname, out_filename)
                input_filepath = os.path.join(gen_dir, file)
                output_filepath = os.path.join(gen_dir, filepath)
                cp_cmd = ['cp', '-f', input_filepath, output_filepath]
                subprocess.run(cp_cmd, check=True)
                parse_cmd = ['sed', '-i', '-e', 's:\@GPERF_LEN_TYPE\@:size_t:', output_filepath]
                subprocess.run(parse_cmd, check=True)

    except Exception as e:
        print("parse error!")
        return


def parse_c_files(params, files, gen_dir):
    try:
        for file in files:
            if os.path.exists(os.path.join(gen_dir, file)):
                dirname = os.path.dirname(file)
                filename, _ = os.path.splitext(os.path.basename(file)) # xxx.txt -> xxx
                out_filename = '{}{}'.format(filename, '.c')
                filepath = os.path.join(dirname, out_filename)
                input_filepath = os.path.join(gen_dir, file)
                output_filepath = os.path.join(gen_dir, filepath)
                parse_cmd = ['gperf']
                for param in params:
                    parse_cmd += param.split()

                parse_cmd += ['--output-file={}'.format(output_filepath), input_filepath]
                subprocess.run(parse_cmd, check=True)

    except Exception as e:
        print("parse error!")
        return


def main():
    parse_path = argparse.ArgumentParser()
    parse_path.add_argument('--gen-dir', help='generate path of strongswan')
    parse_path.add_argument('--parse-files-h', help='.h.in files need to be parsed')
    parse_path.add_argument('--parse-files-txt', help='.txt files need to be parsed')
    parse_path.add_argument('--parse-args', help='params for gperf cmd')
    args = parse_path.parse_args()
    gen_dir = args.gen_dir
    parse_files_h = args.parse_files_h.split()
    parse_files_txt = args.parse_files_txt.split()
    parse_params = args.parse_args.split()
    parse_h_files(parse_files_h, gen_dir)
    parse_c_files(parse_params, parse_files_txt, gen_dir)
    return 0

if __name__ == '__main__':
    sys.exit(main())
