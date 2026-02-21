/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implement common interface for uev
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 18 19:46:33 2019
 */

#include <string.h>
#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <libhmtrace/hmtrace.h>
#include <libhmuev/uev_template.h>
#include <libhmtrace/uev_trace/uev_trace_template.h>

static int __find_uev_insert_pos(struct uev_header *header, const struct uev_request_obj *obj,
				 struct dlist_node **prevlist)
{
	int ret = E_HM_OK;
	struct uev_request_obj *pos = NULL;

	dlist_for_each_entry(pos, &header->list, struct uev_request_obj, list) {
		if (pos == obj) {
			hm_debug("uev %s register failed: already registered.\n",
				 header->name);
			ret = E_HM_OBJEXIST;
			break;
		}
		if (pos->prio < obj->prio) {
			break;
		}
		*prevlist = &pos->list;
	}
	return ret;
}

int _uev_common_register(struct uev_header *header, struct uev_request_obj *obj)
{
	struct dlist_node *prevlist = NULL;
	int ret = E_HM_OK;

	if ((header == NULL) || (obj == NULL) || (obj->handler == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		(void)raw_rwlock_wrlock(&header->rwlock);
		prevlist = &header->list;
		if (header->refcnt == 0U) {
			/* enable "jump label" at "uev_trace_xxx" in the future */
			header->enable = true;
		} else {
			ret = __find_uev_insert_pos(header, obj, &prevlist);
		}

		if (ret == E_HM_OK) {
			header->refcnt++;
			dlist_insert(prevlist, &obj->list);
		}
		(void)raw_rwlock_unlock(&header->rwlock);
	}
	return ret;
}

int _uev_common_unregister(struct uev_header *header, struct uev_request_obj *obj)
{
	struct uev_request_obj *pos = NULL;
	struct uev_request_obj *next = NULL;
	bool found_obj = false;
	int ret = E_HM_OK;

	if ((header == NULL) || (obj == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		(void)raw_rwlock_wrlock(&header->rwlock);
		dlist_for_each_entry_safe(pos, next, &header->list, struct uev_request_obj, list) {
			if (pos == obj) {
				dlist_delete(&obj->list);
				header->refcnt--;
				found_obj = true;
				break;
			}
		}
		if (!found_obj) {
			(void)raw_rwlock_unlock(&header->rwlock);
			hm_error("uev %s unregister failed: no such request.\n", header->name);
			ret = E_HM_NOOBJ;
		}
	}

	if (ret == E_HM_OK) {
		if (header->refcnt == 0U) {
			header->enable = false;
		}
		(void)raw_rwlock_unlock(&header->rwlock);
	}

	return ret;
}

void uev_data_write_int(struct uev_trace_data *data, int v)
{
	size_t sz = sizeof(v);
	if ((data->offset + sz) <= data->size) {
		*(int *)((uintptr_t)(void *)data->buf + data->offset) = v;
		data->offset += sz;
	} else {
		hm_warn("uev data write int faield, offset:%u\n",
			(unsigned int)data->offset);
	}
}

void uev_data_write_u64(struct uev_trace_data *data, uint64_t v)
{
	size_t sz = sizeof(v);
	if ((data->offset + sz) <= data->size) {
		*(uint64_t *)((uintptr_t)(void *)data->buf + data->offset) = v;
		data->offset += sz;
	} else {
		hm_warn("uev data write int faield, offset:%u\n",
			(unsigned int)data->offset);
	}
}

void uev_data_write_string(struct uev_trace_data *data, const void* src)
{
	size_t sz = 0U;
	if (strcpy_s((char *)((uintptr_t)(void *)data->buf + data->offset),
		     data->size - data->offset, src) == 0) {
		sz = strlen(src) + 1U;
		data->offset += sz;
	} else {
		hm_warn("uev data write string failed, offset:%u, src size:%u\n",
			(unsigned int)data->offset, (unsigned int)sz);
	}
}

void uev_data_write(struct uev_trace_data *data, const void *buf, size_t sz)
{
	if ((data->offset + sz) <= data->size) {
		int err = memcpy_s(&data->buf[data->offset], sz, buf, sz);
		if (err != E_HM_OK) {
			hm_warn("uev data write memcpy_s failed.\n");
		}
		data->offset += sz;
	} else {
		hm_warn("uev data write field failed, offset:%u\n",
			(unsigned int)data->offset);
	}
}

int uev_enable_event(const struct hmtrace_event *trace_event)
{
	struct uev_trace_event *uev_evt = NULL;

	uev_evt = container_of(trace_event, struct uev_trace_event, evt);
	uev_enable_request(&uev_evt->uev_obj);
	return E_HM_OK;
}

int uev_disable_event(const struct hmtrace_event *trace_event)
{
	struct uev_trace_event *uev_evt = NULL;

	uev_evt = container_of(trace_event, struct uev_trace_event, evt);
	uev_disable_request(&uev_evt->uev_obj);
	return E_HM_OK;
}

int uev_get_available_events(struct bunch *bunch, void *data)
{
	struct hmtrace_class *trace_class;
	int32_t total_size = 0;
	int32_t total_len = 0;
	const struct uev_trace_event *event;
	const struct uev_trace_event_table *table;
	size_t idx;

	trace_class = (struct hmtrace_class *)data;
	for_each_uev_event_table(trace_class, table) {
		for_each_uev_event(table, event, idx) {
			total_size++;
			/* '1' for space between two events */
			total_len += (int32_t)strlen(event->evt.name) + 1;
		}
	}
	bunch_write_int32(bunch, total_size);
	bunch_write_int32(bunch, total_len);

	for_each_uev_event_table(trace_class, table) {
		for_each_uev_event(table, event, idx) {
			bunch_write_string(bunch, event->evt.name);
		}
	}

	return E_HM_OK;
}

int uev_register_event(struct hmtrace_class *trace_class,
		       const void *buffer, unsigned int buffer_size)
{
	int ret = E_HM_NOSR;
	struct uev_trace_event *uevt;
	const struct uev_trace_event_table *table;
	size_t idx;

	if (buffer == NULL || buffer_size == 0) {
		return E_HM_INVAL;
	}
	const char *event_name = buffer;
	if (event_name[buffer_size - 1U] != '\0') {
		return E_HM_INVAL;
	}

	for_each_uev_event_table(trace_class, table) {
		for_each_uev_event(table, uevt, idx) {
			if (strcmp((const char *)buffer, uevt->evt.name) != 0) {
				continue;
			}
			ret = uevt->register_handler(&uevt->uev_obj);
			if (ret != E_HM_OK) {
				return ret;
			}
			ret = hm_trace_register_event(trace_class, &uevt->evt);
			if (ret != E_HM_OK) {
				/* do not roll back if failed again */
				(void)uevt->unregister_handler(&uevt->uev_obj);
			}
		}
	}

	return ret;
}

int uev_unregister_event(struct hmtrace_class *trace_class,
			 const void *buffer, unsigned int buffer_size)
{
	int ret = E_HM_NOSR;
	struct uev_trace_event *uevt;
	const struct uev_trace_event_table *table;
	size_t idx;

	if (buffer == NULL || buffer_size == 0) {
		return E_HM_INVAL;
	}
	const char *event_name = buffer;
	if (event_name[buffer_size - 1U] != '\0') {
		return E_HM_INVAL;
	}

	for_each_uev_event_table(trace_class, table) {
		for_each_uev_event(table, uevt, idx) {
			if (strcmp((const char *)buffer, uevt->evt.name) != 0) {
				continue;
			}

			ret = hm_trace_unregister_event(&uevt->evt);
			if (ret != E_HM_OK) {
				return ret;
			}
			/* do not roll back for unregister action */
			return uevt->unregister_handler(&uevt->uev_obj);
		}
	}

	return ret;
}
