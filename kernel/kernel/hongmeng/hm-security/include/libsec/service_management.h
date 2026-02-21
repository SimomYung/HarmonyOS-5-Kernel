/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces of security service management
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 09:55:13 2023
 */
#ifndef ULIBS_LIBSEC_SERVICE_MANAGEMENT_H
#define ULIBS_LIBSEC_SERVICE_MANAGEMENT_H

#include <hongmeng/types.h>
#include <libhmactv/actv.h>
#include <libsec/module.h>

#define HM_SECS_SEL_LOAD_POLICY 1u
#define HM_SECS_SEL_POST_LOAD 2u

static inline
bool hm_secs_check_recv_data(const struct actvpool_recv *data, size_t size)
{
	return data->size >= size;
}

#define hm_secs_conv_recv_data(data_ptr, type) ((type *)(ptr_from_void((data_ptr)->data, type)))
#define hm_secs_recv_simple_data(data, type, error_handle) ((void *)({		\
		bool valid = hm_secs_check_recv_data((data), sizeof(type));	\
		if (!valid) {							\
			error_handle;						\
		}								\
		valid ? hm_secs_conv_recv_data((data), type) : NULL;		\
	}))

typedef int (*hm_secs_mgmt_func)(unsigned int command, const struct actvpool_recv *data, unsigned int service);

int hm_secs_mgmt_register(unsigned int module, hm_secs_mgmt_func func);

int hm_secs_mgmt(unsigned int module, unsigned int command,
		 const struct actvpool_recv *data, unsigned int service);

#define SEC_SERVICE_UNKNOWN (0x0)
#define SEC_SERVICE_SYSMGR (0x1)
#define SEC_SERVICE_VFS	(0x2)
#define SEC_SERVICE_NETWORK (0x4)
#define SEC_SERVICE_DEVMGR (0x8)
#define SEC_SERVICE_DEVHOST (0x10)
#define SEC_SERVICE_PWRMGR (0x20)
#define SEC_SERVICE_KEYMGR (0x40)
#define SEC_SERVICE_NUM (7u)

#endif
