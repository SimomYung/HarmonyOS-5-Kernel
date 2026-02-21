#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide code generators
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from hmdsl.common.c_biast.biast import CTranslationUnit, CFunction, \
    CVariableDeclaration, CParameterDeclaration, CType, CPointerType, \
    CStatement, CExpression, CIdentifierExpression, CMacroExpression, \
    CMacroStatement, CStringLiteral, CTypeDeclaration, CTypeQualifier, \
    CVoidType, CStorageQualifier, CIntType

from .generator import Generator
from .c_common import Snippet as S, subst_code, code_to_str, code_to_text, \
    placeholder_snippet
from .c_convert import become_passed, become_exported, name_of
from ..utils import Object, cmd_print, get_command_options
from ..template.iterator import ObjectError

from .code_hook_common import gen_expr_expected_result, gen_args_call_method, \
    gen_params_hook, gen_expr_default_result, gen_params_method, gen_type_cred, \
    gen_type_hook_result, generation_trend, generation_disabled
from .code_module import gen_func_call_method, gen_stmt_call_methods, gen_func_method_alias


def gen_expr_cred_initializor(hook):
    """
    Generate an expression of the initializor of the credential of the hook.
    """
    return CMacroExpression.of('SEC_CHK_CRED_XACT_RESET' if
                               hook.credential.transaction else 'SEC_CHK_CRED_RESET')


def gen_var_decl_cred(hook):
    """
    Generate a variable declaration of the credential of the hook.
    """
    if not hook.credential.enabled:
        return []
    return subst_code(CVariableDeclaration.of('''
        @type_cred:type cred = @expr_cred_initializor:expr;
    '''), [
        S(gen_type_cred, hook),
        S(gen_expr_cred_initializor, hook),
    ])



def gen_var_hook_point(hook):
    """
    Generate a variable of the hook point.
    """
    if not hook.credential.enabled:
        code = f'sec_hook_point_{hook["name"]}'
    elif hook.credential.active:
        code = f'sec_active_cred_chk_point_{hook["name"]}'
    else:
        code = f'sec_chk_point_{hook["name"]}'
    return CIdentifierExpression.of(code)


def gen_var_decl_hook_point(hook):
    """
    Generate a variable declaration of the hook point.
    """
    return CVariableDeclaration.of(subst_code('''
        struct sec_chk_point @{var_hook_point} = {
            .list = @DLIST_HEAD_INIT:expr(@{var_hook_point}.list),
        };
    ''' if not hook['seharmony_enable'] else '''
        struct sec_chk_point @{var_hook_point} = {
            .list = @DLIST_HEAD_INIT:expr(@{var_hook_point}.list),
            .overlay = @DLIST_HEAD_INIT:expr(@{var_hook_point}.overlay),
        };
    ''', [
        S(lambda: name_of(gen_var_hook_point(hook))).add_name('var_hook_point'),
    ]))


def gen_func_hook(hook, generate_overlay):
    """
    Generate a function of the hook.
    """
    if not generate_overlay:
        return CIdentifierExpression.of(f'sec_chk_{hook["name"]}' if
                                        hook.credential.enabled else f'sec_hook_{hook["name"]}')
    return CIdentifierExpression.of(f'sec_chk_overlay_{hook["name"]}' if
                                    hook.credential.enabled else f'sec_hook_overlay_{hook["name"]}')


def gen_func_decl_hook(hook, generate_overlay):
    """
    Generate a function declaration of the result of the hook.
    """
    param = gen_params_hook(hook)
    if generate_overlay:
        param = gen_params_method(hook)
    return CFunction(name=name_of(gen_func_hook(hook, generate_overlay)), parameters=param,
                     return_type=gen_type_hook_result(hook), body=None)


def gen_type_method(hook):
    """
    Generate a type of the method of the hook.
    """
    return CType.of(f'struct sec_chk_method_{hook["name"]}')


def gen_type_method_func(hook):
    """
    Generate a type of the function of the method of the hook.
    """
    if not hook.credential.enabled:
        code = f'sec_hook_{hook["name"]}_ft'
    elif hook.credential.active:
        code = f'sec_active_cred_chk_{hook["name"]}_ft'
    else:
        code = f'sec_chk_{hook["name"]}_ft'
    return CType.of(code)


def gen_type_decl_method(hook):
    """
    Generate a type declaration of the method of the hook.
    """
    return subst_code(CTypeDeclaration.of(subst_code('''
        struct @{type_method} {
            struct sec_chk_method_common hook;
            @type_method_func:type func;
        };
    ''', [
        S(lambda: name_of(gen_type_method(hook))).add_name('type_method'),
    ])), [
        S(gen_type_method_func, hook),
    ])


