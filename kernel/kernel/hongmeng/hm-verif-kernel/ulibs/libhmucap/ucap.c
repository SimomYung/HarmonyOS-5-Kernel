/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: The api of the ucap
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 30 15:43:05 2018
 */

#include <libhmucap/ucap.h>

#include <libhmlog/hmlog.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <libstrict/strict.h>
#include <libstrict/overflow.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>

#include <hmkernel/capability.h>

#include <libhwsecurec/securec.h>
#include <libsecure/bounds_check.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>

#include <libhmactv/actv.h>

#include <libhmucap/kobjpool.h>
#include "../libhmsync/raw_tls.h"

#include "ucap_internal.h"

int ucap_extend_ctable(cref_t cnode, unsigned int water)
{
	return ucap_default_ops.extend_ctable(cnode, water);
}

int ucap_cslots_water(cref_t cnode, enum hmobj cap_type)
{
	return ucap_default_ops.cslots_water(cnode, cap_type);
}

static cref_t
hm_ucap_new_cap_by_proxy(cref_t cnode, cref_t base_pmem,
		enum hmobj captype, __u8 right,
		unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4,
		bool create_proxy)
{
	__u64 newcap_flag;
	int free_slots;
	bool again = false;
	cref_t res = ERR_TO_REF(E_HM_INVAL);
	int water = ucap_default_ops.cslots_water(cnode, captype);
	int proxy_created = create_proxy ? __REF_IS_RREF(cnode) : 0;

	newcap_flag = __CNODE_PROXY_NEWCAPFLAG(captype, right, water, proxy_created);
	do {
		free_slots = syscap_CNodeNewCap(cnode, base_pmem, newcap_flag,
						p1, p2, p3, p4, &res);
		/* successful return */
		if (free_slots > water) {
			break;
		}

		/* try extend table but no need to care result. */
		if (free_slots > 0) {
			int err;

			err = ucap_default_ops.extend_ctable(cnode, (unsigned int)water);
			/* extend fail, but we have already created a new cap */
			if ((err < 0) && (err != E_HM_CNODE_CTABLE_EXTEND_ENOUGH)) {
				hm_warn("ucap_extend_ctable failed, err=%s\n",
					hmstrerror(err));
			}
			break;
		}

		/* error return */
		if (free_slots != E_HM_CNODE_CTABLE_NEED_EXTEND) {
			res = ERR_TO_REF(free_slots);
			break;
		}

		/* try to extend ctable and new cap again */
		again = true;
		/* water level too low */
		free_slots = ucap_default_ops.extend_ctable(cnode, (unsigned int)water);
		if (free_slots < 0) {
			if (free_slots == E_HM_EXTENDTBL) {
				/*
				 * This is the case of recursively extend_ctable,
				 * we reserved certain amount of slots for memory
				 * usage in extend_ctable, including buddy memory
				 * usage and slab extend.
				 */
				newcap_flag = __CNODE_PROXY_NEWCAPFLAG(captype, right, 0, proxy_created);
			} else if (free_slots != E_HM_CNODE_CTABLE_EXTEND_ENOUGH) {
				hm_warn("newcap fail and extend ctable fail, err=%s\n",
					hmstrerror(free_slots));
				again = false;
			} else {
				/* do nothing */
			}
		}
	} while (again);

	return res;
}

cref_t
hm_ucap_new_cap(cref_t cnode, cref_t base_pmem,
		enum hmobj captype, __u8 right,
		unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4)
{
	return hm_ucap_new_cap_by_proxy(cnode, base_pmem,
				       captype, right,
				       p1, p2, p3, p4, true);
}

cref_t
hm_ucap_new_cap_noproxy(cref_t cnode, cref_t base_pmem,
		enum hmobj captype, __u8 right,
		unsigned long p1, unsigned long p2,
		unsigned long p3, unsigned long p4)
{
	return hm_ucap_new_cap_by_proxy(cnode, base_pmem,
				       captype, right,
				       p1, p2, p3, p4, false);
}

