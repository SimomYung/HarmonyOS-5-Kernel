/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: API for udk extension
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 28 22:24:01 2023
 */

#ifndef __UDK_EXTENSION_H__
#define __UDK_EXTENSION_H__

/*
 * extension prototypes
 *
 * @return 0 if success, POSIX errno if fail
 */
int dh_udk_extension_init(void);

#define DH_UDK_EXTRALIB_INIT "dh_udk_extension_init"

typedef int (*dh_udk_extension_init_fn)(void);

#endif /* __UDK_EXTENSION_H__ */