def gen_var_decl_ret(hook):
    """
    Generate a variable declaration of the return value of the hook.
    """
    if hook.result.type == 'void':
        return []
    return subst_code(CVariableDeclaration.of('''
        @type_res:type ret = @default_result:expr;
    '''), [
        S(gen_type_hook_result, hook).add_name('type_res'),
        S(gen_expr_default_result, hook).add_name('default_result'),
    ])


def gen_stmt_skip_mod_not_enabled(hook):
    """
    Generate a statement to skip if security module is not enabled.
    """
    enable_skip_mod = get_command_options().skip_mod
    if enable_skip_mod or hook.skip_mod_not_enabled:
        return CStatement.of('''
            if (!is_sec_mod_enabled()) {
                break;
            }
        ''')
    return []


def gen_stmt_skip_empty(hook, generate_overlay):
    """
    Generate a statement to skip the empty hook list.
    """
    return subst_code(CStatement.of('''
        if (dlist_empty(&@var_hook_point:expr.list)) {
            break;
        }
    ''' if not generate_overlay else '''
        if (dlist_empty(&@var_hook_point:expr.overlay)) {
            break;
        }
    '''), [
        S(gen_var_hook_point, hook),
    ])


def gen_stmt_skip_self_call(hook):
    """
    Generate a statement to skip if the call is from self.
    """
    if not hook.skip_self_call:
        return []
    return CStatement.of('''
        if (requester_frame_is_fromself()) {
            break;
        }
    ''')


def gen_func_cred_init(hook):
    """
    Generate a statement to skip if the call is from self.
    """
    if hook.cred_init_simple.enabled:
        return CIdentifierExpression.of('sec_chk_cred_xact_init' if
                                    hook.credential.transaction else 'sec_chk_cred_init_requester_simple')
    return CIdentifierExpression.of('sec_chk_cred_xact_init' if
                                    hook.credential.transaction else 'sec_chk_cred_init_requester')


def gen_stmt_init_cred(hook):
    """
    Generate a statement to initialize the credential.
    """
    cred = hook.credential
    if not cred.enabled or cred.active:
        return []
    return subst_code([CStatement.of('''
        ret = @func_cred_init:expr(&cred);
    '''), CStatement.of('''
        if (ret != E_HM_OK) {
            ret = E_HM_SRCH;
            break;
        }
    ''')], [
        S(gen_func_cred_init, hook),
    ])


def gen_stmt_traverse_list(hook):
    """
    Generate a statement to traverse the hook list.
    """
    for_stmt = CMacroStatement.of('''
        @dlist_for_each:stmt(mnode, &@var_hook_point:expr.list)
    ''')
    body_stmt = CStatement.of('''
        {
            @type_method:type *meth =
                @container_of:expr(mnode, @type_method:type, hook.node);
            ret = (meth->func)(@args_call_method:expr);
            if (ret != @expected_result:expr) {
                break;
            }
        }
    ''' if hook.result.type != 'void' else '''
        {
            @type_method:type *meth =
                @container_of:expr(mnode, @type_method:type, hook.node);
            (meth->func)(@args_call_method:expr);
        }
    ''')
    return subst_code([for_stmt, body_stmt], [
        S(gen_var_hook_point, hook),
        S(gen_type_method, hook),
        S(gen_args_call_method, hook),
        S(gen_expr_expected_result, hook).add_name('expected_result') if
            hook.result.type != 'void' else placeholder_snippet,
    ])


def gen_stmt_skip_log(hook):
    """
    Generate a statement to skip log generation.
    """
    if not hook.log.enabled or hook.result.type == 'void':
        return []
    return subst_code(CStatement.of('''
        if (ret == @expected_result:expr) {
            break;
        }
    '''), [
        S(gen_expr_expected_result, hook).add_name('expected_result'),
    ])


def gen_str_result_format_specifier(hook):
    """
    Generate a literal string of the format specifier of the type of the result
    of the hook.
    """
    try:
        fmt_spec = {
            'int': '%d',
            'long': '%ld',
            'ssize_t': '%z',
        }[hook.result.type]
    except KeyError as exception:
        raise ObjectError('Unknown type', *exception.args) from exception
    return CStringLiteral(fmt_spec)


def gen_str_hook_name(hook):
    """
    Generate a literal string of the name of the hook.
    """
    return CStringLiteral(hook['name'])


