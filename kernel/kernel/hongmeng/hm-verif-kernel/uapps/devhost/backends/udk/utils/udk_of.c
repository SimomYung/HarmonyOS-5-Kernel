/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Define common fdt interface for of driver and udk
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 05 15:50:34 2021
 */

#include <udk/of.h>
#include <udk/log.h>
#include <udk/mutex.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/securec.h>
#include <internal/of.h>
#include <libstrict/strict.h>

static struct udk_dt_node *g_soc_root = NULL;
static struct udk_mutex g_udk_of_mutex = UDK_MUTEX_INIT;

struct udk_dt_prop *udk_of_get_node_properties(const struct udk_dt_node *node)
{
	return (node == NULL) ? NULL : node->properties;
}

struct udk_dt_node *udk_of_get_node_child(const struct udk_dt_node *parent)
{
	return (parent == NULL) ? NULL : parent->child;
}

struct udk_dt_node *udk_of_get_node_sibling(const struct udk_dt_node *node)
{
	return (node == NULL) ? NULL : node->sibling;
}

const char *udk_of_get_node_name(const struct udk_dt_node *node)
{
	return (node == NULL) ? NULL : (const char *)node->name;
}

unsigned int udk_of_get_node_phandle(const struct udk_dt_node *node)
{
	return (node == NULL) ? 0 : node->phandle;
}

struct udk_dt_prop *udk_of_next_dt_property(const struct udk_dt_prop *prop)
{
	return (prop == NULL) ? NULL : prop->next;
}

void *udk_of_get_prop_value(const struct udk_dt_prop *prop)
{
	return (prop == NULL) ? NULL : prop->value;
}

size_t udk_of_get_prop_length(const struct udk_dt_prop *prop)
{
	return (prop == NULL) ? 0U : prop->length;
}

/* The tree structure:
 *
 * node0
 *  |
 *  | child
 *  |
 * node1 -------> node8
 *  |   sibling
 *  |
 * node2 --> node3 --> node4
 *                      |
 *                      |
 *                     node5 --> node6 --> node7
 *
 * The preorder traversal rule as follow:
 * 1. if prev has child, next is prev->child
 *    (node0 -> node1)
 * 2. if prev has sibling, next is prev->sibling
 *    (node2 -> node3)
 * 3. if prev has neither, find its first parent(or upper) with sibling,
 *    and the next is parent->sibling
 *    (node7 -> node8)
 *
 */
struct udk_dt_node *udk_of_next_dt_node(struct udk_dt_node *prev)
{
	struct udk_dt_node *np = prev;
	/* preorder traversal */
	if (np == NULL) {
		return NULL;
	}

	if (np->child != NULL) {
		return np->child;
	}

	/* find the first parent node with sibling */
	while (np->parent != NULL && np->sibling == NULL) {
		np = np->parent;
	}

	/* when np->parent == NULL && np->sibling == NULL
	 * np will point to the root node,
	 * np->sibling == NULL */
	np = np->sibling;

	return np;
}

/* Obtain the next child node after prev in the subtree of the root. */
struct udk_dt_node *udk_of_next_child_node(struct udk_dt_node *root, struct udk_dt_node *prev)
{
	struct udk_dt_node *np = prev;

	if (np == NULL) {
		return NULL;
	}

	if (np->child != NULL) {
		return np->child;
	}

	/* find the first parent node with sibling, except root node */
	while ((np != root) && (np->parent != NULL) && (np->sibling == NULL)) {
		np = np->parent;
	}
	/* we should stop at root node to ensure next child node in the subtree of the root */
	if (np == root) {
		return NULL;
	}

	/* when np->parent == NULL && np->sibling == NULL
	 * np will point to the root node,
	 * np->sibling == NULL */
	np = np->sibling;

	return np;
}

struct udk_dt_node *udk_of_find_node_by_name(struct udk_dt_node *root,
					     const char *name)
{
	struct udk_dt_node *np = NULL;

	if (root == NULL || name == NULL) {
		return NULL;
	}
	for_each_of_hmnode(root, np) {
		if (strcmp(np->name, name) == 0) {
			return np;
		}
	}
	return NULL;
}

struct udk_dt_node *udk_of_find_node_by_fullname(struct udk_dt_node *root,
						 const char *fullname)
{
	struct udk_dt_node *np = NULL;

	if (root == NULL || fullname == NULL) {
		return NULL;
	}
	for_each_of_hmnode(root, np) {
		if (strcmp(np->full_name, fullname) == 0) {
			return np;
		}
	}
	return NULL;
}

