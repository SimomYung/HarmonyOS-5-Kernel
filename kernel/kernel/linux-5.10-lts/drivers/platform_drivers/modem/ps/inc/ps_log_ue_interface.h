/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef PS_LOG_UE_INTERFACE_H
#define PS_LOG_UE_INTERFACE_H

#include <stdbool.h>
#include "mdrv_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PS_LOG_MODEM_INVALID 0x07 /* 不清楚modemid时使用，如果明确modemid请填准确值 */

/*
 * 通道中限制的最大数据长度为64KB，16是一级头8个byte + 二级头8个byte,
 * 具体结构可参见维测接口文档
 */
#define PS_LOG_LAYER_MSG_LENGTH_MAX (64 * 1024 - 16)

/*
 * 版本要求trace消息最大数据长度为16KB，16是一级头8个byte + 二级头8byte
 * 具体参见维测接口文档
 */
#define PS_LOG_TRACE_LENGTH_MAX (64 * 1024 - 16)

/*
 * 版本要求CNF消息最大数据长度为16KB，24是一级头8 + 二级头8 + 信令头8
 * 具体参见维测接口文档
 */
#define PS_LOG_CNF_LENGTH_MAX (16 * 1024 - 24)

/*
 * 由于将三级头放到组件内部了，解析结构又不能变化，为了减少组件定义结构体的工具量，为组件提供统一增加三级头的event宏定义
 * name是组件的结构体名
 */
#define PS_LOG_EVENT_STRUCT_T(name) \
typedef struct { \
	ps_log_event_head_t head; \
	name event; \
} event_##name

#define PS_LOG_EVENT_STRUCT_NAME_T(name) event_##name

/*
 * 由于将三级头放到组件内部了，解析结构又不能变化，为了减少组件定义结构体的工具量，为组件提供统一增加三级头的trans宏定义
 * name是组件的结构体名
 */
#define PS_LOG_TRANS_STRUCT_T(name) \
typedef struct { \
	ps_log_trans_head_t head; \
	name trans; \
} trans_##name

#define PS_LOG_TRANS_STRUCT_NAME_T(name) trans_##name

#define PS_LOG_TRANS_STRUCT(name) \
struct trans_##name { \
	ps_log_trans_head_t head; \
	struct name trans; \
}

#define PS_LOG_TRANS_STRUCT_NAME(name) struct trans_##name

/*
 * 结构说明： group id定义
 */
enum ps_log_gid {
	PS_LOG_GID_PS    = 0x2, /* 在TSP上，除VOICE外协议栈组件使用的GID */
	PS_LOG_GID_AUDIO = 0xc, /* VOICE组件使用的GID */
};

/*
 * 描述: 维测消息类型
 */
enum ps_log_log_type {
	PS_LOG_TYPE_CMD             = 0x0,    /* 控制消息 */
	PS_LOG_TYPE_RESERVED1       = 0x1,    /* 保留 */
	PS_LOG_TYPE_LAYER           = 0x2,    /* 层间消息 */
	PS_LOG_TYPE_PRINT           = 0x3,    /* 打印消息 */
	PS_LOG_TYPE_EVENT           = 0x4,    /* 事件消息 */
	PS_LOG_TYPE_RESERVED3       = 0x5,    /* 保留 */
	PS_LOG_TYPE_RESERVED4       = 0x6,    /* 保留 */
	PS_LOG_TYPE_STRUCT          = 0x7,    /* trans消息 */
	PS_LOG_TYPE_RESERVED5       = 0x8,    /* 保留 */
	PS_LOG_TYPE_RESERVED6       = 0x9,    /* 保留 */
	PS_LOG_TYPE_RESERVED7       = 0xa,    /* 保留 */
	PS_LOG_TYPE_RESERVED8       = 0xb,    /* 保留 */
	PS_LOG_TYPE_RESERVED9       = 0xe,    /* 保留 */
	PS_LOG_TYPE_RESERVED10      = 0xf,    /* 保留 */
	PS_LOG_TYPE_RESERVED11      = 0x13,   /* 保留 */
	PS_LOG_TYPE_RESERVED12      = 0x1f    /* 保留 */
};

/*
 * 描述: 打印级别
 */
enum ps_log_print_level {
	PS_LOG_PRINT_OFF     = 0x0,
	PS_LOG_PRINT_ERROR   = 0x1,
	PS_LOG_PRINT_WARNING = 0x2,
	PS_LOG_PRINT_NORMAL  = 0x3,
	PS_LOG_PRINT_INFO    = 0x4,
	PS_LOG_PRINT_BUTT    = 0x5
};

/*
 * 描述: mode定义
 */
enum ps_log_mode_type {
	PS_LOG_MODE_LTE  = 0x0,
	PS_LOG_MODE_TDS  = 0x1, /* 协议栈不用，为了工具的兼容性，枚举定义时保留 */
	PS_LOG_MODE_GSM  = 0x2,
	PS_LOG_MODE_UMTS = 0x3,
	PS_LOG_MODE_1X   = 0x4, /* 协议栈不用，为了工具的兼容性，枚举定义时保留 */
	PS_LOG_MODE_HRPD = 0x5, /* 协议栈不用，为了工具的兼容性，枚举定义时保留 */
	PS_LOG_MODE_NR   = 0x6,
	PS_LOG_MODE_LTEV = 0x7,
	PS_LOG_MODE_D2D  = 0x9,
	PS_LOG_MODE_TTS  = 0xa,
	PS_LOG_MODE_RESERVED  = 0xb,
	PS_LOG_MODE_COMM = 0xf
};