def gen_var_decl_src_cred(hook):
    """
    Generate a variable declaration and init the src cred of xact cred.
    """
    if not hook.log.enabled or hook.result.type == 'void':
        return []
    cred = hook.credential
    if cred.enabled and cred.transaction:
        return CVariableDeclaration.of('''
            struct sec_chk_cred *src_cred = sec_chk_src_cred(&cred);
        ''')
    return []


def gen_stmt_log(hook):
    """
    Generate a statement to generate logs.
    """
    if not hook.log.enabled or hook.result.type == 'void':
        return []
    cred = hook.credential
    fmt_spec = gen_str_result_format_specifier(hook).content
    if not cred.enabled:
        fmt = f'Access denied. (alias=%s, ret=%s({fmt_spec}))\\n'
        log = '''
            sec_hook_deny_log(NULL, @fmt:expr, @str_hook_name:expr,
                        hmstrerror((int)ret), ret);
        '''
    elif cred.transaction:
        fmt = f'Access denied. (alias=%s, forward=[0x%x->0x%x], ret=%s({fmt_spec}))\\n'
        log = '''
            sec_hook_deny_log(src_cred, @fmt:expr, @str_hook_name:expr,
                        cred._req.fwd_cnode_idx, cred._req.src_cnode_idx,
                        hmstrerror((int)ret), ret);
        '''
    else:
        fmt = f'Access denied. (alias=%s, proc=[0x%x], ret=%s({fmt_spec}))\\n'
        log = '''
            sec_hook_deny_log(&cred, @fmt:expr, @str_hook_name:expr, cred.cnode_idx,
                        hmstrerror((int)ret), ret);
        '''
    return subst_code(CStatement.of(log), [
        S(lambda: CStringLiteral(fmt)).add_name('fmt'),
        S(gen_str_hook_name, hook),
    ])


def gen_stmt_return(hook):
    """
    Generate a statement to return the result from the hook function.
    """
    if hook.result.type == 'void':
        return []
    return CStatement.of('''
        return ret;
    ''')


def gen_stmt_close_pan(hook):
    """
    Generate a statement to close pan from the hook function.
    """
    if hook['seharmony_enable'] and get_command_options().need_close_pan:
        return CStatement.of('''
            need_open = sec_close_mod_pan();
        ''')
    return []


def gen_type_hook_bool(hook):
    """
    Generate a type of the result of the hook.
    """
    return CType.of('bool')


def gen_expr_default_bool_result(hook):
    """
    Generate an expression of the default result of the hook.
    """
    return CExpression.of('false')


def gen_stmt_open_pan(hook):
    """
    Generate a statement to open pan from the hook function.
    """
    if hook['seharmony_enable'] and get_command_options().need_close_pan:
        return CStatement.of('''
            sec_open_mod_pan(need_open);
        ''')
    return []


def gen_var_need_open_pan(hook):
    """
    Generate a need_open_pan var from the hook function.
    """
    if hook['seharmony_enable'] and get_command_options().need_close_pan:
        return subst_code(CVariableDeclaration.of('''
            @type_bool:type need_open = @res_bool:expr;
         '''), [
             S(gen_type_hook_bool, hook).add_name('type_bool'),
             S(gen_expr_default_bool_result, hook).add_name('res_bool'),
             ])
    return []


def gen_func_impl_hook(hook):
    """
    Generate a function implement of the hook.
    """
    if generation_trend(hook) == 'performance':
        return gen_func_impl_hook_p(hook)
    generate_overlay = False
    func = gen_func_decl_hook(hook, generate_overlay)
    func.body = CStatement.of('''{
            struct dlist_node *mnode = NULL;
            @var_decl_cred:decl
            @var_decl_ret:decl
            do {
                @var_need_open_pan:decl
                @stmt_skip_empty:stmt
                @stmt_skip_self_call:stmt
                @stmt_init_cred:stmt
                @stmt_close_pan:stmt
                @stmt_traverse_list:stmt
                @stmt_open_pan:stmt
                @stmt_skip_log:stmt
                @var_decl_src_cred:decl
                @stmt_log:stmt
            } while (0);
            @stmt_return:stmt
        }''')
    return subst_code(func, [
        S(gen_var_decl_cred, hook),
        S(gen_var_decl_ret, hook),
        S(gen_var_need_open_pan, hook),
        S(gen_stmt_skip_empty, hook, generate_overlay),
        S(gen_stmt_skip_self_call, hook),
        S(gen_stmt_init_cred, hook),
        S(gen_stmt_close_pan, hook),
        S(gen_stmt_traverse_list, hook),
        S(gen_stmt_open_pan, hook),
        S(gen_stmt_skip_log, hook),
        S(gen_var_decl_src_cred, hook),
        S(gen_stmt_log, hook),
        S(gen_stmt_return, hook),
    ])


