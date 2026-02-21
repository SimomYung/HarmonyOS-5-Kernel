#include <linux/workqueue.h>
#include <linux/version.h>
#include <linux/time.h>
#include <securec.h>
 
#include <chipset_common/hwusb/hw_pd/hw_pd_core.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_interface.h>
#include <chipset_common/hwpower/hardware_monitor/water_detect.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif /* CONFIG_HUAWEI_OHOS_DSM */
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/usb/hw_pd/hw_pd_event.h>

#ifndef HWLOG_TAG
#define HWLOG_TAG huawei_pd_quirks
HWLOG_REGIST();
#endif

#define TIME_T_MAX (time64_t)((1UL << ((sizeof(time64_t) << 3) - 1)) - 1)

static struct work_struct fb_work;
static struct delayed_work cc_moisture_flag_restore_work;

/* used to handle abnormal cc change and report dmd */
static struct abnomal_change_info {
	enum abnomal_change_type event_id;
	bool first_enter; /* used to control if first cc change happend */
	int change_counter; /* used to count cc interrupt */
	int dmd_counter; /* used to count dmd report time */
	struct timespec64 ts64_last; /* last trigger time of cc interrupt  */
	struct timespec64 ts64_dmd_last; /* last trigger time of dmd report */
	int change_data[PD_DPM_CC_CHANGE_BUF_SIZE]; /* cc interrupt distributed data struct */
	int dmd_data[PD_DPM_CC_CHANGE_BUF_SIZE]; /* dmd data struct */
};

static struct abnomal_change_info abnomal_change[] = {
	{PD_DPM_ABNORMAL_CC_CHANGE, true, 0, 0, {0}, {0}, {0}, {0}},
	{PD_DPM_UNATTACHED_VBUS_ONLY, true, 0, 0, {0}, {0}, {0}, {0}},
};

static void abnomal_cc_check_timer(int event, int* change_counter)
{
	unsigned int time_diff_index;
	struct timespec64 ts64_now = { 0 };
	struct timespec64 ts64_sum = { 0 };
	struct timespec64 ts64_interval = { 0 };
	int time_diff;

	ts64_interval.tv_sec = 0;
    ts64_interval.tv_nsec = pd_state(abnormal_cc_interval) * NSEC_PER_MSEC;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
	ktime_get_coarse_real_ts64(&ts64_now);
#else
	ts64_now = current_kernel_time64();
#endif
	if (abnomal_change[event].first_enter) {
		abnomal_change[event].first_enter = false;
	} else {
		ts64_sum = timespec64_add_safe(abnomal_change[event].ts64_last,
			ts64_interval);
		if (ts64_sum.tv_sec == TIME_T_MAX) {
			hwlog_err("%s time overflow happend\n", __func__);
			*change_counter = 0;
		} else if (timespec64_compare(&ts64_sum, &ts64_now) >= 0) {
			++*change_counter;
			hwlog_info("%s change_counter = %d,\n", __func__, *change_counter);

			time_diff = (ts64_now.tv_sec -
				abnomal_change[event].ts64_last.tv_sec) *
				PD_DPM_CC_CHANGE_MSEC +
				(ts64_now.tv_nsec - abnomal_change[event].ts64_last.tv_nsec) /
				NSEC_PER_MSEC;
			time_diff_index = time_diff / (PD_DPM_CC_CHANGE_INTERVAL /
				PD_DPM_CC_CHANGE_BUF_SIZE);
			if (time_diff_index >= PD_DPM_CC_CHANGE_BUF_SIZE)
				time_diff_index = PD_DPM_CC_CHANGE_BUF_SIZE - 1;
			++abnomal_change[event].change_data[time_diff_index];
		} else {
			*change_counter = 0;
			memset_s(abnomal_change[event].change_data, PD_DPM_CC_CHANGE_BUF_SIZE, 0,
				PD_DPM_CC_CHANGE_BUF_SIZE);
		}
	}

	abnomal_change[event].ts64_last = ts64_now;
}

int hw_pd_ops_water_detect(void)
{
	int ret;

	ret = hw_pd_check_wather_detect();
	if (ret == WATER_DET_FALSE) {
		hwlog_err("%s is_water_detected FALSE\n", __func__);
		return WATER_DET_FALSE; /* instead of goto AFTER */
	} else if (ret == WATER_DET_TRUE) {
		hwlog_err("%s is_water_detected TRUE\n", __func__);
		return WATER_DET_TRUE;
	} else {
		return WATER_DET_DENY;
	}
}

static void abnomal_cc_exec_water_dectect(int event, int* dmd_counter, int* change_counter)
{
	int i;
	unsigned int flag;

	if (hw_pd_ops_water_detect() == WATER_DET_FALSE)
		return; /* instead of goto AFTER */

	if (pd_state(cc_moisture_report)) {
		pd_state(cc_moisture_happened) = true;
		pd_state(cc_moisture_status) = true;
	}

	hw_pd_set_cc_mode(PD_DPM_CC_MODE_UFP);

	for (i = 0; i < PD_DPM_CC_CHANGE_BUF_SIZE; i++)
		abnomal_change[event].dmd_data[i] += abnomal_change[event].change_data[i];
	*change_counter = 0;
	memset_s(abnomal_change[event].change_data, PD_DPM_CC_CHANGE_BUF_SIZE, 0, PD_DPM_CC_CHANGE_BUF_SIZE);
	++*dmd_counter;

	if (pd_state(cc_detect_moisture)) {
		hwlog_err("%s moisture_detected\n", __func__);
		flag = WD_NON_STBY_MOIST;
		power_event_bnc_notify(POWER_BNT_WD, POWER_NE_WD_REPORT_UEVENT, &flag);
	}
}

