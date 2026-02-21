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

#include "ips_mntn_cfg.h"
#include "ips_util.h"
#include "netfilter_ips_mntn.h"
#include "ips_mntn_internal.h"
#include "ips_traffic_statistic.h"
#include "securec.h"
#include "netfilter_ex_ctrl.h"
#include "netfilter_buffer.h"
#include "mdrv_event.h"
#include "mdrv_msg.h"
#include "mdrv_diag.h"
#include "ps_log_ue_interface.h"

#define THIS_MODU mod_nfext

STATIC LIST_HEAD (ips_mntn_notify_chain);

/* 网桥中转钩包配置参数检查 */
STATIC bool IPSMNTN_BridgeTraceCfgChkParam(const IPS_MntnTraceCfgReq *msg)
{
    if (msg->bridgeArpTraceCfg.choice > IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN) {
        return false;
    }
    return true;
}

/* TCP/IP协议栈报文钩包配置参数检查 */
STATIC bool IPSMNTN_TraceCfgChkParam(const IPS_MntnTraceCfgReq *msg)
{
    if ((msg->preRoutingTraceCfg.choice > IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN) ||
        (msg->postRoutingTraceCfg.choice > IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN) ||
        (msg->localTraceCfg.choice > IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN) ||
        (msg->bridgeArpTraceCfg.choice > IPS_MNTN_TRACE_WHOLE_DATA_LEN_CHOSEN)) {
        return false;
    }

    if (msg->adsIpConfig > IPS_TRACE_MSG_YES) {
        return false;
    }
    return true;
}

/* 钩包配置参数检查 */
STATIC bool IPSMNTN_TraceAdvancedCfgChkParam(const IPS_MntnTraceCfgReq *msg)
{
    /* TCP/IP协议栈报文钩包配置参数检查和网桥中转报文钩包配置参数检查 */
    if ((IPSMNTN_TraceCfgChkParam(msg) == false) || (IPSMNTN_BridgeTraceCfgChkParam(msg) == false)) {
        return false;
    }

    return true;
}

STATIC void IPSMNTN_TraceCfgNotify(IPS_TraceMsgFlagUint8 adsIpConfig)
{
    struct ips_trace_nofity_s *node = NF_NULL_PTR;
    struct list_head *tmp = NF_NULL_PTR;
    bool traceEnable = (adsIpConfig == IPS_TRACE_MSG_YES) ? true : false;

    list_for_each(tmp, &ips_mntn_notify_chain) {
        node = list_entry(tmp, struct ips_trace_nofity_s, list);
        if (node->notifier != NF_NULL_PTR) {
            node->notifier(traceEnable);
        }
    }
}

/* 保存钩包配置 */
STATIC void IPSMNTN_TraceAdvancedCfgReq(const NfMsgBlock *msg)
{
    const OM_IpsAdvancedTraceCfgReq *cfgReq = (OM_IpsAdvancedTraceCfgReq*)msg;
    bool result = false;

    /* 检测配置参数是否合法 */
    result = IPSMNTN_TraceAdvancedCfgChkParam(&(cfgReq->ipsAdvanceCfgReq));

    if (result == false) {
        IPS_PRINT_ERR("IPSMNTN_TraceAdvancedCfgReq check fail!\n");
        return;
    }

#ifdef CONFIG_IPS_NFEXT
    if (NFEXT_ConfigEffective(&(cfgReq->ipsAdvanceCfgReq)) == false) {
        IPS_PRINT_ERR("IPS_MNTN_SetCallbackFunc fail!\n");
        return;
    }

    IPSMNTN_SetIpsTraceMsgCfg(&(cfgReq->ipsAdvanceCfgReq));
#endif

    IPSMNTN_TraceCfgNotify(cfgReq->ipsAdvanceCfgReq.adsIpConfig);
}

void IPSMNTN_RegTraceCfgNotify(IPS_MntnTraceCfgNotifyFunc notifyFunc)
{
    struct ips_trace_nofity_s *node;

    node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (node != NF_NULL_PTR) {
        node->notifier = notifyFunc;
        list_add_tail(&node->list, &ips_mntn_notify_chain);
    }
}

