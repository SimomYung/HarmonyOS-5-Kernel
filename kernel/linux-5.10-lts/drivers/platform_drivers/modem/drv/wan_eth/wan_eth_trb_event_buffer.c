/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2020. All rights reserved.
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

#include <linux/netdevice.h>
#include <linux/jiffies.h>
#include <linux/version.h>

#include "product_config.h"
#include "wan_eth_trb_event_buffer.h"
#include "wan_eth_common.h"
#include "bsp_pcie.h"
#include "bsp_dump.h"
#include "mdrv_errno.h"

#define WETH_EVT_TYPE_LEN   4
#define WETH_EVT_CHN_ID_LEN 4
#define WETH_EVT_DEV_ID_LEN 4
#define WETH_EVT_PARA_LEN   20
#define WETH_MASK(n) (((1) << (n)) - 1)

struct event_buff_ctx g_weth_event_ctx[WETH_DIR_LAST];

struct event_buff_ctx *weth_get_event_buff_ctx(enum weth_dir dir)
{
    return &g_weth_event_ctx[dir];
}

int weth_trb_process_send_event(struct event_buff_ctx *ectx, unsigned int event_type,
                                int send_irq, unsigned int rw_ptr);

STATIC void weth_trb_event_process(struct event_buff_ctx *ectx)
{
    union write_read_pointer wr_rd_p = ectx->rx_event->wr_rd_p;
    unsigned int rx_wp = wr_rd_p.bits.wp;
    unsigned int rx_rp = wr_rd_p.bits.rp;
    unsigned int rx_num;
    union event_element rx_event;
    unsigned int event_type;

    if ((rx_wp >= ectx->event_buff_size) || rx_rp >= ectx->event_buff_size) {
        ectx->rx_stats.invalid_event++;
        return;
    }

    ectx->rx_stats.handle_event_start++;
    rx_num = weth_rw_get_busy(rx_wp, rx_rp, ectx->event_buff_size);

    while (rx_num > 0) {
        rx_event = ectx->rx_event->event[rx_rp];
        event_type = (unsigned int)rx_event.bits.event_type;
        if (event_type >= WETH_EVENT_TYPE_NUM || ectx->event_cb[event_type] == NULL) {
            ectx->rx_stats.invalid_event++;
            rx_rp = weth_rw_get_pos(rx_rp + 1, ectx->event_buff_size);
            rx_num--;
            weth_pr_err("evt %d not support\n", event_type);
            continue;
        }
        ectx->rx_stats.event_count[event_type]++;
        ectx->event_cb[event_type]((unsigned int)rx_event.bits.dev_id, (unsigned int)rx_event.bits.rw_ptr);

        rx_rp = weth_rw_get_pos(rx_rp + 1, ectx->event_buff_size);
        rx_num--;
        ectx->rx_stats.event_handled++;
    }

    ectx->rx_event->wr_rd_p.bits.rp = rx_rp;
    /* read for order */
    rx_rp = ectx->rx_event->wr_rd_p.bits.rp;

    return;
}

STATIC irqreturn_t weth_trb_event_handler(int index, void *para)
{
    int i;

    for (i = 0; i < WETH_DIR_LAST; i++) {
        weth_trb_event_process(&g_weth_event_ctx[i]);
    }

    return IRQ_HANDLED;
}

int weth_trb_send_event(enum weth_dir dir, unsigned int event_type, int send_irq, unsigned int rw_ptr)
{
    struct event_buff_ctx *ectx = &g_weth_event_ctx[dir];
    struct common_info *comm_info = &g_common_info;
    int ret;

    atomic_add(1, &comm_info->bar_access);
    if (unlikely(!comm_info->link_status)) {
        if (comm_info->init_finish) {
            comm_info->link_down_tx++;
            atomic_sub(1, &comm_info->bar_access);
            return -1;
        }
    }

    ret = weth_trb_process_send_event(ectx, event_type, send_irq, rw_ptr);
    atomic_sub(1, &comm_info->bar_access);

    return ret;
}

STATIC void weth_send_irq(struct event_buff_ctx *ectx)
{
#ifdef CONFIG_WETH_DEV
    bsp_pcie_ep_send_msi(PCIE_EP_MSI_ETH_DEV);
#endif

#ifdef CONFIG_WETH
    bsp_pcie_rc_send_msi(PCIE_RC_MSI_ETH_DEV);
#endif

    return;
}

