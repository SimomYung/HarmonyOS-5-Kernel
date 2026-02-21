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
#include <securec.h>
#include <linux/module.h>
#include <osl_sem.h>
#include <osl_thread.h>
#include <osl_malloc.h>
#include <msg_id.h>
#include <mdrv_msg.h>
#include <mdrv_diag.h>
#include <bsp_slice.h>
#include <bsp_msg.h>
#include <bsp_odt.h>
#include <bsp_dpa.h>
#include <bsp_diag.h>
#include "ppm.h"
#include "soft_decode.h"
#include "scm_src.h"
#include "msg_chan.h"
#include "diag_msg_proc.h"
#include "diag_msp_msg.h"
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

msg_chan_info_s g_msg_chan_info;

struct payload {
    unsigned int p_id;
    unsigned int p_len;

    unsigned char p_data[0];
};

struct msghead {
    unsigned int sid : 4;    /* server id */
    unsigned int ver : 4;    /* service version */
    unsigned int mdmid : 3;  /* modem id */
    unsigned int rsv : 5;
    unsigned int ssid : 8;    /* subsystem id */
    unsigned int msgtype : 2;
    unsigned int segidx : 4;   /* segment index */
    unsigned int segend : 1;   /* end segment flag */
    unsigned int segment : 1;    /* segment flag */
    unsigned int trans_id;
    unsigned int timestamp;
    struct payload pyld;
};

struct oldmsghead {
    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int mdmid : 3;
    unsigned int rsv : 1;
    unsigned int ssid : 4;
    unsigned int srvid : 8;
    unsigned int msgtype : 2;
    unsigned int segidx : 4;
    unsigned int segend : 1;
    unsigned int segment : 1;
    unsigned int transid;
    unsigned int timelow;
    unsigned int timehigh;
    struct payload pyld;
};

struct msgslice {
    unsigned int segidx;
    unsigned int msglen;
    unsigned int offset;
    unsigned int timestamp;
    struct list_head list;
    struct msghead *msghd;
};

#define MSGSLICE_TIMEOUT_VALUE (3 * 32768) /* 3s */

static void scan_timeout_msgslice(void)
{
    struct msgslice *curslice = NULL;
    struct msgslice *nextslice = NULL;
    unsigned int curtime = bsp_get_slice_value();

    osl_sem_down(&g_msg_chan_info.slicesem);

    list_for_each_entry_safe(curslice, nextslice, &g_msg_chan_info.slicelist, list) {
        if ((curtime > curslice->timestamp) && \
            ((curtime - curslice->timestamp) > MSGSLICE_TIMEOUT_VALUE)) {

            if ((curslice->list.prev != NULL) && (curslice->list.next != NULL)) {
                list_del(&curslice->list);
            }

            if (curslice->msghd != NULL) {
                diag_err("scan one timeout msg slice p_id:0x%x\n", curslice->msghd->pyld.p_id);
                osl_free(curslice->msghd);
            }

            osl_free(curslice);
        }
    }

    osl_sem_up(&g_msg_chan_info.slicesem);
}

static struct msgslice *get_matched_msgslice(struct msghead *msghd, unsigned int slicelen)
{
    unsigned int msglen;
    struct msgslice *curslice = NULL;
    struct msgslice *nextslice = NULL;

    osl_sem_down(&g_msg_chan_info.slicesem);
    list_for_each_entry_safe(curslice, nextslice, &g_msg_chan_info.slicelist, list) {
        curslice->msghd->segidx = msghd->segidx;
        curslice->msghd->segend = msghd->segend;

        /* the pyld of second package does not contain p_id and p_len, it's all data */
        if (memcmp(curslice->msghd, msghd, (sizeof(struct msghead) - sizeof(struct payload))) == 0) {
            osl_sem_up(&g_msg_chan_info.slicesem);
            return curslice;
        }
    }
    osl_sem_up(&g_msg_chan_info.slicesem);

    if (msghd->segidx != 0) {
        diag_err("get matched can't find this segment slice p_id:0x%x\n", msghd->pyld.p_id);
        return NULL;
    }

    curslice = (struct msgslice *)osl_malloc(sizeof(struct msgslice));
    if (curslice == NULL) {
        diag_err("get matched msgslice malloc fail\n");
        return NULL;
    }

    INIT_LIST_HEAD(&curslice->list);

