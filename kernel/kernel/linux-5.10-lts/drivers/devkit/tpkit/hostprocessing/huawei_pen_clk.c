/*
 * huawei_pen_clk.c
 *
 * Used to provide a clock signal to the stylus.
 *
 * Copyright (c) 2021-2021 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "huawei_pen_clk.h"

static struct thp_pen_clk_common *pen_clk_common = NULL;
static struct thp_pen_clk *multi_pen_clk[TOTAL_PANEL_NUM] = {NULL, NULL};
static u32 use_multi_pen_clk;
#define DEFAULT_CLK_FREQ 19200000
#define MS_TO_S 1000
#define MIN_CALIBRATION_TIME 60 /* 60s */

#ifdef CONFIG_CONTEXTHUB_IO_DIE_STS
static int thp_io_die_sts_clk_ctrl(uint32_t status, u32 panel_id)
{
	sts_clk_out_t clk_op;
	int rc;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	thp_request_vote_of_polymeric_chip(cd);
	mutex_lock(&multi_pen_clk[panel_id]->clk_mutex);
	clk_op.op_id = status;
	rc = sts_clk_out_sys(&clk_op);
	if (rc < 0) {
		thp_log_err("%s fail, rc = %d\n", __func__, rc);
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		thp_release_vote_of_polymeric_chip(cd);
		return -EINVAL;
	}
	mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
	thp_release_vote_of_polymeric_chip(cd);
	if (status == STS_SUB_CMD_OPEN) {
		atomic_set(&multi_pen_clk[panel_id]->status, CLK_ENABLE);
		thp_log_info("%s:enable clk success\n", __func__);
	} else {
		atomic_set(&multi_pen_clk[panel_id]->status, CLK_DISABLE);
		thp_log_info("%s:disable clk success\n", __func__);
	}
	return NO_ERR;
}
#endif

#if (defined(CONFIG_CONTEXTHUB_IO_DIE_PCIE) || defined(CONFIG_CONTEXTHUB_IO_DIE_HIPHYIO))
static int thp_io_die_clk_ctrl(uint32_t status, u32 panel_id)
{
	int rc;

	mutex_lock(&multi_pen_clk[panel_id]->clk_mutex);
	rc = iodie_clk_out_sys(status);
	if (rc < 0) {
		thp_log_err("%s fail, rc = %d\n", __func__, rc);
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return -EINVAL;
	}
	mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
	if (status == IODIE_CLK_OUT_CMD_OPEN) {
		atomic_set(&multi_pen_clk[panel_id]->status, CLK_ENABLE);
		thp_log_info("%s:enable clk success\n", __func__);
	} else {
		atomic_set(&multi_pen_clk[panel_id]->status, CLK_DISABLE);
		thp_log_info("%s:disable clk success\n", __func__);
	}
	return NO_ERR;
}
#endif

static int thp_soc_clk_enable(u32 panel_id)
{
	struct pinctrl_state *state = NULL;
	int rc;

	mutex_lock(&multi_pen_clk[panel_id]->clk_mutex);
	state = pinctrl_lookup_state(multi_pen_clk[panel_id]->clk_ctrl, "pwm_enable");
	if (IS_ERR(state)) {
		thp_log_err("%s: pinctrl lookup state fail\n", __func__);
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return -EINVAL;
	}
	rc = pinctrl_select_state(multi_pen_clk[panel_id]->clk_ctrl, state);
	if (rc) {
		thp_log_err("%s: pinctrl select state fail\n");
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return rc;
	}

	rc = clk_set_rate(multi_pen_clk[panel_id]->clk, (unsigned long)multi_pen_clk[panel_id]->clk_freq);
	if (rc) {
		thp_log_err("%s: set clk rate to %u fail\n", __func__,
			multi_pen_clk[panel_id]->clk_freq);
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return rc;
	}
	rc = clk_prepare_enable(multi_pen_clk[panel_id]->clk);
	if (rc) {
		thp_log_err("%s: clk prepare and enable fail.\n", __func__);
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return rc;
	}
	mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
	atomic_set(&multi_pen_clk[panel_id]->status, CLK_ENABLE);
	thp_log_info("%s:success\n", __func__);
	return NO_ERR;
}

