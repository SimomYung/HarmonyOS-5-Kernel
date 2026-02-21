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
static u16 ring_buffer[MAX_BUFFER_SIZE];
static u32 read_out_buffer[MAX_BUFFER_SIZE];
static u16 *buffer_ptr = &ring_buffer[0];
static u16 *head_ptr = &ring_buffer[0];
static u16 *tail_ptr = &ring_buffer[0];
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
static int tcs3410_work_cnt[MAX_SENSOR_ID] = {0};

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

#ifdef ENABLE_DUMP_REGS
static void tcs3410_dump_reg(struct tcs3410_ctx *ctx)
{
	uint8_t i = 0;
	uint8_t value = 0;
	int ret = 0;

	for (i = 0x80; i < 0xff; i++) {
		value = 0xff;
		ret = color_sensor_get_byte(ctx->handle, i, &value);
		hwlog_info("%s : [0x%02x] : 0x%02x, rc : %d\n", __func__, i, value, ret);
	}
	ret = color_sensor_get_byte(ctx->handle, 0xff, &value);
	hwlog_info("%s : [0x%02x] : 0x%02x, rc : %d\n", __func__, 0xff, value, ret);
}
#endif

static int tcs3410_fifo_reset(struct tcs3410_ctx *ctx)
{
	uint8_t i2c_data[READ_TWO_BITS];
	uint8_t ret_val = 0;
	uint16_t level = 0;

	// Make sure fifo data is cleared  and confirm the fifo level is 0
	// Clear the FIFO
	tcs3410_set_field(ctx->handle, TCS3410_CONTROL, TCS3410_MASK_FIFO_CLR, TCS3410_MASK_FIFO_CLR);

	// confirm the fifo level is 0
	color_sensor_read_fifo(ctx->handle, TCS3410_FIFO_LEVEL, i2c_data, READ_TWO_BITS);

	level = (uint16_t)((i2c_data[0] << TCS3410_SHIFT_FIFO_LEVEL) | (i2c_data[1] & TCS3410_MASK_STATUS0_FIFO_LVL_L));
	hwlog_debug("FIFO Reset Complete:Level:  0x%02X\n", level);

	if (level != 0) {
		ret_val = -1;
		hwlog_info("FIFO level is not 0 after reset, FIFO Cleared but FIFO_LVL != 0\n");
	} else {
		hwlog_info("FIFO Reset Complete\n");
	}
	return ret_val;
}

static void clear_fifo_data(struct tcs3410_ctx *ctx)
{
	// Clear the flicket processing data.
	(void)memset_s(&ctx->fd, sizeof(ctx->fd), 0, sizeof(ctx->fd));
	tcs3410_fifo_reset(ctx);
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
			hwlog_info("%d: Chip is TCS3410\n", deviceId);
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
	hwlog_info("%s: als_nr_samples = %d, sample_time = %d, itime_ms = %d \n", __func__, ctx->als_nr_samples,
		ctx->sample_time, ctx->alg_ctx.itime_ms);
}