def gen_stmt_unused_method(hook):
    """
    Generate a statement to return an UNUSED() method.
    """
    if len(hook.arguments) == 0:
        return []
    unused_code = CStatement.of('''
        UNUSED(@args_call_method:expr);
    ''')
    return subst_code([unused_code], [
        S(gen_args_call_method, hook)
    ])


def gen_func_impl_hook_p(hook):
    """
    Generate a function implement of the hook.
    """
    func = gen_func_decl_hook(hook, False)
    func.body = subst_code(CStatement.of('''{
            @var_decl_cred:decl
            @var_decl_ret:decl
            @stmt_unused_method:stmt
            do {
                @var_need_open_pan:decl
                @stmt_skip_mod_not_enabled:stmt
                @stmt_skip_self_call:stmt
                @stmt_init_cred:stmt
                @stmt_close_pan:stmt
                @stmt_call_methods:stmt
                @stmt_open_pan:stmt
                @stmt_skip_log:stmt
                @var_decl_src_cred:decl
                @stmt_log:stmt
            } while (0);
            @stmt_return:stmt
        }'''), [
        S(gen_var_decl_cred, hook),
        S(gen_var_decl_ret, hook),
        S(gen_stmt_unused_method, hook),
        S(gen_var_need_open_pan, hook),
        S(gen_stmt_skip_mod_not_enabled, hook),
        S(gen_stmt_skip_self_call, hook),
        S(gen_stmt_init_cred, hook),
        S(gen_stmt_close_pan, hook),
        S(gen_stmt_call_methods, hook),
        S(gen_stmt_open_pan, hook),
        S(gen_stmt_skip_log, hook),
        S(gen_var_decl_src_cred, hook),
        S(gen_stmt_log, hook),
        S(gen_stmt_return, hook),
    ])
    return func


def gen_type_decl_method_func(hook):
    """
    Generate a type declaration of the method function of the hook.
    """
    return CTypeDeclaration.of(subst_code('''
        typedef int(*@{type_method_func})(@{params_method});
    ''', [
        S(lambda: name_of(gen_type_method_func(hook))).add_name('type_method_func'),
        S(lambda: ', '.join([code_to_str(p) for p in gen_params_method(hook)])).add_name('params_method'),
    ]))


def gen_type_method_func_union(hook):
    """
    Generate a type of the union of the method function and the common function
    pointer.
    """
    return CType.of(f'{gen_type_method_func(hook).name}_union')


def gen_type_decl_method_func_union(hook):
    """
    Generate a type declaration of the union of the method function of the hook.
    """
    return subst_code(CTypeDeclaration.of(subst_code('''
        typedef union {
            void *func;
            @type_method_func:type func_p;
        } @{type_method_func_union};
    ''', [
        S(lambda: name_of(gen_type_method_func_union(hook))).add_name('type_method_func_union'),
    ])), [
        S(gen_type_method_func, hook),
    ])


def gen_var_decl_mnode(hook):
    """
    Generate a variable declaration for the mnode of the hook.
    """
    if not hook.ac.seharmony.checkall:
        return []
    return CVariableDeclaration.of('''
            struct dlist_node *mnode = NULL;
                    ''')


def gen_var_decl_meth(hook):
    """
    Generate a variable of meth
    """
    if hook.ac.seharmony.checkall:
        return []
    body_decl = CVariableDeclaration.of('''
                    @type_method:type *meth =
                    @dlist_first_entry:expr(&@var_hook_point:expr.overlay, @type_method:type, hook.node);
                    ''')
    return subst_code([body_decl], [
        S(gen_type_method, hook),
        S(gen_var_hook_point, hook),
    ])


def gen_args_call_overlay_method(hook):
    """
    Generate args for call method.
    """
    params = []
    if hook.credential.enabled:
        params.append(CExpression.of('cred'))
    params.extend(become_passed(gen_params_hook(hook)))
    return params


