/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: ubsan
 */

#include <stdbool.h>
#include <stdio.h>

#include "ubsan.h"

extern void abort(void);


static volatile bool ubsan_abort = true;

static bool is_int_type(struct type_descriptor *type)
{
    return type->type_kind == type_kind_int;
}

static bool is_signed_int_type(struct type_descriptor *type)
{
    return is_int_type(type) && (type->type_info & 1);
}

static bool is_unsigned_int_type(struct type_descriptor *type)
{
    return is_int_type(type) && !(type->type_info & 1);
}

static void handle_overflow__(struct source_location *loc, const char *reason)
{
    printf("======================================================================\n");
    printf("UBSAN: %s in %s:%u:%u\n", reason, loc->file_name, loc->line, loc->column);
    printf("======================================================================\n");
    if (ubsan_abort) {
        abort();
    }
}

static void handle_integer_overflow(struct overflow_data *data, void *lhs, void *rhs, char op)
{
    struct type_descriptor *type = data->type;

    if (!is_int_type(type)) {
        printf("UBSAN: warning(%s:%d, %s)\n", __FILE__, __LINE__, __func__);
    }

    char *reason = (is_signed_int_type(type) ? "signed-integer-overflow" : "unsigned-integer-overflow");
    handle_overflow__(&data->loc, reason);
}

void __ubsan_handle_add_overflow(struct overflow_data *data, void *lhs, void *rhs)
{
    handle_integer_overflow(data, lhs, rhs, '+');
}

void __ubsan_handle_sub_overflow(struct overflow_data *data, void *lhs, void *rhs)
{
    handle_integer_overflow(data, lhs, rhs, '-');
}

void __ubsan_handle_mul_overflow(struct overflow_data *data, void *lhs, void *rhs)
{
    handle_integer_overflow(data, lhs, rhs, '*');
}


void __ubsan_handle_negate_overflow(struct overflow_data *data, void *old_val)
{
    handle_overflow__(&data->loc, "negation-overflow");
}

void __ubsan_handle_divrem_overflow(struct overflow_data *data, void *lhs, void *rhs)
{
    handle_overflow__(&data->loc, "division-overflow");
}