static void tcs3410_get_saturation(struct tcs3410_ctx *ctx)
{
	uint32_t atime_us = 0;

	atime_us = (ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * MOD_CLOCK / TCS3410_DEFAULT_MODULATOR_PERIOD_TO_MS;

	ctx->als.saturation_high = (uint16_t)((ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * 8 / 10);
	ctx->als.saturation_low = (uint16_t)((ctx->als_nr_samples + 1) * (ctx->sample_time + 1) * 2 / 10);

	hwlog_debug("%s : als_nr_samples = %d, sample_time = %d, saturation_high = %d, saturation_low = %d\n",
		__func__, ctx->als_nr_samples, ctx->sample_time, ctx->als.saturation_high, ctx->als.saturation_low);
}
static void tcs3410_set_als_gain(struct tcs3410_ctx *ctx)
{
	uint8_t step = 0;
	uint8_t modulator0_gain = ctx->als.pd_gain[MODULATOR_0];
	uint8_t modulator1_gain = ctx->als.pd_gain[MODULATOR_1];
	uint8_t modulator2_gain = ctx->als.pd_gain[MODULATOR_2];

	hwlog_debug("%s : modulator0_gain = %d, modulator1_gain = %d,  modulator2_gain = %d \n", __func__, modulator0_gain,
		modulator1_gain, modulator2_gain);

	step = (ctx->reg_status[ALS_STAT] & TCS3410_MASK_ALS_STATUS_MEAS_SEQR_STEP) >>
			TCS3410_SHIFT_ALS_STATUS_MEAS_SEQR_STEP;
	if (step == TCS3410_MEAS_STEP_0) {
		color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_L,
			((modulator1_gain << TCS3410_SHIFT_GAIN_MOD1) | (modulator0_gain << TCS3410_SHIFT_GAIN_MOD0)));
			
		color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_H,
			(modulator2_gain << TCS3410_SHIFT_GAIN_MOD2));
	}

	if (step == TCS3410_MEAS_STEP_1) {
		color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_L,
			((modulator1_gain << TCS3410_SHIFT_GAIN_MOD1) | (modulator0_gain << TCS3410_SHIFT_GAIN_MOD0)));

		color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_H,
			(modulator2_gain << TCS3410_SHIFT_GAIN_MOD2));
	}
}

static bool tcs3410_als_auto_gain_validate(struct tcs3410_ctx *ctx, tcs3410_modulator_t type)
{
	bool changed = false;

	ctx->als.pd_gain[type] = ctx->gain_reg[type];

	hwlog_debug("%s: raw_data = %d, [type] = %d, gain = %d, " "saturation_high = %d, saturation_low = %d. \n", __func__,
		ctx->als.raw_pds[type], type, ctx->als.pd_gain[type], ctx->als.saturation_high, ctx->als.saturation_low);

	// raw data is forced to 0xfffe when digital saturtion and 0xffff when analog saturation
	if ((ctx->als.raw_pds[type] >= ctx->als.saturation_high) ||
		(ctx->reg_status[ALS_STAT] & tcs3410_als_chan_asat_mask[type])) {
		hwlog_debug("%s : too high\n", __func__);
		if (ctx->als.pd_gain[type] > MIN_ALS_GAIN_LEVEL) {
			ctx->als.pd_gain[type] = ctx->als.pd_gain[type] - 1;
			changed = true;
			hwlog_debug("%s : decrease gain to %d, type : %d\n", __func__, ctx->als.pd_gain[type], type);
		}
	} else if (ctx->als.raw_pds[type] < ctx->als.saturation_low || ctx->als.raw_pds[type] < 0xff) {
		// Set ALS gain to the next bigger gain
		// If the clear channel raw count is less than 20% of the max raw count, increase the ALS gain.
		hwlog_debug("%s : too low\n", __func__);
		if (ctx->als.pd_gain[type] < MAX_ALS_GAIN_LEVEL) {
			ctx->als.pd_gain[type] = ctx->als.pd_gain[type] + 1;
			changed = true;
			hwlog_debug("%s : increase gain to %d, type : %d\n", __func__, ctx->als.pd_gain[type], type);
		}
	}
	return changed;
}

