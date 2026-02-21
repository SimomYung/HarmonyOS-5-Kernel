/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
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
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS"
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
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <bsp_dump.h>
#include <bsp_slice.h>
#include <bsp_om_enum.h>
#include <bsp_diag.h>
#include <mdrv_diag.h>
#include <securec.h>
#include "vdev.h"

#define THIS_MODU mod_vcom

#define MCI_DUMP_BUFFER_SIZE 0x8000
#define MCI_DUMP_CHAN_SIZE 0x200
#define MCI_DUMP_CHAN_NUM (MCI_DUMP_BUFFER_SIZE / MCI_DUMP_CHAN_SIZE)
#define MCI_DUMP_ITEM_SIZE 0x10
#define MCI_DUMP_ITEM_NUM (MCI_DUMP_CHAN_SIZE / MCI_DUMP_ITEM_SIZE)

#define MCI_HIDS_MSG_ID 0x9f383361

#define HIDS_REPORT_LIST_TH 32
#define HIDS_REPORT_LIST_MAX_NUM 128
#define HIDS_NODE_SIZE (sizeof(struct mci_hids_node))
#define HIDS_ITEM_SIZE (sizeof(struct mci_hids_item))
#define HIDS_BUF_MAX_SIZE (HIDS_REPORT_LIST_TH * HIDS_ITEM_SIZE)

#define HIDS_REPORT_TIMEOUT_MS 1000
#define HIDS_REPORT_TIMEOUT() (jiffies + msecs_to_jiffies(HIDS_REPORT_TIMEOUT_MS))

void mci_item_record(void *dst_buff, unsigned int dst_size, const void *src_buff, unsigned int src_size,
    enum vcom_buff_type type)
{
    int ret;

    memset_s(dst_buff, dst_size, 0, dst_size);
    src_size = src_size > dst_size ? dst_size : src_size;
    if (type == VCOM_USER_TYPE) {
        ret = copy_from_user(dst_buff, src_buff, src_size);
    } else if (type == VCOM_KERNEL_TYPE) {
        ret = memcpy_s(dst_buff, dst_size, src_buff, src_size);
    } else {
        VCOM_PRINT_ERROR("type: %u is error\n", type);
        return;
    }

    if (ret) {
        VCOM_PRINT_ERROR("buffer memcpy fail\n");
        return;
    }
}

void mci_hids_node_enqueue(struct mci_hids_ctx *ctx, struct mci_hids_node *node)
{
    unsigned long flags;

    spin_lock_irqsave(&ctx->report_lock, flags);
    list_add_tail(&node->list, &ctx->report_list);
    ctx->report_num++;
    ctx->buf_alloc_num++;
    spin_unlock_irqrestore(&ctx->report_lock, flags);
}

struct mci_hids_ctx *vcom_get_hids_ctx(void)
{
    return &vcom_get_debug_stax()->hids_ctx;
}

void mci_hids_record(struct vcom_hd *hd, struct mci_record_info *record_info, enum vcom_buff_type type)
{
    struct mci_hids_ctx *ctx = vcom_get_hids_ctx();
    struct mci_hids_node *node = NULL;

    if (ctx->report_buf == NULL || record_info->size == 0) {
        return;
    }

    if (ctx->report_num >= HIDS_REPORT_LIST_MAX_NUM) {
        ctx->record_buf_full++;
        VCOM_PRINT("hids record buf full\n");
        return;
    }

    node = (struct mci_hids_node *)kzalloc(HIDS_NODE_SIZE, GFP_KERNEL);
    if (node == NULL) {
        ctx->buf_alloc_fail++;
        VCOM_PRINT("alloc hids node fail\n");
        return;
    }

    node->item.channel_id = hd->dev_id;
    node->item.length = record_info->size;
    node->item.op_succ = record_info->op_succ;
    node->item.time = record_info->time;
    node->item.pos = record_info->pos;
    node->item.pid = record_info->pid;
    mci_item_record(node->item.msg, sizeof(node->item.msg), record_info->buffer, record_info->size, type);

    mci_hids_node_enqueue(ctx, node);
    if (ctx->report_num >= HIDS_REPORT_LIST_TH) {
        del_timer(&ctx->report_timer);
        queue_delayed_work(ctx->report_work_queue, &ctx->report_work, 0);
        VCOM_PRINT("num %d, schedule work\n", ctx->report_num);
    } else {
        mod_timer(&ctx->report_timer, HIDS_REPORT_TIMEOUT());
    }
}

static void mci_hids_report_work(struct work_struct *work)
{
    int ret;
    struct mci_hids_node *node = NULL;
    struct mci_hids_ctx *ctx = container_of(work, struct mci_hids_ctx, report_work.work);
    unsigned char *hids_buf = ctx->report_buf;
    unsigned int data_len = 0;
    unsigned long flags;

    VCOM_PRINT("hids report %d\n", ctx->report_num);
    while (1) {
        spin_lock_irqsave(&ctx->report_lock, flags);
        node = list_first_entry_or_null(&ctx->report_list, struct mci_hids_node, list);
        if (node == NULL) {
            spin_unlock_irqrestore(&ctx->report_lock, flags);
            break;
        }
        list_del(&node->list);
        ctx->report_num--;
        spin_unlock_irqrestore(&ctx->report_lock, flags);

        ret = memcpy_s(&hids_buf[data_len], HIDS_BUF_MAX_SIZE - data_len, &node->item, HIDS_ITEM_SIZE);
        if (ret) {
            VCOM_PRINT_ERROR("memset fail\n");
        }
        data_len += HIDS_ITEM_SIZE;

        /* 批量上报 */
        if (data_len >= HIDS_BUF_MAX_SIZE) {
            ret = bsp_diag_trans_report(MCI_HIDS_MSG_ID, DIAG_DRV_HDS_PID, hids_buf, data_len);
            if (ret != BSP_OK) {
                ctx->report_fail++;
                VCOM_PRINT("report fail %d\n", ret);
            } else {
                ctx->report_succ++;
            }
            data_len = 0;
        }

        kfree(node);
        ctx->buf_free_num++;
    }

    if (data_len > 0) {
        ret = bsp_diag_trans_report(MCI_HIDS_MSG_ID, DIAG_DRV_HDS_PID, hids_buf, data_len);
        if (ret != BSP_OK) {
            ctx->report_fail++;
            VCOM_PRINT("report fail %d\n", ret);
        } else {
            ctx->report_succ++;
        }
    }

    ret = memset_s(hids_buf, HIDS_BUF_MAX_SIZE, 0, HIDS_BUF_MAX_SIZE);
    if (ret) {
        VCOM_PRINT_ERROR("memset fail\n");
    }
}

