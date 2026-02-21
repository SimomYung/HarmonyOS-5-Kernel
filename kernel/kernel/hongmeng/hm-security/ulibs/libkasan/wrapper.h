/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Macro used to generate wrap function
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 16 11:49:29 2018
 */

#if !defined(KASAN_WRAPPER_DEFINE) && !defined(KASAN_WRAPPER_DECL)
# error Must be included from wrapper_xx_decl.h or wrapper_xx_define.h
#endif

#if defined(KASAN_WRAPPER_DEFINE) && defined(KASAN_WRAPPER_DECL)
# error Must be included from wrapper_xx_decl.h or wrapper_xx_define.h
#endif

#include "para_map.h"
#include <stddef.h>
#include <libhwsecurec/securec.h>

#ifdef KASAN_WRAPPER_DEFINE
#ifdef KASAN_SHARED
#define KASAN_WRAPPER(ret_type, name, ARG) \
ret_type ori_##name(_PARA(PACK(ARG))); \
void kasan_reg_##name(void);

#define KASAN_WRAPPER_VOID(name, ARG) \
void ori_##name(_PARA(PACK(ARG))); \
void kasan_reg_##name(void);
#else
#define KASAN_WRAPPER(ret_type, name, ARG) \
ret_type __wrap_##name(_PARA(PACK(ARG))); \
ret_type __real_##name(_PARA(PACK(ARG))); \
ret_type ori_##name(_PARA(PACK(ARG))) \
{ \
	return __real_##name(_ARGS(PACK(ARG))); \
}

#define KASAN_WRAPPER_VOID(name, ARG) \
void __wrap_##name(_PARA(PACK(ARG))); \
void __real_##name(_PARA(PACK(ARG))); \
void ori_##name(_PARA(PACK(ARG))) \
{ \
	__real_##name(_ARGS(PACK(ARG))); \
	return; \
}

#endif

#define KASAN_WRAPPER_NODECL KASAN_WRAPPER

#else /* else of ifdef KASAN_WRAPPER_DEFINE */

#define KASAN_WRAPPER(ret_type, name, ARG) \
ret_type name(_PARA(PACK(ARG))); \
ret_type ori_##name(_PARA(PACK(ARG))); \
void kasan_reg_##name(void);

#define KASAN_WRAPPER_VOID(name, ARG) \
void name(_PARA(PACK(ARG))); \
void ori_##name(_PARA(PACK(ARG))); \
void kasan_reg_##name(void);

#define KASAN_WRAPPER_NODECL(ret_type, name, ARG) \
ret_type ori_##name(_PARA(PACK(ARG)));

#endif /* end of ifdef KASAN_WRAPPER_DEFINE */

#include KASAN_WRAPPER_INCL

#undef KASAN_WRAPPER_DEFINE
#undef KASAN_WRAPPER_DECL
#undef KASAN_WRAPPER_INCL
#undef KASAN_WRAPPER
#undef KASAN_WRAPPER_NODECL
#undef KASAN_WRAPPER_VOID
