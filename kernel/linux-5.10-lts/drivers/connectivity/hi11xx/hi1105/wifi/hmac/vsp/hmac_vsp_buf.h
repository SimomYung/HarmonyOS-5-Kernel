/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : vsp dma-buf管理
 * 作    者 : wifi
 * 创建日期 : 2022年6月9日
 */

#ifndef HMAC_VSP_BUF_H
#define HMAC_VSP_BUF_H

#include "hmac_vsp.h"

#ifdef _PRE_WLAN_FEATURE_VSP
#ifdef CONFIG_VCODEC_VSP_SUPPORT
void hmac_vsp_dma_list_init(void);
void hmac_vsp_clear_dma_buf_list(void);
#endif
#endif
#endif