    msglen = msghd->pyld.p_len + sizeof(struct msghead);
    curslice->msglen = msglen;

    curslice->msghd = (struct msghead *)osl_malloc(msglen);
    if (curslice->msghd == NULL) {
        diag_err("get matched malloc msg head fail\n");
        osl_free(curslice);
        return NULL;
    }

    if (memcpy_s(curslice->msghd, msglen, msghd, slicelen)) {
        diag_err("get matched memcpy slice msg fail\n");
    }

    curslice->segidx = 0;
    curslice->offset = slicelen;
    curslice->timestamp = mdrv_timer_get_normal_timestamp();

    osl_sem_down(&g_msg_chan_info.slicesem);
    list_add_tail(&curslice->list, &g_msg_chan_info.slicelist);
    osl_sem_up(&g_msg_chan_info.slicesem);

    return curslice;
}

static unsigned int merge_matched_msgslice(struct msgslice *msgslice, struct msghead *msghd, unsigned int slicelen)
{
    unsigned int datalen = slicelen - sizeof(struct msghead) + sizeof(struct payload);
    unsigned char *dataptr = (unsigned char *)msgslice->msghd + msgslice->offset;
    unsigned int leftlen = msgslice->msglen - msgslice->offset;

    if ((datalen > leftlen) && (msghd->segend == 0)) {
        diag_err("merge matched msg data length err\n");
        return DIAG_PARSEMSG_MERGE_MSGLEN_ERR;
    }

    datalen = (datalen > leftlen) ? leftlen : datalen;

    if (memcpy_s(dataptr, leftlen, (unsigned char *)&msghd->pyld, datalen)) {
        diag_err("merge matched msgslice fail\n");
        return DIAG_PARSEMSG_MERGE_MSGLEN_ERR;
    }

    msgslice->segidx = msghd->segidx;
    msgslice->offset += datalen;
    msgslice->timestamp = mdrv_timer_get_normal_timestamp();

    return 0;
}

#define DIAG_PACK_MSG 0x6961
#define DIAGMSG_SLICE_MAX_LEN (4 * 1024)
#define PAYLOAD_MAX_LEN (DIAGMSG_SLICE_MAX_LEN - sizeof(struct msghead))
#define PAYLOAD_MAX_CNT (16)
/* only msg slice of segidx=0 has p_id and p_len */
#define PACK_MAX_PAYLOAD_LEN (PAYLOAD_MAX_CNT * DIAGMSG_SLICE_MAX_LEN - sizeof(struct msghead))

static unsigned int combine_msg_package(struct msghead *msghd, unsigned int msglen, struct msghead **packmsg)
{
    unsigned int ret;
    struct msgslice *msgslice = NULL;
    unsigned int msghdlen = (msghd->ver == DIAG_FRAME_VER_1) ? \
        sizeof(struct msghead) : sizeof(struct oldmsghead);
    unsigned int pyldlen = (msghd->ver == DIAG_FRAME_VER_1) ? \
        msghd->pyld.p_len : ((struct oldmsghead *)msghd)->pyld.p_len;

    /* only msg slice of segidx=0 has p_id and p_len,
     * the following msg slice's payload is all data, when splitting msg
     */
    if (((msghd->segidx == 0) && (pyldlen > PACK_MAX_PAYLOAD_LEN)) \
        || (((msghd->segment == 0) || ((msghd->ver != DIAG_FRAME_VER_1))) && (msglen != (msghdlen + pyldlen)))) {
        diag_err("get matched msgslice length too long(msglen:0x%x, msghdlen:0x%x, p_len:0x%x)\n", msglen, msghdlen, pyldlen);
        diag_err("segidx:0x%x, segment:0x%x, ver:0x%x, len:0x%x\n", msghd->segidx, msghd->segment, msghd->ver, msghdlen);
        return DIAG_PARSEMSG_GET_INVALID_MSG_ERR;
    }

    /* old version msg can't be splitted to segment */
    if ((msghd->ver != DIAG_FRAME_VER_1) || (msghd->segment == 0)) {
        *packmsg = msghd;
        return DIAG_PACK_MSG;
    }

    scan_timeout_msgslice();

    msgslice = get_matched_msgslice(msghd, msglen);
    if (msgslice == NULL) {
        diag_err("combine msg package get matched slice fail\n");
        return DIAG_PARSEMSG_GET_MATCHED_MSG_ERR;
    }

    if ((msghd->segidx == 0) || (msghd->segidx != (msgslice->segidx + 1))) {
        diag_debug("msg package no need to combine\n");
        return 0;
    }

    ret = merge_matched_msgslice(msgslice, msghd, msglen);
    if (ret) {
        diag_err("msg package merge fail:%d\n", ret);
        return DIAG_PARSEMSG_MERGE_MSGSLICE_ERR;
    }

    if (msghd->segend) {
        *packmsg = msgslice->msghd;
        return DIAG_PACK_MSG;
    }

    return 0;
}

