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


def copy_file(src_file_path, dst_path):
    try:
        if os.path.exists(dst_path):
            rm_cmd = ['rm', '-rf', dst_path]
            subprocess.run(rm_cmd, check=True)

        mk_cmd = ['mkdir', '-p', dst_path]
        subprocess.run(mk_cmd, check=True)

        copy_cmd = ['cp', '-rvpf', src_file_path, '-t', dst_path]
        subprocess.run(copy_cmd, check=True)

    except Exception as e:
        print("copy error!")
        return


def apply_patch(patch_file, target_dir):
    try:
        if not os.path.exists(target_dir):
            return

        patch_cmd = ['patch', '-p1', "--fuzz=0", "--no-backup-if-mismatch", '-i', patch_file, '-d', target_dir]
        subprocess.run(patch_cmd, check=True)

    except Exception as e:
        print("apply_patch error!")
        return


def do_patch(args, target_dir):
    patch_file = [
        "0001-modify-for-system-vpn.patch",
        "0002-optimized-ipc-use-innerAPI.patch",
        "0003-adapt-hongmeng-kernel.patch",
        "0004-fix-iptable.patch",
        "0005-get-conf-by-ipc.patch",
        "0006-fix-connect-fail.patch"
    ]

    for patch in patch_file:
        file_path = os.path.join(args.source_file, patch)
        apply_patch(file_path, target_dir)


def main():
    ppp_path = argparse.ArgumentParser()
    ppp_path.add_argument('--gen-dir', help='generate path of ppp')
    ppp_path.add_argument('--source-file', help='ppp source dir')
    args = ppp_path.parse_args()
    cp_dir = args.gen_dir
    copy_file(args.source_file, cp_dir)

    patch_dir = os.path.join(args.gen_dir, "ppp")
    do_patch(args, patch_dir)
    return 0

if __name__ == '__main__':
    sys.exit(main())
