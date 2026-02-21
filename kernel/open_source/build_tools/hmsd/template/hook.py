#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Hook template
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

import copy

from .template import Template
from .node import NodeDict, NodeList, NodeNamedList, NodeStr, NodeInt, NodeBool, SimpleFunction
from .dependency import DefaultDictDependOn, DependOn, SimpleDependOn
from ..capability import Capability


def depend_on_scheme(default, special_cases):
    """
    Construct the dependency of hook schemes and a default value with some
    other values of special cases.
    """
    # depend on $r.hooks.[hook_name].scheme
    return DefaultDictDependOn('$2.scheme', default, special_cases)


def _node_embedded_code():
    """
    Return a node which describes embedded code.
    """
    code = NodeDict(default={})
    code['hook'] = NodeStr(optional=True)
    code['ac'] = NodeDict(default={})
    code['ac']['buckle'] = NodeStr(optional=True)
    code['ac']['ability'] = NodeStr(optional=True)
    code['ac']['capability'] = NodeStr(optional=True)
    code['ac']['encaps'] = NodeStr(optional=True)
    code['ac']['mac'] = NodeStr(optional=True)
    code['ac']['dac'] = NodeStr(optional=True)
    code['ac']['yama'] = NodeStr(optional=True)
    code['ac']['userns'] = NodeStr(optional=True)
    code['ac']['keyring'] = NodeStr(optional=True)
    code['ac']['audit'] = NodeStr(optional=True)
    code['ac']['misc'] = NodeStr(optional=True)
    code['ac']['dec'] = NodeStr(optional=True)
    code['event'] = NodeDict(default={})
    code['event']['audit'] = NodeStr(optional=True)
    return code


def _node_generation():
    """
    Return a node which describes some content to be inserted into
    generated code.
    """
    generation = NodeDict(default={})
    generation['header'] = NodeDict(default={})
    generation['header']['include'] = NodeDict(default={})
    generation['header']['include']['hook'] = NodeStr(optional=True)
    generation['header']['embedded_code'] = _node_embedded_code()
    generation['source'] = NodeDict(default={})
    generation['source']['include'] = NodeDict(default={})
    generation['source']['include']['hook'] = NodeStr(optional=True)
    generation['source']['embedded_code'] = _node_embedded_code()
    return generation


def _node_subhook_hook():
    """
    Return a node which describes sub-hooks of a hook.
    """
    hook = NodeDict(default={})
    hook['enabled'] = NodeBool(default=True)
    hook['call'] = NodeNamedList(
        required_fn=SimpleDependOn('$p.enabled'),
        optional=True,
        element=NodeStr()
    )
    hook['return'] = NodeStr(
        required_fn=SimpleDependOn('$p.enabled'),
    )
    hook['hook'] = NodeStr(
        default_fn=DependOn(
            ('$p.$n', '$p.$p.$p.subhook_prefix'),
            lambda dep: f'{dep[1]}{dep[0].lower()}',
            'The default name is the element name of subhooks with the '
            'subhook prefix.'
        ),
        required_fn=SimpleDependOn('$p.enabled')
    )
    return hook


def _node_subhook():
    """
    Return a node which describes a hook containing sub-hooks.
    """
    subhook = NodeDict(
        default={},
        required_fn=DependOn(
            '$2.scheme',
            lambda dep: dep.startswith('combine hooks/'),
            'Command is required if the scheme of hook is `combine/*`.'
        )
    )
    subhook['prefix'] = NodeStr(default="")
    subhook['hooks'] = NodeNamedList(element=_node_subhook_hook())

    subhook['command'] = NodeDict(
        required_fn=DependOn(
            '$2.scheme',
            lambda dep: dep == 'combine hooks/command',
            'Command is required if the scheme of hook is '
            '`combine/command`.'
        )
    )
    subhook['command']['argument'] = NodeStr()
    subhook['command']['handle_unknown_command'] = NodeStr(
        default='return errors',
        values=('return errors', 'print logs')
    )

    return subhook


def _node_hook_scheme():
    """
    Return a node which describes the scheme of a hook.
    """
    return NodeStr(values=(
        "security context/initialization",
        "security context/finalization",
        "security context/update",
        "security check/inner constraint",
        "security check/object access",
        "security check/privilege",
        "security check/watershed",
        "security event",
        "combine hooks/command",
        "combine hooks/normal",
        "other",
    ))


