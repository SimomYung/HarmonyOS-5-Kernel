/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Sysfs function of devmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 23 10:12:08 2021
 */

#include <string.h>
#include <libsysif/devmgr/api.h>

bool __devmgr_type_is_valid_name(const char *name, unsigned int maxlen)
{
	return !((name == NULL) || (name[0] == '\0') ||
		 (strnlen(name, maxlen) == (size_t)maxlen));
}

bool __devmgr_type_is_valid_filename(const char *name, unsigned int maxlen)
{
	/* module name should not contains '/' */
	return (__devmgr_type_is_valid_name(name, maxlen) &&
		(strchr(name, '/') == NULL));
}
