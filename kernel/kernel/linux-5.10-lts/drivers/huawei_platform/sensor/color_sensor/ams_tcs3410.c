/*
 * ams_tcs3410.c
 *
 * code for AMS tcs3410 sensor
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Description: code for AMS tcs3410 sensor
 * Author: caozhipeng
 * Create: 2024-09-18
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
#include "ams_tcs3410.h"
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/pm.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <huawei_platform/log/hw_log.h>
#include <securec.h>

#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif


// defines for special tests
#define HWLOG_TAG color_sensor
HWLOG_REGIST();

static bool color_calibrate_result = true;
static bool report_calibrate_result;
static u16 g_max_buffer_size;
static u16 *ring_buffer = NULL;
static u32 *read_out_buffer = NULL;
static u16 *buffer_ptr = NULL;
static u16 *head_ptr = NULL;
static u16 *tail_ptr = NULL;
static u8 first_circle_end;
static DEFINE_MUTEX(ring_buffer_mutex);
static DEFINE_MUTEX(enable_handle_mutex);
static struct color_chip *p_chip;
static int enable_status_before_cali;
static UINT16 report_logcount;
extern int color_report_val[MAX_REPORT_LEN];
extern uint32_t flicker_support;
extern int (*color_default_enable)(bool enable);
static void read_fd_data(struct color_chip *chip);
static bool als_enabled = false;
static int fd_enable = 0;
static bool is_already_report = true;
static int tcs3410_work_cnt[MAX_SENSOR_ID] = {0};

static void tcs3410_set_sample_time(struct tcs3410_ctx *ctx, uint16_t time);
static void tcs3410_set_als_polling_time(struct tcs3410_ctx *ctx, struct color_chip *chip);
static UINT32 const tcs3410_als_gain_lvl[] = {
	GAIN_LEVEL_1,
	GAIN_LEVEL_2,
	GAIN_LEVEL_3,
	GAIN_LEVEL_4,
	GAIN_LEVEL_5
};

static uint8_t tcs3410_als_chan_asat_mask[TCS3410_CHAN_NUM] = {
	TCS3410_ALS_STATUS_ALS_DATA0_SATURATION_MASK,
	TCS3410_ALS_STATUS_ALS_DATA1_SATURATION_MASK,
	TCS3410_ALS_STATUS_ALS_DATA2_SATURATION_MASK
};

// Indexed by the register value of gain - needs to be global
static uint32_t gain_tbl[GAIN_TBL_SIZE] = {
	(uint32_t)(12800 / 249.13),  //0.5x
	(uint32_t)(12800 / 123.85),  // 1x
	(uint32_t)(12800 / 62.97),  // 2x
	(uint32_t)(12800 / 31.72),  // 4x
	(uint32_t)(12800 / 15.53),  // 8x
	(uint32_t)(12800 / 7.97),  // 16x
	(uint32_t)(12800 / 3.99),  // 32x
	(uint32_t)(12800 / 2.01),  // 64x
	(uint32_t)(12800),  // 128x
	(uint32_t)(12800 * 1.93),  // 256x
	(uint32_t)(12800 * 3.8),  // 512x
	(uint32_t)(12800 * 7.42),  // 1024x
	(uint32_t)(12800 * 14.06),  // 2048x
	(uint32_t)(12800 * 25.35)  // 4096x  It's disabled by default
};

static void tcs3410_time_base_correction(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	uint64_t time_val_ns;
	uint32_t get_target;
	uint8_t buffer[BUFF_LEN] = {0};
	uint64_t step_time;
	struct timespec64 old_time;
	struct timespec64 new_time;

	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_OFF);
	color_sensor_set_byte(ctx->handle, TCS3410_VSYNC_CFG, VSYNC_CFG_VAL);
	color_sensor_set_byte(ctx->handle, TCS3410_TRIGGER_MODE, TRIGGER_MODE_VAL);
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_ON);
	color_sensor_set_byte(ctx->handle, TCS3410_VSYNC_CONTROL, SW_VSYNC_TRIGGER);

	ktime_get_real_ts64(&old_time);
	//# Wait period as defined above, accuracy of wait time is decisive for this method
	mdelay(CLOCK_DELAY_TIME);
	color_sensor_set_byte(ctx->handle, TCS3410_VSYNC_CONTROL, SW_VSYNC_TRIGGER);
	ktime_get_real_ts64(&new_time);
	time_val_ns = new_time.tv_nsec - old_time.tv_nsec;
	color_sensor_read_fifo(ctx->handle, TCS3410_VSYNC_PERIOD_L, buffer, BUFF_LEN);
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_OFF);
	get_target = (((buffer[1] << SHIFT_8) & 0x0000FF00) | (buffer[0] & 0x000000FF));

	if (get_target == INVALID_DENOMINATOR)
		return;
	step_time = (uint64_t)(time_val_ns * TIME_CONVERSION) / get_target; //get ns
	hwlog_info("%s: step_time = %llu get_target = %u time_val_ns = %llu\n",
		__func__, step_time, get_target, time_val_ns);

	if ((step_time <= HIGH_THRESHOLD) && (step_time >= LOW_THRESHOLD)) {
		if (chip->support_flk_2k)
			ctx->sample_time = (uint16_t)(SAMPLE_2KHZ / step_time) - 1;
		else
			ctx->sample_time = (uint16_t)(SAMPLE_1KHZ / step_time) - 1;
		hwlog_info("%s: new sample time = %u\n", __func__, ctx->sample_time);
	} else {
		hwlog_info("%s: is out of range -3/100 ~ +3/100)\n", __func__);
		if (chip->support_flk_2k)
			ctx->sample_time = TCS3410_SAMPLE_TIME_2K;
		else
			ctx->sample_time = TCS3410_SAMPLE_TIME_1K;
	}
	tcs3410_set_sample_time(ctx, ctx->sample_time);
}

static void tcs3410_set_field(struct i2c_client *handle, uint8_t reg, uint8_t data, uint8_t mask)
{
	uint8_t original_data = 0;
	uint8_t new_data;

	if (!handle) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	mutex_lock(&enable_handle_mutex);
	color_sensor_get_byte(handle, reg, &original_data);
	new_data = original_data & (~mask);
	new_data |= (data & mask);
	color_sensor_set_byte(handle, reg, new_data);
	mutex_unlock(&enable_handle_mutex);
}

static void clear_fifo_data(struct tcs3410_ctx *ctx)
{
	// Clear the flicket processing data.
	(void)memset_s(&ctx->fd, sizeof(ctx->fd), 0, sizeof(ctx->fd));
	tcs3410_set_field(ctx->handle, TCS3410_CONTROL, TCS3410_MASK_FIFO_CLR, TCS3410_MASK_FIFO_CLR);
}

static int tcs3410_get_chipid(struct i2c_client *handle)
{
	uint8_t i;
	uint8_t deviceId = 0;

	if (!handle) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -1;
	}

	for (i = 0; i < CHIPID_GET_TIME; i++) {
		color_sensor_get_byte(handle, TCS3410_ID, &deviceId);
		if (deviceId == TCS3410_CHIP_VALUE) {
			hwlog_info("%u: Chip is TCS3410\n", deviceId);
			return 0;
		}
	}
	return -1;
}

static uint16_t tcs3410_rbits_for_gain(uint16_t gain)
{
	uint16_t rbits = 0;

	if (gain <= TCS3410_RBITS_GAIN_1)
		rbits = TCS3410_RBITS_VALUE_4;
	else if (gain <= TCS3410_RBITS_GAIN_2)
		rbits = TCS3410_RBITS_VALUE_3;
	else if (gain <= TCS3410_RBITS_GAIN_3)
		rbits = TCS3410_RBITS_VALUE_2;
	else if (gain <= TCS3410_RBITS_GAIN_4)
		rbits = TCS3410_RBITS_VALUE_1;

	return rbits;
}

static void tcs3410_set_atime(struct tcs3410_ctx *ctx)
{
	uint16_t atime_ms = 0;
	uint32_t atime_us = 0;

	atime_us = (ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * MOD_CLOCK / TCS3410_DEFAULT_MODULATOR_PERIOD_TO_MS;

	atime_ms = (atime_us / TCS3410_DEFAULT_MODULATOR_PERIOD_TO_MS);
	ctx->alg_ctx.itime_ms =(uint16_t)atime_ms;
	hwlog_info("%s: als_nr_samples = %u, sample_time = %u, itime_ms = %u \n", __func__, ctx->als_nr_samples,
		ctx->sample_time, ctx->alg_ctx.itime_ms);
}

static void tcs3410_get_saturation(struct tcs3410_ctx *ctx)
{
	uint32_t atime_us = 0;

	atime_us = (ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * MOD_CLOCK / TCS3410_DEFAULT_MODULATOR_PERIOD_TO_MS;

	ctx->als.saturation_high = (uint16_t)((ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * 9 / 10);
	ctx->als.saturation_low = (uint16_t)((ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * 5 / 100);

	hwlog_debug("%s : als_nr_samples = %u, sample_time = %u, saturation_high = %u, saturation_low = %u\n",
		__func__, ctx->als_nr_samples, ctx->sample_time, ctx->als.saturation_high, ctx->als.saturation_low);
}
static void tcs3410_set_als_gain(struct tcs3410_ctx *ctx)
{
	uint8_t modulator0_gain = ctx->als.pd_gain[MODULATOR_0];
	uint8_t modulator1_gain = ctx->als.pd_gain[MODULATOR_1];
	uint8_t modulator2_gain = ctx->als.pd_gain[MODULATOR_2];

	hwlog_debug("%s : modulator0_gain = %u, modulator1_gain = %u,  modulator2_gain = %u \n", __func__, modulator0_gain,
		modulator1_gain, modulator2_gain);

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_L,
		((modulator1_gain << TCS3410_SHIFT_GAIN_MOD1) | (modulator0_gain << TCS3410_SHIFT_GAIN_MOD0)));
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_H,
		(modulator2_gain << TCS3410_SHIFT_GAIN_MOD2));

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_L,
		((modulator1_gain << TCS3410_SHIFT_GAIN_MOD1) | (modulator0_gain << TCS3410_SHIFT_GAIN_MOD0)));
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_H,
		(modulator2_gain << TCS3410_SHIFT_GAIN_MOD2));
}

static bool tcs3410_als_auto_gain_validate(struct tcs3410_ctx *ctx, tcs3410_modulator_t type)
{
	bool changed = false;
	ctx->als.pd_gain[type] = ctx->gain_reg[type];

	hwlog_debug("%s: raw_data = %u, [type] = %d, gain = %u, " "saturation_high = %u, saturation_low = %u. \n", __func__,
		ctx->als.raw_pds[type], type, ctx->als.pd_gain[type], ctx->als.saturation_high, ctx->als.saturation_low);

	// raw data is forced to 0xfffe when digital saturtion and 0xffff when analog saturation
	if ((ctx->als.raw_pds[type] >= ctx->als.saturation_high) ||
		(ctx->reg_status[ALS_STAT] & tcs3410_als_chan_asat_mask[type])) {
		hwlog_debug("%s : too high\n", __func__);
		if (ctx->als.pd_gain[type] > 2 && ctx->alg_ctx.als_polling_cnt != NORMAL_MODE) {
			ctx->als.pd_gain[type] = ctx->als.pd_gain[type] - FAST_STEP;
			changed = true;
			hwlog_debug("%s : decrease gain to %u, type : %d\n", __func__, ctx->als.pd_gain[type], type);
		} else if (ctx->als.pd_gain[type] > MIN_ALS_GAIN_LEVEL) {
			ctx->als.pd_gain[type] = ctx->als.pd_gain[type] - NORMAL_STEP;
			changed = true;
			hwlog_debug("%s : decrease gain to %u, type : %d\n", __func__, ctx->als.pd_gain[type], type);
		}
	} else if (ctx->als.raw_pds[type] < ctx->als.saturation_low || ctx->als.raw_pds[type] < 0xff) {
		// Set ALS gain to the next bigger gain
		// If the clear channel raw count is less than 20% of the max raw count, increase the ALS gain.
		hwlog_debug("%s : too low\n", __func__);
		if (ctx->als.pd_gain[type] < 11 && ctx->alg_ctx.als_polling_cnt != NORMAL_MODE) {
			ctx->als.pd_gain[type] = ctx->als.pd_gain[type] + FAST_STEP;
			changed = true;
			hwlog_debug("%s : increase gain to %u, type : %d\n", __func__, ctx->als.pd_gain[type], type);
		} else if (ctx->als.pd_gain[type] < MAX_ALS_GAIN_LEVEL) {
			ctx->als.pd_gain[type] = ctx->als.pd_gain[type] + NORMAL_STEP;
			changed = true;
			hwlog_debug("%s : increase gain to %u, type : %d\n", __func__, ctx->als.pd_gain[type], type);
		}
	}
	if (ctx->als.raw_pds[type] < ALS_DARK_DATA)
		ctx->als.pd_gain[type] = MAX_ALS_GAIN_LEVEL;

	ctx->als.pd_gain[MODULATOR_0] = ctx->als.pd_gain[type];
	ctx->als.pd_gain[MODULATOR_1] = ctx->als.pd_gain[type];
	ctx->als.pd_gain[MODULATOR_2] = ctx->als.pd_gain[type];

	hwlog_debug("%s: gain set gain to %u\n", __func__, ctx->als.pd_gain[MODULATOR_0]);

	return changed;
}

static bool tcs3410_need_change_gain(struct tcs3410_ctx *ctx)
{
	bool changed[NUM_MODULATORS] = {false};
	uint8_t tmp_enable = 0;
	int i;
	uint32_t max_raw_data = 0;
	int max_raw_idx = 0;

	for (i = 0; i < NUM_MODULATORS; i++) {
		if (ctx->als.raw_pds[i] > max_raw_data) {
			max_raw_data = ctx->als.raw_pds[i];
			max_raw_idx = i;
		}
	}
	hwlog_debug("tcs3410_als_auto_gain max_raw_data = %u, max_raw_idx = %d\n", max_raw_data, max_raw_idx);
	tcs3410_get_saturation(ctx);
	if (tcs3410_als_auto_gain_validate(ctx, max_raw_idx))
		changed[max_raw_idx] = 1;
	hwlog_debug("tcs3410_als_auto_gain: changed[0] = %d,changed[1] = %d,changed[2] = %d\n", changed[0], changed[1],
		changed[2]);
	if (changed[0] || changed[1] || changed[2]) {
		mutex_lock(&ctx->lock);
		color_sensor_get_byte(ctx->handle, TCS3410_ENABLE, &tmp_enable);
		tcs3410_set_field(ctx->handle, TCS3410_ENABLE, 0, TCS3410_MASK_AEN | TCS3410_MASK_FDEN);
		tcs3410_set_als_gain(ctx);
		color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, tmp_enable);
		ctx->alg_ctx.step_status = IDLE_STEP;
		mutex_unlock(&ctx->lock);
		hwlog_debug("%s: changed status = %u, %u, %u tmp_enable = %u\n", __func__, changed[0], changed[1], changed[2],
			tmp_enable);
	}

	return (changed[0] || changed[1] || changed[2]);
}

static uint8_t get_rgb_fd_enable_status(struct i2c_client *handle)
{
	uint8_t enable_sta = 0;

	if (!handle) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}
	color_sensor_get_byte(handle, TCS3410_ENABLE, &enable_sta);
	return enable_sta;
}

static void tcs3410_als_set_enable(struct color_chip *chip, int en)
{
	struct tcs3410_ctx *ctx = NULL;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;
	if (ctx->alg_ctx.enabled && (en == 1)) {
		hwlog_info("%s, rgb already enabled, return\n", __func__);
		return;
	}

	if (!ctx->alg_ctx.enabled && (en == 0)) {
		hwlog_info("%s, rgb already disabled, return\n", __func__);
		return;
	}
	ctx->alg_ctx.enabled = en;
	ctx->alg_ctx.als_polling_cnt = ALS_POLLING_CNT;
	ctx->alg_ctx.step_status = IDLE_STEP;
	hwlog_info("%s: tcs3410_work_cnt = %d, %d\n", __func__, tcs3410_work_cnt[FLK_ID], tcs3410_work_cnt[RGB_ID]);
	if (en) {
		ctx->als_first_report = 1;
		tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_MASK_PON | TCS3410_MASK_AEN,
			TCS3410_MASK_PON | TCS3410_MASK_AEN);
		mod_timer(&chip->work_timer,
			jiffies + msecs_to_jiffies(ALS_FAST_POLLING_TIME));
		hwlog_info("tcs3410 als enable done\n");
	} else {
		ctx->als_first_report = 0;
		tcs3410_set_field(ctx->handle, TCS3410_ENABLE, 0, TCS3410_MASK_AEN);
		hwlog_info("tcs3410 als disable done\n");
	}
	report_logcount = 0;
}

static void tcs3410_als_timer_wrk(struct timer_list *t)
{
	struct color_chip *chip = from_timer(chip, t, work_timer);

	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	queue_work(system_power_efficient_wq, &chip->als_work);
}

static void tcs3410_flc_timer_wrk(struct timer_list *t)
{
	struct color_chip *chip = from_timer(chip, t, fd_timer);

	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	queue_work(system_power_efficient_wq, &chip->fd_work);
}

static void fd_enable_set(struct color_chip *chip, uint8_t en)
{
	struct tcs3410_ctx *ctx = NULL;
	uint8_t fd_enable_status;
	uint8_t cnt = 0;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;
	hwlog_info("%s: tcs3410_work_cnt = %d, %d\n", __func__, tcs3410_work_cnt[FLK_ID], tcs3410_work_cnt[RGB_ID]);
	fd_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if (en) {
		if ((fd_enable_status & FDEN) != FDEN) {
			while ((fd_enable_status & FDEN) != FDEN && cnt < WAIT_TIME_MAX) {
				tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_MASK_FDEN, TCS3410_MASK_FDEN);
				cnt += EACH_TIME_WAIT_TIME;
				mdelay(2); // wait 2ms every time
				fd_enable_status = get_rgb_fd_enable_status(ctx->handle);
			}
			tcs3410_set_field(ctx->handle, TCS3410_INTENAB, TCS3410_MASK_FIEN | TCS3410_MASK_SIEN,
				TCS3410_MASK_FIEN | TCS3410_MASK_SIEN);
			tcs3410_set_field(ctx->handle, TCS3410_SIEN, TCS3410_MASK_SIEN_SIEN_MEASR_SEQR,
				TCS3410_MASK_SIEN_SIEN_MEASR_SEQR);
			hwlog_info("tcs3410 fd enable done\n");

			mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FIRST_FLK_TIMER)); // first enable flicker timer
			hwlog_info("fd_enable 6ms for a gain quickly\n");
		} else {
			hwlog_warn("fd_enable fd already been enabled\n");
		}
	} else {
		if ((fd_enable_status & FDEN) == FDEN) {
			tcs3410_set_field(ctx->handle, TCS3410_INTENAB, TCS3410_ENABLE_OFF, TCS3410_MASK_AIEN | TCS3410_MASK_SIEN);
			tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_OFF, TCS3410_MASK_FDEN);
			tcs3410_set_field(ctx->handle, TCS3410_SIEN, TCS3410_ENABLE_OFF, TCS3410_MASK_SIEN_SIEN_MEASR_SEQR);

			// clear ring_buffer when close the fd
			mutex_lock(&ring_buffer_mutex);
			(void)memset_s(ring_buffer, g_max_buffer_size * BUF_RATIO_8_16BIT, 0, g_max_buffer_size * BUF_RATIO_8_16BIT);
			buffer_ptr = &ring_buffer[0];
			head_ptr = &ring_buffer[0];
			tail_ptr = &ring_buffer[0];
			mutex_unlock(&ring_buffer_mutex);
			hwlog_info("fd_enable now disable fd sensor\n");
		} else {
			hwlog_warn("fd_enable has already been disabled\n");
		}
	}
}

void tcs3410_fd_show_enable(struct color_chip *chip, int *state)
{
	uint8_t fd_enable_status;

	if (!chip || !state) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	fd_enable_status = get_rgb_fd_enable_status(chip->client);
	if ((fd_enable_status & TCS3410_MASK_FDEN) == TCS3410_MASK_FDEN)
		*state = 1;
	else
		*state = 0;
}

static void tcs3410_show_enable(struct color_chip *chip, int *state)
{
	uint8_t fd_enable_status;

	if (!chip || !state) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	fd_enable_status = get_rgb_fd_enable_status(chip->client);
	if ((fd_enable_status & TCS3410_MASK_AEN) == TCS3410_MASK_AEN)
		*state = 1;
	else
		*state = 0;
}

void tcs3410_fd_store_enable(struct color_chip *chip, int state)
{
	struct tcs3410_ctx *ctx = NULL;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;
	fd_enable = state;
	if (ctx->als_first_report == 1) {
		hwlog_info("%s: return delay fd enable \n", __func__);
		return;
	}
	if (state)
		// check the calibration process, disable fd function
		fd_enable_set(chip, AMSDRIVER_FD_ENABLE);
	else
		fd_enable_set(chip, AMSDRIVER_FD_DISABLE);

	hwlog_info("%s enable = %d success\n", __func__, state);
}

static void tcs3410_handle_als(struct tcs3410_ctx *ctx)
{
	uint8_t als_data_reg[9];
	uint8_t als_status = 0;

	ctx->als.raw_pds[MODULATOR_0] = 0;
	ctx->als.raw_pds[MODULATOR_1] = 0;
	ctx->als.raw_pds[MODULATOR_2] = 0;
	// read 9bit data
	color_sensor_read_fifo(ctx->handle, TCS3410_ALS_STATUS, als_data_reg, READ_NINE_BITS);
	ctx->reg_status[ALS_STAT] = als_data_reg[0];
	ctx->reg_status[ALS_STAT2] = als_data_reg[7];
	ctx->reg_status[ALS_STAT3] = als_data_reg[8];

	hwlog_debug("0x94 = 0x%02X, 0x9B = 0x%02X, 0x9C = 0x%02X", ctx->reg_status[ALS_STAT], ctx->reg_status[ALS_STAT2],
		ctx->reg_status[ALS_STAT3]);

	als_status = ctx->reg_status[ALS_STAT];
	ctx->als.raw_pds[MODULATOR_0] = (uint16_t)als_data_reg[1] | ((uint16_t)als_data_reg[2] << SHIFT_8);
	ctx->als.raw_pds[MODULATOR_1] = (uint16_t)als_data_reg[3] | ((uint16_t)als_data_reg[4] << SHIFT_8);
	ctx->als.raw_pds[MODULATOR_2] = (uint16_t)als_data_reg[5] | ((uint16_t)als_data_reg[6] << SHIFT_8);
	hwlog_debug("Raw PD Data in %s: MOD0 = 0x%04X, MOD1 = 0x%04X, MOD2 = 0x%04X", __func__, ctx->als.raw_pds[MODULATOR_0],
		ctx->als.raw_pds[MODULATOR_1], ctx->als.raw_pds[MODULATOR_2]);

	if (als_status & TCS3410_MASK_ALS_DATA0_SCALED_STATUS)
		ctx->als.raw_pds[MODULATOR_0] = (ctx->als.raw_pds[MODULATOR_0] >> 4);

	if (als_status & TCS3410_MASK_ALS_DATA1_SCALED_STATUS)
		ctx->als.raw_pds[MODULATOR_1] = (ctx->als.raw_pds[MODULATOR_1] >> 4);

	if (als_status & TCS3410_MASK_ALS_DATA2_SCALED_STATUS)
		ctx->als.raw_pds[MODULATOR_2] = (ctx->als.raw_pds[MODULATOR_2] >> 4);

	ctx->gain_reg[MODULATOR_0] = ((ctx->reg_status[ALS_STAT2] & 0x0F) >> 0);
	ctx->gain_reg[MODULATOR_1] = ((ctx->reg_status[ALS_STAT2] & 0xF0) >> 4);
	ctx->gain_reg[MODULATOR_2] = ((ctx->reg_status[ALS_STAT3] & 0x0F) >> 0);

	ctx->step = (ctx->reg_status[ALS_STAT] & TCS3410_MASK_ALS_STATUS_MEAS_SEQR_STEP) >>
			TCS3410_SHIFT_ALS_STATUS_MEAS_SEQR_STEP;
	hwlog_debug("%s: step = %u\n", __func__, ctx->step);
}

static void tcs3410_get_als_normdata(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	uint32_t diff = 0;
	ctx->gain_mod[MODULATOR_0] = gain_tbl[ctx->gain_reg[MODULATOR_0]];
	ctx->gain_mod[MODULATOR_1] = gain_tbl[ctx->gain_reg[MODULATOR_1]];
	ctx->gain_mod[MODULATOR_2] = gain_tbl[ctx->gain_reg[MODULATOR_2]];

	if (ctx->gain_mod[MODULATOR_0] == 0 || ctx->gain_mod[MODULATOR_1] == 0 ||
		ctx->gain_mod[MODULATOR_2] == 0)
		return;
	hwlog_debug("is_zoom_data = %d", ctx->alg_ctx.is_zoom_data);
	if (ctx->alg_ctx.is_zoom_data) {
		for (int i = 0; i < TCS3410_CHAN_NUM; i++) {
			if (chip->support_flk_2k)
				ctx->als.raw_pds[i] = ctx->als.raw_pds[i] * FD_NR_SAMPLES_2K / FD_QUICK_NR_SAMPLES;
			else
				ctx->als.raw_pds[i] = ctx->als.raw_pds[i] * FD_NR_SAMPLES_1K / FD_QUICK_NR_SAMPLES;
		}
	}

	// normalize counts based on gains , Normalize data if gains are different
	ctx->norm_pds[MODULATOR_0] = (uint32_t)(ctx->als.raw_pds[MODULATOR_0] * GC_AGAIN * GAIN_RATIO) /
		(ctx->gain_mod[MODULATOR_0]);
	ctx->norm_pds[MODULATOR_1] = (uint32_t)(ctx->als.raw_pds[MODULATOR_1] * GC_AGAIN * GAIN_RATIO) /
		(ctx->gain_mod[MODULATOR_1]);
	ctx->norm_pds[MODULATOR_2] = (uint32_t)(ctx->als.raw_pds[MODULATOR_2] * GC_AGAIN * GAIN_RATIO) /
		(ctx->gain_mod[MODULATOR_2]);
	ctx->norm_data[PD_CLEAR] = ctx->norm_pds[MODULATOR_0];
	ctx->norm_data[PD_BLUE] = ctx->norm_pds[MODULATOR_1];
	if (ctx->step == 0) { // c b r
		ctx->alg_ctx.step_status = ENTER_STEP0;
		ctx->norm_data[PD_RED] = ctx->norm_pds[MODULATOR_2];
		hwlog_debug("%s ENTER_STEP0:c_norm = %u, r_norm = %u, b_norm = %u\n", __func__, ctx->norm_data[PD_CLEAR],
			ctx->norm_data[PD_RED], ctx->norm_data[PD_BLUE]);
		ctx->step0_c_val = ctx->norm_data[PD_CLEAR];
	} else { // c b g
		if (ctx->alg_ctx.step_status == ENTER_STEP0)
			ctx->alg_ctx.step_status = ENTER_STEP1;
		ctx->norm_data[PD_GREEN] = ctx->norm_pds[MODULATOR_2];
		hwlog_debug("%s ENTER_STEP1:c_norm = %u, g_norm = %u, b_norm = %u\n", __func__, ctx->norm_data[PD_CLEAR],
			ctx->norm_data[PD_GREEN], ctx->norm_data[PD_BLUE]);
		ctx->step1_c_val = ctx->norm_data[PD_CLEAR];
		if ((ctx->step0_c_val > 0) && (ctx->step0_c_val < STEP0_C_VAL_100)) {
			if (((ctx->step1_c_val > STEP1_C_VAL_1000) && (ctx->step1_c_val < AMS_FLOAT_TO_FIX)) ||
				((ctx->step1_c_val > AMS_FLOAT_TO_FIX) && (ctx->step1_c_val < CANNEL_MAX_CONT))) {
				diff = (uint32_t)(ctx->step1_c_val / ctx->step0_c_val);
				ctx->norm_data[PD_RED] = ctx->norm_data[PD_RED] * diff;
				hwlog_debug("%s :step0_c_val = %u, step1_c_val = %u, r_norm = %u\n", __func__, ctx->step0_c_val,
					ctx->step1_c_val, ctx->norm_data[PD_RED]);
			}
		}
	}
}

static void tcs3410_handle_als_event(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	if (!ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	
	color_sensor_get_byte(ctx->handle, TCS3410_STATUS2, &ctx->alg_ctx.status);
	is_already_report = true;
	if (ctx->alg_ctx.status & STATUS2_ALS_DATA_VALID_MASK) {
		tcs3410_handle_als(ctx);
		ctx->is_change_gain = tcs3410_need_change_gain(ctx);
		if (ctx->is_change_gain) {
			report_calibrate_result = false;
			tcs3410_set_field(ctx->handle, TCS3410_CONTROL, TCS3410_MASK_FIFO_CLR, TCS3410_MASK_FIFO_CLR);
			hwlog_info("%s: tcs3410_need_change_gain\n", __func__);
			return;
		}
		tcs3410_get_als_normdata(ctx, chip);
	} else {
		is_already_report = false;
		hwlog_debug("%s: No new als data were available\n", __func__);
	}
}

static int tcs3410_report_data(int value[])
{
	if (!als_enabled)
		return 0;
	report_logcount++;
	if (report_logcount <= AMS_REPORT_LOG_COUNT_NUM || report_logcount % AMS_REPORT_LOG_COUNT_NUM == 0)
		hwlog_info("color_report_val: %d, %d, %d, %d",
			color_report_val[1], color_report_val[2], color_report_val[3], color_report_val[4]);

	if (report_logcount >= MAX_REPORT_LOGCOUNT)
		report_logcount = 0;

	return ap_color_report(value, TCS3410_REPORT_DATA_LEN * sizeof(int));
}

static void tcs3410_cal_als(struct color_chip *chip)
{
	uint32_t i;
	struct tcs3410_ctx *ctx = NULL;
	ctx = chip->device_ctx;
	bool valid_data = true;

	for (i = 0; i < TCS3410_REPORT_DATA_LEN; i++) {
		if (color_report_val[i + 1] == 0) {
			hwlog_warn("%s: color_report_val[%d] == 0, can not divide by zero\n", __func__, i + 1);
			ctx->cal_ctx.ratio[i] = CAL_RATIO_DEFAULT;
			valid_data = false;
		} else {
			hwlog_info("%s: color_report_val[%d] == %d\n", __func__, i + 1, color_report_val[i + 1]);
			ctx->cal_ctx.ratio[i] = (ctx->cal_ctx.target[i] * AMS_FLOAT_TO_FIX) / (color_report_val[i + 1]);
			hwlog_info("%s: ratio = %u\n", __func__, ctx->cal_ctx.ratio[i]);
		}
	}
	report_calibrate_result = valid_data;
}

static void tcs3410_report_als(struct color_chip *chip)
{
	hwlog_debug("%s: ENTER\n", __func__);
	UINT8 i;
	struct tcs3410_ctx *ctx = NULL;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	ctx = chip->device_ctx;
	color_report_val[0] = TCS3410_REPORT_DATA_LEN;
	for (i = 0; i < TCS3410_REPORT_DATA_LEN; i++)
		color_report_val[i + 1] = ctx->norm_data[i];

	if (ctx->als_first_report == 1) {
		if (!chip->in_cal_mode && is_already_report) {
			tcs3410_report_data(color_report_val + 1);
			if (fd_enable == AMSDRIVER_FD_ENABLE) {
				hwlog_info("%s enable fd after report 1st als: als_first_report = 0, fd_enable = 1 \n", __func__);
				fd_enable_set(chip, AMSDRIVER_FD_ENABLE);
			}
			ctx->als_first_report = 0;
		}
	} else {
		if (!chip->in_cal_mode && ctx->alg_ctx.step_status == ENTER_STEP1 && is_already_report) {
			tcs3410_report_data(color_report_val + 1);
			ctx->alg_ctx.step_status = IDLE_STEP;
			hwlog_debug("%s: ctx->alg_ctx.step_status = %u \n", __func__, ctx->alg_ctx.step_status);
		}
	}
	if (chip->in_cal_mode)
		tcs3410_cal_als(chip);
}

static void tcs3410_als_work(struct work_struct *work)
{
	UINT8 rgb_enable_status;
	struct tcs3410_ctx *ctx = NULL;
	struct color_chip *chip = NULL;

	if (!work) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	chip = container_of(work, struct color_chip, als_work);
	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: Pointer chip is NULL\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;
	tcs3410_work_cnt[RGB_ID]++;
	tcs3410_handle_als_event(ctx, chip);

	if (ctx->is_change_gain == false)
		tcs3410_report_als(chip);

	rgb_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if ((rgb_enable_status & AEN) != AEN) {
		hwlog_warn("%s: rgb already disabled, del timer\n", __func__);
		if (ctx->alg_ctx.enabled) {
			tcs3410_set_field(ctx->handle, TCS3410_ENABLE, PON | AEN, PON | AEN);
			hwlog_info("%s: alg_ctx enabled: status= %u\n", __func__, ctx->alg_ctx.enabled);
		} else {
			return;
		}
	}
	tcs3410_set_als_polling_time(ctx, chip);
	hwlog_debug("%s itime is = %u, als_polling_time = %u \n", __func__,
		ctx->alg_ctx.itime_ms, ctx->alg_ctx.als_polling_time);
	mod_timer(&chip->work_timer, jiffies + msecs_to_jiffies(ctx->alg_ctx.als_polling_time));
}

static void tcs3410_get_read_out_buffer(void)
{
	uint32_t i;
	uint32_t head_ptr_delta;
	u16 *temp_ptr = &ring_buffer[0];
	u16 *temp_head_ptr = head_ptr;

	head_ptr_delta = (uint32_t)((uintptr_t)temp_head_ptr - (uintptr_t)(&ring_buffer[0]));
	head_ptr_delta = head_ptr_delta / ASTEP_LEN;

	if (head_ptr_delta > g_max_buffer_size - 1)
		head_ptr_delta = g_max_buffer_size - 1;

	(void)memset_s(read_out_buffer, g_max_buffer_size * sizeof(*read_out_buffer),
		0, g_max_buffer_size * sizeof(*read_out_buffer));

	for (i = 0; i < (g_max_buffer_size - head_ptr_delta); i++) {
		read_out_buffer[i] = *temp_head_ptr;
		hwlog_debug("Read out_buffer[%u] = %u from ring_buffer[%u]\n",
			i, read_out_buffer[i], (temp_head_ptr - &ring_buffer[0]));
		temp_head_ptr++;
	}

	for (i = (g_max_buffer_size - head_ptr_delta); i < g_max_buffer_size; i++) {
		read_out_buffer[i] = *temp_ptr;
		hwlog_debug("Read out_buffer[%u] = %u from ring_buffer[%u]\n",
			i, read_out_buffer[i], (temp_ptr - &ring_buffer[0]));
		temp_ptr++;
	}

#ifdef EXTENDED_LOG
	for (i = 0; i < g_max_buffer_size; i++)
		hwlog_debug("FD_Buffer[%u],%u\n", i, read_out_buffer[i]);
#endif
}

void tcs3410_ring_buffer_process(uint16_t fifo_lvl, uint16_t *buf_16)
{
	UINT32 i;
	if (!buf_16) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	mutex_lock(&ring_buffer_mutex);
	for (i = 0; i < fifo_lvl; i++) {
		hwlog_debug("Writing data: buf_16[%u] = %u to ring buffer at position %u\n",
			i, buf_16[i], (buffer_ptr - &ring_buffer[0]));
		*buffer_ptr = buf_16[i];
		tail_ptr = buffer_ptr;
		buffer_ptr++;
		if (buffer_ptr == &ring_buffer[g_max_buffer_size]) {
			buffer_ptr = &ring_buffer[0];
			first_circle_end = 1;
		}
	}
	if ((tail_ptr == &ring_buffer[g_max_buffer_size - 1]) ||
		(first_circle_end == 0))
		head_ptr = &ring_buffer[0];

	if ((first_circle_end == 1) &&
		(tail_ptr != &ring_buffer[g_max_buffer_size - 1]))
		head_ptr = (tail_ptr + 1);

	mutex_unlock(&ring_buffer_mutex);
}

static void pull_fd_data_to_buffer(struct tcs3410_ctx *ctx, UINT16 *buf, UINT16 num)
{	
	UINT8 fd_enable_status;
	UINT16 i;
	UINT8 fd_gain = 0;

	color_sensor_get_byte(ctx->handle, TCS3410_ALS_STATUS2, &fd_gain);
	fd_gain = fd_gain & 0x0F;
	if (fd_gain >= GAIN_TBL_SIZE)
		return;
	fd_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if ((fd_enable_status & FDEN) == FDEN) {
		// normalization
		for (i = 0; i < num; i++)
			buf[i] = (UINT16)(((UINT32)(buf[i]) * GC_AGAIN * GAIN_RATIO) / gain_tbl[fd_gain]);
		tcs3410_ring_buffer_process(num, buf);
	}
}

static void reset_fd_buffer(struct tcs3410_ctx *ctx)
{
	// if fd_gain is adjusted  clear the ringbuffer and reset the related pointer.
	mutex_lock(&ring_buffer_mutex);
	(void)memset_s(ring_buffer, g_max_buffer_size * BUF_RATIO_8_16BIT, 0, g_max_buffer_size * BUF_RATIO_8_16BIT);
	// clear ring_buffer when close the fd
	buffer_ptr = &ring_buffer[0];
	head_ptr = &ring_buffer[0];
	tail_ptr = &ring_buffer[0];
	mutex_unlock(&ring_buffer_mutex);
}

static void handle_fifo_overflow_underflow(struct tcs3410_ctx *ctx, uint8_t fifo_overflow, uint8_t fifo_underflow)
{	
	hwlog_err("FIFO error!,overflow: %u, underflow: %u. Now clear the FIFO and the flicker data\n", 
		fifo_overflow, fifo_underflow);
	uint8_t temp_enable = 0;

	color_sensor_get_byte(ctx->handle, TCS3410_ENABLE, &temp_enable);
	tcs3410_set_field(ctx->handle, TCS3410_ENABLE, 0, TCS3410_MASK_AEN | TCS3410_MASK_FDEN);
	clear_fifo_data(ctx); // clear FIFO
	reset_fd_buffer(ctx);
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, temp_enable);
}

static void tcs3410_flicker_data_process(struct tcs3410_ctx *ctx, uint8_t *fifo_readdata, int readlength)
{
	ctx->fd.flicker_data_number = 0;
	for (int i = 0; i < readlength; i++) {
		if (!ctx->fd.flicker_data_l_filled) {
			ctx->fd.flicker_data_l = fifo_readdata[i];
			ctx->fd.flicker_data_l_filled = 1;
			continue;
		} else {
			ctx->fd.flicker_data_h = fifo_readdata[i];
			ctx->fd.flicker_data_l_filled = 0;
			ctx->fd.flicker_data[ctx->fd.flicker_data_number] = (ctx->fd.flicker_data_l |
				(ctx->fd.flicker_data_h << SHIFT_8)) >> SHIFT_4; // remove the 4 residual bits
			ctx->fd.flicker_data_number++;
			ctx->fd.flicker_data_l = 0;
			ctx->fd.flicker_data_h = 0;
		}
	}
	ctx->fd.flicker_data_number = (ctx->fd.flicker_data_number > FFT_BIN_COUNT) ?
		FFT_BIN_COUNT : ctx->fd.flicker_data_number;
	pull_fd_data_to_buffer(ctx, ctx->fd.flicker_data, ctx->fd.flicker_data_number);
	(void)memset_s(ctx->fd.flicker_data, sizeof(ctx->fd.flicker_data), 0, sizeof(ctx->fd.flicker_data));
}

static void handle_fifo_data(struct tcs3410_ctx *ctx)
{
	uint8_t fifo_readdata[MAX_DEVICE_FIFO_LEN] = {0};
	uint8_t fifo_overflow = 0;
	uint8_t fifo_underflow = 0;
	uint16_t readlength = 0;

	// read FIFO level
	color_sensor_read_fifo(ctx->handle, TCS3410_FIFO_LEVEL, fifo_readdata, READ_TWO_BITS);
	ctx->fifo_level = (uint16_t)((fifo_readdata[0] << TCS3410_SHIFT_FIFO_LEVEL) |
		(fifo_readdata[1] & TCS3410_MASK_STATUS0_FIFO_LVL_L));
	fifo_overflow = fifo_readdata[1] >> TCS3410_SHIFT_FIFO_STATUS0_FIFO_OVERFLOW;
	fifo_underflow = (fifo_readdata[1] & 0x40) >> TCS3410_SHIFT_FIFO_STATUS0_FIFO_UNDERFLOW;

	if (fifo_overflow || fifo_underflow) {
		handle_fifo_overflow_underflow(ctx, fifo_overflow, fifo_underflow);
		return;
	}
	ctx->fifo_level = (ctx->fifo_level > g_max_buffer_size) ? g_max_buffer_size : ctx->fifo_level;
	if (ctx->fifo_level % 2 != 0) {
		hwlog_debug("fifo level is : %u\n", ctx->fifo_level);
		if (ctx->fifo_level > 1)
			ctx->fifo_level--;
	}
	readlength = ctx->fifo_level;
	if (readlength == 0) {
		hwlog_info("fifo level is : %u\n", ctx->fifo_level);
		return;
	}

	color_sensor_read_fifo(ctx->handle, TCS3410_FIFO_DATA, fifo_readdata, readlength);
	tcs3410_flicker_data_process(ctx, fifo_readdata, readlength);
}

static void read_fd_data(struct color_chip *chip)
{
	struct tcs3410_ctx *ctx = NULL;
	uint8_t reg_status[STAT_END];
	uint8_t status;
	UINT8 rgb_enable_status;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s, pointer is null\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;

	mutex_lock(&ctx->lock);

	rgb_enable_status = get_rgb_fd_enable_status(ctx->handle); // 01000011
	if ((rgb_enable_status & FDEN) != FDEN && (fd_enable == 0)) {
		hwlog_info("%s: fd already disabled, del timer\n", __func__);
		mutex_unlock(&ctx->lock);
		return;
	}

	mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FD_POLLING_TIME));

	color_sensor_get_byte(ctx->handle, TCS3410_STATUS, &reg_status[STAT]);
	status = reg_status[STAT];
	if (!(status & TCS3410_MASK_FIEN)) {
		hwlog_info("%s tcs3410_handle_fd_data no fifo/sint interrupt, status == %u\n", __func__, status);
		mutex_unlock(&ctx->lock);
		return;
	} else {
		hwlog_debug("%s tcs3410_handle_fd_data with fifo interrupt status == %u\n", __func__, status);
		tcs3410_set_field(ctx->handle, TCS3410_STATUS, TCS3410_MASK_FIEN, TCS3410_MASK_FIEN);
		handle_fifo_data(ctx);
	}

	mutex_unlock(&ctx->lock);
}

static void tcs3410_fd_work(struct work_struct *work)
{
	struct color_chip *chip = NULL;

	if (!work) {
		hwlog_err("%s: Pointer work is NULL\n", __func__);
		return;
	}
	chip = container_of(work, struct color_chip, fd_work);
	if (!chip) {
		hwlog_err("%s: Pointer chip is NULL\n", __func__);
		return;
	}
	tcs3410_work_cnt[FLK_ID]++;
	ams_mutex_lock(&chip->lock);
	read_fd_data(chip);
	ams_mutex_unlock(&chip->lock);
}

static void tcs3410_get_fd_data(struct color_chip *chip, char *ams_fd_data)
{
	int ret = 0;
	if (!chip || !chip->device_ctx || !ams_fd_data) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return ;
	}
	mutex_lock(&ring_buffer_mutex);
	tcs3410_get_read_out_buffer();

	ret = memcpy_s(ams_fd_data, g_max_buffer_size * sizeof(*read_out_buffer),
		read_out_buffer, g_max_buffer_size * sizeof(*read_out_buffer));
	if (ret != 0) return;
	mutex_unlock(&ring_buffer_mutex);
}

static void tcs3410_store_enable(struct color_chip *chip, int enable)
{
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	color_calibrate_result = false;
	chip->in_cal_mode = false;
	if (enable)
		tcs3410_als_set_enable(chip, AMSDRIVER_ALS_ENABLE);
	else
		tcs3410_als_set_enable(chip, AMSDRIVER_ALS_DISABLE);
}

static char *tcs3410_chip_name(void)
{
	return "ams_tcs3410";
}

void tcs3410_show_calibrate(struct color_chip *chip, struct at_color_sensor_output_t *out)
{
	UINT32 i;
	struct tcs3410_ctx *ctx = NULL;

	if (!out || !chip || !chip->device_ctx) {
		hwlog_err("%s input para NULL\n", __func__);
		return;
	}

	ctx = chip->device_ctx;
	if (chip->in_cal_mode == false)
		hwlog_err("%s not in cali mode\n", __func__);
	out->gain_arr = CAL_STATE_GAIN_LAST;
	out->color_arr = TCS3410_REPORT_DATA_LEN;
	out->result = (UINT32)report_calibrate_result;
	for (i = 0; i < TCS3410_REPORT_DATA_LEN; i++){
		out->cali_rst[i][0] = ctx->cal_ctx.ratio[i];
	}
	hwlog_info("%s : cali= %u, %u, %u, %u, rst = %u\n", __func__, 
		out->cali_rst[0][0], out->cali_rst[1][0], out->cali_rst[2][0], out->cali_rst[3][0], out->result);
}

void tcs3410_store_calibrate(struct color_chip *chip, struct at_color_sensor_input_t *in)
{
	struct tcs3410_ctx *ctx = NULL;
	int ret = 0;

	if (!chip || !chip->device_ctx || !in) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	report_calibrate_result = false;

	hwlog_info("%s get tar_clear_f1_f2_f3 = %u, %u, %u, %u\n", __func__, in->reserverd[0], in->reserverd[1],
		in->reserverd[2], in->reserverd[3]);

	ctx = chip->device_ctx;
	hwlog_info("%s start calibration mode\n", __func__);
	chip->cali_ctx.cal_raw_count = 0;

	// copy target value
	ret = memcpy_s(ctx->cal_ctx.target, TCS3410_REPORT_DATA_LEN * sizeof(UINT32), in->reserverd,
		TCS3410_REPORT_DATA_LEN * sizeof(UINT32));
	if (ret != 0)
		return;
	chip->in_cal_mode = true;
	// make the calibrate_result true, it will be reset on any error
	color_calibrate_result = false;
}

static int tcs3410_rgb_report_type(void)
{
	return AWB_SENSOR_RAW_SEQ_TYPE_TCS3410;
}

static char *tcs3410_algo_type(void)
{
	return "tcs3410_hal";
}

int tcs3410_enable_rgb(bool enable)
{
	struct color_chip *chip = p_chip;
	als_enabled = enable;

	hwlog_info("%s: enter\n", __func__);

	if (!chip) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return 0;
	}

	if (enable)
		tcs3410_als_set_enable(chip, AMSDRIVER_ALS_ENABLE);
	else
		tcs3410_als_set_enable(chip, AMSDRIVER_ALS_DISABLE);
	if (!als_enabled && fd_enable)
		fd_enable_set(chip, AMSDRIVER_FD_ENABLE);

	return 1;
}

EXPORT_SYMBOL_GPL(tcs3410_enable_rgb);

static void configure_functions(struct color_chip *chip)
{
	chip->get_flicker_data = tcs3410_get_fd_data;
	chip->flicker_enable_show_state = tcs3410_fd_show_enable;
	chip->flicker_enable_store_state = tcs3410_fd_store_enable;
	chip->color_chip_name = tcs3410_chip_name;

	chip->color_enable_show_state = tcs3410_show_enable;
	chip->color_enable_store_state = tcs3410_store_enable;

	chip->at_color_show_calibrate_state = tcs3410_show_calibrate;
	chip->at_color_store_calibrate_state = tcs3410_store_calibrate;
	chip->color_report_type = tcs3410_rgb_report_type;
	chip->color_algo_type = tcs3410_algo_type;
	color_default_enable = tcs3410_enable_rgb;
}

static void tcs3410_flc_cali_timer_wrk(struct timer_list *data)
{
	struct color_chip *chip = from_timer(chip, data, fd_cali_timer);

	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	queue_work(system_power_efficient_wq, &chip->fd_cali_work);
}

static void tcs3410_cali_work(struct work_struct *work)
{
	struct color_chip *chip = NULL;

	if (!work) {
		hwlog_err("%s: Pointer work is NULL\n", __func__);
		return;
	}
	chip = container_of(work, struct color_chip, fd_cali_work);
	if (!chip) {
		hwlog_err("%s: Pointer chip is NULL\n", __func__);
		return;
	}
	mutex_lock(&chip->lock);
	tcs3410_time_base_correction(chip->device_ctx, chip);
	mutex_unlock(&chip->lock);
}

static void init_library(struct color_chip *chip)
{
	int ret;

	timer_setup(&chip->work_timer, tcs3410_als_timer_wrk, 0);
	INIT_WORK(&chip->als_work, tcs3410_als_work);

	timer_setup(&chip->fd_timer, tcs3410_flc_timer_wrk, 0);
	INIT_WORK(&chip->fd_work, tcs3410_fd_work);

	timer_setup(&chip->fd_cali_timer, tcs3410_flc_cali_timer_wrk, 0);
	INIT_WORK(&chip->fd_cali_work, tcs3410_cali_work);
	mod_timer(&chip->fd_cali_timer, jiffies + msecs_to_jiffies(FLK_CALI_POLLING_TIME));

	configure_functions(chip);

	p_chip = chip;
	ret = color_register(chip);
	if (ret < 0)
		hwlog_err("%s color_register fail\n", __func__);

	hwlog_info("rgb sensor %s ok\n", __func__);
}

static void set_flicker_num_samples(struct tcs3410_ctx *ctx)
{
	uint16_t num_samples = ctx->fd_nr_samples;

	hwlog_info("set_flicker_num_samples, num_samples = %u\n", num_samples);

	// set the low order bits
	color_sensor_set_byte(ctx->handle, TCS3410_FD_NR_SAMPLES0, (uint8_t)((uint16_t)num_samples & NUM_SAMPLES_LOW_MASK));

	// set the high order bits, bits 10:8 of fd_nr_samples mapped to -> 2:0
	tcs3410_set_field(ctx->handle, TCS3410_FD_NR_SAMPLES1,
		(uint8_t)(((uint16_t)num_samples & NUM_SAMPLES_HIGH_MASK) >> SHIFT_8), TCS3410_MASK_FD_NR_SAMPLES_HI);
}

static void tcs3410_update_als_gain(struct tcs3410_ctx *ctx)
{
	uint8_t modulator0_gain = ctx->als.pd_gain[MODULATOR_0];
	uint8_t modulator1_gain = ctx->als.pd_gain[MODULATOR_1];
	uint8_t modulator2_gain = ctx->als.pd_gain[MODULATOR_2];

	hwlog_debug("tcs3410_update_als_gain modulator0_gain = %u, modulator1_gain = %u, modulator2_gain = %u \n",
		modulator0_gain, modulator1_gain, modulator2_gain);

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_L,
		((modulator1_gain << TCS3410_SHIFT_GAIN_MOD1) | (modulator0_gain << TCS3410_SHIFT_GAIN_MOD0)));
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_H,
		(modulator2_gain << TCS3410_SHIFT_GAIN_MOD2));

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_L,
		((modulator1_gain << TCS3410_SHIFT_GAIN_MOD1) |
		(modulator0_gain << TCS3410_SHIFT_GAIN_MOD0)));
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_H, (modulator2_gain << TCS3410_SHIFT_GAIN_MOD2));
}

static void set_fifo_threshold(struct tcs3410_ctx *ctx)
{
	uint16_t threshold = ctx->fifo_threshold;

	hwlog_info("%s: threshold = %u\n", __func__, threshold);

	// set the high order bits
	color_sensor_set_byte(ctx->handle, TCS3410_FIFO_THR, (uint8_t)((threshold & 0x01FF) >> 1));

	// set the low order bit
	tcs3410_set_field(ctx->handle, TCS3410_CFG2, (uint8_t)(threshold & 0x0001), TCS3410_MASK_CFG2_FIFO_THRESH_L);
}

static void tcs3410_configure_fifo(struct tcs3410_ctx *ctx)
{
	uint8_t mask;
	uint8_t value = 0;

	set_fifo_threshold(ctx);

	// Enable end markers
	mask = (TCS3410_FD_END_MARKER_WRITE_EN | TCS3410_FD_CHKSUM_MARKER_WRITE_EN | TCS3410_FD_GAIN_WRITE_EN);
	tcs3410_set_field(ctx->handle, TCS3410_MEAS_MODE1, value, mask);

	// fifo compression configuration
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG0, 0x0F);  // 0x0F == no compression
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG1, 0x00);
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG2, 0x00);

	hwlog_info("%s: done\n", __func__);
}

static void tcs3410_set_gain(struct tcs3410_ctx *ctx)
{
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_L,
		((TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD1) | (TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD0)));
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_H,
		(TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD2));

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_L,
		((TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD1) | (TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD0)));
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_H,
		(TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD2));
}

/*
 *  The sequencer has 4 steps. Sequence steps are performed
 *  in order beginning with step 0. A sequence round is
 *  step0, step1, step2, step3.
 *
 *  Only steps 0 and 1 are used in this driver.
 *  ALS data is collected in step 0.
 *  Flicker data is collected in step 1.
 *
 * Photodiode Filter Assignment
 * pd0 = F   -- M0  step0      -- M1  step1
 * pd1 = C   -- M2  step0
 * pd2 = F   -- M0  step0      -- M1  step1
 * pd3 = G                     -- M1  step1
 * pd4 = B                     -- M2  step1
 * pd5 = R   -- M1  step0
 */

