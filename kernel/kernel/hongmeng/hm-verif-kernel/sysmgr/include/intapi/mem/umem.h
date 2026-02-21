/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Interfaces for accessing to user memory by sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 13 22:16:37 2020
 */
#ifndef SYSMGR_INTAPI_MEM_UMEM_H
#define SYSMGR_INTAPI_MEM_UMEM_H
/*
 * All memory used in user processes is mapped in sysmgr, except
 * that for kernel object.
 *
 * Therefore, sysmgr can access those user process memory page
 * by page. And all interfaces provided here are base on it.
 */
#include <intapi/mem/file.h>
#include <intapi/mem/vspace.h>
#include <libhmelf/elf.h>

/*
 * umem_read
 *
 * read data from user memory to sysmgr buffer
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr buffer
 * @param bytes:  byte number of data to read
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_read(struct vspace_s *vspace,
	      const void *uptr, void *buf, size_t bytes);
/*
 * umem_read_no_lockvs
 *
 * read data from user memory to sysmgr buffer without lock user vspace
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr buffer
 * @param bytes:  byte number of data to read
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_read_no_lockvs(struct vspace_s *vspace,
	      const void *uptr, void *buf, size_t bytes);
/*
 * umem_read_noload
 *
 * read data from user memory to sysmgr buffer without loading pages
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr buffer
 * @param bytes:  byte number of data to read
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_read_noload(struct vspace_s *vspace,
		     const void *uptr, void *buf, size_t bytes);
/*
 * umem_write
 *
 * write data to user memory
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr data
 * @param bytes:  byte number of data to write
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_write(struct vspace_s *vspace,
	       void *uptr, const void *data, size_t bytes);
/*
 * umem_write
 *
 * write data to user memory without lock user vspace
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr data
 * @param bytes:  byte number of data to write
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_write_no_lockvs(struct vspace_s *vspace,
	       void *uptr, const void *data, size_t bytes);

/*
 * umem_zero
 *
 * clear range of user memory
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param bytes:  byte number of user memory to clear
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_zero(struct vspace_s *vspace,
	      void *uptr, size_t bytes);

/*
 * umem_force_write
 *
 * write data to user memory without perm check
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr data
 * @param bytes:  byte number of data to write
 *
 * @return Return E_HM_OK if read success, error number if fail.
 *
 * @note It has the same behavior with umem_write except that it
 *       is able to write data to private mapping user memory that
 *       has no write protection.
 *       This interface has security risk. And user should ensure
 *       the safety of its usage.
 */
int umem_force_write(struct vspace_s *vspace,
		     void *uptr, const void *data, size_t bytes);

/*
 * umem_force_read
 *
 * read data to user memory without perm check
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param buf:    pointer for sysmgr data
 * @param bytes:  byte number of buf to read
 *
 * @return Return E_HM_OK if read success, error number if fail.
 *
 * @note It has the same behavior with umem_read except that it
 *       is able to read data to private mapping user memory that
 *       has no read protection.
 *       This interface has security risk. And user should ensure
 *       the safety of its usage.
 */
int umem_force_read(struct vspace_s *vspace,
		    const void *uptr, void *buf, size_t bytes);

/*
 * umem_force_zero
 *
 * clear range of user memory without perm check
 *
 * @param vspace: vspace of user process
 * @param uptr:   pointer to user memory
 * @param bytes:  byte number of user memory to clear
 *
 * @return Return E_HM_OK if read success, error number if fail.
 *
 * @note It has the same behavior with umem_zero except that it
 *       is able to clear range that has no write protection.
 *       This interface has security risk. And user should ensure
 *       the safety of its usage.
 */
int umem_force_zero(struct vspace_s *vspace,
		    void *uptr, size_t bytes);

/*
 * umem_attach_stat
 *
 * attach sysmgr's pointer to fs_stat or devhost_stat
 *
 * @param vspace:  vspace of VFS process
 * @param uptr:    pointer to fs_stat in VFS's vspace
 * @param stat:    pointer needs to point to stat
 * @param length:  sizeof struct stat
 *
 * @return Return E_HM_OK if attached success, error number if fail.
 *
 * @note: This interface could attach pointer to fs_stat in VFS's
 *        vspace, it could protect fs_stat's page of releasing to
 *        buddy when sysmgr reading it.
 */
int umem_attach_stat(struct vspace_s *vspace,
		     const void *uptr,
		     void **stat, size_t length);