STATIC bool NFEXT_CheckOmMsgLength(const NfMsgBlock *msg)
{
    uint16_t msgId = (uint16_t)(*((uint32_t *)((uint8_t *)(msg) + NF_OS_MSG_HEAD_LENGTH)));
    uint32_t msgLen = 0;
    if (NF_OS_GET_MSG_LEN(msg) == 0) {
        return false;
    }
    if (msgId == ID_OM_IPS_ADVANCED_TRACE_CONFIG_REQ) {
        msgLen = sizeof(OM_IpsAdvancedTraceCfgReq);
    } else if (msgId == ID_OM_IPS_MNTN_INFO_CONFIG_REQ) {
        msgLen = sizeof(OM_IpsMntnCfgReq);
    } else if (msgId == ID_OM_IPS_MNTN_TRAFFIC_CTRL_REQ) {
        msgLen = sizeof(OM_IpsMntnTrafficCtrlReq);
    }
    if (msgLen > NF_OS_GET_MSG_LEN(msg) + NF_OS_MSG_HEAD_LENGTH) {
        return false;
    }
    return true;
}

/* NFExt接收到来自OM模块的消息处理 */
STATIC void NFEXT_RcvOmMsg(const NfMsgBlock *msg)
{
    uint16_t msgId;

    if (msg == NF_NULL_PTR || NFEXT_CheckOmMsgLength(msg) == false) {
        IPS_PRINT_ERR("NFEXT_RcvOmMsg:msg invalid!\n");
        return;
    }

    msgId = (uint16_t)(*((uint32_t *)((uint8_t *)(msg) + NF_OS_MSG_HEAD_LENGTH)));

    switch (msgId) {
        case ID_OM_IPS_ADVANCED_TRACE_CONFIG_REQ:
            IPSMNTN_TraceAdvancedCfgReq(msg);
            break;
#ifdef CONFIG_IPS_NFEXT
        case ID_OM_IPS_MNTN_INFO_CONFIG_REQ:
            break;
        case ID_OM_IPS_MNTN_TRAFFIC_CTRL_REQ:
            IPSMNTN_TrafficCtrlReq(msg);
            break;
#endif
        default:
            IPS_PRINT_ERR("NFExt_RcvConfig:Receive Unkown Type Message=%d!\n", msgId);
            break;
    }
}

/* NFExt可维可测控制消息处理函数 */
void NFEXT_MsgProc(NfMsgBlock *msg)
{
    if (msg == NF_NULL_PTR) {
        IPS_PRINT_ERR("NFEXT_MsgProc: Message is NULL!\n");
        return;
    }

    switch (NF_OS_GET_SENDER_ID(msg)) {
        case PS_PID_LOGP: /* 来自OM的透传消息处理 */
            NFEXT_RcvOmMsg(msg);
            break;
        default:
            break;
    }
}

#define NFEXT_BULID_OM_HEADER_INFO(header, msgid, logMode, logLevel, msgtype) do { \
        (header)->msg_id = msgid; \
        (header)->mode = logMode; \
        (header)->modem_id = NF_MODEM_ID_0; \
        (header)->level = logLevel; \
        (header)->gid = PS_LOG_GID_PS; \
        (header)->version = 0; \
        (header)->msg_type = msgtype; \
} while (0)

/* 功能描述: DIAG_TransReport函数封装 */
int32_t NFEXT_DiagTransReport(uint32_t msgId, const uint8_t *data, uint16_t length)
{
    struct ps_log_rpt_head rptHead = {0};
    NFEXT_BULID_OM_HEADER_INFO(&rptHead, msgId, PS_LOG_MODE_COMM, PS_LOG_OPEN_LEVEL_2,
        PS_LOG_TYPE_STRUCT);
    ps_log_report(&rptHead, (void *)data, length);
    return NF_OK;
}
