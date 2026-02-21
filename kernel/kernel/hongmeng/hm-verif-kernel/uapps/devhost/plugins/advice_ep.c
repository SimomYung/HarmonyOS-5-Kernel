/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Support advice extension point
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 15:59:14 2020
 */
#include <devhost/plugin/advice_ep.h>

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>


int __advice_ep_init(struct extension_point *_ep)
{
	BUG_ON(_ep == NULL);
	struct advice_extension_point *ep = container_of(_ep, struct advice_extension_point,
							 base);
	dlist_init(&ep->advice_list);
	return E_HM_OK;
}

static int __add_advice_tail_head(struct advice_extension_point *ep, struct advice *advice_obj)
{
	struct advice *next = NULL;
	if (advice_obj->selfish == 0) {
		dlist_insert_tail(&ep->advice_list, &advice_obj->dnode);
		return E_HM_OK;
	}

	/* add selfish advice at head */
	if (dlist_empty(&ep->advice_list)) {
		dlist_insert_head(&ep->advice_list, &advice_obj->dnode);
		return E_HM_OK;
	}
	next = dlist_first_entry(&ep->advice_list, struct advice, dnode);
	if (next->selfish != 0) {
		return E_HM_INVAL;
	}
	dlist_insert_head(&ep->advice_list, &advice_obj->dnode);
	return E_HM_OK;
}

int __advice_ep_attach(struct extension_point *_ep, struct libdh_extension *_ext)
{
	struct advice_extension_point *ep = NULL;
	struct advice *advice_obj = NULL;
	struct advice_extension_info *ext_info = NULL;
	struct libdh_plugin *plugin = NULL;
	BUG_ON(_ep == NULL || _ext == NULL);

	plugin = _ext->owner();
	BUG_ON(plugin == NULL);
	ext_info = (struct advice_extension_info *)_ext->ext_info;
	ep = container_of(_ep, struct advice_extension_point, base);

	if (_ext->type != DH_EP_TYPE_ADVICE) {
		hm_error("%s : not advice extension\n", plugin->name);
		return E_HM_INVAL;
	}
	if (ext_info == NULL || ext_info->around == NULL) {
		hm_error("%s : no advice extension found\n", plugin->name);
		return E_HM_INVAL;
	}
	if (ext_info->ext_inst != NULL) {
		hm_error("extension has instance already\n");
		return E_HM_INVAL;
	}

	advice_obj = (struct advice *)malloc(sizeof(*advice_obj));
	if (advice_obj == NULL) {
		return E_HM_NOMEM;
	}

	advice_obj->before = ext_info->before;
	advice_obj->after = ext_info->after;
	advice_obj->around = ext_info->around;
	advice_obj->selfish = ext_info->selfish;

	advice_obj->ep = ep;
	advice_obj->ext = _ext;
	advice_obj->default_fn = _ep->_private;

	if (__add_advice_tail_head(ep, advice_obj) < 0) {
		free(advice_obj);
		return E_HM_INVAL;
	}
	ext_info->ext_inst = advice_obj;
	hm_debug("add advice %p to ep %s\n", advice_obj, _ep->name);

	return E_HM_OK;
}

void __advice_ep_detach(struct extension_point *_ep, struct libdh_extension *_ext)
{
	struct advice *advice_obj = NULL;
	struct advice_extension_info *ext_info = NULL;

	BUG_ON(_ep == NULL || _ext == NULL);
	ext_info = (struct advice_extension_info *)_ext->ext_info;

	advice_obj = ext_info->ext_inst;
	if (advice_obj != NULL) {
		dlist_delete(&advice_obj->dnode);
		ext_info->ext_inst = NULL;
		free(advice_obj);
	}
}

struct advice *around_advice_next(const struct advice *self)
{
	if (self == NULL) {
		return NULL;
	}
	if (self->dnode.next == &(self->ep->advice_list)) {
		return NULL;
	}
	struct advice *next = dlist_next_entry(self, struct advice, dnode);
	return next;
}

static int __ctx_call_next(struct advice_ctx *ctx)
{
	struct advice *self = NULL;
	struct advice *next = NULL;
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	--ctx->limit;
	if (ctx->limit < 0) {
		return E_HM_INVAL;
	}
	self = ctx->advice_obj;
	if (self == NULL) {
		return E_HM_INVAL;
	}
	next = around_advice_next(self);
	if (next == NULL) { /* call default */
		if (self->ep != NULL && self->ep->advice_call_helper != NULL) {
			if (self->default_fn == NULL) {
				return E_HM_OK; /* allow: default_func is null */
			}
			return self->ep->advice_call_helper(ctx, self, ADVICE_ORIGIN, 0);
		}
		return E_HM_INVAL;
	}

	if (next->ep != NULL && next->ep->advice_call_helper != NULL) {
		return next->ep->advice_call_helper(ctx, next, ADVICE_AROUND, 0);
	}
	return E_HM_INVAL;
}

int __advice_ep_callchain(const struct extension_point *_ep, size_t arg_size, void *args)
{
	const struct advice_extension_point *ep = NULL;
	struct advice *advice_obj = NULL;
	struct advice_ctx stack_ctx = {};
	struct dlist_node *pos = NULL;
	int (*base_fn)(void) = NULL;
	int ret = E_HM_OK;

	BUG_ON(_ep == NULL || args == NULL);
	ep = container_of(_ep, struct advice_extension_point,
			  base);
	base_fn = ep->base._private;
	if (arg_size != ep->args_size) {
		hm_error("invalid argument size for the advice\n");
		return E_HM_INVAL;
	}

	dlist_for_each(pos, &ep->advice_list) {
		advice_obj = dlist_entry(pos, struct advice, dnode);
		/* before event */
		stack_ctx.stack_args = args;
		if (advice_obj->before != NULL) {
			(void)ep->advice_call_helper(&stack_ctx, advice_obj, ADVICE_BEFORE, 0);
		}
	}
	if (dlist_empty(&ep->advice_list)) {
		if (base_fn != NULL) {
			struct advice tmp_ad = {};
			tmp_ad.default_fn = base_fn;
			stack_ctx.stack_args = args;
			return ep->advice_call_helper(&stack_ctx, &tmp_ad, ADVICE_ORIGIN, 0);
		}
	} else {
		/* around advice */
		advice_obj = dlist_first_entry(&ep->advice_list, struct advice, dnode);
		if (advice_obj->around != NULL) {
			stack_ctx.stack_args = args;
			stack_ctx.advice_obj = NULL;
			stack_ctx.limit = AROUND_NEXT_CALL_LIMIT;
			stack_ctx.call_next = __ctx_call_next;
			stack_ctx.call_next_args = ep->call_around_args_helper;
			ret = ep->advice_call_helper(&stack_ctx, advice_obj, ADVICE_AROUND, 0);
		}
	}

	dlist_for_each(pos, &ep->advice_list) {
		advice_obj = dlist_entry(pos, struct advice, dnode);
		/* after event */
		stack_ctx.stack_args = args;
		if (advice_obj->after != NULL) {
			(void)ep->advice_call_helper(&stack_ctx, advice_obj, ADVICE_AFTER, ret);
		}
	}

	return ret;
}