int weth_trb_process_send_event(struct event_buff_ctx *ectx, unsigned int event_type,
                            int send_irq, unsigned int rw_ptr)
{
    union write_read_pointer wr_rd_p;
    unsigned int tx_wp;
    unsigned int tx_rp;
    unsigned long flags;
    union event_element evt = { 0 };

    ectx->tx_stats.send_event++;

    spin_lock_irqsave(&ectx->tx_lock, flags);
    wr_rd_p = ectx->tx_event->wr_rd_p;
    tx_wp = wr_rd_p.bits.wp;
    tx_rp = wr_rd_p.bits.rp;

    if (tx_wp >= ectx->event_buff_size || tx_wp < 0) {
        spin_unlock_irqrestore(&ectx->tx_lock, flags);
        weth_pr_err("tx_wp %d err\n", tx_wp);
        ectx->tx_stats.invalid_tx_wp++;
        return -1;
    }

    if (weth_rw_get_idle(tx_wp, tx_rp, ectx->event_buff_size) <= 0) {
        spin_unlock_irqrestore(&ectx->tx_lock, flags);
        weth_pr_err("event_buffer full, w:%d, r:%d, size:%d, type:%d\n", tx_wp, tx_rp,
                    ectx->event_buff_size,(int)event_type);
        ectx->tx_stats.event_buffer_full++;
        return -1;
    }

    if (weth_rw_get_idle(tx_wp, tx_rp, ectx->event_buff_size) <= 1) {
        weth_pr_err("event_buffer full\n");
        ectx->tx_stats.event_buffer_nearly_full++;
        send_irq = 1;
    }

    evt.bits.event_type = event_type & WETH_MASK(WETH_EVT_TYPE_LEN);
    evt.bits.rw_ptr = rw_ptr & WETH_MASK(WETH_EVT_PARA_LEN);
    ectx->tx_event->event[tx_wp].bit32 = evt.bit32;

    ectx->tx_event->wr_rd_p.bits.wp = weth_rw_get_pos(tx_wp + 1, ectx->event_buff_size);
    ectx->tx_stats.event_count[event_type]++;
    spin_unlock_irqrestore(&ectx->tx_lock, flags);
    weth_pr_dbg("%s w:%d r:%d evt 0x%x\n", ectx == &g_weth_event_ctx[0]? "dl" : "ul", tx_wp, tx_rp, ectx->tx_event->event[tx_wp].bit32);
    if (send_irq) {
        /* read for order */
        evt.bit32 = ectx->tx_event->event[tx_wp].bit32;
        tx_wp = ectx->tx_event->wr_rd_p.bits.wp;
        tx_rp = ectx->tx_event->wr_rd_p.bits.rp;

        weth_send_irq(ectx);
        ectx->tx_stats.send_irq++;
    }

    return 0;
}

void weth_trb_event_buff_set_cb(enum weth_dir dir, weth_event_cb_t ops[])
{
    struct event_buff_ctx *ectx = &g_weth_event_ctx[dir];
    int i;

    for (i = 0; i < WETH_EVENT_TYPE_NUM; i++) {
        ectx->event_cb[i] = ops[i];
    }
}

STATIC void weth_trb_event_buff_msi_init(struct event_buff_ctx *ectx)
{
#ifdef CONFIG_WETH_DEV
    bsp_pcie_ep_msi_request(PCIE_RC_MSI_ETH_DEV, weth_trb_event_handler, "weth_hal_ep_irq", ectx);
#endif

#ifdef CONFIG_WETH
    bsp_pcie_rc_msi_request(PCIE_EP_MSI_ETH_DEV, weth_trb_event_handler, "weth_hal_rc_irq", ectx);
#endif

    return;
}

STATIC void weth_event_buff_msi_deinit(struct event_buff_ctx *ectx)
{
#ifdef CONFIG_WETH_DEV
    bsp_pcie_ep_msi_free(PCIE_RC_MSI_ETH_DEV);
#endif

#ifdef CONFIG_WETH
    bsp_pcie_rc_msi_free(PCIE_EP_MSI_ETH_DEV);
#endif

    return;
}

int weth_event_get_dir_status(enum weth_dir dir)
{
    unsigned int tx_wp;
    unsigned int tx_rp;
    unsigned long flags;
    struct event_buff_ctx *ectx = NULL;

    ectx = &g_weth_event_ctx[dir];
    ectx->tx_stats.send_event++;
    spin_lock_irqsave(&ectx->tx_lock, flags);
    tx_wp = ectx->tx_event->wr_rd_p.bits.wp;
    tx_rp = ectx->tx_event->wr_rd_p.bits.rp;
    spin_unlock_irqrestore(&ectx->tx_lock, flags);

    return weth_rw_get_busy(tx_wp, tx_rp, ectx->event_buff_size);
}

