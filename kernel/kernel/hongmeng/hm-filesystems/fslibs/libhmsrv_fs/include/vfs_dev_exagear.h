/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: dev exagear
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 21 09:49:41 2025
 */

#ifndef VFS_DEV_EXAGEAR_H
#define VFS_DEV_EXAGEAR_H

#define ___IOC(a, b, c, d)	(((a) << 30) | ((b) << 8) | (c) | ((d) << 16))
#define ___IOC_NONE		0U
#define ___IOC_WRITE		1U
#define ___IOC_READ		2U

#define ___IO(a, b)		___IOC(___IOC_NONE, (a), (b), 0)
#define ___IOW(a, b, c)		___IOC(___IOC_WRITE, (a), (b), sizeof(c))
#define ___IOR(a, b, c)		___IOC(___IOC_READ, (a), (b), sizeof(c))
#define ___IOWR(a, b, c)	___IOC(___IOC_READ | ___IOC_WRITE, (a), (b), sizeof(c))

#define __EXAGEAR32_IOCTL_BASE	'e'

/*
 * The ABI version for this module.
 *
 * Minor version increments indicate API additions and major version increments
 * indicate breaking changes.
 */
struct exagear32_abi_version {
	__u16 major;
	__u16 minor;
};
#define __EXAGEAR32_GET_VERSION						\
	___IOR(__EXAGEAR32_IOCTL_BASE, 0xa0, struct exagear32_abi_version)

/*
 * Sets the given fields in the kernel memory map for the current process.
 *
 * This is similar to prctl_mm_map but with 2 differences:
 * - exe_fd is removed.
 * - all fields are u64.
 */
struct exagear32_mm {
	__u64 start_code;
	__u64 end_code;
	__u64 start_data;
	__u64 end_data;
	__u64 start_brk;
	__u64 brk;
	__u64 start_stack;
	__u64 arg_start;
	__u64 arg_end;
	__u64 env_start;
	__u64 env_end;
	__u64 auxv;
	__u64 auxv_size;
};
#define __EXAGEAR32_SET_MM					\
	___IOW(__EXAGEAR32_IOCTL_BASE, 0xa1, struct exagear32_mm)

#define __EXAGEAR32_SET_MMAP_BASE				\
	___IO(__EXAGEAR32_IOCTL_BASE, 0xa2)

/*
 * Executes a compat (32-bit) ioctl on the given file descriptor.
 */
struct exagear32_compat_ioctl {
	__u32 fd;
	__u32 cmd;
	__u32 arg;
};
#define __EXAGEAR32_COMPAT_IOCTL					\
	___IOW(__EXAGEAR32_IOCTL_BASE, 0xa3, struct exagear32_compat_ioctl)

/*
 * Executes a compat (32-bit) set_robust_list on the current thread.
 */
struct exagear32_compat_robust_list {
	__u32 head;
	__u32 len;
};
#define __EXAGEAR32_COMPAT_SET_ROBUST_LIST					\
	___IOW(__EXAGEAR32_IOCTL_BASE, 0xa4, struct exagear32_compat_robust_list)
#define __EXAGEAR32_COMPAT_GET_ROBUST_LIST					\
	___IOR(__EXAGEAR32_IOCTL_BASE, 0xa5, struct exagear32_compat_robust_list)

/*
 * Executes a compat (32-bit) getdents64 syscall.
 *
 * This has special semantics on ext4 since it returns directory offsets with a
 * different encoding that fits in 32 bits.
 */
struct exagear32_compat_getdents64 {
	__u32 fd;
	__u32 count;
	__u64 dirp;
};
#define __EXAGEAR32_COMPAT_GETDENTS64						\
	___IOW(__EXAGEAR32_IOCTL_BASE, 0xa6, struct exagear32_compat_getdents64)

/*
 * Executes a compat (32-bit) lseek syscall.
 *
 * This has special semantics on ext4 since it operates on directory offsets
 * with a different encoding that fits in 32 bits.
 */
struct exagear32_compat_lseek {
	__u32 fd;
	__u32 whence;
	__u64 offset;
	__u64 result;
};
#define __EXAGEAR32_COMPAT_LSEEK						\
	___IOWR(__EXAGEAR32_IOCTL_BASE, 0xa7, struct exagear32_compat_lseek)

#endif