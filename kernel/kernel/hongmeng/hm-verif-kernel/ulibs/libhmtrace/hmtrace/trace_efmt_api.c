/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Trace event format API used at tracing side
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 11 18:04:10 2020
 */

#include <stdlib.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmtrace/efmt.h>
#include <libhmtrace/hmtrace.h>
#include <libhwsecurec/securec.h>

#include "core/trace_efmt.h"

#ifdef CONFIG_HMTRACE
void init_event_format(struct event_format *evt_fmt)
{
	if (evt_fmt != NULL) {
		evt_fmt->print_fmt = NULL;
		evt_fmt->field_array = NULL;
		dlist_init(&(evt_fmt->field_list));
	}
}

/*
 * Called to print event data with readable style. It makes use of the format
 * string and fields information to prepare the arguments of printf function
 * call.
 */
int print_event(char *out, size_t maxlen,
		struct event_format *evt_fmt, const void *evt_data)
{
	int ret;
	struct byte_stream str_buf;
	struct byte_stream raw_evt;
	/*
	 * Visitor function arguments:
	 * args[0]: bytes stream indicates string buffer recording output
	 * args[1]: bytes stream indicates event raw data
	 */
	void *args[] = { (void *)&str_buf, (void *)&raw_evt };

	if (out == NULL || evt_fmt == NULL || evt_data == NULL) {
		return E_HM_INVAL;
	}

	/* str_buf is the character stream of recording output */
	str_buf.offset = 0;
	str_buf.maxlen = maxlen;
	str_buf.bytes = (typeof(str_buf.bytes))out;
	/* raw_evt is the byte stream of event raw data */
	raw_evt.offset = 0;
	raw_evt.maxlen = SIZE_MAX;
	raw_evt.bytes = (typeof(raw_evt.bytes))evt_data;

	ret = call_field_visitor((void *)args, evt_fmt, print_field);
	if (ret != E_HM_OK) {
		return ret;
	}

	return str_buf.offset;
}

static void free_field_desc(struct field_desc *desc)
{
	if (desc->type != NULL) {
		free(desc->type);
		desc->type = NULL;
	}
	if (desc->name != NULL) {
		free(desc->name);
		desc->name = NULL;
	}
}

/*
 * Called by uprobe to free entire field_desc allocated via define_event_field.
 */
void free_field_list(const struct event_format *evt_fmt)
{
	struct field_node *next = NULL;
	struct field_node *node = NULL;

	if (evt_fmt == NULL) {
		return;
	}

	dlist_for_each_entry_safe(node, next, &evt_fmt->field_list,
				  struct field_node, list) {
		dlist_delete(&node->list);
		free_field_desc(&node->desc);
		free(node);
	}
}

struct event_format *alloc_event_format(void)
{
	struct event_format *evt_fmt =
		(struct event_format *)malloc(sizeof(struct event_format));
	if (evt_fmt != NULL) {
		init_event_format(evt_fmt);
	}
	return evt_fmt;
}

/*
 * Called by hmtrace.elf to free the unmarshalled event_format data
 */
void free_event_format(struct event_format *evt_fmt)
{
	if ((evt_fmt != NULL) && (evt_fmt->print_fmt != NULL)) {
		free(evt_fmt->print_fmt);
		evt_fmt->print_fmt = NULL;
	}
	free_field_list(evt_fmt);
	free(evt_fmt);
}

/*
 * Called to create one field_desc object dynamically which is used in uprobe
 */
int define_event_field(const char *type, const char *name,
		       struct event_format *evt_fmt)
{
	struct field_node *node = NULL;

	if (type == NULL || name == NULL || evt_fmt == NULL) {
		return E_HM_INVAL;
	}

	node = (struct field_node *)malloc(sizeof(struct field_node));
	if (node == NULL) {
		return E_HM_NOMEM;
	}

	node->desc = field_desc_of_type(type);
	if (node->desc.type_code == FIELD_TYPE_INVAL) {
		free(node);
		return E_HM_INVAL;
	}

