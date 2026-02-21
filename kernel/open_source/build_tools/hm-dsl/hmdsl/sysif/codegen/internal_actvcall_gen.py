#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Generate internal client apis
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:37:08 PM CST
"""

from __future__ import absolute_import

from typing import List, Union, Tuple
from hmdsl.common.c_biast.subst import subst
from hmdsl.common.logging import get_logger
from hmdsl.sysif.model import Server, Manager, SyscallMethod, MethodType, LsyscallMethod, LsyscallType
from hmdsl.common.c_biast.biast import CExpression, CTranslationUnit, CAttribute, CFunction, \
    CVariableDeclaration, CParameterDeclaration, CStorageQualifier, CType, CAliasType, CVoidType, \
    CSignDescriptor, CIntType, CLongType, CLonglongType, CPointerType, CFunctionType, \
    CIdentifierExpression, CStatement, CDeclaration, CClassicalAttribute, CCastExpression, CMacroExpression, \
    CMacroAttribute


logger = get_logger("codegen.internal_actvcall_gen")


def gen_internal_actvcall(server: Server) -> List[Union[str, CTranslationUnit]]:
    """
    Generate definitions of actv calls
    """
    res: List[Union[str, CTranslationUnit]] = []
    res.append(CTranslationUnit(list(__mk_rpc_calls(server))))
    for manager in server.managers:
        res += __mk_client_actvcall_apis(manager)

    return res


def __mk_client_actvcall_apis(manager: Manager) -> List[Union[str, CTranslationUnit]]:
    syscalls: List[Union[CDeclaration, CFunction]] = []
    for syscall in manager.syscalls:
        syscalls += __generate_internal_actv_decls(syscall)
        syscalls += __generate_internal_actvcalls(syscall)
    res: List[Union[str, CTranslationUnit]] = [CTranslationUnit(syscalls)]
    res += manager.print_lsyscalls_in_arch_macros(__generate_internal_lsyscall)
    return res


def __generate_internal_lsyscall(lsyscalls: List[LsyscallMethod]) -> List[CTranslationUnit]:
    template = CFunction(
        name='%fake%',
        parameters=[CParameterDeclaration(name="syscall_num", type=CLongType()),
                    CParameterDeclaration.of("@params:decl")],
        return_type=CType.of("@return_type:type"),
        body=CStatement.of(
            """ {long ret = @lsyscall_callee:expr(syscall_num, @args:expr);
            @error_cast:stmt
            return (@return_type:type)ret;
            }
            """))
    err_cast = CStatement.constant_of(
        """
        if (ret < 0 && ret > -4096){
            ret = posix2hmerrno(-ret);
        }
        """)
    native_calls = list(lsyscall for lsyscall in lsyscalls if lsyscall.lsyscall_type is not LsyscallType.NONATIVE)
    funcs: List[CFunction] = []
    for method in native_calls:
        return_type = CType.constant_of(method.direct_return_type)
        parameters = list(CParameterDeclaration(param.name, CType.constant_of(param.parameter_type))
                          for param in method.parameters)
        args = list(CCastExpression(CLongType(), CIdentifierExpression(param.name)) for param in method.parameters)
        nargs = len(parameters)
        # lsyscall with unsigned int return type is considered to be always valid, thus returns no error number.
        if isinstance(return_type, CIntType) and return_type.sign_desc is CSignDescriptor.UNSIGNED:
            lsyscall = subst(template, {"return_type": return_type,
                                        "params": parameters,
                                        "args": args,
                                        "error_cast": [],
                                        "lsyscall_callee": CIdentifierExpression(f"lsyscall_syscall{nargs}"),
                                        }, False)
        else:
            lsyscall = subst(template, {"return_type": return_type,
                                        "params": parameters,
                                        "args": args,
                                        "error_cast": err_cast,
                                        "lsyscall_callee": CIdentifierExpression(f"lsyscall_syscall{nargs}"),
                                        }, False)

        lsyscall.name = f"__lsyscall_{method.manager_name}_{method.name}"
        if method.lsyscall_type is LsyscallType.NATIVEWITHSELFCALL:
            lsyscall.attributes = [CClassicalAttribute("weak", None)]
        funcs.append(lsyscall)
    return [CTranslationUnit(list(funcs))]


def __mk_rpc_call_template(server: Server) -> Tuple[CFunction, CFunction]:
    rpc_template = CFunction(
        name='%fake%',
        parameters=[CParameterDeclaration(name=None, type=CVoidType())],
        return_type=CIntType(),
        body=CStatement.of(
            """{@__RPC_DEF_RETVAR:expr(int, ret);
            @rpc_callee:expr(@group:expr, @buf_type:expr, ret);
            return ret;
            }
            """),
        storage_qualifiers={CStorageQualifier.STATIC},
        attributes=[CAttribute.constant_of("@__used:attr"),
                    CAttribute.constant_of("@__noinline:attr"),
                    CAttribute.constant_of("@__opt_noframe:attr")]
    )

    rpc_cap_template = subst(
        rpc_template,
        {'group': [],
         'rpc_callee': CIdentifierExpression('__RPC_CAP_SYSCALL')},
        False)

    # function body will later be replaced by a panic if server's group tag is None
    subst(rpc_template,
          {'group': CExpression.of(server.group or 'NO_GROUP_ALERT'),
           'rpc_callee': CIdentifierExpression('__RPC_FAST_SYSCALL')},
          True)

    return (rpc_template, rpc_cap_template)


def __mk_rpc_calls(server: Server) -> List[CFunction]:
    ret = []
    rpc_template: CFunction
    rpc_cap_template: CFunction
    rpc_template, rpc_cap_template = __mk_rpc_call_template(server)
    call_nobuf = subst(rpc_template, {'buf_type': CIdentifierExpression("NOBUF")}, False)
    call_nobuf.name = '__internal_rpccall_nobuf'
    ret.append(call_nobuf)

    call_buf = subst(rpc_template, {'buf_type': CIdentifierExpression("SENDBUF")}, False)
    call_buf.name = '__internal_rpccall_buf'
    ret.append(call_buf)

    call_ret = subst(rpc_template, {'buf_type': CIdentifierExpression("RETBUF")}, False)
    call_ret.name = '__internal_rpccall_retbuf'
    ret.append(call_ret)

    call_fallback = subst(rpc_template, {'buf_type': CIdentifierExpression("FALLBACK")}, False)
    call_fallback.name = '__internal_rpccall_fallback'
    ret.append(call_fallback)

    if server.group is None:
        for func in ret:
            func.body = CStatement.constant_of(
                """
                {hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\\n");
                return E_HM_NOSYS;
                }
                """)

    capcall_nobuf = subst(rpc_cap_template, {'buf_type': CIdentifierExpression("NoBuf")}, False)
    capcall_nobuf.name = '__internal_rpccapcall_nobuf'
    ret.append(capcall_nobuf)

    capcall_buf = subst(rpc_cap_template, {'buf_type': CIdentifierExpression("SendBuf")}, False)
    capcall_buf.name = '__internal_rpccapcall_buf'
    ret.append(capcall_buf)

    capcall_ret = subst(rpc_cap_template, {'buf_type': CIdentifierExpression("RetBuf")}, False)
    capcall_ret.name = '__internal_rpccapcall_retbuf'
    ret.append(capcall_ret)

    capcall_fallback = subst(rpc_cap_template, {'buf_type': CIdentifierExpression("")}, False)
    capcall_fallback.name = '__internal_rpccapcall_fallback'
    ret.append(capcall_fallback)

    return ret


def __generate_internal_actv_decls(method: SyscallMethod) -> List[CVariableDeclaration]:
    if method.manager is None or method.manager.server is None:
        logger.error("method %s is invalid", method.name)
        raise ValueError(f"method {method.name} has no attached manager or server")

    call_prefix = method.manager.server.call_prefix
    caller_name_postfix = f"{call_prefix}_{method.manager_name}_{method.name}"

    # postfix is nobuf, retbuf, buf, etc.
    internal_impl_postfix = method.method_type.name.lower()

    call_attributes = [
        CAttribute.alias(f"__internal_rpccall_{internal_impl_postfix}"),
        CAttribute.constant_of("@__internal_rpccall:attr")
    ]
    capcall_attributes = [
        CAttribute.alias(f"__internal_rpccapcall_{internal_impl_postfix}"),
        CAttribute.constant_of("@__internal_rpccapcall:attr")
    ]

    # Only for the fallback case we need to obtain the indirect_returned_values
    # from a reply buffer, namely `rply_buf`.
    rply = []
    if method.method_type is MethodType.FALLBACK:
        rply = [
            CParameterDeclaration('__rply_buf', CPointerType(CVoidType())),
            CParameterDeclaration('rply_buf_sz', CLongType(CSignDescriptor.UNSIGNED))
        ]

    actv_decl_template = CVariableDeclaration(
        name='',
        type=CFunctionType(
            parameter_decls=[
                CParameterDeclaration.of("@rref:decl"),
                CParameterDeclaration('rpc_info', CLonglongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('__buf', CPointerType(CVoidType())),
                *rply],
            return_type=CType.constant_of(method.direct_return_type),
            variable_length_argument=True),
        storage_qualifiers={CStorageQualifier.STATIC})

    actv_call = subst(actv_decl_template, {'rref': []}, False)
    actv_call.attributes = call_attributes
    actv_call.name = f'____actvcall_{caller_name_postfix}'

    actv_capcall = subst(actv_decl_template,
                         {'rref': CParameterDeclaration('__rref', CAliasType('rref_t'))},
                         False)
    actv_capcall.attributes = capcall_attributes
    actv_capcall.name = f"____actvcapcall_{caller_name_postfix}"

    return [actv_call, actv_capcall]


def __generate_internal_actvcall_template(method: SyscallMethod) -> CFunction:
    return_type = CType.constant_of(method.direct_return_type)
    parameters = list(CParameterDeclaration(param.name, CType.constant_of(param.parameter_type))
                      for param in method.parameters)

    # Only for the fallback case we need to obtain the indirect_returned_values
    # from a reply buffer, namely `rply_buf`.
    rply = []
    if method.method_type is MethodType.FALLBACK:
        rply = [
            CParameterDeclaration("__rply_buf", CPointerType(CVoidType())),
            CParameterDeclaration("rply_buf_sz", CLongType(CSignDescriptor.UNSIGNED))
        ]
    rply_names = ['__rply_buf', 'rply_buf_sz'] if method.method_type is MethodType.FALLBACK else []

    actv_call_template = CFunction(
        name='',
        parameters=[CParameterDeclaration.of("@rref:decl"),
                    CParameterDeclaration("__rpc_info", CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration("__buf", CPointerType(CVoidType())),
                    *rply,
                    *parameters],
        return_type=return_type,
        body=CStatement.of(
            """
            {@return_type:type __ret;
             @local_args:decl
             __ret = @callee:expr(@_rref:expr, __rpc_info, __buf, @_rply:expr, @call_args:expr);
             return __ret;
            }
            """),
        attributes=[CAttribute.constant_of("@__internal_actvcall:attr")])

    local_args = __mk_local_var(method)
    call_args = __mk_rpc_pass_arg(method)
    return subst(actv_call_template,
                 {'return_type': return_type, 'local_args': local_args,
                  '_rply': list(CIdentifierExpression(p) for p in rply_names), 'call_args': call_args}, True)


def __generate_internal_actvcalls(method: SyscallMethod) -> List[CFunction]:
    if method.manager is None or method.manager.server is None:
        logger.error("method %s is invalid", method.name)
        raise ValueError(f"method {method.name} has no attached manager or server")

    actv_call_template = __generate_internal_actvcall_template(method)
    call_prefix = method.manager.server.call_prefix
    caller_postfix = f"{call_prefix}_{method.manager_name}_{method.name}"
    actv_call = subst(actv_call_template,
                      {'rref': [],
                       'callee': CIdentifierExpression(f"____actvcall_{caller_postfix}"),
                       '_rref': [],
                       },
                      False)
    actv_call.name = f'__actvcall_{caller_postfix}'

    actv_capcall = subst(actv_call_template,
                         {'rref': CParameterDeclaration("__rref", CAliasType('rref_t')),
                          'callee': CIdentifierExpression(f"____actvcapcall_{caller_postfix}"),
                          '_rref': CIdentifierExpression("__rref"),
                          },
                         False)
    actv_capcall.name = f"__actvcapcall_{caller_postfix}"

    return [actv_call, actv_capcall]


def __mk_rpc_pass_arg(method: SyscallMethod) -> List[CMacroExpression]:
    call_args = []
    for param in method.parameters:
        call_args.append(CMacroExpression(
            '__ARCH_ACTV_RPC_PASS_ARG',
            [CIdentifierExpression(param.name), CIdentifierExpression(f'__actv_local_var_{param.name}')]))
    return call_args


def __mk_local_var(method: SyscallMethod) -> List[CVariableDeclaration]:
    local_args = []
    for param in method.parameters:
        arg = CVariableDeclaration(
            f"__actv_local_var_{param.name}",
            CType.constant_of(param.parameter_type),
            initializer=CIdentifierExpression(param.name))
        arg.attributes = [CMacroAttribute('__maybe_unused', [])]
        local_args.append(arg)
    return local_args
