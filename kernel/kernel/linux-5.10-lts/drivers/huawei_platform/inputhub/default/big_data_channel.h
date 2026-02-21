/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: big data channel header file
 * Author: DIVS_SENSORHUB
 * Create: 2012-05-29
 */

#ifndef _BIG_DATA_CHANNEL_H_
#define _BIG_DATA_CHANNEL_H_

#include <linux/types.h>
#include <linux/workqueue.h>

#define USR_NUM 5
#define PC_NUM 3

typedef struct {
	unsigned int pc_index;
	unsigned short pc_num;
	unsigned short tag;
} pc_group;

typedef struct {
	pc_group pc1[PC_NUM];
	unsigned short total_count;
	unsigned short usr_name;
} usr_pack;

typedef struct {
	usr_pack ddr_usr[USR_NUM];
	uint32_t ddr_request_time;
} ddr_request_pack;

typedef enum {
	INT_PARAM,
	STR_PARAM,
} big_data_param_type_t;

typedef enum {
	BIG_DATA_STR,
} big_data_str_tag_t;

typedef struct {
	const char *param_name;
	big_data_param_type_t param_type;
	int tag;
} big_data_param_detail_t;

typedef struct {
	int event_id;
	int param_num;
	big_data_param_detail_t *param_data;
} big_data_event_detail_t;

typedef enum {
	VEHICLE,
	RIDING,
	WALK_SLOW,
	RUN_FAST,
	STATIONARY,
} big_data_ar_state_t;

typedef struct detect_result {
	unsigned int crash_signal; // 0 typedef
	unsigned int phone_postion; // 1
	unsigned int confidence; // 2:crash_confidence
	unsigned int fft[3]; // 3-5:index_size
	unsigned int collosion_lastime[10]; // 10:index_size 6-15
	unsigned int collision_delta_speed[10]; // 10: index_size 16-25
	unsigned int collision_average_acc[10]; // 10: index_size 26-35
	unsigned int gravity_lostime; // 36
	unsigned int brake_absoult_speed[12]; // 12: index_size 37-48
	unsigned int gps_initial_speed; // 49
	unsigned int gps_end_speed; // 50
	unsigned int baro_max_value; // 51
	unsigned int baro_min_value; // 52
	unsigned int ar_status; // 53
	unsigned int ar_confidence; // 54
	unsigned int static_time_after_collision; // 55
	unsigned int audio_event; // 56
	unsigned int audio_confidence; // 57
} detect_result;

typedef struct
{
	int type;
	unsigned int timestamp;
	unsigned int day;
	unsigned int offset_base;
	unsigned int offset_cur;
	unsigned int offset_day;
	unsigned int offset_week;
	unsigned int offset_month;
	unsigned int offset_quarter;
	unsigned int offset_year;
	unsigned int offset_history;
	unsigned int status;
	unsigned int chn_datas_0;
	unsigned int chn_datas_1;
	unsigned int chn_datas_2;
	unsigned int chn_datas_3;
	unsigned int chn_datas_4;
	unsigned int chn_datas_5;
	unsigned int chn_datas_6;
	unsigned int chn_datas_7;
} cover_offset_big_data_t;

typedef enum {
	POSTURE_OPEN_STREAM = 0,
	POSTURE_CLOSE_STREAM,
	OPEN_POSTURE_SCREEN_ON,
	OPEN_POSTURE_SCREEN_OFF,
	OPEN_POSTURE_SCREEN_ON_1HZ,
	OPEN_POSTURE_SCREEN_OFF_1HZ,
	CLOSE_POSTURE_SCREEN_ON,
	CLOSE_POSTURE_SCREEN_OFF,
} posture_state_t;

typedef struct {
	int64_t happen_time;
	posture_state_t posture_state;
	int32_t duration;
} dubai_posture_t;

int iomcu_dubai_log_fetch(uint32_t event_type, void *data, uint32_t length);
detect_result get_crash_result(void);
void ddr_req_statis_que_work(void);
void set_resume_time(struct timeval *ddr_request_tv_end);
void set_suspend_time(struct timeval *ddr_request_tv_begin);

#endif /* _BIG_DATA_CHANNEL_H_ */