int hm_ucap_delete_cap(cref_t cnode, cref_t kobj)
{
	int err;
	int retry = 0;

#define DEBUG_TIMES	10
#define WARN_TIMES	25
	do {
		err = syscap_CNodeDeleteCap(cnode, kobj);
		retry++;
	} while (err == E_HM_CNODE_DEL_USED && retry < RETRY_TIMES);

	if (err < 0) {
		if (err != E_HM_CNODE_DEL_USED) {
			hm_error("syscap_CNodeDeleteCap failed, err=%s, retry times=%d\n",
				 hmstrerror(err), retry);
		} else {
			if (retry >= WARN_TIMES) {
				hm_debug("syscap_CNodeDeleteCap retry times=%d\n", retry);
			} else if (retry >= DEBUG_TIMES) {
				hm_debug("syscap_CNodeDeleteCap retry times=%d\n", retry);
			} else {
				/* retry times < DEBUG_TIMES, do nothing */
			}
		}
	}

	if (retry == RETRY_TIMES && err < 0) {
		int ret;
		struct sysarg_cnode_statcap stat;

		mem_zero_s(stat);
		ret = syscap_CNodeStatCap(cnode, &stat, kobj, CNODE_STATCAP_TYPE);
		BUG_ON(ret < 0, "syscap_CNodeStatCap failed, err=%s\n", hmstrerror(ret));

		hm_debug("syscap_CNodeDeleteCap retry times=%d, captype=%u\n",
			retry, stat.captype);
	}

#undef WARN_TIMES
#undef DEBUG_TIMES
	return err;
}

cref_t
ucap_new_ctable_mem(cref_t cnode, const uint64_t *pfn, uint64_t nrpfn)
{
	__u64 newcap_flag;
	int free_slots;
	cref_t res = ERR_TO_REF(E_HM_INVAL);

	newcap_flag = __CNODE_PROXY_NEWCAPFLAG(hmobj_PMEM, 0xff, 0, __REF_IS_RREF(cnode));
	free_slots = syscap_CNodeNewCap(cnode, 0,
					newcap_flag,
					(unsigned long)(uintptr_t)pfn,
					(unsigned long)nrpfn, 0, 0, &res);
	if (free_slots < 0) {
		res = ERR_TO_REF(free_slots);
	}
	return res;
}

enum table_type {
	RTABLE = 0,
	UTABLE,
	TABLE_MAX,
};

static const int err_table_enough[TABLE_MAX] = {
	[RTABLE] = ERR_AS_INT(E_HM_CNODE_RTABLE_EXTEND_ENOUGH),
	[UTABLE] = ERR_AS_INT(E_HM_CNODE_UTABLE_EXTEND_ENOUGH),
};

static const int err_table_noslot[TABLE_MAX] = {
	[RTABLE] = ERR_AS_INT(E_HM_CNODE_NO_RTBLSLOT),
	[UTABLE] = ERR_AS_INT(E_HM_CNODE_NO_UTBLSLOT),
};

static int
do_extend_table(cref_t cnode, enum table_type type, unsigned int water)
{
	struct ucap_mem mem;
	cref_t pmem_cref;
	int err;

	mem_zero_s(mem);
	pmem_cref = ucap_default_ops.alloc_pmem(cnode, PAGE_SIZE, &mem);
	if (IS_REF_ERR(pmem_cref)) {
		hm_warn("ucap_alloc_pmem failed, err=%s\n",
			hmstrerror(REF_TO_ERR(pmem_cref)));
		err = REF_TO_ERR(pmem_cref);
	} else {
		switch (type) {
		case RTABLE:
			err = syscap_CNodeExtendRTable(cnode, pmem_cref, water);
			break;
		case UTABLE:
			err = syscap_CNodeExtendUTable(cnode, pmem_cref, water);
			break;
		case TABLE_MAX:
			/* fall-through */
		default:
			hm_debug("wrong table type, type=%u\n", (unsigned int)type);
			err = E_HM_INVAL;
			break;
		}

		if (err < 0) {
			int ret = ucap_default_ops.free_pmem(cnode, &mem);
			if (ret < 0) {
				hm_warn("Fail to free pmem, memory leak, err=%s\n",
					hmstrerror(ret));
			}
		} else {
			/*
			 * Table pmems are destroyed in process exiting procedure
			 * and cannot be freed until cnode is destroyed.
			 * ucap_default_ops.record_table_pmem is not expected to
			 * be failed, we just report a warning here for bug location.
			 */
			int ret = ucap_default_ops.record_table_pmem(cnode, &mem);
			if (ret < 0) {
				hm_warn("record pmem failed: %s, lead to memory leak\n", hmstrerror(ret));
				err = ret;
			}
		}
	}

	return err;
}

