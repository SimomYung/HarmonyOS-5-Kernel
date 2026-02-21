/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Parse the devicetree node about i2c
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 15 15:42:57 CST 2022
 */

#include <udk/i2c.h>
#include <libstrict/strict.h>

#ifdef CONFIG_UDK_I2C

#include <udk/log.h>
#include <udk/errno.h>

static int of_parse_i2c_id(struct udk_dt_node *node, uint32_t *i2c_id)
{
	int err;
	uint32_t id;
	uint32_t phandle;
	struct udk_dt_node *i2c_node = NULL;

	err = udk_of_prop_read_u32(node, "i2c-adapter", &phandle);
	if (err < 0) {
		udk_warn("i2c client read adapter failed: %s\n", strerror(-err));
		return err;
	}
	i2c_node = udk_of_find_node_by_phandle(node, phandle);
	if (i2c_node == NULL) {
		udk_warn("could not find i2c node\n");
		return -ENOENT;
	}
	err = udk_of_prop_read_u32(i2c_node, "i2c-id", &id);
	if (err < 0) {
		udk_warn("could not find i2c :%s\n", strerror(-err));
		return err;
	}
	*i2c_id = id;

	return 0;
}

struct udk_i2c_client *udk_i2c_of_add_client(struct udk_dt_node *node)
{
	int err;
	uint32_t i2c_id;
	unsigned int address;

	if (node == NULL) {
		udk_warn("invalid node\n");
		return NULL;
	}
	err = of_parse_i2c_id(node, &i2c_id);
	if (err < 0) {
		return NULL;
	}
	err = udk_of_prop_read_u32(node, "i2c-client", &address);
	if (err < 0) {
		udk_warn("read i2c client addr failed: %s\n", strerror(-err));
		return NULL;
	}

	return udk_i2c_add_client(i2c_id, address);
}

#else /* CONFIG_UDK_I2C */

struct udk_i2c_client *udk_i2c_of_add_client(struct udk_dt_node *node)
{
	UNUSED(node);

	return NULL;
}

#endif /* CONFIG_UDK_I2C */
