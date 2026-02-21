/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Provide interfaces to implement servers with security checks
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:21:57 2019
 */
#ifndef ULIBS_LIBSEC_SERVER_H
#define ULIBS_LIBSEC_SERVER_H

#include <stddef.h>
#ifndef __HOST_LLT__
#include <stdbool.h>
#else
#include <hmkernel/types.h>
#endif

#include "chk.h"
#include "cred.h"
#include "module.h"
#include "component.h"
#include "server_env.h"

struct sec_mod;
struct sec_comp;

/*
 * Check use-after-free:
 * This struct is not used in hm-security, and used in 
 * hm-filesystem/fs/security/security.c. And it's initialized staitcally.
 */
struct sec_mod_support {
	uint32_t bit;
	struct sec_mod *mod;
};

struct sec_server_attr {
	bool is_creator;
	struct sec_server_env *env;
};

/*
 * Check use-after-free:
 * This struct is not used in hm-security, and used in 
 * hm-filesystem/fs/security/security.c. And it's initialized staitcally.
 */
struct sec_comp_support {
	uint32_t bit;
	struct sec_comp *comp;
};

int sec_server_init_with_env(struct sec_mod **mod, size_t mod_num,
			     struct sec_comp **comp, size_t comp_num, const struct sec_server_attr *attr);

int sec_server_init(struct sec_mod **mod, size_t mod_num,
		    struct sec_comp **comp, size_t comp_num, bool is_creator);

void sec_server_fini(struct sec_mod * const *mod, size_t mod_num,
		     struct sec_comp * const *comp, size_t comp_num);

#endif
