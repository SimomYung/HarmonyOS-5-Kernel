/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost backend args
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 23 10:36:37 2021
 */
#include <lnxbase/lnxbase.h>

#include <getopt.h>
#include <stdlib.h>
#include <libalgo/rbtree.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <devhost/log.h>

#include "internal.h"

struct backend_arg {
	struct rbt_node node;
    char *key;
    char *value;
};

static struct lnxbase_args backend_args;
static struct rbt_tree __args_tree = RBT_ROOT;

static void __release_backend_arg(struct rbt_node *p)
{
    struct backend_arg *cur_arg = rbt_entry(p, struct backend_arg, node);

    BUG_ON(cur_arg == NULL);
    free(cur_arg->key);
    free(cur_arg->value);
    free(cur_arg);
}

static struct backend_arg *backend_arg_search(const char *key)
{
	struct rbt_node *p = __args_tree.root;
	struct backend_arg *cur = NULL;

	if (key == NULL) {
		return NULL;
	}

	while (p != NULL) {
		cur = rbt_entry(p, struct backend_arg, node);
		if (strcmp(key, cur->key) > 0) {
			if (p->rbt_right == NULL) {
				break;
			}
			p = p->rbt_right;
		} else if (strcmp(key, cur->key) < 0) {
			if (p->rbt_left == NULL) {
				break;
			}
			p = p->rbt_left;
		} else {
			return cur;
		}
	}

	return NULL;
}

static int backend_args_insert(struct backend_arg *arg)
{
	struct rbt_node *p = __args_tree.root;
	struct backend_arg *cur = NULL;
	bool is_right = false;

	if (unlikely(arg == NULL)) {
		return E_HM_INVAL;
	}

	while (p != NULL) {
		cur = rbt_entry(p, struct backend_arg, node);
		if (strcmp(arg->key, cur->key) > 0) {
			if (p->rbt_right == NULL) {
				is_right = true;
				break;
			}
			p = p->rbt_right;
		} else if (strcmp(arg->key, cur->key) < 0) {
			if (p->rbt_left == NULL) {
				is_right = false;
				break;
			}
			p = p->rbt_left;
		} else {
			return E_HM_POSIX_EXIST;
		}
	}

	rbt_insert(&__args_tree, &arg->node, p, is_right);

	return E_HM_OK;
}

static int make_backend_args(const char *key, const char *value)
{
	struct backend_arg *arg = NULL;
	int err = E_HM_OK;
	arg = (struct backend_arg *)malloc(sizeof(struct backend_arg));
	if (unlikely(arg == NULL)) {
		return E_HM_NOMEM;
	}
    arg->key = strdup(key);
    arg->value = strdup(value);
    if (unlikely(arg->key == NULL || arg->value == NULL)) {
        if (arg->key != NULL) {
            free(arg->key);
        }
        if (arg->value != NULL) {
            free(arg->value);
        }
        free(arg);
        return E_HM_NOMEM;
    }

	err = backend_args_insert(arg);
	if (unlikely(err != E_HM_OK)) {
        free(arg->key);
        free(arg->value);
		free(arg);
	}
	return err;
}

static int __parse_extra_args(char *subopts)
{
	int err = E_HM_OK;
	char *pair = NULL;
	char *key = NULL;
	char *buf = NULL;
	char *value = NULL;
	char *tok_buf = NULL;

	if (unlikely(subopts == NULL)) {
		return E_HM_INVAL;
	}
	pair = strtok_r(subopts, ",", &tok_buf);

	while (pair != NULL) {
		key = pair;
		buf = strstr(pair, "=");
		if (buf == NULL) {
			err = E_HM_INVAL;
			break;
		}
		buf[0] = '\0';
		value = buf + 1;
		err = make_backend_args(key, value);
		if (err != E_HM_OK) {
			break;
		}
		pair = strtok_r(NULL, ",", &tok_buf);
	}

    if (err != E_HM_OK) {
        rbt_destroy_tree(&__args_tree, __release_backend_arg);
        dh_panic("__parse_extra_args failed (%s)\n", hmstrerror(err));
    }
	return err;
}

static const struct option long_options[] = {
	{"irqthread_affinity", required_argument, NULL, (int)'i'},
	{"extensions", required_argument, NULL, (int)'e'},
	{"args", required_argument, NULL, (int)'a'},
	{NULL, no_argument, NULL, 0}
};

int lnxbase_args_init(void)
{
	int err;
	int opt;
	const struct devhost_backend_args *args = devhost_get_backend_args();

	while ((opt = getopt_long(args->argc, args->argv, "", long_options, NULL)) != -1) {
		switch (opt) {
		case (int) 'e':
			backend_args.extensions = optarg;
			break;
		case (int) 'a':
			err = __parse_extra_args(optarg);
			if (err != E_HM_OK) {
				hm_error("lnxbase: args parse failed, args: '%s'\n", optarg);
				return -hmerrno2posix(err);
			}
			break;
		default:
			break;
		}
	}

	return 0;
}

const struct lnxbase_args *lnxbase_backend_args(void)
{
	return &backend_args;
}

const char *lnxbase_backend_args_find(const char *key)
{
	struct backend_arg *arg = NULL;

	arg = backend_arg_search(key);
	if (arg == NULL) {
		return NULL;
	}
	return arg->value;
}
