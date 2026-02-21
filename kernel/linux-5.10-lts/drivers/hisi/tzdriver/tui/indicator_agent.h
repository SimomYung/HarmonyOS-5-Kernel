/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: indicator agent for indicator light display and interact
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef INDICATOR_AGENT_H
#define INDICATOR_AGENT_H

#define INDICATOR_CLOSE 0
#define INDICATOR_SCENE 2
#define INDICATOR_ABNORMAL 255

enum indicator_agent_cmd_t {
	INDICATOR_LAYER_ADD = 0,
	INDICATOR_LAYER_DEL = 1,
	INDICATOR_LAYER_INVALID,
};

enum indicator_state {
	INDICATOR_STATE_UNUSED = 0,
	INDICATOR_STATE_RUNNING = 1,
	INDICATOR_STATE_ERROR,
};

enum indicator_agent_event {
	INDICATOR_AGENT_EVENT_INVALID = 0,
	INDICATOR_AGENT_EVENT_DSS_POWERON = 1,
	INDICATOR_AGENT_EVENT_DSS_POWEROFF = 2,
	INDICATOR_AGENT_EVENT_DSS_POWERON_ABNORMAL = 3,
	INDICATOR_AGENT_EVENT_DSS_POWEROFF_ABNORMAL = 4,
	INDICATOR_AGENT_EVENT_TUI_ENTRY = 5,
	INDICATOR_AGENT_EVENT_TUI_EXIT = 6,
	INDICATOR_AGENT_EVENT_MAX
};

#define TEE_PRIVACY_PROTECT_ENHANCE                        \
    {                                                      \
        0x53933180, 0xc8b0, 0x4a83,                        \
        {                                                  \
            0x88, 0xd7, 0x8c, 0x76, 0xd2, 0x11, 0x7d, 0x19 \
        }                                                  \
    }


/* callback function */
typedef int (*notify_dpu) (void *pdata, int secure);

struct indicator_panel_info {
	uint32_t panel_type;
	uint32_t xres;
	uint32_t yres;
};

struct indicator_layer_rect {
	int32_t x;
	int32_t y;
    uint32_t w;
    uint32_t h;
};

struct indicator_status {
	uint32_t sensor_type;
	uint32_t color;
	uint32_t x;
	uint32_t y;
	uint32_t size;
	uint32_t animation;
};

struct indicator_ctrl {
	enum indicator_agent_cmd_t cmd;  /* cmd to agent */
	int32_t ret;                     /* ret from REE to TEE */
	struct indicator_status status;  /* Center and diameter */
	uint32_t panel_type;
	int32_t magic;                   /* default is agent id */
};

#ifdef CONFIG_TEE_INDICATOR
int register_indicator_agent(void);
int register_secure_display_driver(notify_dpu fun, void *pdata);
void unregister_secure_display_driver(void);
int send_secure_display_msg_config(int type);

void secure_display_poweron_work_start(bool is_normal);
void secure_display_poweroff_work_start(bool is_normal);
void secure_display_tui_work_start(bool is_tui_entry);

#else
static inline int register_indicator_agent(void)
{
	return 0;
}
static inline int register_secure_display_driver(notify_dpu fun, void *pdata)
{
	(void)fun;
	(void)pdata;
	return 0;
}
static inline void unregister_secure_display_driver(void)
{
}
static inline int send_secure_display_msg_config(int type)
{
	(void)type;
	return 0;
}
static inline void secure_display_poweron_work_start(bool is_normal)
{
	(void)is_normal;
	return;
}
static inline void secure_display_poweroff_work_start(bool is_normal)
{
	(void)is_normal;
	return;
}
static inline void secure_display_tui_work_start(bool is_tui_entry)
{
	(void)is_tui_entry;
	return;
}
#endif

#endif
