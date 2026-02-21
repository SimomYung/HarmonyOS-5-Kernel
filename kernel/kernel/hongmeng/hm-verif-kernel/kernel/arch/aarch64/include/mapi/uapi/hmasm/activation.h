/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The uapi of the Activation for aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 14 19:29:08 2019
 */
#ifndef AARCH64_UAPI_ASM_ACTIVATION_H
#define AARCH64_UAPI_ASM_ACTIVATION_H

/*
 * The credentials that are invalid to use for granting
 * or setting. (-1 ~ -512)
 */
#if defined(__LP64__)
#define __ARCH_ACTV_CRED_INVAL_MAX		0xffffffffffffffffUL
#define __ARCH_ACTV_CRED_INVAL_MIN		0xfffffffffffffe00UL
#elif defined(__ILP32__)
#define __ARCH_ACTV_CRED_INVAL_MAX		0xffffffffUL
#define __ARCH_ACTV_CRED_INVAL_MIN		0xfffffe00UL
#else
#error "Unsupported ABI"
#endif

/*
 * default credential set when handling exception but the exp_actv
 * didn't bind to a exp_actv_pool at ACTV_BIND_POINT_FASTCALL_G1_NOBUF.
 */
#define __ARCH_ACTV_CRED_INVAL_EXCEPTION	__ARCH_ACTV_CRED_INVAL_MAX
/*
 * default credential set when forwarding a transaction but there
 * is no way to get the right credential by xact_conf.
 */
#define __ARCH_ACTV_CRED_INVAL_FWD_XACT		(__ARCH_ACTV_CRED_INVAL_MAX - 1UL)
/*
 * The credential set to indicate that the current call is a self actv call
 * which means the client and the server are in the same process.
 */
#define __ARCH_ACTV_CRED_INVAL_SELFCALL		(__ARCH_ACTV_CRED_INVAL_MAX - 2UL)
/*
 * The credential set to indicate that this is an actv upcall from the kernel.
 */
#define __ARCH_ACTV_CRED_INVAL_UPCALL		(__ARCH_ACTV_CRED_INVAL_MAX - 3UL)

/*
 * The credential set to indicate that this is an actv lsyscall from the kernel.
 */
#define __ARCH_ACTV_CRED_INVAL_LSYSCALL		(__ARCH_ACTV_CRED_INVAL_MAX - 4UL)

/*
 * default credential set when sync base actv credential by local area but there
 * is no way to get the exception table binding info.
 */
#define __ARCH_ACTV_CRED_INVAL_SERVICE_INFO		(__ARCH_ACTV_CRED_INVAL_MAX - 5UL)

/*
 * The credential set to indicate that selfcall with xact semantics.
 */
#define __ARCH_ACTV_CRED_INVAL_SELFCALL_XACT	(__ARCH_ACTV_CRED_INVAL_MAX - 6UL)

/*
 * default credential set to indicate that it is undefined
 * currently and can be set once again when needed.
 *
 * In future, if __ARCH_ACTV_CRED_UNDEFINED was all replaced by
 * __ARCH_ACTV_CRED_UNINITED in user space, it could be removed.
 */
#define __ARCH_ACTV_CRED_UNDEFINED		(__ARCH_ACTV_CRED_INVAL_MIN - 1UL)
/*
 * default credential set to indicate that it is uninited currently and
 * will trigger the upcall to set the credential.
 */
#define __ARCH_ACTV_CRED_UNINITED		(__ARCH_ACTV_CRED_INVAL_MIN - 2UL)

#endif
