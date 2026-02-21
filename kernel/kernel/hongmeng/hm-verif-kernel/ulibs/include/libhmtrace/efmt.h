/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Trace event data format description API
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 11 18:02:13 2020
 */
#ifndef ULIBS_LIBHMTRACE_EFMT_H
#define ULIBS_LIBHMTRACE_EFMT_H

#include <stdint.h>
#include <inttypes.h>
#include <lib/dlist.h>
#include <libstrict/strict.h>

struct field_desc {
	char *type;
	char *name;
	unsigned int size;
	unsigned int offset;
	unsigned int type_code;
	unsigned int is_signed;
};

struct field_node {
	struct field_desc desc;
	struct dlist_node list;
};

struct event_format {
	/*
	 * Record print format of event:
	 * "<name>=%<format> <name>=%<format> ...", REC-><name>, REC-><name>..."
	 */
	char *print_fmt;
	/*
	 * Used to record field_desc items defined statically
	 * at utrace and function trace scenario.
	 */
	struct field_desc *field_array;
	/*
	 * Used to record field_desc items defined dynamically
	 * at uprobe scenario.
	 */
	struct dlist_node field_list;
};

/*
 * Called to initialize event_format during runtime for uprobe scenario.
 */
void init_event_format(struct event_format *evt_fmt);


/*
 * Called to allocate a initialized event_format data
 */
struct event_format* alloc_event_format(void);

/*
 * Called by hmtrace.elf to free the unmarshalled event_format data
 *
 * Parameters:
 *  - evt_fmt		struct event_format data received from instrument component
 */
void free_event_format(struct event_format *evt_fmt);

/* Called by hmtrace.elf to produce string output of event raw data with
 * readable style field by field.
 *
 * Parameters:
 *  - out		string buffer saves output
 *  - maxlen		maxinum length of string buffer saving output
 *  - evt_fmt		struct event_format data used to parse event raw data
 *  - evt_data		raw data buffer record event trace entry
 *
 *  Upon successful return, return the number of character, excluding terminal
 *  byte, otherwise, return errno.
 * */
int print_event(char *out, size_t maxlen,
		struct event_format *evt_fmt, const void *evt_data);

/*
 * Called by uprobe to define one field_desc for each fetcharg and record it
 * into event_format data.
 *
 * Parameters:
 *  - type		string indicates data type
 *  - name		string indicates the identifier of fetcharg
 *  - evt_fmt		struct event_format data used to record field_desc
 *
 *  Upon successful return, return E_HM_OK, otherwise, negative value.
 */
int define_event_field(const char *type, const char *name,
		       struct event_format *evt_fmt);

/*
 * Called by uprobe to free entire field_desc data allocated via define_event_field.
 *
 * Parameters:
 *  - evt_fmt		struct event_format data used to record field_desc
 */
void free_field_list(const struct event_format *evt_fmt);

/*
 * Called by instrumented process to marshal event_format data and send it
 * to hmtrace.elf.
 *
 * Parameters:
 *  - evt_fmt		struct event_format data used to record field_desc
 *  - data		dynamic allocated buffer save marshalled event_format data,
 *  			remember to free this buffer after use up.
 *  - size		save the total size of marshalled event_format data
 *
 *  Upon successful return, return E_HM_OK, otherwise, negative value.
 */
int marshal_event_format(struct event_format *evt_fmt, uintptr_t *data,
			 unsigned int *size);

/*
 * Called by hmtrace.elf to unmarshal event_format from raw data received
 * from instrumented process.
 *
 * Parameters:
 *  - buf		buffer store the marshalled event_format data
 *  - size		save the total size of buffer records marshalled event_format.
 *
 *  Upon successful return, return event_format pointer, on fail, return NULL.
 */
struct event_format *unmarshal_event_format(uintptr_t buf, unsigned int *size);


/*
 * Called by hmtrace.elf to print event format information
 *
 * Parameters:
 *  - evt_fmt		struct event_format data used to record field_desc
 *
 *  Upon successful return, return E_HM_OK, otherwise, negative value.
 */
int show_event_format(struct event_format *evt_fmt);

