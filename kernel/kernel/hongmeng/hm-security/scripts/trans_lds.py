#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Tue Sep 8th 12:00:00 2020
"""This scripts translates the lds for xom

"""

from __future__ import absolute_import
import os
import re
import stat
import argparse

PATTERN_RESOLVED = 100
PATTERN_NOT_MATCH = 101

RET_MATCHED = 0
RET_FBUFFER = 1

ARM = 0
AARCH64 = 1


class Profile:
    """The class for the basic profile"""

    def __init__(self):
        self.__is_relro = 0
        self.__is_static = 0
        self.__arch = ARM
        self.__is_xom = False
        self.__set_linker = True

    @property
    def is_relro(self):
        """Decorator for is_relro"""
        return self.__is_relro

    @property
    def is_static(self):
        """Decorator for is_static"""
        return self.__is_static

    @property
    def arch(self):
        """Decorator for arch"""
        return self.__arch

    @property
    def is_xom(self):
        """Decorator for is_xom"""
        return self.__is_xom

    @property
    def set_linker(self):
        """Decorator for set_linker"""
        return self.__set_linker

    @is_relro.setter
    def is_relro(self, value):
        self.__is_relro = value

    @is_static.setter
    def is_static(self, value):
        self.__is_static = value

    @arch.setter
    def arch(self, value):
        self.__arch = value

    @is_xom.setter
    def is_xom(self, value):
        self.__is_xom = value

    @set_linker.setter
    def set_linker(self, value):
        self.__set_linker = value


class Segment:
    """The class for the segment in the elf"""
    def __init__(self, sec_pattern, pre_hook, post_hook, coalesce):
        self.patterns = sec_pattern
        self.pre_hook = pre_hook
        self.post_hook = post_hook
        self.pre_done = False
        self.post_done = False
        self.need_coalesce = coalesce

    def common_resolve_line(self, line, fbuffer, file_output):
        """Resolve the common line"""
        match_or_not = PATTERN_NOT_MATCH
        for patterns in self.patterns:
            if re.match(patterns[0], line):
                matched_str = re.sub(patterns[1], patterns[2], fbuffer)
                # not matched, set post_done if needed
                if matched_str == fbuffer:
                    continue
                match_or_not = PATTERN_RESOLVED
                fbuffer = re.sub(patterns[1], patterns[3], fbuffer)
                append_outlds(matched_str, file_output)
                break
        return match_or_not, fbuffer

    def resolve_line(self, line, fbuffer, profile, file_output):
        """Resolve line"""
        for patterns in self.patterns:
            matched_line = re.search(patterns[0], line)
            if matched_line and (not self.pre_done):
                if self.pre_hook is not None:
                    self.pre_hook(fbuffer, profile, file_output)
                self.pre_done = True
            if matched_line:
                if self.need_coalesce:
                    return self.resolve_all(fbuffer, file_output)
                return self.common_resolve_line(line, fbuffer, file_output)
            if (not matched_line) and self.pre_done and (not self.post_done):
                self.post_done = True
                if self.post_hook is not None:
                    self.post_hook(line, fbuffer)
        return PATTERN_NOT_MATCH, fbuffer

    def resolve_all(self, fbuffer, file_output):
        """Resolve all"""
        match_or_not = PATTERN_NOT_MATCH
        for patterns in self.patterns:
            if re.match(patterns[1], fbuffer):
                matched_str = re.sub(patterns[1], patterns[2], fbuffer)
                # not matched, set post_done if needed
                if matched_str == fbuffer:
                    continue
                match_or_not = PATTERN_RESOLVED
                fbuffer = re.sub(patterns[1], patterns[3], fbuffer)
                append_outlds(matched_str, file_output)
        return match_or_not, fbuffer


_common_pattern = [
        r'^[ \t]+%s[ \t]+:',
        r'(?P<prev>[\s\S]*)(?P<section>[ \t]+%s[ \t]+:[^{}]+{[^{}]*})'
        r'(?P<middle>[^\r\n]*[\r\n])(?P<last>[\s\S]*)',
        r'\g<section> %s\g<middle>',
        r'\g<prev>\g<last>'
]

_only_if_ro_pattern = [
        r'^[ \t]+%s[ \t]+:',
        r'(?P<prev>[\s\S]*)(?P<section>[ \t]+%s[ \t]+:[ \t]+ONLY_IF_RO\s+{[^{}]*})'
        r'(?P<middle>[^\r\n]*[\r\n])(?P<last>[\s\S]*)',
        r'\g<section> %s\g<middle>',
        r'\g<prev>\g<last>'
]
_only_if_rw_pattern = [
        r'^[ \t]+%s[ \t]+:',
        r'(?P<prev>[\s\S]*)(?P<section>[ \t]+%s[ \t]+:[ \t]+ONLY_IF_RW\s+{[^{}]*})'
        r'(?P<middle>[^\r\n]*[\r\n])(?P<last>[\s\S]*)',
        r'\g<section> %s\g<middle>',
        r'\g<prev>\g<last>'
]

