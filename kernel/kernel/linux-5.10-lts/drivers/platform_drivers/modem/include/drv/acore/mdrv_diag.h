/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#ifndef __MDRV_DIAG_H__
#define __MDRV_DIAG_H__

#include <stdarg.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
* @brief 工具与DIAG之间的命令原语
*/
#define DIAG_CMD_HOST_CONNECT               0x10065000
#define DIAG_CMD_HOST_CONNECT_AGENT         0x10066000
#define DIAG_CMD_HOST_DISCONNECT            0x10065001
#define DIAG_CMD_HOST_RXREADY               0x10065002
#define DIAG_CMD_HMISRV_CONN                0x10065003
#define DIAG_CMD_HMISRV_DISCONN             0x10065004
#define DIAG_CMD_HMISRV_AUTH                0x10065005
#define DIAG_CMD_HMISRV_LOGCAT_CONN         0x10065006
#define DIAG_CMD_HMISRV_LOGCAT_DISCONN      0x10065007
#define DIAG_CMD_MXT_CONN                   0x10065008
#define DIAG_CMD_MXT_DISCONN                0x10065009
#define DIAG_CMD_MODEM_WILL_RESET           0x10014000
#define DIAG_CMD_GET_TIMESTAMP_VALUE        0x10014001
#define DIAG_CMD_GET_MODEM_NUM              0x10014002
#define DIAG_CMD_PID_TABLE_MSG              0x10014003
#define DIAG_CMD_DIAG_TOOL_CONN_EVENT       0x10014010
#define DIAG_CMD_CONFIG_FINISH              0x1001500D
#define DIAG_CMD_LOG_CAT_PRINT              0x10015310
#define DIAG_CMD_LOG_CAT_TRACE              0x10015311 /* 转发给协议栈 */
#define DIAG_CMD_LOG_CAT_AIR                0x10015312 /* 转发给协议栈 */
#define DIAG_CMD_LOG_CAT_L1                 0x10015314 /* 待确认是否已经废弃 */
#define DIAG_CMD_LOG_CAT_EVENT              0x10015315 /* 转发给协议栈 */
#define DIAG_CMD_LOG_CAT_CMD                0x10015316 /* 待确认是否已经废弃 */
#define DIAG_CMD_LOG_CAT_MSG                0x10015317 /* 待确认是否已经废弃 */
#define DIAG_CMD_DEBUG_MSG                  0x10015318 /* 待确认与DIAG_CMD_LOG_MNTN的关系 */
#define DIAG_CMD_LOG_USERPLANE              0x10015319 /* 转发给协议栈 */
#define DIAG_CMD_LOG_TRANS                  0x1001531A
#define DIAG_CMD_LOG_TIMESTAMP              0x1001531B
#define DIAG_CMD_LOG_MNTN                   0x1001531C /* DIAG Debug配置开关 */
#define DIAG_CMD_FLOW_TEST                  0x1001531D
#define DIAG_CMD_DT_HB                      0x1001531E /* 待确认是否已经废弃 */
#define DIAG_CMD_RATE_INFO                  0x1001531F
#define DIAG_CMD_MINILOG                    0x10015320 /* 不使用，待删除 */
#define DIAG_CMD_DRV_PRINT_CFG              0x10015321 /* 建议删除，统一使用PRINT开关控制 */
#define DIAG_CMD_GTR_SET                    0x10015454
#define DIAG_CMD_GU_GTR_SET                 0x10015455 /* GU的RTT测试，只转发，不回复 */
#define DIAG_CMD_5G_GTR_SET                 0x10015456 /* 5G GTR测试命令 */
#define DIAG_CMD_5G_PS_GTR_SET              0x10015457 /* 5G PS GTR测试命令 */
#define DIAG_CMD_APPLOG_CONFIG              0xEF000001
#define DIAG_DEBUG_TSP_SRVPOINT_MNTN_CMDID  0x9f382002 /* 待确认与DIAG_CMD_LOG_MNTN的关系 */
#define DIAG_DEBUG_TSP_REPORT_MNTN_CMDID    0x9f382003 /* 待确认与DIAG_CMD_LOG_MNTN的关系 */
#define DIAG_CMD_HIGH_TIMESTAPM_IND         0x10015101
#define DIAG_CMD_RATE_INFO_IND              0x9f382005

