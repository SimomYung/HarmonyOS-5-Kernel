/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
#include "netfilter_ips_mntn.h"
#include "ips_util.h"
#include "ips_traffic_statistic.h"
#include "ips_om_interface.h"
#include "netfilter_ex.h"
#include "netfilter_ex_ctrl.h"
#include "mdrv_diag.h"
#include "securec.h"
#include "mdrv_event.h"
#include "mdrv_msg.h"
#include "ips_mntn_cfg.h"
#include "ps_log_ue_interface.h"

#define THIS_MODU mod_nfext

#define IPS_IFNAMSIZ 16 /* 虚拟网卡名称长度 */
#define IPS_END_NAME 16
#define IPS_MNTN_TRACE_MAX_BYTE_LEN 1514 /* 捕获TCP/IP协议栈报文最长长度 */
#define MAC_HEADER_LENGTH 14
#define NFEXT_STATS_ALLOC_MEM_FAIL 4
#define NF_MSG_SYNC_EVENT 0x200
#define NF_TASK_PRIORITY 38

struct task_struct *msgWorker = NF_NULL_PTR;

/* TCP/IP协议栈报文捕获信息 */
typedef struct {
    uint16_t primId;
    uint16_t toolId;
    uint8_t  netIfName[IPS_IFNAMSIZ]; /* 虚拟网卡名称 */
    uint16_t len;
    uint8_t  data[2];
} IpsMntnPktInfo;

/* 网桥中转报文捕获信息 */
typedef struct {
    uint16_t primId;
    uint16_t toolId;
    uint8_t  srcPort[IPS_END_NAME];  /* 发送虚拟网卡信息 */
    uint8_t  destPort[IPS_END_NAME]; /* 接收虚拟网卡名称 */
    uint16_t len;
    uint8_t  data[2];
} IpsMntnBridgePktInfo;

/* APP控制命令捕获信息 */
typedef struct {
    uint16_t len;
    uint8_t  data[2];
} IpsMntnAppCmdInfo;

/* 流控信息 */
typedef struct {
    uint16_t primId;
    uint16_t toolId;
    uint32_t fcType;
} IpsMntnFlowCtrlInfo;

STATIC IPS_MntnTraceCfgReq g_ipsTraceMsgCfg = {{0}}; /* TCP/IP协议栈可维可测配置信息 */

/* 向OM发送透传消息, 自处理任务勾包完成后释放透传指针; 由于可能导致乱序，不再支持非中断上下文直接勾包 */
STATIC void IPSMNTN_TransMsg(uint8_t *traceData, uint32_t contentLen, IPS_MntnMsgTypeUint16 msgName)
{
    NFEXT_DataRingBufHead *rptHead = (NFEXT_DataRingBufHead *)traceData;
    ps_log_trans_head_t *transHead;
    NFEXT_DataRingBuf bufData = { 0 };

    rptHead->msgName = msgName;
    rptHead->rptLen = contentLen + sizeof(ps_log_trans_head_t);

    transHead = (ps_log_trans_head_t *)(traceData + sizeof(NFEXT_DataRingBufHead));
    transHead->snd_pid = ACPU_PID_NFEXT;

    bufData.data = traceData;

    if (NFEXT_AddDataToRingBuf(&bufData) != NF_OK) {
        NF_EXT_MEM_FREE(ACPU_PID_NFEXT, traceData);
    }
}

/* 根据消息配置信息，获取捕获报文长度 */
STATIC uint32_t IPSMNTN_GetPktLenByTraceCfg(const IPS_MntnTraceCfg *traceCfg, uint16_t pktLen, uint8_t *pktData,
    uint32_t *dataLen)
{
    IPS_MntnTraceChosenUint32 choice = traceCfg->choice;
    uint32_t traceDataLen = 0;
    *dataLen = 0;

#ifndef CONFIG_PS_AP_ENG
    choice = IPS_MIN(choice, IPS_MNTN_TRACE_MSG_HEADER_CHOSEN);
#endif

    /* 判断定时器是否启动 */
    switch (choice) {
        case IPS_MNTN_TRACE_NULL_CHOSEN:
            return NF_ERR;
        case IPS_MNTN_TRACE_MSG_HEADER_CHOSEN:
            traceDataLen = IPS_GetIpDataTraceLen(ACPU_PID_NFEXT, pktData, pktLen);
            *dataLen = IPS_MIN(traceDataLen, pktLen);
            break;
        case IPS_MNTN_TRACE_CONFIGURABLE_LEN_CHOSEN:
            *dataLen = IPS_MIN(traceCfg->traceDataLen, pktLen);
            break;
        case IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN:
            *dataLen = pktLen;
            break;
        default:
            return NF_ERR;
    }

    return NF_OK;
}

