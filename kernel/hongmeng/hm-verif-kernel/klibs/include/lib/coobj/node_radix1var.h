/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Data structures for variable-sized object allocator
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 21 15:34:34 2023 +0800
 */
#ifndef KLIBS_COOBJ_NODE_RADIX1VAR_H
#define KLIBS_COOBJ_NODE_RADIX1VAR_H

#include <lib/coobj/node_base.h>
#include <libhmlog/hmlog.h>

#define NUM_RADIX1VAR_CHUNK_OBJ 480U
#define RADIX1VAR_REAL_DATA_OFFSET 512U
#define RADIX1VAR_META_REGION_SHIFT 9U
#define RADIX1VAR_META_DATA_SIZE 1U
#define RADIX1VAR_MIN_OBJ_SIZE 16U
#define RADIX1VAR_MIN_OBJ_SHIFT 4U
#define RADIX1VAR_MAX_OBJ_SIZE 256U
#define RADIX1VAR_MAX_OBJ_SHIFT 8U
#define RADIX1VAR_CHUNK_SIZE 8192U
#define RADIX1VAR_CHUNK_SHIFT 13U

#define L_RADIX1VAR_MAX_OBJ_SIZE 4096U
#define L_RADIX1VAR_OBJ_SIZE_NUM 4U

static inline
__bool ____coobj_radix1var_is_large_obj(__relpos_t relpos)
{
	__bool large_obj = __false;
	if ((relpos & ((1 << (RADIX1VAR_CHUNK_SHIFT - RADIX1VAR_MIN_OBJ_SHIFT)) - 1)) == 0) {
		large_obj = __true;
	}
	return large_obj;
}
static inline
__relpos_t ____coobj_radix1var_size_to_list_idx(__relpos_t size)
{
	__relpos_t idx = 0;
	__relpos_t size_class = (size - 1) >> RADIX1VAR_MIN_OBJ_SHIFT;

	while (size_class > 0) {
		size_class >>= 1;
		idx++;
	}
	return idx;
}

static inline
__bool ____coobj_radix1var_free_list_check(
	__relpos_t size, __relpos_t *obj_idx, __relpos_t *target_idx,
	struct ____coobj_radix1var_free_list *list)
{
	__bool check_result = __false;
	*target_idx = ____coobj_radix1var_size_to_list_idx(size);

	if (*target_idx >= NUM_RADIX1VAR_OBJ_SIZE) {
		if (list->free_list[*target_idx].size > 0) {
			check_result = __true;
			*obj_idx = *target_idx;
		}
	} else {
		for (__relpos_t i = *target_idx; i < NUM_RADIX1VAR_OBJ_SIZE; i++) {
			if (list->free_list[i].size > 0) {
				check_result = __true;
				*obj_idx = i;
				break;
			}
		}
	}
	return check_result;
}

static inline
struct ____coobj_radix1var_meta_data_s *____coobj_radix1var_relpos_meta_data(
	void *page, __relpos_t meta_size, __relpos_t idx)
{
	return (struct ____coobj_radix1var_meta_data_s *)____coobj_relpos_raw(page, meta_size, idx);
}

static inline
void ____coobj_radix1var_meta_data_update_free(
	void *page, __relpos_t meta_size, __relpos_t idx, __u8 free, unsigned int flags)
{
	struct ____coobj_radix1var_meta_data_s *meta = ____coobj_radix1var_relpos_meta_data(page, meta_size, idx);
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		struct ____coobj_radix1var_meta_data_s meta_tmp = *meta;
		meta_tmp.free = free;
		hm_hkip_rowm_cpy_nofail(meta, &meta_tmp, sizeof(meta_tmp));
	} else {
		meta->free = free;
	}
#else
	meta->free = free;
#endif
}

static inline
void ____coobj_radix1var_meta_data_update(
	void *page, __relpos_t meta_size, __relpos_t idx, __u8 len, __u8 free, unsigned int flags)
{
	struct ____coobj_radix1var_meta_data_s *meta = ____coobj_radix1var_relpos_meta_data(page, meta_size, idx);
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		struct ____coobj_radix1var_meta_data_s meta_tmp = *meta;
		meta_tmp.len = len;
		meta_tmp.free = free;
		hm_hkip_rowm_cpy_nofail(meta, &meta_tmp, sizeof(meta_tmp));
	} else {
		meta->len = len;
		meta->free = free;
	}
