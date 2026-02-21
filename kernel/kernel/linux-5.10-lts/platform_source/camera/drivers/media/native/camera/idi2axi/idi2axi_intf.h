/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi device interface
 *
 * This file is released under the GPLv2.
 */
#ifndef IDI2AXI_INTF_H
#define IDI2AXI_INTF_H
#include <linux/videodev2.h>

#define I2A_MAX_CHANNELS 4u

enum idi2axi_flush_channel_ret {
	IDI2AXI_FLUSH_CHANNEL_SUCC = 0,
	IDI2AXI_FLUSH_CHANNEL_IS_NULL,
	IDI2AXI_FLUSH_CHANNEL_TIMEOUT,
};

enum idi2axi_config_type {
	IDI2AXI_CONFIG_POWER_ON = 0,
	IDI2AXI_CONFIG_POWER_OFF,
};

enum idi2axi_event_info {
	EVENT_IDI2AXI_RECOVER_CODE = 0,
	EVENT_IDI2AXI_CAMERA_OPEN,
	EVENT_IDI2AXI_CAMERA_CLOSE,
	EVENT_IDI2AXI_POWER_ON,
	EVENT_IDI2AXI_POWER_OFF,
};

enum idi2axi_api_id {
	/* request */
	COMMAND_IDI2AXI_REQUEST,
	COMMAND_IDI2AXI_ATTACH_CHANNEL,
	COMMAND_IDI2AXI_FLUSH_CHANNEL,
	COMMAND_IDI2AXI_USECASE_CONFIG,
	/* response */
	REQUEST_IDI2AXI_RESPONSE,
	/* event sent to ap */
	IDI2AXI_EVENT_SENT,

	COMMAND_IDI2AXI_MAX,
};

enum {
	IDI2AXI_NAME_SIZE = 32,
	IDI2AXI_V4L2_EVENT_TYPE = V4L2_EVENT_PRIVATE_START + 0x00099000,
	IDI2AXI_HIGH_PRIO_EVENT = 0x1500,
	IDI2AXI_MIDDLE_PRIO_EVENT = 0x2000,
	IDI2AXI_LOW_PRIO_EVENT = 0x3000,
};

typedef struct _tag_i2a_kinfo {
	char name[IDI2AXI_NAME_SIZE];
} i2a_kinfo_t;

typedef struct _idi2axi_msg_req {
	unsigned int fd;
	unsigned int frame_num;
	unsigned int port[I2A_MAX_CHANNELS];
} idi2axi_msg_req;

typedef struct _idi2axi_msg_ack_req {
	unsigned int fd;
	unsigned int frame_num;
	unsigned int timestamp_low;
	unsigned int timestamp_high;
	unsigned int addr;
	bool valid;
	unsigned int port;
} idi2axi_msg_ack_req;

typedef struct _idi2axi_event {
	unsigned int event_id;
} idi2axi_event;

typedef struct _channel_port_dt_buf_offset {
	unsigned int port;
	unsigned int dt;
	unsigned int buf_offset;
} channel_port_dt_buf_offset;

typedef struct _channel_param {
	unsigned int cnt;
	channel_port_dt_buf_offset ch_relation[I2A_MAX_CHANNELS];
} idi2axi_msg_attach_channel_req;

typedef struct _idi2axi_msg_flush {
	unsigned int cnt;
	unsigned int port[I2A_MAX_CHANNELS];
} idi2axi_msg_flush;

typedef struct _idi2axi_msg_usecase_config {
	unsigned int buf_size;
} idi2axi_msg_usecase_config;

typedef struct _tag_idi2axi_msg_t {
	unsigned int api_name;
	unsigned int message_id;
	unsigned int message_size;
	bool sync;
	union {
		idi2axi_msg_req req;
		idi2axi_msg_ack_req ack_req;
		idi2axi_event event_sent;
		idi2axi_msg_attach_channel_req attach_chanel_req;
		idi2axi_msg_flush flush_req;
		idi2axi_msg_usecase_config usecase_config_req;
	} u;
} idi2axi_msg_t;

typedef struct _tag_idi2axi_cfg_data {
	int cfgtype;
	union {
		char name[32];
	} u;
} idi2axi_cfg_data;

typedef enum _tag_idi2axi_event_kind {
	IDI2AXI_MSG_CB,
} idi2axi_event_kind_t;

typedef struct _tag_idi2axi_event {
	idi2axi_event_kind_t kind;
} idi2axi_event_t;

typedef struct _tag_idi2axi_err_info {
	uint32_t info[12];
} idi2axi_err_info;

#define IDI2AXI_IOCTL_CFG              _IOWR('I', BASE_VIDIOC_PRIVATE + 61, idi2axi_cfg_data)
#define IDI2AXI_IOCTL_GET_INFO         _IOR('I',  BASE_VIDIOC_PRIVATE + 62, i2a_kinfo_t)
#define IDI2AXI_IOCTL_SEND_MSG         _IOWR('I', BASE_VIDIOC_PRIVATE + 63, idi2axi_msg_t)
#define IDI2AXI_IOCTL_RECV_MSG         _IOWR('I', BASE_VIDIOC_PRIVATE + 64, idi2axi_msg_t)
#define IDI2AXI_IOCTL_GET_ERR_INFO     _IOR('I',  BASE_VIDIOC_PRIVATE + 65, idi2axi_err_info)
#endif /* end of include guard: IDI2AXI_H */
