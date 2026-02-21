#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide some common classes and functions
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022


class Builder():
    """
    Builder is a container of classes.
    """

    def __init__(self, has_cache=False):
        self._templates = {}
        self._cache = {} if has_cache else None


    def register(self, name, cls):
        """
        Register a class.
        """
        self._templates[name] = cls


    def make(self, name, *args, use_cache=True, **kargs):
        """
        Make an object.
        """
        use_cache = self._cache is not None and use_cache
        if use_cache:
            tmpl = self._cache.get(name)
            if tmpl is not None:
                return tmpl
        try:
            tmpl = self._templates[name](*args, **kargs)
        except Exception as exception:
            raise Exception('failed to make', name, exception) from exception
        if use_cache:
            self._cache[name] = tmpl
        return tmpl


    def all_templates(self):
        """
        Get all classes.
        """
        return self._templates.keys()

    def for_each(self, function):
        """
        Each template execute a function
        """
        for _, cls in self._templates.items():
            function(cls)


def objectify(data):
    """
    Convert data to a object of Object class.
    """
    if isinstance(data, dict):
        obj = Object(data)
    elif isinstance(data, (list, tuple)):
        obj = tuple(objectify(e) for e in data)
    else:
        obj = data
    return obj


class ReadOnlyException(Exception):
    """
    Object is readonly.
    """


class Object():
    """
    Object uses attributes of python objects instead of fields of dictories.
    """

    def __init__(self, data):
        for key, value in data.items():
            setattr(self, key, objectify(value))
        self._metadata = {}
        self._data_fields = data.keys()


    def __setattr__(self, name, value):
        """
        Set a data attribute.
        """
        if not hasattr(self, '_data_fields'): # uninitialized
            return super().__setattr__(name, value)
        raise ReadOnlyException('Object is read only')


    def __repr__(self):
        """
        Get develop-like description of the object.
        """
        return f'Object {self}'


    def __str__(self):
        """
        Get description of the object.
        """
        return str({f: getattr(self, f) for f in self._data_fields})


    def __setitem__(self, name, attribute):
        """
        Set an item of metadata.
        """
        self._metadata[name] = attribute


    def __getitem__(self, name):
        """
        Get an item of metadata.
        """
        return self._metadata[name]


    def __iter__(self):
        """
        Get an iterator of data fields.
        """
        return iter(self._data_fields)


    @classmethod
    def item_of(cls, obj, name):
        """
        Get an item of an object.
        """
        return getattr(obj, name)


    @classmethod
    def items_of(cls, obj, fmt='list'):
        """
        Get items of an object.
        """
        if fmt == 'list':
            return [(name, getattr(obj, name)) for name in obj]
        if fmt == 'dict':
            return {name: getattr(obj, name) for name in obj}
        raise ValueError('invalid format', fmt)


def cmd_print(*args, **kwargs):
    """
    Print something to standard output.
    """
    print(*args, **kwargs)


def do_nothing(*_):
    """
    Just handle arguments of stubing functions.
    """


# For W0133, return None is OK. It's simple and friendly.
NONE = None


def function_placeholder(func):
    """
    This is a decorator. Functions decorated by this just placeholders to avoid
    some warnings like pylint-R0903 when we sure that it's worth to define
    classes.
    """
    def uncallable():
        """
        This function should not be called.
        """
        raise Exception('This function should not be called.')

    do_nothing(func)

    return uncallable


class ArgumentError(Exception):
    """
    ArgumentError describes errors caused by passing function arugment.
    """


def argument_filter(arguments, filtered_keys):
    """
    Filter some of arguments, and return filtered values by the filtered key
    order.
    """
    filtered_arguments = []
    for key in filtered_keys:
        try:
            filtered_arguments.append(arguments.pop(key))
        except KeyError as exception:
            raise ArgumentError('The argument is not given',
                                arguments, *exception.args) from exception
    return filtered_arguments


def argument_unwind(arguments, keys, restrict=True):
    """
    Convert the arguments dictionary to a list by the argument `key` order.
    """
    if restrict and len(arguments) != len(keys):
        raise ArgumentError('Arguments are not expected',
                            arguments, keys)
    try:
        return (arguments[key] for key in keys)
    except KeyError as exception:
        raise ArgumentError('The argument is not given',
                            arguments, *exception.args) from exception


_COMMAND_OPTIONS = None


def set_command_options(options):
    """
    Set command options
    """
    global _COMMAND_OPTIONS
    _COMMAND_OPTIONS = options


def get_command_options():
    """
    Get command options
    """
    return _COMMAND_OPTIONS

# vim: ts=4:sw=4:expandtab