#define DIAG_CMD_NV_WR                      0x90015001
#define DIAG_CMD_NV_WRDDR                   0x90015002
#define DIAG_CMD_NV_RD                      0x90015003
#define DIAG_CMD_NV_FLUSH                   0x90015004
#define DIAG_CMD_GET_NV_LIST                0x90015005
#define DIAG_CMD_GET_NV_RESUME_LIST         0x90015006
#define DIAG_CMD_NV_AUTH                    0x90015007
#define DIAG_CMD_HIMS_VERSION_QUERY         0x9001500A

/**
* @brief DIAG发送给各个子系统的建连请求
*/
#define ID_MSG_DIAG_CMD_CONNECT_REQ (0x00010200)
#define ID_MSG_DIAG_CMD_CONNECT_CNF (ID_MSG_DIAG_CMD_CONNECT_REQ)

/**
* @brief DIAG发送给各个子系统的断连请求
*/
#define ID_MSG_DIAG_CMD_DISCONNECT_REQ (0x00010202)
#define ID_MSG_DIAG_CMD_DISCONNECT_CNF (ID_MSG_DIAG_CMD_DISCONNECT_REQ)

/**
* @brief DIAG PRINT消息允许的最大长度，超过该长度会被截断上报
*/
#define DIAG_PRINT_MAX_LEN (512 - 1)

/**
* @brief mdrv_diag_ind_report接口的data_type入参，可以通过 “|”的方式进行多选
*/
#define DIAG_REALTIME_REPORT_TYPE           0x1  /* 需要实时上报的消息 */
#define DIAG_PARSE_IN_UE_TYPE               0x2  /* 需要在单板侧内部进行解析的消息，包括CHR，Kit消息，标识业务故障的关键维测信息等 */
#define DIAG_COMMON_OAM_TYPE                0x4  /* 普通维测消息，不需要在单板内部进行解析，上报到HIDS进行解析的消息 */

/**
* @brief ind消息头长度
*/
#define DIAG_IND_INFO_HEAD_LEN (sizeof(diag_ind_info_s) - sizeof(unsigned char *))

/**
* @brief cnf消息头长度
*/
#define DIAG_CNF_INFO_HEAD_LEN (sizeof(diag_cnf_info_s) - sizeof(unsigned char *))

/**
* @brief 获取DIAG_TOOL连接状态宏
*/
#define DIAG_GET_DIAG_TOOL_CONN_STATE(state) ((state) & 0x1)

/**
* @brief 获取HMI_SERVICE连接状态宏
*/
#define DIAG_GET_HMI_SRV_CONN_STATE(state) (((state) & 0x2) >> 1)

/**
* @brief MXT连接状态
*/
#define DIAG_GET_MXT_CONN_STATE(state) (((state) & 0x4) >> 2)

/**
* @brief 获取开机LOG状态
*/
#define DIAG_GET_POWERON_LOG_STATE(state) (((state) & 0x8) >> 3)

typedef enum {
    DIAG_ERRNO_START = 0x4000,
    DIAG_ERRNO_MSG_LEN_ERROR,
    DIAG_ERRNO_PARA_ERROR,
    DIAG_ERRNOR_VSNPRINTF_ERROR,
    DIAG_ERRNO_MALLOC_FAIL,
    DIAG_ERRNO_MEMCPY_FAIL,
    DIAG_ERRNO_GROUP_ID_ERROR,
    DIAG_ERRNO_GET_PDEV_FAIL,
    DIAG_ERRNO_GET_ODT_DST_BUF_FAIL,
    DIAG_ERRNO_NVM_AUTH_FAIL,
    DIAG_ERRNO_NVM_NUM_ERROR,
    DIAG_ERRNO_GET_NVM_LEN_FAIL,
    DIAG_ERRNO_CMD_ID_ERROR,
    DIAG_ERRNO_PHY_GTR_FN_NULL,
    DIAG_ERRNO_DIAG_TOOL_HAVE_ALREADY_LINKED,
    DIAG_ERRNO_NOTIFY_EVENT_FAIL,
    DIAG_ERRNO_DISCONNECT,
    DIAG_ERRNO_TRANS_OFF,
    DIAG_ERRNOR_NO_FREE_SPACE = 0xabab,
    DIAG_ERRNO_BUTT
} diag_errno_info_e;

