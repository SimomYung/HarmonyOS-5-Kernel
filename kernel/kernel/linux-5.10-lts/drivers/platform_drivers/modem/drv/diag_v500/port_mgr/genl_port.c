/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

#include <securec.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/delay.h>
#include "genl_port.h"
#include <mdrv_timer.h>
#include "bsp_print.h"
#include "bsp_odt.h"
#include "diag_debug.h"
#include "msg_chan.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define GENL_NAME "diag_genl"
#define GENL_TRANS_MAX (64 * 1024) // bytes
#define GENL_HDR_LEN (NLMSG_HDRLEN + GENL_HDRLEN)
#define GENL_FULLPKT_MAX_TRANS_SIZE 0x4000 // bytes

struct genl_data_desc {
    unsigned int msg_seq;
    unsigned int blk_num;
    unsigned int blk_seq;
    unsigned int reserved;
    unsigned int len;
    unsigned char *data;
};

typedef struct {
    unsigned short msg_type;
    unsigned short op;
    unsigned int args_length;
    unsigned int args_start;
} genl_usr_hdr_resp_s;

typedef struct {
    unsigned short msg_type;
    unsigned short op;
    unsigned int args_length;
    union {
        unsigned int args_start;
        struct {
            unsigned int msg_seq;
            unsigned int blk_num;
            unsigned int blk_seq;
            unsigned int reserved;
            unsigned int length;
            unsigned char data[0];
        } send_data;
    };
} genl_usr_hdr_s;
#define GENL_USR_HDR_ARGS_SIZE(_m) (sizeof(((genl_usr_hdr_s *)0)->_m))
#define GENL_MAX_TRANS_SIZE (NLMSG_DEFAULT_SIZE - GENL_HDR_LEN - sizeof(genl_usr_hdr_s))

typedef enum {
	GENL_CMD_UNSPEC,
    GENL_CMD_REQ_MSG, /* CNF上报和REQ下发 */
    GENL_CMD_NO_CODE_MSG, /* 非编码命令 */
    GENL_CMD_CODE_MSG, /* 普通OAM消息命令 */
    GENL_CMD_BUTT,
} genl_cmd_e;

// 需要与chn顺序保持一致
unsigned int cmd_map[GENL_PORT_CHAN_BUTT] = {GENL_CMD_REQ_MSG, GENL_CMD_NO_CODE_MSG, GENL_CMD_CODE_MSG};

typedef struct {
    struct net *net;
    unsigned int port_id;
    genl_recv_cb chn_recv_cb;
} genl_port_chan_ctrl_s;

typedef struct {
    struct genl_family family;
    genl_port_chan_ctrl_s chn_info[GENL_PORT_CHAN_BUTT];
    enum genl_state state;
    genl_event_cb notify_event;
    genl_port_switch_cb notify_port_switch;
    struct wakeup_source *wakelock;
    genl_is_logcat_conn_cb genl_is_logcat_conn;
} diag_genl_ctrl_s;
diag_genl_ctrl_s g_genl_ctrl = {0};

void genl_triger_logcat_discon(void)
{
    unsigned int logcat_discon_msg[2] = {0}; // CP要求ap->cp diag_msg消息必须有data部分，也即msg_len > 4bytes
    msg_frame_s *msg = (msg_frame_s *)logcat_discon_msg;
    int ret;

    diag_err("genl triger logcat disconn\n");
    msg->msg_id = DIAG_MSG_LOGCAT_DISCONN_REQ;
    ret = diag_msg_send((unsigned char *)msg, sizeof(logcat_discon_msg));
    if (ret) {
        diag_err("send genl triger logcat disconn msg fail, ret=0x%x\n", ret);
    }
}

void genl_set_port_info(genl_chan_e chn, struct genl_info *info)
{
    g_genl_ctrl.chn_info[chn].net = genl_info_net(info);
    g_genl_ctrl.chn_info[chn].port_id = info->snd_portid;
}

