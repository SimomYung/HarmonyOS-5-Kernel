/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Keymgr path info
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 11 19:37:31 2021
 */
#ifndef LIBHMSRV_KEYMGR_H
#define LIBHMSRV_KEYMGR_H

#define KEYMGR_PATH	"keymgr.actv"
rref_t key_get_keymgr_rref(void);
int key_release_keymgr_rref(rref_t rref);

#endif
