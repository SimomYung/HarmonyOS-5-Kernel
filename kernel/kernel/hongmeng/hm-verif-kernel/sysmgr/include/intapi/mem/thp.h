/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 *
 * Description: Transparent Huge Page
 * Author: Zang Leigang <zangleigang@hisilicon.com>
 * Create: Tue May 14 05:01:34 PM CST 2024
 */

#ifndef SYSMGR_INTAPI_MEM_THP_H
#define SYSMGR_INTAPI_MEM_THP_H

#ifdef CONFIG_THP
void thpd_vspace_enable(struct thread_s *thread);
void thpd_vspace_exit(struct vspace_s *vspace);
int thp_init_sysfs(void);
#endif

#endif /* SYSMGR_INTAPI_MEM_THP_H */