#else
	meta->len = len;
	meta->free = free;
#endif
}

static inline
void ____coobj_radix1var_meta_data_update_len(
	void *page, __relpos_t meta_size, __relpos_t idx, __u8 len, unsigned int flags)
{
	struct ____coobj_radix1var_meta_data_s *meta = ____coobj_radix1var_relpos_meta_data(page, meta_size, idx);
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		struct ____coobj_radix1var_meta_data_s meta_tmp = *meta;
		meta_tmp.len = len;
		hm_hkip_rowm_cpy_nofail(meta, &meta_tmp, sizeof(meta_tmp));
	} else {
		meta->len = len;
	}
#else
	meta->len = len;
#endif
}

static inline
void ____coobj_radix1var_free_list_del_head(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct coobj_dlist_s *dlist, void *page,
	__relpos_t tepar_type_size, __relpos_t tepar_chunk_order, __relpos_t chunk_pos, unsigned int flags)
{
	struct coobj_dlist_node_s *dhead = ____coobj_relpos_dnode(page, tepar_type_size, chunk_pos);

	dlist->head = dhead->next;
	dlist->size -= 1;

	if (dhead->next != ____COOBJ_INVALID_POS) {
		struct coobj_dlist_node_s *next = ____coobj_relpos_dnode(
			lnode->child_dnode_list[dhead->next >> tepar_chunk_order].page_uva, tepar_type_size,
				dhead->next & ____coobj_order_mask(tepar_chunk_order));
#ifdef CONFIG_HKIP_SECCOMP_PROT
		if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
			__relpos_t invalid_pos = ____COOBJ_INVALID_POS;
			hm_hkip_rowm_cpy_nofail(&next->prev, &invalid_pos, sizeof(invalid_pos));
			hm_hkip_rowm_cpy_nofail(&dhead->next, &invalid_pos, sizeof(invalid_pos));
		} else {
			next->prev = ____COOBJ_INVALID_POS;
			dhead->next = ____COOBJ_INVALID_POS;
		}
#else
		next->prev = ____COOBJ_INVALID_POS;
		dhead->next = ____COOBJ_INVALID_POS;
#endif
	}
}

static inline
__relpos_t ____coobj_radix1var_free_list_pop_head(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct coobj_dlist_s *dlist,
	__relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	__relpos_t relpos = dlist->head;
	if (relpos != ____COOBJ_INVALID_POS) {
		__relpos_t chunk_pos = relpos & ____coobj_order_mask(tepar_chunk_order);
		void *cur_page = lnode->child_dnode_list[relpos >> tepar_chunk_order].page_uva;

		____coobj_radix1var_free_list_del_head(lnode, dlist, cur_page, tepar_type_size,
						       tepar_chunk_order, chunk_pos, flags);

		____coobj_radix1var_meta_data_update_free(cur_page, RADIX1VAR_META_DATA_SIZE,
		chunk_pos - RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size, 1, flags);
	}
	return relpos;
}

static inline
void ____coobj_radix1var_free_list_add_head(
	struct ____coobj_radix1var_uholder_lnode_s *lnode,
	struct coobj_dlist_s *dlist, void *page, __relpos_t relpos,
	__relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	if (dlist->head != ____COOBJ_INVALID_POS) {
		struct coobj_dlist_node_s *dhead = ____coobj_relpos_dnode(
			lnode->child_dnode_list[dlist->head >> tepar_chunk_order].page_uva, tepar_type_size,
				dlist->head & ____coobj_order_mask(tepar_chunk_order));
#ifdef CONFIG_HKIP_SECCOMP_PROT
		if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
			hm_hkip_rowm_cpy_nofail(&dhead->prev, &relpos, sizeof(relpos));
		else
#endif
		dhead->prev = relpos;
	}

	__relpos_t chunk_pos = relpos & ____coobj_order_mask(tepar_chunk_order);
	struct coobj_dlist_node_s *dnode = ____coobj_relpos_dnode(page, tepar_type_size, chunk_pos);
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		__relpos_t invalid_pos = ____COOBJ_INVALID_POS;
		hm_hkip_rowm_cpy_nofail(&dnode->next, &dlist->head, sizeof(dlist->head));
		hm_hkip_rowm_cpy_nofail(&dnode->prev, &invalid_pos, sizeof(invalid_pos));
	} else {
		dnode->next = dlist->head;
		dnode->prev = ____COOBJ_INVALID_POS;
	}
