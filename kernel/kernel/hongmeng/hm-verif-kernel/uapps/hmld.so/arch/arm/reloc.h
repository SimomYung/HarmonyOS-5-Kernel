/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of reloc type
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */

#ifndef HMLDSO_ARCH_ARM_RELOC_H
#define HMLDSO_ARCH_ARM_RELOC_H

#define LDSO_REL_SYMBOLIC    R_ARM_ABS32
#define LDSO_REL_GOT         R_ARM_GLOB_DAT
#define LDSO_REL_PLT         R_ARM_JUMP_SLOT
#define LDSO_REL_RELATIVE    R_ARM_RELATIVE
#define LDSO_REL_COPY        R_ARM_COPY
#define LDSO_REL_DTPMOD      R_ARM_TLS_DTPMOD32
#define LDSO_REL_DTPOFF      R_ARM_TLS_DTPOFF32
#define LDSO_REL_TPOFF       R_ARM_TLS_TPOFF32

#endif