STATIC IPS_MntnTraceCfg *IPSMNTN_MatchPktTraceCfg(IPS_MntnMsgTypeUint16 msgType)
{
    /* 根据消息类型选择对应配置信息 */
    if (msgType == ID_IPS_TRACE_INPUT_DATA_INFO) {
        return &(g_ipsTraceMsgCfg.preRoutingTraceCfg);
    } else if (msgType == ID_IPS_TRACE_OUTPUT_DATA_INFO) {
        return &(g_ipsTraceMsgCfg.postRoutingTraceCfg);
    } else if ((msgType >= ID_IPS_TRACE_BRIDGE_DATA_INFO) && (msgType <= ID_IPS_TRACE_BR_FORWARD_FLOW_CTRL_STOP)) {
        return &(g_ipsTraceMsgCfg.localTraceCfg);
    } else {
        return NF_NULL_PTR;
    }
}

#define IPS_MNTN_TRACE_GRO_MAX_BYTE_LEN 64000 /* 捕获TCP/IP协议栈报文最长长度 */

/* 捕获TCP/IP协议栈接收和发送报文回调函数 */
void IPSMNTN_PktInfoCb(const char *netIfName, uint32_t nameLen, const struct sk_buff *skb,
    IPS_MntnMsgTypeUint16 msgType)
{
    uint8_t *traceData = NF_NULL_PTR;
    IpsMntnPktInfo *traceMsg = NF_NULL_PTR;
    const IPS_MntnTraceCfg *traceCfg = NF_NULL_PTR;
    uint8_t *ipHeader = NF_NULL_PTR;
    uint32_t ipDataLen = 0;
    uint32_t captureLen;

    /* debug tool未连接 */
    if (mdrv_diag_get_conn_state() == NF_DISCONN) {
        return;
    }

    traceCfg = IPSMNTN_MatchPktTraceCfg(msgType);
    if (traceCfg == NF_NULL_PTR) {
        return;
    }

    ipHeader = skb_network_header(skb);
    /* 根据消息配置信息，获取输出报文长度 */
    if (IPSMNTN_GetPktLenByTraceCfg(traceCfg, skb->len, ipHeader, &ipDataLen) == NF_ERR) {
        /* 不捕获该报文 */
        return;
    }

    ipDataLen = IPS_MIN(IPS_MNTN_TRACE_GRO_MAX_BYTE_LEN, ipDataLen);
    captureLen = IPS_OFFSET_OF(IpsMntnPktInfo, data);
    /* IP数据长度加MAC头长度对应WireShark解析报文中的Capture Length，即捕获长度 */
    captureLen += ipDataLen;
    captureLen += MAC_HEADER_LENGTH;

    if (IPSMNTN_PktIsOverFlow(captureLen) == true) {
        return;
    }

    /* 从Linux内核申请内存 */
    traceData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, captureLen + sizeof(ps_log_trans_head_t) + sizeof(NFEXT_DataRingBufHead));
    if (traceData == NF_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }
    /* 构建透明消息 */
    traceMsg = (IpsMntnPktInfo *)(traceData + sizeof(ps_log_trans_head_t) + sizeof(NFEXT_DataRingBufHead));
    /* traceMsg->len对应WireShark解析报文中的Frame Length，即帧长度 */
    traceMsg->len = skb->len + MAC_HEADER_LENGTH;
    traceMsg->primId = msgType;
    IPS_SF_CHK(memcpy_s(traceMsg->netIfName, IPS_IFNAMSIZ, netIfName, nameLen));

    /* 在某些情况下(如配置 traceCfg->ulTraceDataLen 为0)或ipDataLen为0的情况，存在异常，这里进行保护 */
    if (ipDataLen > 0) {
        if (skb_copy_bits(skb, 0, traceMsg->data + MAC_HEADER_LENGTH, ipDataLen) < 0) {
            NF_EXT_MEM_FREE(ACPU_PID_NFEXT, traceData);
            return;
        }
    }

    /* user版本下脱敏IP地址 */
