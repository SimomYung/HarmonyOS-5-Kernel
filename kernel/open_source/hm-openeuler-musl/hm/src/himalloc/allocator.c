/**
 * @file allocator.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator 源代码文件。
 * @details
 * @author anonymous
 * @date 2021-03-10
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-10  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "himalloc_inner.h"
#include "allocator_inner.h"
#include "himalloc.h"
#include "allocator.h"

/* 鸿蒙版本下为了使用鸿蒙的推栈功能，需要包含鸿蒙OS里的dump头文件 */
#ifdef OS_HONGMENG
#include <mem/dump.h>
#endif

/* HM（内存健康管理）版本下，还需要依赖mem_hm CBB */
#ifdef CONFIG_DEBUG_BUILD
#include "mem_hm.h"
#endif

/* 底层libc库是musl的情况下，需要包含libc.h 主要用于解决PAGE_SIZE的定义问题 */
#ifdef _MUSL_SOURCE
#include "libc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* 底层libc库是glibc的情况下，需要定义PAGE_SIZE宏 */
#ifndef _MUSL_SOURCE
#define PAGE_SIZE (getpagesize())
#endif

/**
 * @ingroup allocator
 * @brief 对memalign的两个参数进行有效性检测。
 * @par 描述：对memalign的两个参数进行有效性检测。
 * @attention \n
 * 1.该静态接口是正常版本和HM（内存健康管理）版本共用的接口。
 * @param align     [IN] 地址对齐的大小，必须是2的幂次
 * @param size      [IN] 申请内存空间的大小
 * @retval ture 参数正确
 * @retval false 参数错误
 * @par 依赖：无。
 * @see MemAlign、MemAlignF
 */
static __pure bool IsMemAlignParaRight(size_t align, size_t size)
{
    if ((align & (size_t)(-(ssize_t)align)) != align) {
        errno = EINVAL;
        return false;
    }

    if (size > SIZE_MAX - align) {
        errno = ENOMEM;
        return false;
    }

    if (size == 0) {
        return false;
    }

    return true;
}

static __pure bool MulOverFlow(size_t n, size_t size, size_t *r)
{
    if ((size != 0UL) && (n > ((size_t)-1) / size)) {
        return true;
    }

    *r = size * n;
    return false;
}

static bool IsSizeTooLarge(size_t size)
{
    bool rc = false;

    if (size > (size_t)PTRDIFF_MAX) {
        errno = ENOMEM;
        rc = true;
    }

    return rc;
}

#ifdef CONFIG_DEBUG_BUILD

/**
 * @ingroup allocator
 * @brief 为MemMallocF、MemReallocF函数申请到的内存填充调试信息。
 * @par 描述：为MemMallocF、MemReallocF函数申请到的内存填充调试信息。
 * @attention
 * @param size       [IN] 用户可用空间长度
 * @param stackAddr  [IN] 调用者地址
 * @param headAddr   [IN] 被填充信息的起始地址
 * @retval 无
 * @par 依赖：无。
 * @see MemMallocF、MemReallocF
 */
static void MemMallocFSetDFXInfo(size_t size, const uintptr_t *stackAddr, void *headAddr)
{
    int32_t index;
    void *tmpAddr = headAddr;

    *(uint32_t *)tmpAddr = ALLOCATOR_DFX_HEAD_MAGIC;

    tmpAddr = (void *)((uintptr_t)tmpAddr + sizeof(uint32_t));
    *(size_t *)tmpAddr = size;

    tmpAddr = (void *)((uintptr_t)tmpAddr + sizeof(size_t));
    for (index = 0; index < STACK_LEVEL; index++) {
        *(uintptr_t *)tmpAddr = stackAddr[index];
        tmpAddr = (void *)((uintptr_t)tmpAddr + sizeof(uintptr_t));
    }

    *(uint32_t *)tmpAddr = ALLOCATOR_DFX_STACK_END_MAGIC;
    tmpAddr = (void *)((uintptr_t)tmpAddr + sizeof(uint32_t) + size);
    *(uint32_t *)tmpAddr = ALLOCATOR_DFX_TAIL_MAGIC;
}

