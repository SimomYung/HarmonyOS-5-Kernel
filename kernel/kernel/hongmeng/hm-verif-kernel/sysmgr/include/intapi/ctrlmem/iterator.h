/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ctrlmem iterator related functions in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 10 15:15:51 2023
 */
#ifndef INTAPI_CTRLMEM_ITERATOR_H
#define INTAPI_CTRLMEM_ITERATOR_H

#include <libhmsync/raw_mutex.h>
#include <hmkernel/ctrlmem/iterator.h>
#include <mm/pgtable.h>

struct iterator_s {
	struct raw_mutex lock;
	struct __ctrlmem_iterator_page_s *pg;
};

#define CTRLMEM_NR_ITERATOR 8

struct __ctrlmem_iterator_page_s *iteratorpg_alloc(void);

void iteratorpg_free(struct __ctrlmem_iterator_page_s *iter_page);

void iterator_page_init(void);

struct iterator_s *iterator_acquire(void);

void iterator_release(struct iterator_s *iter);

struct __ctrlmem_kobj_vspace_s;

static inline void iterator_init_unmap_head(struct __ctrlmem_iterator_page_s *iter_pg,
					    struct __ctrlmem_kobj_vspace_s *kobj,
					    uintptr_t tlbg_va)
{
	struct __ctrlmem_iterator_head_s *head = NULL;

	head = &iter_pg->head;
	head->t.type = (uint32_t)__CTRLMEM_ITERATOR_BATCH_TLBG_UNMAP;
	head->u.unmap_head.kobj_vspace = ptr_cast(kobj, void);
	head->u.unmap_head.tlbg = ulong_to_ptr(tlbg_va, void);
	head->u.unmap_head.num = 0;
}

static inline void iterator_write_unmap_piece(struct __ctrlmem_iterator_page_s *iter_pg,
					      pte_t *ppte, uintptr_t vaddr,
					      uint64_t paddr)
{
	struct __ctrlmem_iterator_piece_s *piece = NULL;
	struct __ctrlmem_iterator_head_s *head = NULL;

	head = &iter_pg->head;
	piece = &iter_pg->pieces[head->u.unmap_head.num];
	piece->u.unmap_piece.ppte = ptr_to_void(ppte);
	piece->u.unmap_piece.va = vaddr;
	piece->u.unmap_piece.pa = paddr;
	head->u.unmap_head.num++;
}

static inline void iterator_read_unmap_piece(struct __ctrlmem_iterator_page_s *iter_pg,
					      uintptr_t *vaddr, uint64_t *paddr, uint32_t idx)
{
	struct __ctrlmem_iterator_piece_s *piece = NULL;

	piece = &iter_pg->pieces[idx];
	*vaddr = piece->u.unmap_piece.va;
	*paddr = piece->u.unmap_piece.pa;
}

static inline unsigned int iterator_read_succ_unmap_num(struct __ctrlmem_iterator_page_s *iter_pg)
{
	return iter_pg->head.u.unmap_head.stop_num;
}

static inline bool iterator_unmap_is_empty(const struct __ctrlmem_iterator_page_s *iter_pg)
{
	return (iter_pg->head.u.unmap_head.num == 0);
}

static inline bool iterator_unmap_is_full(const struct __ctrlmem_iterator_page_s *iter_pg)
{
	return (iter_pg->head.u.unmap_head.num == __CTRLMEM_NR_ITERATOR_PIECE);
}

static inline void iterator_write_unmap_head_num(struct __ctrlmem_iterator_page_s *iter_pg, uint32_t num)
{
	iter_pg->head.u.unmap_head.num = num;
}

#endif
