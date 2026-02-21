/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of io in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <udk/iolib.h>

#include <hmkernel/barrier.h>
#include <hongmeng/types.h>
#include <libhmsrv_sys/hm_mem.h>

void udk_write8(volatile void *addr, uint8_t val)
{
	uint8_t __val = val;

	wmb();
	*(volatile uint8_t *)addr = __val;
}

void udk_write16(volatile void *addr, uint16_t val)
{
	uint16_t __val = htole16(val);

	wmb();
	*(volatile uint16_t *)addr = __val;
}

void udk_write32(volatile void *addr, uint32_t val)
{
	uint32_t __val = htole32(val);

	wmb();
	*(volatile uint32_t *)addr = __val;
}

void udk_write64(volatile void *addr, uint64_t val)
{
	uint64_t __val = htole64(val);

	wmb();
	*(volatile uint64_t *)addr = __val;
}

uint8_t udk_read8(const volatile void *addr)
{
	uint8_t val;

	val = *(volatile uint8_t *)addr;
	rmb();
	return val;
}

uint16_t udk_read16(const volatile void *addr)
{
	uint16_t val;

	val = *(volatile uint16_t *)addr;
	rmb();
	return le16toh(val);
}

uint32_t udk_read32(const volatile void *addr)
{
	uint32_t val;

	val = *(volatile uint32_t *)addr;
	rmb();
	return le32toh(val);
}

uint64_t udk_read64(const volatile void *addr)
{
	uint64_t val;

	val = *(volatile uint64_t *)addr;
	rmb();
	return le64toh(val);
}

/* big-endian read/wirte */
void udk_write_be8(volatile void *addr, uint8_t val)
{
	udk_write8(addr, val);
}

void udk_write_be16(volatile void *addr, uint16_t val)
{
	uint16_t __val = htobe16(val);

	wmb();
	*(volatile uint16_t *)addr = __val;
}

void udk_write_be32(volatile void *addr, uint32_t val)
{
	uint32_t __val = htobe32(val);

	wmb();
	*(volatile uint32_t *)addr = __val;
}

void udk_write_be64(volatile void *addr, uint64_t val)
{
	uint64_t __val = htobe64(val);

	wmb();
	*(volatile uint64_t *)addr = __val;
}

uint8_t udk_read_be8(const volatile void *addr)
{
	return udk_read8(addr);
}

uint16_t udk_read_be16(const volatile void *addr)
{
	uint16_t val;

	val = *(volatile uint16_t *)addr;
	rmb();
	return be16toh(val);
}

uint32_t udk_read_be32(const volatile void *addr)
{
	uint32_t val;

	val = *(volatile uint32_t *)addr;
	rmb();
	return be32toh(val);
}

uint64_t udk_read_be64(const volatile void *addr)
{
	uint64_t val;

	val = *(volatile uint64_t *)addr;
	rmb();
	return be64toh(val);
}