#else
	dnode->next = dlist->head;
	dnode->prev = ____COOBJ_INVALID_POS;
#endif
	dlist->size += 1;
	dlist->head = relpos;
}

static inline
__bool ____coobj_radix1var_uholder_object_alloc_check_nolock(
	__relpos_t size, __relpos_t *obj_idx, __relpos_t *target_idx, struct ____coobj_radix1var_uholder_lnode_s *lnode,
	struct ____coobj_radix1var_free_list *free_list, __relpos_t tepar_chunk_order, __relpos_t tepar_level1_order)
{
	__bool check_result = __false;
	if (____coobj_radix1var_free_list_check(size, obj_idx, target_idx, free_list)) {
		/* If check returns true, a 2^obj_idx obj can be allocated from free_list */
		check_result = __true;
	} else if (____coobj_relpos_check(lnode->atords.shrink_list.head, tepar_level1_order)) {
		/* If the shrink_list is not empty, the allocation could from shrink_list */
		check_result = __true;
	}
	return check_result;
}

static inline
void ____coobj_radix1var_local_free_list_link(
	void *page, __relpos_t baseof_chunk, __relpos_t chunk_relpos,
	__relpos_t prev_chunk_relpos, __relpos_t next_chunk_relpos, __relpos_t tepar_type_size, unsigned int flags)
{
	struct coobj_dlist_node_s *dnode = ____coobj_relpos_dnode(page, tepar_type_size, chunk_relpos);
#ifdef CONFIG_HKIP_SECCOMP_PROT
	__relpos_t next = baseof_chunk + next_chunk_relpos;
	__relpos_t prev = baseof_chunk + prev_chunk_relpos;
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		hm_hkip_rowm_cpy_nofail(&dnode->next, &next, sizeof(next));
		hm_hkip_rowm_cpy_nofail(&dnode->prev, &prev, sizeof(prev));
	} else {
		dnode->next = next;
		dnode->prev = prev;
	}
#else
	dnode->next = baseof_chunk + next_chunk_relpos;
	dnode->prev = baseof_chunk + prev_chunk_relpos;
#endif
}

/* Chunk layout of the temporary solution
 * Meta data	data
 * 512B 	512B + 1k + 2k + 4k
 */
static inline
void ____coobj_radix1var_free_list_batch_add_large(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_free_list *free_list, void *page,
	__relpos_t level1_relpos, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	__relpos_t baseof_chunk = level1_relpos << tepar_chunk_order;
	__relpos_t meta_pos_offset = RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size;
	for (int i = 0; i < (int)L_RADIX1VAR_OBJ_SIZE_NUM; i++) {
		__relpos_t node_pos = baseof_chunk + (1 << i) * meta_pos_offset;
		struct coobj_dlist_node_s *node = ____coobj_relpos_dnode(
			page, tepar_type_size, (1 << i) * meta_pos_offset);
		/* update global free_list */
		struct coobj_dlist_s *dlist = &free_list->free_list[NUM_RADIX1VAR_OBJ_SIZE + i];
		if (dlist->head != ____COOBJ_INVALID_POS) {
#ifdef CONFIG_HKIP_SECCOMP_PROT
			if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
				hm_hkip_rowm_cpy_nofail(&node->next, &dlist->head, sizeof(dlist->head));
			else
#endif
			node->next = dlist->head;
			__relpos_t level1_relpos = dlist->head >> tepar_chunk_order;
			__relpos_t chunk_relpos = dlist->head & ____coobj_order_mask(tepar_chunk_order);
			struct coobj_dlist_node_s *dhead = ____coobj_relpos_dnode(
				lnode->child_dnode_list[level1_relpos].page_uva, tepar_type_size, chunk_relpos);
#ifdef CONFIG_HKIP_SECCOMP_PROT
			if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
				hm_hkip_rowm_cpy_nofail(&dhead->prev, &node_pos, sizeof(node_pos));
			else
#endif
			dhead->prev = node_pos;
		}
		dlist->size += 1;
		dlist->head = node_pos;
		__relpos_t idx = ((1 << i) - 1) * meta_pos_offset;
		____coobj_radix1var_meta_data_update(page, RADIX1VAR_META_DATA_SIZE, idx,
						     NUM_RADIX1VAR_OBJ_SIZE + i + 1, 0, flags);
	}
}

