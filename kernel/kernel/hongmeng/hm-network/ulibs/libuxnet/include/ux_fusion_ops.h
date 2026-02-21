/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network domain
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#ifndef __LIBUXNET_UX_FUSION_OPS_H
#define __LIBUXNET_UX_FUSION_OPS_H

#include "hm_socket.h"

extern struct fusion_ops *fs_ops;

#ifdef CONFIG_NET_FUSION
static inline void ux_fusion_ops_register(struct fusion_ops *ops)
{
	fs_ops = ops;
}

static inline bool is_fusion_ops_enable(void)
{
	if (fs_ops && fs_ops->fusion_copy_from_user
	    && fs_ops->fusion_copy_to_user) {
		return true;
	}
	return false;
}

static inline int fusion_copy_to_user(void *dst, const void *src, unsigned long n)
{
	return fs_ops->fusion_copy_to_user(dst, src, n);
}

static inline int fusion_copy_from_user(void *dst, const void *src, unsigned long n)
{
	return fs_ops->fusion_copy_from_user(dst, src, n);
}

static inline int fusion_register_shrinker(unsigned long (*shrinker_query)(void *priv),
					   unsigned long (*shrinker_shrink)(void *priv, int req_nr),
					   void *priv)
{
	if (fs_ops == NULL) {
		return E_HM_OK;
	}
	return fs_ops->fusion_register_shrinker(shrinker_query, shrinker_shrink, priv);
}

static inline void *fusion_valloc(unsigned int size)
{
	if (fs_ops == NULL) {
		return malloc(size);
	}
	return fs_ops->fusion_valloc(size);
}

static inline void fusion_vfree(void *ptr)
{
	if (fs_ops == NULL) {
		free(ptr);
	} else {
		fs_ops->fusion_vfree(ptr);
	}
}

static inline int fusion_ctx_setup(void)
{
	if (fs_ops == NULL || fs_ops->backend_ctx_setup == NULL) {
		return E_HM_NOSYS;
	} else {
		return fs_ops->backend_ctx_setup();
	}
}

#else

static inline int fusion_ctx_setup(void)
{
	return E_HM_OK;
}

static inline void ux_fusion_ops_register(struct fusion_ops *ops)
{
	UNUSED(ops);
}

static inline bool is_fusion_ops_enable(void)
{
	return false;
}

static inline int fusion_copy_to_user(void *dst, const void *src, unsigned long n)
{
	UNUSED(dst, src, n);
	return E_HM_NOSYS;
}

static inline int fusion_copy_from_user(void *dst, const void *src, unsigned long n)
{
	UNUSED(dst, src, n);
	return E_HM_NOSYS;
}

static inline int fusion_register_shrinker(unsigned long (*shrinker_query)(void *priv),
					   unsigned long (*shrinker_shrink)(void *priv, int req_nr),
					   void *priv)
{
	UNUSED(shrinker_query, shrinker_shrink, priv);
	return E_HM_NOSYS;
}

static inline void *fusion_valloc(unsigned int size)
{
	UNUSED(size);
	return NULL;
}

static inline void fusion_vfree(void *ptr)
{
	UNUSED(ptr);
}
#endif /* !CONFIG_NET_FUSION */

#endif /* __LIBUXNET_UX_FUSION_OPS_H */
