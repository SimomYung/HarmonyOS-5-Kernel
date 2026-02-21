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
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <uapi/linux/if_ether.h>
#include <linux/ktime.h>
#include <linux/if_vlan.h>
#include "securec.h"
#include "bsp_pcie.h"
#include "mdrv_remote_eth.h"
#include "wan_eth_trb_table_rc.h"
#include "wan_eth_common.h"
#include "wan_eth_trb_event_buffer.h"
#include "bsp_dump.h"
#include "mdrv_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* _cplusplus */

#define WETH_MUX_NUM 30
#define INIT_SKB_TIMEOUT 100

#define WETH_TRB_GET_DEV_ID(trb) ((trb)->ctrl)

struct weth_trb_desc_queue {
    struct weth_trb_desc *desc_virt; // trb描述符虚拟地址
    struct sk_buff **local_skb;
    dma_addr_t desc_phy; // trb描述符物理地址
    unsigned int size;
    unsigned int last_wptr; // 上次通知ep侧搬trb的坐标
    unsigned int wptr;
    unsigned int rptr;
    spinlock_t lock;
};

struct weth_trb_desc_cache {
    struct weth_trb_desc *trb;
    struct sk_buff **skb;
};

struct weth_trb_tab_stats {
    unsigned long long rx_packets;
    unsigned long long rx_bytes;
};

struct weth_trb_tab_ctx {
    struct device *dev_for_map;

    struct weth_trb_desc_queue tx_desc;
    spinlock_t tx_lock;

    struct weth_trb_desc_queue rx_desc;
    struct weth_trb_desc_cache rx_desc_cache; // 下行接收数据trb临时缓存，每次从头开始用
    struct weth_trb_desc *rx_proc_trb_cache; // 补充trb临时缓存，每次从头开始用
    spinlock_t rx_lock;

    weth_trb_tab_rx_cb_t rx_cb;
    weth_trb_tab_table_sync_finish_cb_t table_sync_finish_cb;
    struct workqueue_struct *refill_queue;
    struct delayed_work refill_work;
    struct tasklet_struct refill_push;

    unsigned int rx_data_num_err;
    unsigned int rx_trb_cache_err;
    unsigned int rx_skb_null;
    unsigned int rx_trb_null;
    unsigned int rx_data_err;
    unsigned int skb_tail_null;
    unsigned int rx_set_desc_fail;

    unsigned int refill_q_full;
    unsigned int alloc_skb_fail;
    unsigned int txq_full;
    unsigned int txq_addr_null;
    unsigned int txq_rptr_overflow;
    unsigned int txq_localskb_null;

    struct weth_trb_tab_stats stats;
};

struct weth_trb_tab_ctx g_weth_trb_tab_ctx;

struct weth_trb_tab_ctx *weth_get_trb_tab_ctx(void)
{
    return &g_weth_trb_tab_ctx;
}

void weth_trb_tab_free(struct weth_trb_tab_ctx *tab_ctx)
{
    dma_free_coherent(tab_ctx->dev_for_map, tab_ctx->tx_desc.size * sizeof(struct weth_trb_desc),
        tab_ctx->tx_desc.desc_virt, tab_ctx->tx_desc.desc_phy);

    dma_free_coherent(tab_ctx->dev_for_map, tab_ctx->rx_desc.size * sizeof(struct weth_trb_desc),
        tab_ctx->rx_desc.desc_virt, tab_ctx->rx_desc.desc_phy);

    if (tab_ctx->refill_queue != NULL) {
        destroy_workqueue(tab_ctx->refill_queue);
    }
}

int weth_trb_tab_get_tx_desc_space(struct weth_trb_desc_queue *tx_q, unsigned int tx_num, unsigned int *free_num,
    unsigned int *wptr, unsigned int *wptr_next)
{
    unsigned int ava_num = 0;
    unsigned long flags;

    spin_lock_irqsave(&tx_q->lock, flags);
    ava_num = weth_rw_get_idle(tx_q->wptr, tx_q->rptr, tx_q->size);
    if (ava_num == 0) {
        *free_num = 0;
        spin_unlock_irqrestore(&tx_q->lock, flags);
        return -ENOMEM;
    }
    *wptr = tx_q->wptr;

    if (ava_num < tx_num) {
        *free_num = ava_num;
    } else {
        *free_num = tx_num;
    }

    tx_q->wptr = weth_rw_get_pos(tx_q->wptr + *free_num, tx_q->size);

    *wptr_next = tx_q->wptr;
    spin_unlock_irqrestore(&tx_q->lock, flags);

    return 0;
}