void pen_clk_control(int status, u32 panel_id)
{
	int stylus3_status = thp_get_status(THP_STATUS_STYLUS3, panel_id);
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 i;
	bool flag = true;
	bool no_need_operate_pen_clk;

	for (i = 0; i <= cd->dual_thp; i++)
		flag = flag && thp_get_core_data(i)->suspended;

	/*
	 * need_extra_system_clk: need clk from soc.
	 * stylus3_status: the link status of pen
	 * support_fw_xtal_switch: if true, open pen clk when screen on and close pen clk when screen off
	 */
	no_need_operate_pen_clk = (!cd->need_extra_system_clk) ||
		(!stylus3_status && cd->support_fw_xtal_switch);
	if (no_need_operate_pen_clk)
		return;
	if (status) {
		thp_pen_clk_enable(panel_id);
	} else if (flag) {
		thp_pen_clk_disable(panel_id);
		if (stylus3_status)
			start_pen_clk_timer();
	}
}
EXPORT_SYMBOL(pen_clk_control);

int thp_pen_clk_enable(u32 panel_id)
{
	thp_log_info("%s:enter, panel %u\n", __func__, panel_id);
	if (!multi_pen_clk[panel_id]) {
		thp_log_info("%s:panel %d pen clk is null\n", __func__, panel_id);
		return NO_ERR;
	}
	if (atomic_read(&multi_pen_clk[panel_id]->status) != CLK_DISABLE) {
		thp_log_err("%s:panel %d invalid status\n", __func__, panel_id);
		return NO_ERR;
	}

	if (multi_pen_clk[panel_id]->clk_source == CLK_FROM_POLYMERIC_CHIP) {
#ifdef CONFIG_CONTEXTHUB_IO_DIE_STS
		return thp_io_die_sts_clk_ctrl(STS_SUB_CMD_OPEN, panel_id);
#elif (defined(CONFIG_CONTEXTHUB_IO_DIE_PCIE) || defined(CONFIG_CONTEXTHUB_IO_DIE_HIPHYIO))
		return thp_io_die_clk_ctrl(IODIE_CLK_OUT_CMD_OPEN, panel_id);
#else
		thp_log_err("%s: need check clk_source config\n", __func__);
		return NO_ERR;
#endif
	}

	return thp_soc_clk_enable(panel_id);
}
EXPORT_SYMBOL(thp_pen_clk_enable);

static int thp_soc_clk_disable(u32 panel_id)
{
	struct pinctrl_state *state = NULL;
	int rc;

	mutex_lock(&multi_pen_clk[panel_id]->clk_mutex);
	state = pinctrl_lookup_state(multi_pen_clk[panel_id]->clk_ctrl, "pwm_disable");
	if (IS_ERR(state)) {
		thp_log_err("%s: pinctrl lookup state fail.\n", __func__);
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return -EINVAL;
	}
	rc = pinctrl_select_state(multi_pen_clk[panel_id]->clk_ctrl, state);
	if (rc) {
		thp_log_err("%s: pinctrl select state fail\n");
		mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
		return rc;
	}

	clk_disable_unprepare(multi_pen_clk[panel_id]->clk);
	mutex_unlock(&multi_pen_clk[panel_id]->clk_mutex);
	atomic_set(&multi_pen_clk[panel_id]->status, CLK_DISABLE);
	thp_log_info("%s:success\n", __func__);
	return NO_ERR;
}

int thp_pen_clk_disable(u32 panel_id)
{
	thp_log_info("%s:enter, panel %u\n", __func__, panel_id);

	if (!multi_pen_clk[panel_id]) {
		thp_log_info("%s:panel %d pen clk is null\n", __func__, panel_id);
		return NO_ERR;
	}
	if (atomic_read(&multi_pen_clk[panel_id]->status) != CLK_ENABLE) {
		thp_log_err("%s:panel %d invalid status\n", __func__, panel_id);
		return NO_ERR;
	}

	if (multi_pen_clk[panel_id]->clk_source == CLK_FROM_POLYMERIC_CHIP) {
#ifdef CONFIG_CONTEXTHUB_IO_DIE_STS
		return thp_io_die_sts_clk_ctrl(STS_SUB_CMD_CLOSE, panel_id);
#elif (defined(CONFIG_CONTEXTHUB_IO_DIE_PCIE) || defined(CONFIG_CONTEXTHUB_IO_DIE_HIPHYIO))
		return thp_io_die_clk_ctrl(IODIE_CLK_OUT_CMD_CLOSE, panel_id);
#else
		thp_log_err("%s: need check clk_source config\n", __func__);
		return NO_ERR;
#endif
	}

	return thp_soc_clk_disable(panel_id);
}
EXPORT_SYMBOL(thp_pen_clk_disable);

