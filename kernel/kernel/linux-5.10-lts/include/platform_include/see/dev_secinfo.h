/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: dev_secinfo header
 * Create: 2023/3/31
 */
#ifndef __DEV_SECINFO_H__
#define __DEV_SECINFO_H__
#include <linux/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DEV_SECINFO_OK                           0x0
#define DEV_SECINFO_ERR_FUNC_UNSUPPORT           0x8ff7ffff

#define PTN_NAME_LENGTH_MAX                      36
#define ROTPK_HASH_MAX_BYTES                     0x40

struct dev_file_pos {
	char ptn[PTN_NAME_LENGTH_MAX];
	u64 offset;
	u64 size;
};

struct dev_rotpk_hash {
	u32 bytes;
	u8 val[ROTPK_HASH_MAX_BYTES];
};

enum dev_se_type {
	DEV_SECINFO_ENG_DEVICE = 0x5e,
	DEV_SECINFO_SHIP_DEVICE = 0xc4,
};

#ifdef CONFIG_SECURITY_INFO
u32 dev_secinfo_set_uapp_enable_state(u32 state);
u32 dev_secinfo_socid(u8 *buf, u32 len);
u32 dev_secinfo_rotpk_hash(struct dev_rotpk_hash *rotpk_hash);
u32 dev_secinfo_se_type(u32 *type);
u32 dev_secinfo_empower_cert_pos(struct dev_file_pos *empower_cert);
u32 dev_secinfo_update_all_nvcnt(void);
u32 dev_secinfo_huk2_enhance_update_cnt(void);

#else /* CONFIG_SECURITY_INFO */
static inline u32 dev_secinfo_set_uapp_enable_state(u32 state)
{
	(void)state;
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}

static inline u32 dev_secinfo_socid(u8 *buf, u32 len)
{
	(void)buf;
	(void)len;
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}

static inline u32 dev_secinfo_rotpk_hash(struct dev_rotpk_hash *rotpk_hash)
{
	(void)rotpk_hash;
	pr_err("%s not implement\n", __func__);
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}

static inline u32 dev_secinfo_se_type(u32 *type)
{
	(void)type;
	pr_err("%s not implement\n", __func__);
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}

static inline u32 dev_secinfo_empower_cert_pos(struct dev_file_pos *empower_cert)
{
	(void)empower_cert;
	pr_err("%s not implement\n", __func__);
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}

static inline u32 dev_secinfo_update_all_nvcnt(void)
{
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}

static inline u32 dev_secinfo_huk2_enhance_update_cnt(void)
{
	return DEV_SECINFO_ERR_FUNC_UNSUPPORT;
}
#endif /* CONFIG_SECURITY_INFO */

#ifdef __cplusplus
}
#endif

#endif /* __DEV_SECINFO_H__ */