/* return _ENOUGH if success, return other <0 if failed, never return >=0 */
static int extend_table(cref_t cnode, enum table_type type)
{
	int err = do_extend_table(cnode, type, 1);
	return (err < 0) ? err : err_table_enough[type];
}

#define until_got_slot(cnode, err, table)				\
	for ((err) = err_table_enough[table];				\
	     (err) == err_table_enough[table];				\
	     (err) = ((err) == err_table_noslot[table]) ? extend_table(cnode, table) : (err))

/*
 * Require an rref on tgt_cnode to prepare to acquire granted capability in
 * the src_cnode_idx's cnode
 * Usual next step: hm_ucap_grant
 * Free rref: hm_ucap_reject
 * If flags is CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_RREF, the rref will be
 * free when using hm_ucap_revoke to free the matched uref
 */
rref_t hm_ucap_require(cref_t tgt_cnode, uint32_t src_cnode_idx,
		       unsigned long flags)
{
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	int err;

	until_got_slot(tgt_cnode, err, RTABLE) {
		err = syscap_CNodeRequire(tgt_cnode, src_cnode_idx, flags,
					  &rref);
	}

	if (err < 0) {
		rref = ERR_TO_REF(err);
	}

	return rref;
}


/*
 * Grant a capbility in src_cnode to the tgt_rref's cnode, and make a
 * matched uref on src_cnode, the grant information records on both
 * tgt_rref's rtable and uref's utable
 * Destroy the grant: hm_ucap_reject or hm_ucap_revoke
 * Free uref: hm_ucap_revoke
 * If flags is CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF, the uref will be
 * free when using hm_ucap_reject to free the matched rref
 */
uref_t hm_ucap_grant(cref_t src_cnode, rref_t tgt_rref, unsigned long flags,
		     cref_t cref, uint8_t rights, unsigned long args)
{
	uref_t uref = ERR_TO_REF(E_HM_INVAL);
	int err;

	until_got_slot(src_cnode, err, UTABLE) {
		err = syscap_CNodeGrant(src_cnode, tgt_rref, flags, cref,
					rights, args, &uref);
	}

	if (err < 0) {
		uref = ERR_TO_REF(err);
	}

	return uref;
}

/*
 * Export an capbility and make a uref on src_cnode to prepare to grant
 * this capbility to the tgt_cnode_idx's cnode
 * Usual next step: hm_ucap_accept_grant
 * Free uref: hm_ucap_revoke
 * If flags is CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF, the uref will be
 * free when using hm_ucap_reject to free the matched rref
 */
uref_t hm_ucap_export(cref_t src_cnode, uint32_t tgt_cnode_idx,
		      unsigned long flags, cref_t cref, uint8_t rights,
		      unsigned long args)
{
	uref_t uref = ERR_TO_REF(E_HM_INVAL);
	int err;

	until_got_slot(src_cnode, err, UTABLE) {
		err = syscap_CNodeExport(src_cnode, tgt_cnode_idx, flags, cref,
					 rights, args, &uref);
	}

	if (err < 0) {
		uref = ERR_TO_REF(err);
	}

	return uref;
}

/*
 * Accept the grant from the src_uref's capbility to tgt_cnode, and make
 * a matched rref on tgt_cnode, the grant information records on both
 * rref's rtable and src_uref's utable
 * Destroy the grant: hm_ucap_reject or hm_ucap_revoke
 * Free rref: hm_ucap_reject
 * If flags is CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_RREF, the rref will be
 * free when using hm_ucap_revoke to free the matched uref
 */
rref_t hm_ucap_accept_grant(cref_t tgt_cnode, uref_t src_uref, unsigned long flags)
{
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	int err;

	until_got_slot(tgt_cnode, err, RTABLE) {
		err = syscap_CNodeAcceptGrant(tgt_cnode, src_uref, flags,
					      &rref);
	}

	if (err < 0) {
		rref = ERR_TO_REF(err);
	}

	return rref;
}

/*
 * Free the uref's utable and destroy the grant from the uref to
 * matched rref if exists. The matched rref will also be free if
 * flags is CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_RREF
 */
