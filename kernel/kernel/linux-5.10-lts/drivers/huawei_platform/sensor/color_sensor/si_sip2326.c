/*
 * si_sip2326.c
 *
 * code for SI sip2326 sensor
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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
#include "si_sip2326.h"
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/timer.h>
#include <securec.h>
#include <huawei_platform/log/hw_log.h>
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif
#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <huawei_platform/devdetect/hw_dev_dec.h>
#endif

#define HWLOG_TAG color_sensor
HWLOG_REGIST();

static bool fd_timer_stopped;
static bool report_calibrate_result;
static bool color_calibrate_result = true;
static DEFINE_MUTEX(ring_buffer_mutex);
static DEFINE_MUTEX(enable_handle_mutex);
static u16 g_max_buffer_size;
static uint16_t *ring_buffer = NULL;
static uint32_t *read_out_buffer = NULL;
static uint16_t *buffer_ptr = NULL;
static uint16_t *head_ptr = NULL;
static uint16_t *tail_ptr = NULL;
static UINT16 report_logcount;
static uint8_t first_circle_end;
static struct color_chip *p_chip;
extern UINT32 flicker_support;
static bool flicker_cali = false;
extern int color_report_val[MAX_REPORT_LEN];
static int sip2326_work_cnt[MAX_SENSOR_ID] = {0};
extern int (*color_default_enable)(bool enable);
static uint32_t als_gains[] = {
	25,
	50,
	100,
	200,
	400,
	800,
	1600,
	3200,
	6400,
	12800,
	25600,
	51200,
	102400
};

static void sip2326_set_field(struct i2c_client *handle, uint8_t reg, uint8_t data, uint8_t mask)
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

static void sip2326_get_field(struct i2c_client *handle, UINT8 reg, UINT8 *data, UINT8 mask)
{
	if (!handle || !data) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	color_sensor_get_byte(handle, reg, data);
	*data &= mask;
}

static int sip2326_report_data(int value[])
{
	return ap_color_report(value, SI_REPORT_DATA_LEN * sizeof(int));
}

static int sip2326_set_enable(struct i2c_client *handle, uint8_t reg, uint8_t data, uint8_t mask)
{
	int ret = 0;
	uint8_t ori_data = 0;
	uint8_t new_data;

	if (!handle) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}
	(void)color_sensor_get_byte(handle, SIP2326_ALS_ENABLE, &ori_data);
	new_data = ori_data & (~mask);
	new_data |= (data & mask);
	if (new_data != ori_data)
		ret = color_sensor_set_byte(handle, SIP2326_ALS_ENABLE, new_data);

	return ret;
}

static void sip2326_dump_regs(struct sip2326_ctx *ctx)
{
	uint8_t reg_value = 0;
	uint8_t i = 0;

	struct sip2326_reg_setting_t dump_reg[] = {
		{ SIP2326_RST_CTRL, 0x00 },
		{ SIP2326_ALS_ENABLE, 0x00 },
		{ SIP2326_ALS_CTRL0, 0x00 },
		{ SIP2326_ALS_CTRL1, 0x00 },
		{ SIP2326_ALS_INTE_STEP_TIME_H, 0x00 },
		{ SIP2326_ALS_INTE_STEP_TIME_M, 0x00 },
		{ SIP2326_ALS_INTE_STEP_TIME_L, 0x00 },
		{ SIP2326_ALS_GAIN1, 0x00 },
		{ SIP2326_ALS_GAIN2, 0x00 },
		{ SIP2326_PD_SEL0, 0x00 },
		{ SIP2326_PD_SEL1, 0x00 },
		{ SIP2326_FLK_CTRL, 0x00 },
		{ SIP2326_FLK_INTE_TIME_H, 0x00 },
		{ SIP2326_FLK_INTE_TIME_M, 0x00 },
		{ SIP2326_FLK_INTE_TIME_L, 0x00 },
		{ SIP2326_FMODE0, 0x00 },
		{ SIP2326_FMODE1, 0x00 },
	};

	for (i = 0; i < ARR_SIZE(dump_reg); i++) {
		color_sensor_get_byte(ctx->handle, dump_reg[i].reg, &reg_value);
		hwlog_err("dump_reg: reg is 0x%x, value is 0x%x\n", dump_reg[i].reg, reg_value);
	}
}

static int sip2326_get_chipid(struct i2c_client *handle)
{
	uint8_t chipId = 0;
	uint8_t i;

	if (!handle) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -1;
	}
	for (i = 0; i < CHIPID_GET_TIME; i++) {
		color_sensor_get_byte(handle, SIP2326_ID_REG, &chipId);
		if (chipId == DEVICE_ID) {
			hwlog_info("%s: Chip is SIP2326\n", __func__);
			return 0;
		}
	}
	return -1;
}

static int sip2326_als_set_time(struct sip2326_ctx *ctx, uint16_t itime_ms)
{
	int ret = -1;
	unsigned int i = 0;
	unsigned int al_inte_step_time = 0;
	uint8_t tx_buf[3] = {0};

	al_inte_step_time = (uint32_t)(itime_ms * SCALER * SCALER / ATIME_DUTY_CYCLE - 1);
	tx_buf[0] = (unsigned char)((al_inte_step_time >> SHIFT_16) & 0x03);
	tx_buf[1] = (unsigned char)((al_inte_step_time >> SHIFT_8) & 0xFF);
	tx_buf[2] = (unsigned char)(al_inte_step_time & 0xFF);

	struct sip2326_reg_setting_t time_regs[] = {
		{ SIP2326_ALS_INTE_STEP_TIME_H, tx_buf[0] },
		{ SIP2326_ALS_INTE_STEP_TIME_M, tx_buf[1] },
		{ SIP2326_ALS_INTE_STEP_TIME_L, tx_buf[2] },
	};

	for (i = 0; i < ARR_SIZE(time_regs); ++i) {
		ret = color_sensor_set_byte(ctx->handle, time_regs[i].reg, time_regs[i].value);
		if (ret < 0) {
			hwlog_err("light sensor set als time %u failed!", i);
			return ret;
		}
	}
	ctx->als_ctx.itime_ms = itime_ms;
	if (al_inte_step_time >= MAX_THRESHOLD)
		ctx->als_ctx.max_range = MAX_THRESHOLD;
	else
		ctx->als_ctx.max_range = (uint16_t)(al_inte_step_time + 1);

	hwlog_debug("light sensor itime : %u dev->max_range is %u", ctx->als_ctx.itime_ms, ctx->als_ctx.max_range);
	return 0;
}

static int sip2326_als_set_flicker_time(struct sip2326_ctx *ctx, uint16_t itime_us)
{
	int ret = -1;
	unsigned int i = 0;
	unsigned int al_inte_step_time = 0;
	uint8_t tx_buf[3] = {0};

	al_inte_step_time = (uint32_t)(itime_us * SCALER / ATIME_DUTY_CYCLE - 1);
	tx_buf[0] = (unsigned char)((al_inte_step_time >> SHIFT_16) & 0x03);
	tx_buf[1] = (unsigned char)((al_inte_step_time >> SHIFT_8) & 0xFF);
	tx_buf[2] = (unsigned char)(al_inte_step_time & 0xFF);

	struct sip2326_reg_setting_t time_regs[] = {
		{ SIP2326_FLK_INTE_TIME_H, tx_buf[0] },
		{ SIP2326_FLK_INTE_TIME_M, tx_buf[1] },
		{ SIP2326_FLK_INTE_TIME_L, tx_buf[2] },
	};

	for (i = 0; i < ARR_SIZE(time_regs); ++i) {
		ret = color_sensor_set_byte(ctx->handle, time_regs[i].reg, time_regs[i].value);
		if (ret < 0) {
			hwlog_err("light sensor set flicker time %u failed!", i);
			return ret;
		}
	}

	if (al_inte_step_time >= MAX_THRESHOLD)
		ctx->flc_ctx.max_flicker_range = MAX_THRESHOLD;
	else
		ctx->flc_ctx.max_flicker_range = (uint16_t)(al_inte_step_time + 1);
	hwlog_debug("light sensor max_flicker_range is %u", ctx->flc_ctx.max_flicker_range);
	return 0;
}

static void sip2326_set_flk_gain(struct sip2326_ctx *ctx, uint8_t count)
{
	hwlog_debug("%s : gain = %u\n", __func__, count);
	sip2326_set_field(ctx->handle, SIP2326_ALS_GAIN4, (uint8_t)count, 0x0F);
}

static void sip2326_set_als_channel_gain(struct sip2326_ctx *ctx, uint8_t channel, uint8_t count)
{
	switch(channel) {
	case 0:
		sip2326_set_field(ctx->handle, SIP2326_ALS_GAIN1, (uint8_t)count, 0x0F);
		break;
	case 1:
		sip2326_set_field(ctx->handle, SIP2326_ALS_GAIN1, (uint8_t)(count << SHIFT_4), 0xF0);
		break;
	case 2:
		sip2326_set_field(ctx->handle, SIP2326_ALS_GAIN2, (uint8_t)count, 0x0F);
		break;
	case 3:
		sip2326_set_field(ctx->handle, SIP2326_ALS_GAIN2, (uint8_t)(count << SHIFT_4), 0xF0);
		break;
	default:
		break;
	}
}

static uint32_t find_highest_bit(uint32_t value)
{
	int8_t i;

	if (value == 0)
		return 0;
	for (i = INTEGER_SIZE - 1; i >= 0; i--) {
		if ((value >> (uint32_t)i) & 1)
			return (uint32_t)i;
	}
	return 0;
}

static uint8_t get_rgb_fd_enable_status(struct i2c_client *handle)
{
	uint8_t enable_sta = 0;

	if (!handle) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return -EFAULT;
	}
	color_sensor_get_byte(handle, SIP2326_ALS_ENABLE, &enable_sta);
	return enable_sta;
}

static void sip2326_optimize_gain(uint32_t maximum_adc, uint32_t max, uint8_t *gain_index, uint8_t *saturation)
{
	uint32_t gain_change = 0;
	if (max == 0)
		max = 1;
	if (max >= maximum_adc) {
		if (*gain_index > LOW_AUTO_GAIN_VALUE)
			*gain_index /= AUTO_GAIN_DIV;
		else
			*gain_index = LOWEST_GAIN_ID;
		*saturation = 1;
	} else {
		/* value too low, increase the gain to 80% maximum */
		gain_change = (uint32_t)((maximum_adc * SATURATION_HIGH_PERCENT) / (max * PERCENT_BASE));
		if (gain_change == 0 && (*gain_index) != 0) {
			(*gain_index)--;
		} else {
			gain_change = (maximum_adc * SATURATION_LOW_PERCENT) / (max * PERCENT_BASE);
			gain_change = find_highest_bit(gain_change);
			if (((*gain_index) + gain_change) > MAX_GAIN_ID)
				*gain_index = MAX_GAIN_ID;
			else
				*gain_index += gain_change;
		}
		*saturation = 0;
	}
}

