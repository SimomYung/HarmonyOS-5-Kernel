/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Captype fdtable
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 10 15:41:35 2023
 */

/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(FDTable, 21, CAPALLGRANT(FDTable)
		     CAPNOGRANTMOVE(FDTable)
		     CAPRAWCREFRLOOKUP(FDTable)
		     CAPDEFAULTSIZE(FDTable),
		     CAPMETHOD(FDTable, WakeupPoll)
		     CAPMETHOD(FDTable, FetchSelect)
		     CAPMETHOD(FDTable, AllocFd)
		     CAPMETHOD(FDTable, InstallFd)
		     CAPMETHOD(FDTable, CloseFd)
		     CAPMETHOD(FDTable, DupFd)
		     CAPMETHOD(FDTable, FreeFd)
		     CAPMETHOD(FDTable, AllocAndInstallFd)
		     CAPMETHOD(FDTable, GetFd)
		     CAPMETHOD(FDTable, GetCloseOnExec)
		     CAPMETHOD(FDTable, SetClientData)
		     CAPMETHOD(FDTable, GetFdType)
		     CAPMETHOD(FDTable, GetFdNum)
		     CAPMETHOD(FDTable, SetRlimit)
		     CAPMETHOD(FDTable, GatherCommonFiles)
		     CAPMETHOD(FDTable, CloneFDTable)
		     CAPMETHOD(FDTable, FdoSetupCofile)
		     CAPMETHOD(FDTable, SetCloseAndGetRpccnt)
		     CAPMETHOD(FDTable, CleanupPolllist)
	)

#ifdef CAP_EXPORT_API

#include <hmasm/types.h>

#define RPCCNT_OVERFLOW_FLAG			0x01
#define MAX_FD_RPCCNT				8192
#define set_common_file_overflow(file)		((file) | RPCCNT_OVERFLOW_FLAG)
#define clear_common_file_overflow(file)	((file) & (~(uintptr_t)RPCCNT_OVERFLOW_FLAG))
#define is_common_file_overflow(file)		(((file) & RPCCNT_OVERFLOW_FLAG) == RPCCNT_OVERFLOW_FLAG)

#define GATHER_CLOEXEC_FILES		0x01
#define GATHER_FILES_AND_RPCCNT		0x02
#define GATHER_ALL_FILES		0x04
#define GATHER_INSTALLED_FILES		0x08

enum {
	CRIGHT_FDTABLE_APP_BIT,
	CRIGHT_FDTABLE_SERVER_BIT,
	CRIGHT_FDTABLE_OWNER_BIT,
	CRIGHT_FDTABLE_MAX
};

enum {
	FD_ALLOCED,
	FD_INSTALLED,
	FD_CLOSING,
};

enum fd_type {
	FD_FS = 0,
	FD_NET,
	FD_KERNEL,
	/* sockfs mainly refer to unix socket now */
	FD_SOCKFS,
	FD_MIXED_FS_NET,
};

struct sysarg_fdtable_fd_config {
	__u32 start;
	__s32 remote_fd;
	__u32 fd_type;
	__u32 flags;
};

struct sysarg_fdtable_fd_install {
	__u32 start;
	__u32 fd;
	__s32 remote_fd;
	__u32 vfs_id;
	__u32 flags;
	__u32 fd_type;
	__u32 server;
	void *common_file;
	void *priv; /* used to set devhost file currently */
	void *file_in_kobj; /* use to set vfs file to ctrlmem kobj */
};

struct sysarg_fdtable_fd_dup_req {
	__u32 old_fd;
	__u32 fd_type;
	__u32 new_fd;
	__u32 start;
	__u32 flags;
};

struct sysarg_fdtable_fd_dup_res {
	__u32 new_fd;
};

struct sysarg_fdtable_fd_stat {
	__u32 fd;
	__s32 remote_fd;
	__u32 fd_type;
	__u32 server;
	__u32 vfs_id;
	__u32 flags;
	/* use this to store vfs_client_data */
	void *cred;
	/* use this to store file/socket */
	void *common_file;
	void *priv;
	__u64 pfn; /* used to reclaim page */
};

struct sysarg_fdtable_fd_cofile {
	__u32 fd;
	__u32 fd_type;
	void *file;
	void  *cofile; /* struct ctrlmem_kobj_cofile_s */
};

struct sysarg_fdtable_close_on_exec {
	__s32 remote_fd;
	__u32 rpccnt;
	__u64 common_file;
	__u32 fd_type;
};

struct sysarg_fdtable_rpccnt_file {
	__u32 rpccnt;
	__u32 fd_type;
	__u64 common_file;
};

struct sysarg_fdtable_all_file {
	__s32 fd;
	__s32 remote_fd;
	__u64 common_file;
	__u32 fd_type;
};

