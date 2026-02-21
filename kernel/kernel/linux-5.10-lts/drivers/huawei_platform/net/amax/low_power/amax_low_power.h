/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: This module is to support amax low power management.
 * Create: 2024-12
 */

#ifndef AMAX_LOW_POWER_H_
#define AMAX_LOW_POWER_H_

#include "dps_data_txrx.h"

#define AMAX_LOW_POWER_SLEEP 1
#define AMAX_LOW_POWER_WAKE_UP 0

#define MAX_PRIV_CMD_SIZE 4096
#define AMAX_LOW_POWER_PERIOD 400 /* 4s */

typedef struct {
    uint8_t tx_assist_pkt_flag;
    uint8_t rx_assist_pkt_flag;
    uint8_t sleep_state;
    uint8_t router_wake_up_state;
    bool is_router_support_low_power;
} amax_sleep_control_info;


bool is_router_support_low_power(void);

void set_router_support_low_power(bool set_val);
void set_tx_assit_pkt_flag(uint8_t set_val);
void set_rx_assit_pkt_flag(uint8_t set_val);

void amax_send_low_power_frame(uint8_t is_sleep, dps_ops_type type);
void process_low_power_req(uint8_t *buf, int32_t len);
void process_low_power_rsp(uint8_t *buf, int32_t len);

void report_low_power_to_fwk(uint8_t is_sleep);
void amax_low_power_assist_link_pkt_process(void);
void amax_low_power_process(void);
void reset_low_power_control_info(void);
void low_power_set_proc(void);

#endif