static void thp_parse_common_pen_clk_info(struct device_node *node, struct thp_pen_clk *clk_data, u32 panel_id)
{
	int rc;

	const char *clk_name = (panel_id == MAIN_TOUCH_PANEL) ? "clock-names" : "sub-clock-names";
	const char *clk_freq = (panel_id == MAIN_TOUCH_PANEL) ? "clock-freq" : "sub-clock-freq";
	const char *clk_source = (panel_id == MAIN_TOUCH_PANEL) ? "clock-source" : "sub-clock-source";
	of_property_read_string(node, clk_name, &clk_data->clk_name);
	thp_log_info("%s = %s\n", clk_name, clk_data->clk_name);

	rc = of_property_read_u32(node, clk_freq, &clk_data->clk_freq);
	if (rc < 0)
		clk_data->clk_freq = DEFAULT_CLK_FREQ;
	thp_log_info("%s = %d\n", clk_freq, clk_data->clk_freq);

	rc = of_property_read_u32(node, clk_source, &clk_data->clk_source);
	if (rc < 0)
		clk_data->clk_source = CLK_FROM_SOC;
	thp_log_info("%s = %d\n", clk_source, clk_data->clk_source);
}

static int thp_pen_clk_init(struct platform_device *pdev, u32 panel_id)
{
	struct device_node *pen_clk_node = NULL;
	struct thp_pen_clk *clk_data = NULL;

	clk_data = devm_kzalloc(&pdev->dev, sizeof(*clk_data), GFP_KERNEL);
	if (!clk_data) {
		thp_log_info("%s:no memory for pen clk %u\n", __func__, panel_id);
		return -ENOMEM;
	}

	pen_clk_node = of_find_compatible_node(NULL, NULL, "huawei,pen_clk");
	if (!pen_clk_node) {
		thp_log_info("find_compatible_node huawei,pen_clk error\n");
		goto err;
	}

	if (panel_id == MAIN_TOUCH_PANEL) {
		of_property_read_u32(pen_clk_node, "use-multi-pen-clk", &use_multi_pen_clk);
		thp_log_info("%s:use_multi_pen_clk = %d\n", __func__, use_multi_pen_clk);
	}

	thp_parse_common_pen_clk_info(pen_clk_node, clk_data, panel_id);
	/* pen clrl clk init */
	if (clk_data->clk_source == CLK_FROM_SOC) {
		clk_data->clk = devm_clk_get(&pdev->dev, clk_data->clk_name);
		if (IS_ERR(clk_data->clk)) {
			thp_log_err("%s: get clk fail\n", __func__);
			goto err;
		}

		clk_data->clk_ctrl = devm_pinctrl_get(&pdev->dev);
		if (!clk_data->clk_ctrl) {
			thp_log_err("%s: get clk_ctrl failed\n", __func__);
			goto err;
		}
	}
	mutex_init(&clk_data->clk_mutex);
	atomic_set(&clk_data->status, CLK_DISABLE);
	multi_pen_clk[panel_id] = clk_data;
	thp_log_info("%s:init pen clk %u success\n", __func__, panel_id);
	return NO_ERR;
err:
	devm_kfree(&pdev->dev, clk_data);
	clk_data = NULL;
	return -EINVAL;
}

static int _pen_clk_calibration(u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int rc;
	unsigned int flag;

	rc = thp_pen_clk_enable(panel_id);
	if (rc) {
		thp_log_err("%s: enable panel %u clk failed\n", __func__, panel_id);
		return -EINVAL;
	}
	msleep(10); /* Wait for the clock signal to stabilize */
	flag = (!cd || !cd->thp_dev || !cd->thp_dev->ops ||
		!cd->thp_dev->ops->clk_calibration);
	if (flag) {
		thp_log_err("%s:input is null\n", __func__);
		goto err;
	}
	rc = cd->thp_dev->ops->clk_calibration(cd->thp_dev);
	if (rc) {
		thp_log_err("%s: enable clk failed\n", __func__);
		goto err;
	}
err:
	msleep(100); /* delay 100ms to wait clk calibration finish */
	if (cd->work_status == SUSPEND_DONE) {
		rc = thp_pen_clk_disable(panel_id);
		if (rc) {
			thp_log_err("%s: disable panel %u clk failed\n", __func__, panel_id);
			return -EINVAL;
		}
	}	
	thp_log_info("%s: out\n", __func__);
	return NO_ERR;
}