static void abnomal_cc_send_dmd(int event)
{
	char dsm_buf[PD_DPM_CC_DMD_BUF_SIZE] = { 0 };
	int i;
	struct timespec64 ts64_dmd_interval = { 0 };
	struct timespec64 ts64_dmd_now = { 0 };
	struct timespec64 ts64_dmd_sum = { 0 };

	ts64_dmd_interval.tv_sec = PD_DPM_CC_DMD_INTERVAL;
	ts64_dmd_interval.tv_nsec = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
	ktime_get_coarse_real_ts64(&ts64_dmd_now);
#else
	ts64_dmd_now = current_kernel_time64();
#endif
	ts64_dmd_sum = timespec64_add_safe(abnomal_change[event].ts64_dmd_last,
		ts64_dmd_interval);
	if (ts64_dmd_sum.tv_sec == TIME_T_MAX) {
		hwlog_err("%s time overflow happend when add 24 hours\n", __func__);
	} else if (timespec64_compare(&ts64_dmd_sum, &ts64_dmd_now) < 0) {
		sprintf_s(dsm_buf, PD_DPM_CC_DMD_BUF_SIZE - 1,
			"cc abnormal is triggered:");
		for (i = 0; i < PD_DPM_CC_CHANGE_BUF_SIZE; i++)
			sprintf_s(dsm_buf + strlen(dsm_buf), PD_DPM_CC_DMD_BUF_SIZE - 1,
				" %d", abnomal_change[event].dmd_data[i]);
		sprintf_s(dsm_buf + strlen(dsm_buf), PD_DPM_CC_DMD_BUF_SIZE - 1, "\n");

		power_dsm_report_dmd(POWER_DSM_BATTERY,
			POWER_DSM_ERROR_NO_WATER_CHECK_IN_USB, dsm_buf);

		abnomal_change[event].ts64_dmd_last = ts64_dmd_now;
	} else {
		hwlog_info("error: cc abnormal happend within 24 hour, do not report\n");
	}
}

/* Handle abnormal cc change event, like water-in or vbus-only. */
void hw_pd_abnomal_cc_change(enum abnomal_change_type event)
{
	int* change_counter = NULL;
	int* dmd_counter = NULL;
	int dmd_counter_threshold = PD_DPM_CC_DMD_COUNTER_THRESHOLD;
	int change_counter_threshold = PD_DPM_CC_CHANGE_COUNTER_THRESHOLD;

	if ((event != PD_DPM_ABNORMAL_CC_CHANGE) && (event != PD_DPM_UNATTACHED_VBUS_ONLY)) {
		hwlog_err("%s invalid event %d\n", __func__, event);
		return;
	}

	change_counter = &abnomal_change[event].change_counter;
	dmd_counter = &abnomal_change[event].dmd_counter;

	abnomal_cc_check_timer(event, change_counter);

	if (*change_counter >= change_counter_threshold) {
		hwlog_err("%s change_counter hit\n", __func__);
		if (pd_state(v700_cc_abnormal))
			hw_pd_set_cc_mode(PD_DPM_CC_MODE_UFP);
		abnomal_cc_exec_water_dectect(event, dmd_counter, change_counter);
	}

	if ((*dmd_counter >= dmd_counter_threshold) && pd_state(cc_abnormal_dmd)) {
		*dmd_counter = 0;
		abnomal_cc_send_dmd(event);
	}
}

/* Reset cc moisture flag after system fb event comes */
static void cc_moisture_restore_work(struct work_struct *work)
{
	hwlog_err("%s %d,%d\n", __func__,
		  pd_state(cc_moisture_happened),
		  pd_state(cc_moisture_status));
	if (!pd_state(cc_moisture_happened))
		pd_state(cc_moisture_status) = false;
}

/*
 * Function in work to process system fb event.
 * Broadcast this event to other module like charger driver.
 * Start a delayed work to reset cc moisture flag.
 */
static void fb_unblank_work(struct work_struct *work)
{
	unsigned int flag = WD_NON_STBY_DRY;

	hwlog_info("%s set pd to drp\n", __func__);
	hw_pd_set_cc_mode(PD_DPM_CC_MODE_DRP);
	power_event_bnc_notify(POWER_BNT_WD, POWER_NE_WD_REPORT_UEVENT, &flag);

	if (pd_state(cc_moisture_report) == 0)
		return;

	pd_state(cc_moisture_happened) = false;
	/* 120000: delay 2 minute to restore the flag */
	schedule_delayed_work(&cc_moisture_flag_restore_work, msecs_to_jiffies(120000));
}

/* Callback function to deal with system fb event */
static void fb_unblank_proc_cb(unsigned int type, char *event_data)
{
	hwlog_err("%s FB_BLANK_UNBLANK come\n", __func__);
	schedule_work(&fb_work);
}

/* Register calback function for fb unblank notification */
void hw_pd_abnormal_cc_detection_init(void)
{
	hwlog_info("%s abnormal_cc_detection_init\n", __func__);
	hw_pd_event_reg(HW_PD_EVENT_TYPE_FB, NULL, fb_unblank_proc_cb);
	INIT_WORK(&fb_work, fb_unblank_work);
	INIT_DELAYED_WORK(&cc_moisture_flag_restore_work, cc_moisture_restore_work);
}
