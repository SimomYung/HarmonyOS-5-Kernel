/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure check methods head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:19:35 2019
 */
#ifndef ULIBS_LIBSEC_CHK_H
#define ULIBS_LIBSEC_CHK_H

#ifndef __HOST_LLT__
#include <stdbool.h>
#endif
// libc
#include <stdlib.h>
// hongmeng
#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libsysif/utils.h>
// security
#include "chk_cred.h"
#include "server_env.h"

struct sec_chk_point {
	struct dlist_node	list;
#ifdef NEED_SECURITY_FRAME_OVERLAY
	struct dlist_node	overlay;
#endif
};

struct sec_chk_method_common {
	struct dlist_node	node;
	struct dlist_node	*head;
};

struct sec_chk_method {
	struct sec_chk_method_common	hook;
	void				*func;
};

#define sec_method_num(...)	_sec_method_num(__VA_ARGS__, 96, 95, 94, 93, 92, 91, 90,	\
						89, 88, 87, 86, 85, 84, 83, 82, 81, 80,		\
						79, 78, 77, 76, 75, 74, 73, 72, 71, 70,		\
						69, 68, 67, 66, 65, 64, 63, 62, 61, 60,		\
						59, 58, 57, 56, 55, 54, 53, 52, 51, 50,		\
						49, 48, 47, 46, 45, 44, 43, 42, 41, 40,		\
						39, 38, 37, 36, 35, 34, 33, 32, 31, 30,		\
						29, 28, 27, 26, 25, 24, 23, 22, 21, 20,		\
						19, 18, 17, 16, 15, 14, 13, 12, 11, 10,		\
						9, 8, 7, 6, 5, 4, 3, 2, 1)

#define _sec_method_num(_, _1, _2, _3, _4, _5, _6, _7, _8, _9,			\
			_10, _11, _12, _13, _14, _15, _16, _17, _18, _19,	\
			_20, _21, _22, _23, _24, _25, _26, _27, _28, _29,	\
			_30, _31, _32, _33, _34, _35, _36, _37, _38, _39,	\
			_40, _41, _42, _43, _44, _45, _46, _47, _48, _49,	\
			_50, _51, _52, _53, _54, _55, _56, _57, _58, _59,	\
			_60, _61, _62, _63, _64, _65, _66, _67, _68, _69,	\
			_70, _71, _72, _73, _74, _75, _76, _77, _78, _79,	\
			_80, _81, _82, _83, _84, _85, _86, _87, _88, _89,	\
			_90, _91, _92, _93, _94, _95, N, ...) N

