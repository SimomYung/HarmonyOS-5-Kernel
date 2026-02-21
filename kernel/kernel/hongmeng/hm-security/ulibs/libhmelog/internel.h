/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: Definitions hongmeng log encryption
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 14:45:15 2024
 */
#ifndef ULIBS_INCLUDE_LIBHMELOG_INTERNEL_H
#define ULIBS_INCLUDE_LIBHMELOG_INTERNEL_H

#include <libhmcrypt/hmbase64.h>
#include <libhmelog/hmelog.h>

#define HMELOG_ENCRYPT_BUF_LEN (base64_decode_out_size(HMELOG_ENCRYPT_BASE64_BUF_LEN))
/* block size must be a multiple of 3 */
#define ENCRYPT_BLOCK_SIZE (3*8)

#define min(a, b) (((a) < (b)) ? (a) : (b))

#define HMELOG_PUBLIC_KEY ("-----BEGIN PUBLIC KEY-----\n"				\
	"MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA51FGVR6owPhirneegAOL\n"	\
	"nmM3Kl0ksijTrWc4QT0Ot7LBSnfcuVy7CY3CZXquGHn6K+XXgB5Hqafh5Fi+Cw8i\n"	\
	"MplXrQPNp3Yq+KxwNGBm9u8MrmHNPDAxWLsc51D1L259yXc+yIf0NjmXj64ptxXN\n"	\
	"i3sz5OgxOMxiERxwHZzpWg2Khanrptdn8PWckONTi+kBP4Px5VZZwTSLYq1Lzk0Z\n"	\
	"zgKV8L29RVOgx0o+chpZsFYeRCXN4DnbMWEvrfoaQDK94NGRCvxhga4SYcULW7bJ\n"	\
	"t9pLXX1yLF+TNzV8v/neCfOZmX4k/XWOCvzL/EVZqeysgQzt5gQ1i0aQAHYPgd7T\n"	\
	"x1cZ6/rMHNpCjJyFGLUBDNOhndHe4VD2OozKQVxsyddB81W/rsCYRArcaBm11QWY\n"	\
	"kLXZiVaCyAUJLIQsj4UQMvDhw5wUjIoY4C0SjgHKBo0CNpzGiZWus16daydvLrsP\n"	\
	"1hfY36gtANqZNgRFN7/NQKFgDLG7fS1Vr1xGjESYUSOSVTnOdzuF4CK4PS2Dh++q\n"	\
	"0dQTeVGKkjUtP/vUXmb2Egj0QhEEY+pF0AUs9E35xqnn/H5o51+5488s9+dHjamB\n"	\
	"83F8je6EEEMSrNR38TuC4aZmZIBzi8+/qMQKxa/2s5CVaqownG+wY5Pu8ynTkDoW\n"	\
	"qh370NAhMO8Lo01xIO3M0oUCAwEAAQ==\n"					\
	"-----END PUBLIC KEY-----")
#define HMELOG_PUBLIC_CIPHERTEXT_LEN 512
#define HMELOG_ENCRYPT_MODE HMCRYPTO_SKCIPHER_AES256_CTR

enum hmelog_meta_print_phase {
	hmelog_phase_pubkey_version = 0, /* include public key version and key tag */
	hmelog_phase_key,
	hmelog_phase_iv,
	hmelog_phase_max,
};

#define HMELOG_PUBLIC_KEY_VERSION 1
#define HMELOG_PUBLIC_KEY_VERSION_FIRST_FMT "[ELPKV][%lu]%d[/ELPKV][ELT]%016llx[/ELT]\n"
#define HMELOG_PUBLIC_KEY_VERSION_SECOND_FMT "[ELPKV1][%lu]%d[/ELPKV1][ELT1]%016llx[/ELT1]\n"

#define HMELOG_TAG_KEY_FIRST_HEAD "[ELK][%lu]"
#define HMELOG_TAG_KEY_FIRST_TAIL "[/ELK]\n"
#define HMELOG_TAG_KEY_SECOND_HEAD "[ELK1][%lu]"
#define HMELOG_TAG_KEY_SECOND_TAIL "[/ELK1]\n"

#define HMELOG_TAG_IV_FIRST_HEAD "[ELIV][%lu]"
#define HMELOG_TAG_IV_FIRST_TAIL "[/ELIV]\n"
#define HMELOG_TAG_IV_SECOND_HEAD "[ELIV1][%lu]"
#define HMELOG_TAG_IV_SECOND_TAIL "[/ELIV1]\n"

#define HMELOG_TAG_LOG_HEAD "[ELC][%lu][%lu]"
#define HMELOG_TAG_LOG_TAIL "[/ELC]\n"

#define HMELOG_COMMON_PART_TAIL "\n"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof ((a)[0]))
#endif

#endif
