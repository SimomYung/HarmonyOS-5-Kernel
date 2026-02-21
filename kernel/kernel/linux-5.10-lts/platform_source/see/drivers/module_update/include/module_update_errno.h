/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: header file to provide error number
 * Create: 2025/01/18
 */

#ifndef __MODULE_UPDATE_ERRNO_H__
#define __MODULE_UPDATE_ERRNO_H__

#define MODULE_UPDATE_OK                         0x0
#define MODULE_UPDATE_PTR_NULL                   0x1
#define MODULE_UPDATE_IO_SIZE_NOT_MATCH          0x2
#define MODULE_UPDATE_COPY_FROM_USER_FAIL        0x3
#define MODULE_UPDATE_DATA_SIZE_EXCEED           0x4
#define MODULE_UPDATE_WORK_CTX_INIT_FAIL         0x5

#define MODULE_UPDATE_ERR_NULL_POINTER           0x8ff70003
#define MODULE_UPDATE_ERR_TEEK_INIT_CTX          0x8ff70030
#define MODULE_UPDATE_ERR_TEEK_OPEN_SESSION      0x8ff70031

#endif /* __MODULE_UPDATE_ERRNO_H__ */