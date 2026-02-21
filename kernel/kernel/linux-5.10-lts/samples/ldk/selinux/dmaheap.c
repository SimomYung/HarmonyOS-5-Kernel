// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/kernel.h>
#include <security.h>
#include <objsec.h>

#include "sec.h"
#include "av_permissions.h"

int security_iris_heap_allow(void)
{
	u32 sid;
	int ret = sec_current_task_sid(&sid);
	if (ret == 0) {
		ret = sec_avc_check(sid, sid, SECCLASS_DMAHEAP, DMAHEAP__IRIS_HEAP);
	}
	return ret;
}