static bool sip2326_als_auto_gain(struct sip2326_ctx *ctx, uint32_t max)
{
	uint8_t old_gain0_count;
	uint8_t new_gain0_count;
	uint8_t saturation_status = 0;
	uint8_t i;

	old_gain0_count = ctx->als_ctx.gain0_count;
	new_gain0_count = ctx->als_ctx.gain0_count;
	ctx->als_ctx.tgain0 = als_gains[old_gain0_count];
	sip2326_optimize_gain(ctx->als_ctx.max_range, max, &new_gain0_count, &saturation_status);
	
	if (old_gain0_count != new_gain0_count) {
		sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, 0, AL_EN_ALL);
		for (i = 0; i < CHANNEL_NUM; i++)
			sip2326_set_als_channel_gain(ctx, i, new_gain0_count);
		ctx->als_ctx.gain0_count = new_gain0_count;
		sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, AL_EN_ALL, AL_EN_ALL);
	}
	hwlog_debug("%s : old_gain0_count = %u, new_gain0_count = %u, saturation_status = %u", __func__,
		old_gain0_count, new_gain0_count, saturation_status);

	return saturation_status;
}

static void sip2326_get_dts_parameter(const struct device *dev, struct color_chip *chip)
{
	int ret;

	ret = of_property_read_u32(dev->of_node, "flk_data_len", &chip->flk_data_len);
	if (ret < 0) {
		hwlog_info("%s, get flk_data_len failed use default\n", __func__);
		chip->flk_data_len = DEFAULT_FLK_DATA_LEN; // default flk data len
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

static void sip2326_init_parameter(struct sip2326_ctx *ctx, struct color_chip *chip)
{
	if (chip->support_flk_2k) {
		ctx->flc_ctx.flicker_itime = FLICKER_ITIME_2K;
		g_max_buffer_size = MAX_BUFFER_SIZE_2K;
		ctx->flc_cali_ctx.fd_cali_freq = CALI_ACCORD_2K;
	} else {
		ctx->flc_ctx.flicker_itime = FLICKER_ITIME_1K;
		g_max_buffer_size = MAX_BUFFER_SIZE_1K;
		ctx->flc_cali_ctx.fd_cali_freq = CALI_ACCORD_1K;
	}
	init_buffers(g_max_buffer_size);
}

static void sip2326_reset_regs(struct sip2326_ctx *ctx)
{
	uint8_t i;
	if (!ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	for (i = 0; i < ARR_SIZE(sip2326_settings); i++)
		color_sensor_set_byte(ctx->handle, sip2326_settings[i].reg, sip2326_settings[i].value);

	sip2326_als_set_time(ctx, DEFAULT_ITIME);
	sip2326_als_set_flicker_time(ctx, ctx->flc_ctx.flicker_itime);

	for (i = 0; i < CHANNEL_NUM; i++)
		sip2326_set_als_channel_gain(ctx, i, DEFAULT_IGAIN_COUNT);
	ctx->als_ctx.gain0_count = DEFAULT_IGAIN_COUNT;
	// initialize calibration default values
	for (i = 0; i < SI_REPORT_DATA_LEN; i++) {
		ctx->cal_ctx.ratio[i] = SIP2326_CAL_RATIO;
		ctx->cal_ctx.target[i] = 0;
	}
}

static bool sip2326_handle_als(struct sip2326_ctx *ctx)
{
	uint8_t adc_data[SIP2326_ADC_BYTES] = {0};
	bool re_enable = false;
	struct sip2326_adc_data_t curr_raw;
	uint32_t max_value = 0;

	(void)memset_s(&curr_raw, sizeof(curr_raw), 0, sizeof(curr_raw));

	color_sensor_read_fifo(ctx->handle, SIP2326_ALS_DATA, adc_data, 8);
	curr_raw.red = (uint16_t)((adc_data[0] << SHIFT_8) | adc_data[1]);
	curr_raw.green = (uint16_t)((adc_data[2] << SHIFT_8) | adc_data[3]);
	curr_raw.blue = (uint16_t)((adc_data[4] << SHIFT_8) | adc_data[5]);
	curr_raw.ir = (uint16_t)((adc_data[6] << SHIFT_8) | adc_data[7]);
	ctx->als_ctx.curr_itime = ctx->als_ctx.itime_ms;
	hwlog_debug("ADC Data: Red: %u, Green: %u, Blue: %u, IR: %u\n",
		curr_raw.red, curr_raw.green, curr_raw.blue, curr_raw.ir);

	max_value = MAX(curr_raw.red, curr_raw.green);
	max_value = MAX(max_value, curr_raw.blue);
	max_value = MAX(max_value, curr_raw.ir);
	re_enable = sip2326_als_auto_gain(ctx, max_value);
	if (!re_enable) {
		ctx->als_ctx.data[0] = curr_raw.ir;
		ctx->als_ctx.data[1] = curr_raw.red;
		ctx->als_ctx.data[2] = curr_raw.green;
		ctx->als_ctx.data[3] = curr_raw.blue;
		if (!ctx->als_ctx.first_flag_after_enable) {
			sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, 0, AL_EN_ALL);
			sip2326_als_set_time(ctx, DEFAULT_ITIME);
			sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, AL_EN_ALL, AL_EN_ALL);
		}
	}
	return re_enable;
}

static bool sip2326_handle_als_event(struct sip2326_ctx *ctx)
{
	bool ret = false;

	color_sensor_get_byte(ctx->handle, SIP2326_ALS_INT_STATUS, &ctx->als_ctx.status);
	color_sensor_get_byte(ctx->handle, SIP2326_DATA_VALID, &ctx->als_ctx.data_valid);
	hwlog_debug("sip2326_status : 0x%x,	sip2326_data_valid : 0x%x",
		ctx->als_ctx.status, ctx->als_ctx.data_valid);
	if (ctx->als_ctx.data_valid & ALS_DATA_VALID)
		// only get available staus, then read data
		ret = sip2326_handle_als(ctx);
	else
		hwlog_info("%s: No new als data were available\n", __func__);

	return ret;
}

static void sip2326_get_als_data(struct sip2326_ctx *ctx, uint32_t *p_data, uint32_t num)
{
	int ret = 0;

	if (!ctx || !p_data) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	if (num != SI_REPORT_DATA_LEN) {
		hwlog_err("%s: length out of range: %u\n", __func__, num);
		return;
	}
	ret = memcpy_s(p_data, num * sizeof(uint32_t), ctx->als_ctx.data, num * sizeof(uint32_t));
	if (ret != 0)
		return;
	ctx->als_ctx.als_update &= ~(SI_SIP2326_FEATURE_ALS);
}

void sip2326_als_set_enable(struct color_chip *chip, uint8_t en)
{
	struct sip2326_ctx *ctx = NULL;
	uint8_t i;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct sip2326_ctx *)chip->device_ctx;
	hwlog_info("%s = %u sip2326_work_cnt = %d, %d\n", __func__, en, 
		sip2326_work_cnt[FLK_ID], sip2326_work_cnt[RGB_ID]);

	if (ctx->als_ctx.enabled && (en == 1)) {
		if (ctx->als_ctx.first_flag_after_enable) {
			hwlog_info("%s, rgb already enabled, return\n", __func__);
			return;
		} else {
			sip2326_report_data(color_report_val + 1);
			hwlog_info("%s, rgb already enabled and report\n", __func__);
		}
		return;
	}

	if (!ctx->als_ctx.enabled && (en == 0)) {
		hwlog_err("%s: rgb already disaled ,return\n", __func__);
		return;
	}
	ctx->als_ctx.enabled = en;

	if (en) {
		sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, 0, AL_EN_ALL);
		sip2326_als_set_time(ctx, SHORT_ITIME);
		for (i = 0; i < CHANNEL_NUM; i++) {
			sip2326_set_als_channel_gain(ctx, i, FIRST_IGAIN);
			ctx->als_ctx.data[i] = 0;
		}
		ctx->als_ctx.gain0_count = FIRST_IGAIN;
		ctx->als_ctx.first_flag_after_enable = true;
		ctx->als_ctx.first_flag_count = 0;
		sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, AL_EN_ALL, AL_EN_ALL);
		mod_timer(&chip->work_timer, jiffies + msecs_to_jiffies(AUTOZERO_TIMER_OFFSET + SHORT_ITIME));
		hwlog_info("sip2326 als enable done\n");
	} else {
		sip2326_set_enable(ctx->handle, SIP2326_ALS_ENABLE, 0, AL_EN_ALL);
		hwlog_info("sip2326 als disable done\n");
	}
	report_logcount = 0;
}

