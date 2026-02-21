/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Write and read file credentials
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 20:10:19 2019
 */

#ifndef ULIBS_LIBSEC_FILE_CRED_H
#define ULIBS_LIBSEC_FILE_CRED_H

#include <stdint.h>
#include <libhmseharmony/seharmony.h>
#include <libsysif/fs/api.h>
#include <libpolicy_api.h>
#include "dac.h"
#include "mac.h"

struct sec_file_cred {
	struct sec_ctx_mac mac;
	struct sec_ctx_dac dac;
	struct sec_ctx_sel sel;
};

int sec_file_cred_write_mac(struct vfs_file_cred *file_cred,
			    const struct sec_ctx_mac *mac_cred);
int sec_file_cred_read_mac(const struct vfs_file_cred *file_cred,
			   struct sec_ctx_mac *mac_cred);
int sec_file_cred_write_dac(struct vfs_file_cred *file_cred,
			    const struct sec_ctx_dac *dac_cred);
int sec_file_cred_read_dac(const struct vfs_file_cred *file_cred,
			   struct sec_ctx_dac *dac_cred);

int sec_file_cred_write_sel(struct vfs_file_cred *file_cred,
			    const struct sec_ctx_sel *sel_cred);
int sec_file_cred_read_sel(const struct vfs_file_cred *file_cred,
			   struct sec_ctx_sel *sel_cred);
#endif /* ifndef ULIBS_LIBSEC_FILE_CRED_H */