STATIC inline unsigned int weth_trb_get_devid_by_skb(struct sk_buff *skb)
{
    struct remote_eth_cb_map_s *map_info;

    map_info = (struct remote_eth_cb_map_s *)skb->cb;
    return (map_info->userfield0 & 0xFFFF);
}

int weth_trb_tab_set_desc(struct weth_trb_desc_queue *desc_q, struct sk_buff *skb[], struct weth_trb_desc desc[],
                          unsigned int num, unsigned int wptr)
{
    int ret;
    unsigned int remain;

    if (wptr + num <= desc_q->size) {
        /* 环形到边空间足够存储，不需要翻转 */
        ret = memcpy_s(&desc_q->local_skb[wptr], sizeof(struct sk_buff *) * num, skb, sizeof(struct sk_buff *) * num);
        if (ret) {
            weth_pr_err("%d memcpy fail ret %d wptr %d num %d\n", __LINE__, ret, wptr, num);
            return ret;
        }

        ret = memcpy_s(&desc_q->desc_virt[wptr], sizeof(struct weth_trb_desc) * num, desc, sizeof(struct weth_trb_desc) * num);
        if (ret) {
            weth_pr_err("%d memcpy fail ret %d wptr %d num %d\n", __LINE__, ret, wptr, num);
            return ret;
        }

        return 0;
    }

    remain = desc_q->size - wptr;
    ret = memcpy_s(&desc_q->local_skb[wptr], sizeof(struct sk_buff *) * remain, skb, sizeof(struct sk_buff *) * remain);
    if (ret) {
        weth_pr_err("%d memcpy fail ret %d wptr %d num %d\n", __LINE__, ret, wptr, remain);
        return ret;
    }

    ret = memcpy_s(&desc_q->desc_virt[wptr], sizeof(struct weth_trb_desc) * remain, desc, sizeof(struct weth_trb_desc) * remain);
    if (ret) {
        weth_pr_err("%d memcpy fail ret %d wptr %d num %d\n", __LINE__, ret, wptr, remain);
        return ret;
    }

    num -= remain;
    ret = memcpy_s(&desc_q->local_skb[0], sizeof(struct sk_buff *) * num, skb + remain, sizeof(struct sk_buff *) * num);
    if (ret) {
        weth_pr_err("%d memcpy fail ret %d wptr %d num %d\n", __LINE__, ret, wptr, num);
        return ret;
    }
    ret = memcpy_s(&desc_q->desc_virt[0], sizeof(struct weth_trb_desc) * num, desc + remain, sizeof(struct weth_trb_desc) * num);
    if (ret) {
        weth_pr_err("%d memcpy fail ret %d wptr %d num %d\n", __LINE__, ret, wptr, num);
        return ret;
    }
    weth_pr_dbg("tx_q bot_num %u top_num %u\n", remain, num);
    return 0;
}

int weth_trb_tab_set_tx_desc(struct sk_buff *skb[], struct weth_trb_desc desc[], unsigned int num, unsigned int wptr)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;

    return weth_trb_tab_set_desc(&(tab_ctx->tx_desc), skb, desc, num, wptr);
}

int weth_trb_tab_set_rx_desc(struct sk_buff *skb[], struct weth_trb_desc desc[], unsigned int num)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;

    return weth_trb_tab_set_desc(&(tab_ctx->rx_desc), skb, desc, num, tab_ctx->rx_desc.wptr);
}

#define WETH_SEND_NUM 10

void weth_trb_set_tx_trb(struct weth_trb_desc *trb, struct sk_buff *skb, unsigned long long addr)
{
    trb->addr = addr;
    trb->data_len = skb->len;
    WETH_TRB_GET_DEV_ID(trb) = weth_trb_get_devid_by_skb(skb);

    trb->pkt_info.ul_bits.ip_proto = ((struct ethhdr *)skb->data)->h_proto;
    trb->pkt_info.ul_bits.vlan_proto = ((struct vlan_ethhdr *)(skb->data))->h_vlan_proto;

    weth_pr_dbg("data addr 0x%llx len %d\n", trb->addr, trb->data_len);
    weth_pr_pkg(skb->data, MAX_PRINT_PKT_LEN);
}

