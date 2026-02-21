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
import sys


def generate_output(plugins, output_file=None):
    """Generate a source file containing plugin constructor registrations"""
    if output_file:
        fd = os.open(output_file, os.O_WRONLY | os.O_CREAT, 0o666)
        with os.fdopen(fd, 'w', encoding='utf-8') as f:
            f.write("/**\n")
            f.write(" * Register plugin constructors for static libraries\n")
            f.write(" * Created by {0}\n".format(__file__))
            f.write(" */\n\n")
            f.write("#include <plugins/plugin.h>\n")
            f.write("#include <plugins/plugin_loader.h>\n\n")

            for plugin in plugins:
                f.write("plugin_t *{0}_plugin_create();\n".format(plugin.replace('-', '_')))

            f.write("\nstatic void register_plugins() __attribute__ ((constructor));\n")
            f.write("static void register_plugins()\n")
            f.write("{\n")

            for plugin in plugins:
                f.write('    plugin_constructor_register("{0}", {1}_plugin_create);\n'.format(plugin, plugin.replace('-', '_')))

            f.write("}\n\n")
            f.write("static void unregister_plugins() __attribute__ ((destructor));\n")
            f.write("static void unregister_plugins()\n")
            f.write("{\n")

            for plugin in plugins:
                f.write('    plugin_constructor_register("{0}", NULL);\n'.format(plugin))

            f.write("}\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--parse-plugins', help="name of a plugin for which to generate constructor registration")
    parser.add_argument('--gen-dir', help="file to write the generated output to")
    parser.add_argument('--output-file', help="file to write the generated output to")
    args = parser.parse_args()
    generate_output(args.parse_plugins.split(), os.path.join(args.gen_dir, args.output_file))

if __name__ == '__main__':
    sys.exit(main())