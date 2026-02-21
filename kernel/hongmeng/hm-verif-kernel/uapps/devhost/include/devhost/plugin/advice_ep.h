/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Struct and macro for advice extension point
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 15:12:10 2020
 */
#ifndef DEVHOST_PLUGIN_ADVICE_EP_H
#define DEVHOST_PLUGIN_ADVICE_EP_H

#include <devhost/plugin/base.h>

/* advice extension point */
enum advice_call_type {
	ADVICE_EV_INVAL,
	ADVICE_BEFORE,
	ADVICE_AROUND,
	ADVICE_AFTER,
	ADVICE_ORIGIN,
};

struct advice;
struct advice_ctx;
struct advice_extension_point {
	struct extension_point base;
	struct dlist_node advice_list;
	size_t args_size;
	int (*advice_call_helper)(struct advice_ctx *ctx, struct advice *next, enum advice_call_type type, int ret);
	int (*call_around_args_helper)(struct advice_ctx *ctx, ...);
};

struct advice_extension_info {
	int selfish;
	void (*before)(void /* args */);
	int (*around)(struct advice_ctx *ctx, ...);
	void (*after)(int around_ret, ...);
	struct advice *ext_inst;
};

struct advice {
	struct advice_extension_point *ep;
	struct libdh_extension *ext;
	struct dlist_node dnode;
	int selfish;

	int (*default_fn)(void /* args */);
	void (*before)(void /* args */);
	int (*around)(struct advice_ctx *ctx, ...);
	void (*after)(int around_ret, ...);
};

#define AROUND_NEXT_CALL_LIMIT	1
struct advice_ctx {
	struct advice *advice_obj;
	void *stack_args; /* the args ptr of top advice located at stack */
	int limit;
	int (*call_next)(struct advice_ctx *ctx); /* call around/default use top advice args  */
	int (*call_next_args)(struct advice_ctx *ctx, ...); /* call around/default use custom args */
};

int __advice_ep_init(struct extension_point *_ep);
int __advice_ep_attach(struct extension_point *_ep, struct libdh_extension *_ext);
void __advice_ep_detach(struct extension_point *_ep, struct libdh_extension *_ext);
struct advice *around_advice_next(const struct advice *self);
int __advice_ep_callchain(const struct extension_point *_ep, size_t arg_size, void *args);

#define __ARGS_CONNECT_HELPER0(arg1, ...)	arg1
#define __ARGS_CONNECT_HELPER1(arg1, ...)	arg1, __VA_ARGS__
#define __ARGS_CONNECT_HELPER2(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER3(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER4(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER5(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER6(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER7(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER8(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER9(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER10(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER11(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER12(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER13(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER14(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER15(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER16(arg1, ...)	__ARGS_CONNECT_HELPER1(arg1, __VA_ARGS__)
#define __ARGS_CONNECT_HELPER(n, arg1, ...)	__ARGS_CONNECT_HELPER##n(arg1, __VA_ARGS__)

#define __ARGS_CONNECT_PARAM_VAL_2(n, arg1, arg2, ...)	arg1, __ARGS_CONNECT_HELPER(n, arg2, __VA_ARGS__)
#define __ARGS_CONNECT_PARAM_VAL_1(n, arg1, ...)	__ARGS_CONNECT_HELPER(n, arg1, __VA_ARGS__)