typedef enum {
    DIAG_SID_DIAG_TOOL = 0x2,      /* HIDS/HiAuto等PC工具使用 */
    DIAG_SID_HMI_SRV = 0x3,        /* HmiService工具使用 */
    DIAG_SID_DEBUG = 0x8           /* Hi-MXT等调试工具使用 */
} diag_frame_sid_e;

typedef enum {
    DIAG_FRAME_VER_0,
    DIAG_FRAME_VER_1,
    DIAG_FRAME_VER_2,
    DIAG_FRAME_VER_BUTT
} diag_frame_ver_e;

/**
* @brief DIAG FRAME中SSID（注意：新增SSID需要找维测SE申请）
*/
typedef enum {
    DIAG_FRAME_SSID_DEFAULT = 0x0, /* 0x00：未指定（仅下发REQ使用，表示由UE确定分发目标，CNF和IND不能使用该值） */
    DIAG_FRAME_SSID_APP_CPU = 0x1,      /* 0x01：A-CPU */
    DIAG_FRAME_SSID_MODEM_CPU = 0x2,    /* 0x02：2G/3G/4.5G子系统C-CPU（底软、PS使用） */
    DIAG_FRAME_SSID_HIFI = 0x6,
    DIAG_FRAME_SSID_MCU = 0x9,
    DIAG_FRAME_SSID_BUTT,
} diag_frame_ssid_e;

/**
* @brief DIAG FRAME中MT类型
*/
typedef enum {
    DIAG_FRAME_MT_RSV = 0x0,
    DIAG_FRAME_MT_REQ = 0x1,
    DIAG_FRAME_MT_CNF = 0x2,
    DIAG_FRAME_MT_IND = 0x3
} diag_frame_mt_e;

/**
* @brief DIAG MSG_ID中消息类型
*/
typedef enum {
    DIAG_FRAME_MSG_CMD          = 0x0,
    DIAG_FRAME_MSG_AIR          = 0x1,
    DIAG_FRAME_MSG_LAYER        = 0x2,
    DIAG_FRAME_MSG_PRINT        = 0x3,
    DIAG_FRAME_MSG_EVENT        = 0x4,
    DIAG_FRAME_MSG_USER         = 0x5,
    DIAG_FRAME_MSG_VOLTE        = 0x6,
    DIAG_FRAME_MSG_STRUCT       = 0x7,
    DIAG_FRAME_MSG_DOT          = 0x8,
    DIAG_FRAME_MSG_DSP_PRINT    = 0x9,
    DIAG_FRAME_MSG_CNF          = 0xa,
    DIAG_FRAME_MSG_IND          = 0xb,
    DIAG_FRAME_MSG_HASH         = 0xe,
    DIAG_FRAME_MSG_DT           = 0xF,
    DIAG_FRAME_MSG_CHR          = 0x13,
    DIAG_FRAME_MSG_MXT          = 0x14,
    DIAG_FRAME_MSG_STAT         = 0x1f
} diag_frame_msg_type_e;

