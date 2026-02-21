/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 05 12:02:14 2023
 */

#include <stddef.h>
#include <libhmsrv_sys/hm_ctrlmem_eventpoll.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>
#include <libhmsrv_sys/hm_kshm.h>
#include <hmkernel/pgstr/pgstr.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>

#define BITS_PER_BYTE 8U
#define PAGE_OFFSET(page, revent) ((unsigned int)((((uintptr_t)(revent)) - ((uintptr_t)(page))) / sizeof(vatomic32_t)))

static struct raw_mutex g_revents_lock = RAW_MUTEX_INITIALIZER;
static DLIST_HEAD(g_revents_pages);
static DLIST_HEAD(g_revents_full_pages);

static void *alloc_one_revents_page(int *ret_shm_id)
{
	int shm_id;
	size_t ret_size;
	void *addr = NULL;

	shm_id = hm_kshm_open(__PGSTR_TYPE_REVENTSPG, 0U,
			      PAGE_SIZE, SHM_O_CREAT | SHM_O_RDWR, &ret_size);
	if (shm_id >= 0) {
		addr = hm_mem_mmap(NULL, PAGE_SIZE, MPROT_READ | PROT_WRITE,
				   MFLAG_MAP_SHARED | MFLAG_MAP_ANON | MFLAG_MAP_POPULATE, shm_id, 0);
		if (addr == MAP_FAILED) {
			(void)hm_kshm_close(shm_id);
			(void)hm_kshm_unlink(shm_id);
			addr = NULL;
		}
	}

	if (addr != NULL) {
		*ret_shm_id = shm_id;
		NOFAIL(memset_s(addr, PAGE_SIZE, 0, PAGE_SIZE));
	} else {
		hm_error("failed to mmap page for eventpoll revents\n");
	}
	return addr;
}

static struct revents_page_s *create_revents_page(void)
{
	struct revents_page_s *new_page = NULL;

	new_page = (struct revents_page_s *)malloc(sizeof(struct revents_page_s));
	if (new_page != NULL) {
		if (unlikely(memset_s(new_page->bitmap, REVENTS_PER_PAGE / BITS_PER_BYTE,
			0, REVENTS_PER_PAGE / BITS_PER_BYTE) != 0)) {
			hm_error("failed to memset bitmap\n");
			goto err;
		}
		new_page->page = alloc_one_revents_page(&new_page->shm_id);
		if (new_page->page == NULL) {
			hm_error("failed to alloc revent_page\n");
			goto err;
		}
	}
	return new_page;
err:
	if (new_page != NULL) {
		free(new_page);
	}
	return NULL;
}

static vatomic32_t *get_unused_eventpoll_revents(struct revents_page_s **revent_page)
{
	vatomic32_t *ret = NULL;
	unsigned int bit_idx = 0U;
	struct revents_page_s *first_page = NULL;
	struct revents_page_s *new_page = NULL;

	raw_mutex_lock(&g_revents_lock);

	if (!dlist_empty(&g_revents_pages)) {
		/*
		 * once the page is full, it will be moved to g_revents_full_pages,
		 * so we can find an unused revent on the first page.
		 */
		first_page = dlist_first_entry(&g_revents_pages, struct revents_page_s, node);
		bit_idx = raw_bitmap_find_first_zero(first_page->bitmap, REVENTS_PER_PAGE);
		BUG_ON(bit_idx >= REVENTS_PER_PAGE);
		ret = (vatomic32_t *)(first_page->page) + bit_idx;
		*revent_page = first_page;
		vatomic32_write(ret, 0);
		(void)raw_bitmap_set_bit(first_page->bitmap, REVENTS_PER_PAGE, bit_idx);
		first_page->used_cnt++;
		if (first_page->used_cnt >= (int)REVENTS_PER_PAGE) {
			first_page->used_cnt = (int)REVENTS_PER_PAGE;
			dlist_delete(&first_page->node);
			dlist_insert(&g_revents_full_pages, &first_page->node);
		}
	}
	raw_mutex_unlock(&g_revents_lock);

	if (ret == NULL) {
		new_page = create_revents_page();
		if (new_page != NULL) {
			raw_mutex_lock(&g_revents_lock);
			dlist_insert_head(&g_revents_pages, &new_page->node);
			*revent_page = new_page;
			ret = (vatomic32_t *)(new_page->page);
			vatomic32_write(ret, 0);
			new_page->used_cnt = 1;
			(void)raw_bitmap_set_bit(new_page->bitmap, REVENTS_PER_PAGE, 0);
			raw_mutex_unlock(&g_revents_lock);
		}
	}
	return ret;
}

