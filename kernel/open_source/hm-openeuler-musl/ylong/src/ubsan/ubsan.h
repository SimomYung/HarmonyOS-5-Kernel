/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: ubsan
 */

#ifndef UBSAN_H
#define UBSAN_H

#include <stdint.h>

enum {
    // An integer type. Lowest bit is 1 for a signed value, 0 for an unsigned
    type_kind_int = 0x0000,
    type_kind_float = 0x0001,
    type_unknow = 0xffff
};

struct type_descriptor {
    uint16_t type_kind;
    uint16_t type_info;
    char type_name[1];
};

struct source_location {
    const char *file_name;
    uint32_t line;
    uint32_t column;
};

struct overflow_data {
    struct source_location loc;
    struct type_descriptor *type;
};


/*
 * When compiling with -fsanitize=signed-integer-overflow the compiler expects functions
 * with the following signatures.
 */
void __ubsan_handle_add_overflow(struct overflow_data *data, void *lhs, void *rhs);
void __ubsan_handle_sub_overflow(struct overflow_data *data, void *lhs, void *rhs);
void __ubsan_handle_mul_overflow(struct overflow_data *data, void *lhs, void *rhs);
void __ubsan_handle_negate_overflow(struct overflow_data *data, void *old_val);
void __ubsan_handle_divrem_overflow(struct overflow_data *data, void *lhs, void *rhs);

#endif
