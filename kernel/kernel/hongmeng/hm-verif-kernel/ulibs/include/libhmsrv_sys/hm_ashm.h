/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Interface for share memory management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 07 20:03:17 2021
 */
#ifndef ULIBS_LIBHMSRV_SYS_HM_ASHM_H
#define ULIBS_LIBHMSRV_SYS_HM_ASHM_H

#include <stddef.h>
#include <stdint.h>
#include <libmem/flags.h>
#include <hmkernel/capability.h>

/* This interface will be removed! Use hm_ashm_open_auth instead. */
int hm_ashm_open(uint64_t key, size_t sz, uint64_t flags, size_t *rsz);

/*
 * Create or open an anonymous native share memory
 *
 * @param key              [I] Key for one anonymous native share memory
 *
 * @param owner_cnode_idx  [I] Cnode idx of owner of share memory to open.
 *			       This interface will check whether the shared memory
 *			       corresponding to the @key is created by the process
 *			       specified by the @owner_cnode_idx
 *
 * @param sz               [I] Size of share memory to open, it is ignored
 *			       when opens one exist share memory
 *
 * @param flags            [I] Flags for share memory opened
 *
 * @param rsz              [O] Size of share memory opened
 *
 * @return                 Share memory descriptor if succeed, error code if fail
 *
 * @note                   Key for one share memory is a global identifier
 *                         for this share memory
 */
int hm_ashm_open_auth(uint64_t key, uint32_t owner_cnode_idx, size_t sz,
		      uint64_t flags, size_t *rsz);
/*
 * Close an anonymous share memory
 *
 * @param sd  [I] Share memory descriptor to close
 *
 * @return    E_HM_OK if succeed, error code if fail
 */
int hm_ashm_close(int sd);
/*
 * Unlink an anonymous share memory
 *
 * @param sd  [I] Share memory descriptor to unlink
 *
 * @return    E_HM_OK if succeed, error code if fail
 *
 * @note      This interface is temporary and will be deleted later
 */
int hm_ashm_unlink(int sd);
/* This interface will be removed! Use hm_ashm_open_auth instead. */
int hm_ashm_check_key(uint64_t key, uint32_t owner);
/*
 * Grant a opened anonymous share memory to current actv caller
 *
 * @param sd        [I] Share memory descriptor
 *
 * @param flags     [I] Permission to operate the shared memory
 *
 * @param key       [O] Key for the share memory
 *
 * @return          E_HM_OK if succeed, error code if fail
 *
 * @note            The caller of this function should be in
 *                  one activation call context, otherwise
 *                  it's undefined
 */
int hm_ashm_grant_to_caller(int sd, uint32_t flags, uint64_t *key);
/*
 * Grant a opened anonymous share memory to a process specified by @xref
 *
 * @param sd        [I] Share memory descriptor
 *
 * @param flags     [I] Permission to operate the shared memory
 *
 * @param xref      [I] rref、cref or uref of process
 *
 * @param key       [O] Key for the share memory
 *
 * @return          E_HM_OK if succeed, error code if fail
 */
int hm_ashm_grant(int sd, uint32_t flags, xref_t xref, uint64_t *key);
/*
 * Grant a opened anonymous share memory to a process specified by cnode_idx
 *
 * @param sd        [I] Share memory descriptor
 *
 * @param flags     [I] Permission to operate the shared memory
 *
 * @param tgt_idx   [I] target process cnode index
 *
 * @param key       [O] Key for the share memory
 *
 * @return          E_HM_OK if succeed, error code if fail
 */
int hm_ashm_grant_by_cnode_idx(int sd, uint32_t flags, uint32_t tgt_idx, uint64_t *key);
/*
 * Transfer the caller's permission on share memory to the process specified
 * by the @xref
 *
 * @param key       [I] Key for the share memory whose access permission is transferred
 *
 * @param xref      [I] rref、cref or uref of process
 *
 * @return          E_HM_OK if succeed, error code if fail
 *
 * @note            Only vfs can invoke this function and the process specified
 *		    by @xref must be devhost
 */
int hm_ashm_grant_trans(uint64_t key, xref_t xref);
#endif /* ULIBS_LIBHMSRV_SYS_HM_ASHM_H */
