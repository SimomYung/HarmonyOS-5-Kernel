/**
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 *
 * Description: 用于产线测试用例增强版本
 * Author: @CompanyNameTag
 */
#define HISI_LOG_TAG "[HCC_TEST_ENHANCE]"
#include "plat_pm_wlan.h"
#include "oal_schedule.h"
#include "oal_workqueue.h"
#include "oal_dft.h"
#include "oal_hcc_bus.h"
#include "oal_hcc_test.h"
#include "pcie_linux_mpxx.h"
#include "oal_hcc_test_enhance.h"

#ifdef CONFIG_MPXX_HCC_TEST_ENHANCE
#define HCC_TEST_ENHANCE_TIMER_DEFUALT_TIMEOUT 1000 // ms

static oal_workqueue_stru *g_test_loop_workqueue;
static oal_work_stru g_test_loop_work;
static oal_mutex_stru g_test_req_lock; /* request from ioctl etc. */
static uint32_t g_timeout_ms = 0;
static uint32_t g_interval_ms = 0;
static hcc_test_enhance_result g_hcc_test_enhance_result;
static bool g_test_loop_req_flag = false;

static void test_loop_worker_func(oal_work_stru *worker)
{
    int32_t ret1, ret2;
    uint32_t timeout_ms, dfr_sts;
    uint32_t interval_ms;
    uint64_t throuput = 0;
    declare_time_cost_stru(cost);
    if (!g_test_loop_req_flag) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "test worker request flag is false, abort");
        return;
    }
    timeout_ms = g_timeout_ms;
    interval_ms = g_interval_ms;
    (void)memset_s(&g_hcc_test_enhance_result, sizeof(g_hcc_test_enhance_result), 0, sizeof(g_hcc_test_enhance_result));
    g_hcc_test_enhance_result.errno_run = -OAL_ENODEV;
    g_hcc_test_enhance_result.errno_check = -OAL_ENODEV;
    g_test_loop_req_flag = false; // 请求已处理，下次需要再次触发
    /* 关闭WiFi低功耗 */
    wlan_pm_disable();
    /* 关闭WiFi DFR */
    dfr_sts = hcc_get_exception_status();
    hcc_set_exception_status(0);
    oal_get_time_cost_start(cost);
    ret1 = hcc_test_loop_pcie_chan(interval_ms, timeout_ms, &throuput);
    g_hcc_test_enhance_result.errno_run = ret1;
    if (ret1 != OAL_SUCC) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "test pcie loop failed, ret=%d", ret1);
    }
    g_hcc_test_enhance_result.throuput = throuput;
    /* 不管成功失败，获取PCIE误码率 */
    ret2 = oal_pcie_get_link_quality_info(hcc_get_bus(HCC_EP_WIFI_DEV),
                                          &g_hcc_test_enhance_result.tx_ber,
                                          &g_hcc_test_enhance_result.rx_ber);
    g_hcc_test_enhance_result.errno_check = ret2;
    if (ret2 != OAL_SUCC) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "get link qulity info failed, ret=%d", ret2);
    }
    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    g_hcc_test_enhance_result.real_runtime_us = time_cost_var_sub(cost);
    oal_print_mpxx_log(MPXX_LOG_INFO, "test worker end, tx_ber=0x%x, rx_ber=0x%x, throuput=%u Mbps",
                       g_hcc_test_enhance_result.tx_ber, g_hcc_test_enhance_result.rx_ber,
                       g_hcc_test_enhance_result.throuput);
    hcc_set_exception_status(dfr_sts);
}

static inline void test_loop_reqlock(void)
{
    oal_mutex_lock(&g_test_req_lock);
}

static inline void test_loop_requnlock(void)
{
    oal_mutex_unlock(&g_test_req_lock);
}

