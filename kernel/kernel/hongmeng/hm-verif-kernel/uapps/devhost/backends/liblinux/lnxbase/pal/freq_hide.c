/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Freq-Info hide feature.
 * Author: Huawei HISI Kirin
 * Create: Fri Sep 13 15:48:19 2024
 */
 
#include <liblinux/pal.h>
#include <libhmsrv_sys/hm_freq_hide.h>
 
bool liblinux_pal_get_freq_privileged_user(void)
{
	return hm_freq_privileged_user();
}