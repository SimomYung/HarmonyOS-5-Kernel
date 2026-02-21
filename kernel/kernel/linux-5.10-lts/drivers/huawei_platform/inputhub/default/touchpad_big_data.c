/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: touchpad bigdata source file
 * Author: huangjinyu <huangjinyu3@h-partners.com>
 * Create: 2024-12-10
 */

#include "touchpad_big_data.h"

#define HWLOG_TAG touchpad_big_data
HWLOG_REGIST();

static bool report_touchpad_bigdata(void)
{
    int ret;
    bool success = false;
    ret = report_behavior_bigdata(); // 上报用户行为数据
    if (ret) {
        reset_behavior_bigdata();
        success = true;
    }
    ret = report_fault_bigdata(); // 上报故障数据
    if (ret) {
        reset_fault_bigdata();
        success = true;
    }
    return success;
}

static bool report_behavior_bigdata(void)
{
#ifdef CONFIG_HUAWEI_OHOS_DSM
    struct hiview_hisysevent *event = NULL;
    int ret;

    event = hisysevent_create("THP", "DFT_STAT_NUM_EDGE_CLCIK", STATISTIC);
    if (event == NULL) {
        hwlog_err("create hisysevent failed in %s", __func__);
        return false;
    }

    char msg[MSG_REPORT_LEN] = { 0 };
    ret = sprintf_s(msg, sizeof(msg), "tpFingerNum[%d,%d,%d,%d,%d], Press[%d], Short[%d], Long[%d], Slide[%d]",
        g_tpBigData.tpFingerNum[0],
        g_tpBigData.tpFingerNum[1],
        g_tpBigData.tpFingerNum[2],
        g_tpBigData.tpFingerNum[3],
        g_tpBigData.tpFingerNum[4],
        g_tpBigData.tpPressNum,
        g_tpBigData.tpShortTouchNum,
        g_tpBigData.tpLongTouchNum,
        g_tpBigData.tpSlideNum
    );
    if (ret == SPRINTF_S_FAIL) {
        return false;
    }

    ret = hisysevent_put_string(event, "FAULT_MSG", msg);
    if (ret != 0) {
        hwlog_err("hisysevent put string failed in %s", __func__);
    }

    ret = hisysevent_write(event);
    if (ret != 0) {
        hwlog_err("hisysevent write failed in %s", __func__);
    }

	hisysevent_destroy(&event);

    return true;
#endif

    return false;
}

static bool report_fault_bigdata(void)
{
#ifdef CONFIG_HUAWEI_OHOS_DSM
    struct hiview_hisysevent *event = NULL;
    int ret;

    event = hisysevent_create("THP", "DMDI_THP_HARDWARE_ASSERTED", FAULT);
    if (event == NULL) {
        hwlog_err("create hisysevent failed in %s", __func__);
        return false;
    }

    char msg[MSG_REPORT_LEN] = { 0 };
    ret = sprintf_s(msg, sizeof(msg), "%s[%d], %s[%d], %s[%d], %s[%d], %s[%d], %s[%d]",
        faultNames[24], g_faultBigData[24],
        faultNames[25], g_faultBigData[25],
        faultNames[26], g_faultBigData[26],
        faultNames[0], g_faultBigData[0],
        faultNames[1], g_faultBigData[1],
        faultNames[21], g_faultBigData[21]);
    if (ret == SPRINTF_S_FAIL) {
        return false;
    }

    ret = hisysevent_put_string(event, "FAULT_MSG", msg);
    if (ret != 0) {
        hwlog_err("hisysevent put string failed in %s", __func__);
    }

    ret = hisysevent_write(event);
    if (ret != 0) {
        hwlog_err("hisysevent write failed in %s", __func__);
    }

	hisysevent_destroy(&event);

    return true;
#endif

    return false;
}

static void check_tp_overflow(uint16_t checkData)
{
    if (checkData >= UINT16_MAX) {
        report_touchpad_bigdata();
    }
}

static void get_finger_num_touchpad(uint8_t fingerNum)
{
    if (fingerNum > g_tpParm.maxFingerNumTouchPad) {
        g_tpParm.maxFingerNumTouchPad = fingerNum;
    }
}

static void rec_max_finger_num(void) {
    if (g_tpParm.maxFingerNumTouchPad > TP_FIN_MAX) {
        return;
    }
    g_tpBigData.tpFingerNum[g_tpParm.maxFingerNumTouchPad - 1]++;
    check_tp_overflow(g_tpBigData.tpFingerNum[g_tpParm.maxFingerNumTouchPad - 1]);
}