def _node_hook_credential():
    """
    Return a node which describes the credential of a hook.
    """
    credential = NodeDict(default={})
    credential['enabled'] = NodeBool(
        default_fn=depend_on_scheme(True, {
            "security context/finalization": False,
            "security context/update": False,
        })
    )
    credential['transaction'] = NodeBool(
        required_fn=SimpleDependOn('$p.enabled'),
        default=False
    )
    credential['active'] = NodeBool(
        required_fn=SimpleDependOn('$p.enabled'),
        default=False
    )
    return credential


def _node_hook_cred_init_simple():
    """
    Return a node which describes whether to simplify cred_init function.
    """
    credential = NodeDict(default={})
    credential['enabled'] = NodeBool(default=False)
    return credential


def _node_hook_log():
    """
    Return a node which describes the log of a hook.
    """
    log = NodeDict(default={})
    log['enabled'] = NodeBool(
        default_fn=depend_on_scheme(True, {
            "security context/finalization": False,
            "security context/update": False,
            "security event": False
        })
    )
    log['level'] = NodeStr(
        required_fn=SimpleDependOn('$p.enabled'),
        values=('error', 'info', 'warn', 'debug'),
        default='info',
    )
    return log


def _node_hook_skip_mod_not_enabled():
    """
    Return a node which describes whether to skip when security module not enabled.
    """
    return NodeBool(default=False)


def _node_hook_skip_self_call():
    """
    Return a node which describes whether to skip when sysif call is from self.
    """
    return NodeBool(
        default_fn=depend_on_scheme(False, {
            "security context/initialization": False,
            "security context/finalization": False,
            "security context/update": False,
            "security event": False
        })
    )


def _node_hook_arguments():
    """
    Return a node which describes one of arguments of a hook.
    """
    element = NodeDict()
    element['name'] = NodeStr()
    element['type'] = NodeStr()
    semantic_values = ['object', 'mode']
    element['semantic'] = NodeStr(
        values=semantic_values,
        optional=True,
        default_fn=DependOn(
            '$p.name',
            lambda dep: dep if dep in semantic_values else None,
            'If the argument name is one of valid values, the default value is the same as the name'
        )
    )
    return NodeList(
        element=element,
        default_fn=depend_on_scheme(
            None, {
                "security check/privilege": [],
                "security check/watershed": [],
                "security check/inner constraint": [],
            }
        )
    )


def _node_hook_result():
    """
    Return a node which describes the result of a hook.
    """
    result = NodeDict(default={})
    result['type'] = NodeStr(
        values=('void', 'int', 'long', 'ssize_t'),
        default_fn=depend_on_scheme('int', {
            "security event": 'void'
        })
    )
    result['default_value'] = NodeStr(
        required_fn=DependOn(
            '$p.type',
            lambda dep: dep != 'void',
            'The type of result is not `void`'
        ),
        default='E_HM_OK'
    )
    result['expected_value'] = NodeStr(
        required_fn=DependOn(
            '$p.type',
            lambda dep: dep != 'void',
            'The type of result is not `void`'
        ),
        default='E_HM_OK'
    )
    return result


def _node_hook_generation():
    """
    Return a node which describes generation strategy of a hook.
    """
    generation = NodeDict(default={})
    generation['enabled'] = NodeBool(default=True)
    generation['trend'] = NodeStr(
        required_fn=SimpleDependOn('$p.enabled'),
        values=('auto', 'performance', 'space'),
        default='auto'
    )
    generation['performance_compatibility'] = NodeBool(
        required_fn=SimpleDependOn('$p.enabled'),
        default=True
    )
    return generation


def _node_hook_condition():
    """
    Return a node which describes the condition of a hook.
    """
    condition = NodeDict(default={})
    condition['ifdef'] = NodeList(
        element=NodeStr(),
        standardize_fn=SimpleFunction(
            lambda obj: obj if isinstance(obj, list) else [obj],
            'A textual condition or a list of conditions(only support and relation)',
        ),
        optional=True
    )
    return condition


def _node_hook_object():
    """
    Return a node which describes the object accessed in a hook.
    """
    obj = NodeDict(default={})
    obj['enabled'] = NodeBool(
        default_fn=depend_on_scheme(False, {
            'security context/initialization': True,
            'security context/finalization': True,
            'security context/update': True,
            'security check/object access': True,
        })
    )
    obj['type'] = NodeStr(
        default_fn=SimpleDependOn('$r.default_object_type'),
        required_fn=SimpleDependOn('$p.enabled')
    )
    return obj