def gen_stmt_traverse_overlay_list(hook):
    """
    Generate a statement to traverse overlay list.
    """
    dlist_stmt = CMacroStatement.of('''
        @dlist_for_each:stmt(mnode, &@var_hook_point:expr.overlay)
    ''')
    func_stmt = CStatement.of('''
        {
            @type_method:type *meth = @container_of:expr(mnode, @type_method:type, hook.node);
            ret = (meth->func)(@args_call_overlay_method:expr);
            if (ret != @expected_result:expr) {
                break;
            }
        }
    ''' if hook.result.type != 'void' else '''
        {
            @type_method:type *meth = @container_of:expr(mnode, @type_method:type, hook.node);
            (meth->func)(@args_call_overlay_method:expr);
        }
    ''')
    return subst_code([dlist_stmt, func_stmt], [
        S(gen_var_hook_point, hook),
        S(gen_type_method, hook),
        S(gen_args_call_overlay_method, hook),
        S(gen_expr_expected_result, hook).add_name('expected_result') if
            hook.result.type != 'void' else placeholder_snippet,
    ])


def gen_stmt_overlay_check(hook):
    """
    Generate a statement for overlay check function.
    """
    if hook.ac.seharmony.checkall:
        return gen_stmt_traverse_overlay_list(hook)
    func_stmt = CStatement.of('''
        ret = (meth->func)(@args_call_overlay_method:expr);
    ''' if hook.result.type != 'void' else '''
        {
            (meth->func)(@args_call_overlay_method:expr);
        }
    ''')
    return subst_code([func_stmt], [
        S(gen_var_hook_point, hook),
        S(gen_type_method, hook),
        S(gen_args_call_overlay_method, hook),
    ])


def gen_func_impl_overlay_hook(hook):
    """
    Generate a function implement of the overlay hook.
    """
    generate_overlay = True
    func = gen_func_decl_hook(hook, generate_overlay)
    func.body = CStatement.of('''{
            @var_decl_mnode:decl
            @var_decl_ret:decl

            do {
                @stmt_skip_empty:stmt
                @var_decl_meth:decl
                @stmt_overlay_check:stmt
            } while (0);

            @stmt_return:stmt
        }''')
    return subst_code(func, [
        S(gen_var_decl_ret, hook),
        S(gen_var_decl_mnode, hook),
        S(gen_stmt_skip_empty, hook, generate_overlay),
        S(gen_var_decl_meth, hook),
        S(gen_stmt_overlay_check, hook),
        S(gen_stmt_return, hook),
    ])


def gen_expr_overlay_method_number(hooks):
    """
    Generate a expression of overlay method number
    """
    return CExpression.of(f'{hooks["overlay_method_number"]}')


def gen_decl_variable_overlay_method(hooks):
    """
    Generate a variable of overlay method array.
    """
    body_decl = CVariableDeclaration.of('''
                    static struct sec_chk_method _method[@expr_overlay_method_number:expr];
                    ''')
    return subst_code([body_decl], [
        S(gen_expr_overlay_method_number, hooks),
    ])


def gen_decl_variable_overlay_method_num():
    """
    Generate a variable of overlay method number.
    """
    return CVariableDeclaration.of('''
                    unsigned int _method_num = 0u;
                    ''')


def gen_expr_method_func(hook):
    """
    Generate a expression of the function of the method of the hook.
    """
    if not hook.credential.enabled:
        code = f'sec_hook_{hook["name"]}_ft'
    elif hook.credential.active:
        code = f'sec_active_cred_chk_{hook["name"]}_ft'
    else:
        code = f'sec_chk_{hook["name"]}_ft'
    return CExpression.of(code)


def gen_stmt_overlay_check_method(hook, generate_overlay):
    """
    Generate a statement to initialize overlay check method.
    """
    meth_stmt = CStatement.of('''
        do {
            _method[_method_num].func = sec_ptr_type_cast(@expr_method_func:expr, @func_hook:expr);
            _method[_method_num].hook.head = &@var_hook_point:expr.list;
            dlist_init(&_method[_method_num].hook.node);
            _method_num++;
        } while (0);
    ''')
    return subst_code([meth_stmt], [
        S(gen_var_hook_point, hook),
        S(gen_func_hook, hook, generate_overlay),
        S(gen_expr_method_func, hook),
    ])


def gen_stmt_overlay_method(hooks):
    """
    Generate a statement for overlay component.
    """
    code = []
    generate_overlay = True
    for _, hook in Object.items_of(hooks):
        if generation_trend(hook) == 'performance':
            continue
        code += gen_stmt_overlay_check_method(hook, generate_overlay)
    return code


def gen_stmt_add_overlay_method():
    """
    Generate a statement to add overlay method.
    """
    return CStatement.of('''
               sec_add_method(_method, ARRAY_SIZE(_method), false);
           ''')