void pen_clk_calibration(void)
{
	int rc;
	int stylus3_status;

	u32 panel_id = get_active_panel();
	thp_log_info("%s:enter\n", __func__);
	if (!multi_pen_clk[panel_id] || !pen_clk_common) {
		thp_log_err("%s:pen clk info is null\n", __func__);
		return;
	}
	stylus3_status = thp_get_status(THP_STATUS_STYLUS3, panel_id);
	if (!stylus3_status) {
		thp_log_err("%s:pen is not be connectted\n", __func__);
		return;
	}
	__pm_stay_awake(pen_clk_common->pen_wake_lock);
	rc = _pen_clk_calibration(panel_id);
	start_pen_clk_timer();
	if (rc) {
		thp_log_err("%s:_pen_clk_calibration rc = %d\n", __func__, rc);
		__pm_relax(pen_clk_common->pen_wake_lock);
		goto err;
	}
	pen_clk_common->last_ts = ktime_get_boottime();
	thp_log_info("%s:clk calibration at %ld ms\n", __func__,
		ktime_to_ms(pen_clk_common->last_ts));
	__pm_relax(pen_clk_common->pen_wake_lock);
	return;
err:
	thp_log_info("%s:out\n", __func__);
}

static void pen_clk_calibration_work_fn(struct work_struct *work)
{
	thp_log_info("%s:enter\n", __func__);
	pen_clk_calibration();
}

void start_pen_clk_timer(void)
{
	unsigned int wakeup_ms = 300 * 1000; /* 5min */
	ktime_t now;
	ktime_t add;
	int rc;

	if (!pen_clk_common) {
		thp_log_info("%s:pen_clk_common is null\n", __func__);
		return;
	}
	now = ktime_get_boottime();
	add = ktime_set(wakeup_ms / MSEC_PER_SEC, (wakeup_ms % MSEC_PER_SEC) * NSEC_PER_MSEC);
	mutex_lock(&pen_clk_common->timer_mutex);
	rc = alarm_cancel(&pen_clk_common->alarm);
	alarm_start(&pen_clk_common->alarm, ktime_add(now, add));
	thp_log_info("%s:start new timer, rc = %d\n", __func__, rc);
	mutex_unlock(&pen_clk_common->timer_mutex);
}
EXPORT_SYMBOL(start_pen_clk_timer);

void stop_pen_clk_timer(void)
{
	int rc;

	if (!pen_clk_common) {
		thp_log_info("%s:pen_clk_common is null\n", __func__);
		return;
	}
	mutex_lock(&pen_clk_common->timer_mutex);
	rc = alarm_cancel(&pen_clk_common->alarm);
	mutex_unlock(&pen_clk_common->timer_mutex);
	thp_log_info("%s:alarm_cancel, rc = %d\n", __func__, rc);
}

DECLARE_WORK(pen_clk_calibration_work, pen_clk_calibration_work_fn);

static int pen_clk_resume(struct device *dev)
{
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);
	int stylus3_status;
	long delta;
	bool flag = false;
	u32 i;

	for (i = 0; i <= cd->dual_thp; i++)
		flag = flag || !thp_get_core_data(i)->suspended;
	if (flag) {
		thp_log_err("%s: tp in resume state\n", __func__);
		return NO_ERR;
	}
	stylus3_status = thp_get_status(THP_STATUS_STYLUS3, cd->panel_id);
	if (!stylus3_status) {
		thp_log_info("%s:pen is not be enable,return\n", __func__);
		return NO_ERR;
	}

	pen_clk_common->cur_ts = ktime_get_boottime();
	thp_log_info("%s:at %ld ms\n", __func__, ktime_to_ms(pen_clk_common->cur_ts));
	/*
	 * The last calibration until now takes less than 1 minutes,
	 * then the calibration will not be performed during this wake-up
	 */
	delta = (ktime_to_ms(pen_clk_common->cur_ts) - ktime_to_ms(pen_clk_common->last_ts)) / MS_TO_S;
	if (delta > MIN_CALIBRATION_TIME) {
		thp_log_info("%s:wakeup pen_clk_calibration_work\n", __func__);
		schedule_work(&pen_clk_calibration_work);
	}
	thp_log_info("%s:out\n", __func__);
	return NO_ERR;
}

