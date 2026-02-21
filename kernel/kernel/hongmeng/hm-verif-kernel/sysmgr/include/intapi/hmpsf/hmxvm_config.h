/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: xvm global configurations
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 21 14:46:10 2024
 */
#ifndef H_SYSMGR_INCLUDE_HMPSF_HMXVM_CONFIG_H
#define H_SYSMGR_INCLUDE_HMPSF_HMXVM_CONFIG_H

enum hmxvm_jit_config_e {
	HMXVM_JIT_CONFIG_DISABLED,
	HMXVM_JIT_CONFIG_ENABLED,
	HMXVM_JIT_CONFIG_ENFORCED,
};

int hmxvm_set_xvm_jit_config(enum hmxvm_jit_config_e config);
enum hmxvm_jit_config_e hmxvm_get_xvm_jit_config(void);
bool hmxvm_is_jit_allowed(void);

#endif