static bool tcs3410_need_change_gain(struct tcs3410_ctx *ctx)
{
	bool changed[NUM_MODULATORS] = {false, false, false};
	unsigned int idx;

	tcs3410_get_saturation(ctx);

	for (idx = 0; idx < NUM_MODULATORS; idx++) {
		if (tcs3410_als_auto_gain_validate(ctx, idx))
			changed[idx] = true;
	}

	if (changed[0] || changed[1] || changed[2]) {
		hwlog_debug("%s : changed status = %u, %u, %u\n", __func__, changed[0], changed[1], changed[2]);
		tcs3410_set_als_gain(ctx); // set gain
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
	UINT32 i;
	struct tcs3410_ctx *ctx = NULL;
	UINT32 autozero_offset = 10;
	uint8_t enable_sta = 0;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;
	ctx->alg_ctx.enabled = en;
	hwlog_info("%s: tcs3410_work_cnt = %d, %d\n", __func__, tcs3410_work_cnt[FLK_ID], tcs3410_work_cnt[RGB_ID]);
	if (en) {
		tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_MASK_PON | TCS3410_MASK_AEN,
			TCS3410_MASK_PON | TCS3410_MASK_AEN);
		mod_timer(&chip->work_timer,
			jiffies + msecs_to_jiffies(autozero_offset + ctx->alg_ctx.itime_ms + HIGH_TIMER_OFFSET));
		hwlog_info("tcs3410 als enable done\n");
	} else {
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

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct tcs3410_ctx *)chip->device_ctx;
	hwlog_info("%s: tcs3410_work_cnt = %d, %d\n", __func__, tcs3410_work_cnt[FLK_ID], tcs3410_work_cnt[RGB_ID]);
	fd_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if (en) {
		if ((fd_enable_status & FDEN) != FDEN) {
			tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_MASK_FDEN, TCS3410_MASK_FDEN);
			tcs3410_set_field(ctx->handle, TCS3410_INTENAB, TCS3410_MASK_FIEN | TCS3410_MASK_SIEN,
				TCS3410_MASK_FIEN | TCS3410_MASK_SIEN);
			tcs3410_set_field(ctx->handle, TCS3410_SIEN, TCS3410_MASK_SIEN_SIEN_MEASR_SEQR,
				TCS3410_MASK_SIEN_SIEN_MEASR_SEQR);
			hwlog_info("tcs3410 fd enable done\n");

			// first enable flicker timer
			mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FIRST_FLK_TIMER));
			hwlog_info("fd_enable 6ms for a gain quickly\n");
		} else {
			hwlog_warn("fd_enable fd already been enabled\n");
		}
	} else {
		if ((fd_enable_status & FDEN) == FDEN) {
			// disable flicker
			tcs3410_set_field(ctx->handle, TCS3410_INTENAB, TCS3410_ENABLE_OFF, TCS3410_MASK_AIEN | TCS3410_MASK_SIEN);
			tcs3410_set_field(ctx->handle, TCS3410_ENABLE, TCS3410_ENABLE_OFF, TCS3410_MASK_FDEN);
			tcs3410_set_field(ctx->handle, TCS3410_SIEN, TCS3410_ENABLE_OFF, TCS3410_MASK_SIEN_SIEN_MEASR_SEQR);

			// clear ring_buffer when close the fd
			mutex_lock(&ring_buffer_mutex);
			(void)memset_s(ring_buffer, MAX_BUFFER_SIZE * BUF_RATIO_8_16BIT, 0, MAX_BUFFER_SIZE * BUF_RATIO_8_16BIT);
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
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
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
}

static void tcs3410_get_als_normdata(struct tcs3410_ctx *ctx)
{
	int step = 0;

	ctx->gain_mod[MODULATOR_0] = gain_tbl[ctx->gain_reg[MODULATOR_0]];
	ctx->gain_mod[MODULATOR_1] = gain_tbl[ctx->gain_reg[MODULATOR_1]];
	ctx->gain_mod[MODULATOR_2] = gain_tbl[ctx->gain_reg[MODULATOR_2]];
	hwlog_debug("%s: gain_mod0 = %d, gain_mod1 = %d, gainmod2 = %d\n", __func__,
		ctx->gain_mod[MODULATOR_0], ctx->gain_mod[MODULATOR_1], ctx->gain_mod[MODULATOR_2]);

	if (ctx->als.raw_pds[MODULATOR_0] == 0 || ctx->als.raw_pds[MODULATOR_1] == 0 ||
		ctx->als.raw_pds[MODULATOR_2] == 0)
		return;
	if (ctx->gain_mod[MODULATOR_0] == 0 || ctx->gain_mod[MODULATOR_1] == 0 ||
		ctx->gain_mod[MODULATOR_2] == 0)
		return;
	// normalize counts based on gains , Normalize data if gains are different
	ctx->norm_pds[MODULATOR_0] = (uint32_t)(ctx->als.raw_pds[MODULATOR_0] * GC_AGAIN * GAIN_RATIO) /
		(ctx->gain_mod[MODULATOR_0]);
	ctx->norm_pds[MODULATOR_1] = (uint32_t)(ctx->als.raw_pds[MODULATOR_1] * GC_AGAIN * GAIN_RATIO) /
		(ctx->gain_mod[MODULATOR_1]);
	ctx->norm_pds[MODULATOR_2] = (uint32_t)(ctx->als.raw_pds[MODULATOR_2] * GC_AGAIN * GAIN_RATIO) /
		(ctx->gain_mod[MODULATOR_2]);

	step = (ctx->reg_status[ALS_STAT] & TCS3410_MASK_ALS_STATUS_MEAS_SEQR_STEP) >>
			TCS3410_SHIFT_ALS_STATUS_MEAS_SEQR_STEP;

	ctx->norm_data[PD_CLEAR] = ctx->norm_pds[MODULATOR_0];
	ctx->norm_data[PD_BLUE] = ctx->norm_pds[MODULATOR_1];

	if (step == 0)
		ctx->norm_data[PD_RED] = ctx->norm_pds[MODULATOR_2];
	else
		ctx->norm_data[PD_GREEN] = ctx->norm_pds[MODULATOR_2];

	hwlog_debug("%s :c_norm = %d, r_norm = %d, g_norm = %d, b_norm = %d\n", __func__, (uint16_t)ctx->norm_data[PD_CLEAR],
		(uint16_t)ctx->norm_data[PD_RED], (uint16_t)ctx->norm_data[PD_GREEN], (uint16_t)ctx->norm_data[PD_BLUE]);
}

static void tcs3410_handle_als_event(struct tcs3410_ctx *ctx)
{
	if (!ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	
	color_sensor_get_byte(ctx->handle, TCS3410_STATUS2, &ctx->alg_ctx.status);
	
	if (ctx->alg_ctx.status & STATUS2_ALS_DATA_VALID_MASK) {
		tcs3410_handle_als(ctx);

		if (tcs3410_need_change_gain(ctx))
			return;
		tcs3410_get_als_normdata(ctx);
	} else {
		hwlog_debug("%s: No new als data were available\n", __func__);
	}
}

static int tcs3410_report_data(int value[])
{
	if (!als_enabled)
		return 0;
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
			hwlog_info("%s: ratio = %d\n", __func__, ctx->cal_ctx.ratio[i]);
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
	
	report_logcount++;
	if (report_logcount <= AMS_REPORT_LOG_COUNT_NUM || report_logcount % AMS_REPORT_LOG_COUNT_NUM == 0)
		hwlog_info("color_report_val: %d, %d, %d, %d", color_report_val[1], color_report_val[2], color_report_val[3],
			color_report_val[4]);

	if (report_logcount >= MAX_REPORT_LOGCOUNT)
		report_logcount = 0;

	if (chip->in_cal_mode == false)
		tcs3410_report_data(color_report_val + 1);
	else
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
	tcs3410_handle_als_event(ctx);
	tcs3410_report_als(chip);

	rgb_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if ((rgb_enable_status & AEN) != AEN) {
		hwlog_warn("%s: rgb already disabled, del timer\n", __func__);
		if (ctx->alg_ctx.enabled) {
			tcs3410_set_field(ctx->handle, TCS3410_ENABLE, PON | AEN, PON | AEN);
			hwlog_info("%s: alg_ctx enabled: status=%d\n", __func__, ctx->alg_ctx.enabled);
		} else {
			return;
		}
	}
	hwlog_debug("itime is %d \n", ctx->alg_ctx.itime_ms);
	mod_timer(&chip->work_timer, jiffies + msecs_to_jiffies(ctx->alg_ctx.itime_ms + HIGH_TIMER_OFFSET));
}

static void tcs3410_get_read_out_buffer(void)
{
	uint32_t i;
	uint32_t head_ptr_delta;
	u16 *temp_ptr = &ring_buffer[0];
	u16 *temp_head_ptr = head_ptr;

	head_ptr_delta = (uint32_t)((uintptr_t)temp_head_ptr - (uintptr_t)(&ring_buffer[0]));
	head_ptr_delta = head_ptr_delta / ASTEP_LEN;

	if (head_ptr_delta > MAX_BUFFER_SIZE - 1)
		head_ptr_delta = MAX_BUFFER_SIZE - 1;

	(void)memset_s(read_out_buffer, sizeof(read_out_buffer), 0, sizeof(read_out_buffer));

	for (i = 0; i < (MAX_BUFFER_SIZE - head_ptr_delta); i++) {
		read_out_buffer[i] = *temp_head_ptr;
		hwlog_debug("Read out_buffer[%d] = %u from ring_buffer[%d]\n",
			i, read_out_buffer[i], (temp_head_ptr - &ring_buffer[0]));
		temp_head_ptr++;
	}

	for (i = (MAX_BUFFER_SIZE - head_ptr_delta); i < MAX_BUFFER_SIZE; i++) {
		read_out_buffer[i] = *temp_ptr;
		hwlog_debug("Read out_buffer[%d] = %u from ring_buffer[%d]\n",
			i, read_out_buffer[i], (temp_ptr - &ring_buffer[0]));
		temp_ptr++;
	}

#ifdef EXTENDED_LOG
	for (i = 0; i < MAX_BUFFER_SIZE; i++)
		hwlog_info("FD_Buffer[%d],%u\n", i, read_out_buffer[i]);
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
		hwlog_debug("Writing data: buf_16[%d] = %d to ring buffer at position %d\n",
			i, buf_16[i], (buffer_ptr - &ring_buffer[0]));
		*buffer_ptr = buf_16[i];
		tail_ptr = buffer_ptr;
		buffer_ptr++;
		if (buffer_ptr == &ring_buffer[MAX_BUFFER_SIZE]) {
			buffer_ptr = &ring_buffer[0];
			first_circle_end = 1;
		}
	}
	if ((tail_ptr == &ring_buffer[MAX_BUFFER_SIZE - 1]) ||
		(first_circle_end == 0))
		head_ptr = &ring_buffer[0];

	if ((first_circle_end == 1) &&
		(tail_ptr != &ring_buffer[MAX_BUFFER_SIZE - 1]))
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
	(void)memset_s(ring_buffer, MAX_BUFFER_SIZE * BUF_RATIO_8_16BIT, 0, MAX_BUFFER_SIZE * BUF_RATIO_8_16BIT);
	// clear ring_buffer when close the fd
	buffer_ptr = &ring_buffer[0];
	head_ptr = &ring_buffer[0];
	tail_ptr = &ring_buffer[0];
	mutex_unlock(&ring_buffer_mutex);
}

static void handle_fifo_overflow_underflow(struct tcs3410_ctx *ctx, uint8_t fifo_overflow, uint8_t fifo_underflow)
{	
	hwlog_err("FIFO error!,overflow: %d, underflow: %d. Now clear the FIFO and the flicker data\n", 
		fifo_overflow, fifo_underflow);
	uint8_t temp_enable = 0;
	color_sensor_get_byte(ctx->handle, TCS3410_ENABLE, &temp_enable);
	color_sensor_set_byte(ctx->handle, TCS3410_ENABLE, 0);
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
			ctx->fd.flicker_data_number ++;
			ctx->fd.flicker_data_l = 0;
			ctx->fd.flicker_data_h = 0;
		}
	}
	ctx->fd.flicker_data_number = (ctx->fd.flicker_data_number > FFT_BIN_COUNT) ?
		FFT_BIN_COUNT : ctx->fd.flicker_data_number;
	pull_fd_data_to_buffer(ctx, ctx->fd.flicker_data, ctx->fd.flicker_data_number);
	ctx->fd.flicker_data_number = 0;
	(void)memset_s(ctx->fd.flicker_data, sizeof(ctx->fd.flicker_data), 0, sizeof(ctx->fd.flicker_data));
}

