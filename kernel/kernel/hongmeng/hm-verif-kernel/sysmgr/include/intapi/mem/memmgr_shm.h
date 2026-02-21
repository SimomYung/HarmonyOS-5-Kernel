/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Memmgr shared memory management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 22 09:19:55 2019
 */
#ifndef SYSMGR_MEMMGR_SHM_H
#define SYSMGR_MEMMGR_SHM_H

#include <intapi/mem/shm_node.h>

int memmgr_shm_open(struct vspace_s *client_vspace,
		    struct shm_attr_s *shm_attr, struct shm_ret_s *shm_ret);

int memmgr_shm_open_anon(struct vspace_s *client_vspace, uint64_t key,
			 uint32_t owner_cnode_idx, size_t aligned_len,
			 uint64_t oflag, struct shm_ret_s *shm_ret);

int memmgr_kshm_open_anon(struct vspace_s *client_vspace, uint64_t key,
			  uint32_t owner_cnode_idx, size_t aligned_len,
			  uint64_t oflag, struct shm_ret_s *shm_ret);

int memmgr_shm_unlink(const struct vspace_s *client_vspace, const char *name);

int memmgr_shm_unlink_anon(struct vspace_s *client_vspace, int32_t shm_id);

int memmgr_shm_close(struct vspace_s *client_vspace,
		     int32_t shm_id);

int memmgr_shm_check_owner(const struct vspace_s *vspace, uint64_t key,
			   struct vspace_s *tgt_vspace);

int memmgr_shm_reclaim_range(struct vspace_s *client_vspace,
			     int32_t shm_id, uintptr_t start_addr, size_t len);

int memmgr_grant_shm(uint32_t prot,
		     const struct shm_grantor_s *shm_grantor,
		     struct vspace_s *tgt_vspace,
		     uint64_t *ret_key);

/* Grant anonymous shared memory to the process and make the process open shm
 * @param shm_grantor  [I] The information of grantor
 *
 * @param prot         [I] Desired share memory protection of the mapping
 *
 * @param tgt_vspace   [I] Vspace that is granted permission
 *
 * @param oflag        [I] Flags for share memory opened
 *
 * @param shm_ret      [O] Returned info for shm
 *
 * @return  E_HM_OK for success, error code for failure.
 *
 * @note    This interface is used by sysmgr to grant shm to a process and make
 *          process open shm
 */
int memmgr_shm_grant_open_anon(const struct shm_grantor_s *shm_grantor,
			       uint32_t prot, struct vspace_s *tgt_vspace,
			       uint64_t oflag, struct shm_ret_s *shm_ret);

int memmgr_shm_grant_trans(const struct vspace_s *caller_vspace,
		       struct vspace_s *tgt_vspace, uint64_t key);

int memmgr_shm_query_info(struct vspace_s *vspace, int32_t shm_id,
			  struct shm_node_info *shm_info);

int memmgr_shm_query_paddr(struct vspace_s *vspace, int32_t shm_id,
			   unsigned long long offset, unsigned long long *dst,
			   unsigned long size);

int memmgr_shm_dump_info(void);

int memmgr_shm_chmod(const char *name, mode_t set_mode);

#endif
