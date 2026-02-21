/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: sve header
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 09 16:51:35 2024
 */
#ifndef AARCH64_HMASM_SVE_H
#define AARCH64_HMASM_SVE_H

#include <hmasm/types.h>

#ifdef CONFIG_AARCH64_SVE

u64 arch_read_zcr_features(void);

#else

static inline u64 arch_read_zcr_features(void)
{
	return 0;
}

#endif /* CONFIG_AARCH64_SVE */

#endif /* !AARCH64_HMASM_SVE_H */
