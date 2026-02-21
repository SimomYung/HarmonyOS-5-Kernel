#!/usr/bin/env python
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

from dataclasses import replace
import os
import shutil
import sys
import subprocess


def _run_cmd(cmd):
    print(cmd)
    res = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE)
    sout, serr = res.communicate()
    return  sout.rstrip().decode('utf-8'), serr, res.returncode


def _make_dir(file_path):
    is_exist = os.path.exists(file_path)
    print (file_path)
    if not is_exist:
        os.makedirs(file_path)
    print (file_path)


def _read_file(file_path):
    file_handler = open(file_path, 'r+', encoding='utf-8')
    content = file_handler.read()
    file_handler.close

    return content

def _write_file(file_path, content):
    file_handler = open(file_path, 'w', encoding='utf-8')
    file_handler.write(content)
    file_handler.close


def _write_internal_methods(new_init_file, internal_method_name, init_file_list):
    new_init_file.write("void init_" + internal_method_name + "(void);\n")
    new_init_file.write("void init_" + internal_method_name + "(void)\n{\n")
    for init_file_name in init_file_list:
        new_init_file.write(init_file_name)
    new_init_file.write("}")


def _need_rebuild(src_file, dest_file, src_md5_file):
    if os.path.exists(src_file) and os.path.exists(dest_file) and os.path.exists(src_md5_file):
        this_md5, err, returncode = _run_cmd("md5sum " + src_file + " | awk '{print $1}'")
        last_md5, err, returncode = _run_cmd("cat " + src_md5_file)
        if this_md5 == last_md5:
            return 0
        else:
            return 1
    else:
        print("src_file, dest_file or src_md5_file doesn't exist. Generate new md5 file.")
        this_md5, err, returncode = _run_cmd("md5sum " + src_file + " | awk '{print $1}' >" + src_md5_file)
        return 1


def main():
    # sys.argv[1]: filter pattern
    # sys.argv[2]: exclude file list
    # sys.argv[3]: output file
    # sys.argv[4]: internal method name
    ori_path = os.path.dirname(os.path.abspath(__file__))
    out_path = os.path.dirname(os.path.dirname(os.path.dirname(ori_path)))
    out_path += "/out"
    gen_path = out_path + "/gen"
    md5_path = out_path + "/gen/md5"
    _make_dir(gen_path)
    _make_dir(md5_path)
    new_init_file_name = gen_path + "/" + sys.argv[3]
    if os.path.exists(new_init_file_name):
        os.remove(new_init_file_name)
    os.mknod(new_init_file_name)
    new_init_file = open(new_init_file_name, 'a', encoding='utf-8')

    init_file_list = []
    for filter_file in os.listdir(ori_path):
        file_name, extension = os.path.splitext(filter_file)
        if (sys.argv[1] in filter_file) & (extension == ".c") & (filter_file not in sys.argv[2]):
            print(filter_file)
            src_path = os.path.join(ori_path, filter_file)
            src_md5_file = os.path.join(md5_path, filter_file + ".md5")
            dst_path = os.path.join(gen_path, filter_file)
            need_rebuild = _need_rebuild(src_path, dst_path, src_md5_file)
            if need_rebuild:
                shutil.copy(src_path, dst_path)
            content = _read_file(dst_path)
            if "_init(void)" in content:
                replace_init_text = filter_file.rstrip("\.c") + "_init(void)"
                new_content = content.replace("_init(void)", replace_init_text)
                if need_rebuild:
                    _write_file(dst_path, new_content)

                new_init_file.write("extern " + "void  " + replace_init_text + ";\n")
                init_file_list.append(filter_file.rstrip("\.c") + "_init" + "();\n")

    internal_method_name = sys.argv[4]
    _write_internal_methods(new_init_file, internal_method_name, init_file_list)
    new_init_file.close


if __name__ == '__main__':
    sys.exit(main())
