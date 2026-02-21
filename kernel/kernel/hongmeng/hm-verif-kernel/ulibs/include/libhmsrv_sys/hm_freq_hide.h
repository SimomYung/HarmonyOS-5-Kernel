/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Freq-Info hide feature.
 * Author: Huawei HISI Kirin
 * Create: Fri Sep 13 15:48:19 2024
 */

#ifndef ULIBS_INCLUDE_HM_FREQ_HIDE_H
#define ULIBS_INCLUDE_HM_FREQ_HIDE_H

#ifdef CONFIG_DFC_FREQ_HIDE
bool hm_freq_privileged_user(void);
#else
static inline bool hm_freq_privileged_user(void)
{
    return false;
}
#endif
#endif