/**
* @brief DIAG MSG_ID中group_id
*/
typedef enum {
    DIAG_MSG_TYPE_RSV       = 0x0,
    DIAG_MSG_TYPE_MSP       = 0x1,
    DIAG_MSG_TYPE_PS        = 0x2,
    DIAG_MSG_TYPE_PHY       = 0x3,
    DIAG_MSG_TYPE_BBP       = 0x4,
    DIAG_MSG_TYPE_HSO       = 0x5,
    DIAG_MSG_TYPE_BSP       = 0x9,
    DIAG_MSG_TYPE_EASYRF    = 0xa,
    DIAG_MSG_TYPE_AP_BSP    = 0xb,
    DIAG_MSG_TYPE_AUDIO     = 0xc,
    DIAG_MSG_TYPE_APP       = 0xe,
    DIAG_MSG_TYPE_BUTT
} diag_frame_group_e;

/**
* @brief DIAG MSG_ID中模类型
*/
typedef enum mdrv_diag_mode_type {
    DIAG_MODE_LTE  = 0x0,
    DIAG_MODE_TDS  = 0x1,
    DIAG_MODE_GSM  = 0x2,
    DIAG_MODE_UMTS = 0x3,
    DIAG_MODE_1X   = 0x4,
    DIAG_MODE_HRPD = 0x5,
    DIAG_MODE_NR   = 0x6,
    DIAG_MODE_LTEV = 0x7,
    DIAG_MODE_D2D  = 0x9,
    DIAG_MODE_TTS  = 0xa,
    DIAG_MODE_COMM = 0xf
} diag_frame_mode_e;

/**
* @brief DIAG FRAME Modem_ID
*/
typedef enum {
    DIAG_MODEM_0 = 0x0,
    DIAG_MODEM_1 = 0x1,
    DIAG_MODEM_2 = 0x2,
    DIAG_MODEM_COMMON = 0x7
} diag_frame_modem_id_e;

/**
* @brief DIAG FRAME Level级别定义
*/
typedef enum {
    DIAG_LOG_LEVEL_0,
    DIAG_LOG_LEVEL_1,
    DIAG_LOG_LEVEL_2,
    DIAG_LOG_LEVEL_3,
    DIAG_LOG_LEVEL_BUTT
} diag_frame_level_e;

/**
* @brief DIAG Port Type定义
*/
typedef enum {
    DIAG_PORT_TYPE_CODER,
    DIAG_PORT_TYPE_NO_CODER,
    DIAG_PORT_TYPE_BUTT
} diag_port_type_e;

/**
 * @brief 工具下发给DIAG的链接请求消息
 */
typedef struct {
    unsigned int sid : 8;
    unsigned int modem_id : 3;
    unsigned int rsv : 1;
    unsigned int ssid : 4;
    unsigned int session_id : 8;
    unsigned int mt : 2;
    unsigned int index : 4;
    unsigned int eof : 1;
    unsigned int ff : 1;

    unsigned int trans_id;
    unsigned int timestamp[0x2];

    unsigned int cmd_id;
    unsigned int msg_len;

    unsigned char data[0];
} diag_conn_info_s;

/**
 * @brief 工具下发给DIAG的配置消息，考虑到兼容性，使用原消息格式
 */
typedef struct {
    unsigned int sid : 4;
    unsigned int ver_id : 4;
    unsigned int modem_id : 3;
    unsigned int rsv1 : 5;
    unsigned int ssid : 8;
    unsigned int mt : 2;
    unsigned int index : 4;
    unsigned int eof : 1;
    unsigned int ff : 1;

    unsigned int trans_id;
    unsigned int timestamp;

    unsigned int cmd_id;
    unsigned int msg_len;

    unsigned int auid;
    unsigned short sn;
    unsigned char rsv2;
    unsigned char port_type;
    unsigned char data[0];
} diag_frame_req_s;

/**
 * @brief DIAG发送给业务组件的消息结构
 */
typedef struct {
    unsigned int cmd_id;
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

    unsigned int auid;
    unsigned short sn;
    unsigned char rsv;
    unsigned char port_type;
    unsigned char data[0];
} diag_cfg_info_s;

/**
 * @brief DIAG给工具CNF消息结构
 */
