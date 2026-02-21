/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: The sequential number for each system server.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 31 10:27:56 2025
*/
#ifndef __SYSIF_SERVER_NUM_H__
#define __SYSIF_SERVER_NUM_H__
enum __server_audit_numbers {
	__SERVER_INVAL = 0,
	__server_crypto,
	__server_devhost,
	__server_devmgr,
	__server_freqmgr,
	__server_fs,
	__server_hguard,
	__server_keymgr,
	__server_net,
	__server_pwrmgr,
	__server_srvmgr,
	__server_sysmgr,
	__server_uvmm,
	__SERVER_MAX
};
#endif
