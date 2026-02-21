/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: sec_auth lib header
 * Create: 2025/2/18
 */
#ifndef __SEC_AUTH_LIB_H__
#define __SEC_AUTH_LIB_H__

#include <linux/types.h>
#include <platform_include/see/sec_auth_ca.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SEC_AUTH_LIB_OK                          0x0
#define SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT          0x4D00FFFF

#define ROTPK_HASH_MAX_BYTES                     0x40

#define FAKE_DATA_SIZE                           0x20

typedef struct auth_node {
	struct image_id_t id;
	void *cert_addr;
	u32 cert_size;
	void *data_addr;
	u32 data_size;
} auth_node;

#ifdef CONFIG_SEC_AUTH_LIB
u32 sec_auth_lib_init(void);
u32 sec_auth_lib_deinit(void);

u32 sec_auth_node_init(auth_node *node);
u32 sec_auth_node_read_cert(auth_node *node);
u32 sec_auth_node_read_cert_parse_data_size(auth_node *node);
u32 sec_auth_node_read_data(auth_node *node);
u32 sec_auth_node_verify(auth_node *node, struct authed_data *out);
u32 sec_auth_node_final(auth_node *node);
u32 sec_auth_cert(auth_node *node, struct authed_data *out);

u32 check_image_hash(u8 *data_addr, u32 data_size, u8 *authed_hash, u32 hash_size);

#else /* CONFIG_SEC_AUTH_LIB */

static inline u32 sec_auth_lib_init(void)
{
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_lib_deinit(void)
{
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_node_init(auth_node *node)
{
	(void)node;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_node_read_cert(auth_node *node)
{
	(void)node;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_node_read_cert_parse_data_size(auth_node *node)
{
	(void)node;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_node_read_data(auth_node *node)
{
	(void)node;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_node_verify(auth_node *node, struct authed_data *out)
{
	(void)node;
	(void)out;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_node_final(auth_node *node)
{
	(void)node;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 check_image_hash(u8 *data_addr, u32 data_size, u8 *authed_hash, u32 hash_size)
{
	(void)data_addr;
	(void)data_size;
	(void)authed_hash;
	(void)hash_size;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

static inline u32 sec_auth_cert(auth_node *node, struct authed_data *out)
{
	(void)node;
	(void)out;
	return SEC_AUTH_LIB_ERR_FUNC_UNSUPPORT;
}

#endif /* CONFIG_SEC_AUTH_LIB */

#ifdef __cplusplus
}
#endif

#endif /* __SEC_AUTH_LIB_H__ */
