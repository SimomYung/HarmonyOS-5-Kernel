/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lsyscall set callee_args operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 21 15:57:35 2023
 */

#ifndef ASM_LSYSCALL_ARGS_H
#define ASM_LSYSCALL_ARGS_H

/* calculate by __arch_actv_rpc_stksz_init/append/done */
/* time related lsyscall */
#define STK_BUF_NR_CLOCK_GETTIME 0u
#define STK_BUF_NR_CLOCK_GETTIME_COMPAT 1u

/* timerfd related lsyscall */
#define STK_BUF_NR_TIMERFD_CREATE 0u
#define STK_BUF_NR_TIMERFD_CREATE_COMPAT 1u
int arch_timerfd_create_native_xfer_hook(bool fastpath, struct actv_s *caller,
					  struct actv_s *callee);
#define STK_BUF_NR_TIMERFD_SETTIME 0u
#define STK_BUF_NR_TIMERFD_SETTIME_COMPAT 5u
int arch_timerfd_settime_native_xfer_hook(bool fastpath, struct actv_s *caller,
					  struct actv_s *callee);
#define STK_BUF_NR_TIMERFD_GETTIME 0u
#define STK_BUF_NR_TIMERFD_GETTIME_COMPAT 3u
int arch_timerfd_gettime_native_xfer_hook(bool fastpath, struct actv_s *caller,
					  struct actv_s *callee);

#define STK_BUF_NR_FSYNC 0u
#define STK_BUF_NR_FSYNC_COMPAT 2u
int arch_fsync_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

#define STK_BUF_NR_SYNCFS 0u
#define STK_BUF_NR_SYNCFS_COMPAT 2u
int arch_syncfs_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

#define STK_BUF_NR_FDATASYNC 0u
#define STK_BUF_NR_FDATASYNC_COMPAT 2u
int arch_fdatasync_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_PREAD64 0u
#define STK_BUF_NR_PREAD64_COMPAT 7u
int arch_pread64_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

void arch_pread64_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_PWRITE64 0u
#define STK_BUF_NR_PWRITE64_COMPAT 7u
int arch_pwrite64_native_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);

void arch_pwrite64_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_PREADV 0u
#define STK_BUF_NR_PREADV_COMPAT 6u
int arch_preadv_native_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);

void arch_preadv_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_PREADV2 0u
#define STK_BUF_NR_PREADV2_COMPAT 7u
int arch_preadv2_native_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);

void arch_preadv2_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_PWRITEV 0u
#define STK_BUF_NR_PWRITEV_COMPAT 6u
int arch_pwritev_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

void arch_pwritev_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_PWRITEV2 0u
#define STK_BUF_NR_PWRITEV2_COMPAT 7u
int arch_pwritev2_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

void arch_pwritev2_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_READV 0u
#define STK_BUF_NR_READV_COMPAT 4u
int arch_readv_native_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);

void arch_readv_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_LSEEK 0u
#define STK_BUF_NR_LSEEK_COMPAT 4u
int arch_lseek_native_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);

void arch_lseek_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_READ 0u
#define STK_BUF_NR_READ_COMPAT 4u
int arch_read_native_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);

void arch_read_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_WRITE 0u
#define STK_BUF_NR_WRITE_COMPAT 4u
int arch_write_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

void arch_write_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_DUP 0u
#define STK_BUF_NR_DUP_COMPAT 3u
int arch_dup_native_xfer_hook(bool fastpath, struct actv_s *caller,
			      struct actv_s *callee);

void arch_dup_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_DUP3 0u
#define STK_BUF_NR_DUP3_COMPAT 5u
int arch_dup3_native_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);

void arch_dup3_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_FCNTL 0u
#define STK_BUF_NR_FCNTL_COMPAT 5u
int arch_fcntl_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

void arch_fcntl_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_GETSOCKNAME 0u
#define STK_BUF_NR_GETSOCKNAME_COMPAT 4u
int arch_getsockname_native_xfer_hook(bool fastpath, struct actv_s *caller,
				      struct actv_s *callee);

#define STK_BUF_NR_GETPEERNAME 0u
#define STK_BUF_NR_GETPEERNAME_COMPAT 4u
int arch_getpeername_native_xfer_hook(bool fastpath, struct actv_s *caller,
				      struct actv_s *callee);

#define STK_BUF_NR_WRITEV 0u
#define STK_BUF_NR_WRITEV_COMPAT 4u
int arch_writev_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

