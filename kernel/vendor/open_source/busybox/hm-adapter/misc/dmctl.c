/* vi: set sw=4 ts=4: */
/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022.
 * Description: Implementation of dmctl cmd
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 18 11:33:13 2022
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

//config:config HM_DMCTL
//config:	bool "hm dmctl"
//config:	default n
//config:	help
//config:	The dmctl command.

//kbuild:lib-$(CONFIG_HM_DMCTL) += dmctl.o
//kbuild:lib-$(CONFIG_HM_DMCTL) += kmod.o
//applet:IF_HM_DMCTL(APPLET_NOFORK(dmctl, dmctl, BB_DIR_BIN, BB_SUID_DROP, dmctl))

/* "dmctl --help" is special-cased to ignore --help */
//usage:#define dmctl_trivial_usage NOUSAGE_STR
//usage:#define dmctl_usage ""
//usage:#define dmctl_full_usage
//usage:	"$ dmctl\n"
//usage:	"dmctl test succeed\n"

#include <stdio.h>
#include <getopt.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_io/io.h>
#include <libhmsrv_io/mgmt.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhwsecurec/securec.h>
#include <unistd.h>
#include "dmctl.h"
#include "kmod.h"
#undef barrier
#undef ARRAY_SIZE
#include "libbb.h"

#define DMCTL_OPTIONS "dst:i:a:g:u:h"
#define DMCTL_OK	0
#define DMCTL_ERR	1

static bool g_internal_mod;

static void show_usage(const char *name)
{
	(void)printf("HongMeng device manager tool\n");
	(void)printf("Usage: %s [TARGET] <COMMAND> [ARGUMENT...]\n", name);
	(void)printf("\n"
	       "TARGET: (root devhost if none specified)\n"
	       "  -t, --target-devid             specify target devhost by devid\n"
	       "  -a, --target-devhost           specify target devhost by cnode_idx\n"
	       "  -g, --target-group		 specify target devhost by isolate group\n"
	       "\n"
	       "COMMAND:\n"
	       "  -d, --dump		         dump device/driver information\n"
	       "  -s, --sync                     sync device asynchronous operations\n"
	       "  -i, --insert modname [modargs] insert modules to target devhost\n"
	       "  -u, --unload modname           unload modules from target devhost\n"
	       "  -h, --help                     show help information\n"
	       "\n");
}

static int __dump_callback(void *ctx, const char *data)
{
	int *dumplen = (int *)ctx;
	int ret;

	BUG_ON(dumplen == NULL);

	ret = printf("%s", data);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	*dumplen += ret;

	return E_HM_OK;
}
static int dmctl_dump(void)
{
	int dumplen = 0;
	int ret;

	ret = hmsrv_io_mgmt_dump(__dump_callback, (void *)&dumplen);
	if (ret < 0) {
		(void)fprintf(stderr, "query dump data failed, ret=%s\n",
			      hmstrerror(ret));
	}

	if (dumplen < 0) {
		(void)fprintf(stderr, "<no output>\n");
	}

	return ret;
}

/* the length of =, \"\" and \0 */
#define ARG_EXTRA_LEN 4
/*
 * arg is x=y, arg_str is x="y"
 */
static int parse_one_arg(char *arg, char *arg_str, unsigned int max_len)
{
	char *equal_char = NULL;
	char *name = NULL;
	char *value = NULL;
	char *tmp = NULL;
	int name_len;
	int value_len;
	int ret;

	name = arg;
	equal_char = strchr(arg, '=');
	if (equal_char == NULL) {
		return E_HM_INVAL;
	}
	name_len = (int)(equal_char - arg);
	if (name_len == 0) {
		return E_HM_INVAL;
	}

	value = arg + name_len + 1;
	value_len = ((int)strlen(arg) - name_len) - 1;
	if (value_len <= 0) {
		return E_HM_INVAL;
	}
	/* detect if there is illegal \" in the string */
	tmp = strchr(value, '"');
	if (tmp != NULL) {
		(void)fprintf(stderr, "argument %s has illegal extra \"\n", arg);
		return E_HM_INVAL;
	}

	ret = sprintf_s(arg_str, max_len, "%.*s=\"%.*s\"",
			(unsigned int)name_len, name, (unsigned int)value_len, value);
	if (ret < 0) {
		/* the only failed case of sprintf_s here is destMax is not long enough */
		(void)fprintf(stderr, "argument list is too long\n");
		return E_HM_INVAL;
	}

	return name_len + value_len + ARG_EXTRA_LEN;
}