static void tcs3410_smux_init(struct tcs3410_ctx *ctx)
{
	// Step 0
	// pd0->M0, pd1->M2, pd2->M0, pd3->M2
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_PHDX_SMUX_L, 0x04);
	// pd5->M1
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_PHDX_SMUX_H, 0x0E);

	// Step 1
	// pd0->M1, pd2->M1, pd3->M1
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_L, 0xC4);
	// pd4->M2
	tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_H, 0x02, MOD_PHD_MASK);

	// Step 2
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_L, 0);
	tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_H, 0, MOD_PHD_MASK);

	// Step 3
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP3_MOD_PHDX_SMUX_L, 0);
	tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP3_MOD_PHDX_SMUX_H, 0, MOD_PHD_MASK);

	// Do not write ALS data to fifo
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG0, MOD_PHD_MASK);
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG1, MOD_PHD_MASK);
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG2, MOD_PHD_MASK);
}

static void tcs3410_sequencer_init(struct tcs3410_ctx *ctx)
{
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_FD_0, SEQUENCER_INIT_VALUE);
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_ALS_FD_1, SEQUENCER_INIT_VALUE);
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_APERS_AND_VSYNC_WAIT, SEQUENCER_INIT_VALUE);
}

static void tcs3410_set_sample_time(struct tcs3410_ctx *ctx, uint16_t time)
{
	color_sensor_set_byte(ctx->handle, TCS3410_SAMPLE_TIME0, (uint8_t)((uint16_t)time & SAMPLE_TIME_LOW_MASK));
	color_sensor_set_byte(ctx->handle, TCS3410_SAMPLE_TIME1,
		(uint8_t)(((uint16_t)time & SAMPLE_TIME_HIGH_MASK) >> SHIFT_8));
}

