/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Mem function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 27 06:23:51 2019
 */

/* must be first include */
#include <sys/mman.h>
#include <sys/prctl.h>

#include <vfs.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <hmasm/lsyscall.h>

#include <lib/perfra/ksus.h>
#include <lib/trace/trace_misc.h>
#include <libmem/mstat.h>
#include <libmem/mprint.h>
#include <libhmucap/ucap.h>
#include <libhmlog/hmlog.h>
#include <libmem/flags.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <libbunch/bunch_ipc.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_thread.h>

#ifdef CONFIG_SPIF
struct ksus_desc_s ksus_desc;
struct ksus_desc_s *get_ksus_desc(void)
{
	return &ksus_desc;
}
#else
struct ksus_desc_s *get_ksus_desc(void)
{
	return NULL;
}
#endif

struct local_map_arg_s {
	const void *addr;
	size_t len;
	int fd;
	uint32_t prot;
	uint32_t flags;
	int64_t offset;
	size_t guard_size;
	rref_t srv_rref;
};

static int do_map_call(const struct local_map_arg_s *args, uintptr_t *rvaddr, bool *unmapped)
{
	struct __actvret_hmcall_mem_mmap actv_ret;
	struct __actvret_hmcall_mem_mmap_ex actv_ret_ex;
	int rc;

	mem_zero_s(actv_ret);
	mem_zero_s(actv_ret_ex);
	if (args->srv_rref == 0ULL) {
		if (args->guard_size != 0) {
			rc = actvxactcall_hmcall_mem_mmap_ex(false, /* !is_fwd */
						     true, /* allow_refwd */
						     args->addr, args->len, args->prot, args->flags,
						     args->fd, args->offset, args->guard_size,
						     &actv_ret_ex);
		} else {
			rc = actvxactcall_hmcall_mem_mmap(false, /* !is_fwd */
						     true, /* allow_refwd */
						     args->addr, args->len, args->prot, args->flags,
						     args->fd, args->offset,
						     &actv_ret);
		}
	} else {
		BUG_ON(args->guard_size != 0);
		rc = actvxactcapcall_hmcall_mem_mmap(false, /* !is_fwd */
						     true, /* allow_refwd */
						     args->srv_rref,
						     args->addr, args->len, args->prot, args->flags,
						     args->fd, args->offset,
						     &actv_ret);
	}
	if (args->guard_size != 0) {
		if (rc == E_HM_OK) {
			*rvaddr = (uintptr_t)actv_ret_ex.rvaddr;
		}
	} else {
		if (rc == E_HM_OK) {
			*rvaddr = (uintptr_t)actv_ret.rvaddr;
		} else {
			if ((actv_ret.rvaddr == MAP_FAILED_VADDR) && (unmapped != NULL)) {
				*unmapped = (actv_ret.unmapped != 0);
			}
		}
	}
	return rc;
}

static int do_mmap(rref_t srv, const void *addr, size_t len,
		   uint32_t prot, uint32_t flags,
		   int fd, int64_t offset, uintptr_t *rvaddr,
		   bool *unmapped)
{
	struct local_map_arg_s args;
	int rc;

	mem_zero_s(args);
	args.addr = addr;
	args.len = len;
	args.prot = prot;
	args.flags = flags;
	args.offset = offset;
	args.fd = fd;
	args.srv_rref = srv;
	args.guard_size = 0;
	rc = do_map_call(&args, rvaddr, unmapped);
	return rc;
}

static int do_mmap_ex(rref_t srv, const void *addr, size_t len,
		   uint32_t prot, uint32_t flags,
		   int fd, int64_t offset, uintptr_t *rvaddr, size_t guard_size)
{
	struct local_map_arg_s args;
	int rc;

	mem_zero_s(args);

	args.addr = addr;
	args.len = len;
	args.prot = prot;
	args.flags = flags;
	args.offset = offset;
	args.fd = fd;
	args.srv_rref = srv;
	args.guard_size = guard_size;
	rc = do_map_call(&args, rvaddr, NULL);
	return rc;
}

