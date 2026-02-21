/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: APIs for vregion
 * Author: Huawei OS Kernel Lab
 * Created: Thu May 25 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_VREGION_H
#define INTAPI_CTRLMEM_KOBJS_VREGION_H

#include <hmkernel/ctrlmem/kobjs/vregion.h>
#include <intapi/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <vmem/vspace.h>

#include <intapi/ctrlmem/pcache.h>

#include <stdint.h>

#ifdef CONFIG_SYSMGR_CTRLMEM_PAGEFAULT
static inline void vrobj_vrshared_lock_and_consume_oplog(struct vspace_vrshared_s *vrshared)
{
	ctrlmem_kobj_wrlock(&vrshared->rwlock);
	/*
	 * Consume all oplog.
	 * This lock makes sure kernel is impossible to generate page fault
	 * on this vspace after we gain this lock. Since all vregion write
	 * operations should be done in critical section, we are sure vregion
	 * in oplog is valid when we consume them.
	 */
	ctrlmem_pcache_oplog_consume_all();
	/*
	 * After consume all entries observable in oplog, wait for all
	 * unprocessed entry finished. (unprocessed entry is entries read by some
	 * consumer but not finished yet).
	 */
	ctrlmem_pcache_oplog_wait_for_unprocessed();
}

static inline void vrobj_vrshared_lock(struct vspace_vrshared_s *vrshared)
{
	ctrlmem_kobj_wrlock(&vrshared->rwlock);
}

static inline void vrobj_vrshared_unlock(struct vspace_vrshared_s *vrshared)
{
	ctrlmem_kobj_wrunlock(&vrshared->rwlock);
}
#else /* CONFIG_SYSMGR_CTRLMEM_PAGEFAULT */
static inline void vrobj_vrshared_lock_and_consume_oplog(struct vspace_vrshared_s *vrshared)
{
	UNUSED(vrshared);
}

static inline void vrobj_vrshared_lock(struct vspace_vrshared_s *vrshared)
{
	UNUSED(vrshared);
}

static inline void vrobj_vrshared_unlock(struct vspace_vrshared_s *vrshared)
{
	UNUSED(vrshared);
}
#endif /* CONFIG_SYSMGR_CTRLMEM_PAGEFAULT */

#endif