static void tcs3410_set_wait_time(struct tcs3410_ctx *ctx, uint8_t time)
{
	color_sensor_set_byte(ctx->handle, TCS3410_WTIME, time);
}

static void tcs3410_set_agc_enable(struct tcs3410_ctx *ctx, uint8_t on_off)
{
	// Enable / Disable AGC
	uint8_t value;

	if (on_off)
		value = TCS3410_MASK_MOD_CALIB_NTH_ITERATION_AGC_ENABLE;
	else
		value = 0;

	tcs3410_set_field(ctx->handle, TCS3410_MOD_CALIB_CFG2, value, TCS3410_MASK_MOD_CALIB_NTH_ITERATION_AGC_ENABLE);
}

static void tcs3410_set_agc_mode(struct tcs3410_ctx *ctx, uint8_t mode)
{
	uint8_t on_off;

	hwlog_info("%s: mode = %u\n", __func__, mode);

	switch (mode) {
		case 1:
			// Turn on Saturation AGC
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_H,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN);
			// Turn off Predict AGC
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_H, 0x00,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN);
			on_off = 1;
			break;

		case 2:
			// Turn on Predict AGC
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_H,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN);
			// Turn off Saturation AGC
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_H, 0x00,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN);
			on_off = 1;
			break;

		case 3:
			// Turn on both Saturation AGC and Predict AGC
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_H,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN);
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_H,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN);
			on_off = 1;
			break;

		case 0:
		default:
			// Disable all AGC
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_H, 0x00,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN);
			tcs3410_set_field(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_H, 0x00,
				TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN);
			on_off = 0;
			break;
	}
	// Set double step down for ASAT AGC
	tcs3410_set_field(ctx->handle, TCS3410_MEAS_MODE0, TCS3410_MEAS_MODE0_MASK_ENABLE_AGC_ASAT_DOUBLE_STEP_DOWN,
		TCS3410_MEAS_MODE0_MASK_ENABLE_AGC_ASAT_DOUBLE_STEP_DOWN);

	// Set for every iteration
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_CALIB_CFG0, 0x01);

	tcs3410_set_agc_enable(ctx, on_off);
}

