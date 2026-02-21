/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : FRW模块初始化与卸载
 * 作者       : 康国昌
 * 创建日期   : 2012年9月18日
 */
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
/* 头文件包含 */
#include "dpe_frw_main.h"
#include "dpe_frw_event.h"
#include "dpe_hcc_if.h"

/* part0 -- DPA异步事件处理接口 */
frw_event_sub_table_item_stru g_ast_dmac_misc_event_sub_table[HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT] = {
#ifdef _PRE_WLAN_FEATURE_VSP
    [HAL_EVENT_DMAC_COMMON_TIMER_IRQ] = { .p_func = dmac_common_timer_out_event },
#endif
};

/* part1 HMAC --> DMAC模块，HOST_DRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX_BUTT] = {
    [DMAC_TX_HOST_DRX] = { .p_func = dmac_tx_data_event_handler,
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
    [HMAC_TX_DMAC_SCH] = { .p_func = dmac_event_config_syn,
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
    [DMAC_TX_HOST_DTX] = { .p_func = dmac_tx_data_event_handler,
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
};

/* HOST CRX子表 */
frw_event_sub_table_item_stru g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_BUTT] = {
    [HMAC_TO_DMAC_SYN_CFG] = { .p_func = dmac_event_config_syn, // hmac_to_dmac_cfg_msg_stru核间通信事件通用接口
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
    [HMAC_TO_DMAC_SYN_CREATE_CFG_VAP] = { .p_func = dmac_cfg_vap_init_event, // 创建配置vap
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event},
};

/* DMAC模块，WLAN_DTX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx[DMAC_TX_WLAN_DTX_BUTT] = {
    [DMAC_TX_WLAN_DTX] = { .p_func = dmac_tx_data_event_handler } // 已废弃
};

frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT] = {
    /* 注册DMAC模块WLAN_CTX事件处理函数表 */
    [DMAC_WLAN_CTX_EVENT_SUB_TYPE_ADD_USER] = {
        .p_func = dpe_user_add, // 具体的业务处理函数
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event,  // DPA处理
        .p_tx_adapt_func = frw_convert_event_to_extend_hdr },  // WCPU处理
    [DMAC_WLAN_CTX_EVENT_SUB_TYPE_DEL_USER] = { .p_func = dmac_user_del_event_handle,
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
    [DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC] = { .p_func = dmac_rx_process_sync_event,
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event }, // 聚合
    [DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT] = { .p_func = dmac_tx_process_mgmt_event,  // 管理帧发送统一接口
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    [DMAC_WLAN_CTX_EVENT_SUB_TYPE_ADD_MULTI_USER] = { .p_func = dmac_mcast_ampdu_multi_user_add,
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event },
#endif
};

/* part2 DMAC --> HMAC模块, WLAN_DRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT] = {
    [DMAC_WLAN_DRX_EVENT_SUB_TYPE_WOW_RX_DATA]= {
        .p_rx_adapt_func = frw_hcc_rx_convert_to_event,
        .p_func = hmac_dev_rx_process_data},
};

/* HMAC模块 WLAN_CRX事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_d2h_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT] = {
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_INIT] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX_WOW] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_DELBA] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_DISASSOC] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_DEAUTH] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_TX_RING_ADDR] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_WLAN_CRX_EVENT_SUB_TYPE_VSP_INFO_ADDR] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
};

/* HMAC模块 MISC杂散事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_dmac_to_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_BUTT] = {
    [DMAC_MISC_SUB_TYPE_DISASOC] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
    [DMAC_MISC_SUB_TYPE_TX_SWITCH_STATE] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr }, /* DPE */
    [DMAC_MISC_PSM_GET_HOST_RING_STATE] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr }, /* DPE */
};
/* HMAC模块 发向HOST侧的配置事件处理函数注册结构定义 */
frw_event_sub_table_item_stru g_dmac_to_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_BUTT] = {
    [DMAC_TO_HMAC_DEL_BA] = { .p_tx_adapt_func = frw_convert_event_to_extend_hdr },
};

/* 异步处理事件 */
frw_event_sub_table_item_stru *g_dmac_misc_event_sub_table_ptr = g_ast_dmac_misc_event_sub_table;
/* Part1: 以下是ACPU post向Dmac收的事件 */
frw_event_sub_table_item_stru *g_dmac_host_crx_table_ptr = g_ast_dmac_host_crx_table;
frw_event_sub_table_item_stru *g_dmac_tx_host_drx_ptr = g_ast_dmac_tx_host_drx;
frw_event_sub_table_item_stru *g_dmac_wlan_ctx_event_sub_table_ptr = g_ast_dmac_wlan_ctx_event_sub_table;
frw_event_sub_table_item_stru *g_dmac_tx_wlan_dtx_ptr = g_ast_dmac_tx_wlan_dtx;
/* Part2: 以下是WCPU post向Hmac收的事件 */
frw_event_sub_table_item_stru *g_dmac_to_hmac_wlan_crx_event_sub_table_ptr = g_d2h_wlan_crx_event_sub_table;
frw_event_sub_table_item_stru *g_hmac_wlan_drx_event_sub_table_ptr = g_ast_hmac_wlan_drx_event_sub_table;
frw_event_sub_table_item_stru *g_dmac_to_hmac_wlan_misc_event_sub_table_ptr = g_dmac_to_hmac_wlan_misc_event_sub_table;
frw_event_sub_table_item_stru *g_dmac_to_hmac_wlan_ctx_event_sub_table_ptr = g_dmac_to_hmac_wlan_ctx_event_sub_table;

void event_fsm_table_register(void)
{
    memset_s(g_ast_event_table, sizeof(g_ast_event_table), 0, sizeof(g_ast_event_table));

    /* 注册DPA模块MISC事件字表, 处理异步事件 */
    frw_event_table_register(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_0,
        g_dmac_misc_event_sub_table_ptr);

    /* Part1: 以下是ACPU post向Dmac收的事件 */
    /* 注册DMAC模块HOST_DRX事件处理函数表 */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_DRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_tx_host_drx_ptr);
    /* 注册DMAC模块HOST_CRX事件 */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_CRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_host_crx_table_ptr);
    /* 注册DMAC模块WLAN_DTX事件处理函数表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_tx_wlan_dtx_ptr);
    /* 注册DMAC模块WLAN_CTX事件处理函数表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CTX, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_wlan_ctx_event_sub_table_ptr);

    /* Part2: 以下是WCPU post向Hmac收的事件 */
    /* 注册WLAN_DRX事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_hmac_wlan_drx_event_sub_table_ptr);
    /* 注册WLAN_CRX事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_to_hmac_wlan_crx_event_sub_table_ptr);

    /* 注册DMAC模块MISC事件字表 */
    frw_event_table_register(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_to_hmac_wlan_misc_event_sub_table_ptr);
    /* 注册统计结果查询事件子表 */
    frw_event_table_register(FRW_EVENT_TYPE_HOST_SDT_REG, FRW_EVENT_PIPELINE_STAGE_1,
        g_dmac_to_hmac_wlan_ctx_event_sub_table_ptr);
}
#endif
