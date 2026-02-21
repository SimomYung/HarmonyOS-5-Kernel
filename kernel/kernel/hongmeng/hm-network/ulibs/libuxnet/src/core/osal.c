/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:  OSAL(OS Abstraction Layer) implementation file
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 5 15:48:34 2023
 */

#include "osal.h"
#include "ux_netlog.h"

#include <libhmactv/actv.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_procmgr.h>
#ifdef CONFIG_NETFS_FUSION
#include <intapi/nsmgr/ns_container.h>
#endif

void osal_hm_send_signal(int signo)
{
	int rc;
	pid_t vpid;
	struct siginfo_set sig_set;

	mem_zero_s(sig_set);
	vpid = hm_actv_local_thread_vpid_of();
	rc = actvxactcall_hmcall_signal_kill(true, true, vpid, signo, sig_set);
	if (rc < 0) {
		net_warn(HMNET_CORE,
			"send signal failed, vpid=%d, signo=%d, rc=%d\n",
			vpid, signo, rc);
	}
}

static inline bool is_valid_credential(unsigned long credential)
{
	if ((credential >= (unsigned long)__ARCH_ACTV_CRED_INVAL_MIN) ||
	    (credential == (unsigned long)__ARCH_ACTV_CRED_UNDEFINED)) {
		return false;
	}

	return true;
}

/*
 * once cred be set to owner,
 * we can get cred in actv context by callinfo,
 * not traverse owner-tree.
 */
unsigned long osal_hm_actv_cred(void)
{
	unsigned long cred;

	cred = sysif_actv_current_callinfo()->credential;
	if (is_valid_credential(cred)) {
		return cred;
	}

	/* ptr will be null */
	return 0;
}

uint32_t osal_hm_actv_src_cnode(void)
{
	return sysif_actv_src_cnode_idx();
}

uint32_t osal_hm_actv_fwd_cnode(void)
{
	return sysif_actv_fwd_cnode_idx();
}

pid_t osal_hm_root_ns_pid(void)
{
	return hm_actv_local_thread_pid_of();
}

bool osal_hm_check_thread_in_dh(void)
{
#ifdef CONFIG_NETFS_FUSION
	return current_thread_in_DH();
#else
	return false;
#endif
}

pid_t osal_hm_convert_pid_to_pidns(pid_t root_ns_pid, int pid_ns_id)
{
	struct __actvret_hmcall_proc_rootpid_to_pidinns ret_pid = {0};
	int rc;

	rc = actvxactcall_hmcall_proc_rootpid_to_pidinns(true, false, root_ns_pid, pid_ns_id, &ret_pid);
	if (rc < 0) {
		net_error(HMNET_CORE, "convert root_ns_pid = %d to pid_ns_id = %d failed rc = %s\n",
			  root_ns_pid, pid_ns_id, hmstrerror(rc));
	}

	return rc >= 0 ? ret_pid.ret_pid : 0;
}

int osal_hm_lock_pid(pid_t root_ns_pid)
{
	int rc;

	rc = actvxactcall_hmcall_proc_lock_pid(true, false, root_ns_pid, false /* get */);
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "lock pid failed rc = %s\n", hmstrerror(rc));
	}

	return rc;
}

void osal_hm_unlock_pid(pid_t root_ns_pid)
{
	int rc;

	rc = actvcall_hmcall_proc_lock_pid(root_ns_pid, true /* put */);
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "unlock pid failed pid = %d rc = %s\n", root_ns_pid, hmstrerror(rc));
	}
}

int osal_current_pid_nsid(void)
{
	unsigned int current_pid_nsid = -1;
	uint32_t self_cnode_idx = osal_hm_actv_src_cnode();
	int rc;

	rc = hm_nsid_of_cnode_idx(self_cnode_idx, CLONE_NEWPID, &current_pid_nsid);

	return rc == E_HM_OK ? (int)current_pid_nsid : rc;
}

pid_t osal_hm_try_convert_pid_to_current_pidns(pid_t root_ns_pid)
{
	pid_t rc = 0;

	if (root_ns_pid > 0) {
		int current_pid_nsid = osal_current_pid_nsid();
		if (current_pid_nsid >= 0) {
			rc = osal_hm_convert_pid_to_pidns(root_ns_pid, current_pid_nsid);
			if (rc < 0) {
				rc = 0;
			}
		}
	}

	return rc;
}
