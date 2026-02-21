/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: dfa cypto
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __DFA_CRYPTO_H__
#define __DFA_CRYPTO_H__
#include "ufshcd.h"

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_KM
int dfa_resume_inline_crypt_key(struct ufs_hba *hba);
#else
static inline int dfa_resume_inline_crypt_key(struct ufs_hba *hba) { return 0; }
#endif
int dfa_uie_init(struct ufs_hba *hba, bool in_eh);

#endif