static void handle_fifo_data (struct tcs3410_ctx *ctx)
{
	uint8_t fifo_readdata[MAX_DEVICE_FIFO_LEN] = {0};
	uint8_t fifo_overflow = 0;
	uint8_t fifo_underflow = 0;
	int readlength = 0;

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
	ctx->fifo_level = (ctx->fifo_level > MAX_BUFFER_SIZE) ? MAX_BUFFER_SIZE : ctx->fifo_level;
	if (ctx->fifo_level % 2 != 0) {
		hwlog_debug("fifo level is : %d\n", ctx->fifo_level);
		if (ctx->fifo_level > 1)
			ctx->fifo_level--;
	}
	readlength = ctx->fifo_level;
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
	if ((rgb_enable_status & FDEN) != FDEN) {
		hwlog_info("%s: fd already disabled, del timer\n", __func__);
		mutex_unlock(&ctx->lock);
		return;
	}

	mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FD_POLLING_TIME));

	color_sensor_get_byte(ctx->handle, TCS3410_STATUS, &reg_status[STAT]);
	status = reg_status[STAT];
	if (!(status & TCS3410_MASK_FIEN)) {
		hwlog_info("tcs3410_handle_fd_data no fifo/sint interrupt, status == %d\n", status);
		mutex_unlock(&ctx->lock);
		return;
	} else {
		hwlog_debug("tcs3410_handle_fd_data with fifo interrupt status == %d\n", status);
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
	struct tcs3410_ctx *ctx = NULL;
	int ret = 0;
	if (!chip || !chip->device_ctx || !ams_fd_data) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return ;
	}
	mutex_lock(&ring_buffer_mutex);
	tcs3410_get_read_out_buffer();

	ret = memcpy_s(ams_fd_data, sizeof(read_out_buffer), read_out_buffer, sizeof(read_out_buffer));
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
	UINT32 j;
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
	UINT8 rgb_enable_status;
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

