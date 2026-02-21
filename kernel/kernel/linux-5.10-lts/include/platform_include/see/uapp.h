/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: header of UAPP (Unique Auth Per Phone)
 * Create: 2022/04/20
 */

#ifndef __UAPP_H__
#define __UAPP_H__

#include <linux/types.h>
#include <linux/kernel.h>
#include <platform_include/see/dev_secinfo.h>

/* error code */
#define UAPP_OK                         0x0
#define UAPP_FUNC_UNSUPPORT_ERR         0x3DA7FFFF

enum uapp_enable_state {
	UAPP_DISABLE = 0,
	UAPP_ENABLE = 1,
	UAPP_ENABLE_STATE_MAX,
};

struct uapp_bindfile_pos {
	struct dev_file_pos bindfile;
	struct dev_file_pos bindfile_backup;
};

#ifdef CONFIG_UAPP
u32 uapp_set_enable_state(u32 state);
u32 uapp_get_enable_state(u32 *state);
u32 uapp_valid_bindfile_pubkey(u32 key_idx);
u32 uapp_revoke_bindfile_pubkey(u32 key_idx);
u32 uapp_get_bindfile_pos(struct uapp_bindfile_pos *bindfile);

#else
static inline u32 uapp_set_enable_state(u32 state)
{
	(void)state;
	pr_err("%s not implement\n", __func__);
	return UAPP_FUNC_UNSUPPORT_ERR;
}

static inline u32 uapp_get_enable_state(u32 *state)
{
	(void)state;
	pr_err("%s not implement\n", __func__);
	return UAPP_FUNC_UNSUPPORT_ERR;
}

static inline u32 uapp_valid_bindfile_pubkey(u32 key_idx)
{
	(void)key_idx;
	pr_err("%s not implement\n", __func__);
	return UAPP_FUNC_UNSUPPORT_ERR;
}

static inline u32 uapp_revoke_bindfile_pubkey(u32 key_idx)
{
	(void)key_idx;
	pr_err("%s not implement\n", __func__);
	return UAPP_FUNC_UNSUPPORT_ERR;
}


static inline u32 uapp_get_bindfile_pos(struct uapp_bindfile_pos *bindfile)
{
	(void)bindfile;
	pr_err("%s not implement\n", __func__);
	return UAPP_FUNC_UNSUPPORT_ERR;
}

#endif /* CONFIG_UAPP */

#endif /* __UAPP_H__ */
