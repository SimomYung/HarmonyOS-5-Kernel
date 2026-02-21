/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define TLB operation related macros so it can be compiled in LLT.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 25 03:58:22 2024
 */

#ifndef AARCH64_ASM_TLBCONSTS_H
#define AARCH64_ASM_TLBCONSTS_H

#define TLBI_VA_BITS    44
#define TLBI_VA_SHIFT   12
#define TLBI_ASID_SHIFT 48

#define TLBI_IPA_BITS   36

#endif
