/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Interfaces of lockdep
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 22 11:47:35 2023
 */
#ifndef ULIBS_LIBHMSYNC_LOCKDEP_H
#define ULIBS_LIBHMSYNC_LOCKDEP_H

struct lockdep_lock_class_key_s {
	/* interface for subclass */
	unsigned int subkey;
};

struct lockdep_lock_class_s {
	struct lockdep_lock_class_key_s *key;
	const char *name;
};

struct lockdep_map_s {
	struct lockdep_lock_class_key_s *key;
};

int lockdep_fill_lockstat(char *data, size_t data_len);
void lockdep_clear_lockstat(void);
int lockdep_fill_lockstat_status(char *data, size_t data_len);
void lockdep_set_lockstat_status(unsigned long status);

#endif
