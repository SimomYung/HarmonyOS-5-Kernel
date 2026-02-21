// SPDX-License-Identifier: GPL-2.0-only
#include <linux/random.h>
#include <liblinux/pal.h>

#include <liblinux/kapi.h>

void get_random_bytes(void *buf, int nbytes)
{
	prandom_bytes(buf, nbytes);
}
EXPORT_SYMBOL(get_random_bytes);

u32 get_random_u32(void)
{
	return prandom_u32();
}
EXPORT_SYMBOL(get_random_u32);

u64 get_random_u64(void)
{
	uint64_t value;
	prandom_bytes(&value, sizeof(value));
	return value;
}
EXPORT_SYMBOL(get_random_u64);

int __must_check get_random_bytes_arch(void *buf, int nbytes)
{
	prandom_bytes(buf, nbytes);
	return nbytes;
}
EXPORT_SYMBOL(get_random_bytes_arch);

LIBLINUX_KAPI_DIFF
void add_device_randomness(const void *buf, unsigned int size)
{
	(void)buf;
	(void)size;
}
EXPORT_SYMBOL(add_device_randomness);

LIBLINUX_KAPI_DIFF
void add_interrupt_randomness(int irq, int irq_flags)
{
	(void)irq;
	(void)irq_flags;
}
EXPORT_SYMBOL_GPL(add_interrupt_randomness);

LIBLINUX_KAPI_DIFF
void add_bootloader_randomness(const void *buf, unsigned int size)
{
	(void)buf;
	(void)size;
}
EXPORT_SYMBOL_GPL(add_bootloader_randomness);
