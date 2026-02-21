/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide operations for exec file
 * Author: Huawei OS Kernel Lab
 * Create: Wed 11 Dec 2019 11:41:08 AM CST
 */
#include <vfs.h>
#include <libhmelf/elf.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libsecure/bounds_check.h>

#include "elf_ex.h"

struct elf_ex_context {
	int fd;
	size_t size;
	void *data;
};

static int get_file_size(int fd, size_t *size)
{
	struct stat st;
	int err = E_HM_OK;

	err = vfs_fstat(fd, &st);
	if (err != E_HM_OK) {
		hm_warn("stat elf file failed: %s\n", hmstrerror(err));
	} else if (st.st_size == 0) {
		hm_warn("elf file size is 0\n");
		err = E_HM_NOEXEC;
	} else {
		*size = (size_t)st.st_size;
	}

	return err;
}

static void close_file(int fd)
{
	int err;
	err = vfs_close(fd);
	if (err != E_HM_OK) {
		hm_warn("close elf file failed: %s\n", hmstrerror(err));
	}
}

/* octal value 0444, MISRA rule 7.1 suggests octal constants shall not be used */
#define READ_MODE 0x124

static struct elf_ex_context *alloc_ex_context(void *vaddr, size_t file_size, int fd)
{
	struct elf_ex_context *ex_ctx = malloc(sizeof(struct elf_ex_context));
	if (ex_ctx != NULL) {
		ex_ctx->data = vaddr;
		ex_ctx->size = file_size;
		ex_ctx->fd = fd;
	}
	return ex_ctx;
}

static int elf_ex_open(struct elf_context *elf_ctx, const char *elf_file)
{
	int fd = -1;
	int err = E_HM_OK;
	void *vaddr = NULL;
	size_t file_size = (size_t)0;

	if (elf_ctx == NULL || elf_file == NULL) {
		hm_warn("arg is NULL\n");
		err = E_HM_INVAL;
	} else {
		fd = vfs_open(elf_file, O_RDONLY, READ_MODE);
		if (fd < 0) {
			err = fd;
			hm_warn("open elf file:%s failed, err=%s\n", elf_file, hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = get_file_size(fd, &file_size);
		if (err != E_HM_OK) {
			hm_warn("get elf file '%s' size failed\n", elf_file);
			close_file(fd);
		}
	}
	if (err == E_HM_OK) {
		vaddr = hm_mem_mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
		if (vaddr == MAP_FAILED) {
			hm_warn("hm_mem_mmap failed\n");
			close_file(fd);
			err = E_HM_NOMEM;
		}
	}
	if (err == E_HM_OK) {
		struct elf_ex_context *ex_ctx = alloc_ex_context(vaddr, file_size, fd);
		if (ex_ctx == NULL) {
			hm_warn("malloc elf ex context failed\n");
			int rc = hm_mem_munmap(vaddr, file_size);
			if (rc != E_HM_OK) {
				hm_warn("mem unmap failed %s\n", hmstrerror(rc));
			}
			close_file(fd);
			err = E_HM_NOMEM;
		} else {
			elf_ctx->private_data = (void *)ex_ctx;
		}
	}
	return err;
}

static ssize_t elf_ex_read(struct elf_context *elf_ctx, void *data,
			   off_t offset, size_t length)
{
	size_t diff;
	size_t size = (size_t)0;
	int rc = E_HM_OK;
	struct elf_ex_context *ex_ctx = NULL;

	if (elf_ctx == NULL) {
		hm_warn("elf_context is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && offset < 0) {
		hm_warn("read offset less than zero\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && data == NULL) {
		hm_warn("data is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		ex_ctx = (struct elf_ex_context *)elf_ctx->private_data;
		BUG_ON(ex_ctx == NULL);
		if (ex_ctx->data == NULL) {
			hm_warn("elf data is NULL\n");
			rc = E_HM_INVAL;
		} else if (ex_ctx->size == 0U) {
			hm_warn("elf size is invalid\n");
			rc = E_HM_INVAL;
		} else {
			/* do nothings */
		}
	}
	/* overflow */
	if (rc == E_HM_OK && (size_t)offset >= ex_ctx->size) {
		rc = E_HM_OVERFLOW;
	}
	if (rc == E_HM_OK) {
		diff = (size_t)(ex_ctx->size - (unsigned long long)offset);
		size = length > diff ? diff : length;
		offset = (off_t)array_index_fixup((size_t)offset, ex_ctx->size);
		NOFAIL(memcpy_s(data, length, ptr_add(ex_ctx->data, offset), size));
	}

	return ((rc == E_HM_OK) ? (ssize_t)size : (ssize_t)rc);
}

static int elf_ex_close(struct elf_context *elf_ctx)
{
	int err = E_HM_OK;
	struct elf_ex_context *ex_ctx = NULL;

	if (elf_ctx == NULL) {
		hm_warn("elf_context is NULL\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		ex_ctx = (struct elf_ex_context *)elf_ctx->private_data;
		err = hm_mem_munmap(ex_ctx->data, ex_ctx->size);
		if (err != E_HM_OK) {
			hm_warn("unmap shm failed: %s\n", hmstrerror(err));
		}
		close_file(ex_ctx->fd);
		free(elf_ctx->private_data);
		elf_ctx->private_data = NULL;
	}

	return err;
}

static int elf_ex_filesz(struct elf_context *elf_ctx, size_t *size)
{
	struct elf_ex_context *ex_ctx = NULL;
	int rc = E_HM_OK;

	if (elf_ctx == NULL) {
		hm_warn("elf_context is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && size == NULL) {
		hm_warn("size addr is NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		ex_ctx = (struct elf_ex_context *)elf_ctx->private_data;
		if (ex_ctx->size == 0U) {
			hm_warn("invalid file size\n");
			rc = E_HM_INVAL;
		} else {
			*size = ex_ctx->size;
		}
	}

	return rc;
}

struct elf_fops elf_ex_fops = {
	.open	= elf_ex_open,
	.read	= elf_ex_read,
	.write	= NULL,
	.close	= elf_ex_close,
	.lseek	= NULL,
	.filesz = elf_ex_filesz,
	.get_load_start = NULL,
};