int weth_trb_tab_save_tx_desc(struct sk_buff *skb, unsigned int tot_num, unsigned int *txq_wptr)
{
    int ret;
    unsigned long long addr;
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    struct weth_trb_desc tmp_trb[WETH_SEND_NUM];
    struct sk_buff *tmp_skb[WETH_SEND_NUM];
    unsigned int queue_num;
    unsigned int send_cnt = 0;
    unsigned int wptr;
    unsigned int cnt = 0;

    ret = weth_trb_tab_get_tx_desc_space(&tab_ctx->tx_desc, tot_num, &queue_num, &wptr, txq_wptr);
    if (ret) {
        __weth_free_skb_list_by_null(skb);
        tab_ctx->txq_full++;
        return -ENOMEM;
    }

    while (skb != NULL && cnt < queue_num) {
        addr = dma_map_single(tab_ctx->dev_for_map, skb->data, skb->len, DMA_TO_DEVICE);
        if (addr == 0) {
            __weth_free_skb(skb);
            tab_ctx->txq_addr_null++;
            continue;
        }
        tmp_skb[send_cnt] = skb;

        weth_trb_set_tx_trb(&tmp_trb[send_cnt], skb, addr);
        send_cnt++;
        if (send_cnt == WETH_SEND_NUM) {
            ret = weth_trb_tab_set_tx_desc(tmp_skb, tmp_trb, send_cnt, wptr);
            send_cnt = 0;
            wptr += WETH_SEND_NUM;
            if (wptr >= tab_ctx->tx_desc.size) {
                wptr -= tab_ctx->tx_desc.size;
            }
        }
        skb = skb->next;
        cnt++;
    }

    if (send_cnt > 0) {
        weth_trb_tab_set_tx_desc(tmp_skb, tmp_trb, send_cnt, wptr);
    }

    if (queue_num < tot_num) {
        __weth_free_skb_list_by_null(skb);
    }

    return ret;
}

int weth_trb_tab_init_skb_base(struct device *dev, struct sk_buff **skb_buf, dma_addr_t *data_phy)
{
    struct sk_buff *skb = NULL;

    /* alloc skb */
    skb = __weth_alloc_skb(WETH_SKB_MTU);
    if (skb == NULL) {
        return -1;
    }

    *(unsigned int*)skb->data = 0x5a5a5a5a;

    /* invalid skb data cache */
    *data_phy = dma_map_single(dev, skb->data, WETH_SKB_MTU, DMA_FROM_DEVICE);
    *skb_buf = skb;
    return 0;
}

int weth_trb_alloc_rx_desc(struct weth_trb_tab_ctx *tab_ctx)
{
    int ret;
    unsigned int cnt = 0;
    unsigned int tot_num;
    struct weth_trb_desc_queue *rx_q = &tab_ctx->rx_desc;
    struct weth_trb_desc_cache *desc_buf = &tab_ctx->rx_desc_cache;

    tot_num = weth_rw_get_idle(rx_q->wptr, rx_q->rptr, rx_q->size);
    if (tot_num == 0) {
        /* queue is full */
        tab_ctx->refill_q_full++;
        return 0;
    }

#ifdef WETH_NETWORK_PEAK_SPEED_TEST_EN
    tot_num += 1; // 峰值测试模式不会补表因此全部申请完成
#endif

    while (cnt < tot_num) {
        /* alloc skb */
        ret = weth_trb_tab_init_skb_base(tab_ctx->dev_for_map, &(desc_buf->skb[cnt]), &(desc_buf->trb[cnt].addr));
        if (ret) {
            tab_ctx->alloc_skb_fail++;
            break;
        }

        cnt++;
    }

    if (cnt > 0) {
        ret = weth_trb_tab_set_rx_desc(desc_buf->skb, desc_buf->trb, cnt);
        if (ret == 0) {
#ifdef WETH_NETWORK_PEAK_SPEED_TEST_EN
            cnt -= 1; // 峰值测试模式不会补表因此全部申请完成
#endif
            rx_q->wptr = weth_rw_get_pos(rx_q->wptr + cnt, rx_q->size);
        } else {
            tab_ctx->rx_set_desc_fail++;
        }
    }

    if (ret != 0 || cnt < tot_num) {
        /* 需要继续补表 */
        queue_delayed_work(tab_ctx->refill_queue, &tab_ctx->refill_work, 0); /* restart work in 10 jiffies */
    }

    return ret;
}

