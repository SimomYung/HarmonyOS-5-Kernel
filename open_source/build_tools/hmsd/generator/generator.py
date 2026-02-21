#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Generator class
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import os
from .. import utils


class Generator():
    """
    Generator is the base class of generators.
    """

    def __init__(self, options):
        self._options = options

    @staticmethod
    def register_options(option_parse):
        """
        Register command options
        """
        utils.do_nothing(option_parse)

    @staticmethod
    def standardize_options(options):
        """
        Standardize command options
        """
        utils.do_nothing(options)

    def generate(self, sec_desc):
        """
        Stubing generation.
        """
        utils.do_nothing(self, sec_desc)
        raise NotImplementedError()


    def get_output_file_name(self, source_file_path, new_suffix=None,
                             use_base=False):
        """
        Get the output file name.
        """
        file_name = os.path.basename(source_file_path) if use_base else source_file_path
        if new_suffix is not None:
            file_name = file_name.rsplit('.', maxsplit=1)[0]
            file_name = f'{file_name}.{new_suffix}'
        return os.path.join(self._options.output, file_name)


    def open_output_file(self, source_file_path, new_suffix=None, mode=0o644):
        """
        Open the output file.
        """
        path = self.get_output_file_name(source_file_path, new_suffix)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        return os.fdopen(os.open(path, os.O_WRONLY | os.O_CREAT, mode), 'w')


_generator_builder = utils.Builder()


def make_generator(name, options):
    """
    Make a generator by name.
    """
    return _generator_builder.make(name, options)


def register_generator(name, cls):
    """
    Register a generator class with a name.
    """
    return _generator_builder.register(name, cls)


def register_options(option_parse):
    """
    Get generator-specific options
    """
    _generator_builder.for_each(lambda gen: gen.register_options(option_parse))


def standardize_options(options):
    """
    Standardize all generator-specific command options
    """
    _generator_builder.for_each(lambda gen: gen.standardize_options(options))

# vim: ts=4:sw=4:expandtab
