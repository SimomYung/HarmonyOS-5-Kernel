/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Sysif dsl grammar
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 15 17:09:31 2022
 */

grammar SysifGrammar;

start : item_list+=item* EOF;

item : server_info
     | manager
     | c_item
     ;

/** Here comes grammar for C style items in sysif DSL.
 * We design the first two rules for users to include C headers or define their own types
 * when developing sysif methods. But obviously, nobody knows them and nobody uses them for now.
 * Thus the third rule uses the old macro `SYSIF_EXPORT_API` as a convention code,
 * which accomplishes the task where the first two rules were designed for AND
 * also compats forward for existing servers' sysif methods. */
c_item : 'INCLUDE_C_HEADER' '(' path ')' # include_header
       | DECLARE_DATATYPE # declare_datatype
       | SYSIF_DECLARE_DATATYPE # sysif_convention_code
       ;
DECLARE_DATATYPE: 'DECLARE_DATATYPE_BEGIN' .*? 'DECLARE_DATATYPE_END';

/** This rule use `SYSIF_EXPORT_API` as a convention code in .sysif files.
 * Those C codes appear inside this macro will be put inside the generated sysif headers files. */
SYSIF_DECLARE_DATATYPE : '#ifdef' [ ]*? 'SYSIF_EXPORT_API' .*? '#endif';

/** sysif DSL grammar */
server_info : 'USE_CALL_PREFIX' '(' info ')' # call_prefix
            | 'USE_HANDLER_PREFIX' '(' info ')' # hdlr_prefix
            | 'USE_SERVER_NAME' '(' info ')' # server_name
            | 'INCLUDE_SUBSYSTEM' '(' path ')' # subsystem
            | 'GEN_WEAK_HDLR' # gen_weak_hdlr
            | 'GROUP' '(' group_num ')' # group
            | 'RO_AFTER_INIT_POSTPONED' # ro_after_init_postponed
            ;

path : (CNAME SLASH)* CNAME (EXTEND CNAME)*;
group_num : INT;
info : CNAME;

manager : 'DEFINE_MANAGER' '(' manager_name ',' max_method_num ',' method_list+=method+ ')';
max_method_num : INT;

method : method_type '(' method_body ')'                 # syscall
       | lsyscall_type '(' lsyscall_body ')'             # lsyscall
       ;

lsyscall_body : method_name ',' (is_native ',' (has_selfcall ',')?)? syscallnum ',' params_def (',' archi)? (',' no_stk_check)?;
archi : 'ARCH' '(' arch_name ')';

lsyscall_type: 'DEFINE_LSYSCALL_METHOD' # ret_int
             | 'DEFINE_LSYSCALL_METHOD_NONE' # ret_int_none
             | 'DEFINE_LSYSCALL_METHOD_LONG' # ret_long
             | 'DEFINE_LSYSCALL_METHOD_UINT' # ret_uint
             ;

method_type : 'DEFINE_METHOD' # normal
            | 'DEFINE_SIMPLE_METHOD' # simple
            | 'DEFINE_METHOD_ULONG' # ulong_normal
            | 'DEFINE_SIMPLE_METHOD_ULONG' # ulong_simple
            | 'DEFINE_METHOD_LONG' # long_normal
            | 'DEFINE_SIMPLE_METHOD_LONG' # long_simple
            ;
method_body : manager_name ',' method_name ',' params_def ',' ret_def ',' slowdata_def (',' no_stk_check)?;
params_def : 'ARG' '(' INT ',' param_list+=parameter (',' param_list+=parameter)* ')' # params_list
           | 'ARG_NO' # no_params
           ;
ret_def : 'RET' '(' INT ',' param_list+=parameter (',' param_list+=parameter)* ')' # ret_list
        | 'RET_NO' # no_ret
        ;
slowdata_def : 'SLOWDATA' # is_slowdata
             | 'SLOWDATA_NO' # not_slowdata
             ;
no_stk_check : 'NO_STK_CHECK';
is_native : 'NATIVE';
has_selfcall : 'SELFCALL';

parameter : type_name ',' param_name;
type_name : typ+=CNAME+ typ+=POINTER*;
param_name : CNAME;

manager_name : CNAME;
method_name : CNAME;
syscallnum : CNAME;
arch_name : CNAME;

POINTER : '*';
SLASH : '/';
EXTEND : '.';
CNAME : ( '_' | LETTER ) ( '_' | LETTER | DIGIT )*;
LETTER : [a-zA-Z];
INT : DIGIT+;
fragment DIGIT : [0-9];
/** Comments inside .sysif are dropped for now.
 * But in case there might be any usage in the future, we still put them in HIDDEN channel. */
COMMENT : ('//' .*? '\n' | '/*' .*? '*/') -> channel(HIDDEN);
WS : [ \t\f\r\n]+ -> skip;