#ifndef CONFIG_PS_AP_ENG
    IPS_MaskIpAddrTraces(ACPU_PID_NFEXT, traceMsg->data + MAC_HEADER_LENGTH, (uint16_t)ipDataLen);
#endif

    IPSMNTN_TransMsg(traceData, captureLen, msgType);
}

STATIC IPS_MntnTraceCfg *IPSMNTN_MatchBridgePktTraceCfg(IPS_MntnMsgTypeUint16 msgType)
{
    if ((msgType >= ID_IPS_TRACE_BRIDGE_DATA_INFO) && (msgType <= ID_IPS_TRACE_BR_FORWARD_FLOW_CTRL_STOP)) {
        return &(g_ipsTraceMsgCfg.bridgeArpTraceCfg);
    }
    return NF_NULL_PTR;
}

/* 捕获网桥中转报文回调函数 */
void IPSMNTN_BridgePktInfoCb(const char *srcPort, const char *destPort, uint8_t *pktData,
    uint16_t pktLen, IPS_MntnMsgTypeUint16 msgType)
{
    IpsMntnBridgePktInfo *traceMsg = NF_NULL_PTR;
    uint8_t *traceData = NF_NULL_PTR;
    const IPS_MntnTraceCfg *traceCfg = NF_NULL_PTR;
    uint32_t ipDataLen = 0;
    uint32_t captureLen;

    if (pktData == NF_NULL_PTR) {
        return;
    }

    if (mdrv_diag_get_conn_state() == NF_DISCONN) {
        return;
    }
    /* 根据消息类型选择对应配置信息 */
    traceCfg = IPSMNTN_MatchBridgePktTraceCfg(msgType);
    if (traceCfg == NF_NULL_PTR) {
        return;
    }
    /* 获得IP报文长度 */
    if (IPSMNTN_GetPktLenByTraceCfg(traceCfg, pktLen, pktData, &ipDataLen) == NF_ERR) {
        /* 不捕获该报文 */
        return;
    }

    ipDataLen = IPS_MIN(IPS_MNTN_TRACE_MAX_BYTE_LEN, ipDataLen);
    captureLen = IPS_OFFSET_OF(IpsMntnBridgePktInfo, data);
    /* IP数据长度加MAC头长度对应WireShark解析报文中的Capture Length，即捕获长度 */
    captureLen += ipDataLen;
    captureLen += MAC_HEADER_LENGTH;

    /* 从Linux内核申请内存 */
    traceData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, captureLen + sizeof(ps_log_trans_head_t) + sizeof(NFEXT_DataRingBufHead));
    if (traceData == NF_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }

    /* 构建透明消息 */
    traceMsg = (IpsMntnBridgePktInfo *)(traceData + sizeof(ps_log_trans_head_t) + sizeof(NFEXT_DataRingBufHead));
    /* traceMsg->len对应WireShark解析报文中的Frame Length，即帧长度 */
    traceMsg->len = pktLen + MAC_HEADER_LENGTH;
    traceMsg->primId = msgType;

    if (srcPort != NF_NULL_PTR) {
        IPS_SF_CHK(memcpy_s(traceMsg->srcPort, IPS_END_NAME, srcPort, IPS_END_NAME));
    }
    if (destPort != NF_NULL_PTR) {
        IPS_SF_CHK(memcpy_s(traceMsg->destPort, IPS_END_NAME, destPort, IPS_END_NAME));
    }
    if (ipDataLen > 0) {
        IPS_SF_CHK(memcpy_s(traceMsg->data + MAC_HEADER_LENGTH, ipDataLen, pktData, ipDataLen));
    }

    /* user版本下脱敏IP地址 */
#ifndef CONFIG_PS_AP_ENG
    IPS_MaskIpAddrTraces(ACPU_PID_NFEXT, traceMsg->data + MAC_HEADER_LENGTH, (uint16_t)ipDataLen);
#endif

    IPSMNTN_TransMsg(traceData, captureLen, msgType);
}

