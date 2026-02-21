/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Noise source runtime health test for entropy
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 17 20:19:09 2020
 */
#ifndef ULIBS_LIBENTROPY_HEALTH_TEST_H
#define ULIBS_LIBENTROPY_HEALTH_TEST_H

#include <stdint.h>

struct health_test_data {
	uint8_t old_sample;
	/* repeat count */
	int count;
	/* It is calculated according to the assumed entropy of smaple */
	int max_count;
	/* just for Adaptive Proportion Test */
	int times;
	/* health test fail count */
	unsigned fail_count;
};

/*
 * According to the formula provided by NIST SP800-90B 4.2.1
 * Select one conservative value
 */
#define HEALTH_RC_MAX_COUNT	64

/*
 * According to the formula from NIST SP800-90B 4.2.2
 * Select one conservative value
 */
#define HEALTH_SYSMGR_AP_MAX_COUNT	461
#define HEALTH_DEVHOST_AP_MAX_COUNT	507


/* health test failure count */
#define HEALTH_FAILURE_COUNT_MAX	1024

/*
 * Repetition Count Test
 *
 * @param	data		[I] health test data
 * @param	next_sample	[1] next test smaple
 *
 * @return	E_HM_OK success, caller needs to do test with new next sample
 *		E_HM_POSIX_FAULT failed, caller needs do fail process
 *		E_HM_INVAL failed, parameter is invalid
 * @note	None
 * @see		NIST SP800-90B 4.4.1
 * @deprecated	None
 */
int health_rc_test(struct health_test_data *data, uint8_t next_sample);

/*
 * Batch Repetition Count Test
 *
 * @param	data		[I] health test data
 * @param	next_sample	[I] next test smaple buffer
 * @param	sample_count	[I] smaple buffer size
 *
 * @return	E_HM_OK success, caller needs to do test with new next sample
 *		E_HM_POSIX_FAULT failed, caller needs do fail process
 *		E_HM_INVAL failed, parameter is invalid
 * @note	None
 * @see		NIST SP800-90B 4.4.1
 * @deprecated	None
 */
int health_rc_test_batch(struct health_test_data *data, const uint8_t *next_sample,
			 int sample_count);

/*
 * Adaptive Proportion Test
 *
 * @param	data		[I]health test data
 * @param	next_sample	[I]test smalpe
 *
 * @return:	E_HM_OK success, caller needs to do test with new next sample
 *		E_HM_POSIX_FAULT failed, caller needs do fail process
 * @note	None
 * @see		NIST SP800-90B 4.4.2
 * @deprecated	None
 */
int health_ap_test(struct health_test_data *data, uint8_t next_sample);

/*
 * Batch Adaptive Proportion Test
 *
 * @param	data		[I]health test data
 * @param	next_sample	[I]test smalpe buffer addr
 * @param	sample_count	[I]test smalpe buffer size
 *
 * @return:	E_HM_OK success, caller needs to do test with new next sample
 *		E_HM_POSIX_FAULT failed, caller needs do fail process
 * @note	None
 * @see		NIST SP800-90B 4.4.2
 * @deprecated	None
 */
int health_ap_test_batch(struct health_test_data *data, const uint8_t *next_sample,
			 int sample_count);

/*
 * health startup test for entropy source
 *
 * @param	data_count [I]smaple nums this time
 *
 * @return	E_HM_OK finish test
 *		E_HM_AGAIN test continue
 * @note	None
 * @see		NIST SP800-90B 4.2
 * @deprecated	None
 */
int health_startup_test(unsigned int data_count);

#endif
