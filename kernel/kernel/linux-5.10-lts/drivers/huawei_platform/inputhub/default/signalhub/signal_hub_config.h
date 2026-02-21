

#ifndef __SIGNAL_HUB_CONFIG_H__
#define __SIGNAL_HUB_CONFIG_H__
#include "contexthub_route.h"

#define MAX_TUNER_NUM 2 /* 支持最大的tuner数 */
#define MAX_TUNER_INST_NUM 52 /* 单个tuner支持的最大mipi指令数 */
#define MAX_TUNER_STATE_NUM 10 /* 单个tuner支持的最大可配置状态数 */
#define MAX_INST_NUM_PER_TUNER_STATE 10 /* 每个可配置态支持的最大mipi指令数 */
#define MAX_TUNER_ANT_NUM 3 /* 单个tuner支持的最大耦合天线数 */
#define MAX_TUNER_SUBSYSTEM_NUM 3 /* 单个tuner支持的最大互助子系统数 */
#define MAX_MASK_TABLE_NUM 32 /* 二级仲裁表的最大mask个数 */

typedef struct {
	uint16_t mipi_pin_id;
	uint16_t mipi_div_freq;
} signalhub_mipi_driver_cfg;

typedef enum {
	SIGNALHUB_TUNER_STATE_A0 = 0, /* 低功耗态 */
	SIGNALHUB_TUNER_STATE_A1, /* 退出低功耗态 */
	SIGNALHUB_TUNER_STATE_A123, /* 系统1&2&3平衡态 */
	SIGNALHUB_TUNER_STATE_A12, /* 系统1&2平衡态 */
	SIGNALHUB_TUNER_STATE_A13, /* 系统1&3平衡态 */
	SIGNALHUB_TUNER_STATE_A23, /* 系统2&3平衡态 */
	SIGNALHUB_TUNER_STATE_A11, /* 系统1最优 */
	SIGNALHUB_TUNER_STATE_A22, /* 系统2最优 */
	SIGNALHUB_TUNER_STATE_A33, /* 系统3最优 */
	SIGNALHUB_TUNER_STATE_BUTT, /* 单个tuner每种场景下的最大可配置状态数 */
} signalhub_tuner_state;

typedef enum {
	SIGNALHUB_WIFI_2P4G_ANTO = 0,
	SIGNALHUB_WIFI_2P4G_ANT1,
	SIGNALHUB_WIFI_2P4G_ANT2,
	SIGNALHUB_WIFI_5G_ANT0,
	SIGNALHUB_WIFI_5G_ANT1,
	SIGNALHUB_BTSLE_2P4G_ANT0,
	SIGNALHUB_BTSLE_2P4G_ANT1,
	SIGNALHUB_BTSLE_2P4G_ANT2,
	SIGNALHUB_BTSLE_5G_ANT0,
	SIGNALHUB_BTSLE_5G_ANT1,
	SIGNALHUB_LBS_L1_ANT,
	SIGNALHUB_LBS_L5_ANT,
	SIGNALHUB_LOGIC_ANT_BUTT,
} signalhub_nc_logic_ant;

typedef enum {
	SIGNALHUB_NC_2P4G = 0,
	SIGNALHUB_NC_5G,
	SIGNALHUB_LBS_L1,
	SIGNALHUB_LBS_L5,
	SIGNALHUB_NC_5G_2P4G,
	SIGNALHUB_NC_2P4G_L1,
	SIGNALHUB_CELL,
	SIGNALHUB_SUBSYSTEM_BUTT,
} signalhub_subsystem;

typedef enum {
	SIGNALHUB_SCENE_HALL0 = 0, /* hall 折叠态 */
	SIGNALHUB_SCENE_HALL1, /* hall 展开态 */
	SIGNALHUB_SCENE_BUTT, /* 单个tuner最大场景数 */
} signalhub_scene;

typedef struct {
	uint32_t byte_cnt : 5;
	uint32_t mipi_port_sel : 3;
	uint32_t slave_id : 4;
	uint32_t reg_addr :16;
	uint32_t mipi_ctrl :4;
} signalhub_mipi_cmd;

typedef struct {
	uint32_t byte0 : 8;
	uint32_t byte1 : 8;
	uint32_t byte2 : 8;
	uint32_t byte3 : 8;
} signalhub_mipi_data;

typedef union {
	uint16_t value;
	struct mask{
		uint16_t system1_mask : 1;
		uint16_t system2_mask : 1;
		uint16_t system3_mask : 1;
		uint16_t system1_status : 1;
		uint16_t system2_status : 1;
		uint16_t system3_status : 1;
		uint16_t rsv : 10;
	} u;
} signalhub_tuner_mask;

typedef struct {
	signalhub_tuner_mask mask_info; /* mask信息 */
	uint16_t tuner_cfg_index;
} signalhub_tuner_mask_table;

typedef struct {
	signalhub_scene scene; /* 整机场景信息 */
	uint16_t tuner_state_cfg_index[SIGNALHUB_TUNER_STATE_BUTT]; /* 不同天线态的配置索引 */
	uint16_t masktable_num;
	signalhub_tuner_mask_table masktable[MAX_MASK_TABLE_NUM]; /* 掩码表 */
} signalhub_scene_tuner_cfg;

typedef struct {
	signalhub_mipi_cmd cmd;
	signalhub_mipi_data data;
} signalhub_mipi_unit;

typedef struct {
	uint8_t mipi_cnt;
	uint8_t rsv;
	uint8_t mipi_index_list[MAX_INST_NUM_PER_TUNER_STATE];
} signalhub_tuner_state_cfg;

typedef struct {
	uint8_t enable; /* 使能开关 */
	uint8_t tuner_index; /* tuner天线Index */
	uint8_t hw_ant_num; /* 耦合的物理天线个数 */
	uint8_t hw_ant_index[MAX_TUNER_ANT_NUM]; /* 耦合的物理天线号 */
	uint8_t system_num; /* 互助子系统个数 */
	signalhub_subsystem system[MAX_TUNER_SUBSYSTEM_NUM]; /* 互助子系统*/
	uint8_t scene_num; /* 场景个数 */
	signalhub_scene_tuner_cfg scene_tuner_cfg[SIGNALHUB_SCENE_BUTT]; /* 每个场景的tuner配置 */
} signalhub_tuner_config;

typedef struct {
	signalhub_mipi_unit mipi_res[MAX_TUNER_NUM][MAX_TUNER_INST_NUM];
	signalhub_tuner_state_cfg tuner_cfg_res[MAX_TUNER_NUM][MAX_TUNER_STATE_NUM];
	signalhub_tuner_config tuner_cfg[MAX_TUNER_NUM];
	uint8_t ant_map[SIGNALHUB_LOGIC_ANT_BUTT];
	signalhub_mipi_driver_cfg mipi_driver_cfg;
} signalhub_platform_data;

typedef struct {
	uint16_t subcmd;
	uint16_t size;
	signalhub_platform_data data;
} signalhub_dts_cmd_data_header;

int init_config_from_dts(void);
#endif