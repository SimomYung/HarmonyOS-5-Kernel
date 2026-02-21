/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of ME-CF generators
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 19 14:08:23 2020
 */
/* Implementation of ME-CF random number generator from: L'ecuyer P. Maximally
 * equidistributed combined Tausworthe generators[J]. Mathematics of computation,
 * 1996, 65(213): 203-213.
 * Usage:
 * 1. Alloc a struct of prng_mecf_state_{short|middle|long};
 * 2. Call prng_mecf_{short|middle|long}_init() to init the state with random seeds,
 *    and the seeds should be from a secure random source, such as /dev/random.
 * 3. Call prng_mecf_{short|middle|long}() to generator random numbers.
 */
#include <hongmeng/errno.h>
#include <libprng/non_crypto_prng.h>
#include <libhmlog/hmlog.h>

/* Define Machine word */
#define L_64 64

#define quick_taus_stage1(A, K, Q, S) ((((A) << (Q)) ^ (A)) >> ((K) - (S)))
#define quick_taus_stage2(A, B, C, S) ((((A) & (C)) << (S)) ^ (B))
#define right_rotate_shift(input, bits) (((input) << ((L_64) - (bits))) | ((input) >> (bits)))

/* Parameters for short period length (2^(K1 + K2) = 2^60 bits)
 * (K1,K2,Q1,Q2,S1,S2) is set as (31,29,3,2,22,19)
 * Cx_S is a "mask" comprised of Kx ones followd by L - Kx zeros.
 */
#define K1_S 31
#define K2_S 29
#define Q1_S 3
#define Q2_S 2
#define	S1_S 22
#define S2_S 19
#define C1_S 0xFFFFFFFEU
#define C2_S 0xFFFFFFF8U

/* Parameters for middle period length (2^(K1 + ... + K4) = 2^113 bits)
 * (K1,K2,K3,K4,Q1,Q2,Q3,Q4,S1,S2,S3,S4) =
 * (31,29,28,25,06,02,13,03,18,02,07,13)
 * Cx_S is a "mask" comprised of Kx ones followd by L - Kx zeros.
 */
#define K1_M 31
#define K2_M 29
#define K3_M 28
#define K4_M 25
#define Q1_M 6
#define Q2_M 2
#define Q3_M 13
#define Q4_M 3
#define	S1_M 18
#define S2_M 2
#define S3_M 7
#define S4_M 13
#define C1_M 0xFFFFFFFEU
#define C2_M 0xFFFFFFF8U
#define C3_M 0xFFFFFFF0U
#define C4_M 0xFFFFFF80U

/* Parameters for long period length (2^(K1 + ... + K5) = 2^258 bits)
 * (K1,K2,K3,K4,K5,Q1,Q2,Q3,Q4,Q5,S1,S2,S3,S4,S5) =
 * (63,55,52,47,41,01,24,03,05,03,10,05,29,23,08)
 * Cx_S is a "mask" comprised of Kx ones followd by L - Kx zeros.
 */
#define K1_L 63
#define K2_L 55
#define K3_L 52
#define K4_L 47
#define K5_L 41
#define Q1_L 1
#define Q2_L 24
#define Q3_L 3
#define Q4_L 5
#define Q5_L 3
#define	S1_L 10
#define S2_L 5
#define S3_L 29
#define S4_L 23
#define S5_L 8
#define C1_L 0xFFFFFFFFFFFFFFFEULL
#define C2_L 0xFFFFFFFFFFFFFE00ULL
#define C3_L 0xFFFFFFFFFFFFF000ULL
#define C4_L 0xFFFFFFFFFFFE0000ULL
#define C5_L 0xFFFFFFFFFF800000ULL


static uint32_t check_state_u32(uint32_t state, uint32_t limit)
{
	return state < limit ? (state + limit) : state;
}
static uint64_t check_state_u64(uint64_t state, uint64_t limit)
{
	return state < limit ? (state + limit) : state;
}

/* Initiaze the state of mecf prng using a 64 bit seed:
 * z1 is set as the least significant 32 bits of seed
 * z2 is set as the most significant 32 bits of seed.
 * Make sure that the K_j most significant bits of state.z_j must be nonzero.
 * In other words, for prng_mecf_short, whose (K1, K2) = (31, 29) and L = 32,
 * z1, z2 MUST >= 2, 8 respectively.
 */
int prng_mecf_short_init(struct prng_mecf_state_short *state, uint64_t seed)
{
	if (state == NULL) {
		hm_error("state is NULL\n");
		return E_HM_INVAL;
	}

	state->z1 = check_state_u32((uint32_t)seed, 2U);
	state->z2 = check_state_u32((uint32_t)(seed >> 32), 8U);

	return E_HM_OK;
}

/*
 * Initialize the mecf prng state using a 64 bit seed:
 * z1 is set as the least significant 32 bits of seed
 * z2 is set as the [16, 47] bits of seed
 * z3 is set as the [32, 63] bits of seed
 * z4 is set as the combination of the [0, 15] and [48, 64] bits of seed
 * For prng_mecf_middle, whose (K1, K2, K3, K4) = (31, 29, 28, 25) and L = 32,
 * z1, z2, z3, z4 MUST >= 2, 8, 16, 128 respectively.
 */
