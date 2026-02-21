/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2025. All rights reserved.
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

#include <product_config.h>
#include <securec.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <bsp_slice.h>
#include <bsp_print.h>
#include <osl_malloc.h>
#include <dump_frame.h>
#include "dump_genl.h"

struct genl_ops g_dump_genl_ops[] = {
    {
        .cmd = DUMP_GENL_CMD_UPLOAD,
        .doit = dump_genl_rcv_handler,
    },
};

dump_genl_ctrl_s g_dump_ctrl_info;

void dump_genl_set_port_info(struct genl_info *info)
{
    g_dump_ctrl_info.con_info.net = genl_info_net(info);
    g_dump_ctrl_info.con_info.port_id = info->snd_portid;
}

int dump_genl_rcv_para_check(struct sk_buff *skb, struct genl_info *info)
{
    struct nlmsghdr *nlhdr = NULL;

    if (skb == NULL || info == NULL) {
        dump_error("parameter null\n");
        return -1;
    }

    nlhdr = nlmsg_hdr(skb);
    if (nlhdr == NULL) {
        dump_error("nlhdr is null\n");
        return -1;
    }

    if ((skb->len < NLMSG_LENGTH(GENL_HDRLEN)) || (skb->len < nlhdr->nlmsg_len)) {
        dump_error("skb_len invalid, skb_len=0x%x\n", skb->len);
        return -1;
    }

    return 0;
}

static int dump_genl_send_fragment_data(dump_genl_data_desc_s *data_desc)
{
    dump_genl_ctrl_s *ctrl_info = &g_dump_ctrl_info;
    struct sk_buff *genl_skb = NULL;
    char *payload_hdr = NULL;
    dump_genl_unicast_msg_s *unicast_msg = NULL;
    int ret;
    unsigned int payload_len = NLMSG_ALIGN(sizeof(dump_genl_unicast_msg_s) + data_desc->blk_len);

    if (ctrl_info->con_info.net == NULL) {
        dump_error("dump genl chan net is null\n");
        return DUMP_ERR_PORT_NULL;
    }

    genl_skb = genlmsg_new(payload_len, GFP_KERNEL);
    if (genl_skb == NULL) {
        dump_error("nlmsg_new fail\n");
        return DUMP_ERR_MSG_NEW_FAIL;
    }

    payload_hdr = genlmsg_put(genl_skb, ctrl_info->con_info.port_id, 0, &ctrl_info->family, 0, DUMP_GENL_CMD_UPLOAD);
    if (payload_hdr == NULL) {
        ret = DUMP_ERR_MSG_PUT_FAIL;
        goto err;
    }

    unicast_msg = (dump_genl_unicast_msg_s *)payload_hdr;
    unicast_msg->data_hdr.kernel_msg_type = data_desc->msg_type;
    unicast_msg->data_hdr.op = data_desc->op;
    unicast_msg->data_hdr.data_len = DUMP_GENL_UNICAST_MSG_ARGS_SIZE(data_pkt) + data_desc->blk_len;
    unicast_msg->data_hdr.seq_id = data_desc->msg_seq;
    unicast_msg->data_pkt.blk_num = data_desc->blk_num;
    unicast_msg->data_pkt.blk_seq = data_desc->blk_seq;
    unicast_msg->data_pkt.blk_len = data_desc->blk_len;
    unicast_msg->data_pkt.file_id = data_desc->file_id;
    unicast_msg->data_pkt.file_offset = data_desc->file_offset;
    unicast_msg->data_pkt.start_magic = DUMP_UPLOAD_PKT_START_MAGIC;
    unicast_msg->data_pkt.end_magic = DUMP_UPLOAD_PKT_END_MAGIC;

    ret = memcpy_s(unicast_msg->data, unicast_msg->data_pkt.blk_len, data_desc->data, data_desc->blk_len);
    if (ret) {
        dump_error("memcpy_s unicast send data fail, ret=0x%x\n", ret);
        goto err;
    }

    skb_put(genl_skb, payload_len);
    genlmsg_end(genl_skb, payload_hdr);

    ret = genlmsg_unicast(ctrl_info->con_info.net, genl_skb, ctrl_info->con_info.port_id);
    if (ret < 0) {
        dump_debug("genl unicast fail, ret=%d\n", ret);
        return ret;
    }
    return 0;

err:
    dev_kfree_skb_any(genl_skb);
    return ret;
}

