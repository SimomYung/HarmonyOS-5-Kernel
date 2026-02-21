/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Encaps cred check head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 21:34:03 2024
 */

#ifndef ULIBS_LIBENCAPS_ENCAPS_CRED_H
#define ULIBS_LIBENCAPS_ENCAPS_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#define ENCAPS_VERSION_OUTDATE 1234U
#define MAX_ENCAPS_COUNT 64U
#define ENCAPS_INIT_VERSION 1U
#define ENCAPS_HAP_VERSION 2U

#define base_cred_to_encaps(b_cred) (sec_cred_idx_to_ptr(((struct base_cred *)(b_cred))->encaps_idx))

/* consistent with MAX_KEY_LEN in encaps_parse.h */
#define ENCAPS_MAX_KEY_LEN 64
struct encaps_for_prctl {
	uint32_t option;
	char *key;
	uint32_t key_len;
	uint32_t value_type;
	uint32_t value;
};

static inline void check_bcred_encaps(struct sec_cred_encaps *encaps_cred)
{
	UNUSED(encaps_cred);
	return;
}

static inline struct sec_cred_encaps *check_bcred_encaps_idx(const struct sec_chk_cred *cred, struct base_cred *b_cred)
{
	UNUSED(b_cred);
	return base_cred_to_encaps(cred->base_cred);
}

static inline struct sec_cred_encaps *encaps_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
	struct sec_cred_encaps *encaps_cred = NULL;
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (b_cred->encaps_idx == 0) {
		return encaps_cred;
	}
	encaps_cred = check_bcred_encaps_idx(cred, b_cred);
	check_bcred_encaps(encaps_cred);
	return encaps_cred;
}

bool sec_cred_check_encaps(struct sec_cred_encaps *encaps_cred, const char *key, uint32_t type, uint32_t value);
bool sec_cred_has_encaps(struct sec_cred_encaps *encaps_cred, const char *key);
int sec_cred_get_encaps(struct sec_cred_encaps *encaps_cred, const char *key, uint32_t *type, uint32_t *value);
bool cred_check_encaps_from_process(struct cred_encaps *encaps_cred, const char *key, uint32_t type, uint32_t value);
bool cred_has_encaps_from_process(struct cred_encaps *encaps_cred, const char *key);
int cred_get_encaps_from_process(struct cred_encaps *encaps_cred, const char *key, uint32_t *type, uint32_t *value);

#endif
