/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Parse the devicetree node about clock
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 05 19:34:23 2021
 */

#include <udk/of.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/clock.h>
#include <udk/iolib.h>
#include <udk/device.h>
#include <udk/bitops.h>
#include <udk/securec.h>
#include <udk/lib/dlist.h>

#include <lib/utils.h>
#include <sys/types.h>

struct clock_of_desc {
	struct udk_clock_desc *desc;
	struct dlist_node node;
};

struct clock_of_device {
	const struct udk_device *device;
	struct dlist_node node;
	struct dlist_node desc_list;
};

static DLIST_HEAD(clock_dev_list);

static void set_clock_reg(struct udk_clock_reg *reg, const uint32_t *value,
			  size_t length)
{
	size_t index = 0U;

	if (index < length) {
		reg->offset = (unsigned long)value[index++];
	}
	if (index < length) {
		reg->shift = (unsigned short)value[index++];
	}
	if (index < length) {
		reg->mask = (unsigned short)UDK_MAKE_MASK(value[index]);
	}
}

static struct udk_clock_gate *clock_gate_parse(const struct udk_dt_node *node)
{
	int err;
	uint32_t values[2] = {0};
	struct udk_clock_gate *gate = NULL;

	err = udk_of_prop_read_u32_array(node, "gate-reg", values, ARRAY_SIZE(values));
	if (err < 0) {
		return NULL;
	}
	gate = (struct udk_clock_gate *)udk_malloc(sizeof(*gate));
	if (gate == NULL) {
		return NULL;
	}
	mem_zero_s(*gate);
	set_clock_reg(&gate->reg, values, ARRAY_SIZE(values));

	return gate;
}

static int clock_mux_set_parents(struct udk_clock_mux *mux, const char *value,
				 size_t length)
{
	const char **parents = NULL;
	const char *base = value;
	const char *pos = base;
	unsigned int parent_num = 0UL;
	unsigned int i;

	while ((size_t)(pos - base) < length) {
		pos = strchr(pos, '\0');
		if (pos == NULL) {
			return -EINVAL;
		}
		pos = pos + 1;
		parent_num++;
	}
	parents = udk_malloc(parent_num * sizeof(const char *));
	if (parents == NULL) {
		return -ENOMEM;
	}
	pos = base;
	for (i = 0; i < parent_num; i++) {
		parents[i] = pos;
		pos = strchr(pos, '\0') + 1;
	}
	mux->parents = parents;
	mux->parent_num = parent_num;

	return 0;
}

static int clock_mux_parents_parse(struct udk_clock_mux *mux,
				   const struct udk_dt_prop *prop)
{
	void *val = NULL;
	size_t len = 0U;
	int err;

	val = udk_of_get_prop_value(prop);
	if (val == NULL) {
		return -ENOENT;
	}
	len = udk_of_get_prop_length(prop);
	if (len == 0U) {
		return -EINVAL;
	}
	err = clock_mux_set_parents(mux, (const char *)val, len);
	if (err < 0) {
		return err;
	}

	return 0;
}

static struct udk_clock_mux *clock_mux_parse(const struct udk_dt_node *node)
{
	int err;
	uint32_t values[3] = {0};
	struct udk_dt_prop *prop = NULL;
	struct udk_clock_mux *mux = NULL;

	prop = udk_of_find_property(node, "mux-parents");
	if (prop == NULL) {
		return NULL;
	}
	mem_zero_a(values);
	err = udk_of_prop_read_u32_array(node, "mux-reg",
					 values, ARRAY_SIZE(values));
	if (err < 0) {
		return NULL;
	}
	mux = (struct udk_clock_mux *)udk_malloc(sizeof(struct udk_clock_mux));
	if (mux == NULL) {
		return NULL;
	}
	mem_zero_s(*mux);
	err = clock_mux_parents_parse(mux, prop);
	if (err < 0) {
		udk_free(mux);
		return NULL;
	}
	set_clock_reg(&mux->reg, values, ARRAY_SIZE(values));

	return mux;
}

static const char * const div_method_str[UDK_CLOCK_DIV_METHOD_MAX] = {
	"parallel",
	"power_of_two",
	"max_at_zero",
};