static void init_library(struct color_chip *chip)
{
	int ret;

	timer_setup(&chip->work_timer, tcs3410_als_timer_wrk, 0);
	INIT_WORK(&chip->als_work, tcs3410_als_work);

	timer_setup(&chip->fd_timer, tcs3410_flc_timer_wrk, 0);
	INIT_WORK(&chip->fd_work, tcs3410_fd_work);

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

	hwlog_info("set_flicker_num_samples, num_samples = %d\n", num_samples);

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

	hwlog_debug("tcs3410_update_als_gain modulator0_gain = %d, modulator1_gain = %d, modulator2_gain = %d \n",
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

	hwlog_info("%s: threshold = %d\n", __func__, threshold);

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

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_L,
		((TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD1) | (TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD0)));

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP2_MOD_GAINX_L,
		((TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD1) | (TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD0)));

	color_sensor_set_byte(ctx->handle, TCS3410_MEAS_SEQR_STEP3_MOD_GAINX_L,
		((TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD1) | (TCS3410_128_X_GAIN << TCS3410_SHIFT_GAIN_MOD0)));
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

	hwlog_info("%s: mode = %d\n", __func__, mode);

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

#ifdef ENABLE_DUMP_REGS
	tcs3410_dump_reg(ctx);
#endif

	hwlog_info("tcs3410_init done \r\n");
	return 0;
}


static void tcs3410_init_parameter(struct tcs3410_ctx *ctx)
{
	ctx->sample_time = TCS3410_SAMPLE_TIME;
	ctx->als_nr_samples = ALS_NR_SAMPLES;
	ctx->wait_time = TCS3410_WAIT_TIME;
	ctx->agc_mode = TCS3410_AGC_MODE;
	ctx->agc_num_samples = AGC_NUM_SAMPLES;
	ctx->fifo_threshold = FIFO_THRESHOLD;
	ctx->fd_nr_samples = FD_NR_SAMPLES;
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

	tcs3410_init_parameter(ctx);
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
