/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2024. All rights reserved.
 * Description: Implementation for kbox snapshot
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 24 15:35:10 2024
 */

#include <fs/comp.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <libkbox/kbox_snapshot.h>

#define COMP_MAGIC_SIZE    4U
#define entry_data_of(entry) ptr_to_type(ptr_add(entry, sizeof(struct file_entry)), const char *)
#define total_entry_len(entry) (get_entry_datalen(entry) + sizeof(struct file_entry))

struct decompress_args_s {
	const void *in;
	size_t in_size;
	void *out;
	size_t out_size;
};

#define FAIL_CONTENT_LEN_MAX    64U
static int do_decompress(const struct decompress_args_s *decomp_args, decomp_func_t decomp_func,
			 dump_func_t dump_func, uintptr_t dump_arg, unsigned int *decomp_fail_count)
{
	int ret = E_HM_OK;
	char fail_content[FAIL_CONTENT_LEN_MAX] = {0};
	const void *dump_content = NULL;
	unsigned int dump_len = 0U;
	int decomp_ret;

	decomp_ret = decomp_func(decomp_args->in, decomp_args->in_size,
				 decomp_args->out, decomp_args->out_size);
	if (decomp_ret < 0) {
		int len;
		hm_error("decompress failed, data_size=%zu, ret=%d\n", decomp_args->in_size, decomp_ret);
		len = snprintf_s(fail_content, sizeof(fail_content), sizeof(fail_content) - 1,
				 "entry invalid, skip %zu bytes. err=%d\n", decomp_args->in_size, decomp_ret);
		if (len < 0) {
			ret = E_HM_FAULT;
		} else {
			dump_len = (unsigned int)len;
			dump_content = (const void *)fail_content;
		}
		(*decomp_fail_count)++;
	} else {
		dump_len = (unsigned int)decomp_ret;
		dump_content = (const void *)(decomp_args->out);
	}

	if (ret == E_HM_OK && dump_len > 0) {
		ret = dump_func(dump_content, dump_len, dump_arg);
		if (ret < 0) {
			hm_error("dump failed, ret=%d\n", ret);
			ret = E_HM_FAULT;
		}
	}

	return ret;
}

/* seek position to next valid comp entry */
static int seek_comp_entry(const char *text, unsigned int len)
{
	unsigned int pos = 0;
	int result = -1;

	while ((pos + COMP_MAGIC_SIZE) <= len) {
		/* magic number "COMP", always stored in little endian */
		while (pos < len && text[pos] != 'P') {
			pos++;
		}
		pos++;

		if (pos + 2 < len && text[pos] == 'M' && text[pos + 1] == 'O' && text[pos + 2] == 'C') {
			result = pos - 1;
			if (len - result < sizeof(struct file_entry)) {
				/* no more space for holding one entry */
				result = -1;
			}
			break;
		}
	}

	return result;
}

static const struct file_entry *find_next_entry(const char *buffer, size_t size)
{
	const struct file_entry *entry = NULL;
	const char *curr_pos = buffer;
	size_t left_size = size;
	size_t gap_size;

	while (left_size > 0) {
		const struct file_entry *tmp_entry = NULL;
		unsigned int data_len;
		int offset = seek_comp_entry(curr_pos, left_size);
		if (offset < 0) {
			break;
		}

		tmp_entry = ptr_to_type(ptr_add(curr_pos, offset), const struct file_entry *);
		gap_size = ptr_diff(tmp_entry, curr_pos);
		data_len = get_entry_datalen(tmp_entry);
		if (data_len <= SNAPSHOT_COMP_UNIT && total_entry_len(tmp_entry) <= (left_size - gap_size)) {
			entry = tmp_entry;
			break;
		} else {
			/* just skip this magic and move next */
			curr_pos = ptr_add(curr_pos, COMP_MAGIC_SIZE);
			left_size -= COMP_MAGIC_SIZE;
		}
	}

	return entry;
}

static int do_kbox_decompress_snapshot(struct decompress_args_s *decomp_args,
				       decomp_func_t decomp_func,
				       dump_func_t dump_func, uintptr_t dump_arg)
{
	int ret = E_HM_OK;
	const char *curr_pos = ptr_to_type(decomp_args->in, const char *);
	size_t left_size = decomp_args->in_size;
	const struct file_entry *entry = NULL;
	unsigned int decomp_fail_count = 0;

	while (left_size > 0) {
		entry = find_next_entry(curr_pos, left_size);
		if (entry == NULL) {
			ret = E_HM_OK;
			break;
		}

		/* write raw data between 2 entries */
		if (ptr_to_type(entry, const char *) != curr_pos) {
			ret = dump_func(curr_pos, ptr_diff(entry, curr_pos), dump_arg);
			if (ret < 0) {
				hm_error("dump failed, ret=%d\n", ret);
				ret = E_HM_FAULT;
				break;
			}
			left_size -= (size_t)ptr_diff(entry, curr_pos);
			curr_pos = ptr_to_type(entry, const char *);
		}

		decomp_args->in = entry_data_of(entry);
		decomp_args->in_size = get_entry_datalen(entry);
		ret = do_decompress(decomp_args, decomp_func, dump_func, dump_arg, &decomp_fail_count);
		if (ret < 0) {
			break;
		}

		curr_pos = ptr_add(curr_pos, total_entry_len(entry));
		left_size -= total_entry_len(entry);
	}

	if (ret == E_HM_OK && left_size > 0) {
		/* there is no entry in left data, write left raw data */
		int err = dump_func((const void *)curr_pos, (unsigned int)left_size, dump_arg);
		if (err < 0) {
			hm_error("dump failed, ret=%d\n", err);
			ret = E_HM_FAULT;
		}
	}

	if (ret == E_HM_OK && decomp_fail_count > 0) {
		hm_error("Failed to decompress %u entries\n", decomp_fail_count);
		ret = E_HM_FAULT;
	}

	return ret;
}

int kbox_decompress_snapshot(const void *input_buf, size_t input_len,
			     decomp_func_t decomp_func,
			     dump_func_t dump_func, uintptr_t dump_arg)
{
	int ret = E_HM_OK;
	void *buf_decomp = NULL;
	struct decompress_args_s decomp_args;

	if ((input_buf == NULL) || (input_len == 0) || (decomp_func == NULL) || (dump_func == NULL)) {
		ret = E_HM_INVAL;
	} else {
		buf_decomp = malloc(SNAPSHOT_COMP_UNIT);
		if (buf_decomp == NULL) {
			hm_error("no memory for decompression\n");
			ret = E_HM_NOMEM;
		} else {
			decomp_args.in = input_buf;
			decomp_args.in_size = input_len;
			decomp_args.out = buf_decomp;
			decomp_args.out_size = SNAPSHOT_COMP_UNIT;

			ret = do_kbox_decompress_snapshot(&decomp_args, decomp_func, dump_func, dump_arg);
			if (ret < 0) {
				hm_error("error occurred during decompression, err=%s\n", hmstrerror(ret));
			}
		}

		free(buf_decomp);
	}

	return ret;
}