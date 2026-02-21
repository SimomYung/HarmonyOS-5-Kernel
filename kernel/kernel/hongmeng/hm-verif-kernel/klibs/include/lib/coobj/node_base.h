/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Basic data structures for allocators
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_NODE_BASE_H
#define KLIBS_COOBJ_NODE_BASE_H

#include <lib/coobj/import.h>

/*
 * coobj roles: there are 3 roles in coobj:
 *	* renter: a read-only role which could only read the shared objects pages
 *	* holder: a writable role which could read/write the shared objects pages, but could not
 *		allocate pages for the shared objects. The holder could allocate/free objects in
 *		the shared pages. However, when the pages lack enough space, the holder has to
 *		ask mapper for allocating more shared object pages.
 *	* mapper: the role for only allocating the shared object pages. The mapper
 *		also builds the point-to data structures for the renter and holder.
 *
 *	* The renter and holder is named as user.
 */

/* aggregative allocator data structure */
struct ____coobj_atords_agg_s {
	/* free_list corresponds to partially allocated chunk's list */
	struct coobj_slist_s free_list;
};
#define ____COOBJ_ATORDS_AGG_INIT \
	((struct ____coobj_atords_agg_s) { \
		.free_list = ____COOBJ_SLIST_INIT, \
	})

/* individual allocator data structure */
struct ____coobj_atords_ind_s {
	/* free_list corresponds to partially allocated chunk's list */
	struct coobj_dlist_s free_list;
	/* null_list corresponds to unmapped chunk slot's list */
	struct coobj_dlist_s null_list;
	/* shrink_list corresponds to unallocated but mapped chunk's list */
	struct coobj_dlist_s shrink_list;
};
#define ____COOBJ_ATORDS_IND_INIT \
	((struct ____coobj_atords_ind_s) { \
		.free_list = ____COOBJ_DLIST_INIT, \
		.null_list = ____COOBJ_DLIST_INIT, \
		.shrink_list = ____COOBJ_DLIST_INIT, \
	})

#define ____COOBJ_RESV_VM_INFO_INIT \
	((struct ____coobj_resv_vm_info_s) { \
		.resv_vm_start = 0, \
		.resv_vm_end = 0, \
		.resv_vm_cur = 0, \
	})

struct ____coobj_mrenter_tgt_page_info_s {
	/* The corresponding page's shared memory status.
	 * NOTE: since the shmid is already recorded in the corresponding mholder,
	 * we don't repeatly record the shmid again.
	 */
	struct ____coobj_mrenter_shmst_s page_shmst;

	/* The corresponding page's virtual address */
	__uptr_t page_uva;
};

/**************************************************
 * uholder dnode
 **************************************************/
struct ____coobj_uholder_dnode_s {
	struct coobj_dlist_node_s dlist_node;

	/* The slab allocator */
	struct ____coobj_atords_agg_s atords;

	/* page for allocator */
	void *page_uva;
};
static inline
void ____coobj_uholder_dnode_init(
	struct ____coobj_uholder_dnode_s *dnode)
{
	dnode->dlist_node = ____COOBJ_DLIST_NODE_INIT;
	dnode->atords = ____COOBJ_ATORDS_AGG_INIT;
	dnode->page_uva = NULL;
}
#define RADIX1VAR_USER_NODE_PADDING ((sizeof(__relpos_t) * 4 - sizeof(__uptr_t)) / sizeof(__uptr_t))
struct ____coobj_radix1var_uholder_dnode_s {
	struct coobj_dlist_node_s dlist_node;
	/* page for allocator */
	void *page_uva;
	__uptr_t status;
	__uptr_t padding[RADIX1VAR_USER_NODE_PADDING];
};

/**************************************************
 * urenter node
 **************************************************/
struct ____coobj_urenter_node_s {
	struct coobj_dlist_node_s dlist_node;
	union {
		void *page_uva;
		struct ____coobj_urenter_node_s *child_node_list;
	};
	__uptr_t status;
	__uptr_t padding[RADIX1VAR_USER_NODE_PADDING];
};
static inline
void ____coobj_urenter_node_init(
	struct ____coobj_urenter_node_s *node)
{
	node->page_uva = NULL;
	node->status = 0;
}

/**************************************************
 * mholder dnode
 **************************************************/
