/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 30 11:20:00 2019
 */

#include <sys/mman.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <hmkernel/capability.h>

#include <libmem/flags.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <libsysif/utils.h>
#include <libsysif/uvmm/api.h>
#include <libsysif/sysmgr/api.h>

#include <libhmsrv_sys/hm_vm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

#include <vfs.h>

struct vm_shm_open_attr {
	rref_t srv_rref;
	int32_t static_shm_idx;
	unsigned int oflags;
	unsigned short acc_mode;
};

struct vm_shm_open_ret {
	unsigned long long shm_id;
	unsigned long len;
	unsigned long long paddr;
	uint32_t cnode_idx;
};

int hm_vm_check_gpa_list(const struct gpa_list_s *gpa_list)
{
	int rc = E_HM_OK;

	if (gpa_list == NULL) {
		rc = E_HM_INVAL;
	} else if ((gpa_list->nr_slots == 0) ||
		   (gpa_list->nr_slots > VM_GPA_SLOTS_MAX)) {
		rc = E_HM_INVAL;
	} else {
		unsigned int nr;

		for (nr = 0; nr < gpa_list->nr_slots; nr++) {
			if (((gpa_list->slots[nr].gpa & PAGE_OFFMASK) != 0UL) ||
			    ((gpa_list->slots[nr].length & PAGE_OFFMASK) != 0UL) ||
			    (gpa_list->slots[nr].length == 0UL)) {
				rc = E_HM_INVAL;
				break;
			}
		}
	}
	return rc;
}

int hm_vm_map_iomem(struct vm_iomem_args *args)
{
	int err = E_HM_OK;

	if (args == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = actvcall_hmcall_vm_map_iomem(args->vm_cref, args->regions, args->region_num, args->map_type);
	}

	if (err != E_HM_OK) {
		hm_error("iomem failed, err=%s\n", hmstrerror(err));
	}
	return err;
}

int hm_vm_map_shmmem(struct vm_shmmem_args *args)
{
	int i;
	int err = E_HM_OK;
	struct __actvret_hmcall_vm_map_shmmem ret;

	if ((args == NULL) || (args->hpa == NULL) ||
	    (args->gpa == NULL) || (args->region_size == NULL)) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		mem_zero_s(ret);
		err = actvcall_hmcall_vm_map_shmmem(args->vm_cref, &ret);
		if (err != E_HM_OK) {
			hm_error("shmmem failed, err=%s\n", hmstrerror(err));
		} else {
			for (i = 0; i < VM_SHM_REGIONS_NUM; i++) {
				args->hpa[i] = ret.shm_regions.hpa[i];
				args->gpa[i] = ret.shm_regions.gpa[i];
				args->region_size[i] = ret.shm_regions.size[i];
				hm_debug("vmem%d: hpa %"PRIx64", gpa %"PRIx64", size %"PRIx64" \n",
					 i, args->hpa[i], args->gpa[i], args->region_size[i]);
			}
		}
	}
	return err;
}

static uint32_t prot_of_oflags(unsigned int oflags)
{
	uint32_t prot_flags;

	if ((oflags & UVMM_SHM_O_READONLY) != 0U) {
		prot_flags = MPROT_READ;
	} else if ((oflags & UVMM_SHM_O_WRONLY)) {
		prot_flags = MPROT_WRITE;
	} else {
		prot_flags = (MPROT_READ | MPROT_WRITE);
	}
	return prot_flags;
}