static void tcs3410_set_als_thresh(struct tcs3410_ctx *ctx, uint32_t low, uint32_t high)
{
	color_sensor_set_byte(ctx->handle, TCS3410_AILT0, (uint8_t)(low & ALS_THRESH_MASK_0));
	color_sensor_set_byte(ctx->handle, TCS3410_AILT1, (uint8_t)(low & ALS_THRESH_MASK_1) >> SHIFT_8);
	color_sensor_set_byte(ctx->handle, TCS3410_AILT2, (uint8_t)(low & ALS_THRESH_MASK_2) >> SHIFT_8);
	color_sensor_set_byte(ctx->handle, TCS3410_AIHT0, (uint8_t)(high & ALS_THRESH_MASK_0));
	color_sensor_set_byte(ctx->handle, TCS3410_AIHT1, (uint8_t)(high & ALS_THRESH_MASK_1) >> SHIFT_8);
	color_sensor_set_byte(ctx->handle, TCS3410_AIHT2, (uint8_t)(high & ALS_THRESH_MASK_2) >> SHIFT_8);
}

static void tcs3410_set_als_persistence(struct tcs3410_ctx *ctx, uint8_t persistence)
{
	tcs3410_set_field(ctx->handle, TCS3410_CFG5, persistence, TCS3410_MASK_APERS);
}

