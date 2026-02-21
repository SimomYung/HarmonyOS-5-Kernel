/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include "nstack.h"
#include <linux/rculist.h>
#include "../smartcom_netlink.h"
#include "../smartcom_utils.h"

#ifdef CONFIG_HW_NSTACK_FI
#include "fi/fi.h"
#endif

void nstack_event_process(int32_t event, uint8_t *pdata, uint16_t len)
{
	switch (event) {
#ifdef CONFIG_HW_NSTACK_FI
	case NETLINK_SMARTCOM_DK_NSTACK_FI_CFG:
		if (pdata == NULL || len < sizeof(struct fi_cfg_head) ||
			((struct fi_cfg_head *)pdata)->len != len) {
			smartcom_loge("nstack received unsupported message, length: %u, expect: %zu",
				len, sizeof(struct fi_cfg_head));
			return;
		}
		fi_cfg_process((struct fi_cfg_head *)pdata);
	break;
#endif

	default:
		smartcom_loge(" : nstack received unsupported message");
		break;
	}
}
EXPORT_SYMBOL(nstack_event_process); /*lint !e580*/

void nstack_init(struct sock *nlfd)
{
#ifdef CONFIG_HW_NSTACK_FI
	fi_init(nlfd);
#endif
}
EXPORT_SYMBOL(nstack_init); /*lint !e580*/

void nstack_deinit(void)
{
#ifdef CONFIG_HW_NSTACK_FI
	fi_deinit();
#endif
}
EXPORT_SYMBOL(nstack_deinit); /*lint !e580*/