/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Kbox device definition
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 11 14:55:32 2019
 */

#ifndef ULIBS_LIBKBOX_DEVICE_H
#define ULIBS_LIBKBOX_DEVICE_H

#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <libkbox/param.h>
#include <libhmsync/atomic.h>
#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>
#include <hmkernel/const.h>

/*
 * kbox device structure:
 *
 * +------+-------------+------------+--------+------------+
 * | head |  region  0  | region  1  |  ....  | region  n  |
 * +------+-------------+------------+--------+------------+
 *
 * head and region sizes must be aligned to 4KB.
 */
struct kbox_device_head {
	/* magic number, always stored in little endian */
	uint32_t magic;
	/* endian order, also stored in little endian */
	uint16_t endian;
	/*
	 * all other members are stored in the byte order of the CPU creating
	 * this kbox.
	 */
	uint16_t version;
	uint32_t size; /* kbox size */
	uint32_t region_count;
	char region_names[KBOX_REGION_COUNT_MAX][KBOX_REGION_NAME_SIZE];
	uint32_t region_sizes[KBOX_REGION_COUNT_MAX]; /* region size */
} __attribute__((packed));

#define KBOX_MAGIC 		0x4B424F58U /* hex ascii code for "KBOX" */

#define KBOX_VERSION		((uint16_t)1)
#define KBOX_VERSION_MAX	UINT16_MAX

raw_static_assert(KBOX_VERSION < KBOX_VERSION_MAX,
		  kbox_version_must_be_less_than_KBOX_VERSION_MAX);

/* make sure uint32_t and raw_atomic_int_t are the same size */
static char kbox_error_uint32_and_raw_atomic_int_are_not_same_size[
	    sizeof(uint32_t) != sizeof(raw_atomic_int_t) ? -1 : 0]
	    __attribute__((unused));

#define KBOX_EMPTY_SHIFT	__KBOX_REGION_CBITS_EMPTY_SHIFT
#define KBOX_EMPTY_MASK		__KBOX_REGION_CBITS_EMPTY_MASK
#define KBOX_SEQ_TOP		__KBOX_REGION_CBITS_SEQ_TOP
#define KBOX_SEQ_MASK		__KBOX_REGION_CBITS_SEQ_MASK

#define KBOX_PATH			"kbox"
#define SYS_KBOX_PATH			"/sys/kbox/"
/* 5 indicates sizeof "kbox"&"/" subtract the duplicate count of '\0' */
#define KBOX_FILE_LEN			((KBOX_REGION_NAME_SIZE) + (5U))
/* 6 indicates sizeof "_clear" subtract the duplicate count of '\0' */
#define KBOX_CLEAR_FILE_LEN		((KBOX_FILE_LEN) + (6U))
/* 10 indicates sizeof "/sys/kbox/" subtract the duplicate count of '\0' */
#define SYSFS_KBOX_FILE_LEN		((KBOX_REGION_NAME_SIZE) + (10U))
/* 6 indicates sizeof "_clear" subtract the duplicate count of '\0' */
#define SYSFS_KBOX_CLEAR_FILE_LEN	((SYSFS_KBOX_FILE_LEN) + (6U))

static inline void kbox_cbits_split(uint32_t cbits, uint32_t *empty, uint32_t *seq)
{
	if (empty != NULL) {
		*empty = ((cbits & KBOX_EMPTY_MASK) >> KBOX_EMPTY_SHIFT);
	}
	if (seq != NULL) {
		*seq = (cbits & KBOX_SEQ_MASK);
	}
}

static inline void kbox_cbits_merge(uint32_t *cbits, uint32_t empty, uint32_t seq)
{
	if (cbits != NULL) {
		*cbits = (((empty << KBOX_EMPTY_SHIFT) & KBOX_EMPTY_MASK) |
			  (seq & KBOX_SEQ_MASK));
	}
}

static inline size_t kbox_seq_add(size_t seq, size_t diff)
{
	return (seq + diff) & KBOX_SEQ_MASK;
}

static inline size_t kbox_seq_minus(size_t seq_src, size_t diff)
{
	size_t seq = seq_src;
	if (unlikely(seq < diff)) {
		seq += KBOX_SEQ_TOP;
	}
	return (seq - diff) & KBOX_SEQ_MASK;
}

struct kbox_device {
	struct kbox_device_head head;
	/*
	 * type cast KBOX_ALIGN and sizeof(struct kbox_device_head) to int, so
	 * if KBOX_ALIGN is smaller than sizeof(struct kbox_device_head), we'll
	 * get an negative array size error.
	 */
	uint8_t padding[(int)KBOX_ALIGN - (int)sizeof(struct kbox_device_head)];
	struct kbox_device_region regions[0];
} __attribute__((packed));

static inline int kbox_check_region_name_chars(const char *name)
{
	int err = E_HM_OK;
	const char *p = NULL;
	for (p = name; *p != '\0'; p++) {
		if ((isalnum((int)*p) == 0) && *p != '-' && *p != '_') {
			err = E_HM_BADMSG;
			break;
		}
	}
	return err;
}

static int kbox_check_region_name_len(const char *name)
{
	int err;
	size_t len;
	len = strnlen(name, KBOX_REGION_NAME_SIZE);
	if (len == 0UL) {
		err = E_HM_NOMSG;
	} else if (len >= KBOX_REGION_NAME_SIZE) {
		err = E_HM_MSGSIZE;
	} else {
		err = E_HM_OK;
	}
	return err;
}

/*
 * Return Value:
 * E_HM_OK		name is valid
 * E_HM_INVAL		name is NULL
 * E_HM_NOMSG		length of name is zero
 * E_HM_EMSGSIZE	length of name is too long
 * E_HM_BADMSG		name contains illegal characters
 */
static inline int kbox_check_region_name(const char *name)
{
	int err = E_HM_INVAL;
	if (name != NULL) {
		err = kbox_check_region_name_len(name);
		if (err == E_HM_OK) {
			err = kbox_check_region_name_chars(name);
		}
	}
	return err;
}

#define is_aligned(x, a) (((x) & ((typeof(x))((a) - (typeof(a))(1)))) == (typeof(x))(0))

/*
 * Return Value:
 * E_HM_OK		size is valid
 * E_HM_RANGE		size is out of range
 * E_HM_ALIGN		size is not aligned correctly
 */
static inline int kbox_check_region_size(size_t size)
{
	int err = E_HM_OK;
	if (size < KBOX_REGION_SIZE_MIN || size > KBOX_REGION_SIZE_MAX) {
		err = E_HM_RANGE;
	}
	if ((err == E_HM_OK) && !is_aligned(size, (size_t)KBOX_ALIGN)) {
		err = E_HM_ALIGN;
	}
	return err;
}

#endif /* !ULIBS_LIBKBOX_DEVICE_H */