struct ____coobj_mholder_dnode_s {
	/* The counter-part uholder's data structure */
	union {
		struct ____coobj_uholder_dnode_s *uholder_dnode_mva;
		struct ____coobj_radix1var_uholder_dnode_s *uholder_radix1var_dnode_mva;
	};

	/* The corresponding in-holder data page's shared memory id */
	____coobj_shmid_t dpage_hshmid;

	/* The corresponding in-holder data page's virtual address */
	__uptr_t dpage_hva;
	/* Page size of large chunk */
	__relpos_t dpage_size;
};

static inline
void ____coobj_mholder_dnode_set_uholder_mva(
	struct ____coobj_mholder_dnode_s *dnode, void *dnode_mva, __bool var_ator)
{
	if (!var_ator) {
		dnode->uholder_dnode_mva = (struct ____coobj_uholder_dnode_s *)dnode_mva;
	} else {
		dnode->uholder_radix1var_dnode_mva = (struct ____coobj_radix1var_uholder_dnode_s *)dnode_mva;
	}
}
static inline
void ____coobj_mholder_dnode_set_dpage_hshmid(
	struct ____coobj_mholder_dnode_s *dnode, ____coobj_shmid_t dpage_hshmid)
{
	dnode->dpage_hshmid = dpage_hshmid;
}
static inline
void ____coobj_mholder_dnode_set_dpage_hva(
	struct ____coobj_mholder_dnode_s *dnode, __uptr_t dpage_hva, __bool var_ator)
{
	dnode->dpage_hva = dpage_hva;
	if (!var_ator) {
		dnode->uholder_dnode_mva->page_uva = (void *)dpage_hva;
	} else {
		dnode->uholder_radix1var_dnode_mva->page_uva = (void *)dpage_hva;
	}
}

static inline
void ____coobj_mholder_dnode_init(
	struct ____coobj_mholder_dnode_s *dnode, struct ____coobj_uholder_dnode_s *dnode_mva)
{
	____coobj_mholder_dnode_set_uholder_mva(dnode, dnode_mva, __false);
	____coobj_mholder_dnode_set_dpage_hshmid(dnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_dnode_set_dpage_hva(dnode, ____COOBJ_INVALID_PAGE, __false);
}

/**************************************************
 * mrenter dnode
 **************************************************/
struct ____coobj_mrenter_dnode_s {
	struct ____coobj_urenter_node_s *urenter_node_mva;

	/* The shmid of the shared page in the renter */
	____coobj_shmid_t dpage_rshmid;

	struct ____coobj_mrenter_tgt_page_info_s dpage_rinfo;
};
static inline
void ____coobj_mrenter_dnode_set_urenter_mva(
	struct ____coobj_mrenter_dnode_s *dnode, struct ____coobj_urenter_node_s *node_mva)
{
	dnode->urenter_node_mva = node_mva;
}
static inline
void ____coobj_mrenter_dnode_set_dpage_rshmid(
	struct ____coobj_mrenter_dnode_s *dnode, ____coobj_shmid_t dpage_rshmid)
{
	dnode->dpage_rshmid = dpage_rshmid;
}
static inline
void ____coobj_mrenter_dnode_set_dpage_rshmst(
	struct ____coobj_mrenter_dnode_s *dnode, struct ____coobj_mrenter_shmst_s page_rshmst)
{
	dnode->dpage_rinfo.page_shmst = page_rshmst;
}
static inline
void ____coobj_mrenter_dnode_set_dpage_rva(
	struct ____coobj_mrenter_dnode_s *dnode, __uptr_t page_rva)
{
	dnode->dpage_rinfo.page_uva = page_rva;
	dnode->urenter_node_mva->page_uva = (void *)page_rva;
}
static inline
void ____coobj_mrenter_dnode_init(
	struct ____coobj_mrenter_dnode_s *dnode, struct ____coobj_urenter_node_s *node_mva)
{
	____coobj_mrenter_dnode_set_urenter_mva(dnode, node_mva);
	____coobj_mrenter_dnode_set_dpage_rshmid(dnode, ____COOBJ_INVALID_SHMID);
	____coobj_mrenter_dnode_set_dpage_rshmst(dnode, ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE);
	____coobj_mrenter_dnode_set_dpage_rva(dnode, ____COOBJ_INVALID_PAGE);
}

/***********************
 * uholder lnode
 ***********************/
struct ____coobj_uholder_lnode_s {
	/* For root node, dnode is not used */
	struct coobj_dlist_node_s dlist_node;