def gen_overlay_comp_func_hook(hook, operation):
    """
    Generate a overlay component function of the hook.
    """
    return CIdentifierExpression.of(f'overlay_{hook["server"]}_{hook["manager"]}_{operation}')


def gen_func_decl_overlay_comp_hook(hooks, operation):
    """
    Generate a function declaration of the overlay component of the hook.
    """
    func_type = CVoidType()
    param = []
    if operation == 'init':
        func_type = CIntType()
        env_type = CType.of('struct sec_server_env')
        env_type.add_qualifier(CTypeQualifier.CONST)
        env_type = CPointerType.create(env_type, 1)
        param += [CParameterDeclaration('env', env_type)]
    return CFunction(
        name=name_of(gen_overlay_comp_func_hook(hooks, operation)),
        parameters=param,
        return_type=func_type,
        body=None,
        storage_qualifiers={CStorageQualifier.STATIC}
    )


def gen_func_impl_overlay_comp_init_hook(hooks):
    """
    Generate a function implement of overlay componet of the hook.
    """
    operation = 'init'
    func = gen_func_decl_overlay_comp_hook(hooks, operation)
    func.body = CStatement.of('''{
            (void)env;
            @decl_variable_overlay_method_num:decl
            @stmt_overlay_method:stmt
            BUG_ON(_method_num > ARRAY_SIZE(_method));
            @stmt_add_overlay_method:stmt

            return E_HM_OK;
        }''')
    return subst_code(func, [
        S(gen_decl_variable_overlay_method_num),
        S(gen_stmt_overlay_method, hooks),
        S(gen_stmt_add_overlay_method),
    ])


def gen_stmt_del_overlay_method():
    """
    Generate a statement to delete overlay method.
    """
    return CStatement.of('''
               sec_del_method(_method, ARRAY_SIZE(_method));
           ''')


def gen_func_impl_overlay_comp_fini_hook(hooks):
    """
    Generate a function implement of overlay componet of the hook.
    """
    operation = 'fini'
    func = gen_func_decl_overlay_comp_hook(hooks, operation)
    func.body = CStatement.of('''{
            @stmt_del_overlay_method:stmt

        }''')
    return subst_code(func, [
        S(gen_stmt_del_overlay_method),
    ])


def gen_expr_hook_server(hook):
    """
    Generate an expression of the server.
    """
    return CIdentifierExpression.of(f'{hook["server"]}_{hook["manager"]}')


def gen_var_decl_overlay_component(hook):
    """
    Generate a variable of the overlay component.
    """
    if hook['overlay_method_number'] == 0:
        return CVariableDeclaration.of(subst_code('''
            struct sec_comp sec_comp_overlay_@{expr_hook_server} = {
                .mod = &sec_mod_overlay,
                .ops = {
                    .init = NULL,
                    .fini = NULL,
                },
            };
        ''', [
            S(lambda: name_of(gen_expr_hook_server(hook))).add_name('expr_hook_server'),
        ]))

    return CVariableDeclaration.of(subst_code('''
        struct sec_comp sec_comp_overlay_@{expr_hook_server} = {
            .mod = &sec_mod_overlay,
            .ops = {
                .init = overlay_@{expr_hook_server}_init,
                .fini = overlay_@{expr_hook_server}_fini,
            },
        };
    ''', [
        S(lambda: name_of(gen_expr_hook_server(hook))).add_name('expr_hook_server'),
    ]))


