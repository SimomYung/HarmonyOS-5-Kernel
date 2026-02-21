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

import tarfile
import argparse
import os
import subprocess
import sys
import shutil


def apply_patch(source_dir):
    patch_list = [
        'modifying_driver_search_path.patch',
        'dll.c.patch',
    ]

    for patch in patch_list:
        patch_dir = os.path.join(source_dir, 'patches', patch)
        patch_dir = os.path.abspath(patch_dir)
        source_dir = os.path.abspath(source_dir)
        patch_cmd = ['patch', '-p1', "--fuzz=0", "--no-backup-if-mismatch", '-i', patch_dir, '-d', source_dir]
        try:
            result = subprocess.run(patch_cmd, check=True, text=True, capture_output=True)
            if result.returncode != 0:
                raise subprocess.CalledProcessError(result.returncode, result.args)
        except subprocess.CalledProcessError:
            print("sane-backends: {} apply error!".format(patch))

def main():
    backends_path = argparse.ArgumentParser()
    backends_path.add_argument('--source-dir', help='generate path of log', required=True)
    args = backends_path.parse_args()
    source_dir = args.source_dir
    apply_patch(source_dir)
    return 0


if __name__ == '__main__':
    sys.exit(main())