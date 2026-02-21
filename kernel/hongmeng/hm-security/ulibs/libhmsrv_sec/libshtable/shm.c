/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Shared hash table share memory functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:50:11 2019
 */
#include "shm.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>

#include "libshtable/shtable.h"

static uint32_t shm_size(const struct shtable *sht, size_t element_size)
{
	uint32_t size;
	uint32_t align_up_size = 0;
	int rc = E_HM_OK;

	if (element_size == 0U) {
		hm_error("element size is invalid\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (sht->size > UINT32_MAX / element_size) {
			hm_error("share memory size is invalid\n");
			rc = E_HM_INVAL;
		}
	}
	if (rc == E_HM_OK) {
		size = (uint32_t)(element_size * sht->size);
		align_up_size = PAGE_ALIGN_UP(size);
		if (align_up_size < size) {
			hm_error("share memory size is invalid\n");
			align_up_size = 0;
		}
	}
	return align_up_size;
}

static void shm_close_and_unlink(struct memmgr_shm_name name, bool is_creator,
				 int shm_id)
{
	CERTAIN_NOFAIL(hm_mem_shm_close(shm_id));
	if (is_creator) {
		CERTAIN_NOFAIL(hm_mem_shm_unlink((const char *)name.path));
	}
}

static int shm_create_and_open(struct memmgr_shm_name name, bool is_creator,
			       size_t size, int *shm_id)
{
	int id;
	uint64_t oflags = 0U;
	int rc = E_HM_OK;

	if (is_creator) {
		oflags |= (uint64_t)O_CREAT | (uint64_t)O_EXCL | (uint64_t)O_RDONLY;
	}
	id = hm_mem_shm_open_with_mode((const char *)name.path, size, oflags,
			(unsigned)S_IRUSR | (unsigned)S_IRGRP | (unsigned)S_IROTH, NULL);
	if (id < 0) {
		hm_error("failed to open shared memory, err=%s\n", hmstrerror(id));
		rc = E_HM_INVAL;
	} else {
		*shm_id = id;
	}

	return rc;
}

static int shm_name(const char *prefix, const char *name,
		    struct memmgr_shm_name *n)
{
	size_t prefix_size = 0;
	size_t name_size;
	int rc = E_HM_OK;

	if (prefix != NULL) {
		prefix_size = strlen(prefix);
		if (prefix_size > 0U) {
			if (prefix_size >= sizeof(n->path) ||
			    strncpy_s(n->path, sizeof(n->path), prefix,
				      prefix_size) != 0) {
				rc = E_HM_INVAL;
				hm_error("share memory name copy failed\n");
			}
		}
	}

	if (rc == E_HM_OK) {
		name_size = strlen(name);
		if (name_size >= sizeof(n->path) - prefix_size ||
		    strncpy_s(n->path + prefix_size, sizeof(n->path) - prefix_size,
			      name, name_size) != 0) {
			hm_error("share memory name copy failed\n");
			rc = E_HM_INVAL;
		}
	}

	return rc;
}

int sht_shm_open(const struct shtable *sht, struct sht_shm *shm,
		 const char *name, size_t element_size)
{
	struct memmgr_shm_name n;
	int shm_id;
	uint32_t size;
	bool is_creator = false;
	void *addr = NULL;
	int rc = E_HM_INVAL;

	if (sht == NULL || shm == NULL || name == NULL) {
		goto exit;
	}
	is_creator = sht->ctrl != NULL;

	size = shm_size(sht, element_size);
	if (size == 0U) {
		goto exit;
	}
	rc = shm_name(sht->prefix, name, &n);
	if (rc != E_HM_OK) {
		goto exit;
	}
	rc = shm_create_and_open(n, is_creator, size, &shm_id);
	if (rc != E_HM_OK) {
		goto exit;
	}
	addr = hm_mem_mmap(NULL, size, is_creator ? ((uint32_t)PROT_READ | (uint32_t)PROT_WRITE) : (uint32_t)PROT_READ,
			   (uint32_t)(MAP_SHARED) | (uint32_t)(MAP_ANONYMOUS), shm_id, 0);
	if (addr == MAP_FAILED) {
		hm_error("failed to map share memory\n");
		rc = E_HM_NOMEM;
		goto close_shm;
	}

	shm->name = strdup(n.path);
	if (shm->name == NULL) {
		hm_error("failed to duplicate the string\n");
		rc = E_HM_NOMEM;
		goto unmap;
	}
	shm->size = size;
	shm->base = ptr_to_type(addr, uintptr_t);

	(void)hm_mem_shm_close(shm_id);
	goto exit;
unmap:
	(void)hm_mem_munmap(addr, size);
close_shm:
	shm_close_and_unlink(n, is_creator, shm_id);
exit:
	return rc;
}

void sht_shm_close(const struct shtable *sht, struct sht_shm *shm)
{
	bool is_creator = false;

	if (sht != NULL && shm != NULL && shm->name != NULL) {
		is_creator = sht->ctrl != NULL;

		CERTAIN_NOFAIL(hm_mem_munmap(ptr_to_void(shm->base), shm->size));
		if (is_creator) {
			CERTAIN_NOFAIL(hm_mem_shm_unlink(shm->name));
		}
		free((void *)shm->name);
		shm->name = NULL;
	}
}
