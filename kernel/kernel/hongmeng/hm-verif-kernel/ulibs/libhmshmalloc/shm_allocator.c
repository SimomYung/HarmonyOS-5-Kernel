/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Share memory allocator
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 19 18:19:32 2019
 */
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>

#include <libhmlog/hmlog.h>
#include <libmem/types.h>
#include <libhwsecurec/securec.h>
#include <libhmshmalloc/shm_allocator.h>
#include <libstrict/overflow.h>

#define SHARE_MEM_VERSION_MAJOR	(uint16_t)1
#define SHARE_MEM_VERSION_MINOR	(uint16_t)0
#define SHARE_MEM_VERSION_PATCH	(uint16_t)0
#define SHARE_MEM_VERSION_BUILD	(uint16_t)0

#define  SHARE_MEM_SIZE	        ((unsigned int)1 << CONFIG_SHMALLOC_MEM_BITS)

#define  SHARE_MEM_MAGIC        0x5a5a5a5a

#define  SHARE_MEM_NO_UNIT	0U

struct share_mem_ctrl_info {
	struct raw_mutex lock;
	uint32_t magic_word;
	uint32_t head_len;
	uint32_t unit_num;
	uint32_t unit_free;
	size_t total_len;
	size_t user_len;
	size_t unit_len;
	DEFINE_RAW_BITMAP(bitmap, SHARE_MEM_SIZE);
};

void shmalloc_version_get(struct shm_version *version)
{
	if (version != NULL) {
		version->major = SHARE_MEM_VERSION_MAJOR;
		version->minor = SHARE_MEM_VERSION_MINOR;
		version->patch = SHARE_MEM_VERSION_PATCH;
		version->build = SHARE_MEM_VERSION_BUILD;
	}
}

int shmalloc_version_check(const struct shm_version *expected)
{
	int rc = E_HM_OK;

	if (expected == NULL) {
		rc = E_HM_INVAL;
	}

	if ((rc == E_HM_OK) && (expected->major > SHARE_MEM_VERSION_MAJOR)) {
		rc = E_HM_NOSYS;
	}

	if ((rc == E_HM_OK) && (expected->minor > SHARE_MEM_VERSION_MINOR)) {
		rc = E_HM_NOSYS;
	}

	return rc;
}

unsigned int
shmalloc_get_unit_free_num(struct share_mem_ctrl_info *pmem_ctrl_info)
{
	unsigned int ret = SHARE_MEM_NO_UNIT;

	if (pmem_ctrl_info != NULL) {
		RAW_MUTEX_GUARD(_, &pmem_ctrl_info->lock);
		ret = pmem_ctrl_info->unit_free;
	} else {
		hm_error("pmem_ctrl_info is null\n");
	}

	return ret;
}

struct share_mem_ctrl_info *
shmalloc_init(void *buffer, size_t total_len, size_t unit_size)
{
	const size_t head_len = SHARE_MEM_HEAD_LEN;
	unsigned int unit_num = 0U;
	size_t align_len = ALIGN_UP(total_len, PAGE_SIZE);
	size_t load_len = 0;
	struct share_mem_ctrl_info *pmem_ctrl_info = NULL;
	int rc = E_HM_OK;

	if (buffer == NULL) {
		hm_error("buffer is null\n");
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && (unit_size == 0UL)) {
		hm_error("unit_size is zero\n");
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && (align_len != total_len)) {
		hm_error("total_len is not aligned. total_len:0x%zx\n", total_len);
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		unit_num = (unsigned int)((align_len - head_len) / unit_size);
		if (unit_num > SHARE_MEM_SIZE) {
			hm_error("unit_num is larger than SHARE_MEM_SIZE. unit_num:0x%x\n", unit_num);
			rc = E_HM_INVAL;
		}
	}
	if (rc == E_HM_OK) {
		pmem_ctrl_info = (struct share_mem_ctrl_info *)buffer;
		mem_zero_s(*pmem_ctrl_info);
		raw_mutex_init_shared(&pmem_ctrl_info->lock);

		noovfl_umul(load_len, unit_num, unit_size,
			    "load_len is out of limit of long long\n");
		noovfl_uadd(pmem_ctrl_info->user_len, load_len, head_len,
			    "user_len is out of limit of long long\n");

		pmem_ctrl_info->magic_word = SHARE_MEM_MAGIC;
		pmem_ctrl_info->total_len = align_len;
		pmem_ctrl_info->head_len = (uint32_t)head_len;
		pmem_ctrl_info->unit_len = unit_size;
		pmem_ctrl_info->unit_num = unit_num;
		pmem_ctrl_info->unit_free = unit_num;

		hm_debug("cell pool create success.align_len:0x%zx buffer:%p unit_size:0x%zx unitnum:0x%x\n",
			 align_len, buffer, unit_size, unit_num);
	}

	return pmem_ctrl_info;
}