def _node_hook_code():
    """
    Return a node which describes the implement of a hook.
    """
    return NodeStr(
        optional=True,
        required_fn=DependOn(
            '$p.generation.enabled',
            lambda dep: not dep,
            'Code is required when generation is disabled.'
        )
    )


def _node_hook_rollback():
    """
    Return a node which describes the name of the function which rolls back
    the modification of this hook.
    """
    return NodeStr(
        required_fn=DependOn(
            '$p.scheme',
            lambda dep: dep == 'security context/initialization',
            'Rollback is required if the scheme of hook is '
            '`security context/initialization`'
        )
    )


def _node_hook_ac_common_test():
    """
    Return a node which describes the credentials and asserts of a test of
    an access control module.
    """
    cap_std_fn = SimpleFunction(
        lambda obj: Capability.capbitset(obj) if isinstance(obj, list) else obj,
        'A numeric capability bitmap or a list of capability names'
    )
    cap_node = NodeInt(
        optional=True,
        standardize_fn=cap_std_fn
    )

    test_case = NodeDict()
    test_case['dac'] = NodeDict(default={})
    test_case['dac']['uid'] = NodeInt(optional=True)
    test_case['dac']['gid'] = NodeInt(optional=True)
    test_case['dac']['groups'] = NodeList(element=NodeInt(), optional=True)
    test_case['mac'] = NodeDict(default={})
    test_case['mac']['label'] = NodeStr(optional=True)
    test_case['cap'] = NodeDict(default={})
    test_case['cap']['ambient_caps'] = copy.copy(cap_node)
    test_case['cap']['bounding_caps'] = copy.copy(cap_node)
    test_case['cap']['effective_caps'] = copy.copy(cap_node)
    test_case['cap']['inheritable_caps'] = copy.copy(cap_node)
    test_case['cap']['keep_caps'] = NodeBool(optional=True)
    test_case['return'] = NodeStr()
    test = NodeList(
        element=test_case,
        required_fn=DependOn(
            ['$p.enabled', '$p.code'],
            lambda dep: dep[0] and dep[1] is not None,
            'When the security module is enabled and it has custom code',
        )
    )
    return test


def _ac_common():
    """
    Return a node which describes the common part of access control modules.
    """
    common = NodeDict(default={})
    common['enabled'] = NodeBool(
        default_fn=depend_on_scheme(True, {})
    )
    common['alias'] = NodeStr(
        required_fn=SimpleDependOn('$p.enabled'),
        optional=True,
    )
    common['code'] = NodeStr(
        default=None,
        optional=True,
        required_fn=SimpleDependOn('$p.enabled')
    )
    common['tests'] = _node_hook_ac_common_test()
    common['description'] = NodeStr(
        required_fn=SimpleDependOn('$p.enabled'),
        optional_fn=DependOn('$p.code',
            lambda dep: dep is None,
            'When there is custom code, it is optional',
        )
    )
    return common


def _node_hook_ac_buckle():
    """
    Return a node which describes the buckle module.
    """
    buckle = _ac_common()
    buckle['enabled'].default_fn.special_cases = {
        "security context/initialization": False,
        "security context/finalization": False,
        "security context/update": False,
        "security check/object access": False,
        "security check/inner constraint": True,
        "security check/privilege": False,
        "security check/watershed": False,
        "security event": False,
        "combine hooks/command": False,
        "combine hooks/normal": False,
        "other": True,
    }
    buckle['permission'] = NodeList(
        element=NodeStr(),
        required_fn=SimpleDependOn('$p.enabled'),
        standardize_fn=SimpleFunction(
            lambda obj: obj if isinstance(obj, list) else [obj],
            'A textual permission or a list of permissions',
        )
    )
    return buckle


def _ac_priv():
    """
    Return a node which describes the common part of access control modules
    which check privileges.
    """
    priv = _ac_common()
    priv['enabled'].default_fn.special_cases = {
        "security context/initialization": False,
        "security context/finalization": False,
        "security context/update": False,
        "security check/object access": False,
        "security check/inner constraint": False,
        "security check/privilege": True,
        "security check/watershed": True,
        "security event": False,
        "combine hooks/command": False,
        "combine hooks/normal": False,
        "other": True,
    }
    priv['permission'] = NodeList(
        element=NodeStr(),
        required_fn=SimpleDependOn('$p.enabled'),
        standardize_fn=SimpleFunction(
            lambda obj: obj if isinstance(obj, list) else [obj],
            'A textual permission or a list of permissions',
        )
    )
    priv['relation'] = NodeStr(
        default='and',
        values=['and', 'or'],
        required_fn=SimpleDependOn('$p.enabled')
    )
    return priv