#define sec_method_exp_1(ctx) ctx
#define sec_method_exp_2(ctx, ...) ctx; sec_method_exp_1(__VA_ARGS__)
#define sec_method_exp_3(ctx, ...) ctx; sec_method_exp_2(__VA_ARGS__)
#define sec_method_exp_4(ctx, ...) ctx; sec_method_exp_3(__VA_ARGS__)
#define sec_method_exp_5(ctx, ...) ctx; sec_method_exp_4(__VA_ARGS__)
#define sec_method_exp_6(ctx, ...) ctx; sec_method_exp_5(__VA_ARGS__)
#define sec_method_exp_7(ctx, ...) ctx; sec_method_exp_6(__VA_ARGS__)
#define sec_method_exp_8(ctx, ...) ctx; sec_method_exp_7(__VA_ARGS__)
#define sec_method_exp_9(ctx, ...) ctx; sec_method_exp_8(__VA_ARGS__)
#define sec_method_exp_10(ctx, ...) ctx; sec_method_exp_9(__VA_ARGS__)
#define sec_method_exp_11(ctx, ...) ctx; sec_method_exp_10(__VA_ARGS__)
#define sec_method_exp_12(ctx, ...) ctx; sec_method_exp_11(__VA_ARGS__)
#define sec_method_exp_13(ctx, ...) ctx; sec_method_exp_12(__VA_ARGS__)
#define sec_method_exp_14(ctx, ...) ctx; sec_method_exp_13(__VA_ARGS__)
#define sec_method_exp_15(ctx, ...) ctx; sec_method_exp_14(__VA_ARGS__)
#define sec_method_exp_16(ctx, ...) ctx; sec_method_exp_15(__VA_ARGS__)
#define sec_method_exp_17(ctx, ...) ctx; sec_method_exp_16(__VA_ARGS__)
#define sec_method_exp_18(ctx, ...) ctx; sec_method_exp_17(__VA_ARGS__)
#define sec_method_exp_19(ctx, ...) ctx; sec_method_exp_18(__VA_ARGS__)
#define sec_method_exp_20(ctx, ...) ctx; sec_method_exp_19(__VA_ARGS__)
#define sec_method_exp_21(ctx, ...) ctx; sec_method_exp_20(__VA_ARGS__)
#define sec_method_exp_22(ctx, ...) ctx; sec_method_exp_21(__VA_ARGS__)
#define sec_method_exp_23(ctx, ...) ctx; sec_method_exp_22(__VA_ARGS__)
#define sec_method_exp_24(ctx, ...) ctx; sec_method_exp_23(__VA_ARGS__)
#define sec_method_exp_25(ctx, ...) ctx; sec_method_exp_24(__VA_ARGS__)
#define sec_method_exp_26(ctx, ...) ctx; sec_method_exp_25(__VA_ARGS__)
#define sec_method_exp_27(ctx, ...) ctx; sec_method_exp_26(__VA_ARGS__)
#define sec_method_exp_28(ctx, ...) ctx; sec_method_exp_27(__VA_ARGS__)
#define sec_method_exp_29(ctx, ...) ctx; sec_method_exp_28(__VA_ARGS__)
#define sec_method_exp_30(ctx, ...) ctx; sec_method_exp_29(__VA_ARGS__)
#define sec_method_exp_31(ctx, ...) ctx; sec_method_exp_30(__VA_ARGS__)
#define sec_method_exp_32(ctx, ...) ctx; sec_method_exp_31(__VA_ARGS__)
#define sec_method_exp_33(ctx, ...) ctx; sec_method_exp_32(__VA_ARGS__)
#define sec_method_exp_34(ctx, ...) ctx; sec_method_exp_33(__VA_ARGS__)
#define sec_method_exp_35(ctx, ...) ctx; sec_method_exp_34(__VA_ARGS__)
#define sec_method_exp_36(ctx, ...) ctx; sec_method_exp_35(__VA_ARGS__)
#define sec_method_exp_37(ctx, ...) ctx; sec_method_exp_36(__VA_ARGS__)
#define sec_method_exp_38(ctx, ...) ctx; sec_method_exp_37(__VA_ARGS__)
#define sec_method_exp_39(ctx, ...) ctx; sec_method_exp_38(__VA_ARGS__)
#define sec_method_exp_40(ctx, ...) ctx; sec_method_exp_39(__VA_ARGS__)
#define sec_method_exp_41(ctx, ...) ctx; sec_method_exp_40(__VA_ARGS__)
#define sec_method_exp_42(ctx, ...) ctx; sec_method_exp_41(__VA_ARGS__)
#define sec_method_exp_43(ctx, ...) ctx; sec_method_exp_42(__VA_ARGS__)
#define sec_method_exp_44(ctx, ...) ctx; sec_method_exp_43(__VA_ARGS__)
#define sec_method_exp_45(ctx, ...) ctx; sec_method_exp_44(__VA_ARGS__)
#define sec_method_exp_46(ctx, ...) ctx; sec_method_exp_45(__VA_ARGS__)
#define sec_method_exp_47(ctx, ...) ctx; sec_method_exp_46(__VA_ARGS__)
#define sec_method_exp_48(ctx, ...) ctx; sec_method_exp_47(__VA_ARGS__)
#define sec_method_exp_49(ctx, ...) ctx; sec_method_exp_48(__VA_ARGS__)
#define sec_method_exp_50(ctx, ...) ctx; sec_method_exp_49(__VA_ARGS__)
#define sec_method_exp_51(ctx, ...) ctx; sec_method_exp_50(__VA_ARGS__)
#define sec_method_exp_52(ctx, ...) ctx; sec_method_exp_51(__VA_ARGS__)
#define sec_method_exp_53(ctx, ...) ctx; sec_method_exp_52(__VA_ARGS__)
#define sec_method_exp_54(ctx, ...) ctx; sec_method_exp_53(__VA_ARGS__)
#define sec_method_exp_55(ctx, ...) ctx; sec_method_exp_54(__VA_ARGS__)
#define sec_method_exp_56(ctx, ...) ctx; sec_method_exp_55(__VA_ARGS__)
#define sec_method_exp_57(ctx, ...) ctx; sec_method_exp_56(__VA_ARGS__)
#define sec_method_exp_58(ctx, ...) ctx; sec_method_exp_57(__VA_ARGS__)
#define sec_method_exp_59(ctx, ...) ctx; sec_method_exp_58(__VA_ARGS__)
#define sec_method_exp_60(ctx, ...) ctx; sec_method_exp_59(__VA_ARGS__)
#define sec_method_exp_61(ctx, ...) ctx; sec_method_exp_60(__VA_ARGS__)
#define sec_method_exp_62(ctx, ...) ctx; sec_method_exp_61(__VA_ARGS__)
#define sec_method_exp_63(ctx, ...) ctx; sec_method_exp_62(__VA_ARGS__)
#define sec_method_exp_64(ctx, ...) ctx; sec_method_exp_63(__VA_ARGS__)
#define sec_method_exp_65(ctx, ...) ctx; sec_method_exp_64(__VA_ARGS__)
#define sec_method_exp_66(ctx, ...) ctx; sec_method_exp_65(__VA_ARGS__)
#define sec_method_exp_67(ctx, ...) ctx; sec_method_exp_66(__VA_ARGS__)
#define sec_method_exp_68(ctx, ...) ctx; sec_method_exp_67(__VA_ARGS__)
#define sec_method_exp_69(ctx, ...) ctx; sec_method_exp_68(__VA_ARGS__)
#define sec_method_exp_70(ctx, ...) ctx; sec_method_exp_69(__VA_ARGS__)
#define sec_method_exp_71(ctx, ...) ctx; sec_method_exp_70(__VA_ARGS__)
#define sec_method_exp_72(ctx, ...) ctx; sec_method_exp_71(__VA_ARGS__)
#define sec_method_exp_73(ctx, ...) ctx; sec_method_exp_72(__VA_ARGS__)
#define sec_method_exp_74(ctx, ...) ctx; sec_method_exp_73(__VA_ARGS__)
#define sec_method_exp_75(ctx, ...) ctx; sec_method_exp_74(__VA_ARGS__)
#define sec_method_exp_76(ctx, ...) ctx; sec_method_exp_75(__VA_ARGS__)
#define sec_method_exp_77(ctx, ...) ctx; sec_method_exp_76(__VA_ARGS__)
#define sec_method_exp_78(ctx, ...) ctx; sec_method_exp_77(__VA_ARGS__)
#define sec_method_exp_79(ctx, ...) ctx; sec_method_exp_78(__VA_ARGS__)
#define sec_method_exp_80(ctx, ...) ctx; sec_method_exp_79(__VA_ARGS__)
#define sec_method_exp_81(ctx, ...) ctx; sec_method_exp_80(__VA_ARGS__)
#define sec_method_exp_82(ctx, ...) ctx; sec_method_exp_81(__VA_ARGS__)
#define sec_method_exp_83(ctx, ...) ctx; sec_method_exp_82(__VA_ARGS__)
#define sec_method_exp_84(ctx, ...) ctx; sec_method_exp_83(__VA_ARGS__)
#define sec_method_exp_85(ctx, ...) ctx; sec_method_exp_84(__VA_ARGS__)
#define sec_method_exp_86(ctx, ...) ctx; sec_method_exp_85(__VA_ARGS__)
#define sec_method_exp_87(ctx, ...) ctx; sec_method_exp_86(__VA_ARGS__)
#define sec_method_exp_88(ctx, ...) ctx; sec_method_exp_87(__VA_ARGS__)
#define sec_method_exp_89(ctx, ...) ctx; sec_method_exp_88(__VA_ARGS__)
#define sec_method_exp_90(ctx, ...) ctx; sec_method_exp_89(__VA_ARGS__)
#define sec_method_exp_91(ctx, ...) ctx; sec_method_exp_90(__VA_ARGS__)
#define sec_method_exp_92(ctx, ...) ctx; sec_method_exp_91(__VA_ARGS__)
#define sec_method_exp_93(ctx, ...) ctx; sec_method_exp_92(__VA_ARGS__)
#define sec_method_exp_94(ctx, ...) ctx; sec_method_exp_93(__VA_ARGS__)
#define sec_method_exp_95(ctx, ...) ctx; sec_method_exp_94(__VA_ARGS__)
#define sec_method_exp_96(ctx, ...) ctx; sec_method_exp_95(__VA_ARGS__)
#define _sec_method_exp(n, ...) sec_method_exp_##n(__VA_ARGS__)
#define sec_method_exp(n, ...) _sec_method_exp(n, __VA_ARGS__)

