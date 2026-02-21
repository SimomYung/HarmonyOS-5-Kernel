/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Native interfaces of hm event operation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 17:00:57 2023
 */
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_event.h>

#ifdef CONFIG_EVENTMGR
int hm_event_register_event(uref_t uref, enum evtmgr_etypes type, unsigned int callno)
{
	int rc = E_HM_OK;

	if ((unsigned int)type >= (unsigned int)EVTMGR_ETYPE_MAX) {
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && IS_REF_ERR(uref)) {
		rc = REF_TO_ERR(uref);
	}
	if (rc == E_HM_OK) {
		struct evtmgr_eparam_s param = {
			.uref = uref,
			.type = type,
			.callno = callno
		};
		rc = actvcall_hmcall_eventmgr_register_event(param);
		if (rc != E_HM_OK) {
			hm_debug("event register failed, err=%s\n", hmstrerror(rc));
		}
	}
	return rc;
}

int hm_event_unregister_event(uref_t uref, enum evtmgr_etypes type)
{
	int rc = E_HM_OK;

	if ((unsigned int)type >= (unsigned int)EVTMGR_ETYPE_MAX) {
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && IS_REF_ERR(uref)) {
		rc = REF_TO_ERR(uref);
	}
	if (rc == E_HM_OK) {
		rc = actvcall_hmcall_eventmgr_unregister_event(uref, type);
		if (rc != E_HM_OK) {
			hm_debug("event unregister failed, err=%s\n", hmstrerror(rc));
		}
	}
	return rc;
}

int hm_event_rule_ctrl(uref_t uref, enum evtmgr_erule_cmd cmd, const void *data, size_t size)
{
	int rc = E_HM_OK;

	if ((unsigned int)cmd >= (unsigned int)EVTMGR_ERULE_MAX) {
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && IS_REF_ERR(uref)) {
		rc = REF_TO_ERR(uref);
	}
	if (rc == E_HM_OK) {
		struct evtmgr_erule_param_s param = {
			.uref = uref,
			.cmd = cmd,
			.data = data,
			.size = size
		};
		rc = actvcall_hmcall_eventmgr_rule_ctrl(param);
		if (rc != E_HM_OK) {
			hm_debug("event set rule failed, err=%s\n", hmstrerror(rc));
		}
	}
	return rc;
}
#else /* CONFIG_EVENTMGR */
int hm_event_register_event(uref_t uref, enum evtmgr_etypes type, unsigned int callno)
{
	UNUSED(uref, type, callno);
	return E_HM_NOSYS;
}
int hm_event_unregister_event(uref_t uref, enum evtmgr_etypes type)
{
	UNUSED(uref, type);
	return E_HM_NOSYS;
}
int hm_event_rule_ctrl(uref_t uref, enum evtmgr_erule_cmd cmd, const void *data, size_t size)
{
	UNUSED(uref, cmd, data, size);
	return E_HM_NOSYS;
}
#endif /* CONFIG_EVENTMGR */