struct udk_dt_node *udk_of_find_child_by_name(struct udk_dt_node *root,
					     const char *name)
{
	struct udk_dt_node *np = NULL;

	if (root == NULL || name == NULL) {
		return NULL;
	}

	for_each_of_all_child(root, np) {
		if (strcmp(np->name, name) == 0) {
			return np;
		}
	}

	return NULL;
}

struct udk_dt_prop *udk_of_find_property(const struct udk_dt_node *node,
					 const char *pname)
{
	struct udk_dt_prop *prop = NULL;

	if (node == NULL || pname == NULL) {
		return NULL;
	}
	for_each_property_of_dt_node(node, prop) {
		if (strcmp(prop->name, pname) == 0) {
			return prop;
		}
	}

	return NULL;
}

int udk_of_prop_read_string(const struct udk_dt_node *node,
			    const char *prop_name,
			    const char **val)
{
	struct udk_dt_prop *prop = NULL;

	if (node == NULL || prop_name == NULL || val == NULL) {
		return -EINVAL;
	}
	prop = udk_of_find_property(node, prop_name);
	if (prop == NULL) {
		*val = NULL;
		return -EINVAL;
	}
	*val = (const char *)prop->value;

	return 0;
}

int udk_of_prop_read_u64(const struct udk_dt_node *node,
			 const char *prop_name,
			 uint64_t *val)
{
	struct udk_dt_prop *prop = NULL;

	if (node == NULL || prop_name == NULL || val == NULL) {
		return -EINVAL;
	}
	prop = udk_of_find_property(node, prop_name);
	if (prop == NULL) {
		return -EINVAL;
	}
	if (prop->length < sizeof(uint64_t)) {
		return -EINVAL;
	}
	*val = fdt64_value(prop->value);

	return 0;
}

int udk_of_prop_read_u32_array(const struct udk_dt_node *node,
			       const char *prop_name,
			       uint32_t *array,
			       size_t length)
{
	struct udk_dt_prop *prop = NULL;
	uint32_t *values = NULL;

	if (node == NULL || prop_name == NULL ||
	    array == NULL || length == 0U) {
		return -EINVAL;
	}
	prop = udk_of_find_property(node, prop_name);
	if (prop == NULL) {
		return -EINVAL;
	}
	if (prop->length < length * sizeof(uint32_t)) {
		return -EINVAL;
	}
	values = (uint32_t *)prop->value;
	for (size_t i = 0; i < length; i++) {
		array[i] = fdt32_value(&values[i]);
	}

	return 0;
}

int udk_of_prop_read_u32(const struct udk_dt_node *node,
			 const char *prop_name,
			 uint32_t *val)
{
	return udk_of_prop_read_u32_array(node, prop_name, val, 1U);
}

int udk_of_prop_read_string_array(const struct udk_dt_node *node,
				  const char *prop_name, const char **strs,
				  size_t length)
{
	struct udk_dt_prop *prop = NULL;
	const char *pos = NULL;
	const char *end = NULL;

	if (node == NULL || prop_name == NULL ||
	    strs == NULL || length == 0) {
		return -EINVAL;
	}
	prop = udk_of_find_property(node, prop_name);
	if (prop == NULL) {
		return -ENOENT;
	}
	pos = (const char *)prop->value;
	end = (const char *)prop->value + prop->length;
	for (size_t i = 0; (pos < end) && (i < length); i++) {
		size_t len = strnlen(pos, end - pos) + 1U;
		if ((pos + len) > end) {
			return -EILSEQ;
		}
		*strs = pos;
		strs++;
		pos += len;
	}

	return 0;
}

int udk_of_init_soc_root(struct udk_dt_node *node)
{
	if (node == NULL) {
		return -EINVAL;
	}
	if (g_soc_root != NULL) {
		return -EEXIST;
	}
	g_soc_root = node;

	return 0;
}

struct udk_dt_node *udk_of_soc_root(void)
{
	return g_soc_root;
}

struct udk_dt_node *udk_of_find_root_node(struct udk_dt_node *node)
{
	if (node == NULL) {
		udk_error("invalid node\n");
		return NULL;
	}
	while (node->parent != NULL) {
		node = node->parent;
	}

	return node;
}

struct udk_dt_node *udk_of_find_node_by_phandle(struct udk_dt_node *node,
						unsigned int phandle)
{
	struct udk_dt_node *iter_node = NULL;
	struct udk_dt_node *root = NULL;

	if (node == NULL) {
		udk_warn("invalid node\n");
		return NULL;
	}
	root = udk_of_find_root_node(node);
	if (root == NULL) {
		udk_warn("root node not found\n");
		return NULL;
	}
	for_each_of_hmnode(root, iter_node) {
		if (phandle == iter_node->phandle) {
			return iter_node;
		}
	}

	return NULL;
}

