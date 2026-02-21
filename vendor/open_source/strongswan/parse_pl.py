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


def parse_pl_files(files, gen_dir):
    try:
        for file in files:
            if os.path.exists(os.path.join(gen_dir, file)):
                dirname = os.path.dirname(file)
                pl_filename = os.path.basename(file)
                dirname_filepath = os.path.join(gen_dir, dirname)
                os.chdir(dirname_filepath)
                parse_cmd = ['perl', pl_filename]
                subprocess.run(parse_cmd, check=True)

    except Exception as e:
        print("parse error!")
        return


def main():
    parse_path = argparse.ArgumentParser()
    parse_path.add_argument('--gen-dir', help='generate path of strongswan')
    parse_path.add_argument('--parse-files-pl', help='pl files to be parsed')
    args = parse_path.parse_args()
    gen_dir = args.gen_dir
    parse_files_pl = args.parse_files_pl.split()
    parse_pl_files(parse_files_pl, gen_dir)
    return 0

if __name__ == '__main__':
    sys.exit(main())
