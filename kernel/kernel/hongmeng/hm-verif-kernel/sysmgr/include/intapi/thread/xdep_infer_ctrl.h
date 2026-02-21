/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Xdep infer ctrl interface declaration
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 26 17:02:37 2024
 */
#ifndef SYSMGR_INCLUDE_XDEP_INFER_CTRL_H
#define SYSMGR_INCLUDE_XDEP_INFER_CTRL_H

#ifdef CONFIG_THREAD_FINE_LOAD
int xdep_infer_ctrl(unsigned int cmd, unsigned long arg);
#endif

#endif
