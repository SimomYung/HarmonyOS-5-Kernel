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


def parse_y_files(ylwrap_path, files, gen_dir):
    try:
        for file in files:
            if os.path.exists(os.path.join(gen_dir, file)):
                dirname = os.path.dirname(file)
                filename, _ = os.path.splitext(os.path.basename(file))
                out_filename = '{}{}'.format(filename, '.c')
                out_filename_h = '{}{}'.format(filename, '.h')
                filepath = os.path.join(dirname, out_filename)
                filepath_h = os.path.join(dirname, out_filename_h)
                input_filepath = os.path.join(gen_dir, file)
                output_filepath = os.path.join(gen_dir, filepath)
                output_filepath_h = os.path.join(gen_dir, filepath_h)
                output_out = os.path.join(gen_dir, os.path.join(dirname, ('{}{}'.format(filename, '.out'))))
                parse_cmd = [
                    '/bin/bash',
                    ylwrap_path,
                    input_filepath,
                    'y.tab.c',
                    output_filepath,
                    'y.tab.h',
                    output_filepath_h,
                    'y.output', output_out, '--', 'bison', '-y', '-v', '-d'
                ]
                subprocess.run(parse_cmd, check=True)

    except Exception as e:
        print("parse error!" + e)
        return


def main():
    parse_path = argparse.ArgumentParser()
    parse_path.add_argument('--gen-dir', help='generate path of strongswan')
    parse_path.add_argument('--source-file', help='strongswan source dir')
    parse_path.add_argument('--parse-files', help='.y files need to be parsed')
    args = parse_path.parse_args()
    gen_dir = args.gen_dir
    source_dir = args.source_file
    parse_files = args.parse_files.split()
    ylwrap_cmd_path = os.path.join(source_dir, "ylwrap")
    parse_y_files(ylwrap_cmd_path, parse_files, gen_dir)
    return 0

if __name__ == '__main__':
    sys.exit(main())
