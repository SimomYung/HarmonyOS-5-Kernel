// SPDX-License-Identifier: GPL-2.0
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsec/cred.h>
#include <libhmseharmony/avc.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/network.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_security.h>
#include "sec.h"

int sec_load_policy(const char *buffer, unsigned long size)
{
	int rc = hm_sel_load_policy_fwd(buffer, size);
	return -hmerrno2posix(rc);
}

int sec_update_enforce(int value)
{
	int rc = hm_sec_shconf_set_field(HM_SEC_SHCONF_OFFSET_SEL_ENFORCING, value);
	return -hmerrno2posix(rc);
}

int sec_current_task_sid(uint32_t *sid)
{
	int err;
	struct sec_cred_seharmony *selinux_cred;
	struct base_cred *b_cred;
	__u64 idx = 0;
	err = sysfast_miscdata_read(&idx, __MISCDATA_TCB_CRED,
				    __MISCACCESS_SELF);

	if (err != E_HM_OK)
		return -hmerrno2posix(err);

	b_cred = (struct base_cred *)sec_cred_idx_to_ptr(idx);

	selinux_cred = &SEL_OF_CRED(b_cred);
	if (sid != NULL)
		*sid = selinux_cred->sid;
	return -hmerrno2posix(err);
}

int sec_avc_perm(unsigned int tsid, unsigned short tclass, unsigned int requested)
{
	struct hmsel_sid_context con;
	int ret;

	ret = sec_current_task_sid(&con.ssid);
	if (ret == E_HM_OK) {
		con.tsid = tsid;
		con.tclass = tclass;
		ret = hmsel_avc_check(con, requested);
	}

	return -hmerrno2posix(ret);
}

int socket_sockcreate_sid(unsigned int tsid, unsigned short tclass, unsigned int *socksid)
{
	unsigned int ssid = tsid;

	if (ssid > HM_SECSID_NULL) {
		*socksid = ssid;
		return 0;
	}

	struct hmsel_sid_context con = {
		.ssid = ssid,
		.tsid = tsid,
		.tclass = tclass,
	};

	return hmsel_transition_sid(con, NULL, socksid);
}

unsigned short socket_type_to_secclass(int domain, int type, int protocol)
{
	return socket_type_to_sclass(domain, type, protocol);
}

int sec_update_cache_threshold(int value)
{
	int rc = hm_sec_shconf_set_field(HM_SEC_SHCONF_OFFSET_SEL_LOG_RESTRICT, value);
	return -hmerrno2posix(rc);
}

/* In the Harmony kernel, Security Identifier (SID) is independent of processes. Therefore, cross-process security
 * context verification cannot be completed by comparing whether the SIDs are the same. This interface is added to check
 * whether the security context is consistent with the SID of the curent task during SELinux access control of the
 * tzdriver driver node.
 */
bool sec_is_context_match_current_sid(const char *context)
{
	if (context == NULL)
		return false;
	uint32_t sid = 0;
	int ret = sec_current_task_sid(&sid);
	if (ret != E_HM_OK) {
		hm_error("failed to get current tast sid, ret=%d\n", ret);
		return false;
	}

	return hmsel_is_context_match_sid(context, sid);
}