int prng_mecf_middle_init(struct prng_mecf_state_middle *state, uint64_t seed)
{
	if (state == NULL) {
		hm_error("state is NULL\n");
		return E_HM_INVAL;
	}

	state->z1 = check_state_u32((uint32_t)seed, 2U);
	state->z2 = check_state_u32((uint32_t)(seed >> 16), 8U);
	state->z3 = check_state_u32((uint32_t)(seed >> 32), 16U);
	state->z4 = check_state_u32((uint32_t)right_rotate_shift(seed, 48), 128U);

	return E_HM_OK;
}

/*
 * Initialize the state of mecf prng using two 64 bit seeds: seed1 and seed2:
 * z1 is set as seed1
 * z2 is set as right rotation of seed1 by 32 bits
 * z3 is set as seed2
 * z4 is set as right rotation of seed2 by 32 bits
 * z5 is set as seed1 xor seed2
 * For prng_mecf_long, whose (K1, K2, K3, K4, k5) = (63, 55, 52, 47, 41) and
 * L = 64, z1, z2, z3, z4, z5 MUST >= (1 << (64 - K_i)) respectively.
 */
int prng_mecf_long_init(struct prng_mecf_state_long *state,
			uint64_t seed1, uint64_t seed2)
{
	if (state == NULL) {
		hm_error("state is NULL\n");
		return E_HM_INVAL;
	}

	state->z1 = check_state_u64(seed1, (uint64_t)1 << (L_64 - K1_L));
	state->z2 = check_state_u64(right_rotate_shift(seed1, 32), (uint64_t)1 << (L_64 - K2_L));
	state->z3 = check_state_u64(seed2, (uint64_t)1 << (L_64 - K3_L));
	state->z4 = check_state_u64(right_rotate_shift(seed2, 32), (uint64_t)1 << (L_64 - K4_L));
	state->z5 = check_state_u64(seed1 ^ seed2, (uint64_t)1 << (L_64 - K5_L));

	return E_HM_OK;
}

int prng_mecf_short(struct prng_mecf_state_short *state, uint32_t *result)
{
	uint32_t b;

	if (state == NULL || result == NULL) {
		hm_error("state or result is NULL\n");
		return E_HM_INVAL;
	}
	b = quick_taus_stage1(state->z1, K1_S, Q1_S, S1_S);
	state->z1 = quick_taus_stage2(state->z1, b, C1_S, S1_S);
	b = quick_taus_stage1(state->z2, K2_S, Q2_S, S2_S);
	state->z2 = quick_taus_stage2(state->z2, b, C2_S, S2_S);

	*result = state->z1 ^ state->z2;
	return E_HM_OK;
}

int prng_mecf_middle(struct prng_mecf_state_middle *state, uint32_t *result)
{
	uint32_t b;

	if (state == NULL || result == NULL) {
		hm_error("state or result is NULL\n");
		return E_HM_INVAL;
	}

	b = quick_taus_stage1(state->z1, K1_M, Q1_M, S1_M);
	state->z1 = quick_taus_stage2(state->z1, b, C1_M, S1_M);
	b = quick_taus_stage1(state->z2, K2_M, Q2_M, S2_M);
	state->z2 = quick_taus_stage2(state->z2, b, C2_M, S2_M);
	b = quick_taus_stage1(state->z3, K3_M, Q3_M, S3_M);
	state->z3 = quick_taus_stage2(state->z3, b, C3_M, S3_M);
	b = quick_taus_stage1(state->z4, K4_M, Q4_M, S4_M);
	state->z4 = quick_taus_stage2(state->z4, b, C4_M, S4_M);

	*result = state->z1 ^ state->z2 ^ state->z3 ^ state->z4;
	return E_HM_OK;
}

int prng_mecf_long(struct prng_mecf_state_long *state, uint64_t *result)
{
	uint64_t b;

	if (state == NULL || result == NULL) {
		hm_error("state or result is NULL\n");
		return E_HM_INVAL;
	}

	b = quick_taus_stage1(state->z1, K1_L, Q1_L, S1_L);
	state->z1 = quick_taus_stage2(state->z1, b, C1_L, S1_L);
	b = quick_taus_stage1(state->z2, K2_L, Q2_L, S2_L);
	state->z2 = quick_taus_stage2(state->z2, b, C2_L, S2_L);
	b = quick_taus_stage1(state->z3, K3_L, Q3_L, S3_L);
	state->z3 = quick_taus_stage2(state->z3, b, C3_L, S3_L);
	b = quick_taus_stage1(state->z4, K4_L, Q4_L, S4_L);
	state->z4 = quick_taus_stage2(state->z4, b, C4_L, S4_L);
	b = quick_taus_stage1(state->z5, K5_L, Q5_L, S5_L);
	state->z5 = quick_taus_stage2(state->z5, b, C5_L, S5_L);

	*result = state->z1 ^ state->z2 ^ state->z3 ^ state->z4 ^ state->z5;
	return E_HM_OK;
}
