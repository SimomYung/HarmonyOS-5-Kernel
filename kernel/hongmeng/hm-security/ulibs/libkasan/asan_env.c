/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: "ASAN_OPTIONS" environment variable handling function.
 *		 And all options init, check and preapre operation will be done in this file.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 07 14:17:21 2022
 */

// libc
#include <stdlib.h>

// hongmeng
#include <vfs.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

// security kasan
#include "macro.h"
#include "shadow.h"
#include "asan_test.h"
#include "asan_env.h"

// unique asan environment
#define HM_ASAN_ENV "HM_ASAN_OPTIONS"

/* Notice: log file id should be -1, this is for sysmgr and uapps */
#define INVALID_FILE_ID (-1)
INNER struct asan_env_s default_env = {NULL, {0}, {0}, INVALID_FILE_ID};

struct asan_env_s *asan_env_gs(void)
{
	return &default_env;
}

void asan_env_register(struct asan_option_s *option)
{
	if (option != NULL && option->index > INDEX_MIN && option->index < INDEX_MAX) {
		default_env.options[option->index] = option;
	}
}

void asan_env_unregister(int index)
{
	if (index > INDEX_MIN && index < INDEX_MAX) {
		default_env.options[index] = NULL;
	}
}

INNER void check_all_asan_opts(void)
{
	int i = INDEX_MIN;
	for(i += 1; i < INDEX_MAX && default_env.options[i] != NULL; i++) {
		if (default_env.options[i]->check == NULL ||
		    default_env.options[i]->check() < 0) {
			hm_debug("asan option[%s] is invalid\n", default_env.options[i]->prefix);
			asan_env_unregister(i);
		}
	}
}

// use after check_all_asan_opts
INNER void show_all_valid_opts(void)
{
	int i = INDEX_MIN;
	for(i += 1; i < INDEX_MAX && default_env.options[i] != NULL; i++) {
		hm_debug("option[%d]: %s=%s\n", (int)i, default_env.options[i]->prefix,
			 default_env.options[i]->str);
	}
}

INNER int prepare_all_valid_opts(void)
{
	int i = INDEX_MIN;
	for(i += 1; i < INDEX_MAX && default_env.options[i] != NULL; i++) {
		if (default_env.options[i]->prepare != NULL && default_env.options[i]->prepare() < 0) {
			/*
			 * If the preparation fails, the program will be terminated.
			 * For example, open file failed.
			 */
			hm_error("failed to prepare the asan options\n");
			return E_HM_INVAL;
		}
	}
	return E_HM_OK;
}

// success, return 0; fail, return -1
// Notice: token is from default_env.env
INNER int asan_option_init_by(char *token, struct asan_option_s *option)
{
	int ret = -1;
	if (option == NULL || token == NULL) {
		hm_debug("parameter is NULL\n");
		return ret;
	}

	// this option is not be initialized
	size_t prefix_size = strlen(option->prefix);
	size_t token_size = strlen(token);
	if (token_size <= prefix_size) {
		hm_debug("token is too small\n");
		return ret;
	}

	if (strncmp(option->prefix, token, prefix_size) == 0 && *(token + prefix_size) == '=') {
		option->str = (token + prefix_size) + 1;
		hm_debug("asan options: %s\n", option->str);
		ret = 0;
	} else {
		hm_debug("%s init failed\n", option->prefix);
	}
	return ret;
}

INNER int init_all_asan_opts(void)
{
	char *env = getenv(HM_ASAN_ENV);
	if (env == NULL) {
		hm_debug("Do not have %s environment.\n", HM_ASAN_ENV);
		return 0;
	}

	default_env.env = strdup(env);
	if (default_env.env == NULL) {
		hm_warn("do not have enough memory to dup kasan environment\n");
		return -1;
	}
	hm_debug("asan env: %s\n", default_env.env);

	char *token = strtok(default_env.env, ":");
	if (token == NULL) {
		hm_debug("Do not have options.\n");
		return 0;
	}

	// Init all options which has registered to @default_env
	while (token != NULL) {
		int index;
		// visit all options
		index = INDEX_MIN;
		for (index += 1; index < INDEX_MAX; index++) {
			if (default_env.options[index] == NULL || default_env.options[index]->str != NULL) {
				// options is empty or option is initialized
				continue;
			}
			if (asan_option_init_by(token, default_env.options[index]) == 0) {
				// use this token, and other options do not need to compare with it
				break;
			}
		}
		token = strtok(NULL, ":");
	}
	return 0;
}

int asan_env_init(void)
{
	int ret = 0;

	// register and init all asan options in asan_options_def.c
	hm_debug("register_all_asan_opts start\n");
	register_all_asan_opts();

	hm_debug("init_all_asan_opts start\n");
	ret = init_all_asan_opts();
	if (ret < 0) {
		return ret;
	}

	// Check whether the options are normal. If option is invalid, unregister it.
	hm_debug("check_all_asan_opts start\n");
	check_all_asan_opts();
	show_all_valid_opts();

	hm_debug("prepare_all_valid_opts start\n");
	// prepare operation before asan env is used
	ret = prepare_all_valid_opts();

	return ret;
}

void asan_env_fini(void)
{
	free(default_env.env);
	default_env.env = NULL;
	for (int i = INDEX_MIN; i < INDEX_MAX; i++) {
		if (default_env.options[i] != NULL) {
			default_env.options[i]->str = NULL;
		}
	}

	if (default_env.log_file_fd != INVALID_FILE_ID) {
		hm_debug("close kasan log file, fd is %d\n", default_env.log_file_fd);
		if (vfs_close(default_env.log_file_fd) != 0) {
			hm_debug("close kasan log file failed\n");
		}
	}
}
