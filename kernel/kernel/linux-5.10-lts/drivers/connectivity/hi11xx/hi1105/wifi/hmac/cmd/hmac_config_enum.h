/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_config_enum.h 的头文件
 * 作    者 :
 * 创建日期 : 2013年1月8日
 */

#ifndef HMAC_CONFIG_ENUM_H
#define HMAC_CONFIG_ENUM_H
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define SHORTGI_CFG_STRU_LEN 4

#define HMAC_MAX_MCS_NUM               8 /* 单流和双流支持的mac最大个数 */
#define HAMC_QUERY_INFO_FROM_DMAC_TIME (5 * OAL_TIME_HZ)
#ifdef _PRE_EMU
#define HMAC_WAIT_EVENT_RSP_TIME 100 /* 等待配置命令事件完成的超时时间(s) */
#else
#define HMAC_WAIT_EVENT_RSP_TIME 10 /* 等待配置命令事件完成的超时时间(s) */
#endif
#define HMAC_S_TO_MS 1000   /* 秒和毫秒的转换系数 */
#define HMAC_MS_TO_US 1000   /* 毫秒和微秒的转换系数 */
#define HMAC_S_TO_US 1000000   /* 秒和微秒的转换系数 */
#define HMAC_US_TO_NS 1000   /* 微秒和纳秒的转换系数 */
#define HMAC_NETBUF_REMAIN_MAX_DIFF_LEN 104 /* 帧头的填充部分长度 */
/* APUT OWE group definition, hipriv.sh BIT format transit to pst_hmac_vap->owe_group */
#define WAL_HIPRIV_OWE_19 BIT(0)
#define WAL_HIPRIV_OWE_20 BIT(1)
#define WAL_HIPRIV_OWE_21 BIT(2)
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif