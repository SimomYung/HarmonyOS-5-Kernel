/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Define secure component
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:19:41 2019
 */
#ifndef ULIBS_LIBSEC_COMPONENT_H
#define ULIBS_LIBSEC_COMPONENT_H

#include <stddef.h>
#include <stdint.h>
#include "server_env.h"
#include <stdbool.h>

typedef int (*sec_comp_init_ft)(const struct sec_server_env *);
typedef void (*sec_comp_fini_ft)(void);
struct sec_comp_ops {
	sec_comp_init_ft init;
	sec_comp_fini_ft fini;
};

struct sec_mod;

/*
 * Check use-after-free:
 * The mod is initialized to static variable and will not be freed.
 */
struct sec_comp {
	struct sec_mod *mod;
	struct sec_comp_ops ops;
};

int sec_comp_init(struct sec_comp const *comp, const struct sec_server_env *env);
void sec_comp_fini(struct sec_comp const *comp);

struct sec_chk_method;

void sec_add_method(struct sec_chk_method *method, size_t num, bool high_priority);
void sec_del_method(struct sec_chk_method *method, size_t num);

#endif
