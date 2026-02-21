/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: header of device/driver compatible string
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 18 10:26:13 2020
 */

#ifndef ULIBS_INCLUDE_DEVHOST_COMPATIBLE_H
#define ULIBS_INCLUDE_DEVHOST_COMPATIBLE_H

#include <hongmeng/types.h>

struct compatible_string {
	char *value;
	size_t length;
};

int compatible_string_init(struct compatible_string *compat_str,
			   const char *compat, size_t compat_len);

void compatible_string_reset(struct compatible_string *compat_str);

int compatible_string_concat(struct compatible_string *compat_str,
			     const char *compat, size_t compat_len);

const char *compatible_string_next(const struct compatible_string *compat,
				   const char *cur);

/* return matched compatible string in `a` */
const char *compatible_string_compare(const struct compatible_string *a,
				      const struct compatible_string *b);

/* only for ko&so driver compatible */
int compatible_string_init_from_libname(struct compatible_string *compat_str,
					const char *libname);

#endif /* ULIBS_INCLUDE_DEVHOST_COMPATIBLE_H */
