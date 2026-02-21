/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022.
 * Description: Modules management utils
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 18 11:33:25 2022
 */

#include "kmod.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fsuid.h>
#include <sys/prctl.h>
#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libhmsrv_io/mgmt.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhwsecurec/securec.h>
#include "dmctl.h"

#define MOD_OPTIONS "hV"
#define BUF_SIZE	2048
#define INFO_SIZE	512
#define DMCTL_VERSION "1.0"

typedef int (*main_func)(int argc, char *argv[]);

struct sub_fun_info {
	const char *name;
	main_func sub_entry;
};

struct mod_info {
	char name[INFO_SIZE];
	int size;
	int used;
	char by[INFO_SIZE];
};

static struct option long_options[] = {
	{"version", 0, NULL, 'V'},
	{"help", 0, NULL, 'h'},
	{NULL, 0, NULL, 0}
};

static void insmod_show_usage(const char *name)
{
	(void)printf("HongMeng insmod tool\n");
	(void)printf("Usage: %s [OPTION] filename [ARGUMENT]\n", name);
	(void)printf("OPTIONS:\n"
	       "  -V, --version		         show version\n"
	       "  -h, --help                     show help information\n\n");
}

static void mod_show_ver(const char *name)
{
	(void)printf("HongMeng %s version\n", name);
	(void)printf("dmctl v%s\n", DMCTL_VERSION);
}

static int insmod_main(int argc, char *argv[])
{
	int opt = 0;
	int ret = KMOD_NONE;

	while ((opt != -1) && (opt != '?') && (ret == KMOD_NONE)) {
		opt = getopt_long(argc, argv, MOD_OPTIONS, long_options, NULL);
		switch (opt) {
		case 'h':
			insmod_show_usage(argv[0]);
			ret = KMOD_OK;
			break;
		case 'V':
			mod_show_ver(argv[0]);
			ret = KMOD_OK;
			break;
		default:
			if (argc > 1) {
				/* insmod module info start with index 2 */
				ret = dmctl_insert_mod(&argv[2], argc - 2, argv[optind],
						       DM_MGMT_ROOT_TARGET);
			} else {
				insmod_show_usage(argv[0]);
				ret = KMOD_DONE;
			}
			break;
		}
	}
	return ret;
}

static int mod_parse(int fd, int *ret)
{
	char buf[BUF_SIZE] = {0};
	struct mod_info info = {0};
	int index = 0;
	int fields;
	int rd_size;
	int by_len;
	char *permanent = NULL;

	while (index < BUF_SIZE) {
		rd_size = (int)read(fd, buf + index, 1);
		if (rd_size == 0) {
			*ret = KMOD_OK;
			return KMOD_DONE;
		}
		if (buf[index] == '\n') {
			break;
		}
		index++;
	}

	fields = sscanf_s(buf, "%s %d %d %s", info.name, INFO_SIZE - 1,
			&info.size, &info.used, info.by, INFO_SIZE - 1);
	/*
	 * You must ensure that all 4 parameters are obtained successfully,
	 * otherwise the information may be incorrect.
	 */
	if (fields < 4) {
		*ret = E_HM_INVAL;
		return KMOD_DONE;
	}

	permanent = strstr(info.by, "[permanent]");
	if (permanent != NULL) {
		*permanent = '\0';
	}

	by_len = (int)strlen(info.by);
	if ((by_len > 0) && (by_len < INFO_SIZE)) {
		if (info.by[by_len - 1] == ',') {
			info.by[by_len - 1] = '\0';
		}
	}

	if ((info.by[0] != '-') && (info.used != 0)) {
		(void)printf("%-19s %8d %2d %s\n", info.name, info.size, info.used, info.by);
	} else {
		(void)printf("%-19s %8d %2d\n", info.name, info.size, info.used);
	}
	return KMOD_OK;
}

static int lsmod_main(int argc, char *argv[])
{
	int fd = -1;
	int ret = KMOD_OK;
	(void)argc;
	(void)argv;

	fd = open("/proc/devhost/root/modules", O_RDONLY);
	if (fd < 0) {
		(void)fprintf(stderr, "open modules file failed! errno = %d\n", fd);
		return E_HM_INVAL;
	}
	(void)printf("%-24sSize  Used by\n", "Module");
	while (mod_parse(fd, &ret) == KMOD_OK) {
		if (ret != KMOD_OK) {
			break;
		}
	}
	(void)printf("\n");
	(void)close(fd);
	return ret;
}

static void rmmod_show_usage(const char *name)
{
	(void)printf("HongMeng rmmod tool\n");
	(void)printf("Usage: %s [OPTION] modulename ...\n", name);
	(void)printf("OPTIONS:\n"
	       "  -V, --version		         show version\n"
	       "  -h, --help                     show help information\n\n");
}

static int rmmod_main(int argc, char *argv[])
{
	int opt = 0;
	int ret = KMOD_NONE;

	while ((opt != -1) && (opt != '?') && (ret == KMOD_NONE)) {
		opt = getopt_long(argc, argv, MOD_OPTIONS, long_options, NULL);
		switch (opt) {
		case 'h':
			rmmod_show_usage(argv[0]);
			ret = KMOD_OK;
			break;
		case 'V':
			mod_show_ver(argv[0]);
			ret = KMOD_OK;
			break;
		default:
			if (argc > 1) {
				return dmctl_unload_mod(argv[1], DM_MGMT_ROOT_TARGET);
			} else {
				rmmod_show_usage(argv[0]);
				ret = KMOD_DONE;
			}
			break;
		}
	}

	return ret;
}

static struct sub_fun_info sub_info[] = {
	{"insmod",      insmod_main},
	{"lsmod",       lsmod_main},
	{"rmmod",       rmmod_main},
};

int dmctl_handle_kmod_alias(int argc, char *argv[], int *ret)
{
	unsigned int i;
	char *sub_name = NULL;

	sub_name = basename(argv[0]);
	for (i = 0; i < sizeof(sub_info) / sizeof(struct sub_fun_info); i++) {
		if (strcmp(sub_info[i].name, sub_name) == 0) {
			*ret = sub_info[i].sub_entry(argc, argv);
			return KMOD_DONE;
		}
	}
	return KMOD_NONE;
}
