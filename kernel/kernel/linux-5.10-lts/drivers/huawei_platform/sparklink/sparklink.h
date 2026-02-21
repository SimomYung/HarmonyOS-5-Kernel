/*
 * sparklink.h
 *
 * interface for huawei sparklink driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef _SPARKLINK_H_
#define _SPARKLINK_H_
#include <linux/kernel.h>
#include <linux/types.h>

/* log level */
#define SPARKLINK_LOG_FLOW 2
#define SPARKLINK_LOG_INFO 1
#define SPARKLINK_LOG_WARN 0
#define SPARKLINK_LOG_ERR 0


int sparklink_log_level(void);
#define sparklink_flowmsg(fmt, args...) \
do { \
	if (sparklink_log_level() >= SPARKLINK_LOG_FLOW) \
		printk(KERN_INFO "sparklink:%s:" fmt, __FUNCTION__, ##args); \
} while (0)

#define sparklink_infomsg(fmt, args...) \
do { \
	if (sparklink_log_level() >= SPARKLINK_LOG_INFO) \
		printk(KERN_INFO "sparklink:%s:" fmt, __FUNCTION__, ##args); \
} while (0)

#define sparklink_warnmsg(fmt, args...) \
do { \
	if (sparklink_log_level() >= SPARKLINK_LOG_WARN) \
		printk(KERN_INFO "sparklink:%s:" fmt, __FUNCTION__, ##args); \
} while (0)

#define sparklink_errmsg(fmt, args...) \
do { \
	if (sparklink_log_level() >= SPARKLINK_LOG_ERR) \
		printk(KERN_ERR "sparklink:%s:" fmt, __FUNCTION__, ##args); \
} while (0)

#define SPARKLINK_UART_BUFF_LEN 100
#define SPARKLINK_UART_RECEIVE_TIME 20
#define SPARKLINK_UART_RECEIVE_INTERVAL 30

typedef int (*uart_read_callback)(const unsigned char *buf, int len);
// add for sparklink uart interface
struct sparklink_uart_ops {
	int (*uart_init)(const char *uart_port, long baud);
	void (*uart_clear)(void);
	void (*reconfig_baudrate)(long baud);
	int (*uart_read)(u8 *buf, size_t *len, int time_out, int response_len);
	int (*uart_write)(u8 *buf, size_t len);
	int (*uart_deinit)(void);
	int (*uart_read_start)(uart_read_callback func);
	void (*uart_read_end)(void);
};

int sparklink_uart_ops_register(struct sparklink_uart_ops *ops);
int sparklink_uart_ops_unregister(void);

// add for sparklink device interface
#define SPARKLINK_I2C_READ 0
#define SPARKLINK_I2C_WRITE 1

#define SPARKLINK_STATE_UPDATE_FAIL (-2)
#define SPARKLINK_STATE_UPDATING (-1)
#define SPARKLINK_STATE_POWERON 0
#define SPARKLINK_STATE_BADFLASH 1
#define SPARKLINK_STATE_REPAIR_OK 2
#define SPARKLINK_STATE_POWEROFF 3
#define SPARKLINK_STATE_RESET 4
#define SPARKLINK_STATE_UPDATE 5 /* reserve */
#define SPARKLINK_STATE_MAX 6

/* reg of 1162 */
#define LCD_ON_OFF_ADDR 0x13
#define LCD_STATE_OFF 0x2
#define LCD_STATE_ON 0x1

#define DEBUGLOG_ADDR 0x20
#define DEBUGLOG_MAX_LEN 0x80
#define DEBUGLOG_READ 0x1

#define NV_ADDR 0x05
#define NV_LEN 0x2
#define NV_MAGIC 0x254d

#define SF_RESET_ADDR 0x16
#define SF_RESET 0x1

/* lock irq and close bt core */
#define DISABLE_IQR_BT_ADDR 0x1A
#define DISABLE_IQR_BT 0x1
#define DISABLE_IQR_BT_DELAY_TIME 50

#define HW_RESET_DELAY_TIME 10
#define SHUTDOWN_DELAY_TIME 5
#define WAKEUP_DELAY_TIME 5
#define WAKEUP_GPIO_REVERSE_DELAY_TIME 10

#define SF_RESET_SUPPORT_ADDR 0x7
#define SF_RESET_SUPPORT 0x1
#define SF_RESET_NO_SUPPORT 0x0

#define UPDATE_IN_AIR_STATE_REG_ADDR 0x8
#define UPDATE_IN_AIR_OPEN 0x55
#define UPDATE_IN_AIR_CLOSE 0x0

#define SPARKLINK_CHIPID_SUCCESS 0x1
#define SPARKLINK_CHIPID_FAIL 0x0

#define TIME_ADDR 0x12
#define TIME_LEN 0x4

#define RF_POWER_ADDR 0x17
#define RF_POWER_LEN 2
#define I2C_CHECK_RETRY 15
#define I2C_DELAY_TIME_1S 1000
#define I2C_DELAY_TIME_5S 5000
 
#define UART_CHANNEL_REG_ADDR 0x1e
#define SPARKLINK_UART_L0 0
#define SPARKLINK_UART_H0 1
#define SPARKLINK_UART_CHANNEL_MAX 2
#define SPARKLINK_UART_MAX_LEN 2

#define UPDATE_IN_AIR_PARAM_MAX 256
#define UPDATE_IN_AIR_PARAM_MAX_LEN 4
#define UPDATE_IN_AIR_TIME_DEFAULT  1

#define SHIFTING_RATIO 1000
#define MINUTES_PER_HOUR 60

#define SPARKLINK_POWER_STATE_ADDR  0x1F
#define SPARKLINK_POWER_ON 0x80
#define SPARKLINK_POWER_OFF 0x85

#define SPARKLINK_POWER_ONOFF_MAX 2
#define SPARKLINK_POWER_ONOFF_MAX_LEN 2
#define SPARKLINK_NOTIFY_POWER_ON 1
#define SPARKLINK_NOTIFY_POWER_OFF 0

/* keyboard use 27 */
#define SPARKLINK_UART 28

#define ATTACH_KEYBOARD_ADDR 0x19
#define ATTACH_PEN_ADDR 0x18
#define PEN_KB_ATTACH_EVENT 0x1
#define PEN_KB_AWAY_EVENT 0x0

#define BYTE_LEN 0x8

// add for sparklink driver interface
struct sparklink_driver_ops {
	int (*device_powermode)(int powermode);
	int (*device_get_state)(void);
	int (*device_version)(void);
	int (*i2c_log)(int addr, int rw_flage, char *buf, size_t len);
	int (*device_uart_switch_channel)(int uart_channelmode);
	int (*device_ringing_ack)(u32 ringing_ack_value);
	int (*ota_fwupdate)(void);
	int (*device_update_in_air_set_mode)(int state);
	int (*device_get_update_in_air_state)(u32 *val);
	int (*device_record_power_state)(int power_onoff_state);
};

int sparklink_driver_ops_register(struct sparklink_driver_ops *ops);
int sparklink_driver_ops_unregister(void);
struct sparklink_uart_ops *sparklink_get_uart_ops(void);
void hi116x_i2c_operate_start(void);
void hi116x_i2c_operate_end(void);
u32 hi116x_get_uart_desc(void);
#endif /* _SPARKLINK_H_ */
