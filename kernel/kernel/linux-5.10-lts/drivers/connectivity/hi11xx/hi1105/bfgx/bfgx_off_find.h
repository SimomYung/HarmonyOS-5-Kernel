/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: bfgx_platform_msg_handle.c header file
 * Author: @CompanyNameTag
 */

#ifndef BFGX_OFF_FIND_H
#define BFGX_OFF_FIND_H

/* 其他头文件包含 */
#include <linux/miscdevice.h>
#include "hw_bfg_ps.h"
#include "bfgx_core.h"

struct off_find_para {
    uint32_t cmd;        //
    uint32_t data_len;   // 可扩展的数据长度
    uint32_t data[1];     // 可扩展的数据地址
};

enum callback_cmd {
    OFF_FIND_ENABLE = 0x1,
    OFF_FIND_DISABLE = 0x2,
    OFF_FIND_AVOID_LEAKAGE = 0x3,
    OFF_FIND_BATTERY_RSV = 0x4,

    OFF_FIND_CMD_BUTT
};

/* 函数声明 */
void off_find_init(struct platform_device *pdev);
int32_t off_find_get_status(void);
int32_t off_find_cmd_parse(struct bfgx_dev_node *dev_node, uint8_t OAL_USER *data);
struct bfgx_subsys_driver_desc *get_off_find_default_drv_desc(void);
void off_find_thread_completion_handle(uint8_t msg);
#endif /* BFGX_OFF_FIND_H */