static inline
void ____coobj_radix1var_free_list_batch_add(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_free_list *free_list, void *page,
	__relpos_t level1_relpos, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	__relpos_t baseof_chunk = level1_relpos << tepar_chunk_order;
	__relpos_t interval = RADIX1VAR_MAX_OBJ_SIZE / tepar_type_size;
	__relpos_t meta_pos_offset = RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size;
	__relpos_t idx;

	for (idx = interval; idx < NUM_RADIX1VAR_CHUNK_OBJ - interval; idx += interval) {
		__relpos_t pos = meta_pos_offset + idx;
		____coobj_radix1var_local_free_list_link(page, baseof_chunk, pos,
			pos - interval, pos + interval, tepar_type_size, flags);
	}

	struct coobj_dlist_node_s *first = ____coobj_relpos_dnode(page, tepar_type_size, meta_pos_offset);
	__relpos_t first_pos = baseof_chunk + meta_pos_offset;
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		__relpos_t first_next = first_pos + interval;
		__relpos_t first_prev = ____COOBJ_INVALID_POS;
		hm_hkip_rowm_cpy_nofail(&first->next, &first_next, sizeof(first_next));
		hm_hkip_rowm_cpy_nofail(&first->prev, &first_prev, sizeof(first_prev));
	} else {
		first->next = first_pos + interval;
		first->prev = ____COOBJ_INVALID_POS;
	}
#else
	first->next = first_pos + interval;
	first->prev = ____COOBJ_INVALID_POS;
#endif

	struct coobj_dlist_node_s *last = ____coobj_relpos_dnode(page, tepar_type_size, meta_pos_offset + idx);
	__relpos_t last_pos = baseof_chunk + meta_pos_offset + idx;
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0) {
		__relpos_t last_next = ____COOBJ_INVALID_POS;
		__relpos_t last_prev = last_pos - interval;
		hm_hkip_rowm_cpy_nofail(&last->next, &last_next, sizeof(last_next));
		hm_hkip_rowm_cpy_nofail(&last->prev, &last_prev, sizeof(last_prev));
	} else {
		last->next = ____COOBJ_INVALID_POS;
		last->prev = last_pos - interval;
	}
#else
	last->next = ____COOBJ_INVALID_POS;
	last->prev = last_pos - interval;
#endif

	/* update global free_list */
	struct coobj_dlist_s *dlist = &free_list->free_list[NUM_RADIX1VAR_OBJ_SIZE - 1];
	if (dlist->head != ____COOBJ_INVALID_POS) {
#ifdef CONFIG_HKIP_SECCOMP_PROT
		if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
			hm_hkip_rowm_cpy_nofail(&last->next, &dlist->head, sizeof(dlist->head));
		else
#endif
		last->next = dlist->head;
		__relpos_t level1_relpos = dlist->head >> tepar_chunk_order;
		__relpos_t chunk_relpos = dlist->head & ____coobj_order_mask(tepar_chunk_order);
		struct coobj_dlist_node_s *dhead = ____coobj_relpos_dnode(
			lnode->child_dnode_list[level1_relpos].page_uva, tepar_type_size, chunk_relpos);
#ifdef CONFIG_HKIP_SECCOMP_PROT
		if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
			hm_hkip_rowm_cpy_nofail(&dhead->prev, &last_pos, sizeof(last_pos));
		else
#endif
		dhead->prev = last_pos;
	}
	dlist->size += NUM_RADIX1VAR_CHUNK_OBJ / (RADIX1VAR_MAX_OBJ_SIZE / tepar_type_size);
	dlist->head = first_pos;
}

/* New chunk is initilized as the largest objects it can accommodate.
 * To be optimized later according to the real distribution of object size.
 */