/*
 * The type of @P needs to be consistent with @T, otherwise gcc warning
 * (comparison of distinct pointer types lack a case) will be generated.
 * This warning will cause compile failure.
 */
#define sec_ptr_type_check(P, T)	(((T)NULL == (P)) ? (P) : (P))
#define func_ptr_type_cast(M, U)	({U u; u.func_p = (M); u.func;})
#define sec_ptr_type_cast(METHOD, FUNC)	func_ptr_type_cast(sec_ptr_type_check(FUNC, METHOD), METHOD##_union)

static inline bool sec_method_is_dummy(const struct sec_chk_method *method)
{
	return method->func == NULL;
}

#ifdef NEED_SECURITY_FRAME_OVERLAY
#define sec_init_overlay_list(head, list) (head = list)
#else
#define sec_init_overlay_list(head, list)
#endif

#define sec_method_init(...) sec_method_exp(sec_method_num(__VA_ARGS__), __VA_ARGS__)

// This dummy method avoids code warning of zero size of sec chk method array
#define sec_define_dummy_method() do {							\
	(void)is_overlay;								\
	__method[__method_num].func = NULL;						\
	__method[__method_num].hook.head = NULL;					\
	dlist_init(&__method[__method_num].hook.node);					\
	__method_num++;									\
} while (0)
#define sec_define_chk_method(_meth, _func) do {					\
	__method[__method_num].func = sec_ptr_type_cast(sec_chk_##_meth##_ft, _func);	\
	if (is_overlay) {								\
		sec_init_overlay_list(__method[__method_num].hook.head, &sec_chk_point_##_meth.overlay);	\
	} else {									\
		__method[__method_num].hook.head = &sec_chk_point_##_meth.list;		\
	}										\
	dlist_init(&__method[__method_num].hook.node);					\
	__method_num++;									\
} while (0)

#define sec_define_hook_method(_meth, _func) do {					\
	__method[__method_num].func = sec_ptr_type_cast(sec_hook_##_meth##_ft, _func);	\
	if (is_overlay) {								\
		sec_init_overlay_list(__method[__method_num].hook.head, &sec_hook_point_##_meth.overlay);	\
	} else {									\
		__method[__method_num].hook.head = &sec_hook_point_##_meth.list;	\
	}										\
	dlist_init(&__method[__method_num].hook.node);					\
	__method_num++;									\
} while (0)

#define sec_define_active_credchk_chk_method(_meth, _func) do {						\
	__method[__method_num].func = sec_ptr_type_cast(sec_active_cred_chk_##_meth##_ft, _func);	\
	if (is_overlay) {										\
		sec_init_overlay_list(__method[__method_num].hook.head, &sec_active_cred_chk_point_##_meth.overlay);	\
	} else {											\
		__method[__method_num].hook.head = &sec_active_cred_chk_point_##_meth.list;		\
	}												\
	dlist_init(&__method[__method_num].hook.node);							\
	__method_num++;											\
} while (0)

#define sec_define_chk_xact_method sec_define_chk_method

#define SEC_DEFINE_COMPONENT(_component, _module, ...)						\
SEC_DEFINE_COMPONENT_CUSTOM(_component, _module,						\
			    ((sec_comp_init_ft)NULL), ((sec_comp_fini_ft)NULL),			\
			    ##__VA_ARGS__)

#define SEC_DEFINE_COMPONENT_CUSTOM(_component, _module, _func_init, _func_fini, ...)		\
SEC_DEFINE_COMPONENT_COMMON(_component, _module, _func_init, _func_fini, false, false,		\
			    ##__VA_ARGS__)

#define SEC_DEFINE_COMPONENT_COMMON(_component, _module, _func_init, _func_fini,		\
					   _high_priority, _in_overlay_list, ...)		\
static struct sec_chk_method _module##_##_component##_method[sec_method_num(__VA_ARGS__)];	\
static int _##_module##_##_component##_init(void)						\
{												\
	unsigned int __method_num = 0u;								\
	bool is_overlay = _in_overlay_list;							\
	struct sec_chk_method *__method = _module##_##_component##_method;			\
	sec_method_init(__VA_ARGS__);								\
	BUG_ON(__method_num > ARRAY_SIZE(_module##_##_component##_method));			\
	sec_add_method(_module##_##_component##_method,						\
		       ARRAY_SIZE(_module##_##_component##_method), _high_priority);		\
												\
	return E_HM_OK;										\
}												\
static int _module##_##_component##_init(const struct sec_server_env *env)			\
{												\
	int ret = E_HM_OK;									\
	if (_func_init != NULL) {								\
		ret = _func_init(env);								\
	}											\
	if (ret == E_HM_OK) {									\
		ret = _##_module##_##_component##_init();					\
	}											\
	return ret;										\
}												\
static void _module##_##_component##_fini(void)							\
{												\
	if (_func_fini != NULL) {								\
		_func_fini();									\
	}											\
	sec_del_method(_module##_##_component##_method,						\
		       ARRAY_SIZE(_module##_##_component##_method));				\
}												\
struct sec_comp sec_comp_##_module##_##_component = {						\
	.mod = &sec_mod_##_module,								\
	.ops = {										\
		.init = _module##_##_component##_init,						\
		.fini = _module##_##_component##_fini,						\
	},											\
}

#define SEC_DEFINE_OVERLAY_COMPONENT(_component, _module, ...) \
SEC_DEFINE_COMPONENT_COMMON(_component, _module,						\
			    ((sec_comp_init_ft)NULL), ((sec_comp_fini_ft)NULL),			\
			    false, false, ##__VA_ARGS__)

#define SEC_DEFINE_SEHARMONY_COMPONENT(_component, _module, ...) \
SEC_DEFINE_COMPONENT_COMMON(_component, _module,						\
			    ((sec_comp_init_ft)NULL), ((sec_comp_fini_ft)NULL),			\
			    true, true, ##__VA_ARGS__)

#ifdef NEED_SECURITY_FRAME_OVERLAY
#define SEC_DEFINE_MAC_COMPONENT(_component, _module, ...) \
SEC_DEFINE_COMPONENT_COMMON(_component, _module,						\
			    ((sec_comp_init_ft)NULL), ((sec_comp_fini_ft)NULL),			\
			    false, true, ##__VA_ARGS__)
#else
#define SEC_DEFINE_MAC_COMPONENT(_component, _module, ...) \
SEC_DEFINE_COMPONENT_COMMON(_component, _module,						\
			    ((sec_comp_init_ft)NULL), ((sec_comp_fini_ft)NULL),			\
			    false, false, ##__VA_ARGS__)
#endif

#endif
