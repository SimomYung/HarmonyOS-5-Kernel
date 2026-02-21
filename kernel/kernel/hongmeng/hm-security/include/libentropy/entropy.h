/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Entropy collection for RNG head file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 11:49:29 2019
 */
#ifndef ULIBS_LIBENTROPY_ENTROPY_H
#define ULIBS_LIBENTROPY_ENTROPY_H

/* As extra parameter of entropy_read_randomness */
#define ENTROPY_BOOT_NOISE_SOURCE	0
#define ENTROPY_SYSMGR_NOISE_SOURCE	1
#define ENTROPY_DEVHOST_NOISE_SOURCE	2
#define ENTROPY_NOISE_SOURCE_MAX	3

unsigned int entropy_test_fail_count(void);

/*
 * init entropy health test data
 *
 * @param	rc_test_max_count [I] for Repetition Count Test
 * @param	ap_test_max_count [I] for Adaptive Proportion Test
 *
 * @return	None
 * @note	None
 * @see		None
 * @deprecated	None
 */
void entropy_init_health_data(int rc_test_max_count, int ap_test_max_count);

/*
 * store random event data
 *
 * @param	data [I] random event data source buffer
 * @param	data_len [I] random event data length
 * @param	entropy_src [I] random event source (now)
 *
 * @return	None
 * @note	None
 * @see		NIST SP800-90B section 3
 * @deprecated	None
 */
void entropy_add_randomness(const void *data, unsigned int data_len,
			    unsigned int entropy_src);

/*
 * Read random event data
 *
 * @param	out [O] output buffer
 * @param	out_buf_len [I] output buffer length
 * @param	fail_count [O] health test fail count
 *
 * @return	output data size
 * @note	fail_count will notify crypto finally
 * @see		NIST SP800-90B section 3
 * @deprecated	None
 */
int entropy_read_randomness(unsigned char *out, unsigned int out_buf_len,
			    unsigned int *fail_count);

#endif