static void mci_report_timer_handle(struct timer_list *t)
{
    struct mci_hids_ctx *ctx = container_of(t, struct mci_hids_ctx, report_timer);

    ctx->timeout_num++;
    VCOM_PRINT("time out\n");

    queue_delayed_work(ctx->report_work_queue, &ctx->report_work, 0);
}

int mci_hids_init(struct mci_hids_ctx *ctx)
{
    INIT_LIST_HEAD(&ctx->report_list);
    spin_lock_init(&ctx->report_lock);

    INIT_DELAYED_WORK(&ctx->report_work, mci_hids_report_work);
    ctx->report_work_queue = create_singlethread_workqueue("mci hids report");
    if (ctx->report_work_queue == NULL) {
        VCOM_PRINT_ERROR("creat workqueue failed\n");
        return -1;
    }

    ctx->report_buf = (unsigned char *)kzalloc(HIDS_BUF_MAX_SIZE, GFP_KERNEL);
    if (ctx->report_buf == NULL) {
        VCOM_PRINT_ERROR("alloc hids report buf fail\n");
        return -1;
    }

    /* init timer */
    timer_setup(&ctx->report_timer, mci_report_timer_handle, 0);

    VCOM_PRINT_ERROR("hids init ok\n");
    return 0;
}

void mci_dump_record(struct vcom_hd *hd, struct mci_record_info *record_info, enum vcom_buff_type type)
{
    struct vcom_debug_stax *debug_ctx = vcom_get_debug_stax();
    unsigned char *channel_buffer = NULL;
    struct vcom_dump_item *item = NULL;
    unsigned int dump_index;
    unsigned long flags;

    if (debug_ctx->dump_ctx.dump_buffer == NULL || hd->dev_id >= MCI_DUMP_CHAN_NUM || hd->dump_enable == 0) {
        return;
    }

    spin_lock_irqsave(&debug_ctx->dump_ctx.dump_lock, flags);
    dump_index = hd->dump_index;
    hd->dump_index = (hd->dump_index + 1) % MCI_DUMP_ITEM_NUM;
    spin_unlock_irqrestore(&debug_ctx->dump_ctx.dump_lock, flags);

    channel_buffer = (unsigned char *)((uintptr_t)debug_ctx->dump_ctx.dump_buffer + hd->dev_id * MCI_DUMP_CHAN_SIZE);
    item = (struct vcom_dump_item *)(channel_buffer + dump_index * MCI_DUMP_ITEM_SIZE);
    item->length = record_info->size;
    item->time = record_info->time;
    item->op_succ = record_info->op_succ;
    item->pos = record_info->pos;
    item->pid = record_info->pid;
    mci_item_record(item->msg, sizeof(item->msg), record_info->buffer, record_info->size, type);
}

void mci_debug_record(struct vcom_hd *hd, enum vcom_buff_type type, struct mci_record_info *record_info)
{
    record_info->time = bsp_get_slice_value();
    mci_dump_record(hd, record_info, type);
    mci_hids_record(hd, record_info, type);
}

void mci_dump_init(struct vcom_debug_stax *debug_ctx)
{
    spin_lock_init(&debug_ctx->dump_ctx.dump_lock);
    debug_ctx->dump_ctx.dump_buffer = bsp_dump_register_field(DUMP_MODEMAP_VCOM, "MCI", MCI_DUMP_BUFFER_SIZE, 0);
    if (debug_ctx->dump_ctx.dump_buffer == NULL) {
        VCOM_PRINT_ERROR("dump buffer alloc fail\n");
    }
}

int mci_debug_init(void)
{
    struct vcom_debug_stax *debug_ctx = vcom_get_debug_stax();

    mci_dump_init(debug_ctx);
    return mci_hids_init(&debug_ctx->hids_ctx);
}

void mci_get_hids_stats(void)
{
    struct mci_hids_ctx *ctx = vcom_get_hids_ctx();

    bsp_err("report_fail     %d\n", ctx->report_fail);
    bsp_err("report_succ     %d\n", ctx->report_succ);
    bsp_err("record_buf_full %d\n", ctx->record_buf_full);
    bsp_err("list num        %d\n", ctx->report_num);
    bsp_err("buf alloc fail  %d\n", ctx->buf_alloc_fail);
    bsp_err("buf alloc num   %d\n", ctx->buf_alloc_num);
    bsp_err("buf free num    %d\n", ctx->buf_free_num);
    bsp_err("timeout_num     %d\n", ctx->timeout_num);
}
