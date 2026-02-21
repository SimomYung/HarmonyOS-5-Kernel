/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Activation exception handler
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul  2 12:13:38 CST 2020
 */
#ifndef ARM_ULIBS_ASM_LIBSYSIF_UTILS_H
#define ARM_ULIBS_ASM_LIBSYSIF_UTILS_H

#include <stdint.h>
#include <hmasm/exception.h>

#ifndef __HOST_LLT__
/*
 * Define the common actv hdlr entry for the actv rpc started from kernel
 */
#define DECLARE_KCALL_ACTVHDLR_ENTRY(name, rettype)		\
rettype name(unsigned long long sender, unsigned long credential)

#define DEFINE_KCALL_ACTVHDLR_ENTRY(name, rettype)		\
static rettype __##name(unsigned long long sender,		\
			unsigned long credential,		\
			__u32 arg1,				\
			__u32 arg2,				\
			__u32 arg3);				\
rettype __opt_noframe name(unsigned long long sender,		\
			   unsigned long credential)		\
{								\
	/* r0, r1: sender; r2: credential; r4: rpc info */	\
	/* r3, r5, r6: hold the extra parameters */		\
	register __u32 arg1 asm ("r3");				\
	register __u32 arg2 asm ("r5");				\
	register __u32 arg3 asm ("r6");				\
								\
	int err;						\
								\
	if (credential == __ARCH_ACTV_CRED_INVAL_FWD_XACT) {	\
		err = E_HM_INVAL;				\
	} else {						\
		err = __##name(sender, credential, arg1, arg2, arg3); \
	}							\
	return err;						\
}								\
static rettype __##name(unsigned long long sender,		\
			unsigned long credential,		\
			__u32 arg1,				\
			__u32 arg2,				\
			__u32 arg3)

#else

#define DECLARE_KCALL_ACTVHDLR_ENTRY(name, rettype)		\
static inline rettype name(void)				\
{								\
	return 0;						\
}								\

#endif /* __HOST_LLT__ */
#endif /* !ARM_ULIBS_ASM_LIBSYSIF_UTILS_H */
