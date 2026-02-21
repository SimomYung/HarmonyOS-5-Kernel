/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface for kshm management
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 13 14:32:21 2023
 */
#ifndef ULIBS_LIBHMSRV_SYS_HM_KSHM_H
#define ULIBS_LIBHMSRV_SYS_HM_KSHM_H

#include <hmkernel/capability.h>
#include <libmem/flags.h>

#include <stddef.h>
#include <stdint.h>

/*
 * Create or open a kshm memory
 *
 * @param key       [I] Regart as kshm type if create a new kshm,
 *                      regart as kshm descriptor if open a exist kshm
 *
 * @owner_cnode_idx [I] Cnode index of owner of kshm, Check whether the kshm memory
 *                      corresponding to the @key is created by the process
 *                      specified by the @owner_cnode_idx
 *
 * @param sz        [I] Size of kshm memory to open, it will be ignored
 *                      when open an exist kshm memory
 *
 * @param flags     [I] Flags for kshm memory to create/open
 *
 * @param rsz       [O] Return size of kshm memory to open
 *
 * @return          Kshm descriptor if succeed, error code if fail
 *
 * @note            This interface is similar as hm_ashm_open_anon but stricter
 *
 */
int hm_kshm_open(uint64_t key, uint32_t owner_cnode_idx, size_t sz, uint64_t flags,
		 size_t *rsz);
/*
 * Close a kshm memory
 *
 * @param  kshmd    [I] The descriptor of kshm to close
 *
 * @return          E_HM_OK if succeed, error code if fail
 */
int hm_kshm_close(int kshmd);

/*
 * Unlink a kshm memory
 *
 * @param  kshmd    [I] The descriptor of kshm to unlink
 *
 * @return          E_HM_OK if succeed, error code if fail
 *
 * @note            This interface is temporary and will be deleted later
 */
int hm_kshm_unlink(int kshmd);

/*
 *  Check whether the kshm memory corresponding to the @key is created by the
 *  process specified by the @owner.
 *
 * @param key       [I] kshm descriptor for the kshm memory to check
 *
 * @param owner     [I] Owner(cnode idx) of the kshm memory to check
 *
 * @return          E_HM_OK if succeed, error code if fail
 */

/*
 * Grant a valid kshm memory to current actv caller
 *
 * @param kshmd     [I] Kshm descriptor
 *
 * @param flags     [I] Permission to operate the kshm memory
 *
 * @param key       [O] Key for the kshm memory
 *
 * @return          E_HM_OK if succeed, error code if fail
 *
 * @note            This function should be only called in activation context
 */
int hm_kshm_grant_to_caller(int kshmd, uint32_t flags, uint64_t *key);

/*
 * Grant a valid kshm memory to a process specified by @xref
 *
 * @param kshmd     [I] Kshm memory descriptor
 *
 * @param flags     [I] Permission to operate the kshm memory
 *
 * @param xref      [I] rref、cref or uref of process
 *
 * @param key       [O] Key for a kshm memory
 *
 * @return          E_HM_OK if succeed, error code if fail
 */
int hm_kshm_grant(int kshmd, uint32_t flags, xref_t xref, uint64_t *key);

/*
 * Grant a valid kshm memory to a process specified by cnode_idx
 *
 * @param kshmd     [I] Kshm memory descriptor
 *
 * @param flags     [I] Permission to operate the kshm memory
 *
 * @param tgt_idx   [I] target process cnode index
 *
 * @param key       [O] Key for a kshm memory
 *
 * @return          E_HM_OK if succeed, error code if fail
 */
int hm_kshm_grant_by_cnode_idx(int kshmd, uint32_t flags, uint32_t tgt_idx, uint64_t *key);

/*
 * Transfer the caller's permission of memory to the process specified
 * by the @xref
 *
 * @param key       [I] Key for a kshm memory to transfer access permission
 *
 * @param xref      [I] rref、cref or uref of process
 *
 * @return          E_HM_OK if succeed, error code if fail
 *
 * @note            Only vfs can invoke this function and the process specified
 *		    by @xref must be devhost
 */
int hm_kshm_grant_trans(uint64_t key, xref_t xref);

#endif /* ULIBS_LIBHMSRV_SYS_HM_KSHM_H */