int genl_recv_para_check(struct sk_buff *skb, struct genl_info *info)
{
    struct nlmsghdr *nlhdr = NULL;

    if (skb == NULL || info == NULL) {
        diag_err("parameter null\n");
        return -1;
    }
    nlhdr = nlmsg_hdr(skb);
    if (nlhdr == NULL) {
        diag_err("nlhdr is null\n");
        return -1;
    }
    if (skb->len < GENL_HDR_LEN) {
        diag_err("skb_len invalid, skb_len=0x%x\n", skb->len);
        return -1;
    }
    return 0;
}

int genl_recv_code_msg(struct sk_buff *skb, struct genl_info *info)
{
    int ret;
    diag_debug("genl code ind chn recv msg\n");
    ret = genl_recv_para_check(skb, info);
    if (ret) {
        return ret;
    }
    g_genl_ctrl.state = GENL_CONNECT;
    genl_set_port_info(GENL_IND_CODER_CHAN, info);
    return 0;
}

int genl_recv_no_code_msg(struct sk_buff *skb, struct genl_info *info)
{
    int ret;
    diag_debug("genl no code ind chn recv msg\n");
    ret = genl_recv_para_check(skb, info);
    if (ret) {
        return ret;
    }
    g_genl_ctrl.state = GENL_CONNECT;
    genl_set_port_info(GENL_IND_NOCODER_CHAN, info);
    return 0;
}

static int genl_cmd_response(unsigned char *data, unsigned int len)
{
    diag_genl_ctrl_s *ctrl = &g_genl_ctrl;
    genl_chan_e chn = GENL_CNF_NOCODER_CHAN;

    struct sk_buff *skb = NULL;
    unsigned int payload_len = NLMSG_ALIGN(len);
    void *msg_head = NULL;
    int ret = -1;

    if (ctrl->chn_info[chn].net == NULL) {
        diag_err("genl chan net is null, chn=0x%x\n", chn);
        return -1;
    }
    skb = genlmsg_new(payload_len, GFP_KERNEL);
    if (skb == NULL) {
        diag_err("genlmsg_new fail\n");
        return -1;
    }
    msg_head = genlmsg_put(skb, ctrl->chn_info[chn].port_id, 0, &ctrl->family, 0, cmd_map[chn]);
    if (msg_head == NULL) {
        diag_err("genlmsg_put fail\n");
        goto failure;
    }
    // 原代码分包封包头，测试大包可传输可以去掉； 另外考虑nla_put按genl attr存放数据；
    // 注意与上层处理一致
    ret = memcpy_s(msg_head, sizeof(genl_usr_hdr_resp_s), data, len);
    if (ret) {
        diag_err("genl usr_data memcpy_s fail\n");
        goto failure;
    }
    skb_put(skb, payload_len);
    genlmsg_end(skb, msg_head);

    ret = genlmsg_unicast(ctrl->chn_info[chn].net, skb, ctrl->chn_info[chn].port_id);
    if (ret < 0) {
        diag_err("genl unicast fail, ret=%d, chan_id = 0x%x", ret, chn);
        return ret;
    }
    return 0;
failure:
    dev_kfree_skb_any(skb);
    return ret;
}

static int genl_get_logmode(unsigned char *data, unsigned int len)
{
    int ret = -1;
    unsigned int mode;

    if (len != sizeof(genl_logmode_s)) {
        diag_err("get logmode data len err\n");
        return ret;
    }

    mode = bsp_odt_get_dst_chan_mode(ODT_CODER_DST_OM_IND);

    diag_crit("genl get ind chan log mode:0x%x ok\n", mode);
    return mode;
}

