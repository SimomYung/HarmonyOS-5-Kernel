/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "vos_himallocgrow.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN size_t g_uvHimallocPhyPageSize;

HIDDEN VOS_MEM_AREA_T g_HimallocAreaId = VOS_MEM_INVALID_T;

/* linux利用mmap所获取内存赋予读写权限 */
#define PAGES_PROT_COMMIT ((int32_t)((uint32_t)PROT_READ | (uint32_t)PROT_WRITE))

/* 操作系统物理页的大小 */
#define VOS_HIMALLOC_PPAGE_SIZE g_uvHimallocPhyPageSize

/* 操作系统物理页的大小 */
#define VOS_HIMALLOC_PPAGE_SIZE_MASK (VOS_HIMALLOC_PPAGE_SIZE - 1U)

#define VOS_HIMALLOC_MIN_GROW_SIZE (128U * 1024U)

HIDDEN uint32_t VosHimallocGrowModuleInit(void)
{
    uint32_t uiRet = HIMALLOC_OK;
    g_uvHimallocPhyPageSize = (size_t)sysconf(_SC_PAGESIZE);
    g_HimallocAreaId = VosMemAreaInit();
    if (g_HimallocAreaId == VOS_MEM_INVALID_T) {
        uiRet = HIMALLOC_ERRNO_NOMEM;
    }

    return uiRet;
}

static void *VosHimallocGrowDefault(uint8_t ucArenaId, size_t *puvAllocSize, VosMemAreaRegionTypeE *penAllocType)
{
    size_t uvAllocSize;
    void *pAllocAddr;

    (void)ucArenaId;

    uvAllocSize = *puvAllocSize;
    if (uvAllocSize < VOS_HIMALLOC_MIN_GROW_SIZE) {
        uvAllocSize = VOS_HIMALLOC_MIN_GROW_SIZE;
    }

    pAllocAddr = mmap(NULL, uvAllocSize, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANON), -1, 0);
    if (pAllocAddr == ulong_to_ptr((unsigned long)MAP_FAILED, void)) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, size(%lu)",
            __FUNCTION__, uvAllocSize);
        __hmattr_sideff_resolved("no relative side effect");
        errno = ENOMEM;
        return HIMALLOC_NULL_PTR;
    }
    *penAllocType = MEM_AREA_REGION_MMAP;
    *puvAllocSize = uvAllocSize;

    return pAllocAddr;
}

static void VosHimallocShrinkDefault(uint8_t ucArenaId, void *pAddr, size_t size)
{
    (void)ucArenaId;

    (void)munmap(pAddr, size);

    return;
}

HIDDEN void *VosHimallocGrowAlloc(VOS_HIMALLOC_ARENA_S *pstArena, size_t *puvSize, VosMemAreaRegionTypeE *penAllocType)
{
#if (VOS_HIMALLOC_VPAGE_SIZE != (1 << VOS_HIMALLOC_LG_BLOCK_SIZE))
    /* 自动扩展需要多扩展半页，防止尾部预留后内存不足 */
    if (*puvSize + VOS_HIMALLOC_VPAGE_SIZE < VOS_HIMALLOC_VPAGE_SIZE) {
        /* 传入size过大导致溢出 */
        VosHimallocLogWrite(HIMALLOC_ERRNO_OVERFLOW, "[HIMALLOC-%s]malloc size is too large(%lu)",
            __FUNCTION__, *puvSize);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    *puvSize += VOS_HIMALLOC_VPAGE_SIZE;
#endif

    return VosHimallocGrowDefault(pstArena->ucArenaId, puvSize, penAllocType);
}

HIDDEN void VosHimallocGrowFree(VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr, size_t uvSize,
    VosMemAreaRegionTypeE enAllocType)
{
    (void)enAllocType;
    VosHimallocShrinkDefault(pstArena->ucArenaId, pAddr, uvSize);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
