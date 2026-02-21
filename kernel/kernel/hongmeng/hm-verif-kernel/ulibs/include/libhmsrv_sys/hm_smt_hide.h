/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: SMT hide feature.
 * Author: Huawei HISI Kirin
 * Create: Fri Nov 03 19:03:19 2023
 */

#ifndef ULIBS_INCLUDE_HM_SMT_HIDE_H
#define ULIBS_INCLUDE_HM_SMT_HIDE_H

#ifdef CONFIG_SCHED_SMT_HIDE
bool hm_smt_privileged_user(void);
#else
static inline bool hm_smt_privileged_user(void)
{
	return true;
}
#endif
#endif