/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: smartcs hand cert
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 19 22:48:24 2025
 */
#ifndef ULIBS_HMCERT_SMARTCS_H
#define ULIBS_HMCERT_SMARTCS_H

#include <stdint.h>

int smartcs_init(void);
void smartcs_uninit(void);
void hm_sig_agc_fastpath_fini(struct hmcert_sig_info_s *sig_info);
int hm_sig_verify_agc_fastpath(const unsigned char *signer_info, size_t signer_info_len,
			       const unsigned char *data, size_t data_len,
			       struct hmcert_sig_info_s **_sig_info);

#endif
