/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Crypto APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 04 10:26:09 2021
 */
#ifndef DEVHOST_PLUGIN_CRYPTO_H
#define DEVHOST_PLUGIN_CRYPTO_H

#include <hongmeng/errno.h>

#ifdef CONFIG_DEVHOST_ENTROPY_COLLECT
int hdr_devhost_core_entropy_read(unsigned long long sender,
				  unsigned long credential,
				  unsigned int num);
#else
static inline
int hdr_devhost_core_entropy_read(unsigned long long sender,
				  unsigned long credential,
				  unsigned int num)
{
	(void)sender;
	(void)credential;
	(void)num;
	return E_HM_NOSYS;
}
#endif /* CONFIG_DEVHOST_ENTROPY_COLLECT */

#endif /* DEVHOST_PLUGIN_CRYPTO_H */
