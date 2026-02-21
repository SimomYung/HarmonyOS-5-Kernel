/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK devicetree base
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 14 14:27:10 2019
 */

#include "of_fdt.h"
#include <udk/of.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/securec.h>
#include <libstrict/strict.h>

struct udk_dt_node *udk_of_root = NULL;

static struct udk_dt_prop *find_property(const struct udk_dt_node *node, const char *pname)
{
	struct udk_dt_prop *property = NULL;

	for_each_property_of_dt_node(node, property) {
		if (strcmp(property->name, pname) == 0) {
			return property;
		}
	}
	return NULL;
}

/* dts node name:  uart@09000000
 * dt_node name:   uart
 */
static int dt_construct_name(const char *name, char **buffer)
{
	size_t len = 0;
	const char *tmp_ptr = name;

	if (name == NULL) {
		return 0;
	}

	/* count real name length (before'@') */
	while (*tmp_ptr != '\0' && *tmp_ptr != '@') {
		tmp_ptr++;
		len++;
		if (len > MAX_BUFFER_LEN) {
			udk_error("node name is too long\n");
			return -EINVAL;
		}
	}

	*buffer = udk_malloc(sizeof(char) * (len + 1));
	if (*buffer == NULL) {
		return -ENOMEM;
	}

	mem_zero_b(*buffer, len + 1);
	NOFAIL(strncpy_s(*buffer, len + 1, name, len));

	return 0;
}

/* dts root node full_name: /
*/
static int dt_format_root_fullname(char **buffer)
{
	const char *root_fn = "/";	/* root node fullname */
	const size_t root_fn_len = strlen(root_fn); /* fullname length */
	const size_t fullname_len = root_fn_len + 1; /* including '\0' */

	*buffer = udk_malloc(sizeof(char) * fullname_len);
	if (*buffer == NULL) {
		return -ENOMEM;
	}

	mem_zero_b(*buffer, fullname_len);
	NOFAIL(strncpy_s(*buffer, fullname_len,
			 root_fn, root_fn_len));
	return 0;
}

/* dts normal node full_name:	/soc
 *				/soc/uart@09000000
 */
static int dt_format_normal_fullname(const struct udk_dt_node *parent,
				     const char *name, char **buffer)
{
	size_t parent_fn_len; /* paren fullname length */
	size_t name_len; /* node name length */
	size_t fullname_len = 0;
	char *tmp_ptr = NULL;

	parent_fn_len	= strlen(parent->full_name);
	name_len	= strlen(name);

	if (parent->parent != NULL) {
		/* node fullname: parent_fn + '/' + node_name + '\0' */
		fullname_len = parent_fn_len + 1 + name_len + 1;
	} else {
		/* node fullname: parent_fn('/') + node_name + '\0' */
		fullname_len = parent_fn_len + name_len + 1;
	}

	if (fullname_len > MAX_BUFFER_LEN) {
		udk_error("node path is too long\n");
		return -EINVAL;
	}

	*buffer = udk_malloc(sizeof(char) * fullname_len);
	if (*buffer == NULL) {
		return -ENOMEM;
	}
	mem_zero_b(*buffer, fullname_len);

	tmp_ptr = *buffer;
	NOFAIL(strncpy_s(tmp_ptr, fullname_len,
			 parent->full_name, parent_fn_len));
	tmp_ptr += parent_fn_len;

	/* if parent root is '/', we don't need to add
	 * '/' between parent_fn and node_name */
	if (parent->parent != NULL) {
		*(tmp_ptr++) = '/';
	}
	NOFAIL(strncpy_s(tmp_ptr, fullname_len, name, name_len));

	return 0;
}

static int dt_construct_fullname(const struct udk_dt_node *parent,
				 const char *name, char **buffer)
{
	if (buffer == NULL) {
		return -EINVAL;
	}
	if (parent == NULL) {
		return dt_format_root_fullname(buffer);
	} else {
		return dt_format_normal_fullname(parent, name, buffer);
	}
}

