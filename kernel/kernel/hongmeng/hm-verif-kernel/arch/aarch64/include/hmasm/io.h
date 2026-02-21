/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 7 10:56:55 2018
 */
#ifndef AARCH64_ASM_IO_H
#define AARCH64_ASM_IO_H

#include <hmkernel/bitops/byteorder.h>
#include <hmkernel/compiler.h>
#include <hmasm/barrier.h>
#include <hmasm/types.h>

#define __iormb()		rmb()
#define __iowmb()		wmb()

static inline void
__write8(volatile void __iomem *addr, u8 val)
{
	asm volatile("strb %w0, [%1]" : : "rZ" (val), "r" (addr));
}

static inline u8
__read8(volatile void __iomem *addr)
{
	u8 val;

	asm volatile("ldarb %w0, [%1]" : "=r" (val) : "r" (addr));
	return val;
}

static inline void
__write16(volatile void __iomem *addr, u16 val)
{
	asm volatile("strh %w0, [%1]" : : "rZ" (val), "r"(addr));
}

static inline u16
__read16(volatile void __iomem *addr)
{
	u16 val;

	asm volatile("ldarh %w0, [%1]" : "=r" (val) : "r" (addr));
	return le16_to_cpu(val);
}

static inline void
__write32(volatile void __iomem *addr, u32 val)
{
	u32 __val = cpu_to_le32(val);

	asm volatile("str %w0, [%1]" : : "rZ" (__val), "r" (addr));
}

static inline u32
__read32(volatile void __iomem *addr)
{
	u32 val;

	/* See ARM erratum 832075 */
	asm volatile("ldar %w0, [%1]" : "=r" (val) : "r" (addr));
	return le32_to_cpu(val);
}

static inline void
__write64(volatile void __iomem *addr, u64 val)
{
	u64 __val = cpu_to_le64(val);

	asm volatile("str %x0, [%1]" : : "rZ" (__val), "r" (addr));
}

static inline u64
__read64(volatile void __iomem *addr)
{
	u64 val;

	asm volatile("ldar %x0, [%1]" : "=r" (val) : "r" (addr));
	return le64_to_cpu(val);
}

#define write8(c, v)	do { __iowmb(); __write8((c), (v)); } while (0)
#define read8(c)	({ u8 v = __read8(c); __iormb(); v; })

#define write16(c, v)	do { __iowmb(); __write16((c), (v)); } while (0)
#define read16(c)	({ u16 v = __read16(c); __iormb(); v; })

#define write32(c, v)	do { __iowmb(); __write32((c), (v)); } while (0)
#define read32(c)	({ u32 v = __read32(c); __iormb(); v; })

#define write64(c, v)	do { __iowmb(); __write64((c), (v)); } while (0)
#define read64(c)	({ u64 v = __read64(c); __iormb(); v; })

static inline u64
io_reg_read64(void __iomem *base_addr, unsigned long offset)
{
	u8 *addr = (u8 *)(uptr_t)(base_addr);
	return read64(&addr[offset]);
}

static inline void
io_reg_write64(void __iomem *base_addr, unsigned long offset, u64 v)
{
	u8 *addr = (u8 *)(uptr_t)base_addr;
	write64(&addr[offset], v);
}

#endif