static inline
void ____coobj_radix1var_uholder_dnode_extend_page_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, void *page, struct ____coobj_radix1var_free_list *free_list,
	__relpos_t target_idx, __relpos_t level1_relpos, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order,
	unsigned int flags)
{
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
		hm_hkip_rowm_set(page, (uint8_t)0, (size_t)RADIX1VAR_REAL_DATA_OFFSET);
	else
#endif
	memset_s(page, RADIX1VAR_REAL_DATA_OFFSET, 0, RADIX1VAR_REAL_DATA_OFFSET);
	if (target_idx >= NUM_RADIX1VAR_OBJ_SIZE) {
		____coobj_radix1var_free_list_batch_add_large(lnode, free_list, page, level1_relpos,
			tepar_type_size, tepar_chunk_order, flags);
	} else {
		for (__relpos_t idx = 0; idx < NUM_RADIX1VAR_CHUNK_OBJ; idx += RADIX1VAR_MAX_OBJ_SIZE / tepar_type_size) {
			____coobj_radix1var_meta_data_update(page, RADIX1VAR_META_DATA_SIZE, idx, NUM_RADIX1VAR_OBJ_SIZE, 0, flags);
		}
		____coobj_radix1var_free_list_batch_add(lnode, free_list, page, level1_relpos,
			tepar_type_size, tepar_chunk_order, flags);
	}
}

static inline
int ____coobj_radix1var_uholder_dnode_extend_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_uholder_dnode_s *dnode,
	struct ____coobj_radix1var_free_list *free_list, struct ____coobj_base_user_s *base_user,
	__relpos_t baseof_chunk, __relpos_t target_idx, __relpos_t level1_relpos, coobj_typeid_t tepar_typeid,
	__relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	int err = E_HM_OK;
	if (dnode->page_uva == NULL) {
		err = ____coobj_base_user_uholder_map(
			base_user, ____coobj_chunkid(tepar_typeid, 0x0, baseof_chunk), 0, __false);
		if (err == E_HM_OK) {
			____coobj_radix1var_uholder_dnode_extend_page_nolock(lnode, dnode->page_uva, free_list,
				target_idx, level1_relpos, tepar_type_size, tepar_chunk_order, flags);
		}
	} else {
		err = E_HM_EXIST;
	}
	return err;
}

static inline
int ____coobj_radix1var_uholder_dnode_slow_extend_nolock(
	struct ____coobj_radix1var_uholder_dnode_s *dnode, struct ____coobj_base_user_s *base_user,
	__relpos_t size, __relpos_t baseof_chunk, coobj_typeid_t tepar_typeid)
{
	int err = E_HM_OK;
	if (dnode->page_uva == NULL) {
		err = ____coobj_base_user_uholder_map(
			base_user, ____coobj_chunkid(tepar_typeid, 0x0, baseof_chunk), size, __true);
	} else {
		err = E_HM_EXIST;
	}
	return err;
}

static inline
void ____coobj_radix1var_uholder_l1node_extend_page(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, __relpos_t tepar_level1_order)
{
	/* Initialize the null_list */
	____coobj_dlist_init(&lnode->atords.null_list);
	for (__relpos_t relpos = 0; relpos < ((__relpos_t)1 << tepar_level1_order); relpos++) {
		____coobj_radix1var_uholder_dnode_init(&lnode->child_dnode_list[relpos]);
	}
	____coobj_dlist_init_link(&lnode->atords.null_list, lnode->page_uva,
		____COOBJ_RADIX1VAR_UHOLDER_DNODE_SIZE, tepar_level1_order);
	____coobj_dlist_init(&lnode->atords.shrink_list);
}

static inline
__bool ____coobj_radix1var_uholder_object_alloc_slowpath_check_extend_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_base_user_s *base_user,
	__relpos_t baseof_level1, coobj_typeid_t tepar_typeid, __relpos_t tepar_level1_order)
{
	__bool check_result = __false;
	int err = E_HM_OK;
	if (lnode->page_uva == NULL) {
		/* First time to allocate the child_node_list for lnode */
		err = ____coobj_base_user_uholder_map(
			base_user, ____coobj_chunkid(tepar_typeid, 0x1, baseof_level1), 0, __true);
		if (err == E_HM_OK) {
			____coobj_radix1var_uholder_l1node_extend_page(lnode, tepar_level1_order);
			check_result = __true;
		}
	} else if (lnode->atords.null_list.head == ____COOBJ_INVALID_POS) {
		hm_error("coobj is out of large chunks\n");
	} else {
		check_result = __true;
	}
	return check_result;
}

