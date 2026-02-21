/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: touchpad bigdata header file
 * Author: huangjinyu <huangjinyu3@h-partners.com>
 * Create: 2024-12-10
 */

#ifndef _TOUCHPAD_BIGDATA_H_
#define _TOUCHPAD_BIGDATA_H_

#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <dfx/hiview_hisysevent.h>
#endif
#include <securec.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <huawei_platform/log/hw_log.h>

#define UINT16_MAX ((uint16_t)(-1))
#define TOUCHPAD_BIGDATA

#define TP_FIN_MAX 5
#define TP_AREA_NUM 11
#define TEN_MINUTE 600000
#define FAULT_INFO_MAX 64
#define FAULT_TYPE_MAX 56
#define MSG_REPORT_LEN 256
#define SPRINTF_S_FAIL (-1)
#define EDDY_SWEEP_ERROR 22
#define FORCE_BASELINE_ABNORAML 23
#define HID_ENUM_TIMEOUT 41

typedef struct {
    uint16_t x;
    uint16_t y;
} POINT;

typedef struct {
    uint16_t tpFingerNum[TP_FIN_MAX];
    uint16_t tpPressNum;
    uint16_t tpShortTouchNum;
    uint16_t tpLongTouchNum;
    uint16_t tpSlideNum;
    uint16_t tpF2Touch;
} TouchPadBigData;

typedef enum {
    ZERO_FINGER,
    ONE_FINGER,
    TWO_FINGER,
    THREE_FINGER,
    FOUR_FINGER,
    FIVE_FINGER,
    SIX_FINGER,
    SEVEN_FINGER,
    EIGHT_FINGER,
    NINE_FINGER,
    TEN_FINGER
} FingerState;

typedef struct {
    uint16_t lastTime;
    uint8_t lastPressState;
    uint16_t maxFingerNumTouchPad;
    POINT orgCursorPos;
    uint16_t tpFrameNum;
    bool tpIsMove;
    bool tpIsOneFinger;
} __packed TpParm;

typedef struct {
    uint8_t touchValid : 1;
    uint8_t touchEvent : 1;
    uint8_t reserve : 3;
    uint8_t id : 3;
    uint16_t fingerX;
    uint16_t fingerY;
    uint8_t width;
    uint8_t height;
    uint8_t pressure;
} __packed FingerRawData;

typedef struct {
    uint16_t pkgLength;
    uint8_t reportId;
    FingerRawData touchInfo[TP_FIN_MAX];
    uint16_t scanTime;
    uint8_t fingerNumber;
    uint8_t key : 1;
    uint8_t reserve : 7;
} __packed TouchPadRawData;

typedef struct {
    uint16_t pkgLength;
    uint8_t reportId;
    uint8_t commandId;
    uint8_t faultInfo[FAULT_INFO_MAX];
} FaultReportData;

static const char *faultNames[] = {
    "TP_IIC2_MASTER_WRITE",
    "FIRST_READ_TP_FAIL",
    "CHECK_VENDOR_TYPE_FAIL",
    "IIC1_ERROR_NONE",
    "IIC1_ERROR_OTHER",
    "IIC1_ERROR_LOW_TIMEOUT",
    "IIC1_ERROR_RESET_OVER",
    "IIC2_CALLBACK_ERROR",
    "IIC2_BUSY_ERROR",
    "IIC2_ERROR_RESET_OVER",
    "IIC2_ERROR_LOW_TIMEOUT",
    "IIC3_ERROR_LOW_TIMEOUT",
    "IIC3_ERROR_RESET_OVER",
    "REPEAT_SET_ERR",
    "REPEAT_GET_ERR_BUSY",
    "REPEAT_GET_ERR_ERROR",
    "REPEAT_GET_ERR_BYPASS",
    "REPEAT_TP_INT_RELEASE",
    "REPEAT_READ_TP_FAIL",
    "MEMSET_ERR",
    "I2C_RESET_LOW_LEVEL",
    "MEIJU_STEP_LOG",
    "EDDY_SWEEP_ERROR",
    "FORCE_BASELINE_ABNORAML",
    "TPIC_ABNORMAL",
    "MOTOR_VIBRATION_ABNORMAL",
    "WATCHDOG_TIMEOUT",
    "HID_FIFO_LEN_OVER",
    "HID_FIFO_POINT_NULL",
    "HID_START_RCV_FAIL",
    "HID_FISRT_GET_FAIL",
    "HID_FISRT_GET_ERR",
    "HID_GET_FW_FAIL",
    "HID_GET_FT_STATUS_FAIL",
    "HID_SEND_HID_DESC_FAIL",
    "HID_SEND_REPORT_DISC_FAIL",
    "HID_PWR_ON_BYPASS_FAIL",
    "HID_SLEEP_BYPASS_FAIL",
    "HID_RESET_BYPASS_FAIL",
    "HID_VIB_IN_GET_LOG",
    "HID_SEND_INPUT_FAIL",
    "HID_ENUM_TIMEOUT",
    "HID_UNKNOWN_GET_CMD",
    "HID_UNKNOWN_GET_ID",
    "HID_INT_TIMEOUT",
    "HID_BYPASS_FAIL",
    "HID_CMD_FIFO_FULL",
    "HID_REAPT_RESET_ZERO",
    "HID_REAPT_RESET_FAIL",
    "HID_REAPT_RESET_ERR",
    "HID_REAPT_RESET_ERR2",
    "ELAN_SET_WETHAN_FAIL1",
    "ELAN_SET_WETHAN_FAIL2",
    "ELAN_GET_WETHAN_FAIL",
    "ELAN_CLOSE_WETHAN_FAIL",
    "HID_GET_VENDOR_INFO_FAIL",
};

static TpParm g_tpParm;
static TouchPadBigData g_tpBigData;
static uint16_t g_faultBigData[FAULT_TYPE_MAX];
static bool isBigDataTimerRunning = false;

static bool report_touchpad_bigdata(void);
static bool report_behavior_bigdata(void);
static bool report_fault_bigdata(void);
static void check_tp_overflow(uint16_t checkData);
static void get_finger_num_touchpad(uint8_t fingerNum);
static void get_touchpad_ges(const TouchPadRawData *touchPadRawData);
static bool is_all_finger_up_touchpad(const TouchPadRawData *touchPadRawData);
static void rec_touchpad_ges(void);
static void reset_tp_param(void);
static void reset_behavior_bigdata(void);
static void reset_fault_bigdata(void);
static int report_tp_bigdata_timer(void *data);
static void fault_report_handle(FaultReportData *faultRawData);
void parse_touchpad_bigdata(TouchPadRawData *touchPadRawData, struct device *dev, uint16_t productId);

#endif