static void sip2326_cal_als(struct color_chip *chip)
{
	uint32_t i;
	struct sip2326_ctx *ctx = NULL;
	ctx = chip->device_ctx;
	bool valid_data = true;

	for (i = 0; i < SI_REPORT_DATA_LEN; i++) {
		if (color_report_val[i + 1] == 0) {
			hwlog_warn("%s: color_report_val[%u] == 0, can not divide by zero\n", __func__, i + 1);
			ctx->cal_ctx.ratio[i] = SIP2326_CAL_RATIO;
			valid_data = false;
		} else {
			hwlog_info("%s: color_report_val[%u] == %d\n", __func__, i + 1, color_report_val[i + 1]);
			ctx->cal_ctx.ratio[i] = (ctx->cal_ctx.target[i] * SI_SIP2326_FLOAT_TO_FIX) / (color_report_val[i + 1]);
			hwlog_info("%s: ratio = %u\n", __func__, ctx->cal_ctx.ratio[i]);
		}
	}
	report_calibrate_result = valid_data;
}

static void sip2326_report_als(struct color_chip *chip)
{
	UINT32 raw_data[SI_REPORT_DATA_LEN] = {0};
	struct sip2326_ctx *ctx = NULL;
	uint32_t curr_gain;
	uint32_t curr_itime;
	uint8_t i;

	ctx = chip->device_ctx;
	sip2326_get_als_data(chip->device_ctx, &raw_data[0], SI_REPORT_DATA_LEN);
	curr_gain = ctx->als_ctx.tgain0;
	curr_itime = ctx->als_ctx.curr_itime;
	hwlog_debug("curr_gain : %d, curr_itime : %d\n", curr_gain, curr_itime);
	if (curr_gain == 0 || curr_itime == 0)
		return;

	color_report_val[0] = SI_REPORT_DATA_LEN;
	for (i = 0; i < SI_REPORT_DATA_LEN; i++) {
		raw_data[i] *= SI_SIP2326_GAIN_OF_GOLDEN * GAIN_RATIO / curr_itime * PERCENT_BASE;
		raw_data[i] /= curr_gain;
		color_report_val[i + 1] = raw_data[i];
	}
	report_logcount++;
	if (report_logcount <= SIP_LOG_LENGTH || report_logcount % SIP_LOG_LENGTH == 0) {
		hwlog_info("color_report_val: %d, %d, %d, %d", color_report_val[1], color_report_val[2], color_report_val[3],
			color_report_val[4]);
	}

	if (report_logcount >= MAX_REPORT_LOGCOUNT)
		report_logcount = 0;

	if (chip->in_cal_mode == false) {
		sip2326_report_data(color_report_val + 1);
		if (ctx->flc_ctx.enable && ctx->als_ctx.first_flag_after_enable) {
			sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN|ALS_EN, ALS_FLK_EN|ALS_EN);
			mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FD_POLLING_TIME));
		}
		if (ctx->als_ctx.first_flag_after_enable) {
			ctx->als_ctx.first_flag_count++;
			if (ctx->als_ctx.first_flag_count >= FIRST_FLAG_MAX_COUNT) {
				ctx->als_ctx.first_flag_after_enable = false;
				ctx->als_ctx.first_flag_count = 0;
			}
		}
	} else {
		ctx->als_ctx.first_flag_after_enable = false;
		sip2326_cal_als(chip);
	}
}

