/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Macro
 * Author: Huawei OS Kernel Lab
 * Create: Web Jan 15 15:15:57 2025
 */

#ifndef UAPI_HMKERNEL_LITE_HOOK
#define UAPI_HMKERNEL_LITE_HOOK

#define ___hm_lite_hook_concat(a, b) a ## b
#define __hm_lite_hook_concat(a, b) ___hm_lite_hook_concat(a, b)
#define ___hm_lite_hook_get_arg2(arg1, arg2, ...) arg2
#define __hm_lite_hook_get_arg2(...) ___hm_lite_hook_get_arg2(__VA_ARGS__)

#define __hm_lite_test_config_1 dummy,
#define __hm_lite_hook_calc_config_01(_conf)		\
	__hm_lite_hook_get_arg2(__hm_lite_hook_concat(__hm_lite_test_config_, _conf) 1, 0)
#define __hm_lite_hook_config_gen_ops(_conf, _ops)	\
	__hm_lite_hook_concat(_ops, __hm_lite_hook_calc_config_01(_conf))

#define __hm_lite_hook_emit_code1(_code1, _)  _code1
#define __hm_lite_hook_emit_code0(_, _code0)  _code0
#define ___hm_lite_hook_if_def(_conf, _code1, _code0)					\
	__hm_lite_hook_config_gen_ops(_conf, __hm_lite_hook_emit_code)(_code1, _code0)

#define __hm_lite_hook_if_def(_conf, _code, ...)		\
	___hm_lite_hook_if_def(_conf, _code, __VA_ARGS__)

#define __hm_lite_hook_else

#define ___hm_lite_hook_get_arg1(arg1, ...) arg1
#define __hm_lite_hook_get_arg1(...) __hm_lite_hook_get_arg1(__VA_ARGS__, )


#define __HM_LITE_HOOK_PROTO(...) __VA_ARGS__
#define __HM_LITE_HOOK_ARGS(...)  __VA_ARGS__

#define __hm_lite_hook_unused(...)
#define __hm_lite_hook_prefix(func_name) __hook_##func_name
#define __hm_lite_hook_declare_extern(func_name, ret_type, proto)	\
extern ret_type func_name(proto);

#define __hm_lite_hook_declare_empty(func_name, ret_type, default_ret, proto, args)	\
static ret_type inline func_name(proto) {						\
	__hm_lite_hook_unused(args);							\
	return default_ret;								\
}

#define ___HM_LITE_HOOK_DECLARE(config, func_name, ret_type, default_ret, proto, args)	\
__hm_lite_hook_if_def(config,								\
	__hm_lite_hook_declare_extern(func_name, ret_type,				\
				       __HM_LITE_HOOK_PROTO(proto))			\
	__hm_lite_hook_else,								\
	__hm_lite_hook_declare_empty(func_name, ret_type, default_ret,			\
				      __HM_LITE_HOOK_PROTO(proto),			\
				      __HM_LITE_HOOK_ARGS(args))			\
)

#define __HM_LITE_HOOK_DECLARE(config, func_name, ret_type, default_ret, proto, args)	\
	___HM_LITE_HOOK_DECLARE(config, __hm_lite_hook_prefix(func_name),		\
				ret_type, default_ret,					\
				__HM_LITE_HOOK_PROTO(proto), __HM_LITE_HOOK_ARGS(args))

#define __HM_LITE_HOOK_DEFINE(func_name, ret_type, ...)		\
       ret_type __hm_lite_hook_prefix(func_name)(__VA_ARGS__)\

#define __HM_LITE_HOOK_CALL(func_name, ...)		\
	__hm_lite_hook_prefix(func_name)(__VA_ARGS__)	\

#endif
