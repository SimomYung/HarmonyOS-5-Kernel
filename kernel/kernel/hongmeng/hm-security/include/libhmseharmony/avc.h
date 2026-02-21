/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony head file for avc interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_EXPORT_AVC_H
#define HMSEL_EXPORT_AVC_H

#include <libmem/mcache.h>
#include <libhmseharmony/services.h>
#include <libhmseharmony/seharmony.h>

#define HMSEL_AVC_STRICT	 1
#define HMSEL_AVC_EXTENDED_PERMS 2
#define HMSEL_AVC_NONBLOCKING    4

DECLARE_ULIBS_MCACHE(hmsel_avc_xperms_node)
int hmsel_avc_check_noaudit(struct hmsel_sid_context scon, hmsel_av_t requested,
			       unsigned flags, struct hmsel_av_decision *avd);

/* return only seharmony common audit info */
int hmsel_avc_check(struct hmsel_sid_context scon, hmsel_av_t requested);

bool hmsel_sid_context_is_equal(hmsel_sid_t ssid, hmsel_sid_t tsid);

/* return with caller specified audit callback info */
int hmsel_avc_query(struct hmsel_sid_context scon, hmsel_av_t requested, const struct sec_sel_audit *ad);
int hmsel_avc_xperms_query(struct hmsel_sid_context scon, hmsel_av_t requested,
			   uint8_t driver, uint8_t xperms,
			   const struct sec_sel_audit *ad);
int hmsel_cap_check(const struct sec_chk_cred *cred, hmsel_sid_t sid, int cap, unsigned int opts, bool initns);

int hmsel_avc_audit(struct hmsel_sid_context scon, hmsel_av_t requested,
		    struct hmsel_av_decision *avd, int res,
		    const struct sec_sel_audit *ad, hmsel_av_t auditdeny);
int hmsel_avc_query_audit(struct hmsel_sid_context scon, hmsel_av_t requested,
			  const struct sec_sel_audit *ad, hmsel_av_t auditdeny);

#endif
