/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Supply epoll server common interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 12 21:00:44 2020
 */

#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>
#include <libhmsrv_sys/hm_ctrlmem_syscalls_layer.h>
#include <libhmsrv_sys/hm_kshm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/raw_rwlock.h>
#include <libsysif/utils.h>
#include <libhmlog/hmlog.h>
#include <libmem/flags.h>
#include <hmkernel/ctrlmem/kobjctnr.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hmkernel/ctrlmem.h>
#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/syscall.h>
#include <hongmeng/errno.h>
#include <hongmeng/macro.h>
#include <fcntl.h>
#include <stdbool.h>
#include <libhmsrv_sys/hm_thread.h>
#include <lib/dlist.h>
#include <hmasm/processor.h>
#include <libhmsync/raw_rwlock.h>

#define PAGE_POOL_SIZE 10
struct ctrlmem_page {
	struct __ctrlmem_kobjctnr_page_s *page;
	struct dlist_node node;
};
static struct ctrlmem_page *g_page_pool[PAGE_POOL_SIZE] = { NULL };
static struct raw_mutex g_page_pool_lock = RAW_MUTEX_INITIALIZER;

static struct dlist_node g_page_list = DLIST_HEAD_INIT(g_page_list);
static struct raw_rwlock g_page_lock = RAW_RDLOCK_INITIALIZER;

static void __free_and_unlink_shmid(int shm_id)
{
	int err;

	err = hm_kshm_close(shm_id);
	if (err != E_HM_OK) {
		hm_error("failed to close kobjctnr page %s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = hm_kshm_unlink(shm_id);
		if (err != E_HM_OK) {
			hm_error("failed to unlink kobjctnr page %s\n", hmstrerror(err));
		}
	}
}

#ifdef __HOST_UT__
int host_llt_cleanup_page_pool()
{
	int i, ret = 0;
	unsigned int shm_id;
	struct ctrlmem_page *ctrlmem_page = NULL;
	struct __ctrlmem_kobjctnr_page_s *page = NULL;

	for (i = 0; i < PAGE_POOL_SIZE; i++) {
		if (g_page_pool[i] != NULL) {
			ctrlmem_page = g_page_pool[i];
			page = ctrlmem_page->page;
			shm_id = page->header.u.v32.v1;
			(void)hm_mem_munmap(page, sizeof(struct __ctrlmem_kobjctnr_page_s));
			__free_and_unlink_shmid(shm_id);
			free(ctrlmem_page);
			g_page_pool[i] = NULL;
			ret++;
		}
	}
	return ret;
}
#endif

static bool is_full_page_for_sz(struct __ctrlmem_kobjctnr_page_s *ctnr_page, unsigned int _nfreelist)
{
	bool ret = true;
	unsigned int nfreelist = 0;
	unsigned int npiece = __CTRLMEM_KOBJCTNR_NPIECE_INVALID;

	for (nfreelist = _nfreelist; nfreelist < __CTRLMEM_KOBJCTNR_FREELISTS_NR; nfreelist++) {
		npiece = (unsigned int)ctnr_page->freelists[nfreelist].head;
		if (__ctrlmem_kobjctnr_npiece_is_valid(npiece)) {
			ret = false;
			break;
		}
	}
	return ret;
}

static void move_full_pages_to_tail(struct dlist_node *page_list, unsigned int size)
{
	int err = E_HM_OK;
	struct ctrlmem_page *pos, *tmp;
	struct dlist_node *move_pos, *move_tmp;
	DLIST_HEAD(full_pages);
	unsigned int nfreelist = 0U;

	while (size > (1U << nfreelist)) {
		nfreelist++;
	}
	if (!__ctrlmem_kobjctnr_nfreelist_is_valid(nfreelist)) {
		err = E_HM_INVAL;
		hm_error("size is invalid, sz=%u\n", size);
	}

	if (err == E_HM_OK) {
		dlist_for_each_entry_safe(pos, tmp, page_list, struct ctrlmem_page, node) {
			if (!is_full_page_for_sz(pos->page, nfreelist)) {
				break;
			}
			dlist_delete(&pos->node);
			dlist_insert(&full_pages, &pos->node);
		}
		/* remove these fulled pages to the list tail */
		dlist_for_each_safe(move_pos, move_tmp, &full_pages) {
			dlist_delete(move_pos);
			dlist_insert_tail(page_list, move_pos);
		}
	}
}

static bool page_is_freeable(struct __ctrlmem_kobjctnr_page_s *page)
{
	__u64 *p = (ptr_cast(&page->freelists[0], __u64));
	return (*p == __CTRLMEM_KOBJCTNR_FREELISTS_ALLCLEAR_U64) ? true : false;
}

static void __hm_ctrlmem_free_page(struct ctrlmem_page *ctrlmem_page)
{
	struct __ctrlmem_kobjctnr_page_s *page = ctrlmem_page->page;
	int shm_id = (int)page->header.u.v32.v1;
	int err;

	err = hm_mem_munmap(page, sizeof(struct __ctrlmem_kobjctnr_page_s));
	if (err != E_HM_OK) {
		hm_error("failed to unmmap kobjctnr page %s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		__free_and_unlink_shmid(shm_id);
	}

	if (err == E_HM_OK) {
		free(ctrlmem_page);
	}
}

static void hm_ctrlmem_free_page(struct ctrlmem_page *ctrlmem_page)
{
	int  i;
	bool really_free = true;

	if (ctrlmem_page != NULL) {
		/* add to page pool */
		raw_mutex_lock(&g_page_pool_lock);
		for (i = 0; i < PAGE_POOL_SIZE; i++) {
			if (g_page_pool[i] == NULL) {
				g_page_pool[i] = ctrlmem_page;
				really_free = false;
				break;
			}
		}
		raw_mutex_unlock(&g_page_pool_lock);

		/* really free */
		if (really_free == true) {
			__hm_ctrlmem_free_page(ctrlmem_page);
		}
	}
}

static int set_ctnr_header(struct __ctrlmem_kobjctnr_page_s *page, int shm_id)
{
	return sysctrlmem_kobjctnr_set_header_u32(&page->header.u.v32.v1, (__u32)shm_id);
}

static struct ctrlmem_page *__hm_ctrlmem_alloc_page(void)
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
		ret = shm_id;
	}

	if (ret == E_HM_OK) {
		addr = hm_mem_mmap(NULL, sizeof(struct __ctrlmem_kobjctnr_page_s), MPROT_READ,
				   MFLAG_MAP_SHARED | MFLAG_MAP_ANON, shm_id, 0);
		if (addr == MAP_FAILED) {
			__free_and_unlink_shmid(shm_id);
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		page = addr;
		ret = set_ctnr_header(page, shm_id);
		if (ret != E_HM_OK) {
			(void)hm_mem_munmap(page, sizeof(struct __ctrlmem_kobjctnr_page_s));
			__free_and_unlink_shmid(shm_id);
			page = NULL;
		}
	}

	if (ret == E_HM_OK) {
		ctrlmem_page = (struct ctrlmem_page *)malloc(sizeof(struct ctrlmem_page));
		if (ctrlmem_page == NULL) {
			(void)hm_mem_munmap(page, sizeof(struct __ctrlmem_kobjctnr_page_s));
			__free_and_unlink_shmid(shm_id);
			page = NULL;
		} else {
			ctrlmem_page->page = page;
		}
	}

	return ctrlmem_page;
}

static bool hm_ctrlmem_alloc_page(void)
{
	int i;
	struct ctrlmem_page *ctrlmem_page = NULL;
	bool succ = false;

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
		ctrlmem_page = __hm_ctrlmem_alloc_page();
	}

	(void)raw_rwlock_wrlock(&g_page_lock);
	if (ctrlmem_page != NULL) {
		dlist_insert_head(&g_page_list, &ctrlmem_page->node);
	}
	(void)raw_rwlock_unlock(&g_page_lock);

	if (ctrlmem_page != NULL) {
		succ = true;
	}

	return succ;
}

