/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: UDK clock framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 07 20:24:17 2021
 */
#ifndef	__UDK_CLOCK_H__
#define	__UDK_CLOCK_H__

#include <udk/of.h>
#include <udk/errno.h>
#include <udk/types.h>
#include <udk/device.h>
#include <libstrict/strict.h>

#define UDK_CLOCK_OFF		0
#define UDK_CLOCK_ON		1

struct udk_clock;

struct udk_clock_reg {
	unsigned long offset;
	unsigned short shift;
	unsigned short mask;
};

struct udk_clock_gate {
	struct udk_clock_reg reg;
};

struct udk_clock_mux {
	struct udk_clock_reg reg; /* indicates the mux parent index */
	const char **parents;
	unsigned int parent_num;
};

enum udk_clock_div_method {
	UDK_CLOCK_DIV_PARALLEL,
	UDK_CLOCK_DIV_POWER_OF_TWO,
	UDK_CLOCK_DIV_MAX_AT_ZERO,
	UDK_CLOCK_DIV_METHOD_MAX
};

struct udk_clock_div {
	struct udk_clock_reg reg;
	enum udk_clock_div_method method;
};

struct udk_clock_key {
	unsigned int type;
	unsigned int index;
};

struct udk_clock_desc {
	const char *name;
	const char *parent;
	unsigned long rate;
	struct udk_clock_gate *gate;
	struct udk_clock_mux *mux;
	struct udk_clock_div *div;
	struct udk_clock_key *key;
};

struct udk_clock_hw_ops {
	unsigned long	(*reg_read)(unsigned long offset);
	void		(*reg_write)(unsigned long offset, unsigned long val);
	unsigned int	(*get_parent)(const struct udk_clock_desc *desc);
	int		(*set_parent)(const struct udk_clock_desc *desc,
				      unsigned int index);
	unsigned long	(*recalc_rate)(const struct udk_clock_desc *desc,
				       unsigned long parent_rate);
	int		(*set_rate)(const struct udk_clock_desc *desc,
				    unsigned long rate, unsigned long parent_rate);
};

#ifdef CONFIG_UDK_CLOCK

struct udk_clock *udk_clock_register(const struct udk_clock_desc *desc,
				     const struct udk_clock_hw_ops *hw_ops);
int udk_clock_unregister(struct udk_clock *clock);
struct udk_clock *udk_clock_lookup(const char *name);
int udk_clock_enable(struct udk_clock *clock);
int udk_clock_disable(struct udk_clock *clock);
bool udk_clock_is_enabled(struct udk_clock *clock);
struct udk_clock *udk_clock_get_parent(struct udk_clock *clock);
int udk_clock_set_parent(struct udk_clock *clock, struct udk_clock *parent);
unsigned long udk_clock_get_rate(struct udk_clock *clock);
int udk_clock_set_rate(struct udk_clock *clock, unsigned long rate);

int udk_clock_of_register(struct udk_device *device,
			  const struct udk_clock_hw_ops *hw_ops);
int udk_clock_of_unregister(struct udk_device *device);

#else

static inline struct udk_clock *udk_clock_register(const struct udk_clock_desc *desc,
						   const struct udk_clock_hw_ops *hw_ops)
{
	UNUSED(desc, hw_ops);
	return NULL;
}

static inline int udk_clock_unregister(struct udk_clock *clock)
{
	UNUSED(clock);
	return -ENOSYS;
}

static inline struct udk_clock *udk_clock_lookup(const char *name)
{
	UNUSED(name);
	return NULL;
}

static inline int udk_clock_enable(struct udk_clock *clock)
{
	UNUSED(clock);
	return -ENOSYS;
}

static inline int udk_clock_disable(struct udk_clock *clock)
{
	UNUSED(clock);
	return -ENOSYS;
}

static inline bool udk_clock_is_enabled(struct udk_clock *clock)
{
	UNUSED(clock);
	return false;
}

static inline struct udk_clock *udk_clock_get_parent(struct udk_clock *clock)
{
	UNUSED(clock);
	return NULL;
}

static inline int udk_clock_set_parent(struct udk_clock *clock,
				       struct udk_clock *parent)
{
	UNUSED(clock, parent);
	return -ENOSYS;
}

static inline unsigned long udk_clock_get_rate(struct udk_clock *clock)
{
	UNUSED(clock);
	return 0UL;
}

static inline int udk_clock_set_rate(struct udk_clock *clock, unsigned long rate)
{
	UNUSED(clock, rate);
	return -ENOSYS;
}

static inline int udk_clock_of_register(struct udk_device *device,
					const struct udk_clock_hw_ops *hw_ops)
{
	UNUSED(device, hw_ops);
	return -ENOSYS;
}

static inline int udk_clock_of_unregister(struct udk_device *device)
{
	UNUSED(device);
	return -ENOSYS;
}

#endif /* CONFIG_UDK_CLOCK */
#endif /* __UDK_CLOCK_H__ */
