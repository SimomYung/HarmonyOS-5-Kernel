#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Generate selfcall for server apis
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:33:17 PM CST
"""

from __future__ import absolute_import

from dataclasses import dataclass, field
from typing import List, Union

from hmdsl.common.c_biast.subst import subst
from hmdsl.common.logging import get_logger
from hmdsl.sysif.model import Server, MethodType, LsyscallMethod, LsyscallType, SyscallMethod
from hmdsl.common.c_biast.biast import CExpression, CFunction, CParameterDeclaration, CType, CPointerType, \
    CAliasType, CVoidType, CSignDescriptor, CLongType, CLonglongType, CIdentifierExpression, CStatement, \
    CTranslationUnit, CVariableDeclaration, CAttribute


logger = get_logger("codegen.actv_selfcall_gen")


@dataclass
class BufferDescription:
    """
    To describe the buffer of different types sysif methods
    """

    # the parameter declarations required to describe the buffer
    decls: List[CParameterDeclaration] = field(default_factory=list)

    # The expression where we refer to the buffer, NULL when the buffer
    # is unavailable. Sometimes buf_expr can be nullable even if it IS
    # declared. Don't know why.
    buf_expr: CExpression = field(default_factory=CExpression.null)

    # Size of the buffer. 0 if it is unavailable.
    buf_sz_expr: CExpression = field(default_factory=lambda: CExpression.constant_of('0ul'))


def __gen_sendbuf(method: SyscallMethod) -> BufferDescription:
    desc = BufferDescription()
    sendbuf_name = "__buf"

    if method.method_type is not MethodType.RETBUF:
        desc.decls.append(CParameterDeclaration.constant_of("@__maybe_unused:attr void *" + sendbuf_name))

    if method.method_type in (MethodType.FALLBACK, MethodType.BUF):
        desc.buf_expr = CIdentifierExpression(sendbuf_name)
        desc.buf_sz_expr = CExpression.constant_of(
            "@__RPC_INFO_DECODE_MEM_BUF_LEN:expr((unsigned long)__rpc_info)")

    return desc


def __gen_rplybuf(method: SyscallMethod) -> BufferDescription:
    desc = BufferDescription()
    rplybuf_name = "__rply_buf"

    if method.indirect_returned_values is not None:
        # RETBUF/FALLBACK
        desc.decls.append(CParameterDeclaration(rplybuf_name, CPointerType(CVoidType())))
        desc.buf_expr = CIdentifierExpression(rplybuf_name)

        if method.method_type is MethodType.FALLBACK:
            desc.decls.append(CParameterDeclaration(f"rply_buf_sz",
                                                    CLongType(CSignDescriptor.UNSIGNED)))
            desc.buf_sz_expr = CIdentifierExpression(f"rply_buf_sz")
        else:
            desc.buf_sz_expr = CExpression.constant_of(
                "@__RPC_INFO_DECODE_MEM_BUF_LEN:expr((unsigned long)__rpc_info)")

    return desc


def gen_selfcall(server: Server, override: bool) -> List[Union[str, CTranslationUnit]]:
    """
    Generate self call for server sysif apis
    """
    selfcalls: List[Union[str, CTranslationUnit]] = []
    syscalls: List[CFunction] = []
    lsyscalls: List[Union[str, CTranslationUnit]] = []
    for manager in server.managers:
        for method in manager.syscalls:
            if override:
                syscalls += __mk_selfcall(method)
            elif method.support_self_call:
                syscalls += __mk_selfcall(method)
        lsyscalls += manager.print_lsyscalls_in_arch_macros(__mk_lsys_selfcall)

    selfcalls = [CTranslationUnit(list(syscalls))]
    selfcalls += lsyscalls

    return selfcalls


def __mk_lsys_selfcall(lsyscalls: List[LsyscallMethod]) -> List[CTranslationUnit]:
    valid_calls = list(call for call in lsyscalls if call.lsyscall_type is LsyscallType.NATIVEWITHSELFCALL)
    funcs: List[CFunction] = []
    var_decl = CVariableDeclaration("unused_callnum", CLongType(), initializer=CIdentifierExpression("syscall_num"))
    var_decl.attributes = [CAttribute.constant_of("@__maybe_unused:attr")]
    template = CFunction(
        name='%fake%',
        parameters=[CParameterDeclaration(name="syscall_num", type=CLongType()),
                    CParameterDeclaration.of("""@parameters:decl""")],
        return_type=CType.of("@return_type:type"),
        body=CStatement.of(
            """{
            @callnum_decl:decl
            struct sysif_actv_selfcall selfcall = {
                NULL,
                0UL,
                NULL,
                0UL,
                __RPC_INFO_ENCODE_XACT(false, true, 0, 0, 0)
            };
            return @callee:expr((unsigned long long)(uintptr_t)(& selfcall), @__ACTV_CRED_SELFCALL:expr, @args:expr);
            }"""))
    for method in valid_calls:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manager or server")
        callee = CIdentifierExpression(
            f"actvhdlr_{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}")
        return_type = CType.constant_of(method.direct_return_type)
        parameters = list(CParameterDeclaration(param.name, CType.constant_of(param.parameter_type))
                          for param in method.parameters)
        args = list(CIdentifierExpression(param.name) for param in method.parameters)
        lsyscall = subst(template,
                         {'callnum_decl': var_decl, 'return_type': return_type, 'callee': callee,
                          'parameters': parameters, 'args': args},
                         False)
        lsyscall.name = f"__lsyscall_{method.manager_name}_{method.name}"
        funcs.append(lsyscall)

    return [CTranslationUnit(list(funcs))]


def __mk_selfcall_template(method: SyscallMethod) -> CFunction:
    parameters = list(CParameterDeclaration(p.name, CType.constant_of(p.parameter_type))
                      for p in method.parameters)

    selfcall_template = CFunction(
        name='',
        parameters=[CParameterDeclaration.of("@rref:decl"),
                    CParameterDeclaration("__rpc_info", CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration.of("@buf_decls:decl"),
                    *parameters],
        return_type=CType.constant_of(method.direct_return_type),
        body=CStatement.constant_of(
            """{
            @unused_rref:stmt
            unsigned long credential = __ACTV_CRED_SELFCALL;
            struct sysif_actv_selfcall selfcall = {
                   @_send_buf:expr,
                   @_send_bufsz:expr,
                   @_rply_buf:expr,
                   @_rply_bufsz:expr,
                   __rpc_info
                };
            if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
                credential = __ACTV_CRED_SELFCALL_XACT;
            }
            return @callee:expr((unsigned long long)(uintptr_t)(& selfcall),
                    @credential:expr, @args:expr);
            }"""))

    # Do not subst a constant template in place
    return selfcall_template


def __mk_selfcall(method: SyscallMethod) -> List[CFunction]:
    if method.manager is None or method.manager.server is None:
        logger.error("method %s is invalid", method.name)
        raise ValueError(f"method {method.name} has no attached manager or server")

    # Do not subst a constant template in place
    selfcall_template = __mk_selfcall_template(method)

    callee = CIdentifierExpression(
        f'actvhdlr_{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}')
    func_name_postfix = f'{method.manager.server.call_prefix}_{method.manager_name}_{method.name}'

    sendbuf_desc = __gen_sendbuf(method)
    rplybuf_desc = __gen_rplybuf(method)

    if not method.support_self_call:
        return []

    template = subst(selfcall_template, {
        'buf_decls': sendbuf_desc.decls + rplybuf_desc.decls,
        '_send_buf': sendbuf_desc.buf_expr,
        '_send_bufsz': sendbuf_desc.buf_sz_expr,
        '_rply_buf': rplybuf_desc.buf_expr,
        '_rply_bufsz': rplybuf_desc.buf_sz_expr,
        'callee': callee,
        'args': list(CIdentifierExpression(p.name) for p in method.parameters)
    })

    actvcall = subst(template, {'rref': [], 'unused_rref': []})
    actvcall.name = f'__actvcall_{func_name_postfix}'

    actvcapcall = subst(template, {
        'rref': CParameterDeclaration("__rref", CAliasType('rref_t')),
        'unused_rref': CStatement.constant_of("@UNUSED:expr(__rref);"),
    })
    actvcapcall.name = f'__actvcapcall_{func_name_postfix}'

    return [actvcall, actvcapcall]