static int genl_set_logmode(unsigned char *data, unsigned int len)
{
    int ret = -1;
    genl_logmode_s *logmode;

    if (g_genl_ctrl.genl_is_logcat_conn == NULL) {
        diag_err("cant get logcat stat, ignore set logmod\n");
        return 0;
    }
    if (g_genl_ctrl.genl_is_logcat_conn() == 0) {
        diag_err("logcat disconn, ignore set logmod\n");
        return ret;
    }

    if (len != sizeof(genl_logmode_s)) {
        diag_err("set logmode data len err\n");
        return ret;
    }

    logmode = (genl_logmode_s *)data;

    ret = bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_IND, logmode->mode);
    if (ret) {
        diag_err("genl set ind chan log mode:0x%x fail\n", logmode->mode);
        return ret;
    }

    diag_crit("genl set ind chan log mode:0x%x ok\n", logmode->mode);
    return ret;
}

static int genl_get_lite_mode(unsigned char *data, unsigned int len)
{
    int ret = -1;
    unsigned int mode;

    if (len != sizeof(genl_logmode_s)) {
        diag_err("get lite mode data len err\n");
        return ret;
    }

    mode = bsp_odt_get_dst_chan_mode(ODT_CODER_DST_OM_NO_CODER_IND);

    diag_crit("genl get lite chan log mode:0x%x ok\n", mode);
    return mode;
}

typedef struct {
    timer_attr_s tmattr;
    timer_handle tmhandle;
} diag_genl_timer_s;
diag_genl_timer_s g_diag_genl_timer_manager;

static int genl_set_lite_mode(unsigned char *data, unsigned int len)
{
    int ret = -1;
    genl_logmode_s *logmode;
    diag_genl_timer_s *lite_mode_timer = &g_diag_genl_timer_manager;

    if (len != sizeof(genl_logmode_s)) {
        diag_err("set lite mode data len err\n");
        return ret;
    }

    logmode = (genl_logmode_s *)data;

    ret = bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_NO_CODER_IND, logmode->mode);
    if (ret) {
        diag_err("genl set lite chan log mode:0x%x fail\n", logmode->mode);
        return ret;
    }

    if ((logmode->mode == ODT_IND_MODE_DIRECT) && (logmode->timeout != 0)) {
        if (lite_mode_timer->tmhandle >= 0) {
            (void)mdrv_timer_delete(lite_mode_timer->tmhandle);
            lite_mode_timer->tmhandle = -1;
        }
        lite_mode_timer->tmattr.time = logmode->timeout * 1000;
        lite_mode_timer->tmhandle = mdrv_timer_add(&lite_mode_timer->tmattr);
    }

    diag_crit("genl set lite chan log mode:0x%x ok, timeout:0x%x\n", logmode->mode, logmode->timeout);
    return ret;
}

static int genl_port_switch(unsigned char *data, unsigned int len)
{
    int ret = -1;

    if (g_genl_ctrl.notify_port_switch == NULL) {
        diag_err("no port switch cb\n");
        return ret;
    }

    ret = g_genl_ctrl.notify_port_switch(data, len);
    if (ret) {
        diag_err("port switch fail\n");
    }

    diag_crit("port switch ok");
    return ret;
}

static int genl_handle_cmd(unsigned char *data, unsigned int len)
{
    if (g_genl_ctrl.chn_info[GENL_CNF_NOCODER_CHAN].chn_recv_cb == NULL) {
        diag_err("req_chn no recv_cb\n");
        return -1;
    }

    g_diag_dbg.diag_msp_proc_dbg.genl_dl_cnf_nocoder_cnt++;
    g_genl_ctrl.chn_info[GENL_CNF_NOCODER_CHAN].chn_recv_cb(data, len);

    return 0;
}

void genl_req_msg_response(unsigned short msg_type, unsigned int ret)
{
    genl_usr_hdr_resp_s response = { 0 };

    response.msg_type = msg_type;
    response.op = GENL_MSG_CMD;
    response.args_length = sizeof(unsigned int);
    response.args_start = ret;

    genl_cmd_response((unsigned char *)&response, sizeof(genl_usr_hdr_resp_s));
}