static inline
int ____coobj_radix1var_uholder_l1node_extend_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_free_list *free_list,
	struct ____coobj_base_user_s *base_user, __relpos_t *obj_idx, __relpos_t target_idx, __relpos_t baseof_level1,
	coobj_typeid_t tepar_typeid, unsigned int flags, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order)
{
	int err = E_HM_OK;
	if (lnode->page_uva == NULL) {
		err = ____coobj_base_user_uholder_map(
			base_user, ____coobj_chunkid(tepar_typeid, 0x1, baseof_level1), 0, __false);
		if (err == E_HM_OK) {
			____coobj_radix1var_uholder_l1node_extend_page(lnode, tepar_level1_order);
		}
	} else if (lnode->atords.null_list.head == ____COOBJ_INVALID_POS) {
		hm_error("null_list empty\n");
		err = E_HM_NOMEM;
	}
	if (err == E_HM_OK) {
		/* Allocate one new chunk */
		__relpos_t level1_relpos = lnode->atords.null_list.head;
		struct ____coobj_radix1var_uholder_dnode_s *dnode = &lnode->child_dnode_list[level1_relpos];
		__relpos_t baseof_chunk = baseof_level1 + (level1_relpos << tepar_chunk_order);
		err = ____coobj_radix1var_uholder_dnode_extend_nolock(
			lnode, dnode, free_list, base_user, baseof_chunk, target_idx, level1_relpos,
			tepar_typeid, tepar_type_size, tepar_chunk_order, flags);
		if (err == E_HM_OK) {
			if (target_idx < NUM_RADIX1VAR_OBJ_SIZE) {
				*obj_idx = NUM_RADIX1VAR_OBJ_SIZE - 1;
			} else {
				*obj_idx = target_idx;
			}
			/* The newly allocated chunk is moved from null_list to global free_list,
			 * shrink_list will be refactored later.
			 */
			____coobj_dlist_pop(&lnode->atords.null_list, lnode->page_uva,
					    ____COOBJ_RADIX1VAR_UHOLDER_DNODE_SIZE);
		}
	}
	return err;
}

static inline
__relpos_t ____coobj_radix1var_uholder_alloc_slowpath_pop_pos_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_base_user_s *base_user, __relpos_t size,
	__relpos_t baseof_level1, coobj_typeid_t tepar_typeid, __relpos_t tepar_chunk_order)
{
	int err = E_HM_OK;
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	__relpos_t level1_relpos = lnode->atords.null_list.head;
	struct ____coobj_radix1var_uholder_dnode_s *dnode = &lnode->child_dnode_list[level1_relpos];
	__relpos_t baseof_chunk = baseof_level1 + (level1_relpos << tepar_chunk_order);
	err = ____coobj_radix1var_uholder_dnode_slow_extend_nolock(
		dnode, base_user, size, baseof_chunk, tepar_typeid);
	if (err == E_HM_OK) {
		relpos = baseof_chunk;
		____coobj_dlist_pop(&lnode->atords.null_list, lnode->page_uva,
					____COOBJ_RADIX1VAR_UHOLDER_DNODE_SIZE);
	}
	return relpos;
}

static inline
__u8 ____coobj_radix1var_meta_data_get_len(
	void *page, __relpos_t meta_size, __relpos_t idx)
{
	__u8 len;
	struct ____coobj_radix1var_meta_data_s *meta = ____coobj_radix1var_relpos_meta_data(page, meta_size, idx);
	len = meta->len;

	return len;
}

static inline
__relpos_t ____coobj_radix1var_uholder_free_list_alloc_fastpath_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct coobj_dlist_s *dlist,
	__relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	__relpos_t relpos = ____coobj_radix1var_free_list_pop_head(lnode, dlist, tepar_type_size, tepar_chunk_order, flags);
	return relpos;
}

/* According to the current buddy style, an object of size 2^n will be splitted into two 2^(n-1) ones,
 * which must be in the same dnode page.
 */