#define __DEFINE_ADVICE_ARGS_HELPER(name, n, ...)	\
	struct name##_args {				\
		__ARGS_STRUCT_FIELD(n, ##__VA_ARGS__)	\
	};						\
	static int name##_advice_call_helper(struct advice_ctx *ctx, struct advice *next,	\
					     enum advice_call_type type, int ret)	\
	{										\
		struct advice_ctx stack_ctx = *ctx;					\
		struct name##_args *param = (struct name##_args *)ctx->stack_args;	\
		((void)(param));							\
		if (param == NULL) {							\
			return E_HM_INVAL;						\
		}									\
		if (type == ADVICE_BEFORE) {						\
			__ARGS_CALL_FUNC_PTR(next->before, void, __ARGS_USE(n, param));	\
			return E_HM_OK;							\
		} else if (type == ADVICE_AROUND) {					\
			/* must deliver it from top to bottom */			\
			stack_ctx.limit = AROUND_NEXT_CALL_LIMIT;			\
			stack_ctx.advice_obj = next;					\
			return __ARGS_CALL_FUNC_PTR(next->around, int,			\
						    __ARGS_CONNECT_PARAM_VAL_1(n, &stack_ctx, __ARGS_USE(n, param)));	\
		} else if (type == ADVICE_AFTER) {					\
			__ARGS_CALL_FUNC_PTR(next->after, void,				\
					     __ARGS_CONNECT_PARAM_VAL_1(n, ret, __ARGS_USE(n, param)));		\
			return E_HM_OK;							\
		} else if (type == ADVICE_ORIGIN) {					\
			return __ARGS_CALL_FUNC_PTR(next->default_fn, int, __ARGS_USE(n, param));	\
		}									\
		return E_HM_INVAL;							\
	}										\
	static int name##_call_around_with_args_helper(__ARGS_CONNECT_PARAM_VAL_1(n, struct advice_ctx *ctx,	\
							__ARGS_FUNC_PARAM(n, ##__VA_ARGS__))			\
							)				\
	{										\
		struct advice_ctx stack_ctx = {};					\
		struct advice *self = NULL;						\
		struct advice *next = NULL;						\
		if (ctx == NULL) {							\
			return E_HM_INVAL;						\
		}									\
		--ctx->limit;								\
		if (ctx->limit < 0) {							\
			return E_HM_INVAL;						\
		}									\
		stack_ctx = *ctx;							\
		self = ctx->advice_obj;							\
		next = around_advice_next(self);					\
		if (next == NULL) {							\
			if (self != NULL) {						\
				return __ARGS_CALL_FUNC_PTR(self->default_fn, int, __ARGS_FUNC_PVAL(n, ##__VA_ARGS__));	\
			}								\
			return E_HM_INVAL;						\
		}									\
		stack_ctx.limit = AROUND_NEXT_CALL_LIMIT;				\
		stack_ctx.advice_obj = next;						\
		return __ARGS_CALL_FUNC_PTR(next->around, int,				\
					    __ARGS_CONNECT_PARAM_VAL_1(n, &stack_ctx, __ARGS_FUNC_PVAL(n, ##__VA_ARGS__)));	\
	}

#define DEFINE_ADVICE_EP_PROTOTYPE(name, ...)	\
	__DEFINE_ADVICE_ARGS_HELPER(name, COUNT_ARG(__VA_ARGS__), ##__VA_ARGS__)

#define DECLARE_ADVICE_EP(name, ret, base_default, ...) do { 	\
	/* declare advice extension point */			\
	__DECLARE_EXTENSION_POINT(name,				\
				  struct advice_extension_point, base,	\
				  __advice_ep_init, NULL,	\
				  __advice_ep_attach, __advice_ep_detach,	\
				  base_default,			\
				  DH_EP_TYPE_ADVICE, 0,		\
				  __SUBCLASS_MEMBER_SET(advice_list, DLIST_HEAD_INIT(__ep_##name.advice_list)),	\
				  __SUBCLASS_MEMBER_SET(args_size, sizeof(struct name##_args)),		\
				  __SUBCLASS_MEMBER_SET(advice_call_helper, name##_advice_call_helper),	\
				  __SUBCLASS_MEMBER_SET(call_around_args_helper,			\
							(void *)(uintptr_t)name##_call_around_with_args_helper)	\
				  );				\
	/* call advice extension point callchain */		\
	struct name##_args __stack_args = { __ARGS_STRUCT_VAL(COUNT_ARG(__VA_ARGS__), ##__VA_ARGS__) };	\
	dh_ep_rdlock(&__ep_##name.base);								\
	ret = __advice_ep_callchain(&__ep_##name.base, sizeof(__stack_args), &__stack_args);		\
	dh_ep_unlock(&__ep_##name.base);								\
} while (0)

#endif /* DEVHOST_PLUGIN_ADVICE_EP_H */
