
#include "i2a_mgr.h"
#include <cam_buf.h>
#include <linux/dma-buf.h>
#include <linux/types.h>
#include <securec.h>
#include "idi2axi_channel.h"


static void i2a_handle_eof_buf(struct i2a_dev_base_t *i2a_base_info, struct buf_node *bn, void *data);
void i2a_mgr_init(i2a_mgr_t *i2a_mgr)
{
	if (!i2a_mgr)
		return;
	init_completion(&i2a_mgr->eof_done);
	spin_lock_init(&i2a_mgr->buf_lock);
	INIT_LIST_HEAD(&i2a_mgr->req_list);
	INIT_LIST_HEAD(&i2a_mgr->done_list);
}

void i2a_clear(struct device *dev, struct idi2axi_channel * channels)
{
	if (!dev || !channels)
		return;
	for (int i = 0; i < I2A_MAX_CHANNELS; i++) {
		struct idi2axi_channel *ich = &(channels[i]);
		if (i2a_is_attached(ich)) {
			complete(&ich->flush_done);
			idi2axi_release_channel(ich);
		}
	}
	dev_info(dev, "%s: clear when close idi2axi", __func__);
}

int i2a_attach_channel(struct idi2axi_channel * channels, i2a_mgr_t *i2a_mgr, idi2axi_msg_attach_channel_req *ch_param)
{
	if (!channels || !i2a_mgr || !ch_param) {
		pr_err("%s: input param is null", __func__);
		return -1;
	}
	if (ch_param->cnt > I2A_MAX_CHANNELS) {
		pr_err("%s: ch_param->cnt(%u) > max", __func__, ch_param->cnt);
		return -1;
	}
	struct idi2axi_channel *ich = NULL;
	i2a_mgr->each_sensor_channel_cnt = ch_param->cnt;
	for (int i = 0; i < i2a_mgr->each_sensor_channel_cnt; i++) {
		ich = idi2axi_request_channel(channels, i2a_handle_eof_buf, i2a_mgr, &(ch_param->ch_relation[i]));
		if (!ich)
			return -1;
		pr_info("%s: i2a id = %u request channel id = %u ok", __func__, ich->i2a_base_info->id, ich->vc);
	}
	return 0;
}

static uint64_t i2a_get_buf_addr(unsigned int fd)
{
	int ret;
	struct phys_format physical;
	memset_s(&physical, sizeof(physical), 0, sizeof(physical));
	ret = cam_buf_get_phys(fd, &physical);
	if (ret < 0) {
		pr_err("%s: fail to get phys addr for fd:%d ret=%d", __func__, fd, ret);
		return 0;
	}
	return physical.phys;
}

struct buf_node * create_buf_node(unsigned int i2a_id, i2a_mgr_t *i2a_mgr,
	unsigned int fd, unsigned int frame_num, unsigned int offset)
{
	struct buf_node *bn = NULL;
	struct dma_buf *dmabuf = NULL;
	uint64_t buf_addr = i2a_get_buf_addr(fd);
	if (!buf_addr) {
		pr_err("%s: i2a id = %u frame_num = %u buf va is null", __func__, i2a_id, frame_num);
		return bn;
	}

	bn = kzalloc(sizeof(*bn), GFP_KERNEL);
	if (!bn) {
		pr_err("%s: i2a id = %u frame_num = %u kzalloc bn fail", __func__, i2a_id, frame_num);
		return bn;
	}

	INIT_LIST_HEAD(&bn->entry);
	bn->fd = fd;
	bn->base_addr = buf_addr;
	dmabuf = dma_buf_get(bn->fd);
	int size = dmabuf->size;
	dma_buf_put(dmabuf);
	bn->buf_addr = buf_addr + (size / i2a_mgr->each_sensor_channel_cnt * offset);
	if (bn->buf_addr > buf_addr + size) {
		pr_err("%s: i2a id = %u frame_num = %u fd = %u offset = %u config error?!",
				__func__, i2a_id, frame_num, fd, offset);
		kfree(bn);
		return NULL;
	}
	bn->frame_num = frame_num;
	reinit_completion(&i2a_mgr->eof_done);
	pr_info("%s: i2a id = %u fd %u buf_addr 0x%px frame_num %u dmabufsize %zu", __func__,
			i2a_id, bn->fd, bn->buf_addr, bn->frame_num, size);
	return bn;
}

static int i2a_add_req_2_req_list(unsigned int i2a_id, i2a_mgr_t *i2a_mgr, unsigned int fd,
	unsigned int api_name, unsigned int msg_id)
{
	unsigned long flags;
	struct req_node * req_bn = NULL;
	req_bn = kzalloc(sizeof(*req_bn), GFP_KERNEL);
	if (!req_bn) {
		pr_err("%s: i2a id = %u kzalloc req_bn fail", __func__, i2a_id);
		return -ENOMEM;
	}

	req_bn->fd = fd;
	req_bn->eof_cnt = 0;
	req_bn->api_name = api_name;
	req_bn->msg_id = msg_id;

	spin_lock_irqsave(&i2a_mgr->buf_lock, flags);
	list_add_tail(&req_bn->entry, &i2a_mgr->req_list);
	spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
	return 0;
}

