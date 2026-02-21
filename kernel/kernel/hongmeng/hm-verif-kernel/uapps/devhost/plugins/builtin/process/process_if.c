/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Handler for devhost process sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 04 14:26:54 2023
 */

#include <libsysif/devhost/server.h>

#include <devhost/poll.h>
#include <devhost/plugin.h>
#include <devhost/process.h>

static task_exit_notify_fn_t g_task_exit_notify = NULL;

PUBLIC_SYMBOL
void devhost_task_exit_notify_register(task_exit_notify_fn_t func)
{
	if (g_task_exit_notify == NULL) {
		g_task_exit_notify = func;
	}
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_process_revoke, dh_process_revoke_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, cnode_idx, pid_t, pid, uid_t, uid)
{
	UNUSED(sender, credential);
	devhost_reject_fdtable_rref(cnode_idx);
	if (g_task_exit_notify != NULL) {
		g_task_exit_notify(pid, uid);
	}
	return E_HM_OK;
}

/*
 * devhost handles process notifications
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_process_revoke`
 * export symbol: NULL
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(process, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_process_revoke, dh_process_revoke_ext)
);
