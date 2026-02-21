/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat
 * Create : 2024/03/12
 */
#ifndef __SEPLAT_H_
#define __SEPLAT_H_

#include <linux/types.h>
#include "teek_client_api.h"

/* max. buffer size supported by seplat TPM */
#define MAX_COMMAND_SIZE       1536
#define MAX_RESPONSE_SIZE      1536

/*
 * struct seplat_tpm_private - seplat tpm private data
 * @chip:     struct tpm_chip instance registered with tpm framework.
 * @resp_len: cached response buffer length.
 * @resp_buf: cached response buffer.
 */
struct seplat_tpm_private {
	struct tpm_chip *chip;
	size_t resp_len;
	u8 resp_buf[MAX_RESPONSE_SIZE];
};

/*
 * struct seplat_private - Seplat's private data
 * @tpm:      seplat tpm private data.
 * @ta_uuid:  Seplat TA uuid.
 */
struct seplat_private {
	struct seplat_tpm_private tpm;
	TEEC_UUID ta_uuid;
};

bool seplat_chiptype_exist_get(void);
int seplat_get_dieid(char *dieid, u32 len);
#endif /* __SEPLAT_H_ */
