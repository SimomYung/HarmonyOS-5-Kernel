/* SPDX-License-Identifier: GPL-2.0 */
#ifndef CODE_DECRYPT_H
#define CODE_DECRYPT_H

#include <linux/types.h>
#include <linux/workqueue.h>

#define CIPHER_KEY_LEN		((256 / 8) * 2)
#define APP_IDENTIFY_LEN			32
#define APP_IDENTIFY_STR_LEN			33
#define DECRYPT_DEV_MODE			0664
#define SESSION_START_DEFAULT		1
#define TEE_PARAM_0					0
#define TEE_PARAM_1					1
#define TEE_PARAM_2					2
#define TEE_PARAM_3					3

#define CODE_DECRYPT_KEY_VERSION_1 1
#define CODE_DECRYPT_KEY_VERSION_2 2

struct tee_code_decrypt_key {
	int version; /* interface version */
	uint32_t bundleid;

	uint32_t version_code;
	uint8_t app_id[APP_IDENTIFY_STR_LEN];

	uint8_t key[CIPHER_KEY_LEN];
	uint32_t ret_code;
};

struct code_decrypt_tee_task {
	struct work_struct work;
	struct tee_code_decrypt_key *tee_key;
	int ret;
};

#define CODE_DECRYPT_MAGIC 'c'

#define CODE_DECRYPT_GET_TEE_KEY _IOW(CODE_DECRYPT_MAGIC, 1, struct tee_code_decrypt_key)

#endif