static inline bool is_div_method_invalid(enum udk_clock_div_method method)
{
	return method >= UDK_CLOCK_DIV_METHOD_MAX;
}

static enum udk_clock_div_method clock_div_method_parse(const struct udk_dt_node *node)
{
	int err;
	int idx = 0;
	const char *method_str = NULL;

	err = udk_of_prop_read_string(node, "div-method", &method_str);
	if (err < 0) {
		return UDK_CLOCK_DIV_METHOD_MAX;
	}
	for (idx = 0; idx < UDK_CLOCK_DIV_METHOD_MAX; idx++) {
		err = strcmp(method_str, div_method_str[idx]);
		if (err == 0) {
			return (enum udk_clock_div_method)idx;
		}
	}
	udk_warn("invalid div_method: %s\n", method_str);

	return UDK_CLOCK_DIV_METHOD_MAX;
}

static struct udk_clock_div *clock_div_parse(const struct udk_dt_node *node)
{
	int err;
	uint32_t values[3];
	enum udk_clock_div_method method = UDK_CLOCK_DIV_METHOD_MAX;
	struct udk_clock_div *div = NULL;

	method = clock_div_method_parse(node);
	if (is_div_method_invalid(method)) {
		return NULL;
	}
	mem_zero_a(values);
	err = udk_of_prop_read_u32_array(node, "div-reg",
					 values, ARRAY_SIZE(values));
	if (err < 0) {
		return NULL;
	}
	div = (struct udk_clock_div *)udk_malloc(sizeof(struct udk_clock_div));
	if (div == NULL) {
		return NULL;
	}
	mem_zero_s(*div);
	div->method = method;
	set_clock_reg(&div->reg, values, ARRAY_SIZE(values));

	return div;
}

static struct udk_clock_key *clock_key_parse(const struct udk_dt_node *node)
{
	int err;
	uint32_t values[2];
	struct udk_clock_key *key = NULL;

	mem_zero_a(values);
	err = udk_of_prop_read_u32_array(node, "clock-key",
					 values, ARRAY_SIZE(values));
	if (err < 0) {
		return NULL;
	}
	key = (struct udk_clock_key *)udk_malloc(sizeof(*key));
	if (key == NULL) {
		return NULL;
	}
	mem_zero_s(*key);
	key->type = values[0];
	key->index = values[1];

	return key;
}

static const char *clock_parent_parse(const struct udk_dt_node *node)
{
	int err;
	const char *value = NULL;

	err = udk_of_prop_read_string(node, "clock-parent", &value);
	if (err < 0) {
		/* if no clock-parent node, means clock is root or mux */
		return NULL;
	}

	return value;
}

static unsigned long clock_rate_parse(const struct udk_dt_node *node)
{
	int err;
	uint32_t rate;

	err = udk_of_prop_read_u32(node, "clock-rate", &rate);
	if (err < 0) {
		/* if no clock-rate node, means need to calculate */
		return 0UL;
	}

	return (unsigned long)rate;
}

static struct udk_clock_desc *clock_of_desc_create(const struct udk_dt_node *devnode)
{
	struct udk_clock_desc *desc = NULL;

	desc = udk_malloc(sizeof(struct udk_clock_desc));
	if (desc == NULL) {
		udk_error("no memory for clock desc\n");
		return NULL;
	}
	mem_zero_s(*desc);
	desc->name = udk_of_get_node_name(devnode);
	desc->gate = clock_gate_parse(devnode);
	desc->parent = clock_parent_parse(devnode);
	desc->rate = clock_rate_parse(devnode);
	desc->mux = clock_mux_parse(devnode);
	desc->div = clock_div_parse(devnode);
	desc->key = clock_key_parse(devnode);

	return desc;
}

static void clock_of_desc_destroy(struct udk_clock_desc *desc)
{
	if (desc->gate != NULL) {
		udk_free(desc->gate);
	}
	if (desc->mux != NULL) {
		if (desc->mux->parents != NULL) {
			udk_free(desc->mux->parents);
		}
		udk_free(desc->mux);
	}
	if (desc->div != NULL) {
		udk_free(desc->div);
	}
	if (desc->key != NULL) {
		udk_free(desc->key);
	}
	udk_free(desc);
}

static struct clock_of_device *clock_of_device_create(const struct udk_device *device)
{
	struct clock_of_device *clk_dev = NULL;

