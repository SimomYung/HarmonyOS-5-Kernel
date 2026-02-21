/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of fs malloc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 05 12:28:08 2020
 */

#include <utils/fs_malloc.h>

#include <string.h>
#include <malloc.h>

#include <vfs.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(VFSCORE);

/* caller should handler E_NOMEM if size > 0 and return NULL */
void *fs_zmalloc(size_t size)
{
	void *buf = NULL;
	int ret;

	if (size > 0) {
		buf = malloc(size);
		if (buf != NULL) {
			ret = memset_s(buf, size, 0, size);
			BUG_ON(ret != 0, "memset_s failed errno(%d), size(%zu)\n", ret, size);
		}
	}

	return buf;
}

/* no need to check whether buf is NULL in caller func */
void fs_free(void *buf)
{
	if (buf != NULL) {
		free(buf);
	}
}
