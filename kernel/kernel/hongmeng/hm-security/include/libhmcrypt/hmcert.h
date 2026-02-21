/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hand cert
 * Author: Huawei OS Kernel Lab
 * Create: Create: Tue July 25 12:10:24 2023
 */
#ifndef ULIBS_HMCERT_H
#define ULIBS_HMCERT_H

#include <stdint.h>

#define MAX_OWNERID_LEN 64
#define DESCRIPTION_LEN	128
#define CERT_CHAIN_PATH_LEN_MAX 3
#define CERT_SIGNER_NUM_MAX 3

struct hmcert_item_s {
	char *subject;
	char *issuer;
	char *desc;
	size_t desc_len;
};

#define CERT_MODE_UNINIT 0
#define CERT_MODE_LOCAL 1
#define CERT_MODE_CHAIN_PARTIAL 2
#define CERT_MODE_CHAIN_COMPLETE 3
#define CERT_MODE_MAX 4
struct hmcert_sig_info_s {
	struct hmcert_item_s certs[CERT_CHAIN_PATH_LEN_MAX];
	void *pkcs7;
	void *scert_digest;  /* signer cert digest */
	char *ownerid;
	/* description of local key signing the sig if MAY_LOCAL_CODE, otherwise NULLs */
	char *may_local_desp;
	size_t may_local_desp_len;
	size_t path_len;
	size_t ownerid_len;
	time_t signtime;
	int root_id;
	int signer_id;
	int mode;  /* local, partial, or complete */
};

#define SKID_LEN	(4)
#define PKALG_ELN	(32)
/* In proc keys, the follow message of a cert will output, where
 * skid: subject key identifier;
 * pk_alg: the alg of public key.
 */
struct cert_info {
	uint8_t skid[SKID_LEN];
	size_t skid_len;
	int pk_len;
	char pk_alg[PKALG_ELN];
};

int get_ownerid_nid(bool create);
int get_signtime_nid(void);
int find_ownerid(void* type, struct hmcert_sig_info_s *sig_info);
int find_signtime(void* type, time_t *signtime);
int hm_sig_one_cert(void *x509_cert, struct hmcert_item_s *item, void *pkcs7_sinfos,
	bool *is_root, bool *is_singer);
void __noinline hm_warn_openssl(void);
int hm_cert_description(const char *in_data, size_t in_data_len, char *description,
			size_t *description_len, struct cert_info *cinfo);
int hm_sig_issue_serial(const unsigned char *sig_data, size_t sig_data_len,
			char *description, size_t *description_len);
int hm_sig_info_read(const unsigned char *sig_data, size_t sig_data_len,
		     struct hmcert_sig_info_s **_sig_info);
int hm_sig_info_fini(struct hmcert_sig_info_s *sig_info);
int hm_signature_init(void);
int hm_fsverity_find_matched_root(struct hmcert_sig_info_s *sig_info, const char *in_data, size_t in_data_len);
bool hm_fsverity_cmp_digest(struct hmcert_sig_info_s *sig_info, const unsigned char *digest, size_t digest_len);
int hm_fsverity_set_digest(struct hmcert_sig_info_s *sig_info, unsigned char **__digest, size_t *__digest_len);
int hm_fsverity_verify_certchain(const char *cert_data, struct hmcert_sig_info_s *sig_info,
				 unsigned char *data, size_t data_len);
int hm_fsverity_verify(const char *in_data, const unsigned char *sig_data, size_t sig_data_len,
                       unsigned char *data, size_t data_len);

#endif
