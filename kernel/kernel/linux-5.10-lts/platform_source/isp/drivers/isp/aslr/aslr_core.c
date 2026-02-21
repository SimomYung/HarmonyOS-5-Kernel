/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: isp aslr core implemantation.
 * Create: 2025/2/26
 */

#include "aslr_core_pri.h"

__attribute__((weak)) uintptr_t OsAslrMemMap(uintptr_t addr)
{
    return addr;
}

static uintptr_t OsAslrGetSymbolAddr(const AslrRelaItem *item, uintptr_t random)
{
    return 0;
}

static uintptr_t OsAslrGetRelocStartIndex(const AslrCB *data)
{
    uintptr_t relocIndex = 0;
    AslrRelaItem *item = NULL;
    for (; relocIndex < data->relInfo.relCount; relocIndex++) {
        item = (AslrRelaItem *)(data->relInfo.relTable + (data->relInfo.relEntrySize * relocIndex));
        if (item->rela.r_offset >= data->imageInfo.linkInfo->aslrImageStart && \
            item->rela.r_offset < data->imageInfo.linkInfo->aslrImageEnd) {
            continue;
        }
        return relocIndex;
    }
    return relocIndex;
}

uintptr_t OsAslrDoProcess(AslrCB *data)
{
    AslrRelaItem *item = NULL;
    uintptr_t relocIndex = OsAslrGetRelocStartIndex(data);
    for (uintptr_t i = relocIndex; i < data->relInfo.relCount; ++i) {
        item = (AslrRelaItem *)(data->relInfo.relTable + (data->relInfo.relEntrySize * i));

        data->relocParam.relocType = LD_ELF_R_TYPE(item->rela.r_info);
        data->relocParam.shType = (uint32_t)data->imageInfo.shType; /* rel or rela section */
        data->relocParam.symAdd = OsAslrGetSymbolAddr(item, data->random); /* reloc position call func or data addr */
        data->relocParam.position = OsAslrMemMap((uintptr_t)item->rela.r_offset + data->random);
        if (data->relocParam.position == 0) {
            return LOS_NOK;
        }

        if (data->relocParam.shType == LD_SHT_RELA) {
            data->relocParam.addend = item->rela.r_addend;
        }
        data->relocParam.baseSegment = data->random; /* image loaded addr */
        data->relocParam.thumbFunc = 0;
        if (ArchAslrRelocImp(&item->rela, &data->relocParam) != LOS_OK) {
            return LOS_NOK;
        }
    }
    return LOS_OK;
}