	node->desc.type = strdup(type);
	if (node->desc.type == NULL) {
		free(node);
		return E_HM_NOMEM;
	}

	node->desc.name = strdup(name);
	if (node->desc.name == NULL) {
		free(node->desc.type);
		free(node);
		return E_HM_NOMEM;
	}

	dlist_init(&node->list);
	dlist_insert_tail(&evt_fmt->field_list, &node->list);

	return E_HM_OK;
}

/*
 * The data Layout of marshalled event_format data
 *
 * event_format:
 * | field item number<int> | field_desc | field_desc | ... |
 *
 * field_desc:
 * | type<string> | name<string> | size<unsigned int> |
 * offset<unsigned int> | type_code<unsigned int> | is_signed<unsigned int> |
 */
int marshal_event_format(struct event_format *evt_fmt, uintptr_t *data,
			 unsigned int *size)
{
	int ret = E_HM_OK;
	struct byte_stream stream;
	/*
	 * desc_info: total size of marshalled event_format data, reserve 4 bytes
	 * space saving total_size itself firstly.
	 */
	uint32_t total_size = sizeof(uint32_t);

	/*
	 * Calculate the total size of marshalled event_format raw data and
	 * the number of filed_desc items, at least include one field_desc
	 * item.
	 */
	ret = call_field_visitor((void *)&total_size, evt_fmt, calculate_size);
	if ((ret != E_HM_OK) || (total_size == sizeof(uint32_t))) {
		return E_HM_INVAL;
	}

	stream.offset = 0;
	stream.maxlen = total_size;
	stream.bytes = (uintptr_t)malloc(total_size);
	if (stream.bytes == 0) {
		return E_HM_NOMEM;
	}

	/* Marshal the number of field_desc item */
	ret = encode_int(&stream, total_size);
	if (ret != E_HM_OK) {
		free((void *)stream.bytes);
		return ret;
	}

	/* Marshal field_desc items */
	ret = call_field_visitor((void *)&stream, evt_fmt, encode_field);
	if (ret != E_HM_OK) {
		free((void *)stream.bytes);
		return ret;
	}

	*data = stream.bytes;
	*size = total_size;
	return E_HM_OK;
}

/*
 * The data format of marshalled event_format is following:
 *
 * event_format:
 * | field desc number<int> | field_desc | field_desc | ... |
 *
 * field_desc:
 * | type<string> | name<string> | size<unsigned int> |
 * offset<unsigned int> | type_code<unsigned int> | is_signed<unsigned int> |
 */
struct event_format *unmarshal_event_format(uintptr_t buf, unsigned int *size)
{
	unsigned int total_size;
	struct byte_stream stream;
	struct field_node *node = NULL;
	struct event_format *evt_fmt = NULL;

	stream.bytes = buf;
	stream.offset = 0;
	stream.maxlen = sizeof(total_size);
	total_size = decode_int(&stream);
	if (total_size == 0) {
		return NULL;
	}
	/*
	 * maxlen is the size of marshalled field_desc items, excludes
	 * the 4 bytes space used to save total_size
	 */
	stream.maxlen = total_size - sizeof(total_size);

	evt_fmt = (struct event_format *)malloc(sizeof(struct event_format));
	if (evt_fmt == NULL) {
		return NULL;
	}
	init_event_format(evt_fmt);

	/* unmarshal field_item one by one */
	while (stream.offset < (int)(unsigned int)total_size) {
		node = decode_field(&stream);
		if (node == NULL) {
			free_event_format(evt_fmt);
			return NULL;
		}
		dlist_insert_tail(&evt_fmt->field_list, &node->list);
	}

	if (stream.offset != (int)(unsigned int)total_size) {
		/* marshalled data size is oversize */
		free_event_format(evt_fmt);
		evt_fmt = NULL;
	} else if (size != NULL) {
		/* record the size of data unmarshalled from buf */
		*size = total_size;
	}

	return evt_fmt;
}