void *MemMallocF(size_t size, uintptr_t *stackAddr)
{
    size_t tmpSize;
    void *addr;
    void *userAddr;

    tmpSize = size + ALLOCATOR_DFX_SIZE;

    if (tmpSize <= size) {
        errno = ENOMEM;
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    addr = ALLOCATOR_ALLOC(tmpSize);
    if (addr != NULL) {
        userAddr = (void *)((uintptr_t)addr + ALLOCATOR_DFX_HEAD_SIZE);
        MemMallocFSetDFXInfo(tmpSize - ALLOCATOR_DFX_SIZE, stackAddr, addr);
        addr = userAddr;
    }

    return addr;
}

void *MemMalloc(size_t size)
{
    uintptr_t stackBuf[STACK_LEVEL] = {0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead};

#ifdef OS_HONGMENG
    (void)dump_stack(stackBuf, STACK_LEVEL);
#else
    stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif

    return MemMallocF(size, stackBuf);
}

WEAK_ALIAS(MemMalloc, malloc);

size_t MemMallocUsableSize(void *mem)
{
    size_t *sizeAddr;
    uint32_t *offsetFlagAddr;

    if (mem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return 0;
    }

    offsetFlagAddr = (uint32_t *)((uintptr_t)(mem) - sizeof(uint32_t));

    if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_OFFSET_MAGIC) {
        sizeAddr = (size_t *)((uintptr_t)(mem) - (((*offsetFlagAddr) & ~ALLOCATOR_DFX_OFFSET_MAGIC) >> SHIFT_BIT) +
            sizeof(uint32_t));
    } else if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_STACK_END_MAGIC) {
        sizeAddr = (size_t *)((uintptr_t)(mem)-ALLOCATOR_DFX_HEAD_SIZE + sizeof(uint32_t));
    } else {
        errno = EINVAL;
        return 0;
    }

    return *sizeAddr;
}
WEAK_ALIAS(MemMallocUsableSize, malloc_usable_size);

void *MemCallocF(size_t n, size_t size)
{
    void *newMem;
    size_t newSize;
    uintptr_t stackBuf[STACK_LEVEL] = {0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead};

    if (size && n > (size_t)-1 / size) {
        errno = ENOMEM;
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    newSize = size * n;

#ifdef OS_HONGMENG
    (void)dump_stack(stackBuf, STACK_LEVEL);
#else
    stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif

    newMem = MemMallocF(newSize, stackBuf);
    if (newMem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return newMem;
    }

    (void)memset(newMem, 0, newSize);

    return newMem;
}
WEAK_ALIAS(MemCallocF, calloc);

void MemFree(void *mem)
{
    uint32_t *headMagicAddr;
    uint32_t *offsetFlagAddr;
    uint32_t *tailMagicAddr;
    size_t *sizeAddr;
    uint32_t switchInfo;

    if (mem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    switchInfo = MemHMSwitchGet();
    if ((switchInfo & MEM_HM_OPTION_WITH_DAMAGE_CHECK) == MEM_HM_OPTION_WITH_DAMAGE_CHECK) {
        if (MemHMUserMemDamageCheck(mem) != ALLOCATOR_OK) {
            __hmattr_sideff_resolved("no relative side effect");
            return;
        }
    }

    offsetFlagAddr = (uint32_t *)((uintptr_t)(mem) - sizeof(uint32_t));

    if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_OFFSET_MAGIC) {
        headMagicAddr =
            (uint32_t *)((uintptr_t)(mem) - (((*offsetFlagAddr) & ~ALLOCATOR_DFX_OFFSET_MAGIC) >> SHIFT_BIT));
    } else if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_STACK_END_MAGIC) {
        headMagicAddr = (uint32_t *)((uintptr_t)(mem)-ALLOCATOR_DFX_HEAD_SIZE);
    } else {
        errno = EINVAL;
        return;
    }

    sizeAddr = (size_t *)((uintptr_t)headMagicAddr + sizeof(uint32_t));
    tailMagicAddr = (uint32_t *)((uintptr_t)(mem) + (*sizeAddr));

    *headMagicAddr = 0;
    *tailMagicAddr = 0;

    ALLOCATOR_FREE((void *)headMagicAddr);
}
WEAK_ALIAS(MemFree, free);