static void tcs3410_set_als_num_samples(struct tcs3410_ctx *ctx, uint16_t num_samples)
{
	color_sensor_set_byte(ctx->handle, TCS3410_ALS_NR_SAMPLES0,
		(uint8_t)((uint16_t)num_samples & NUM_SAMPLES_LOW_MASK));
	color_sensor_set_byte(ctx->handle, TCS3410_ALS_NR_SAMPLES1,
		(uint8_t)(((uint16_t)num_samples & NUM_SAMPLES_HIGH_MASK) >> SHIFT_8));
}

static void tcs3410_set_agc_num_samples(struct tcs3410_ctx *ctx, UINT16 num_samples)
{
	color_sensor_set_byte(ctx->handle, TCS3410_AGC_NR_SAMPLES_LO,
		(uint8_t)((UINT16)num_samples & NUM_SAMPLES_LOW_MASK));
	color_sensor_set_byte(ctx->handle, TCS3410_AGC_NR_SAMPLES_HI,
		(uint8_t)(((UINT16)num_samples & NUM_SAMPLES_HIGH_MASK) >> SHIFT_8));
}

static void tcs3410_set_als_quik_config(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	uint8_t tmp_enable = 0;

	ctx->alg_ctx.step_status = IDLE_STEP;
	color_sensor_get_byte(ctx->handle, TCS3410_ENABLE, &tmp_enable);
	tcs3410_set_field(ctx->handle, TCS3410_ENABLE, 0, TCS3410_MASK_AEN | TCS3410_MASK_FDEN);
	tcs3410_set_gain(ctx);
	ctx->alg_ctx.is_zoom_data = true;
	tcs3410_set_als_num_samples(ctx, ALS_QUICK_SAMPLE_TIME);
	ctx->als_nr_samples = ALS_QUICK_SAMPLE_TIME;
	ctx->fd_nr_samples = FD_QUICK_NR_SAMPLES;
	set_flicker_num_samples(ctx);
	hwlog_info("%s fd_enable = %u, als_first_report = %u \n", __func__, fd_enable, ctx->als_first_report);
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, tmp_enable);
	tcs3410_set_atime(ctx);
	hwlog_info("%s tmp_enable = %d fd_enable= %d, als_first_report = %u, is_zoom_data = %u", __func__,
		tmp_enable, fd_enable, ctx->als_first_report, (int)ctx->alg_ctx.is_zoom_data);
}

