/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interface of lite hooks
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 25 08:47:55 2023
 */
 
#ifndef __LITE_VENDOR_HOOK_H__
#define __LITE_VENDOR_HOOK_H__
 
#include <lib/trace/tracepoint_defs.h>
#include <hmkernel/boot/mem.h>
#include <lib/klog.h>
#include <hmkernel/spinlock.h>
#include <hmkernel/compiler.h>

#define HM_WARN_ONCE(format...)	({			\
	static bool __section(.data.once) __warned = false;		\
								\
	if (__warned != true) {		\
		__warned = true;				\
		klog_warn(format);				\
	}							\
})

struct __lvh_func {
	void *func;
	void *data;
	bool has_data;
};

struct lite_vendor_hook {
    struct spinlock_s hook_lock;
	struct __lvh_func *funcs;
};
#endif // __LITE_VENDOR_HOOK_H___

#ifdef CREATE_HM_LITE_VENDOR_HOOK

#define DEFINE_HM_HCK_LITE_HOOK(name, proto, args)	\
	struct lite_vendor_hook __lvh_##name __used	\
	__section(__vendor_hooks) = {	\
		.hook_lock = SPINLOCK_INIT(),	\
		.funcs = NULL };	\
	void lvh_probe_##name(proto) { return; }	\
	void lvh_probe_data_##name(void *lvh_data, proto) { return; }
 
#undef DECLARE_HM_HCK_LITE_HOOK
#define DECLARE_HM_HCK_LITE_HOOK(name, proto, args)	\
	DEFINE_HM_HCK_LITE_HOOK(name, PARAMS(proto), PARAMS(args))

#else // #ifndef CREATE_HM_LITE_VENDOR_HOOK

#define REGISTER_HM_HCK_LITE_HOOK(name, probe)	\
	extern typeof(lvh_probe_##name) probe;	\
	do {	\
		if (register_lvh_##name(probe))	\
			HM_WARN_ONCE("LVH register failed!\n");	\
	} while (0)

#define REGISTER_HM_HCK_LITE_DATA_HOOK(name, probe, data)	\
	extern typeof(lvh_probe_data_##name) probe;	\
	do {	\
		if (register_lvh_data_##name(probe, data))	\
			HM_WARN_ONCE("LVH register failed!\n");	\
	} while (0)

#define CALL_HM_HCK_LITE_HOOK(name, args...)	\
	call_lvh_##name(args)

#define __DECLARE_HM_HCK_LITE_HOOK(name, proto, args)	\
	extern struct lite_vendor_hook __lvh_##name;	\
	extern void lvh_probe_##name(proto);	\
	extern void lvh_probe_data_##name(void *lvh_data, proto);	\
	static inline void	\
	call_lvh_##name(proto)	\
	{	\
		struct __lvh_func *funcs = (&__lvh_##name)->funcs;	\
		if (funcs && funcs->func) {	\
			if (unlikely(funcs->has_data))	\
				((void(*)(void *, proto))funcs->func)(funcs->data, args);	\
			else	\
				((void(*)(proto))funcs->func)(args);	\
		}	\
	}	\
	static inline int	\
	__register_lvh_##name(void *probe, void *data, bool has_data)	\
	{	\
		int err = 0;	\
		spinlock_acquire(&__lvh_##name.hook_lock);	\
		struct __lvh_func *funcs = (&__lvh_##name)->funcs;	\
		if (funcs) {	\
			if (funcs->func != probe || funcs->data != data)	\
				err = -1;	\
			goto out;	\
		}	\
		\
		funcs = (struct __lvh_func*)boot_alloc(sizeof(struct __lvh_func));	\
		if (!funcs) {	\
			err = -2;	\
			goto out;	\
		}	\
		\
		funcs->func = probe;	\
		funcs->data = data;	\
		funcs->has_data = has_data;	\
		(&__lvh_##name)->funcs = funcs;	\
	out:	\
		spinlock_release(&__lvh_##name.hook_lock);	\
		return err;	\
	}	\
	static inline int	\
	register_lvh_##name(void (*probe)(proto))	\
	{	\
		return __register_lvh_##name((void *)probe, NULL, false);	\
	}	\
	static inline int	\
	register_lvh_data_##name(void (*probe)(void *lvh_data, proto), void *data)	\
	{	\
		return __register_lvh_##name((void *)probe, data, true);	\
	}

#undef DECLARE_HM_HCK_LITE_HOOK
#define DECLARE_HM_HCK_LITE_HOOK(name, proto, args)	\
	__DECLARE_HM_HCK_LITE_HOOK(name, PARAMS(proto), PARAMS(args))

#endif // CREATE_HM_LITE_VENDOR_HOOK