int
shmalloc_unit_alloc(struct share_mem_ctrl_info *pmem_ctrl_info,
		    uintptr_t *va)
{
	unsigned int id;
	size_t offset = 0UL;
	size_t load_len = 0UL;
	size_t head_len = 0UL;
	int rc = E_HM_OK;

	if ((va != NULL) && (pmem_ctrl_info != NULL)) {
		RAW_MUTEX_GUARD(_, &pmem_ctrl_info->lock);
		head_len = (size_t)(pmem_ctrl_info->head_len);
		id = raw_bitmap_find_first_zero(pmem_ctrl_info->bitmap, pmem_ctrl_info->unit_num);
		rc = raw_bitmap_set_bit(pmem_ctrl_info->bitmap, pmem_ctrl_info->unit_num, id);
		if (rc < 0) {
			rc = E_HM_NOMEM;
		} else {
			noovfl_umul(load_len, id, pmem_ctrl_info->unit_len,
				    "load_len is out of limit of long long\n");
			noovfl_uadd(offset, load_len, head_len,
				    "offset is out of limit of long long\n");
			*va = (uintptr_t)pmem_ctrl_info + offset;
			pmem_ctrl_info->unit_free--;
		}
	} else {
		hm_error("va or pmem_ctrl_info is null\n");
		rc = E_HM_INVAL;
	}

	return rc;
}

int
shmalloc_unit_free(struct share_mem_ctrl_info *pmem_ctrl_info,
		   unsigned int offset)
{
	unsigned int pay_offset;
	int rc = E_HM_OK;

	if (pmem_ctrl_info == NULL) {
		hm_error("pmem_ctrl_info is null\n");
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) &&
	    ((offset > (pmem_ctrl_info->user_len - pmem_ctrl_info->unit_len)) ||
	     (offset < (unsigned int)SHARE_MEM_HEAD_LEN) ||
	     ((((size_t)offset - pmem_ctrl_info->head_len) % pmem_ctrl_info->unit_len) != 0UL))) {
		hm_error("offset is invalid offset:0x%x pmem_ctrl_info->total_len:0x%zx\n",
			 offset, pmem_ctrl_info->total_len - pmem_ctrl_info->unit_len);
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		pay_offset = (offset - pmem_ctrl_info->head_len) / (uint32_t)pmem_ctrl_info->unit_len;

		RAW_MUTEX_GUARD(_, &pmem_ctrl_info->lock);

		if (raw_bitmap_test_bit(pmem_ctrl_info->bitmap, pmem_ctrl_info->unit_num, pay_offset) == 0) {
			hm_error("double free, offset:0x%x \n", offset);
			rc = E_HM_INVAL;
		} else {
			rc = raw_bitmap_clear_bit(pmem_ctrl_info->bitmap, pmem_ctrl_info->unit_num, pay_offset);
			pmem_ctrl_info->unit_free++;
		}
	}

	return rc;
}
