#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
Description: Provide code generators
Author: Huawei OS Kernel Lab
Create: Thu Jul 12 15:22:42 2023
"""

from dataclasses import dataclass
from typing import List
from enum import Enum
from hmdsl.common.c_biast.biast import CFunction, \
    CStatement, CExpression, CIdentifierExpression, \
    CStorageQualifier, CCompoundStatement, CAttribute
from .c_common import Snippet as S, subst_code
from ..template.iterator import ObjectError
from ..utils import do_nothing, Object, get_command_options, NONE
from .c_convert import name_of, become_passed
from .code_hook_common import gen_args_call_method
from .code_hook_common import gen_expr_default_result
from .code_hook_common import gen_params_method
from .code_hook_common import gen_type_hook_result
from .code_hook_common import gen_expr_expected_result
from .code_hook_common import generation_trend


@dataclass
class SecurityModule:
    """
    Security module
    """
    name: str


@dataclass
class SecurityModuleGroup(SecurityModule):
    """
    Security module group
    """
    class Strategy(Enum):
        """
        Security module group strategy
        """
        ALL = 1
        ANY = 2
        FIRST = 3

    modules: List[SecurityModule]
    strategy: Strategy


class SecurityModuleRelationParser:
    """
    Parsering security module relation
    """
    def __init__(self):
        def cmp_ch(tgt):
            return lambda char: char == tgt

        def isalpha(char):
            return char.isalpha()

        self.stack = []
        self.result = None
        self.group_number = 0
        self.state_machine = {
            'init': [[cmp_ch('{'), lambda char: self.start_group(char, 'group')]],
            # trick: Add } to pair
            'left': [[isalpha, self.append_strategy]],
            'strategy': [
                [isalpha, self.append_strategy],
                [cmp_ch(':'), lambda _: 'colon'],
            ],
            'colon': [
                [cmp_ch('{'), self.start_group],
                # trick: Add } to pair
                [isalpha, self.append_name],
            ],
            'module': [
                [cmp_ch('{'), self.start_group],
                [isalpha, self.append_name],
                [cmp_ch(','), self.append_module],
                [cmp_ch('}'), self.append_group],
            ],
            'right': [
                [cmp_ch(','), self.append_module],
                [cmp_ch('}'), self.append_group],
            ],
            'comma': [
                [cmp_ch('{'), self.start_group],
                # trick: Add } to pair
                [isalpha, self.append_name],
            ],
        }

    def __call__(self, relation):
        """
        Parse module relation
        """
        status = 'init'
        for idx, char in enumerate(relation):
            conditions = self.state_machine.get(status, [])
            for condition in conditions:
                cond, move = condition
                if cond(char):
                    status = move(char)
                    break
            else:
                raise ValueError('Invalid charactor', idx, char, status, relation)
        if status != 'fini':
            raise ValueError('Incomplete relation', status, relation)
        result = self.result
        self.result = None
        return result

    def start_group(self, _, name=None):
        """
        Start a module group
        """
        if name is None:
            name = self.stack[-1]['modules'][-1]
            if name == '':
                # anonymous group
                name = f'ag{self.group_number}'
                self.group_number += 1
        self.stack.append({
            'strategy': '',
            'modules': [''],
            'name': name,
        })
        return 'left'

    def append_strategy(self, char):
        """
        Append a charactor into the strategy string
        """
        self.stack[-1]['strategy'] += char
        return 'strategy'

    def append_name(self, char):
        """
        Append a charactor into the name string
        """
        self.stack[-1]['modules'][-1] += char
        return 'module'

    def end_module(self):
        """
        Convert the last parsed module
        """
        modules = self.stack[-1]['modules']
        if isinstance(modules[-1], str):
            modules[-1] = SecurityModule(modules[-1])

    def append_module(self, _):
        """
        Append a module into the module list
        """
        self.end_module()
        self.stack[-1]['modules'].append('')
        return 'comma'

    def append_group(self, _):
        """
        Append a group module into the module list
        """
        self.end_module()
        stack_frame = self.stack.pop()
        strategy = SecurityModuleGroup.Strategy[stack_frame['strategy'].upper()]
        group = SecurityModuleGroup(stack_frame['name'], stack_frame['modules'], strategy)
        if len(self.stack) == 0:
            self.result = group
            return 'fini'
        self.stack[-1]['modules'][-1] = group
        return 'right'


relation_parse = SecurityModuleRelationParser()


def gen_func_call_method_list(hook, group):
    """
    static inline
    @{result_type} sec_chk_@{group_name}_@{hook_name}(@{params_call_method})
    {
        struct dlist_node *mnode = NULL;
        @{result_type} ret = @{default_result};
        dlist_for_each(mnode, &@{var_hook_point}) {
            @{type_method} *meth =
                container_of(mnode, @{type_method}, hook.node);
            ret = (meth->func)(@{args_call_method});
            if (ret != @{expected_result}) {
                break;
            }
        }
        return ret;
    }
    """
    do_nothing(hook, group)
    raise NotImplementedError('to be continued')


def gen_func_call_method_list_no_cond(hook, group):
    """
    static inline
    void sec_chk_@{group_name}_@{hook_name}(@{params_call_method})
    {
        struct dlist_node *mnode = NULL;
        dlist_for_each(mnode, &@{var_hook_point}) {
            @{type_method} *meth =
                container_of(mnode, @{type_method}, hook.node);
            (meth->func)(@{args_call_method});
        }
    }
    """
    do_nothing(hook, group)
    raise NotImplementedError('to be continued')


def gen_func_call_method_no_cond(hook, group):
    """
    static inline
    void sec_chk_@{group_name}_@{hook_name}(@{params_call_method})
    {
        sec_chk_@{module0_name}_@{hook_name}(@{args_call_method});
        sec_chk_@{module1_name}_@{hook_name}(@{args_call_method});
        ...
    }
    """
    func_list = []
    stmt_list = []
    if group.strategy != SecurityModuleGroup.Strategy.ALL:
        raise ObjectError('Only "all" strategy is valid', hook['name'], group.strategy)
    for module in group.modules:
        if isinstance(module, SecurityModuleGroup):
            raise ObjectError('Group is invalid', hook['name'], module.name)
        if not module_is_enabled(hook, module):
            continue
        stmt_list.append(subst_code(CStatement.of('''
            @func_method:expr(@args_call_method:expr);
        '''), [
            S(gen_func_method, hook, module),
            # don't use args_call_method
            S(lambda: become_passed(gen_params_method(hook))).add_name('args_call_method'),
        ]))
    body_stmt = CCompoundStatement(body=stmt_list)
    func_list.append(CFunction(
        name=name_of(gen_func_method(hook, group)),
        parameters=gen_params_method(hook),
        return_type=gen_type_hook_result(hook),
        storage_qualifiers={CStorageQualifier.STATIC},
        body=body_stmt,
    ))
    return func_list


def gen_func_call_method_cond_rollback(hook, group, condition):
    """
    static inline
    @{result_type} sec_chk_@{group_name}_@{hook_name}(@{params_call_method})
    {
        unsigned step = 0;
        @{result_type} ret = @{default_result};
        do {
            ret = sec_chk_@{module0_name}_@{hook_name}(@{args_call_method});
            if (@{condition}) {
                break;
            }
            step++;
            ret = sec_chk_@{module1_name}_@{hook_name}(@{args_call_method});
            if (@{condition}) {
                break;
            }
            step++;
            ...
        } while (0);
        if (@{condition}) {
            switch (step) {
                ...
            case 2:
                sec_chk_@{module1_name}_@{rollback_hook_name}(@{args_call_method});
                /* fallthrough */
            case 1:
                sec_chk_@{module0_name}_@{rollback_hook_name}(@{args_call_method});
                /* fallthrough */
            case 0:
                break;
            default:
                BUG("Can't rollback");
            }
        }
        return ret;
    }
    """
    do_nothing(hook, group, condition)
    raise NotImplementedError('to be continued')


def gen_func_method(hook, module):
    """
    Generate the function name of module methods
    """
    if isinstance(module, SecurityModule):
        module = module.name
    return CIdentifierExpression.of(f'sec_meth_{hook["name"]}_{module}')


def module_is_enabled(hook, module):
    """
    Return whether module is enabled or all submodules are enabled recursively
    """
    if isinstance(module, SecurityModuleGroup):
        return any(module_is_enabled(hook, sub) for sub in module.modules)
    return Object.item_of(hook.ac, module.name).enabled and \
        module.name in get_command_options().module


def gen_func_call_method_cond(hook, group, condition):
    """
    static inline
    @{result_type} sec_meth_@{hook_name}_@{group_name}(@{params_call_method})
    {
        @{result_type} ret = @{default_result};
        do {
            ret = sec_meth_@{hook_name}_@{module0_name}(@{args_call_method});
            if (@{condition}) {
                break;
            }
            ret = sec_meth_@{hook_name}_@{module1_name}(@{args_call_method});
            if (@{condition}) {
                break;
            }
            ...
        } while (0);
        return ret;
    }
    """
    func_list = []
    stmt_list = []
    need_alias = generation_trend(hook) == 'performance'
    for module in group.modules:
        if not module_is_enabled(hook, module):
            continue
        if isinstance(module, SecurityModuleGroup):
            func_list.extend(gen_func_call_method(hook, module, use_list=False))
        else:
            mod_obj = Object.item_of(hook.ac, module.name)
            if mod_obj.alias is None and need_alias:
                raise ObjectError('No alias', hook['name'], module)
        stmt_list.append(subst_code(CStatement.of('''
            ret = @func_method:expr(@args_call_method:expr);
        '''), [
            S(gen_func_method, hook, module),
            # don't use args_call_method
            S(lambda: become_passed(gen_params_method(hook))).add_name('args_call_method'),
        ]))
        stmt_list.append(subst_code(CStatement.of('''
            if (@condition:expr) {
                break;
            }
        '''), [
            S(lambda: condition).add_name('condition'),
        ]))
    if len(stmt_list) == 0:
        return []
    loop_body_stmt = CCompoundStatement(body=stmt_list)
    body_stmt = subst_code(CCompoundStatement.of('''{
        @type_hook_result:type ret;
        do @call_methods:stmt while (0);
        return ret;
    }'''), [
        S(gen_type_hook_result, hook),
        S(lambda: loop_body_stmt).add_name('call_methods'),
    ])
    func_list.append(CFunction(
        name=name_of(gen_func_method(hook, group)),
        parameters=gen_params_method(hook),
        return_type=gen_type_hook_result(hook),
        storage_qualifiers={CStorageQualifier.STATIC},
        body=body_stmt,
    ))
    return func_list


def gen_func_call_method_all(hook, group):
    """
    Generate a function which calls module methods with the `ALL` strategy
    """
    return gen_func_call_method_cond(
        hook, group, subst_code(
            CExpression.of('ret != @expr_expected_result:expr'),
            [S(gen_expr_expected_result, hook)]
        ))


def gen_func_call_method_any(hook, group):
    """
    Generate a function which calls module methods with the `ANY` strategy
    """
    return gen_func_call_method_cond(
        hook, group, subst_code(
            CExpression.of('ret == @expr_expected_result:expr'),
            [S(gen_expr_expected_result, hook)]
        ))


def gen_func_call_method_first(hook, group):
    """
    Generate a function which calls module methods with the `FIRST` strategy
    """
    func_list = []
    for module in group.modules:
        if not module_is_enabled(hook, module):
            continue
        if isinstance(module, SecurityModuleGroup):
            func_list.extend(gen_func_call_method(hook, module, use_list=False))
        first_module = module
        break
    body_stmt = subst_code(CCompoundStatement.of('''{
        return @func_method:expr(@args_call_method:expr);
    }'''), [
        S(gen_func_method, hook, first_module),
        S(lambda: become_passed(gen_params_method(hook))).add_name('args_call_method'),
    ])
    func_list.append(CFunction(
        name=name_of(gen_func_method(hook, group)),
        parameters=gen_params_method(hook),
        return_type=gen_type_hook_result(hook),
        storage_qualifiers={CStorageQualifier.STATIC},
        body=body_stmt,
    ))
    return func_list


def gen_func_call_method_unrolling(hook, group):
    """
    Generate a function which calls module methods without traversing lists
    """
    if hook.result.type == 'void':
        return gen_func_call_method_no_cond(hook, group)
    if group.strategy == SecurityModuleGroup.Strategy.ALL:
        return gen_func_call_method_all(hook, group)
    if group.strategy == SecurityModuleGroup.Strategy.ANY:
        return gen_func_call_method_any(hook, group)
    if group.strategy == SecurityModuleGroup.Strategy.FIRST:
        return gen_func_call_method_first(hook, group)
    raise NotImplementedError('Unknown strategy')


def gen_func_call_method(hook, group=None, use_list=None):
    """
    Generate a function which calls module methods
    """
    if use_list is None:
        use_list = generation_trend(hook) != 'performance'
    if group is None:
        group = relation_parse(hook.module_relation)
    if use_list:
        # need use gen_func_call_method_list
        return []
    return gen_func_call_method_unrolling(hook, group)


def gen_stmt_call_methods(hook):
    """
    Generate a statement calling the base group
    """
    module_group = relation_parse(hook.module_relation)
    if module_is_enabled(hook, module_group):
        if hook.result.type != 'void':
            code = '''ret = @func_method:expr(@args_call_method:expr);'''
        else:
            code = '''@func_method:expr(@args_call_method:expr);'''
        snippets = [
            S(gen_func_method, hook, module_group),
            S(gen_args_call_method, hook),
        ]
    else:
        if hook.result.type != 'void':
            code = '''ret = @expr_default_result:expr;'''
            snippets = [S(gen_expr_default_result, hook)]
        else:
            code = ''
            snippets = []
    return subst_code(CStatement.of(code), snippets)


def can_find_module_in(module, target_name):
    """
    Whether can find a module name in the module group or the module
    """
    if isinstance(module, SecurityModuleGroup):
        return any([can_find_module_in(submod, target_name) for submod in module.modules])
    return module.name == target_name


def gen_func_method_alias(hook, module):
    """
    Generate a function which is a method alias
    """
    module_group = relation_parse(hook.module_relation)
    mod_obj = Object.item_of(hook.ac, module)
    if not mod_obj.enabled or mod_obj.alias is None:
        return NONE
    if not can_find_module_in(module_group, module):
        raise ValueError(f'cannot find module in relation', hook['name'], module, hook.module_relation)
    return CFunction(
        name=name_of(gen_func_method(hook, module)),
        parameters=gen_params_method(hook),
        return_type=gen_type_hook_result(hook),
        attributes=[CAttribute.alias(mod_obj.alias)],
        body=None
    )

# vim: ts=4:sw=4:expandtab
