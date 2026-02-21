/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Struct and macro for sysif extension point
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 14:47:12 2020
 */
#ifndef DEVHOST_PLUGIN_SYSIF_EP_H
#define DEVHOST_PLUGIN_SYSIF_EP_H

#include <libstrict/strict.h>
#include <devhost/plugin/base.h>

/* sysif extension point */
typedef long (*sysif_ep_fn_t)(unsigned long long sender, unsigned long cred, ...);

struct sysif_extension_inst;
struct sysif_extension_point {
	struct extension_point base;
	struct sysif_extension_inst *ext_inst;
};

struct sysif_extension_inst {
	struct sysif_extension_point *ep;
	struct libdh_extension *ext;
	sysif_ep_fn_t plugin_fn;
	struct libdh_extension *precond_ext;
	sysif_ep_fn_t precond;
};

struct sysif_extension_info {
	sysif_ep_fn_t ext_fn;
	sysif_ep_fn_t precond;
	struct sysif_extension_inst *ext_inst;
};

int __sysif_ep_init(struct extension_point *_ep);
void __sysif_ep_deinit(struct extension_point *_ep);
int __sysif_ep_attach(struct extension_point *_ep, struct libdh_extension *_ext);
void __sysif_ep_detach(struct extension_point *_ep, struct libdh_extension *_ext);

int sysif_precond_checker_entry(struct sysif_extension_point *ep,
				int callno,
				unsigned long long sender,
				unsigned long cred);

#define __SYSIF_EP_CALLCHAIN(sysifno, _ep, t, default_fn, ...)	do {	\
	t __ret = E_HM_OK;					\
	struct sysif_extension_point *ep = (_ep);		\
	dh_ep_rdlock(&ep->base);				\
	if (ep->ext_inst == NULL) {				\
		dh_ep_unlock(&ep->base);			\
		return E_HM_INVAL;				\
	}							\
	__ret = sysif_precond_checker_entry(ep, sysifno, sender, credential);	\
	if (__ret == E_HM_OK && ep->ext_inst->precond != NULL) {		\
		__ret = (ep->ext_inst->precond)(__VA_ARGS__);	\
	}							\
	if (__ret == E_HM_OK) {					\
		__ret = ep->ext_inst->plugin_fn != NULL ? 	\
			__ARGS_CALL_FUNC_PTR(ep->ext_inst->plugin_fn, long, ##__VA_ARGS__) : \
			E_HM_NOSYS;				\
	}							\
	dh_ep_unlock(&ep->base);				\
	return __ret;						\
} while (0)

#define __DEFINE_SYSIF_EP_HANDLER_CHECKER(sysif_name, ext_name, t, argn, ...)						\
	static __maybe_unused t sysif_name##_sysifep_args_chk(__SC_MAP(argn, __SC_DECL, ##__VA_ARGS__))			\
	{														\
		typeof(___COMB(actvhdlr, sysif_name)(__SC_MAP(argn, __SC_USE, ##__VA_ARGS__))) __args_chk_ret = 0;	\
		return __args_chk_ret;											\
	}

#define __DECLARE_SYSIF_EP(pre_hdr, m_mgr, svr, t, fn, ...)	do {	\
	__DECLARE_EXTENSION_POINT(pre_hdr##_##m_mgr,			\
				  struct sysif_extension_point, base,	\
				  __sysif_ep_init, __sysif_ep_deinit,	\
				  __sysif_ep_attach, __sysif_ep_detach,	\
				  fn, DH_EP_TYPE_SYSIF, DH_EP_FLAGS_STATIC,	\
				  __SUBCLASS_MEMBER_SET(ext_inst, NULL));				\
	__SYSIF_EP_CALLCHAIN(__##svr##_method_##m_mgr, &__ep_##pre_hdr##_##m_mgr, t, fn, ##__VA_ARGS__);\
} while (0)

#define __DECLARE_SYSIF_EP_ARGS(pre_hdr, m_mgr, svr, t, fn, argn, ...)		\
	__DECLARE_SYSIF_EP(pre_hdr, m_mgr, svr, t, fn, __SC_MAP(argn, __SC_USE, __VA_ARGS__))

static __used int __default_sysif_nosys(unsigned long long sender, unsigned long cred, ...)
{
	UNUSED(sender, cred);
	return E_HM_NOSYS;
}

#define DECLARE_SYSIF_EP_ALS_HDLR(pre_hdr, m_mgr, svr, ...)		\
	DEFINE_ACTVHDLR_ALS(pre_hdr##_##m_mgr, __VA_ARGS__)		\
	{								\
		__DECLARE_SYSIF_EP_ARGS(pre_hdr, m_mgr, svr, int,	\
					__default_sysif_nosys,		\
					__HDLR_ARG_NR(__VA_ARGS__),	\
					__VA_ARGS__);			\
	}

#define DECLARE_SYSIF_EP_CUSTOM_HDLR(custom_macro, pre_hdr, m_mgr, svr, t, ...)	\
	custom_macro(pre_hdr##_##m_mgr, __VA_ARGS__)			\
	{								\
		__DECLARE_SYSIF_EP_ARGS(pre_hdr, m_mgr, svr, t,		\
					__default_sysif_nosys,		\
					__HDLR_ARG_NR(__VA_ARGS__),	\
					__VA_ARGS__);			\
	}

#endif /* DEVHOST_PLUGIN_SYSIF_EP_H */
