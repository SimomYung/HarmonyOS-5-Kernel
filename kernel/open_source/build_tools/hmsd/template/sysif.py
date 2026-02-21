#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Sysif template
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .template import Template
from .node import NodeDict, NodeList, NodeNamedList, NodeStr, SimpleFunction
from .dependency import DependOn, SimpleDependOn


def _node_test():
    """
    Return a node which describes tests.
    """
    test = NodeDict()
    test['setup'] = NodeStr(optional=True)
    test['prepare'] = NodeStr(optional=True)
    test['code'] = NodeStr()
    test['rollback'] = NodeStr(optional=True)
    test['teardown'] = NodeStr(optional=True)
    test['expect'] = NodeDict(default={})
    test['expect']['return'] = NodeStr(optional=True)
    test['expect']['code'] = NodeStr(optional=True)
    return test


def _node_constraint():
    """
    Return a node which describes constraints.
    """
    constraint = NodeDict()
    constraint['method'] = NodeStr(
        valid_fn=SimpleFunction(
            lambda obj: obj == "logic" or obj.startswith('hook.'),
            'Be `logic` or start with `hook.`')
    )
    constraint['description'] = NodeStr(
        optional_fn=DependOn('$p.method',
            lambda dep: dep != 'logic',
            'Code logic should be described.'),
    )
    constraint['tests'] = NodeList(
        element=_node_test(),
        # if we can get the authorization of the activation pool,
        # this node should not have the default value.
        default_fn=DependOn('$r.channel.path',
            lambda dep: [] if dep is None else None,
            'If the path of the channel is none, the default value is an empty list'
        )
    )
    return constraint


def _node_sysif():
    """
    Return a node which describes a system interface.
    """
    sysif = NodeDict()
    sysif['scope'] = NodeStr(values=('inner', 'exported'))
    sysif['clients'] = NodeList(
        element=NodeStr(),
        required_fn=DependOn('$p.scope',
            lambda dep: dep == 'inner',
            'Inner interface should specify clients.')
    )
    sysif['constraints'] = NodeList(element=_node_constraint())
    return sysif


def _node_generation():
    """
    Return a node which describes some content to be inserted into
    generated code.
    """
    generation = NodeDict(default={})
    generation['test'] = NodeDict(default={})
    generation['test']['include'] = NodeStr(optional=True)
    generation['test']['embedded_code'] = NodeStr(optional=True)
    return generation


def _node_channel():
    """
    Return a node which describes the channel.
    """
    channel = NodeDict()
    channel['name'] = NodeStr(default_fn=SimpleDependOn('$r.server'))
    channel['path'] = NodeStr(optional=True)
    return channel


def _node_root():
    root = NodeDict()
    root['template'] = NodeStr(values=['sysif'])
    root['server'] = NodeStr()
    root['channel'] = _node_channel()
    root['manager'] = NodeStr()
    root['interfaces'] = NodeNamedList(element=_node_sysif())
    root['generation'] = _node_generation()
    return root


def make_sysif_template():
    """
    Make a templete of system interfaces.
    """
    return Template(_node_root())


# vim: ts=4:sw=4:expandtab
