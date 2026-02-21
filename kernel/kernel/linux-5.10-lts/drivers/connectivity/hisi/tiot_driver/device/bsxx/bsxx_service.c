/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description: BSXX service interface. \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-03-31, Create file. \n
 */
#include "bsxx_service.h"
#include "bsxx_device_info.h"
#include "bsxx_board_port.h"
#include "tiot_board_log.h"
#include "tiot_controller.h"
#include "tiot_controller_helper.h"
#include "tiot_pm_wakelock.h"
#include "tiot_packet.h"
#include "tiot_sys_msg_types.h"
#include "tiot_board_pin_port.h"
#include "tiot_board_uart_port.h"
#include "tiot_board_misc_port.h"
#include "tiot_board.h"
#include "tiot_service_interface.h"
#include "tiot_service_interface_ext.h"

#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
/* 加载 & 数据收发复用，不同时使用 */
/* 数据收发暂定5K用于缓存数据. */
/* 固件加载时 ~3K用于保存解析后命令, ~128字节用于加载回复接收. */
#define BSXX_FW_LOAD_REUSE_BUFF_START 0U
#define BSXX_FW_LOAD_RX_BUFF_START    BSXX_FW_LOAD_REUSE_BUFF_START
#define BSXX_FW_LOAD_RX_BUFF_SIZE     128U

#define BSXX_FW_LOAD_REUSE_BUFF_END   (BSXX_FW_LOAD_RX_BUFF_START + BSXX_FW_LOAD_RX_BUFF_SIZE)

#define BSXX_COMMON_BUFF_MAX    ((CONFIG_BSXX_PACKET_BUFF_MAX > BSXX_FW_LOAD_REUSE_BUFF_END) ? \
                                 CONFIG_BSXX_PACKET_BUFF_MAX : BSXX_FW_LOAD_REUSE_BUFF_END)
#endif

#define BSXX_OPEN_FLAGS_FLOWCTRL_BIT   0

#define BSXX_PACKET_QUEUE_NODES    64

typedef enum {
    BSXX_RX_SUBSYS_0 = 0,
    BSXX_RX_SUBSYS_1 = 1,
    BSXX_RX_SUBSYS_MAX
} bsxx_rx_subsys_id;

static tiot_controller g_bsxx_controllers[CONFIG_BSXX_DEV_NUM] = { 0 };
/* 仅UART需要，使用SPI可优化. */
static tiot_packet_context g_bsxx_pkt_ctx[CONFIG_BSXX_DEV_NUM];
static tiot_packet g_bsxx_pkt_queue_nodes[CONFIG_BSXX_DEV_NUM][BSXX_PACKET_QUEUE_NODES];
static tiot_packet g_bsxx_oml_pkt_queue_nodes[CONFIG_BSXX_DEV_NUM][BSXX_PACKET_QUEUE_NODES];
static tiot_packet_queue g_bsxx_pkt_queues[CONFIG_BSXX_DEV_NUM][BSXX_RX_SUBSYS_MAX];
#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
static uint8_t g_bsxx_common_buff_mem[CONFIG_BSXX_DEV_NUM][BSXX_COMMON_BUFF_MAX];
#endif

void bsxx_service_close_ext(uint8_t dev_id);
int32_t tiot_pm_send_disallow_sleep(tiot_xfer_manager *xfer);
int32_t tiot_pm_check_sleeped(uint32_t wakein_pin, uint8_t check_times);

static uint16_t bsxx_packet_manager_match_func(uint32_t subsys_code)
{
    if ((subsys_code == GNSS_FIRST_MSG) || (subsys_code == GNSS_COMMON_MSG) || (subsys_code == GNSS_LAST_MSG)) {
        return BSXX_RX_SUBSYS_0;
    }
    if (subsys_code == OML_MSG) {
        return BSXX_RX_SUBSYS_1;
    }
    return BSXX_RX_SUBSYS_MAX;
}