_line_match_pattern = [
        r'^[ \t]*%s[ \t]*[\r\n]',
        r'(?P<prev>[\s\S]*)(?P<target_str>[ \t]*%s[^\r\n]*[\r\n])(?P<last>[\s\S]*)',
        r'\g<target_str>',
        r'\g<prev>\g<last>'
]

_rodata_template = [
        ['.interp', ':rodata :interp'],
        ['.note.gnu.build-id', ':rodata'],
        ['.hash', ':rodata'],
        ['.gnu.hash', ':rodata'],
        ['.dynsym', ':rodata'],
        ['.dynstr', ':rodata'],
        ['.gnu.version', ':rodata'],
        ['.gnu.version_d', ':rodata'],
        ['.gnu.version_r', ':rodata'],
        ['.rela.dyn', ':rodata'],
        ['.rela.plt', ':rodata'],
        ['.rel.dyn', ':rodata'],
        ['.rel.plt', ':rodata'],
        ['.rodata', ':rodata'],
        ['.rodata1', ':rodata'],
        ['.ARM.extab', ':rodata'],
        ['.ARM.exidx', ':rodata'],
        ['.eh_frame_hdr', ':rodata :eh_frame_header']
]

_rodata_only_if_ro_template = [
        ['.eh_frame', ':rodata'],
        ['.gcc_except_table', ':rodata'],
        ['.gnu_extab', ':rodata'],
        ['.exception_ranges', ':rodata'],
]

_data_only_if_rw_template = [
        ['.eh_frame', ':data'],
        ['.gcc_except_table', ':data'],
        ['.gnu_extab', ':data'],
        ['.exception_ranges', ':data']
]

_text_template = [
        ['.init', ':text'],
        ['.plt', ':text'],
        ['.iplt', ':text'],
        ['.text', ':text'],
        ['.fini', ':text'],
]

_xom_text_template = [
        ['.text.xom', ':text_xom']
]

_data_template = [
        ['.tdata', ':tls :data'],
        ['.tbss', ':tls'],
        ['.preinit_array', ':data'],
        ['.init_array', ':data'],
        ['.fini_array', ':data'],
        ['.ctors', ':data'],
        ['.dtors', ':data'],
        ['.jcr', ':data'],
        ['.data.rel.ro', ':data'],
        ['.dynamic', ':dynamic :data'],
        ['.got', ':data'],
        ['.data', ':data'],
        ['.data1', ':data'],
        ['.bss', ':data'],
]

_phdr_template = [
        ['SECTIONS', 'SECTIONS']
]


def gen_line_match_pattern(line_temp):
    """Get the line match pattern"""
    seg0 = _line_match_pattern[0] % line_temp[0]
    seg1 = _line_match_pattern[1] % line_temp[0]
    seg2 = _line_match_pattern[2]
    seg3 = _line_match_pattern[3]
    return [seg0, seg1, seg2, seg3]


def gen_common_pattern(pattern, temp):
    """Get the common pattern"""
    seg0 = pattern[0] % re.escape(temp[0])
    seg1 = pattern[1] % re.escape(temp[0])
    seg2 = pattern[2] % temp[1]
    seg3 = pattern[3]
    return [seg0, seg1, seg2, seg3]


def generate_phdr(segment_list):
    """Generate the phdr"""
    patterns = []
    for temp in _phdr_template:
        tmp = gen_line_match_pattern(temp)
        patterns.append(tmp)
    _segment = Segment(patterns, emit_phdr, None, False)
    segment_list.append(_segment)


def generate_rodata(segment_list):
    """Generate read-only data"""
    patterns = []
    for temp in _rodata_template:
        tmp = gen_common_pattern(_common_pattern, temp)
        patterns.append(tmp)
    for temp in _rodata_only_if_ro_template:
        tmp = gen_common_pattern(_only_if_ro_pattern, temp)
        patterns.append(tmp)
    _segment = Segment(patterns, None, None, True)
    segment_list.append(_segment)


def emit_text_prev(_fbuffer, profile, file_output):
    """Emit text prev"""
    append_outlds(' . = ALIGN(CONSTANT(MAXPAGESIZE)) + (. & (CONSTANT(MAXPAGESIZE) - 1));\n', file_output)
    if profile.is_xom and (profile.arch == ARM):
        append_outlds(" .text.xom : {*(.text.xom)} :text_xom\n", file_output)
        append_outlds(' . = ALIGN(CONSTANT(MAXPAGESIZE)) + (. & (CONSTANT(MAXPAGESIZE) - 1));\n', file_output)


def generate_text(segment_list):
    """Generate text"""
    patterns = []
    for temp in _text_template:
        tmp = gen_common_pattern(_common_pattern, temp)
        patterns.append(tmp)
    _segment = Segment(patterns, emit_text_prev, None, False)
    segment_list.append(_segment)