int i2a_queue_buf(unsigned int i2a_id, struct idi2axi_channel * channels, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg)
{
	int ret = -EFAULT;
	if (!channels || !i2a_mgr || !msg) {
		pr_err("%s: input param is null", __func__);
		return ret;
	}
	struct buf_node *bn = NULL;
	idi2axi_msg_req* req = (idi2axi_msg_req*)(&msg->u.req);
	if (!req) {
		pr_err("%s: i2a id = %u req msg is null", __func__, i2a_id);
		return ret;
	}

	for (int i = 0; i < i2a_mgr->each_sensor_channel_cnt; i++) {
		int ch_id = req->port[i];
		if (ch_id > I2A_MAX_CHANNELS) {
			pr_err("%s: i2a id = %u config error channel id = %u > max", __func__, i2a_id, ch_id);
			ret = -EFAULT;
			break;
		}
		struct idi2axi_channel *ich = &(channels[ch_id]);
		bn = create_buf_node(i2a_id, i2a_mgr, req->fd, req->frame_num, ich->buf_offset);
		if (!bn) {
			ret = -EFAULT;
			break;
		}
		ret = idi2axi_channel_qbuf(ich, bn);
		if (ret) {
			kfree(bn);
			break;
		}
	}

	if (!ret)
		ret = i2a_add_req_2_req_list(i2a_id, i2a_mgr, req->fd, msg->api_name, msg->message_id);
	return ret;
}

static void i2a_handle_eof_buf(struct i2a_dev_base_t *i2a_base_info, struct buf_node *bn, void *data)
{
	unsigned long flags;
	struct req_node *req_bn = NULL;
	struct req_node *temp = NULL;
	struct done_node* done_bn = NULL;
	i2a_mgr_t *i2a_mgr = data;
	bool find = false;
	pr_info("i2a id = %u frame_num = %u eof coms buf:%#lx", i2a_base_info->id, bn->frame_num, bn->buf_addr);

	spin_lock_irqsave(&i2a_mgr->buf_lock, flags);
	if (list_empty(&i2a_mgr->req_list)) {
		pr_err("i2a id = %u req list is null exist error", i2a_base_info->id);
		spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
		return;
	}
	list_for_each_entry_safe(req_bn, temp, &i2a_mgr->req_list, entry) {
		if (bn->fd == req_bn->fd) {
			find = true;
			req_bn->eof_cnt++;
			req_bn->flags |= bn->flag;
			if (req_bn->eof_cnt == i2a_mgr->each_sensor_channel_cnt) {
				if (i2a_mgr->close_dev) {
					goto clear_res;
				}
				done_bn = kzalloc(sizeof(*done_bn), GFP_KERNEL);
				if (!done_bn) {
					pr_err("i2a id = %u kzalloc done_bn fail", i2a_base_info->id);
					goto clear_res;
				}
				if (memcpy_s((void*)(&done_bn->u.bn), sizeof(struct buf_node), (void*)bn, sizeof(struct buf_node))) {
					pr_err("%s:%d: i2a id = %u memcpy_s copy failed", __func__, __LINE__, i2a_base_info->id);
					goto clear_res;
				}
				done_bn->u.bn.flag |= req_bn->flags;
				done_bn->api_name = req_bn->api_name;
				done_bn->msg_id = req_bn->msg_id;
				done_bn->u.bn.base_addr = bn->base_addr;
				list_add_tail(&done_bn->entry, &i2a_mgr->done_list);
				complete(&i2a_mgr->eof_done);
				if (i2a_base_info->dev != NULL)
					idi2axi_notify_event(i2a_base_info->dev);
				break;
			} else {
				kfree(bn);
				spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
				return;
			}
		}
	}

clear_res:
	if (find) {
		list_del(&req_bn->entry);
		kfree(req_bn);
	} else {
		pr_err("i2a id = %u eof come buf not find req error", i2a_base_info->id);
	}
	kfree(bn);
	spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
}