int genl_msg_cmd_proc(unsigned char *data, unsigned int data_len)
{
    int ret;

    if (data_len < sizeof(genl_usr_hdr_s)) {
        diag_err("genl cmd len err, len:%d\n", data_len);
        return -1;
    }
    data = data + sizeof(genl_usr_hdr_s);
    data_len = data_len - sizeof(genl_usr_hdr_s);
    ret = genl_handle_cmd(data, data_len);
    return ret;
}

int genl_recv_req_msg(struct sk_buff *skb, struct genl_info *info)
{
    unsigned char *data = NULL;
    unsigned int data_len;
    int ret;
    genl_usr_hdr_s *user_hdr = NULL;

    ret = genl_recv_para_check(skb, info);
    if (ret) {
        return ret;
    }
    g_genl_ctrl.state = GENL_CONNECT;

    genl_set_port_info(GENL_CNF_NOCODER_CHAN, info);

    data = genlmsg_data(nlmsg_data(nlmsg_hdr(skb)));
    user_hdr = (genl_usr_hdr_s *)data;
    if (skb->len <= GENL_HDR_LEN + sizeof(unsigned short)) {
        diag_err("genlmsg data len error, skb.len:%d\n", skb->len);
        return 0;
    }
    data_len = skb->len - GENL_HDR_LEN;

    switch (user_hdr->msg_type) {
        case GENL_MSG_SWITCH_PORT:
            ret = genl_port_switch((unsigned char *)&user_hdr->args_start, user_hdr->args_length);
            break;

        case GENL_MSG_GET_LOG_MODE:
            ret = genl_get_logmode((unsigned char *)&user_hdr->args_start, user_hdr->args_length);
            break;

        case GENL_MSG_SET_LOG_MODE:
            ret = genl_set_logmode((unsigned char *)&user_hdr->args_start, user_hdr->args_length);
            break;

        case GENL_MSG_GET_LITE_MODE:
            ret = genl_get_lite_mode((unsigned char *)&user_hdr->args_start, user_hdr->args_length);
            break;

        case GENL_MSG_SET_LITE_MODE:
            ret = genl_set_lite_mode((unsigned char *)&user_hdr->args_start, user_hdr->args_length);
            break;

        case GENL_MSG_HANDLE_CMD:
            ret = genl_msg_cmd_proc(data, data_len);
            break;

        case GENL_MSG_BIND_PORT:
            diag_err("genl get bind msg type\n");
            ret = 0;
            break;

        default:
            diag_err("genl get invalid msg type:%d\n", user_hdr->msg_type);
            ret = -1;
    }

    genl_req_msg_response(user_hdr->msg_type, (unsigned int)ret);

    diag_err("genl %d msg handle end\n", user_hdr->msg_type);

    return 0;
}
EXPORT_SYMBOL(genl_recv_req_msg);

int genl_register_recv_cb(genl_chan_e chn, genl_recv_cb cb)
{
    if (chn >= GENL_PORT_CHAN_BUTT || cb == NULL) {
        return -1;
    }
    g_genl_ctrl.chn_info[chn].chn_recv_cb = cb;
    return 0;
}

int genl_fill_payload(genl_chan_e chn, void *msg_head, struct genl_data_desc *desc)
{
    genl_usr_hdr_s *usr_hdr = NULL;
    int ret;

    // 原代码分包封包头，测试大包可传输可以去掉； 另外考虑nla_put按genl attr存放数据；
    // 注意与上层处理一致
    usr_hdr = (genl_usr_hdr_s*)msg_head;
    usr_hdr->msg_type = 0;
    usr_hdr->op = 0;
    usr_hdr->args_length = GENL_USR_HDR_ARGS_SIZE(send_data) + desc->len;
    usr_hdr->send_data.msg_seq = desc->msg_seq;
    usr_hdr->send_data.blk_num = desc->blk_num;
    usr_hdr->send_data.blk_seq = desc->blk_seq;
    usr_hdr->send_data.reserved = 0;
    usr_hdr->send_data.length = desc->len;

    ret = memcpy_s(usr_hdr->send_data.data, usr_hdr->send_data.length, desc->data, desc->len);
    if (ret) {
        g_diag_dbg.mntn_data.genl_dbg[chn].memcpy_fail++;
        diag_err("genl usr_data memcpy_s fail\n");
        return -1;
    }
    return 0;
}

