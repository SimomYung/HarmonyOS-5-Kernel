/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:OAM MLOG
 * Author: @CompanyNameTag
 */
#ifdef _PRE_REPORT_MLOG
/* 头文件包含 */
#include "securec.h"
#include "oam_main.h"
#include "oam_mlog.h"
#include "oam_log.h"
#ifdef _PRE_HISI_HERTZ
#include "mlog_print_ker.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAM_MLOG_C

#define DRV_WIFI_MODULE          "WiFiDrv"
oam_mlog_stru g_oam_mlog;
/* 函数声明 */
OAL_STATIC void oam_mlog_wq(oal_work_stru *work_func);

/*****************************************************************************
 函 数 名  : oam_mlog_init
 功能描述  : 初始化产品log
 修改历史      :23.12.29移植
  1.日    期   : 2023年12月29日
*****************************************************************************/
void oam_mlog_init(void)
{
    uint32_t index;
    oam_mlog_rpt_stru *oam_mlog_rpt = NULL;
    oam_mlog_stru *oam_mlog = &g_oam_mlog;

    memset_s(oam_mlog, sizeof(oam_mlog_stru), 0, sizeof(oam_mlog_stru));
    oal_spin_lock_init(&oam_mlog->spin_lock);
    oam_mlog_rpt = oal_memalloc(sizeof(oam_mlog_rpt_stru) * OAM_ONT_LOG_MAX_COUNT);
    if (oam_mlog_rpt == NULL) {
        return;
    }

    /* 初始化工作队列 */
    oal_dlist_init_head(&oam_mlog->mlog_used_list);
    oal_dlist_init_head(&oam_mlog->mlog_free_list);
    oal_init_work(&oam_mlog->mlog_work, oam_mlog_wq);

    for (index = 0; index < OAM_ONT_LOG_MAX_COUNT; index++) {
        oal_dlist_add_tail(&oam_mlog_rpt[index].dlist_head_entry, &oam_mlog->mlog_free_list);
    }

    oam_mlog->mlog_mem = oam_mlog_rpt;
}

/*****************************************************************************
 函 数 名  : oam_reg_exit
 功能描述  : 去初始化产品log
 修改历史      :23.12.29移植
  1.日    期   : 2023年12月29日

*****************************************************************************/
void oam_mlog_exit(void)
{
    oam_mlog_stru *oam_mlog = &g_oam_mlog;

    if (oam_mlog->mlog_mem == NULL) {
        return;
    }

    oal_cancel_work_sync(&oam_mlog->mlog_work);
    oal_free(oam_mlog->mlog_mem);
    memset_s(oam_mlog, sizeof(oam_mlog_stru), 0, sizeof(oam_mlog_stru));
}

/*****************************************************************************
 函 数 名  : oam_mlog_wq
 功能描述  : workqueue的入口函数，调用产品日志接口
 修改历史      :23.12.29移植
  1.日    期   : 2023年12月29日

*****************************************************************************/
OAL_STATIC void oam_mlog_wq(oal_work_stru *work_func)
{
    oal_dlist_head_stru *list_entry = NULL;
    oam_mlog_rpt_stru *oam_mlog_rpt = NULL;
    oal_uint irq_save;
    oal_dlist_head_stru list_used;
    oam_mlog_stru *oam_mlog = &g_oam_mlog;

    if (oam_mlog->mlog_mem == NULL) {
        return;
    }

    oal_dlist_init_head(&list_used);

    oal_spin_lock_irq_save(&oam_mlog->spin_lock, &irq_save);
    if (!oal_dlist_is_empty(&oam_mlog->mlog_used_list)) {
        oal_dlist_remove_head(&list_used, &oam_mlog->mlog_used_list, oam_mlog->mlog_used_list.pst_prev);
    }
    oal_spin_unlock_irq_restore(&oam_mlog->spin_lock, &irq_save);

    // 从链表头部取一个节点
    oal_dlist_search_for_each(list_entry, &list_used)
    {
        // 获取节点指向的内容
        oam_mlog_rpt = oal_dlist_get_entry(list_entry, oam_mlog_rpt_stru, dlist_head_entry);
        // mlog日志分支
#ifdef _PRE_HISI_HERTZ
        mlog_print(DRV_WIFI_MODULE, oam_mlog_rpt->level, oam_mlog_rpt->log_string);
#endif
    }
    if (!oal_dlist_is_empty(&list_used)) {
        oal_spin_lock_irq_save(&oam_mlog->spin_lock, &irq_save);
        oal_dlist_join_tail(&oam_mlog->mlog_free_list, &list_used);
        oal_spin_unlock_irq_restore(&oam_mlog->spin_lock, &irq_save);
    }
}

/* 以下为日志上报部分接口 */

/*****************************************************************************
 函 数 名  : oam_mlog_new
 功能描述  : 申请一个新的log节点
 输入参数  : 无
 输出参数  : 节点指针
 返 回 值  : 节点指针
 修改内容   : 新生成函数

*****************************************************************************/
OAL_STATIC oam_mlog_rpt_stru *oam_mlog_new(void)
{
    oal_dlist_head_stru *list_entry = NULL;
    uintptr_t irq_save;
    oam_mlog_stru *oam_mlog = &g_oam_mlog;

    if (oam_mlog->mlog_mem == NULL) {
        return NULL;
    }
    oal_spin_lock_irq_save(&oam_mlog->spin_lock, &irq_save);
    if (!oal_dlist_is_empty(&oam_mlog->mlog_free_list)) {
        list_entry = oal_dlist_delete_head(&oam_mlog->mlog_free_list);
    }
    oal_spin_unlock_irq_restore(&oam_mlog->spin_lock, &irq_save);
    if (list_entry == NULL) {
        return NULL;
    }
    return oal_dlist_get_entry(list_entry, oam_mlog_rpt_stru, dlist_head_entry);
}

