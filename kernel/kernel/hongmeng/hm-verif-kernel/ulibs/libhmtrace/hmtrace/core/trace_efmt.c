/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Trace event format internal implementation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 25 12:01:58 2020
 */
#include "trace_efmt.h"

#include <ctype.h>
#include <limits.h>
#include <endian.h>
#include <string.h>
#include <stdbool.h>
#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmtrace/efmt.h>
#include <libhmtrace/hmtrace.h>
#include <libhwsecurec/securec.h>

#undef PRINT_ITEM
#define PRINT_ITEM(alias, type, cast, fmt, _size_, _is_signed_) \
	static int sprint_##alias(struct byte_stream *str_buf, const type data)\
	{								\
		/* <name>=<item> */					\
		int ret = sprintf_s((char *)(str_buf->bytes + 		\
				    (uintptr_t)(unsigned int)str_buf->offset),\
				    str_buf->maxlen, "%" fmt, (cast)data);\
		size_t cnt = (size_t)(unsigned int)ret;			\
		if ((ret > 0) && (str_buf->maxlen >= cnt)) {		\
			str_buf->maxlen -= cnt;				\
			str_buf->offset += ret;				\
		} else {						\
			ret = -1;					\
		}							\
		return ret;						\
	}								\
	static int sprint_##alias##_array(struct byte_stream *str_buf,	\
					  const type *var, int num)	\
	{								\
		int i;							\
		int ret = 0;						\
		int old_offset = str_buf->offset;			\
		/* <name>=[item,item,...,item] */			\
		ret = sprint_STRING(str_buf, "[");			\
		if (ret < 0) {						\
			return ret;					\
		}							\
		for (i = 0; i < num; i++) {				\
			ret = sprint_##alias(str_buf, var[i]);		\
			if (ret < 0) {					\
				return ret;				\
			}						\
			if (i != num - 1) {				\
				ret = sprint_STRING(str_buf, ",");	\
			} else {					\
				ret = sprint_STRING(str_buf, "]");	\
			}						\
			if (ret < 0) {					\
				return ret;				\
			}						\
		}							\
		return str_buf->offset - old_offset;			\
	}								\
	static int sprint_##alias##_field(struct byte_stream *str_buf,	\
					  const struct field_desc *desc,\
					  struct byte_stream *raw_data)	\
	{								\
		int ret;						\
		int num;						\
		const type *var = (const type *)(raw_data->bytes + 	\
				(uintptr_t)(unsigned int)raw_data->offset);\
		/* Print "<name>=" */					\
		ret = sprint_STRING(str_buf, (const char*)desc->name);	\
		if (ret < 0) {						\
			return E_HM_ACCES;				\
		}							\
		ret = sprint_STRING(str_buf, "=");			\
		if (ret < 0) {						\
			return E_HM_ACCES;				\
		}							\
		/* Print string */					\
		if (desc->size == (typeof(desc->size))-1) {		\
			ret = sprint_STRING(str_buf, (const char*)var);	\
			if (ret < 0) {					\
				return E_HM_ACCES;			\
			}						\
			raw_data->offset += ret + 1;			\
		} else {						\
			/* Print array of basic type */			\
			num = (int)(desc->size / sizeof(type));		\
			if (num == 1) {					\
				ret = sprint_##alias(str_buf, *var);	\
			} else {					\
				ret = sprint_##alias##_array(str_buf, var, num);\
			}						\
			if (ret < 0) {					\
				return E_HM_ACCES;			\
			}						\
			raw_data->offset += (int)desc->size;		\
		}							\
		/* Print ' ' */						\
		ret = sprint_STRING(str_buf, " ");			\
		if (ret < 0) {						\
			return E_HM_ACCES;				\
		}							\
		return E_HM_OK;						\
	}

#undef PRINT_MAP
#define PRINT_MAP(tuple) PRINT_PARAM(tuple)

DEFINE_PRINT_FMT()

#undef PRINT_ITEM
#define PRINT_ITEM(alias, type, cast, fmt, size, is_signed) \
	sprint_##alias##_field,

#undef PRINT_MAP
#define PRINT_MAP(tuple)				\
	__maybe_unused					\
	static print_type_t sprint_func_table[] = {	\
		NULL,					\
		PRINT_PARAM(tuple)			\
		NULL					\
	};

DEFINE_PRINT_FMT()

