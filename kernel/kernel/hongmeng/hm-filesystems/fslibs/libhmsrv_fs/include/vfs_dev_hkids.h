/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Device header file for hkids
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 18 11:42:15 2024
 */

#ifndef VFS_DEV_HKIDS_H
#define VFS_DEV_HKIDS_H

#include <sys/ioctl.h>


/* file */
#define CREATEFILE 0x00000001
#define DELFILE 0x00000002
#define SETFILEATTR 0x00000004
#define WRITEFILE 0x00000008
#define READFILE 0x00000010
/* process */
#define CREATPROCESS 0x00000020
#define DESTROYPROCESS 0x00000040
#define GETPROCESSATTR 0x00000080
#define SETPROCESSATTR 0x00000100
/* program behavior */
#define CREATEPIPE 0x00000200
#define EXECCMD 0x00000400
#define CALLAPI 0x00000800
/* network */
#define ACCESSPORT 0x00001000
#define CONNECTNET 0x00002000
/* memory tamper */
#define PROCESSCODETAMPER 0x00004000
#define KERNELKEYDATATAMPER 0x00008000
/* resource consumption */
#define CPURESOURCECONSEME 0x00010000
#define MEMRESOURCECONSEME 0x00020000
#define STORAGERESOURCECONSEME 0x00040000
/* account management */
#define LOGINSUCCESS 0x00080000
#define LOGINFAILED 0x00100000
#define NEWACCOUNT 0x00200000
/* device management */
#define OPERATEDEV 0x00400000
/* memory corruption kmodule list */
#define KMODULELIST 0x00800000

#define ALL_TOPIC 0xFFFFFFFF

enum {
	HKIDS_MODULE_FILE_OPEN = 0,
	HKIDS_MODULE_FILE_MODIFY = 1,
	HKIDS_MODULE_FILE_RENAME = 3,
	HKIDS_MODULE_FILE_ATTR = 4,
	HKIDS_MODULE_CREATPROCESS = 6,
	HKIDS_MODULE_DESTROYPROCESS,
	HKIDS_MODULE_GETPROCESSATTR,
	HKIDS_MODULE_SETPROCESSATTR,
	HKIDS_MODULE_CALLAPI = 11,
	HKIDS_MODULE_HKIP = 32,
	HKIDS_MODULE_XPM = 33,
	HKIDS_MODULE_BOOT_INTEGRITY = 34,
	HKIDS_MODULE_PRIVILEGED_PROCESS = 63,
	HKIDS_MODULE_NUM = 64,
};

struct hkids_ioctl_arg {
	unsigned int module_id;
	unsigned int cmd_id;
	void *cmd_args;
	unsigned int cmd_args_size;
};

#define DEV_HKIDS_MINOR					0x23

#define HM_HKIDS_IOCTL_BASE				'h'
#define HM_HKIDS_CMD_SEC_SUB_ID			0x01
#define HM_HKIDS_CMD_SEC_UNSUB_ID		0x02
#define HM_HKIDS_CMD_SEC_READFROM_ID	0x03
#define HM_HKIDS_CMD_SEC_EXEC_CMD_ID	0x04

#define HKIDS_CMD_SEC_SUB \
	_IOW(HM_HKIDS_IOCTL_BASE, HM_HKIDS_CMD_SEC_SUB_ID, struct hkids_ioctl_arg)
#define HKIDS_CMD_SEC_UNSUB \
	_IOW(HM_HKIDS_IOCTL_BASE, HM_HKIDS_CMD_SEC_UNSUB_ID, struct hkids_ioctl_arg)
#define HKIDS_CMD_SEC_READFROM \
	_IOW(HM_HKIDS_IOCTL_BASE, HM_HKIDS_CMD_SEC_READFROM_ID, struct hkids_ioctl_arg)
#define HKIDS_CMD_SEC_EXEC_CMD \
	_IOW(HM_HKIDS_IOCTL_BASE, HM_HKIDS_CMD_SEC_EXEC_CMD_ID, struct hkids_ioctl_arg)

#endif // VFS_DEV_HKIDS_H
