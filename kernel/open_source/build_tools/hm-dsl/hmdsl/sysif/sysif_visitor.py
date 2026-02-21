#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Description: A sysif visitor to construct server object from parser
Author: Huawei OS Kernel Lab
Create: Thu Apr 14 10:21:59 2022
"""

from __future__ import absolute_import

from pathlib import Path
from typing import List, Dict
from sys import exit as sysexit

from antlr4 import InputStream, CommonTokenStream

from hmdsl.sysif.SysifGrammarLexer import SysifGrammarLexer
from hmdsl.sysif.SysifGrammarParser import SysifGrammarParser
from hmdsl.sysif.SysifGrammarVisitor import SysifGrammarVisitor
from hmdsl.sysif.model import Server, CCode, Manager, Parameter, SyscallMethod, LsyscallMethod, \
    general_call_macro_placeholder, valid_arch_macros
from hmdsl.common.error import DSLErrorListener
from hmdsl.common.logging import get_logger


logger = get_logger("sysif.sysif_visitor")


class SysifVisitor(SysifGrammarVisitor):
    """ A sysif dsl Visitor """

    def __init__(self, includes: List[str], legacy_mode: bool):
        self.includes = includes
        self.legacy_mode = legacy_mode

    @staticmethod
    def visitDeclare_datatype(ctx: SysifGrammarParser.Declare_datatypeContext):
        """ Visit a parse tree produced by SysifGrammarParser#declare_datatype."""
        # `DECLARE_DATATYPE_BEGIN` has length 22 and `DECLARE_DATATYPE_END` is 20 long.
        content = ctx.getText()[22:-20]
        datatype = CCode(content=content)
        return Server(datatypes=[datatype])

    @staticmethod
    def visitSysif_convention_code(ctx: SysifGrammarParser.Sysif_convention_codeContext):
        """ Visit a parse tree produced by SysifGrammarParser#sysif_convention_code."""
        content = ctx.getText()
        idx = content.find('SYSIF_EXPORT_API')
        content = content[(idx + 16):-6]
        datatype = CCode(content=content)
        return Server(datatypes=[datatype])

    @staticmethod
    def visitGen_weak_hdlr(__ctx: SysifGrammarParser.Gen_weak_hdlrContext):
        """ Visit a parse tree produced by SysifGrammarParser#gen_weak_hdlr."""
        return Server(generate_weak_handler=True)

    @staticmethod
    def visitPath(ctx: SysifGrammarParser.PathContext):
        """ Visit a parse tree produced by SysifGrammarParser#path."""
        count = ctx.getChildCount()
        path = ""
        for i in range(count):
            path += ctx.getChild(i).getText()
        return path

    @staticmethod
    def visitGroup_num(ctx: SysifGrammarParser.Group_numContext):
        """ Visit a parse tree produced by SysifGrammarParser#group_num."""
        group = ctx.INT().getText()
        return group

    @staticmethod
    def visitInfo(ctx: SysifGrammarParser.InfoContext):
        """ Visit a parse tree produced by SysifGrammarParser#info."""
        info = ctx.CNAME().getText()
        return info

    @staticmethod
    def visitMax_method_num(ctx: SysifGrammarParser.Max_method_numContext):
        """ Visit a parse tree produced by SysifGrammarParser#max_method_num."""
        capacity = int(ctx.INT().getText())
        return capacity

    @staticmethod
    def visitRet_int(__ctx: SysifGrammarParser.Ret_intContext):
        """ Visit a parse tree produced by SysifGrammarParser#ret_int."""
        return "int"

    @staticmethod
    def visitRet_int_none(__ctx: SysifGrammarParser.Ret_int_noneContext):
        """ Visit a parse tree produced by SysifGrammarParser#ret_int_none."""
        return "none"

    @staticmethod
    def visitRet_long(__ctx: SysifGrammarParser.Ret_longContext):
        """ Visit a parse tree produced by SysifGrammarParser#ret_long."""
        return "long"

    @staticmethod
    def visitRet_uint(__ctx: SysifGrammarParser.Ret_uintContext):
        """ Visit a parse tree produced by SysifGrammarParser#ret_uint."""
        return "unsigned int"

    @staticmethod
    def visitNormal(__ctx: SysifGrammarParser.NormalContext):
        """ Visit a parse tree produced by SysifGrammarParser#normal."""
        return ("int", False)

    @staticmethod
    def visitSimple(__ctx: SysifGrammarParser.SimpleContext):
        """ Visit a parse tree produced by SysifGrammarParser#simple."""
        return ("int", True)

    @staticmethod
    def visitUlong_normal(__ctx: SysifGrammarParser.Ulong_normalContext):
        """ Visit a parse tree produced by SysifGrammarParser#ulong_normal."""
        return ("unsigned long", False)

    @staticmethod
    def visitUlong_simple(__ctx: SysifGrammarParser.Ulong_simpleContext):
        """ Visit a parse tree produced by SysifGrammarParser#ulong_simple."""
        return ('unsigned long', True)

    @staticmethod
    def visitLong_normal(__ctx: SysifGrammarParser.Long_normalContext):
        """ Visit a parse tree produced by SysifGrammarParser#long_normal."""
        return ("long", False)

    @staticmethod
    def visitLong_simple(__ctx: SysifGrammarParser.Long_simpleContext):
        """ Visit a parse tree produced by SysifGrammarParser#long_simple."""
        return ('long', True)

    @staticmethod
    def visitNo_params(__ctx: SysifGrammarParser.No_paramsContext):
        """ Visit a parse tree produced by SysifGrammarParser#no_params."""
        return []

    @staticmethod
    def visitNo_ret(__ctx: SysifGrammarParser.No_retContext):
        """ Visit a parse tree produced by SysifGrammarParser#no_ret."""
        return None

    @staticmethod
    def visitIs_slowdata(__ctx: SysifGrammarParser.Is_slowdataContext):
        """ Visit a parse tree produced by SysifGrammarParser#is_slowdata."""
        return True

    @staticmethod
    def visitNot_slowdata(__ctx: SysifGrammarParser.Not_slowdataContext):
        """ Visit a parse tree produced by SysifGrammarParser#not_slowdata."""
        return False

    @staticmethod
    def visitNo_stk_check(__ctx: SysifGrammarParser.No_stk_checkContext):
        """ Visit a parse tree produced by SysifGrammarParser#no_stk_check."""
        return False

    @staticmethod
    def visitType_name(ctx: SysifGrammarParser.Type_nameContext):
        """ Visit a parse tree produced by SysifGrammarParser#type_name."""
        typ = " ".join(list(t.text for t in ctx.typ))
        return typ

    @staticmethod
    def visitParam_name(ctx: SysifGrammarParser.Param_nameContext):
        """ Visit a parse tree produced by SysifGrammarParser#param_name."""
        name = ctx.CNAME().getText()
        return name

    @staticmethod
    def visitManager_name(ctx: SysifGrammarParser.Manager_nameContext):
        """ Visit a parse tree produced by SysifGrammarParser#manager_name."""
        name = ctx.CNAME().getText()
        return name

    @staticmethod
    def visitSyscallnum(ctx: SysifGrammarParser.SyscallnumContext):
        """Visit a parse tree produced by SysifGrammarParser#syscallnum."""
        syscallnum = ctx.CNAME().getText()
        return syscallnum

    @staticmethod
    def visitArch_name(ctx: SysifGrammarParser.Arch_nameContext):
        """Visit a parse tree produced by SysifGrammarParser#arch_name."""
        arch_macro = ctx.CNAME().getText()
        return arch_macro

    @staticmethod
    def visitMethod_name(ctx: SysifGrammarParser.Method_nameContext):
        """Visit a parse tree produced by SysifGrammarParser#method_name."""
        name = ctx.CNAME().getText()
        return name

    def visitStart(self, ctx: SysifGrammarParser.StartContext):
        """Visit a parse tree produced by SysifGrammarParser#start."""
        server = Server()
        for i in ctx.item_list:
            server.update(self.visit(i))
        return server

    def visitItem(self, ctx: SysifGrammarParser.ItemContext):
        """ Visit a parse tree produced by SysifGrammarParser#item."""
        return self.visitChildren(ctx)

    def visitInclude_header(self, ctx: SysifGrammarParser.Include_headerContext):
        """ Visit a parse tree produced by SysifGrammarParser#include_header."""
        path = self.visit(ctx.path())
        header = CCode(content=path)
        return Server(headers=[header])

    def visitCall_prefix(self, ctx: SysifGrammarParser.Call_prefixContext):
        """ Visit a parse tree produced by SysifGrammarParser#call_prefix."""
        prefix = self.visit(ctx.info())
        return Server(call_prefix=prefix)

    def visitHdlr_prefix(self, ctx: SysifGrammarParser.Hdlr_prefixContext):
        """ Visit a parse tree produced by SysifGrammarParser#hdlr_prefix."""
        prefix = self.visit(ctx.info())
        return Server(handler_prefix=prefix)

    def visitServer_name(self, ctx: SysifGrammarParser.Server_nameContext):
        """ Visit a parse tree produced by SysifGrammarParser#server_name."""
        name = self.visit(ctx.info())
        return Server(name=name)

    def visitSubsystem(self, ctx: SysifGrammarParser.SubsystemContext):
        """ Visit a parse tree produced by SysifGrammarParser#subsystem."""
        path = self.visit(ctx.path())
        return self.__visit_file(path)

    def visitGroup(self, ctx: SysifGrammarParser.GroupContext):
        """ Visit a parse tree produced by SysifGrammarParser#group."""
        group_num = self.visit(ctx.group_num())
        server = Server(group=group_num)
        return server

    def visitRo_after_init_postponed(self, ctx: SysifGrammarParser.Ro_after_init_postponedContext):
        """ Visit a parse tree produced by SysifGrammarParser#ro_after_init_postponed."""
        return Server(ro_after_init_postponed=True)

    def visitManager(self, ctx: SysifGrammarParser.ManagerContext):
        """ Visit a parse tree produced by SysifGrammarParser#manager. """
        name = self.visit(ctx.manager_name())
        capacity = self.visit(ctx.max_method_num())
        syscalls = []
        lsyscall_map: Dict[str, List[LsyscallMethod]] = dict()
        for method in ctx.method_list:
            call = self.visit(method)
            if isinstance(call, SyscallMethod):
                syscalls.append(call)
            elif isinstance(call, LsyscallMethod):
                if call.arch_macro is None:
                    lsyscall_map.setdefault(general_call_macro_placeholder(), [])
                    lsyscall_map.get(general_call_macro_placeholder(), []).append(call)
                elif call.arch_macro not in valid_arch_macros():
                    logger.error("arch_macro: [%s] is not supported yet", call.arch_macro)
                    sysexit(1)
                else:
                    lsyscall_map.setdefault(call.arch_macro, [])
                    lsyscall_map.get(call.arch_macro, []).append(call)

        general_calls_num = len(syscalls) + len(lsyscall_map.get(general_call_macro_placeholder(), []))
        if len(set(lsyscall_map.keys()) & valid_arch_macros()) <= 0 and general_calls_num > capacity:
            logger.error("manager `%s`'s methods exceed it's capacity of `%s`", name, str(capacity))
            sysexit(1)

        if len(set(lsyscall_map.keys() & valid_arch_macros())) > 0:
            for macro, calls in lsyscall_map.items():
                if macro != general_call_macro_placeholder() and general_calls_num + len(calls) > capacity:
                    logger.error("manager `%s`'s methods exceed it's capacity of `%s`", name, str(capacity))
                    sysexit(1)

        mgr = Manager(name, None, capacity, syscalls, lsyscall_map)
        for method in mgr.methods:
            method.manager = mgr

        # a general lsyscall cannot be redefined in arm lsyscall or aarch64 lsyscall
        redefined_names = \
            set(x.name for x in mgr.general_lsyscalls) & (
                set(x.name for x in mgr.aarch64_lsyscalls) |
                set(x.name for x in mgr.arm_lsyscalls)
            )

        for redefined_name in redefined_names:
            logger.error("lsyscall `%s` is redefined with and without an `ARCH`", redefined_name)
            sysexit(1)

        return Server(managers=[mgr])

    def visitSyscall(self, ctx: SysifGrammarParser.SyscallContext):
        """ Visit a parse tree produced by SysifGrammarParser#syscall."""
        direct_return_type, support_self_call = self.visit(ctx.method_type())
        mgr_name, name, params, indirect_returned_values, slow_data, stk_check = \
            self.visit(ctx.method_body())
        if self.legacy_mode:
            mthd = SyscallMethod(name, None, params, direct_return_type, support_self_call,
                                 mgr_name, indirect_returned_values, slow_data, stk_check)
        else:
            mthd = SyscallMethod(name, None, params, direct_return_type, support_self_call,
                                 None, indirect_returned_values, slow_data, stk_check)
        return mthd

    def visitLsyscall(self, ctx: SysifGrammarParser.LsyscallContext):
        """ Visit a parse tree produced by SysifGrammarParser#lsyscall."""
        name, is_native, has_selfcall, syscallnum, params, macro_name, stk_check = self.visit(ctx.lsyscall_body())
        if len(params) < 0 or len(params) > 8:
            logger.error("lsyscall `%s` has %s args, but it should be in the range [0, 7]", name, str(len(params)))
            sysexit(1)
        direct_return_type = self.visit(ctx.lsyscall_type())
        map_none = False
        if direct_return_type == "none":
            map_none = True
            direct_return_type = "int"
        mthd = LsyscallMethod(name, None, params, direct_return_type, has_selfcall,
                              syscallnum, is_native, macro_name, stk_check, map_none)
        return mthd

    def visitMethod_body(self, ctx: SysifGrammarParser.Method_bodyContext):
        """ Visit a parse tree produced by SysifGrammarParser#method_body."""
        manager_name = self.visit(ctx.manager_name())
        method_name = self.visit(ctx.method_name())
        params = self.visit(ctx.params_def())
        ret = self.visit(ctx.ret_def())
        slowdata = self.visit(ctx.slowdata_def())
        if ctx.no_stk_check() is None:
            # no_arg sysif not check
            stk_check = len(params) > 0
        else:
            stk_check = self.visit(ctx.no_stk_check())

        body = [manager_name, method_name, params, ret, slowdata, stk_check]
        return body

    def visitLsyscall_body(self, ctx: SysifGrammarParser.Lsyscall_bodyContext):
        """ Visit a parse tree produced by SysifGrammarParser#lsyscall_body."""
        method_name = self.visit(ctx.method_name())
        is_native = ctx.is_native() is not None
        has_selfcall = ctx.has_selfcall() is not None
        syscallnum = self.visit(ctx.syscallnum())
        params = self.visit(ctx.params_def())
        macro_name = None if ctx.archi() is None else self.visit(ctx.archi())
        if ctx.no_stk_check() is None:
            # no_arg sysif not check
            stk_check = len(params) > 0
        else:
            stk_check = self.visit(ctx.no_stk_check())

        body = [method_name, is_native, has_selfcall, syscallnum, params, macro_name, stk_check]
        return body

    def visitParams_list(self, ctx: SysifGrammarParser.Params_listContext):
        """ Visit a parse tree produced by SysifGrammarParser#params_list."""
        params = list(self.visit(p) for p in ctx.param_list)
        arg_num = int(ctx.INT().getText())
        if len(params) <= 0 or len(params) != arg_num:
            logger.error("ARG list `%s` has incorrect number %s", ' '.join([str(p) for p in params]), str(arg_num))
            sysexit(1)

        return params

    def visitRet_list(self, ctx: SysifGrammarParser.Ret_listContext):
        """ Visit a parse tree produced by SysifGrammarParser#ret_list."""
        params = list(self.visit(p) for p in ctx.param_list)
        arg_num = int(ctx.INT().getText())
        if len(params) <= 0 or len(params) != arg_num:
            logger.error("RET list `%s` has incorrect number %s", ' '.join([str(p) for p in params]), str(arg_num))
            sysexit(1)

        return params

    def visitArchi(self, ctx: SysifGrammarParser.ArchiContext):
        """ Visit a parse tree produced by SysifGrammarParser#archi."""
        return self.visit(ctx.arch_name())

    def visitParameter(self, ctx: SysifGrammarParser.ParameterContext):
        """ Visit a parse tree produced by SysifGrammarParser#parameter."""
        param = Parameter(name=self.visit(ctx.param_name()),
                          parameter_type=self.visit(ctx.type_name())
                          )
        return param

    def __visit_file(self, source_file: str):
        path = ""
        for include in self.includes:
            if Path(f"{include}/{source_file}").is_file():
                path = f"{include}/{source_file}"
                break
        else:
            logger.error("Cannot find `%s` in the includes!", source_file)
            sysexit(1)

        with open(path, 'r') as file_hd:
            txt = file_hd.read()

        tree = parse_to_tree(txt)
        return self.visit(tree)


def parse_to_tree(txt):
    """
    Parse texts to grammar tree using defined sysif dsl grammar
    """
    input_stream = InputStream(txt)
    lexer = SysifGrammarLexer(input_stream)
    lexer.removeErrorListeners()
    lexer.addErrorListener(DSLErrorListener())
    stream = CommonTokenStream(lexer)
    parser = SysifGrammarParser(stream)
    parser.removeErrorListeners()
    parser.addErrorListener(DSLErrorListener())
    tree = parser.start()
    return tree
