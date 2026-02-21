/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 16 09:44:43 2019
 */
#define _GNU_SOURCE
#include <errno.h>
#include <stdlib.h>

#include "pthread_impl.h"

int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize,
				const struct cpu_set_t *cpuset)
{
	int rc = 0;

	if (cpusetsize == 0 || cpuset == NULL) {
		free((void *)(uintptr_t)attr->_a_cpuset);
		attr->_a_cpuset = 0;
		attr->_a_cpusetsize = 0;
	} else {
		if (attr->_a_cpusetsize != cpusetsize) {
			cpu_set_t *new_cpuset = NULL;
			new_cpuset = (cpu_set_t *)malloc(cpusetsize);
			if (new_cpuset == NULL) {
				rc = ENOMEM;
			} else {
				free((void *)(uintptr_t)attr->_a_cpuset);
				attr->_a_cpuset = (uintptr_t)new_cpuset;
				attr->_a_cpusetsize = cpusetsize;
			}
		}
		if (rc == 0) {
			void *ptr = memcpy((void *)(uintptr_t)attr->_a_cpuset,
					   (const void *)cpuset, cpusetsize);
			if (ptr == NULL) {
				rc =  EFAULT;
			}
		}
	}

	return rc;
}
