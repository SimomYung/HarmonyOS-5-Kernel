/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: macro for sysif uev
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 18 19:46:33 2019
 */
#ifndef ULIBS_LIBHMUEV_SYSIF_UEV_MACRO_H
#define ULIBS_LIBHMUEV_SYSIF_UEV_MACRO_H
#ifdef __sysif_trace__
#define __HM_UEV__
#include <libhmuev/uev_template.h>
#undef __HM_UEV__
#include <libhmsrv_sys/hm_timer.h>

#define SYSIF_TRACE_DECLARE(name, t, n, ...)			\
	DECLARE_UEV(_, name##_enter, UEV_ARG(n, __VA_ARGS__))	\
	DECLARE_UEV(_, name##_exit,\
		    UEV_ARG(8, unsigned long long, sender, unsigned long, credential,	\
			    t, ret, void *, actv_ret,	\
			    __u64, ready_time, __u64, block_time,	\
			    __u32, ready_cnt, __u32, block_cnt))
#define UEV_TRACE_ENTER(name, ...)	\
	uev_trace_##name##_enter(__VA_ARGS__);	\
	hm_actv_local_actv_set_need_reset();
#define UEV_TRACE_EXIT(name, sender, credential, ret)	\
	void *actv_ret = NULL;		\
	__u64 __ready_time = 0ULL;	\
	__u64 __block_time = 0ULL;	\
	__u32 __ready_cnt = 0U;		\
	__u32 __block_cnt = 0U;		\
	if (uev_enabled(&uev_head_##name##_exit)) {	\
		actv_ret = hm_actvpool_ret(sender, credential);	\
	}	\
	if (uev_enabled(&uev_head_##name##_enter) && uev_enabled(&uev_head_##name##_exit)) {	\
		if (!hm_actv_local_actv_is_need_reset()) {	\
			__ready_time = tocks_to_ns(hm_actv_local_actv_ready_time_of());	\
			__block_time = tocks_to_ns(hm_actv_local_actv_block_time_of());	\
			__ready_cnt = hm_actv_local_actv_ready_cnt_of();	\
			__block_cnt = hm_actv_local_actv_block_cnt_of();	\
		}	\
		hm_actv_local_actv_set_need_reset();	\
	}	\
	uev_trace_##name##_exit(sender, credential, ret, actv_ret,	\
				__ready_time, __block_time,__ready_cnt,	\
				__block_cnt);
#else
#define SYSIF_TRACE_DECLARE(name, t, n, ...)
#define UEV_TRACE_ENTER(name, ...)
#define UEV_TRACE_EXIT(name, sender, credential, ret)
#endif
#endif
