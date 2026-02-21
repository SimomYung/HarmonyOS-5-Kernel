/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 事件管理对外接口(对IPC接口、对业务接口)实现
 * 作者       :
 * 创建日期   : 2012年10月12日
 */
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD

/* 头文件包含 */
#include "securec.h"
#include "dpe_frw_event.h"

/* 事件表全局变量 */
frw_event_table_item_stru g_ast_event_table[FRW_EVENT_TABLE_MAX_ITEMS];

/*
 * 功能描述  : 注册相应事件对应的事件处理函数
 */
void frw_event_table_register(uint8_t en_type, frw_event_pipeline_enum en_pipeline,
    frw_event_sub_table_item_stru *sub_table)
{
    uint8_t uc_index = (uint8_t)((en_type << 1) | ((uint8_t)en_pipeline & 0x01));

    if (oal_unlikely(sub_table == NULL)) {
        oam_error_log0(0, OAM_SF_FRW, "{frw_event_table_register_etc: sub_table is null ptr!}");
        return;
    }

    if (oal_unlikely(uc_index >= FRW_EVENT_TABLE_MAX_ITEMS)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_table_register_etc, array overflow! %d}", uc_index);
        return;
    }

    g_ast_event_table[uc_index].pst_sub_table = sub_table;
}

OAL_STATIC uint32_t frw_hcc_event_process(oal_netbuf_stru *netbuf_dpa, oal_netbuf_stru *netbuf)
{
    uint32_t ret;

    /* HCC 收到的event除了DPA处理，还可能转发，需要优先本核处理后转发 */
    /* For hcc post event adapt，hcc与wifi适配层，hcc rx post do */
    if (frw_event_table->pst_sub_table[sub_type].p_rx_adapt_func != NULL) {
        /* rx adapt first */
        ret = frw_event_table->pst_sub_table[sub_type].p_rx_adapt_func(netbuf_dpa);
        if (ret != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_FRW,
                "{frw_master_hcc_event_process::tx adapt process failed! type[0x%x][0x%x]}", main_type, sub_type);
        }
        if (frw_event_table->pst_sub_table[sub_type].p_func == NULL) {
            oam_error_log1(0, OAM_SF_FRW,
                "{frw_master_hcc_event_process::pfuc NULL! type[0x%x][0x%x]}", main_type, sub_type);
            return OAL_ERR_CODE_PTR_NULL;
        }
        /* call action frame */
        frw_event_table->pst_sub_table[sub_type].p_func(netbuf_dpa);
    }

    /* For wifi post event adapt，wifi与hcc适配层，去向hcc tx接口 */
    if (frw_event_table->pst_sub_table[sub_type].p_tx_adapt_func != NULL) {
        ret = frw_event_table->pst_sub_table[sub_type].p_tx_adapt_func(netbuf);
        if (ret != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_FRW,
                "{frw_event_lookup_process_entry::tx adapt process failed! type[0x%x][0x%x]}", main_type, sub_type);
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t frw_wifi_event_process(oal_netbuf_stru *netbuf)
{
    uint32_t ret;

    /* WIFI 一般只会单向传输，除了异步事件WIFI抛出并WIFI执行,由pfunc直接执行不需要注册trx adapt接口 */
    /* For wifi post event adapt，wifi与hcc适配层，去向hcc tx接口 */
    if (frw_event_table->pst_sub_table[sub_type].p_tx_adapt_func != NULL) {
        ret = frw_event_table->pst_sub_table[sub_type].p_tx_adapt_func(netbuf);
        if (ret != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_FRW,
                "{frw_wifi_event_process::tx adapt process failed! type[0x%x][0x%x]}", main_type, sub_type);
        }
        return ret;
    }

    oam_error_log2(0, OAM_SF_FRW,
        "{frw_wifi_event_process::err event! type[0x%x][0x%x]}", main_type, sub_type);
    return OAL_SUCC;
}

OAL_STATIC uint32_t frw_pipe_1_event_process(oal_netbuf_stru *netbuf,
    frw_event_table_item_stru *frw_event_table, uint8_t main_type, uint8_t sub_type, uint8_t event_type)
{
    oal_netbuf_stru *new_skb = NULL;

    if (oal_all_null_ptr2(frw_event_table->pst_sub_table[sub_type].p_tx_adapt_func,
                          frw_event_table->pst_sub_table[sub_type].p_rx_adapt_func)) {
        oam_error_log2(0, OAM_SF_FRW,
            "{frw_event_lookup_process_entry::tx/rx adapt all NULL! type[0x%x][0x%x]}", main_type, sub_type);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 如果该事件需要多核流转，需要拷贝到新的skb实现流转设计 */
    if ((frw_event_table->pst_sub_table[sub_type].p_tx_adapt_func != NULL) &&
        (frw_event_table->pst_sub_table[sub_type].p_rx_adapt_func != NULL)) {
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
        new_skb = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, 1536, OAL_NETBUF_PRIORITY_HIGH); // payload 1536
        memcpy_s(oal_sbuf_cb(new_skb), sizeof(frw_event_stru), oal_sbuf_cb(netbuf), sizeof(frw_event_stru));
        /* EICC异步 */
        memcpy_s(oal_netbuf_data(new_skb), oal_netbuf_len(netbuf), oal_netbuf_data(netbuf), oal_netbuf_len(netbuf));
#endif
        /* HCC收到的事件才会多核流转 */
        return frw_hcc_event_process(new_skb, netbuf);
    }
    /* WIFI抛出的事件 */
    return frw_wifi_event_process(netbuf);
}
/*
 * 功能描述  : 根据事件类型，子类型以及分段号，找到相应事件处理函数
 */
