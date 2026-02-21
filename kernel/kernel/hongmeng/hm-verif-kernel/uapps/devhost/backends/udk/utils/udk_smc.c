/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of smc in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <udk/smc.h>

#include <stdlib.h>
#include <libhwsecurec/securec.h>

#define UDK_SMC_RESULT_SIZE 8

struct udk_smc_result {
	unsigned long arg[UDK_SMC_RESULT_SIZE];
};

struct udk_smc_result *udk_smc_result_alloc(void)
{
	struct udk_smc_result *ptr = NULL;
	ptr = malloc(sizeof(struct udk_smc_result));
	if (ptr == NULL) {
		return NULL;
	}
	mem_zero_s(*ptr);
	return ptr;
}

void udk_smc_result_destroy(struct udk_smc_result *result)
{
	if (result == NULL) {
		return;
	}
	free(result);
	return;
}

int udk_smc_result_read(const struct udk_smc_result *result, unsigned int idx, unsigned long *out)
{
	if (result == NULL || idx >= UDK_SMC_RESULT_SIZE || out == NULL) {
		return -EINVAL;
	}
	*out = result->arg[idx];
	return 0;
}

#if defined(__aarch64__)
#  define REG_NAME(x)	"x"#x
#elif defined(__arm__)
#  define REG_NAME(x)	"r"#x
#else
#  error "unsupported platform"
#endif

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"
int udk_smc_call(unsigned long func_id, unsigned long arg0, unsigned long arg1, unsigned long arg2,
		 unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6,
		 struct udk_smc_result *result)
{
	register unsigned long _func_id asm(REG_NAME(0)) = func_id;
	register unsigned long _arg0 asm(REG_NAME(1)) = arg0;
	register unsigned long _arg1 asm(REG_NAME(2)) = arg1;
	register unsigned long _arg2 asm(REG_NAME(3)) = arg2;
	register unsigned long _arg3 asm(REG_NAME(4)) = arg3;
	register unsigned long _arg4 asm(REG_NAME(5)) = arg4;
	register unsigned long _arg5 asm(REG_NAME(6)) = arg5;
	register unsigned long _arg6 asm(REG_NAME(7)) = arg6;

	/* 0x4200 is the syscall number for modif_call */
	asm volatile(
		__asmeq("%0", REG_NAME(0))
		__asmeq("%1", REG_NAME(1))
		__asmeq("%2", REG_NAME(2))
		__asmeq("%3", REG_NAME(3))
		__asmeq("%4", REG_NAME(4))
		__asmeq("%5", REG_NAME(5))
		__asmeq("%6", REG_NAME(6))
		__asmeq("%7", REG_NAME(7))
#ifdef UDK_FUSION
		"smc #0\n"
#else
		"svc    #0x4200\n"
#endif
	: "+r" (_func_id), "+r" (_arg0), "+r" (_arg1), "+r" (_arg2)
	: "r"(_arg3), "r" (_arg4), "r" (_arg5), "r" (_arg6));

	/* the register 0, 1, 2, 3 are the results of smc calls */
	if (result != NULL) {
		result->arg[0] = _func_id;
		result->arg[1] = _arg0;
		result->arg[2] = _arg1;
		result->arg[3] = _arg2;
	}

	/*
	 * SMCCC requires smc call to return negative value for errors.
	 * In the case of no bound atfd module, kernel returns E_HM_NOENT,
	 * which is a negative value, so we are safe here.
	 */
	return (int)(unsigned int)_func_id;
}

int udk_smc_call_ng(unsigned long func_id, unsigned long arg0, unsigned long arg1, unsigned long arg2,
		 unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6,
		 struct udk_smc_result *result)
{
	register unsigned long _func_id asm(REG_NAME(0)) = func_id;
	register unsigned long _arg0 asm(REG_NAME(1)) = arg0;
	register unsigned long _arg1 asm(REG_NAME(2)) = arg1;
	register unsigned long _arg2 asm(REG_NAME(3)) = arg2;
	register unsigned long _arg3 asm(REG_NAME(4)) = arg3;
	register unsigned long _arg4 asm(REG_NAME(5)) = arg4;
	register unsigned long _arg5 asm(REG_NAME(6)) = arg5;
	register unsigned long _arg6 asm(REG_NAME(7)) = arg6;

	/* 0x4200 is the syscall number for modif_call */
	asm volatile(
		__asmeq("%0", REG_NAME(0))
		__asmeq("%1", REG_NAME(1))
		__asmeq("%2", REG_NAME(2))
		__asmeq("%3", REG_NAME(3))
		__asmeq("%4", REG_NAME(4))
		__asmeq("%5", REG_NAME(5))
		__asmeq("%6", REG_NAME(6))
		__asmeq("%7", REG_NAME(7))
#ifdef UDK_FUSION
		"smc #0\n"
#else
		"svc    #0x4200\n"
#endif
	: "+r" (_func_id), "+r" (_arg0), "+r" (_arg1), "+r" (_arg2)
	: "r"(_arg3), "r" (_arg4), "r" (_arg5), "r" (_arg6));

	/* the register 0, 1, 2, 3 are the results of smc calls */
	if (result != NULL) {
		result->arg[0] = _func_id;
		result->arg[1] = _arg0;
		result->arg[2] = _arg1;
		result->arg[3] = _arg2;
		result->arg[4] = _arg3;
		result->arg[5] = _arg4;
		result->arg[6] = _arg5;
		result->arg[7] = _arg6;
	}

	/*
	 * SMCCC requires smc call to return negative value for errors.
	 * In the case of no bound atfd module, kernel returns E_HM_NOENT,
	 * which is a negative value, so we are safe here.
	 */
	return (int)(unsigned int)_func_id;
}
