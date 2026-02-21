/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network revoke handler
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 21 09:55:16 2021
 */

#ifndef __LIBUXNET_UX_REVOKE_HANDLER_
#define __LIBUXNET_UX_REVOKE_HANDLER_

#include <stdint.h>

typedef void (*ux_revoke_handler_t)(uint32_t cnode_idx);

int ux_register_revoke_handler(const char *name, const ux_revoke_handler_t handler);
void ux_unregister_revoke_handler(const ux_revoke_handler_t handler);

void ux_revoke_by_cnode(uint32_t cnode_idx);
#endif