/* 捕获TCP/IP协议栈控制报文回调函数 */
void IPSMNTN_CtrlPktInfoCb(const char *netIfName, uint32_t nameLen, const uint8_t *pktData,
    uint16_t pktLen, IPS_MntnMsgTypeUint16 msgType)
{
    uint8_t *traceData = NF_NULL_PTR;
    IpsMntnPktInfo *traceMsg = NF_NULL_PTR;
    uint32_t ipDataLen;
    uint32_t captureLen;

    if (pktData == NF_NULL_PTR) {
        return;
    }

    /* debug tool未连接 */
    if (mdrv_diag_get_conn_state() == NF_DISCONN) {
        return;
    }
    ipDataLen = IPS_MIN(IPS_MNTN_TRACE_MAX_BYTE_LEN, pktLen);
    captureLen = IPS_OFFSET_OF(IpsMntnPktInfo, data);
    /* 对应WireShark解析报文中的Capture Length，即捕获长度 */
    captureLen += ipDataLen;
    /* 从Linux内核申请内存 */
    traceData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, captureLen + sizeof(ps_log_trans_head_t) + sizeof(NFEXT_DataRingBufHead));
    if (traceData == NF_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }

    /* 构建透明消息 */
    traceMsg = (IpsMntnPktInfo *)(traceData + sizeof(ps_log_trans_head_t) + sizeof(NFEXT_DataRingBufHead));
    /* traceMsg->len对应WireShark解析报文中的Frame Length，即帧长度 */
    traceMsg->len = pktLen;
    traceMsg->primId = msgType;
    IPS_SF_CHK(memcpy_s(traceMsg->netIfName, IPS_IFNAMSIZ, netIfName, nameLen));
    if (ipDataLen > 0) {
        IPS_SF_CHK(memcpy_s(traceMsg->data, ipDataLen, pktData, ipDataLen));
    }

    /* user版本下脱敏IP地址 */
#ifndef CONFIG_PS_AP_ENG
    IPS_MaskIpAddrTraces(ACPU_PID_NFEXT, traceMsg->data, (uint16_t)ipDataLen);
#endif

    IPSMNTN_TransMsg(traceData, captureLen, msgType);
}

#define MNTN_FLOW_CTRL_INFO_LEN ((uint32_t)sizeof(IpsMntnFlowCtrlInfo))
/* 协议栈流控可维可测上报 */
void IPSMNTN_FlowCtrl(uint32_t fcType, IPS_MntnMsgTypeUint16 msgType)
{
    IpsMntnFlowCtrlInfo flowCtrlMsg = { 0 };
    uint16_t dataLen = MNTN_FLOW_CTRL_INFO_LEN + sizeof(ps_log_trans_head_t);
    int32_t ret;
    uint8_t *transData = NF_EXT_MEM_ALLOC(ACPU_PID_NFEXT, dataLen);
    if (transData == NF_NULL_PTR) {
        NF_EXT_STATS_INC(1, NFEXT_STATS_ALLOC_MEM_FAIL);
        return;
    }
    ((ps_log_trans_head_t *)transData)->snd_pid = ACPU_PID_NFEXT;

    /* 填充信息 */
    flowCtrlMsg.fcType = fcType;
    flowCtrlMsg.primId = msgType;
    IPS_SF_CHK(memcpy_s(transData + sizeof(ps_log_trans_head_t), MNTN_FLOW_CTRL_INFO_LEN, &flowCtrlMsg, MNTN_FLOW_CTRL_INFO_LEN));

    ret = NFEXT_DiagTransReport(msgType, transData, dataLen);
    if (ret != NF_OK) {
        IPS_PRINT_WARNING("Call NFEXT_DiagTransReport fail!\n");
    }
    NF_EXT_MEM_FREE(ACPU_PID_NFEXT, transData);
}

/* 读取脱敏NV配置，返回脱敏开启标志 */
STATIC bool IPSMNTN_GetPrivacyFilterCfg(void)
{
    uint8_t logFilterFlag = true;
#ifdef CONFIG_PS_AP_ENG
    logFilterFlag = (g_ipsTraceMsgCfg.logFilterFlag == 0) ? false : true;
    IPS_PRINT_ERR("IPSMNTN_GetPrivacyFilterCfg logFilterFlag=%d\n", logFilterFlag);
#endif

    return logFilterFlag;
}

