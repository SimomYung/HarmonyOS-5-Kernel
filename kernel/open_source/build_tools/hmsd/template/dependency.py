#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide some classes to describe dependencies
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import itertools
from dataclasses import dataclass
from typing import Union, List
from .node import TextualizedFunction, DescribedFunction
from ..utils import argument_unwind, argument_filter


@dataclass
class Dependency():
    """
    Dependency describes dependencies on objects specified by a path or some
    paths.
    """

    target_path: Union[str, List[str]]

    def target(self, iterator):
        """
        Get the target objects according to a iterator and the stored paths.
        """
        if isinstance(self.target_path, str):
            return iterator.get_object_by_path(self.target_path)

        return [iterator.get_object_by_path(p) for p in self.target_path]


class SimpleDependOn(TextualizedFunction):
    """
    SimpleDependOn describes dependencies on unoperated objects.
    """

    def __init__(self, target_path):
        self._dependency = Dependency(target_path)


    def __call__(self, **kwargs):
        """
        Call the function.
        """
        iterator, = argument_unwind(kwargs, ['iterator'])
        dep = self._dependency.target(iterator)
        if isinstance(self._dependency.target_path, list):
            return itertools.accumulate(dep, lambda a, b: a and b)
        return dep


    @property
    def _textualized_dependency(self):
        """
        Get the textualized dependency.
        """
        target_path = self._dependency.target_path
        return '`, `'.join(target_path) if isinstance(target_path, list) else target_path

    def textualize(self):
        """
        Return a text to describe the function.
        """
        return f'The value depends on `{self._textualized_dependency}`.'


class DependOn(SimpleDependOn):
    """
    DependOn describes dependencies on objects and an operation.
    """

    def __init__(self, target_path, function, description):
        super().__init__(target_path)
        self._fn = DescribedFunction(function, description)


    def __call__(self, **kwargs):
        """
        Call the function.
        """
        iterator = argument_filter(kwargs, ['iterator'])[0]
        dep = self._dependency.target(iterator)
        return self._fn(dep=dep, **kwargs)


    def textualize(self):
        """
        Return a text to describe the function.
        """
        return f'{super().textualize()} {self._fn.description}'


class DependOnNot(SimpleDependOn):
    """
    DependOnNot describes dependencies on the opposit of objects.
    """

    def __call__(self, **kwargs):
        """
        Call the function.
        """
        return not super().__call__(**kwargs)


    def textualize(self):
        """
        Return a text to describe the function.
        """
        return f'The value depends on the opposite of `{self._textualized_dependency}`.'


@dataclass
class DefaultDictDependOn(SimpleDependOn):
    """
    DefaultDictDependOn uses the objects of the dependency as the key to get a
    value from the dictory of special cases.
    """

    def __init__(self, target_path, default, special_cases):
        super().__init__(target_path)
        self.default = default
        self.special_cases = special_cases


    def __call__(self, **kwargs):
        """
        Call the function.
        """
        iterator, = argument_unwind(kwargs, ['iterator'])
        target_path = self._dependency.target_path
        if not isinstance(target_path, str):
            raise ValueError('multi-dependence is not allowed')
        return self.special_cases.get(iterator.get_object_by_path(target_path), self.default)


    def textualize(self):
        """
        Return a text to describe the function.
        """
        target_path = self._dependency.target_path
        special_info = '; '.join([
            f'the default value is `{self.special_cases[k]}` when `{target_path}` is `{k}`' \
            for k in self.special_cases
        ])
        return f'{super().textualize()} Usually the default value is `{self.default}`, but {special_info}.'

# vim: ts=4:sw=4:expandtab