int hm_ucap_revoke(cref_t src_cnode, uref_t uref)
{
	int err;

	err = syscap_CNodeRevoke(src_cnode, uref);
	if (err < 0) {
		hm_warn("Revoke uref failed, uref=%"PRIxref", err=%s\n",
			uref, hmstrerror(err));
	}
	return err;
}

/*
 * Free the rref's rtable and destroy the grant from matched uref
 * to the rref if exists. The matched uref will also be free if
 * flags is CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF
 */
int hm_ucap_reject(cref_t tgt_cnode, rref_t rref)
{
	int err;

	err = syscap_CNodeReject(tgt_cnode, rref);
	if (err < 0) {
		hm_warn("Reject rref failed,  rref=%"PRIxref", err=%s\n",
			rref, hmstrerror(err));
	}
	return err;
}

/*
 * Move src_rref's grant to dst_rref, the src_rref will be free
 * if move success.
 */
int hm_ucap_grant_move(cref_t src_cnode, rref_t dst_rref, rref_t src_rref)
{
	int err;

	err = syscap_CNodeGrantMove(src_cnode, dst_rref, src_rref);
	if (err < 0) {
		hm_warn("GrantMove failed, src_rref=%"PRIxref", dst_rref=%"PRIxref", err=%s\n",
			src_rref, dst_rref, hmstrerror(err));
	}
	return err;
}

/*
 * Record on the src_rref's rtable that matched uref's capability's export
 * will be move to dst_cnode_idx's cnode.
 * Usual next step: hm_ucap_accept_move
 */
int hm_ucap_export_move(cref_t src_cnode, rref_t src_rref, uint32_t dst_cnode_idx)
{
	int err;

	err = syscap_CNodeExportMove(src_cnode, src_rref, dst_cnode_idx);
	if (err < 0) {
		hm_warn("ExportMove failed, src_rref=%"PRIxref", err=%s\n",
			src_rref,  hmstrerror(err));
	}
	return err;
}

/*
 * Move the src_rref's matched uref's capability's export to dst_cnode, and make
 * a new matched rref on dst_cnode and do the grant. Free the src_rref.
 */
rref_t hm_ucap_accept_move(cref_t dst_cnode, rref_t src_rref, unsigned long flags)
{
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	int err;

	until_got_slot(dst_cnode, err, RTABLE) {
		err = syscap_CNodeAcceptMove(dst_cnode, src_rref, flags, &rref);
	}
	if (err < 0) {
		rref = ERR_TO_REF(err);
	}

	return rref;
}

static int
extend_table_to_water(cref_t cnode, enum table_type type, unsigned int water)
{
	int free_slots;
	int err = E_HM_OK;

	do {
		free_slots = do_extend_table(cnode, type, water);
	} while ((free_slots >= E_HM_OK) && ((unsigned int)free_slots < water));

	if ((free_slots < 0) && (free_slots != err_table_enough[type])) {
		err = free_slots;
	}

	return err;
}

int hm_ucap_extend_utable(cref_t cnode, unsigned int water)
{
	return extend_table_to_water(cnode, UTABLE, water);
}

int hm_ucap_extend_rtable(cref_t cnode, unsigned int water)
{
	return extend_table_to_water(cnode, RTABLE, water);
}

int kobj_check_captype(cref_t cnode, cref_t cref, enum hmobj captype)
{
	int err;
	struct sysarg_cnode_statcap stat = {0};

	err = syscap_CNodeStatCap(cnode, &stat, cref, CNODE_STATCAP_TYPE);
	if (err < 0) {
		hm_warn("cnode stat cap failed, err=%s\n", hmstrerror(err));
	}
	if ((err == E_HM_OK) && (stat.captype != (__u32)captype)) {
		hm_warn("check captype failed, expect=%u, actual=%u\n",
			enum_hmobj_to_uint(captype), stat.captype);
		err = E_HM_INVAL;
	}

	return err;
}

cref_t hm_ucap_new_permission(uint64_t permission)
{
	int err;
	cref_t ret;

	struct __actvret_hmcall_proc_new_permission new_permissionret;

	mem_zero_s(new_permissionret);
	err = actvcall_hmcall_proc_new_permission(permission, &new_permissionret);
	if (err < 0) {
		hm_debug("failed to create permission, %s\n", hmstrerror(err));
		ret = ERR_TO_REF(err);
	} else {
		ret = new_permissionret.cref;
	}

	return ret;
}