int genl_send_segment(genl_chan_e chn, struct genl_data_desc *desc)
{
    diag_genl_ctrl_s *ctrl = &g_genl_ctrl;
    struct sk_buff *skb = NULL;
    unsigned int payload_len = NLMSG_ALIGN(sizeof(genl_usr_hdr_s) + desc->len);
    void *msg_head = NULL;
    int ret = -1;

    if (ctrl->chn_info[chn].net == NULL) {
        g_diag_dbg.mntn_data.genl_dbg[chn].net_is_null++;
        diag_err("genl chan net is null, chn=0x%x\n", chn);
        return -1;
    }
    skb = genlmsg_new(payload_len, GFP_KERNEL);
    if (skb == NULL) {
        g_diag_dbg.mntn_data.genl_dbg[chn].genlmsg_new_fail++;
        diag_err("genlmsg_new fail\n");
        return -1;
    }
    msg_head = genlmsg_put(skb, ctrl->chn_info[chn].port_id, 0, &ctrl->family, 0, cmd_map[chn]);
    if (msg_head == NULL) {
        g_diag_dbg.mntn_data.genl_dbg[chn].genlmsg_put_fail++;
        diag_err("genlmsg_put fail\n");
        goto failure;
    }
    if (genl_fill_payload(chn, msg_head, desc)) {
        goto failure;
    }
    skb_put(skb, payload_len);
    genlmsg_end(skb, msg_head);

    ret = genlmsg_unicast(ctrl->chn_info[chn].net, skb, ctrl->chn_info[chn].port_id);
    if (ret < 0) {
        g_diag_dbg.mntn_data.genl_dbg[chn].unicast_fail++;
        g_diag_dbg.mntn_data.genl_dbg[chn].unicast_errno = ret;
        return ret;
    }
    g_diag_dbg.mntn_data.genl_dbg[chn].genl_send_len += payload_len;
    return 0;
failure:
    dev_kfree_skb_any(skb);
    return ret;
}

void genl_disconn_proc(void)
{
    diag_genl_ctrl_s *ctrl = &g_genl_ctrl;
    ctrl->state = GENL_DISCONNECT;
    diag_err("genl disconnect callback.\n");
    if (ctrl->notify_event != NULL) {
        ctrl->notify_event(ctrl->state);
    }
}

int g_genl_retry_times = 100; // 100MBPS流量，50M odt_dst_buff, 500ms充满，retry 100次仍-11丢包退出
void genl_set_retry(int times)
{
    if (times <= 0) {
        return;
    }
    g_genl_retry_times = times;
}