STATIC void weth_trb_tab_refill_push(struct work_struct *work)
{
    int ret;
    unsigned int num;
    struct weth_trb_tab_ctx *tab_ctx = container_of(work, struct weth_trb_tab_ctx, refill_work.work);

#ifdef WETH_NETWORK_PEAK_SPEED_TEST_EN
    struct weth_trb_desc_queue *rx_q = &tab_ctx->rx_desc;

    num = weth_rw_get_busy(rx_q->wptr, rx_q->rptr, rx_q->size);
    if (unlikely(num == 0)) {
        /* queue is full */
        weth_pr_err("rc rx trb q is empty\n");
        return;
    }
    rx_q->wptr = weth_rw_get_pos(rx_q->wptr + num, rx_q->size);

#else
    ret = weth_trb_alloc_rx_desc(tab_ctx);
    if (ret) {
        return;
    }
#endif

    /* 新增数量超过dpa trb_buf 大小时，通知dpa */
    num = weth_get_ptr_diff(tab_ctx->rx_desc.last_wptr, tab_ctx->rx_desc.wptr, tab_ctx->rx_desc.size);
    if (num < WETH_NOTIFY_WATER_LINE) {
        return;
    }

    /* 通知dpa搬表 */
    ret = weth_trb_send_event(WETH_DIR_DL, WETH_RC_RX_REFILL_TRB_COMPLETE, 1, tab_ctx->rx_desc.last_wptr); // TODO wptr acore灌包异常
    if (ret) {
        weth_pr_err("weth_trb_send_event refill fail %d\n", ret);
    }
    tab_ctx->rx_desc.last_wptr = tab_ctx->rx_desc.wptr;

    return;
}

int weth_trb_desc_init(struct weth_trb_tab_ctx *tab_ctx)
{
    /* rc tx desc */
    tab_ctx->tx_desc.desc_virt = (struct weth_trb_desc *)dma_alloc_coherent(tab_ctx->dev_for_map,
        WETH_EP_RX_TAB_MAX_NUM * sizeof(struct weth_trb_desc), &(tab_ctx->tx_desc.desc_phy), GFP_KERNEL);
    if (tab_ctx->tx_desc.desc_virt == NULL) {
        weth_pr_err("tx trb desc alloc error\n");
        return -1;
    }
    weth_pr_err("tx trb phy addr 0x%llx\n", tab_ctx->tx_desc.desc_phy);

    tab_ctx->tx_desc.local_skb = (struct sk_buff **)kzalloc(WETH_EP_RX_TAB_MAX_NUM * sizeof(struct sk_buff *), GFP_KERNEL);
    if (tab_ctx->tx_desc.local_skb == NULL) {
        weth_pr_err("tx desc skb ptr alloc error\n");
        return -1;
    }

    tab_ctx->tx_desc.size = WETH_EP_RX_TAB_MAX_NUM;
    spin_lock_init(&tab_ctx->tx_desc.lock);

    /* rc rx desc */
    tab_ctx->rx_desc.desc_virt = (struct weth_trb_desc *)dma_alloc_coherent(tab_ctx->dev_for_map,
        WETH_RC_RX_TAB_MAX_NUM * sizeof(struct weth_trb_desc), &(tab_ctx->rx_desc.desc_phy), GFP_KERNEL);
    if (tab_ctx->rx_desc.desc_virt == NULL) {
        weth_pr_err("rx trb desc alloc error\n"); // dma_free_coherent(tab_ctx->dev_for_map
        return -1;
    }
    weth_pr_err("rx trb phy addr 0x%llx\n", tab_ctx->rx_desc.desc_phy);

    tab_ctx->rx_desc.local_skb = (struct sk_buff **)kzalloc(WETH_RC_RX_TAB_MAX_NUM * sizeof(struct sk_buff *), GFP_KERNEL);
    if (tab_ctx->rx_desc.local_skb == NULL) {
        weth_pr_err("rx desc skb ptr alloc error\n");
        return -1;
    }

    tab_ctx->rx_desc_cache.skb = (struct sk_buff **)kzalloc(WETH_RC_RX_TAB_MAX_NUM * sizeof(struct sk_buff *), GFP_KERNEL);
    if (tab_ctx->rx_desc_cache.skb == NULL) {
        weth_pr_err("rx desc cache skb ptr alloc error\n");
        return -1;
    }
    tab_ctx->rx_desc_cache.trb = (struct weth_trb_desc *)kzalloc(WETH_RC_RX_TAB_MAX_NUM * sizeof(struct weth_trb_desc), GFP_KERNEL);
    if (tab_ctx->rx_desc_cache.trb == NULL) {
        weth_pr_err("rx desc cache trb alloc error\n");
        return -1;
    }

    tab_ctx->rx_proc_trb_cache = (struct weth_trb_desc *)kzalloc(WETH_RC_RX_TAB_MAX_NUM * sizeof(struct weth_trb_desc), GFP_KERNEL);
    if (tab_ctx->rx_proc_trb_cache == NULL) {
        weth_pr_err("rx proc trb cache alloc error\n");
        return -1;
    }

    tab_ctx->rx_desc.size = WETH_RC_RX_TAB_MAX_NUM;
    spin_lock_init(&tab_ctx->rx_desc.lock);

    return 0;
}

