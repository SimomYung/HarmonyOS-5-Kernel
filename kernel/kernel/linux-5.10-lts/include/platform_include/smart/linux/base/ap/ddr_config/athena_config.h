/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: ATHENA CONFIG
* Create: 2024-12-13
*/

#ifndef __ATHENA_CONFIG_H__
#define __ATHENA_CONFIG_H__

#define ATHENA_EVENT_MAX_DEPENDS    2

#define ATHENA_EVENT_STATE_ON       1
#define ATHENA_EVENT_STATE_OFF      0
#define ATHENA_EVENT_STATE_BUTT     2

#define ATHENA_LIMIT_MAX_INST       (128)

typedef enum {
        ATHENA_EVENT_STILL = 0,
        ATHENA_EVENT_MAX_POWER,
        ATHENA_EVENT_PERI_VOTE,
        ATHENA_EVENT_DEEP_IDLE,
        ATHENA_EVENT_AP_SLEEP,
        ATHENA_EVENT_SCREEN_ON,
        ATHENA_EVENT_SYSCACHE_REQ,
        ATHENA_EVENT_PLL,
        ATHENA_EVENT_FLL,
        ATHENA_CPU_BUSY,
        ATHENA_EVENT_BUTT,
} athena_event_e;

typedef enum {
        ATHENA_LIMIT_HEAP = 0,
        ATHENA_LIMIT_STACK,
        ATHENA_LIMIT_BUTT,
} athena_limit_e;

typedef struct {
	unsigned int depend;              // depend evt, refs to "athena_event_e"
	unsigned int current_state;       // 0 : OFF, 1 : ON
	unsigned long long current_start; // current state start time.
	unsigned long long duration[ATHENA_EVENT_STATE_BUTT][ATHENA_EVENT_MAX_DEPENDS];	// duration for event ON/OFF (depend evt ON/OFF). unit is ms
} athena_event_t;

typedef struct {
	unsigned int inst_id;
	unsigned int lmt_val;
} athena_limit_t;

typedef struct {
	athena_event_t evt[ATHENA_EVENT_BUTT];
	athena_limit_t lmt[ATHENA_LIMIT_MAX_INST];
        unsigned long long         app_sts[4];
} athena_stastics_t;

#endif