int genl_send(genl_chan_e chn, unsigned char *buf, unsigned int len)
{
    struct genl_data_desc desc = {0};
    unsigned int block_num, tail_size, i;
    int ret = 0;
    int timeout = g_genl_retry_times;

    if (chn >= GENL_PORT_CHAN_BUTT || buf == NULL) {
        return -1;
    }

    block_num = len / GENL_MAX_TRANS_SIZE;
    tail_size = len % GENL_MAX_TRANS_SIZE;
    desc.blk_num = (tail_size == 0 ? block_num : block_num + 1);
    desc.reserved = 0;
    desc.data = buf;
    desc.len = GENL_MAX_TRANS_SIZE;

    __pm_stay_awake(g_genl_ctrl.wakelock);
    for (i = 0; i < block_num; i++) {
retry:
        ret = genl_send_segment(chn, &desc);
        if (ret == -ECONNREFUSED) {
            goto DISCONN_PROC;
        } else if (ret == -EAGAIN) {
            timeout--;
            if (timeout <= 0) {
                diag_err("genl=-11 retry timeout\n");
                goto out;
            }
            msleep(1);
            goto retry;
        }
        desc.data += GENL_MAX_TRANS_SIZE;
        desc.blk_seq++;
    }

    if (tail_size) {
        desc.len = tail_size;
        ret = genl_send_segment(chn, &desc);
        if (ret == -ECONNREFUSED) {
            goto DISCONN_PROC;
        }
    }
out:
    __pm_relax(g_genl_ctrl.wakelock);
    return ret;

DISCONN_PROC:
    if (chn == GENL_IND_CODER_CHAN) {
        genl_triger_logcat_discon();
    } else {
        genl_disconn_proc();
    }
    __pm_relax(g_genl_ctrl.wakelock);

    return ret;
}

int genl_register_event_cb(genl_event_cb cb)
{
    g_genl_ctrl.notify_event = cb;
    return 0;
}

int genl_register_port_switch_cb(genl_port_switch_cb cb)
{
    g_genl_ctrl.notify_port_switch = cb;
    return 0;
}

int genl_register_get_logcat_stat(genl_is_logcat_conn_cb cb)
{
    g_genl_ctrl.genl_is_logcat_conn = cb;
    return 0;
}

struct genl_ops g_genl_ops[] = {
    {
        .cmd = GENL_CMD_REQ_MSG,
        .doit = genl_recv_req_msg,
    },
    {
        .cmd = GENL_CMD_NO_CODE_MSG,
        .doit = genl_recv_no_code_msg,
    },
    {
        .cmd = GENL_CMD_CODE_MSG,
        .doit = genl_recv_code_msg,
    },
};

int genl_lite_mode_timeout_proc(int data)
{
    int ret;

    ret = bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_NO_CODER_IND, ODT_IND_MODE_DELAY);
    if (ret) {
        diag_err("timeout set lite chan log mode delay fail\n");
    }
    diag_crit("genl lite mode back delay mode ok\n");
    return ret;
}

void genl_lite_mode_timer_init(void)
{
    int ret;
    unsigned char lite_mode_timer_name[] = "genl_lite_mode";
    diag_genl_timer_s *lite_mode_timer = &g_diag_genl_timer_manager;

    lite_mode_timer->tmhandle = -1;
    ret = strcpy_s(lite_mode_timer->tmattr.name, TIMER_NAME_MAX_LEN, lite_mode_timer_name);
    if (ret) {
        diag_err("init genl lite mode timer fail: %d\n", ret);
        return;
    }
    lite_mode_timer->tmattr.flag = TIMER_FLAG_DEFAULT;
    lite_mode_timer->tmattr.callback = genl_lite_mode_timeout_proc;
}

int genl_init(void)
{
    int ret = 0;
    diag_genl_ctrl_s *ctrl = &g_genl_ctrl;

    ctrl->family.hdrsize = 0;
    ctrl->family.version = 1;
    ctrl->family.ops = g_genl_ops;
    ctrl->family.n_ops = ARRAY_SIZE(g_genl_ops);
    ret = strncpy_s(ctrl->family.name, sizeof(ctrl->family.name), GENL_NAME, sizeof(GENL_NAME));
    if (ret) {
        diag_err("strncpy genl name failed, ret=0x%x\n", ret);
        return ret;
    }

    ret = genl_register_family(&ctrl->family);
    if (ret) {
        diag_err("genl_register_family failed, ret=0x%x\n", ret);
        return ret;
    }

    ctrl->wakelock = wakeup_source_register(NULL, "diag_genl");
    if (ctrl->wakelock == NULL) {
        diag_err("diag genl ctrl wakelock register fail\n");
        return -1;
    }

    genl_lite_mode_timer_init();
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
