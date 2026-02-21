#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Generate sysif server apis
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:34:52 PM CST
"""

from __future__ import absolute_import

from typing import List, Union
from hmdsl.common.c_biast.subst import subst
from hmdsl.common.logging import get_logger
from hmdsl.sysif.model import MethodType, Server, SyscallMethod, LsyscallMethod
from hmdsl.common.c_biast.biast import CTranslationUnit, CFunction, CDeclaration, \
    CVariableDeclaration, CParameterDeclaration, CFieldDeclaration, CStorageQualifier, CType, \
    CAliasType, CVoidType, CSignDescriptor, CIntType, CLongType, CLonglongType, CPointerType, \
    CStructType, CUnionType, CFunctionType, CStatement


logger = get_logger("codegen.actvhdlr_gen")


def gen_actvhdlr(server: Server) -> List[Union[str, CTranslationUnit]]:
    """
    Generate server apis, namely actvhdlr, for the server
    """
    actvhdlr: List[Union[str, CTranslationUnit]] = []
    actvhdlr_bridge: List[Union[str, CTranslationUnit]] = []
    actv_ret: List[Union[str, CTranslationUnit]] = []
    for manager in server.managers:
        actvhdlr += __declare_actv_hdlrs(manager.syscalls)
        actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.syscalls)
        actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.syscalls, True)
        actvhdlr += manager.print_lsyscalls_in_arch_macros(__declare_actv_hdlrs)
        actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.general_lsyscalls)
        actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.general_lsyscalls, True)
        if manager.aarch64_lsyscalls:
            actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.aarch64_lsyscalls)
            actvhdlr_bridge.append(f'#ifdef __sysif_server_{server.name}_no_compat32_handlers__\n')
            actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.aarch64_lsyscalls, True)
            actvhdlr_bridge.append("#endif\n")
        if manager.arm_lsyscalls:
            actvhdlr_bridge.append(f'#ifndef __sysif_server_{server.name}_no_compat32_handlers__\n')
            actvhdlr_bridge += __declare_actv_bridge_hdlrs(manager.arm_lsyscalls, True)
            actvhdlr_bridge.append("#endif\n")

        actv_ret += __mk_actvret(manager.syscalls)
        actv_ret += manager.print_lsyscalls_in_arch_macros(__mk_actvret)

    res: List[Union[str, CTranslationUnit]]
    res = ['#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR\n']
    res += actvhdlr
    res.append('#else /* aarch64 */\n')
    res += actvhdlr_bridge
    res.append('#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */\n')
    res += actv_ret

    res += __mk_actvhdlr_union(server)
    res += __mk_actvhdlr_table_decls(server)

    return res


def __mk_actvhdlrs_template() -> CVariableDeclaration:
    actvhdlrs_template = CVariableDeclaration(
        name=None,
        type=CFunctionType(
            parameter_decls=[
                CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration.of("""@parameters:decl""")
            ],
            return_type=CType.of("""@return_type:type"""),
            variable_length_argument=False),
        storage_qualifiers={CStorageQualifier.EXTERN})
    return actvhdlrs_template


def __declare_actv_hdlrs(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    actvhdlrs_template = __mk_actvhdlrs_template()
    hdlrs = []
    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manager or server")

        func_name_postfix = f"""{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}"""
        params = list(CParameterDeclaration(p.name, CType.constant_of(p.parameter_type)) for p in method.parameters)
        actvhdlr_func = subst(
            actvhdlrs_template,
            {'parameters': params,
             'return_type': CType.constant_of(method.direct_return_type)},
            False)
        actvhdlr_func.name = f"""actvhdlr_{func_name_postfix}"""
        hdlrs.append(actvhdlr_func)

    return [CTranslationUnit(list(hdlrs))]


def __declare_actv_bridge_hdlrs(methods: Union[List[SyscallMethod], List[LsyscallMethod]], compat: bool = False) -> \
        List[CTranslationUnit]:
    actvhdlrs_template = __mk_actvhdlrs_template()
    hdlrs = []
    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manager or server")

        func_name_postfix = f"""{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}"""
        if not compat:
            params = list(CParameterDeclaration(p.name, CType.constant_of(p.parameter_type)) for p in method.parameters)
            actvhdlr_func = subst(
                actvhdlrs_template,
                {'parameters': params,
                 'return_type': CType.constant_of(method.direct_return_type)},
                False)
            actvhdlr_func.name = f"""actvhdlr_{func_name_postfix}"""
            hdlrs.append(actvhdlr_func)
        else:
            actvhdlr_bridge_func = subst(
                actvhdlrs_template,
                {'parameters': [],
                 'return_type': CType.constant_of(method.direct_return_type)},
                False)

            if not isinstance(actvhdlr_bridge_func.type, CFunctionType):
                logger.error("expecting function type, but %s found", str(actvhdlr_bridge_func.type))
                raise ValueError(f"expecting function type in {actvhdlr_bridge_func}")

            actvhdlr_bridge_func.type.variable_length_argument = True
            actvhdlr_bridge_func.name = f"__actvshadowhdlr_{func_name_postfix}"
            hdlrs.append(actvhdlr_bridge_func)

    return [CTranslationUnit(list(hdlrs))]


def __mk_actvret(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    res = []
    prepare_ret_template = CFunction(
        name='',
        parameters=[CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED))],
        return_type=CType.of('@ret_type:type *'),
        body=CStatement.of(
            """{
            void * __ret =
            hm_actvpool_prepare_ret(sender, credential, @sizeof:expr(@ret_type:type));
            return (@ret_type:type *)__ret;
            }"""),
        inline=True,
        variable_length_argument=False,
        storage_qualifiers={CStorageQualifier.STATIC})

    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manager or server")

        ret_type = CStructType(
            name=f'__actvret_{method.manager.server.call_prefix}_{method.manager_name}_{method.name}',
            field_decls=None
        )

        prepare_ret = subst(
            prepare_ret_template,
            {'ret_type': ret_type},
            False)
        prepare_ret.name = \
            f'actvhdlr_{method.manager.server.handler_prefix}_{method.manager_name}_{method.name}_prepare_ret'
        res.append(prepare_ret)

    return [CTranslationUnit(list(res))]


def __construct_hdlrs_union_fields(server: Server, macro: str) -> List[CFieldDeclaration]:
    fields = []
    bridgehdlrs_template = CFieldDeclaration(
        name='%fake%',
        type=CPointerType(CFunctionType(
            parameter_decls=[
                CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration.of('@rpc_unused:decl'),
                CParameterDeclaration.of('@fallback:decl'),
                CParameterDeclaration.of("""@parameters:decl""")],
            return_type=CType.of("""@return_type:type"""),
            variable_length_argument=False)))
    mthds = []
    for manager in server.managers:
        mthds = manager.syscalls + manager.general_lsyscalls
        if macro == "__aarch64__":
            mthds += manager.aarch64_lsyscalls
        elif macro == "__arm__":
            mthds += manager.arm_lsyscalls

        for method in mthds:
            func_name_postfix = f"""{server.handler_prefix}_{method.manager_name}_{method.name}"""
            params = list(CParameterDeclaration(p.name, CType.constant_of(p.parameter_type)) for p in method.parameters)
            fallback_unused = [
                CParameterDeclaration('__actv_fallback_unused_1', CLongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('__actv_fallback_unused_2', CLongType(CSignDescriptor.UNSIGNED))
            ] if method.method_type is MethodType.FALLBACK else []
            bridgehdlr = subst(bridgehdlrs_template,
                               {'rpc_unused': [], 'fallback': fallback_unused, 'parameters': params,
                                'return_type': CType.constant_of(method.direct_return_type)},
                               False)
            bridgehdlr.name = f"""p__actvbridgehdlr_{func_name_postfix}"""
            fields.append(bridgehdlr)
            if isinstance(method, SyscallMethod):
                bridgecapcall = subst(
                    bridgehdlrs_template,
                    {'rpc_unused': CParameterDeclaration.of('@__RPC_CAPCALL_HDLR_UNUSED_ARGS:decl'),
                     'fallback': fallback_unused, 'parameters': params,
                     'return_type': CType.constant_of(method.direct_return_type)},
                    False)
                bridgecapcall.name = f"p__actvbridgecapcallhdlr_{func_name_postfix}"
                fields.append(bridgecapcall)
            elif isinstance(method, LsyscallMethod):
                bridgecapcall = subst(bridgehdlr, {}, False)
                bridgecapcall.name = f"p__actvbridgecapcallhdlr_{func_name_postfix}"
                fields.append(bridgecapcall)
    return fields


def __construct_compat_hdlrs_union_fields(server: Server) -> List[CFieldDeclaration]:
    fields = []
    compat_template = CFieldDeclaration(
        name='%fake%',
        type=CPointerType(CFunctionType(
            parameter_decls=[
                CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('sp', CLongType(CSignDescriptor.UNSIGNED)),
                CParameterDeclaration('x3', CLongType(CSignDescriptor.UNSIGNED))],
            return_type=CType.of("""@return_type:type"""),
            variable_length_argument=False)))
    mthds = []
    for manager in server.managers:
        mthds = manager.syscalls + list(manager.general_lsyscalls + manager.arm_lsyscalls)

        for method in mthds:
            func_name_postfix = f"""compat_{server.handler_prefix}_{method.manager_name}_{method.name}"""
            compathdlr = subst(compat_template,
                               {'return_type': CType.constant_of(method.direct_return_type)},
                               False)
            compathdlr.name = f"""p__actvbridgehdlr_{func_name_postfix}"""
            fields.append(compathdlr)
            if isinstance(method, SyscallMethod):
                compatcapcall = subst(compat_template,
                                      {'return_type': CType.constant_of(method.direct_return_type)},
                                      False)
                compatcapcall.name = f"p__actvbridgecapcallhdlr_{func_name_postfix}"
                fields.append(compatcapcall)
            elif isinstance(method, LsyscallMethod):
                compatcapcall = subst(compathdlr, {}, False)
                compatcapcall.name = f"p__actvbridgecapcallhdlr_{func_name_postfix}"
                fields.append(compatcapcall)
    return fields


def __construct_hdlrs_union(server: Server, macro: str) -> List[CVariableDeclaration]:
    hdlrs = [
        CFieldDeclaration('p_sysif_actvhdlr_ptr_void', CPointerType(CVoidType())),
        CFieldDeclaration(
            name='p_sysif_default_handler',
            type=CPointerType(CFunctionType(
                parameter_decls=[
                    CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                    CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED))],
                return_type=CIntType(),
                variable_length_argument=False)))
    ]

    union_fields = hdlrs + __construct_hdlrs_union_fields(server, macro)
    compat_union_fields = union_fields + __construct_compat_hdlrs_union_fields(server)
    union_template = CVariableDeclaration(None, CUnionType(
        name=f'{server.name}_hdlrs_union',
        field_decls=[CDeclaration.of("@fields:decl"),
                     CDeclaration.of('@kcall_entry:decl')]))
    hdlrs_union = subst(union_template, {'fields': union_fields}, False)
    compat_hdlrs_union = subst(union_template, {'fields': compat_union_fields}, False)
    return [hdlrs_union, compat_hdlrs_union]


def __mk_kcall_actv_hdlr(server: Server) -> List[List[List[Union[str, CTranslationUnit]]]]:
    hdlrs_union_aarch, hdlrs_compat_union = __construct_hdlrs_union(server, "__aarch64__")
    hdlrs_union_arm, _ = __construct_hdlrs_union(server, "__arm__")
    llt_kcall = CFieldDeclaration('p_sysif_handle_kcall_entry',
                                  CPointerType(CFunctionType(parameter_decls=[], return_type=CIntType(),
                                                             variable_length_argument=False)))
    hdlrs_union_llt: List[Union[str, CTranslationUnit]]
    hdlrs_compat_union_llt: List[Union[str, CTranslationUnit]]
    hdlrs_union_general, compat_general = __construct_hdlrs_union(server, "")
    if server.is_general():
        hdlrs_union_llt = [CTranslationUnit([subst(hdlrs_union_general, {'kcall_entry': llt_kcall}, False)])]
        hdlrs_compat_union_llt = [CTranslationUnit([subst(compat_general, {'kcall_entry': llt_kcall}, False)])]
    else:
        hdlrs_union_llt = ["#ifdef __aarch64__\n"]
        hdlrs_union_llt.append(CTranslationUnit([subst(hdlrs_union_aarch, {'kcall_entry': llt_kcall}, False)]))
        hdlrs_union_llt.append("#elif defined(__arm__)\n")
        hdlrs_union_llt.append(CTranslationUnit([subst(hdlrs_union_arm, {'kcall_entry': llt_kcall}, False)]))
        hdlrs_union_llt.append("#else\n")
        hdlrs_union_llt.append(CTranslationUnit([subst(hdlrs_union_general, {'kcall_entry': llt_kcall}, False)]))
        hdlrs_union_llt.append("#endif\n")
        hdlrs_compat_union_llt = ["#ifdef __aarch64__\n"]
        hdlrs_compat_union_llt.append(CTranslationUnit([subst(hdlrs_compat_union, {'kcall_entry': llt_kcall}, False)]))
        hdlrs_compat_union_llt.append("#else\n")
        hdlrs_compat_union_llt.append(CTranslationUnit([subst(hdlrs_union_general, {'kcall_entry': llt_kcall}, False)]))
        hdlrs_compat_union_llt.append("#endif\n")

    aarch_kcall = CFieldDeclaration("p_sysif_handle_kcall_entry",
                                    CPointerType(CFunctionType(
                                        parameter_decls=[
                                            CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                                            CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED)),
                                            CParameterDeclaration('arg1', CAliasType('__u64')),
                                            CParameterDeclaration('arg2', CAliasType('__u64')),
                                            CParameterDeclaration('arg3', CAliasType('__u64'))],
                                        return_type=CIntType(),
                                        variable_length_argument=False)))
    union_aarch: List[Union[str, CTranslationUnit]]
    union_aarch = [CTranslationUnit([subst(hdlrs_union_aarch, {'kcall_entry': aarch_kcall}, False)])]
    compat_union_aarch: List[Union[str, CTranslationUnit]]
    compat_union_aarch = [CTranslationUnit([subst(hdlrs_compat_union, {'kcall_entry': aarch_kcall}, False)])]
    arm_kcall = CFieldDeclaration('p_sysif_handle_kcall_entry',
                                  CPointerType(CFunctionType(
                                      parameter_decls=[
                                          CParameterDeclaration('sender', CLonglongType(CSignDescriptor.UNSIGNED)),
                                          CParameterDeclaration('credential', CLongType(CSignDescriptor.UNSIGNED))],
                                      return_type=CIntType(),
                                      variable_length_argument=False)))
    union_arm: List[Union[str, CTranslationUnit]]
    union_arm = [CTranslationUnit([subst(hdlrs_union_arm, {'kcall_entry': arm_kcall}, False)])]
    return [[hdlrs_union_llt, hdlrs_compat_union_llt], [union_aarch, compat_union_aarch], [union_arm]]


def __mk_actvhdlr_union(server: Server) -> List[Union[str, CTranslationUnit]]:
    unions = __mk_kcall_actv_hdlr(server)
    hdlrs_union_llt, hdlrs_compat_union_llt = unions[0]
    hdlrs_union_aarch, hdlrs_compat_union_aarch = unions[1]
    hdlrs_union_arm = unions[2][0]

    res: List[Union[str, CTranslationUnit]]
    res = [f'#ifdef __sysif_server_{server.name}_no_compat32_handlers__\n']
    # make handlers union
    res.append('#ifdef __HOST_LLT__\n')
    res += hdlrs_union_llt
    res.append('#else\n')
    res.append('#ifdef __aarch64__\n')
    res += hdlrs_union_aarch
    res.append('#endif /* aarch64 */\n')
    res.append('#ifdef __arm__\n')
    res += hdlrs_union_arm
    res.append('#endif /* arm */\n')
    res.append('#endif /* not HOST_LLT */\n')

    res.append('#else\n')
    res.append('#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR\n')
    res.append('#error "Only support 64-bit server, build for aarch64 please!"\n')
    res.append('#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */\n')

    # make campat handlers union
    res.append('#ifdef __HOST_LLT__\n')
    res += hdlrs_compat_union_llt
    res.append('#else\n')
    res.append('#ifdef __aarch64__\n')
    res += hdlrs_compat_union_aarch
    res.append('#endif /* aarch64 */\n')
    res.append('#endif /* not HOST_LLT */\n')

    res.append('#endif /* compat32 */\n\n')

    return res


def __mk_actvhdlr_table_decls(server: Server) -> List[Union[str, CTranslationUnit]]:
    nocompat_macro = f'defined(__sysif_server_{server.name}_no_compat32_handlers__)'
    noilp32_macro = f'defined(__sysif_server_{server.name}_no_ilp32_handlers__)'
    times = f'__{server.name}_method_TIMES'

    res: List[Union[str, CTranslationUnit]] = [
        f'#if {nocompat_macro} && {noilp32_macro}\n',
        f'#define {times} 1\n'
        f'#elif !{nocompat_macro} && !{noilp32_macro}\n',
        f'#define {times} 3\n'
        '#else\n',
        f'#define {times} 2\n'
        '#endif\n'
    ]

    decls = CVariableDeclaration.of(
        f""" extern union {server.name}_hdlrs_union
        sysif_{server.name}_actvhdlr_table[__{server.name}_method_MAX * {times}];
        """)
    decls += CVariableDeclaration.of(
        f""" extern union {server.name}_hdlrs_union
        sysif_{server.name}_actvcapcallhdlr_table[__{server.name}_method_MAX * {times}];
        """)
    decls += CVariableDeclaration.of(
        f""" extern void sysif_show_{server.name}_methods(void);
        """)
    res.append(CTranslationUnit(decls))

    return res