static struct udk_dt_prop *new_property(const char *pname,
					const size_t data_len,
					const void *data)
{
	size_t pname_len;
	char *prop_name = NULL;
	void *prop_data = NULL;
	struct udk_dt_prop *new_prop = NULL;

	pname_len = strlen(pname) + 1;
	if (pname_len <= 1) {
		udk_error("property name can not be null\n");
		return NULL;
	}

	if (data_len > 0 && data == NULL) {
		udk_error("property has no value, name=%s\n", pname);
		return NULL;
	}

/* In udk fusion mode, we will create property from ldk dtb, it's unpredictably */
#ifndef UDK_FUSION
	if (data_len > MAX_PROP_VALUE_LEN) {
		udk_error("property value overlength, pname=%s, len=%zu\n", pname, data_len);
		return NULL;
	}
#endif

	new_prop = udk_malloc(sizeof(struct udk_dt_prop));
	if (new_prop == NULL) {
		goto error_alloc_property_struct;
	}
	mem_zero_s(*new_prop);

	prop_name = udk_malloc(pname_len);
	if (prop_name == NULL) {
		goto error_alloc_property_name;
	}
	mem_zero_b(prop_name, pname_len);
	NOFAIL(strncpy_s(prop_name, pname_len,
			 pname, strlen(pname)));

	if (data_len > 0) {
		prop_data = udk_malloc(data_len);
		if (prop_data == NULL) {
			goto error_alloc_property_data;
		}
		NOFAIL(memcpy_s(prop_data, (size_t)data_len,
				data, (size_t)data_len));
	}

	new_prop->name = prop_name;
	new_prop->length = data_len;
	new_prop->value = prop_data;
	new_prop->next = NULL;

	return new_prop;

error_alloc_property_data:
	udk_free(prop_name);
error_alloc_property_name:
	udk_free(new_prop);
error_alloc_property_struct:
	return NULL;
}

static void delete_property(struct udk_dt_prop *prop)
{
	void *del_ptr = NULL;

	if (prop == NULL) {
		return;
	}
	if (prop->name != NULL) {
		del_ptr = prop->name;
		prop->name = NULL;
		udk_free(del_ptr);
	}
	if (prop->value != NULL) {
		del_ptr = prop->value;
		prop->value = NULL;
		udk_free(del_ptr);
	}

	udk_free(prop);
}

static void delete_property_list(struct udk_dt_prop *plist_head)
{
	struct udk_dt_prop *iter_ptr = plist_head;
	struct udk_dt_prop *del_ptr = NULL;

	if (plist_head == NULL) {
		return;
	}
	while (iter_ptr != NULL) {
		del_ptr = iter_ptr;
		iter_ptr = iter_ptr->next;
		delete_property(del_ptr);
	}
}

static void delete_dt_node(struct udk_dt_node *np)
{
	void *del_ptr = NULL;

	if (np == NULL) {
		return;
	}
	if (np->name != NULL) {
		del_ptr = np->name;
		np->name = NULL;
		udk_free(del_ptr);
	}
	if (np->full_name != NULL) {
		del_ptr = np->full_name;
		np->full_name = NULL;
		udk_free(del_ptr);
	}
	del_ptr = np->properties;
	np->properties = NULL;
	delete_property_list(del_ptr);

	udk_free(np);
}

static void delete_dt_node_recurse(struct udk_dt_node *root)
{
	void *del_ptr = NULL;

	if (root == NULL) {
		return;
	}
	if (root->child != NULL) {
		del_ptr = root->child;
		root->child = NULL;
		delete_dt_node_recurse(del_ptr);
	}
	if (root->sibling != NULL) {
		del_ptr = root->sibling;
		root->sibling = NULL;
		delete_dt_node_recurse(del_ptr);
	}
	delete_dt_node(root);
}

static int dt_construct_dtb_property(const void *dtb, int node_offset,
				     struct udk_dt_prop **prop_list)
{
	int offset;
	int err = 0;
	struct udk_dt_prop *prop_head = NULL;
	struct udk_dt_prop *prop_tail = NULL;

	if (prop_list == NULL) {
		return -EINVAL;
	}
	fdt_for_each_property_offset(offset, dtb, node_offset) {
		const void *data = NULL;
		const char *pname = NULL;
		int data_len = 0;
		struct udk_dt_prop *new_prop = NULL;

		data = fdt_getprop_by_offset(dtb, offset, &pname, &data_len);
		if (data == NULL) {
			udk_error("Failed: fdt resolving error\n");
			err = -EINVAL;
			goto error_construct_property;
		}

		new_prop = new_property(pname, (size_t)(unsigned int)data_len, data);
		if (new_prop == NULL) {
			udk_error("create new prop failed\n");
			err = -ENOMEM;
			goto error_construct_property;
		}

		if (prop_tail == NULL) {
			prop_head = new_prop;
			prop_tail = new_prop;
		} else {
			prop_tail->next = new_prop;
			prop_tail = new_prop;
		}
	}

	*prop_list = prop_head;
	return 0;

error_construct_property:
	delete_property_list(prop_head);

	return err;
}

static int dt_attach_default_property(struct udk_dt_node *np)
{
	/* attach default property: name */
	struct udk_dt_prop *name_prop = NULL;
	struct udk_dt_prop *tail = NULL;
	size_t name_len;
	const char *null_name = "<NULL>";
	const size_t null_len = strlen(null_name);

	name_prop = find_property(np, "name");
	if (name_prop != NULL) {
		/* has property name */
		return 0;
	}

	name_len = strlen(np->name);
	/* name is regarded as a prop value, we have to add
	 * length of tail '\0' */
	if (name_len == 0) {
		name_prop = new_property("name", null_len + 1, null_name);
	} else {
		name_prop = new_property("name", name_len + 1, np->name);
	}
	if (name_prop == NULL) {
		return -ENOMEM;
	}

	tail = np->properties;
	if (tail == NULL) {
		np->properties = name_prop;
	} else {
		while (tail->next != NULL) {
			tail = tail->next;
		}
		/* name_prop->next is set NULL in new_property() */
		tail->next = name_prop;
	}

	return 0;
}