	clk_dev = (struct clock_of_device *)udk_malloc(sizeof(*clk_dev));
	if (clk_dev == NULL) {
		return NULL;
	}
	mem_zero_s(*clk_dev);
	clk_dev->device = device;
	dlist_init(&clk_dev->desc_list);
	dlist_insert(&clock_dev_list, &clk_dev->node);

	return clk_dev;
}

static int clock_of_device_add_desc(struct clock_of_device *clk_dev,
				    struct udk_clock_desc *desc)
{
	struct clock_of_desc *node = NULL;

	node = (struct clock_of_desc *)udk_malloc(sizeof(*node));
	if (node == NULL) {
		return -ENOMEM;
	}
	node->desc = desc;
	dlist_insert(&clk_dev->desc_list, &node->node);

	return 0;
}

static void clock_of_device_destroy(struct clock_of_device *clk_dev)
{
	struct clock_of_desc *pos = NULL;
	struct clock_of_desc *n = NULL;

	dlist_for_each_entry_safe(pos, n, &clk_dev->desc_list,
				  struct clock_of_desc, node) {
		clock_of_desc_destroy(pos->desc);
		dlist_delete(&pos->node);
		udk_free(pos);
	}
	dlist_delete(&clk_dev->node);
	udk_free(clk_dev);
}

int udk_clock_of_register(struct udk_device *device,
			  const struct udk_clock_hw_ops *hw_ops)
{
	struct udk_dt_node *clocks = NULL;
	struct udk_dt_node *child = NULL;
	struct udk_clock_desc *desc = NULL;
	struct udk_clock *clock = NULL;
	struct clock_of_device *clk_dev = NULL;
	int err = 0;

	if (device == NULL || hw_ops == NULL) {
		return -EINVAL;
	}
	clocks = udk_of_find_node_by_name(device->dt_node, "clocks");
	if (clocks == NULL) {
		return -ENOENT;
	}
	clk_dev = clock_of_device_create(device);
	if (clk_dev == NULL) {
		return -ENOMEM;
	}
	for_each_child_dt_node(clocks, child) {
		desc = clock_of_desc_create(child);
		if (desc == NULL) {
			udk_error("parse desc from dt_node failed\n");
			err = -EINVAL;
			break;
		}
		clock = udk_clock_register(desc, hw_ops);
		if (clock == NULL) {
			udk_error("register clock '%s' failed\n", desc->name);
			clock_of_desc_destroy(desc);
			err = -EINVAL;
			break;
		}
		err = clock_of_device_add_desc(clk_dev, desc);
		if (err < 0) {
			udk_error("add clock description to device failed, err: %d\n", err);
			(void)udk_clock_unregister(clock);
			clock_of_desc_destroy(desc);
			break;
		}
	}

	return err;
}

static struct clock_of_device *clock_of_device_lookup(const struct udk_device *device)
{
	struct clock_of_device *pos = NULL;

	dlist_for_each_entry(pos, &clock_dev_list,
			     struct clock_of_device, node) {
		if (pos->device == device) {
			return pos;
		}
	}

	return NULL;
}

int udk_clock_of_unregister(struct udk_device *device)
{
	struct clock_of_device *clk_dev = NULL;
	struct clock_of_desc *pos = NULL;
	struct clock_of_desc *n = NULL;
	struct udk_clock *clock = NULL;
	int err = 0;

	if (device == NULL) {
		return -EINVAL;
	}
	clk_dev = clock_of_device_lookup(device);
	if (clk_dev == NULL) {
		return -ENOENT;
	}
	dlist_for_each_entry_safe(pos, n, &clk_dev->desc_list,
				  struct clock_of_desc, node) {
		if (pos->desc == NULL) {
			continue;
		}
		clock = udk_clock_lookup(pos->desc->name);
		if (clock != NULL) {
			err = udk_clock_unregister(clock);
			if (err < 0) {
				udk_error("unregister clock '%s' failed, err: %d\n",
					  pos->desc->name, err);
				break;
			}
		}
		clock_of_desc_destroy(pos->desc);
		dlist_delete(&pos->node);
		udk_free(pos);
	}
	if (err == 0) {
		clock_of_device_destroy(clk_dev);
	}

	return err;
}