def gen_hooks(hooks, classified=True):
    """
    Generate hooks.
    """
    if classified:
        hook_functions = []
        hook_points = []
        hook_methods = []
        overlay_check_number = 0
        for name, hook in Object.items_of(hooks):
            hook['name'] = name
            hook['seharmony_enable'] = hooks['seharmony_enable']
            hook['server'] = hooks['server']
            if generation_disabled(hook):
                overlay_check_number += 1
                continue
            hook_points += gen_var_decl_hook_point(hook)
            hook_methods += gen_type_decl_method(hook)
            hook_functions.extend(gen_func_call_method(hook))
            hook_functions.append(gen_func_impl_hook(hook))
            if hook['seharmony_enable'] and generation_trend(hook) != 'performance':
                hook_functions.append(gen_func_impl_overlay_hook(hook))
                overlay_check_number += 1
        if hooks['seharmony_enable']:
            hooks['overlay_method_number'] = overlay_check_number
            if overlay_check_number > 0:
                hook_points += gen_decl_variable_overlay_method(hooks)
                hook_functions.append(gen_func_impl_overlay_comp_init_hook(hooks))
                hook_functions.append(gen_func_impl_overlay_comp_fini_hook(hooks))
            hook_functions += gen_var_decl_overlay_component(hooks)
        res = hook_methods + hook_points + hook_functions
    else:
        res = []
        overlay_check_number = 0
        for name, hook in Object.items_of(hooks):
            hook['name'] = name
            hook['seharmony_enable'] = hooks['seharmony_enable']
            hook['server'] = hooks['server']
            if generation_disabled(hook):
                continue
            res += gen_var_decl_hook_point(hook)
            res += gen_type_decl_method(hook)
            res.extend(gen_func_call_method(hook))
            res.append(gen_func_impl_hook(hook))
            if hook['seharmony_enable'] and generation_trend(hook) != 'performance':
                res.append(gen_func_impl_overlay_hook(hook))
                overlay_check_number += 1
        if hooks['seharmony_enable']:
            hooks['overlay_method_number'] = overlay_check_number
            res += gen_decl_variable_overlay_method(hooks)
            res.append(gen_func_impl_overlay_comp_init_hook(hooks))
            res.append(gen_func_impl_overlay_comp_fini_hook(hooks))
            res += gen_var_decl_overlay_component(hooks)
    return res


def gen_hook_api(hooks, classified=True):
    """
    Generate APIs of hooks.
    """
    if classified:
        method_functions = []
        method_function_unions = []
        hook_points = []
        hook_functions = []
        for name, hook in Object.items_of(hooks):
            hook['name'] = name
            hook['seharmony_enable'] = hooks['seharmony_enable']
            method_functions += gen_type_decl_method_func(hook)
            method_function_unions += gen_type_decl_method_func_union(hook)
            hook_points += become_exported(gen_var_decl_hook_point(hook))
            hook_functions.append(become_exported(gen_func_impl_hook(hook)))
            if hook['seharmony_enable']:
                overlay_function = gen_func_impl_overlay_hook(hook)
                if overlay_function is not None:
                    hook_functions.append(become_exported(overlay_function))
        res = method_functions + method_function_unions + hook_points + \
            hook_functions
    else:
        res = []
        for name, hook in Object.items_of(hooks):
            hook['name'] = name
            hook['seharmony_enable'] = hooks['seharmony_enable']
            res += gen_type_decl_method_func(hook)
            res += gen_type_decl_method_func_union(hook)
            res += become_exported(gen_var_decl_hook_point(hook))
            res.append(become_exported(gen_func_impl_hook(hook)))
            if hook['seharmony_enable']:
                overlay_function = gen_func_impl_overlay_hook(hook)
                if overlay_function is not None:
                    res.append(become_exported(overlay_function))
    return res


def embed_code(code):
    """
    Embed codes.
    """
    return '' if code is None else f'{code}\n'


def gen_hook_header_file(hook):
    """
    Generate the header file content of the hook.
    """
    name_u = hook.manager.upper()
    hook.hooks['seharmony_enable'] = hook['seharmony_enable']
    res = [
        f'#ifndef __HM_SEC_HOOK_{name_u}_H__\n',
        f'#define __HM_SEC_HOOK_{name_u}_H__\n',
        embed_code(hook.generation.header.include.hook),
        'struct sec_chk_cred;\n',
        'struct sec_chk_cred_xact;\n',
        embed_code(hook.generation.header.embedded_code.hook),
        CTranslationUnit(gen_hook_api(hook.hooks)),
        '#endif\n',
    ]
    return res


def gen_hook_source_file(hook):
    """
    Generate the source file content of the hook.
    """
    hook.hooks['seharmony_enable'] = hook['seharmony_enable']
    hook.hooks['server'] = hook.server
    hook.hooks['manager'] = hook.manager
    res = []
    if hook['seharmony_enable']:
        res = [
            embed_code('#include <libsec/overlay.h>'),
        ]
    res += [
        embed_code('#include <hongmeng/errno.h>'),
        embed_code('#include <libsec/log.h>'),
        embed_code('#include <libsec/cred.h>'),
        embed_code('#include <libsec/chk_cred.h>'),
        embed_code('#include <libsec/component.h>'),
        embed_code(f'#include <generated/hmsd/hook/{hook.server}/{hook.manager}.h>'),
        embed_code('#include <libmem/utils.h>'),
        embed_code(hook.generation.source.include.hook),
        embed_code(hook.generation.source.embedded_code.hook),
    ]
    if get_command_options().skip_mod:
        res += [
            embed_code('bool is_sec_mod_enabled(void);'),
        ]
    if get_command_options().trend == 'performance':
        res += [
            embed_code('#define HMSD_DECL'),
            embed_code(f'#include <generated/hmsd/module/{hook.server}/{hook.manager}.h>'),
        ]
    res += [
        CTranslationUnit(gen_hooks(hook.hooks)),
    ]
    return res