#define FIELD_TYPE_INFO_INIT \
	{ .desc = { .type = NULL,			\
		    .name = NULL,			\
		    .size = 0,				\
		    .offset = 0,			\
		    .type_code = FIELD_TYPE_INVAL,	\
		    .is_signed = 0 },			\
	  .alias = NULL, .format = NULL }

#define FIELD_TYPE_INFO_GUARD FIELD_TYPE_INFO_INIT

#undef PRINT_ITEM
#define PRINT_ITEM(_alias_, _type_, cast, _format_, _size_, _is_signed_)\
	{ .desc = { .type = #_type_,				\
		    .name = NULL,				\
		    .size = (_size_),				\
		    .offset = 0,				\
		    .type_code = FIELD_TYPE_##_alias_,		\
		    .is_signed = (_is_signed_) },		\
	  .alias = #_alias_, .format = #_format_ },

#undef PRINT_MAP
#define PRINT_MAP(tuple)					\
	static struct field_type_info type_info_table[] = {	\
		FIELD_TYPE_INFO_GUARD,				\
		PRINT_PARAM(tuple)				\
		FIELD_TYPE_INFO_GUARD };

DEFINE_PRINT_FMT()

#define FMT_WRONG "fmt_wrong"
static struct field_fmt num_fmt = {{FMT_WRONG, "%"PRIu8, "%"PRIu16, "%"PRIu32, "%"PRIu64,
				   "%"PRId8, "%"PRId16, "%"PRId32, "%"PRId64}};
static struct field_fmt hex_num_fmt = {{FMT_WRONG, "0x%"PRIX8, "0x%"PRIX16, "0x%"PRIX32, "0x%"PRIX64,
				       NULL, NULL, NULL, NULL}};
static struct field_fmt tag_fmt =  {{"%s", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};
static struct field_fmt string_fmt =  {{"%s", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};
static struct field_fmt struct_fmt =  {{"bytes()", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};
static struct field_fmt evtname_fmt =  {{"%s", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};

#undef OUTPUT_ITEM
#define OUTPUT_ITEM(type) &type##_fmt,

#undef OUTPUT_TABLE
#define OUTPUT_TABLE(table)	\
	__maybe_unused		\
	static struct field_fmt *field_fmt_table[] = {\
		NULL,		\
		table		\
		NULL		\
	};

DEFINE_OUTPUT_TABLE(void)
#define SIZE8_FMT_INDEX(base)	((base) + 1)
#define SIZE16_FMT_INDEX(base)	((base) + 2)
#define SIZE32_FMT_INDEX(base)	((base) + 3)
#define SIZE64_FMT_INDEX(base)	((base) + 4)

static inline bool is_array_type(const struct field_desc *desc)
{
	return ((desc->type_code > FIELD_TYPE_ARRAY) && (desc->type_code < FIELD_TYPE_OUTPUT));
}

static inline bool is_output_type(const struct field_desc *desc)
{
	return (desc->type_code >= FIELD_TYPE_OUTPUT);
}

static inline unsigned int output_field_index(const struct field_desc *desc)
{
	return desc->type_code - FIELD_TYPE_OUTPUT;
}

static inline unsigned int basic_type_code(const struct field_desc *desc)
{
	if (is_array_type(desc)) {
		return desc->type_code - FIELD_TYPE_ARRAY;
	} else {
		return desc->type_code;
	}
}

int call_field_visitor(void *visit_args, struct event_format *evt_fmt,
		       field_visitor_t visit_fn)
{
	int ret = E_HM_OK;
	struct field_node *node = NULL;
	struct field_node *next = NULL;
	struct field_desc *desc = NULL;

	if (evt_fmt == NULL || visit_fn == NULL) {
		return E_HM_INVAL;
	}

	if (dlist_get_first(&evt_fmt->field_list) == NULL ||
	    dlist_get_last(&evt_fmt->field_list) == NULL) {
		hm_warn("event_format need to be initialized via "
			"init_event_format firstly\n");
		return E_HM_POSIX_NOENT;
	}

	desc = evt_fmt->field_array;
	while ((desc != NULL) && (desc->type_code != FIELD_TYPE_INVAL)) {
		ret = visit_fn(visit_args, desc, evt_fmt);
		if (ret != E_HM_OK) {
			return ret;
		}
		desc += 1;
	}

	dlist_for_each_entry_safe(node, next, &evt_fmt->field_list,
				  struct field_node, list) {
		ret = visit_fn(visit_args, &node->desc, evt_fmt);
		if (ret != E_HM_OK) {
			return ret;
		}
	}
	return ret;
}

int print_field(void *visit_args, const struct field_desc *desc,
		const struct event_format *evt_fmt)
{
	print_type_t print_fn = NULL;
	void **args = (void **)visit_args;
	struct byte_stream *str_buf = NULL;
	struct byte_stream *raw_evt = NULL;

	UNUSED(evt_fmt);
	if (visit_args == NULL) {
		return E_HM_INVAL;
	}
	str_buf = (struct byte_stream *)args[0];
	raw_evt = (struct byte_stream *)args[1];
	if (is_output_type(desc)) {
		return output_field(str_buf, desc, raw_evt);
	}

	print_fn = sprint_func_table[basic_type_code(desc)];
	if (print_fn == NULL) {
		return E_HM_INVAL;
	}

	return print_fn(str_buf, desc, raw_evt);
}

struct field_desc field_desc_of_type(const char *type)
{
	int i;
	int ret;
	char *alias = NULL;
	struct field_type_info info = FIELD_TYPE_INFO_INIT;

	if (type == NULL) {
		return info.desc;
	}

	alias = strdup(type);
	if (alias == NULL) {
		return info.desc;
	}
	/*
	 * Convert string/s8/s16/s32/s64/u8/u16/u32/u64/x8/x16/x32/x64
	 * to uppercase. See macro DEFINE_PRINT_FMT in efmt.h to understand.
	 */
	if (strcmp(type, "string") == 0) {
		ret = strcpy_s(alias, strlen(alias) + 1, "STRING");
		if (ret != 0) {
			free(alias);
			return info.desc;
		}
	} else	if ((type[0] == 's') || (type[0] == 'u') || (type[0] == 'x')) {
		alias[0] = (char)toupper((int)alias[0]);
	} else {
		free(alias);
		/* Unsupported type */
		return info.desc;
	}

	for (i = 1; type_info_table[i].desc.type_code != FIELD_TYPE_INVAL; i++) {
		if (strcmp(type_info_table[i].alias, alias) == 0) {
			info = type_info_table[i];
			break;
		}
	}
	free(alias);

	return info.desc;
}

/* Marshal and unmarshal related routines */
int encode_int(struct byte_stream *stream, unsigned int val)
{
	int ret;
	size_t size = sizeof(val);
	uint32_t le_val = htole32(val);

	ret = memcpy_s((void *)(stream->bytes +
		(uintptr_t)(unsigned int)stream->offset), stream->maxlen, &le_val, size);
	if (ret != 0) {
		return E_HM_ACCES;
	}
	stream->offset += (int)size;
	stream->maxlen -= size;
	return E_HM_OK;
}

static int encode_string(struct byte_stream *stream,
			 const char *str)
{
	int ret;
	size_t len = strlen(str);

	ret = strcpy_s((char *)(stream->bytes + (uintptr_t)(unsigned int)stream->offset),
		       stream->maxlen, str);
	if (ret != 0) {
		return E_HM_ACCES;
	}
	stream->offset += (int)len + 1;
	stream->maxlen -= len + 1;
	return E_HM_OK;
}

int encode_field(void *visit_args, const struct field_desc *desc,
		 const struct event_format *evt_fmt)
{
	int ret;
	struct byte_stream *stream = (struct byte_stream *)visit_args;
	if (stream == NULL) {
		return E_HM_INVAL;
	}

	UNUSED(evt_fmt);

	ret = encode_string(stream, desc->type);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = encode_string(stream, desc->name);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = encode_int(stream, desc->size);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = encode_int(stream, desc->offset);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = encode_int(stream, desc->type_code);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = encode_int(stream, desc->is_signed);
	if (ret != E_HM_OK) {
		return ret;
	}

	return E_HM_OK;
}

int calculate_size(void *visit_args, const struct field_desc *desc,
		   const struct event_format *evt_fmt)
{
	UNUSED(evt_fmt);
	uint32_t *total_size = (uint32_t *)visit_args;
	if (total_size == NULL) {
		return E_HM_INVAL;
	}
	*total_size += (uint32_t)((strlen(desc->type) + 1) + (strlen(desc->name) + 1) +
			sizeof(desc->size) + sizeof(desc->offset) +
			sizeof(desc->is_signed) + sizeof(desc->type_code));
	return E_HM_OK;
}

unsigned int decode_int(struct byte_stream *stream)
{
	unsigned int val;

	val = le32toh(*((unsigned int *)(stream->bytes +
			(uintptr_t)(unsigned int)stream->offset)));
	stream->offset += (int)(unsigned int)sizeof(val);
	stream->maxlen -= sizeof(val);
	return val;
}

/*
 * Unmarshal string with dynamic size from data buffer, since string
 * object is malloced during unmarshal operation, remember to free it
 * after use up.
 */
static char *decode_string(struct byte_stream *stream)
{
	size_t len;
	char *dst = NULL;
	char *src = (char *)(stream->bytes + (uintptr_t)(unsigned int)stream->offset);

	dst = strndup(src, stream->maxlen);
	if (dst == NULL) {
		return NULL;
	}

	len = strlen(dst) + 1;
	stream->offset += (int)(unsigned int)(len);
	stream->maxlen -= len;
	return dst;
}

/*
 * Unmarshal field_node object from data buffer, since field_node object
 * is malloced during unmarshal operation, remember to free it after use up.
 */
struct field_node *decode_field(struct byte_stream *stream)
{
	struct field_node *node = NULL;

	node = (struct field_node *)malloc(sizeof(struct field_node));
	if (node == NULL) {
		return NULL;
	}

	node->desc.type = decode_string(stream);
	if (node->desc.type == NULL) {
		free(node);
		return NULL;
	}

	node->desc.name = decode_string(stream);
	if (node->desc.name == NULL) {
		free(node->desc.type);
		free(node);
		return NULL;
	}

	node->desc.size = decode_int(stream);
	node->desc.offset = decode_int(stream);
	node->desc.type_code = decode_int(stream);
	node->desc.is_signed = decode_int(stream);
	dlist_init(&node->list);
	return node;
}

int show_field(void *raw_data, const struct field_desc *desc,
	       const struct event_format *evt_fmt)
{
	UNUSED(raw_data);
	UNUSED(evt_fmt);
	trace_info("\tfield:%s %s;\toffset:%u;\tsize:%u;\tsigned:%u;\n",
		   desc->type, desc->name, desc->offset,
		   desc->size, desc->is_signed);
	return E_HM_OK;
}

static struct field_fmt *get_field_fmt(const struct field_desc *desc)
{
	return field_fmt_table[output_field_index(desc)];
}

static unsigned int get_field_fmt_index(const struct field_desc *desc)
{
	/* string or struct type return index 0 */
	if (desc->type_code > (unsigned int)FIELD_TYPE_OUTPUT + enum_output_hex_num) {
		return 0;
	}

	unsigned int fmt_index = desc->is_signed * 4;
	switch (desc->size) {
	case sizeof(uint8_t):
		fmt_index = SIZE8_FMT_INDEX(fmt_index);
		break;
	case sizeof(uint16_t):
		fmt_index = SIZE16_FMT_INDEX(fmt_index);
		break;
	case sizeof(uint32_t):
		fmt_index = SIZE32_FMT_INDEX(fmt_index);
		break;
	case sizeof(uint64_t):
		fmt_index = SIZE64_FMT_INDEX(fmt_index);
		break;
	default:
		/* num or hex num type return index 0 if size is invalid */
		fmt_index = 0;
	}

	return fmt_index;
}

static char *field_fmt(const struct field_desc *desc)
{
	struct field_fmt *fmt = get_field_fmt(desc);
	unsigned int fmt_index = get_field_fmt_index(desc);

	return fmt->fmt[fmt_index];
}

#define NAME_NR   2
int calc_format_len(void *raw_data, const struct field_desc *desc,
		    const struct event_format *evt_fmt)
{
	size_t *fmt_len = (size_t *)raw_data;
	size_t field_fmt_len;

	UNUSED(evt_fmt);
	/* '<name>=%<fmt> ' and ', REC-><name>' */
	if (is_output_type(desc)) {
		char *fmt = field_fmt(desc);
		field_fmt_len = strlen(fmt);
	} else {
		field_fmt_len = strlen(type_info_table[basic_type_code(desc)].format);
	}
	*fmt_len += strlen(desc->name) * NAME_NR + strlen(desc->type) +
		    field_fmt_len +
		    strlen(", REC->") + strlen("=% ");
	if (is_array_type(desc)) {
		*fmt_len += strlen("[]");
	}

	return E_HM_OK;
}

int define_format_part(void *raw_data, const struct field_desc *desc,
		       const struct event_format *evt_fmt)
{
	int len = 0;
	int err = E_HM_OK;

	/* '<name>=%<fmt> ' or '<name>=[%<fmt>]' */
	if (is_output_type(desc)) {
		char *fmt = field_fmt(desc);
		len = sprintf_s(evt_fmt->print_fmt + (uintptr_t)(*(size_t *)raw_data),
				HMTRACE_PRINT_FMT_SIZE, "%s=%s ", desc->name, fmt);
	} else if (is_array_type(desc)) {
		len = sprintf_s(evt_fmt->print_fmt + (uintptr_t)(*(size_t *)raw_data),
				HMTRACE_PRINT_FMT_SIZE, "%s=%%[%s] ", desc->name,
				type_info_table[basic_type_code(desc)].format);
	} else {
		len = sprintf_s(evt_fmt->print_fmt + (uintptr_t)(*(size_t *)raw_data),
				HMTRACE_PRINT_FMT_SIZE, "%s=%%%s ", desc->name,
				type_info_table[basic_type_code(desc)].format);
	}
	if (len < 0) {
		hm_warn("sprintf_s field_desc name failed\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		*(size_t *)raw_data += (size_t)(unsigned int)len;
	}

	return err;
}

int define_record_part(void *raw_data, const struct field_desc *desc,
		       const struct event_format *evt_fmt)
{
	int len = 0;
	int err = E_HM_OK;

	/* ', REC-><name>' */
	len = sprintf_s(evt_fmt->print_fmt + (uintptr_t)(*(size_t *)raw_data),
			HMTRACE_PRINT_FMT_SIZE, ", REC->[%s]", desc->name);
	if (len < 0) {
		hm_warn("sprintf_s field_desc name failed\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		*(size_t *)raw_data += (size_t)(unsigned int)len;
	}

	return err;
}

static int update_output_offset(struct byte_stream *str_buf, size_t sz)
{
	int ret = E_HM_OK;

	if ((sz > 0) && (str_buf->maxlen >= sz)) {
		str_buf->maxlen -= sz;
		str_buf->offset += (int)sz;
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

static void update_field_offset(struct byte_stream *raw_evt, size_t sz)
{
	raw_evt->offset += (int)sz;
}

static int _output_num(struct byte_stream *str_buf, const void *var, const struct field_desc *desc)
{
	int ret = 0;
	size_t size = desc->size;
	unsigned int fmt_base = desc->is_signed * 4;
	struct field_fmt *fmt = get_field_fmt(desc);
	char *buf = (char *)(str_buf->bytes + (uintptr_t)(unsigned int)str_buf->offset);

	switch (size) {
	case sizeof(uint8_t):
		ret = sprintf_s(buf, str_buf->maxlen, fmt->fmt[SIZE8_FMT_INDEX(fmt_base)], *(uint8_t *)var);
		break;
	case sizeof(uint16_t):
		ret = sprintf_s(buf, str_buf->maxlen, fmt->fmt[SIZE16_FMT_INDEX(fmt_base)], *(uint16_t *)var);
		break;
	case sizeof(uint32_t):
		ret = sprintf_s(buf, str_buf->maxlen, fmt->fmt[SIZE32_FMT_INDEX(fmt_base)], *(uint32_t *)var);
		break;
	case sizeof(uint64_t):
		ret = sprintf_s(buf, str_buf->maxlen, fmt->fmt[SIZE64_FMT_INDEX(fmt_base)], *(uint64_t *)var);
		break;
	default:
		return E_HM_INVAL;
	}

	int output_offset = ret;
	ret = update_output_offset(str_buf, (size_t)(unsigned int)output_offset);
	if (ret < E_HM_OK) {
		return ret;
	}

	return (int)size;
}

static int _output_string(struct byte_stream *str_buf, const void *var, const struct field_desc *desc)
{
	UNUSED(desc);

	int ret = 0;
	char *buf = (char *)(str_buf->bytes + (uintptr_t)(unsigned int)str_buf->offset);

	ret = sprintf_s(buf, str_buf->maxlen, "%s", var);
	int update_offset = ret;
	int field_offset = ret + 1;
	ret = update_output_offset(str_buf, (size_t)(unsigned int)update_offset);
	if (ret < E_HM_OK) {
		return ret;
	}

	return field_offset;
}

#define NR_OUTPUT_STRUCT 4
static int _output_struct(struct byte_stream *str_buf, const void *var, const struct field_desc *desc)
{
	size_t i;
	int ret;
	size_t size = desc->size;
	char *buf;

	ret = _output_string(str_buf, "bytes(", desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	for (i = 0; i < size; i++) {
		buf = (char *)(str_buf->bytes + (uintptr_t)(unsigned int)str_buf->offset);
		ret = sprintf_s(buf, str_buf->maxlen, "%02x", *((unsigned char *)var + i));
		if (ret < 0) {
			return ret;
		}
		ret = update_output_offset(str_buf, (size_t)(unsigned int)ret);
		if (ret < 0) {
			return ret;
		}
		/* One space is output for every four values */
		if ((((i + 1) % NR_OUTPUT_STRUCT) == 0) && (i != (size - 1))) {
			ret = _output_string(str_buf, " ", desc);
			if (ret < E_HM_OK) {
				return ret;
			}
		}
	}

	ret = _output_string(str_buf, ")", desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	return (int)desc->size;
}

typedef int (*_output_fn)(struct byte_stream *buf, const void *var, const struct field_desc *desc);
/* output format is <name>=<value> */
static int _output_field(struct byte_stream *str_buf, struct byte_stream *raw_evt,
			 const struct field_desc *desc, _output_fn fn)
{
	int ret;
	const void *var = (const void *)(raw_evt->bytes + (uintptr_t)(unsigned int)raw_evt->offset);

	ret = _output_string(str_buf, (const char*)desc->name, desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	ret = _output_string(str_buf, "=", desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	ret = fn(str_buf, var, desc);
	if (ret < E_HM_OK) {
		return ret;
	}
	size_t sz = (size_t)(unsigned int)ret;
	update_field_offset(raw_evt, sz);

	ret = _output_string(str_buf, " ", desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	return E_HM_OK;
}

static int output_field_num(struct byte_stream *str_buf, struct byte_stream *raw_evt, const struct field_desc *desc)
{
	return _output_field(str_buf, raw_evt, desc, _output_num);
}

static int output_field_hex_num(struct byte_stream *str_buf, struct byte_stream *raw_evt, const struct field_desc *desc)
{
	return _output_field(str_buf, raw_evt, desc, _output_num);
}

static int output_field_string(struct byte_stream *str_buf, struct byte_stream *raw_evt, const struct field_desc *desc)
{
	return _output_field(str_buf, raw_evt, desc, _output_string);
}

static int output_field_struct(struct byte_stream *str_buf, struct byte_stream *raw_evt, const struct field_desc *desc)
{
	return _output_field(str_buf, raw_evt, desc, _output_struct);
}

static int output_field_evtname(struct byte_stream *str_buf, struct byte_stream *raw_evt, const struct field_desc *desc)
{
	int ret = E_HM_OK;
	const void *var = (const void *)(raw_evt->bytes + (uintptr_t)(unsigned int)raw_evt->offset);

	ret = _output_string(str_buf, var, desc);
	if (ret < E_HM_OK) {
		return ret;
	}
	size_t sz = (size_t)(unsigned int)ret;
	update_field_offset(raw_evt, sz);

	ret = _output_string(str_buf, ": ", desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	return E_HM_OK;
}

static int output_field_tag(struct byte_stream *str_buf, struct byte_stream *raw_evt, const struct field_desc *desc)
{
	int ret = E_HM_OK;
	const void *var = (const void *)(raw_evt->bytes + (uintptr_t)(unsigned int)raw_evt->offset);

	ret = _output_string(str_buf, var, desc);
	if (ret < E_HM_OK) {
		return ret;
	}
	size_t sz = (size_t)(unsigned int)ret;
	update_field_offset(raw_evt, sz);

	ret = _output_string(str_buf, " ", desc);
	if (ret < E_HM_OK) {
		return ret;
	}

	return E_HM_OK;
}
#undef OUTPUT_ITEM
#define OUTPUT_ITEM(type) output_field_##type,

#undef OUTPUT_TABLE
#define OUTPUT_TABLE(table)	\
	__maybe_unused		\
	static output_field_fn output_field_fn_table[] = {\
		NULL,		\
		table		\
		NULL		\
	};
DEFINE_OUTPUT_TABLE(void)

int output_field(struct byte_stream *str_buf, const struct field_desc *desc,
		 struct byte_stream *raw_evt)
{
	unsigned int output_id = output_field_index(desc);
	output_field_fn fn = output_field_fn_table[output_id];
	if (fn == NULL) {
		return E_HM_INVAL;
	}

	return fn(str_buf, raw_evt, desc);
}