int weth_trb_tab_init(struct device *dev_for_map, struct weth_trb_info *tx_trb, struct weth_trb_info *rx_trb)
{
    int ret;
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;

    (void)memset_s(tab_ctx, sizeof(struct weth_trb_tab_ctx), 0, sizeof(struct weth_trb_tab_ctx));

    tab_ctx->dev_for_map = dev_for_map;

    tab_ctx->refill_queue = create_singlethread_workqueue("weth_refill");
    if (tab_ctx->refill_queue == NULL) {
        ret = -1;
        goto fail;
    }
    INIT_DELAYED_WORK(&tab_ctx->refill_work, weth_trb_tab_refill_push);

    ret = weth_trb_desc_init(tab_ctx);
    if (ret) {
        goto fail;
    }

    // 通知ep侧，trb信息
    weth_trb_tab_set_addr_to_bar(tx_trb, rx_trb);
    ret = weth_trb_send_event(WETH_DIR_DL, WETH_LINK_UP, 1, 0);
    if (ret) {
        weth_pr_err("weth_send_event fail:%d, line:%d\n", ret, __LINE__);
    }

    ret = weth_trb_send_event(WETH_DIR_UL, WETH_LINK_UP, 1, 0);
    if (ret) {
        weth_pr_err("weth_send_event fail:%d, line:%d\n", ret, __LINE__);
    }
    /* init download stream trb */
    ret = weth_trb_alloc_rx_desc(tab_ctx);
    if (ret) {
        goto fail;
    }
    /* 通知dpa搬表 */
    tab_ctx->rx_desc.last_wptr = tab_ctx->rx_desc.wptr;
    ret = weth_trb_send_event(WETH_DIR_DL, WETH_RC_RX_REFILL_TRB_COMPLETE, 1, tab_ctx->rx_desc.last_wptr);
    if (ret) {
        weth_pr_err("weth_trb_send_event refill fail %d\n", ret);
        goto fail;
    }

    return 0;

fail:
    weth_pr_err("trb tab init fail %d\n", ret); // weth_trb_tab_free(tab_ctx);
    return -1;
}

void weth_trb_tab_rc_tx_complete(unsigned int rw_ptr)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    struct weth_trb_desc_queue *tx_q = &tab_ctx->tx_desc;
    struct sk_buff *skb = NULL;
    unsigned int i;
    unsigned int num;

    num = weth_get_ptr_diff(tx_q->rptr, rw_ptr, tx_q->size);
    if (num >= tx_q->size) {
        tab_ctx->txq_rptr_overflow++;
        return;
    }

    for (i = 0; i < num; i++) {
        skb = tx_q->local_skb[tx_q->rptr];
        tx_q->local_skb[tx_q->rptr] = NULL;
        tx_q->rptr = weth_rw_get_pos(tx_q->rptr + 1, tx_q->size);

        if (skb == NULL) {
            tab_ctx->txq_localskb_null++;
            continue;
        }

        dma_unmap_single(tab_ctx->dev_for_map, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
        __weth_free_skb(skb);
    }
}

int weth_trb_tab_start_tx_data_dma(unsigned int txq_wptr)
{
    int ret;

    ret = weth_trb_send_event(WETH_DIR_UL, WETH_RC_TX_UPDATE_WPTR, 1, txq_wptr);
    if (ret) {
        weth_pr_err("weth_send_event fail:%d, line:%d\n", ret, __LINE__);
    }
    return ret;
}

