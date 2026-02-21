/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: function for base hash operation
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "auth_path_impl.h"
#include <linux/types.h>
#include <linux/string.h>
#if defined(CONFIG_SELINUX_AUTH_ENBALE) && defined(CONFIG_SECURITY_SELINUX)
#ifdef CONFIG_LIBLINUX
#include <sec.h>
#else
#include <linux/security.h>
#endif
#include "tc_ns_log.h"
#endif
 
#if defined(CONFIG_SELINUX_AUTH_ENBALE) && defined(CONFIG_SECURITY_SELINUX)
static bool is_context_match_current_sid(const char *s_ctx, u32 ctx_len)
{
	bool ret;
#ifdef CONFIG_LIBLINUX
	ret = sec_is_context_match_current_sid(s_ctx);
#else
	u32 sid = 0, tid = 0;
	security_task_getsecid(current, &sid);
	int rc = security_secctx_to_secid(s_ctx, ctx_len, &tid);
	if (rc != 0) {
		tloge("security secctx=%s to secid failed, rc %d\n", s_ctx, rc);
		return false;
	}
	ret = (sid == tid);
#endif
	if (ret != true) {
		tloge("context is not match current sid, s_ctx=%s\n", s_ctx);
		return false;
	}
	return true;
}
 
// This interface has been tailored on the wireless
int check_proc_selinux_access(const char *s_ctx)
{
	if (s_ctx == NULL) {
		tloge("bad params\n");
		return CHECK_ACCESS_FAIL;
	}
	u32 s_ctx_len = strnlen(s_ctx, MAX_SCTX_LEN);
	if (s_ctx_len == 0 || s_ctx_len >= MAX_SCTX_LEN) {
		tloge("invalid selinux ctx\n");
		return CHECK_ACCESS_FAIL;
	}
 
	if (is_context_match_current_sid(s_ctx, s_ctx_len) != true)
		return CHECK_ACCESS_FAIL;
 
	return CHECK_ACCESS_SUCC;
}
 
#endif