static void bsxx_device_wakeup_host_cb(uint32_t pin)
{
    tiot_controller *cur_ctrl;
    cur_ctrl = tiot_find_controller_by_pin(g_bsxx_controllers, CONFIG_BSXX_DEV_NUM,
                                           pin, BSXX_PIN_DEVICE_WAKEUP_HOST);
    if (cur_ctrl != NULL) {
        tiot_pm_set_event(&cur_ctrl->pm, TIOT_PM_EVENT_WAKEUP_ACK);
    }
}

static int32_t bsxx_service_packet_init(tiot_controller *ctrl, uint8_t dev_id)
{
    int32_t ret;
    tiot_packet_context *pkt_ctx = NULL;
#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
    tiot_packet_buffer pkt_buff = { g_bsxx_common_buff_mem[dev_id], CONFIG_BSXX_PACKET_BUFF_MAX, 0 };
#endif
    tiot_packet_context_param param = {
        .flags = 0,
        .rx_manager_info = { g_bsxx_pkt_queues[dev_id], BSXX_RX_SUBSYS_MAX, bsxx_packet_manager_match_func },
        .buff = NULL
    };
#ifndef CONFIG_BOARD_DYNAMIC_ALLOC
    param.buff = &pkt_buff;
#endif
    g_bsxx_pkt_queues[dev_id][BSXX_RX_SUBSYS_0].queue_nodes = g_bsxx_pkt_queue_nodes[dev_id];
    g_bsxx_pkt_queues[dev_id][BSXX_RX_SUBSYS_0].queue_size = BSXX_PACKET_QUEUE_NODES;
    g_bsxx_pkt_queues[dev_id][BSXX_RX_SUBSYS_1].queue_nodes = g_bsxx_oml_pkt_queue_nodes[dev_id];
    g_bsxx_pkt_queues[dev_id][BSXX_RX_SUBSYS_1].queue_size = BSXX_PACKET_QUEUE_NODES;
    pkt_ctx = &g_bsxx_pkt_ctx[dev_id];
    ret = tiot_packet_init(pkt_ctx, &ctrl->transfer, &param);
    if (ret != 0) {
        tiot_print_err("[TIOT]packet init fail.\r\n");
    }
    return ret;
}

static int32_t bsxx_service_install_packet(tiot_controller *ctrl, uint8_t dev_id)
{
    tiot_xfer_packet_ops bsxx_packet_ops = {
        .read_out = tiot_packet_read_out,
        .rx_data_store = tiot_packet_rx_data_store,
        .tx_push = tiot_packet_tx_push,
    };
    uint32_t power_ctrl_pin;
    const tiot_device_timings *timings = &ctrl->dev_info->timings;
    /* 收发模块注册包格式 */
    if (tiot_xfer_install_packet(&ctrl->transfer, (tiot_xfer_packet_ops *)&bsxx_packet_ops,
                                 (uintptr_t)&g_bsxx_pkt_ctx[dev_id]) != 0) {
        tiot_print_err("[TIOT]packet install fail.\r\n");
        return -1;
    }
    power_ctrl_pin = ctrl->pm.pm_info[TIOT_PIN_POWER_CTRL];
    /* 主控不控制上下电时，不需要等待UART状态. */
    if (power_ctrl_pin != TIOT_PIN_NONE) {
        /* 必须等待device UART初始化后才能接收消息, 待确认时间. */
        osal_msleep(timings->init_time_ms);
    }
    return 0;
}

static int32_t service_register_packet(tiot_controller *ctrl, uint8_t dev_id)
{
    int32_t ret;
    ret = bsxx_service_packet_init(ctrl, dev_id);
    if (ret != 0) {
        return -1;
    }
    ret = bsxx_service_install_packet(ctrl, dev_id);
    if (ret != 0) {
        tiot_packet_deinit(&g_bsxx_pkt_ctx[dev_id]);
    }
    return ret;
}

static inline int32_t bsxx_get_dev_id(tiot_controller *ctrl, uint8_t *dev_id)
{
    uint8_t i;
    for (i = 0; i < CONFIG_BSXX_DEV_NUM; i++) {
        if (ctrl == &g_bsxx_controllers[i]) {
            *dev_id = i;
            return 0;
        }
    }
    tiot_print_err("[TIoT]invalid device\n");
    return -1;
}

