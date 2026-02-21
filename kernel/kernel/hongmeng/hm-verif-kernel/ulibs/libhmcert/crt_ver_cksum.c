/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Create or verify system file checksums
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 21 20:32:56 2021
 */

#include <libsysif/sysmgr/api.h>
#include <libhmcert/crt_ver_cksum.h>
#include <string.h>
#include <vfs.h>
#include <libhmcrypt/hmcrypto_hash.h>

#define HEX_STR_WIDTH 2
#define READ_BUF 4096

hmcert_hash_ctx g_hash_ctx = {0};

void hmcert_hash_ctx_init(hmcert_hash_ctx *hash_ctx)
{
	g_hash_ctx.hash_newctx = hash_ctx->hash_newctx;
	g_hash_ctx.hash_freectx = hash_ctx->hash_freectx;
	g_hash_ctx.hash_init = hash_ctx->hash_init;
	g_hash_ctx.hash_update = hash_ctx->hash_update;
	g_hash_ctx.hash_final = hash_ctx->hash_final;
}

void hmcert_hash_ctx_fini(void)
{
	g_hash_ctx.hash_newctx = NULL;
	g_hash_ctx.hash_freectx = NULL;
	g_hash_ctx.hash_init = NULL;
	g_hash_ctx.hash_update = NULL;
	g_hash_ctx.hash_final = NULL;
}

static int hm_cert_verify_path_and_chksum(const char *path_and_chksum)
{
	int rc = E_HM_INVAL;
	if (path_and_chksum != NULL) {
		rc = actvcall_hmcall_cert_verify_path_and_chksum(path_and_chksum, BIG_BUF_SIZE);
	}
	return rc;
}

static int convert_checksum(const char *path, const unsigned char *sha_buffer,
			    int sha_len, char *result_buf, int buf_len)
{
	int err = E_HM_OK;
	char sum_str[SHA256SUM_STR_LEN + 1] = {0};

	if ((buf_len > BIG_BUF_SIZE) || (sha_len != SHA256SUM_RAW_LEN)) {
		err = E_HM_POSIX_FAULT;
	}
	if ((result_buf == NULL) || (sha_buffer == NULL)) {
		err = E_HM_POSIX_NOENT;
	}
	/* convert sha256 value from binary form into hex string form */
	if (err == E_HM_OK) {
		for (int i = 0; i < sha_len; i++) {
			err = snprintf_s(&sum_str[i * HEX_STR_WIDTH],
					 (size_t)(unsigned)(SHA256SUM_STR_LEN + 1 - i * HEX_STR_WIDTH),
					 HEX_STR_WIDTH, "%02x", sha_buffer[i]);
			if (err <= 0) {
				err = E_HM_POSIX_FAULT;
				break;
			}
		}
		if (err > 0) {
			err = E_HM_OK;
		}
	}
	if (err == E_HM_OK) {
		err = sprintf_s(result_buf, (size_t)(unsigned)buf_len, "%s %s\n", sum_str, path);
		if (err > 0) {
			err = E_HM_OK;
		} else {
			err = E_HM_POSIX_FAULT;
		}
	}
	return err;
}

static int get_hash_value_vfs(int fd_out, unsigned char *buf, size_t *buf_len)
{
	int ret = E_HM_OK;
	ssize_t read_len;
	hmcrypto_hash_ctx *ctx = NULL;
	unsigned char buffer[READ_BUF] = {0};

	ctx = g_hash_ctx.hash_newctx(HMCRYPTO_HASH_SHA2_256);
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (g_hash_ctx.hash_init(ctx) != E_HM_OK) {
		ret = E_HM_INVAL;
	}

	while (ret == E_HM_OK) {
		read_len = vfs_read(fd_out, buffer, READ_BUF);
		if (read_len > 0) {
			if (g_hash_ctx.hash_update(ctx, buffer, (size_t)read_len) != E_HM_OK) {
				ret = E_HM_INVAL;
			}
		} else if (read_len == 0) {
			break;
		} else {
			hm_warn("read file failed\n");
			ret = E_HM_INVAL;
		}
	}
	/* get 256-bit binary sha256 value */
	if (ret == E_HM_OK && g_hash_ctx.hash_final(ctx, buf, buf_len) != E_HM_OK) {
		ret = E_HM_INVAL;
	}

	g_hash_ctx.hash_freectx(ctx);

	return ret;
}

static int cal_file_check_value(const char *path, char *result, int buf_len)
{
	int err = E_HM_INVAL;
	int fd_out = -1;
	unsigned char sha_buffer[SHA256SUM_RAW_LEN] = {0};
	size_t sha_buffer_len = SHA256SUM_RAW_LEN;

	if (buf_len > BIG_BUF_SIZE) {
		goto out;
	}
	if (result == NULL) {
		goto out;
	}

	if ((fd_out = vfs_open(path, O_RDONLY, 0)) < 0) {
		hm_warn("open file %s failed: %s\n", path, hmstrerror(fd_out));
		goto out;
	}

	err = get_hash_value_vfs(fd_out, sha_buffer, &sha_buffer_len);
	if (err != E_HM_OK) {
		hm_warn("vfs get hash value error\n");
		goto out;
	}

	err = convert_checksum(path, sha_buffer, SHA256SUM_RAW_LEN, result, buf_len);
out:
	if (fd_out != -1) {
		(void)vfs_close(fd_out);
	}
	return err;
}

int compare_file_chksum(const char *path)
{
	char *result = NULL;
	int rc = E_HM_OK;

	result = calloc(1, BIG_BUF_SIZE);
	if (result == NULL) {
		rc = E_HM_NOMEM;
	}
	if (rc == E_HM_OK) {
		rc = cal_file_check_value(path, result, BIG_BUF_SIZE);
	}
	if (rc == E_HM_OK) {
		rc = hm_cert_verify_path_and_chksum(result);
	}
	if (result != NULL) {
		free(result);
	}
	return rc;
}