/* 用例执行时起线程worker，并启动定时器，超时后worker退出，定时器回调获取结果 */
static int32_t test_loop_start_work(uint32_t timeout_ms, uint32_t interval_ms)
{
    /* 创建一个workqueue,异步处理 */
    if (g_test_loop_workqueue != NULL) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "workqueue is busy, double trigger");
        return -OAL_EIO;
    }
    g_test_loop_workqueue = oal_create_singlethread_workqueue_m("hcc_test_loop_enhance");
    if (g_test_loop_workqueue == NULL) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "create workqueue failed!");
        return -OAL_ENOMEM;
    }
    g_timeout_ms = timeout_ms;
    g_interval_ms = interval_ms;
    g_test_loop_req_flag = true;
    oal_init_work(&g_test_loop_work, test_loop_worker_func);
    if (!oal_queue_work(g_test_loop_workqueue, &g_test_loop_work)) {
        oal_destroy_workqueue(g_test_loop_workqueue);
        g_test_loop_workqueue = NULL;
        oal_print_mpxx_log(MPXX_LOG_ERR, "queue work failed, work busy");
        return -OAL_EBUSY;
    }
    oal_print_mpxx_log(MPXX_LOG_INFO, "start test worker, timeout %u ms, interval %u ms", timeout_ms, interval_ms);
    return OAL_SUCC;
}

static int32_t test_loop_stop_work(void)
{
    if (g_test_loop_workqueue == NULL) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "no work running, stop failed, maybe double stop");
        return -OAL_ENODEV;
    }
    if (oal_work_is_busy(&g_test_loop_work)) {
        oal_print_mpxx_log(MPXX_LOG_INFO, "test loop is running, try to abort");
        // 提前终止
        hcc_test_loop_pcie_chan_abort();
    }
    oal_cancel_work_sync(&g_test_loop_work);
    oal_destroy_workqueue(g_test_loop_workqueue);
    g_test_loop_workqueue = NULL;
    return OAL_SUCC;
}

/* 启动/终止 HCC回环测试任务，指定运行时间，占空比，超时停止或主动停止时获取运行结果 */
int32_t conn_test_pcie_loop_enhance_start(uint32_t timeout_ms, uint32_t interval_ms)
{
    int32_t ret;
    oal_print_mpxx_log(MPXX_LOG_INFO, "test pcie loop enhance req start begin");
    test_loop_reqlock();
    ret = test_loop_start_work(timeout_ms, interval_ms);
    if (ret != OAL_SUCC) {
        test_loop_requnlock();
        return ret;
    }
    test_loop_requnlock();
    oal_print_mpxx_log(MPXX_LOG_INFO, "test pcie loop enhance req start end");
    return OAL_SUCC;
}
oal_module_symbol(conn_test_pcie_loop_enhance_start);

int32_t conn_test_pcie_loop_enhance_stop(hcc_test_enhance_result *result)
{
    int32_t ret;
    errno_t errno_ret;
    if (oal_warn_on(result == NULL)) {
        return -OAL_ENOMEM;
    }
    oal_print_mpxx_log(MPXX_LOG_INFO, "test pcie loop enhance req stop begin");
    test_loop_reqlock();
    ret = test_loop_stop_work();
    if (ret != OAL_SUCC) {
        test_loop_requnlock();
        return ret;
    }
    errno_ret = memcpy_s(result, sizeof(*result), &g_hcc_test_enhance_result, sizeof(g_hcc_test_enhance_result));
    if (errno_ret != EOK) {
        /* 结构体一致，这里不应该触发 */
        oal_print_mpxx_log(MPXX_LOG_ERR, "memcpy_s failed, errno_ret=%d", errno_ret);
        test_loop_requnlock();
        return -OAL_ENOMEM;
    }
    test_loop_requnlock();
    oal_print_mpxx_log(MPXX_LOG_INFO, "test pcie loop enhance req stop end");
    return OAL_SUCC;
}
oal_module_symbol(conn_test_pcie_loop_enhance_stop);

/* system test */
int32_t conn_test_pcie_loop_enhance_stop_tc001(void)
{
    int32_t ret;
    hcc_test_enhance_result result = { 0 };
    ret = conn_test_pcie_loop_enhance_stop(&result);
    if (ret != OAL_SUCC) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "test pcie loop enhance req stop failed, ret=%d", ret);
    } else {
        oal_print_mpxx_log(MPXX_LOG_INFO, "result: tx_ber=0x%x, rx_ber=0x%x, throuput=%u Mbps, "\
                           "errno_run=%d, errno_check=%d, real_runtime_us=%llu us",
                           result.tx_ber, result.rx_ber, result.throuput,
                           result.errno_run, result.errno_check, result.real_runtime_us);
    }
    return ret;
}

void hcc_test_enhance_init(void)
{
    mutex_init(&g_test_req_lock);
}

void hcc_test_enhance_deinit(void)
{
    mutex_destroy(&g_test_req_lock);
}
#endif