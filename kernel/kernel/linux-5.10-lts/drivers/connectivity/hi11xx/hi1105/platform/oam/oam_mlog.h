/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:oam_mlog.c header file
 * Author: @CompanyNameTag
 */

#ifndef OAM_MLOG_H
#define OAM_MLOG_H

#ifdef _PRE_REPORT_MLOG
/* 其他头文件包含 */
#include "oal_ext_if.h"
#include "oal_util.h"

#define OAM_ONT_LOG_STRING_LIMIT  200
#define OAM_ONT_LOG_MAX_COUNT     200

typedef struct {
    oal_dlist_head_stru   dlist_head_entry;
    uint8_t               chip_id;
    uint8_t               event_id;
    uint8_t               level;
    uint8_t               resv[1];
    int8_t                log_string[OAM_ONT_LOG_STRING_LIMIT];
} oam_mlog_rpt_stru;
// ont log节点信息
typedef struct {
    oal_spin_lock_stru                       spin_lock;
    oal_dlist_head_stru                      mlog_used_list;
    oal_dlist_head_stru                      mlog_free_list;
    oal_work_stru                            mlog_work;
    oam_mlog_rpt_stru                        *mlog_mem;
} oam_mlog_stru;
void oam_mlog_init(void);
void oam_mlog_exit(void);
void oam_report_mlog_enter(uint32_t para,
    int8_t *param_string, oal_uint param1, oal_uint param2, oal_uint param3, oal_uint param4);

#endif
#endif /* end of oam_mlog.h */
