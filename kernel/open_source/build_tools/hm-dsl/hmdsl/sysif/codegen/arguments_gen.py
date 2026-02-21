#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Generate return types and structures for sysif apis
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:36:13 PM CST
"""

from __future__ import absolute_import

from typing import List, Union, Tuple, Dict
from itertools import chain
from hmdsl.common.c_biast.subst import subst
from hmdsl.common.logging import get_logger
from hmdsl.sysif.model import Server, Manager, LsyscallMethod, SyscallMethod, Method
from hmdsl.common.c_biast.biast import CTranslationUnit, CDeclaration, CVariableDeclaration, \
    CFieldDeclaration, CType, CStructType, CUnionType, CEnumType, CEnumItem, CIntegerConstant


logger = get_logger("codegen.arguments_gen")


def gen_actv_args_static_assert(server: Server) -> List[Union[str, CTranslationUnit]]:
    """
    On gcc 7.5.0 or lower version, when sysif arg struct size is over 96 byte, copy will use builtin
    memcpy, leading to callee saved register take stack cost, impact calculating stack args size.
    """
    asserts: List[Union[str, CTranslationUnit]] = []
    filters = [
        "char", "short", "int", "long", "long long", "unsigned", "unsigned char", "unsigned short",
        "unsigned int", "unsigned long", "unsigned long long", "bool", "size_t", "ssize_t", "__u8",
        "__u16", "__u32", "__u64", "__s8", "__s16", "__s32", "__s64", "uintptr_t", "uint8_t",
        "uint16_t", "uint32_t", "uint64_t", "intptr_t", "int8_t", "int16_t", "int32_t", "int64_t"
    ]
    for manager in server.managers:
        for method in manager.syscalls:
            for param in method.parameters:
                if "*" in param.parameter_type or param.parameter_type in filters:
                    continue
                argname = f'{server.call_prefix}_{method.manager_name}_{method.name}_arg_{param.name}'
                asserts.append(
                    f"""raw_static_assert(sizeof({param.parameter_type}) <= 96,
                    {argname}_is_too_large);\n""")

    return asserts


def gen_actv_ret(server: Server) -> List[Union[str, CTranslationUnit]]:
    """
    Generate actv ret structures and apis for server
    """
    res: List[Union[str, CTranslationUnit]] = []

    for manager in server.managers:
        res += __mk_actvret_struct(manager.syscalls)
        res += manager.print_lsyscalls_in_arch_macros(__mk_actvret_struct)

    res += server.print_methods_in_arch_macros(__mk_actvret_union)
    res += __mk_svr_enum(server)
    res += __mk_callno(server)

    for manager in server.managers:
        res += __mk_ret_static_assert(manager.syscalls)
        res += manager.print_lsyscalls_in_arch_macros(__mk_ret_static_assert)

    res += [
        f"""raw_static_assert(sizeof(union __actvret_{server.call_prefix}) < 512,
        {server.call_prefix}_too_many_actvret_data);\n"""
    ]
    return res


def __mk_actvret_struct(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[CTranslationUnit]:
    res: List[CVariableDeclaration] = []

    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} has no attached manager or server")

        retname_postfix = f'{method.manager.server.call_prefix}_{method.manager_name}_{method.name}'
        fields = list(CFieldDeclaration(p.name, CType.constant_of(p.parameter_type))
                      for p in method.indirect_returned_values or [])
        actvret = CVariableDeclaration(
            name=None,
            type=CStructType(
                name=f'__actvret_{retname_postfix}',
                field_decls=fields))
        res.append(actvret)

    return [CTranslationUnit(list(res))]


def __mk_actvret_union(methods: List[Tuple[Manager, List[Method]]]) -> List[Union[str, CTranslationUnit]]:
    svr = methods[0][0].server
    if svr is None:
        logger.error("manager %s is invalid", methods[0][0].name)
        raise ValueError(f"manager {methods[0][0].name} is not attached to a server")
    union_template = CVariableDeclaration(
        name=None,
        type=CUnionType(name=f'__actvret_{svr.call_prefix}',
                        field_decls=[CDeclaration.of('@fields:decl')]))

    fields = []
    for _, mthds in methods:
        for method in mthds:
            fields.append(CFieldDeclaration(
                f'{method.manager_name}_{method.name}',
                CStructType(f'__actvret_{svr.call_prefix}_{method.manager_name}_{method.name}')
            ))

    decl = subst(union_template, {'fields': fields}, False)

    return [CTranslationUnit([decl])]


def __mk_ret_static_assert(methods: Union[List[SyscallMethod], List[LsyscallMethod]]) -> List[str]:
    asserts: List[str] = []
    for method in methods:
        if method.manager is None or method.manager.server is None:
            logger.error("method %s is invalid", method.name)
            raise ValueError(f"method {method.name} is not attached to a manager or a server")

        retname = f'{method.manager.server.call_prefix}_{method.manager_name}_{method.name}'
        asserts.append(
            f"""raw_static_assert(sizeof(struct __actvret_{retname}) < 512,
            {retname}_too_many_actvret_data);\n"""
        )

    return asserts


def __mk_svr_enum(server: Server) -> List[CTranslationUnit]:
    mgr_items = [CEnumItem(f'__{server.name}_INVAL_mgr', CIntegerConstant(0))]
    mgr_items += list(CEnumItem(
        f'__{server.name}_{manager.name}_mgr', None
    ) for manager in server.managers)

    mgr_items.append(CEnumItem(f'__{server.name}_MAX_mgr', None))

    server_enum = CTranslationUnit([CVariableDeclaration(
        name=None,
        type=CEnumType(
            name=f'__{server.name}_mgr',
            enum_items=mgr_items)
    )])

    return [server_enum]


def __mk_callno(svr: Server) -> List[str]:
    decls = [f"#define __{svr.name}_method_MIN __ACTV_HIGHTABLE_START\n"]

    pfx = f"__{svr.name}_method"
    cnt = 1
    for mgr in svr.managers:
        for mthd in mgr.lsyscall_methods_list(with_mgr_name=True):
            decls.append(f"#define {pfx}_{mthd} (__ACTV_HIGHTABLE_START + {cnt})\n")
            cnt += 1

    # put lsyscalls before actvcalls, thus any changes on actvcalls would not affect lsyscall map in kernel
    for mgr in svr.managers:
        for mthd in mgr.actvcall_methods_list(with_mgr_name=True, with_rsvd=True):
            decls.append(f"#define {pfx}_{mthd} (__ACTV_HIGHTABLE_START + {cnt})\n")
            cnt += 1
    decls.append(f"#define {pfx}_MAX (__ACTV_HIGHTABLE_START + {cnt})\n")

    return decls


def mk_lsyscall_info(mthd: LsyscallMethod, callno: str) -> str:
    """ generate informations for lsyscall """
    infos = "{lsys_no}, (__ACTV_HIGHTABLE_START + {sysif_no}), {args_num}, {ret_type}"
    rettype_map = {
        "int": "__LSYSCALL_RET_TYPE_INT",
        "long": "__LSYSCALL_RET_TYPE_LONG",
        "unsigned int": "__LSYSCALL_RET_TYPE_UINT",
        "none": "__LSYSCALL_RET_TYPE_NONE"
    }
    ret_key = "none" if mthd.map_none else mthd.direct_return_type
    ret_type = rettype_map.get(ret_key, "UNKNOWN")
    return infos.format(
        lsys_no=mthd.syscall_num, sysif_no=callno,
        args_num=str(len(mthd.parameters)), ret_type=ret_type
    )


def mk_lsyscall_callno(svr: Server) -> List[str]:
    """ collect lsyscalls callnums """
    items = []
    cnt = 1
    item_template = "{arch}_LSYSCALL_MAP_ITEM(SVR_{svr}, {item_info})\n"
    for mgr in svr.managers:
        name_to_callno: Dict[str, int] = dict()
        lsyscalls = list(chain(*mgr.lsyscalls.values()))
        general_lsyscalls = list(filter(lambda x: x.arch_macro is None, lsyscalls))
        aarch64_lsyscalls = list(filter(lambda x: x.arch_macro == "__aarch64__", lsyscalls))
        arm_lsyscalls = list(filter(lambda x: x.arch_macro == "__arm__", lsyscalls))

        for mthd in general_lsyscalls:
            if isinstance(mthd, LsyscallMethod):
                name_to_callno[mthd.name] = cnt
                info = mk_lsyscall_info(mthd, str(cnt))
                items += [
                    item_template.format(arch="AARCH64", svr=svr.name.upper(), item_info=info),
                    item_template.format(arch="ARM", svr=svr.name.upper(), item_info=info)
                ]
            cnt += 1

        for mthd in aarch64_lsyscalls:
            name_to_callno[mthd.name] = cnt
            info = mk_lsyscall_info(mthd, str(cnt))
            items.append(item_template.format(arch="AARCH64", svr=svr.name.upper(), item_info=info))
            cnt += 1

        for mthd in arm_lsyscalls:
            if mthd.name in name_to_callno:
                call_num = name_to_callno[mthd.name]
                info = mk_lsyscall_info(mthd, str(call_num))
                items.append(item_template.format(arch="ARM", svr=svr.name.upper(), item_info=info))
                continue
            name_to_callno[mthd.name] = cnt
            info = mk_lsyscall_info(mthd, str(cnt))
            items.append(item_template.format(arch="ARM", svr=svr.name.upper(), item_info=info))
            cnt += 1

    return items
