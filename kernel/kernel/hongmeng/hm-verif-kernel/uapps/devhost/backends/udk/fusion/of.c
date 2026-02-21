/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Framework of UDK OF
 * Author: Huawei OS Kernel Lab
 * Create: Thur Feb 29 15:50:54 2024
 */

#include <stdarg.h>

#include <udk/of.h>
#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/fusion.h>
#include <udk/timer.h>
#include <udk/device.h>
#include <udk/module.h>
#include <udk/lib/dlist.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_timer.h>

#include <internal/dtb.h>
#include "device.h"
#include "of.h"
#include "of/of_dev.h"
#include "of/of_fdt.h"

/* for use sd_proxy_dev_fetch to check sdproxy is ready */
#include "sd/include/api.h"

#ifdef CONFIG_UDK_FUSION_OF

#define FWTYPE_LEN    8
/* no need to protect, we ensure it will no concurrency scenario */
static DLIST_HEAD(dtnode_name_list);
struct dtnode_name {
	char *name;
	struct dlist_node node;
};

static DLIST_HEAD(dtnode_rm_list);
struct dtnode_rm {
	struct udk_dt_node *dtnode;
	struct dlist_node node;
};

static unsigned int of_node_num = 0;

static int of_dtb_read(void **dtb)
{
	int ret;
	void *dtb_data = NULL;
	unsigned long fwblob_size;
	const char *fwblob_path = "/firmware/blob";

	/* query firmware blob size */
	ret = hm_sysctrl_read_dtb(fwblob_path, strlen(fwblob_path) + 1U, NULL, 0);
	if (ret < 0) {
		udk_error("query firmware blob size failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
	fwblob_size = (unsigned int)ret;

	dtb_data = udk_malloc((size_t)fwblob_size);
	if (dtb_data == NULL) {
		return E_HM_NOMEM;
	}

	ret = hm_sysctrl_read_dtb(fwblob_path, strlen(fwblob_path) + 1U,
				  dtb_data, fwblob_size);
	if (ret < 0) {
		udk_error("load firmware blob failed, err=%s\n", hmstrerror(ret));
		udk_free(dtb_data);
		return ret;
	}

	*dtb = dtb_data;
	return E_HM_OK;
}

/* load dtb in scenario that exist ldk dtb, no support native udk */
static int of_dtb_load(void **dtb)
{
	int ret;
	const char *fwtype_path = "/firmware/type";
	char fwtype_string[FWTYPE_LEN] = { 0 };

	/* query firmware type */
	ret = hm_sysctrl_read_dtb(fwtype_path, strlen(fwtype_path),
				  fwtype_string, sizeof(fwtype_string) - 1U);
	if (ret < 0) {
		udk_error("no firmware, unsupported native udk dtb load\n");
		return ret;
	}
	if (strncmp(fwtype_string, "fdt", sizeof(fwtype_string)) != 0) {
		udk_error("unsupported firmware type \"%s\"\n", fwtype_string);
		return E_HM_INVAL;
	}
	return of_dtb_read(dtb);
}

static int of_device_instantiate(struct udk_dt_node *root, struct udk_device *root_device)
{
	int ret;
	char *dev_compat = NULL;
	struct udk_dt_node *devnode = NULL;
	struct udk_dt_prop *prop = NULL;
	struct udk_driver *drv = NULL;

	for_each_of_hmnode(root, devnode) {
		prop = udk_of_find_property(devnode, "compatible");
		if (prop == NULL) {
			continue;
		}
		dev_compat = (char *)prop->value;
		drv = NULL;
		udk_info("dev_compat = %s\n", dev_compat);
		ret = udk_fusion_driver_acquire(dev_compat, &drv);
		if (ret < 0) {
			continue;
		}
		udk_info("instantiate device compat %s\n", dev_compat);

		/* increment device probe count */
		udk_device_probe_inc();

		/* instantiate the device */
		ret = udk_of_device_instantiate(devnode, root_device);
		if (ret < 0) {
			udk_error("Failed to populate devices from DTS\n");

			/* decrement device probe count on failure */
			udk_device_probe_dec();

			return posix2hmerrno(-ret);
		}
	}
	udk_info("of_device_instantiate finish\n");
	return E_HM_OK;
}

static bool of_node_found(const char *name)
{
	struct dtnode_name *dtn = NULL;

	dlist_for_each_entry(dtn, &dtnode_name_list, struct dtnode_name, node) {
		if (strcmp(dtn->name, name) == 0) {
			return true;
		}
	}

	return false;
}

static void foreach_child_node_execute(struct udk_dt_node *root, void (*func)(struct udk_dt_node *))
{
	struct udk_dt_node *curr = NULL;

	for_each_of_all_child(root, curr) {
		func(curr);
	}
}

#define OF_NODE_KEEP_MARK    1
static void mark_node(struct udk_dt_node *node)
{
	node->dev = (void *)(uintptr_t)OF_NODE_KEEP_MARK;
}

static void delete_node(struct udk_dt_node *node)
{
	struct dtnode_rm *dtnode_rm = udk_malloc(sizeof(struct dtnode_rm));

	BUG_ON(dtnode_rm == NULL);
	mem_zero_s(*dtnode_rm);
	dtnode_rm->dtnode = node;
	dlist_insert_tail(&dtnode_rm_list, &dtnode_rm->node);
	of_node_num--;
}

static bool of_node_should_mark(struct udk_dt_node *devnode)
{
	struct udk_dt_prop *prop = udk_of_find_property(devnode, "udk-status");

	/* this node has been marked udk disabled */
	if ((prop != NULL) && (strcmp((const char *)udk_of_get_prop_value(prop), "disabled") == 0)) {
		return false;
	}
	/* legacy method, drivers obviously set depends nodes */
	if (of_node_found(devnode->name)) {
		return true;
	}
	/* drivers no set, and dts set enabled */
	if ((prop != NULL) && (strcmp((const char *)udk_of_get_prop_value(prop), "enabled") == 0)) {
		return true;
	}
	/* drivers no set, dts no set, default should not be kept */
	return false;
}

static void of_node_mark(struct udk_dt_node *devnode)
{
	struct udk_dt_node *curr = devnode;

	of_node_num++;
	if (!of_node_should_mark(devnode)) {
		return;
	}
	/* we tmp use dt_node->dev to mark this node should be keep, it will be cleared soon */
	do {
		if (curr->dev != NULL) {
			break;
		}
		mark_node(curr);
		/* its parent should be keep too */
		curr = curr->parent;
	} while (curr != NULL);
	/* its child node should keep too */
	foreach_child_node_execute(devnode, mark_node);
}

static void of_node_free(struct udk_dt_node *devnode)
{
	struct udk_dt_node *curr = NULL;
	struct udk_dt_node *parent = NULL;

	if (devnode->dev != NULL) {
		return;
	}

	foreach_child_node_execute(devnode, delete_node);
	parent = devnode->parent;
	if (parent != NULL) {
		/* this node is parent's first child, set sibling as parent's first child */
		if (parent->child == devnode) {
			parent->child = devnode->sibling;
		} else {
			/* find this node's prev sibling and next sibling, reconstruct its relationship */
			curr = parent->child;
			while (curr->sibling != devnode) {
				curr = curr->sibling;
			}
			curr->sibling = devnode->sibling;
		}
	}
	delete_node(devnode);
}

static struct udk_dt_node *of_node_free_next(struct udk_dt_node *full_root)
{
	struct udk_dt_node *curr = NULL;

	for_each_of_hmnode(full_root, curr) {
		if (curr->dev == NULL) {
			return curr;
		}
	}
	return NULL;
}

static void do_really_free_nodes(void)
{
	struct dtnode_rm *curr = NULL;
	struct dtnode_rm *next = NULL;

	dlist_for_each_entry_safe(curr, next, &dtnode_rm_list, struct dtnode_rm, node) {
		dlist_delete(&curr->node);
		udk_of_delete_dt_node(curr->dtnode);
		udk_free(curr);
	}
}

static void of_device_tree_trim(struct udk_dt_node *full_root)
{
	unsigned int of_nodes_keep = 0;
	struct udk_dt_node *devnode = NULL;

	foreach_child_node_execute(full_root, of_node_mark);
	udk_info("of nodes totally = %u\n", of_node_num);
	/* free unused nodes */
	do {
		devnode = of_node_free_next(full_root);
		if (devnode != NULL) {
			of_node_free(devnode);
		}
	} while (devnode != NULL);

	/* do really free nodes */
	do_really_free_nodes();

	udk_info("of nodes remaining = %u\n", of_node_num);
	/* reamaining nodes should be keep, we clear tmp mark */
	for_each_of_hmnode(full_root, devnode) {
		of_nodes_keep++;
		devnode->dev = NULL;
	}
	if (of_node_num != of_nodes_keep) {
		udk_warn("of_node_num = %u != of_nodes_keep = %u\n", of_node_num, of_nodes_keep);
	}
}

static int of_create(const char *name)
{
	struct udk_device *udkdev = NULL;
	void *dtb = NULL;
	int ret;

	udkdev = udk_device_alloc(name);
	if (udkdev == NULL) {
		udk_error("Failed to allocate root device\n");
		return E_HM_NOMEM;
	}
	ret = udk_device_add(udkdev);
	if (ret != 0) {
		udk_error("devhost create device failed\n");
		udk_device_free(udkdev);
		return posix2hmerrno(-ret);
	}

	/* load dtb will malloc dtb_data buffer, should be freed when finish */
	ret = of_dtb_load(&dtb);
	if (ret < 0) {
		udk_error("load kernel firmware data failed: %s\n", hmstrerror(ret));
		udk_device_destroy(udkdev);
		return ret;
	}

	udk_of_root = udk_of_unflatten_tree(dtb);
	if (udk_of_root == NULL) {
		udk_error("resolve dts failed\n");
		udk_free(dtb);
		udk_device_destroy(udkdev);
		return E_HM_NOENT;
	}

	of_device_tree_trim(udk_of_root);
	ret = of_device_instantiate(udk_of_root, udkdev);
	if (ret < 0) {
		udk_error("of device instantiate failed, %s\n", hmstrerror(ret));
		udk_of_free_tree(udk_of_root);
		udk_free(dtb);
		udk_device_destroy(udkdev);
		return ret;
	}

	udk_free(dtb);
	return E_HM_OK;
}

static bool of_init_can_start(void)
{
#define QUERY_STATUS_TIMES    500000
#define QUERY_STATUS_SLEEP_TIME_NS    (5UL * NSEC_PER_MSEC)
	int count = 0;
	bool is_created = false;
	struct sd_proxy_dev_s *proxy = NULL;
	struct timespec ts = {0, QUERY_STATUS_SLEEP_TIME_NS};

	do {
		proxy = sd_proxy_dev_fetch();
		is_created = ((proxy != NULL) && proxy->is_created);
		count++;
		if (is_created || (count >= QUERY_STATUS_TIMES)) {
			break;
		}
		(void)udk_nanosleep(&ts, NULL);
	} while (!is_created && count < QUERY_STATUS_TIMES);
	return is_created;
}

int udk_fusion_of_init(void)
{
	if (!of_init_can_start()) {
		udk_warn("sd proxy.ko has not been created\n");
	}
	return of_create("of_udk");
}

static int of_node_name_add(const char *name)
{
	size_t len = strlen(name);
	char *buffer = NULL;
	struct dtnode_name *dtn = NULL;

	if (of_node_found(name)) {
		return 0;
	}

	buffer = udk_malloc(sizeof(char) * (len + 1));
	if (buffer == NULL) {
		return -ENOMEM;
	}

	dtn = udk_malloc(sizeof(struct dtnode_name));
	if (dtn == NULL) {
		udk_free(buffer);
		return -ENOMEM;
	}

	mem_zero_s(*dtn);
	mem_zero_b(buffer, len + 1);
	NOFAIL(strncpy_s(buffer, len + 1, name, len));
	dtn->name = buffer;
	dlist_insert(&dtnode_name_list, &dtn->node);

	return 0;
}
/* it should be called before udk_register_driver, i.e., before udk_fusion_of_init */
void udk_fusion_of_dtnode_deps(size_t count, ...)
{
	size_t i;
	va_list args;
	char *name = NULL;
	int ret = 0;

	va_start(args, count);
	for (i = 0; i < count; i++) {
		name = va_arg(args, char *);
		ret = of_node_name_add(name);
		BUG_ON(ret != 0);
	}
	va_end(args);
}

struct udk_dt_node *udk_fusion_of_root(void)
{
	return udk_of_root;
}
#else

struct udk_dt_node *udk_fusion_of_root(void)
{
	return NULL;
}

int udk_fusion_of_init(void)
{
	udk_info("udk fusion of disabled\n");
	return 0;
}

void udk_fusion_of_dtnode_deps(size_t count, ...)
{
	(void)count;
}
#endif