static int formalize_args(int argc, char *argv[], char *arg_str, unsigned int max_len)
{
	int pos = 0;
	unsigned int len = 0;
	int ret;
	while ((pos < argc) && (len < max_len)) {
		ret = parse_one_arg(argv[pos], arg_str + len, max_len - len);
		if (ret < 0) {
			return E_HM_INVAL;
		}
		/* modify the end of the last arg to space to concat string */
		if (len > 0) {
			arg_str[len - 1U] = ' ';
		}
		len += (unsigned int)ret;
		pos++;
	}
	return pos;
}

int dmctl_insert_mod(char *argv[], int argc, const char *name,
		     struct devmgr_mgmt_target target)
{
	char args[MODULE_ARGS_LEN] = {0};
	int ret;

	ret = formalize_args(argc, argv, args, MODULE_ARGS_LEN);
	if (ret < 0) {
		(void)fprintf(stderr, "argument list parsing failed\n");
		return E_HM_INVAL;
	}

	/* load module */
	if (g_internal_mod) {
		ret = hmsrv_io_mgmt_load_module(target, name, args);
	} else {
		ret = hmsrv_io_mgmt_load_external_module(target, name, args);
	}
	if (ret != 0) {
		(void)fprintf(stderr, "load module \"%s\" with args \"%s\" failed, err=%s\n",
			name, args, hmstrerror(ret));
		return ret;
	}

	return E_HM_OK;
}

int dmctl_unload_mod(const char *name, struct devmgr_mgmt_target target)
{
	int ret;
	ret = hmsrv_io_mgmt_unload_module(target, name);
	if (ret != 0) {
		(void)fprintf(stderr, "unload module \"%s\" failed, err=%s\n",
			      name, hmstrerror(ret));
	}
	return ret;
}

static int __parse_target_group(const char *tgtgrp, unsigned int *cnode_idx_out)
{
	FILE *fp = NULL;
	char *buf = NULL;
	size_t buflen = PATH_MAX;
	char group[ISOLATE_GROUP_NAME_LEN] = {'\0'};
	unsigned int cnode_idx;
	bool found = false;
	int ret;

	/* "-" is resered for invalid isolate group name */
	if (strcmp(tgtgrp, "-") == 0) {
		ret = E_HM_INVAL;
		goto err_out;
	}

	/* search procfs for target group name */
	fp = fopen("/proc/devmgr/devhosts", "r");
	if (fp == NULL) {
		ret = E_HM_IO;
		goto err_out;
	}

	buf = malloc(buflen);
	if (buf == NULL) {
		ret = E_HM_NOMEM;
		goto err_malloc;
	}

	while (getline(&buf, &buflen, fp) > 0) {
		ret = sscanf_s(buf, "%31s 0x%x",
			       group, sizeof(group), &cnode_idx);
		/*
		 * You must ensure that all 2 parameters are obtained successfully,
		 * otherwise the information may be incorrect.
		 */
		if (ret != 2) {
			continue;
		}
		if (strcmp(group, optarg) == 0) {
			found = true;
			break;
		}
	}

	if (found) {
		*cnode_idx_out = cnode_idx;
		ret = E_HM_OK;
	} else {
		ret = E_HM_NOENT;
	}

	free(buf);
err_malloc:
	(void)fclose(fp);
err_out:
	return ret;
}

