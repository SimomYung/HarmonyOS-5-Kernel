/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: internal API for sysmgr notify thread/process/session context changed
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 15 16:15:52 2025
 */
#ifndef KEYMGR_KRCTL_H
#define KEYMGR_KRCTL_H

int keymgr_krctl(int op, void *keyring, unsigned long arg);
#endif
