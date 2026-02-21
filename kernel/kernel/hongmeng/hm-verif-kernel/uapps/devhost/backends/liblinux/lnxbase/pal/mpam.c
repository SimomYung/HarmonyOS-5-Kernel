/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: liblinux PAL implement for MPAM event
 * Author: Huawei Hisilicon
 * Create: Mon Aug 12 16:30:00 2024
 */
#include <liblinux/pal.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <devhost/log.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmactv/actv.h>
#include <hmkernel/trigger.h>
#include <hmkernel/interrupt/kvic.h>
#include <hmasm/kern_syscall.h>

int liblinux_pal_mpam_event_ctrl(void *event, unsigned int size)
{
	if (!event || !size) {
		return E_HM_INVAL;
	}

	return hm_sysctrl_read_conf(__SYSCONF_MPAM_EVENT, event, size);
}

int liblinux_pal_mpam_event_bind(unsigned long long cref)
{
	return hm_thread_kvic_source_bind(cref, __KVIC_SOURCE_ID_MPAM_EVENT, 0);
}

int liblinux_pal_mpam_event_unbind(unsigned long long cref)
{
	return hm_thread_kvic_source_unbind(cref, __KVIC_SOURCE_ID_MPAM_EVENT);
}

int liblinux_pal_mpam_event_wait(void)
{
	return hm_trigger_wait(__TRIGGER_WAIT_DONE, NULL);
}

int liblinux_pal_mpam_event_read(void *buf, unsigned long buf_size)
{
	return sysfast_trigger_read(buf, buf_size);
}