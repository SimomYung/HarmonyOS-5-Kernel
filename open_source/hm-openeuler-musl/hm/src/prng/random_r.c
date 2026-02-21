/*
 * Copyright 2011-2012 Nicholas J. Kain, licensed under standard MIT license
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 6 14:38:24 2020
 */
#define _BSD_SOURCE
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

#define BOUNDARY_VALUE_0	8U
#define BOUNDARY_VALUE_1	32U
#define BOUNDARY_VALUE_2	64U
#define BOUNDARY_VALUE_3	128U
#define BOUNDARY_VALUE_4	256U

#define NUM_0	0U
#define NUM_1	7U
#define NUM_2	15U
#define NUM_3	31U
#define NUM_4	63U

#define SHIFT_VALUE_0	8
#define SHIFT_VALUE_1	16
#define SHIFT_VALUE_2	32

/*
 * LCG(Xn+1 = (A * Xn + B) % M) should satisfy:
 * 1. B and M are are coprime numbers.
 * 2. A-1 is divisible by all the prime factors of M.
 * 3. A-1 is divisible by 4 if m is divisible by 4.
 * 4. A < M and B < M and X0 < M
 */
static uint32_t lcg31(uint32_t x)
{
	return (1103515245U * x + (uint32_t)12345) & 0x7fffffffU; /* A:1103515245U, B:12345, M:2^31(0x7fffffffU + 1) */
}

static uint64_t lcg64(uint64_t x)
{
	return 6364136223846793005ULL * x + 1ULL; /* A:6364136223846793005ULL, B:1ULL, M:2^64 */
}

static void savestate_r(struct random_data *data)
{
	data->state[-1] = (data->num << SHIFT_VALUE_1) | 
			  (data->tail << SHIFT_VALUE_0) | data->head;
}

static void loadstate_r(uint32_t *state, struct random_data *data)
{
	data->state = state + 1;
	data->num = data->state[-1] >> SHIFT_VALUE_1;
	data->tail = (data->state[-1] >> SHIFT_VALUE_0) & 0xffU;
	data->head = data->state[-1] & 0xffU;
}

static void __srandom_r(unsigned seed, struct random_data *data)
{
	unsigned int k;
	unsigned int num = data->num;
	uint32_t *state = data->state;
	uint64_t s = seed;

	if (num == NUM_0) {
		state[0] = (uint32_t)seed;
	} else {
		/* 3 is coprime for 31 and 7 but not for 15 and 63 */
		data->tail = ((num == NUM_3) || (num == NUM_1)) ? 3 : 1;
		data->head = 0;
		for (k = 0; k < num; k++) {
			s = lcg64(s);
			state[k] = (uint32_t)(s >> SHIFT_VALUE_2);
		}
		/* make sure x contains at least one odd number */
		state[0] |= 1U;
	}
}

int srandom_r(unsigned seed, struct random_data *data)
{
	int ret = 0;
	if (data == NULL) {
		errno = EINVAL;
		ret = -1;
	}
	if (ret == 0) {
		__srandom_r(seed, data);
	}
	return ret;
}

int initstate_r(unsigned seed, char *state, size_t size, struct random_data *data)
{
	int ret = 0;
	if (state == NULL || data == NULL || size < BOUNDARY_VALUE_0) {
		errno = EINVAL;
		ret= -1;
	}
	if (ret == 0) {
		if (data->state != NULL) {
			savestate_r(data);
		}

		if (size < BOUNDARY_VALUE_1) {
			data->num = NUM_0;
		} else if (size < BOUNDARY_VALUE_2) {
			data->num = NUM_1;
		} else if (size < BOUNDARY_VALUE_3) {
			data->num = NUM_2;
		} else if (size < BOUNDARY_VALUE_4) {
			data->num = NUM_3;
		} else {
			data->num = NUM_4;
		}
		data->state = (unsigned int *)(uintptr_t)state + 1;
		__srandom_r(seed, data);
		savestate_r(data);
	}

	return ret;
}

int setstate_r(char *state, struct random_data *data)
{
	int ret = 0;
	if (state == NULL || data == NULL) {
		errno = EINVAL;
		ret = -1;
	}
	if (ret == 0) {
		savestate_r(data);
		loadstate_r((uint32_t*)(uintptr_t)state, data);
	}
	return ret;
}

int random_r(struct random_data *data, long *result)
{
	unsigned int num;
	int ret = 0;
	uint32_t *state = NULL;
	if (data == NULL || result == NULL) {
		errno = EINVAL;
		ret = -1;
	}
	if (ret == 0) {
		num = data->num;
		state = data->state;
		if (num == 0U) {
			state[0] = lcg31(state[0]);
			*result = (long)(unsigned long)(state[0]);
		} else {
			state[data->tail] += state[data->head];
			*result = (long)(unsigned long)(state[data->tail] >> 1U);
			if (++(data->tail) == num) {
				data->tail = 0;
			}
			if (++(data->head) == num) {
				data->head = 0;
			}
		}
	}

	return ret;
}