static void tcs3410_set_als_normal_config(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	uint8_t tmp_enable = 0;

	ctx->alg_ctx.step_status = IDLE_STEP;
	color_sensor_get_byte(ctx->handle, TCS3410_ENABLE, &tmp_enable);
	tcs3410_set_field(ctx->handle, TCS3410_ENABLE, 0, TCS3410_MASK_AEN | TCS3410_MASK_FDEN);
	ctx->alg_ctx.is_zoom_data = false;
	if (chip->support_flk_2k) {
		ctx->als_nr_samples = ALS_NR_SAMPLES_2K;
		tcs3410_set_als_num_samples(ctx, ALS_NR_SAMPLES_2K);
		ctx->fd_nr_samples = FD_NR_SAMPLES_2K;
	} else {
		ctx->als_nr_samples = ALS_NR_SAMPLES_1K;
		tcs3410_set_als_num_samples(ctx, ALS_NR_SAMPLES_1K);
		ctx->fd_nr_samples = FD_NR_SAMPLES_1K;
	}
	set_flicker_num_samples(ctx);
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, tmp_enable);
	hwlog_debug("%s tmp_enable = %d fd_enable= %d, als_first_report = %u", __func__,
		tmp_enable, fd_enable, ctx->als_first_report);
	tcs3410_set_atime(ctx);
	ctx->alg_ctx.als_polling_cnt--;
}

