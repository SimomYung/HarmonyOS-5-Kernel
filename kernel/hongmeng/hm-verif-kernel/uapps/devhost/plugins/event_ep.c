/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Support event extension point
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 15:12:10 2020
 */
#include <devhost/plugin/event_ep.h>

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>

/* event extension point */
int __event_ep_init(struct extension_point *_ep)
{
	struct event_extension_point *ep = NULL;

	BUG_ON(_ep == NULL);
	ep = container_of(_ep, struct event_extension_point, base);

	dlist_init(&ep->before_event_list);
	dlist_init(&ep->after_event_list);

	return E_HM_OK;
}

enum event_ep_type {
	EVENT_EP_BEFPRE_TYPE = 1,
	EVENT_EP_AFTER_TYPE,
};

static int __new_event_inst_for_ep(struct event_extension_point *ep, struct libdh_extension *_ext,
				   enum event_ep_type ev_type, struct event_node **out_node)
{
	struct event_extension_info *ext_info = (struct event_extension_info *)_ext->ext_info;
	struct event_node *ev_node = (struct event_node *)malloc(sizeof(struct event_node));
	if (ev_node == NULL) {
		return E_HM_NOMEM;
	}

	ev_node->ep = ep;
	ev_node->ext = _ext;
	if (ev_type == EVENT_EP_BEFPRE_TYPE) {
		if (ext_info->before_inst != NULL) {
			free(ev_node);
			hm_error("extension has instance already\n");
			return E_HM_INVAL;
		}
		ev_node->fn = ext_info->before;
		ext_info->before_inst = ev_node;
	} else if (ev_type == EVENT_EP_AFTER_TYPE) {
		if (ext_info->after_inst != NULL) {
			free(ev_node);
			hm_error("extension has instance already\n");
			return E_HM_INVAL;
		}
		ev_node->fn = ext_info->after;
		ext_info->after_inst = ev_node;
	} else {
		free(ev_node);
		return E_HM_INVAL;
	}
	*out_node = ev_node;
	return E_HM_OK;
}

int __event_ep_attach(struct extension_point *_ep, struct libdh_extension *_ext)
{
	struct event_extension_point *ep = NULL;
	struct event_node *before_event = NULL;
	struct event_node *after_event = NULL;
	struct event_extension_info *ext_info = NULL;
	struct libdh_plugin *plugin = NULL;
	int ret;
	BUG_ON(_ep == NULL || _ext == NULL);

	plugin = _ext->owner();
	BUG_ON(plugin == NULL);
	ext_info = (struct event_extension_info *)_ext->ext_info;
	ep = container_of(_ep, struct event_extension_point, base);

	if (_ext->type != DH_EP_TYPE_EVENT) {
		hm_error("%s : not event extension\n", plugin->name);
		return E_HM_INVAL;
	}

	if (ext_info == NULL ||
	    ((ext_info->before == NULL) && (ext_info->after == NULL))) {
		hm_error("%s: there is no event extension\n", plugin->name);
		return E_HM_INVAL;
	}

	/* before handler */
	if (ext_info->before != NULL) {
		ret = __new_event_inst_for_ep(ep, _ext, EVENT_EP_BEFPRE_TYPE, &before_event);
		if (ret < 0) {
			hm_error("new event instance failed\n");
			return ret;
		}
		BUG_ON(before_event == NULL);
		dlist_insert_tail(&ep->before_event_list, &before_event->dnode);
	}

	/* after handler */
	if (ext_info->after != NULL) {
		ret = __new_event_inst_for_ep(ep, _ext, EVENT_EP_AFTER_TYPE, &after_event);
		if (ret < 0) {
			if (before_event != NULL) {
				dlist_delete(&before_event->dnode);
				free(before_event);
			}
			hm_error("new event instance failed\n");
			return ret;
		}
		BUG_ON(after_event == NULL);
		dlist_insert_tail(&ep->after_event_list, &after_event->dnode);
	}

	hm_debug("plugin %s --> ep %s\n", plugin->name, _ep->name);
	return E_HM_OK;
}

void __event_ep_detach(struct extension_point *_ep, struct libdh_extension *_ext)
{
	struct event_node *before_event = NULL;
	struct event_node *after_event = NULL;
	struct event_extension_info *ext_info = NULL;
	BUG_ON(_ep == NULL || _ext == NULL);

	ext_info = (struct event_extension_info *)_ext->ext_info;

	/* before handler */
	before_event = ext_info->before_inst;
	after_event = ext_info->after_inst;
	if (before_event != NULL) {
		dlist_delete(&before_event->dnode);
		ext_info->before_inst = NULL;
		free(before_event);
	}

	/* after handler */
	if (after_event != NULL) {
		dlist_delete(&after_event->dnode);
		ext_info->after_inst = NULL;
		free(after_event);
	}
}