struct udk_device *udk_of_find_device_by_node(struct udk_dt_node *node)
{
	return (node == NULL) ? NULL : node->dev;
}

static bool node_type_is_eq(const struct udk_dt_node *node, const char *type)
{
	return strcmp(node->type, type) == 0;
}

static bool node_name_is_eq(const struct udk_dt_node *node, const char *name)
{
	return strcmp(node->name, name) == 0;
}

static const char *prop_get_next_string(struct udk_dt_prop *prop, const char *cur)
{
	const char *curv = cur;

	if (prop == NULL) {
		return NULL;
	}

	if (cur == NULL) {
		return prop->value;
	}

	curv += strlen(cur) + 1;
	if (curv >= (char *)prop->value + prop->length) {
		return NULL;
	}

	return curv;
}

static int node_priority_by_compatible(const struct udk_dt_node *node, const char *compat)
{
#define PROP_DEFAULT_MAX_NUM (INT_MAX / 4)
	struct udk_dt_prop *prop = NULL;
	const char *cp = NULL;
	int index = PROP_DEFAULT_MAX_NUM;
#undef PROP_DEFAULT_MAX_NUM

	if (compat != NULL && strlen(compat) != 0) {
		prop = udk_of_find_property(node, "compatible");
		for (cp = prop_get_next_string(prop, NULL); cp != NULL && index > 0;
		     cp = prop_get_next_string(prop, cp), index--) {
			if (strcasecmp(cp, compat) == 0) {
				return index;
			}
		}
	}

	return -EEXIST;
}

static int udk_of_match_node_by_constrain(const struct udk_dt_node *node, const char *compat,
				       const char *type, const char *name)
{
#define SCORE_COMPAT_WEIGHT 2U
#define SCORE_TYPE_WEIGHT 1U
#define SCORE_DEFAULT_VALUE 1U
	int tmp = 0, score = 0;

	/* Matching compatible is the most important */
	if (compat != NULL && strlen(compat) != 0) {
		tmp = node_priority_by_compatible(node, compat);
		if (tmp < 0) {
			return 0;
		}
		score += (unsigned int)tmp << SCORE_COMPAT_WEIGHT;
	}

	/* Matching type is better than matching name */
	if (type != NULL && strlen(type) != 0) {
		if (!node_type_is_eq(node, type)) {
			return 0;
		}
		score += SCORE_DEFAULT_VALUE << SCORE_TYPE_WEIGHT;
	}

	/* Matching name is a bit better than not */
	if (name != NULL && strlen(name) != 0) {
		if (!node_name_is_eq(node, name)) {
			return 0;
		}
		score += SCORE_DEFAULT_VALUE;
	}

#undef SCORE_COMPAT_WEIGHT
#undef SCORE_TYPE_WEIGHT
#undef SCORE_DEFAULT_VALUE
	return score;
}

struct udk_dt_node *udk_of_find_node_by_compatible(struct udk_dt_node *root, const char *type,
						const char *compatible)
{
	struct udk_dt_node *np = NULL;

	if (compatible == NULL) {
		udk_error("invalid compatible\n");
		return NULL;
	}
	if (root == NULL) {
		root = udk_of_soc_root();
		if (root == NULL) {
			udk_error("soc root is NULL");
			return NULL;
		}
	}

	for_each_of_hmnode(root, np) {
		if (udk_of_match_node_by_constrain(np, compatible, type, NULL) != 0) {
			break;
		}
	}

	return np;
}

const struct udk_dt_node_info *udk_of_match_node(struct udk_dt_node_info *matches,
					     const struct udk_dt_node *node)
{
	const struct udk_dt_node_info *best_match = NULL;
	int score, best_score = 0;

	if (matches == NULL || node == NULL) {
		return NULL;
	}

	(void)udk_mutex_lock(&g_udk_of_mutex);
	for (; matches->name[0] != '\0' || matches->type[0] != '\0' ||
	       matches->compatible[0] != '\0'; matches++) {
		score = udk_of_match_node_by_constrain(node, matches->compatible,
						       matches->type, matches->name);
		if (score > best_score) {
			best_match = matches;
			best_score = score;
		}
	}
	(void)udk_mutex_unlock(&g_udk_of_mutex);
	return best_match;
}