static void destroy_msg_package(struct msghead *msghd)
{
    struct msgslice *curslice = NULL;
    struct msgslice *nextslice = NULL;

    if (msghd->segment == 0) {
        return;
    }

    osl_sem_down(&g_msg_chan_info.slicesem);
    list_for_each_entry_safe(curslice, nextslice, &g_msg_chan_info.slicelist, list) {
        curslice->msghd->segidx = msghd->segidx;
        curslice->msghd->segend = msghd->segend;

        if (memcmp(curslice->msghd, msghd, sizeof(struct msghead)) != 0) {
            continue;
        }

        if ((curslice->list.prev != NULL) && (curslice->list.next != NULL)) {
            list_del(&curslice->list);
        }

        if (curslice->msghd != NULL) {
            osl_free(curslice->msghd);
        }

        osl_free(curslice);
    }
    osl_sem_up(&g_msg_chan_info.slicesem);
}

int diag_dl_msg_check(unsigned int port_type, const unsigned char *data, unsigned int data_len, unsigned int *cmd_id)
{
    unsigned int diag_ver;

    // MXT 无需检查
    if (((diag_frame_req_s *)data)->sid == DIAG_SID_DEBUG) {
        diag_debug("recv mxt msg, data_len:0x%x\n", data_len);
        return 0;
    }
    // 注意：要求HMI和HIDS 连接、断连、鉴权消息，ver=0；hmi重数据通道开关ver=1; TODO 优化它
    // 向前兼容，SID 0:4G版本; 1:5G版本; 2:维测融合版本。DIAG_TOOL为了少修改，融合版本也会传1
    // 这里利用diag_ver==1区分了已经鉴权后的消息交互 or 连接和非法消息
    diag_ver = ((diag_frame_req_s *)data)->ver_id;
    if (diag_ver == DIAG_FRAME_VER_1) {
        *cmd_id = ((diag_frame_req_s *)data)->cmd_id;
        if (diag_is_auth_succ((diag_frame_req_s *)data, port_type) == false) {
            diag_err("diag msg auth fail, cmd_id=0x%x\n", *cmd_id);
            diag_dbg_show_cp_conn_stat_trace();
            return -1;
        }
    } else {
        *cmd_id = ((diag_conn_info_s *)data)->cmd_id;
        if (diag_msg_is_conn_cmd(*cmd_id) != true) {
            diag_err("diag tool struct error, cmd_id=0x%x\n", *cmd_id);
            return -1;
        }
    }
    return 0;
}