static inline void bsxx_set_open_param(tiot_xfer_open_param *xfer_open_param, tiot_service_open_param *param)
{
    if (param == NULL) {
        return;
    }
    xfer_open_param->rx_cbk_param.buff = param->buff;
    xfer_open_param->rx_cbk_param.buff_len = param->buff_len;
    xfer_open_param->rx_cbk_param.rx_callback = param->rx_callback;
}

static void service_reset_flow_ctrl(tiot_xfer_manager *xfer, tiot_service_open_param *param)
{
    struct tiot_xmit_config config;
    uint32_t new_flow_ctrl;
    if (param == NULL) {
        return;
    }
    /* 如果入参存在，则按入参重新配置xmit. */
    new_flow_ctrl = (param->flags & (1u << BSXX_OPEN_FLAGS_FLOWCTRL_BIT)) ?
                    TIOT_UART_ATTR_FLOW_CTRL_ENABLE : TIOT_UART_ATTR_FLOW_CTRL_DISABLE;
    (void)tiot_xfer_get_config(xfer, &config);
    if (config.attr.flow_ctrl == new_flow_ctrl) {
        return;
    }
    config.attr.flow_ctrl = new_flow_ctrl;
    (void)tiot_xfer_set_config(xfer, &config);
}

static int32_t bsxx_set_initial_pm_state(tiot_pm *pm)
{
    int32_t ret = 0;
    uint32_t power_ctrl_pin = pm->pm_info[TIOT_PIN_POWER_CTRL];
    /* 主机侧如果控制上电，切换为WORK状态，不释放唤醒锁. */
    /* 主机侧如果不控制上电，仅进行通信和低功耗，切换为SLEEP状态，释放唤醒锁. */
    if (power_ctrl_pin == TIOT_PIN_NONE) {
        ret = tiot_pm_set_event(pm, TIOT_PM_EVENT_SLEEP_ACK);
        if (ret != 0) {
            tiot_print_err("host set default state sleep fail.\r\n");
        }
    } else {
        (void)tiot_pm_set_event(pm, TIOT_PM_EVENT_REPORT_WORK);
    }
    return ret;
}

static int32_t bsxx_service_open(tiot_controller *ctrl, void *param)
{
    int ret = -1;
    uint8_t dev_id;
    uint32_t dev_wkup_host_pin;
    tiot_xfer_open_param xfer_open_param = { 0 };
    tiot_service_open_param *service_param = (tiot_service_open_param *)param;
    if (bsxx_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }
    bsxx_set_open_param(&xfer_open_param, service_param);
    /* open流程主机保持唤醒. */
    tiot_pm_wake_lock();
    /* 传输启动 */
    if (tiot_xfer_open(&ctrl->transfer, &xfer_open_param) != 0) {
        /* 通信接口打开失败 */
        goto xfer_open_fail;
    }
    if (tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_POWERON) != 0) {
        /* 上电失败 */
        goto power_on_fail;
    }
    /* 加载固件 */
    ret = tiot_firmware_load(&ctrl->firmware);
    if (ret != 0) {
        tiot_print_err("[TIoT]Firmware load fail\n");
        goto fw_load_fail;
    }
    /* 加载后重新设置流控. */
    service_reset_flow_ctrl(&ctrl->transfer, service_param);
    ret = service_register_packet(ctrl, dev_id);
    if (ret != 0) {
        goto fw_load_fail;
    }
    if (bsxx_set_initial_pm_state(&ctrl->pm) != 0) {
        goto pm_state_init_fail;
    }
    /* 使能中断 */
    dev_wkup_host_pin = ctrl->pm.pm_info[TIOT_PIN_DEVICE_WAKEUP_HOST];
    tiot_pin_int_request(dev_wkup_host_pin, bsxx_device_wakeup_host_cb);
    /* open流程结束主机允许睡眠. */
    tiot_pm_wake_unlock();
    /* 打开OK */
    tiot_print_suc("[TIoT]Controller open succ.\n");
    return 0;

pm_state_init_fail:
    tiot_xfer_uninstall_packet(&ctrl->transfer);
    tiot_packet_deinit(&g_bsxx_pkt_ctx[dev_id]);
fw_load_fail:
    (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_POWEROFF);