static int dt_set_node_phandle(struct udk_dt_node *np)
{
	struct udk_dt_prop *prop = find_property(np, "phandle");
	void *data = NULL;

	if (prop == NULL) {
		return 0;
	}

	data = prop->value;
	if (data == NULL) {
		udk_error("property phandle has no value\n");
		return -EINVAL;
	}
	np->phandle = (unsigned int)fdt32_value(data);

	return 0;
}

static int dt_construct_property(const void *dtb, int node_offset,
				 struct udk_dt_node *np)
{
	int err;

	/* construct properties from dtb */
	err = dt_construct_dtb_property(dtb, node_offset, &np->properties);
	if (err != 0) {
		return err;
	}
	/* construct default properties <name> */
	err = dt_attach_default_property(np);
	if (err != 0) {
		return err;
	}
	/* set np->phandle */
	err = dt_set_node_phandle(np);
	if (err != 0) {
		return err;
	}

	return 0;
}

static void dt_set_node_type(struct udk_dt_node *np)
{
	struct udk_dt_prop *prop = find_property(np, "device_type");

	if (prop == NULL || prop->value == NULL) {
		np->type = "<NULL>";
	} else {
		np->type = prop->value;
	}
}

static struct udk_dt_node *unflatten_device_node(const void *dtb,
						 int node_offset,
						 struct udk_dt_node *parent);
static int dt_construct_subnode(const void *dtb,
				struct udk_dt_node *parent,
				int parent_offset)
{
	int offset;

	fdt_for_each_subnode(offset, dtb, parent_offset) {
		struct udk_dt_node *tail = parent->child;
		struct udk_dt_node *new_child = NULL;

		new_child = unflatten_device_node(dtb, offset, parent);
		if (new_child == NULL) {
			return -EFAULT;
		}
		if (tail == NULL) {
			parent->child = new_child;
		} else {
			while (tail->sibling != NULL) {
				tail = tail->sibling;
			}
			tail->sibling = new_child;
		}
	}

	return 0;
}

static struct udk_dt_node *unflatten_device_node(const void *dtb,
						 int node_offset,
						 struct udk_dt_node *parent)
{
	struct udk_dt_node *np = NULL;
	const char *name = NULL;
	int err = 0;

	/* caller will do parameter check */
	np = udk_malloc(sizeof(struct udk_dt_node));
	if (np == NULL) {
		goto error_np_malloc;
	}
	mem_zero_s(*np);

	/* get fdt node name (eg. uart@09000000) */
	name = fdt_get_name(dtb, node_offset, NULL);
	if (name == NULL && parent != NULL) {
		goto error_construct_name;
	}
	/* fill node->name (eg. uart) */
	err = dt_construct_name(name, &np->name);
	if (err != 0) {
		goto error_construct_name;
	}
	/* fill node->full_name (eg. /cpus/cpu@6/pmu) */
	err = dt_construct_fullname(parent, name, &np->full_name);
	if (err != 0) {
		goto error_construct_fullname;
	}
	/* construct properties from dtb */
	err = dt_construct_property(dtb, node_offset, np);
	if (err != 0) {
		goto error_construct_property_list;
	}

	dt_set_node_type(np);
	np->offset = (unsigned int)node_offset;
	np->parent = parent;
	np->sibling = NULL;
	np->child = NULL;

	/* construct subnode */
	err = dt_construct_subnode(dtb, np, node_offset);
	if (err != 0) {
		goto error_create_child_node;
	}

	return np;

error_create_child_node:
	delete_dt_node_recurse(np->child);
error_construct_property_list:
	delete_property_list(np->properties);
	udk_free(np->full_name);
error_construct_fullname:
	udk_free(np->name);
error_construct_name:
	udk_free(np);
error_np_malloc:
	return NULL;
}

struct udk_dt_node *udk_of_unflatten_tree(const void* dtb)
{
	struct udk_dt_node* np = NULL;

	if (dtb == NULL) {
		udk_error("Invalid dtb address\n");
		return NULL;
	}

	if (fdt_check_header(dtb) != 0) {
		udk_error("Invalid device tree blob header\n");
		return NULL;
	}

	np = unflatten_device_node(dtb, 0, NULL);

	return np;
}

void udk_of_free_tree(struct udk_dt_node *root)
{
	delete_dt_node_recurse(root);
}

