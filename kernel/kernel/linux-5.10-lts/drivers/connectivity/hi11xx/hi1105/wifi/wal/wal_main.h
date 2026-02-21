/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : wal_main.c 的头文件
 * 作    者 :
 * 创建日期 : 2012年9月18日
 */

#ifndef WAL_MAIN_H
#define WAL_MAIN_H

#include "oal_ext_if.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_MAIN_H

#ifdef _PRE_WLAN_EXPORT
#define NB_GET_REPORT_INFO 0         /* 无人机获取参数消息 */
#define MAX_NB_ACTION_LEN 128   /* 窄带最长action帧长 */
#define OAL_NB_DATA_BUF_LEN   128
#define NETLINK_NB_REPORT 26
struct nb_globals {
    oal_sock_stru *nlsk;
    uint8_t mode;
    uint8_t *data;
    uint32_t usepid;
};

struct nb_msg_hdr_stru {
    uint32_t cmd;
    uint32_t len;
};
#endif

typedef enum {
    WAL_HOST_CRX_SUBTYPE_CFG,
    WAL_HOST_CRX_SUBTYPE_RESET_HW,
    WAL_HOST_CRX_SUBTYPE_BUTT
} wal_host_crx_subtype_enum;

typedef enum {
    WAL_HOST_DRX_SUBTYPE_TX,
    WAL_HOST_DRX_SUBTYPE_BUTT
} wal_host_drx_subtype_enum;

extern oam_wal_func_hook_stru g_st_wal_drv_func_hook;

int32_t wal_main_init(void);
void wal_main_exit(void);
extern oal_wakelock_stru g_st_wal_wakelock;
static inline void wal_wake_lock(void)
{
    oal_wake_lock(&g_st_wal_wakelock);
}
static inline void wal_wake_unlock(void)
{
    oal_wake_unlock(&g_st_wal_wakelock);
}

#endif /* end of wal_main */