power_on_fail:
    tiot_xfer_close(&ctrl->transfer);
xfer_open_fail:
    tiot_pm_wake_unlock();
    return ret;
}

static void bsxx_service_close(tiot_controller *ctrl)
{
    uint8_t dev_id;
    uint32_t dev_wkup_host_pin;
    if (bsxx_get_dev_id(ctrl, &dev_id) != 0) {
        return;
    }
    dev_wkup_host_pin = ctrl->pm.pm_info[TIOT_PIN_DEVICE_WAKEUP_HOST];
    /* 驱动不控制上下电时，host上层先复位，再close，此时不下发allow sleep，待优化. */
    (void)tiot_pm_set_event(&ctrl->pm, TIOT_PM_EVENT_POWEROFF);
    tiot_xfer_close(&ctrl->transfer);
    tiot_xfer_uninstall_packet(&ctrl->transfer);
    tiot_packet_deinit(&g_bsxx_pkt_ctx[dev_id]);
    tiot_pin_int_release(dev_wkup_host_pin);
}

static int32_t bsxx_service_write(tiot_controller *ctrl, const uint8_t *data,
                                  uint32_t len, const tiot_xfer_tx_param *param)
{
    uint8_t dev_id;
    int32_t ret;
    tiot_pm *pm = NULL;
    const tiot_xfer_tx_param tx_param = { SUBSYS_GNSS };
    tiot_unused(param);
    if (bsxx_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }
    pm = &ctrl->pm;
    (void)tiot_pm_set_event(pm, TIOT_PM_EVENT_WORK_VOTE_UP);
    /* 尝试唤醒device */
    ret = tiot_pm_set_event(pm, TIOT_PM_EVENT_WAKEUP);
    if (ret != 0) {
        tiot_print_err("[TIoT]wake up error\n");
        goto vote_down;
    }
    /* 等待成功唤醒device */
    ret = tiot_pm_wait_state(&ctrl->pm, TIOT_PM_STATE_WORK, ctrl->dev_info->timings.wakeup_wait_ms);
    if (ret <= 0) {
        tiot_print_warning("[TIoT]wakeup device fail, pm state is 0x%x.\n", pm->state);
    }
    ret = tiot_xfer_send(&ctrl->transfer, data, len, &tx_param);
vote_down:
    (void)tiot_pm_set_event(pm, TIOT_PM_EVENT_WORK_VOTE_DOWN);
    return ret;
}

static int32_t bsxx_service_read(tiot_controller *ctrl, uint8_t *buff, uint32_t buff_len,
                                 const tiot_xfer_rx_param *param)
{
    uint8_t dev_id;
    if (bsxx_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }
    return tiot_xfer_recv(&ctrl->transfer, buff, buff_len, param);
}

static int32_t bsxx_pm_suspend_prepare(tiot_controller *ctrl)
{
    const uint8_t sleep_check_times = 50;
    uint32_t wakein_pin = ctrl->pm.pm_info[TIOT_PIN_DEVICE_WAKEUP_HOST];

    tiot_print_info("[TIoT]suspend prepare, pm state is 0x%x\n", ctrl->pm.state);
    tiot_pm_check_sleeped(wakein_pin, sleep_check_times);
    return 0;
}

static int32_t bsxx_pm_resume_prepare(tiot_controller *ctrl)
{
    tiot_unused(ctrl);
    /* Host下发数据时再自然唤醒device */
    return 0;
}

static int32_t bsxx_pm_suspend(tiot_controller *ctrl)
{
    /* 主机侧UART下电情况下, suspend前关闭uart. */
    ctrl->transfer.xmit_suspend = TIOT_XFER_SUSPEND;
    tiot_xfer_close(&ctrl->transfer);
    return 0;
}

static int32_t bsxx_pm_resume(tiot_controller *ctrl)
{
    uint16_t suspend_state;
    /* 主机侧UART下电情况下, resume后重新打开uart，并恢复uart配置. */
    tiot_uart_config config;
    (void)tiot_xfer_open(&ctrl->transfer, NULL);
    (void)tiot_xfer_get_config(&ctrl->transfer, &config);
    (void)tiot_xfer_set_config(&ctrl->transfer, &config);
    suspend_state = ctrl->transfer.xmit_suspend;
    ctrl->transfer.xmit_suspend = TIOT_XFER_RESUME;

    /* UART恢复后执行推迟的disallow. */
    if (suspend_state & TIOT_XFER_DELAY_MSG) {
        tiot_print_info("[TIoT]send disallow sleep msg in resume.\r\n");
        if (tiot_pm_send_disallow_sleep(&ctrl->transfer) != 0) {
            /* 对应收到wakein intr持的唤醒锁 */
            tiot_pm_wake_unlock();
        }
    }
    return 0;
}

/*                   进程冻结
 * SUSPEND_PREPARE  --------> SUSPEND
 *                               |
 *                   进程恢复     v
 * RESUME_PREPARE  <--------  RESUME
 */
static int32_t bsxx_service_pm_ctrl(tiot_controller *ctrl, uint8_t cmd)
{
    uint8_t dev_id;
    if (bsxx_get_dev_id(ctrl, &dev_id) != 0) {
        return -1;
    }
    tiot_print_info("[TIoT]pm ctrl cmd: %d\n", cmd);
    switch (cmd) {
        case TIOT_PM_CTRL_SUSPEND_PREPARE:
            return bsxx_pm_suspend_prepare(ctrl);
        case TIOT_PM_CTRL_RESUME_PREPARE:
            return bsxx_pm_resume_prepare(ctrl);
        case TIOT_PM_CTRL_SUSPEND:
            return bsxx_pm_suspend(ctrl);
        case TIOT_PM_CTRL_RESUME:
            return bsxx_pm_resume(ctrl);
        default:
            tiot_print_err("[TIoT]unknown pm ctrl cmd: %d\n", cmd);
            return -1;
    }
}

int32_t bsxx_service_init(void)
{
    uint8_t i;
    int32_t ret;
    tiot_board_info per_board_info;
    tiot_controller_info ctrl_info;
    const bsxx_board_info *board_info;
    for (i = 0; i < CONFIG_BSXX_DEV_NUM; i++) {
        if (bsxx_board_init(&i) != 0) {
            tiot_print_err("[TIoT]board init fail\n");
            return -1;
        }
    }
    board_info = bsxx_board_get_info();
    if (board_info == NULL) {
        tiot_print_err("[TIoT]no board info.\n");
        goto deinit;
    }
    for (i = 0; i < CONFIG_BSXX_DEV_NUM; i++) {
        per_board_info.fw_info.cfg_path = board_info->cfg_path;
        per_board_info.hw_info.xmit_id = board_info->hw_infos[i].xmit_id;
        per_board_info.hw_info.pm_info = board_info->hw_infos[i].pm_info;
        ctrl_info.board_info = &per_board_info;
        ctrl_info.dev_info = bsxx_device_get_info();
        ret = tiot_controller_init(&g_bsxx_controllers[i], &ctrl_info);
        if (ret != 0) {
            tiot_print_err("[TIoT]controller init fail\n");
            goto deinit;
        }
    }
    for (i = 0; i < CONFIG_BSXX_DEV_NUM; i++) {
        g_bsxx_controllers[i].ops.open = bsxx_service_open;
        g_bsxx_controllers[i].ops.close = bsxx_service_close;
        g_bsxx_controllers[i].ops.write = bsxx_service_write;
        g_bsxx_controllers[i].ops.read = bsxx_service_read;
        g_bsxx_controllers[i].ops.pm_ctrl = bsxx_service_pm_ctrl;
    }
    return 0;
deinit:
    bsxx_service_deinit();
    return -1;
}

void bsxx_service_deinit(void)
{
    uint8_t i;
    for (i = 0; i < CONFIG_BSXX_DEV_NUM; i++) {
        tiot_controller_deinit(&g_bsxx_controllers[i]);
    }
    for (i = 0; i < CONFIG_BSXX_DEV_NUM; i++) {
        bsxx_board_deinit(&i);
    }
}

uintptr_t bsxx_service_get_ctrl(uint8_t dev_id)
{
    return (uintptr_t)&g_bsxx_controllers[dev_id];
}