void *hm_mem_mmap_fixed(void *addr, size_t len,
			uint32_t prot, uint32_t flags,
			int32_t fd, int64_t offset,
			bool *unmapped, int *ret_err)
{
	uintptr_t rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	int rc;

	rc = do_mmap(0ULL, addr, len, prot, flags | __U(MAP_FIXED),
		     fd, offset, &rvaddr, unmapped);
	if (ret_err != NULL) {
		*ret_err = rc;
	}
	return ulong_to_ptr(rvaddr, void);
}

/* NOTE: this function is a native function, recommend use hm_mem_mmap */
void *hm_mem_mmap_posix(void *addr, size_t len,
			uint32_t prot, uint32_t flags,
			int32_t fd, int64_t offset, int *ret_err)
{
	uintptr_t rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	int rc;

	rc = do_mmap(0ULL, addr, len, prot, flags, fd, offset, &rvaddr, NULL);
	if (ret_err != NULL) {
		*ret_err = rc;
	}
	return ulong_to_ptr(rvaddr, void);
}

void *hm_server_mem_mmap_posix(rref_t server,
			       const void *addr, size_t len,
			       uint32_t prot, uint32_t flags,
			       int32_t fd, int64_t offset, int *ret_err)
{
	uintptr_t rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	int rc;

	if (!IS_REF_VALID(server)) {
		rc = E_HM_INVAL;
	} else {
		rc = do_mmap(server, addr, len, prot, flags, fd, offset, &rvaddr, NULL);
	}
	if (ret_err != NULL) {
		*ret_err = rc;
	}
	return ulong_to_ptr(rvaddr, void);
}

void *hm_mem_mmap(void *addr, size_t len,
		  uint32_t prot, uint32_t flags,
		  int32_t fd, int64_t offset)
{
	uintptr_t rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	int rc;

	rc = do_mmap(0ULL, addr, len, prot, flags, fd, offset, &rvaddr, NULL);
	if (rc != E_HM_OK) {
		rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	}

	return ulong_to_ptr(rvaddr, void);
}

void *hm_mem_mmap_ex(void *addr, size_t len,
		  uint32_t prot, uint32_t flags,
		  int32_t fd, int64_t offset, size_t guard_size)
{
	uintptr_t rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	int rc;

	rc = do_mmap_ex(0ULL, addr, len, prot, flags, fd, offset, &rvaddr, guard_size);
	if (rc != E_HM_OK) {
		rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	}
	return ulong_to_ptr(rvaddr, void);
}

static inline int check_munmap_args(const void *addr, size_t len)
{
	int rc = E_HM_OK;

	if ((len == 0UL) || !PAGE_ALIGNED(ptr_to_type(addr, uintptr_t)) ||
	    !is_range_valid((uintptr_t)addr, len)) {
		rc = E_HM_INVAL;
	}
	return rc;
}

int hm_mem_munmap(void *addr, size_t len)
{
	return actvcall_hmcall_mem_munmap(addr, len);
}

int hm_server_mem_munmap(rref_t server, const void *addr_ori, size_t len)
{
	size_t align_len;
	int rc = E_HM_OK;
	const void *addr = untagged_addr(addr_ori);

	if (!IS_REF_VALID(server)) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = check_munmap_args(addr, len);
	}
	if (rc == E_HM_OK) {
		align_len = PAGE_ALIGN_UP(len);
		rc = actvcapcall_hmcall_mem_munmap(server, addr, align_len);
	}
	return rc;
}

long hm_mem_mremap(void *old_addr, size_t old_len, size_t new_len,
		    uint32_t flags, void *new_addr)
{
	return lsyscall_mem_mremap(old_addr, old_len, new_len,
				   flags, new_addr);
}

int hm_mem_rmap_unmap_page(const void *addr, uint32_t *flags)
{
	struct __actvret_hmcall_mem_rmap_unmap_page actv_ret;
	int rc = E_HM_OK;

	if (flags == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		if (PAGE_ALIGNED(ptr_to_type(addr, uintptr_t))) {
			rc = actvcall_hmcall_mem_rmap_unmap_page(addr, &actv_ret);
		} else {
			rc = E_HM_INVAL;
		}
		*flags = actv_ret.flags;
	}

	return rc;
}