static void sip2326_als_timer_wrk(struct timer_list *data)
{
	struct color_chip *chip = from_timer(chip, data, work_timer);

	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	queue_work(system_power_efficient_wq, &chip->als_work);
}

static void sip2326_flc_timer_wrk(struct timer_list *data)
{
	struct color_chip *chip = from_timer(chip, data, fd_timer);

	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	queue_work(system_power_efficient_wq, &chip->fd_work);
}

static void sip2326_flc_cali_timer_wrk(struct timer_list *data)
{
	struct color_chip *chip = from_timer(chip, data, fd_cali_timer);

	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	queue_work(system_power_efficient_wq, &chip->fd_cali_work);
}

static bool check_flicker_status(struct sip2326_ctx *ctx)
{
	uint8_t fd_status1 = 0;
	uint8_t fd_enable_status = 0;
	bool error_detected = false;

	color_sensor_get_byte(ctx->handle, SIP2326_ALS_ENABLE, &fd_enable_status);
	if ((fd_enable_status & ALS_FLK_EN) == 0) {
		sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN, ALS_FLK_EN);
		hwlog_err("check_flicker_status: Flicker enable set.");
	}
	color_sensor_get_byte(ctx->handle, SIP2326_FIFO_STATUS, &fd_status1);
	if (fd_status1 & FIFO_FULL) {
		error_detected = true;
		hwlog_err("ERROR: Flicker FIFO overflow detected");
	}
	return error_detected;
}