void arch_writev_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_CLOSE 0u
#define STK_BUF_NR_CLOSE_COMPAT 3u
int arch_close_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
void arch_close_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_GETSOCKOPT 0u
#define STK_BUF_NR_GETSOCKOPT_COMPAT 6u
int arch_getsockopt_native_xfer_hook(bool fastpath, struct actv_s *caller,
				     struct actv_s *callee);

#define STK_BUF_NR_LISTEN 0u
#define STK_BUF_NR_LISTEN_COMPAT 3u
int arch_listen_native_xfer_hook(bool fastpath, struct actv_s *caller,
				      struct actv_s *callee);

#define STK_BUF_NR_BIND 0u
#define STK_BUF_NR_BIND_COMPAT 4u
int arch_bind_native_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);

#define STK_BUF_NR_CONNECT 0u
#define STK_BUF_NR_CONNECT_COMPAT 4u
int arch_connect_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_SETSOCKOPT 0u
#define STK_BUF_NR_SETSOCKOPT_COMPAT 6u
int arch_setsockopt_native_xfer_hook(bool fastpath, struct actv_s *caller,
				     struct actv_s *callee);

#define STK_BUF_NR_SHUTDOWN 0u
#define STK_BUF_NR_SHUTDOWN_COMPAT 3u
int arch_shutdown_native_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);

#define STK_BUF_NR_FCHOWN 0u
#define STK_BUF_NR_FCHOWN_COMPAT 4u
int arch_fchown_native_xfer_hook(bool fastpath, struct actv_s *caller,
							     struct actv_s *callee);
void arch_fchown_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_IOCTL 0u
#define STK_BUF_NR_IOCTL_COMPAT 4u
int arch_ioctl_native_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);

void arch_ioctl_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_EPOLL_CTL		0u
#define STK_BUF_NR_EPOLL_CTL_COMPAT	8u
int arch_epoll_ctl_native_xfer_hook(struct actv_s *caller, struct actv_s *callee, bool fastpath);

#define STK_BUF_NR_FSTAT 0u
#define STK_BUF_NR_FSTAT_COMPAT 3u
int arch_fstat_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

void arch_fstat_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_INOTIFY_ADD_WATCH 0u
#define STK_BUF_NR_INOTIFY_ADD_WATCH_COMPAT 4u
int arch_inotify_add_watch_native_xfer_hook(bool fastpath, struct actv_s *caller,
					    struct actv_s *callee);

#define STK_BUF_NR_INOTIFY_RM_WATCH 0u
#define STK_BUF_NR_INOTIFY_RM_WATCH_COMPAT 3u
int arch_inotify_rm_watch_native_xfer_hook(bool fastpath, struct actv_s *caller,
					   struct actv_s *callee);

#define STK_BUF_NR_FCHMOD 0u
#define STK_BUF_NR_FCHMOD_COMPAT 3u
int arch_fchmod_native_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);

#define STK_BUF_NR_FLOCK 0u
#define STK_BUF_NR_FLOCK_COMPAT 3u
int arch_flock_native_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);

#define STK_BUF_NR_FCHDIR 0u
#define STK_BUF_NR_FCHDIR_COMPAT 2u
int arch_fchdir_native_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);
#define STK_BUF_NR_RECVFROM 0u
#define STK_BUF_NR_RECVFROM_COMPAT 7u
int arch_recvfrom_native_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);

#define STK_BUF_NR_SENDMMSG 0u
#define STK_BUF_NR_SENDMMSG_COMPAT 5u
int arch_sendmmsg_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
void arch_sendmmsg_native_file_replace_hook(struct actv_s *caller);
#define STK_BUF_NR_SENDTO 0u
#define STK_BUF_NR_SENDTO_COMPAT 7u
int arch_sendto_native_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);
#define STK_BUF_NR_SENDMSG 0u
#define STK_BUF_NR_SENDMSG_COMPAT 4u
int arch_sendmsg_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_RECVMSG 0u
#define STK_BUF_NR_RECVMSG_COMPAT 4u
int arch_recvmsg_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_RECVMMSG 0u
#define STK_BUF_NR_RECVMMSG_COMPAT 6u
int arch_recvmmsg_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
void arch_recvmmsg_native_file_replace_hook(struct actv_s *caller);

