/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: This module is used to start vcom between ril and kernel
 * Author: baijiaxuan2@huawei.com
 * Create: 2024-12-06
 */

#ifndef SATE_LOG_H
#define SATE_LOG_H

#include <linux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SATE_LOG_LVL_HIGH	(1U << 0)
#define SATE_LOG_LVL_ERR	(1U << 1)
#define SATE_LOG_LVL_INFO	(1U << 2)
#define SATE_LOG_LVL_DBG	(1U << 3)

#define SATE_LOGH(fmt, ...) do { \
	if (sate_log_level & SATE_LOG_LVL_HIGH) \
		pr_err("[SATE_VNET]: [HIGH] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define SATE_LOGE(fmt, ...) do { \
	if (sate_log_level & SATE_LOG_LVL_ERR) \
        pr_err("[SATE_VNET]: [ERR] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define SATE_LOGI(fmt, ...) do { \
	if (sate_log_level & SATE_LOG_LVL_INFO) \
        pr_warn("[SATE_VNET]: [INFO] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define SATE_LOGD(fmt, ...) do { \
	if (sate_log_level & SATE_LOG_LVL_DBG) \
        pr_notice("[SATE_VNET]: [DBG] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define SATE_IPVER_4		0x40
#define SATE_IPVER_6		0x60
#define SATE_IPVER_MASK		0xF0

extern unsigned int sate_log_level;

/* Notification request issued by the upper layer is defined as: */
struct msg_head {
	u16 type; // Event enumeration values
    u16 len; // The length behind this field, the limit lower 2048
    unsigned char value[4096];
};

enum nl_cmd_type {
    VNET_MSG_SPEED = 0,
	VNET_MSG_PDCP,
	VNET_MSG_BYTE,
	VNET_MSG_RX
};

struct sate_vnet_priv_s {
    int speed;
    int byteRemain;
    int (*sendPdcpResult)(int pdcpResult, int id);
};

#ifdef __cplusplus
}
#endif

#endif /* SATE_LOG_H */
