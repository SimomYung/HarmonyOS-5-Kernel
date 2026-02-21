/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: cap prox detect header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __CAP_PROX_DETECT_H__
#define __CAP_PROX_DETECT_H__

#include "sensor_sysfs.h"

#define CAP_PROX_DEV_COUNT_MAX       2
#define CAP_MODEM_THRESHOLE_LEN      8
#define CAP_CALIBRATE_THRESHOLE_LEN  4
#define CAP_CHIPID_DATA_LENGTH       2
#define SAR_USE_PH_NUM               2
#define CAP_PROX_RETRY_COUNT         3
#define CAP_PROX_DETECT_SUCC         1
#define CAP_COVER_NORM_UNIT_LEN      2
#define CAP_COVER_OFFSET_RANGE_LEN   2
#define CAP_COVER_DETECT_RANGE_LEN   2
#define CAP_COVER_BASE_THREH_NUM     3
#define CAP_COVER_MIN_THREH_NUM      3
#define CAP_PROX_MAX_CHN_NUM         10

struct semteck_sar_data {
	uint16_t threshold_to_ap;
	uint16_t phone_type;
	uint16_t threshold_to_modem[CAP_MODEM_THRESHOLE_LEN]; /* default array length */
	uint32_t init_reg_val[17];
	uint8_t ph;
	uint16_t calibrate_thred[4];
	uint16_t is_report_state_data;
};

struct abov_sar_data {
	uint16_t phone_type;
	uint16_t abov_project_id;
	uint16_t threshold_to_modem[CAP_MODEM_THRESHOLE_LEN];
	uint8_t ph;
	uint16_t calibrate_thred[CAP_CALIBRATE_THRESHOLE_LEN];
};

struct awi_sar_data {
	uint16_t threshold_to_modem;
	uint16_t calibrate_thred[CAP_CALIBRATE_THRESHOLE_LEN];
	uint16_t phone_type;
	uint16_t is_report_state_data;
	uint16_t ext_calibrate_thred; // ext_ for far cali diff threshold
	uint16_t calibrate_thred1[CAP_CALIBRATE_THRESHOLE_LEN];
	uint16_t threshold_to_modem1; // threshold_to_modem1 to diff1
};

struct tyhx_sar_data {
	uint16_t threshold_to_modem;
	uint16_t calibrate_thred[4]; // 4:diff0 diff1 offset0 offset1
	uint16_t phone_type;
	uint16_t is_report_state_data;
	uint16_t ext_calibrate_thred;
};

struct gh621_sar_data {
	uint16_t threshold_to_modem;
	uint16_t calibrate_thred[4]; // 4:diff0 diff1 offset0 offset1
	uint16_t phone_type;
	uint16_t is_report_state_data;
	uint16_t ext_calibrate_thred;
	uint16_t calibrate_thred1[CAP_CALIBRATE_THRESHOLE_LEN];
	uint16_t threshold_to_modem1; // threshold_to_modem1 to diff1
};

union sar_data {
	struct semteck_sar_data semteck_data;
	struct abov_sar_data abov_data;
	struct awi_sar_data awi_data;
	struct tyhx_sar_data tyhx_data;
	struct gh621_sar_data gh621_data;
	/* add the others here */
};

struct sar_cover_attr {
	uint8_t enable; // support cover function: 0-don't support; 1-support
	uint8_t offset_mode;   // normalize fF unit mode: 1-one param; 2-two param
	uint16_t phone_type;
	uint8_t prox_ch; // small range prox channel
	uint8_t detect_ch; // big range detect channel
	uint8_t raw_cali_type; // 0-raw set zero when cali offset; 1-raw not zero when cali offset
	uint8_t rsv;
	uint32_t ratio;               // normalize unit ratio
	uint32_t unit_val[2];         // noramlize fF unit value;
	uint16_t offset_range[2];     // offset lowest value and max value;
	int32_t base_offset_thred[3]; // diff threshold for base offset
	int32_t min_offset_thred[3];  // diff threshold for history min offset
	uint16_t range[2]; // prox_ch range(fF); detect_ch range(fF)
};

/*
 * calibrate_type: config by bit(0~7): 0-free 1-near 2-far other-reserve
 * sar_datas: data for diffrent devices
 */
struct sar_platform_data {
	struct sensor_combo_cfg cfg;
	GPIO_NUM_TYPE gpio_int;
	GPIO_NUM_TYPE gpio_int_sh;
	uint16_t poll_interval;
	uint16_t calibrate_type;
	struct sar_cover_attr cover_attr;
	union sar_data sar_datas;
};

struct sar_sensor_detect {
	char *chip_name;
	struct sensor_combo_cfg cfg;
	int chip_id_value_bit;
	uint8_t detect_flag;
	uint16_t chip_id;
	uint32_t chip_id_value[CAP_CHIPID_DATA_LENGTH];
};

struct sar_calibrate_data {
	int32_t offset[SAR_USE_PH_NUM];
	int32_t diff[SAR_USE_PH_NUM];
	uint8_t data[MAX_CAP_PROX_CALIBRATE_DATA_LENGTH - SAR_USE_PH_NUM * sizeof(int32_t) * 2];
};

enum {
	CAP_PROX_PHASE0,
	CAP_PROX_PHASE1,
};

struct cap_prox_device_info {
	int32_t obj_tag;
	uint32_t detect_list_id;
	uint8_t cap_prox_dev_index;
	uint8_t cap_prox_first_start_flag;
	struct sar_calibrate_data sar_calibrate_datas;
};

struct sar_platform_data *cap_prox_get_platform_data(int32_t tag);
struct cap_prox_device_info *cap_prox_get_device_info(int32_t tag);
char *cap_prox_get_sensors_id_string(void);
void cap_prox_get_sensors_id_from_dts(struct device_node *dn);
char *cap_prox_get_calibrate_order_string(void);
void read_abov_sar_data_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm);
void read_capprox_data_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm);
int cap_prox_sensor_detect(struct device_node *dn,
	struct sensor_detect_manager *sm, int index);
void read_cap_prox1_info(struct device_node *dn);
void read_cap_prox_info(struct device_node *dn);
void cap_prox_detect_init(struct sensor_detect_manager *sm, uint32_t len);
unsigned int get_sar_detect_result(void);
unsigned int get_sar_aux_detect_result(void);
int cap_prox_data_from_mcu(const struct pkt_header *head);

#endif