typedef struct {
    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int ssid : 8;
    unsigned int trans_id : 8;
    unsigned int mt : 2;
    unsigned int fragment : 4;
    unsigned int eof_flag : 1;
    unsigned int ff_flag : 1;
    unsigned int timestamp;

    unsigned int cmd_id;

    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

    unsigned int auid;
    unsigned short sn;
    unsigned char rsv;
    unsigned char port_type;

    unsigned char *data;
} diag_frame_cnf_s;

/**
 * @brief 业务组件给调用DIAG接口上报confirm消息的数据结构
 */
typedef struct {
    unsigned int cmd_id;

    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

    unsigned int auid;
    unsigned short sn;
    unsigned char rsv;
    unsigned char port_type;

    unsigned char *data;
} diag_cnf_info_s;

/**
 * @brief 业务组件上报IND消息的数据结构
 */
typedef struct {
    unsigned int cmd_id;
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;
    unsigned char *data;
} diag_ind_info_s;

/**
 * @brief diag信令回复接口
 * @par 描述:
 * DIAG提供给业务组件使用，用于回复上层工具下发的信令消息
 * @param[in] unsigned char ssid 业务组件上报的ssid(物理层需要上报真实的SSID，DRV和协议栈可以由平台层统一上报)
 * @param[in] cnf_msg 信令回复消息
 * @param[in] unsigned int data_len 信令回复消息的长度，包括diag_cnf_info_s结构和实际数据内容
 * @see  mdrv_diag_cnf_info_s
 * @retval 0：上报成功；
               非0：上报失败。
 */
int mdrv_diag_cnf_report(diag_cnf_info_s *cnf_msg, unsigned int data_len);

/**
 * @brief diag数据业务高性能上报接口
 * @par 描述:
 * DIAG提供数据业务上报接口，该接口可以上报不超过64K的数据信息，同时用户可定制消息上报类型(支持同时选择实时消息/UE内部解析消息/OAM维测消息)，但该接口性能不敏感，因此在紧时序业务中禁止使用。
 * @param[in] ind_type 用户定制的消息上报类型，支持同时选择实时消息/UE内部解析消息/OAM维测消息
 * @param[in] ssid 业务组件上报的ssid(物理层需要上报真实的SSID，DRV和协议栈可以由平台层统一上报)
 * @param[in] ind_msg 上报的消息内容
 * @param[in] data_len data消息的总长度，包括mdrv_diag_ind_s结构和实际数据内容
 * @see mdrv_diag_ind_s
 * @retval 0：上报成功；
               非0：上报失败。
 */
int mdrv_diag_ind_report(unsigned int ind_type, diag_ind_info_s *ind_msg, unsigned int data_len);

/**
 * @brief diag打印上报接口(非hash)
 * @par 描述:
 * DIAG提供的HIDS 打印维测上报接口，该接口性能不敏感，无法转成hash字典，字符串格式化由接口内部处理。
 * @param[in] head 上报的消息头信息，其中diag_ind_info_s中的data[0]在本接口中不需要填入任何信息
 * @param[in] fmt 字符串信息
 * @param[in] .... 可变参数
 * @see mdrv_diag_ind_s
 * @retval 0：上报成功；
               非0：上报失败。
 */
int mdrv_diag_print_report(diag_ind_info_s *ind_head, const char *fmt, ...);

/**
 * @brief DIAG连接状态查询接口
 * @par 描述:
 * 该接口提供协议栈和物理层使用，用于获取DIAG当前连接状态
 * @param[in]  void
 * @retval：工具连接状态 (0: disconn, 1: conn; bit0: diag_tool连接状态, bit1: hmi_srv连接状态, bit2: Hi-MXT连接状态, bit3: Hi-MXT连接状态)
 */
unsigned int mdrv_diag_get_conn_state(void);