static void i2a_ack(struct i2a_dev_base_t *i2a_base_info, struct done_node *done_bn, idi2axi_msg_t *msg)
{
	msg->message_id = done_bn->msg_id;
	msg->api_name = done_bn->api_name;
	switch(done_bn->api_name) {
	case COMMAND_IDI2AXI_REQUEST:
	{
		idi2axi_msg_ack_req* ack_req = (idi2axi_msg_ack_req*)(&msg->u.ack_req);
		ack_req->fd = done_bn->u.bn.fd;
		ack_req->port = done_bn->u.bn.port;
		ack_req->frame_num = done_bn->u.bn.frame_num;
		u64 ktime = ktime_to_us(done_bn->u.bn.eof_ktime);
		ack_req->timestamp_low = ktime & 0xFFFFFFFFu;
		ack_req->timestamp_high = (ktime >> 32u) & 0xFFFFFFFFu;
		ack_req->addr = done_bn->u.bn.base_addr;
		ack_req->valid = (done_bn->u.bn.flag == I2A_BUF_FL_VALID) ? true : false;
		pr_info("i2a id = %u ack frame_num = %u fd =%u buffer_addr = %#x",
				i2a_base_info->id, ack_req->frame_num, ack_req->fd, ack_req->addr);
	}
	break;
	default:
	break;
	}
}

int i2a_dqueue_buf(struct i2a_dev_base_t *i2a_base_info, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg)
{
	if (!i2a_mgr || !msg || !i2a_base_info || !i2a_base_info->dev) {
		pr_err("%s: input param is null", __func__);
		return -1;
	}
	unsigned long flags;
	uint32_t timeout_ms = DQUEUE_TIMEOUT;
	struct done_node *done_bn = NULL;

	spin_lock_irqsave(&i2a_mgr->buf_lock, flags);
	if (list_empty(&i2a_mgr->done_list)) {
		spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
		if (wait_for_completion_timeout(&i2a_mgr->eof_done, msecs_to_jiffies(timeout_ms)) == 0) {
			pr_err("i2a id = %u wait eof_done timeout!", i2a_base_info->id);
		} else {
			pr_err("i2a id = %u wait eof_done succ", i2a_base_info->id);
		}
		spin_lock_irqsave(&i2a_mgr->buf_lock, flags);
	}

	if (!list_empty(&i2a_mgr->done_list)) {
		done_bn = list_first_entry(&i2a_mgr->done_list, struct done_node, entry);
		if (done_bn) {
			i2a_ack(i2a_base_info, done_bn, msg);
			list_del(&done_bn->entry);
			kfree(done_bn);
		}
	}

	spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
	return 0;
}

int i2a_is_all_channel_idle(struct idi2axi_channel * channels)
{
	if (!channels)
		return -1;
	for (int i = 0; i < I2A_MAX_CHANNELS; i++) {
		struct idi2axi_channel *ich = &(channels[i]);
		if (i2a_is_attached(ich))
			return 1;
	}
	return 0;
}

static void notify_flush_done(struct device *dev, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg)
{
	struct done_node *done_bn = kzalloc(sizeof(*done_bn), GFP_KERNEL);
	if (!done_bn) {
		dev_err(dev, "%s: kzalloc flush bn fail", __func__);
		return;
	}

	INIT_LIST_HEAD(&done_bn->entry);
	done_bn->msg_id = msg->message_id;
	done_bn->api_name = msg->api_name;

	unsigned long flags;
	spin_lock_irqsave(&i2a_mgr->buf_lock, flags);
	list_add_tail(&done_bn->entry, &i2a_mgr->done_list);
	complete(&i2a_mgr->eof_done);
	spin_unlock_irqrestore(&i2a_mgr->buf_lock, flags);
	if (dev)
		idi2axi_notify_event(dev);
}

int i2a_flush(struct device *dev, struct idi2axi_channel * channels, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg)
{
	if (!dev || !channels || !i2a_mgr || !msg) {
		pr_err("%s: input param is null", __func__);
		return -1;
	}
	idi2axi_msg_flush* flush_param = (idi2axi_msg_flush*)(&msg->u.flush_req);
	if (!flush_param) {
		dev_err(dev, "%s: flush param is null", __func__);
		return -1;
	}

	bool timeout = false;
	for (int i = 0; i < i2a_mgr->each_sensor_channel_cnt; i++) {
		int ch_id = flush_param->port[i];
		if (ch_id > I2A_MAX_CHANNELS) {
			dev_err(dev, "%s: config error channel id = %u  > max", __func__, ch_id);
			return -1;
		}
		struct idi2axi_channel *ich = &(channels[ch_id]);
		int ret = idi2axi_release_channel(ich);
		if (ret < 0)
			return -1;
		else if (ret > 0)
			timeout = true;
	}

	if (!timeout)
		notify_flush_done(dev, i2a_mgr, msg);
	return timeout ? 1 : 0;
}

int i2a_usecase_config(struct i2a_dev_base_t *i2a_base_info, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg)
{
	if (!i2a_base_info || !i2a_mgr || !msg) {
		pr_err("%s: input param is null", __func__);
		return -1;
	}
	idi2axi_msg_usecase_config* param = (idi2axi_msg_usecase_config*)(&msg->u.usecase_config_req);
	if (!param) {
		pr_err("%s: i2a id = %u flush param is null", __func__, i2a_base_info->id);
		return -1;
	}

	i2a_mgr->buf_size =  param->buf_size;
	return 0;
}
