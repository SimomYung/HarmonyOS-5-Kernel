#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: SecurityDescription class
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .template import standardize_object


class DescriptionNotFound(Exception):
    """
    DescriptionNotFound is an exception caused by invalid keys.
    """


class SecurityDescription():
    """
    A collection of objects of different template types
    """

    def __init__(self, objects):
        self._sysif = {}
        self._hook = {}
        self._obj = {}
        for file_name, obj in objects.items():
            try:
                obj = standardize_object(obj, obj['template'])
            except Exception as exception:
                raise ValueError('cannot standardize object', file_name, obj, exception) from exception
            if obj.template == 'sysif':
                self._sysif[(obj.server, obj.manager)] = obj
            elif obj.template == 'hook':
                self._hook[(obj.server, obj.manager)] = obj
            elif obj.template == 'object':
                self._obj[obj.type] = obj
            else:
                raise ValueError('Unknown template', obj.template)
            obj['raw_file_name'] = file_name
            obj['security_description'] = self


    @property
    def sysif_desc(self):
        """
        Get all of security description objects of sysif template
        """
        return self._sysif.values()


    @property
    def hook_desc(self):
        """
        Get all of security description objects of hook template
        """
        return self._hook.values()


    @property
    def object_desc(self):
        """
        Get all of security description objects of object template
        """
        return self._obj.values()


    def find_sysif_desc(self, server, manager):
        """
        Get a security description object of sysif template accroding to server
        name and manager name.
        """
        try:
            return self._sysif[(server, manager)]
        except KeyError as exception:
            raise DescriptionNotFound('Cannot find system interface description',
                                      server, manager) from exception


    def find_hook_desc(self, server, manager):
        """
        Get a security description object of hook template accroding to server
        name and manager name.
        """
        try:
            return self._hook[(server, manager)]
        except KeyError as exception:
            raise DescriptionNotFound('Cannot find hook description',
                                      server, manager) from exception


    def find_object_desc(self, obj):
        """
        Get a security description object of object template accroding to object
        name.
        """
        try:
            return self._obj[obj]
        except KeyError as exception:
            raise DescriptionNotFound('Cannot find object description',
                                      obj) from exception

# vim: ts=4:sw=4:expandtab
