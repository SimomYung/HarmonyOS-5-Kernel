/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Trace event format internal implementation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 25 12:08:20 2020
 */
#ifndef ULIBS_LIBHMTRACE_TRACE_EFMT_H
#define ULIBS_LIBHMTRACE_TRACE_EFMT_H

#include <stdio.h>
#include <hongmeng/errno.h>
#include <libhmtrace/efmt.h>
#include <libstrict/strict.h>

#define HMTRACE_PRINT_FMT_SIZE	2048
#define FIELD_FMT_SIZE		9

struct byte_stream {
	uintptr_t bytes;
	int offset;
	size_t maxlen;
};

struct field_fmt {
	char *fmt[FIELD_FMT_SIZE];
};

typedef int (*print_type_t)(struct byte_stream *str_buf, const struct field_desc *desc,
			    struct byte_stream *evt_ent);
typedef int (*output_field_fn)(struct byte_stream *buf, struct byte_stream *raw_evt, const struct field_desc *desc);
typedef int (*field_visitor_t)(void *visit_args, const struct field_desc *desc,
			       const struct event_format *evt_fmt);

struct field_desc field_desc_of_type(const char *type);

/* Visitor routines of field_desc */
int call_field_visitor(void *visit_args, struct event_format *evt_fmt,
		       field_visitor_t visit_fn);
int print_field(void *visit_args, const struct field_desc *desc,
		const struct event_format *evt_fmt);
int output_field(struct byte_stream *str_buf, const struct field_desc *desc,
		 struct byte_stream *raw_evt);
int calculate_size(void *visit_args, const struct field_desc *desc,
		   const struct event_format *evt_fmt);
int encode_field(void *visit_args, const struct field_desc *desc,
		 const struct event_format *evt_fmt);
int show_field(void *raw_data, const struct field_desc *desc,
	       const struct event_format *evt_fmt);
int calc_format_len(void *raw_data, const struct field_desc *desc,
		    const struct event_format *evt_fmt);
int define_format_part(void *raw_data, const struct field_desc *desc,
		       const struct event_format *evt_fmt);
int define_record_part(void *raw_data, const struct field_desc *desc,
		       const struct event_format *evt_fmt);
int encode_int(struct byte_stream *stream, unsigned int val);
struct field_node *decode_field(struct byte_stream *stream);
unsigned int decode_int(struct byte_stream *stream);

struct field_type_info {
	struct field_desc desc;
	const char *alias;  /* used to define field_desc */
	const char *format; /* used to report field_desc */
};

#endif /* ULIBS_LIBHMTRACE_TRACE_EFMT_H */
