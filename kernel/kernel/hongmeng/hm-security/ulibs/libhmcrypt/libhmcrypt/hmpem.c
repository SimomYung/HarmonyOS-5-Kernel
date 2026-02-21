/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Description
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 06 13:22:25 2022
 */
#include "libhmcrypt/hmbase64.h"
#include "libhmcrypt/hmasn1.h"
#include "libhmcrypt/hmpem.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#define MAX_LINE_LEN		512
#define LINE_PREFIX		"-----"
#define LINE_PREFIX_LEN		(sizeof(LINE_PREFIX) - 1)

static int parse_algorithm(struct asn1_data *d, CRYPT_EAL_PkeyPub *key)
{
	int rc = E_HM_OK;

	char alg_rsa[] = {0x06, 0x09, /* OBJECT IDENTIFIER */
		0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, /* rsaEncryption (1 2 840 113549 1 1 1) (PKCS #1) */
		0x05, 0x00 /* NULL */
	};

	if (d->size == sizeof(alg_rsa) && memcmp(d->data, alg_rsa, d->size) == 0) {
		key->id = CRYPT_PKEY_RSA;
	} else {
		rc = E_HM_POSIX_FAULT;
	}

	return rc;
}

static int parse_rsa_pubkey(struct asn1_data *pubkey, CRYPT_EAL_PkeyPub *pkey)
{
	int rc = E_HM_OK;
	struct asn1_data rsakey = {0};
	struct asn1_data n = {0};
	struct asn1_data e = {0};

	rc = asn1_next_child(pubkey, NULL, &rsakey);
	if (rc < 0) {
		return rc;
	}

	rc = asn1_next_child(&rsakey, NULL, &n);
	if (rc < 0) {
		return rc;
	}

	rc = asn1_next_child(&rsakey, &n, &e);
	if (rc < 0) {
		return rc;
	}

	pkey->key.rsaPub.n = malloc(sizeof(uint8_t) * n.size);
	if (pkey->key.rsaPub.n == NULL) {
		hm_error("failed to malloc rsaPub modulus\n");
		return E_HM_NOMEM;
	}
	NOFAIL(memcpy_s(pkey->key.rsaPub.n, n.size, n.data, n.size));
	pkey->key.rsaPub.nLen = n.size;

	pkey->key.rsaPub.e = malloc(sizeof(uint8_t) * e.size);
	if (pkey->key.rsaPub.e == NULL) {
		hm_error("failed to malloc rsaPub exponent\n");
		free(pkey->key.rsaPub.n);
		pkey->key.rsaPub.n = NULL;
		return E_HM_NOMEM;
	}
	NOFAIL(memcpy_s(pkey->key.rsaPub.e, e.size, e.data, e.size));
	pkey->key.rsaPub.eLen = e.size;

	return E_HM_OK;
}

/*
 * The DER object is a SubjectPublicKeyInfo SEQUENCE with two elements:
 *     1. algorithmIdentifier (SEQUENCE)
 *     2. subjectPublicKey (BIT STRING)
 * for rsaEncryption, the 'subjectPublicKey' encapsulates the actual RSAPublicKey:
 *     1. modulus (INTEGER)
 *     2. exponent (INTEGER)
 */
static int parse_hitls_pubkey(uint8_t *cont, uint32_t len, CRYPT_EAL_PkeyPub *key)
{
	int rc = E_HM_OK;
	struct asn1_data keyinfo = {0};
	struct asn1_data alg = {0};
	struct asn1_data pubkey = {0};

	rc = asn1_parse(cont, len, &keyinfo);
	if (rc < 0) {
		return rc;
	}

	rc = asn1_next_child(&keyinfo, NULL, &alg);
	if (rc < 0) {
		return rc;
	}

	rc = parse_algorithm(&alg, key);
	if (rc < 0) {
		return rc;
	}

	rc = asn1_next_child(&keyinfo, &alg, &pubkey);
	if (rc < 0) {
		return rc;
	}

	if (key->id == CRYPT_PKEY_RSA) {
		return parse_rsa_pubkey(&pubkey, key);
	} else {
		return E_HM_POSIX_FAULT;
	}
}

static char *load_pubkey(const char *fname, long *size)
{
	FILE *fp = NULL;
	char *cont = NULL;
	long fsize = 0;
	long pos = 0;
	char buf[MAX_LINE_LEN] = {0};

	fp = fopen(fname, "rb");
	if (fp == NULL) {
		hm_error("failed to open %s\n", fname);
		return NULL;
	}

	(void)fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);
	if (fsize < 0) {
		goto error;
	}

	cont = malloc((size_t)fsize);
	if (cont == NULL) {
		goto error;
	}

	(void)fseek(fp, 0L, SEEK_SET);
	while (fgets(buf, MAX_LINE_LEN, fp) != NULL) {
		if (strncmp(buf, LINE_PREFIX, LINE_PREFIX_LEN) == 0) {
			continue;
		}
		size_t len = strlen(buf);
		if ((size_t)pos + len <= (size_t)fsize) {
			NOFAIL(memcpy_s(cont + pos, len, buf, len));
		}
		pos += (long)len;
	}

	cont[pos] = '\0';

	*size = pos;

error:
	(void)fclose(fp);
	return cont;
}

int pem_parse_hitls_pubkey(const char *path, CRYPT_EAL_PkeyPub *pkey)
{
	int rc = E_HM_INVAL;
	char *pubkey = NULL;
	long len = 0;
	size_t pos = 0;

	pubkey = load_pubkey(path, &len);
	if (pubkey == NULL) {
		hm_error("failed to load pubkey %s\n", path);
		return E_HM_INVAL;
	}

	rc = base64_decode(pubkey, (size_t)len, (uint8_t *)pubkey, &pos);
	if (rc != E_HM_OK) {
		hm_error("failed to decode pubkey\n");
		goto error;
	}

	rc = parse_hitls_pubkey((uint8_t *)pubkey, (uint32_t)pos, pkey);

error:
	free(pubkey);
	return rc;
}
