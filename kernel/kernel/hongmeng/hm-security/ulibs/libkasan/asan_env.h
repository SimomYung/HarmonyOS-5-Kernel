/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Asan options header
  Author: Huawei OS Kernel Lab
 * Create: Wed Dec 01 10:58:57 2021
 */

#ifndef ASAN_ENV_H
#define ASAN_ENV_H

// file path length
#define DEFAULT_PATH_SIZE ((unsigned int)(4 * 1024))
// uint64_t value string max len
#define PID_STR_MAX_LEN ((unsigned int)20)

/*
 * How to add new asan option:
 *  1) add new index in "enum asan_option_index" struct
 *  2) add new option struct and handle function in asan_options_def.c
 */
#define INDEX_MIN 0
// normal option
#define LOG_PATH 1
#define HALT_ON_ERROR 2
// add new option here
#define INDEX_MAX 3

/*
 * if success, return 0
 * otherwise, return -1
 */
typedef int option_check_ft(void);
typedef int option_prepare_ft(void);

struct asan_option_s {
	int index;
	// Name of asan option
	char *prefix;
	// Valid asan option content, all memory is from asan_env
	char *str;
	// Check whether the environment variable meets the requirements of the option.
	option_check_ft *check;
	/*
	 * Preparations before using the option, just like open file.
	 * It should be called after check function.
	 */
	option_prepare_ft *prepare;
};

struct asan_env_s {
	char *env;
	// One-to-one mapping between option and asan option index
	struct asan_option_s *options[INDEX_MAX];
	// Output log file full path
	char full_path[DEFAULT_PATH_SIZE];
	// Output log file id
	int log_file_fd;
};

/* get unique asan_env_s instance */
struct asan_env_s *asan_env_gs(void);

/* register and unregister options */
void asan_env_register(struct asan_option_s *option);
void asan_env_unregister(int index);
void register_all_asan_opts(void);

/* init and fini kasan environment and options */
int asan_env_init(void);
void asan_env_fini(void);

#endif