static inline
void ____coobj_radix1var_uholder_free_list_split(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_free_list *free_list,
	__relpos_t obj_idx, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	struct coobj_dlist_s *dlist = &free_list->free_list[obj_idx];
	__relpos_t relpos = dlist->head;

	if (relpos != ____COOBJ_INVALID_POS) {
		__relpos_t chunk_pos = relpos & ____coobj_order_mask(tepar_chunk_order);
		void *page = lnode->child_dnode_list[relpos >> tepar_chunk_order].page_uva;

		____coobj_radix1var_free_list_del_head(lnode, dlist, page, tepar_type_size, tepar_chunk_order, chunk_pos,
			flags);

		struct coobj_dlist_s *dlist_next = &free_list->free_list[obj_idx - 1];
		____coobj_radix1var_free_list_add_head(lnode, dlist_next, page, relpos + (1 << (obj_idx - 1)),
			tepar_type_size, tepar_chunk_order, flags);
		____coobj_radix1var_free_list_add_head(lnode, dlist_next, page, relpos,
			tepar_type_size, tepar_chunk_order, flags);

		____coobj_radix1var_meta_data_update_len(page, 1,
			chunk_pos - RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size, obj_idx, flags);
		____coobj_radix1var_meta_data_update_len(page, 1,
			chunk_pos + (1 << (obj_idx - 1)) - RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size, obj_idx, flags);
	}
}

static inline
__relpos_t ____coobj_radix1var_uholder_alloc_fastpath_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_free_list *free_list,
	__relpos_t obj_idx, __relpos_t target_idx, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order,
	unsigned int flags)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	if (obj_idx == ____COOBJ_INVALID_POS) {
		return relpos;
	}
	while (obj_idx > target_idx && target_idx < NUM_RADIX1VAR_OBJ_SIZE) {
		____coobj_radix1var_uholder_free_list_split(lnode, free_list, obj_idx, tepar_type_size,
			tepar_chunk_order, flags);
		obj_idx--;
	}
	struct coobj_dlist_s *dlist = &free_list->free_list[target_idx];
	relpos = ____coobj_radix1var_uholder_free_list_alloc_fastpath_nolock(lnode, dlist,
		tepar_type_size, tepar_chunk_order, flags);

	return relpos;
}

static inline
void ____coobj_radix1var_uholder_l1node_free_fastpath_nolock(
	struct ____coobj_radix1var_uholder_lnode_s *lnode, struct ____coobj_radix1var_free_list *free_list,
	__relpos_t relpos, __relpos_t tepar_type_size, __relpos_t tepar_chunk_order, unsigned int flags)
{
	__relpos_t level1_relpos = relpos >> tepar_chunk_order;
	__relpos_t chunk_relpos = relpos & ((1 << tepar_chunk_order) - 1);
	struct ____coobj_radix1var_uholder_dnode_s *dnode = &lnode->child_dnode_list[level1_relpos];

	__u8 len = ____coobj_radix1var_meta_data_get_len(dnode->page_uva, RADIX1VAR_META_DATA_SIZE,
		chunk_relpos - RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size);
	____coobj_radix1var_meta_data_update_free(dnode->page_uva, RADIX1VAR_META_DATA_SIZE,
		chunk_relpos - RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size, 0, flags);

	____coobj_radix1var_free_list_add_head(lnode, &free_list->free_list[len - 1], dnode->page_uva,
		relpos, tepar_type_size, tepar_chunk_order, flags);
}

static inline
struct ____coobj_radix1var_uholder_dnode_s *____coobj_radix1var_mholder_lnode_get_child_lpage_dnode(
	struct ____coobj_mholder_lnode_s *lnode, __uptr_t index)
{
	return &lnode->child_lpage_radix1var_dnode_mva[index];
}

static inline
void ____coobj_radix1var_mholder_dnode_set_uholder_mva(
	struct ____coobj_mholder_dnode_s *dnode, struct ____coobj_radix1var_uholder_dnode_s *dnode_mva)
{
	dnode->uholder_radix1var_dnode_mva = dnode_mva;
}

static inline
void ____coobj_radix1var_mholder_dnode_init(
	struct ____coobj_mholder_dnode_s *dnode, struct ____coobj_radix1var_uholder_dnode_s *dnode_mva)
{
	____coobj_mholder_dnode_set_uholder_mva(dnode, dnode_mva, __true);
	____coobj_mholder_dnode_set_dpage_hshmid(dnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_dnode_set_dpage_hva(dnode, ____COOBJ_INVALID_PAGE, __true);
}

#endif
