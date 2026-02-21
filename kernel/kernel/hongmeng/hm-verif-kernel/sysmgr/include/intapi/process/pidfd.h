/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Used to export pidfd functions to vfs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 22 20:13:22 2024
 */

#ifndef H_SYSMGR_INCLUDE_PIDFD_H
#define H_SYSMGR_INCLUDE_PIDFD_H

#include <sys/types.h>
#include <libhmsrv_sys/hm_signal.h>
#include <hmkernel/ctrlmem/kobjs/polllist.h>
#include <libhmsrv_sys/hm_ctrlmem_eventpoll.h>

int pidfd_pid_s_ptr_find_get(pid_t pid, unsigned int nsid, uintptr_t *pid_ptr);
void pidfd_pid_s_ptr_get(uintptr_t pid_ptr);
void pidfd_pid_s_ptr_put(uintptr_t pid_ptr);
int pidfd_send_signal(uintptr_t pid_ptr, int sig, siginfo_t *info);
int pidfd_register_polllist(uintptr_t pid_ptr, struct polllist_s *polllist, uint32_t *revents);
int pidfd_unregister_polllist(uintptr_t pid_ptr, struct polllist_s *polllist);
int pidfd_notify_all_polllist(uintptr_t pid_ptr);
int pidfd_fetch_pid_t(uintptr_t pid_ptr, pid_t *pid);
#endif