STATIC int weth_trb_tab_rx_cache_desc(struct device *dev, struct weth_trb_desc *cache_buf, struct weth_trb_desc_queue *rx_q, unsigned int num)
{
    unsigned int copy_num = 0;
    unsigned int copy_size = 0;
    int ret;
    unsigned int rptr;

    rptr = rx_q->rptr;

    if (rptr + num <= rx_q->size) {
        /* 无翻转 */
        copy_size = sizeof(struct weth_trb_desc) * num;
        ret = memcpy_s(cache_buf, copy_size, &rx_q->desc_virt[rptr], copy_size);
        if (ret) {
            weth_pr_err("memcpy all err\n");
            return ret;
        }
    } else {
        copy_num = rx_q->size - rptr;
        copy_size = sizeof(struct weth_trb_desc) * copy_num;
        ret = memcpy_s(cache_buf, copy_size, &rx_q->desc_virt[rptr], copy_size);
        if (ret) {
            weth_pr_err("memcpy copy_num err\n");
            return ret;
        }

        copy_size = sizeof(struct weth_trb_desc) * (num - copy_num);
        ret = memcpy_s(&cache_buf[copy_num], copy_size, &rx_q->desc_virt[0], copy_size);
        if (ret) {
            weth_pr_err("memcpy remain err\n");
            return ret;
        }
    }

    return ret;
}

STATIC int weth_trb_proc_dl_skb(struct weth_trb_tab_ctx *tab_ctx, struct weth_trb_desc *trb, struct sk_buff *skb, unsigned int *dev_id)
{
    if (skb == NULL) {
        tab_ctx->rx_skb_null++;
        weth_pr_err("skb null\n");
        return -1;
    }

    if (trb == NULL) {
        tab_ctx->rx_trb_null++;
        dma_unmap_single(tab_ctx->dev_for_map, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
        __weth_free_skb(skb);
        weth_pr_err("trb null\n");
        return -1;
    }

    if (trb->data_len < ETH_HLEN || trb->data_len > WETH_SKB_MTU || WETH_TRB_GET_DEV_ID(trb) > WETH_MUX_NUM) {
        tab_ctx->rx_data_err++;
        dma_unmap_single(tab_ctx->dev_for_map, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);
        __weth_free_skb(skb);
        return -1;
    }

    tab_ctx->stats.rx_packets++;
    tab_ctx->stats.rx_bytes += trb->data_len;
    *dev_id = WETH_TRB_GET_DEV_ID(trb);

    skb_put(skb, trb->data_len);
    dma_unmap_single(tab_ctx->dev_for_map, virt_to_phys(skb->data), skb->len, DMA_FROM_DEVICE);

    return 0;
}

STATIC void weth_trb_tab_rx_data_done(unsigned int rw_ptr)
{
    struct sk_buff *skb_tail = NULL;
    struct sk_buff *skb = NULL;
    unsigned int dev_id;
    unsigned int dev_id_old = 0;
    unsigned int rx_pkg = 0;
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    struct weth_trb_desc_queue *rx_q = &tab_ctx->rx_desc;
    unsigned int num;
    unsigned int i;
    int ret;

    num = weth_get_ptr_diff(rx_q->rptr, rw_ptr, rx_q->size);
    if (num == 0) {
        tab_ctx->rx_data_num_err++;
        return;
    }

    // 将收到的trb，临时缓存在cache中
    ret = weth_trb_tab_rx_cache_desc(tab_ctx->dev_for_map, tab_ctx->rx_proc_trb_cache, rx_q, num);
    if (unlikely(ret)) {
        tab_ctx->rx_trb_cache_err++;
        rx_q->rptr = weth_rw_get_pos(rx_q->rptr + num, rx_q->size);
        return;
    }

    for (i = 0; i < num; i++) {
        skb = rx_q->local_skb[rx_q->rptr];
        rx_q->local_skb[rx_q->rptr] = NULL;
        rx_q->rptr = weth_rw_get_pos(rx_q->rptr + 1, rx_q->size);

        if (weth_trb_proc_dl_skb(tab_ctx, &tab_ctx->rx_proc_trb_cache[i], skb, &dev_id)) {
            continue;
        }

        /* if dev id is change, upload the skb */
        if (skb_tail != NULL && dev_id != dev_id_old) {
            if (tab_ctx->rx_cb != NULL) {
                tab_ctx->rx_cb(dev_id_old, skb_tail, rx_pkg);
            } else {
                weth_pr_err("rx_cb is null\n");
            }
            skb_tail = NULL;
            rx_pkg = 0;
        }
        dev_id_old = dev_id;
        skb_onelink_tail(&skb_tail, skb);
        rx_pkg++;
    }

    if (likely(skb_tail != NULL)) {
        /* upload last skb list to dev */
        tab_ctx->rx_cb(dev_id_old, skb_tail, rx_pkg);
    } else {
        tab_ctx->skb_tail_null++;
    }

    /* schedule workqueue to refill skb */
    queue_delayed_work(tab_ctx->refill_queue, &tab_ctx->refill_work, 0);
}

void weth_trb_tab_get_rx_desc_by_idx(unsigned int rw_ptr)
{
#ifdef WETH_NETWORK_PEAK_SPEED_TEST_EN
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    struct weth_trb_desc_queue *rx_q = &tab_ctx->rx_desc;
    unsigned int num;
    int ret;

    if (rw_ptr >= WETH_RC_RX_TAB_MAX_NUM) {
        return;
    }
    /* 测试下行性能 */
    num = weth_get_ptr_diff(rx_q->rptr, rw_ptr, rx_q->size);
    rx_q->rptr = weth_rw_get_pos(rx_q->rptr + num, rx_q->size);
    rx_q->wptr = weth_rw_get_pos(rx_q->wptr + num, rx_q->size);

    /* 新增数量超过dpa trb_buf 大小时，通知dpa */
    num = weth_get_ptr_diff(tab_ctx->rx_desc.last_wptr, tab_ctx->rx_desc.wptr, tab_ctx->rx_desc.size);
    if (num < WETH_NOTIFY_WATER_LINE) {
        return;
    }

    /* 通知dpa搬表 */
    ret = weth_trb_send_event(WETH_DIR_DL, WETH_RC_RX_REFILL_TRB_COMPLETE, 1, tab_ctx->rx_desc.last_wptr);
    if (ret) {
        weth_pr_err("weth_trb_send_event refill fail %d\n", ret);
    }
    tab_ctx->rx_desc.last_wptr = tab_ctx->rx_desc.wptr;

#else
    if (rw_ptr >= WETH_RC_RX_TAB_MAX_NUM) {
        return;
    }

    weth_trb_tab_rx_data_done(rw_ptr);
#endif
    return;
}

void weth_trb_tab_regist_rx_cb(weth_trb_tab_rx_cb_t rx_cb)
{
    g_weth_trb_tab_ctx.rx_cb = rx_cb;
    return;
}

void weth_trb_tab_set_addr_to_bar(struct weth_trb_info *tx_info, struct weth_trb_info *rx_info)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    struct weth_trb_info trb_info;

    tx_info->base_addr = tab_ctx->tx_desc.desc_phy;
    tx_info->depth = tab_ctx->tx_desc.size;
    rx_info->base_addr = tab_ctx->rx_desc.desc_phy;
    rx_info->depth = tab_ctx->rx_desc.size;

    /* read of order */
    trb_info.base_addr = tx_info->base_addr;
    trb_info.depth = tx_info->depth;
    trb_info.base_addr = rx_info->base_addr;
    trb_info.depth = rx_info->depth;

    weth_pr_err("rc tx base 0x%llx depth %u\n", tab_ctx->tx_desc.desc_phy, tab_ctx->tx_desc.size);
    weth_pr_err("rc bar tx base 0x%llx depth %u\n", tx_info->base_addr, tx_info->depth);

    weth_pr_err("rc rx base 0x%llx depth %u\n", tab_ctx->rx_desc.desc_phy, tab_ctx->rx_desc.size);
    weth_pr_err("rc bar rx base 0x%llx depth %u\n", rx_info->base_addr, rx_info->depth);
}