#define __COMMA0
#define __COMMA1 ,
#define __COMMA2 ,
#define __COMMA3 ,
#define __COMMA4 ,
#define __COMMA5 ,
#define __COMMA(n) __COMMA##n
#define CTRLMEM_ARGS(...) __VA_ARGS__

#define CTRLMEM_PROTO(n, ...) __SC_MAP(n, __SC_DECL, __VA_ARGS__)
#define CTRLMEM_PROTO_APPEND(n, ...) __COMMA(n) __SC_MAP(n, __SC_DECL, __VA_ARGS__)
#define CTRLMEM_PARAM_APPEND(n, ...) __COMMA(n) __SC_MAP(n, __SC_USE, __VA_ARGS__)

#define define_ctrlmem_kobj_spawn(__name, __kobjtype, __size, __args) \
static __kobjtype *do_kobj_spawn_##__name(struct __ctrlmem_kobjctnr_page_s *page, \
		unsigned int size CTRLMEM_PROTO_APPEND(__args)) \
{ \
	int index = -1; \
	__kobjtype *kobj = NULL; \
	struct __ctrlmem_kobjctnr_page_freelist_s *fl = NULL; \
 \
	fl = __ctrlmem_kobjpool_freelist_of_sz(page, size); \
	BUG_ON(fl == NULL); \
	index = sysctrlmem_kobjctnr_spawn_##__name(fl, size CTRLMEM_PARAM_APPEND(__args)); \
	if (index >= 0 && index < __CTRLMEM_KOBJCTNR_PIECES_NR) { \
		kobj = (__kobjtype *)&(page->pieces[index]); \
	} \
	return kobj; \
} \
static __kobjtype *do_spawn_##__name(unsigned int size CTRLMEM_PROTO_APPEND(__args)) \
{ \
	struct ctrlmem_page *pos = NULL; \
	struct ctrlmem_page *tmp = NULL; \
	__kobjtype *kobj = NULL; \
	bool try_reorganize = false; \
 \
	(void)raw_rwlock_rdlock(&g_page_lock); \
	dlist_for_each_entry_safe(pos, tmp, &g_page_list, struct ctrlmem_page, node) { \
		kobj = do_kobj_spawn_##__name(pos->page, size CTRLMEM_PARAM_APPEND(__args)); \
		if (kobj != NULL) { \
			break; \
		} else { \
			try_reorganize = true;\
		} \
	} \
	(void)raw_rwlock_unlock(&g_page_lock); \
	/* When kobj is NULL, all pages are used up, we do not need to reorganize. */ \
	if ((kobj != NULL) && try_reorganize && (raw_rwlock_trywrlock(&g_page_lock) == E_HM_OK)) { \
		move_full_pages_to_tail(&g_page_list, size); \
		(void)raw_rwlock_unlock(&g_page_lock); \
	} \
\
	return kobj; \
} \
__kobjtype *hm_ctrlmem_kobjpool_spawn_##__name(CTRLMEM_PROTO(__args)) \
{ \
	__kobjtype *kobj = NULL; \
	bool succ = true; \
 \
	do { \
		kobj = do_spawn_##__name(__size CTRLMEM_PARAM_APPEND(__args)); \
		if (kobj == NULL) { \
			succ = hm_ctrlmem_alloc_page(); \
		} \
	} while (kobj == NULL && succ); \
 \
	return kobj; \
}

