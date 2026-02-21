/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Support sysif extension point
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 14:47:12 2020
 */
#include <devhost/plugin/sysif_ep.h>

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

/* sysif extension point */
int __sysif_ep_init(struct extension_point *_ep)
{
	struct sysif_extension_point *ep = NULL;
	BUG_ON(_ep == NULL);
	ep = container_of(_ep, struct sysif_extension_point, base);
	ep->ext_inst = (struct sysif_extension_inst *)malloc(sizeof(*ep->ext_inst));
	if (ep->ext_inst == NULL) {
		return E_HM_NOMEM;
	}
	mem_zero_s(*ep->ext_inst);
	return E_HM_OK;
}

void __sysif_ep_deinit(struct extension_point *_ep)
{
	struct sysif_extension_point *ep = NULL;
	BUG_ON(_ep == NULL);
	ep = container_of(_ep, struct sysif_extension_point, base);
	if (ep->ext_inst != NULL) {
		free(ep->ext_inst);
		ep->ext_inst = NULL;
	}
}

int __sysif_ep_attach(struct extension_point *_ep, struct libdh_extension *_ext)
{
	struct sysif_extension_point *ep = NULL;
	struct sysif_extension_info *ext_info = NULL;
	struct libdh_plugin *plugin = NULL;
	BUG_ON(_ep == NULL || _ext == NULL);

	plugin = _ext->owner();
	BUG_ON(plugin == NULL);
	ext_info = (struct sysif_extension_info *)_ext->ext_info;
	ep = container_of(_ep, struct sysif_extension_point, base);

	if (ext_info == NULL ||
	    (ext_info->ext_fn == NULL && ext_info->precond == NULL)) {
		hm_error("%s: there is no sysif extension\n", plugin->name);
		return E_HM_POSIX_NOENT;
	}
	if (_ext->type != DH_EP_TYPE_SYSIF ||
	    (ext_info->ext_fn != NULL && ext_info->precond != NULL)) {
		hm_error("%s: invalid extension\n", plugin->name);
		return E_HM_INVAL;
	}

	if (ext_info->ext_inst != NULL) {
		hm_error("extension has instance already\n");
		return E_HM_INVAL;
	}

	BUG_ON(ep->ext_inst == NULL);
	if ((ep->ext_inst->plugin_fn != NULL && ext_info->ext_fn != NULL) ||
	    (ep->ext_inst->precond != NULL && ext_info->precond != NULL)) {
		hm_error("extension point %s has extension already\n", _ep->name);
		return E_HM_INVAL;
	}
	if (ext_info->ext_fn != NULL) { // sysif ext
		ep->ext_inst->plugin_fn = ext_info->ext_fn;
		ep->ext_inst->ext = _ext;
	}
	if (ext_info->precond != NULL) { // sysif precond ext
		ep->ext_inst->precond = ext_info->precond;
		ep->ext_inst->precond_ext = _ext;
	}
	ep->ext_inst->ep = ep;

	ext_info->ext_inst = ep->ext_inst;

	hm_debug("plugin %s --> ep %s\n", plugin->name, _ep->name);
	return E_HM_OK;
}

void __sysif_ep_detach(struct extension_point *_ep, struct libdh_extension *_ext)
{
	struct sysif_extension_point *ep = NULL;
	struct sysif_extension_info *ext_info = NULL;
	BUG_ON(_ep == NULL || _ext == NULL);

	ext_info = (struct sysif_extension_info *)_ext->ext_info;
	ep = container_of(_ep, struct sysif_extension_point, base);

	if (ep->ext_inst != NULL) {
		if (ep->ext_inst->ext == _ext) { // sysif ext
			ep->ext_inst->plugin_fn = NULL;
			ep->ext_inst->ext = NULL;
		}
		if (ep->ext_inst->precond_ext == _ext) { // sysif precond ext
			ep->ext_inst->precond = NULL;
			ep->ext_inst->precond_ext = NULL;
		}
	}
	ext_info->ext_inst = NULL;
}