void weth_trb_tab_regist_table_sync_cb(weth_trb_tab_table_sync_finish_cb_t table_sync_finish_cb)
{
    g_weth_trb_tab_ctx.table_sync_finish_cb = table_sync_finish_cb;
    return;
}

void weth_trb_tab_reinit(void)
{
}

void weth_rc_dump_trb_table_status(unsigned int dir, unsigned int start_idx, unsigned int len)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    struct weth_trb_desc_queue *desc = NULL;
    struct weth_trb_desc_cache *cache_buf = NULL;
    unsigned int end_idx;
    unsigned int i;

    if (dir == 0) {
        desc = &tab_ctx->rx_desc;
        cache_buf = &tab_ctx->rx_desc_cache;
        weth_pr_err("rc rx trb status:\n");
    } else {
        desc = &tab_ctx->tx_desc;
        weth_pr_err("rc tx trb status:\n");
    }

    weth_pr_err("size %u last_w %u w %u r %u\n", desc->size, desc->last_wptr, desc->wptr, desc->rptr);
    weth_pr_err("addr phy 0x%llx vir 0x%llx\n", (unsigned long long)desc->desc_phy,
        (unsigned long long)desc->desc_virt);

    if (start_idx >= desc->size) {
        start_idx = 0;
    }

    end_idx = start_idx + len;
    if (end_idx >= desc->size) {
        end_idx = desc->size;
    }

    // trb
    for (i = start_idx; i < end_idx; i++) {
        weth_pr_err("%d trb addr 0x%llx ctrl 0x%x len %d\n", i, desc->desc_virt[i].addr, desc->desc_virt[i].ctrl,
            desc->desc_virt[i].data_len);

        if (desc->local_skb[i] != NULL) {
            weth_pr_err("%d skb addr 0x%llx data 0x%x\n", i, (unsigned long long)(uintptr_t)desc->local_skb[i],
                *(unsigned int *)(desc->local_skb[i]->data));
        }
    }

    if (dir != 0) {
        return;
    }
    // tmp cache buf
    weth_pr_err("tmp cache buf:\n");
    for (i = start_idx; i < end_idx; i++) {
        if (cache_buf != NULL && cache_buf->skb[i] != NULL) {
            weth_pr_err("%d trb addr 0x%llx ctrl 0x%x len %d\n", i, cache_buf->trb[i].addr, cache_buf->trb[i].ctrl,
                cache_buf->trb[i].data_len);

            weth_pr_err("%d skb addr 0x%llx data 0x%x\n", i, (unsigned long long)(uintptr_t)cache_buf->skb[i],
                *(unsigned int *)(cache_buf->skb[i]->data));
        }
    }
}