#define STK_BUF_NR_ACCEPT 0u
#define STK_BUF_NR_ACCEPT_COMPAT 4u
int arch_accept_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_ACCEPT4 0u
#define STK_BUF_NR_ACCEPT4_COMPAT 5u
int arch_accept4_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_SOCKET 0u
#define STK_BUF_NR_SOCKET_COMPAT 2u
int arch_socket_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_SOCKETPAIR 0u
#define STK_BUF_NR_SOCKETPAIR_COMPAT 3u
int arch_socketpair_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_FGETXATTR 0u
#define STK_BUF_NR_FGETXATTR_COMPAT 5u
int arch_fgetxattr_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_FSETXATTR 0u
#define STK_BUF_NR_FSETXATTR_COMPAT 6u
int arch_fsetxattr_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_FLISTXATTR 0u
#define STK_BUF_NR_FLISTXATTR_COMPAT 4u
int arch_flistxattr_native_xfer_hook(bool fastpath, struct actv_s *caller,
				     struct actv_s *callee);

#define STK_BUF_NR_FREMOVEXATTR 0u
#define STK_BUF_NR_FREMOVEXATTR_COMPAT 3u
int arch_fremovexattr_native_xfer_hook(bool fastpath, struct actv_s *caller,
				       struct actv_s *callee);

#define STK_BUF_NR_READAHEAD 0u
#define STK_BUF_NR_READAHEAD_COMPAT 6u
int arch_readahead_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_FTRUNCATE 0u
#define STK_BUF_NR_FTRUNCATE_COMPAT 5u
int arch_ftruncate_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_FADVISE 0u
#define STK_BUF_NR_FADVISE_COMPAT 8u
int arch_fadvise_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_FALLOCATE 0u
#define STK_BUF_NR_FALLOCATE_COMPAT 7u
int arch_fallocate_native_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);

#define STK_BUF_NR_IO_URING_ENTER 0u
#define STK_BUF_NR_IO_URING_ENTER_COMPAT 7u
int arch_io_uring_enter_native_xfer_hook(bool fastpath, struct actv_s *caller,
					 struct actv_s *callee);

#define STK_BUF_NR_IO_URING_REGISTER 0u
#define STK_BUF_NR_IO_URING_REGISTER_COMPAT 5u
int arch_io_uring_register_native_xfer_hook(bool fastpath, struct actv_s *caller,
					    struct actv_s *callee);

#define STK_BUF_NR_FUTEX 0u
#define STK_BUF_NR_FUTEX_COMPAT 5u

#define STK_BUF_NR_PRCTL 0u
#define STK_BUF_NR_PRCTL_COMPAT 4u

#define STK_BUF_NR_BPF 0u
#define STK_BUF_NR_BPF_COMPAT 2u
int arch_bpf_native_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

#define STK_BUF_NR_OPENAT 0u
#define STK_BUF_NR_OPENAT_COMPAT 4u
int arch_openat_native_xfer_hook(bool fastpath, struct actv_s *caller, struct actv_s *callee);

#define STK_BUF_NR_PIDFD_SEND_SIGNAL 0u
#define STK_BUF_NR_PIDFD_SEND_SIGNAL_COMPAT 5u
int arch_pidfd_send_signal_native_xfer_hook(bool fastpath,
					    struct actv_s *caller,
					    struct actv_s *callee);

#define STK_BUF_NR_RT_SIGSUSPEND 0u
#define STK_BUF_NR_RT_SIGSUSPEND_COMPAT 1u

#define STK_BUF_NR_RT_SIGTIMEDWAIT 0u
#define STK_BUF_NR_RT_SIGTIMEDWAIT_COMPAT 3u

#define STK_BUF_NR_EXECVE 0u
#define STK_BUF_NR_EXECVE_COMPAT 2u

#define STK_BUF_NR_NANOSLEEP 0u
#define STK_BUF_NR_NANOSLEEP_COMPAT 1u

#define STK_BUF_NR_EPOLL_PWAIT 0u
#define STK_BUF_NR_EPOLL_PWAIT_COMPAT 5u

#define STK_BUF_NR_CLONE 0u
#define STK_BUF_NR_CLONE_COMPAT 4u

#define STK_BUF_NR_MUNMAP 0u
#define STK_BUF_NR_MUNMAP_COMPAT 1u