def _node_hook_ac_ability():
    """
    Return a node which describes the abilty module.
    """
    return _ac_priv()


def _node_hook_ac_capability():
    """
    Return a node which describes the POSIX capability module.
    """
    cap = _ac_priv()
    cap['permission'].element.values = Capability.names()
    return cap


def _node_hook_ac_encaps():
    """
    Return a node which describes encaps module.
    """
    encaps = _ac_common()
    encaps['enabled'].default_fn = None
    encaps['enabled'].default = False
    return encaps


def _node_hook_ac_misc():
    """
    Return a node which describes the misc module
    """
    return  _ac_common()


def _ac_obj(special_cases):
    """
    Return a node which describes the common part of access control modules
    which check access of objects.
    """
    obj = _ac_common()
    obj['enabled'].default_fn.special_cases = {
        "security context/initialization": True,
        "security context/finalization": True,
        "security context/update": True,
        "security check/object access": True,
        "security check/inner constraint": False,
        "security check/privilege": False,
        "security check/watershed": False,
        "security event": False,
        "combine hooks/command": False,
        "combine hooks/normal": False,
        "other": True,
    }
    obj['permission'] = NodeList(
        element=NodeStr(),
        standardize_fn=SimpleFunction(
            lambda obj: obj if isinstance(obj, list) else [obj],
            'A textual permission or a list of permissions',
        ),
        optional=True,
        required_fn=DependOn(
            ['$p.enabled', '$2.scheme'],
            lambda dep: dep[0] and dep[1] in special_cases,
            'When the obj is enabled and the scheme is security check/object access',
        )
    )
    return obj


def _node_hook_ac_mac():
    """
    Return a node which describes the MAC module.
    """
    special_cases = ['security context/initialization', 'security check/object access']
    mac = _ac_obj(special_cases)
    return mac


def _node_hook_ac_dac():
    """
    Return a node which describes the DAC module.
    """
    special_cases = ['security check/object access']
    dac = _ac_obj(special_cases)
    dac['permission'].element.values = ['read', 'write', 'execute']
    dac['override'] = NodeStr(
        optional=True,
        values=Capability.names()
    )
    return dac


def _node_hook_ac_seharmony():
    """
    Return a node which describes the SEHarmony module.
    """
    special_cases = ['security context/initialization', 'security check/object access']
    seharmony = _ac_obj(special_cases)
    seharmony['checkall'] = NodeBool(
        default_fn=depend_on_scheme(False, {
            "security context/initialization": True,
            "security context/finalization": True,
            "security context/update": True,
        })
    )
    return seharmony


def _node_hook_ac_acl():
    """
    Return a node which describes the ACL module.
    """
    acl = _ac_common()
    acl['enabled'].default_fn = None
    acl['enabled'].default = False
    return acl


def _node_hook_ac_dec():
    """
    Return a node which describes the ACL module.
    """
    dec = _ac_common()
    dec['enabled'].default_fn = None
    dec['enabled'].default = False
    return dec


def _node_hook_ac_userns():
    """
    Return a node which describes the User Namespace module.
    """
    userns = _ac_common()
    userns['enabled'].default_fn = None
    userns['enabled'].default = False
    return userns


def _node_hook_ac_yama():
    """
    Return a node which describes the YAMA module.
    """
    yama = _ac_common()
    yama['enabled'].default_fn = None
    yama['enabled'].default = False
    return yama


def _node_hook_ac_keyring():
    """
    Return a node which describes the KEYRING module.
    """
    keyring = _ac_common()
    keyring['enabled'].default_fn = None
    keyring['enabled'].default = False
    return keyring


def _node_hook_ac_audit():
    """
    Return a node which describes the AUDIT module.
    """
    audit = _ac_common()
    audit['enabled'].default_fn = None
    audit['enabled'].default = False
    return audit