static void reset_fd_buffer(struct sip2326_ctx *ctx, bool error_detected)
{
	// if fd_gain is adjusted
	// clear the ringbuffer and reset the related pointer.
	mutex_lock(&ring_buffer_mutex);
	memset_s(ring_buffer, g_max_buffer_size * sizeof(*ring_buffer), 0,
			g_max_buffer_size * sizeof(*ring_buffer));
	// clear ring_buffer when close the fd
	buffer_ptr = &ring_buffer[0];
	head_ptr = &ring_buffer[0];
	tail_ptr = &ring_buffer[0];
	mutex_unlock(&ring_buffer_mutex);

	if (error_detected) {
		sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, 0, ALS_FLK_EN);
		sip2326_set_field(ctx->handle, SIP2326_FMODE0, FIFO_CLEAR, FIFO_CLEAR);
		sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN, ALS_FLK_EN);
	}
}

static uint16_t check_fifo_len(struct sip2326_ctx *ctx)
{
	uint8_t size_low;
	uint8_t size_high;
	uint8_t size_value[2] = {0};
	uint16_t fifo_level = 0;

	color_sensor_read_fifo(ctx->handle, SIP2326_FIFO_STATUS, &size_value[0], 2);
	size_low = ((size_value[0] & 0xE0) >> 5);
	size_high = (size_value[1] << 3);
	fifo_level = size_high | size_low;
	return fifo_level;
}

static bool sip2326_auto_flicker_gain(struct sip2326_ctx *ctx, uint32_t max)
{
	uint8_t old_gain_count;
	uint8_t new_gain_count;
	uint8_t saturation_status = 0;

	old_gain_count = ctx->flc_ctx.gain_flicker_count;
	new_gain_count = ctx->flc_ctx.gain_flicker_count;
	ctx->flc_ctx.fgain = als_gains[old_gain_count];
	sip2326_optimize_gain(ctx->flc_ctx.max_flicker_range, max, &new_gain_count,
		&saturation_status);

	if (old_gain_count != new_gain_count) {
		sip2326_set_field(ctx->handle,SIP2326_ALS_ENABLE, 0, ALS_FLK_EN | ALS_EN);
		sip2326_set_field(ctx->handle, SIP2326_FMODE0, 0x10, 0x10);  // clear fifo
		sip2326_set_flk_gain(ctx, new_gain_count);
		ctx->flc_ctx.gain_flicker_count = new_gain_count;
		sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN | ALS_EN, ALS_FLK_EN | ALS_EN);
		saturation_status = true;
	}

	hwlog_debug("old_gain_count = %u, new_gain_count = %u", old_gain_count, new_gain_count);
	return saturation_status;
}

static void sip2326_ring_buffer_process(uint8_t fifo_lvl, uint16_t *buf_16)
{
	uint32_t i;

	if (!buf_16) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	mutex_lock(&ring_buffer_mutex);
	for (i = 0; i < fifo_lvl; i++) {
		hwlog_debug("Writing data: buf_16[%u] = %u to ring buffer at position %u\n", i, buf_16[i],
			(buffer_ptr - &ring_buffer[0]));
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

static void pull_fd_data_to_buffer(struct sip2326_ctx *ctx, uint16_t *buf, uint8_t num)
{
	uint8_t fd_enable_status;
	uint8_t i;

	fd_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if ((fd_enable_status & ALS_FLK_EN) == ALS_FLK_EN) {
		// normalization
		if (ctx->flc_ctx.fgain == 0) {
			hwlog_err("Division by zero detected : fd_gain is 0\n");
			return;
		}
		for (i = 0; i < num; i++)
			buf[i] = (uint16_t)(((uint32_t)(buf[i]) * SI_SIP2326_FD_GAIN_OF_GOLDEN * GAIN_RATIO ) / ctx->flc_ctx.fgain);
		sip2326_ring_buffer_process(num, buf);
	}
}

static void read_fd_data(struct color_chip *chip)
{
	uint16_t buf_16_bit[REPORT_FIFO_LEN] = {0};
	uint8_t buf_8_bit[REPORT_FIFO_LEN * 2] = {0};
	bool auto_gain = false;
	uint16_t fifo_level = 0;
	struct sip2326_ctx *ctx = NULL;
	bool error_detected = false;
	uint16_t flicker_max = 0;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s, pointer is null\n", __func__);
		return;
	}
	ctx = (struct sip2326_ctx *)chip->device_ctx;

	error_detected = check_flicker_status(ctx);
	fifo_level = check_fifo_len(ctx);
	if (fifo_level >= MAX_FIFO_LEVEL)
		error_detected = true;
	hwlog_debug("%s fd_timer_stop = %d\n", __func__, fd_timer_stopped);
	if (fd_timer_stopped == false)
		mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FD_POLLING_TIME));
	else
		hwlog_info("fd has been disabled, del fd work timer\n");

	hwlog_debug("%s: FIFO_LVL: %u, error: %d\n", __func__, fifo_level, error_detected);
	if (fifo_level > 0 && !error_detected) {
		color_sensor_read_fifo(ctx->handle, SIP2326_ALS_FDATA, buf_8_bit, fifo_level);
		for (uint16_t i = 0; i < fifo_level; i += 2) {
			buf_16_bit[i / 2] = (uint16_t)(buf_8_bit[i + 1]) | ((uint16_t)(buf_8_bit[i]) << SHIFT_8);
			hwlog_debug("%s: buf_16_bit[%u]:%u\n", __func__, i / 2, buf_16_bit[i / 2]);
			if(buf_16_bit[i / 2] > flicker_max)
				flicker_max = buf_16_bit[i / 2];
		}
		auto_gain = sip2326_auto_flicker_gain(ctx, flicker_max);
		if (auto_gain == false)
			pull_fd_data_to_buffer(ctx, buf_16_bit, fifo_level / 2);
	} else {
		hwlog_info("%s: FIFO_LVL: %u, error: %d\n", __func__, fifo_level, error_detected);
	}
	if (auto_gain == true || error_detected) {
		hwlog_info("%s: FD reset, auto_gain: %d, error: %d\n", __func__, auto_gain, error_detected);
		reset_fd_buffer(ctx, error_detected);
	}
}