static struct option long_options[] = {
	{"dump", 0, NULL, 'd'},
	{"sync", 0, NULL, 's'},
	{"target-devid", 1, NULL, 't'},
	{"insert", 1, NULL, 'i'},
	{"target-devhost", 1, NULL, 'a'},
	{"target-group", 1, NULL, 'g'},
	{"unload", 1, NULL, 'u'},
	{"help", 0, NULL, 'h'},
	{NULL, 0, NULL, 0}
};

static int get_devid(const char *arg, int *devid)
{
	char *tmp = NULL;
	errno = 0;

	*devid = (int)strtol(arg, &tmp, 0);
	if ((tmp[0] != '\0') || (errno != 0)) {
		(void)fprintf(stderr, "invalid devid=%s\n", arg);
		return DMCTL_ERR;
	}
	return DMCTL_OK;
}

static int get_cnode_idx(const char *arg, unsigned int *cnode_idx)
{
	char *tmp = NULL;
	errno = 0;

	*cnode_idx = (unsigned int)strtoul(arg, &tmp, 0);
	if ((tmp[0] != '\0') || (errno != 0)) {
		(void)fprintf(stderr, "invalid cnode_idx=%s\n", arg);
		return DMCTL_ERR;
	}
	return DMCTL_OK;
}

static int __parse_target(struct devmgr_mgmt_target *target, int opt, const char *tgtarg)
{
	int ret = E_HM_OK;

	switch (opt) {
	case 't':
		if (get_devid(tgtarg, &target->devid) != DMCTL_OK) {
			return E_HM_INVAL;
		}
		break;
	case 'g':
		ret = __parse_target_group(tgtarg, &target->cnode_idx);
		if (ret < 0) {
			(void)fprintf(stderr, "invalid group=%s\n", tgtarg);
		}
		break;
	case 'a':
		if (get_cnode_idx(tgtarg, &target->cnode_idx) != DMCTL_OK) {
			return E_HM_INVAL;
		}
		break;
	default:
		(void)fprintf(stderr, "invalid target option '-%c %s'\n",
			      (char)opt, tgtarg);
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

static int do_dmctl(int argc, char* argv[])
{
	/* root device by default */
	struct devmgr_mgmt_target target = DM_MGMT_ROOT_TARGET;
	/* target could only be set once */
	bool target_set = false;
	int opt = 0;
	int ret;

	while ((opt != -1) && (opt != '?')) {
		opt = getopt_long(argc, argv, DMCTL_OPTIONS, long_options, NULL);
		switch (opt) {
		case 'd':
			return dmctl_dump();
		case 's':
			return hmsrv_io_mgmt_sync();
		case 'i':
			return dmctl_insert_mod(&argv[optind], argc - optind, optarg, target);
		case 'u':
			return dmctl_unload_mod(optarg, target);

		case 't': /* fallback */
		case 'g': /* fallback */
		case 'a':
			if (target_set) {
				(void)fprintf(stderr, "duplicated target set\n");
				ret = E_HM_EXIST;
			}

			ret = __parse_target(&target, opt, optarg);
			if (ret < 0) {
				return ret;
			}

			target_set = true;
			break;

		case 'h':
			show_usage(argv[0]);
			return 0;

		default:
			show_usage(argv[0]);
			return 1;
		}
	}

	return 0;
}

int dmctl_main(int argc, char* argv[])
{
	int ret;
	GETOPT_RESET();

	if ((argc < 1) || (argv[0] == NULL)) {
		return E_HM_INVAL;
	}

	ret = hmsrv_io_init();
	if (ret < 0) {
		(void)fprintf(stderr, "connect devmgr failed, ret=%s\n",
			      hmstrerror(ret));
		return ret;
	}

	/* for insmod: use file path to load ko */
	g_internal_mod = false;
	if (dmctl_handle_kmod_alias(argc, argv, &ret) == KMOD_DONE) {
		return ret;
	}

	/* for dmctl: use file name located at modbase-dir to load ko */
	g_internal_mod = true;
	ret = do_dmctl(argc, argv);

	return ret;
}
