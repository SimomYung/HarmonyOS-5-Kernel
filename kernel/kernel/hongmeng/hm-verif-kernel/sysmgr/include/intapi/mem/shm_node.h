/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Shared-memory node management
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 4 15:58:36 2018
 */
#ifndef SYSMGR_MEM_SHM_NODE_H
#define SYSMGR_MEM_SHM_NODE_H

#include <stdint.h>
#include <inttypes.h>
#include <sec/ctx.h>
#include <intapi/mem/vspace.h>
#include <libalgo/rb_htable.h>
#include <libhmsync/raw_mutex.h>
#include <libsysif/sysmgr/api.h>
#include <sysif/server.h>

#define MAX_SHM_NAME MEMMGR_SHM_NAME_MAX

/* Macros used to parse shmid and cnode idx from key of native shm */
#define KEY_CNODE_IDX_SHIFT            0x20ULL   /* 32 */
#define KEY_CNODE_IDX_MASK             0xffffffffULL
#define KEY_SHMID_MASK                 0xffffffffULL

struct page_s;
struct vregion_s;
struct vr_operations;
struct vrgroup_s;

/* store shm_node's attributes for allocing */
struct shm_attr_s {
	const char *path;
	size_t len;
	uint64_t oflag;
	uint64_t paddr;
	uint64_t kshm_type;
	mode_t mode;
};

/* store return values in opening of shm_node */
struct shm_ret_s {
	size_t ret_len;
	int32_t ret_id;
};

struct shm_grantor_s {
	struct vspace_s *vspace;
	uint32_t cnode_idx;
	int32_t shm_id;
};

enum shm_state {
	NODE_INUSE = 1,
	NODE_UNLINK,
};

enum shm_type {
	SHM_GENERAL = 1,
	SHM_CELL,
};

/*
 * struct shm_node
 * @id:         Id of shm node, is -1 for perprocess shm node and is only useful
 *              for global shm node
 * @paddr:	Start physical address of shm and is only valid when
 *		SHM_O_WITH_PA is set
 * @name_node:	Only named shm_nodes are maintained on rbtree based hashtable.
 * @id_node:	All shm_nodes are maintained on ID rbtree.
 * @opener:	Records all vspaces open this shm.
 * @grant:	Records all vspaces granted permissions to this shm.
 * @private_data: Store `resv_map` for huge page.
 */
struct pset;
struct shm_node {
	int32_t id;
	int32_t shm_ops_private_data; /* private date for shm_ops */
	uint64_t oflag;
	uint64_t paddr;
	uint64_t kshm_type;
	size_t length;
	struct pset *pset;
	struct vrgroup_s *vrg;
	struct vspace_s *owner;
#ifdef CONFIG_SYSMGR_SHM_NODE_DEBUG_INFO
	char *owner_name;
#endif
	char *node_name;
	void *private_data;
	struct raw_refcnt refcnt;
	struct raw_mutex node_mutex;
	enum shm_state state;
	struct sec_natv_obj_ctx sec_ctx;
	struct rbt_hnode name_node;
	struct rbt_node id_node;
	struct rbt_tree grant;
	struct dlist_node dnode; /* dnode for shm_owned_list */
	struct dlist_node cell_dnode; /* dnode for shm_cell_list */
	struct dlist_node shm_dnode;
	struct shm_operations *shm_ops;
};

/*
 * struct shmd_s - shm_node descriptor
 * @id:    Per-process local_shmid.
 * @oflag: Record open authorization, used for checking mmap.
 */
struct shmd_s {
	bool is_owner;
	bool is_opened;
	bool has_removed;
	int32_t id;
	uint32_t prot;
	struct shm_node *shm_node;
	struct vspace_s *vspace;
	uint64_t oflag;
	struct raw_mutex mutex;
	struct raw_refcnt refcnt;
#ifdef CONFIG_DEBUG_BUILD
	uintptr_t pc;
#endif
};

/*
 * struct shm_grantee_s - grantee info of shm
 * @prot:        Record grant authorization, used for checking open.
 * @shm_node:    the shm_node that to which grantee belongs
 * @vspace:      vspace of grantee
 * @sequence_id: sequence id of process
 * @node:        the node linked in rbt tree of shmd
 * @dnode:       the dnode linked in vspace.shm_info.shm_granted_list
 */
struct shm_grantee_s {
	uint32_t prot;
	struct shm_node *shm_node;
	struct vspace_s *vspace;
	uint64_t sequence_id;
	struct rbt_node node;
	struct dlist_node dnode;
};

/*
 * For global id shm_node, @shmd is NULL.
 * If all global id shm_node has been removed, @shm_node can
 * be replaced by @shmd->shm_node.
 */