static void sip2326_flicker_cali_adjust_itime(struct sip2326_ctx *ctx)
{
	uint32_t itime_reg_count = 0;
	uint32_t itime_reg_count_adjust = 0;
	UINT8 data[3];

	sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, 0, ALS_FLK_EN);
	color_sensor_get_byte(ctx->handle, SIP2326_FLK_INTE_TIME_H, &data[0]);
	color_sensor_get_byte(ctx->handle, SIP2326_FLK_INTE_TIME_M, &data[1]);
	color_sensor_get_byte(ctx->handle, SIP2326_FLK_INTE_TIME_L, &data[2]);
	itime_reg_count = (uint16_t)(data[0] << 16 | data[1] << 8 | data[2]);
	itime_reg_count_adjust = (itime_reg_count * ctx->flc_cali_ctx.fd_data_cnt +
		ctx->flc_cali_ctx.target_data_cnt / 2) / ctx->flc_cali_ctx.target_data_cnt;
	color_sensor_set_byte(ctx->handle, SIP2326_FLK_INTE_TIME_H, (uint8_t)((itime_reg_count_adjust >> 16) & 0x03));
	color_sensor_set_byte(ctx->handle, SIP2326_FLK_INTE_TIME_M, (uint8_t)((itime_reg_count_adjust >> 8) & 0xFF));
	color_sensor_set_byte(ctx->handle, SIP2326_FLK_INTE_TIME_L, (uint8_t)((itime_reg_count_adjust) & 0xFF));
	ctx->flc_ctx.max_flicker_range = (uint16_t)(itime_reg_count_adjust + 1);
	sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN, ALS_FLK_EN);
	hwlog_info("%s:adjust itime itime_reg_count:%u, itime_reg_count_adjust:%u\n",__func__,
		itime_reg_count, itime_reg_count_adjust);
}

static void fd_cali_data(struct color_chip *chip)
{
	uint8_t buf_8_bit[REPORT_FIFO_LEN * 2] = {0};
	uint16_t fifo_level = 0;
	struct sip2326_ctx *ctx = NULL;
	bool error_detected = false;
	int deviation_percent = 0;

	if (!chip || !chip->device_ctx)
		return;

	ctx = (struct sip2326_ctx *)chip->device_ctx;
	if (ctx->flc_cali_ctx.time_start == 0 && flicker_cali)
		ctx->flc_cali_ctx.time_start = ktime_get() / UNIT_MS_TO_NS;

	error_detected = check_flicker_status(ctx);
	fifo_level = check_fifo_len(ctx);
	if(ctx->flc_cali_ctx.fd_timer_cnt >= FLK_CALI_POLLING_TIME && flicker_cali) {
		ctx->flc_cali_ctx.time_end = ktime_get() / UNIT_MS_TO_NS;
		ctx->flc_cali_ctx.target_data_cnt =
			(ctx->flc_cali_ctx.time_end - ctx->flc_cali_ctx.time_start) * ctx->flc_cali_ctx.fd_cali_freq;
		ctx->flc_cali_ctx.fd_data_cnt /= 2; // fifo u16 = fifo_level /2
		hwlog_info("%s: fd_data_cnt: %u, target_data_cnt:%u\n", __func__, ctx->flc_cali_ctx.fd_data_cnt,
			ctx->flc_cali_ctx.target_data_cnt);
		deviation_percent = ((ctx->flc_cali_ctx.fd_data_cnt - ctx->flc_cali_ctx.target_data_cnt) * 1000) /
			ctx->flc_cali_ctx.target_data_cnt;
		if (abs(deviation_percent) > FREQUENCY_TOLERANCE_PERCENT)
			sip2326_flicker_cali_adjust_itime(ctx);
		error_detected = true;
		flicker_cali = false;
		sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, 0, ALS_FLK_EN);
	}
	if (flicker_cali) {
		ctx->flc_cali_ctx.fd_data_cnt += fifo_level;
		ctx->flc_cali_ctx.fd_timer_cnt++;
		mod_timer(&chip->fd_cali_timer, jiffies + msecs_to_jiffies(FLK_CALI_POLLING_TIME));
	} else {
		ctx->flc_cali_ctx.fd_data_cnt = 0;
		ctx->flc_cali_ctx.fd_timer_cnt = 0;
	}
	if (fifo_level >= MAX_FIFO_LEVEL)
		error_detected = true;
	if (fifo_level > 0 && !error_detected)
		color_sensor_read_fifo(ctx->handle, SIP2326_ALS_FDATA, buf_8_bit, fifo_level);
	if (error_detected && flicker_cali) {
		ctx->flc_cali_ctx.fd_timer_cnt = 0;
		ctx->flc_cali_ctx.fd_data_cnt = 0;
		ctx->flc_cali_ctx.time_start = 0;
		hwlog_info("%s: FD reset, error: %d\n", __func__, error_detected);
		reset_fd_buffer(ctx, error_detected);
	}
}

static void sip2326_fd_cali_work(struct work_struct *work)
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
	fd_cali_data(chip);
	mutex_unlock(&chip->lock);
}

static void sip2326_fd_work(struct work_struct *work)
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
	sip2326_work_cnt[FLK_ID]++;
	SI_MUTEX_LOCK(&chip->lock);
	read_fd_data(chip);
	SI_MUTEX_UNLOCK(&chip->lock);
}

static void sip2326_als_work(struct work_struct *work)
{
	bool re_enable = false;
	uint8_t rgb_enable_status;
	struct sip2326_ctx *ctx = NULL;
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
	ctx = (struct sip2326_ctx *)chip->device_ctx;
	sip2326_work_cnt[RGB_ID]++;

	mutex_lock(&chip->lock);
	re_enable = sip2326_handle_als_event(ctx);
	if (!re_enable && (ctx->als_ctx.data_valid & ALS_DATA_VALID))
		sip2326_report_als(chip);
	else
		hwlog_info("%s: over saturation\n", __func__);
	rgb_enable_status = get_rgb_fd_enable_status(ctx->handle);
	if ((rgb_enable_status & AL_EN_ALL) != AL_EN_ALL) {
		hwlog_info("%s: rgb already disabled, del timer\n", __func__);
		if (ctx->als_ctx.enabled) {
			sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, AL_EN_ALL, AL_EN_ALL);
		} else {
			mutex_unlock(&chip->lock);
			return;
		}
	}
	if (ctx->als_ctx.first_flag_after_enable)
		mod_timer(&chip->work_timer, jiffies + msecs_to_jiffies(AUTOZERO_TIMER_OFFSET + SHORT_ITIME));
	else
		mod_timer(&chip->work_timer, jiffies + msecs_to_jiffies(ctx->als_ctx.itime_ms + HIGH_TIMER_OFFSET));
	mutex_unlock(&chip->lock);
}