static int map_anon_shm(uint64_t shm_key, uint32_t cnode_idx, size_t len,
			void *addr, uint32_t oflags, unsigned int mflag, void **rvaddr)
{
	int shm_id;
	int rc = E_HM_OK;

	shm_id = hm_ashm_open_auth(shm_key, cnode_idx, len,
				   SHM_O_RDWR | SHM_O_WITH_PA, NULL);
	/* return error code when id < 0 */
	if (shm_id < 0) {
		rc = shm_id;
		hm_debug("hm_ashm_open_auth failed, err=%s\n", hmstrerror(shm_id));
	}

	if (rc == E_HM_OK) {
		uint32_t flags = MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED | MFLAG_MAP_POPULATE | mflag;
		int ret;

		if (addr != NULL) {
			flags |= MFLAG_MAP_FIXED;
		}
		*rvaddr = hm_mem_mmap_posix(addr, len, prot_of_oflags(oflags), flags,
					    shm_id, 0, &rc);
		if (rc != E_HM_OK) {
			hm_debug("hm_mem_mmap_posix failed, err=%s\n", hmstrerror(rc));
		}

		/* Close shm so we can realloc this static_shm. */
		ret = hm_ashm_close(shm_id);
		if (ret != E_HM_OK) {
			hm_warn("hm_ashm_close failed, ret=%s\n", hmstrerror(ret));
		}
	}

	return rc;
}

