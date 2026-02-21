/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Headers for ME-CF generators
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 19 14:10:47 2020
 */
#ifndef NON_CRYPTO_PRNG_H
#define NON_CRYPTO_PRNG_H

#include <hongmeng/types.h>

struct prng_mecf_state_short {
	uint32_t z1;
	uint32_t z2;
};

struct prng_mecf_state_middle {
	uint32_t z1;
	uint32_t z2;
	uint32_t z3;
	uint32_t z4;
};

struct prng_mecf_state_long {
	uint64_t z1;
	uint64_t z2;
	uint64_t z3;
	uint64_t z4;
	uint64_t z5;
};

int prng_mecf_short_init(struct prng_mecf_state_short *state, uint64_t seed);
int prng_mecf_middle_init(struct prng_mecf_state_middle *state, uint64_t seed);
int prng_mecf_long_init(struct prng_mecf_state_long *state, uint64_t seed1, uint64_t seed2);
int prng_mecf_short(struct prng_mecf_state_short *state, uint32_t *result);
int prng_mecf_middle(struct prng_mecf_state_middle *state, uint32_t *result);
int prng_mecf_long(struct prng_mecf_state_long *state, uint64_t *result);

#endif /* ifndef NON_CRYPTO_PRNG_H */
