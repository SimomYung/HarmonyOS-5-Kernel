/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Noise source runtime health test for entropy
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 17 20:18:45 2020
 */
#include <stdint.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libentropy/health_test.h>

/*
 * According to SP800-90B 4.4.2, Adaptive Proportion Test's
 * window size is 512 (the noise source produces more than
 * two distinct values)
 */
#define AP_TEST_WINDOW_SIZE 512

/* test smaple number for health startup test, see NIST SP800-90B 4.3 4 */
#define STARTUP_TEST_MAX_COUNT	1024

int health_rc_test(struct health_test_data *data, uint8_t next_sample)
{
	if (data == NULL) {
		return E_HM_INVAL;
	}

	if (data->count == 0) {
		data->old_sample = next_sample;
		data->count = 1;
		return E_HM_OK;
	}

	if (data->old_sample == next_sample) {
		data->count++;
		if (data->count >= data->max_count) {
			data->count = data->max_count;
			return E_HM_POSIX_FAULT;
		}
	} else {
		data->old_sample = next_sample;
		data->count = 1;
	}

	return E_HM_OK;
}

int health_rc_test_batch(struct health_test_data *data, const uint8_t *next_sample,
			 int sample_count)
{
	int i;
	int ret;

	if (data == NULL || next_sample == NULL) {
		return E_HM_INVAL;
	}

	for (i = 0; i < sample_count; i++) {
		ret = health_rc_test(data, next_sample[i]);
		if (ret != E_HM_OK) {
			return ret;
		}
	}
	return E_HM_OK;
}

int health_ap_test(struct health_test_data *data, uint8_t next_sample)
{
	if (data == NULL) {
		return E_HM_INVAL;
	}

	if (data->count == 0) {
		data->old_sample = next_sample;
		data->count = 1;
		return E_HM_OK;
	}

	data->times = (data->times + 1) % AP_TEST_WINDOW_SIZE;
	/* check, when times value is from 1 to AP_TEST_WINDOW_SIZE - 1 */
	if (data->times != 0 && data->old_sample == next_sample) {
		data->count++;
		if (data->count >= data->max_count) {
			return E_HM_POSIX_FAULT;
		}
	}

	if (data->times == 0) {
		data->count = 0;
	}
	return E_HM_OK;
}

int health_ap_test_batch(struct health_test_data *data, const uint8_t *next_sample,
			 int sample_count)
{
	int i;
	int ret;

	if (data == NULL || next_sample == NULL) {
		return E_HM_INVAL;
	}

	for (i = 0; i < sample_count; i++) {
		ret = health_ap_test(data, next_sample[i]);
		if (ret != E_HM_OK) {
			return ret;
		}
	}
	return E_HM_OK;
}

int health_startup_test(unsigned data_count)
{
	static unsigned int count = 0;
	if (count < STARTUP_TEST_MAX_COUNT) {
		count += data_count;
		return E_HM_AGAIN;
	}
	return E_HM_OK;
}