void weth_trb_tab_bps_dump(void)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;
    static unsigned long long last_rx_bytes = 0;

    weth_pr_err("rx pkts: %lld bytes: %lld\n", tab_ctx->stats.rx_packets, tab_ctx->stats.rx_bytes);
    weth_pr_err("rx Mbps: %lld\n", (tab_ctx->stats.rx_bytes - last_rx_bytes) * BYTE_TO_BITS / WETH_SPEED_BIT_TO_MB);

    last_rx_bytes = tab_ctx->stats.rx_bytes;
}

int weth_rx_speed_show(void)
{
#define BIT_NUM_PER_BYTE (8)
#define BYTE_TO_MB (1024 * 1024)
#define MS_TO_S (1000)

    static unsigned long long last_bytes = 0;
    static ktime_t last_timestamp = 0;
    unsigned long long cur_bytes = 0;
    unsigned long long inc_bytes;
    ktime_t cur_time;
    s64 time_slice;
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;

    cur_bytes = tab_ctx->stats.rx_bytes;
    cur_time = ktime_get();

    if (cur_bytes >= last_bytes) {
        inc_bytes = cur_bytes - last_bytes;
    } else {
        inc_bytes = 0xFFFFFFFFFFFFFFFF + cur_bytes - last_bytes;
    }

    time_slice = ktime_to_ms(ktime_sub(cur_time, last_timestamp));

    weth_pr_err("rcv_bytes %llu elapsed time %lldms\n", inc_bytes, time_slice);
    weth_pr_err("tx speed %d Mb/s\n", (int)((inc_bytes * BIT_NUM_PER_BYTE * MS_TO_S / BYTE_TO_MB) / time_slice));

    last_bytes = cur_bytes;
    last_timestamp = cur_time;

    return 0;
}

void weth_rc_dump_table_status(void)
{
    struct weth_trb_tab_ctx *tab_ctx = &g_weth_trb_tab_ctx;

    weth_pr_err("weth rc table status:\n");

    weth_pr_err("rx_data_num_err:   %d\n", tab_ctx->rx_data_num_err);
    weth_pr_err("rx_trb_cache_err:  %d\n", tab_ctx->rx_trb_cache_err);
    weth_pr_err("rx_skb_null:       %d\n", tab_ctx->rx_skb_null);
    weth_pr_err("rx_trb_null:       %d\n", tab_ctx->rx_trb_null);
    weth_pr_err("rx_data_err:       %d\n", tab_ctx->rx_data_err);
    weth_pr_err("skb_tail_null:     %d\n", tab_ctx->skb_tail_null);
    weth_pr_err("refill_q_full:     %d\n", tab_ctx->refill_q_full);
    weth_pr_err("rx_set_desc_fail:  %d\n", tab_ctx->rx_set_desc_fail);
    weth_pr_err("txq_full:          %d\n", tab_ctx->txq_full);
    weth_pr_err("txq_addr_null:     %d\n", tab_ctx->txq_addr_null);
    weth_pr_err("txq_rptr_overflow: %d\n", tab_ctx->txq_rptr_overflow);
    weth_pr_err("txq_localskb_null: %d\n", tab_ctx->txq_localskb_null);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