/*
 * 描述: 消息开放level定义
 */
enum ps_log_open_level {
	PS_LOG_OPEN_LEVEL_0 = 0x0, /* 用于内部研发，级别最高 */
	PS_LOG_OPEN_LEVEL_1 = 0x1, /* 用于客户研发 */
	PS_LOG_OPEN_LEVEL_2 = 0x2, /* 用于客户运维 */
	PS_LOG_OPEN_LEVEL_3 = 0x3, /* 用于用户体验，级别最低 */

	PS_LOG_OPEN_LEVEL_BUTT
};

/*
 * 结构说明  : 组件上报非层间，非打印外的维测消息时，需要提供的维测信息
 */
struct ps_log_rpt_head {
	unsigned int msg_id;
	unsigned char mode; /* 消息模式 */
	unsigned char modem_id;
	unsigned char level; /* 消息开放程度 */
	unsigned char gid; /* VOICE组件使用0xc, 除voice组件外的协议栈组件使用0x2 */
	unsigned short msg_type; /* 消息类型 */
	unsigned char version; /* 消息的版本信息 */
	unsigned char rsv;
};

/*
 * 结构说明  : cnf消息头
 */
struct ps_log_cnf_head {
	unsigned char mode; /* 消息模式 */
	unsigned char modem_id;
	unsigned char version; /* 消息的版本信息 */
	unsigned char gid; /* VOICE组件使用0xc, 除voice组件外的协议栈组件使用0x2 */
	unsigned short snd_pid;
	unsigned short rsv;
};

/*
 * 结构说明  : 层间消息维测上报信息
 */
struct ps_log_layer_head {
	unsigned char modem_id;
	unsigned char mode;
	unsigned char level;
	unsigned char gid;
	unsigned char version; /* 消息的版本信息 */
	unsigned char rsv[3];
};

/*
 * 结构说明  : 组件调用回复接口时，需要回传的参数。
 */
struct ps_log_reply_para {
	unsigned int cmd_id;
	unsigned long long sig_head;
};

struct ps_log_print_head {
	unsigned short snd_pid;
	unsigned short print_level;
	unsigned char gid;
	unsigned char rsv[3];
};

/*
 * 结构说明  : event,trans，dt消息三级头定义
 */
typedef struct ps_log_third_head {
	unsigned short snd_pid;
	unsigned short rsv;
} ps_log_third_head_t;

typedef ps_log_third_head_t ps_log_event_head_t;
typedef ps_log_third_head_t ps_log_trans_head_t;

/**
 * @brief 非层间非打印维测上报接口
 * @par 描述: 用于上报trans，event,air,ims空口，dt和chr消息，且只能上报单条消息
 * @attention 注意事项:
 * 1. 此接口只支持单条消息的上报
 * 2. data长度不应超过宏PS_LOG_TRACE_LENGTH_MAX定义的长度，单位Byte，超过长度消息丢弃
 * 3. 此接口可以用来上报trans,event,air,ims空口，路测和chr消息
 * @param[in] rpt_head: trace消息头
 * @param[in] data： 上报的数据内容指针
 * @param[in] length: 消息长度
 */
void ps_log_report(const struct ps_log_rpt_head *rpt_head, const void *data, unsigned short length);

/**
 * @brief 消息回复上报接口
 * @par 描述: 回复工具或者应用下发的消息
 * @attention 注意事项:
 * 1. 只能用于回复一条消息，平台不做拆分；
 * 2. data数据长度不应超过宏PS_LOG_CNF_LENGTH_MAX 定义的长度，单位Byte，超过长度消息丢弃
 * 3. 由于A核没有CHR，所以这里没有实现CHR的回复，如果后续增加CHR回复，需要调整函数实现
 * @param[in] cnf_head: 回复cnf的头信息，参见struct ps_log_cnf_head
 * @param[in] reply_para: 工具下发的信令头和cmdId，由工具下发时填写，组件回复时直接使用
 * @param[in] data: 上报的数据内容指针
 * @param[in] length: 消息长度
 */
void ps_log_reply(const struct ps_log_cnf_head *cnf_head, const struct ps_log_reply_para *reply_para, const void *data, unsigned short length);

/*
 * @brief 层间消息上报接口
 * @par 描述: 供协议栈组件上报层间维测消息
 * @attention 注意事项:
 * 1. data长度不应超过PS_LOG_LAYER_MSG_LENGTH_MAX定义的长度，单位Byte，超过长度消息丢弃
 * @param[in] layer_head: 层间消息上报需要涉及的额外信息
 * @param[in] data： 上报的数据内容指针
 * @param[in] length： data长度
 */
void ps_log_rpt_layer(const struct ps_log_layer_head *layer_head, const void *data, unsigned short length);

/*
 * @brief 维测打印接口
 * @par 描述: 供协议栈组件上报打印消息
 * @attention 注意事项:
 * 1. string字符串的长度超过512会被截断
 * @param[in] prt_head: 打印消息上报需要涉及的额外信息
 * @param[in] string: 需要输出的打印字符串
 */
void ps_log_print(const struct ps_log_print_head *prt_head, const char *string);

#ifdef __cplusplus
}
#endif
#endif
