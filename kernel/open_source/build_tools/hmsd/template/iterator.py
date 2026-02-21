#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide iterators to traverse templates and objects
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import enum
from dataclasses import dataclass
from typing import Any, Optional, Union, List
from ..utils import do_nothing, function_placeholder
from .node import Node, NodeBasic, NodeListBase, NodeDict, NodeList, \
    NodeNamedList


class TemplateError(Exception):
    """
    TemplateError is an error caused by an invalid template.
    """


class ObjectError(ValueError):
    """
    ObjectError is an error caused by an invalid object.
    """


class IteratorError(Exception):
    """
    IteratorError is an error caused while iterating. The error records the
    path of traversed nodes.
    """

    def __init__(self, path, raw_exception):
        super().__init__(path, raw_exception)


    def add_prefix(self, prefix):
        """
        Add a prefix to the path
        """
        self.args = (f'{prefix}.{self.args[0]}', self.args[1])


class PathError(ValueError):
    """
    PathError is an error caused by an invalid path.
    """


NodeAttribute = enum.Enum('NodeAttribute', ('Root', 'Element'))
NodeIndex = Optional[Union[int, str, enum.Enum]]


@dataclass
class StackFrame():
    """
    StackFrame is a frame of a stack. It contains a node, the index of the node
    and some additional data which are stored in a dictory.
    """

    node: Node
    index: NodeIndex
    ext: dict


def index_to_name(index, short_name=False):
    """
    Convert an index to a string of a specific text format.
    """

    if isinstance(index, int):
        return f'#{index}'
    if isinstance(index, str):
        return index
    if isinstance(index, enum.Enum):
        if index == NodeAttribute.Root:
            return '<Root>' if not short_name else '<R>'
        if index == NodeAttribute.Element:
            return '<Element>' if not short_name else '<E>'
    raise ValueError('Unknown index', index)


class CommonTemplateIterator():
    """
    CommonTemplateIterator is the base class of iterators traversing templates.
    """

    def __init__(self, template):
        self._stack: List[Any] = []
        self._tmpl = template

    @staticmethod
    @function_placeholder
    def mask_r0903():
        """
        We define this base class instead of a function to make it easy to
        reuse common traversal processes because we can just replace a part for
        subclasses.
        """

    def traverse(self):
        """
        Traverse the template
        """
        self._traverse_init(self._tmpl.root, NodeAttribute.Root)
        self._traverse(self._tmpl.root, NodeAttribute.Root)
        self._traverse_fini(self._tmpl.root, NodeAttribute.Root)


    def _stack_push(self, node, index, **kwargs):
        """
        Push a frame to the stack.
        """
        self._stack.append(StackFrame(node, index, kwargs))


    def _stack_pop(self):
        """
        Pop a frame from the stack.
        """
        self._stack.pop()


    def _stack_top(self, number=0):
        """
        Get the n-th frame from the top.
        """
        return self._stack[-1 - number]


    def _stack_depth(self):
        """
        Get the depth of the stack.
        """
        return len(self._stack)


    def _traverse_init(self, node, index):
        """
        Stubing for the beginning of the traversal
        """
        do_nothing(self, node, index)


    def _traverse_fini(self, node, index):
        """
        Stubing for the end of the traversal
        """
        do_nothing(self, node, index)


    def _traverse_node(self, node, index):
        """
        Stubing for traversing every node
        """
        do_nothing(self, node, index)
        return False


    def _traverse_node_post(self, node, index):
        """
        Stubing for the end of the traversal of nodes
        """
        do_nothing(self, node, index)


    def _traverse_basic_node(self, node, index):
        """
        Stubing for traversing basic nodes
        """
        do_nothing(self, node, index)


    def _traverse_list_node(self, node, index):
        """
        Stubing for traversing list nodes
        """
        do_nothing(self, node, index)


    def _traverse_list_node_post(self, node, index):
        """
        Stubing for the end of the traversal of list nodes
        """
        do_nothing(self, node, index)


    def _traverse_list_element(self, node, index):
        """
        Stubing for traversing element nodes of list nodes
        """
        do_nothing(self, node, index)


    def _traverse_list_element_post(self, node, index):
        """
        Stubing for the end of the traversal of element nodes of list nodes
        """
        do_nothing(self, node, index)


    def _traverse_dict_node(self, node, index):
        """
        Stubing for traversing dictory nodes
        """
        do_nothing(self, node, index)


    def _traverse_dict_node_post(self, node, index):
        """
        Stubing for the end of the traversal of dictory nodes
        """
        do_nothing(self, node, index)


    def _traverse_dict_element(self, node, index):
        """
        Stubing for traversing element nodes of dictory nodes
        """
        do_nothing(self, node, index)


    def _traverse_dict_element_post(self, node, index):
        """
        Stubing for the end of the traversal of element nodes of dictory nodes
        """
        do_nothing(self, node, index)


    def _traverse_list_sub(self, node):
        """
        Traverse the element node of the list node
        """
        self._traverse(node.element, NodeAttribute.Element)


    def _traverse_dict_sub(self, node):
        """
        Traverse the sub-nodes of the dictory node
        """
        for index in node:
            self._traverse_dict_element(node[index], index)
            self._traverse(node[index], index)
            self._traverse_dict_element_post(node[index], index)


    def _traverse(self, node, index):
        """
        Traverse a node tree by the root node and its index
        """
        try:
            self.__traverse(node, index)
        except Exception as exception:
            if isinstance(exception, IteratorError):
                exception.add_prefix(index_to_name(index))
                raise
            raise IteratorError(index_to_name(index), exception) from exception


    def __traverse(self, node, index):
        """
        Traverse a node tree by the root node and its index. It's the core
        logic to traverse nodes.
        """
        if self._traverse_node(node, index):
            return
        if isinstance(node, NodeBasic):
            self._traverse_basic_node(node, index)
        elif isinstance(node, NodeListBase):
            self._traverse_list_node(node, index)
            self._traverse_list_sub(node)
            self._traverse_list_node_post(node, index)
        elif isinstance(node, NodeDict):
            self._traverse_dict_node(node, index)
            self._traverse_dict_sub(node)
            self._traverse_dict_node_post(node, index)
        else:
            raise TemplateError("Unknown class of node")
        self._traverse_node_post(node, index)


class TemplateIterator(CommonTemplateIterator):
    """
    TemplateIterator is the iterator for templates.
    """

    def _traverse_list_node(self, node, index):
        """
        Push the list node to the stack before traversing sub-nodes.
        """
        self._stack_push(node, index)


    def _traverse_list_node_post(self, node, index):
        """
        Pop the list node after traversing sub-nodes.
        """
        self._stack_pop()


    def _traverse_dict_node(self, node, index):
        """
        Push the dictory node to the stack before traversing sub-nodes.
        """
        self._stack_push(node, index)


    def _traverse_dict_node_post(self, node, index):
        """
        Pop the dictory node after traversing sub-nodes.
        """
        self._stack_pop()


class ObjectIterator(CommonTemplateIterator):
    """
    ObjectIterator is the iterator for objects.
    """

    def __init__(self, template, object_=None):
        super().__init__(template)
        self._object = object_


    @property
    def _top_object(self):
        """
        Get the top object.
        """
        return self._stack_top().ext['object']


    def set_object(self, object_):
        """
        Set the object which will be traversed.
        """
        self._object = object_


    def get_object(self):
        """
        Get the object.
        """
        return self._object

    def get_object_by_path(self, path):
        """
        Get an object by a path.
        """
        level = self._stack_depth() - 1

        obj, node = self._data_node_of_frame(level)
        status = 'init'
        for entry in path.split('.'):
            if len(entry) == 0:
                raise PathError('Empty entry', path)
            if entry[0] == '$':
                try:
                    obj, node, level, status = self._get_special_object_node(
                        obj, node, level, status, entry)
                except Exception as exception:
                    raise PathError('Cannot get special objects', obj, node, path) from exception
            else:
                try:
                    obj, node = self._get_sub_object_node(obj, node, entry)
                except Exception as exception:
                    raise PathError('Cannot get sub-structure', obj, node, path) from exception
                status = 'normal'
        return obj

    def _data_node_of_frame(self, level, want_obj=True):
        """
        Get data and node of a frame
        """
        frame = self._stack[level]
        data = frame.ext['object'] if want_obj else frame.index
        return (data, frame.node)

    def _get_sub_object_node(self, obj, node, entry):
        """
        Get the sub-object and the sub-node by an index
        """
        do_nothing(self)
        if isinstance(node, NodeDict):
            obj = obj[entry]
            node = node[entry]
        elif isinstance(node, NodeNamedList):
            obj = obj[entry]
            node = node.element
        elif isinstance(node, NodeList):
            obj = obj[int(entry)]
            node = node.element
        else:
            raise PathError('Node is not indexable', node, entry)
        return obj, node

    def _get_special_object_node(self, obj, node, level, status, entry):
        """
        Get the special object and node by a variable
        """
        if entry == '$r':
            if status != 'init':
                raise PathError('$r should be at the beginning')
            level = 0
            obj, node = self._data_node_of_frame(level)
            status = 'root'
        elif entry == '$p':
            if status not in ('init', 'parent'):
                raise PathError('$p should be at the beginning')
            level -= 1
            if level < 0:
                raise PathError('No parent node')
            obj, node = self._data_node_of_frame(level)
            status = 'parent'
        elif entry == '$n':
            obj, node = self._data_node_of_frame(level, want_obj=False)
            status = 'name'
        else:
            if status != 'init':
                raise PathError('$[n] should be at the beginning')
            try:
                level = int(entry[1:])
            except Exception as exception:
                raise PathError('Unknown special node', entry) from exception
            if level >= self._stack_depth():
                raise PathError('level is too high')
            obj, node = self._data_node_of_frame(level)
            status = 'level'
        return [obj, node, level, status]

    def _traverse_init(self, node, index):
        """
        Push the root node and the root object.
        """
        self._stack_push(node, index, object=self._object)


    def _traverse_fini(self, node, index):
        """
        Pop the root frame.
        """
        self._stack_pop()


    def _traverse_list_element(self, node, index):
        """
        Push the element node and the element object.
        """
        self._stack_push(node, index, object=self._top_object[index])


    def _traverse_list_element_post(self, node, index):
        """
        Pop the element frame.
        """
        self._stack_pop()


    def _traverse_dict_element(self, node, index):
        """
        Push the sub-node and the sub-object.
        """
        self._stack_push(node, index, object=self._top_object.get(index))


    def _traverse_dict_element_post(self, node, index):
        """
        Pop the element frame.
        """
        self._stack_pop()


    def _current_list_object(self):
        """
        Get a list indexes of sub-objects of the current list object.
        """
        obj = self._top_object
        if isinstance(obj, list):
            return list(range(len(obj)))
        if isinstance(obj, dict):
            return list(obj.keys())
        raise ObjectError('Object is a container', obj)


    def _traverse_list_sub(self, node):
        """
        Traverse the sub-objects of the list object.
        """
        for i in self._current_list_object():
            self._traverse_list_element(node.element, i)
            self._traverse(node.element, i)
            self._traverse_list_element_post(node.element, i)


# vim: ts=4:sw=4:expandtab
