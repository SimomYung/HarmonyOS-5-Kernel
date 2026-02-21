/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of libdh user copy
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 16 22:33:12 2020
 */
#include <libdevhost/uaccess.h>

#include <libhmactv/actv.h>

int libdh_copy_from_user(void *dst, const void __user *src, unsigned long n)
{
	return hm_actv_read_xact_vm(dst, src, n);
}

int libdh_copy_to_user(void __user *dst, const void *src, unsigned long n)
{
	return hm_actv_write_xact_vm(dst, src, n);
}

int libdh_copy_in_user(void __user *dst, const void __user *src, unsigned long n)
{
	return hm_actv_copy_in_xact_vm(dst, src, n);
}