int hm_ucap_delete_permission(cref_t cref)
{
	int err;

	err = kobj_check_captype(0, cref, hmobj_Permission);
	if (err >= 0) {
		err = do_delete_cap(0, cref);
		if (err < 0) {
			hm_warn("failed to delete permission, err=%s\n", hmstrerror(err));
		}
	}

	return err < 0 ? err : E_HM_OK;
}

int hm_ucap_bind_permission(cref_t tgt_cnode, cref_t permission_cref)
{
	int err;

	err = syscap_CNodeBindPermission(tgt_cnode, permission_cref);
	if (err < 0) {
		hm_warn("failed to bind permission, err=%s\n", hmstrerror(err));
	}

	return err < 0 ? err : E_HM_OK;
}

int hm_ucap_unbind_permission(cref_t tgt_cnode)
{
	int err;

	err = syscap_CNodeUnBindPermission(tgt_cnode);
	if (err < 0) {
		hm_warn("failed to unbind permission, err=%s\n", hmstrerror(err));
	}

	return err < 0 ? err : E_HM_OK;
}

int ucap_spawn_cnode(struct sysarg_cnode_spawn_req *spawn_req,
		     struct sysarg_cnode_spawn_res *spawn_res)
{
	int err;
	until_got_slot(0, err, RTABLE) {
		err = syscap_CNodeSpawnCNode(0, CNODE_SPAWNFLAGS(spawn_req->right),
					     spawn_req, spawn_res);
	}
	return err;
}

uint64_t hm_ucap_kobj_alloc_size_dump(void)
{
	return kobj_alloc_size_dump();
}

void hm_ucap_kobj_allocators_lock_parent(void)
{
	kobj_allocators_lock();
}

void hm_ucap_kobj_allocators_unlock_parent(void)
{
	kobj_allocators_unlock();
}

void hm_ucap_kobj_allocators_unlock_child(void)
{
	kobj_allocators_unlock();
}

cref_t hm_ucap_new_epoll(cref_t cnode, const struct __sysarg_epoll_info *info)
{
	int err = E_HM_OK;
	struct kobj_slot *slot = NULL;
	cref_t ep_cref = ERR_TO_REF(E_HM_INVAL);

	slot = kobj_alloc_slot(cnode, hmobj_Epoll,
			       (uint64_t)__CAP_TCB_OBJ_SIZE);
	if (slot == NULL) {
		err = E_HM_NOMEM;
		ep_cref = ERR_TO_REF(E_HM_NOMEM);
		hm_warn("failed to alloc pmem for epoll\n");
	}

	if (err == E_HM_OK) {
		IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");
		ep_cref = hm_ucap_new_cap(cnode, slot->pmem->mem.pmem,
					   hmobj_Epoll, 0xff,
					   ptr_to_ulong(info),
					   0UL, 0UL, 0UL);
		if (IS_REF_ERR(ep_cref)) {
			err = REF_TO_ERR(ep_cref);
			hm_warn("failed to create epoll: %s\n", hmstrerror(err));
			kobj_free_slot(cnode, slot);
		}
	}

	if (err == E_HM_OK) {
		err = kobj_record_slot(ep_cref, slot);
		if (err != E_HM_OK) {
			(void)hm_ucap_delete_cap(0, ep_cref);
			kobj_free_slot(0, slot);
			ep_cref = ERR_TO_REF(err);
		}
	}
	return ep_cref;
}

int hm_ucap_delete_epoll(cref_t cnode, cref_t ep_cref)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(cnode, ep_cref, hmobj_Epoll);
	if (err >= 0) {
		slot = kobj_remove_slot(ep_cref);
		if (slot == NULL) {
			err = E_HM_NOENT;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(cnode, ep_cref);
		if (err < 0) {
			hm_warn("failed to delete epoll, %s\n", hmstrerror(err));
			if (kobj_record_slot(ep_cref, slot) != E_HM_OK) {
				(void)do_delete_cap(cnode, ep_cref);
				kobj_free_slot(cnode, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(cnode, slot);
	}

	return err < 0 ? err : E_HM_OK;
}
