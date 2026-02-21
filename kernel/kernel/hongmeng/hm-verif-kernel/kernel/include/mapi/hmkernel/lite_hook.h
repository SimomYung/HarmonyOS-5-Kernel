/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Macro
 * Author: Huawei OS Kernel Lab
 * Create: Web Jan 17 15:26:57 2025
 */

#ifndef MAPI_HMKERNEL_LITE_HOOK
#define MAPI_HMKERNEL_LITE_HOOK

#include <uapi/hmkernel/lite_hook.h>

#define LH_PROTO(...) __HM_LITE_HOOK_PROTO(__VA_ARGS__)
#define LH_ARGS(...)  __HM_LITE_HOOK_ARGS(__VA_ARGS__)

#define LITE_HOOK_DECLARE(func_name, ret_type, default_ret, proto, args)	\
	__HM_LITE_HOOK_DECLARE(CONFIG_HM_LITE_HOOK, func_name,			\
			       ret_type, default_ret,				\
			       LH_PROTO(proto), LH_ARGS(args))

#define LITE_HOOK_DECLARE_VOID(func_name, proto, args)				\
	__HM_LITE_HOOK_DECLARE(CONFIG_HM_LITE_HOOK, func_name, void, ,		\
			       LH_PROTO(proto), LH_ARGS(args))

#define LITE_HOOK_DEFINE(func_name, ret_type, ...)		\
	__HM_LITE_HOOK_DEFINE(func_name, ret_type, __VA_ARGS__)

#define LITE_HOOK_DEFINE_VOID(func_name, ...)			\
	__HM_LITE_HOOK_DEFINE(func_name, void, __VA_ARGS__)

#define LITE_HOOK_CALL(func_name, ...)			\
	__HM_LITE_HOOK_CALL(func_name, __VA_ARGS__)

#endif