void *hm_mem_alloc_range_restricted(size_t length, unsigned long low_limit,
				    unsigned long high_limit, uint32_t flags,
				    unsigned long align_mask, unsigned long align_offset)
{
	uintptr_t rvaddr = ptr_to_type(MAP_FAILED, uintptr_t);
	struct __actvret_hmcall_mem_alloc_range_restricted actv_ret;
	int rc = E_HM_OK;
	size_t aligned_len = 0UL;

	mem_zero_s(actv_ret);
	if ((flags == 0) || (length == 0) || (high_limit <= low_limit) ||
	    (length > (SIZE_MAX - (size_t)PAGE_SIZE))) {
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && (align_offset != 0UL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		aligned_len = PAGE_ALIGN_UP(length);
		rc = actvxactcall_hmcall_mem_alloc_range_restricted(true, true, aligned_len,
								    low_limit, high_limit,
								    flags, align_mask,
								    align_offset, &actv_ret);
		if (rc == E_HM_OK) {
			rvaddr = (uintptr_t)actv_ret.rvaddr;
		}
	}

	return ulong_to_ptr(rvaddr, void);
}

long hm_mem_brk(const void *brk)
{
	return lsyscall_mem_brk(brk);
}

int hm_mem_mprotect(const void *addr, size_t len, uint32_t prot)
{
	return actvxactcall_hmcall_mem_mprotect(false, true, addr, len, prot);
}

int hm_mem_amend_prot(const void *addr, size_t len, uint32_t set_prot, uint32_t clear_prot)
{
	return actvcall_hmcall_mem_amend_prot(addr, len, set_prot, clear_prot);
}

long hm_mem_msync(const void *addr, size_t len, uint32_t flags)
{
	return lsyscall_syscall3(__NR_msync, ptr_to_type(addr, long), (long)len, (long)flags);
}

long hm_mem_mlock(const void *addr, size_t len)
{
	return lsyscall_syscall2(__NR_mlock, ptr_to_type(addr, long), (long)len);
}

long hm_mem_mlock2(const void *addr, size_t len, uint32_t flags)
{
	return lsyscall_syscall3(__NR_mlock2, ptr_to_type(addr, long), (long)len, (long)flags);
}

long hm_mem_munlock(const void *addr, size_t len)
{
	return lsyscall_syscall2(__NR_munlock, ptr_to_type(addr, long), (long)len);
}

long hm_mem_mlockall(uint32_t flags)
{
	return lsyscall_syscall1(__NR_mlockall, (long)flags);
}

long hm_mem_munlockall(void)
{
	return lsyscall_syscall0(__NR_munlockall);
}

/*
 * Drop all mmaped file pages of lru cache
 *
 * @return the droped file pages in normal cases.
 */
size_t hm_mem_drop_file_cache(void)
{
	struct __actvret_hmcall_mem_drop_file_cache actv_ret;
	int rc = E_HM_OK;
	size_t nr_droped = 0UL;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_mem_drop_file_cache(&actv_ret);
	if (rc == E_HM_OK) {
		nr_droped = actv_ret.nr_droped;
	}
	return nr_droped;
}

int hm_mem_madvise(const void *addr, size_t len, uint32_t flags)
{
	return actvcall_hmcall_mem_madvise(addr, len, flags);
}

cref_t hm_mem_alloc_pmem(size_t size)
{
	struct __actvret_hmcall_mem_alloc_pmem actv_ret;
	cref_t cref;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_mem_alloc_pmem((unsigned long)size, &actv_ret);
	if (rc == E_HM_OK) {
		cref = actv_ret.pmem_cref;
	} else {
		cref = ERR_TO_REF(rc);
	}
	return cref;
}

int hm_mem_free_pmem(cref_t cref)
{
	return actvcall_hmcall_mem_free_pmem(cref);
}

static int shm_check_name(const char *name)
{
	int rc = E_HM_OK;

	if (name == NULL) {
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) &&
	    (strnlen(name, MEMMGR_SHM_NAME_MAX) >= MEMMGR_SHM_NAME_MAX)) {
		hm_debug("path length oversize, max length support = %d\n",
			 (MEMMGR_SHM_NAME_MAX - 1));
		rc = E_HM_NAMETOOLONG;
	}

	return rc;
}