/* The #address-cells and #size-cells properties may be used
 * in any device node that has children in the devicetree hierarchy
 * and describes how child device nodes should be addressed.
 *
 * The #address-cells property defines the number of <u32> cells
 * used to encode the address field in a child node's reg property.
 * The #size-cells property defines the number of <u32> cells used to
 * encode the size field in a child node's reg property.
 * The #address-cells and #size-cells properties are NOT inherited
 * from ancestors in the devicetree. They shall be explicitly defined.
 *
 * A DTSpec-compliant boot program shall supply #address-cells and #size-cells
 * on all nodes that have children.
 *
 * If missing, a client program should assume a default value
 * of 2 for #address-cells, and a value of 1 for #size-cells.
 */
static uint32_t get_reg_addr_cells(const struct udk_dt_node *node)
{
	struct udk_dt_prop *prop = NULL;

	if (node->parent != NULL) {
		prop = find_property(node->parent, "#address-cells");
		if (prop != NULL && prop->value != NULL) {
			return fdt32_value(prop->value);
		}
	}

	return DEFAULT_ADDR_CELLS;
}

static uint32_t get_reg_size_cells(const struct udk_dt_node *node)
{
	struct udk_dt_prop *prop = NULL;

	if (node->parent != NULL) {
		prop = find_property(node->parent, "#size-cells");
		if (prop != NULL && prop->value != NULL) {
			return fdt32_value(prop->value);
		}
	}

	return DEFAULT_SIZE_CELLS;
}

static int udk_of_node_reg_query(const struct udk_dt_node *node,
				 uint32_t *addr_cells_p, uint32_t *size_cells_p)
{
	uint32_t addr_cells;
	uint32_t size_cells;

	addr_cells = get_reg_addr_cells(node);
	size_cells = get_reg_size_cells(node);
	if (addr_cells == 0U || addr_cells > MAX_ADDR_CELLS ||
	    size_cells > MAX_SIZE_CELLS) {
		return -EINVAL;
	}

	*addr_cells_p = addr_cells;
	*size_cells_p = size_cells;

	return 0;
}

int udk_of_reg_count(const struct udk_dt_node *node, unsigned int *nump)
{
	struct udk_dt_prop *reg_prop = 0;
	uint32_t addr_cells = 0;
	uint32_t size_cells = 0;
	uint32_t reg_size;
	int err;

	if (node == NULL) {
		return -EINVAL;
	}
	reg_prop = udk_of_find_property(node, "reg");
	if (reg_prop == NULL) {
		if (nump != NULL) {
			*nump = 0;
		}
		return 0;
	}

	err = udk_of_node_reg_query(node, &addr_cells, &size_cells);
	if (err != 0) {
		return err;
	}

	reg_size = addr_cells + size_cells;
	if (reg_size == 0) {
		return -EFAULT;
	}

	if (reg_prop->length % (sizeof(uint32_t) * reg_size) != 0) {
		return -EFAULT;
	}
	if (nump != NULL) {
		*nump = (unsigned int)(reg_prop->length / (sizeof(uint32_t) * reg_size));
	}

	return 0;
}

int udk_of_get_reg_value(const struct udk_dt_node *node, unsigned int reg_index,
			 uint64_t *addr, uint64_t *size)
{
	uint32_t *start = NULL;
	uint32_t *cellp = NULL;
	struct udk_dt_prop *reg_prop = NULL;
	uint32_t addr_cells = 0;
	uint32_t size_cells = 0;
	int err;

	if (node == NULL) {
		return -EINVAL;
	}

	reg_prop = udk_of_find_property(node, "reg");
	if (reg_prop == NULL || reg_prop->value == NULL) {
		return -ENOENT;
	}

	err = udk_of_node_reg_query(node, &addr_cells, &size_cells);
	if (err != 0) {
		return err;
	}

	start = (uint32_t *)reg_prop->value;
	cellp = &start[reg_index * (addr_cells + size_cells)];
	if (addr != NULL && addr_cells != 0) {
		if (addr_cells == 2) { /* 2: 64bit value */
			*addr = fdt64_value(cellp);
		} else if (addr_cells == 1) {
			*addr = fdt32_value(cellp);
		} else {
			return -EINVAL;
		}
	}

	if (size != NULL && size_cells != 0) {
		if (size_cells == 2) { /* 2: 64bit value */
			*size = fdt64_value(&cellp[addr_cells]);
		} else if (size_cells == 1) {
			*size = fdt32_value(&cellp[addr_cells]);
		} else {
			return -EINVAL;
		}
	}

	return 0;
}

int udk_of_get_reg_names(const struct udk_dt_node *node, const char **names, size_t length)
{
	return udk_of_prop_read_string_array(node, "reg-names", names, length);
}

void udk_of_delete_dt_node(struct udk_dt_node *node)
{
	delete_dt_node(node);
}
