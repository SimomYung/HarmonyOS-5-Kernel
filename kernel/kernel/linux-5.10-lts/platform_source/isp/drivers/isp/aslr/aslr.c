/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: isp aslr arch implemantation.
 * Create: 2025/2/26
 */

#include "aslr_core_pri.h"

static uint32_t AslrArmRelativeReloc(const LDRelocParam *relocParam)
{
    uintptr_t addend;
    uintptr_t position = relocParam->position;

    if (relocParam->shType == LD_SHT_RELA) {
        addend = (uintptr_t)relocParam->addend;
    } else {
        addend = *(uintptr_t *)position;
    }

    /* B(S) + A */
    *(uintptr_t *)position = relocParam->baseSegment + addend;
    return LOS_OK;
}

uint32_t ArchAslrRelocImp(const LD_ELF_RELA *rela, const LDRelocParam *relocParam)
{
    uint32_t ret = LOS_NOK;
    switch (LD_ELF_R_TYPE(rela->r_info)) {
        case OS_R_ARM_AARCH64_RELATIVE:
            ret = AslrArmRelativeReloc(relocParam);
            break;
        default:
            break;
    }
    return ret;
}
