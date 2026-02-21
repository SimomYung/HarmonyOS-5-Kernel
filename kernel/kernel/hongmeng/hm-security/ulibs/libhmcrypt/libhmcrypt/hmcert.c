/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hand cert
 * Author: Huawei OS Kernel Lab
 * Create: Create: Tue July 25 12:10:24 2023
 */

#include <ctype.h>

#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/asn1t.h>
#include <openssl/ec.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/asn1.h>
#include <openssl/pkcs7.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <crypto/x509.h>
#include <stdint.h>

#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmcrypt/hmcert.h>
#include <libkeymgr/fsverity_api.h>
#include <libhmsrv_key/key.h>

static int trans_asn1_to_char(ASN1_INTEGER *num, char *str, size_t *str_len)
{
	BIGNUM *bn = NULL;
	char *tmp = NULL;
	size_t tmp_len;

	if (num == NULL || str == NULL || str_len == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	bn = ASN1_INTEGER_to_BN(num, NULL);
	if (bn == NULL) {
		hm_debug("failed to trans asn1 integer to bn\n");
		return E_HM_INVAL;
	}

	tmp = BN_bn2dec(bn);
	if (tmp == NULL) {
		hm_debug("failed to trans bn to str\n");
		BN_free(bn);
		return E_HM_INVAL;
	}

	tmp_len = strlen(tmp) + 1;
	if (*str_len < tmp_len) {
		hm_debug("the length of str too small\n");
		BN_free(bn);
		return E_HM_INVAL;
	}

	NOFAIL(memcpy_s(str, *str_len, tmp, tmp_len));
	*str_len = tmp_len;
	BN_free(bn);
	OPENSSL_free(tmp);
	return E_HM_OK;
}

#define CERT_LIST_TYPE	(0)
#define CERT_SINGLE_TYPE	(1)

struct x509_mem_item {
	BIO *cbio;
	STACK_OF(X509_INFO) *inf;
	X509 *cert;
	STACK_OF(X509) *certs;
	int type;
};

#define OID_LEN	(64)
#define	OID_ECDSA_SHA256	"1.2.840.10045.4.3.2"
#define OID_ECDSA_SHA256_T	"1.2.840.113549.1.1.11"
#define	OID_ECDSA_SHA384 	"1.2.840.10045.4.3.3"
#define ECDSA_SHA256	"ecdsa-nist-p256"
#define ECDSA_SHA384	"ecdsa-nist-p384"

static int get_pk_alg(X509 *cert, struct cert_info *cinfo)
{
	char oid[OID_LEN + 1] = {0};
	int ret;

	ret = OBJ_obj2txt(oid, OID_LEN, cert->sig_alg.algorithm, 1);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	if (strncmp(oid, OID_ECDSA_SHA256, sizeof(OID_ECDSA_SHA256)) == 0) {
		NOFAIL(memcpy_s(cinfo->pk_alg, PKALG_ELN, ECDSA_SHA256, sizeof(ECDSA_SHA256)));
	} else if (strncmp(oid, OID_ECDSA_SHA256_T, sizeof(OID_ECDSA_SHA256_T)) == 0) {
		NOFAIL(memcpy_s(cinfo->pk_alg, PKALG_ELN, ECDSA_SHA256, sizeof(ECDSA_SHA256)));
	} else if (strncmp(oid, OID_ECDSA_SHA384, sizeof(OID_ECDSA_SHA384)) == 0) {
		NOFAIL(memcpy_s(cinfo->pk_alg, PKALG_ELN, ECDSA_SHA384, sizeof(ECDSA_SHA384)));
	} else {
		hm_debug("keyring: pk alg not support\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static int get_cert_info(X509 *cert, struct cert_info *cinfo)
{
	EVP_PKEY *pkey = NULL;
	const ASN1_OCTET_STRING *skid = NULL;
	unsigned char *str = NULL;
	int str_len;
	int ret;

	if (cinfo == NULL) {
		return E_HM_OK;
	}

	pkey = X509_get0_pubkey(cert);
	if (pkey == NULL) {
		return E_HM_INVAL;
	}
	cinfo->pk_len = EVP_PKEY_get_bits(pkey);

	skid = X509_get0_subject_key_id(cert);
	if (skid == NULL) {
		str_len = 0;
	} else {
		str_len = i2d_ASN1_OCTET_STRING(skid, &str);
		if (str_len < 0) {
			hm_error("keyring failed to call i2d_ASN1_OCTET_STRING\n");
			return E_HM_INVAL;
		}
	}

	if (str_len > SKID_LEN) {
		NOFAIL(memcpy_s(cinfo->skid, SKID_LEN, str + (str_len - SKID_LEN), SKID_LEN));
		cinfo->skid_len = SKID_LEN;
	} else if (str_len > 0) {
		NOFAIL(memcpy_s(cinfo->skid, SKID_LEN, str, (size_t)str_len));
		cinfo->skid_len = (size_t)str_len;
	} else {
		cinfo->skid_len = (size_t)str_len;
	}
	if (str_len > 0) {
		OPENSSL_clear_free(str, (size_t)str_len);
	}
	ret = get_pk_alg(cert, cinfo);

	return ret;
}

static int x509_mem_item_init_list(const char *in_data, size_t in_data_len,
				   struct x509_mem_item *mem_item, struct cert_info *cinfo)
{
	BIO *cbio = NULL;
	STACK_OF(X509_INFO) *inf = NULL;
	X509_INFO *itmp = NULL;
	X509 *cert = NULL;
	STACK_OF(X509) *certs = NULL;

	cbio = BIO_new_mem_buf((void*)in_data, (int)in_data_len);
	if (cbio == NULL) {
		hm_debug("failed to get cbio\n");
		return E_HM_INVAL;
	}

	inf = PEM_X509_INFO_read_bio(cbio, NULL, NULL, NULL);
	if (inf == NULL) {
		goto free_cbio;
	}

	itmp = sk_X509_INFO_value(inf, sk_X509_INFO_num(inf) - 1);
	if (itmp == NULL || itmp->x509 == NULL) {
		goto free_inf;
	}

	cert = itmp->x509;
	if (cinfo != NULL && get_cert_info(cert, cinfo) != E_HM_OK) {
		goto free_inf;
	}

	certs = sk_X509_new_null();
	if (certs == NULL) {
		goto free_inf;
	}

	if (sk_X509_push(certs, cert) <= 0) {
		goto free_certs;
	}

	mem_item->cbio = cbio;
	mem_item->inf = inf;
	mem_item->cert = itmp->x509;
	mem_item->certs = certs;
	mem_item->type = CERT_LIST_TYPE;

	return E_HM_OK;

free_certs:
	sk_X509_free(certs);
free_inf:
	sk_X509_INFO_pop_free(inf, X509_INFO_free);
free_cbio:
	BIO_free(cbio);

	return E_HM_INVAL;
}

static int x509_mem_item_init_single(const unsigned char *in_data, size_t in_data_len,
				     struct x509_mem_item *mem_item, struct cert_info *cinfo)
{
	X509 *cert = NULL;
	STACK_OF(X509) *certs = NULL;

	cert = d2i_X509(NULL, &in_data, (long)in_data_len);
	if (cert == NULL) {
		hm_debug("failed to call d2i_X509\n");
		return E_HM_INVAL;
	}

	if (cinfo != NULL && get_cert_info(cert, cinfo) != E_HM_OK) {
		goto err_out;
	}

	certs = sk_X509_new_null();
	if (certs == NULL) {
		goto err_out;
	}

	if (sk_X509_push(certs, cert) <= 0) {
		goto free_certs;
	}

	mem_item->cbio = NULL;
	mem_item->inf = NULL;
	mem_item->cert = cert;
	mem_item->certs = certs;
	mem_item->type = CERT_SINGLE_TYPE;

	return E_HM_OK;

free_certs:
	sk_X509_free(certs);
err_out:
	X509_free(cert);

	return E_HM_INVAL;
}

static int x509_mem_item_init(const char *in_data, size_t in_data_len, struct x509_mem_item *mem_item,
			      struct cert_info *cinfo)
{
	int ret;

	ret = x509_mem_item_init_single((const unsigned char *)in_data, in_data_len, mem_item, cinfo);
	if (ret != E_HM_OK) {
		ret = x509_mem_item_init_list(in_data, in_data_len, mem_item, cinfo);
	}

	return ret;
}

static void x509_mem_item_fini(struct x509_mem_item *mem_item)
{
	if (mem_item == NULL) {
		return;
	}

	if (mem_item->certs != NULL) {
		sk_X509_free(mem_item->certs);
		mem_item->certs = NULL;
	}

	if (mem_item->inf != NULL) {
		sk_X509_INFO_pop_free(mem_item->inf, X509_INFO_free);
		mem_item->inf = NULL;
	}

	if (mem_item->cbio != NULL) {
		BIO_free(mem_item->cbio);
		mem_item->cbio = NULL;
	}

	if ((mem_item->type == CERT_SINGLE_TYPE) && (mem_item->cert != NULL)) {
		X509_free(mem_item->cert);
		mem_item->cert = NULL;
	}
}

static int get_cert_description(X509_NAME *issuer, ASN1_INTEGER *serial,
				char *description, size_t *description_len)
{
	size_t issuer_len;
	size_t serial_len = 0;
	char *issuer_name = NULL;
	int err = E_HM_INVAL;

	if (issuer == NULL) {
		return E_HM_INVAL;
	}

	issuer_name = X509_NAME_oneline(issuer, NULL, 0);
	if (issuer_name == NULL) {
		return E_HM_INVAL;
	}

	issuer_len = strlen(issuer_name);
	if (*description_len < issuer_len) {
		hm_debug("description len not enough, %d, %d\n", (int)issuer_len, (int)*description_len);
		goto out;
	}
	NOFAIL(memcpy_s(description, *description_len, issuer_name, issuer_len));
	if (serial != NULL) {
		serial_len = *description_len - issuer_len;
		err = trans_asn1_to_char(serial, description + issuer_len, &serial_len);
		if (err != E_HM_OK) {
			goto out;
		}
	}
	err = E_HM_OK;
	*description_len = serial_len + issuer_len;
out:
	OPENSSL_free(issuer_name);
	return err;
}

int hm_cert_description(const char *in_data, size_t in_data_len, char *description,
			size_t *description_len, struct cert_info *cinfo)
{
	X509 *cert = NULL;
	struct x509_mem_item mem_item = {0};
	X509_NAME *issuer = NULL;
	ASN1_INTEGER *serial = NULL;

	int ret;

	if (in_data == NULL || in_data_len == 0 || description == NULL || description_len == NULL ||
	    cinfo == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = x509_mem_item_init(in_data, in_data_len, &mem_item, cinfo);
	if (ret != E_HM_OK) {
		return E_HM_INVAL;
	}

	cert = mem_item.cert;
	issuer = X509_get_issuer_name(cert);
	serial = X509_get_serialNumber(cert);
	if (issuer == NULL || serial == NULL) {
		x509_mem_item_fini(&mem_item);
		return E_HM_INVAL;
	}

	ret = get_cert_description(issuer, serial, description, description_len);
	if (ret != E_HM_OK) {
		NOFAIL(memset_s(description, *description_len, 0, *description_len));
	}

	x509_mem_item_fini(&mem_item);
	return ret;
}

int hm_sig_issue_serial(const unsigned char *sig_data, size_t sig_data_len, char *description, size_t *description_len)
{
	PKCS7 *sig = NULL;
	STACK_OF(PKCS7_SIGNER_INFO) *sk = NULL;
	PKCS7_SIGNER_INFO *signinfo = NULL;
	PKCS7_ISSUER_AND_SERIAL *issue_serial = NULL;
	int ret;

	if (sig_data == NULL || sig_data_len == 0 || description == NULL || description_len == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}
	sig = d2i_PKCS7(NULL, &sig_data, (long)sig_data_len);
	if (sig == NULL) {
		hm_debug("failed to get pkcs7 data\n");
		return E_HM_INVAL;
	}

	sk = PKCS7_get_signer_info(sig);
	if (sk == NULL) {
		hm_debug("failed to get signer info\n");
		PKCS7_free(sig);
		return E_HM_INVAL;
	}

	signinfo = sk_PKCS7_SIGNER_INFO_value(sk, 0);
	if (signinfo == NULL) {
		hm_debug("failed to get signer value\n");
		PKCS7_free(sig);
		return E_HM_INVAL;
	}

	issue_serial = signinfo->issuer_and_serial;
	if (issue_serial == NULL) {
		hm_debug("failed to get issuer and serial\n");
		PKCS7_free(sig);
		return E_HM_INVAL;
	}

	ret = get_cert_description(issue_serial->issuer, issue_serial->serial, description, description_len);
	if (ret != E_HM_OK) {
		NOFAIL(memset_s(description, *description_len, 0, *description_len));
	}
	PKCS7_free(sig);
	return ret;
}

#define OWNERID_OID "1.3.6.1.4.1.2011.2.376.1.4.1"
#define OWNERID_OID_SHORT_NAME "ownerID"
#define OWNERID_OID_LONG_NAME "Code Signature Owner ID"

static int get_ownerid_nid(bool create)
{
	int nid;
	nid = OBJ_txt2nid(OWNERID_OID);
	if (nid == NID_undef && create) {
		OBJ_create(OWNERID_OID, OWNERID_OID_SHORT_NAME, OWNERID_OID_LONG_NAME);
		nid = OBJ_txt2nid(OWNERID_OID);
	}
	return nid;
}

#define SIGNTIME_OID "1.2.840.113549.1.9.5"

int get_signtime_nid(void)
{
	int nid;
	nid = OBJ_txt2nid(SIGNTIME_OID);
	return nid;
}

static int find_ownerid(ASN1_TYPE* asn1_type, struct hmcert_sig_info_s *sig_info)
{
	ASN1_STRING *result = NULL;
	char *ownerid = NULL;
	size_t len;

	if (asn1_type == NULL || asn1_type->type != V_ASN1_UTF8STRING) {
		return E_HM_INVAL;
	}

	result = asn1_type->value.asn1_string;
	if (result == NULL) {
		return E_HM_INVAL;
	}
	len = (size_t)ASN1_STRING_length(result);
	if (len == 0 || len > MAX_OWNERID_LEN) {
		return E_HM_INVAL;
	}
	ownerid = (char *)malloc(len + 1);
	if (ownerid == NULL) {
		return E_HM_NOMEM;
	}
	NOFAIL(memcpy_s(ownerid, len + 1, (const char *)ASN1_STRING_get0_data(result), len));
	ownerid[len] = 0;
	sig_info->ownerid = ownerid;
	sig_info->ownerid_len = len;

	return E_HM_OK;
}

#define SECONDS 3600
#define HOURS 24
int find_signtime(void* type, time_t *signtime)
{
	int err = E_HM_INVAL;
	int day, sec;
	ASN1_TIME *epoch = NULL;
	time_t t = 0;  /* 0 second since epoch */
	ASN1_TIME *st;

	ASN1_TYPE *asn1_type = (ASN1_TYPE *)type;

	if (asn1_type == NULL ||
	    (asn1_type->type != V_ASN1_UTCTIME && asn1_type->type != V_ASN1_GENERALIZEDTIME)) {
		return err;
	}

	st = (ASN1_TIME *)asn1_type->value.ptr;
	epoch = X509_time_adj(NULL, 0, &t);
	if (epoch == NULL) {
		return err;
	}
	if (ASN1_TIME_diff(&day, &sec, epoch, st) == 0) {
		goto out;
	}

	*signtime = day * SECONDS * HOURS + sec;
	err = E_HM_OK;

out:
	ASN1_TIME_free(epoch);
	return err;
}

static int __noinline hm_sig_attributes(STACK_OF(PKCS7_SIGNER_INFO) *sk, struct hmcert_sig_info_s *sig_info)
{
	int err = E_HM_INVAL;
	PKCS7_SIGNER_INFO *sign_info = NULL;
	ASN1_TYPE* asn1_type = NULL;
	int nid_ownerid, nid_signtime;

	BUG_ON(sk == NULL);
	BUG_ON(sig_info == NULL);

	nid_ownerid = get_ownerid_nid(false);
	if (nid_ownerid == NID_undef) {
		hm_warn("fsverity certchain: failed to create nid for ownerid\n");
		return err;
	}
	nid_signtime = get_signtime_nid();
	if (nid_signtime == NID_undef) {
		hm_warn("fsverity certchain: failed to get nid for signtime\n");
		return err;
	}

	sign_info = sk_PKCS7_SIGNER_INFO_value(sk, 0);
	BUG_ON(sign_info == NULL);
	asn1_type = PKCS7_get_signed_attribute(sign_info, nid_ownerid);
	err = find_ownerid(asn1_type, sig_info);
	if (err == E_HM_NOMEM) {
		return err;
	}

	asn1_type = PKCS7_get_signed_attribute(sign_info, nid_signtime);
	(void)find_signtime(asn1_type, &sig_info->signtime);
	/* OK if no found */
	return E_HM_OK;
}

struct cert_name_ctx_s {
	char *o_data;
	char *cn_data;
	char *email_data;
	size_t o_size;
	size_t cn_size;
	size_t email_size;
};

#define BASE_TEN 10
static void cert_name_ctx_fini(struct cert_name_ctx_s *ctx)
{
	if (ctx -> o_data != NULL) {
		free(ctx->o_data);
	}
	if (ctx->cn_data != NULL) {
		free(ctx->cn_data);
	}
	if (ctx->email_data != NULL) {
		free(ctx->email_data);
	}
}

static uint8_t hex2dec(char hex)
{
	if (hex >= '0' && hex <= '9') {
		return ((uint8_t)hex - '0');
	}
	if (hex >= 'a' && hex <= 'f') {
		return ((uint8_t)hex - 'a' + BASE_TEN);
	}
	if (hex >= 'A' && hex <= 'F') {
		return ((uint8_t)hex - 'A' + BASE_TEN);
	}

	return 0U;
}

#define OFFSET_THREE 3
#define OFFSET_TWO 2
#define OFFSET_FOUR 4
static char *alloc_with_convert_hex(char *buf)
{
	size_t index = 0, offset = 0;
	size_t len;
	char *new_buf = NULL;

	BUG_ON(buf == NULL);

	len = strlen(buf);
	new_buf = malloc(len + 1);
	if (new_buf == NULL) {
		hm_warn("no mem\n");
		return NULL;
	}

	NOFAIL(memset_s(new_buf, len + 1, 0, len + 1));
	while (index < len) {
		if (index + OFFSET_THREE < len && buf[index] == '\\' &&
		    (buf[index + 1] == 'x' || buf[index + 1] == 'X') &&
		    isxdigit(buf[index + OFFSET_TWO]) && isxdigit(buf[index + OFFSET_THREE])) {
			new_buf[offset] = (char)((hex2dec(buf[index + OFFSET_TWO]) << OFFSET_FOUR) |
					   hex2dec(buf[index + OFFSET_THREE]));
			index += OFFSET_FOUR;
		} else {
			new_buf[offset] = buf[index];
			index++;
		}
		offset++;
	}
	return new_buf;
}

#define O_HEAD "O="
#define O_HEAD_SIZE 2
#define CN_HEAD "CN="
#define CN_HEAD_SIZE 3
#define EMAIL_HEAD "E="
#define EMAIL_HEAD_SIZE 2
#define SIZE_LIMIT 7

static int cert_name_parse_ctx(char *buf, struct cert_name_ctx_s *ctx)
{
	char *token = NULL;
	char *tmp = NULL;

	BUG_ON(buf == NULL);
	BUG_ON(ctx == NULL);

	token = strtok_r(buf, "/", &tmp);
	while (token != NULL) {
		if (ctx->o_data == NULL && memcmp(token, O_HEAD, O_HEAD_SIZE) == 0) {
			ctx->o_data = alloc_with_convert_hex(token + O_HEAD_SIZE);
			if (ctx->o_data == NULL) {
				return E_HM_INVAL;
			}
			ctx->o_size = strlen(ctx->o_data);
		} else if (ctx->cn_data == NULL && memcmp(token, CN_HEAD, CN_HEAD_SIZE) == 0) {
			ctx->cn_data = alloc_with_convert_hex(token + CN_HEAD_SIZE);
			if (ctx->cn_data == NULL) {
				return E_HM_INVAL;
			}
			ctx->cn_size = strlen(ctx->cn_data);
		} else if (ctx->email_data == NULL && memcmp(token, EMAIL_HEAD, EMAIL_HEAD_SIZE) == 0) {
			ctx->email_data = alloc_with_convert_hex(token + EMAIL_HEAD_SIZE);
			if (ctx->email_data == NULL) {
				return E_HM_INVAL;
			}
			ctx->email_size = strlen(ctx->email_data);
		}
		if (ctx->o_data != NULL && ctx->cn_data != NULL) {
			break;
		}
		token = strtok_r(NULL, "/", &tmp);
	}

	return E_HM_OK;
}

static char *generate_single_name(char *src_data, size_t src_size)
{
	char *name = NULL;

	if (src_data == NULL) {
		return NULL;
	}

	name = (char *)malloc(src_size + 1);
	if (name != NULL) {
		NOFAIL(memcpy_s(name, src_size, src_data, src_size));
		name[src_size] = 0;
	}

	return name;
}

static char *generate_complex_name(char *o_data, size_t o_size, char *cn_data, size_t cn_size)
{
	char *name = NULL;
	if (o_data == NULL || cn_data == NULL) {
		return NULL;
	}
	name = (char *)malloc(o_size + OFFSET_TWO + cn_size + 1);
	if (name != NULL) {
		NOFAIL(memcpy_s(name, o_size, o_data, o_size));
		name[o_size] = ':';
		name[o_size + 1] = ' ';
		NOFAIL(memcpy_s(name + o_size + OFFSET_TWO, cn_size, cn_data, cn_size));
		name[o_size + OFFSET_TWO + cn_size] = 0;
	}

	return name;
}
/*
 * Fabricate and save the issuer and subject names, following x509_fabricate_name on linux
 */
static int cert_fabricate_name(char *buf, char **_name)
{
	int err;
	struct cert_name_ctx_s ctx = {0};
	char *name = NULL;

	BUG_ON(buf == NULL);

	err = cert_name_parse_ctx(buf, &ctx);
	if (err != E_HM_OK) {
		goto out;
	}

	if (ctx.o_data != NULL && ctx.cn_data != NULL) {
		/* Consider combining O and CN, but use only the CN if it is
		 * prefixed by the O, or a significant portion thereof.
		 */
		if (ctx.cn_size >= ctx.o_size && memcmp(ctx.cn_data, ctx.o_data, ctx.o_size) == 0) {
			name = generate_single_name(ctx.cn_data, ctx.cn_size);
		} else if (ctx.cn_size >= SIZE_LIMIT && ctx.o_size >= SIZE_LIMIT &&
			   memcmp(ctx.cn_data, ctx.o_data, SIZE_LIMIT) == 0) {
			name = generate_single_name(ctx.cn_data, ctx.cn_size);
		} else {
			name = generate_complex_name(ctx.o_data, ctx.o_size, ctx.cn_data, ctx.cn_size);
		}
	} else if (ctx.cn_data != NULL) {
		name = generate_single_name(ctx.cn_data, ctx.cn_size);
	} else if (ctx.o_data != NULL) {
		name = generate_single_name(ctx.o_data, ctx.o_size);
	} else {
		name = generate_single_name(ctx.email_data, ctx.email_size);
	}

	if (name != NULL) {
		*_name = name;
		err = E_HM_OK;
	}
out:
	cert_name_ctx_fini(&ctx);
	return err;
}


static int cert_get_name(X509_NAME *xname, char **_name)
{
	int err = E_HM_INVAL;
	char *name_buf;

	if (xname == NULL) {
		return err;
	}

	name_buf = X509_NAME_oneline(xname, NULL, 0);
	if (name_buf != NULL) {
		err = cert_fabricate_name(name_buf, _name);
		OPENSSL_free(name_buf);
	}

	return err;
}

static int hm_sig_local_certs(STACK_OF(PKCS7_SIGNER_INFO) *sinfos, struct hmcert_sig_info_s *sig_info)
{
	int err;
	PKCS7_SIGNER_INFO *si = NULL;
	PKCS7_ISSUER_AND_SERIAL *ias = NULL;

	BUG_ON(sinfos == NULL);
	BUG_ON(sig_info == NULL);

	si = sk_PKCS7_SIGNER_INFO_value(sinfos, 0);
	ias = si->issuer_and_serial;
	sig_info->may_local_desp = malloc(DESCRIPTION_LEN);
	if (sig_info->may_local_desp == NULL) {
		return E_HM_NOMEM;
	}
	sig_info->may_local_desp_len = DESCRIPTION_LEN;
	NOFAIL(memset_s(sig_info->may_local_desp, DESCRIPTION_LEN, 0, DESCRIPTION_LEN));

	err = get_cert_description(ias->issuer, ias->serial,
				   sig_info->may_local_desp, &sig_info->may_local_desp_len);
	if (err != E_HM_OK) {
		return err;
	}

	return E_HM_OK;
}

int hm_signature_init(void)
{
	int nid = get_ownerid_nid(true);
	if (nid == NID_undef) {
		hm_warn("fsverity certchain, create ownerid nid failed\n");
		return E_HM_INVAL;
	}
	return E_HM_OK;
}

int hm_sig_one_cert(void *x509_cert, struct hmcert_item_s *item, void *pkcs7_sinfos,
	bool *is_root, bool *is_singer)
{
	PKCS7_SIGNER_INFO *si = NULL;
	PKCS7_ISSUER_AND_SERIAL *ias = NULL;

	X509 *cert = (X509 *)x509_cert;
	STACK_OF(PKCS7_SIGNER_INFO) *sinfos = (STACK_OF(PKCS7_SIGNER_INFO) *)pkcs7_sinfos;

	if (cert == NULL || item == NULL || sinfos == NULL) {
		return E_HM_INVAL;
	}

	X509_NAME *subject =  X509_get_subject_name(cert);
	X509_NAME *issuer = X509_get_issuer_name(cert);
	ASN1_INTEGER *serial = X509_get_serialNumber(cert);

	if (cert_get_name(subject, &item->subject) != E_HM_OK ||
	    cert_get_name(issuer, &item->issuer) != E_HM_OK) {
		return E_HM_INVAL;
	}

	si = sk_PKCS7_SIGNER_INFO_value(sinfos, 0);
	ias = si->issuer_and_serial;
	if (ASN1_INTEGER_cmp(ias->serial, serial) == 0 && X509_NAME_cmp(ias->issuer, issuer) == 0) {
		/* find signer ca */
		*is_singer = true;
	}

	if (X509_NAME_cmp(subject, issuer) == 0) {
		/* find root ca */
		*is_root = true;
	} else {
		serial = NULL;
	}
	item->desc = malloc(DESCRIPTION_LEN);
	if (item->desc == NULL) {
		return E_HM_NOMEM;
	}
	NOFAIL(memset_s(item->desc, DESCRIPTION_LEN, 0, DESCRIPTION_LEN));
	item->desc_len = DESCRIPTION_LEN;

	return get_cert_description(issuer, serial, item->desc, &item->desc_len);
}

static int __noinline hm_sig_certs(STACK_OF(X509) *certs, STACK_OF(PKCS7_SIGNER_INFO) *sinfos,
				   struct hmcert_sig_info_s *sig_info)
{
	int err;
	int i;
	int n_certs;
	bool is_root;
	bool is_signer;

	BUG_ON(sinfos == NULL);
	BUG_ON(sig_info == NULL);

	sig_info->path_len = 0;
	sig_info->signer_id = CERT_CHAIN_PATH_LEN_MAX;
	sig_info->root_id = CERT_CHAIN_PATH_LEN_MAX;

	if (certs == NULL) {
		/* no certs here, may signed by local key, MAY_LOCAL_CODE */
		sig_info->mode = CERT_MODE_LOCAL;
		return hm_sig_local_certs(sinfos, sig_info);
	}

	sig_info->mode = CERT_MODE_CHAIN_PARTIAL;
	n_certs = sk_X509_num(certs);
	if (n_certs > CERT_CHAIN_PATH_LEN_MAX) {
		hm_debug("the number of certs is over the limited\n");
		return E_HM_INVAL;
	}

	sig_info->path_len = (size_t)n_certs;
	for (i = 0; i < n_certs; i++) {
		is_root = false;
		is_signer = false;
		err = hm_sig_one_cert(sk_X509_value(certs, i), &(sig_info->certs[i]),
			sinfos, &is_root, &is_signer);
		if (err != E_HM_OK) {
			return err;
		}
		if (sig_info->signer_id == CERT_CHAIN_PATH_LEN_MAX && is_signer) {
			sig_info->signer_id = i;
			sig_info->scert_digest = X509_digest_sig(sk_X509_value(certs, i), NULL, NULL);
		}

		if (sig_info->root_id == CERT_CHAIN_PATH_LEN_MAX && is_root) {
			sig_info->root_id = i;
			sig_info->mode = CERT_MODE_CHAIN_COMPLETE;
		}
	}
	return E_HM_OK;
}

int __noinline hm_sig_info_read(const unsigned char *sig_data, size_t sig_data_len,
				struct hmcert_sig_info_s **_sig_info)
{
	int err = E_HM_INVAL;
	PKCS7 *sig = NULL;
	STACK_OF(PKCS7_SIGNER_INFO) *sinfos = NULL;
	struct hmcert_sig_info_s *sig_info = NULL;

	if (sig_data == NULL || sig_data_len == 0 || _sig_info == NULL) {
		hm_warn("fsverity certchain, invalid input params to read sig_info\n");
		return err;
	}

	sig_info = (struct hmcert_sig_info_s *)malloc(sizeof(struct hmcert_sig_info_s));
	if (sig_info == NULL) {
		return E_HM_NOMEM;
	}
	NOFAIL(memset_s(sig_info, sizeof(struct hmcert_sig_info_s), 0, sizeof(struct hmcert_sig_info_s)));

	sig = d2i_PKCS7(NULL, &sig_data, (long)sig_data_len);
	if (sig == NULL) {
		hm_warn("fsverity cerchain, failed to get pkcs7\n");
		goto err_out;
	}
	sig_info->pkcs7 = sig;

	sinfos = PKCS7_get_signer_info(sig);
	if (sinfos == NULL || sk_PKCS7_SIGNER_INFO_num(sinfos) == 0) {
		goto err_out;
	}

	err = hm_sig_attributes(sinfos, sig_info);
	if (err != E_HM_OK) {
		hm_warn("fsverity certchain, failed to get attributes\n");
		goto err_out;
	}

	err = hm_sig_certs(sig->d.sign->cert, sinfos, sig_info);
	if (err != E_HM_OK) {
		hm_warn("fsverity certchain, failed to get certs\n");
		goto err_out;
	}

	*_sig_info = sig_info;
	return E_HM_OK;

err_out:
	hm_sig_info_fini(sig_info);
	return err;
}

int __noinline hm_sig_info_fini(struct hmcert_sig_info_s *sig_info)
{
	struct hmcert_item_s *item;
	size_t i;
	if (sig_info != NULL) {
		for (i = 0; i < sig_info->path_len; i++) {
			item = &(sig_info->certs[i]);
			if (item->subject != NULL) {
				free(item->subject);
			}
			if (item->issuer != NULL) {
				free(item->issuer);
			}
			if (item->desc != NULL) {
				free(item->desc);
			}
		}
		if (sig_info->may_local_desp != NULL) {
			free(sig_info->may_local_desp);
		}
		if (sig_info->ownerid != NULL) {
			free(sig_info->ownerid);
		}
		if (sig_info->scert_digest != NULL) {
			ASN1_OCTET_STRING_free((ASN1_OCTET_STRING *)sig_info->scert_digest);
		}
		if (sig_info->pkcs7 != NULL) {
			PKCS7_free(sig_info->pkcs7);
		}

		NOFAIL(memset_s(sig_info, sizeof(struct hmcert_sig_info_s), 0, sizeof(struct hmcert_sig_info_s)));
		free(sig_info);
	}

	return E_HM_OK;
}

int hm_fsverity_find_matched_root(struct hmcert_sig_info_s *sig_info, const char *in_data, size_t in_data_len)
{
	int err = E_HM_INVAL;
	X509 *root = NULL;
	struct hmcert_item_s *cert = NULL;
	X509_NAME *root_subject = NULL;
	char *root_name = NULL;
	struct x509_mem_item mem_item = {0};
	size_t i;

	err = x509_mem_item_init(in_data, in_data_len, &mem_item, NULL);
	if (err != E_HM_OK) {
		goto out;
	}

	err = E_HM_INVAL;

	root = mem_item.cert;
	root_subject = X509_get_subject_name(root);
	if (root_subject == NULL) {
		goto out;
	}
	root_name = X509_NAME_oneline(root_subject, NULL, 0);
	if (root_name == NULL) {
		goto out;
	}

	for (i = 0; i < sig_info->path_len; i++) {
		cert = &(sig_info->certs[i]);
		if (strcmp(cert->desc, root_name) == 0) {
			err = E_HM_OK;
			break;
		}
	}
	OPENSSL_free(root_name);

out:
	x509_mem_item_fini(&mem_item);
	return err;
}

bool hm_fsverity_cmp_digest(struct hmcert_sig_info_s *sig_info, const unsigned char *digest, size_t digest_len)
{
	ASN1_OCTET_STRING *scert_digest;

	BUG_ON_D(sig_info == NULL);

	scert_digest = (ASN1_OCTET_STRING *)sig_info->scert_digest;
	if (scert_digest == NULL || scert_digest->data == NULL || scert_digest->length == 0) {
		return false;
	}

	if (digest == NULL || digest_len == 0) {
		return false;
	}

	if (digest_len != (size_t)scert_digest->length) {
		return false;
	}

	if (memcmp(scert_digest->data, digest, digest_len) != 0) {
		return false;
	}

	return true;
}

int hm_fsverity_set_digest(struct hmcert_sig_info_s *sig_info, unsigned char **__digest, size_t *__digest_len)
{
	unsigned char *digest = NULL;
	ASN1_OCTET_STRING *scert_digest;

	BUG_ON_D(sig_info == NULL || __digest == NULL || __digest_len == NULL);

	scert_digest = (ASN1_OCTET_STRING *)sig_info->scert_digest;
	if (scert_digest == NULL || scert_digest->data == NULL || scert_digest->length <= 0) {
		return E_HM_INVAL;
	}

	digest = malloc((size_t)scert_digest->length);
	if (digest == NULL) {
		return E_HM_NOMEM;
	}

	NOFAIL(memcpy_s(digest, (size_t)scert_digest->length, scert_digest->data, scert_digest->length));
	*__digest = digest;
	*__digest_len = (size_t)scert_digest->length;

	return E_HM_OK;
}

void __noinline hm_warn_openssl(void)
{
	const char *efile, *efunc, *edata;
	int eline, eflags;
	ERR_get_error_all(&efile, &eline, &efunc, &edata, &eflags);
	hm_warn("fsverity certchain, failed to verify signature\n");
	hm_warn("fsverity certchain, FILE: %s, LINE: %d, FUNC: %s, DATA: %s, FLAGS: %d\n",
		efile, eline, efunc, edata, eflags);
}

static int prepare_x509_store(X509 *cert, time_t signtime, X509_STORE **_store)
{
	int err = E_HM_INVAL;
	X509_STORE *st = X509_STORE_new();
	if (st != NULL) {
		if (cert != NULL && X509_STORE_add_cert(st, cert) == 0) {
			X509_STORE_free(st);
			return err;
		}
		if (signtime != 0) {
			X509_VERIFY_PARAM_set_time(X509_STORE_get0_param(st), signtime);
		}
		X509_STORE_set_purpose(st, X509_PURPOSE_ANY);
		*_store = st;
		err = E_HM_OK;
	}

	return err;
}

static void destroy_x509_store(X509_STORE *st)
{
	if (st != NULL) {
		X509_STORE_free(st);
	}
}

#define OPENSSL_VERIFY_SUCCESS	1

int __noinline hm_fsverity_verify_certchain(const char *cert_data, struct hmcert_sig_info_s *sig_info,
					    unsigned char *data, size_t data_len)
{
	int err;
	PKCS7 *sig = NULL;
	X509_STORE *st = NULL;
	int pkcs7_flags = 0;
	BIO *msg_bio = NULL;
	struct x509_mem_item mem_item = {0};

	if (sig_info == NULL || sig_info->pkcs7 == NULL || data == NULL || data_len == 0) {
		hm_warn("fsverity certchain, invalid input params to verify signature\n");
		return E_HM_INVAL;
	}

	sig = (PKCS7 *)sig_info->pkcs7;

	if (cert_data == NULL && sig->d.sign->cert == NULL) {
		return E_HM_INVAL;
	} else if(cert_data == NULL || sig->d.sign->cert == NULL) {
		/* signer cert has been verified OR local sign mode */
		pkcs7_flags = PKCS7_NOCHAIN | PKCS7_NOVERIFY;
	}

	msg_bio = BIO_new_mem_buf(data, (int)data_len);
	if (msg_bio == NULL) {
		return E_HM_INVAL;
	}

	if (cert_data != NULL) {
		err = x509_mem_item_init(cert_data, FSVERITY_CERT_MAX_SIZE, &mem_item, NULL);
		if (err != E_HM_OK) {
			hm_warn("fsverity cerchain, failed to get cert\n");
			goto free_bio;
		}
	}

	if (sig->d.sign->cert != NULL) {
		err = prepare_x509_store(mem_item.cert, sig_info->signtime, &st);
		if (err != E_HM_OK) {
			goto free_mem_item;
		}
	}

	err = PKCS7_verify(sig, sig->d.sign->cert == NULL ? mem_item.certs : NULL, st, msg_bio, NULL, pkcs7_flags);
	if (err == OPENSSL_VERIFY_SUCCESS) {
		err = E_HM_OK;
	} else {
		hm_warn_openssl();
		err = E_HM_KEYREJECTED;
	}

	destroy_x509_store(st);

free_mem_item:
	x509_mem_item_fini(&mem_item);
free_bio:
	BIO_free(msg_bio);

	return err;
}

int hm_fsverity_verify(const char *in_data, const unsigned char *sig_data, size_t sig_data_len,
		       unsigned char *data, size_t data_len)
{
	PKCS7 *sig = NULL;
	struct x509_mem_item mem_item = {0};
	STACK_OF(X509) *certs = NULL;
	int pkcs7_flags;
	int ret;
	BIO *msg_bio = NULL;

	if (in_data == NULL || sig_data == NULL || sig_data_len == 0 || data == NULL || data_len == 0) {
		hm_debug("Invalid input params\n");
		return E_HM_INVAL;
	}

	msg_bio = BIO_new_mem_buf(data, (int)data_len);
	if (msg_bio == NULL) {
		hm_debug("failed to new mem for data\n");
		return E_HM_INVAL;
	}

	sig = d2i_PKCS7(NULL, &sig_data, (long)sig_data_len);
	if (sig == NULL) {
		hm_debug("failed to get pkcs7 data\n");
		ret = E_HM_INVAL;
		goto free_bio;
	}

	ret = x509_mem_item_init(in_data, FSVERITY_CERT_MAX_SIZE, &mem_item, NULL);
	if (ret != E_HM_OK) {
		hm_debug("failed to init x509 mem item\n");
		ret = E_HM_INVAL;
		goto free_sig;
	}

	certs = mem_item.certs;
	pkcs7_flags = PKCS7_BINARY | PKCS7_DETACHED | PKCS7_NOATTR | PKCS7_NOCERTS | PKCS7_PARTIAL | PKCS7_NOVERIFY;
	ret = PKCS7_verify(sig, certs, NULL, msg_bio, NULL, pkcs7_flags);
	x509_mem_item_fini(&mem_item);
	ret = (ret == OPENSSL_VERIFY_SUCCESS) ? E_HM_OK : E_HM_INVAL;
	if (ret != E_HM_OK) {
		hm_debug("failed to call PKCS7_verify\n");
	}

free_sig:
	PKCS7_free(sig);
free_bio:
	BIO_free(msg_bio);

	return ret;
}