/*
 * umem_detach_stat
 *
 * detach sysmgr's pointer from stat
 *
 * @param stat: pointer point to stat
 *
 * @return Return E_HM_OK if get success, error number if fail.
 *
 * @note This interface will detach sysmgr's pointer from fs_stat
 *       in VFS's vspace, it will release protection of fs_stat's
 *       page.
 */
int umem_detach_stat(void *stat);

/*
 * umem_futex_get_sysaddr
 *
 * get sysmgr address for user futex
 *
 * @param vspace:  vspace of user process
 * @param uptr:    pointer for user address attached futex
 * @param sptr:    pointer of sysmgr attached this futex
 * @param check_writable: check whether the uptr is writable
 * @param check_shared: check whether the uptr is shared
 *
 * @return Return E_HM_OK if read success, error number if fail.
 *
 * @note This action will hold one refcnt of the page for uaddr. And User
 *       should use umem_futex_put_sysaddr to put its refcnt.
 */
int umem_futex_get_sysaddr(struct vspace_s *vspace,
			   const int *uptr, int **sptr,
			   bool check_writable, bool check_shared);

/*
 * umem_futex_put_sysaddr
 *
 * put page refcnt get by umem_futex_get_sysaddr
 *
 * @param futex_sptr:    pointer of sysmgr attached this futex
 *
 */
__no_sanitize_thread void umem_futex_put_sysaddr(const int *futex_sptr);

/*
 * umem_operate_uaddr
 *
 * The sysmgr cannot directly access the user address.
 * Find the va address of the sysmgr corresponding to the
 * user address and operate the value in hook callback mode.
 *
 * @param vspace: 	vspace of user process
 * @param uptr: 	pointer for user address
 * @param args:		operator args
 * @param hook: 	operator function
 *
 * @return Return E_HM_OK if operate success, error number if fail.
 */
int umem_operate_uaddr(struct vspace_s *vspace, const void *uptr, void *args,
		       int hook(void *args, const void *sptr));

/*
 * umem_copy_to_umem
 *
 * copy data from one process to another
 *
 * @param src_vspace:  vspace of source process
 * @param dst_vspace:  vspace of destination process
 * @param src:         pointer of source memory
 * @param dst:         pointer of destination memory
 * @param bytes:       bytes of data need to copy
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_copy_to_umem(struct vspace_s *src_vspace,
		      struct vspace_s *dst_vspace,
		      const void *src, void *dst, size_t bytes);

/*
 * Load elf data to memory of user process by sysmgr
 *
 * @param vspace	[I] Vspace of user process
 * @param ctx		[I] Elf context with headers and operations for elf
 * @param phdr		[I] First address of elf file section array
 * @param data		[I] Memory buffer to store elf data in user process
 *
 * @return		Return E_HM_OK if load success, error number if fail.
 *
 * @note		The memory of user process to store the elf data should be
 *			in one vregion. It fails when user memory covers more than
 *			one vregions.
 */
int umem_load_elf_segment(struct vspace_s *vspace,
			  struct elf_context *ctx,
			  const struct elf_generic_phdr *phdr, uintptr_t data);

/*
 * umem_get_user_pages
 *
 * Get pages from start to start + len.
 *
 * @param vspace		[I] Vspace of user process
 * @param vaddr:		[I]Virtual address of the page to be get
 * @param len:			[I]The length of range
 * @param ret_pa_array:		[I]pa array use to store pa
 * @param array_size:		[I]pa array size
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_get_user_pages(struct vspace_s *vs, uintptr_t vaddr, size_t len,
			unsigned long long *ret_pa_array, size_t array_size, bool writable);

int process_umem_get_user_pages(unsigned int cnode_idx, unsigned long long addr,
				size_t length, unsigned long long *ret_pa_array,
				size_t array_size, bool writable);

bool fusion_sys_va_range_overlap_linear_map(uintptr_t vstart, uintptr_t vend);

int umem_get_anon_user_pages(struct vspace_s *vs, uintptr_t vaddr, size_t len,
			unsigned long long *ret_pa_array, size_t array_size);

/*
 * umem_put_user_pages
 *
 * Put pages from start to start + len.
 *
 * @param len:			[I]The length of range
 * @param ret_pa_array:		[I]pa array use to store pa
 * @param array_size:		[I]pa array size
 *
 * @return Return E_HM_OK if read success, error number if fail.
 */
int umem_put_user_pages(size_t len, unsigned long long *ret_pa_array, size_t array_size);
int umem_put_user_page(unsigned long long pfn);
#endif /* SYSMGR_INTAPI_MEM_UMEM_H */
