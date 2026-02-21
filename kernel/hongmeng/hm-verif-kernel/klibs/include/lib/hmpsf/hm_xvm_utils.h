/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 24 19:07:32 2023
 */
#ifndef XVM_ADAPTER_HM_UTILS_H
#define XVM_ADAPTER_HM_UTILS_H

#include <lib/hmpsf/hmpsf.h>

/*
 * When loading an xvm module, a bind_helpers callback is needed to bind helpers
 * for the module. This is an indispensable part of the module loading procedure.
 * Different from bind_helpers, which is per-module-type, find_entries is
 * per-module-instance. So, find_entries does not need to be called at module
 * loading. That said, we call xvm_if_std_find_entries at module loading, because
 * the standard interface is special and deserves to be stored at xvm_module_t.
 * By storing the entry functions of the standard interface at xvm_module_t, the
 * implementation of xvm maps is made convenient, as the map entry functions can
 * be found at xvm_module_t directly, and does not depend on host-side managment
 * that xvm runtime is oblivious to. But entry functions of non-standard interfaces
 * shall not be stored at xvm_module_t.
 */

/*
 * A closure type is provided for the host to bind helper functions.
 * Sometimes a callback suffices, and no data needs to be carried. Sometimes the
 * env field shall be used to reference data that the callbacks need to use.
 *
 * module: xvm_module_t *
 */
typedef int (*xvm_bind_helpers_callback_t)(void *module, void *env);
typedef struct {
	xvm_bind_helpers_callback_t cb;
	void *env;
} xvm_bind_helpers_closure_t;

struct hmpsf_vm_if_s* hmpsf_xvm_register(struct hmpsf_if_s *ifs);

#endif
