#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
Description: Entry for dsl code generator
Author: Huawei OS Kernel Lab
Create: Thu Apr 07 15:51:56 2022
"""

from __future__ import absolute_import

from typing import List, Union
from hmdsl.sysif.model import Server, SyscallMethod, LsyscallMethod, CType, Parameter
from hmdsl.common.logging import get_logger


def gen_uev_list(server: Server) -> List[str]:
    """ Generate uev trace point list for sysif methods of a given server """
    uev_list = []
    if server.name == "sysmgr":
        upcall_methods = [
            "handle_linux_syscall", "handle_actv_pool_grant_change", "handle_kpagefault_actv",
            "handle_vsfault_actv", "sysif_actv_pool_handle_grant_change", "handle_pool_page_alloc",
            "handle_pool_page_free", "actv_rpc_handler_set_cred", "handle_eventpoll_save_sigmask",
            "handle_eventpoll_restore_sigmask", "handle_thread_activate", "handle_vsmmu_s2_create",
            "handle_fastpfoob", "handle_vm_page_fault", "handle_fastcall_seccomp", "handle_thread_tid2cref",
            "handle_signals", "handle_ptrace_syscall_enter", "handle_ptrace_syscall_exit", "handle_actv_binder_ioctl",
            "handle_hkids_hkip_event_actv"
        ]
        params = [Parameter("arg1", "__u64"), Parameter("arg2", "__u64"), Parameter("arg3", "__u64")]
        # add upcall sysif tracepoint
        for method_name in upcall_methods:
            method = SyscallMethod(method_name, None, params, "int", False, None, None, False)
            uev_list += __gen_sysif_uev_points("sysmgr", method_name, "", method)
        # add exception sysif tracepoint
        exception_methods = [
            "handle_data_fault_actv", "handle_insn_fault_actv", "handle_debug_fault_actv",
            "handle_ss_fault_actv", "handle_und_insn_fault_actv", "handle_sp_pc_fault_actv",
            "handle_hw_wp_fault_actv", "handle_fpu_fault_actv", "handle_exception_from_ex_pool",
            "handle_hw_bp_fault_actv", "handle_pac_fault_actv"
        ]
        exception_params = params + [Parameter("abort_status", "__u64"), Parameter("abort_addr", "__u64"),
                                     Parameter("elr", "__u64")]
        for method_name in exception_methods:
            method = SyscallMethod(method_name, None, exception_params, "int", False, None, None, False)
            uev_list += __gen_sysif_uev_points("sysmgr", method_name, "", method)
        # add hmsignal sysif tracepoint
        signal_params = params + [Parameter("hmsignals", "__u64"),
                                  Parameter("timer_cref", "__u64"),
                                  Parameter("restartable_flags", "__u32")]
        method = SyscallMethod(method_name, None, signal_params, "int", False, None, None, False)
        uev_list += __gen_sysif_uev_points("sysmgr", "handle_hmsignals", "", method)
    elif server.name == "fs":
        upcall_methods = [
            "handle_epoll_recheck", "handle_eventpoll_polllist_sync",
            "handle_select", "handle_select_clear", "handle_eventpoll_recheck",
            "handle_eventpoll_ppoll", "handle_eventpoll_ppoll_clear",
            "handle_common_file_close", "handle_close_on_exec",
            "handle_rpccnt_put",
        ]
        params = [Parameter("arg1", "__u64"), Parameter("arg2", "__u64"), Parameter("arg3", "__u64")]
        # add upcall sysif tracepoint
        for method_name in upcall_methods:
            method = SyscallMethod(method_name, None, params, "int", False, None, None, False)
            uev_list += __gen_sysif_uev_points("fs", method_name, "", method)
    elif server.name == "devhost":
        upcall_methods = [
            "handle_irq_fastpath",
            "handle_rpccnt_put",
        ]
        params = [Parameter("arg1", "__u64"), Parameter("arg2", "__u64"), Parameter("arg3", "__u64")]
        # add upcall sysif tracepoint
        for method_name in upcall_methods:
            method = SyscallMethod(method_name, None, params, "int", False, None, None, False)
            uev_list += __gen_sysif_uev_points("fs", method_name, "", method)

    for manager in server.managers:
        for method in manager.syscalls:
            postfix = f'{server.handler_prefix}_{method.manager_name}_{method.name}'
            actv_ret_s = f'__actvret_{server.call_prefix}_{method.manager_name}_{method.name}'
            uev_list += __gen_sysif_uev_points(server.name, postfix, actv_ret_s, method)
        uev_list += __gen_lsyscall_sysif_uev_points(manager.general_lsyscalls)
        if manager.aarch64_lsyscalls:
            uev_list.append("#ifdef __aarch64__\n")
            uev_list += __gen_lsyscall_sysif_uev_points(manager.aarch64_lsyscalls)
            uev_list.append("#endif\n")
        if manager.arm_lsyscalls:
            aarch64_names = list(call.name for call in manager.aarch64_lsyscalls)
            filter_arm = list(lsyscall for lsyscall in manager.arm_lsyscalls if lsyscall.name not in aarch64_names)
            if filter_arm:
                uev_list.append("#ifdef __aarch64__\n")
                uev_list.append(f"#ifndef __sysif_server_{server.name}_no_compat32_handlers__\n")
                uev_list += __gen_lsyscall_sysif_uev_points(filter_arm)
                uev_list.append("#endif\n")
                uev_list.append("#endif\n")
            uev_list.append("#ifdef __arm__\n")
            uev_list += __gen_lsyscall_sysif_uev_points(manager.arm_lsyscalls)
            uev_list.append("#endif\n")
    return uev_list


def __gen_uev_templates() -> List[str]:
    """
    Generate uev enter point template and uev exit point template
    """
    enter_template = """
    UEV_POINT({svr_name}, {func_name},
    UEV_ARG({arg_num}, unsigned long long, sender, unsigned long, credential{param_list}),
    UEV_TRACE_FIELD({arg_num}, unsigned long long, sender, unsigned long, credential{param_list}),
    UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "{func_name}");
    uev_data_write(&trace_data, &sender, sizeof(unsigned long long));
    uev_data_write(&trace_data, &credential, sizeof(unsigned long));
    {uev_input_assign_list}),
    UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
    OUTPUT_HEX_NUM(unsigned long long, sender)
    OUTPUT_HEX_NUM(unsigned long, credential)
    {uev_input_efmt_list}))
    """

    exit_template = """
    UEV_POINT({svr_name}, {func_name},
    UEV_ARG(8, unsigned long long, sender, unsigned long, credential,
            {ret_typ}, ret, void *, __actv_ret,
            __u64, __ready_time, __u64, __block_time,
            __u32, __ready_cnt, __u32, __block_cnt),
    UEV_TRACE_FIELD(8, unsigned long long, sender, unsigned long, credential,
                    {ret_typ}, ret, void *, __actv_ret,
                    __u64, __ready_time, __u64, __block_time,
                    __u32, __ready_cnt, __u32, __block_cnt),
    UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "{func_name}");
    uev_data_write(&trace_data, &sender, sizeof(unsigned long long));
    uev_data_write(&trace_data, &credential, sizeof(unsigned long));
    uev_data_write(&trace_data, &ret, sizeof({ret_typ}));
    {uev_output_assign_list}
    uev_data_write(&trace_data, &__ready_time, sizeof(__u64));
    uev_data_write(&trace_data, &__block_time, sizeof(__u64));
    uev_data_write(&trace_data, &__ready_cnt, sizeof(__u32));
    uev_data_write(&trace_data, &__block_cnt, sizeof(__u32));),

    UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
    OUTPUT_HEX_NUM(unsigned long long, sender)
    OUTPUT_HEX_NUM(unsigned long, credential)
    {ret_map}({ret_typ}, ret)
    {uev_output_efmt_list}
    OUTPUT_NUM(__u64, ready_time)
    OUTPUT_NUM(__u64, block_time)
    OUTPUT_NUM(__u32, ready_cnt)
    OUTPUT_NUM(__u32, block_cnt)
    ))
    """
    return [enter_template, exit_template]


def __gen_lsyscall_sysif_uev_points(lsyscalls: List[LsyscallMethod]) -> List[str]:
    if not lsyscalls:
        return []

    if lsyscalls[0].manager is None or lsyscalls[0].manager.server is None:
        logger = get_logger("sysif.codegen.uev_gen")
        logger.error("method `%s` is invalid", lsyscalls[0].name)
        raise ValueError(f"method `{lsyscalls[0].name}` has no attached manager or server")

    server = lsyscalls[0].manager.server
    uev_list = []
    svr_name = server.name
    for method in lsyscalls:
        postfix = f'{server.handler_prefix}_{method.manager_name}_{method.name}'
        actv_ret_s = f'__actvret_{server.call_prefix}_{method.manager_name}_{method.name}'
        uev_list += __gen_sysif_uev_points(svr_name, postfix, actv_ret_s, method)
    return uev_list


def __gen_sysif_uev_points(svr_name: str, postfix: str, actv_ret_s: str,
                           method: Union[SyscallMethod, LsyscallMethod]) -> List[str]:
    """
    Generate uev enter point and exit point for a single method.
    """
    enter_t, exit_t = __gen_uev_templates()
    param_list = "".join(list(f",\n{p.parameter_type}, __{p.name}" for p in method.parameters))

    uev_input_assign_list = []
    uev_input_efmt_list = []
    for param in method.parameters:
        uev_input_assign_list.append(
            f"uev_data_write(&trace_data, &__{param.name}, sizeof({param.parameter_type}));")
        uev_input_efmt_list.append(
            f"{_trace_map(param.parameter_type)}({param.parameter_type}, {param.name})")

    uev_output_assign_list = []
    uev_output_efmt_list = []
    ret_params = method.indirect_returned_values
    if ret_params is not None:
        for param in ret_params:
            uev_output_assign_list.append(
                f"uev_data_write(&trace_data, &(((struct {actv_ret_s} *)__actv_ret)->{param.name}),\
                sizeof({param.parameter_type}));")
            uev_output_efmt_list.append(
                f"{_trace_map(param.parameter_type)}({param.parameter_type}, {param.name})")

    uev_points = []
    uev_points.append(enter_t.format(
        func_name=f"{postfix}_enter", svr_name=svr_name, arg_num=(len(method.parameters) + 2),
        param_list=param_list,
        uev_input_assign_list="\n".join(uev_input_assign_list),
        uev_input_efmt_list="\n".join(uev_input_efmt_list)))
    uev_points.append(exit_t.format(
        func_name=f"{postfix}_exit", svr_name=svr_name,
        ret_typ=method.direct_return_type, ret_map=_trace_map(method.direct_return_type),
        uev_output_assign_list="\n".join(uev_output_assign_list),
        uev_output_efmt_list="\n".join(uev_output_efmt_list)))

    return uev_points


def _trace_map(typ: CType) -> str:
    """
    Query TRACE_EFMT interface according to certain C type
    """
    trace_efmts = {
        'struct_union': 'OUTPUT_STRUCT',
        'pointer': 'OUTPUT_HEX_NUM',
        'num': 'OUTPUT_NUM'
    }

    if typ.endswith('*'):
        return trace_efmts.get('pointer', "")
    if typ.startswith('struct ') or typ.startswith('union '):
        return trace_efmts.get('struct_union', "")
    if typ.startswith('enum '):
        return trace_efmts.get('num', "")

    sysif_types = {
        'bool': 'num',
        'unsigned char': 'num',
        'unsigned': 'num',
        'unsigned short': 'num',
        'unsigned int': 'num',
        'unsigned long': 'num',
        'unsigned long long': 'num',
        'uint32_t': 'num',
        'uint64_t': 'num',
        'int64_t': 'num',
        'rref_t': 'num',
        'cref_t': 'num',
        'uref_t': 'num',
        'uintptr_t': 'num',
        '__u64': 'num',
        '__u32': 'num',
        'gid_t': 'num',
        'uid_t': 'num',
        'dev_t': 'num',
        'size_t': 'num',
        'mode_t': 'num',
        '__cpuset_t': 'struct_union',
        'xref_t': 'struct_union',
        'int': 'num',
        'long': 'num',
        'long long': 'num',
        'time_t': 'num',
        'pid_t': 'num',
        'clockid_t': 'num',
        'socklen_t': 'num',
    }

    sysif_type = trace_efmts.get(sysif_types.get(typ, ""), "")
    if not sysif_type:
        logger = get_logger("sysif.codegen.uev_gen")
        logger.error("unexpected sysif type %s for uev trace, add it to the sysif_types dict!", typ)
    return sysif_type
