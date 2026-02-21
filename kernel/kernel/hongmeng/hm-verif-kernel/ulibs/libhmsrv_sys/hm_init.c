/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of init actions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 09 09:42:23 2020
 */
#include <hongmeng/types.h>
#include <libsysif/srvmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_init.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <hmkernel/drivers/rebootcode.h>

static rref_t srvmgr_rref = 0ULL;
static struct raw_mutex srvmgr_mutex = RAW_MUTEX_INITIALIZER;

static int srvmgr_acquire_rref(void)
{
	int err = E_HM_OK;
	raw_mutex_lock(&srvmgr_mutex);
	if (IS_REF_ERR(srvmgr_rref)) {
		err = hm_path_acquire(SRVMGR_SERVER_PATH, &srvmgr_rref);
	}
	raw_mutex_unlock(&srvmgr_mutex);
	return err;
}

int hminit_notify_service_ready(void)
{
	int ret = E_HM_OK;
	rref_t rref = 0ULL;

	/*
	 * As init and vfs execute concurrently,
	 * should retry path acquire until actvpool
	 * of init is ready.
	 */
	do {
		ret = hm_path_acquire(SRVMGR_SERVER_PATH, &rref);
		if (ret == E_HM_POSIX_NOENT) {
			NOFAIL(hm_thread_yield());
		}
	} while (ret == E_HM_POSIX_NOENT);

	if (ret == E_HM_OK) {
		ret = actvcapcall_srvcall_srvmgr_notify(rref,
							(int)HMINIT_EVENT_TYPE_READY);
	}
	if (rref != 0ULL) {
		NOFAIL(hm_path_release(SRVMGR_SERVER_PATH, rref));
	}

	return ret;
}

int hminit_srvmgr_set_dh_pid(pid_t dh_pid)
{
	int ret = E_HM_OK;
	rref_t rref = 0ULL;

	ret = hm_path_acquire(SRVMGR_SERVER_PATH, &rref);
	if (ret == E_HM_OK) {
		ret = actvcapcall_srvcall_srvmgr_set_dh_pid(rref, dh_pid);
	}

	if (rref != 0ULL) {
		(void)hm_path_release(SRVMGR_SERVER_PATH, rref);
	}

	return ret;
}

int hminit_notify_reboot(const char *reboot_reason, unsigned int reboot_flag)
{
	int ret = E_HM_OK;
	size_t len;
	struct reboot_args args;

	mem_zero_s(args);
	if (reboot_reason != NULL) {
		len = strnlen(reboot_reason, REBOOT_STR_MAX_LEN);
		if (len >= REBOOT_STR_MAX_LEN) {
			return E_HM_INVAL;
		}
		if (strncpy_s(args.reboot_reason, sizeof(args.reboot_reason),
			      reboot_reason, strlen(reboot_reason)) != 0) {
			return E_HM_POSIX_FAULT;
		}
	}
	args.reboot_flag = reboot_flag;

	ret = srvmgr_acquire_rref();
	if (ret == E_HM_OK) {
		ret = actvcapcall_srvcall_srvmgr_reboot(srvmgr_rref,
							args);
	}

	return ret;
}

int hminit_notify_shutdown(void)
{
	int ret = E_HM_OK;

	ret = srvmgr_acquire_rref();
	if (ret == E_HM_OK) {
		ret = actvcapcall_srvcall_srvmgr_shutdown(srvmgr_rref);
	}

	return ret;
}

int hminit_notify_softreboot(void)
{
	int ret = E_HM_OK;

	if (IS_REF_ERR(srvmgr_rref)) {
		ret = hm_path_acquire(SRVMGR_SERVER_PATH, &srvmgr_rref);
	}

	if (ret == E_HM_OK) {
		ret = actvcapcall_srvcall_srvmgr_softreboot(srvmgr_rref);
	}

	return ret;
}

int hminit_notify_kexec(void)
{
	int ret = E_HM_OK;

	ret = srvmgr_acquire_rref();
	if (ret == E_HM_OK) {
		ret = actvcapcall_srvcall_srvmgr_kexec(srvmgr_rref);
	}

	return ret;
}
