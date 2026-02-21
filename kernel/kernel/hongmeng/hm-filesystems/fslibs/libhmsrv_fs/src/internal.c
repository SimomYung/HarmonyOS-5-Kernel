/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of internal
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 18 15:23:43 2019
 */

#include "internal.h"

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <fslib_common.h>
#include <vfs.h>
#include <fd.h>

#include <libhmactv/actv_id.h>
#include <libhmsync/raw_thread.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(VFSCORE);

/**
 * str_ncheck - Check input str address and its length.
 * @src:         Input string.
 * @src_max_len: The allowed max length of @src, excluding
 *               ('\0') at the end, which is strlen(src).
 *
 * Return:
 *      E_HM_POSIX_FAULT          - Invalid address of @src.
 *      E_HM_RANGE          - Length of @src is larger than @src_max_len.
 *      E_HM_OK             - Success.
 *
 * NOTE: Input of src_max_len should be max of strlen(src).
 */
int str_ncheck(const char *src, const unsigned long long src_max_len)
{
	int err = 0;
	char checker = 0xFF;
	char *chr = (char *)src;
	unsigned long long len = 0;

	while (len < src_max_len && err == E_HM_OK && checker != '\0') {
		err = safe_copy(&checker, chr, 1);
		chr++;
		len++;
	}

	if (err < 0) {
		hm_debug("buf or path points to an invalid address!\n");
		return E_HM_POSIX_FAULT;
	}

	/*
	 * There are three scenarios as follows:
	 * 1. len < src_max_len && checker == '\0'
	 * 2. len == src_max_len && checker != '\0'
	 * 3. len == src_max_len && checker == '\0'
	 * And if the buffer is end up with '\0', return OK directly.
	 */
	if (checker == '\0') {
		return E_HM_OK;
	}

	if (len >= src_max_len) {
		err = safe_copy(&checker, chr, 1);
		if (err < 0) {
			hm_debug("buf or path points to an invalid address!\n");
			return E_HM_POSIX_FAULT;
		}
		if (checker != '\0') {
			hm_debug("src pointer has an invalid address access "
				 "problem, src max_len is %llu, check len "
				 "%llu\n", src_max_len, len);
			return E_HM_RANGE;
		}
	}

	return err;
}

int str_check(const char *src)
{
	int rc = 0;
	char checker = 0xFF;
	char *chr = (char *)src;

	while (rc == E_HM_OK && checker != '\0') {
		rc = safe_copy(&checker, chr, 1);
		chr++;
	}

	if (rc < 0) {
		hm_debug("buf or path points to an invalid address!\n");
		return E_HM_POSIX_FAULT;
	}

	return rc;
}

/**
 * check_path - check @path is not-NULL, addr is valid,
 * length is valid.
 *
 * @return: hm errno
 */
int check_path(const char *path)
{
	int err;

	err = str_ncheck(path, PATH_MAX - 1);
	if (err != E_HM_OK) {
		return err == E_HM_RANGE ? E_HM_NAMETOOLONG : err;
	}
	if (strlen(path) == 0) {
		hm_debug("path is null string\n");
		return E_HM_POSIX_NOENT;
	}

	return E_HM_OK;
}

bool get_ksys_param_is_fwd(void)
{
	int tid = raw_thread_tid_of(raw_thread_self());
	BUG_ON(tid < 0);
	return (unsigned int)tid >= actv_id_min();
}
