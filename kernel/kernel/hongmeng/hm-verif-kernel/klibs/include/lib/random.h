/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 13 15:52:25 2019
 */
#ifndef KLIBS_RANDOM_H
#define KLIBS_RANDOM_H
#include <lib/jitter.h>

#define __LIB_RANDOM_LCG96_RAND_NUM	63
struct random_state {
	unsigned long long lcg96_rand[__LIB_RANDOM_LCG96_RAND_NUM];

	int rand_idx;

	/*
	 * Flag whether it has been seeded
	 */
	int seed_flag;
};

void random_state_init(struct random_state *state);

void random_state_seed(struct random_state *state, unsigned int seed);
unsigned long random_state_get(struct random_state *state);
int random_state_bytes(struct random_state *state, unsigned char *buf, unsigned int size);
int random_state_seed_with_jitter(struct random_state *state, const struct jitter_mem_ctx *ctx);

void random_seed(unsigned int seed);
unsigned long random_get(void);
int random_bytes(unsigned char *buf, unsigned int size);
int random_seed_with_jitter(const struct jitter_mem_ctx *ctx);

#endif