void *MemReallocF(void *oldMem, size_t newSize)
{
    size_t tmpSize, oldMemSize, length;
    void *tmpAddr;
    void *headAddr;
    void *newMem;
    uint32_t *offsetFlagAddr;
    void *newAddr;
    void *cpyAddr;
    uintptr_t stackBuf[STACK_LEVEL] = {0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead};

    if (oldMem == NULL) {
#ifdef OS_HONGMENG
        (void)dump_stack(stackBuf, STACK_LEVEL);
#else
        stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif
        return MemMallocF(newSize, stackBuf);
    }

    if (newSize == 0) {
        MemFree(oldMem);
        return NULL;
    }

    tmpSize = newSize + ALLOCATOR_DFX_SIZE;
    if (tmpSize <= newSize) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    offsetFlagAddr = (uint32_t *)((uintptr_t)(oldMem) - sizeof(uint32_t));

    if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_STACK_END_MAGIC) {
        size_t *oldSizeAddr = (size_t *)((uintptr_t)(oldMem) - ALLOCATOR_DFX_HEAD_SIZE + sizeof(uint32_t));
        oldMemSize = (*oldSizeAddr) + ALLOCATOR_DFX_SIZE;

        if (oldMemSize >= tmpSize && oldMemSize - tmpSize < ALLOCATOR_DFX_DONT_CARE) {
            tmpAddr = (void *)((uintptr_t)oldMem - sizeof(uint32_t) - STACK_LEVEL * sizeof(uintptr_t));
#ifdef OS_HONGMENG
            (void)dump_stack(stackBuf, STACK_LEVEL);
#else
            stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif
            for (int32_t index = 0; index < STACK_LEVEL; index++) {
                *(uintptr_t *)tmpAddr = stackBuf[index];
                tmpAddr = (void *)((uintptr_t)tmpAddr + sizeof(uintptr_t));
            }

            return oldMem;
        }
    } else if ((*offsetFlagAddr & ALLOCATOR_DFX_CHECK_MASK) == ALLOCATOR_DFX_OFFSET_MAGIC) {
        headAddr = (void *)((uintptr_t)(oldMem) - (((*offsetFlagAddr) & ~ALLOCATOR_DFX_OFFSET_MAGIC) >> SHIFT_BIT));
        oldMemSize = *(size_t *)((uintptr_t)headAddr + sizeof(uint32_t)) + ALLOCATOR_DFX_SIZE;
    } else {
        errno = EINVAL;
        return NULL;
    }

    newMem = ALLOCATOR_ALLOC(tmpSize);
    if (newMem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    newAddr = (void *)((uintptr_t)newMem + ALLOCATOR_DFX_HEAD_SIZE);
#ifdef OS_HONGMENG
    (void)dump_stack(stackBuf, STACK_LEVEL);
#else
    stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif
    MemMallocFSetDFXInfo(tmpSize - ALLOCATOR_DFX_SIZE, stackBuf, newMem);

    length = (tmpSize < oldMemSize) ? tmpSize : oldMemSize;

    cpyAddr = memcpy(newAddr, oldMem, length - ALLOCATOR_DFX_SIZE);
    if (cpyAddr != newAddr) {
        MemFree(newMem);
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    MemFree(oldMem);

    return newAddr;
}

WEAK_ALIAS(MemReallocF, realloc);

void *MemReallocArrayF(void *oldMem, size_t newNumber, size_t newElemSize)
{
    void *ret = NULL;
    size_t size = 0UL;

    if (MulOverFlow(newNumber, newElemSize, &size)) {
        errno = ENOMEM;
    } else {
        ret = MemReallocF(oldMem, size);
    }

    return ret;
}

WEAK_ALIAS(MemReallocArrayF, reallocarray);

/**
 * @ingroup allocator
 * @brief 为MemAlignF函数申请到的内存填充调试信息。
 * @par 描述：为MemAlignF函数申请到的内存填充调试信息。
 * @attention
 * @param size        [IN] 用户可用空间长度
 * @param stackAddr   [IN] 调用者地址
 * @param addr        [IN] 被填充信息的起始地址
 * @param offset      [IN] 偏移长度
 * @retval 无
 * @par 依赖：无。
 * @see MemAlignF
 */
static void MemAlignFSetDFXHeadInfo(size_t size, const uintptr_t *stackAddr, void *addr, uint32_t offset)
{
    int32_t index;

    *(uint32_t *)addr = (offset << SHIFT_BIT) | ALLOCATOR_DFX_HEAD_OFFSET_MAGIC;

    addr = (void *)((uintptr_t)(addr) + sizeof(uint32_t));
    *(size_t *)addr = size;

    addr = (void *)((uintptr_t)addr + sizeof(size_t));
    for (index = 0; index < STACK_LEVEL; index++) {
        *(size_t *)addr = stackAddr[index];
        addr = (void *)((uintptr_t)addr + sizeof(uintptr_t));
    }

    *(uint32_t *)(addr) = ALLOCATOR_DFX_STACK_END_MAGIC;
}

void *MemAlignF(size_t align, size_t size, uintptr_t *stackAddr)
{
    size_t tmpSize;
    void *tmpAddr;
    void *newMem;
    void *userAddr;

    if (IsMemAlignParaRight(align, size) != true) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    // 已做过针对size + tmpAlign的校验
    if (IsSizeTooLarge(size + align)) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }
    if (align <= ALLOCATOR_MIN_ALIGN) {
        align = ALLOCATOR_MIN_ALIGN;
    }

    tmpSize = size + ALLOCATOR_DFX_SIZE;

    newMem = ALLOCATOR_ALLOC(tmpSize + align);
    if (newMem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    userAddr = (void *)(((uintptr_t)newMem + align - 1) & -align);

    if ((uintptr_t)userAddr - (uintptr_t)newMem < ALLOCATOR_DFX_HEAD_SIZE) {
        userAddr = (void *)((uintptr_t)userAddr + align);
    }

    *(uint32_t *)((uintptr_t)userAddr + tmpSize - ALLOCATOR_DFX_SIZE) = ALLOCATOR_DFX_TAIL_MAGIC;

    *(uint32_t *)((uintptr_t)userAddr - sizeof(uint32_t)) =
        (((uintptr_t)userAddr - (uintptr_t)newMem) << SHIFT_BIT) | ALLOCATOR_DFX_OFFSET_MAGIC;

    tmpAddr = newMem;
    MemAlignFSetDFXHeadInfo(tmpSize - ALLOCATOR_DFX_SIZE, stackAddr, tmpAddr, (uintptr_t)userAddr - (uintptr_t)newMem);

    return userAddr;
}

void *MemAlign(size_t align, size_t size)
{
    uintptr_t stackBuf[STACK_LEVEL] = {0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead};

#ifdef OS_HONGMENG
    (void)dump_stack(stackBuf, STACK_LEVEL);
#else
    stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif

    return MemAlignF(align, size, stackBuf);
}

WEAK_ALIAS(MemAlign, memalign);

void *MemVallocF(size_t size)
{
    uintptr_t stackBuf[STACK_LEVEL] = {0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead, 0xdeaddead};

#ifdef OS_HONGMENG
    (void)dump_stack(stackBuf, STACK_LEVEL);
#else
    stackBuf[0] = (uintptr_t)__builtin_return_address(0);
#endif

    return MemAlignF(PAGE_SIZE, size, stackBuf);
}
WEAK_ALIAS(MemVallocF, valloc);

#else

size_t MemMallocUsableSize(void *mem)
{
    size_t size;

    if (mem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return 0UL;
    }

    if (ALLOCATOR_ALLOC_USABLE_SIZE_GET(mem, &size) != ALLOCATOR_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return 0UL;
    }

    return size;
}

/* 正常版本下，为MemMallocUsableSize函数起个别名为malloc_usable_size */
WEAK_ALIAS(MemMallocUsableSize, malloc_usable_size);

void *MemCalloc(size_t n, size_t size)
{
    void *newMem;
    size_t newSize;

    if ((size != 0UL) && (n > ((size_t)-1) / size)) {
        __hmattr_sideff_resolved("no relative side effect");
        errno = ENOMEM;
        return NULL;
    }

    newSize = size * n;

    newMem = VosHimallocAlloc(newSize);
    if (newMem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return newMem;
    }

    (void)memset(newMem, 0, newSize);

    return newMem;
}

/* 正常版本下，为MemCalloc函数起个别名为calloc */
WEAK_ALIAS(MemCalloc, calloc);

void *MemRealloc(void *oldMem, size_t newSize)
{
    size_t oldMemSize;
    void *newMem;
    size_t length;
    void *retAddr;

    if (oldMem == NULL) {
        return VosHimallocAlloc(newSize);
    }

    if (newSize == 0UL) {
        VosHimallocFree(oldMem);
        return NULL;
    }

    if (ALLOCATOR_ALLOC_USABLE_SIZE_GET(oldMem, &oldMemSize) != ALLOCATOR_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }
    if ((newSize <= oldMemSize) && (newSize >= (oldMemSize >> 1U))) {
        /* at most 50% waste of space in this case */
        return oldMem;
    }
    newMem = VosHimallocAlloc(newSize);
    if (newMem == NULL) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    length = (newSize < oldMemSize) ? newSize : oldMemSize;

    retAddr = memcpy(newMem, oldMem, length);
    if (retAddr != newMem) {
        VosHimallocFree(newMem);
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    VosHimallocFree(oldMem);

    return newMem;
}

/* 正常版本下，为MemRealloc函数起个别名为realloc */
WEAK_ALIAS(MemRealloc, realloc);

void *MemReallocArray(void *addr, size_t nmemb, size_t elem_size)
{
    void *ret = NULL;
    size_t size = 0UL;

    if (MulOverFlow(nmemb, elem_size, &size)) {
        errno = ENOMEM;
    } else {
        ret = MemRealloc(addr, size);
    }

    return ret;
}

/* 正常版本下，为MemReallocArray函数起个别名为reallocarray */
WEAK_ALIAS(MemReallocArray, reallocarray);

void *MemAlign(size_t align, size_t size)
{
    size_t tmpAlign = align;

    if (IsMemAlignParaRight(tmpAlign, size) != true) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    if (IsSizeTooLarge(size)) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    if (tmpAlign <= ALLOCATOR_MIN_ALIGN) {
        tmpAlign = ALLOCATOR_MIN_ALIGN;
    }

    return ALLOCATOR_ALLOC_ALIGN(size, tmpAlign);
}

/* 正常版本下，为MemAlign函数起个别名为memalign */
WEAK_ALIAS(MemAlign, memalign);

void *MemValloc(size_t size)
{
    return MemAlign((size_t)PAGE_SIZE, size);
}

/* 正常版本下，为MemValloc函数起个别名为valloc */
WEAK_ALIAS(MemValloc, valloc);

#endif

#ifdef __cplusplus
}
#endif /* __cpluscplus */