struct shm_backend_info {
	int32_t shm_id;
	struct shmd_s *shmd;
	struct shm_node *shm_node;
	struct vr_operations *shm_vr_ops;
};

struct shm_operations {
	int (*create)(const struct shm_node *shm_node);
	void (*destroy)(const struct shm_node *shm_node);
	uint32_t (*get_inode)(const struct shm_node *shm_node);
};

int shm_htable_init(void);
int shm_node_open(struct vspace_s *vspace, struct shm_attr_s *shm_attr,
		  struct shm_ret_s *shm_ret);
/*
 * Create or open an anonymous native share memory
 *
 * @vspace          [I] Vspace of caller
 *
 * @key             [I] Key for the share memory to open
 *
 * @owner_cnode_idx [I] Cnode idx of shm owner
 *
 * @len             [I] length of share memory
 *
 * @oflag           [I] Flags for share memory
 *
 * @shm_ret         [O] Shmid and size of share memory
 */
int shm_node_open_anon(struct vspace_s *vspace, uint64_t key,
		       uint32_t owner_cnode_idx, size_t len, uint64_t oflag,
		       struct shm_ret_s *shm_ret);
int shm_backend_info_acquire(struct vspace_s *vspace, int32_t shm_id,
			     uint32_t prot,
			     struct shm_backend_info *shm_info);
void shm_backend_info_release(struct shm_backend_info *shm_info);
int shm_node_unlink(const struct vspace_s *vspace, const char *path);
int shm_node_close(struct vspace_s *vspace, int32_t shm_id);
void shm_node_cleanup(struct vspace_s *vspace);
int shm_dump_info(void);
int shm_node_chmod(const char *path, mode_t set_mode);
int shm_sec_ctx_init_after_secmgr_boot(void);
int shm_node_opsset(struct vspace_s *vspace, int32_t shm_id,
		     struct shm_operations *ops, void *private_data);
unsigned long shm_node_max_num(void);
size_t shm_acquire_len_by_addr(struct vspace_s *vspace, uintptr_t addr);
bool shm_is_range_in_cell(uint64_t paddr, size_t len,
			  const struct vspace_s *vspace);

/*
 * Transfer the caller's permission on share memory to the process specified
 * by the @tgt_vspace
 *
 * @param caller_vspace   [I] Vspace of caller process
 *
 * @param tgt_vspace      [I] Vspace of process to which the shm permission is transferred
 *
 * @param key             [I] Key for the share memory whose access permission is transferred
 *
 * @return                E_HM_OK if succeed, error code if fail
 *
 * @note                  Only vfs can invoke this function and the process specified
 *		          by @tgt_vspace must be devhost
 */
int shm_node_grant_trans(const struct vspace_s *caller_vspace,
			 struct vspace_s *tgt_vspace, uint64_t key);
int shm_node_unlink_anon(struct vspace_s *vspace, int32_t shm_id);
/*
 * Check the memory protection of the mapping on share memory
 *
 * @param shmd          [I] Shm descriptor of mapped share memory
 *
 * @param prot          [I] Memory protection
 *
 * @param need_opened   [I] Whether the shm should have been opened
 *
 * @return              E_HM_OK if succeed, error code if fail
 */
int shmd_check_map_prot(struct shmd_s *shmd, uint32_t prot, bool need_opened);
int shm_node_check_owner(const struct vspace_s *vspace, uint64_t key,
			 struct vspace_s *tgt_vspace);
int shm_node_query_info(struct vspace_s *vspace, int32_t shm_id,
			struct shm_node_info *shm_info);
int shm_node_query_paddr(struct vspace_s *vspace, int32_t shm_id,
			 unsigned long long offset, unsigned long long *dst,
			 unsigned long size);

/* Return the maximum count of shmd of common processes */
uint32_t shmd_max_count_read(void);
/* Set the maximum count of shmd of common processes */
int shmd_max_count_set(uint32_t val);

static inline unsigned long long generate_key(unsigned cnode_idx, int shm_id)
{
	return (((unsigned long long)cnode_idx << KEY_CNODE_IDX_SHIFT) & (~KEY_SHMID_MASK)) |
	       ((unsigned long long)(unsigned)shm_id & KEY_SHMID_MASK);
}

static inline unsigned key_to_cnode_idx(unsigned long long key)
{
	return (unsigned)((key >> KEY_CNODE_IDX_SHIFT) & KEY_CNODE_IDX_MASK);
}

static inline int key_to_shmid(unsigned long long key)
{
	return (int)(unsigned)(key & KEY_SHMID_MASK);
}
#ifdef __HOST_LLT__
bool llt_is_shm_table_empty(struct vspace_s *vspace);
#endif

#endif