def generate_data(segment_list):
    """Generate data"""
    patterns = []
    for temp in _data_template:
        tmp = gen_common_pattern(_common_pattern, temp)
        patterns.append(tmp)
    for temp in _data_only_if_rw_template:
        tmp = gen_common_pattern(_only_if_rw_pattern, temp)
        patterns.append(tmp)
    _segment = Segment(patterns, None, None, False)
    segment_list.append(_segment)


def generate_segments(segment_list):
    """Generate the segments"""
    generate_phdr(segment_list)
    generate_rodata(segment_list)
    generate_text(segment_list)
    generate_data(segment_list)


def append_outlds(outstr, file_output):
    """Append outlds"""
    file_output.append(outstr)


def trans_lds(file_input, profile, segment_list, file_output):
    """Translate the lds"""
    if not profile.is_xom:
        return
    r_lines = ()
    matched = False
    lines = file_input.splitlines(True)
    if not lines:
        return
    for lineno, line in enumerate(lines):
        for segment in segment_list:
            r_lines = segment.resolve_line(line, file_input, profile, file_output)
            if r_lines[RET_MATCHED] == PATTERN_NOT_MATCH:
                continue
            matched = True
            break
        if matched:
            file_input = r_lines[RET_FBUFFER]
            break
        append_outlds(line, file_output)
        file_input = ''.join(lines[lineno + 1:])
    trans_lds(file_input, profile, segment_list, file_output)


def check_ldflags(file_input):
    """Check the ldflags"""
    is_relro = 0
    is_static = 0
    is_xom = False
    set_linker = True
    arch = ARM
    if re.search('-z relro', file_input):
        is_relro = 1
    if args.executable == 'static':
        is_static = 1
    if args.linker == 'nolinker':
        set_linker = False
    if re.search(r'OUTPUT_ARCH(aarch64)', file_input):
        arch = AARCH64
    elif re.search(r'OUTPUT_ARCH(arm)', file_input):
        arch = ARM
    is_xom = args.xom
    profile = Profile()
    profile.is_relro = is_relro
    profile.is_static = is_static
    profile.is_xom = is_xom
    profile.arch = arch
    profile.set_linker = set_linker
    return profile


def emit_phdr(_fbuffer, profile, file_output):
    """Emit phdr"""
    append_outlds("PHDRS {\n", file_output)
    append_outlds("\tphdr PT_PHDR PHDRS;\n", file_output)
    if profile.set_linker:
        append_outlds("\tinterp PT_INTERP;\n", file_output)
    append_outlds("\trodata PT_LOAD FILEHDR PHDRS FLAGS(4);\n", file_output)
    if profile.is_xom and (profile.arch == ARM):
        append_outlds("\ttext_xom PT_LOAD FLAGS(1);\n", file_output)
        append_outlds("\ttext PT_LOAD FLAGS(5);\n", file_output)
    else:
        append_outlds("\ttext PT_LOAD FLAGS(1);\n", file_output)
    if not profile.is_static:
        append_outlds("\tdynamic PT_DYNAMIC;\n", file_output)
    append_outlds("\ttls PT_TLS;\n", file_output)
    append_outlds("\teh_frame_header PT_GNU_EH_FRAME;\n", file_output)
    append_outlds("\tdata PT_LOAD FLAGS(6);\n", file_output)
    append_outlds("\tgnu_stack PT_GNU_STACK FLAGS(6);\n", file_output)
    if profile.is_relro:
        append_outlds("\tgnu_relro 0x6474e552 FLAGS(4);\n", file_output)
    append_outlds("}\n", file_output)


def correct_format(file_input):
    """Correct the format"""
    matched = re.split(r'={7,}[^\r\n]*[\r\n]', file_input)
    if len(matched) >= 3:
        return matched[1]
    return file_input

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="parse")
    parser.add_argument("-i", "--i", type=str, help="input lds")
    parser.add_argument("-o", "--o", type=str, help="output lds")
    parser.add_argument("executable", choices=['static', 'dynamic'],
                        type=str, help="dynamic or static")
    parser.add_argument("linker", choices=['linker', 'nolinker'],
                        type=str, help="set dynamic linker or not")
    parser.add_argument("-xom", "--xom", default=False, action="store_true", help="xom")
    args = parser.parse_args()
    tmp_segment_list = []

    out_lds = []
    with open(args.i, 'r') as f:
        finput = f.read()
        new_profile = check_ldflags(finput)
        generate_segments(tmp_segment_list)
        finput = correct_format(finput)
        trans_lds(finput, new_profile, tmp_segment_list, out_lds)

    FLAGS = os.O_WRONLY | os.O_CREAT
    MODES = stat.S_IWUSR | stat.S_IRUSR | stat.S_IRGRP | stat.S_IROTH
    with os.fdopen(os.open(args.o, FLAGS, MODES), 'w') as f:
        f.write(''.join(out_lds))