/**
 * @brief DIAG状态变化通知注册函数类型
 * @par 描述:
 * 该接口用于提供给协议栈、PHY、HIFI等，用于注册DIAG连接状态变化通知函数
 * @attention
 * <ul><li>mdrv_diag_conn_state_notify_fun函数会在DIAG和工具连接断开过程中调用，其中不能有任何阻塞性操作</li></ul>
 * @param[in]  state , 工具连接状态 (0: disconn, 1: conn; bit0: diag_tool连接状态, bit1: hmi_srv连接状态, bit2: Hi-MXT连接状态)
 * @retval void。
 */
typedef void (*mdrv_diag_conn_state_notify_fun)(unsigned int state);

/**
 * @brief DIAG连接状态通知注册接口
 * @par 描述:
 * 该接口提供协议栈、物理层等上层组件使用，用于注册DIAG连接状态通知处理函数
 * @attention
 * <ul><li>mdrv_diag_conn_state_notify_fun函数会在DIAG和工具连接断开过程中调用，其中不能有任何阻塞性操作</li></ul>
 * <ul><li>不允许反复注册,单个组件只允许注册一次</li></ul>
 * @param[in] mdrv_diag_conn_state_notify_fun func, DIAG连接状态通知处理函数
 * @retval 0: 注册成功；
 * @retval 12: 注册资源不足；
 */
unsigned int mdrv_diag_conn_state_notify_fun_reg(mdrv_diag_conn_state_notify_fun func);

// 以下接口打桩，后续替换正式接口后删除
#define MDRV_DIAG_GEN_MODULE_EX(modem_id, mode_type, group_id)  \
    (((unsigned int)((modem_id) & 0xff) << 24)  \
     | ((unsigned int)((mode_type) & 0xf) << 16) \
     | ((unsigned int)((group_id)  & 0xf)  << 8))

#define MDRV_DIAG_GEN_MODULE(modem_id, mode_type)  \
    (((unsigned int)((modem_id) & 0xff) << 24)  \
     | ((unsigned int)((mode_type) & 0xf) << 16)  \
     | ((unsigned int)(DIAG_MSG_TYPE_PS & 0xf)  << 8))

typedef enum {
    DIAG_STATE_DISCONN  = 0,
    DIAG_STATE_CONN = 1,
    DIAG_STATE_POWERON_LOG = 2,
    DIAG_STATE_BUTT
} mdrv_diag_state_e;
// iot_acore pmctrl_v200使用
typedef enum {
    PS_LOG_LEVEL_OFF  = 0,
    PS_LOG_LEVEL_ERROR,
    PS_LOG_LEVEL_WARNING,
    PS_LOG_LEVEL_NORMAL,
    PS_LOG_LEVEL_INFO,
    PS_LOG_LEVEL_BUTT
} mdrv_diag_log_level_e;

#define MDRV_DIAG_GEN_LOG_MODULE(modem_id, mode_type, level)  \
    (((unsigned int)((modem_id) & 0xff) << 24)  \
     | ((unsigned int)((mode_type) & 0xf) << 16)  \
     | ((unsigned int)((level) & 0xf) << 12) \
     | ((unsigned int)(DIAG_MSG_TYPE_PS & 0xf)  << 8))

static inline unsigned int mdrv_diag_log_report(unsigned int module_id, unsigned int pid,
    char *file_name, unsigned int line_num, char* fmt, ...)
{
    return 0;
}

typedef struct {
    unsigned int        ulModule;
    unsigned int        ulPid;
    unsigned int        ulMsgId;
    unsigned int        ulReserve;
    unsigned int        ulLength : 16;
    unsigned int        reserve : 8;
    unsigned int        version : 8;
    void               *pData;
} mdrv_diag_trans_ind_s;
static inline unsigned int mdrv_diag_trans_report(mdrv_diag_trans_ind_s *trans_data)
{
    return 0;
}

typedef struct {
    unsigned int        module;
    unsigned int        sendpid;
    unsigned int        recvpid;
    unsigned int        msgid;
    unsigned int        length: 16;
    unsigned int        reserve: 8;
    unsigned int        version: 8;
    void                *data;
} mdrv_diag_trace_s;
static inline unsigned int mdrv_diag_trace_report(mdrv_diag_trace_s *trace_data)
{
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