#undef DEFINE_PRINT_FMT
#define DEFINE_PRINT_FMT()								\
	PRINT_MAP(PRINT_ITEM(STRING, char*, char*,"s", (unsigned int)(-1), 0)		\
		  PRINT_ITEM(CHAR, char, char, "c", sizeof(char), 1)			\
		  PRINT_ITEM(SHORT, short, short, "hd", sizeof(short), 1)		\
		  PRINT_ITEM(INT, int, int, "d", sizeof(int), 1)			\
		  PRINT_ITEM(USHORT, unsigned short, unsigned short, "hu", sizeof(unsigned short), 0)\
		  PRINT_ITEM(UINT, unsigned int, unsigned int, "u", sizeof(unsigned int), 0)\
		  PRINT_ITEM(U8,  uint8_t, unsigned int, PRIu8, sizeof(uint8_t), 0)	\
		  PRINT_ITEM(U16, uint16_t, unsigned int, PRIu16, sizeof(uint16_t), 0)	\
		  PRINT_ITEM(U32, uint32_t, uint32_t, PRIu32, sizeof(uint32_t), 0)	\
		  PRINT_ITEM(U64, uint64_t, uint64_t, PRIu64, sizeof(uint64_t), 0)	\
		  PRINT_ITEM(S8,  int8_t, int, PRId8, sizeof(int8_t), 1)		\
		  PRINT_ITEM(S16, int16_t, int, PRId16, sizeof(int16_t), 1)		\
		  PRINT_ITEM(S32, int32_t, int32_t, PRId32, sizeof(int32_t), 1)		\
		  PRINT_ITEM(S64, int64_t, int64_t, PRId64, sizeof(int64_t), 1)		\
		  PRINT_ITEM(X8,  uint8_t, unsigned int, PRIX8, sizeof(uint8_t), 0)	\
		  PRINT_ITEM(X16, uint16_t, unsigned int, PRIX16, sizeof(uint16_t), 0)	\
		  PRINT_ITEM(X32, uint32_t, uint32_t, PRIX32, sizeof(uint32_t), 0)	\
		  PRINT_ITEM(X64, uint64_t, uint64_t, PRIX64, sizeof(uint64_t), 0))

#undef PRINT_PARAM
#define PRINT_PARAM(args...)	args

#undef PRINT_ITEM
#define PRINT_ITEM(alias, type, cast, fmt, size, is_signed)	FIELD_TYPE_##alias,

#undef PRINT_MAP
#define PRINT_MAP(tuple)			\
	enum FIELD_TYPE_CODE {			\
		FIELD_TYPE_INVAL = 0,		\
		PRINT_PARAM(tuple)		\
		FIELD_TYPE_ARRAY = 100,		\
		FIELD_TYPE_OUTPUT = 200,		\
	};

DEFINE_PRINT_FMT()

#define STR_OF(args...)		#args

#define SIGNED_TYPE(t)		((t)(-1) < (t)1)

#define TRACE_PARAM(args...)	args

/*
 * Called to define event_format data statically, which is used in
 * function trace, function graph and utrace.
 *
 * Usage in function trace and graph:
 * #include "define_format.h"
 * ...
 * struct trace_event functrace;
 * functrace.format = DEFINE_EVENT_FORMAT(functrace,
 * 				TRACE_FIELD(UINT, unsigned_int, field_1)
 * 				TRACE_ARRAY(INT, int, field_2, 10)
 * 				TRACE_STRING(STRING, char*, field_3));
 * ...
 *
 * Usage in utrace:
 * Developer need to modify macro UEV_HEAD_DEFIN to introduce trace_event data like,
 * #define UEV_HEAD_DEFIN(_name)
 *	DEFINE_EVENT_FORMAT(utrace_event_##name,
 *			TRACE_FIELD(UINT, unsigned_int, field_1)
 *			TRACE_ARRAY(INT, int, field_2, 10)
 *			TRACE_STRING(STRING, char *, field_3))
 * 	struct trace_event utrace_event_## name = {
 *	...
 * 	.format = TRACE_EVENT_FORMAT(utrace_event_## name),
 *	... };
 */
#define TRACE_STRING(alias, _type_, item) {	\
	.type      = #_type_,			\
	.name      = #item,			\
	.is_signed = 0U,			\
	.size      = (unsigned int)(-1),	\
	.type_code = (unsigned int)FIELD_TYPE_##alias,	\
	.offset    = 0U,			\
},

#define TRACE_ARRAY(alias, _type_, item, num) {	\
	.type      = #_type_"["STR_OF(num)"]",	\
	.name      = #item,			\
	.is_signed = (unsigned int)SIGNED_TYPE(_type_),		\
	.size      = (unsigned int)sizeof(_type_) * num,	\
	.type_code = (unsigned int)FIELD_TYPE_ARRAY + FIELD_TYPE_##alias,\
	.offset    = 0U,			\
},

#define TRACE_FIELD(alias, _type_, item) {	\
	.type      = #_type_,			\
	.name      = #item,			\
	.is_signed = (unsigned int)SIGNED_TYPE(_type_),	\
	.size      = (unsigned int)sizeof(_type_),	\
	.type_code = (unsigned int)FIELD_TYPE_##alias,	\
	.offset    = 0U,			\
},