#ifdef CONFIG_COMPAT
/* timerfd related lsyscall */
int arch_timerfd_create_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					 struct actv_s *callee);
int arch_timerfd_settime_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					  struct actv_s *callee);
int arch_timerfd_gettime_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					  struct actv_s *callee);

int arch_fsync_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_syncfs_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_fdatasync_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);
int arch_dup_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			      struct actv_s *callee);
#define STK_BUF_NR_DUP2_COMPAT 4u
int arch_dup2_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);
int arch_dup3_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);
int arch_fcntl_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_pread64_compat_xfer_hook(bool fastpath,struct actv_s *caller,
				  struct actv_s *callee);
int arch_pwrite64_compat_xfer_hook(bool fastpath,struct actv_s *caller,
				   struct actv_s *callee);
int arch_preadv_compat_xfer_hook(bool fastpath,struct actv_s *caller,
				 struct actv_s *callee);
int arch_preadv2_compat_xfer_hook(bool fastpath,struct actv_s *caller,
				 struct actv_s *callee);
int arch_pwritev_compat_xfer_hook(bool fastpath,struct actv_s *caller,
				  struct actv_s *callee);
int arch_pwritev2_compat_xfer_hook(bool fastpath,struct actv_s *caller,
				  struct actv_s *callee);
int arch_readv_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_lseek_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_read_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);
int arch_write_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_ioctl_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_getsockname_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				      struct actv_s *callee);
int arch_getpeername_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				      struct actv_s *callee);
#define STK_BUF_NR_RECV_COMPAT 5u
int arch_recv_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);
int arch_recvfrom_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);

#define STK_BUF_NR_SEND_COMPAT 5u
int arch_send_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);

int arch_sendmmsg_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_sendto_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);
int arch_sendmsg_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_recvmsg_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_recvmmsg_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_accept_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_accept4_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_socket_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_socketpair_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);

int arch_writev_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);
int arch_close_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);
int arch_getsockopt_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				     struct actv_s *callee);
int arch_listen_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				      struct actv_s *callee);
int arch_bind_compat_xfer_hook(bool fastpath, struct actv_s *caller,
			       struct actv_s *callee);
int arch_connect_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_setsockopt_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				     struct actv_s *callee);
int arch_shutdown_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);
int arch_fchown_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				   struct actv_s *callee);
int arch_epoll_ctl_compat_xfer_hook(struct actv_s *caller, struct actv_s *callee, bool fastpath);
int arch_fstat_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_inotify_add_watch_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					    struct actv_s *callee);
int arch_inotify_rm_watch_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					   struct actv_s *callee);
int arch_fchmod_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);
int arch_flock_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				struct actv_s *callee);
int arch_fchdir_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				 struct actv_s *callee);
int arch_fgetxattr_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);
int arch_fsetxattr_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);
int arch_flistxattr_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				     struct actv_s *callee);
int arch_fremovexattr_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				       struct actv_s *callee);
int arch_readahead_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);
int arch_ftruncate_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				    struct actv_s *callee);
int arch_fadvise_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_fallocate_compat_xfer_hook(bool fastpath, struct actv_s *caller,
				  struct actv_s *callee);
int arch_io_uring_enter_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					 struct actv_s *callee);
int arch_io_uring_register_compat_xfer_hook(bool fastpath, struct actv_s *caller,
					    struct actv_s *callee);
int arch_bpf_compat_xfer_hook(bool fastpath, struct actv_s *caller, struct actv_s *callee);
int arch_openat_compat_xfer_hook(bool fastpath, struct actv_s *caller, struct actv_s *callee);
int arch_pidfd_send_signal_compat_xfer_hook(bool fastpath, struct actv_s *caller, struct actv_s *callee);
#endif /* CONFIG_COMPAT */

static inline unsigned long
arch_stk_buf_size_of(bool is_compat, unsigned long size,
		     unsigned long compat_size)
{
	unsigned long stk_buf_size = 0UL;
#ifdef CONFIG_COMPAT
	if (is_compat) {
		stk_buf_size = __RPC_INFO_ENCODE_STK_BUF_NR_UL(compat_size);
	} else {
		stk_buf_size = __RPC_INFO_ENCODE_STK_BUF_NR_UL(size);
	}
#else
	UNUSED(is_compat, compat_size);
	stk_buf_size = __RPC_INFO_ENCODE_STK_BUF_NR_UL(size);
#endif
	return stk_buf_size;
}

#endif /* ASM_LSYSCALL_ARGS_H */
