/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Miscdata
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 04 19:26:00 2020
 */
#ifndef MAPI_UAPI_MISCDATA_H
#define MAPI_UAPI_MISCDATA_H

#define __sec_miscdata

/*
 * This header defines user available misc data.
 * Values are part of ABI and should be fixed.
 * internal u_to_k are required to convert misc data
 * to array index.
 */

/*
 * Misc data in actvconf (thread and actvpool).
 * All undefined
 */
#define __MISCDATA_ACTVCONF_SIGHDLR_PC	1	__sec_miscdata
#define __MISCDATA_ACTVCONF_SIGHDLR_SP	2	__sec_miscdata
#define __MISCDATA_ACTVCONF_CRED	3	__sec_miscdata

/*
 * Misc data in activation.
 */
#define __MISCDATA_ACTV_IPC_RW_BASE	6
#define __MISCDATA_ACTV_IPC_RW_LEN	7
#define __MISCDATA_ACTV_SIGPROC_STATE	8
#define __MISCDATA_ACTV_SIGRET_PC	9	__sec_miscdata
#define __MISCDATA_ACTV_SIGRET_SP	10	__sec_miscdata

/*
 * Misc data in tcb.
 */
#define __MISCDATA_TCB_SECCOMP_COIDX	23
#define __MISCDATA_TCB_SECCOMP		26
#define __MISCDATA_TCB_CRED		27	__sec_miscdata
#define __MISCDATA_TCB_VPID		28
#define __MISCDATA_TCB_VTID		29

#define __MISCDATA_TCB_PID		30
#define __MISCDATA_TCB_TID		31
#define __MISCDATA_TCB_ID		32

#define __MISCDATA_TCB_KCOV_IDX_STATE	33

#define __MISCDATA_TCB_UID		34

/*
 * Access (read or write) misc data from where?
 */
#define __MISCACCESS_SELF		1
#define __MISCACCESS_CALLER		2
#define __MISCACCESS_CALLER_G1		3
#define __MISCACCESS_CALLER_L1		4
#define __MISCACCESS_CALLER_G1L1	5
#define __MISCACCESS_XACT		6
#define __MISCACCESS_THREAD		7

#define __MISCACCESS_TRAPSOURCE	__MISCACCESS_CALLER_G1
#define __MISCACCESS_MGR	__MISCACCESS_CALLER_G1
#define __MISCACCESS_BASECALL	__MISCACCESS_CALLER_L1
#define __MISCACCESS_BASEMGR	__MISCACCESS_CALLER_G1L1

#endif
