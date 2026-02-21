/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Provide basic data structure for coobj
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_LIST_H
#define KLIBS_COOBJ_LIST_H

#include <hmasm/types.h>
#include <hmasm/memory.h>
#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <lib/utils.h>

/* Virtual memory for coobj is reserved only on the 64-bit platforms.
 * Currently, we use the high-order bits of the 64-bit index to store the
 * real index of objects in the coobj allocator and the low-order bits to store the
 * offset of obj relative to the start address of the reserved memory area.
 *
 * The location of each dnode/lnode in the virtual memory will be specified later,
 * which will establish the direct mapping between idx and offset, so that 32-bit
 * can be compatible with the index in both fast and slow paths.
 */
#ifdef __aarch64__
typedef __u64 __relpos_t;
#else
typedef __u32 __relpos_t;
#endif
#define ____COOBJ_RELPOS_BITS		(sizeof(__relpos_t) * 8)
#define ____COOBJ_INVALID_POS		((__relpos_t)-1)

static inline
void *____coobj_relpos_raw(
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	return (unsigned char *)page + tepar_type_size * relpos;
}

/**************************************************
 * single linked list
 **************************************************/
struct coobj_slist_s {
	__relpos_t head;
	__relpos_t size;
};

#define ____COOBJ_SLIST_INIT \
	(struct coobj_slist_s) { \
		.head = ____COOBJ_INVALID_POS, \
		.size = 0, \
	}

struct coobj_slist_node_s {
	__relpos_t next;
};

static inline
void ____coobj_slist_init(struct coobj_slist_s *slist)
{
	slist->head = ____COOBJ_INVALID_POS;
	slist->size = 0;
}

static inline
struct coobj_slist_node_s *____coobj_relpos_snode(
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	return (struct coobj_slist_node_s *)____coobj_relpos_raw(page, tepar_type_size, relpos);
}

static inline
void ____coobj_slist_add(
	struct coobj_slist_s *slist,
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	struct coobj_slist_node_s *snode = ____coobj_relpos_snode(page, tepar_type_size, relpos);
	snode->next = slist->head;
	slist->head = relpos;
	slist->size += 1;
}

static inline
__relpos_t ____coobj_slist_pop(
	struct coobj_slist_s *slist,
	void *page,
	__relpos_t tepar_type_size)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	if (slist->head != ____COOBJ_INVALID_POS) {
		relpos = slist->head;
		struct coobj_slist_node_s *snode = ____coobj_relpos_snode(page, tepar_type_size, relpos);
		slist->head = snode->next;
		slist->size -= 1;
	}
	return relpos;
}

static inline
void ____coobj_slist_init_link(
	struct coobj_slist_s *slist,
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order)
{
	for (__relpos_t relpos = 0, relpos_count = (__relpos_t)1 << tepar_chunk_order;
	     relpos < relpos_count; relpos += 1) {
		____coobj_slist_add(slist, page, tepar_type_size, relpos_count - 1 - relpos);
	}
}

static inline
__bool ____coobj_slist_verify(
	struct coobj_slist_s *slist,
	void *page,
	__relpos_t tepar_type_size)
{
	__relpos_t relpos = slist->head;
	__relpos_t count = slist->size;
	while ((relpos != ____COOBJ_INVALID_POS) && (count != 0)) {
		relpos = ____coobj_relpos_snode(page, tepar_type_size, relpos)->next;
		count -= 1;
	}
	return (relpos == ____COOBJ_INVALID_POS) && (count == 0);
}

/**************************************************
 * double linked list node
 **************************************************/
/* NOTE: although coobj_slist_s and coobj_dlist_s are identical by the declaration,
   we still use different definition to explicitly distinguish between them, so that
   we could leverage the C language's type system to avoid some silent bug. */
struct coobj_dlist_s {
	__relpos_t head;
	__relpos_t size;
};

#define ____COOBJ_DLIST_INIT \
	(struct coobj_dlist_s) { \
		.head = ____COOBJ_INVALID_POS, \
		.size = 0, \
	}
