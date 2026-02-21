/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Operations on tls of raw_thread
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 28 20:23:07 2021
 */

#include <libhmsync/raw_thread.h>
#include "raw_thread_impl.h"

struct raw_thread *raw_tls_to_thread(unsigned char *_mem)
{
	unsigned char *mem = _mem;
#ifdef TLS_ABOVE_TP
	/* Make sure tls_base_addr(mem + sizeof(struct raw_thread)) is aligned to plibc->tls_align */
	mem += ((size_t)(1U + ~((uintptr_t)mem + sizeof(struct raw_thread)))) & (plibc->tls_align - 1);
#else
	mem += __raw_thread_tls_size() - sizeof(struct raw_thread);
	mem -= (uintptr_t)mem & (plibc->tls_align - 1UL);
#endif

	return (raw_thread_t)(void *)mem;
}

struct raw_thread *raw_tls_base_to_thread(unsigned long tls)
{
	return TLS_TO_THREAD(tls);
}

#ifndef __ELIBC__

void *raw_tls_copy(unsigned char *mem)
{
	int ret;
	raw_thread_t td;
	uintptr_t *dtv;
	size_t i = 1U;
	struct tls_module *p = plibc->tls_head;

#ifdef TLS_ABOVE_TP
	dtv = (uintptr_t *)(void *)(mem + __raw_thread_tls_size()) - (plibc->tls_cnt + 1);

	td = raw_tls_to_thread(mem);
	mem = (unsigned char *)td + sizeof(struct raw_thread);

	while (p != NULL) {
		dtv[i] = (uintptr_t)(mem + p->offset);
		BUG_ON(p->image == NULL);
		if (p->len > 0) {
			ret = memcpy_s(mem + p->offset, p->len, p->image, p->len);
			BUG_ON(ret != 0);
		}
		i++;
		p = p->next;
	}
#else
	dtv = (void *)mem;

	td = raw_tls_to_thread(mem);
	mem = (unsigned char *)td;

	while (p != NULL) {
		dtv[i] = (uintptr_t)(mem - p->offset);
		BUG_ON(p->image == NULL);
		if (p->len > 0) {
			ret = memcpy_s(mem - p->offset, p->len, p->image, p->len);
			BUG_ON(ret != 0);
		}
		i++;
		p = p->next;
	}
#endif
	dtv[0] = plibc->tls_cnt;
	CMPT_THREAD_REG(td, dtv) = dtv;
	return td;
}

#else /*  __ELIBC__ */

typedef struct new_dtv_info {
	struct new_dtv_info *next;
	void *mem;
} new_dtv_info_t;

typedef struct {
	new_dtv_info_t *head;
	new_dtv_info_t *tail;
} tls_mem_info_t;

static inline size_t get_dtv_pos(size_t tls_size_, size_t tls_num_)
{
	return (tls_size_ - (tls_num_ + 1) * sizeof(uintptr_t) - sizeof(tls_mem_info_t));
}

void *raw_tls_copy(unsigned char *mem)
{
	raw_thread_t td = NULL;
	uintptr_t *dtv = NULL;
	uint8_t *blocks = NULL;
	int index = 0;
	struct tls_module *tls = NULL;

#ifdef TLS_ABOVE_TP
	/*
		 td(struct thread_st end align)
	____________________________________________________________________
	align| thread_st|   blocks    |dtv[0]|dtv[1]|dtv[..]|tls_mem_info_t
	_____|__________|_____________|______|______|_______|_______________
	*/
	dtv = (uintptr_t *)(mem + get_dtv_pos(__raw_thread_tls_size(), plibc->tls_cnt));
	td = raw_tls_to_thread(mem);
	blocks = (uint8_t *)td + sizeof(struct raw_thread);
#else
	/*
	dtv                                                    td(align)
	_____________________________________________________________________
	dtv[0]|dtv[1]|dtv[..]|tls_mem_info_t|   blocks    |align| thread_st
	______|______|_______|______________|_____________|_____|____________
	*/
	td = raw_tls_to_thread(mem);
	dtv = (uintptr_t *)mem;
	blocks = (uint8_t *)td;
#endif

	dtv[index++] = plibc->tls_cnt;
	tls = plibc->tls_head;
	while (tls != NULL) {
#ifdef TLS_ABOVE_TP
		dtv[index] = (uintptr_t)(blocks + tls->offset);
#else
		dtv[index] = (uintptr_t)(blocks - tls->offset);
#endif
		/* clear bss */
		NOFAIL(memset_s((void *)(dtv[index] + tls->len), tls->size - tls->len, 0, tls->size - tls->len));

		if (tls->len > 0)
			NOFAIL(memcpy_s((void *)dtv[index],  tls->len, (void *)tls->image, tls->len));

		index++;
		tls = tls->next;
	}
	tls_mem_info_t *info = (tls_mem_info_t *)(&dtv[(size_t)dtv[0] + 1]);
	info->head = info->tail = NULL;

	NOFAIL(memset_s(RAW_THREAD_TO_PTHREAD(td), sizeof(struct pthread), 0, sizeof(struct pthread)));
	CMPT_THREAD_REG(td, tls_dl_mem) = info;
	CMPT_THREAD_REG(td, dtv) = dtv;
	return td;
}

#endif /* __ELIBC__ */
