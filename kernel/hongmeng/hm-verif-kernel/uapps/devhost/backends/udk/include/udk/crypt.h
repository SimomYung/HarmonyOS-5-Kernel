/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Crypt interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Wen Jul 21 08:57:02 2021
 */
#ifndef __UDK_CRYPT_H__
#define __UDK_CRYPT_H__

int udk_get_random_bytes(void *buf, unsigned int nbytes);

#endif /* __UDK_CRYPT_H__ */
