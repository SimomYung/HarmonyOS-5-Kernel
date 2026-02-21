/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Support HongMeng ctrlmem kobj APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 06 16:54:45 2023
 */
#ifndef DEVHOST_PLUGINS_UDMI_KOBJ_H
#define DEVHOST_PLUGINS_UDMI_KOBJ_H

#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

struct dh_ctrlmem_page_grant_info {
	uint64_t fsmgr_shm_key;
};

struct __ctrlmem_kobj_polllist_s *dh_ctrlmem_spawn_polllist(unsigned long file, __u32 listener_cnt,
				unsigned long *offset, struct dh_ctrlmem_page_grant_info *grant_info);

int dh_ctrlmem_kobj_destroy(void *kobj);
#endif /* DEVHOST_PLUGINS_UDMI_KOBJ_H */
