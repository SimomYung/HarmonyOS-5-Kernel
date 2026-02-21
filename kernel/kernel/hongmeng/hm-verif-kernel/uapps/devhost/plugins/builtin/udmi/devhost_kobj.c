/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Support HongMeng ctrlmem kobj
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 06 16:50:55 2023
 */

#include "devhost_kobj.h"

#include <vfs.h>
#include <vfs_devfs.h>
#include <hmkernel/errno.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_kshm.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_ctrlmem_syscalls_layer.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>
#include <lib/dlist.h>

#include <devhost/log.h>

#define PAGE_POOL_SIZE 10

struct ctrlmem_page {
	struct __ctrlmem_kobjctnr_page_s *page;
	struct dh_ctrlmem_page_grant_info grant_info;
	struct dlist_node node;
};

static struct ctrlmem_page *g_page_pool[PAGE_POOL_SIZE] = { NULL };
static struct raw_mutex g_page_pool_lock = RAW_MUTEX_INITIALIZER;

static struct dlist_node g_page_list = DLIST_HEAD_INIT(g_page_list);
static struct raw_mutex g_page_lock = RAW_MUTEX_INITIALIZER;

static int __dh_kobj_grant_to_fsmgr(struct ctrlmem_page *ctrlmem_page)
{
	int ret;
	uint64_t shm_key;
	xref_t xref;
	struct __ctrlmem_kobjctnr_page_s *page = NULL;
	int shm_id;

	xref.rref = acquire_cache_fsmgr_rref();
	if (unlikely(xref.rref == FS_INVALID_CNODE_RREF)) {
		dh_error("acquire fsmgr rref failed\n");
		return E_HM_POSIX_FAULT;
	}

	page = ctrlmem_page->page;
	shm_id = page->header.u.v32.v1;
	ret = hm_kshm_grant(shm_id, SHM_GRANT_RDONLY, xref, &shm_key);
	if (unlikely(ret < 0)) {
		dh_error("(kshm grant(shm_id=%d) to fsmgr failed, err=%s\n",
			 shm_id, hmstrerror(ret));
		return ret;
	}

	ctrlmem_page->grant_info.fsmgr_shm_key = shm_key;
	return E_HM_OK;
}

static int __dh_kobj_free_page_notify_to_fsmgr(struct ctrlmem_page *ctrlmem_page)
{
	int ret = E_HM_OK;
	uint64_t shm_key = ctrlmem_page->grant_info.fsmgr_shm_key;

	ret = vfs_devfs_remove_kshm(shm_key);
	if (ret != E_HM_OK) {
		dh_error("devfs remove kshm failed, err=%s", hmstrerror(ret));
	}

	return ret;
}