void diag_dl_msg_proc(unsigned int port_type, const unsigned char *data, unsigned int data_len)
{
    int ret;
    unsigned int cmd_id;
    unsigned int total_len;
    msg_frame_s *msg_frame = NULL;
    struct msghead *msghd = NULL;
    struct msghead *packmsg = NULL;

    if ((data == NULL) || (data_len < sizeof(diag_frame_head_s)) || (data_len > DIAG_MSG_MAX_LEN)) {
        diag_err("parse diag dl msg fail\n");
        return;
    }

    __pm_stay_awake(g_msg_chan_info.wakelock);

    if (diag_dl_msg_check(port_type, data, data_len, &cmd_id)) {
        __pm_relax(g_msg_chan_info.wakelock);
        return;
    }

    msghd = (struct msghead *)data;
    if ((msghd->ver != DIAG_FRAME_VER_1) && (data_len < sizeof(struct oldmsghead))) {
        diag_err("parse diag msg recv invalid old msg data\n");
        __pm_relax(g_msg_chan_info.wakelock);
        return;
    }

    ret = combine_msg_package(msghd, data_len, &packmsg);
    if (ret != DIAG_PACK_MSG) {
        diag_debug("combine err\n");
        __pm_relax(g_msg_chan_info.wakelock);
        return;
    }
    if (packmsg->ver == DIAG_FRAME_VER_1) {
        total_len = sizeof(struct msghead) + packmsg->pyld.p_len;
    } else {
        total_len = sizeof(struct oldmsghead) + ((struct oldmsghead *)packmsg)->pyld.p_len;
    }
    if ((packmsg->sid != DIAG_SID_DEBUG) && (packmsg->ver == DIAG_FRAME_VER_1)) {
        DIAG_MSG_SET_PORT_TYPE(packmsg, port_type);
    }

    diag_debug("ver:0x%x, total len:0x%x\n", msghd->ver, total_len);
    msg_frame = (msg_frame_s *)osl_malloc(sizeof(msg_frame_s) + total_len);
    if (msg_frame == NULL) {
        diag_err("msg frame malloc fail\n");
        __pm_relax(g_msg_chan_info.wakelock);
        return;
    }

    msg_frame->msg_id = DIAG_MSG_ID_REQ_DL;
    ret = memcpy_s(msg_frame->data, total_len, packmsg, total_len);
    if (ret) {
        diag_err("msg frame memcpy fail\n");
        osl_free(msg_frame);
        __pm_relax(g_msg_chan_info.wakelock);
        return;
    }

    g_diag_dbg.diag_msp_proc_dbg.dl_msglite_cnt++;
    ret = diag_msg_send((unsigned char *)msg_frame, sizeof(msg_frame_s) + total_len);
    if (ret) {
        diag_err("diag msg send to ccore fail, ret=0x%x, cmd_id = 0x%x\n", ret, cmd_id);
    }

    osl_free(msg_frame);
    destroy_msg_package(packmsg);

    __pm_relax(g_msg_chan_info.wakelock);

    return;
}

void diag_no_coder_proc_register(msg_chan_dl_cb func)
{
    if (g_msg_chan_info.dl_data_proc == NULL) {
        g_msg_chan_info.dl_data_proc = func;
    }

    return;
}

void diag_no_coder_data_rcv_handler(unsigned char *data, unsigned int len)
{
    if (g_msg_chan_info.dl_data_proc != NULL) {
        g_msg_chan_info.dl_data_proc(DIAG_PORT_TYPE_NO_CODER, data, len);
    }
}

int diag_no_coder_disconn_handler(ppm_chan_e chn, ppmstate_e stat)
{
    int ret;

    if (chn != PPM_IND_NOCODER_CHAN && stat != PPM_STAT_DISCONNECT) {
        return -1;
    }

    ret = diag_trigger_disconn(DIAG_TOOL_TYPE_HMISRV);
    if (ret) {
        diag_err("trigger disconn fail\n");
        return ret;
    }

    return 0;
}

int diag_no_coder_chan_init(void)
{
    int ret;

    ret = ppm_register_recv_cb(PPM_CNF_NOCODER_CHAN, diag_no_coder_data_rcv_handler);
    if (ret) {
        diag_err("diag regist no coder ppm chan recv fail, ret:0x%x\n", ret);
    }

    ret = ppm_register_event_cb(PPM_IND_NOCODER_CHAN, diag_no_coder_disconn_handler);
    if (ret) {
        diag_err("diag regist ind no coder ppm chan event fail, ret:0x%x\n", ret);
    }

    return ret;
}

void diag_msg_chan_data_dispatch_cb_regitster(msg_chan_data_dispatch_cb func)
{
    if (g_msg_chan_info.data_dispatch == NULL) {
        g_msg_chan_info.data_dispatch = func;
    } 
}
// 接受C核的消息
int diag_msg_chan_proc(const struct msg_addr *src, void *msg, unsigned int len)
{
    msg_frame_s *msg_frame = NULL;

    if ((src == NULL) || (msg == NULL) || (len < sizeof(msg_frame_s))) {
        diag_err("msglite para invalid, len=0x%x\n", len);
        return -1;
    }

    msg_frame = (msg_frame_s *)msg;

    if (unlikely((src->core != MSG_CORE_TSP) || (src->chnid != MSG_CHN_DIAG))) {
        diag_err("src core or chan id invalid, core=0x%x, chan=0x%x\n", src->core, src->chnid);
        return -1;
    }

    g_diag_dbg.mntn_data.msglite_recv_cnt++;
    if (msg_frame->msg_id == DIAG_MSG_ID_REPORT) {
        g_diag_dbg.mntn_data.msglite_cnf_nocoder_send++;
        (void)ppm_data_send(PPM_CNF_NOCODER_CHAN, msg_frame->data, NULL, len - sizeof(msg_frame_s));
        return 0;
    }

    if (unlikely(g_msg_chan_info.data_dispatch == NULL)) {
        g_diag_dbg.mntn_data.msglite_dispatch_fail++;
        diag_err("diag dispatch cb is null\n");
        return -1;
    }

    g_diag_dbg.mntn_data.msglite_dispatch_cnt++;
    return g_msg_chan_info.data_dispatch(msg_frame, len);
}

