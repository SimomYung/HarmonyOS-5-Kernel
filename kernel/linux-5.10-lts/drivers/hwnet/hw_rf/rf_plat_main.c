/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to start the hw_rf module.
 */

#include <securec.h>
#include <log/hw_log.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/poll.h>
#include <linux/version.h>

#include "rf_plat_driver.h"
#include "vcom/vcom.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG HW_RF_PLAT_MAIN
HWLOG_REGIST();

int32_t hw_rf_init_thread(void *data)
{
	int32_t ret;

	ret = hw_rfvcom_init();
	if (ret != 0) {
		hwlog_err("%s, task init failed\n", __func__);
		return -1;
	}

	ret = hw_rf_dev_init();
	if (ret != 0) {
		hwlog_err("%s, hlct dev init\n", __func__);
		return -1;
	}

	return ret;
}

int32_t __init hw_rf_plat_init(void)
{
	struct task_struct *task = kthread_run(hw_rf_init_thread, NULL, "hw_rf_init_thread");
	if (IS_ERR(task)) {
		hwlog_err("%s: failed to create thread", __func__);
		task = NULL;
		return -1;
	}
	return 0;
}

void __exit hw_rf_plat_exit(void)
{
	hw_rfvcom_exit();
	hw_rf_dev_exit();
}

module_init(hw_rf_plat_init);
module_exit(hw_rf_plat_exit);