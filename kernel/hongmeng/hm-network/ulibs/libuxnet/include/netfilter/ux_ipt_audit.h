/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: AUDIT iptable target head file
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 17 10:01:02 2021
 */
#ifndef _UX_IPT_AUDIT_H_
#define _UX_IPT_AUDIT_H_

/* flags, invflags: */
enum {
	UX_IPT_AUDIT_TYPE_ACCEPT = 0,
	UX_IPT_AUDIT_TYPE_DROP = 1,
	UX_IPT_AUDIT_TYPE_REJECT = 2,
	__UX_IPT_AUDIT_TYPE_MAX = 3,
};

struct ux_ipt_audit_info {
	unsigned char type;
};

#ifdef CONFIG_AUDIT
int init_audit_tg(void);
#else
static inline int init_audit_tg(void)
{
	return E_HM_OK;
}
#endif

#endif
