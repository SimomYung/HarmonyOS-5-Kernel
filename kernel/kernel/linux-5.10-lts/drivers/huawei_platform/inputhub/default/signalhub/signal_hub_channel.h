

#ifndef __SIGNAL_HUB_CHANNEL_H__
#define __SIGNAL_HUB_CHANNEL_H__
#include "contexthub_route.h"

#include <linux/ioctl.h>

#ifndef SUCCESS
#define SUCCESS (0)
#endif
#ifndef FAIL
#define FAIL (-1)
#endif
#define TAG_SIGNALHUB 188
/* ioctl cmd define */
#define SIGHUB_IO 0xB1
#define SIGHUB_STATUS_MSG_MAX_LEN 32
#define SIGHUB_CONFIG_MSG_MAX_LEN 256
#define SIGHUB_MAX_MCU_DATA_SIZE 1024
#define MAX_MIPI_REPORT_DATA_LEN 2

// mipi读取等待时间, 单位ms
#define MIPI_READ_TIMEOUT 1000

enum sighub_subsystem_status_type {
	SIGNAL_HUB_CELLUAR_STATUS = 0,
	SIGNAL_HUB_WIFI_2P4G_STATUS,
	SIGNAL_HUB_WIFI_5G_STATUS,
	SIGNAL_HUB_BT_SLE_2P4G_STATUS,
	SIGNAL_HUB_BT_SLE_5G_STATUS,
	SIGNAL_HUB_LBS_STATUS,
};

enum sighub_config_cmd_type {
	SIGNAL_HUB_SET_FWK_CONFIG = 0,
	SIGNAL_HUB_SET_DTS_CONFIG,
};

enum sighub_cmd_type {
	SIGNAL_HUB_STATUS_CHANGE_CMD = 0,
	SIGNAL_HUB_CONFIG_CHANGE_CMD,
	SIGNAL_HUB_FACTORY_CMD,
	SIGNAL_HUB_DFX_CMD,
	SIGNAL_HUB_FACTORY_INIT_CMD,
	SIGNAL_HUB_CMD_END = 0xF,
};

enum signalhub_ap_report_cmd {
	SIGNALHUB_AP_REPORT_MIPI_READ_CMD = 0x0,
	SIGNALHUB_AP_REPORT_CMD_BUTT,
};

typedef struct {
	uint16_t cmd;
	uint16_t size;
	uint8_t data[SIGHUB_STATUS_MSG_MAX_LEN];
} sighub_status_msg;

typedef struct {
	uint16_t cmd;
	uint16_t size;
	uint8_t data[SIGHUB_CONFIG_MSG_MAX_LEN];
} sighub_config_msg;

typedef struct {
	uint16_t cmd;
	uint16_t size;
	uint8_t data[0];
} sighub_send_msg_data;

typedef struct {
	struct pkt_header hd;
	uint32_t value[0];
} pkt_signalhub_report_data;

typedef struct {
	uint32_t cmd;
	uint32_t rsv;
	uint32_t report_data[MAX_MIPI_REPORT_DATA_LEN];
} signalhub_factory_report_data;

//mipi读取结果返回值格式,三种格式，根据49:53位判断
typedef struct {
	uint64_t rsv : 37;
	uint64_t data0 : 8;
	uint64_t cmd_head : 19;
} reg_read;

typedef struct {
	uint64_t p1 : 1;
	uint64_t data3 : 8;
	uint64_t p2 : 1;
	uint64_t data2 : 8;
	uint64_t p3 : 1;
	uint64_t data1 : 8;
	uint64_t p4 : 1;
	uint64_t data0 : 8;
	uint64_t cmd_head : 28;
} extend_reg_read;

typedef struct {
	uint64_t p1 : 1;
	uint64_t data2 : 8;
	uint64_t p2 : 1;
	uint64_t data1 : 8;
	uint64_t p3 : 1;
	uint64_t data0 : 8;
	uint64_t cmd_head : 37;
} extend_reg_read_long;

/* 对应mipi init 参数 */
typedef struct {
    uint16_t mipiPinId;
    uint16_t mipiDivFreq;
} mipi_init_para;

/* 对应mipi资源池中的的控制字和数据字 */
typedef struct {
	unsigned int bitRwFlag : 1;     /* 读写标志, 0:读, 1:写 */
	unsigned int bitByteCnt : 5;    /* btye cnt, 从1开始有效，最大4 */
	unsigned int bitMipiPort : 3;   /* mipi port选择, 从0开始有效 0~7*/
	unsigned int bitSlaveId : 4;    /* slave Id, 0-15 */
	unsigned int bitRegAddr : 16;   /* 寄存器地址, 0~65535 */
	unsigned int bitRateCtrl : 1;   /* 速率控制, 0:半速, 1: 全速 */
	unsigned int bitMipiCtrl : 1;   /* 0: normal; 1: masked write */
	unsigned int rsv : 1;
} Context_hub_mipi_cmd;

typedef struct {
    unsigned int bitByte0 : 8;
    unsigned int bitByte1 : 8;
    unsigned int bitByte2 : 8;
    unsigned int bitByte3 : 8;
} Context_hub_mipi_data;

typedef struct {
    Context_hub_mipi_cmd cmd;
    Context_hub_mipi_data data;
    uint32_t rsv;
} mipi_drv_unit;

#define SIGHUB_IOCTL_STATUS_CHANGE_CMD  _IOW(SIGHUB_IO, 0, int)
#define SIGHUB_IOCTL_CONFIG_CHANGE_CMD  _IOWR(SIGHUB_IO, 1, int)
#define SIGHUB_IOCTL_OPEN_CMD _IO(SIGHUB_IO, 2)
#define SIGHUB_IOCTL_CLOSE_CMD _IO(SIGHUB_IO, 3)
#define SGHB_IOCTL_SGHB_MIPI_RW_CMD  _IOWR(SIGHUB_IO, 4, mipi_drv_unit)
#define SGHB_IOCTL_SGHB_MIPI_INIT_CMD  _IOWR(SIGHUB_IO, 5, mipi_init_para)

int signal_hub_send_to_sensorhub(int cmd, const void *buf, uint32_t length);
#endif