static void sip2326_show_enable(struct color_chip *chip, int *state) {
	uint8_t rgb_enable_status;

	if (!chip || !state) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	rgb_enable_status = get_rgb_fd_enable_status(chip->client);
	if (rgb_enable_status & AL_EN_ALL)
		*state = 1;
	else
		*state = 0;
}

static void sip2326_store_enable(struct color_chip *chip, int state)
{
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	color_calibrate_result = false;
	chip->in_cal_mode = false;
	if (state)
		sip2326_als_set_enable(chip, SIDRIVER_ALS_ENABLE);
	else
		sip2326_als_set_enable(chip, SIDRIVER_ALS_DISABLE);
}

static void fd_enable_set(struct color_chip *chip, uint8_t en)
{
	struct sip2326_ctx *ctx = NULL;
	uint8_t fd_enable_status;

	if (!chip || !chip->device_ctx) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	ctx = (struct sip2326_ctx *)chip->device_ctx;
	hwlog_info("%s en = %d success\n", __func__, en);
	fd_enable_status = get_rgb_fd_enable_status(ctx->handle);
	hwlog_debug("%s: fd_enable_status = %u\n", __func__, fd_enable_status);
	hwlog_info("%s: sip2326_work_cnt = %d, %d\n", __func__, sip2326_work_cnt[FLK_ID], sip2326_work_cnt[RGB_ID]);
	if (en) {
		// set default gain
		sip2326_set_flk_gain(ctx, DEFAULT_IGAIN_COUNT);
		ctx->flc_ctx.gain_flicker_count = DEFAULT_IGAIN_COUNT;
		ctx->flc_ctx.enable = true;
		// enable flicker
		// first enable flicker timer
		if (!ctx->als_ctx.first_flag_after_enable) {
			sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN | ALS_EN, ALS_FLK_EN | ALS_EN);
			mod_timer(&chip->fd_timer, jiffies + msecs_to_jiffies(FIRST_FLK_TIMER));
			hwlog_info("fd_enable 6ms for a gain quickly\n");
		}
		fd_timer_stopped = false;
		if ((fd_enable_status & ALS_FLK_EN) != ALS_FLK_EN)
			hwlog_info("fd_enable success.\n");
		else
			hwlog_info("fd_enable fd already been enabled\n");
	} else {
		// disable flicker
		sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, 0, ALS_FLK_EN | ALS_EN);
		fd_timer_stopped = true;
		ctx->flc_ctx.enable = false;
		// clear ring_buffer when close the fd
		mutex_lock(&ring_buffer_mutex);
		memset_s(ring_buffer, g_max_buffer_size * sizeof(*ring_buffer), 0,
			g_max_buffer_size * sizeof(*ring_buffer));
		buffer_ptr = &ring_buffer[0];
		head_ptr = &ring_buffer[0];
		tail_ptr = &ring_buffer[0];
		mutex_unlock(&ring_buffer_mutex);
		if ((fd_enable_status & ALS_FLK_EN) == ALS_FLK_EN)
			hwlog_info("fd_enable now disable fd sensor\n");
		else
			hwlog_info("fd_enable has already been disabled\n");
	}
}

void sip2326_fd_show_enable(struct color_chip *chip, int *state)
{
	uint8_t fd_enable_status;

	if (!chip || !state) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}

	fd_enable_status = get_rgb_fd_enable_status(chip->client);
	if ((fd_enable_status & ALS_FLK_EN) == ALS_FLK_EN)
		*state = 1;
	else
		*state = 0;
}

void sip2326_fd_store_enable(struct color_chip *chip, int state)
{
	if (!chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	if (state)
		// check the calibration process, disable fd function
		fd_enable_set(chip, SIDRIVER_FD_ENABLE);
	else
		fd_enable_set(chip, SIDRIVER_FD_DISABLE);

	hwlog_info("%s enable = %d success\n", __func__, state);
}

static void sip2326_get_read_out_buffer(void)
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
		hwlog_info("FD_Buffer[%u],%u\n", i, read_out_buffer[i]);
#endif
}

static void sip2326_get_fd_data(struct color_chip *chip, char *si_fd_data)
{
	int ret = 0;
	if (!chip || !si_fd_data) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	mutex_lock(&ring_buffer_mutex);
	sip2326_get_read_out_buffer();
	ret = memcpy_s(si_fd_data, g_max_buffer_size * sizeof(*read_out_buffer),
		read_out_buffer, g_max_buffer_size * sizeof(*read_out_buffer));
	mutex_unlock(&ring_buffer_mutex);
	if (ret != 0) {
		hwlog_err("%s fail\n", __func__);
		return;
	}
}

static char *sip2326_chip_name(void)
{
	return "si_sip2326";
}

static char *sip2326_algo_type(void)
{
	return "sip2326_nor";
}

static void sip2326_show_calibrate(struct color_chip *chip, struct at_color_sensor_output_t *out)
{
	UINT32 i;
	UINT32 j;
	struct sip2326_ctx *ctx = NULL;

	if (!out || !chip || !chip->device_ctx) {
		hwlog_err("%s input para NULL\n", __func__);
		return;
	}

	ctx = chip->device_ctx;
	if (chip->in_cal_mode == false)
		hwlog_err("%s not in cali mode\n", __func__);
	out->gain_arr = CAL_STATE_GAIN_LAST;
	out->color_arr = SI_REPORT_DATA_LEN;
	out->result = (UINT32)report_calibrate_result;
	for (i = 0; i < SI_REPORT_DATA_LEN; i++)
		out->cali_rst[i][0] = ctx->cal_ctx.ratio[i];

	hwlog_info("%s : cali= %u, %u, %u, %u, rst = %u\n", __func__,
		out->cali_rst[0][0], out->cali_rst[1][0], out->cali_rst[2][0], out->cali_rst[3][0], out->result);
}

static void sip2326_store_calibrate(struct color_chip *chip, struct at_color_sensor_input_t *in)
{
	struct sip2326_ctx *ctx = NULL;
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
	ret = memcpy_s(ctx->cal_ctx.target, SI_REPORT_DATA_LEN * sizeof(UINT32), in->reserverd,
		SI_REPORT_DATA_LEN * sizeof(UINT32));
	if (ret != 0)
		return;
	chip->in_cal_mode = true;
	// make the calibrate_result true, it will be reset on any error
	color_calibrate_result = false;
}

