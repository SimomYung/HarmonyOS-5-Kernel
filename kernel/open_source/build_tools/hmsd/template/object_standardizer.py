#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Object standardizer
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .iterator import ObjectIterator, NodeAttribute, ObjectError, index_to_name
from .template import make_template
from ..utils import objectify


class ObjectStandardizer(ObjectIterator):
    """
    ObjectStandardizer standardizes objects.
    """

    def _traverse_dict_element(self, node, index):
        """
        Add the appeared field of the dictory node.
        """
        self._stack_top().ext['fields'].add(index)
        super()._traverse_dict_element(node, index)


    def _traverse_dict_node(self, node, index):
        """
        Initialize the set of appeared fields.
        """
        self._stack_top().ext['fields'] = set()


    def _traverse_dict_node_post(self, node, index):
        """
        Check unknown fields.
        """
        difference = set(self._top_object).difference(self._stack_top().ext['fields'])
        if len(difference) > 0:
            raise ObjectError('Unknown field(s)', index_to_name(index), *difference)


    def _traverse_node(self, node, index):
        """
        Standardize the top object.
        """
        obj = self._top_object
        if node.is_required(self):
            if obj is None:
                obj = node.get_default_value(self)
            if obj is None and not node.is_optional(self):
                raise ObjectError('object is empty', index_to_name(index))
        else:
            if obj is not None:
                raise ObjectError('object is unexpected', index_to_name(index))

        if obj is None:
            standardized_obj = obj
        else:
            standardized_obj = node.standardize(obj, self)

            if not node.is_valid(standardized_obj, self):
                raise ObjectError('object is invalid', index_to_name(index), obj)

        if index != NodeAttribute.Root:
            self._stack_top(1).ext['object'][index] = standardized_obj
        else:
            self._object = standardized_obj
        self._stack_top().ext['object'] = standardized_obj

        return obj is None


def standardize_object(obj, tmpl):
    """
    Standardize an object based on a template.
    """
    tmpl = make_template(tmpl)
    standardizer = ObjectStandardizer(tmpl, obj)
    standardizer.traverse()
    obj = standardizer.get_object()
    obj = objectify(obj)
    return obj


# vim: ts=4:sw=4:expandtab