static int remote_shm_open(struct vm_shm_open_attr *attr, struct vm_shm_open_ret *ret)
{
	int err = E_HM_OK;
#ifdef CONFIG_VM_MODE_UM
	struct __actvret_hmcall_vm_um_shm_open actv_ret;

	err = actvcall_hmcall_vm_um_shm_open((unsigned int)attr->static_shm_idx,
					     attr->oflags, attr->acc_mode, &actv_ret);
	if (err != E_HM_OK) {
		hm_debug("open shm from sysmgr failed, err=%s\n", hmstrerror(err));
	}

	ret->shm_id = actv_ret.shm_id;
	ret->len = actv_ret.len;
	ret->paddr = actv_ret.paddr;
	ret->cnode_idx = hmsrv_sysmgr_cnode_idx();
#else
	struct __actvret_uvmmcall_uvmm_vm_shm_open actv_ret;
	struct sysarg_cnode_statrref stat = { 0 };

	mem_zero_s(actv_ret);

	err = actvxactcapcall_uvmmcall_uvmm_vm_shm_open(false, true, attr->srv_rref,
							attr->static_shm_idx,
							attr->oflags, attr->acc_mode, &actv_ret);
	if (err != E_HM_OK) {
		hm_debug("open shm from uvmm failed, err=%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = sysfast_cnode_stat_rref(&stat, attr->srv_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	}

	ret->shm_id = actv_ret.shm_id;
	ret->len = actv_ret.len;
	ret->paddr = actv_ret.paddr;
	ret->cnode_idx = stat.src_cnode_idx;
#endif

	return err;
}

static int remote_shm_close(rref_t rref, int32_t static_shm_idx)
{
	int err = E_HM_OK;
#ifdef CONFIG_VM_MODE_UM
	UNUSED(rref, static_shm_idx);
#else
	err = actvxactcapcall_uvmmcall_uvmm_vm_shm_close(false, true, rref, static_shm_idx);
#endif
	return err;
}

#define HUGETLB_CHECK_MASK 0x1FFFFF
static int check_mmap_param(unsigned int mflags, unsigned long len, const struct vm_shm_open_ret *open_ret)
{
	int ret = E_HM_OK;

	if (len > open_ret->len) {
		ret = E_HM_INVAL;
	}

	if ((ret == E_HM_OK) && ((mflags & MAP_HUGETLB) != 0U)) {
		if (((open_ret->paddr & HUGETLB_CHECK_MASK) != 0) ||
		    ((len & HUGETLB_CHECK_MASK) != 0)) {
			hm_debug("mmap with MAP_HUGETLB but addr and len is not aligined. pa=%#llx, len=%#lx\n",
				  open_ret->paddr, len);
			ret = E_HM_INVAL;
		}
	}

	return ret;
}

static int alloc_static_shm(rref_t uvmm_rref, int32_t static_shm_idx, void *addr,
			    unsigned int oflags, unsigned short acc_mode,
			    size_t len, unsigned int mflags, struct hm_vm_shm_info *ret_info)
{
	int ret;
	int err = E_HM_OK;
	void *rvaddr = NULL;
	struct vm_shm_open_attr open_attr;
	struct vm_shm_open_ret open_ret;
	size_t map_len = len;

	open_attr.srv_rref = uvmm_rref;
	open_attr.static_shm_idx = static_shm_idx;
	open_attr.oflags = oflags;
	open_attr.acc_mode = acc_mode;
	mem_zero_s(open_ret);

	if (ret_info == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = remote_shm_open(&open_attr, &open_ret);
	}

	if (err == E_HM_OK) {
		if (map_len == 0) {
			map_len = (size_t)open_ret.len;
		}
		err = check_mmap_param(mflags, map_len, &open_ret);
	}

	if (err == E_HM_OK) {
		err = map_anon_shm(open_ret.shm_id, open_ret.cnode_idx, map_len, addr, oflags, mflags, &rvaddr);
		if (err != E_HM_OK) {
			hm_debug("map anon shm failed, err=%s\n", hmstrerror(err));
			ret = remote_shm_close(uvmm_rref, static_shm_idx);
			if (ret != E_HM_OK) {
				hm_warn("uvmm shm close for roll back failed, ret=%s\n", hmstrerror(ret));
			}
		}
	}
	if (err == E_HM_OK) {
		ret_info->len   = map_len;
		ret_info->vaddr = rvaddr;
		ret_info->paddr = u64_to_ptr(open_ret.paddr, void);
	}
	return err;
}

int hm_vm_alloc_static_shm_with_va(rref_t uvmm_rref, int32_t static_shm_idx,
				   unsigned int oflags, unsigned short acc_mode,
				   void *addr, struct hm_vm_shm_info *ret_info)
{
	return alloc_static_shm(uvmm_rref, static_shm_idx, addr, oflags, acc_mode, 0, 0, ret_info);
}

/* Now offset is ignored, we don't use it. */
int hm_vm_alloc_static_shm_with_size(rref_t uvmm_rref, int32_t static_shm_idx,
				     unsigned int oflags, unsigned short acc_mode,
				     size_t len, size_t offset, struct hm_vm_shm_info *ret_info)
{
	UNUSED(offset);
	return alloc_static_shm(uvmm_rref, static_shm_idx, NULL, oflags, acc_mode, len, 0, ret_info);
}

int hm_vm_alloc_static_shm_with_flags(rref_t uvmm_rref, int32_t static_shm_idx,
				      unsigned int oflags, unsigned short acc_mode,
				      unsigned int mflags, struct hm_vm_shm_info *ret_info)
{
	return alloc_static_shm(uvmm_rref, static_shm_idx,
				NULL, oflags, acc_mode, 0, mflags, ret_info);
}

int hm_vm_alloc_static_shm(rref_t uvmm_rref, int32_t static_shm_idx,
			  unsigned int oflags, unsigned short acc_mode,
			  struct hm_vm_shm_info *ret_info)
{
	return alloc_static_shm(uvmm_rref, static_shm_idx,
				NULL, oflags, acc_mode, 0, 0, ret_info);
}

int hm_vm_free_static_shm(rref_t uvmm_rref, int32_t static_shm_idx,
			  struct hm_vm_shm_info *shm_info)
{
	int err = E_HM_OK;

	if (shm_info == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = remote_shm_close(uvmm_rref, static_shm_idx);
		if (err != E_HM_OK) {
			hm_warn("close shm from uvmm failed, err=%s\n", hmstrerror(err));
		}
	}

	if (err == E_HM_OK && shm_info->vaddr != NULL) {
		err = hm_mem_munmap(shm_info->vaddr, shm_info->len);
		if (err < 0) {
			hm_warn("munmap shm failed, err=%s\n", hmstrerror(err));
		}
	}

	return err;
}

int hm_vm_mmap_vdso(uint64_t gpa, uint64_t size)
{
	return actvcall_hmcall_vm_mmap_vdso(gpa, size);
}

int hm_vsock_vm_rx(rref_t uvmm_rref, const void *hdr, size_t hdr_len,
		   const void *data, size_t data_len)
{
	int err = E_HM_OK;

	if (hdr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = actvcapcall_uvmmcall_vsock_vm_rx(uvmm_rref,
						       hdr, hdr_len, data, data_len);
	}

	return err;
}

int hm_vm_mmap(const struct gpa_list_s *gpa_list, unsigned int prot,
	       unsigned int flags, int fd, long offset)
{
	int rc = E_HM_OK;

	/*
	 * Virtual machine mapping must specify
	 * the guest physical address list.
	 */
	if (!is_map_with_fixed(flags)) {
		rc = E_HM_INVAL;
	} else {
		rc = hm_vm_check_gpa_list(gpa_list);
	}

	if (rc == E_HM_OK) {
		struct hm_actv_buf gpa_list_buf;

		flags |= MFLAG_MAP_VM;
		hm_actv_buf_init(&gpa_list_buf, gpa_list, sizeof(struct gpa_list_s));
		rc = actvxactcall_hmcall_vm_mmap(false, true, prot, flags, offset, fd, gpa_list_buf);
		if (rc != E_HM_OK) {
			hm_debug("vm mmap failed, err=%s\n", hmstrerror(rc));
		}
	}
	return rc;
}

int hm_vm_munmap(const struct gpa_list_s *gpa_list)
{
	int rc;

	rc = hm_vm_check_gpa_list(gpa_list);
	if (rc == E_HM_OK) {
		struct hm_actv_buf gpa_list_buf;

		hm_actv_buf_init(&gpa_list_buf, gpa_list, sizeof(struct gpa_list_s));
		rc = actvcall_hmcall_vm_munmap(gpa_list_buf);
		if (rc != E_HM_OK) {
			hm_debug("vm munap failed, err=%s\n", hmstrerror(rc));
		}
	}
	return rc;
}

int hm_vm_assert_irq(xref_t vm_xref, unsigned int vcpu_id,
		     unsigned int irq_id, unsigned int level)
{
	return syscap_VMAssertIrq(vm_xref.cref, vcpu_id, irq_id, level);
}

int hm_vm_assert_lpi(xref_t vm_xref, unsigned int device_id,
		     unsigned int event_id, unsigned int set)
{
	return syscap_VMAssertLpi(vm_xref.cref, device_id, event_id, set);
}

int hm_vm_assert_fiq(xref_t vm_xref, unsigned int vcpu_id)
{
	return syscap_VMAssertFiq(vm_xref.cref, vcpu_id);
}

int hm_vm_map_to_process(struct vm_map_to_process_args *args)
{
	int err = E_HM_OK;

	if (args == NULL || args->regions == NULL || args->region_num == 0) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = actvcall_hmcall_vm_map_to_process(args->vm_rref, args->regions, args->region_num);
	}
	return err;
}

int hm_vm_gpa_to_hpa(uintptr_t gpa, uintptr_t *hpa)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_vm_gpa_to_hpa actv_ret;

	if (hpa == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_zero_s(actv_ret);
		err = actvcall_hmcall_vm_gpa_to_hpa(gpa, &actv_ret);
		if (err == E_HM_OK) {
			if (safe_copy(ptr_to_void(hpa), ptr_to_type(&(actv_ret.hpa), const void *),
				      sizeof(uintptr_t)) != E_HM_OK) {
				err = E_HM_POSIX_FAULT;
			}
		}
	}
	return err;
}

int hm_vm_get_region_num(xref_t vm_xref, unsigned int *region_num, unsigned int type)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_vm_get_region_num ret;

	err = actvcall_hmcall_vm_get_region_num(vm_xref, type, &ret);
	*region_num = ret.region_num;
	return err;
}

int hm_vm_reset_vsmmu(cref_t vm_cref)
{
	return syscap_VMResetVsmmu(vm_cref);
}
