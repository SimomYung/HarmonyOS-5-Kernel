/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of kstate_api
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 20:09:29 CST 2023
 */

#ifndef __DEVHOST_PLUGIN_KSTATE_API_H__
#define __DEVHOST_PLUGIN_KSTATE_API_H__

struct kstate_binder_info;
struct devhost_kstate_ops {
	int (*binderinfo)(int calling_pid, int called_pid);
	int (*killinfo)(int pid, int seg);
	int (*freeze_binder_info)(struct kstate_binder_info *info);
	int (*request_anco_thaw)(char *reason, unsigned int size);
};

typedef int (*kstate_info_func)(int pid, int pid_seg);
typedef int (*freeze_binder_info_func)(struct kstate_binder_info *info);
typedef int (*request_anco_thaw_func)(char *reason, unsigned int size);
void devhost_kstate_ops_register(kstate_info_func binder_info, kstate_info_func killinfo);
void devhost_freeze_binder_info_ops_register(
	freeze_binder_info_func freeze_binder_info);
void devhost_request_anco_thaw_ops_register(request_anco_thaw_func request_anco_thaw);

extern int hwbinderinfo(int callingpid, int calledpid);
extern int hwkillinfo(int pid, int seg);
extern int hw_freeze_binder_info(struct kstate_binder_info *info);
extern int hw_request_anco_thaw(char* reason, unsigned int size);

#endif /* __DEVHOST_PLUGIN_KSTATE_API_H__ */
