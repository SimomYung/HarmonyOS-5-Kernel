#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Object template
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from .template import Template
from .node import NodeDict, NodeStr, NodeBool
from .dependency import DependOn, SimpleDependOn


def _node_dac_ctx():
    """
    Return a node which describes the context of DAC module.
    """
    ctx = NodeDict(required_fn=SimpleDependOn('$p.enabled'))
    ctx['name'] = NodeStr()
    ctx['uid'] = NodeStr(
        default_fn=DependOn('$p.name',
            lambda dep: f'{dep}->uid',
            '{context.name}->uid'
        )
    )
    ctx['gid'] = NodeStr(
        default_fn=DependOn('$p.name',
            lambda dep: f'{dep}->gid',
            '{context.name}->gid'
        )
    )
    ctx['mode'] = NodeStr(
        default_fn=DependOn('$p.name',
            lambda dep: f'{dep}->mode',
            '{context.name}->mode'
        )
    )
    ctx['acl'] = NodeStr(
        required_fn=SimpleDependOn('$p.$p.acl.enabled'),
        default_fn=DependOn('$p.name',
            lambda dep: f'{dep}->acl',
            '{context.name}->acl'
        )
    )
    ctx['default_acl'] = NodeStr(
        required_fn=SimpleDependOn('$p.$p.acl.default_acl'),
        default_fn=DependOn('$p.name',
            lambda dep: f'{dep}->default_acl',
            '{context.name}->default_acl'
        )
    )
    return ctx


def _node_dac_acl():
    """
    Return a node which describes the ACL extension of DAC module.
    """
    acl = NodeDict(default={})
    acl['enabled'] = NodeBool(default=False)
    acl['default_acl'] = NodeBool(
        default=False,
        required_fn=SimpleDependOn('$p.enabled')
    )
    return acl


def _node_dac():
    """
    Return a node which describes the DAC module.
    """
    dac = NodeDict()
    dac['enabled'] = NodeBool(default=True)
    dac['model'] = NodeStr(
        default='POSIX',
        values=['POSIX', 'SYSV']
    )
    dac['native'] = NodeBool(default=True)
    dac['acl'] = _node_dac_acl()
    dac['context'] = _node_dac_ctx()
    return dac


def _node_mac_ctx():
    """
    Return a node which describes the context of MAC module.
    """
    ctx = NodeDict(required_fn=SimpleDependOn('$p.enabled'))
    ctx['name'] = NodeStr()
    ctx['sid'] = NodeStr(
        default_fn=DependOn('$p.name',
            lambda dep: f'{dep}->sid',
            '{context.name}->sid'
        )
    )
    return ctx


def _node_mac():
    """
    Return a node which describes the MAC module.
    """
    mac = NodeDict()
    mac['enabled'] = NodeBool(default=True)
    mac['prefix'] = NodeStr(required_fn=SimpleDependOn('$p.enabled'))
    mac['context'] = _node_mac_ctx()
    return mac


def _node_root():
    """
    Return the root node.
    """
    root = NodeDict()
    root['template'] = NodeStr(values=['object'])
    root['type'] = NodeStr()
    root['onymous'] = NodeBool(default=True)
    root['name_length_maximum'] = NodeStr(required_fn=SimpleDependOn('$p.onymous'))
    root['mac'] = _node_mac()
    root['dac'] = _node_dac()
    return root


def make_object_template():
    """
    Make a templete of objects.
    """
    return Template(_node_root())


# vim: ts=4:sw=4:expandtab
