/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: actv assist functions
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 02 16:42:22 2021
 */
#include <sys/mman.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmactv/actv.h>
#include <libhmactv/actv_id.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include "../libhmsync/raw_thread_impl.h"

void *actv_pure_mmap_membuf(size_t membuf_size)
{
	char *addr = NULL;
	/* excluding the guard page, map other membuf pages to R/W permission */
	addr = hm_mem_mmap_ex(NULL, membuf_size - PAGE_SIZE,
					     __U(PROT_READ) | __U(PROT_WRITE),
#ifdef CONFIG_FORK_COW
					     __U(MAP_PRIVATE) | __U(MAP_ANON) | MFLAG_NOCOW | MFLAG_MAP_ACTV,
#else
					     __U(MAP_PRIVATE) | __U(MAP_ANON) | MFLAG_MAP_ACTV,
#endif
						-1, 0, PAGE_SIZE);
	if (addr == MAP_FAILED) {
			hm_error("alloc actv membuf by mmap failed\n");
	} else {
		addr = (char *)addr - PAGE_SIZE;
	}
	return (addr == MAP_FAILED) ? NULL : addr;
}

static int actv_pure_assist_mem_alloc(struct actv_pure_attr *attr)
{
	int err = E_HM_OK;
	void *assist_membuf = NULL;

	/* alloc actv assist membuf */
	assist_membuf = actv_pure_mmap_membuf(attr->membuf_size);
	if (assist_membuf == NULL) {
		hm_error("alloc actv assist membuf failed\n");
		err = E_HM_NOMEM;
	} else {
		attr->assist_actv_pure = mem_buf_to_actv_pure(assist_membuf, attr);
		attr->assist_actv_local = actv_pure_to_actv_local(attr->assist_actv_pure, attr);
	}

#ifdef CONFIG_NEED_SHADOW_STACK
	/* alloc actv assist sstack */
	if (err == E_HM_OK) {
		void *assist_sstack_base = raw_mmap_shadow_stack();
		if (assist_sstack_base == NULL) {
			hm_error("alloc actv assist sstack base failed\n");
			err = hm_mem_munmap(assist_membuf, attr->membuf_size);
			if (err < E_HM_OK) {
				hm_warn("munmap assist membuf failed, memory maybe leak, err=%s\n", hmstrerror(err));
			}
			err = E_HM_NOMEM;
		} else {
			attr->assist_sstack_base = assist_sstack_base;
			attr->assist_sstack = (void *)shadow_stack_start_addr((uintptr_t)attr->assist_sstack_base);
		}
	}
#endif

	return err;
}

static void actv_pure_assist_mem_free(struct actv_pure_attr *attr)
{
	int err;
	void *assist_membuf = NULL;

	/* free assist membuf */
	assist_membuf = actv_pure_to_mem_buf(attr->assist_actv_pure, attr);
	BUG_ON(assist_membuf == NULL);
	err = hm_mem_munmap(assist_membuf, attr->membuf_size);
	if (err < E_HM_OK) {
		hm_warn("munmap assist membuf failed, memory maybe leak, err=%s\n", hmstrerror(err));
	}

	/* reset assist membuf related variables in attr */
	attr->assist_actv_pure = 0UL;
	attr->assist_actv_local = 0UL;

#ifdef CONFIG_NEED_SHADOW_STACK
	/* free assist shadow stack */
	BUG_ON(attr->assist_sstack_base == NULL);
	err = hm_mem_munmap(attr->assist_sstack_base, SHADOW_STACK_TOTAL_SIZE);
	if (err < E_HM_OK) {
		hm_warn("munmap assist sstack failed, memory maybe leak, err=%s\n", hmstrerror(err));
	}

	/* reset assist sstack related variables in attr */
	attr->assist_sstack_base = NULL;
	attr->assist_sstack = NULL;
#endif
}

/*
 * (1) The assist stack is intited as the same as the stack of ordinary actv except for
 *     tls and actv_cref.
 * (2) The tls and actv_cref should be set when combining the assist membuf with actv kobj
 *     in rpc entry. The reason is that the related interfaces will use the current actv.
 * (3) This interface should be invoked after the actv_pure_attr set.
 */
int hm_actv_pure_assist_mem_init(struct actv_pure_attr *attr)
{
	int err = E_HM_OK;
	unsigned int actv_id;
	unsigned long actv_pure;
	unsigned long touch_size;
	void *mem_buf = NULL;
	void *touch_buf = NULL;
	void *recv_buf = NULL;

	if (attr == NULL || attr->link_thread == NULL) {
		err = E_HM_INVAL;
	}

	/* 1) alloc assist membuf/sstack */
	if (err == E_HM_OK) {
		err = actv_pure_assist_mem_alloc(attr);
		if (err == E_HM_OK) {
			actv_pure = attr->assist_actv_pure;
		} else {
			hm_error("assist mem alloc failed, err=%s\n", hmstrerror(err));
		}
	}

	/* 2) touch buffer */
	if (err == E_HM_OK) {
		mem_buf = actv_pure_to_mem_buf(actv_pure, attr);
		touch_buf = ulong_to_ptr((ptr_to_ulong(mem_buf) + (unsigned long)PAGE_SIZE), void);
		touch_size = attr->membuf_size - (size_t)PAGE_SIZE;
		err = touch_buffer(touch_buf, touch_size);
		if (err < 0) {
			actv_pure_assist_mem_free(attr);
			hm_error("touch recv buffer failed, err=%s\n", hmstrerror(err));
		}
	}

	/* 3) alloc actv_id for assist membuf */
	if (err == E_HM_OK) {
		if (!resv_actv_idpool_is_inited()) {
			resv_actv_idpool_init();
		}
		err = actv_pure_id_alloc(attr->resv_actv_idpool, &actv_id);
		if (err < 0) {
			actv_pure_assist_mem_free(attr);
			hm_error("actv pure id alloc failed, err=%s\n", hmstrerror(err));
		}
	}

	/* 4) init stack buf: recv_buf is the top of stack buf */
	if (err == E_HM_OK) {
		recv_buf = actv_pure_to_recv_buf(actv_pure, attr);
		actv_pure_stack_init(ulong_to_ptr(actv_pure, struct actv_pure), attr, recv_buf,
				     actv_id, ptr_to_ulong(attr->assist_sstack_base));
		actv_pure_insert_thread_pool(ulong_to_ptr(actv_pure, struct actv_pure), attr);
	}

	return err;
}
