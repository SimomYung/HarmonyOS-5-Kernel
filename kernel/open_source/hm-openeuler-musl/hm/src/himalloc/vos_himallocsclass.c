/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "vos_himallocsclass.h"
#include "himalloc_algo.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * 小于VOS_HIMALLOC_LOOKUP_SIZE，向上对齐至VOS_HIMALLOC_LG_MIN_SIZE
 * 然后通过size >> VOS_HIMALLOC_LG_MIN_SIZE查表获取index
 * 大于VOS_HIMALLOC_LOOKUP_SIZE且小于VOS_HIMALLOC_TCACHE_SIZE，向上对齐至1K
 * 然后通过(VOS_HIMALLOC_LOOKUP_SIZE >> VOS_HIMALLOC_LG_MIN_SIZE + size >> 10)查表获取index
 */
HIDDEN const uint8_t g_aucHimallocSize2Index[] = {
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4,
    5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8,
    9, 9, 9, 9, 9, 9, 9, 9,
    10, 10, 10, 10, 10, 10, 10, 10,
    11, 11, 11, 11, 11, 11, 11, 11,
    12, 12, 12, 12, 12, 12, 12, 12, 13
};

/*
 * 根据size index获取size
 * size class的间隔越大，单个size class宽容度越高，不容易产生外碎片，
 * 但是间隔过大，会导致内碎片偏高，从而导致用户可见范围内空闲内存降低，
 * 如果用户启动内存明显偏高，可以将初始间隔调整为8等方案减低启动内存
 */
HIDDEN const size_t g_auvHimallocIndex2size[VOS_HIMALLOC_MAX_SMALL_INDEX] = {
    16, 32, 64, 128, 192,
    256, 320, 384, 512,
    640, 768, 896, 1024, 2048
};

HIDDEN uint16_t VosHimallocSize2Index(size_t uvSize)
{
    size_t uvIndex;

    VOS_HIMALLOC_ASSERT(uvSize <= VOS_HIMALLOC_MAX_SMALL_SIZE);

    if (uvSize <= VOS_HIMALLOC_MAX_TINY_SIZE) {
        uvIndex = (uvSize - 1U) >> VOS_HIMALLOC_LG_TINY_ALIGN_SIZE;
    } else {
        uvIndex = (VOS_HIMALLOC_MAX_TINY_SIZE >> VOS_HIMALLOC_LG_TINY_ALIGN_SIZE);
    }

    return g_aucHimallocSize2Index[uvIndex];
}

HIDDEN size_t VosHimallocIndex2Size(uint16_t usIndex)
{
    /* usIndex的有效性由调用函数保证 */
    return g_auvHimallocIndex2size[usIndex];
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