	/* The slab allocator */
	struct ____coobj_atords_ind_s atords;

	/* page / element for allocator */
	union {
		void *page_uva;
		struct ____coobj_uholder_dnode_s *child_dnode_list;
		struct ____coobj_uholder_lnode_s *child_lnode_list;
	};
};
static inline
void ____coobj_uholder_lnode_init(
	struct ____coobj_uholder_lnode_s *lnode)
{
	lnode->dlist_node = ____COOBJ_DLIST_NODE_INIT;
	lnode->atords = ____COOBJ_ATORDS_IND_INIT;
	lnode->page_uva = NULL;
}
struct ____coobj_radix1var_atords_ind_s {
	/* null_list corresponds to unmapped chunk slot's list */
	struct coobj_dlist_s null_list;
	/* shrink_list corresponds to unallocated but mapped chunk's list */
	struct coobj_dlist_s shrink_list;
};
#define ____COOBJ_RADIX1VAR_ATORDS_IND_INIT \
	((struct ____coobj_radix1var_atords_ind_s) { \
		.null_list = ____COOBJ_DLIST_INIT, \
		.shrink_list = ____COOBJ_DLIST_INIT, \
	})
struct ____coobj_radix1var_uholder_lnode_s {
	/* For root node, dnode is not used */
	struct coobj_dlist_node_s dlist_node;
	/* page / element for allocator */
	union {
		void *page_uva;
		struct ____coobj_radix1var_uholder_dnode_s *child_dnode_list;
		struct ____coobj_radix1var_uholder_lnode_s *child_lnode_list;
	};
	/* The slab allocator */
	struct ____coobj_radix1var_atords_ind_s atords;
};
/**************************************************
 * mholder lnode
 **************************************************/
struct ____coobj_mholder_lnode_s {
	union {
		struct ____coobj_uholder_lnode_s *uholder_lnode_mva;
		struct ____coobj_radix1var_uholder_lnode_s *uholder_radix1var_lnode_mva;
	};

	/* The child node list */
	union {
		void *child_node_list;
		struct ____coobj_mholder_dnode_s *child_dnode_list;
		struct ____coobj_mholder_lnode_s *child_lnode_list;
	};

	/* The uholder's child node list in the mapper */
	____coobj_shmid_t child_lpage_mshmid;
	____coobj_shmid_t child_lpage_hshmid;

