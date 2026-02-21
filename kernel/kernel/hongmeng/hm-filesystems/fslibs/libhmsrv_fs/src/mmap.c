/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of mmap
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 21 11:41:43 2020
 */

#include <vfs.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <fd.h>

int vfs_mmap_query(int fd, __u32 cnode_idx, char *service,
		   uint32_t len, unsigned int *fd_type)
{
	UNUSED(cnode_idx);
	int err;
	int remote_fd = -1;
	char *server_path = "fsmgr";
	struct __actvret_fscall_vfs_query_info query_ret = {0};

	if (service == NULL || fd_type == NULL) {
		return E_HM_INVAL;
	}

	err = actvcap_fscall(vfs_query_info, fd, 0, 0, &query_ret);
	if (err != E_HM_OK) {
		hm_debug("err: %s fd:%d\n", hmstrerror(err), fd);
		return err;
	}

	if (strlen(server_path) + 1U > len) {
		return E_HM_INVAL;
	}

	if (strcpy_s(service, len, server_path) != 0) {
		hm_error("strcpy failed\n");
		return E_HM_POSIX_FAULT;
	}

	*fd_type = query_ret.file_type;
	remote_fd = query_ret.remote_fd;

	return remote_fd;
}

int vfs_mmap_create(rref_t rref, const struct vfs_mmap_create_info *info,
		    vfs_mmap_fileinfo *fileinfo)
{
	struct __actvret_fscall_vfs_mmap_create ret;
	int err;

	if (info == NULL || fileinfo == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(ret);
	err = actvxactcapcall_fscall_vfs_mmap_create(true, true, rref, info->fd_file, info->offset,
						 info->len, info->cnode_idx,
						 info->prot, info->flags, &ret);
	if (err < 0) {
		hm_debug("FS mmap_create failed %s\n", hmstrerror(err));
		return err;
	}

	fileinfo->node.index = (uint32_t)ret.index;
	fileinfo->node.dev_id = (uint64_t)ret.dev_id;
	fileinfo->node.dax = (uint32_t)ret.dax;
	fileinfo->fid = (unsigned long)ret.fid;
	fileinfo->page_size = ret.pagesize;
	fileinfo->seals = ret.seals;
	fileinfo->setprot = ret.setprot;

	return E_HM_OK;
}

int vfs_mmap_release_file(rref_t rref, unsigned long fid, __u32 cnode_idx)
{
	return actvcapcall_fscall_vfs_mmap_release_file(rref, fid, cnode_idx);
}

int vfs_mmap_change(rref_t rref, unsigned long fid, __u32 cnode_idx, int64_t offset,
		    uint64_t len, uint32_t prot)
{
	int err;

	err = actvcapcall_fscall_vfs_mmap_change(rref, fid, offset, len,
						 cnode_idx, prot);
	if (err < 0) {
		hm_error("FS mmap_change failed %s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int vfs_mprotect_check(rref_t rref, unsigned long fid, __u32 cnode_idx, uint32_t prot, bool shared)
{
	int err;

	err = actvxactcapcall_fscall_vfs_mprotect_check(true, true, rref, fid, cnode_idx, prot, shared);
	if (err < 0) {
		hm_error("FS mprotect_check failed %s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int vfs_mmap_fill_page(rref_t rref, unsigned long fid, __u32 cnode_idx,
		       uint64_t node_page_idx, uint64_t flags,
		       uintptr_t *vaddr)
{
	struct __actvret_fscall_vfs_mmap_fill_page ret;
	int err;

	if (vaddr == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(ret);
	err = actvcapcall_fscall_vfs_mmap_fill_page(rref, fid, node_page_idx,
						    cnode_idx, flags, &ret);
	if (err < 0) {
		return err;
	}

	*vaddr = (uintptr_t)ret.vaddr;

	return E_HM_OK;
}

int vfs_mmap_fetch_page(rref_t rref, unsigned long fid, __u32 cnode_idx,
			uint64_t node_page_idx, uintptr_t *vaddr, bool set_mmap)
{
	struct __actvret_fscall_vfs_mmap_fetch_page ret;
	int err;

	if (vaddr == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(ret);
	err = actvcapcall_fscall_vfs_mmap_fetch_page(rref, fid, node_page_idx,
						     cnode_idx, set_mmap, &ret);
	if (err < 0) {
		return err;
	}

	*vaddr = (uintptr_t)ret.vaddr;

	return E_HM_OK;
}

int vfs_mmap_change_page(rref_t rref, const vfs_mmap_node_t *node, unsigned long fid,
			uint32_t cnode_idx, uint64_t node_page_idx, uint32_t flag, int mmapcnt)
{
	if (node == NULL) {
		return E_HM_INVAL;
	}

	return actvcapcall_fscall_vfs_mmap_change_page(rref, node->index,
						       node->dev_id,
						       fid, cnode_idx,
						       node_page_idx,
						       flag, mmapcnt);
}
