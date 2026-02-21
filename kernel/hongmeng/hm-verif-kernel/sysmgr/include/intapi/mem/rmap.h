/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Declare unmap interface for page
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 21 11:08:48 UTC 2023
 */

#ifndef __SYSMGR_INTAPI_UNMAP_H__
#define __SYSMGR_INTAPI_UNMAP_H__

int sysmgr_rmap_unmap_file_page(void *addr, uint32_t *flags);

#endif /* __SYSMGR_INTAPI_UNMAP_H__ */
