/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi device driver
 *
 * This file is released under the GPLv2.
 */
#include "idi2axi_channel.h"
#include <linux/bits.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/kconfig.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-subdev.h>
#include <media/videobuf2-core.h>
#include <securec.h>
#include "i2a_csi2if.h"
#include "i2a_debug.h"
#include "plat_idi2axi.h"
#include "cam_intf.h"
#include "cam_log.h"
#include "idi2axi_intf.h"


void i2a_channel_init(struct idi2axi_channel *channels, unsigned int max_ch_cnt, struct i2a_dev_base_t *i2a_base_info)
{
	if (!channels || !i2a_base_info) {
		pr_err("%s: input param is null", __func__);
		return;
	}
	struct idi2axi_channel *ch = NULL;
	for (int cid = 0; cid < max_ch_cnt; ++cid) {
		ch = &channels[cid];
		ch->i2a_base_info = i2a_base_info;
		init_completion(&ch->flush_done);
		spin_lock_init(&ch->lock);
		INIT_LIST_HEAD(&ch->buf_list);
		INIT_LIST_HEAD(&ch->done_list);
	}
}

bool check_channel(struct idi2axi_channel *ich)
{
	if (!ich || !ich->i2a_base_info || !ich->i2a_base_info->dev) {
		pr_err("%s: channel param is null", __func__);
		return false;
	}
	return true;
}

static struct idi2axi_channel *i2a_get_channel(struct idi2axi_channel *channels, channel_eof_cb cb,
	void *cb_data, channel_port_dt_buf_offset *param)
{
	unsigned long flags;
	if (param->port > I2A_MAX_CHANNELS) {
		pr_err("%s: config error channel id = %d > max", __func__, param->port);
		return NULL;
	}
	struct idi2axi_channel *ch = &channels[param->port];
	if (!check_channel(ch))
		return NULL;
	if (i2a_is_attached(ch)) {
		pr_err("%s: channel id = %d is already attach", __func__, param->port);
		return NULL;
	}

	ch->requestor = kstrdup_const(dev_name(ch->i2a_base_info->dev), GFP_KERNEL);
	ch->vc = param->port;
	ch->dt = param->dt;
	ch->buf_offset = param->buf_offset;
	spin_lock_irqsave(&ch->lock, flags);
	ch->eof_cb = cb;
	ch->cb_data = cb_data;
	ch->flags = I2A_CH_FL_ATTACHED;
	spin_unlock_irqrestore(&ch->lock, flags);
	return ch;
}

struct idi2axi_channel *idi2axi_request_channel(struct idi2axi_channel *channels,
	channel_eof_cb cb, void *cb_data, struct _channel_port_dt_buf_offset* param)
{
	unsigned long flags;
	if (!channels || !cb || !param || !cb_data) {
		pr_err("%s: input param is null", __func__);
		return NULL;
	}
	struct idi2axi_channel *ch = i2a_get_channel(channels, cb, cb_data, param);
	if (!ch)
		return NULL;

	spin_lock_irqsave(&ch->lock, flags);
	if (plat_idi2axi_channel_config(ch)) {
		ch->flags = 0;
		spin_unlock_irqrestore(&ch->lock, flags);
		return NULL;
	}
	ch->flags |= (I2A_CH_FL_POWERED | I2A_CH_FL_STREAMING);
	spin_unlock_irqrestore(&ch->lock, flags);
	return ch;
}

static void i2a_put_channel(struct idi2axi_channel *ch)
{
	kfree_const(ch->requestor);
	ch->requestor = NULL;
	ch->flags = 0;
	ch->eof_cb = NULL;
	ch->cb_data = NULL;
}

int idi2axi_channel_qbuf(struct idi2axi_channel *ch, struct buf_node *bn)
{
	unsigned long flags;
	if (!bn || !check_channel(ch)) {
		pr_err("%s: channel or bn is null", __func__);
		return -EINVAL;
	}

	spin_lock_irqsave(&ch->lock, flags);
	if (!i2a_is_streaming(ch)) {
		dev_err(ch->i2a_base_info->dev, "channel %s is not streaming", ch->requestor);
		spin_unlock_irqrestore(&ch->lock, flags);
		return -EINVAL;
	}

	list_add_tail(&bn->entry, &ch->buf_list);
	if (list_is_singular(&ch->buf_list)) {
		dev_info(ch->i2a_base_info->dev, "channel:%d queue buf: %#x", ch->vc, bn->buf_addr);
		plat_idi2axi_push_buffer(ch, bn->buf_addr);
	}
	spin_unlock_irqrestore(&ch->lock, flags);
	return 0;
}