static void tcs3410_set_als_polling_time(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	if (ctx->alg_ctx.als_polling_cnt == ALS_POLLING_CNT)
		tcs3410_set_als_quik_config(ctx, chip);
	if (ctx->alg_ctx.als_polling_cnt == 0)
		tcs3410_set_als_normal_config(ctx, chip);
	if (ctx->alg_ctx.als_polling_cnt > 0) {
		ctx->alg_ctx.als_polling_cnt--;
		ctx->alg_ctx.als_polling_time = ALS_FAST_POLLING_TIME;
	} else if (ctx->alg_ctx.als_polling_cnt == NORMAL_MODE) {
		ctx->alg_ctx.als_polling_time = ALS_NORMAL_POLLING_TIME;
	}
}

static void tcs3410_power_device(struct tcs3410_ctx *ctx, uint8_t power)
{
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, 0);
	if (power) {
		tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_ON, PON);
	} else {
		tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_OFF, PON);
	}
}

static int tcs3410_reg_init(struct tcs3410_ctx *ctx)
{
	tcs3410_power_device(ctx, TCS3410_DEVICE_ON);
	tcs3410_smux_init(ctx);
	tcs3410_sequencer_init(ctx);

	color_sensor_set_byte(ctx->handle, TCS3410_MOD_COMP_CFG1,
		(TCS3410_IDAC_9UV << TCS3410_SHIFT_MOD_COMP_CFG1_IDAC_RANGE));
	tcs3410_set_gain(ctx);

	tcs3410_set_sample_time(ctx, ctx->sample_time);
	tcs3410_set_wait_time(ctx, ctx->wait_time);
	tcs3410_set_agc_num_samples(ctx, ctx->agc_num_samples);
	tcs3410_set_agc_mode(ctx, ctx->agc_mode);

	// config als reg
	tcs3410_set_als_thresh(ctx, TCS3410_ALS_LOW_THRESH, TCS3410_ALS_HIGH_THRESH);
	tcs3410_set_als_persistence(ctx, ctx->persist);
	tcs3410_set_als_num_samples(ctx, ctx->als_nr_samples);

	tcs3410_update_als_gain(ctx);

	// set residual configuration - disable residual
	color_sensor_set_byte(ctx->handle, TCS3410_CFG6, 0x00);
	color_sensor_set_byte(ctx->handle, TCS3410_MOD_CALIB_CFG2, 0x00);
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_RESIDUAL_0, 0x00);
	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_RESIDUAL_1_AND_WAIT, 0x00);

	// FD FIFO residual bits ignored. 0 means 4-0=4 res bits in FD FIFO
	color_sensor_set_byte(ctx->handle, TCS3410_CFG9, 0x00);

	tcs3410_set_atime(ctx);

	// Do not write als data to fifo
	tcs3410_set_field(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG0, 0, TCS3410_REG_MOD_FIFO_DATA_WRITE_ENABLE_MASK);

	tcs3410_set_field(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG1, 0, TCS3410_REG_MOD_FIFO_DATA_WRITE_ENABLE_MASK);

	tcs3410_set_field(ctx->handle, TCS3410_MOD_FIFO_DATA_CFG2, 0, TCS3410_REG_MOD_FIFO_DATA_WRITE_ENABLE_MASK);

	set_flicker_num_samples(ctx);
	clear_fifo_data(ctx);
	tcs3410_configure_fifo(ctx);

	hwlog_info("tcs3410_init done \r\n");
	return 0;
}