OAL_STATIC uint32_t frw_event_lookup_process_entry(frw_event_param_stru *event_para)
{
    frw_event_table_item_stru *frw_event_table = NULL;
    uint8_t index_t;
    uint8_t sub_type = event_para->uc_sub_type;

    /* 根据事件类型及分段号计算事件表索引 */
    index_t = (uint8_t)((event_para->en_type << 1) | (event_para->en_pipeline & 0x01));
    if (oal_unlikely(index_t >= FRW_EVENT_TABLE_MAX_ITEMS)) {
        oam_error_log3(0, OAM_SF_FRW,
            "{frw_event_lookup_process_entry::array overflow! type[0x%x], sub_type[0x%x], pipeline[0x%x].}",
            event_para->en_type, sub_type, event_para->en_pipeline);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* 先把全局变量变成局部变量 */
    frw_event_table = &g_ast_event_table[index_t]; /* 与codex检查冲突，屏蔽掉 */
    if (oal_unlikely(frw_event_table->pst_sub_table == NULL)) {
        oam_error_log2(0, OAM_SF_FRW,
            "{frw_event_lookup_process_entry::sub table is NULL! sub_type[0x%x], index[0x%x].}", sub_type, index_t);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* For pipeline 1 type */
    if (event_para->en_pipeline != FRW_EVENT_PIPELINE_STAGE_0) {
        return  frw_pipe_1_event_process(event_para->netbuf, frw_event_table,
            event_para->en_type, sub_type, event_para->event_type);
    }

    /* For pipeline 0 type */
    if (event_para->en_pipeline == FRW_EVENT_PIPELINE_STAGE_0) {
        if (oal_unlikely(frw_event_table->pst_sub_table[sub_type].p_func == NULL)) {
            oam_error_log2(0, OAM_SF_FRW,
                "{frw_event_lookup_process_entry::sub type is NULL! sub_type[0x%x], index[0x%x].}", sub_type, index_t);
            return OAL_ERR_CODE_PTR_NULL;
        }
        frw_event_table->pst_sub_table[sub_type].p_func(event_para->netbuf);
        return OAL_SUCC;
    }
}

/*
 * 功能描述  : 事件分发接口(分发事件至核间通讯、事件队列、或者查表寻找相应事件处理函数)
 */
OAL_STATIC uint32_t frw_event_dispatch_event(frw_event_param_stru *event)
{
    /* 如果pipleline为1，根据事件类型，子类型以及分段号，转发相应的事件处理函数 */
    if (event->en_pipeline == FRW_EVENT_PIPELINE_STAGE_1) {
        return frw_event_lookup_process_entry(event);
    }

    /* 如果pipleline为0，则将事件入队。 */
    return frw_event_post_event(event, 0);
}

OAL_STATIC void frw_event_hdr_init(frw_event_stru *frw_event, frw_event_param_stru *param)
{
    frw_event_hdr_stru  *pst_hdr = &frw_event->st_event_hdr;

    pst_hdr->en_type       = param->type;
    pst_hdr->uc_sub_type   = param->subtype;
    pst_hdr->en_pipeline   = param->pipeline;
    pst_hdr->us_length     = param->payload_size;
    pst_hdr->device_id     = param->hal_device_id;
    pst_hdr->vap_id        = param->vap_id;
    pst_hdr->transfer_event_to = param->transfer_event_to;
}

OAL_STATIC void frw_generate_event_netbuf(frw_event_param_stru *param)
{
    frw_event_stru *frw_event = NULL;
    oal_netbuf_stru *netbuf = param->netbuf;
    uint8_t *event_payload = NULL;

    if (netbuf == NULL) {
        netbuf = oal_mem_netbuf_alloc(
            OAL_NORMAL_NETBUF, 1536, OAL_NETBUF_PRIORITY_HIGH); // payload 1536
    }

    if (netbuf == NULL) {
        return NULL;
    }

    /* 事件信息维护在sbuf cb(128 bytes) */
    frw_event = (frw_event_stru*)oal_netbuf_cb(netbuf);
    frw_event_hdr_init(frw_event, param);

    /* 事件体维护在sbuf payload(1536 bytes), 只包含事件长度 */
    event_payload = (uint8_t*)oal_netbuf_data(netbuf);
    /* 传递非skb需要拷贝信息到新申请的skb中, 后续流程交由skb流转, payload对象由业务负责释放或其他处理 */
    if (param->payload != NULL) {
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
        /* DMA拷贝 */
        memcpy_s(event_payload, param->payload_size, param->payload, param->payload_size);
#endif
    }
    param->netbuf = netbuf;
}

/* WIFI post event总入口 */
uint32_t frw_post_event(frw_event_param_stru *param)
{
    if (param == NULL) {
        return OAL_FAIL;
    }

    frw_generate_event_netbuf(param);

    if (frw_event_dispatch_event(param) != OAL_SUCC) {
        oam_warning_log2(param->vap_id, OAM_SF_ANY, "{frw_post_event::post event[%d][%d] failed!}",
            param->type, param->subtype);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
#endif
