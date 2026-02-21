/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: contexthub route source file
 * Author: DIVS_SENSORHUB
 * Create: 2012-05-29
 */
#ifndef __LINUX_INPUTHUB_ROUTE_PLU_H__
#define __LINUX_INPUTHUB_ROUTE_PLU_H__

#define DROP_NV_NUM 440
#define DROP_NV_SIZE 100
#define DROP_FASTBOOT_RECORD 30
#define DROP_FASTBOOT_NUM 8
#define SENSOR_DROP_IMONITOR_ID 936005000
#define SENSOR_DROP_HIGH 30

#ifdef CONFIG_HUAWEI_DSM
#define SENSOR_RAW_DATA_ACC1_IMONITOR_ID  936005014
#define SENSOR_RAW_DATA_ACC2_IMONITOR_ID  936005015
#define SENSOR_RAW_DATA_ACC3_IMONITOR_ID  936005016
#define SENSOR_RAW_DATA_GYRO1_IMONITOR_ID 936005017
#define SENSOR_RAW_DATA_GYRO2_IMONITOR_ID 936005018
#define SENSOR_RAW_DATA_GYRO3_IMONITOR_ID 936005019

#define RAW_DATA_EVENTS_NUM               6
#define RAW_DATA_EVENT_KEY_NUM            100
#define RAW_DATA_PARAM_MAX_SIZE           20
#define RAW_DATA_EVENT_TABLE_SIZE         (RAW_DATA_EVENT_KEY_NUM * 8) // RAW_DATA_EVENT_KEY_NUM * sizeof(uint64_t)
#endif

#pragma pack(4)
struct pkt_drop_info_record_t {
	long time;
	int height;
};

struct pkt_drop_fastboot_record_t {
	struct pkt_drop_info_record_t drop_info[DROP_FASTBOOT_NUM];
	int index;
};
#pragma pack()

void schedule_drop_work(drop_info_t drop_data);
#if (defined CONFIG_HUAWEI_DSM) || (defined CONFIG_HUAWEI_OHOS_DSM)
void drop_fastboot_record(const pkt_drop_data_req_t *head);
#endif

#ifdef CONFIG_HUAWEI_DSM
int process_drop_report(const pkt_drop_data_req_t *head);
int sensor_raw_big_data_report(const struct pkt_header *head);
#elif (defined CONFIG_HUAWEI_OHOS_DSM)
int process_drop_report_ohos(const pkt_drop_data_req_t *head);
#endif

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
void dmd_modem_record(pkt_mode_dmd_log_report_req_t* pkt);
#endif

#endif /* __LINUX_INPUTHUB_ROUTE_PLU_H__ */

