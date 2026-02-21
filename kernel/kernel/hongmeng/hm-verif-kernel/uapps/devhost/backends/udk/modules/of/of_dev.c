/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Special devices zero & null
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */

#include <libstrict/strict.h>

#include <udk/char.h>
#include <udk/device.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/log.h>
#include <udk/securec.h>
#include <udk/of.h>

#include "of_dev.h"
#include "of_fdt.h"

/*
 * collect regs and store in udkdev
 * if anything goes wrong, return err and don't need to release resources
 * all resources will be released in udk_device_destroy.
 */
static int collect_regs(const struct udk_dt_node *devnode,
			struct udk_device *udkdev)
{
	uint64_t reg_addr = 0;
	uint64_t reg_size = 0;
	unsigned num_regs = 0;
	const char **reg_names = NULL;
	size_t names_size = 0;
	unsigned i;
	int err;

	/* Getting <reg> information from dts */
	err = udk_of_reg_count(devnode, &num_regs);
	if (err == 0 && num_regs > 0) {
		names_size = (size_t)sizeof(char *) * num_regs;
		reg_names = (const char **)udk_malloc(names_size);
		if (reg_names == NULL) {
			return -ENOMEM;
		}
		mem_zero_b(reg_names, names_size);
		err = udk_of_get_reg_names(devnode, reg_names, num_regs);
		if (err < 0) {
			/* reg name is not necessary, so ignore the err */
			udk_debug("get reg name failed: %s\n", strerror(-err));
		}
		for (i = 0; i < num_regs; i++) {
			err = udk_of_get_reg_value(devnode, i,
						   &reg_addr, &reg_size);
			if (err < 0) {
				udk_warn("Failed to get for reg, reg=%u\n", i);
				break;
			}
			/* Truncate to 32-bit address on 32-bit platforms */
			err = udk_device_add_iomem_with_name(udkdev, (unsigned long long)reg_addr,
							     (size_t)reg_size, 0x0, reg_names[i]);
			if (err < 0) {
				udk_warn("Add iomem to dev failed: name=%s,index=%u\n",
					 udkdev->name, i);
				break;
			}
		}
		udk_free(reg_names);
	}

	return err;
}

static int collect_resources(struct udk_device *udkdev,
			     struct udk_dt_node *devnode)
{
	struct udk_dt_prop *lpp = NULL;
	int err;

	err = collect_regs(devnode, udkdev);
	if (err != 0) {
		udk_error("devnode get regs num failed, name=%s\n", devnode->full_name);
		return err;
	}

	/* Adding other properties for udk device */
	for_each_property_of_dt_node(devnode, lpp) {
		err = udk_device_add_property(udkdev, lpp->name,
					      lpp->length, lpp->value);
		if (err < 0) {
			/*
			 * No immediate resource release is required.
			 * udk_device_destroy will do it.
			 */
			udk_error("Failed to add property, name=%s\n", lpp->name);
			return err;
		}
	}

	udkdev->dt_node = devnode;
	devnode->dev = udkdev;

	return err;
}

static const char *basename(const struct udk_dt_node *devnode)
{
	const char *fullname = devnode->full_name;
	const char *p = NULL;

	if (fullname != NULL) {
		p = strrchr(fullname, '/');
	}

	if (p == NULL) {
		return "<null>";
	} else {
		return p + 1;
	}
}

static int make_udkdev_name(struct udk_dt_node *devnode,
			    char *buf,
			    size_t maxlen)
{
	uint64_t reg_addr = 0;
	unsigned int num_regs = 0;
	int err;

	if ((maxlen == 0) || (buf == NULL)) {
		return -EINVAL;
	}

	err = udk_of_reg_count(devnode, &num_regs);
	if (err != 0) {
		udk_error("Invalid udk_dt_node\n");
		return -EINVAL;
	}

	if (num_regs > 0) {
		err = udk_of_get_reg_value(devnode, 0,
					   &reg_addr, NULL);
		if (err != 0) {
			return err;
		}

		err = snprintf_s(buf, maxlen, maxlen - 1, "%"PRIx64".%s",
				 reg_addr, devnode->name);
		if (err < 0) {
			return err;
		}
	} else if (devnode->parent != NULL) {
		err = snprintf_s(buf, maxlen, maxlen - 1, "%s:%s",
				 basename(devnode->parent),
				 devnode->name);
		if (err < 0) {
			return err;
		}
	} else {
		err = snprintf_s(buf, maxlen, maxlen - 1,
				 "%s", devnode->full_name);
		if (err < 0) {
			return err;
		}
	}

	return err;
}

static void udk_of_device_configure(struct udk_device *udkdev)
{
	property_t *prop = NULL;

	if (udk_device_get_prop(udkdev, "no-probe") == NULL) {
		prop = udk_device_get_prop(udkdev, "compatible");
		if (prop != NULL) {
			udk_device_set_probeable(udkdev, prop->bytes,
						 prop->nbytes);
		}
	}

	prop = udk_device_get_prop(udkdev, "isolate");
	if (prop != NULL) {
		udk_device_set_isolate(udkdev, prop->bytes);
	}

	if (udk_device_get_prop(udkdev, "no-autoload") == NULL) {
		udk_device_set_autoload(udkdev);
	}
}

int udk_of_device_instantiate(struct udk_dt_node *devnode,
			      struct udk_device *parent_udkdev)
{
	struct udk_device *udkdev = NULL;
	int err;
	char devname[DEVICE_NAME_LEN];

	mem_zero_a(devname);

	/* Step 1: create a native device */
	err = make_udkdev_name(devnode, devname, DEVICE_NAME_LEN);
	if (err < 0) {
		udk_error("Make udkdev name failed\n");
		return err;
	}
	udkdev = udk_device_alloc(devname);
	if (udkdev == NULL) {
		return -ENOMEM;
	}
	udkdev->parent = parent_udkdev;

	/* Step 2: populate properties and resources */
	err = collect_resources(udkdev, devnode);
	if (err < 0) {
		udk_error("Failed to collect resources %s\n",
			  devnode->full_name);
		udk_device_free(udkdev);
		return -EINVAL;
	}

	udk_of_device_configure(udkdev);

	/* Step 3: register the device to devhost */
	err = udk_device_add(udkdev);
	if (err != 0) {
		udk_error("Failed to register native device, name=%s\n",
			  devnode->full_name);
		udk_device_free(udkdev);
		return -EINVAL;
	}

	return err;
}

/*
 * udk_of_device_create is only used in proxy mode.
 * In this scenario, dtb is transfered by other devhost,
 * so free dt node if failed to create device.
 * Destruction of proxy devices is not currently supported,
 * so releasing dt node is not considered for the time.
 */
int udk_of_device_create(const void *dtb)
{
	struct udk_dt_node *root = NULL;
	int ret = 0;

	root = udk_of_unflatten_tree(dtb);
	if ((root == NULL) || (root->child == NULL)) {
		udk_error("resolve dts failed\n");
		return -ENOENT;
	}
	/* Only one device can be created at a time */
	ret = udk_of_device_instantiate(root->child, NULL);
	if (ret < 0) {
		udk_error("failed to populate devices from DTS, name=%s\n", root->child->name);
		udk_of_free_tree(root);
	}

	return ret;
}