static int sip2326_rgb_report_type(void)
{
	return AWB_SENSOR_RAW_SEQ_TYPE_SIP2326;
}

int sip2326_enable_rgb(bool enable)
{
	struct color_chip *chip = p_chip;

	hwlog_info("%s: enter\n", __func__);
	if (!chip) {
		hwlog_err("%s: chip Pointer is NULL\n", __func__);
		return 0;
	}

	if (enable)
		sip2326_als_set_enable(chip, SIDRIVER_ALS_ENABLE);
	else
		sip2326_als_set_enable(chip, SIDRIVER_ALS_DISABLE);

	return SET_ENABLE_COMPLETE;
}
EXPORT_SYMBOL_GPL(sip2326_enable_rgb);

static void configure_functions(struct color_chip *chip)
{
	chip->color_enable_show_state = sip2326_show_enable;
	chip->color_enable_store_state = sip2326_store_enable;

	chip->get_flicker_data = sip2326_get_fd_data;
	chip->flicker_enable_show_state = sip2326_fd_show_enable;
	chip->flicker_enable_store_state = sip2326_fd_store_enable;

	chip->color_chip_name = sip2326_chip_name;
	chip->color_algo_type = sip2326_algo_type;

	chip->at_color_show_calibrate_state = sip2326_show_calibrate;
	chip->at_color_store_calibrate_state = sip2326_store_calibrate;
	chip->color_report_type = sip2326_rgb_report_type;
	color_default_enable = sip2326_enable_rgb;
}

static void init_library(struct color_chip *chip)
{
	int ret;

	timer_setup(&chip->work_timer, sip2326_als_timer_wrk, 0);
	INIT_WORK(&chip->als_work, sip2326_als_work);

	timer_setup(&chip->fd_timer, sip2326_flc_timer_wrk, 0);
	INIT_WORK(&chip->fd_work, sip2326_fd_work);

	timer_setup(&chip->fd_cali_timer, sip2326_flc_cali_timer_wrk, 0);
	INIT_WORK(&chip->fd_cali_work, sip2326_fd_cali_work);

	configure_functions(chip);

	p_chip = chip;
	ret = color_register(chip);
	if (ret < 0) {
		hwlog_err("%s color_register fail\n", __func__);
		return;
	}

	hwlog_info("rgb sensor %s ok\n", __func__);
}

static void startup_calibration(struct sip2326_ctx *ctx, struct color_chip *chip)
{
	if (!ctx || !chip) {
		hwlog_err("%s: Pointer is NULL\n", __func__);
		return;
	}
	hwlog_info("%s enter\n", __func__);
	// set default gain
	sip2326_set_flk_gain(ctx, DEFAULT_IGAIN_COUNT);
	// enable flicker
	sip2326_set_field(ctx->handle, SIP2326_ALS_ENABLE, ALS_FLK_EN | ALS_EN, ALS_FLK_EN | ALS_EN);
	flicker_cali = true;
	mod_timer(&chip->fd_cali_timer, jiffies + msecs_to_jiffies(FLK_CALI_POLLING_TIME));
}

static int sip2326_probe(struct i2c_client *client, const struct i2c_device_id *idp)
{
	int ret = -1;
	struct device *dev = NULL;
	static struct color_chip *chip = NULL;
	struct sip2326_ctx *ctx = NULL;
	hwlog_info("Enter %s\n", __func__);
	if (!client)
		return -EFAULT;
	color_notify_support();  // declare support sip2326
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

	if (sip2326_get_chipid(chip->client) < 0) {
		hwlog_info("%s failed: SI_UNKNOWN_DEVICE\n", __func__);
		goto id_failed;
	}
	flicker_support = 1;
	chip->device_ctx = kzalloc(sizeof(struct sip2326_ctx), GFP_KERNEL);
	if (!chip->device_ctx)
		goto id_failed;

	ctx = chip->device_ctx;
	ctx->handle = chip->client;
	mutex_init(&(ctx->flc_ctx.lock));
	sip2326_get_dts_parameter(dev, chip);
	sip2326_init_parameter(ctx, chip);
	sip2326_reset_regs(ctx);
	init_library(chip);
	startup_calibration(ctx, chip);
	hwlog_info("%s succ\n", __func__);
	return 0;

id_failed:
	if (chip->device_ctx)
		kfree(chip->device_ctx);
	i2c_set_clientdata(client, NULL);
	hwlog_err("%s id_failed\n", __func__);
malloc_failed:
	if (chip)
		kfree(chip);
	hwlog_err("%s malloc_failed\n", __func__);
init_failed:
	hwlog_err("%s Probe failed\n", __func__);
	color_notify_absent();
	return ret;
}

int sip2326_suspend(struct device *dev)
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

int sip2326_resume(struct device *dev)
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

int si_sip2326_remove(struct i2c_client *client)
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

static struct i2c_device_id sidriver_idtable[] = {
	{ "si_sip2326", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, sidriver_idtable);

static const struct dev_pm_ops si_sip2326_pm_ops = {
	.suspend = sip2326_suspend,
	.resume = sip2326_resume,
};

static const struct of_device_id sidriver_of_id_table[] = {
	{ .compatible = "si,sip2326" },
	{},
};

static struct i2c_driver si_sip2326_driver = {
	.driver = {
			.name = "si_sip2326",
			.owner = THIS_MODULE,
			.of_match_table = sidriver_of_id_table,
	},
	.id_table = sidriver_idtable,
	.probe = sip2326_probe,
	.remove = si_sip2326_remove,
};

static int __init sip2326_init(void)
{
	int rc;

	hwlog_info("Enter %s\n", __func__);
	rc = i2c_add_driver(&si_sip2326_driver);

	hwlog_info("%s %d\n", __func__, rc);
	return rc;
}

static void __exit sip2326_exit(void)
{
	hwlog_info("%s\n", __func__);
	i2c_del_driver(&si_sip2326_driver);
}

module_init(sip2326_init);
module_exit(sip2326_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("color_sensor driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");