/*****************************************************************************
 函 数 名  : oam_mlog_free
 功能描述  : 释放一个log节点
 输入参数  : 节点指针
 输出参数  : 无
 修改内容   : 新生成函数

*****************************************************************************/
OAL_STATIC void oam_mlog_free(oam_mlog_rpt_stru *oam_mlog_rpt)
{
    uintptr_t irq_save;
    oam_mlog_stru *oam_mlog = &g_oam_mlog;

    if ((oam_mlog->mlog_mem == NULL) || (oam_mlog_rpt == NULL)) {
        return;
    }

    oal_spin_lock_irq_save(&oam_mlog->spin_lock, &irq_save);
    oal_dlist_add_tail(&oam_mlog_rpt->dlist_head_entry, &oam_mlog->mlog_free_list);
    oal_spin_unlock_irq_restore(&oam_mlog->spin_lock, &irq_save);
}

/*****************************************************************************
 函 数 名  : oam_mlog_add
 功能描述  : 将log节点添加到used链表
 输入参数  : oam_mlog_rpt 节点指针
 输出参数  : 无
 修改内容   : 新生成函数

*****************************************************************************/
OAL_STATIC uint32_t oam_mlog_add(oam_mlog_rpt_stru *oam_mlog_rpt)
{
    uintptr_t irq_save;
    oam_mlog_stru *oam_mlog = &g_oam_mlog;

    if (oam_mlog->mlog_mem == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_spin_lock_irq_save(&oam_mlog->spin_lock, &irq_save);
    oal_dlist_add_tail(&oam_mlog_rpt->dlist_head_entry, &oam_mlog->mlog_used_list);
    oal_spin_unlock_irq_restore(&oam_mlog->spin_lock, &irq_save);

    oal_workqueue_schedule(&oam_mlog->mlog_work);
    return OAL_SUCC;
}

#ifdef _PRE_HISI_HERTZ
/* 枚举值位于头文件 mlog_print_ker.h 中 */
OAL_STATIC void oam_mlog_level_adapt(oam_mlog_rpt_stru *log_rpt, oam_log_level_enum log_level)
{
    if (log_level == OAM_LOG_LEVEL_ERROR) {
        log_rpt->level = MLOG_LEVEL_ERROR;
    } else {
        log_rpt->level = MLOG_LEVEL_WARN;
    }
}
#endif

OAL_STATIC void  oam_report_mlog(uint32_t para,
    int8_t *param_string, oal_uint param1, oal_uint param2, oal_uint param3, oal_uint param4)
{
    oam_log_level_enum log_level = get_log_lev(para);
    int32_t str_len = 0;
    oam_mlog_rpt_stru *oam_mlog_rpt = oam_mlog_new();

    if (oam_mlog_rpt == NULL) {
        return;
    }

    str_len = sprintf_s(oam_mlog_rpt->log_string,
        sizeof(oam_mlog_rpt->log_string) - 1, param_string,
        (int32_t)param1, (int32_t)param2, (int32_t)param3, (int32_t)param4);
    if (str_len <= 0) {
        oam_mlog_free(oam_mlog_rpt);
        return;
    }

    if (oam_mlog_rpt->log_string[str_len - 1] != '\n') {
        if (str_len > (int32_t)sizeof(oam_mlog_rpt->log_string) - 2) { // 最后2个字符需要固定修改
            str_len = (int32_t)sizeof(oam_mlog_rpt->log_string) - 2; // 最后2个字符需要固定修改
        }
        oam_mlog_rpt->log_string[str_len]   = '\n';
        oam_mlog_rpt->log_string[str_len + 1] = 0;
    }
#ifdef _PRE_HISI_HERTZ
    oam_mlog_level_adapt(oam_mlog_rpt, log_level);
#endif
    // 添加节点
    oam_mlog_add(oam_mlog_rpt);
}

OAL_STATIC uint8_t oam_check_feature_need_report(uint8_t feature_id)
{
    // 若有新的特性需要上报产品，在此添加特性宏枚举值
    uint8_t report_feature[] = {
        OAM_SF_MLOG
        };
    uint8_t max_len = sizeof(report_feature) / sizeof(uint8_t);
    uint8_t index = 0;
    for (index = 0; index < max_len; index++) {
        if (report_feature[index] == feature_id) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

/*****************************************************************************
 函 数 名  : oam_report_mlog_enter
 功能描述  : mlog上报对外提供的总入口
 输出参数  : 无
 修改历史      :
  1.日    期   : 2024年01月02日
*****************************************************************************/
void oam_report_mlog_enter(uint32_t para,
    int8_t *param_string, oal_uint param1, oal_uint param2, oal_uint param3, oal_uint param4)
{
    uint8_t feature_id = get_feature_id(para);

    if (param_string == NULL) {
        return;
    }

    /* 如果特性不需要上报，则直接返回 */
    if (oam_check_feature_need_report(feature_id) != OAL_TRUE) {
        return;
    }

    oam_report_mlog(para, param_string, param1, param2, param3, param4);
}

#endif
