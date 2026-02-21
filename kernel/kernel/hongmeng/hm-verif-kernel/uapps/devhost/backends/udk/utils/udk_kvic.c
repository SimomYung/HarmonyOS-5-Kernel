/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Function of udk kvic
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 15:14:11 2019
 */
#include <udk/kvic.h>

#include <devhost/interrupt.h>
#include <internal/init.h>
#include <libhwsecurec/securec.h>

#define UDK_KVIC_ID_INVALID (-1)

static int g_kvic_id = UDK_KVIC_ID_INVALID;

int udk_kvic_prepare_irq(unsigned int kvic_irq_type,
			 void *arg, unsigned int arg_len)
{
	return devhost_kvic_prepare_irq(kvic_irq_type, arg, arg_len);
}

/* reference to devhost_kvic_create_mapping */
unsigned int udk_kvic_create_mapping(unsigned int type)
{
	return devhost_create_irq_mapping((unsigned int)g_kvic_id, &type, 1U);
}

/* init g_kvic_id at the beginning */
static int udk_kvic_init(void)
{
	g_kvic_id = devhost_kvic_default_ctrl();
	if (g_kvic_id < 0) {
		return g_kvic_id;
	}
	return 0;
}
udk_init_call(udk_kvic_init);
