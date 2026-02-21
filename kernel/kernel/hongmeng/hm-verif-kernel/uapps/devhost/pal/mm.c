/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: PAL implement for mm operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 30 09:09:52 2019
 */
#include <devhost/memory.h>

#include <inttypes.h>
#include <libmem/umem.h>
#include <libmem/utils.h>
#include <libalgo/idr.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmactv/actv.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libdevhost/mm.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <sys/socket.h>

#include "devhost.h"
#include "devhost_backend.h"
#include <devhost/log.h>

#include <hmasm/ctrlmem/vspace.h>

#ifdef __aarch64__

static inline bool xact_owner_is_myself(void)
{
	return sysif_actv_fwd_cnode_idx() == g_dh_self_cnode_idx;
}

static inline unsigned int curr_nsupervs(void)
{
	unsigned int nsupervs = 0;

	if (likely(!xact_owner_is_myself())) {
		struct arch_actv_local *local = hm_actv_current_arch_actv_local();
		nsupervs = __RPC_INFO_DECODE_XACT_SLOTIDX(local->rpcinfo);
	}
	return nsupervs;
}

static inline unsigned int curr_raw_nsupervs(void)
{
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	unsigned int nsupervs = 0;

	nsupervs = __RPC_INFO_DECODE_XACT_SLOTIDX(local->rpcinfo);

	return nsupervs;
}

static inline void *__supervs_xact_addr(void *addr)
{
	void *supervs_addr = NULL;
	unsigned int nsupervs = curr_nsupervs();
	if (likely(nsupervs != 0u)) {
		__u64 offset = __ARCH_SUPERVS_SUBSPACE_SIZE * nsupervs;
		supervs_addr = (void *)((__u64)(unsigned long)(addr) + offset);
	}
	return supervs_addr;
}

static inline void *supervs_xact_addr(void *addr)
{
	void *ret = NULL;

	addr = untagged_addr(addr);
	if (likely((uintptr_t)(addr) < __ARCH_SUPERVS_SUBSPACE_SIZE)) {
		ret = __supervs_xact_addr(addr);
	}
	return ret;
}

static inline const void *__supervs_xact_addr_const(const void *addr)
{
	const void *supervs_addr = NULL;
	unsigned int nsupervs = curr_nsupervs();
	if (likely(nsupervs != 0u)) {
		__u64 offset = __ARCH_SUPERVS_SUBSPACE_SIZE * nsupervs;
		supervs_addr = (const void *)((__u64)(unsigned long)(addr) + offset);
	}
	return supervs_addr;
}

static inline const void *supervs_xact_addr_const(const void *addr)
{
	const void *ret = NULL;

	addr = untagged_addr(addr);
	if (likely((uintptr_t)(addr) < __ARCH_SUPERVS_SUBSPACE_SIZE)) {
		ret = __supervs_xact_addr_const(addr);
	}
	return ret;
}

static inline const void *supervs_xact_raw_addr_const(const void *addr,
						      unsigned int nsupervs)
{
	const void *ret = NULL;

	addr = untagged_addr(addr);
	if (likely((uintptr_t)(addr) < __ARCH_SUPERVS_SUBSPACE_SIZE &&
		   nsupervs != 0u)) {
		__u64 offset = __ARCH_SUPERVS_SUBSPACE_SIZE * nsupervs;
		ret = (const void *)((__u64)(unsigned long)(addr) + offset);
	}
	return ret;
}

static inline void *supervs_xact_raw_addr(void *addr, unsigned int nsupervs)
{
	void *ret = NULL;

	addr = untagged_addr(addr);
	if (likely((uintptr_t)(addr) < __ARCH_SUPERVS_SUBSPACE_SIZE &&
		   nsupervs != 0u)) {
		__u64 offset = __ARCH_SUPERVS_SUBSPACE_SIZE * nsupervs;
		ret = (void *)((__u64)(unsigned long)(addr) + offset);
	}
	return ret;
}

#else

static inline unsigned int curr_raw_nsupervs(void)
{
	return 0;
}

static inline const void *supervs_xact_addr_const(const void *addr)
{
	UNUSED(addr);
	return NULL;
}

static inline void *supervs_xact_addr(void *addr)
{
	UNUSED(addr);
	return NULL;
}

