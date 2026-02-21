/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Ringbuffer for ekox which don't support concurrent.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 21 11:00:56 2022
 */

#include <lib/ekbox_rb.h>
#include <hmkernel/errno.h>

#ifdef __KERNEL__
#include <lib/string.h>
#else
#include <libhwsecurec/securec.h>
#endif /* __KERNEL__ */

/* magic value equals to hex ascii code of "EKBOXRBF". */
#define MAGIC_NUM 0x454b424f58524246ULL

/* Caller should make sure that the hd is not NULL. */
void ekbox_rb_init(struct ekbox_head_s *hd, __u32 size)
{
	mem_zero_b(hd, size);
	hd->buf_size = size - sizeof(struct ekbox_head_s);
	hd->widx = 0;
	hd->full = __false;
	hd->magic_num = MAGIC_NUM;
}

static void ekbox_widx_inc(struct ekbox_head_s *hd)
{
	hd->widx = (hd->widx + 1) % hd->buf_size;
	if (!hd->full && (hd->widx == 0)) {
		hd->full = __true;
	}
}

/* Ekbox ringbuf is drop old mode, old data will be overwritten by new data.
 * Caller should make sure that the hd is not NULL and str is valid.
 */
void ekbox_rb_puts(struct ekbox_head_s *hd, const char *str, __u32 str_len)
{
	__u32 index;
	char *buf = (char *)ptr_add(hd, sizeof(struct ekbox_head_s));

	for (index = 0; index < str_len; index++) {
		*(char *)(buf + hd->widx) = str[index];
		ekbox_widx_inc(hd);
	}
}

/* Caller should make sure that the hd is not NULL. */
__bool ekbox_rb_is_inited(const struct ekbox_head_s *hd, __u32 size)
{
	__bool inited = __false;

	if ((hd->magic_num == MAGIC_NUM) &&
	    (hd->buf_size == (size - sizeof(struct ekbox_head_s))) &&
	    (hd->widx < hd->buf_size)) {
		inited = __true;
	}

	return inited;
}

int ekbox_rb_read(const struct ekbox_head_s *hd, __u32 ekbox_size,
		ekbox_read_func_t read_func, __uptr_t read_arg)
{
	int err = E_HM_OK;
	unsigned int head_sz = sizeof(struct ekbox_head_s);
	int read_len;
	int total_read = 0;

	if ((hd == NULL) || (read_func == NULL)) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK && !ekbox_rb_is_inited(hd, ekbox_size)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		const char *ekbox_buf = (char *)ptr_add(hd, head_sz);
		if (hd->full) {
			/* copy right part */
			read_len = read_func(ptr_add(ekbox_buf, hd->widx), hd->buf_size - hd->widx, read_arg);
			if (read_len < 0) {
				err = read_len;
			} else {
				total_read += read_len;
			}
		}
		if (err == E_HM_OK && hd->widx > 0) {
			read_len = read_func(ekbox_buf, hd->widx, read_arg);
			if (read_len < 0) {
				err = read_len;
			} else {
				total_read += read_len;
			}
		}
	}
	if (err == E_HM_OK) {
		err = total_read;
	}

	return err;
}
