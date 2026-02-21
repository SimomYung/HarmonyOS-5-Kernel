/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "pcs_bc_cap_get_dir.h"

#ifndef CONFIG_LIBLINUX
#include <linux/path.h>
#endif
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/dcache.h>
#include <linux/limits.h>
#include <linux/slab.h>
#include <linux/errno.h>

#include "securec.h"

#include "product_config_ps_ap.h"
#include "pcs_log.h"
#include "pcs_def.h"

pcs_get_bc_cap_dir_cb pcs_bc_cap_cb = NULL;
STATIC DEFINE_MUTEX(pcs_bc_cap_cb_lock);

void pcs_show_bc_cap_dir(void)
{
	int ret;
	char *dir = NULL;

	mutex_lock(&pcs_bc_cap_cb_lock);
	if (pcs_bc_cap_cb == NULL) {
		mutex_unlock(&pcs_bc_cap_cb_lock);
		PCS_LOGE("cb is NULL");
		return;
	}

	ret = pcs_bc_cap_cb(&dir);
	mutex_unlock(&pcs_bc_cap_cb_lock);

	if (ret <= 0 || ret >= PCS_BC_CAP_DIR_MAX_LEN) {
		PCS_LOGE("ret: %d, error", ret);
		return;
	}
}

struct pcs_dir_diag_info {
	ps_log_third_head_t head;
	enum pcs_diag_cause cause;
	char dir[PCS_BC_CAP_PATH_MAX_LEN];
};

void pcs_reg_get_bc_cap_dir_cb(pcs_get_bc_cap_dir_cb cb)
{
	mutex_lock(&pcs_bc_cap_cb_lock);
	pcs_bc_cap_cb = cb;
	mutex_unlock(&pcs_bc_cap_cb_lock);

	pcs_show_bc_cap_dir();
}
EXPORT_SYMBOL(pcs_reg_get_bc_cap_dir_cb);

pcs_get_bc_cap_dir_cb pcs_get_bc_cap_cb(void)
{
	return pcs_bc_cap_cb;
}

#define ASCII_SLASH 0x2F

const char *pcs_bc_cap_dir_allowlist[] = {
	"/vendor/etc/cellular_cloud/ca_cloud/ca_plmn",
	"/chip_prod/etc/dataservice/ca",

	"/system/app/config",

#ifdef CONFIG_PS_AP_ENG
	"/data/test/bc_cap",
#endif
};

#ifdef CONFIG_LIBLINUX
STATIC char *pcs_realpath(char *path, char *buf, u32 buf_len)
{
	return liblinux_pal_realpath(path, buf, buf_len);
}

#else
STATIC char *pcs_realpath(const char *path, char *buf, u32 buf_len)
{
	int ret;
	char *dpath = NULL;
	struct path base_path = {
		.dentry = NULL
	};

	ret = kern_path(path, LOOKUP_FOLLOW, &base_path);
	if (ret != 0) {
		PCS_DIAG_LOGE("err , ret = %d", ret);
		return NULL;
	}

	dpath = d_path(&base_path, buf, buf_len);
	path_put(&base_path);
	return dpath;
}
#endif

bool pcs_bc_cap_verify_dir(char *dir, size_t len)
{
	char *real_dir = NULL;
	char buf[PCS_BC_CAP_PATH_MAX_LEN] = {0};
	u32 i;

	real_dir = pcs_realpath(dir, buf, sizeof(buf));
	if (IS_ERR_OR_NULL(real_dir)) {
		PCS_DIAG_LOGE("pcs_realpath fail, dir = %s", dir);
		return false;
	}

	for (i = 0; i < ARRAY_SIZE(pcs_bc_cap_dir_allowlist); i++) {
		if (strlen(real_dir) < strlen(pcs_bc_cap_dir_allowlist[i]))
			continue;

		if (strncmp(real_dir, pcs_bc_cap_dir_allowlist[i],
			    strlen(pcs_bc_cap_dir_allowlist[i])) == 0)
			return true;
	}

	PCS_DIAG_LOGE("real_dir = %s , is not allow", real_dir);
	return false;
}

int pcs_get_bc_cap_dir(char *data, u32 data_len)
{
	char *dir = NULL;
	int len;

	mutex_lock(&pcs_bc_cap_cb_lock);
	if (pcs_bc_cap_cb == NULL) {
		mutex_unlock(&pcs_bc_cap_cb_lock);
		PCS_DIAG_LOGE("cb is null");
		return -EINVAL;
	}

	len = pcs_bc_cap_cb(&dir);
	mutex_unlock(&pcs_bc_cap_cb_lock);

	if (len <= 0) {
		PCS_DIAG_LOGE("bc_cap_cb error");
		return -ENOENT;
	}

	if (len > PCS_BC_CAP_DIR_MAX_LEN || len > data_len - 1) {
		PCS_DIAG_LOGE("len too long");
		return -ENAMETOOLONG;
	}

	if (dir[len - 1] != 0) {
		PCS_DIAG_LOGE("dir invalid");
		return -EINVAL;
	}

	if (!pcs_bc_cap_verify_dir(dir, (size_t)len))
		return -EINVAL;

	if (memcpy_s(data, data_len, dir, strlen(dir) + 1)) {
		PCS_DIAG_LOGE("memcpy_s fail, dir = %s", dir);
		return -EINVAL;
	}

	len--;

	/* Make sure the directory ends with a slash. */
	if (data[len - 1] != ASCII_SLASH) {
		data[len] = ASCII_SLASH;
		data[len + 1] = 0;
		len++;
	}

	return len;
}
