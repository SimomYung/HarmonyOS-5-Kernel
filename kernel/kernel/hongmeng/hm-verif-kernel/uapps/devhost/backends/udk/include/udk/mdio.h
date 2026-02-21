/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK Management Data Input/Output bus framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 15 15:01:05 2021
 */
#ifndef	__UDK_MDIO_H__
#define	__UDK_MDIO_H__

#include <udk/errno.h>
#include <libstrict/strict.h>

struct udk_mdio;
struct udk_mdio_desc;

struct udk_mdio_ops {
	int (*reset)(struct udk_mdio_desc *desc);
	int (*read)(struct udk_mdio_desc *desc, unsigned int id,
		    unsigned int reg);
	int (*write)(struct udk_mdio_desc *desc, unsigned int id,
		     unsigned int reg, uint16_t val);
};

struct udk_mdio_desc {
	const char *name;
	struct udk_mdio_ops ops;
	void *priv;
};

#ifdef CONFIG_UDK_MDIO

int udk_mdio_register(struct udk_mdio_desc *desc);
int udk_mdio_unregister(struct udk_mdio_desc *desc);

struct udk_mdio *udk_mdio_get(const char *name);
int udk_mdio_put(struct udk_mdio *mdio);
int udk_mdio_reset(struct udk_mdio *mdio);
int udk_mdio_read(struct udk_mdio *mdio, unsigned int id, unsigned int reg);
int udk_mdio_write(struct udk_mdio *mdio, unsigned int id,
		   unsigned int reg, uint16_t val);

#else

static inline int udk_mdio_register(struct udk_mdio_desc *desc)
{
	UNUSED(desc);

	return -ENOSYS;
}

static inline int udk_mdio_unregister(struct udk_mdio_desc *desc)
{
	UNUSED(desc);

	return -ENOSYS;
}

static inline struct udk_mdio *udk_mdio_get(const char *name)
{
	UNUSED(name);

	return NULL;
}

static inline int udk_mdio_put(struct udk_mdio *mdio)
{
	UNUSED(mdio);

	return 0;
}

static inline int udk_mdio_reset(struct udk_mdio *mdio)
{
	UNUSED(mdio);

	return -ENOSYS;
}

static inline int udk_mdio_read(struct udk_mdio *mdio,
				unsigned int id, unsigned int reg)
{
	UNUSED(mdio, id, reg);

	return -ENOSYS;
}

static inline int udk_mdio_write(struct udk_mdio *mdio, unsigned int id,
				 unsigned int reg, uint16_t val)
{
	UNUSED(mdio, id, reg, val);

	return -ENOSYS;
}

#endif

#endif