static int pen_clk_suspend(struct device *dev)
{
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);
	int stylus3_status;

	if (!cd) {
		thp_log_err("%s:cd is null\n", __func__);
		return NO_ERR;
	}

	thp_log_info("%s:enter\n", __func__);
	stylus3_status = thp_get_status(THP_STATUS_STYLUS3, cd->panel_id);
	if (!stylus3_status) {
		thp_log_info("%s:pen is not be enable,return\n", __func__);
		return NO_ERR;
	}
	start_pen_clk_timer();
	thp_log_info("%s:out\n", __func__);
	return NO_ERR;
}

static enum alarmtimer_restart pen_clk_alarm_handle_timer(
	struct alarm *alarm, ktime_t now)
{
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);
	bool flag = false;
	u32 i;

	thp_log_info("%s:in\n", __func__);
	for (i = 0; i <= cd->dual_thp; i++)
		flag = flag || !thp_get_core_data(i)->suspended;
	if (flag) {
		thp_log_err("%s:tp work in resume status\n", __func__);
		return ALARMTIMER_NORESTART;
	}

	schedule_work(&pen_clk_calibration_work);
	thp_log_info("%s:pen clk timer, wakeup pen_clk_calibration_work\n", __func__);
	return ALARMTIMER_NORESTART;
}

static int pen_clk_common_init(struct platform_device *pdev)
{
	struct thp_pen_clk_common *clk_common = NULL;

	clk_common = devm_kzalloc(&pdev->dev, sizeof(*clk_common), GFP_KERNEL);
	if (!clk_common) {
		thp_log_info("%s:no memory\n", __func__);
		return -ENOMEM;
	}
	
	mutex_init(&clk_common->timer_mutex);
    clk_common->last_ts = ktime_get_boottime();
    thp_log_info("%s:last_ts = %ld ms\n", __func__, ktime_to_ms(clk_common->last_ts));
    clk_common->pen_wake_lock = wakeup_source_register(&pdev->dev, "pen_wake_lock");
    if (!clk_common->pen_wake_lock) {
        thp_log_err("%s: failed to init wakelock\n", __func__);
        goto err;
    }

	alarm_init(&clk_common->alarm, ALARM_BOOTTIME, pen_clk_alarm_handle_timer);
	pen_clk_common = clk_common;
	return NO_ERR;
err:
	devm_kfree(&pdev->dev, clk_common);
	clk_common = NULL;
	return -EINVAL;
}

static int pen_clk_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);

	thp_log_info("%s:enter\n", __func__);
	ret = thp_pen_clk_init(pdev, MAIN_TOUCH_PANEL);
	if (ret) {
		thp_log_info("fail to init pen clk\n", __func__);
		return ret;
	}

	if (cd->multi_panel_index != SINGLE_TOUCH_PANEL) {
		if (use_multi_pen_clk)
			ret = thp_pen_clk_init(pdev, SUB_TOUCH_PANEL);
		else
			multi_pen_clk[SUB_TOUCH_PANEL] = multi_pen_clk[MAIN_TOUCH_PANEL];
		if (ret) {
			thp_log_info("fail to init sub pen clk\n", __func__);
			goto err;
		}
	}
	ret = pen_clk_common_init(pdev);
	if (ret) {
		thp_log_info("fail to init pen clk common data\n", __func__);
		goto err1;
	}
	return NO_ERR;
err1:
	if (use_multi_pen_clk)
		devm_kfree(&pdev->dev, multi_pen_clk[SUB_TOUCH_PANEL]);
	multi_pen_clk[SUB_TOUCH_PANEL] = NULL;
err:
	devm_kfree(&pdev->dev, multi_pen_clk[MAIN_TOUCH_PANEL]);
	multi_pen_clk[MAIN_TOUCH_PANEL] = NULL;
	return ret;
}

static const struct dev_pm_ops pen_clk_pm_ops = {
	.prepare = pen_clk_suspend,
	.resume = pen_clk_resume,
};

static const struct of_device_id pen_clk_match_table[] = {
	{
		.compatible = "huawei,pen_clk",
		.data = NULL,
	},
	{},
};

static struct platform_driver pen_clk_driver = {
	.probe = pen_clk_probe,
	.driver = {
		.name = "pen-pwm",
		.owner = THIS_MODULE,
		.pm = &pen_clk_pm_ops,
		.of_match_table = of_match_ptr(pen_clk_match_table),
	},
};

int pen_clk_init(void)
{
	thp_log_info("%s:in\n", __func__);
	return platform_driver_register(&pen_clk_driver);
}

void pen_clk_exit(void)
{
	thp_log_info("%s:in\n", __func__);
	platform_driver_unregister(&pen_clk_driver);
}