static int do_hm_mem_shm_open(struct memmgr_shm_name shm_name, size_t len, uint64_t oflags,
			      mode_t mode, size_t *ret_len)
{
	struct __actvret_hmcall_mem_shm_open fun_ret;
	int rc;

	mem_zero_s(fun_ret);
	rc = actvcall_hmcall_mem_shm_open(shm_name, len, (unsigned long long)oflags, mode, &fun_ret);
	if (rc < 0) {
		hm_debug("actvcall_hmcall_mem_shm_open failed with error: %s\n",
			 hmstrerror(rc));
	} else {
		if (ret_len != NULL) {
			*ret_len = (size_t)fun_ret.ret_len;
		}
		rc = fun_ret.shm_id;
	}

	return rc;
}
/*
 * Deal with client's add named share memory request and create it with DAC rights by mode
 *
 * @params name [I] share memory name acquired
 *
 * @params len [I] share memory length acquired
 *
 * @params oflags [I] Provide the operation flags of named share memory
 *
 * @params mode [I] Provide the new mode of shm for changeing dac rights
 *
 * @params ret_len [O] share memory length on success
 *
 * @return shm_id on success, error code on failure.
 */
int hm_mem_shm_open_with_mode(const char *name, size_t len, uint64_t oflags,
			      mode_t mode, size_t *ret_len)
{
	int rc;
	struct memmgr_shm_name shm_name;
	size_t aligned_len;

	rc = shm_check_name(name);
	if ((rc == E_HM_OK) && (len > (SIZE_MAX - (size_t)PAGE_SIZE))) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		mem_zero_s(shm_name);
		NOFAIL(strncpy_s(shm_name.path, MEMMGR_SHM_NAME_MAX, name, MEMMGR_SHM_NAME_MAX - 1));
		aligned_len = PAGE_ALIGN_UP(len);
		rc = do_hm_mem_shm_open(shm_name, aligned_len, oflags, mode, ret_len);
		while ((rc == E_HM_NOMEM) && ((oflags & SHM_O_NOFAIL) != 0U)) {
			(void)hm_thread_yield();
			rc = do_hm_mem_shm_open(shm_name, aligned_len, oflags, mode, ret_len);
		}
	}
	return rc;
}

int hm_mem_shm_open(const char *name, size_t len, uint64_t oflags,
		    size_t *ret_len)
{
	int rc;
	mode_t mode = (unsigned)S_IRUSR | (unsigned)S_IRGRP;

	rc = hm_mem_shm_open_with_mode(name, len, oflags, mode, ret_len);
	/*
	 * The return code of shm_open_with_mode is shm_id which is cannot
	 * less or equal than 0.
	 */
	if (rc <= 0) {
		hm_debug("shm_open_with_mode failed, err: %s\n", hmstrerror(rc));
	}

	return rc;
}

int hm_mem_shm_unlink(const char* name)
{
	int rc = E_HM_OK;
	struct memmgr_shm_name shm_name;

	if (name == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		mem_zero_s(shm_name);
		NOFAIL(strncpy_s(shm_name.path, MEMMGR_SHM_NAME_MAX, name, MEMMGR_SHM_NAME_MAX - 1));
		rc = actvcall_hmcall_mem_shm_unlink(shm_name);
		if (rc < 0) {
			hm_debug("actvcall_hmcall_mem_shm_unlink failed with error: %s\n",
				 hmstrerror(rc));
		}
	}
	return rc;
}

int hm_mem_shm_close(int32_t shm_id)
{
	return actvcall_hmcall_mem_shm_close(shm_id);
}

/*
 * hm_mem_shm_reclaim_pages - Reclaims a specified number
 * of physical pages from a specified address in the shared memory.
 *
 * @shm_id: the id of shared memory
 * @start_addr: start address of the shared memory to be reclaimed
 * @nr_pages: the number of reclaimed pages
 *
 * return value:
 *  0 if all pages are successfully reclaimed
 *  < 0 if no page is reclaimed
 *  > 0 if some pages are recycled and the return value
 *  indicates the number of reclaimed pages
 */
