/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK I/O routines
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:34:05 2019
 */
#ifndef __UDK_IOLIB_H__
#define __UDK_IOLIB_H__

#include <stdint.h>
#include <wchar.h>
#include <udk/types.h>

void *udk_malloc(size_t size);
void *udk_malloc_ex(size_t size, uint32_t paf);
void udk_free(void *ptr);
int udk_wctomb(char *s, wchar_t wc);

/* little-endian read/write */
void udk_write8(volatile void *addr, uint8_t val);
void udk_write16(volatile void *addr, uint16_t val);
void udk_write32(volatile void *addr, uint32_t val);
void udk_write64(volatile void *addr, uint64_t val);
uint8_t udk_read8(const volatile void *addr);
uint16_t udk_read16(const volatile void *addr);
uint32_t udk_read32(const volatile void *addr);
uint64_t udk_read64(const volatile void *addr);

/* big-endian read/write */
void udk_write_be8(volatile void *addr, uint8_t val);
void udk_write_be16(volatile void *addr, uint16_t val);
void udk_write_be32(volatile void *addr, uint32_t val);
void udk_write_be64(volatile void *addr, uint64_t val);
uint8_t udk_read_be8(const volatile void *addr);
uint16_t udk_read_be16(const volatile void *addr);
uint32_t udk_read_be32(const volatile void *addr);
uint64_t udk_read_be64(const volatile void *addr);

#endif /* __UDK_IOLIB_H__ */
