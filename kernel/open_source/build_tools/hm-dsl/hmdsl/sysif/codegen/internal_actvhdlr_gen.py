#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Generate internal server apis
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:38:29 PM CST
"""

from __future__ import absolute_import

from typing import List, Union, Tuple, Set
from hmdsl.common.c_biast.subst import subst
from hmdsl.common.logging import get_logger
from hmdsl.sysif.model import Server, Manager, Method, Parameter, MethodType, SyscallMethod, \
    LsyscallMethod
from hmdsl.sysif.codegen.actvcall_gen import generate_arg_stksz
from hmdsl.common.c_biast.biast import CExpression, CTranslationUnit, CAttribute, \
    CClassicalAttribute, CFunction, CVariableDeclaration, CParameterDeclaration, \
    CStorageQualifier, CType, CVoidType, CSignDescriptor, CLongType, CLonglongType, CArrayType, \
    CUnionType, CIdentifierExpression, CStringLiteral, CCallExpression, CIndexDesignator, \
    CDesignation, CConstructorExpression, CIntegerConstant, CStatement, CCompoundStatement, \
    CExpressionStatement, CReturnStatement


logger = get_logger("codegen.internal_actvhdlr_gen")


def gen_internal_actvhdlr(server: Server) -> List[Union[str, CTranslationUnit]]:
    """
    Generate definitions of hdlr apis
    """
    res = []
    if server.generate_weak_handler:
        res += __mk_weak_hdlr(server)
    res += __mk_actvhdlrs(server)
    res += __mk_actvhdlr_tables(server)
    res += server.print_methods_in_arch_macros(__mk_sysif_show)
    return res


def __mk_weak_hdlr(server: Server) -> List[Union[str, CTranslationUnit]]:
    shadowhdlr_bridge: List[Union[str, CTranslationUnit]] = []
    actvhdlr_bridge: List[Union[str, CTranslationUnit]] = []
    actvhdlr: List[Union[str, CTranslationUnit]] = []
    for manager in server.managers:
        actvhdlr += __mk_weak_actvhdlrs(manager.syscalls)
        actvhdlr += manager.print_lsyscalls_in_arch_macros(__mk_weak_actvhdlrs)

        shadowhdlr_bridge += __mk_weak_bridgehdlrs(manager.syscalls)
        shadowhdlr_bridge += __mk_weak_bridgehdlrs(manager.general_lsyscalls)
        actvhdlr_bridge += __mk_weak_actvhdlrs(manager.syscalls)
        actvhdlr_bridge += __mk_weak_actvhdlrs(manager.general_lsyscalls)

        names: Set[str] = set()
        if manager.aarch64_lsyscalls:
            shadowhdlr_bridge += __mk_weak_bridgehdlrs(manager.aarch64_lsyscalls)
            names.update(set(md.name for md in manager.aarch64_lsyscalls))
            actvhdlr_bridge.append(f'#ifdef __sysif_server_{server.name}_no_compat32_handlers__\n')
            actvhdlr_bridge += __mk_weak_actvhdlrs(manager.aarch64_lsyscalls)
            actvhdlr_bridge.append("#endif\n")
        if manager.arm_lsyscalls:
            rest_arm = list(call for call in manager.arm_lsyscalls if call.name not in names)
            shadowhdlr_bridge += __mk_weak_bridgehdlrs(rest_arm)
            actvhdlr_bridge.append(f'#ifndef __sysif_server_{server.name}_no_compat32_handlers__\n')
            actvhdlr_bridge += __mk_weak_actvhdlrs(manager.arm_lsyscalls)
            actvhdlr_bridge.append("#endif\n")

    res: List[Union[str, CTranslationUnit]]
    res = ['#ifdef __ARCH_RPC_REQUIRE_BRIDGE_HDLR\n']
    # aarch64
    res += shadowhdlr_bridge
    res += actvhdlr_bridge
    res.append('#else\n')
    res += actvhdlr
    res.append('#endif\n')
    return res


def __mk_weak_hdlrs_template() -> CFunction:
    actvhdlr_template = CFunction(
        name='%fake%',
        parameters=[CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration.of("@params:decl")],
        return_type=CType.of("@return_type:type"),
        body=CCompoundStatement([CReturnStatement(CIdentifierExpression('E_HM_NOSYS'))]),
        attributes=[CClassicalAttribute('weak', None)])
    return actvhdlr_template


def __mk_weak_actvhdlrs(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    actvhdlr_template = __mk_weak_hdlrs_template()
    hdlrs = []
    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manger or server")
        parameters = list(CParameterDeclaration(p.name, CType.of(p.parameter_type))
                          for p in method.parameters)
        hdlr = subst(
            actvhdlr_template,
            {'params': parameters,
             'return_type': CType.of(method.direct_return_type)},
            False)
        hdlr.name = f'actvhdlr_{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}'
        hdlrs.append(hdlr)

    return [CTranslationUnit(list(hdlrs))]


def __mk_weak_bridgehdlrs(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    actvhdlr_template = __mk_weak_hdlrs_template()
    bridgehdlrs = []
    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manger or server")

        shadowhdlr = subst(actvhdlr_template,
                           {'params': [],
                            'return_type': CType.of(method.direct_return_type)},
                           False)
        shadowhdlr.name = \
            f'__actvshadowhdlr_{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}'
        shadowhdlr.variable_length_argument = True
        bridgehdlrs.append(shadowhdlr)

    return [CTranslationUnit(list(bridgehdlrs))]


def __mk_actvhdlrs(server: Server) -> List[Union[str, CTranslationUnit]]:
    bridge_hdlrs: List[Union[str, CTranslationUnit]] = []
    ilp32_hdlrs: List[Union[str, CTranslationUnit]] = []
    compats: List[Union[str, CTranslationUnit]] = []
    hdlrs: List[Union[str, CTranslationUnit]] = []
    for manager in server.managers:
        bridge_hdlrs += __mk_bridge_hdlr(manager.syscalls)
        bridge_hdlrs += __mk_bridge_hdlr(manager.general_lsyscalls)
        ilp32_hdlrs += __mk_bridge_hdlr_ilp32(manager.syscalls)
        ilp32_hdlrs += __mk_bridge_hdlr_ilp32(manager.general_lsyscalls)
        if manager.aarch64_lsyscalls:
            bridge_hdlrs += __mk_bridge_hdlr(manager.aarch64_lsyscalls)
            ilp32_hdlrs += __mk_bridge_hdlr_ilp32(manager.aarch64_lsyscalls)
        compats += __mk_compat_hdlr(manager.syscalls)
        compats += __mk_compat_hdlr(manager.general_lsyscalls)
        compats += __mk_compat_hdlr(manager.arm_lsyscalls)
        hdlrs += __mk_hdlr(manager.syscalls)
        hdlrs += manager.print_lsyscalls_in_arch_macros(__mk_hdlr)

    res: List[Union[str, CTranslationUnit]] = ['#ifdef __ARCH_RPC_REQUIRE_BRIDGE_HDLR\n']
    # aarch handlers
    res += bridge_hdlrs
    # compat handlers
    res.append(f'#ifndef __sysif_server_{server.name}_no_compat32_handlers__\n')
    res.append('#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__\n')
    res.append(CTranslationUnit(CVariableDeclaration.of(
        """struct compat_x3 {
                unsigned int reserverd;
                unsigned int w3;};
        """)))
    res.append('#else\n')
    res.append(CTranslationUnit(CVariableDeclaration.of(
        """struct compat_x3 {
            unsigned int w3;
            unsigned int reserverd;};
        """)))
    res.append('#endif /* CONFIG_BIG_ENDIAN */\n')
    res += compats
    res.append('#endif /* compat handlers */\n')
    # ilp32 handlers
    res.append(f'#ifndef __sysif_server_{server.name}_no_ilp32_handlers__\n')
    res += ilp32_hdlrs
    res.append('#endif\n')
    res.append('#else\n')
    res += hdlrs
    res.append('#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */\n')
    return res


def __mk_hdlr_template(ilp32: bool) -> CFunction:
    attributes = [CAttribute.of("@__opt_nonoframe:attr"), CAttribute.of("@__opt_nostkprot:attr")]
    if not ilp32:
        attributes = [CAttribute.of("@__opt_nonoframe:attr")]

    hdlr_template = CFunction(
        name='%fake',
        parameters=[CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration.of('@rpc_unused:decl'),
                    CParameterDeclaration.of('@fallback_unused:decl'),
                    CParameterDeclaration.of('@parameters:decl')],
        return_type=CType.of("@ret_type:type"),
        body=CStatement.of(
            """{
            @unused_rpc:stmt
            @unused_fallback:stmt
            @frame:decl
            @stksz:decl
            @actv:decl
            @check_decl:decl
            @check_lsys_decl:decl
            @check_lsys:stmt
            @stk_check:stmt
            @adjust_ilp32_args:stmt
            return @callee:expr(sender, credential, @append_args:expr);
            }"""),
        storage_qualifiers={CStorageQualifier.STATIC},
        attributes=attributes)
    return hdlr_template


HDLR_TEMPLATE = __mk_hdlr_template(False)
HDLR_TEMPLATE_ILP32 = __mk_hdlr_template(True)


def __adjust_ilp32_args(method_parameters: List[Parameter]) -> List[CStatement]:
    """
    if method_parameters are empty, there is no parameter which needs cast
    """
    if len(method_parameters) == 0:
        return []

    adjust_one_template = CStatement.of(
        """
        do {
            @__adjust_one_arg:stmt(@param_type:type, @param_name:expr)
        } while(0);
        """)

    ret = []
    for param in method_parameters:
        adjust_one = subst(
            adjust_one_template,
            {'param_type': CType.of(param.parameter_type),
             'param_name': CIdentifierExpression(f'{param.name}')},
            False)
        ret.append(adjust_one)
    return ret


def __lsyscall_check() -> Tuple[List[CVariableDeclaration], List[CStatement]]:
    decls = CVariableDeclaration.of(
        """__u32 src_cnode_idx = @__RPC_SENDER_DECODE_SRC_CIDX:expr(sender);
        """)
    decls += CVariableDeclaration.of(
        """__u32 fwd_cnode_idx = @__RPC_SENDER_DECODE_FWD_CIDX:expr(sender);
        """)

    lsyscall_stmts = [
        CStatement.of(
            """if(unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)){
                return E_HM_NOSYS;
            } else {
                sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
            }""")
    ]

    return (decls, lsyscall_stmts)


__BRIDGE_HDLR_FRAME = CVariableDeclaration.of(
    'unsigned long * @__maybe_unused:attr __p_prev_frame = __builtin_frame_address(0);'
    )

__BRIDGE_HDLR_STKSZ = CVariableDeclaration.of(
    'unsigned long @__maybe_unused:attr __stacksz = @__ARCH_ACTV_STACK_ARG_SIZE:expr;'
    )

__ACTV_LOCAL = CVariableDeclaration.of(
    """struct arch_actv_local * @__maybe_unused:attr __local = hm_actv_current_arch_actv_local();
    """)

__LSYS_DECL, __LSYS_STMT = __lsyscall_check()


def __get_method_info(method: Union[SyscallMethod, LsyscallMethod]) -> Tuple[str, str, List[CParameterDeclaration]]:
    if method.manager is None or method.manager.server is None:
        logger.error("method %s is invalid", method.name)
        raise ValueError(f"method {method.name} has no attached manger or server")
    func_call_postfix = f'{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}'
    is_fallback = '1' if method.method_type is MethodType.FALLBACK else '0'
    parameters = list(CParameterDeclaration(p.name, CType.of(p.parameter_type)) for p in method.parameters)
    return (func_call_postfix, is_fallback, parameters)


def __mk_bridge_hdlr(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    res = []
    for method in methods:
        func_call_postfix, is_fallback, parameters = __get_method_info(method)
        append_args = list(CExpression.of(f'@__ARCH_ACTV_RPC_RECV_ARG:expr({p.parameter_type}, {p.name})')
                           for p in method.parameters)
        (check_decl, stk_check_stmt) = __stk_check(parameters, '0', is_fallback, (False, False, False)) \
            if method.stk_check else ([], [])
        (check_decl_cap, stk_check_cap) = __stk_check(parameters, '1', is_fallback, (False, False, False)) \
            if method.stk_check else ([], [])
        fallback_unused = [
            CParameterDeclaration('__actv_fallback_unused_1', CLongType(CSignDescriptor.UNSIGNED)),
            CParameterDeclaration('__actv_fallback_unused_2', CLongType(CSignDescriptor.UNSIGNED))
        ] if method.method_type is MethodType.FALLBACK else []
        unused_fallback = CStatement.of('UNUSED(__actv_fallback_unused_1, __actv_fallback_unused_2);') \
            if method.method_type is MethodType.FALLBACK else []
        bridge_req = subst(HDLR_TEMPLATE,
                           {'rpc_unused': [], 'unused_rpc': [], 'frame': __BRIDGE_HDLR_FRAME, 'actv': __ACTV_LOCAL,
                            'stksz': __BRIDGE_HDLR_STKSZ,
                            'check_lsys_decl': __LSYS_DECL if isinstance(method, LsyscallMethod) else [],
                            'check_lsys': __LSYS_STMT if isinstance(method, LsyscallMethod) else [],
                            'check_decl': check_decl, 'stk_check': stk_check_stmt, 'fallback_unused': fallback_unused,
                            'unused_fallback': unused_fallback, 'append_args': append_args, 'parameters': parameters,
                            'ret_type': CType.of(method.direct_return_type), 'adjust_ilp32_args': [],
                            'callee': CIdentifierExpression(f'__actvshadowhdlr_{func_call_postfix}')},
                           False)
        bridge_req.name = f'__actvbridgehdlr_{func_call_postfix}'
        res.append(bridge_req)
        if isinstance(method, SyscallMethod):
            bridge_req_cap = subst(
                HDLR_TEMPLATE,
                {'rpc_unused': CParameterDeclaration.of('@__RPC_CAPCALL_HDLR_UNUSED_ARGS:decl'), 'actv': __ACTV_LOCAL,
                 'unused_rpc': CStatement.of('UNUSED(@__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT:expr);'),
                 'frame': __BRIDGE_HDLR_FRAME, 'stksz': __BRIDGE_HDLR_STKSZ, 'check_lsys_decl': [],
                 'check_decl': check_decl_cap, 'stk_check': stk_check_cap, 'check_lsys': [],
                 'fallback_unused': fallback_unused, 'unused_fallback': unused_fallback, 'append_args': append_args,
                 'parameters': parameters, 'ret_type': CType.of(method.direct_return_type), 'adjust_ilp32_args': [],
                 'callee': CIdentifierExpression(f'__actvshadowhdlr_{func_call_postfix}')},
                False)
            bridge_req_cap.name = f'__actvbridgecapcallhdlr_{func_call_postfix}'
            res.append(bridge_req_cap)
    return [CTranslationUnit(list(res))]


def __mk_bridge_hdlr_ilp32(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    res = []
    for method in methods:
        func_call_postfix, is_fallback, parameters = __get_method_info(method)
        append_args = list(CExpression.of(f'@__ARCH_ACTV_RPC_RECV_ARG:expr({p.parameter_type}, {p.name})')
                           for p in method.parameters)
        (check_decl, stk_check_stmt) = __stk_check(parameters, '0', is_fallback, (False, False, True)) \
            if method.stk_check else ([], [])
        (check_decl_cap, stk_check_cap) = __stk_check(parameters, '1', is_fallback, (False, False, True)) \
            if method.stk_check else ([], [])
        fallback_unused = [
            CParameterDeclaration('__actv_fallback_unused_1', CLongType(CSignDescriptor.UNSIGNED)),
            CParameterDeclaration('__actv_fallback_unused_2', CLongType(CSignDescriptor.UNSIGNED))
        ] if method.method_type is MethodType.FALLBACK else []
        unused_fallback = CStatement.of('UNUSED(__actv_fallback_unused_1, __actv_fallback_unused_2);') \
            if method.method_type is MethodType.FALLBACK else []
        bridge_req = subst(HDLR_TEMPLATE_ILP32,
                           {'rpc_unused': [], 'unused_rpc': [], 'frame': __BRIDGE_HDLR_FRAME, 'actv': __ACTV_LOCAL,
                            'stksz': __BRIDGE_HDLR_STKSZ,
                            'check_lsys': __LSYS_STMT if isinstance(method, LsyscallMethod) else [],
                            'check_lsys_decl': __LSYS_DECL if isinstance(method, LsyscallMethod) else [],
                            'check_decl': check_decl, 'stk_check': stk_check_stmt, 'unused_fallback': unused_fallback,
                            'fallback_unused': fallback_unused, 'append_args': append_args, 'parameters': parameters,
                            'ret_type': CType.of(method.direct_return_type),
                            'adjust_ilp32_args': __adjust_ilp32_args(method.parameters),
                            'callee': CIdentifierExpression(f'__actvshadowhdlr_{func_call_postfix}')},
                           False)
        bridge_req.name = f'__actvbridgehdlr_ilp32_{func_call_postfix}'
        res.append(bridge_req)
        if isinstance(method, SyscallMethod):
            bridge_req_cap = subst(
                HDLR_TEMPLATE_ILP32,
                {'rpc_unused': CParameterDeclaration.of('@__RPC_CAPCALL_HDLR_UNUSED_ARGS:decl'),
                 'unused_rpc': CStatement.of('UNUSED(@__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT:expr);'),
                 'frame': __BRIDGE_HDLR_FRAME, 'stksz': __BRIDGE_HDLR_STKSZ, 'check_lsys_decl': [],
                 'check_decl': check_decl_cap, 'stk_check': stk_check_cap, 'check_lsys': [],
                 'unused_fallback': unused_fallback, 'append_args': append_args, 'parameters': parameters,
                 'fallback_unused': fallback_unused, 'ret_type': CType.of(method.direct_return_type),
                 'adjust_ilp32_args': __adjust_ilp32_args(method.parameters), 'actv': __ACTV_LOCAL,
                 'callee': CIdentifierExpression(f'__actvshadowhdlr_{func_call_postfix}')},
                False)
            bridge_req_cap.name = f'__actvbridgecapcallhdlr_ilp32_{func_call_postfix}'
            res.append(bridge_req_cap)
    return [CTranslationUnit(list(res))]


def __mk_hdlr(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    res = []
    for method in methods:
        func_call_postfix, is_fallback, parameters = __get_method_info(method)
        append_args = list(CExpression.of(f'@__ARCH_ACTV_RPC_RECV_ARG:expr({p.parameter_type}, {p.name})')
                           for p in method.parameters)
        (check_decl, stk_check_stmt) = __stk_check(parameters, '0', is_fallback, (False, True, False)) \
            if method.stk_check else ([], [])
        (check_decl_cap, stk_check_cap) = __stk_check(parameters, '1', is_fallback, (False, True, False)) \
            if method.stk_check else ([], [])
        fallback_unused = [
            CParameterDeclaration('__actv_fallback_unused_1', CLongType(CSignDescriptor.UNSIGNED)),
            CParameterDeclaration('__actv_fallback_unused_2', CLongType(CSignDescriptor.UNSIGNED))
        ] if method.method_type is MethodType.FALLBACK else []
        unused_fallback = CStatement.of('UNUSED(__actv_fallback_unused_1, __actv_fallback_unused_2);') \
            if method.method_type is MethodType.FALLBACK else []
        bridge_nreq = subst(HDLR_TEMPLATE,
                            {'rpc_unused': [], 'unused_rpc': [], 'frame': [], 'stksz': [],
                             'check_lsys': __LSYS_STMT if isinstance(method, LsyscallMethod) else [],
                             'fallback_unused': fallback_unused, 'check_decl': check_decl, 'actv': __ACTV_LOCAL,
                             'stk_check': stk_check_stmt, 'parameters': parameters,
                             'check_lsys_decl': __LSYS_DECL if isinstance(method, LsyscallMethod) else [],
                             'callee': CIdentifierExpression(f'actvhdlr_{func_call_postfix}'),
                             'unused_fallback': unused_fallback, 'append_args': append_args,
                             'adjust_ilp32_args': [], 'ret_type': CType.of(method.direct_return_type)},
                            False)
        bridge_nreq.name = f'__actvbridgehdlr_{func_call_postfix}'
        res.append(bridge_nreq)
        if isinstance(method, SyscallMethod):
            bridge_nreq_cap = subst(
                HDLR_TEMPLATE,
                {'rpc_unused': CParameterDeclaration.of('@__RPC_CAPCALL_HDLR_UNUSED_ARGS:decl'), 'actv': __ACTV_LOCAL,
                 'unused_rpc': CStatement.of('UNUSED(@__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT:expr);'),
                 'frame': [], 'stksz': [], 'check_decl': check_decl_cap, 'stk_check': stk_check_cap,
                 'callee': CIdentifierExpression(f'actvhdlr_{func_call_postfix}'), 'check_lsys_decl': [],
                 'unused_fallback': unused_fallback, 'append_args': append_args, 'adjust_ilp32_args': [],
                 'ret_type': CType.of(method.direct_return_type), 'parameters': parameters, 'check_lsys': [],
                 'fallback_unused': fallback_unused},
                False)
            bridge_nreq_cap.name = f'__actvbridgecapcallhdlr_{func_call_postfix}'
            res.append(bridge_nreq_cap)
    return [CTranslationUnit(list(res))]


def __mk_compat_hdlr_template() -> CFunction:
    compat_template = CFunction(
        name='%fake%',
        parameters=[CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('sp', CLongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('x3', CLongType(CSignDescriptor.UNSIGNED))],
        return_type=CType.of("@ret_type:type"),
        body=CStatement.of(
            """{
            union @__maybe_unused:attr {
                unsigned long x3;
                struct compat_x3 __compat_x3;
            } ___val = {0};
            ___val.x3 = x3;
            unsigned int @__maybe_unused:attr ___w3 = ___val.__compat_x3.w3;
            char * data @__maybe_unused:attr = (char *)(uintptr_t)(sp);
            unsigned int @__maybe_unused:attr cursor = @rpc_unused:expr;
            bool @__maybe_unused:attr is_r3_passing = @abool:expr;
            enum __data_model @__maybe_unused:attr model = _DATA_MODEL_ILP32;
            enum __type @__maybe_unused:attr type;
            @def_args:decl
            @fetch_args:stmt
            @actv:decl
            @check_decl:decl
            @check_lsys_decl:decl
            @check_lsys:stmt
            @stk_check:stmt
            return @callee:expr(sender, credential, @args:expr);
            }"""),
        storage_qualifiers={CStorageQualifier.STATIC},
        attributes=[CAttribute.of('@__opt_nonoframe:attr')])
    return compat_template


def __mk_compat_hdlr(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    compat_template = __mk_compat_hdlr_template()
    res = []
    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manger or server")
        func_postfix = f'compat_{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}'
        return_postfix = f'{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}'
        def_args = list(CVariableDeclaration(f'_{p.name}', CType.of(p.parameter_type)) for p in method.parameters)
        for def_arg in def_args:
            def_arg.attributes = [CAttribute.of('@__maybe_unused:attr')]
        (check_decl, stk_check_stmt) = __stk_check([], 'unused', 'unused', (True, False, False)) \
            if method.stk_check else ([], [])
        if method.method_type is MethodType.FALLBACK:
            rpc_unused = CExpression.of('@__RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE:expr')
            rpc_unused_cap = CExpression.of('@__RPC_ACTVCALL_HDLR_UNUSED_ARGS_THREE:expr')
            passing_bool = CIdentifierExpression('false')
        else:
            rpc_unused = CExpression.of('@__RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE:expr')
            rpc_unused_cap = CExpression.of('@__RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE:expr')
            passing_bool = CIdentifierExpression('true')
        hdlr_compat = subst(compat_template,
                            {'def_args': def_args, 'fetch_args': __fetch(method.parameters), 'actv': __ACTV_LOCAL,
                             'check_decl': check_decl, 'stk_check': stk_check_stmt,
                             'check_lsys_decl': __LSYS_DECL if isinstance(method, LsyscallMethod) else [],
                             'args': list(CIdentifierExpression(f'_{p.name}') for p in method.parameters),
                             'rpc_unused': rpc_unused, 'abool': passing_bool,
                             'check_lsys': __LSYS_STMT if isinstance(method, LsyscallMethod) else [],
                             'callee': CIdentifierExpression(f'__actvshadowhdlr_{return_postfix}'),
                             'ret_type': CType.of(method.direct_return_type)},
                            False)
        hdlr_compat.name = f'__actvbridgehdlr_{func_postfix}'
        res.append(hdlr_compat)
        if isinstance(method, SyscallMethod):
            hdlr_cap_compat = subst(compat_template,
                                    {'def_args': def_args, 'fetch_args': __fetch(method.parameters),
                                     'check_decl': check_decl, 'stk_check': stk_check_stmt, 'check_lsys': [],
                                     'args': list(CIdentifierExpression(f'_{p.name}') for p in method.parameters),
                                     'rpc_unused': rpc_unused_cap, 'abool': CIdentifierExpression('false'),
                                     'callee': CIdentifierExpression(f'__actvshadowhdlr_{return_postfix}'),
                                     'ret_type': CType.of(method.direct_return_type), 'check_lsys_decl': [],
                                     'actv': __ACTV_LOCAL},
                                    False)
            hdlr_cap_compat.name = f'__actvbridgecapcallhdlr_{func_postfix}'
            res.append(hdlr_cap_compat)
    return [CTranslationUnit(list(res))]


def __fetch(method_parameters: List[Parameter]) -> List[CStatement]:
    fetch_template = CStatement.of(
        """
        do{
            int fetch_sz;
            unsigned long align;
            unsigned long arg_sz = @sizeof:expr(@param_type:type);
            type = @__type_of:expr(@param_type:type);
            @__fetch_one_arg:stmt((void *)(&(@param_name:expr)), data,
                        @count:expr, arg_sz, model, type, @param_type:type, @param_name:expr)
            if(fetch_sz < 0){
                panic("failed to fetch arg: %s\\n", hmstrerror(fetch_sz));
            }
            cursor += fetch_sz;
        }while(0);
        """)

    ret = []
    for i, param in enumerate(method_parameters):
        fetch = subst(
            fetch_template,
            {'param_type': CType.of(param.parameter_type),
             'param_name': CIdentifierExpression(f'_{param.name}'),
             'count': CIntegerConstant(i)},
            False)
        ret.append(fetch)

    return ret


def __stk_check(params: List[CParameterDeclaration],
                is_capcall: str, is_fallback: str, hdlr_types: Tuple[bool, bool, bool]) \
        -> Tuple[List[CVariableDeclaration], List[CStatement]]:
    decls = CVariableDeclaration.of(
        """unsigned long __nr_ul = @__RPC_INFO_DECODE_STK_BUF_NR_UL:expr(__local->rpcinfo);
        """)

    is_compat, is_bridge, is_ilp32 = hdlr_types

    if not is_compat:
        if is_bridge:
            decls += CVariableDeclaration.of(
                """unsigned long @__maybe_unused:attr __stacksz = 0ul;
                """)
        decls += CVariableDeclaration.of(
            f"""struct __arch_actv_stksz_state __stksz_state;
            """)
        stmts = generate_arg_stksz(params, is_capcall, is_fallback)
        if is_ilp32:
            stmts.append(CStatement.of(
                """if(unlikely(__nr_ul * @sizeof:expr(unsigned int) != __stacksz)){
                    return E_HM_INVAL;
                }"""))
        else:
            stmts.append(CStatement.of(
                """if(unlikely(__nr_ul * @sizeof:expr(unsigned long) != __stacksz)){
                    return E_HM_INVAL;
                }"""))

        return (decls, stmts)

    stmts = [
        CStatement.of(
            """if(unlikely(__nr_ul * @sizeof:expr(__u32) != cursor)){
            return E_HM_INVAL;
        }""")
    ]

    return (decls, stmts)


def __mk_hdlr_table_template(server: Server) -> CVariableDeclaration:
    svr = server.name
    hdlr_table_template = CVariableDeclaration(
        name='%fake%',
        type=CArrayType(
            base_type=CUnionType(name=f'{svr}_hdlrs_union'),
            capacity=CIdentifierExpression(f'__{svr}_method_MAX * __{svr}_method_TIMES')),
        initializer=CExpression.of('@inits:expr')
    )

    # Add __ro_after_init for read-only protection during initialization.
    if not server.ro_after_init_postponed:
        # Though fs and sysmgr merged, they still have their very own initialization progress.
        # Thus we use special macro for fs to protect them separatelly.
        unique_section = "_fs" if server.name == "fs" else ""
        hdlr_table_template.attributes = [CAttribute.of(f"@__ro_after_init_hdlr{unique_section}:attr")]

    return hdlr_table_template


def __mk_hdlr_table_inits(server: Server, offset: str) \
        -> List[Tuple[CDesignation, CExpression]]:
    svr = server.name
    start = offset.strip("+") if offset else "0"
    inits = [
        (CDesignation.of(f'[{start} ... {offset} __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] ='),
         CConstructorExpression.of(
             '{.p_sysif_default_handler = &sysif_actvhdlr_default_handler}')),
        (CDesignation.of(f'[{offset} __ACTV_UPCALL_POOL_GRANT_CHANGE] ='),
            CConstructorExpression.of(
             '{.p_sysif_handle_kcall_entry = &sysif_actv_pool_handle_grant_change}')),
        (CDesignation.of(
             f'[{offset} __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... {offset} __{svr}_method_MIN] ='),
         CConstructorExpression.of(
             '{.p_sysif_default_handler = &sysif_actvhdlr_default_handler}'))
    ]

    return inits


def __hdlr_type(index: int, compat: bool) -> str:
    """
    Return type of current hdlr.
    There are 4 possible cases: normal, compat, ilp32, compat + ilp32
    """
    if index == 0:
        return '__aarch64__'
    if index == 1 and compat:
        return 'compat'
    return 'ilp32'


def __construct_hdlr_table_items(server: Server, is_cap: bool, items_type: str, offset: str) -> \
        List[Tuple[CDesignation, CExpression]]:
    """ partially construct initializers of hdlr table with reserved methods and real methods """
    ret: List[Tuple[CDesignation, CExpression]] = []
    cap = 'capcall' if is_cap else ''
    # ilp32 and aarch64 share the same function type
    union_type = '_compat' if items_type == 'compat' else ''
    union_name = f'_{items_type}' if items_type in ('compat', 'ilp32') else ''
    rsvd_method_ref = CConstructorExpression.of(
        '{.p_sysif_default_handler = &sysif_actvhdlr_reserved_default_handler}')

    for manager in server.managers:
        mthds: List[Union[SyscallMethod, LsyscallMethod]] = []
        mthds = manager.syscalls + list(manager.general_lsyscalls)
        arch_rsvd = []
        if items_type in ("__aarch64__", "ilp32"):
            mthds += list(manager.aarch64_lsyscalls)
            mthds_names = set(md.name for md in mthds)
            arch_rsvd = list(filter(lambda x: x.name not in mthds_names, manager.arm_lsyscalls))
        elif items_type in ("__arm__", "compat"):
            mthds += list(manager.arm_lsyscalls)
            mthds_names = set(md.name for md in mthds)
            arch_rsvd = list(filter(lambda x: x.name not in mthds_names, manager.aarch64_lsyscalls))

        ret += list((CDesignation([CIndexDesignator(CIdentifierExpression(
            f'{offset} __{server.name}_method_{m}'))]), rsvd_method_ref)
                    for m in manager.rsvd_methods_list(with_mgr_name=True))
        for method in mthds:
            hdlr_name = f'{server.handler_prefix}_{method.manager_name}_{method.name}'
            _type = f'p__actvbridge{cap}hdlr{union_type}_{hdlr_name}'
            _name = f'__actvbridge{cap}hdlr{union_name}_{hdlr_name}'
            if isinstance(method, LsyscallMethod) and is_cap:
                _name = f'__actvbridgehdlr{union_name}_{hdlr_name}'

            ret.append((CDesignation([CIndexDesignator(CIdentifierExpression(
                f'{offset} __{server.name}_method_{method.manager_name}_{method.name}'))]),
                    CConstructorExpression.of('{%s}' % f'.{_type} = &{_name}')))
        ret += list((CDesignation([CIndexDesignator(CIdentifierExpression(
            f'{offset} __{server.name}_method_{method.manager_name}_{method.name}'))]), rsvd_method_ref)
                    for method in arch_rsvd)
    return ret


def __construct_hdlr_tables(server: Server, items_type: str, offset: str) -> \
        List[List[Tuple[CDesignation, CExpression]]]:
    """ Partialy construct initializers for actvhdlr table and actvcapcallhdlr table"""
    hdlrs = __mk_hdlr_table_inits(server, offset)
    capcall_hdlrs = __mk_hdlr_table_inits(server, offset)
    hdlrs += __construct_hdlr_table_items(server, False, items_type, offset)
    capcall_hdlrs += __construct_hdlr_table_items(server, True, items_type, offset)
    return [hdlrs, capcall_hdlrs]


def __mk_actvhdlr_table_no_bridge(server: Server) -> List[Union[str, CTranslationUnit]]:
    """ Partialy construct initializers according to data mode """
    template = __mk_hdlr_table_template(server)
    general_hdlrs, general_capcall_hdlrs = __construct_hdlr_tables(server, "", "")
    # make handlers table
    general_hdlr_table_arm = subst(template, {'inits': CConstructorExpression(general_hdlrs)}, False)
    general_hdlr_table_arm.name = f'sysif_{server.name}_actvhdlr_table'
    # make capcall handlers table
    general_capcallhdlr_table_arm = subst(template, {'inits': CConstructorExpression(general_capcall_hdlrs)}, False)
    general_capcallhdlr_table_arm.name = f'sysif_{server.name}_actvcapcallhdlr_table'
    if server.is_general():
        return [CTranslationUnit([general_hdlr_table_arm, general_capcallhdlr_table_arm])]

    hdlrs, capcall_hdlrs = __construct_hdlr_tables(server, "__aarch64__", "")
    hdlrs_arm, capcall_hdlrs_arm = __construct_hdlr_tables(server, "__arm__", "")

    ret: List[Union[str, CTranslationUnit]] = []
    ret.append("#ifdef __aarch64__\n")
    # make handlers table
    hdlr_table = subst(template, {'inits': CConstructorExpression(hdlrs)}, False)
    hdlr_table.name = f'sysif_{server.name}_actvhdlr_table'
    # make capcall handlers table
    capcallhdlr_table = subst(template, {'inits': CConstructorExpression(capcall_hdlrs)}, False)
    capcallhdlr_table.name = f'sysif_{server.name}_actvcapcallhdlr_table'
    ret.append(CTranslationUnit([hdlr_table, capcallhdlr_table]))
    ret.append("#elif defined(__arm__)\n")
    # make handlers table
    hdlr_table_arm = subst(template, {'inits': CConstructorExpression(hdlrs_arm)}, False)
    hdlr_table_arm.name = f'sysif_{server.name}_actvhdlr_table'
    # make capcall handlers table
    capcallhdlr_table_arm = subst(template, {'inits': CConstructorExpression(capcall_hdlrs_arm)}, False)
    capcallhdlr_table_arm.name = f'sysif_{server.name}_actvcapcallhdlr_table'
    ret.append(CTranslationUnit([hdlr_table_arm, capcallhdlr_table_arm]))
    ret.append("#else\n")
    ret.append(CTranslationUnit([general_hdlr_table_arm, general_capcallhdlr_table_arm]))
    ret.append("#endif\n")
    return ret


def __mk_actvhdlr_table_bridge(server: Server, compat: bool, ilp32: bool) -> List[Union[str, CTranslationUnit]]:
    """ Partialy construct initializers according to data mode """
    times = int(compat) + int(ilp32) + 1
    template = __mk_hdlr_table_template(server)
    hdlrs = []
    capcall_hdlrs = []
    for index in range(times):
        offset = f'{index} * __{server.name}_method_MAX +' if index else ""
        offset = offset.strip('1 * ')
        hdlr_type = __hdlr_type(index, compat)
        _hdlrs, _capcall_hdlrs = __construct_hdlr_tables(server, hdlr_type, offset)
        hdlrs += _hdlrs
        capcall_hdlrs += _capcall_hdlrs

    # make handlers table
    hdlr_table = subst(template, {'inits': CConstructorExpression(hdlrs)}, False)
    hdlr_table.name = f'sysif_{server.name}_actvhdlr_table'
    # make capcall handlers table
    capcallhdlr_table = subst(template, {'inits': CConstructorExpression(capcall_hdlrs)}, False)
    capcallhdlr_table.name = f'sysif_{server.name}_actvcapcallhdlr_table'
    return [CTranslationUnit([hdlr_table, capcallhdlr_table])]


def __mk_actvhdlr_tables(server: Server) -> List[Union[str, CTranslationUnit]]:
    nocompat_macro = f'defined(__sysif_server_{server.name}_no_compat32_handlers__)'
    noilp32_macro = f'defined(__sysif_server_{server.name}_no_ilp32_handlers__)'

    # arm or aarch64 only
    res: List[Union[str, CTranslationUnit]]
    res = [f'#if {nocompat_macro} && {noilp32_macro}\n']
    res += __mk_actvhdlr_table_no_bridge(server)
    res.append('#else\n')
    res.append('#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR\n')
    res.append('#error "Only support 64-bit server, build for aarch64 please!"\n')
    res.append('#endif\n')

    # aarch64 + compat
    res.append(f'#if !{nocompat_macro} && {noilp32_macro}\n')
    res += __mk_actvhdlr_table_bridge(server, True, False)
    res.append(f'#endif /* aarch64 + compat */\n')

    # aarch64 + ilp32
    res.append(f'#if {nocompat_macro} && !{noilp32_macro}\n')
    res += __mk_actvhdlr_table_bridge(server, False, True)
    res.append(f'#endif /* aarch64 + ilp32 */\n')

    res.append(f'#if !{nocompat_macro} && !{noilp32_macro}\n')
    res += __mk_actvhdlr_table_bridge(server, True, True)
    res.append('#endif /* aarch64 + compat + ilp32 */\n')
    res.append('#endif /* arm or aarch64 only */\n')

    return res


def __mk_sysif_show(methods: List[Tuple[Manager, List[Method]]]) -> List[Union[str, CTranslationUnit]]:
    server = methods[0][0].server
    if server is None:
        logger.error("manager %s is invalid", methods[0][0].name)
        raise ValueError(f"manager {methods[0][0].name} is not attached to a server")

    sysif_show = CFunction(
        name=f'sysif_show_{server.name}_methods',
        parameters=[],
        return_type=CVoidType(),
        body=CStatement.of(
            """{
            @hm_debugs:stmt
            hm_info("%s_%s, max method num: %d\\n", @name:expr, "method", @method_num:expr);
            }"""))

    hm_debugs = []
    for _, mthds in methods:
        for method in mthds:
            hm_debugs.append(CExpressionStatement(
                CCallExpression(CIdentifierExpression('hm_debug'), [
                    CStringLiteral('method is %s_%s_%s_%s, callnum is %u\\n'),
                    CStringLiteral('actvcall'),
                    CStringLiteral(server.call_prefix),
                    CStringLiteral(method.manager_name),
                    CStringLiteral(method.name),
                    CIdentifierExpression(
                        f'__{server.name}_method_{method.manager_name}_{method.name}')])))
            hm_debugs.append(CExpressionStatement(
                CCallExpression(CIdentifierExpression('hm_debug'), [
                    CStringLiteral('method is %s_%s_%s_%s, callnum is %u\\n'),
                    CStringLiteral('actvhdlr'),
                    CStringLiteral(server.handler_prefix),
                    CStringLiteral(method.manager_name),
                    CStringLiteral(method.name),
                    CIdentifierExpression(
                        f'__{server.name}_method_{method.manager_name}_{method.name}')])))

    subst(sysif_show,
          {'hm_debugs': hm_debugs,
           'name': CStringLiteral(server.name),
           'method_num': CIdentifierExpression(f'__{server.name}_method_MAX')},
          True)

    return list([CTranslationUnit([sysif_show])])