int hm_mem_shm_reclaim_pages(int32_t shm_id, const void *start_addr,
			     unsigned long nr_pages)
{
	int ret;

	if (start_addr == NULL) {
		hm_debug("invalid parameters start_addr ptr is NULL\n");
		ret = E_HM_INVAL;
	} else {
		ret = actvcall_hmcall_mem_shm_reclaim_pages(shm_id, start_addr, nr_pages);
		if (ret < 0) {
			hm_debug("actvcall_hmcall_mem_shm_reclaim_pages failed "
				 "with error: %s\n", hmstrerror(ret));
		}
	}
	return ret;
}

int hm_mem_query_shm_info(int32_t shm_id,
			  struct shm_node_info *shm_info)
{
	int rc;
	struct __actvret_hmcall_mem_query_shm_info info_ret;

	mem_zero_s(info_ret);

	rc = actvcall_hmcall_mem_query_shm_info(shm_id, &info_ret);
	/* judge msg call whether success */
	if (rc < 0) {
		hm_debug("actvcall_hmcall_mem_shm_info failed with error: %s\n",
			 hmstrerror(rc));
	} else {
		if (shm_info != NULL) {
			*shm_info = info_ret.shm_info;
		}
	}
	return rc;
}

/*
 * query the physical address of all pages in shared memory
 *
 * @shm_id: id of shared memory
 * @dst: the buffer to which the physical address is written
 * @size: the size(bytes) of dest buffer
 * @return: On success, the number of addresses written to the buffer pointed by
 *          @dst is returned. On error, error code is returned.
 */
int hm_mem_query_shm_paddr(int32_t shm_id,
			   unsigned long long *dst,
			   unsigned long size)
{
	int rc;

	rc = actvcall_hmcall_mem_query_shm_paddr(shm_id, dst, size);
	/* judge msg call whether success */
	if (rc < 0) {
		hm_debug("actvcall_hmcall_mem_shm_info failed with error: %s\n",
			 hmstrerror(rc));
	}
	return rc;
}

/*
 * dump all shm_node and its info as follows:
 * 1. shmid
 * 2. size
 * 3. name
 * 4. owner's name
 *
 * Remarks: This is a debug function, and only can work in
 * debug version image
 */
int hm_mem_shm_dump_info(void)
{
	int rc;

	rc = actvcall_hmcall_mem_shm_dump_info();
	if (rc < 0) {
		hm_debug("actvcall_hmcall_mem_shm_dump_info failed with error: %s\n",
			 hmstrerror(rc));
	}

	return rc;
}

struct mem_stat_config {
	rref_t server;
	unsigned int flags;
};

static int mem_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct mem_stat_config *config = NULL;

	config = (struct mem_stat_config *)data;
	return actvcall_hmcall_mem_stat(config->flags, *attr);
}

static int server_mem_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct mem_stat_config *config = NULL;

	config = (struct mem_stat_config *)data;
	return actvcapcall_hmcall_mem_stat(config->server, config->flags, *attr);
}

static int __hm_mem_stat_common(rref_t server, unsigned int flags, struct bunch *bunch)
{
	int rc = E_HM_OK;
	struct mem_stat_config config;
	struct bunch_ipc_reader_ctx ctx;

	if (bunch == NULL) {
		hm_debug("invalid bunch: 0x%lx\n", vaddr_of(bunch));
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		mem_zero_s(config);
		config.flags = flags;
		config.server = server;
		ctx.private_data = ptr_to_void(&config);
		ctx.target_info = (uint64_t)hmsrv_sysmgr_cnode_idx();
		if (server == (rref_t)0) {
			ctx.ipc = mem_stat_ipc;
		} else {
			ctx.ipc = server_mem_stat_ipc;
		}

		rc = bunch_ipc_request(bunch, &ctx);
		if (rc < 0) {
			hm_error("bunch ipc try request data with shm failed: %s\n",
				hmstrerror(rc));
		}
	}

	return rc;
}