int weth_event_get_status(void)
{
    int i;
    unsigned int num = 0;

    for (i = 0; i < WETH_DIR_LAST; i++) {
        num += weth_event_get_dir_status(i);
    }

    return num;
}

void weth_trb_event_buff_reinit(enum weth_dir dir, void *tx_event, void *rx_event, unsigned int evt_max_num)
{
    struct event_buff_ctx *ectx = &g_weth_event_ctx[dir];
    ectx->tx_event = (struct event_buffer *)tx_event;
    ectx->rx_event = (struct event_buffer *)rx_event;
    ectx->handle_event_buff_size = evt_max_num;
    weth_trb_event_buff_msi_init(ectx);
}

void weth_event_buff_exit(void)
{
    int i;

    for (i = 0; i < WETH_EVENT_TYPE_NUM; i++) {
        weth_event_buff_msi_deinit(&g_weth_event_ctx[i]);
    }
}

int weth_trb_event_buff_init(enum weth_dir dir, enum weth_pcie_mode work_mode, void *tx_event, void *rx_event, unsigned int evt_max_num)
{
    struct event_buff_ctx *ectx = &g_weth_event_ctx[dir];

    ectx->work_mode = work_mode;
    ectx->event_buff_size = evt_max_num;
    ectx->handle_event_buff_size = evt_max_num;
    ectx->pcpu_packets_thru = MAX_QLEN_PCPU;
    spin_lock_init(&ectx->tx_lock);
    spin_lock_init(&ectx->rx_lock);
    ectx->tx_event = (struct event_buffer *)tx_event;
    ectx->rx_event = (struct event_buffer *)rx_event;

    if (dir == WETH_DIR_DL) {
        weth_trb_event_buff_msi_init(ectx);
    }

    weth_pr_err("event buffer init ok\n");
    return 0;
}

#ifdef CONFIG_WETH_DEBUG

void weth_trb_debug_event(enum weth_dir dir, unsigned int num)
{
    unsigned int i;
    unsigned int wp;
    unsigned int rp;
    struct event_buff_ctx *ectx = NULL;

    if (dir == WETH_DIR_DL) {
        weth_pr_print("dl evt buffer\n");
    } else {
        dir = WETH_DIR_UL;
        weth_pr_print("ul evt buffer\n");
    }

    if (num >= ectx->event_buff_size) {
        weth_pr_print("evt num %d overflow\n", num);
        return;
    }

    ectx = &g_weth_event_ctx[dir];
    wp = ectx->tx_event->wr_rd_p.bits.wp;
    rp = ectx->tx_event->wr_rd_p.bits.rp;
    weth_pr_print("tx evt w %d r %d\n", wp, rp);
    for (i = 0; i < num; i++) {
        weth_pr_print("%d evt 0x%x rw_ptr %d\n", i, ectx->tx_event->event[i].bit32, ectx->tx_event->event[i].bits.rw_ptr);
    }
    weth_pr_print("\n");

    wp = ectx->rx_event->wr_rd_p.bits.wp;
    rp = ectx->rx_event->wr_rd_p.bits.rp;

    weth_pr_print("rx evt w %d r %d\n", wp, rp);
    weth_pr_print("handle_event_start:    %d\n", ectx->rx_stats.handle_event_start);
    weth_pr_print("invalid_event:         %d\n", ectx->rx_stats.invalid_event);
    weth_pr_print("invalid_index:         %d\n", ectx->rx_stats.invalid_index);
    weth_pr_print("event_handled:         %d\n", ectx->rx_stats.event_handled);
    weth_pr_print("link_up:               %d\n", ectx->rx_stats.event_count[WETH_LINK_UP]);
    weth_pr_print("link_down:             %d\n", ectx->rx_stats.event_count[WETH_LINK_DOWN]);
    weth_pr_print("rc_rx_dma_complete:    %d\n", ectx->rx_stats.event_count[WETH_RC_RX_DMA_COMPLETE]);
    for (i = 0; i < num; i++) {
        weth_pr_print("%d evt 0x%x rw_ptr %d\n", i, ectx->rx_event->event[i].bit32, ectx->rx_event->event[i].bits.rw_ptr);
    }
    weth_pr_print("\n");
}
#endif
