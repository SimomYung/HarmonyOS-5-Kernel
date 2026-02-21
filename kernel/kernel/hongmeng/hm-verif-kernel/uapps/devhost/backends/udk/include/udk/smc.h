/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Smc interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Wen Jul 21 09:04:51 2021
 */
#ifndef __UDK_SMC_H__
#define __UDK_SMC_H__

struct udk_smc_result;

struct udk_smc_result *udk_smc_result_alloc(void);
void udk_smc_result_destroy(struct udk_smc_result *result);
int udk_smc_result_read(const struct udk_smc_result *result, unsigned int idx, unsigned long *out);
int udk_smc_call(unsigned long func_id, unsigned long arg0, unsigned long arg1, unsigned long arg2,
		 unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6,
		 struct udk_smc_result *result);

int udk_smc_call_ng(unsigned long func_id, unsigned long arg0, unsigned long arg1, unsigned long arg2,
		 unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6,
		 struct udk_smc_result *result);
#endif /* __UDK_SMC_H__ */
