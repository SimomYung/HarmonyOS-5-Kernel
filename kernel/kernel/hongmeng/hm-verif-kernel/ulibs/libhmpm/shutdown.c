/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The ulibs for shutdown and reboot
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 11:45:55 2019
 */
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/shutdown.h>
#include <libhmactv/actv.h>
#include <libhwsecurec/securec.h>
#include <libsysif/pwrmgr/api.h>

#include "pm_sysif.h"

int hm_shutdown_timedout(long timeout_ms)
{
	int err;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	return actvcapcall_pmcall_shutdown_enter(rref, timeout_ms);
}

int hm_shut_down(void)
{
	return hm_shutdown_timedout(0L);
}

int hm_kexec_timedout(long timeout_ms)
{
	int err;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	return actvcapcall_pmcall_shutdown_kexec_enter(rref, timeout_ms);
}

int hm_softreboot(void)
{
	int err;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	return actvcapcall_pmcall_shutdown_softreboot_enter(rref);
}

int hm_reboot_timedout(const char *cmd, unsigned int reboot_reason, long timeout_ms)
{
	struct reboot_cmd reboot_cmd;
	rref_t rref;
	size_t len;
	int err;

	mem_zero_s(reboot_cmd);
	reboot_cmd.reboot_reason = reboot_reason;
	if (cmd != NULL) {
		len = strnlen(cmd, REBOOT_CMD_LEN);
		if (len >= REBOOT_CMD_LEN) {
			hm_warn("invalid reboot cmd len=%zu\n", len);
			return E_HM_INVAL;
		}
		if (strncpy_s(reboot_cmd.string, sizeof(reboot_cmd.string),
			      cmd, strlen(cmd)) != EOK) {
			hm_warn("strncpy_s failed\n");
			return E_HM_RESFAULT;
		}
	}
	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	hm_info("start actvcapcall_pmcall_shutdown_reboot_enter. timeout=%ld msec, reason:%u\n",
		timeout_ms, reboot_reason);
	return actvcapcall_pmcall_shutdown_reboot_enter(rref, reboot_cmd, timeout_ms);
}

int hm_reboot(const char *cmd)
{
	return hm_reboot_timedout(cmd, 0U, 0L);
}

int hm_system_state(enum hm_system_state *state)
{
	int err = E_HM_OK;
	rref_t rref;
	struct __actvret_pmcall_pwrmgr_system_state __ret;

	mem_zero_s(__ret);
	if (state == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = hm_path_acquire(PWRMGR_PATH, &rref);
		if (err != E_HM_OK) {
			hm_error("path '%s' acquire failed: %s\n",
				 PWRMGR_PATH, hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = actvcapcall_pmcall_pwrmgr_system_state(rref, &__ret);
		if (err == E_HM_OK) {
			*state = __ret.state;
		} else {
			hm_error("query hm system state failed: %s\n", hmstrerror(err));
		}
		(void)hm_path_release(PWRMGR_PATH, rref);
	}

	return err;
}