	union {
		__uptr_t child_lpage_mva;
		struct ____coobj_uholder_dnode_s *child_lpage_dnode_mva;
		struct ____coobj_uholder_lnode_s *child_lpage_lnode_mva;
		struct ____coobj_radix1var_uholder_dnode_s *child_lpage_radix1var_dnode_mva;
		struct ____coobj_radix1var_uholder_lnode_s *child_lpage_radix1var_lnode_mva;
	};
	__uptr_t child_lpage_hva;
};
static inline
void ____coobj_mholder_lnode_set_uholder_mva(
	struct ____coobj_mholder_lnode_s *lnode, void *lnode_mva, __bool var_ator)
{
	if (!var_ator) {
		lnode->uholder_lnode_mva = (struct ____coobj_uholder_lnode_s *)lnode_mva;
	} else {
		lnode->uholder_radix1var_lnode_mva = (struct ____coobj_radix1var_uholder_lnode_s *)lnode_mva;
	}
}
static inline
void ____coobj_mholder_lnode_set_child_node_list(
	struct ____coobj_mholder_lnode_s *lnode, void *child_node_list)
{
	lnode->child_node_list = child_node_list;
}
static inline
void ____coobj_mholder_lnode_set_child_lpage_mshmid(
	struct ____coobj_mholder_lnode_s *lnode, ____coobj_shmid_t child_lpage_mshmid)
{
	lnode->child_lpage_mshmid = child_lpage_mshmid;
}
static inline
void ____coobj_mholder_lnode_set_child_lpage_hshmid(
	struct ____coobj_mholder_lnode_s *lnode, ____coobj_shmid_t child_lpage_hshmid)
{
	lnode->child_lpage_hshmid = child_lpage_hshmid;
}
static inline
void ____coobj_mholder_lnode_set_child_lpage_mva(
	struct ____coobj_mholder_lnode_s *lnode, __uptr_t child_lpage_mva)
{
	lnode->child_lpage_mva = child_lpage_mva;
}
static inline
void ____coobj_mholder_lnode_set_child_lpage_hva(
	struct ____coobj_mholder_lnode_s *lnode, __uptr_t child_lpage_hva, __bool var_ator)
{
	lnode->child_lpage_hva = child_lpage_hva;
	if (!var_ator) {
		lnode->uholder_lnode_mva->page_uva = (void *)child_lpage_hva;
	} else {
		lnode->uholder_radix1var_lnode_mva->page_uva = (void *)child_lpage_hva;
	}
}
static inline
void ____coobj_mholder_lnode_init(
	struct ____coobj_mholder_lnode_s *lnode, void *lnode_mva, __bool var_ator)
{
	____coobj_mholder_lnode_set_uholder_mva(lnode, lnode_mva, var_ator);
	____coobj_mholder_lnode_set_child_node_list(lnode, NULL);
	____coobj_mholder_lnode_set_child_lpage_mshmid(lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_lnode_set_child_lpage_hshmid(lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_lnode_set_child_lpage_mva(lnode, ____COOBJ_INVALID_PAGE);
	____coobj_mholder_lnode_set_child_lpage_hva(lnode, ____COOBJ_INVALID_PAGE, var_ator);
}

/**************************************************
 * mrenter lnode
 **************************************************/
struct ____coobj_mrenter_lnode_s {
	struct ____coobj_urenter_node_s *urenter_node_mva;

	/* The child node list */
	union {
		void *child_node_list;
		struct ____coobj_mrenter_dnode_s *child_dnode_list;
		struct ____coobj_mrenter_lnode_s *child_lnode_list;
	};

	____coobj_shmid_t child_lpage_mshmid;
	____coobj_shmid_t child_lpage_rshmid;

	union {
		__uptr_t child_lpage_mva;
		struct ____coobj_urenter_node_s *child_lpage_node_mva;
	};
	struct ____coobj_mrenter_tgt_page_info_s child_lpage_rinfo;
};
static inline
void ____coobj_mrenter_lnode_set_urenter_mva(
	struct ____coobj_mrenter_lnode_s *lnode, struct ____coobj_urenter_node_s *node_mva)
{
	lnode->urenter_node_mva = node_mva;
}
static inline
void ____coobj_mrenter_lnode_set_child_node_list(
	struct ____coobj_mrenter_lnode_s *lnode, void *child_node_list)
{
	lnode->child_node_list = child_node_list;
}
static inline
void ____coobj_mrenter_lnode_set_child_lpage_mshmid(
	struct ____coobj_mrenter_lnode_s *lnode, ____coobj_shmid_t child_lpage_mshmid)
{
	lnode->child_lpage_mshmid = child_lpage_mshmid;
}
static inline
void ____coobj_mrenter_lnode_set_child_lpage_rshmid(
	struct ____coobj_mrenter_lnode_s *lnode, ____coobj_shmid_t child_lpage_rshmid)
{
	lnode->child_lpage_rshmid = child_lpage_rshmid;
}
static inline
void ____coobj_mrenter_lnode_set_child_lpage_mva(
	struct ____coobj_mrenter_lnode_s *lnode, __uptr_t child_lpage_mva)
{
	lnode->child_lpage_mva = child_lpage_mva;
}
static inline
void ____coobj_mrenter_lnode_set_child_lpage_rshmst(
	struct ____coobj_mrenter_lnode_s *lnode, struct ____coobj_mrenter_shmst_s child_lpage_rshmst)
{
	lnode->child_lpage_rinfo.page_shmst = child_lpage_rshmst;
}
static inline
void ____coobj_mrenter_lnode_set_child_lpage_rva(
	struct ____coobj_mrenter_lnode_s *lnode, __uptr_t child_lpage_rva)
{
	lnode->child_lpage_rinfo.page_uva = child_lpage_rva;
	lnode->urenter_node_mva->page_uva = (void *)child_lpage_rva;
}
static inline
void ____coobj_mrenter_lnode_init(
	struct ____coobj_mrenter_lnode_s *lnode, struct ____coobj_urenter_node_s *node_mva)
{
	____coobj_mrenter_lnode_set_urenter_mva(lnode, node_mva);
	____coobj_mrenter_lnode_set_child_node_list(lnode, (void *)____COOBJ_INVALID_PAGE);
	____coobj_mrenter_lnode_set_child_lpage_rshmid(lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mrenter_lnode_set_child_lpage_mshmid(lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mrenter_lnode_set_child_lpage_mva(lnode, ____COOBJ_INVALID_PAGE);
	____coobj_mrenter_lnode_set_child_lpage_rshmst(lnode, ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE);
	____coobj_mrenter_lnode_set_child_lpage_rva(lnode, ____COOBJ_INVALID_PAGE);
}

/* For small objects, there're 5 different obj size: 16, 32, 64, 128, 256 */
#define NUM_RADIX1VAR_OBJ_SIZE 5U
#define L_NUM_RADIX1VAR_OBJ_SIZE 9U
struct ____coobj_radix1var_meta_data_s {
	/* len denotes the size of the object. if len = 0, size = 0, else size = 2 ^ (len - 1) */
	__u8 len : 4;
	__u8 free : 1;
	__u8 reserved : 3;
};
struct ____coobj_radix1var_free_list {
	struct coobj_dlist_s free_list[L_NUM_RADIX1VAR_OBJ_SIZE];
};
static inline
void ____coobj_radix1var_free_list_init(
	struct ____coobj_radix1var_free_list *list)
{
	for (__relpos_t i = 0; i < L_NUM_RADIX1VAR_OBJ_SIZE; i++) {
		list->free_list[i] = ____COOBJ_DLIST_INIT;
	}
}
static inline
void ____coobj_radix1var_uholder_dnode_init(struct ____coobj_radix1var_uholder_dnode_s *dnode)
{
	dnode->dlist_node = ____COOBJ_DLIST_NODE_INIT;
	dnode->page_uva = NULL;
}
static inline
void ____coobj_radix1var_uholder_lnode_init(
	struct ____coobj_radix1var_uholder_lnode_s  *lnode)
{
	lnode->dlist_node = ____COOBJ_DLIST_NODE_INIT;
	lnode->atords = ____COOBJ_RADIX1VAR_ATORDS_IND_INIT;
	lnode->page_uva = NULL;
}

static inline
__bool ____coobj_relpos_check(
	__relpos_t relpos,
	__relpos_t const_chunk_order)
{
	return ((relpos != ____COOBJ_INVALID_POS) && (relpos < ((__relpos_t)1 << const_chunk_order))) ?
		__true : __false;
}

static inline
void *____coobj_user_raw_relpos(
	void *page_uva,
	__relpos_t relpos,
	__relpos_t tepar_type_size)
{
	return page_uva == NULL ? NULL : (void *)(((unsigned char *)page_uva) + tepar_type_size * relpos);
}

static inline
__bool ____coobj_urenter_version_check(__u64 pre, __u64 post)
{
	return (pre == post) && ((post & 1) == 1);
}

static inline
__u32 ____coobj_flags_to_perm(unsigned int flags)
{
	__u32 perm = COOBJ_MAPPER_PERM_RW;
#ifdef CONFIG_HKIP_SECCOMP_PROT
	if ((flags & HM_COOBJ_FLAGS_HKIP_ENABLE) != 0)
		perm = COOBJ_MAPPER_PERM_HKIP_ROWM;
#endif
	return perm;
}

enum {
	____COOBJ_UHOLDER_DNODE_SIZE = sizeof(struct ____coobj_uholder_dnode_s),
	____COOBJ_UHOLDER_LNODE_SIZE = sizeof(struct ____coobj_uholder_lnode_s),
	____COOBJ_URENTER_NODE_SIZE = sizeof(struct ____coobj_urenter_node_s),
	____COOBJ_MHOLDER_DNODE_SIZE = sizeof(struct ____coobj_mholder_dnode_s),
	____COOBJ_MHOLDER_LNODE_SIZE = sizeof(struct ____coobj_mholder_lnode_s),
	____COOBJ_MRENTER_DNODE_SIZE = sizeof(struct ____coobj_mrenter_dnode_s),
	____COOBJ_MRENTER_LNODE_SIZE = sizeof(struct ____coobj_mrenter_lnode_s),
	____COOBJ_RADIX1VAR_UHOLDER_DNODE_SIZE = sizeof(struct ____coobj_radix1var_uholder_dnode_s),
};

#endif