int diag_msg_send(unsigned char *data, unsigned int len)
{
    msg_head_s *head = NULL;
    unsigned int total_len = sizeof(msg_head_s) + len;
    int ret;

    head = (msg_head_s *)mdrv_msg_alloc(DRV_MID_DIAG_APSS, total_len, MSG_FULL_LEN);
    if (head == NULL) {
        diag_err("msg malloc fail\n");
        return DIAG_TRANSFERTASK_MALLOC_ERR;
    }

    MDRV_MSG_HEAD_FILL(head, DRV_MID_DIAG_APSS, DRV_MID_DIAG_TSP, len);

    if (memcpy_s(head->data, len, data, len)) {
        diag_err("msg data send memcpy fail\n");
    }

    ret = mdrv_msg_send(head, 0);
    if (ret) {
        diag_err("msg send fail:%d\n", ret);
        mdrv_msg_free(DRV_MID_DIAG_APSS, (void *)head);
    }

    return ret;
}

int diag_msglite_send_to_tsp(unsigned char *data, unsigned int len)
{
    struct msg_addr dst;

    dst.core = MSG_CORE_TSP;
    dst.chnid = MSG_CHN_DIAG;
    g_diag_dbg.mntn_data.msglite_send_cnt++;

    return bsp_msg_lite_sendto(g_msg_chan_info.msg_chan_hdl, &dst, data, len);
}

int diag_msglite_send_to_dpa(unsigned char *data, unsigned int len)
{
    struct msg_addr dst;

    dst.core = MSG_CORE_DPA;
    dst.chnid = MSG_CHN_DIAG;
    g_diag_dbg.mntn_data.msglite_send_cnt++;

    return bsp_msg_sendto_dpa(g_msg_chan_info.msg_chan_hdl, &dst, data, len);
}

int diag_msglite_init(void)
{
    struct msgchn_attr attr = {0};
    bsp_msgchn_attr_init(&attr);

    attr.chnid = MSG_CHN_DIAG;
    attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP);
    attr.lite_notify = diag_msg_chan_proc;

    return bsp_msg_lite_open(&g_msg_chan_info.msg_chan_hdl, &attr);
}

int diag_msg_chan_info_init(void)
{
    g_msg_chan_info.wakelock = wakeup_source_register(NULL, "diagmsg");
    if (g_msg_chan_info.wakelock == NULL) {
        diag_err("diag msg ctrl wakelock register fail\n");
        return -1;
    }

    osl_sem_init(1, &g_msg_chan_info.slicesem);

    INIT_LIST_HEAD(&g_msg_chan_info.slicelist);

    return 0;
}

int diag_chan_mgr_init(void)
{
    int ret;

    ret = diag_msg_chan_info_init();
    if (ret) {
        diag_err("msg chan info init fail:0x%x\n", ret);
        return ret;
    }

    ret = diag_msglite_init();
    if (ret) {
        diag_err("msglite init fail:0x%x\n", ret);
        return ret;
    }

    diag_coder_proc_register(diag_dl_msg_proc);

    ret = diag_coder_chan_init();
    if (ret) {
        diag_err("coder chan init fail:0x%x\n", ret);
        return ret;
    }

    diag_no_coder_proc_register(diag_dl_msg_proc);

    ret = diag_no_coder_chan_init();
    if (ret) {
        diag_err("no coder chan init fail:0x%x\n", ret);
        return ret;
    }

    ret = scm_init();
    if (ret) {
        diag_err("scm init fail:0x%x\n", ret);
        return ret;
    }

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