static int calculateDist(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

static void get_touchpad_ges(const TouchPadRawData *touchPadRawData)
{ // 点击 or 滑动
    if (touchPadRawData == NULL || touchPadRawData->touchInfo == NULL) {
        return;
    }
    POINT point;
    point.x = touchPadRawData->touchInfo[0].fingerX;
    point.y = touchPadRawData->touchInfo[0].fingerY;
    if (g_tpParm.tpFrameNum == 1) {
        g_tpParm.orgCursorPos = point;
    }
    if (g_tpParm.tpFrameNum > 1 && !g_tpParm.tpIsMove) {
        if (calculateDist(point.x, point.y, g_tpParm.orgCursorPos.x, g_tpParm.orgCursorPos.y) > 20000) {
            g_tpParm.tpIsMove = true;
        }
    }
}

static bool is_all_finger_up_touchpad(const TouchPadRawData *touchPadRawData)
{
    if (touchPadRawData == NULL) {
        return false;
    }
    for (size_t i = 0; i < TP_FIN_MAX; i++) {
        if (touchPadRawData->touchInfo[i].touchEvent != 0) {
            return false;
        }
    }
    return true;
}

static void rec_touchpad_ges(void)
{
    const uint16_t tpShortTouchThre = 40; // tp Short Touch Frame Thre
    if (g_tpParm.tpIsOneFinger) {
        if (g_tpParm.tpIsMove) {
            g_tpBigData.tpSlideNum++; // 滑动
            check_tp_overflow(g_tpBigData.tpSlideNum);
        } else {
            if (g_tpParm.tpFrameNum < tpShortTouchThre) {
                g_tpBigData.tpShortTouchNum++; // 单指点击
                check_tp_overflow(g_tpBigData.tpShortTouchNum);
            } else {
                g_tpBigData.tpLongTouchNum++;
                check_tp_overflow(g_tpBigData.tpLongTouchNum);
            }
        }
    }
}

static void reset_tp_param(void)
{
    int ret = memset_s(&g_tpParm, sizeof(g_tpParm), 0, sizeof(g_tpParm));
    if (ret != 0) {
        hwlog_err("memset_s failed in %s", __func__);
    }
    g_tpParm.tpIsOneFinger = true;
}

static void reset_behavior_bigdata(void)
{
    int ret = memset_s(&g_tpBigData, sizeof(g_tpBigData), 0, sizeof(g_tpBigData));
    if (ret != 0) {
        hwlog_err("memset_s g_tpBigData failed in %s", __func__);
    }
}

static void reset_fault_bigdata(void)
{
    int ret = memset_s(&g_faultBigData, sizeof(g_faultBigData), 0, sizeof(g_faultBigData));
    if (ret != 0) {
        hwlog_err("memset_s g_faultBigData failed in %s", __func__);
    }
}

static int report_tp_bigdata_timer(void *data)
{
    while (1) {
        if (!report_touchpad_bigdata()) {
            hwlog_err("failed to report bigdata");
            break;
        }
        msleep(TEN_MINUTE);
    }
    return 0;
}

static void fault_report_handle(FaultReportData *faultRawData) {
    uint8_t* faultInfo = faultRawData->faultInfo;
    if (faultInfo[0] == EDDY_SWEEP_ERROR) {
        uint16_t *scan_params = (uint16_t *)(faultRawData->faultInfo + 1);
        hwlog_err("touchpad eddy sweep: %d %d %d %d",
           scan_params[0], scan_params[1], scan_params[2], scan_params[3]);
        return;
    }
    if (faultInfo[0] == FORCE_BASELINE_ABNORAML) {
        uint16_t *curBase = (uint16_t *)(faultRawData->faultInfo + 1);
        hwlog_err("touchpad current baseline: %d %d %d %d",
            curBase[0], curBase[1], curBase[2], curBase[3]);
        return;
    }
    if (faultInfo != NULL && faultInfo[0] >= 0 && faultInfo[0] <= FAULT_TYPE_MAX) {
        g_faultBigData[faultInfo[0]]++;
        check_tp_overflow(g_faultBigData[faultInfo[0]]);
    }
}

void parse_touchpad_bigdata(TouchPadRawData *touchPadRawData, struct device *dev, uint16_t productId)
{
    if (touchPadRawData == NULL || dev == NULL) {
        return;
    }
    // 产品隔离
    if (productId != 0x7853 && productId != 0x01E0) {
        return;
    }

    // 故障打点处理
    FaultReportData* faultRawData = (FaultReportData *)touchPadRawData;
    if (faultRawData->reportId == 0x51 && faultRawData->commandId == 156) {
        fault_report_handle(faultRawData);
    }

    // 报点数据包合法性检测
    if (touchPadRawData->pkgLength != 0x2f || touchPadRawData->reportId != 4) {
        return;
    }

    if (!isBigDataTimerRunning) {
        struct task_struct *tsk = kthread_run(report_tp_bigdata_timer, (void *)dev, "report-tpbigdata-timer");
        if (IS_ERR(tsk)) {
            hwlog_err("Fail to start thread report-tpbigdata-timer.\n");
        } else {
            isBigDataTimerRunning = true;
        }
    }

    uint16_t curTime = touchPadRawData->scanTime;
    if (curTime == g_tpParm.lastTime) {
        return;
    } else {
        g_tpParm.lastTime = curTime;
    }
    g_tpParm.tpFrameNum++;
    // pressNum计数
    if (touchPadRawData->key == false && g_tpParm.lastPressState == true) {
        g_tpBigData.tpPressNum++;
        check_tp_overflow(g_tpBigData.tpPressNum);
    }
    g_tpParm.lastPressState = touchPadRawData->key;
    get_finger_num_touchpad(touchPadRawData->fingerNumber); // 手指数统计
    if (touchPadRawData->fingerNumber != 1) {
        g_tpParm.tpIsOneFinger = false;
    }
    if (g_tpParm.tpIsOneFinger) {
        get_touchpad_ges(touchPadRawData); // 初步判断是点击还是滑动
    }
    if (is_all_finger_up_touchpad(touchPadRawData)) { // 一次动作结束
        rec_max_finger_num();
        rec_touchpad_ges();                         // 手势
        reset_tp_param();                           // 清空
    }
}