static void tcs3410_get_dts_parameter(const struct device *dev, struct color_chip *chip)
{
	int ret;

	ret = of_property_read_u32(dev->of_node, "flk_data_len", &chip->flk_data_len);
	if (ret < 0) {
		hwlog_info("%s, get flk_data_len failed use default\n", __func__);
		chip->flk_data_len = DEFAULT_FLK_DATA_LEN; // default support flk
	}

	ret = of_property_read_u32(dev->of_node, "support_flk_size_adj", &chip->support_flk_size_adj);
	if (ret < 0) {
		hwlog_info("%s, get support_flk_size_adj failed\n", __func__);
		chip->support_flk_size_adj = 0;
	}

	ret = of_property_read_u32(dev->of_node, "support_flk_2k", &chip->support_flk_2k);
	if (ret < 0) {
		hwlog_info("%s, get support_flk_2k failed\n", __func__);
		chip->support_flk_2k = 0;
	}

	hwlog_info("%s, flk_data_len = %d support_flk_size_adj = %d chip->support_flk_2k = %d\n", __func__,
		chip->flk_data_len, chip->support_flk_size_adj, chip->support_flk_2k);
}

static void init_buffers(u16 buffer_size) {
	ring_buffer = (u16*)kzalloc(buffer_size * sizeof(u16), GFP_KERNEL);
	read_out_buffer = (u32*)kzalloc(buffer_size * sizeof(u32), GFP_KERNEL);
	if (!ring_buffer || !read_out_buffer) {
		hwlog_info("%s kzalloc fail", __func__);
		if (ring_buffer)
			kfree(ring_buffer);
		if (read_out_buffer)
			kfree(read_out_buffer);
		return;
	}
	hwlog_info("%s kzalloc succ", __func__);
	buffer_ptr = &ring_buffer[0];
	head_ptr = &ring_buffer[0];
	tail_ptr = &ring_buffer[0];
}

static void tcs3410_init_parameter(struct tcs3410_ctx *ctx, struct color_chip *chip)
{
	if (chip->support_flk_2k) {
		ctx->sample_time = TCS3410_SAMPLE_TIME_2K;
		ctx->fd_nr_samples = FD_NR_SAMPLES_2K;
		ctx->als_nr_samples = ALS_NR_SAMPLES_2K;
		g_max_buffer_size = MAX_BUFFER_SIZE_2K;
	} else {
		ctx->sample_time = TCS3410_SAMPLE_TIME_1K;
		ctx->fd_nr_samples = FD_NR_SAMPLES_1K;
		ctx->als_nr_samples = ALS_NR_SAMPLES_1K;
		g_max_buffer_size = MAX_BUFFER_SIZE_1K;
	}
	init_buffers(g_max_buffer_size);
	ctx->wait_time = TCS3410_WAIT_TIME;
	ctx->agc_mode = TCS3410_AGC_MODE;
	ctx->agc_num_samples = AGC_NUM_SAMPLES;
	ctx->fifo_threshold = FIFO_THRESHOLD;
	ctx->persist = TCS3410_PERSIST;

	ctx->als.pd_gain[MODULATOR_0] = PD_GAIN_DEFAULT;
	ctx->als.pd_gain[MODULATOR_1] = PD_GAIN_DEFAULT;
	ctx->als.pd_gain[MODULATOR_2] = PD_GAIN_DEFAULT;
}

int tcs3410_probe(struct i2c_client *client, const struct i2c_device_id *idp)
{
	hwlog_info("%s enter! \n", __func__);
	int ret = -1;
	struct device *dev = NULL;
	static struct color_chip *chip = NULL;
	struct tcs3410_ctx *ctx = NULL;

	if (!client)
		return -EFAULT;

	color_notify_support();

	dev = &client->dev;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		goto init_failed;

	chip = kzalloc(sizeof(struct color_chip), GFP_KERNEL);
	if (!chip)
		goto malloc_failed;

	mutex_init(&chip->lock);
	chip->client = client;
	chip->pdata = dev->platform_data;
	i2c_set_clientdata(chip->client, chip);

	chip->in_cal_mode = false;
	chip->cali_ctx.cal_state = 0;
	
	if (tcs3410_get_chipid(chip->client) < 0) {
		hwlog_info("%s failed: AMS_UNKNOWN_DEVICE \n", __func__);
		goto id_failed;
	}
	flicker_support = 1 ;
	chip->device_ctx = kzalloc(sizeof(struct tcs3410_ctx), GFP_KERNEL);
	if (!chip->device_ctx)
		goto id_failed;

	ctx = chip->device_ctx;
	ctx->handle = chip->client;
	mutex_init(&(ctx->lock));
	tcs3410_get_dts_parameter(dev, chip);
	tcs3410_init_parameter(ctx, chip);
	tcs3410_reg_init(ctx);
	init_library(chip);
	return 0;

id_failed:
	if (chip->device_ctx)
		kfree(chip->device_ctx);
	i2c_set_clientdata(client, NULL);
	hwlog_err("%s id_failed\n", __func__);
malloc_failed:
	if (chip)
		kfree(chip);
init_failed:
	hwlog_err("%s Probe failed\n", __func__);
	color_notify_absent();
	return ret;
}

int tcs3410_suspend(struct device *dev)
{
	struct color_chip *chip = NULL;

	if (!dev) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}
	chip = dev_get_drvdata(dev);
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}

	hwlog_info("%s\n", __func__);
	return 0;
}

int tcs3410_resume(struct device *dev)
{
	struct color_chip *chip = NULL;

	if (!dev) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}
	chip = dev_get_drvdata(dev);
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}
	hwlog_info("%s\n", __func__);
	return 0;
}

int ams_tcs3410_remove(struct i2c_client *client)
{
	struct color_chip *chip = NULL;

	if (!client) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -1;
	}
	chip = i2c_get_clientdata(client);
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -1;
	}

	free_irq(client->irq, chip);
	i2c_set_clientdata(client, NULL);
	kfree(chip->device_ctx);
	kfree(chip);
	kfree(ring_buffer);
	kfree(read_out_buffer);
	return 0;
}

static struct i2c_device_id amsdriver_idtable[] = {
	{ "ams_tcs3410", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, amsdriver_idtable);

static const struct dev_pm_ops ams_tcs3410_pm_ops = {
	.suspend = tcs3410_suspend,
	.resume = tcs3410_resume,
};

static const struct of_device_id amsdriver_of_id_table[] = {
	{ .compatible = "ams,tcs3410" },
	{},
};

static struct i2c_driver ams_tcs3410_driver = {
	.driver = {
		.name = "ams_tcs3410",
		.owner = THIS_MODULE,
		.of_match_table = amsdriver_of_id_table,
	},
	.id_table = amsdriver_idtable,
	.probe = tcs3410_probe,
	.remove = ams_tcs3410_remove,
};

static int __init tcs3410_init(void)
{
	int rc;
	hwlog_info("%s\n", __func__);
	rc = i2c_add_driver(&ams_tcs3410_driver);
	hwlog_info("%s %d\n", __func__, rc);
	return rc;
}

static void __exit tcs3410_exit(void)
{
	hwlog_info("%s\n", __func__);
	i2c_del_driver(&ams_tcs3410_driver);
}

module_init(tcs3410_init);
module_exit(tcs3410_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("fd_sensor driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
