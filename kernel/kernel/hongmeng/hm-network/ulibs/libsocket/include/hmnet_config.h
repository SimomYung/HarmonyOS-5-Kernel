/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for ipcfg
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 27 14:20:01 2019
 */

#ifndef __HM_NET_CONFIG_H_
#define __HM_NET_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

int hm_net_show_stats(const char *protocol);
int hm_net_cfg_log(const char *module, const char *arg);

#ifdef __cplusplus
}
#endif

#endif