def _node_hook_ac():
    """
    Return a node which describes access control of a hook.
    """
    access_control = NodeDict(default={})
    access_control['buckle'] = _node_hook_ac_buckle()
    access_control['ability'] = _node_hook_ac_ability()
    access_control['capability'] = _node_hook_ac_capability()
    access_control['encaps'] = _node_hook_ac_encaps()
    access_control['mac'] = _node_hook_ac_mac()
    access_control['dac'] = _node_hook_ac_dac()
    access_control['seharmony'] = _node_hook_ac_seharmony()
    access_control['acl'] = _node_hook_ac_acl()
    access_control['userns'] = _node_hook_ac_userns()
    access_control['yama'] = _node_hook_ac_yama()
    access_control['keyring'] = _node_hook_ac_keyring()
    access_control['audit'] = _node_hook_ac_audit()
    access_control['misc'] = _node_hook_ac_misc()
    access_control['dec'] = _node_hook_ac_dec()
    return access_control


def _node_hook_event_audit():
    """
    Return a node which describes the audit function of a hook.
    """
    audit = NodeDict(default={})
    return audit


def _node_hook_event():
    """
    Return a node which describes response methods of a security event of
    a hook.
    """
    event = NodeDict(default={})
    event['audit'] = _node_hook_event_audit()
    return event


def _node_hook_module_relation():
    mod_rlt = NodeStr(default_fn=depend_on_scheme(
        '{all:dac,mac,capability,ability,seharmony}', {
            "security context/initialization": '{all:dac,dec,mac,capability,encaps,ability,seharmony,acl,userns,yama,keyring,audit}',
            "security context/finalization": '{all:dac,dec,mac,capability,encaps,ability,seharmony,acl,userns,yama,keyring,audit}',
            "security context/update": '{all:dac,dec,mac,capability,encaps,ability,seharmony,acl,userns,yama,keyring,audit}',
            'security check/inner constraint': '{all:buckle}',
            'security check/object access': '{all:dac,overlay{first:seharmony,mac}}',
            'security check/privilege': '{all:capability,overlay{first:seharmony,ability}}',
            'security check/watershed': '{all:capability,overlay{first:seharmony,ability}}',
        }
    ))
    return mod_rlt


def _node_hook():
    """
    Return a node which describes a hook.
    """
    hook = NodeDict()
    hook['scheme'] = _node_hook_scheme()
    hook['credential'] = _node_hook_credential()
    hook['cred_init_simple'] = _node_hook_cred_init_simple()
    hook['log'] = _node_hook_log()
    hook['skip_mod_not_enabled'] = _node_hook_skip_mod_not_enabled()
    hook['skip_self_call'] = _node_hook_skip_self_call()
    hook['arguments'] = _node_hook_arguments()
    hook['module_relation'] = _node_hook_module_relation()
    hook['result'] = _node_hook_result()
    hook['generation'] = _node_hook_generation()
    hook['description'] = NodeStr(optional=True)
    hook['code'] = _node_hook_code()
    hook['condition'] = _node_hook_condition()
    hook['object'] = _node_hook_object()
    hook['rollback'] = _node_hook_rollback()
    hook['subhook'] = _node_subhook()
    hook['ac'] = _node_hook_ac()
    hook['event'] = _node_hook_event()
    return hook


def _node_cond_mod():
    """
    Return a node which describes conditions of security modules.
    """
    mod = NodeDict(default={})
    mod['dac'] = NodeStr(default="")
    mod['mac'] = NodeStr(default="")
    mod['capability'] = NodeStr(default="")
    return mod


def _node_root():
    """
    Return the root node.
    """
    root = NodeDict()
    root['template'] = NodeStr(values=['hook'])
    root['server'] = NodeStr()
    root['manager'] = NodeStr()
    root['default_object_type'] = NodeStr(optional=True)
    root['condition'] = NodeDict(default={})
    root['condition']['ifdef'] = NodeList(
                                    element=NodeStr(),
                                    standardize_fn=SimpleFunction(
                                        lambda obj: obj if isinstance(obj, list) else [obj],
                                        'A textual condition or a list of conditions(only support and relation)',
                                    ),
                                    optional=True
                                )
    root['condition']['module'] = _node_cond_mod()
    root['hooks'] = NodeNamedList(element=_node_hook())
    root['generation'] = _node_generation()
    return root


def make_hook_template():
    """
    Make a templete of hooks.
    """
    return Template(_node_root())


# vim: ts=4:sw=4:expandtab