static int show_event_format_part(struct event_format *evt_fmt, size_t fmt_len)
{
	int ret = E_HM_OK;
	size_t pos = 0;

	if (fmt_len > HMTRACE_PRINT_FMT_SIZE) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		evt_fmt->print_fmt = malloc(sizeof(char) * (fmt_len + 1));
	}
	if (evt_fmt->print_fmt != NULL) {
		ret = sprintf_s(evt_fmt->print_fmt, HMTRACE_PRINT_FMT_SIZE, "\"");
		pos++;
		/*
		 * Generate string like: <name>=%fmt <name>=%fmt ... <name>=%fmt
		 */
		if (ret > 0) {
			ret = call_field_visitor(&pos, evt_fmt, define_format_part);
		}
		if (ret == E_HM_OK) {
			ret = sprintf_s(&evt_fmt->print_fmt[pos - 1], HMTRACE_PRINT_FMT_SIZE, "\"");
		}
		/*
		 * Generate string like: REC-><name>, REC-><name>, ..., REC-><name>
		 */
		if (ret > 0) {
			ret = call_field_visitor(&pos, evt_fmt, define_record_part);
		}
		if (ret != E_HM_OK) {
			free(evt_fmt->print_fmt);
			evt_fmt->print_fmt = NULL;
		}
	} else {
		ret = E_HM_NOMEM;
	}
	return ret;
}
/*
 * Called to report the print format by hmtrace for each event
 * with following style:
 * format:
 * 	filed:<type> <name>;    offset:<offset>;        size:<size>;    signed:<is_signed>;
 * 	filed:<type> <name>;    offset:<offset>;        size:<size>;    signed:<is_signed>;
 * 	filed:<type> <name>;    offset:<offset>;        size:<size>;    signed:<is_signed>;

 * 	print fmt: "<type>=%<fmt> <type>=[%<fmt>] ... ", REC-><name>, ..., REC-><mame>
 */
int show_event_format(struct event_format *evt_fmt)
{
	int ret = E_HM_OK;
	size_t fmt_len = 0;

	trace_info("format:\n");
	ret = call_field_visitor(NULL, evt_fmt, show_field);
	if ((evt_fmt->print_fmt == NULL) && (ret == E_HM_OK)) {
		/*
		 * Calculate the length of string to store entire 'print_fmt',
		 * which is consist of the format and argument part of each
		 * field.
		 */
		ret = call_field_visitor(&fmt_len, evt_fmt, calc_format_len);
		if (fmt_len == 0) {
			ret = E_HM_INVAL;
		}

		if (ret == E_HM_OK) {
			ret = show_event_format_part(evt_fmt, fmt_len);
		}
	}
	if (ret == E_HM_OK) {
		trace_info("print_fmt: %s\n", evt_fmt->print_fmt);
	}

	return ret;
}

#else /* CONFIG_HMTRACE */

void init_event_format(struct event_format *evt_fmt)
{
	UNUSED(evt_fmt);
}

struct event_format* alloc_event_format(void)
{
	return NULL;
}

void free_event_format(struct event_format *evt_fmt)
{
	UNUSED(evt_fmt);
}

int print_event(char *out, size_t maxlen,
		struct event_format *evt_fmt, const void *evt_data)
{
	UNUSED(out, maxlen, evt_fmt, evt_data);
	return E_HM_NOSYS;
}

int define_event_field(const char *type, const char *name,
		       struct event_format *evt_fmt)
{
	UNUSED(type, name, evt_fmt);
	return E_HM_NOSYS;
}

void free_field_list(const struct event_format *evt_fmt)
{
	UNUSED(evt_fmt);
}

int marshal_event_format(struct event_format *evt_fmt, uintptr_t *data,
			 unsigned int *size)
{
	UNUSED(evt_fmt, data, size);
	return E_HM_NOSYS;
}

struct event_format *unmarshal_event_format(uintptr_t buf, unsigned int *size)
{
	UNUSED(buf, size);
	return NULL;
}

int show_event_format(struct event_format *evt_fmt)
{
	UNUSED(evt_fmt);
	return E_HM_NOSYS;
}
#endif