/* 根据脱敏配置，调整钩包配置 */
STATIC void IPSMNTN_TraceSensitiveAdjustSwitch(void)
{
    uint8_t adjustFlag = false;

    /* 非敏感状态不做任何更改 */
    if (IPSMNTN_GetPrivacyFilterCfg() != true) {
        return;
    }

    /* 如下校正勾包状态 */
    if (g_ipsTraceMsgCfg.bridgeArpTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.bridgeArpTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = true;
    }

    if (g_ipsTraceMsgCfg.preRoutingTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.preRoutingTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = true;
    }

    if (g_ipsTraceMsgCfg.postRoutingTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.postRoutingTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = true;
    }

    if (g_ipsTraceMsgCfg.localTraceCfg.choice > IPS_MNTN_TRACE_MSG_HEADER_CHOSEN) {
        g_ipsTraceMsgCfg.localTraceCfg.choice = IPS_MNTN_TRACE_MSG_HEADER_CHOSEN;
        adjustFlag = true;
    }

    if (adjustFlag == true) {
        IPS_PRINT_WARNING("In Sensitive status, Some config has been modify!\n");
    }
}

void IPSMNTN_SetIpsTraceMsgCfg(const IPS_MntnTraceCfgReq *cfgReq)
{
    /* 保存配置参数 */
    g_ipsTraceMsgCfg = *cfgReq;

    IPSMNTN_TraceSensitiveAdjustSwitch();
}

int32_t NFEXT_TaskProc(void *data)
{
    void *msg_get = NULL;
    int32_t ret;
    uint32_t event = 0;
    const uint32_t expectEvent = NFEXT_DATA_PROC_NOTIFY | NFEXT_DEBUG_TOOL_DISCONN_NOTIFY | NF_MSG_SYNC_EVENT;

#ifdef CONFIG_IPS_NFEXT
    ret = NFEXT_Init();
    if (ret) {
        return NF_OK;
    }
    NFEXT_BindToCpu();
#endif

    for (;;) {
        ret = mdrv_event_vrecv(expectEvent, MDRV_EVENT_ANY, MDRV_EVENT_WAIT_FOREVER, &event);
        if (ret) {
            IPS_PRINT_ERR("[nfext_message_task]recv event fail, ret=0x%x\n", ret);
#ifndef __UT_CENTER__
            continue;
#endif
        }

        if (event != NF_MSG_SYNC_EVENT) {
#ifdef CONFIG_IPS_NFEXT
            NFEXT_EventProc(event);
#ifndef __UT_CENTER__
            continue;
#endif
#endif
        }

        ret = mdrv_msg_recv(0, &msg_get, 0);
        if ((ret == NF_OK) && (msg_get != NF_NULL_PTR)) {
            if (NF_OS_GET_RECEIVER_ID(msg_get) == ACPU_PID_NFEXT) {
                NFEXT_MsgProc(msg_get);
            }

            mdrv_msg_free(ACPU_PID_NFEXT, msg_get);
        }
#ifdef __UT_CENTER__
        break;
#endif
    }

    return NF_OK;
}

int32_t nfext_task_load(void)
{
    struct sched_param param;
    int32_t ret;

    msgWorker = kthread_create(NFEXT_TaskProc, NULL, "nfext_message_task");
    if (msgWorker == NF_NULL_PTR) {
        IPS_PRINT_ERR("nfext_task_load kthread_create fail\n");
        return NF_ERR;
    }

    ret = mdrv_msg_qcreate(msgWorker->pid, 1);
    if (ret) {
        IPS_PRINT_ERR("create queue fail, ret=%x\n", ret);
        return ret;
    }

    ret = mdrv_msg_register_mid(msgWorker->pid, ACPU_PID_NFEXT, 0);
    if (ret) {
        IPS_PRINT_ERR("register msg mid fail, ret=%x\n", ret);
        return ret;
    }

    NFEXT_SetTaskId(msgWorker->pid);
    IPS_PRINT_ERR("[init]<nfext_msg_init>: g_nfextTaskId:%d\n", msgWorker->pid);

    /* 设置任务优先级 */
    param.sched_priority = NF_TASK_PRIORITY;

    ret = sched_setscheduler(msgWorker, SCHED_FIFO, &param);
    if (ret) {
        IPS_PRINT_ERR("nfext sched_setscheduler fail, ret=%x\n", ret);
        return ret;
    }

    wake_up_process(msgWorker);

    return NF_OK;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(nfext_task_load);
#endif