static int idi2axi_channel_flush(struct idi2axi_channel *ch)
{
	int is_working = 0;
	unsigned long flags;
	struct buf_node *bn = NULL;
	struct buf_node *tmp = NULL;
	long jiff = (long)msecs_to_jiffies(FLUSH_DONE_TIME);
	int ret = IDI2AXI_FLUSH_CHANNEL_SUCC;
	struct device *dev = ch->i2a_base_info->dev;

	spin_lock_irqsave(&ch->lock, flags);
	if (!i2a_is_streaming(ch)) {
		spin_unlock_irqrestore(&ch->lock, flags);
		dev_err(dev, "%s: channel is not streaming not need flush", __func__);
		return -1;
	}
	reinit_completion(&ch->flush_done);
	ch->flags &= ~I2A_CH_FL_STREAMING;
	is_working = plat_idi2axi_channel_working(ch);
	spin_unlock_irqrestore(&ch->lock, flags);

	if (is_working) {
		if (wait_for_completion_timeout(&ch->flush_done, jiff) == 0) {
			dev_err(dev, "ch %s flush timeout", ch->requestor);
			ret = IDI2AXI_FLUSH_CHANNEL_TIMEOUT;
		} else {
			dev_info(dev, "ch %s flush done", ch->requestor);
		}
	}

	spin_lock_irqsave(&ch->lock, flags);
	list_for_each_entry_safe(bn, tmp, &ch->done_list, entry) {
		dev_info(dev, "%s:channel %u done_list flush frame_num = %u", __func__, ch->vc, bn->frame_num);
		list_del(&bn->entry);
		ch->eof_cb(ch->i2a_base_info, bn, ch->cb_data); // after cb, bn is invalid
	}
	list_for_each_entry_safe(bn, tmp, &ch->buf_list, entry) {
		dev_info(dev, "%s:channel %u buf_list flush frame_num = %u", __func__, ch->vc, bn->frame_num);
		bn->flag = I2A_BUF_FL_FLUSHED;
		list_del(&bn->entry);
		ch->eof_cb(ch->i2a_base_info, bn, ch->cb_data); // after cb, bn is invalid
	}
	spin_unlock_irqrestore(&ch->lock, flags);
	return ret;
}

int idi2axi_release_channel(struct idi2axi_channel *ch)
{
	if (!check_channel(ch))
		return -1;
	int ret = idi2axi_channel_flush(ch);
	if (ret < 0)
		return ret;

	unsigned long flags;
	spin_lock_irqsave(&ch->lock, flags);
	if (i2a_is_powered(ch)) {
		ch->flags &= ~(I2A_CH_FL_POWERED);
		plat_idi2axi_channel_deconfig(ch);
	}
	if (i2a_is_attached(ch))
		i2a_put_channel(ch);
	spin_unlock_irqrestore(&ch->lock, flags);
	return ret;
}

static void push_buffer_to_channel(struct idi2axi_channel *ch)
{
	struct buf_node *bn = NULL;
	if (list_empty(&ch->buf_list)) {
		dev_err(ch->i2a_base_info->dev, "channel:%u buf list empty", ch->vc);
		return;
	}

	bn = list_first_entry(&ch->buf_list, struct buf_node, entry);
	plat_idi2axi_push_buffer(ch, bn->buf_addr);
	pr_info("i2a_id = %u  frame_num = %d push to channel:%d buf: %#lx",
			ch->i2a_base_info->id, bn->frame_num, ch->vc, bn->buf_addr);
}

static void pop_buffer_from_channel(struct idi2axi_channel *ch)
{
	struct buf_node *bn = NULL;
	uint64_t eof_buf = plat_idi2axi_pop_buffer(ch);

	list_for_each_entry(bn, &ch->buf_list, entry) {
		if (eof_buf == bn->buf_addr) {
			pr_info("i2a_id = %u frame_num = %d pop from channel = %d eof_buf 0x%px bn->buf_addr ox%px",
					ch->i2a_base_info->id, bn->frame_num, ch->vc, eof_buf, bn->buf_addr);
			bn->flag = I2A_BUF_FL_VALID;
			bn->eof_ktime = ktime_get_boottime();
			list_move_tail(&bn->entry, &ch->done_list);
			break;
		}
	}
}

void idi2axi_channel_eof_handler(struct idi2axi_channel *ch)
{
	if (!check_channel(ch))
		return;
	unsigned long flags;
	spin_lock_irqsave(&ch->lock, flags);
	pop_buffer_from_channel(ch);
	if (i2a_is_streaming(ch))
		push_buffer_to_channel(ch); // if flushing, won't push any more
	else
		complete(&ch->flush_done);

	spin_unlock_irqrestore(&ch->lock, flags);
}