static int __dh_ctrlmem_free_page(struct ctrlmem_page *ctrlmem_page)
{
	int ret;
	struct __ctrlmem_kobjctnr_page_s *page = ctrlmem_page->page;
	unsigned int shm_id = page->header.u.v32.v1;

	ret = __dh_kobj_free_page_notify_to_fsmgr(ctrlmem_page);
	if (ret != E_HM_OK) {
		dh_error("notify fsmgr free page failed, err=%s\n", hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		ret = hm_mem_munmap(page, sizeof(struct __ctrlmem_kobjctnr_page_s));
	}

	if (ret == E_HM_OK) {
		ret = hm_kshm_close(shm_id);
	}

	if (ret == E_HM_OK) {
		ret = hm_kshm_unlink(shm_id);
	}

	if (ret == E_HM_OK) {
		free(ctrlmem_page);
	}

	return ret;
}

static bool page_is_freeable(const struct __ctrlmem_kobjctnr_page_s *page)
{
	__u64 *p = (ptr_cast(&page->freelists[0], __u64));
	return (*p == __CTRLMEM_KOBJCTNR_FREELISTS_ALLCLEAR_U64) ? true : false;
}

static void dh_ctrlmem_free_page(struct __ctrlmem_kobjctnr_page_s *page)
{
	bool really_free = true;
	struct ctrlmem_page *ctrlmem_page = NULL;
	struct ctrlmem_page *pos = NULL;
	struct ctrlmem_page *tmp = NULL;
	int ret;

	raw_mutex_lock(&g_page_lock);
	dlist_for_each_entry_safe(pos, tmp, &g_page_list, struct ctrlmem_page, node) {
		if (pos->page == page && page_is_freeable(page)) {
			dlist_delete(&pos->node);
			ctrlmem_page = pos;
			break;
		}
	}
	raw_mutex_unlock(&g_page_lock);

	if (ctrlmem_page == NULL) {
		return;
	}

	/* add to page pool */
	raw_mutex_lock(&g_page_pool_lock);
	for (unsigned int i = 0; i < PAGE_POOL_SIZE; i++) {
		if (g_page_pool[i] == NULL) {
			g_page_pool[i] = ctrlmem_page;
			really_free = false;
			break;
		}
	}
	raw_mutex_unlock(&g_page_pool_lock);

	/* really free */
	if (really_free == true) {
		ret = __dh_ctrlmem_free_page(ctrlmem_page);
		if (ret != E_HM_OK) {
			hm_error("dh ctrlmem free page failed: %s\n", hmstrerror(ret));
			raw_mutex_lock(&g_page_lock);
			dlist_insert_tail(&g_page_list, &ctrlmem_page->node);
			raw_mutex_unlock(&g_page_lock);
		}
	}
}

static int set_ctnr_header(struct __ctrlmem_kobjctnr_page_s *page, int shm_id)
{
	return sysctrlmem_kobjctnr_set_header_u32(&page->header.u.v32.v1, (__u32)shm_id);
}

static struct ctrlmem_page *__dh_ctrlmem_alloc_page(void)
{
	int shm_id;
	void *addr;
	int ret = E_HM_OK;
	struct __ctrlmem_kobjctnr_page_s *page = NULL;
	struct ctrlmem_page *ctrlmem_page = NULL;
	size_t ret_sz;

	shm_id = hm_kshm_open(__PGSTR_TYPE_KOBJ_CONTAINER, 0U,
			      sizeof(struct __ctrlmem_kobjctnr_page_s),
			      SHM_O_CREAT | SHM_O_RDONLY, &ret_sz);
	if (shm_id < 0) {
		return NULL;
	}

	addr = hm_mem_mmap(NULL, sizeof(struct __ctrlmem_kobjctnr_page_s), MPROT_READ,
			   MFLAG_MAP_SHARED | MFLAG_MAP_ANON, shm_id, 0);
	if (addr == MAP_FAILED) {
		goto err_mmap;
	}

	page = addr;
	ret = set_ctnr_header(page, shm_id);
	if (ret != E_HM_OK) {
		goto err_setheader;
	}

	ctrlmem_page = (struct ctrlmem_page *)malloc(sizeof(struct ctrlmem_page));
	if (ctrlmem_page == NULL) {
		goto err_malloc;
	}

	ctrlmem_page->page = page;

	/* grant the new ctrlmem page to fsmgr */
	ret = __dh_kobj_grant_to_fsmgr(ctrlmem_page);
	if (ret != E_HM_OK) {
		goto err_grant;
	}

	return ctrlmem_page;

err_grant:
	free(ctrlmem_page);
err_malloc:
err_setheader:
	(void)hm_mem_munmap(page, sizeof(struct __ctrlmem_kobjctnr_page_s));
err_mmap:
	(void)hm_kshm_close(shm_id);
	(void)hm_kshm_unlink(shm_id);
	return NULL;
}

static struct ctrlmem_page *dh_ctrlmem_alloc_page(void)
{
	int i;
	struct ctrlmem_page *ctrlmem_page = NULL;

	raw_mutex_lock(&g_page_pool_lock);
	for (i = 0; i < PAGE_POOL_SIZE; i++) {
		if (g_page_pool[i] != NULL) {
			ctrlmem_page = g_page_pool[i];
			g_page_pool[i] = NULL;
			break;
		}
	}
	raw_mutex_unlock(&g_page_pool_lock);

	if (ctrlmem_page == NULL) {
		/* alloc a new page */
		ctrlmem_page = __dh_ctrlmem_alloc_page();
		if (ctrlmem_page == NULL) {
			return NULL;
		}
	}

	raw_mutex_lock(&g_page_lock);
	dlist_insert_head(&g_page_list, &ctrlmem_page->node);
	raw_mutex_unlock(&g_page_lock);

	return ctrlmem_page;
}

static int ctrlmem_kobj_syscall_spawn_polllist(struct __ctrlmem_kobjctnr_page_s *page, unsigned int sz,
					       unsigned long file, __u32 listener_cnt)
{
	struct __ctrlmem_kobjctnr_page_freelist_s *fl = NULL;

	fl = __ctrlmem_kobjpool_freelist_of_sz(page, sz);
	BUG_ON(fl == NULL);

	return sysctrlmem_kobjctnr_spawn_polllist(fl, sz, file, listener_cnt, 0UL);
}

static struct __ctrlmem_kobj_polllist_s *do_spawn_polllist(unsigned long file, __u32 listener_cnt,
							   struct dh_ctrlmem_page_grant_info *grant_info)
{
	struct __ctrlmem_kobjctnr_page_s *p_page = NULL;
	struct ctrlmem_page *pos, *tmp;
	struct __ctrlmem_kobj_polllist_s *addr = NULL;
	int index = -1;
	int moved = 0;
	struct dlist_node full_pages;

	dlist_init(&full_pages);
	raw_mutex_lock(&g_page_lock);
	dlist_for_each_entry_safe(pos, tmp, &g_page_list, struct ctrlmem_page, node) {
		p_page = pos->page;
		index = ctrlmem_kobj_syscall_spawn_polllist(p_page,
						    __CTRLMEM_KOBJCTNR_KOBJSZ_POLLLIST,
						    file, listener_cnt);
		if (index >= 0 && index < __CTRLMEM_KOBJCTNR_PIECES_NR) {
			addr = (struct __ctrlmem_kobj_polllist_s *)&p_page->pieces[index];
			if (grant_info != NULL) {
				grant_info->fsmgr_shm_key = pos->grant_info.fsmgr_shm_key;
			}
			break;
		} else if (moved < PAGE_POOL_SIZE) {
			dlist_delete(&pos->node);
			dlist_insert(&full_pages, &pos->node);
			moved++;
		}
	}

	/* remove these fulled pages to the list tail */
	dlist_for_each_entry_safe(pos, tmp, &full_pages, struct ctrlmem_page, node) {
		dlist_delete(&pos->node);
		dlist_insert_tail(&g_page_list, &pos->node);
	}

	raw_mutex_unlock(&g_page_lock);
	return addr;
}

struct __ctrlmem_kobj_polllist_s *dh_ctrlmem_spawn_polllist(unsigned long file, __u32 listener_cnt,
				unsigned long *offset, struct dh_ctrlmem_page_grant_info *grant_info)
{
	struct ctrlmem_page *ctrlmem_page = NULL;
	struct __ctrlmem_kobj_polllist_s *addr = NULL;

	addr = do_spawn_polllist(file, listener_cnt, grant_info);
	while (addr == NULL) {
		ctrlmem_page = dh_ctrlmem_alloc_page();
		if (ctrlmem_page == NULL) {
			break;
		}
		addr = do_spawn_polllist(file, listener_cnt, grant_info);
	}

	if (addr != NULL && offset != NULL) {
		*offset = __ctrlmem_kobjctnr_page_ptr_to_offset(addr);
	}

	return addr;
}

#define MAX_DELETE_TIMES 100
int dh_ctrlmem_kobj_destroy(void *kobj)
{
	struct __ctrlmem_kobjctnr_page_s *page = __ctrlmem_kobjctnr_page_ptr_to_page(kobj);
	unsigned int npiece_kobj = __ctrlmem_kobjctnr_page_ptr_to_npiece(kobj);
	int result, try = 0;

	if (__ctrlmem_kobjctnr_npiece_is_valid(npiece_kobj)) {
		result = sysctrlmem_kobj_destroy(page, npiece_kobj);
		while (result == E_HM_AGAIN) {
			try++;
			if (try == MAX_DELETE_TIMES) {
				dh_error("failed to delete ctrlmem polllist\n");
				result = E_HM_INVAL;
				break;
			}
			(void)hm_thread_yield();
			result = sysctrlmem_kobj_destroy(page, npiece_kobj);
		}
	} else {
		result = E_HM_INVAL;
	}
	BUG_ON(result == 0);
	if (result > 0 && page_is_freeable(page)) {
		dh_ctrlmem_free_page(page);
	}

	return (result < 0) ? result : E_HM_OK;
}