struct coobj_dlist_node_s {
	__relpos_t next;
	__relpos_t prev;
};
#define ____COOBJ_DLIST_NODE_INIT \
	((struct coobj_dlist_node_s) { \
		.next = ____COOBJ_INVALID_POS, \
		.prev = ____COOBJ_INVALID_POS, \
	})

static inline
void ____coobj_dlist_init(struct coobj_dlist_s *dlist)
{
	dlist->head = ____COOBJ_INVALID_POS;
	dlist->size = 0;
}

static inline
struct coobj_dlist_node_s *____coobj_relpos_dnode(
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	return (struct coobj_dlist_node_s *)____coobj_relpos_raw(page, tepar_type_size, relpos);
}

static inline
void ____coobj_dlist_add(
	struct coobj_dlist_s *dlist,
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	if (dlist->head != ____COOBJ_INVALID_POS) {
		struct coobj_dlist_node_s *dhead = ____coobj_relpos_dnode(page, tepar_type_size, dlist->head);
		dhead->prev = relpos;
	}
	struct coobj_dlist_node_s *dnode = ____coobj_relpos_dnode(page, tepar_type_size, relpos);
	dnode->next = dlist->head;
	dnode->prev = ____COOBJ_INVALID_POS;
	dlist->size += 1;
	dlist->head = relpos;
}

static inline __always_inline
void ____coobj_dlist_del(
	struct coobj_dlist_s *dlist,
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	struct coobj_dlist_node_s *dnode = ____coobj_relpos_dnode(page, tepar_type_size, relpos);
	if (dnode->prev != ____COOBJ_INVALID_POS) {
		____coobj_relpos_dnode(page, tepar_type_size, dnode->prev)->next = dnode->next;
	}
	if (dnode->next != ____COOBJ_INVALID_POS) {
		____coobj_relpos_dnode(page, tepar_type_size, dnode->next)->prev = dnode->prev;
	}
	if (relpos == dlist->head) {
		dlist->head = dnode->next;
	}
	dlist->size -= 1;
}

static inline
__relpos_t ____coobj_dlist_pop(
	struct coobj_dlist_s *dlist,
	void *page,
	__relpos_t tepar_type_size)
{
	__relpos_t relpos = dlist->head;
	if (relpos != ____COOBJ_INVALID_POS) {
		____coobj_dlist_del(dlist, page, tepar_type_size, relpos);
	}
	return relpos;
}

static inline
void ____coobj_dlist_init_link(
	struct coobj_dlist_s *dlist,
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order)
{
	for (__relpos_t relpos = 0, relpos_count = (__relpos_t)1 << tepar_chunk_order;
	     relpos < relpos_count; relpos += 1) {
		____coobj_dlist_add(dlist, page, tepar_type_size, relpos_count - 1 - relpos);
	}
}

static inline
__bool ____coobj_dlist_verify_prev(
	struct coobj_dlist_s *dlist,
	void *page,
	__relpos_t tepar_type_size,
	__relpos_t relpos)
{
	__bool check = __true;
	__relpos_t prev = ____coobj_relpos_dnode(page, tepar_type_size, relpos)->prev;
	if ((prev != ____COOBJ_INVALID_POS) && (____coobj_relpos_dnode(page, tepar_type_size, prev)->next != relpos)) {
		check = __false;
	}
	return check;
}

static inline
__bool ____coobj_dlist_verify(
	struct coobj_dlist_s *dlist,
	void *page,
	__relpos_t tepar_type_size)
{
	__relpos_t relpos = dlist->head;
	__relpos_t count = dlist->size;
	while ((relpos != ____COOBJ_INVALID_POS) && (count != 0) &&
		____coobj_dlist_verify_prev(dlist, page, tepar_type_size, relpos)) {
		relpos = ____coobj_relpos_dnode(page, tepar_type_size, relpos)->next;
		count -= 1;
	}
	return (relpos == ____COOBJ_INVALID_POS) && (count == 0);
}

#endif
