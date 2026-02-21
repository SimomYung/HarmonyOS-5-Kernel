/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define udk kvic headfile
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 16:28:40 2019
 */

#ifndef __UDK_KVIC_H__
#define __UDK_KVIC_H__

#define UDK_KVIC_IRQ_MAX		16

static inline int is_kvic_irq(unsigned int irq)
{
	return (irq < UDK_KVIC_IRQ_MAX) ? 1 : 0;
}

int udk_kvic_prepare_irq(unsigned int kvic_irq_type,
			 void *arg, unsigned int arg_len);

unsigned int udk_kvic_create_mapping(unsigned int type);

#endif /* __DEVHOST_UDK_KVIC_H__ */
