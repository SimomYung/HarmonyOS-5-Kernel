/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: keymgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 24 20:22:52 2022
 */
#ifndef LIBKEYMGR_INIT_H
#define LIBKEYMGR_INIT_H

#include <stdbool.h>
#include <stddef.h>

// libkeymgr config flags
#define LIBKEYMGR_ACTV_AND_LSYSCALL	(unsigned int)0x1 // enable keymgr self activation and lsyscall registration
#define LIBKEYMGR_SECURITY		(unsigned int)0x2 // enable keymgr access control
#define LIBKEYMGR_MAIN_THREAD		(unsigned int)0x4 // enable keymgr main thread's notify and suspend

// independent keymgr kernel service
#define LIBKEYMGR_SERVICE	(LIBKEYMGR_ACTV_AND_LSYSCALL | LIBKEYMGR_SECURITY | LIBKEYMGR_MAIN_THREAD)
// libkeymgr configuration for sysmgr
#define LIBKEYMGR_SYSMGR	(LIBKEYMGR_ACTV_AND_LSYSCALL | LIBKEYMGR_SECURITY)

// init function for libkeymgr init
typedef int (*lib_init_func_t)(void);
#define LIB_INIT_FUNC_NUM 4

// actvpool init function
typedef int (*actv_init_func_t)(void);

typedef int (*get_random_func_t)(unsigned char *out, size_t size);

struct libkeymgr_config {
	// config libkeymgr flags, just like LIBKEYMGR_SERVICE
	unsigned int flags;
	/*
	 * for example: {func1, func2, NULL, NULL}
	 * these init function will be used in libkeymgr_init
	 */
	lib_init_func_t lib_init_func[LIB_INIT_FUNC_NUM];
	int lib_init_func_num;
	// for LIBKEYMGR_ACTV_AND_LSYSCALL
	actv_init_func_t actv_init_func;

	// get random number function
	get_random_func_t get_rnd_func;

	// The scheduling policy of the garbage collection thread is affected.
	int schedule_policy;
};

static inline bool contains_flags(struct libkeymgr_config *config, unsigned int flags)
{
	return (config != NULL && (config->flags & flags) == flags);
}

int libkeymgr_init(struct libkeymgr_config *config);

#endif // LIBKEYMGR_INIT_H
