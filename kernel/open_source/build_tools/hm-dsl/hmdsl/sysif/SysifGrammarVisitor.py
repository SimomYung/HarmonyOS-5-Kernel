# Generated from /usr1/hmos_for_chipset/src/increment/sourcecode/out/generic_generic_arm_64only/general_7525L_phone_standard/hongmeng/build/tmp/work/x86_64-linux/hm-dsl-native/git-r0/image/usr1/hmos_for_chipset/src/increment/sourcecode/out/generic_generic_arm_64only/general_7525L_phone_standard/hongmeng/build/tmp/work/x86_64-linux/hm-dsl-native/git-r0/recipe-sysroot-native/usr/bin/hm-dsl/hmdsl/sysif/SysifGrammar.g4 by ANTLR 4.13.0
from antlr4 import *
if "." in __name__:
    from .SysifGrammarParser import SysifGrammarParser
else:
    from SysifGrammarParser import SysifGrammarParser

# This class defines a complete generic visitor for a parse tree produced by SysifGrammarParser.

class SysifGrammarVisitor(ParseTreeVisitor):

    # Visit a parse tree produced by SysifGrammarParser#start.
    def visitStart(self, ctx:SysifGrammarParser.StartContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#item.
    def visitItem(self, ctx:SysifGrammarParser.ItemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#include_header.
    def visitInclude_header(self, ctx:SysifGrammarParser.Include_headerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#declare_datatype.
    def visitDeclare_datatype(self, ctx:SysifGrammarParser.Declare_datatypeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#sysif_convention_code.
    def visitSysif_convention_code(self, ctx:SysifGrammarParser.Sysif_convention_codeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#call_prefix.
    def visitCall_prefix(self, ctx:SysifGrammarParser.Call_prefixContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#hdlr_prefix.
    def visitHdlr_prefix(self, ctx:SysifGrammarParser.Hdlr_prefixContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#server_name.
    def visitServer_name(self, ctx:SysifGrammarParser.Server_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#subsystem.
    def visitSubsystem(self, ctx:SysifGrammarParser.SubsystemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#gen_weak_hdlr.
    def visitGen_weak_hdlr(self, ctx:SysifGrammarParser.Gen_weak_hdlrContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#group.
    def visitGroup(self, ctx:SysifGrammarParser.GroupContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ro_after_init_postponed.
    def visitRo_after_init_postponed(self, ctx:SysifGrammarParser.Ro_after_init_postponedContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#path.
    def visitPath(self, ctx:SysifGrammarParser.PathContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#group_num.
    def visitGroup_num(self, ctx:SysifGrammarParser.Group_numContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#info.
    def visitInfo(self, ctx:SysifGrammarParser.InfoContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#manager.
    def visitManager(self, ctx:SysifGrammarParser.ManagerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#max_method_num.
    def visitMax_method_num(self, ctx:SysifGrammarParser.Max_method_numContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#syscall.
    def visitSyscall(self, ctx:SysifGrammarParser.SyscallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#lsyscall.
    def visitLsyscall(self, ctx:SysifGrammarParser.LsyscallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#lsyscall_body.
    def visitLsyscall_body(self, ctx:SysifGrammarParser.Lsyscall_bodyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#archi.
    def visitArchi(self, ctx:SysifGrammarParser.ArchiContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ret_int.
    def visitRet_int(self, ctx:SysifGrammarParser.Ret_intContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ret_int_none.
    def visitRet_int_none(self, ctx:SysifGrammarParser.Ret_int_noneContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ret_long.
    def visitRet_long(self, ctx:SysifGrammarParser.Ret_longContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ret_uint.
    def visitRet_uint(self, ctx:SysifGrammarParser.Ret_uintContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#normal.
    def visitNormal(self, ctx:SysifGrammarParser.NormalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#simple.
    def visitSimple(self, ctx:SysifGrammarParser.SimpleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ulong_normal.
    def visitUlong_normal(self, ctx:SysifGrammarParser.Ulong_normalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ulong_simple.
    def visitUlong_simple(self, ctx:SysifGrammarParser.Ulong_simpleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#long_normal.
    def visitLong_normal(self, ctx:SysifGrammarParser.Long_normalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#long_simple.
    def visitLong_simple(self, ctx:SysifGrammarParser.Long_simpleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#method_body.
    def visitMethod_body(self, ctx:SysifGrammarParser.Method_bodyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#params_list.
    def visitParams_list(self, ctx:SysifGrammarParser.Params_listContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#no_params.
    def visitNo_params(self, ctx:SysifGrammarParser.No_paramsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#ret_list.
    def visitRet_list(self, ctx:SysifGrammarParser.Ret_listContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#no_ret.
    def visitNo_ret(self, ctx:SysifGrammarParser.No_retContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#is_slowdata.
    def visitIs_slowdata(self, ctx:SysifGrammarParser.Is_slowdataContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#not_slowdata.
    def visitNot_slowdata(self, ctx:SysifGrammarParser.Not_slowdataContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#no_stk_check.
    def visitNo_stk_check(self, ctx:SysifGrammarParser.No_stk_checkContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#is_native.
    def visitIs_native(self, ctx:SysifGrammarParser.Is_nativeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#has_selfcall.
    def visitHas_selfcall(self, ctx:SysifGrammarParser.Has_selfcallContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#parameter.
    def visitParameter(self, ctx:SysifGrammarParser.ParameterContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#type_name.
    def visitType_name(self, ctx:SysifGrammarParser.Type_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#param_name.
    def visitParam_name(self, ctx:SysifGrammarParser.Param_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#manager_name.
    def visitManager_name(self, ctx:SysifGrammarParser.Manager_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#method_name.
    def visitMethod_name(self, ctx:SysifGrammarParser.Method_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#syscallnum.
    def visitSyscallnum(self, ctx:SysifGrammarParser.SyscallnumContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by SysifGrammarParser#arch_name.
    def visitArch_name(self, ctx:SysifGrammarParser.Arch_nameContext):
        return self.visitChildren(ctx)



del SysifGrammarParser