static void free_revents_page(struct revents_page_s *page)
{
	(void)hm_mem_munmap(page->page, PAGE_SIZE);
	(void)hm_kshm_close(page->shm_id);
	(void)hm_kshm_unlink(page->shm_id);
	free(page);
}

static int free_one_eventpoll_revents(vatomic32_t *revent, struct revents_page_s *revents_page)
{
	unsigned int bit_idx;
	int err = E_HM_OK;
	struct revents_page_s *freeable_revents_page = NULL;

	bit_idx = PAGE_OFFSET(revents_page->page, revent);
	raw_mutex_lock(&g_revents_lock);
	if (raw_bitmap_test_bit(revents_page->bitmap, REVENTS_PER_PAGE, bit_idx) != 0) {
		(void)raw_bitmap_clear_bit(revents_page->bitmap, REVENTS_PER_PAGE, bit_idx);
		revents_page->used_cnt--;
	} else {
		hm_error("unused idx provided %u\n", bit_idx);
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		if (revents_page->used_cnt <= 0) {
			freeable_revents_page = revents_page;
			dlist_delete(&revents_page->node);
		} else if (revents_page->used_cnt == (REVENTS_PER_PAGE - 1)) {
			dlist_delete(&revents_page->node);
			dlist_insert(&g_revents_pages, &revents_page->node);
		}
	}
	raw_mutex_unlock(&g_revents_lock);
	if (freeable_revents_page != NULL) {
		free_revents_page(freeable_revents_page);
	}

	return err;
}

static vatomic32_t *alloc_one_revent_slot(struct revents_page_s **revents_page)
{
	vatomic32_t *ret = NULL;

	if (revents_page == NULL) {
		return NULL;
	}
	ret = get_unused_eventpoll_revents(revents_page);
	return ret;
}

struct polllist_s *hm_create_polllist(unsigned long file, __u32 listener_cnt)
{
	struct polllist_s *polllist = NULL;
	int err = E_HM_OK;
	struct revents_page_s *revents_page = NULL;

	if (err == E_HM_OK) {
		polllist = (struct polllist_s *)malloc(sizeof(struct polllist_s));
		if (polllist == NULL) {
			hm_error("failed to malloc polllist\n");
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		polllist->revents = alloc_one_revent_slot(&revents_page);
		if (polllist->revents != NULL) {
			polllist->revents_page = revents_page;
		} else {
			free(polllist);
			hm_error("failed to alloc revent slot for polllist\n");
			polllist = NULL;
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		polllist->polllist = hm_ctrlmem_kobjpool_spawn_polllist(file, listener_cnt, polllist->revents);
		if (polllist->polllist == NULL && polllist->revents_page != NULL) {
			(void)free_one_eventpoll_revents(polllist->revents, polllist->revents_page);
			free(polllist);
			hm_error("failed to spawn polllist ctrlmem\n");
			err = E_HM_INVAL;
			polllist = NULL;
		}
	}
	return polllist;
}

int hm_destroy_polllist(struct polllist_s *polllist)
{
	int err = E_HM_OK;

	if (polllist == NULL) {
		err = E_HM_INVAL;
		hm_error("null polllist provided\n");
	}

	if (err == E_HM_OK) {
		err = hm_ctrlmem_kobjpool_destroy(polllist->polllist);
		if (err != E_HM_OK) {
			hm_error("failed to destroy polllist\n");
		}
	}

	/*
	 * free_one_eventpoll_revents must be after hm_ctrlmem_kobjpool_destroy,
	 * because polllist->polllist has a revents-refcnt
	 */
	if (err == E_HM_OK) {
		err = free_one_eventpoll_revents(polllist->revents, polllist->revents_page);
		if (err != E_HM_OK) {
			hm_error("failed to free revent slot polllist idx: %u %s\n",
				 PAGE_OFFSET(polllist->revents_page->page, polllist->revents), hmstrerror(err));
		} else {
			free(polllist);
		}
	}

	return err;
}

int hm_update_polllist_events(struct polllist_s *polllist, __u32 revents)
{
	if (polllist == NULL) {
		return E_HM_INVAL;
	}

	vatomic32_write(polllist->revents, revents);
	return sysctrlmem_kobjctnr_sync_polllist_revents(polllist->polllist, revents);
}

int hm_cleanup_polllist_events(struct polllist_s *polllist, __u32 revents)
{
	__u32 expt, orig, new_val;
	if (polllist == NULL) {
		return E_HM_INVAL;
	}

	orig = vatomic32_read(polllist->revents);
	do {
		expt = orig;
		new_val = orig & (~revents);
		orig = vatomic32_cmpxchg(polllist->revents, expt, new_val);
	} while (expt != orig);
	return sysctrlmem_kobjctnr_clean_polllist_revents(polllist->polllist, revents);
}
