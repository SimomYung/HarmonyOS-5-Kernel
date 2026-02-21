/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: Update and load independent calibration data
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#include "plat_debug.h"
#include "bfgx_dev.h"
#include "plat_pm.h"
#include "bfgx_data_parse.h"
#include "pcie_linux.h"
#include "plat_cali.h"
#include "cali_data_ops_default.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
#ifndef _PRE_NO_HISI_NVRAM
#define HISI_NVRAM_SUPPORT
#endif
#endif

/*
 * 函 数 名  : bfgx_get_nv_data_buf
 * 功能描述  : 返回保存bfgx nv数据的内存地址
 * 输出参数  : nv buf的长度
 * 返 回 值  : bfgx nv数据buf的地址，也可能是NULL
 */
STATIC void *bfgx_get_nv_data_buf(uint32_t *pul_len)
{
    mpxx_bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = (uint32_t)sizeof(pst_bfgx_cali_data_buf->auc_nv_data);

    ps_print_info("bfgx nv buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_nv_data;
}

/*
 * 函 数 名  : bfgx_get_ext_nv_data_buf
 * 功能描述  : 返回保存bfgx extended nv数据的内存地址
 * 输出参数  : nv extended buf的长度
 * 返 回 值  : bfgx extended nv数据buf的地址，也可能是NULL
 */
STATIC void *bfgx_get_ext_nv_data_buf(uint32_t *pul_len)
{
    mpxx_bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = (uint32_t)sizeof(pst_bfgx_cali_data_buf->auc_ext_nv_data);

    ps_print_info("bfgx ext nv buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_ext_nv_data;
}

STATIC void plat_bfgx_cali_data_test(void)
{
    mpxx_bfgx_cali_data_stru *pst_cali_data = NULL;
    uint32_t *p_test = NULL;
    uint32_t count, i;

    pst_cali_data = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;
    p_test = (uint32_t *)(pst_cali_data->auc_bfgx_data);
    count = (uint32_t)(sizeof(pst_cali_data->auc_bfgx_data) / sizeof(uint32_t));

    for (i = 0; i < count; i++) {
        p_test[i] = i;
    }
}

/*
 * 函 数 名  : mpxx_get_bfgx_cali_data
 * 功能描述  : 返回保存bfgx校准数据的内存首地址以及长度
 * 输入参数  : uint8  *buf:调用函数保存bfgx校准数据的首地址
 *            uint32_t buf_len:buf的长度
 *            uint32_t fw_addr 固件加载的地址
 * 返 回 值  : 0表示成功，-1表示失败
 */
STATIC int32_t mpxx_get_bfgx_cali_data(uint8_t *buf, uint32_t buf_len, uint8_t uart_index)
{
    int32_t ret;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *bfgx_data = NULL;
    mpxx_bfgx_cali_data_stru *bfgx_cali_data_buf = NULL;

    ps_core_d = ps_get_core_reference(uart_index);
    if (ps_core_d == NULL) {
        ps_print_err("ps_core is null\n");
        return -EFAIL;
    }

    bfgx_data = &ps_core_d->bfgx_info[BFGX_BT];
    if (bfgx_data->cali_buf == NULL) {
        ps_print_err("cali buf is NULL\n");
        return -ENOMEM;
    }

    if (g_cali_buffer_debug != 0) {
        plat_bfgx_cali_data_test();
    } else {
        bfgx_cali_data_buf = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;
        if (memcpy_s(bfgx_cali_data_buf->auc_bfgx_data, MPXX_BFGX_BT_CALI_DATA_SIZE,
                     bfgx_data->cali_buf, MPXX_BFGX_BT_CALI_DATA_SIZE) != EOK) {
            ps_print_err("mp16c copy buffer len[%d], fail\n", MPXX_BFGX_BT_CALI_DATA_SIZE);
            return -EFAIL;
        }
    }

    ret = save_get_bfgx_cali_data(buf, buf_len);
    if (ret < 0) {
        ps_print_err("save cali fail\n");
        return ret;
    }
    return SUCC;
}

/*
 * 函 数 名  : wifi_get_bfgx_rc_data_buf_addr
 * 功能描述  : 返回保存wifi rc code校准数据的内存地址
 */
void *wifi_get_bfgx_rc_data_buf_addr(uint32_t *pul_len)
{
    mpxx_bfgx_cali_data_stru *pst_bfgx_cali_buf = NULL;

    if (oal_any_null_ptr2(g_bfgx_cali_data_buf, pul_len)) {
        return NULL;
    }

    pst_bfgx_cali_buf = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;
    *pul_len = (uint32_t)sizeof(pst_bfgx_cali_buf->auc_wifi_rc_code_data);

    ps_print_info("wifi cali size is %d\n", *pul_len);

    return pst_bfgx_cali_buf->auc_wifi_rc_code_data;
}

EXPORT_SYMBOL(wifi_get_bfgx_rc_data_buf_addr);

/*
 * 函 数 名  : wifi_get_bt_cali_data_buf
 * 功能描述  : 返回保存wifi cali data for bt数据的内存地址
 * 输出参数  : wifi cali data for bt数据buf的长度
 * 返 回 值  : wifi cali data for bt数据buf的地址，也可能是NULL
 */
void *wifi_get_bt_cali_data_buf(uint32_t *pul_len)
{
    mpxx_bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (oal_any_null_ptr2(g_bfgx_cali_data_buf, pul_len)) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = (uint32_t)sizeof(pst_bfgx_cali_data_buf->auc_wifi_cali_for_bt_data);

    ps_print_info("bfgx wifi cali data for bt buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_wifi_cali_for_bt_data;
}

EXPORT_SYMBOL(wifi_get_bt_cali_data_buf);

/*
 * 函 数 名  : bfgx_get_cust_ini_data_buf
 * 功能描述  : 返回保存bfgx ini定制化数据的内存地址
 * 输出参数  : bfgx ini定制化数据buf的长度
 * 返 回 值  : bfgx ini数据buf的地址，也可能是NULL
 */
STATIC void *bfgx_get_cust_ini_data_buf(uint32_t *pul_len)
{
    mpxx_bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (mpxx_bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = (uint32_t)sizeof(pst_bfgx_cali_data_buf->auc_bt_cust_ini_data);

    ps_print_info("bfgx cust ini buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_bt_cust_ini_data;
}

bfgx_cali_data_ops_stru g_cali_data_ops_mpxx = {
    .bfgx_cali_data_len = MPXX_BFGX_CALI_DATA_BUF_LEN,
    .bt_cali_data_len = MPXX_BFGX_BT_CALI_DATA_SIZE,
    .nv_data_len = MPXX_BFGX_NV_DATA_SIZE,
    .cust_ini_data_len = MPXX_BFGX_BT_CUST_INI_SIZE,
    .get_bfgx_cali_data_ops = mpxx_get_bfgx_cali_data,
    .bfgx_get_nv_data_buf_ops = bfgx_get_nv_data_buf,
    .bfgx_get_ext_nv_data_buf_ops = bfgx_get_ext_nv_data_buf,
    .plat_bfgx_cali_data_test_ops = plat_bfgx_cali_data_test,
    .wifi_get_bfgx_rc_data_buf_addr_ops = wifi_get_bfgx_rc_data_buf_addr,
    .wifi_get_bt_cali_data_buf_ops = wifi_get_bt_cali_data_buf,
    .bfgx_get_cust_ini_data_buf_ops = bfgx_get_cust_ini_data_buf
};
