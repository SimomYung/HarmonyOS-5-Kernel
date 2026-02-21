#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide node classes
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from dataclasses import dataclass, field
from typing import Any, Optional, Iterable, Dict, Callable
from copy import copy
from ..utils import do_nothing, argument_filter


class TextualizedFunction():
    """
    This is an abstract class which has `textualize` function for describing
    what the `__call__` function do.
    """

    def __call__(self, **kwargs):
        """
        Call the function.
        """
        do_nothing(self, kwargs)
        raise NotImplementedError('Function `__call__` is not implemented')


    def textualize(self):
        """
        Return a text to describe the function.
        """
        do_nothing(self)
        raise NotImplementedError('Function `textualize` is not implemented')


@dataclass
class DescribedFunction(TextualizedFunction):
    """
    This is a container class to combine implementation and description.
    """

    function: Callable
    description: str

    def __str__(self):
        """
        Stringify the object.
        """
        return self.description


    def __repr__(self):
        """
        Stringify the object with develop data.
        """
        return f'{repr(self.function)}, "{self.description}"'


    def __call__(self, **kwargs):
        """
        Call the function.
        """
        return self.function(**kwargs)


    def textualize(self):
        """
        Return a text to describe the function.
        """
        return self.description


class SimpleFunction(DescribedFunction):
    """
    This class is a subclass of DescribedFunction class, and ignores iterator
    information. The template iterator usually call functions of nodes with
    itselfself. Simple functions don't care it.
    """

    def __call__(self, **kwargs):
        """
        Call the function.
        """
        argument_filter(kwargs, ['iterator'])
        return self.function(**kwargs)


@dataclass
class Node():
    """
    The node is the basic unit of a template.
    """

    type: Any
    # the node will match a object which is required by generators
    required: bool = True
    required_fn: Optional[TextualizedFunction] = None
    # the required object can be `None`
    optional: bool = False
    optional_fn: Optional[TextualizedFunction] = None
    # the default value is used when the object is required but not provided
    default: Any = None
    default_fn: Optional[TextualizedFunction] = None
    # check whether the object is valid
    valid_fn: Optional[TextualizedFunction] = None
    # standardize the form and type of the object
    standardize_fn: Optional[TextualizedFunction] = None

    def get_default_value(self, iterator):
        """
        Get the default value of this node.
        """
        if self.default_fn is not None:
            return self.default_fn(iterator=iterator)
        return copy(self.default)


    def is_required(self, iterator):
        """
        Whether the node is required.
        """
        if self.required_fn is not None:
            return self.required_fn(iterator=iterator)
        return self.required


    def is_optional(self, iterator):
        """
        Whether the node is optional.
        """
        if self.optional_fn is not None:
            return self.optional_fn(iterator=iterator)
        return self.optional


    def standardize(self, obj, iterator):
        """
        Standardize the object.
        """
        if self.standardize_fn is not None:
            return self.standardize_fn(obj=obj, iterator=iterator)
        return obj


    def is_valid(self, obj, iterator):
        """
        Whether the object is valid.
        """
        if not isinstance(obj, self.type):
            return False
        if self.valid_fn is not None:
            return self.valid_fn(obj=obj, iterator=iterator)
        return True


    def textualize(self, sep='\n', fmt='{}: {}'):
        """
        Return a text to describe the function.
        """
        return sep.join([fmt.format(key,
                                    value.textualize() if hasattr(value, 'textualize') else value)
                         for key, value in self._get_standardize_element_dir().items()
                         if value is not None])


    def _get_standardize_element_dir(self):
        """
        Get the directory of elements to standardize objects.
        """
        return {
            'type': type(self).__name__[len('Node'):], # Skip `Node` prefix
            'required': self.required if self.required_fn is None else self.required_fn,
            'default': self.default if self.default_fn is None else self.default_fn,
            'optional': self.optional if self.optional_fn is None else self.optional_fn,
            'valid': self.valid_fn,
            'standardize': self.standardize_fn,
        }


@dataclass
class NodeBasic(Node):
    """
    NodeBasic is the base class of classes which don't contain other nodes.
    """

    values: Optional[Iterable] = None

    def is_valid(self, obj, iterator):
        """
        Whether the object is valid.
        """
        if not super().is_valid(obj, iterator):
            return False
        return self.values is None or obj in self.values


    def _get_standardize_element_dir(self):
        """
        Get the directory of elements to standardize objects.
        """
        res = super()._get_standardize_element_dir()
        res['values'] = self.values
        return res


@dataclass
class NodeContainer(Node):
    """
    NodeContainer is the base class of classes which contain other nodes.
    """


@dataclass
class NodeListBase(Node):
    """
    NodeListBase is the base class of classes which contain a list of nodes.
    """

    # Note: Default value should not be used here.
    # But because we can't control the initialization order of field before 3.10,
    # we will get `TypeError: non-default argument 'element' follows default argument`.
    # we should use KW_ONLY to solve this problem when we use 3.10+.
    element: Node = field(default_factory=lambda: Node(type=type(None)))


@dataclass
class NodeBool(NodeBasic):
    """
    NodeBool is a node for a boolean value.
    """

    type: Any = bool


@dataclass
class NodeInt(NodeBasic):
    """
    NodeInt is a node for an integer value.
    """

    type: Any = int


@dataclass
class NodeFloat(NodeBasic):
    """
    NodeFloat is a node for a float value.
    """

    type: Any = float


@dataclass
class NodeStr(NodeBasic):
    """
    NodeStr is a node for a string value.
    """

    type: Any = str


@dataclass
class NodeDict(NodeContainer, Dict):
    """
    NodeDict is a node for a dictory whose fields are presupposed.
    """

    type: Any = dict


@dataclass
class NodeNamedList(NodeListBase):
    """
    NodeNamedList is a node for a dictory but like a list that every element is
    controlled by the same node. The keys are unknown when nodes are defined.
    """

    type: Any = dict


@dataclass
class NodeList(NodeListBase):
    """
    NodeList is a node for a list.
    """

    type: Any = list

# vim: ts=4:sw=4:expandtab