int hm_mem_stat(unsigned int flags, struct bunch *bunch)
{
	int rc;

	rc = __hm_mem_stat_common(0, flags, bunch);
	if (rc != E_HM_OK) {
		hm_debug("mem_stat_common failed, err: %s\n", hmstrerror(rc));
	}

	return rc;
}

int hm_server_mem_stat(rref_t server, unsigned int flags, struct bunch *bunch)
{
	int rc;

	if (IS_REF_VALID(server)) {
		rc = __hm_mem_stat_common(server, flags, bunch);
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

#ifndef PR_SET_VMA
#define PR_SET_VMA 0x53564d41
#define PR_SET_VMA_ANON_NAME 0
#endif

int hm_mem_set_anon_name(const void *start, size_t len, const char *name)
{
	return lsyscall_proc_prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME,
				   ptr_to_ulong(start), (unsigned long)len,
				   ptr_to_ulong(name));
}

/*
 * Determine whether pages are resident in memory
 *
 * @params addr   [I] addr going to be scaned
 *
 * @params length [I] scaned size
 *
 * @params vec    [I/O] array for recording scan result
 *
 * @return Returns 0 on success, errno otherwise.
 */
long hm_mem_mincore(const void *addr, size_t length, const unsigned char *vec)
{
	return lsyscall_syscall3(__NR_mincore, ptr_to_type(addr, long), (long)length, ptr_to_type(vec, long));
}

int hm_mem_calculate(struct memstat_deviation *div)
{
	struct __actvret_hmcall_mem_calculate actv_ret;
	int rc = E_HM_OK;

	if (div != NULL) {
		mem_zero_s(actv_ret);
		rc = actvcall_hmcall_mem_calculate(&actv_ret);
		if (rc == E_HM_OK) {
			*div = actv_ret.div;
		}
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

/*
 * when fs process start up, it should call this interface to register itself,
 * otherwise sysmgr will ignore its statistics info.
 */
int hm_mem_fs_stat_register(const void *stat_addr)
{
	int ret;

	ret = actvcall_hmcall_mem_fs_stat_register(stat_addr);
	if (ret < 0) {
		hm_debug("actvcall_hmcall_mem_fs_register failed with error: %s\n",
			 hmstrerror(ret));
	}

	return ret;
}

int hm_mem_hugetlbfs_mount(const struct hgtlb_mount_args_s *args, unsigned int *order)
{
	struct __actvret_hmcall_mem_hugetlbfs_mount actv_ret;
	int rc;

	if ((args != NULL) && (order != NULL)) {
		mem_zero_s(actv_ret);
		rc = actvcall_hmcall_mem_hugetlbfs_mount(*args, &actv_ret);
		if (rc == E_HM_OK) {
			*order = actv_ret.order;
		}
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

int hm_mem_file_mapping_stat(struct hm_mem_fnode_s fnode,
			     struct file_map_info_s *map_info)
{
	struct __actvret_hmcall_mem_file_mapping_stat actv_ret;
	int rc;

	if (map_info != NULL) {
		mem_zero_s(actv_ret);
		rc = actvcall_hmcall_mem_file_mapping_stat(fnode.index, fnode.dev_id, &actv_ret);
		if (rc == E_HM_OK) {
			*map_info = actv_ret.map_info;
		}
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

/*
 * Deal with client's named share memory chmod request
 *
 * @params name [I] share memory name acquired
 *
 * @params mode [I] Provide the new mode of shm for changeing dac rights
 *
 * @return Returns E_HM_OK on success. On any error, returns an errno.
 */
int hm_mem_shm_chmod(const char *name, mode_t mode)
{
	int rc;
	struct memmgr_shm_name shm_name;

	rc = shm_check_name(name);
	if (rc == E_HM_OK) {
		mem_zero_s(shm_name);
		NOFAIL(strncpy_s(shm_name.path, MEMMGR_SHM_NAME_MAX, name, MEMMGR_SHM_NAME_MAX - 1));
		rc = actvcall_hmcall_mem_shm_chmod(shm_name, mode);
		if (rc < 0) {
			hm_debug("actvcall_hmcall_mem_shm_chmod failed with error: %s\n",
				 hmstrerror(rc));
		}
	}
	return rc;
}

/*
 * Dynamically initialize a cma area, size should be power-of-2 * PAGE_SIZE.
 *
 * @param size		[I] size to reserve for cma
 *
 * @param cma_id	[O] cma id if operation succeeds
 *
 * @return E_HM_OK in normal cases, and specific errno if failed.
 */
int hm_mem_cma_dyn_init(size_t size, unsigned int *cma_id)
{
	struct __actvret_hmcall_mem_cma_dyn_init actv_ret;
	int rc;

	if (cma_id != NULL) {
		mem_zero_s(actv_ret);
		rc = actvcall_hmcall_mem_cma_dyn_init(size, &actv_ret);
		if (rc == E_HM_OK) {
			*cma_id = actv_ret.cma_id;
		}
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

/*
 * CMA alloc space from reserved area.
 *
 * @param cma_id	[I] cma area to alloc from
 *
 * @param size		[I] size to alloc
 *
 * @param paddr		[O] physical address if operation succeeds
 *
 * @return E_HM_OK in normal cases, and specific errno if failed.
 */
int hm_mem_cma_alloc(unsigned int cma_id, size_t size,
		     unsigned int align_order,
		     unsigned long long *paddr)
{
	struct __actvret_hmcall_mem_cma_alloc actv_ret;
	int rc;

	if (paddr != NULL) {
		mem_zero_s(actv_ret);
		rc = actvcall_hmcall_mem_cma_alloc(cma_id, size, align_order, &actv_ret);
		if (rc == E_HM_OK) {
			*paddr = actv_ret.paddr;
		}
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

/*
 * CMA free space.
 *
 * @param cma_id	[I] cma area to free
 *
 * @param paddr		[I] physical address
 *
 * @param size		[I] size to free
 *
 * @return E_HM_OK in normal cases, and specific errno if failed.
 */
int hm_mem_cma_free(unsigned int cma_id, unsigned long long paddr, size_t size)
{
	return actvcall_hmcall_mem_cma_free(cma_id, paddr, size);
}

int hm_mem_cma_raw_map(uint64_t *vaddr, unsigned long long paddr,
		       unsigned long len, unsigned long prot,
		       unsigned int flags)
{
	int ret = E_HM_OK;
	uint64_t hint;

	if (len == 0) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hint = prot_to_hint((uint32_t)prot, 0, 0);
		ret = actvcall_hmcall_mem_raw_map(vaddr, paddr, len, flags, hint);
		if (ret != E_HM_OK) {
			hm_error("cma raw map failed, len=0x%lx err=%s\n",
				len, hmstrerror(ret));
		}
	}

	return ret;
}

static int hm_mem_resv_mem(uint64_t *vaddr, unsigned long len)
{
	struct __actvret_hmcall_mem_resv_mem hmcall_ret;
	int ret = E_HM_OK;

	if (len == 0) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		mem_zero_s(hmcall_ret);
		ret = actvcall_hmcall_mem_resv_mem(vaddr, len, &hmcall_ret);
		if (ret != E_HM_OK) {
			hm_error("mem resv mem failed, len=0x%lx err=%s\n",
				len, hmstrerror(ret));
		}
	}

	return ret;
}

int hm_mem_cma_setup(uint64_t *vaddr, unsigned long len)
{
	int ret = E_HM_OK;
	uint64_t hva;

	hva = ptr_to_u64(hm_mem_mmap(0, len,
			MPROT_READ | MPROT_WRITE,
			MFLAG_MAP_PRIVATE | MFLAG_MAP_ANONYMOUS,
			-1, 0));
	if (hva == (uint64_t)(uintptr_t)MAP_FAILED) {
		ret = E_HM_NOMEM;
		hm_info("mem mmap failed, err=%s\n", hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		ret = hm_mem_munmap((void *)(uintptr_t)hva, len);
		if (ret != E_HM_OK) {
			hm_info("mem munmap failed, err=%s\n",
				hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		ret = hm_mem_resv_mem((uint64_t *)(uintptr_t)hva, len);
		if (ret != E_HM_OK) {
			hm_error("cma resv mem failed, err=%s\n",
				 hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		*vaddr = hva;
	}

	return ret;
}

int hm_mem_enable_rsv_mem(uint64_t pfn_start, uint64_t pfn_end)
{
	int ret = E_HM_OK;

	ret = actvcall_hmcall_mem_enable_rsv_mem(pfn_start, pfn_end);
	if (ret != E_HM_OK) {
		hm_debug("mem enable rsv mem failed, err=%s\n", hmstrerror(ret));
	}

	return ret;
}

int hm_mem_devhost_stat_register(const void *stat_addr, size_t stat_size)
{
	int ret;

	ret = actvcall_hmcall_mem_devhost_stat_register(stat_addr, stat_size);
	if (ret < 0) {
		hm_error("actvcall_hmcall_mem_devhost_stat_register failed with error: %s, size=%zu\n",
			 hmstrerror(ret), stat_size);
	}

	return ret;
}

int hm_mem_extfrag_index_read(void *buf, size_t len)
{
	int ret;

	ret = actvcall_hmcall_mem_extfrag_index_read(buf, len);
	if (ret < 0) {
		hm_debug("actvcall_hmcall_mem_extfrag_index_read failed with error: %s, size=%zu\n",
			 hmstrerror(ret), len);
	}

	return ret;
}

int hm_mem_compact(void)
{
	int ret;
	ret = actvcall_hmcall_mem_compact();
	if (ret < 0) {
		hm_debug("actvcall_hmcall_mem_compact failed with error: %s\n", hmstrerror(ret));
	}

	return ret;
}

int hm_mem_cpa_prepare_memory(unsigned int flags, unsigned long water_mark_page,
			      unsigned long size, unsigned long heap_size,
			      unsigned long allocated_size)
{
	int ret;

	/*
	 * flags:
	 *  BIT(0) 1: CAN_DROP
	 *  BIT(1) 1: CAN_KILL
	 *  BIT(2) 1: CAN_RECLAIM
	 *  BIT(3) 1: CAN_COMPAT
	 *  BIT(4) 0: KILL_MODE_FREEMEM_MODEL  1: KILL_MODE_ZRAM_MODEL
	 *  BIT(5) 0: compat_model = 0  (MIGRATE_ASYNC)  1 : compat_model = 1 (MIGRATE_SYNC, ignore compat_order)
	 *  BIT(6-7)
	 *    00: called from cpa_prepare_memory_for_work
	 *    01: called from cpa_prepare_memory_for_alloc
	 *    10: called from do_size_remaining
	 *
	 * water_mark_page:
	 *  CPA water mark page, used when BIT[6:7] = 0b00 for determine whether to do actual work,
	 *  otherwise used for determine whether to do actual drop/reclaim
	 *
	 * size:
	 *  CPA alloc size, used when BIT[6:7] = 0b01 or 0b10, but size may different
	 *  (0b01, size is order aligned, 0b10, size is not order aligned)
	 *
	 * heap_size:
	 * cpa heap total size
	 *
	 * allocated_size:
	 * The size of memory allocated from CPA
	 */
	ret = actvcall_hmcall_mem_cpa_prepare_memory(flags, water_mark_page,
						     size, heap_size, allocated_size);
	if (ret < 0) {
		hm_debug("cpa prepare memory failed %s\n", hmstrerror(ret));
	}
	return ret;
}

void *hm_mem_mmap_lsyscall(void *addr, size_t len,
			   uint32_t prot, uint32_t flags,
			   int32_t fd, int64_t offset)
{
#ifndef __HOST_LLT__
	unsigned long ret = lsyscall_mem_mmap_lsyscall(addr, len, prot, flags,
						       fd, offset);
	return ulong_to_ptr(ret, void);
#else
	return NULL;
#endif
}

int hm_mem_munmap_lsyscall(void *addr, size_t len)
{
	return lsyscall_mem_munmap_lsyscall(addr, len);
}

int hm_mem_dump_pte_with_pgd(pid_t pid, uint64_t pgd, uintptr_t va)
{
	return actvcall_hmcall_mem_dump_pte_with_pgd(pid, pgd, va);
}
