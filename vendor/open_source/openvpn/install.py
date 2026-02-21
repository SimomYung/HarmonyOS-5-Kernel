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


def untar_file(tar_file_path, extract_path, args):
    try:
        if os.path.exists(extract_path):
            rm_cmd = ['rm', '-rf', extract_path]
            subprocess.run(rm_cmd, check=True)

        tar_cmd = ['tar', '-zxvf', tar_file_path, '-C', args.gen_dir]
        subprocess.run(tar_cmd, check=True)

    except Exception as e:
        print("tar error!")
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
      "0001-add-support-openvpn.patch",
      "0002-fix-openvpn-connect-fail.patch",
      "CVE-2024-28882.patch",
      "CVE-2024-5594.patch",
      "0003-get-askpass-config-from-system-vpn.patch",
      "0004-get-ovconfig-to-ipc.patch",
      "CVE-2025-2704.patch",
      "0005-fix-get-fd-error.patch"
    ]

    for patch in patch_file:
        file_path = os.path.join(args.source_file, patch)
        apply_patch(file_path, target_dir)


def main():
    openvpn_path = argparse.ArgumentParser()
    openvpn_path.add_argument('--gen-dir', help='generate path of ppp')
    openvpn_path.add_argument('--source-file', help='openvpn source compressed dir')
    args = openvpn_path.parse_args()
    tar_file_path = os.path.join(args.source_file, "openvpn-2.6.9.tar.gz")
    target_dir = os.path.join(args.gen_dir, "openvpn-2.6.9")
    untar_file(tar_file_path, target_dir, args)
    do_patch(args, target_dir)
    return 0

if __name__ == '__main__':
    sys.exit(main())