#define _OUTPUT_NUM(_type_, item_name) {			\
	.type      = #_type_,				\
	.name      = #item_name,			\
	.is_signed = (unsigned int)SIGNED_TYPE(_type_),	\
	.size      = (unsigned int)sizeof(_type_),	\
	.type_code = (unsigned int)FIELD_TYPE_OUTPUT + enum_output_num,	\
	.offset    = 0U,				\
}
#define OUTPUT_NUM(_type_, item_name) _OUTPUT_NUM(_type_, item_name),

#define _OUTPUT_HEX_NUM(_type_, item_name) {		\
	.type      = #_type_,				\
	.name      = #item_name,			\
	.is_signed = 0U,				\
	.size      = (unsigned int)sizeof(_type_),	\
	.type_code = (unsigned int)FIELD_TYPE_OUTPUT + enum_output_hex_num,	\
	.offset    = 0U,				\
}
#define OUTPUT_HEX_NUM(_type_, item_name) _OUTPUT_HEX_NUM(_type_, item_name),

#define _OUTPUT_EVTNAME(item_name) {			\
	.type      = "char *",				\
	.name      = #item_name,			\
	.is_signed = 0U,				\
	.size      = (unsigned int)(-1),		\
	.type_code = (unsigned int)FIELD_TYPE_OUTPUT + enum_output_evtname,\
	.offset    = 0U,				\
}
#define OUTPUT_EVTNAME(item_name) _OUTPUT_EVTNAME(item_name),

#define _OUTPUT_TAG(item_name) {			\
	.type      = "char *",				\
	.name      = #item_name,			\
	.is_signed = 0U,				\
	.size      = (unsigned int)(-1),		\
	.type_code = (unsigned int)FIELD_TYPE_OUTPUT + enum_output_tag,\
	.offset    = 0U,				\
}
#define OUTPUT_TAG(item_name) _OUTPUT_TAG(item_name),

#define _OUTPUT_STRING(item_name) {			\
	.type      = "char *",				\
	.name      = #item_name,			\
	.is_signed = 0U,				\
	.size      = (unsigned int)(-1),		\
	.type_code = (unsigned int)FIELD_TYPE_OUTPUT + enum_output_string,\
	.offset    = 0U,				\
}
#define OUTPUT_STRING(item_name) _OUTPUT_STRING(item_name),

#define _OUTPUT_STRUCT(_type_, item_name) {		\
	.type      = #_type_,				\
	.name      = #item_name,			\
	.is_signed = 0U,				\
	.size      = (unsigned int)sizeof(_type_),	\
	.type_code = (unsigned int)FIELD_TYPE_OUTPUT + enum_output_struct,\
	.offset    = 0U,                                \
}
#define OUTPUT_STRUCT(_type_, item_name) _OUTPUT_STRUCT(_type_, item_name),

#define DEFINE_EVENT_FORMAT(event, tuple)				\
	static struct field_desc trace_field_array_##event[] = {	\
		tuple							\
		/* end with an invalid field_desc object */		\
		{							\
			.type      = NULL,				\
			.name      = NULL,				\
			.is_signed = 0U,				\
			.size      = 0U,				\
			.type_code = (unsigned int)FIELD_TYPE_INVAL,	\
			.offset    = 0U,				\
		}							\
	};								\
	static struct event_format trace_event_format_##event = { \
		.print_fmt = NULL,				\
		.field_array = trace_field_array_##event,	\
		.field_list = DLIST_HEAD_INIT(trace_event_format_##event.field_list) }\

#define TRACE_EVENT_FORMAT(event)	(&trace_event_format_##event)

#undef DEFINE_OUTPUT_TABLE
#define DEFINE_OUTPUT_TABLE(void)	\
	OUTPUT_TABLE(			\
		OUTPUT_ITEM(num)	\
		OUTPUT_ITEM(hex_num)	\
		OUTPUT_ITEM(tag)	\
		OUTPUT_ITEM(string)	\
		OUTPUT_ITEM(evtname)	\
		OUTPUT_ITEM(struct)	\
	)

#undef OUTPUT_ITEM
#define OUTPUT_ITEM(type) enum_output_##type,

#undef OUTPUT_TABLE
#define OUTPUT_TABLE(table)		\
	enum OUTPUT_TYPE {		\
		enum_output_invalid,	\
		table			\
		enum_output_max		\
	};

DEFINE_OUTPUT_TABLE(void)


#endif