define_ctrlmem_kobj_spawn(polllist, struct __ctrlmem_kobj_polllist_s, __CTRLMEM_KOBJCTNR_KOBJSZ_POLLLIST, \
		CTRLMEM_ARGS(3, unsigned long, file, __u32, listener_cnt, vatomic32_t *, revent))
define_ctrlmem_kobj_spawn(sysevent, struct __ctrlmem_kobj_sysevent_s, __CTRLMEM_KOBJCTNR_KOBJSZ_SYSEVENT, \
		CTRLMEM_ARGS(0))

static void hm_ctrlmem_get_freeable_pages(struct dlist_node *page_list, struct dlist_node *freeable_pages)
{
	struct ctrlmem_page *pos, *tmp;

	/*
	 * In most cases, there is only one empty page in list.
	 * Even if there are many empty pages, it doesn't matter, because it looks for empty pages when applying.
	 */
	dlist_for_each_entry_safe(pos, tmp, page_list, struct ctrlmem_page, node) {
		if (page_is_freeable(pos->page)) {
			dlist_delete(&pos->node);
			dlist_insert(freeable_pages, &pos->node);
			break;
		}
	}
}


#define MAX_DELETE_TIMES 100
int hm_ctrlmem_kobjpool_destroy(void *kobj)
{
	int result = E_HM_INVAL;
	int try = 0;
	bool try_free = false;
	DLIST_HEAD(freeable_pages);
	struct ctrlmem_page *free_pos, *free_tmp;

	if (kobj != NULL) {
		(void)raw_rwlock_rdlock(&g_page_lock);
		struct __ctrlmem_kobjctnr_page_s *page = __ctrlmem_kobjctnr_page_ptr_to_page(kobj);
		unsigned int npiece_kobj = __ctrlmem_kobjctnr_page_ptr_to_npiece(kobj);
		if (__ctrlmem_kobjctnr_npiece_is_valid(npiece_kobj)) {
			result = sysctrlmem_kobj_destroy(page, npiece_kobj);
			while (result == E_HM_AGAIN && try < MAX_DELETE_TIMES) {
				try++;
				(void)raw_rwlock_unlock(&g_page_lock);
				cpu_relax();
				(void)raw_rwlock_rdlock(&g_page_lock);
				result = sysctrlmem_kobj_destroy(page, npiece_kobj);
			}

			if (result == E_HM_AGAIN && try == MAX_DELETE_TIMES) {
				hm_error("failed to delete ctrlmem polllist\n");
				result = E_HM_INVAL;
			}
		} else {
			result = E_HM_INVAL;
		}
		if (result > 0 && page_is_freeable(page)) {
			try_free = true;
		}
		(void)raw_rwlock_unlock(&g_page_lock);
		BUG_ON(result == 0);
		/*
		 * try_free indicates that there may be freeable-pages in the g_page_list.
		 * After try_free is set to true, a new FD may apply for kobj.
		 * In this case, hm_ctrlmem_get_freeable_pages may fail to find freeable-pages.
		 * But that's okay, try_free just provides an opportunity to check pages in g_page_list.
		 */
		if (result > 0 && try_free && (raw_rwlock_trywrlock(&g_page_lock) == E_HM_OK)) {
			hm_ctrlmem_get_freeable_pages(&g_page_list, &freeable_pages);
			(void)raw_rwlock_unlock(&g_page_lock);
		}
		dlist_for_each_entry_safe(free_pos, free_tmp, &freeable_pages, struct ctrlmem_page, node) {
			dlist_delete(&free_pos->node);
			hm_ctrlmem_free_page(free_pos);
		}
	}

	return (result < 0) ? result : E_HM_OK;
}
