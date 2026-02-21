/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Declare interface of of
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 18 15:54:02 2021
 */

#ifndef __UDK_OF_H__
#define __UDK_OF_H__

#include <sys/types.h>

#define NODE_INFO_NAME_LENGTH 32
#define NODE_INFO_TYPE_LENGTH 32
#define NODE_INFO_COMP_LENGTH 32

struct udk_dt_node;
struct udk_dt_prop;
struct udk_device;

struct udk_dt_node_info {
	char name[NODE_INFO_NAME_LENGTH];
	char type[NODE_INFO_TYPE_LENGTH];
	char compatible[NODE_INFO_COMP_LENGTH];
	const void *data;
};

void *udk_of_get_prop_value(const struct udk_dt_prop *prop);
size_t udk_of_get_prop_length(const struct udk_dt_prop *prop);

const char *udk_of_get_node_name(const struct udk_dt_node *node);
struct udk_dt_prop *udk_of_get_node_properties(const struct udk_dt_node *node);
struct udk_dt_node *udk_of_get_node_child(const struct udk_dt_node *parent);
struct udk_dt_node *udk_of_get_node_sibling(const struct udk_dt_node *node);
unsigned int udk_of_get_node_phandle(const struct udk_dt_node *node);

struct udk_dt_prop *udk_of_next_dt_property(const struct udk_dt_prop *prop);
struct udk_dt_node *udk_of_next_dt_node(struct udk_dt_node *prev);
struct udk_dt_node *udk_of_next_child_node(struct udk_dt_node *root, struct udk_dt_node *prev);

#define for_each_property_of_dt_node(dn, pp) \
	for ((pp) = udk_of_get_node_properties(dn); \
	     (pp) != NULL; \
	     (pp) = udk_of_next_dt_property(pp))

#define for_each_child_dt_node(_parent, _child) \
	for ((_child) = udk_of_get_node_child(_parent); \
	     (_child) != NULL; \
	     (_child) = udk_of_get_node_sibling(_child))

#define for_each_of_hmnode(root, node)			\
	for ((node) = udk_of_next_dt_node(root);	\
	     (node) != NULL;				\
	     (node) = udk_of_next_dt_node(node))

#define for_each_of_all_child(root, node)			\
	for ((node) = udk_of_next_child_node(root, root);	\
	     (node) != NULL;					\
	     (node) = udk_of_next_child_node(root, node))

int udk_of_init_soc_root(struct udk_dt_node *node);
struct udk_dt_node *udk_of_soc_root(void);
struct udk_dt_node *udk_of_find_node_by_name(struct udk_dt_node *root,
					     const char *name);
struct udk_dt_node *udk_of_find_node_by_fullname(struct udk_dt_node *root,
						 const char *fullname);
struct udk_dt_node *udk_of_find_child_by_name(struct udk_dt_node *root,
					     const char *name);
struct udk_dt_prop *udk_of_find_property(const struct udk_dt_node *node,
					 const char *pname);

int udk_of_prop_read_string(const struct udk_dt_node *node,
			    const char *prop_name,
			    const char **val);
int udk_of_prop_read_u64(const struct udk_dt_node *node,
			 const char *prop_name,
			 uint64_t *val);
int udk_of_prop_read_u32(const struct udk_dt_node *node,
			 const char *prop_name,
			 uint32_t *val);
int udk_of_prop_read_u32_array(const struct udk_dt_node *node,
			       const char *prop_name,
			       uint32_t *array,
			       size_t length);
int udk_of_prop_read_string_array(const struct udk_dt_node *node,
				  const char *prop_name, const char **strs,
				  size_t length);
struct udk_dt_node *udk_of_find_node_by_phandle(struct udk_dt_node *node,
						unsigned int phanle);
struct udk_device *udk_of_find_device_by_node(struct udk_dt_node *node);
int udk_of_device_create(const void *dtb);
struct udk_dt_node *udk_of_find_node_by_compatible(struct udk_dt_node *root,
						const char *type,
						const char *compatible);
const struct udk_dt_node_info *udk_of_match_node(struct udk_dt_node_info *matches,
					     const struct udk_dt_node *node);

#endif /* __UDK_OF_H__ */
