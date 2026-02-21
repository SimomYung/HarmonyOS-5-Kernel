/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK gpio subsystem framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 12 17:18:26 2021
 */
#ifndef	__UDK_GPIO_H__
#define	__UDK_GPIO_H__

#include <udk/lib/dlist.h>
#include <libstrict/strict.h>

#define UDK_GPIO_DIR_INVALID		(-1)
#define UDK_GPIO_INPUT			0
#define UDK_GPIO_OUTPUT			1
#define UDK_GPIO_DIR_MAX		2

#define UDK_GPIO_LEVEL_INVALID		(-1)
#define UDK_GPIO_LOW			0
#define UDK_GPIO_HIGH			1
#define UDK_GPIO_LEVEL_MAX		2

#define UDK_GPIO_BIAS_INVALID		(-1)
#define UDK_GPIO_PULL_NONE		0
#define UDK_GPIO_PULL_UP		1
#define UDK_GPIO_PULL_DOWN		2
#define UDK_GPIO_BIAS_MAX		3

struct udk_gpio_bank;
struct udk_gpio;

struct udk_gpio_bank_ops {
	int (*request)(struct udk_gpio_bank *bank, unsigned int offset);
	int (*free)(struct udk_gpio_bank *bank, unsigned int offset);
	int (*get_direction)(struct udk_gpio_bank *bank, unsigned int offset);
	int (*set_direction)(struct udk_gpio_bank *bank, unsigned int offset,
			     int direction);
	int (*get_level)(struct udk_gpio_bank *bank, unsigned int offset);
	int (*set_level)(struct udk_gpio_bank *bank, unsigned int offset,
			 int level);
	int (*get_bias)(struct udk_gpio_bank *bank, unsigned int offset);
	int (*set_bias)(struct udk_gpio_bank *bank, unsigned int offset,
			int bias);
	int (*get_mux)(struct udk_gpio_bank *bank, unsigned int offset);
	int (*set_mux)(struct udk_gpio_bank *bank, unsigned int offset,
		       int mux);
};

struct udk_gpio_bank {
	const char *name;
	unsigned int pin_count;
	struct udk_gpio_bank_ops *ops;
};

#ifdef CONFIG_UDK_GPIO

int udk_gpio_register(struct udk_gpio_bank *bank);
int udk_gpio_unregister(struct udk_gpio_bank *bank);
struct udk_gpio *udk_gpio_request(unsigned int pin);
int udk_gpio_free(struct udk_gpio *gpio);
int udk_gpio_get_direction(struct udk_gpio *gpio);
int udk_gpio_set_input(struct udk_gpio *gpio);
int udk_gpio_set_output(struct udk_gpio *gpio, int level);
int udk_gpio_get_level(struct udk_gpio *gpio);
int udk_gpio_set_level(struct udk_gpio *gpio, int level);
int udk_gpio_get_bias(struct udk_gpio *gpio);
int udk_gpio_set_bias(struct udk_gpio *gpio, int bias);
int udk_gpio_get_mux(unsigned int pin);
int udk_gpio_set_mux(unsigned int pin, int mux);

#else /* CONFIG_UDK_GPIO */

static inline int udk_gpio_register(struct udk_gpio_bank *bank)
{
	UNUSED(bank);
	return -ENOSYS;
}

static inline int udk_gpio_unregister(struct udk_gpio_bank *bank)
{
	UNUSED(bank);
	return -ENOSYS;
}

static inline struct udk_gpio *udk_gpio_request(unsigned int pin)
{
	UNUSED(pin);
	return NULL;
}

static inline int udk_gpio_free(struct udk_gpio *gpio)
{
	UNUSED(gpio);
	return -ENOSYS;
}

static inline int udk_gpio_get_direction(struct udk_gpio *gpio)
{
	UNUSED(gpio);
	return -ENOSYS;
}

static inline int udk_gpio_set_input(struct udk_gpio *gpio)
{
	UNUSED(gpio);
	return -ENOSYS;
}

static inline int udk_gpio_set_output(struct udk_gpio *gpio, int level)
{
	UNUSED(gpio, level);
	return -ENOSYS;
}

static inline int udk_gpio_get_level(struct udk_gpio *gpio)
{
	UNUSED(gpio);
	return -ENOSYS;
}

static inline int udk_gpio_set_level(struct udk_gpio *gpio, int level)
{
	UNUSED(gpio, level);
	return -ENOSYS;
}

static inline int udk_gpio_get_bias(struct udk_gpio *gpio)
{
	UNUSED(gpio);
	return -ENOSYS;
}

static inline int udk_gpio_set_bias(struct udk_gpio *gpio, int bias)
{
	UNUSED(gpio, bias);
	return -ENOSYS;
}

static inline int udk_gpio_get_mux(unsigned int pin)
{
	UNUSED(pin);
	return -ENOSYS;
}

static inline int udk_gpio_set_mux(unsigned int pin, int mux)
{
	UNUSED(pin, mux);
	return -ENOSYS;
}

#endif /* CONFIG_UDK_GPIO */

#endif /* __UDK_GPIO_H__ */
