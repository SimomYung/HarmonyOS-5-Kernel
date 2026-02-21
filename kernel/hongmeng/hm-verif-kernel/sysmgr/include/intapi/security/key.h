/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Inner tnterface of sysmgr key manager
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 8 19:59:21 2024
 */

#ifndef SYSMGR_INTAPI_SECURITY_KEYRING
#define SYSMGR_INTAPI_SECURITY_KEYRING

#include <stdint.h>

int key_install_keyring(int short_key_id, uintptr_t key_addr, bool force);
int key_lookup_keyring(int short_key_id, void **key_addr);
void keymgr_post_proc(void);
#endif