static inline const void *supervs_xact_raw_addr_const(const void *addr,
						      unsigned int nsupervs)
{
	UNUSED(addr);
	UNUSED(nsupervs);
	return NULL;
}

static inline void *supervs_xact_raw_addr(void *addr, unsigned int nsupervs)
{
	UNUSED(addr);
	UNUSED(nsupervs);
	return NULL;
}
#endif

int devhost_pal_strcpy_from_user(void *dst, const void *src, unsigned long n)
{
	int ret;

	const void *supervs_src = supervs_xact_addr_const(src);
	if (likely(supervs_src != NULL)) {
		ret = safe_strncpy(dst, supervs_src, n);
	} else {
		ret = E_HM_NOMEM;
	}

	if (unlikely(ret != 0)) {
		ret = hm_strcpy_from_user(dst, src, n);
		if (ret < 0) {
			dh_error("strcpy from user failed, ret=%s\n", hmstrerror(ret));
		}
	}
	return ret;
}

int devhost_pal_copy_from_caller(void *dst, const void *src, unsigned long n)
{
	int ret = E_HM_OK;

	if (dst == NULL || src == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_copy_from_caller(dst, src, n);
	if (ret < 0) {
		dh_error("copy from caller failed, ret=%s\n", hmstrerror(ret));
	}
	return ret;
}

int devhost_pal_copy_to_caller(void *dst, const void *src, unsigned long n)
{
	int ret = E_HM_OK;

	if (dst == NULL || src == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_copy_to_caller(dst, src, n);
	if (ret < 0) {
		dh_error("copy to caller failed, ret=%s\n", hmstrerror(ret));
	}
	return ret;
}

int devhost_pal_copy_from_user(void *dst, const void *src, unsigned long n)
{
	int ret = E_HM_OK;

	const void *supervs_src = supervs_xact_addr_const(src);
	if (likely(supervs_src != NULL)) {
		ret = safe_copy(dst, supervs_src, n);
	} else {
		ret = E_HM_NOMEM;
	}

	if (unlikely(ret != 0)) {
		ret = hm_copy_from_user(dst, src, n);
		if (ret < 0) {
			dh_error("copy from user failed, ret=%s\n", hmstrerror(ret));
		}
	}
	return ret;
}

int devhost_pal_copy_to_user(void *dst, const void *src, unsigned long n)
{
	int ret = E_HM_OK;

	void *supervs_dst = supervs_xact_addr(dst);
	if (likely(supervs_dst != NULL)) {
		ret = safe_copy(supervs_dst, src, n);
	} else {
		ret = E_HM_NOMEM;
	}

	if (unlikely(ret != 0)) {
		ret = hm_copy_to_user(dst, src, n);
		if (ret < 0) {
			dh_error("copy to user failed, ret=%s\n", hmstrerror(ret));
		}
	}
	return ret;
}

int devhost_pal_copy_in_user(void *dst, const void *src, unsigned long n)
{
	int ret = E_HM_OK;

	void *supervs_dst = supervs_xact_addr(dst);
	const void *supervs_src = supervs_xact_addr_const(src);
	if ((supervs_dst != NULL) && (supervs_src != NULL)) {
		ret = safe_copy(supervs_dst, supervs_src, n);
	} else {
		ret = E_HM_NOMEM;
	}

	if (ret != 0) {
		ret = hm_actv_copy_in_xact_vm(dst, src, n);
		if (ret < 0) {
			dh_error("copy in user failed, ret=%s\n", hmstrerror(ret));
		}
	}
	return ret;
}

static inline int ____devhost_pal_copy_from_user(void *dst, const void *src,
					       unsigned long n, int ret)
{
	int res = ret;
	if (unlikely(res != 0)) {
		res = hm_copy_from_user(dst, src, n);
		if (res < 0) {
			dh_error("copy from user failed, ret=%s\n", hmstrerror(res));
		}
	}

	return res;
}

static inline int __devhost_pal_copy_from_user(void *dst, const void *src,
					       unsigned long n, unsigned int nvs)
{
	int ret = E_HM_OK;

	const void *supervs_src = supervs_xact_raw_addr_const(src, nvs);
	if (likely(supervs_src != NULL)) {
		ret = safe_copy(dst, supervs_src, n);
	} else {
		ret = E_HM_NOMEM;
	}

	return ____devhost_pal_copy_from_user(dst, src, n, ret);
}

static inline int ____devhost_pal_copy_to_user(void *dst, const void *src,
					     unsigned long n, int ret)
{
	int res = ret;
	if (unlikely(res != 0)) {
		res = hm_copy_to_user(dst, src, n);
		if (res < 0) {
			dh_error("copy to user failed, ret=%s\n", hmstrerror(ret));
		}
	}

	return res;
}

static inline int __devhost_pal_copy_to_user(void *dst, const void *src,
					     unsigned long n, unsigned int nvs)
{
	int ret = E_HM_OK;

	void *supervs_dst = supervs_xact_raw_addr(dst, nvs);
	if (likely(supervs_dst != NULL)) {
		ret = safe_copy(supervs_dst, src, n);
	} else {
		ret = E_HM_NOMEM;
	}

	return ____devhost_pal_copy_to_user(dst, src, n, ret);
}

/* for support user apps mmap and munmap */
STATIC_DEFINE_RAW_RWLOCK(g_vma_idr_lock);
static DEFINE_IDR(g_vma_idr);

int devhost_pal_umap_populate(const void *va, unsigned long long pa,
			      unsigned long len, unsigned int prot,
			      unsigned int flag)
{
	unsigned long long hint;
	int ret = E_HM_OK;

	hint = prot_to_hint((uint32_t)prot, 0, 0);
	if ((flag & LIBDH_REMAP_NORMAL_NC) != 0) {
		hint |= PGTABLE_ATTR_NOCACHE;
	} else if ((flag & LIBDH_REMAP_DEVICE) != 0) {
		hint |= PGTABLE_ATTR_DEVICE;
	}

	ret = actvxactcall_hmcall_mem_iomap_populate(true, true, va, len, pa, hint);
	if (ret != E_HM_OK) {
		dh_error("mem_iomap_populate failed, ret=%d\n", ret);
	}

	return ret;
}

static unsigned int g_vma_seq_id;
static
struct devhost_vma_info *devhost_pal_usermap_alloc(void)
{
	struct devhost_vma_info *vma = NULL;
	int vma_id = E_HM_INVAL;

	vma = calloc(1, sizeof(struct devhost_vma_info));
	if (vma == NULL) {
		dh_error("devhost vma_info alloc failed\n");
		return NULL;
	}

	RAW_RWLOCK_WR_GUARD(_, &g_vma_idr_lock);
	vma_id = idr_alloc(&g_vma_idr, vma);
	if (vma_id < 0) {
		dh_error("devhost vma_info idr_alloc failed\n");
		free(vma);
		return NULL;
	}

	vma->vma_id = (unsigned int)vma_id;
	vma->seq = g_vma_seq_id++;
	return vma;
}

static
void devhost_pal_usermap_release(const struct devhost_vma_info *vma_info)
{
	struct devhost_vma_info *vma = NULL;

	RAW_RWLOCK_WR_GUARD(_, &g_vma_idr_lock);
	vma = idr_remove(&g_vma_idr, vma_info->vma_id);
	if (vma == NULL) {
		dh_warn("remove id-%u failed\n", vma_info->vma_id);
		return;
	}
	if (vma != vma_info) {
		dh_error("invalid vma info [%u-%u], id [%u-%u]\n", vma_info->vma_id, vma->vma_id,
			 vma_info->seq, vma->seq);
	}

	free(vma);
}

int devhost_pal_usermap_prepare(unsigned long addr, unsigned long len,
				unsigned long prot, unsigned int flags,
				const bool iomap_prepared_ahead,
				struct devhost_vma_info **out)
{
	struct __actvret_hmcall_mem_iomap_prepare func_ret;
	struct devhost_vma_info *vma_info = NULL;
	void *user_vaddr = ulong_to_ptr(addr, void);
	int ret = E_HM_OK;

	if (out == NULL) {
		return E_HM_INVAL;
	}

	vma_info = devhost_pal_usermap_alloc();
	if (vma_info == NULL) {
		return E_HM_NOMEM;
	}
	if (!iomap_prepared_ahead) {
		ret = actvxactcall_hmcall_mem_iomap_prepare(
				true, true, /* `fwd`, `allow refwd` */
				user_vaddr, len, vma_info->vma_id,
				(unsigned int)prot, flags, &func_ret);
		if (ret != E_HM_OK) {
			devhost_pal_usermap_release(vma_info);
			dh_error("mem_iomap_prepare failed, len=%#lx, prot=%#lx, ret=%d\n",
				len, prot, ret);
			return ret;
		}
		vma_info->start = (unsigned long)func_ret.rva;
	} else {
		vma_info->start = addr;
	}

	vma_info->len = len;
	vma_info->prot = prot;
	vma_info->flags = flags;
	vma_info->priv = NULL;
	*out = vma_info;

	return E_HM_OK;
}

void devhost_pal_usermap_finish(const struct devhost_vma_info *vma_info,
				const bool iomap_prepared_ahead, int success, int update)
{
	unsigned int flag = (success != 0) ? 0U : MFLAG_IOMAP_RECLAIM;
	void *user_vaddr = ulong_to_ptr(vma_info->start, void);
	int ret = E_HM_OK;

	if (!iomap_prepared_ahead) {
		if (update != 0) {
			flag |= MFLAG_IOMAP_UPDATE | MFLAG_IOMAP_UPDATE_PROT;
		}
		/* vma_info->prot is from `mmap`. sysmgr don't know the val when `get_unmapped_area` */
		ret = actvxactcall_hmcall_mem_iomap_finish(true, true, (const void *)user_vaddr, flag,
							   vma_info->vma_id, vma_info->prot);
	}
	if (ret != E_HM_OK) {
		dh_warn("mem_iomap_finish failed, ret=%d\n", ret);
	}

	if (success == 0) {
		dh_warn("usermap failed[%u]: start=0x%lx, len=0x%lx, id=%u\n", vma_info->seq,
			vma_info->start, vma_info->len, vma_info->vma_id);
		devhost_pal_usermap_release(vma_info);
	}
}

int devhost_pal_usermap_munmap(unsigned long addr, unsigned long len)
{
	return actvxactcall_hmcall_mem_munmap(true, true, (const void*)addr, len);
}

int __do_devhost_usermap_munmap(unsigned int vma_id, unsigned long start, unsigned long len, int vs_is_releasing)
{
	struct devhost_vma_info *vma_info = NULL;
	int ret = E_HM_OK;

	RAW_RWLOCK_WR_GUARD_START(_, &g_vma_idr_lock);
	vma_info = (struct devhost_vma_info *)idr_find(&g_vma_idr, vma_id);
	if (vma_info == NULL) {
		dh_error("find id-%u failed: start=0x%p, len=0x%lx\n", vma_id, ulong_to_ptr(start, void), len);
		return E_HM_INVAL;
	}

	/*
	 * All must be released at once,
	 * releasing sub-region of the mapping area is unacceptable.
	 */
	if ((vma_info->len != len) || (vma_id != vma_info->vma_id) ||
	    (vma_info->start != start)) {
		dh_error("invalid vma_info[%u]'s len=%#lx, start=0x%p, id=%u: len=%#lx, start=0x%p, id=%u\n",
			 vma_info->seq, vma_info->len, ulong_to_ptr(vma_info->start, void), vma_info->vma_id, len, ulong_to_ptr(start, void), vma_id);
		return E_HM_INVAL;
	}

	vma_info = idr_remove(&g_vma_idr, vma_id);
	if (vma_info == NULL) {
		dh_warn("remove id-%u failed\n", vma_id);
		return E_HM_POSIX_NOENT;
	}
	RAW_RWLOCK_WR_GUARD_END(_, &g_vma_idr_lock);

	ret = devhost_backend_umap_munmap(vma_info->priv, vs_is_releasing);
	if (ret < 0) {
		dh_error("devhost backend umap munmap failed, ret=%d\n", ret);
	}

	free(vma_info);
	return ret;
}

void *devhost_pal_find_vma(unsigned long addr, unsigned long long vs_key)
{
	struct devhost_vma_info *vma_info = NULL;
	struct __actvret_hmcall_mem_iomap_statvregion vr_stat;
	int ret = E_HM_OK;
	bool actv_fwd = (vs_key == 0);

	ret = actvxactcall_hmcall_mem_iomap_statvregion(actv_fwd, true, (void *)addr, vs_key, &vr_stat);
	if (ret != E_HM_OK) {
		dh_error("query vr failed, %d\n", ret);
		return NULL;
	}
	RAW_RWLOCK_RD_GUARD_START(_, &g_vma_idr_lock);
	vma_info = (struct devhost_vma_info *)idr_find(&g_vma_idr, vr_stat.iomem_id);
	if (vma_info == NULL) {
		dh_error("find id-%u failed\n", vr_stat.iomem_id);
		return NULL;
	}
	RAW_RWLOCK_WR_GUARD_END(_, &g_vma_idr_lock);
	return vma_info->priv;
}

int devhost_pal_vm_zap(unsigned long addr, unsigned long len, unsigned int flags)
{
	return actvxactcall_hmcall_mem_madvise(!!(flags & LIBDH_VM_TO_USER), true,
					       (const void*)addr, len, POSIX_MADV_DONTNEED);
}

int __devhost_do_umap_page_fault(unsigned int vma_key, unsigned long long vr_key, unsigned long long addr,
				 unsigned int flags, unsigned long long offset)
{
	struct devhost_vma_info *vma_info = NULL;
	int ret = E_HM_INVAL;

	/*
	 * Due to sysmgr lock sequence requirements, the vregion lock is not held
	 * when sysmgr is page fault to devhost. Sysmgr uses unique vr_key to ensure
	 * vregion consistency. Devhost add rdlock to avoid UAF when unmap and page fualt are
	 * executed concurrently.
	 */
	ret = devhost_backend_handle_mm_rdlock();
	if (unlikely(ret != 0)) {
		ret = E_HM_INVAL;
		goto out;
	}
	RAW_RWLOCK_WR_GUARD_START(_, &g_vma_idr_lock);
	vma_info = (struct devhost_vma_info *)idr_find(&g_vma_idr, vma_key);
	if ((vma_info == NULL) || (vma_info->priv == NULL)) {
		dh_error("find valid vma-%u failed\n", vma_key);
		ret = E_HM_INVAL;
		goto unlock;
	}

	if ((addr - vma_info->start) != offset) {
		dh_error("invalid vma_info's offset=%#llx, offset=%#llx\n", offset, addr - vma_info->start);
		ret = E_HM_INVAL;
		goto unlock;
	}
	RAW_RWLOCK_WR_GUARD_END(_, &g_vma_idr_mutex);

	ret = devhost_backend_handle_page_fault(vma_info->priv, vr_key, addr, flags);
unlock:
	devhost_backend_handle_mm_rdunlock();
out:
	return ret;
}

static inline int __devhost_move_addr_to_kernel(struct msghdr *kmsg, void *uaddr,
						unsigned int nvs)
{
	if (__devhost_pal_copy_from_user(kmsg->msg_name, uaddr,
					 kmsg->msg_namelen, nvs)) {
		dh_error("move addr to kernel failed\n");
		return E_HM_FAULT;
	}
	return E_HM_OK;
}

static inline __always_inline
int kmsghdr_msg_name_set(struct msghdr *kmsg, struct msghdr *msg, unsigned int nvs)
{
	int ret = E_HM_OK;

	kmsg->msg_namelen = msg->msg_namelen;

	if (msg->msg_name == NULL) {
		kmsg->msg_namelen = 0;
		kmsg->msg_name = NULL;
		return ret;
	}

	if ((int)kmsg->msg_namelen < 0) {
		return E_HM_INVAL;
	}

	if (kmsg->msg_namelen > sizeof(struct sockaddr_storage)) {
		kmsg->msg_namelen = sizeof(struct sockaddr_storage);
	}

	if (msg->msg_name && kmsg->msg_namelen) {
		if (kmsg->msg_name) {
			ret = __devhost_move_addr_to_kernel(kmsg, msg->msg_name, nvs);
			if (ret < 0) {
				dh_error("copy msg_name from user failed\n");
				return E_HM_FAULT;
			}
		}
	}

	kmsg->msg_name = msg->msg_name;

	return ret;
}

int devhost_pal_copy_msghdr_from_user(void *dst, void *src)
{
	struct msghdr *kmsg = (struct msghdr *)dst;
	struct msghdr *umsg = (struct msghdr *)src;
	unsigned int nvs = curr_raw_nsupervs();
	struct msghdr msg = {0};
	int ret = E_HM_OK;

	ret = __devhost_pal_copy_from_user(&msg, umsg, sizeof(*umsg), nvs);
	if (ret < 0) {
		dh_error("copy msghdr from user failed\n");
		return E_HM_FAULT;
	}

	if (kmsg->msg_control && (int)msg.msg_controllen > 0 &&
	    (int)msg.msg_controllen <= (int)kmsg->msg_controllen) {
		ret = __devhost_pal_copy_from_user(kmsg->msg_control, msg.msg_control,
						   msg.msg_controllen, nvs);
		if (ret < 0) {
			dh_error("copy msg_control from user failed\n");
			return E_HM_FAULT;
		}
	}

	kmsg->msg_control = msg.msg_control;
	kmsg->msg_controllen = msg.msg_controllen;
	kmsg->msg_flags = msg.msg_flags;

	ret = kmsghdr_msg_name_set(kmsg, &msg, nvs);
	if (ret < 0) {
		return ret;
	}

	if ((unsigned int)msg.msg_iovlen <= (unsigned int)kmsg->msg_iovlen) {
		ret = __devhost_pal_copy_from_user(kmsg->msg_iov, msg.msg_iov,
						   msg.msg_iovlen * sizeof(struct iovec),
						   nvs);
		if (ret < 0) {
			dh_error("copy msg_iov from user failed\n");
			return E_HM_FAULT;
		}
	}

	kmsg->msg_iovlen = msg.msg_iovlen;
	kmsg->msg_iov = msg.msg_iov;

	return 0;
}

#define __devhost_put_user(x, ptr, n)						\
({										\
	__typeof__(*(ptr)) __val = (__typeof__(*(ptr)))(x);			\
	__typeof__(*(ptr)) *__p = (ptr);					\
	int __pu_err = 0;							\
	__pu_err = __devhost_pal_copy_to_user(__p, &__val, sizeof(*__p),	\
					      n) < 0 ? E_HM_FAULT : 0;		\
	__pu_err;								\
})

int devhost_pal_copy_msghdr_to_user(void *dst, void *src, void *uaddr, void *ucmsg, int ulen)
{
	struct msghdr *umsg = (struct msghdr *)dst;
	struct msghdr *kmsg = (struct msghdr *)src;
	unsigned int nvs = curr_raw_nsupervs();
	int ret;
	int len;

	ret = __devhost_put_user(kmsg->msg_flags, &umsg->msg_flags, nvs);
	if (ret < 0) {
		dh_error("put msg_flags failed\n");
		return E_HM_FAULT;
	}

	ret = __devhost_put_user(kmsg->msg_controllen, &umsg->msg_controllen, nvs);
	if (ret < 0) {
		dh_error("put msg_controllen failed\n");
		return E_HM_FAULT;
	}

	if (kmsg->msg_controllen > 0 && ucmsg) {
		ret = __devhost_pal_copy_to_user(ucmsg, kmsg->msg_control,
						 kmsg->msg_controllen, nvs);
		if (ret < 0) {
			dh_error("devhost copy control message failed, ignored\n");
		}
	}

	if (!uaddr) {
		return 0;
	}

	if (ulen < 0) {
		return E_HM_INVAL;
	}

	if ((unsigned int)ulen > kmsg->msg_namelen) {
		len = kmsg->msg_namelen;
	} else {
		len = ulen;
	}

	if (len > 0) {
		ret = __devhost_pal_copy_to_user(uaddr, kmsg->msg_name,
						 len, nvs);
		if (ret < 0) {
			dh_error("copy msg_name to user failed\n");
			return E_HM_FAULT;
		}
	}

	ret = __devhost_put_user(kmsg->msg_namelen, &umsg->msg_namelen, nvs);
	if (ret < 0) {
		dh_error("put msg_namelen failed\n");
		return E_HM_FAULT;
	}

	return 0;
}