def gen_module_alias_file(hook):
    """
    Generate a file containing module aliases
    """
    header_guard = f'GENERATED_HMSD_MODULE_{hook.server}_{hook.manager}_H'.upper()
    res = [
        embed_code(f'#ifndef {header_guard}'),
        embed_code(f'#define {header_guard}'),
        embed_code(f'#include "generated/hmsd/hook/{hook.server}/{hook.manager}.h"'),
        embed_code('#ifdef HMSD_DECL'),
        embed_code('#define __attribute__(attributes)'),
        embed_code('#endif'),
    ]
    for mod in get_command_options().module:
        for _, hook_obj in Object.items_of(hook.hooks):
            if generation_trend(hook_obj) != 'performance':
                continue
            func = gen_func_method_alias(hook_obj, mod)
            if func is not None:
                condition = f'defined(ENABLE_SEC_MOD_{mod.upper()}) || defined(HMSD_DECL)'
                if hook_obj.condition.ifdef is not None:
                    for cond in hook_obj.condition.ifdef:
                        condition = f'defined({cond}) && ({condition})'
                res += [
                    embed_code(f'#if {condition}'),
                    func,
                    embed_code('#endif'),
                ]
    res += [
        embed_code('#undef __attribute__'),
        embed_code(f'#endif /* {header_guard} */'),
    ]
    return res


class CodeGenerator(Generator):
    """
    CodeGenerator generates the source files of hooks.
    """
    @staticmethod
    def register_options(option_parse):
        """
        Register command options
        """
        grp = option_parse.add_argument_group('Code generator')
        grp.add_argument('--code-module', dest='module', default=[], action='append',
                         help='enable module')
        grp.add_argument('--code-trend', dest='trend', default='auto', action='store',
                         choices=['auto', 'performance', 'size'], help='generation trend')
        grp.add_argument('--code-configs', dest='configs', default='', action='store',
                         help='enable config to generate code')
        grp.add_argument('--code-skip-mod', dest='skip_mod', default=False, action='store_true',
                         help='enable skip when mod not enabled')
        grp.add_argument('--code-need-close-pan', dest='need_close_pan', default=False, action='store_true',
                         help='need close pan when service at el1')

    @staticmethod
    def standardize_options(options):
        """
        Standardize command options
        """
        module = set()
        for mod in options.module:
            module.update(mod.split(','))
        options.module = module

    def generate(self, sec_desc):
        """
        Generate the source files of hooks.
        """
        for hook in sec_desc.hook_desc:
            self._gen_hook_source(hook)

    def _gen_hook_source(self, obj):
        """
        Generate the source file of a hook.
        """
        file_name = f'hook/{obj.server}/{obj.manager}.c'
        if self._options.dry:
            cmd_print(self.get_output_file_name(file_name))
            return
        obj['seharmony_enable'] = self._options.seharmony
        code = gen_hook_source_file(obj)
        text = code_to_text(code)
        with self.open_output_file(file_name) as file:
            file.write(text)


class HeaderGenerator(Generator):
    """
    CodeGenerator generates the header files of hooks.
    """

    def generate(self, sec_desc):
        """
        Generate the header files of hooks.
        """
        for hook in sec_desc.hook_desc:
            self._gen_hook_header(hook)
            self._gen_alias(hook)

    def _gen_hook_header(self, obj):
        """
        Generate the header file of a hook.
        """
        file_name = f'hook/{obj.server}/{obj.manager}.h'
        if self._options.dry:
            cmd_print(self.get_output_file_name(file_name))
            return
        obj['seharmony_enable'] = self._options.seharmony
        code = gen_hook_header_file(obj)
        text = code_to_text(code)
        with self.open_output_file(file_name) as file:
            file.write(text)

    def _gen_alias(self, obj):
        """
        Generate the header file of a hook.
        """
        file_name = f'module/{obj.server}/{obj.manager}.h'
        if self._options.dry:
            cmd_print(self.get_output_file_name(file_name))
            return
        code = gen_module_alias_file(obj)
        text = code_to_text(code)
        with self.open_output_file(file_name) as file:
            file.write(text)

# vim: ts=4:sw=4:expandtab