static int dump_genl_send_fragment_data_timeout(dump_genl_data_desc_s *data_desc, u32 start_slice, u32 timeout_ms)
{
    int ret;
    u32 freq = bsp_get_slice_freq();
    u32 timeout_slice = (u32)((u64)((u64)timeout_ms * freq) / 1000 + 1); /* 1000表示1ms */
    u32 cur_slice = bsp_get_slice_value();
    u32 delta_slice = (cur_slice >= start_slice) ? (cur_slice - start_slice) : (0xFFFFFFFF - start_slice + cur_slice);

    ret = dump_genl_send_fragment_data(data_desc);
    if (timeout_ms == 0) {
        return ret;
    }
    while ((ret == -EAGAIN) && (delta_slice < timeout_slice)) {
        msleep(DUMP_EAGAIN_RETRY_MSLEEP_ONCE);
        ret = dump_genl_send_fragment_data(data_desc);
        cur_slice = bsp_get_slice_value();
        delta_slice = (cur_slice >= start_slice) ? (cur_slice - start_slice) : (0xFFFFFFFF - start_slice + cur_slice);
    }
    return ret;
}

int dump_genl_send_data_advance(u8 *buf, u32 len, dump_genl_pkt_ctrl_s *adv_ctrl)
{
    dump_genl_data_desc_s data_desc;
    unsigned int block_num;
    unsigned int tail_size;
    unsigned int i;
    int ret = 0;
    u32 send_len = 0;
    u32 start_slice;

    if (buf == NULL || adv_ctrl == NULL || len == 0) {
        return -1;
    }


    block_num = len / DUMP_GENL_DATA_SIZE;
    tail_size = len % DUMP_GENL_DATA_SIZE;

    (void)memset_s(&data_desc, sizeof(dump_genl_data_desc_s), 0, sizeof(data_desc));

    data_desc.msg_type = adv_ctrl->msg_type;
    data_desc.op = adv_ctrl->op;
    data_desc.file_id = adv_ctrl->file_id;
    data_desc.file_offset = adv_ctrl->file_offset;
    data_desc.blk_num = block_num;
    data_desc.data = buf;
    data_desc.blk_len = DUMP_GENL_DATA_SIZE;

    osl_sem_down(&g_dump_ctrl_info.con_info.chan_sem);
    start_slice = bsp_get_slice_value();
    for (i = 0; i < block_num; i++) {
        ret = dump_genl_send_fragment_data_timeout(&data_desc, start_slice, adv_ctrl->timeout_ms);
        if (ret != 0) {
            goto out;
        }
        send_len += DUMP_GENL_DATA_SIZE;
        data_desc.data += DUMP_GENL_DATA_SIZE;
        data_desc.blk_seq++;
    }

    if (tail_size) {
        data_desc.blk_len = tail_size;
        ret = dump_genl_send_fragment_data_timeout(&data_desc, start_slice, adv_ctrl->timeout_ms);
        if (ret != 0) {
            goto out;
        }
        send_len += tail_size;
    }

out:
    osl_sem_up(&g_dump_ctrl_info.con_info.chan_sem);
    return (send_len == 0) ? ret : send_len;
}

__init int dump_genl_init(void)
{
    int ret = BSP_OK;

    dump_error("dump genl init start\n");

    g_dump_ctrl_info.family.hdrsize = 0;
    g_dump_ctrl_info.family.version = DUMP_GENL_VERSION;
    g_dump_ctrl_info.family.ops = g_dump_genl_ops;
    g_dump_ctrl_info.family.n_ops = ARRAY_SIZE(g_dump_genl_ops);
    ret = strncpy_s(g_dump_ctrl_info.family.name, sizeof(g_dump_ctrl_info.family.name), DUMP_GENL_NAME,
                    sizeof(DUMP_GENL_NAME));
    if (ret) {
        dump_error("strcpy genl name failed, ret=0x%x\n", ret);
        return ret;
    }

    ret = genl_register_family(&g_dump_ctrl_info.family);
    if (ret) {
        dump_error("genl_register_family failed, ret=0x%x\n", ret);
    }

    osl_sem_init(1, &g_dump_ctrl_info.con_info.chan_sem);

    dump_error("dump genl init succ\n");
    return ret;
}