struct sysarg_fdtable_installed_file {
	__s32 fd;
	__s32 remote_fd;
	__u64 common_file;
	__u32 fd_type;
};

/*
 * Some common system calls(i.e. read/write) share the same sysgmr actvpool
 * for sockfs and fs type, so add the flag to distinguish it to route the needed path
 */
#define FD_SET_TYPE(file, type)		((unsigned long)(long)(file) | (unsigned long)(unsigned)type)
#define FD_TYPE_OF(file)	((unsigned long)(long)(file) & 0x3UL)
#define FD_FILE_OF(file)		((unsigned long)(long)(file) & ~0x3UL)

/*
 * act_cap_fdtsize = (FD_LIMIT / (MAX_FDTPAGES_PER_PAGE * MAX_FDS_PER_PAGE)) * sizeof(struct fddpage_s *) +
 *                    sizeof(struct fdtable_metadata_s) + sizeof(struct capability_s *)
 * __CAP_FDTABLE_OBJ_SIZE is act_cap_fdtsize aligning integer powers of 2
 */
#define __CAP_FDTABLE_OBJ_SIZE	512u

#define CRIGHT_FDTABLE_APP	(1U << CRIGHT_FDTABLE_APP_BIT)
#define CRIGHT_FDTABLE_SERVER	(1U << CRIGHT_FDTABLE_SERVER_BIT)
#define CRIGHT_FDTABLE_OWNER	(1U << CRIGHT_FDTABLE_OWNER_BIT)

enum {
	FDTABLE_FD_CLOEXEC_BIT = 0,
	FDTABLE_FD_TRACE_MAKER_BIT,
	FDTABLE_FD_EPOLL_BIT,
	FDTABLE_FD_BINDER_BIT,
	FDTABLE_FD_DEVHOST_BIT,
	FDTABLE_FD_DEVHOST_REVISE_BIT,
	FDTABLE_FD_FAST_COPY_BIT,
	/* a file support poll */
	FDTABLE_FD_POLLABLE_BIT,
	FDTABLE_FD_RECVQUEUE_BIT,
	FDTABLE_FD_SENDQUEUE_BIT,
	/* a file always readable and writable for poll, e.g disk normal file */
	FDTABLE_FD_DEFAULT_POLL_MASK_BIT,

	/* correspond fde flags [0-31] */
	FDTABLE_FD_MAX_BIT = 32,
};

#define FDTABLE_FD_FLAG_CLOEXEC (1U << FDTABLE_FD_CLOEXEC_BIT)
#define FDTABLE_FD_FLAG_TRACEMAKER (1U << FDTABLE_FD_TRACE_MAKER_BIT)
#define FDTABLE_FD_FLAG_BINDER (1 << FDTABLE_FD_BINDER_BIT)
#define FDTABLE_FD_FLAG_EPOLL (1 << FDTABLE_FD_EPOLL_BIT)
#define FDTABLE_FD_FLAG_DEVHOST (1 << FDTABLE_FD_DEVHOST_BIT)
#define FDTABLE_FD_FLAG_DEVHOST_REVISE (1 << FDTABLE_FD_DEVHOST_REVISE_BIT)
#define FDTABLE_FD_FLAG_FAST_COPY (1U << FDTABLE_FD_FAST_COPY_BIT)
#define FDTABLE_FD_POLLABLE (1 << FDTABLE_FD_POLLABLE_BIT)
#define FDTABLE_FD_FLAG_RECVQUEUE (1 << FDTABLE_FD_RECVQUEUE_BIT)
#define FDTABLE_FD_FLAG_SENDQUEUE (1 << FDTABLE_FD_SENDQUEUE_BIT)
#define FDTABLE_FD_DEFAULT_POLL_MASK (1 << FDTABLE_FD_DEFAULT_POLL_MASK_BIT)
#define fdtable_is_close_on_exec(flags)	(((flags) & FDTABLE_FD_FLAG_CLOEXEC) == FDTABLE_FD_FLAG_CLOEXEC)
#define fdtable_is_epoll_fd(flags)	(((flags) & FDTABLE_FD_FLAG_EPOLL) == FDTABLE_FD_FLAG_EPOLL)
#define fdtable_is_devhost_fd(flags)	(((flags) & FDTABLE_FD_FLAG_DEVHOST) == FDTABLE_FD_FLAG_DEVHOST)
#define fdtable_is_binder_fd(flags)	(((flags) & FDTABLE_FD_FLAG_BINDER) == FDTABLE_FD_FLAG_BINDER)
#define fdtable_is_pollable_fd(flags)	(((flags) & FDTABLE_FD_POLLABLE) == FDTABLE_FD_POLLABLE)
#endif
