/*
 * drv_venc_wireless.c
 *
 * This is for wireless lowdelay solution.
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "drv_mem.h"
#include "drv_venc_efl.h"
#include "drv_venc_queue.h"
#include "drv_venc_wireless.h"
#include "venc_wireless_lowdelay_cfg.h"
#include <linux/iommu/mm_iommu.h>

#define RESERVED_LEN 56
#define ALIGN_UP_SIZE 64
#define MAX_PACKET_NUM 2000
#define WAIT_WIFI_RELEASE_TIMEOUT 5
#define WAIT_RELEASE_TIMEOUT_COUNT 4

#define SEND_XPS_STATUS_SUCESS 1
#define SEND_XPS_STATUS_FAILED 2
#define SEND_XPS_STATUS_RETRY  3
#define RETRY_SEND_XPS_TIMES  1

#define VENC_WIRELESS_FIXED_MSDU_HEADER_SIZE 96
#define VENC_WIRELESS_FIXED_XPS_HEADER_LEN 187
#define VENC_WIRELESS_FIXED_PACKAGE_LEN  0x620

enum wireless_buf_status {
	IDLE,
	OWNED_BY_VENC,
	OWNED_BY_WIFI,
};

struct pack_info {
	uint16_t layer_num;
	uint16_t last_package_len;
	uint16_t nomal_package_len;
	uint16_t package_num;
	uint8_t reserved[RESERVED_LEN];
};

struct msdu {
	uint16_t type;
	u_qos_layer_info qos_layer_info;
	uint16_t pack_idx;
	uint16_t payload_len;
	uint32_t cfg0;
};

enum venc_wireless_enty_init_status {
	VENC_WIRELESS_ENTY_UNINIT,
	VENC_WIRELESS_ENTY_INITED,
	VENC_WIRELESS_ENC_INITED
};

static DEFINE_MUTEX(g_venc_wireless_mutex);
static DEFINE_MUTEX(g_venc_send_recv_mutex);
static DEFINE_SPINLOCK(g_venc_wireless_buffer_spinlock);

static struct venc_wireless_entry *venc_get_wireless_entry(void)
{
	static struct venc_wireless_entry g_wireless_entry;
	return &g_wireless_entry;
}

bool __attribute__((weak)) wifi_tx_venc_pkg(struct _tx_layer_ctrl *ctrl)
{
	return true;
}

void __attribute__((weak)) venc_set_callback(struct vcodec_venc_callback *callback)
{
}

uint32_t __attribute__((weak)) hmac_vsp_source_map_dma_buf(struct dma_buf *dmabuf, uintptr_t *iova)
{
	*iova = 0;
	return 0;
}

uint32_t __attribute__((weak)) hmac_vsp_source_unmap_dma_buf(struct dma_buf *dmabuf)
{
	return 0;
}


static int32_t map_wifi_iova(struct dma_buf *dmabuf, uint64_t *wifi_iova)
{
	uintptr_t iova = 0;
	uint32_t ret;


	ret = hmac_vsp_source_map_dma_buf(dmabuf, &iova);
	if (ret || !iova) {
		VCODEC_ERR_VENC("vsp map dma buf failed or iova is 0");
		return -EFAULT;
	}

	*wifi_iova = (uint64_t)iova;
	return 0;
}

static int32_t unmap_wifi_iova(struct dma_buf *dmabuf)
{
	uint32_t ret;


	ret = hmac_vsp_source_unmap_dma_buf(dmabuf);
	if (ret) {
		VCODEC_ERR_VENC("vsp unmap dma buf failed");
		return -EFAULT;
	}
	return 0;
}

static int32_t venc_get_va_by_fd(int32_t shared_fd, uint64_t *wifi_iova, uint64_t *kernel_va)
{
	uint32_t i;
	unsigned long flags;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (shared_fd < 0) {
		VCODEC_FATAL_VENC("shared_fd error, shared_fd: %d", shared_fd);
		return -EINVAL;
	}

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("venc is not inited");
		mutex_unlock(&g_venc_wireless_mutex);
		return -EFAULT;
	}

	for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
		if (entry->stream_buf[i].shared_fd == shared_fd) {
			spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
			entry->stream_buf[i].status = OWNED_BY_WIFI;
			spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
			*wifi_iova = entry->stream_buf[i].wifi_iova;
			*kernel_va = entry->stream_buf[i].kernel_va;
			mutex_unlock(&g_venc_wireless_mutex);
			return 0;
		}
	}
	mutex_unlock(&g_venc_wireless_mutex);

	VCODEC_ERR_VENC("did not find fd %d", shared_fd);
	*kernel_va = 0;
	*wifi_iova = 0;

	return -EFAULT;
}

static uint16_t venc_get_max_layer_suc(struct _send_result *buffer)
{
	uint16_t i;
	uint16_t cnt = 0;

	for (i = 0; i < MAX_LAYER_NUM; i++) {
		if (buffer->layer_suc_num[i] & 0x8000)
			cnt++;
	}
	return cnt;
}

static uint32_t venc_get_rtt_delay(struct _send_result *buffer)
{
	uint64_t recv_ts;
	uint32_t delta_ts = 0;
	uint64_t *send_ts = (uint64_t *)(&buffer->slice_layer->reserve[0]);

	recv_ts = osal_get_sys_time_in_us();
	if (recv_ts >= *send_ts)
		delta_ts = (uint32_t)(recv_ts - *send_ts);

	return delta_ts;
}

/* this func should be called in V700 */
static void venc_modify_msdu_info(uint64_t data_addr, struct pack_info *info,
	uint16_t max_layer, uint32_t frm_no, uint16_t layer_idx)
{
	uint16_t i;
	uint16_t len;
	uint8_t qos;
	struct msdu *msdu_info = NULL;
	u_qos_layer_info qos_layer_info;

	if (info->package_num > MAX_PACKET_NUM)
		return;

	for (i = 0; i < info->package_num; i++) {
		// qos written by hardware
		venc_cfg_qos_layer_info(data_addr, &qos_layer_info);
		// update each package of qos layer info
		qos_layer_info.bits.type = 1; // video type
		qos_layer_info.bits.layer_idx = layer_idx & 0x3;
		qos_layer_info.bits.max_layer = max_layer;
		// update each package of msdu info
		msdu_info = (struct msdu *)(uintptr_t)(data_addr + MSDU_TYPE_START_OFFSET);
		msdu_info->type = 0x0008;
		msdu_info->qos_layer_info = qos_layer_info;
		msdu_info->pack_idx = i;
		len = (i == (info->package_num - 1)) ? info->last_package_len : info->nomal_package_len;
		len -= VENC_WIRELESS_FIXED_MSDU_HEADER_SIZE;
		msdu_info->payload_len = len;
		msdu_info->cfg0 = frm_no & 0x3f;   // frame no 0-63

		data_addr += (len + NEXT_PACKAGE_MSDU_OFFSET);  // offset to next package msdu
	}
}

static void venc_set_mac_info(struct channel_info *channel_info, struct _tx_layer_ctrl *ctrl)
{
	ctrl->mac_sa_address[0] = channel_info->wireless_ldy_info.mac_sal & 0xff;
	ctrl->mac_sa_address[1] = (channel_info->wireless_ldy_info.mac_sal >> 8) & 0xff;
	ctrl->mac_sa_address[2] = (channel_info->wireless_ldy_info.mac_sal >> 16) & 0xff;
	ctrl->mac_sa_address[3] = (channel_info->wireless_ldy_info.mac_sal >> 24) & 0xff;
	ctrl->mac_sa_address[4] = channel_info->wireless_ldy_info.mac_rah_sah & 0xff;
	ctrl->mac_sa_address[5] = (channel_info->wireless_ldy_info.mac_rah_sah >> 8) & 0xff;
	ctrl->mac_ra_address[0] = channel_info->wireless_ldy_info.mac_ral & 0xff;
	ctrl->mac_ra_address[1] = (channel_info->wireless_ldy_info.mac_ral >> 8) & 0xff;
	ctrl->mac_ra_address[2] = (channel_info->wireless_ldy_info.mac_ral >> 16) & 0xff;
	ctrl->mac_ra_address[3] = (channel_info->wireless_ldy_info.mac_ral >> 24) & 0xff;
	ctrl->mac_ra_address[4] = (channel_info->wireless_ldy_info.mac_rah_sah >> 16) & 0xff;
	ctrl->mac_ra_address[5] = (channel_info->wireless_ldy_info.mac_rah_sah >> 24) & 0xff;
}

static int32_t venc_set_head_ctrl(struct channel_info *channel_info, struct _tx_layer_ctrl *ctrl)
{
	uint64_t strmbuf_iova;
	uint64_t kernel_va;
	uint32_t buf_size;
	uint32_t i;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
		if (entry->stream_buf[i].shared_fd == channel_info->wireless_ldy_info.stream_header.sharefd)
			break;
	}
	if (i >= MAX_STRM_BUF_SIZE) {
		VCODEC_ERR_VENC("The stream buffer is full.\n");
		return -EINVAL;
	}

	buf_size = entry->stream_buf[i].buf_size;
	if (buf_size < VENC_WIRELESS_FIXED_XPS_HEADER_LEN) {
		VCODEC_ERR_VENC("invalid buf size %d\n", buf_size);
		return -EINVAL;
	}

	if (venc_get_va_by_fd(channel_info->wireless_ldy_info.stream_header.sharefd, &strmbuf_iova, &kernel_va)) {
		VCODEC_ERR_VENC("get wifi iova failed");
		return -EFAULT;
	}

	ctrl->data_addr = kernel_va;
	ctrl->iova_addr = strmbuf_iova;
	ctrl->layer_number = 0;
	ctrl->last_paket_len = VENC_WIRELESS_FIXED_XPS_HEADER_LEN;
	ctrl->normal_pack_length = VENC_WIRELESS_FIXED_XPS_HEADER_LEN;
	ctrl->paket_number = 1;
	ctrl->version = 0x25; // r2.5
	ctrl->qos_type = 0x1 | (0x1 << 14);
	venc_set_mac_info(channel_info, ctrl);
	ctrl->next = 0;
	return 0;
}

static int32_t venc_wireless_check_and_get_data_ctrl(
	struct channel_info *channel_info, struct pack_info *temp_info, uint64_t *iova, uint64_t *va)
{
	uint64_t strmbuf_iova;
	uint64_t kernel_va;
	uint32_t pack_info_total_len;
	uint32_t buf_size;
	uint32_t i;
	uint16_t cur_layer_idx = channel_info->wireless_ldy_info.cur_layer_idx;
	struct layer_pack_info *layer_pack = NULL;
	struct pack_info *info = NULL;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (channel_info->wireless_ldy_info.layer_num > WIRELESS_LOWDELAY_MAX_LAYER ||
		channel_info->wireless_ldy_info.layer_num < 1 ||
		cur_layer_idx >= channel_info->wireless_ldy_info.layer_num)
		return -EINVAL;

	layer_pack = &channel_info->wireless_ldy_info.layer_pack[cur_layer_idx];
	for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
		if (entry->stream_buf[i].shared_fd == layer_pack->shared_fd)
			break;
	}
	if (i >= MAX_STRM_BUF_SIZE) {
		VCODEC_ERR_VENC("The stream buffer is full.\n");
		return -EINVAL;
	}
	buf_size = entry->stream_buf[i].buf_size;
	if (buf_size == 0) {
		VCODEC_ERR_VENC("invalid buf\n");
		return -EFAULT;
	}

	if (venc_get_va_by_fd(layer_pack->shared_fd, &strmbuf_iova, &kernel_va)) {
		return -EFAULT;
	}

	info = (struct pack_info *)(uintptr_t)kernel_va;
	(void)memcpy_s(temp_info, sizeof(struct pack_info), info, sizeof(struct pack_info));
	if (temp_info->nomal_package_len > VENC_WIRELESS_FIXED_PACKAGE_LEN ||
		temp_info->last_package_len > VENC_WIRELESS_FIXED_PACKAGE_LEN ||
		temp_info->layer_num >= WIRELESS_LOWDELAY_MAX_LAYER ||
		temp_info->package_num > MAX_PACKET_NUM) {
		VCODEC_ERR_VENC("invalid len, nomal_package_len %hu, last_package_len %hu",
			info->nomal_package_len, info->last_package_len);
		return -EFAULT;
	}
	pack_info_total_len = (temp_info->package_num - 1) * temp_info->nomal_package_len + temp_info->last_package_len;
	if (pack_info_total_len > buf_size) {
		VCODEC_ERR_VENC("invalid pack info");
		return -EFAULT;
	}

	*iova = strmbuf_iova;
	*va = kernel_va;
	return 0;
}

static int32_t venc_set_data_ctrl(struct channel_info *channel_info,
	struct venc_wireless_entry *entry, struct _tx_layer_ctrl *head)
{
	uint16_t i;
	uint64_t strmbuf_iova;
	uint64_t kernel_va;
	struct layer_pack_info *layer_pack = NULL;
	int32_t check_result;
	struct pack_info temp_info = {0};
	struct _tx_layer_ctrl *ctrl = head;
	size_t pack_offset = align_up(sizeof(struct pack_info), ALIGN_UP_SIZE);
	uint64_t *send_ts = NULL;
	uint16_t cur_layer_idx;

	if (!entry->ctrl_queue) {
		VCODEC_ERR_VENC("ctrl or ctrl_queue is NULL");
		return -EINVAL;
	}

	if (!ctrl)
		return -EFAULT;

	check_result = venc_wireless_check_and_get_data_ctrl(channel_info, &temp_info, &strmbuf_iova, &kernel_va);
	if (check_result) {
		VCODEC_ERR_VENC("check data ctrl failed");
		ctrl->next = 0;
		return -EINVAL;
	}

	/*
		only support send one layer ctrl info.
		if you want to modify it to support multiple slices and multiple layers,
		you need to consider the code security boundary issues.
	*/
	cur_layer_idx = channel_info->wireless_ldy_info.cur_layer_idx;
	layer_pack = &channel_info->wireless_ldy_info.layer_pack[cur_layer_idx];
	for (i = 0; i < channel_info->wireless_ldy_info.layer_num; i++) {
		if (i != cur_layer_idx)
			continue;

		ctrl->data_addr = kernel_va + pack_offset;
		ctrl->iova_addr = strmbuf_iova + pack_offset;
		ctrl->layer_number = temp_info.layer_num;
		ctrl->last_paket_len = temp_info.last_package_len;
		ctrl->normal_pack_length = temp_info.nomal_package_len;
		ctrl->paket_number = temp_info.package_num;
		ctrl->version = 0x25; // r2.5
		ctrl->qos_type = 0x1 | ((layer_pack->wireless_frame_no & 0x3f) << 8) |
			((channel_info->wireless_ldy_info.cur_slice_idx & 0xf) << 4) |
			(((channel_info->wireless_ldy_info.layer_num - 1) & 0x7) << 1);
		venc_modify_msdu_info(ctrl->data_addr, &temp_info, channel_info->wireless_ldy_info.layer_num,
			layer_pack->wireless_frame_no, i);
		venc_set_mac_info(channel_info, ctrl);

		ctrl->next = 0;
	}

	send_ts = (uint64_t *)(&head->reserve[0]);  // Timestamp before send
	*send_ts = osal_get_sys_time_in_us();
	return 0;
}

static void venc_drv_release_wireless_buf(struct channel_info *channel_info)
{
	struct layer_pack_info *layer_pack = NULL;
	unsigned long flags;
	uint16_t i;
	uint32_t j;
	struct encode_done_info encode_done_info = { 0 };
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	// get backup buffer
	venc_drv_get_backup_info(entry, &encode_done_info);
	// push to enc done queue
	venc_drv_fill_encode_done(entry, &encode_done_info);
	VCODEC_WARN_VENC("wifi cannot send data, push encode_done_info to enc_done_queue");

	if (channel_info->wireless_ldy_info.is_send_stream_header_to_wifi) {
		spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
		for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
			if (entry->stream_buf[i].shared_fd == channel_info->wireless_ldy_info.stream_header.sharefd)
				entry->stream_buf[i].status = IDLE;
		}
		spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
	} else {
		for (i = 0; i < channel_info->wireless_ldy_info.layer_num; i++) {
			layer_pack = &channel_info->wireless_ldy_info.layer_pack[i];
			spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
			for (j = 0; j < MAX_STRM_BUF_SIZE; j++) {
				if (entry->stream_buf[j].shared_fd == layer_pack->shared_fd)
					entry->stream_buf[j].status = IDLE;
			}
			spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
		}
	}
}

static int32_t venc_drv_send_data_to_wifi(struct encode_done_info *encode_done_info)
{
	int32_t ret;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();
	struct _tx_layer_ctrl *ctrl = NULL;
	struct channel_info *channel_info = &encode_done_info->channel_info;

	venc_drv_push_backup_info(entry, encode_done_info);

	ctrl = venc_drv_get_idle_ctrl(entry);
	if (!ctrl) {
		VCODEC_ERR_VENC("get idle ctrl failed");
		return -EFAULT;
	}


	if (channel_info->wireless_ldy_info.is_send_stream_header_to_wifi)
		ret = venc_set_head_ctrl(channel_info, ctrl);
	else
		ret = venc_set_data_ctrl(channel_info, entry, ctrl);
	if (ret) {
		VCODEC_ERR_VENC("set ctrl failed");
		venc_drv_release_idle_ctrl(entry, ctrl);
		venc_drv_release_wireless_buf(channel_info);
		return ret;
	}
	if (wifi_tx_venc_pkg(ctrl))
		return 0;

	venc_drv_release_idle_ctrl(entry, ctrl);
	venc_drv_release_wireless_buf(channel_info);
	VCODEC_FATAL_VENC("send data to wifi failed");
	return -EFAULT;
}

static bool venc_drv_need_send_xps(struct encode_done_info *encode_done_info)
{
	struct venc_wireless_entry *entry = venc_get_wireless_entry();
	if(encode_done_info->channel_info.wireless_ldy_info.is_send_stream_header_to_wifi) {
		entry->curr_xps_info = *encode_done_info;
		entry->curr_xps_existence_flag = true;
	}

	if (entry->curr_xps_existence_flag && entry->last_xps_feedback) {
		entry->last_xps_feedback = false;
		entry->send_xps_success_flag = false;
		entry->curr_xps_existence_flag = false;
		VCODEC_INFO_VENC("send new xps");
		return true;
	}
	return false;
}

static int32_t venc_thread_send_data_handler(void *data)
{
	struct venc_fifo_buffer *send_queue = (struct venc_fifo_buffer *)data;
	struct encode_done_info info = { 0 };
	struct venc_wireless_entry *entry = venc_get_wireless_entry();
	struct layer_pack_info *layer_pack = NULL;
	uint16_t cur_layer_idx;
	uint32_t i;
	unsigned long flags;

	if (queue_is_null(send_queue)) {
		VCODEC_ERR_VENC("send queue is invalid");
		return -EFAULT;
	}

	while (!queue_is_empty(send_queue)) {
		if (pop(send_queue, &info)) {
			VCODEC_ERR_VENC("read encode done info failed");
			return -EFAULT;
		}
		mutex_lock(&g_venc_send_recv_mutex);
		if (venc_drv_need_send_xps(&info)) // Check whether xps need be sent
			venc_drv_send_data_to_wifi(&entry->curr_xps_info);

		if(info.channel_info.wireless_ldy_info.is_send_stream_header_to_wifi) {
			mutex_unlock(&g_venc_send_recv_mutex);
			continue;
		}

		if (entry->send_xps_success_flag) { // Check whether the xps feedback is received successfully
			if (venc_drv_send_data_to_wifi(&info)) {
				VCODEC_ERR_VENC("send data to wifi failed");
				mutex_unlock(&g_venc_send_recv_mutex);
				return -EFAULT;
			}
		} else { // drop frame
			venc_drv_fill_encode_done(entry, &info);
			cur_layer_idx = info.channel_info.wireless_ldy_info.cur_layer_idx;
			layer_pack = &info.channel_info.wireless_ldy_info.layer_pack[cur_layer_idx];

			spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
			for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
				if (entry->stream_buf[i].shared_fd == layer_pack->shared_fd)
					entry->stream_buf[i].status = IDLE;
			}
			spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
		}
		mutex_unlock(&g_venc_send_recv_mutex);
	}
	return 0;
}

static int32_t venc_drv_wireless_create_thread(struct venc_wireless_entry *entry)
{
	int32_t ret;

	if (entry->pack_thread.task) {
		VCODEC_WARN_VENC("venc send package thread is exist");
		return -EBUSY;
	}

	if (!entry->send_queue) {
		VCODEC_ERR_VENC("send_queue is NULL");
		return -EINVAL;
	}
	entry->pack_thread.handler = venc_thread_send_data_handler;
	entry->pack_thread.data = (void *)entry->send_queue;

	ret = venc_drv_create_thread(&entry->pack_thread, "venc_send_package");
	if (ret)
		VCODEC_ERR_VENC("create venc send package thread failed");
	return ret;
}

static void wifi_tx_send_done(struct _send_result *send_result)
{
	uint32_t i;
	unsigned long flags;
	uint16_t cur_layer_idx;
	int32_t shared_fd;
	struct encode_done_info encode_done_info = { 0 };
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("venc is not inited");
		return;
	}

	if (!send_result || !send_result->slice_layer) {
		VCODEC_ERR_VENC("send_result or slice_layer is NULL");
		return;
	}

	// pop buffer
	venc_drv_get_backup_info(entry, &encode_done_info);
	// return ctrl buffer
	venc_drv_release_idle_ctrl(entry, send_result->slice_layer);
	// push fifo buffer
	venc_drv_fill_encode_done(entry, &encode_done_info);

	cur_layer_idx = encode_done_info.channel_info.wireless_ldy_info.cur_layer_idx;
	if (send_result->slice_info & 0x80)
		shared_fd = encode_done_info.channel_info.wireless_ldy_info.stream_header.sharefd;
	else
		shared_fd = encode_done_info.channel_info.wireless_ldy_info.layer_pack[cur_layer_idx].shared_fd;

	for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
		if (entry->stream_buf[i].shared_fd == shared_fd) {
			spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
			entry->stream_buf[i].status = IDLE;
			spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
			break;
		}
	}
}

static void wifi_tx_pkg_xps_feedback_done(struct _send_result *send_result)
{
	struct wifi_feedback_info feedback_info = { 0 };
	struct venc_wireless_entry *entry = venc_get_wireless_entry();
	mutex_lock(&g_venc_send_recv_mutex);
	entry->last_xps_feedback = true;
	if (send_result->layer_suc_num[0] > 0) {
		entry->send_xps_success_flag = true;
		feedback_info.send_status = SEND_XPS_STATUS_SUCESS;
		entry->retry_send_xps_count = RETRY_SEND_XPS_TIMES;
		VCODEC_INFO_VENC("get stream header, send result success");
	} else if (send_result->layer_suc_num[0] == 0) {
		VCODEC_ERR_VENC("get stream header, send result failed, need send retry");
		feedback_info.send_status = SEND_XPS_STATUS_FAILED;
		if (entry->retry_send_xps_count > 0) {
			feedback_info.send_status = SEND_XPS_STATUS_RETRY;
			entry->retry_send_xps_count--;
		}
		entry->send_xps_success_flag = false;
	}
	mutex_unlock(&g_venc_send_recv_mutex);

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->file_private != NULL)
		venc_drv_fill_feedback(entry->file_private->feedback_queue, &feedback_info);
	mutex_unlock(&g_venc_wireless_mutex);
}

static void wifi_tx_pkg_done(struct _send_result *send_result)
{
	struct wifi_feedback_info feedback_info = { 0 };
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (!send_result) {
		VCODEC_ERR_VENC("send result is NULL");
		return;
	}
	if (send_result->slice_info & 0x80) {
		wifi_tx_pkg_xps_feedback_done(send_result);
		return;
	}

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("venc is not inited");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	if (!entry->file_private) {
		VCODEC_ERR_VENC("file_private is NULL");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	feedback_info.channel_id = entry->channel_id;
	feedback_info.frame_idx = send_result->slice_info & 0x3f;
	feedback_info.slice_idx = (send_result->slice_info >> 12) & 0xf;
	feedback_info.layer_idx = venc_get_max_layer_suc(send_result);
	feedback_info.is_tx_or_rx = (send_result->slice_info >> 6) & 0x1;
	feedback_info.slice_layer0_max = send_result->slice_layer0_max;
	feedback_info.slice_layerall_max = send_result->slice_layerall_max;
	feedback_info.rtt_delay = venc_get_rtt_delay(send_result);
	feedback_info.send_status = 0;
	venc_drv_fill_feedback(entry->file_private->feedback_queue, &feedback_info);
	mutex_unlock(&g_venc_wireless_mutex);
}

void venc_drv_wireless_push_sendbuf(struct encode_done_info *info)
{
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (IS_ERR_OR_NULL(entry->pack_thread.task)) {
		VCODEC_ERR_VENC("thread task is not exsit");
		return;
	}

	venc_drv_push_send_info(entry, info);
	VENC_SET_EVENT(entry->pack_thread.lock, entry->pack_thread.condition++);
	wake_up(&entry->pack_thread.wait_queue);
}

static void venc_drv_unmap_wireless_all(void)
{
	uint32_t i;
	int32_t ret;
	int32_t wait_count = 0;
	unsigned long flags;
	struct platform_device *venc_dev = NULL;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
		if (entry->stream_buf[i].shared_fd < 0 || !entry->stream_buf[i].dma_buf)
			continue;

		wait_count = 0;
		do {
			if (entry->stream_buf[i].status != OWNED_BY_WIFI)
				break;
			if (wait_count == WAIT_RELEASE_TIMEOUT_COUNT) {
				VCODEC_ERR_VENC("venc wireless wait owned timeout");
				break;
			}
			wait_count++;
			vcodec_sleep_ms(WAIT_WIFI_RELEASE_TIMEOUT);
		} while (entry->stream_buf[i].status == OWNED_BY_WIFI);

		venc_dev = venc_get_device();
		ret = kernel_iommu_unmap_dmabuf(
			&venc_dev->dev, entry->stream_buf[i].dma_buf, (unsigned long)entry->stream_buf[i].buf_iova);
		if (ret)
			VCODEC_FATAL_VENC("%s: kernel_iommu_unmap_dmabuf failed", __func__);

		if (unmap_wifi_iova(entry->stream_buf[i].dma_buf))
			VCODEC_WARN_VENC("unmap wifi iova failed");

		venc_kunmap_kernel_va(entry->stream_buf[i].dma_buf);
		entry->stream_buf[i].shared_fd = -1;
		entry->stream_buf[i].dma_buf = NULL;
		entry->stream_buf[i].wifi_iova = 0;
		entry->stream_buf[i].kernel_va = 0;
		entry->stream_buf[i].buf_iova = 0;
		entry->stream_buf[i].buf_size = 0;
		spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
		entry->stream_buf[i].status = IDLE;
		spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
	}
}

static int32_t venc_drv_map_wireless(int32_t shared_fd)
{
	uint32_t indx;
	uint64_t wifi_iova;
	uint64_t kernel_va;
	uint64_t iova_addr;
	struct dma_buf *dmabuf = NULL;
	unsigned long flags;
	unsigned long iova_size = 0;
	struct platform_device *venc_dev = NULL;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (shared_fd < 0) {
		VCODEC_ERR_VENC("shared_fd(%d)", shared_fd);
		return -EFAULT;
	}

	for (indx = 0; indx < MAX_STRM_BUF_SIZE; indx++) {
		/* shared fd has been kmapped, no need to map again */
		if (entry->stream_buf[indx].shared_fd == shared_fd) {
			spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
			if (entry->stream_buf[indx].status != IDLE) {
				VCODEC_ERR_VENC("status is not IDLE, can't change");
				spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
				return -EFAULT;
			}
			spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
			return 0;
		}
	}

	dmabuf = venc_kmap_kernel_va(shared_fd, &kernel_va);
	if (!dmabuf) {
		VCODEC_FATAL_VENC("dmabuf is NULL");
		return -EFAULT;
	}

	if (map_wifi_iova(dmabuf, &wifi_iova)) {
		venc_kunmap_kernel_va(dmabuf);
		VCODEC_FATAL_VENC("get wifi iova failed");
		return -EFAULT;
	}


	for (indx = 0; indx < MAX_STRM_BUF_SIZE; indx++) {
		if (entry->stream_buf[indx].dma_buf == NULL)
			break;
	}
	if (indx >= MAX_STRM_BUF_SIZE) {
		VCODEC_ERR_VENC("The stream buffer is full.\n");
		unmap_wifi_iova(dmabuf);
		venc_kunmap_kernel_va(dmabuf);
		return -EFAULT;
	}

	venc_dev = venc_get_device();
	iova_addr = kernel_iommu_map_dmabuf(&venc_dev->dev, dmabuf, 0, &iova_size);
	if (!iova_addr) {
		VCODEC_FATAL_VENC("%s, iommu map dmabuf failed", __func__);
		unmap_wifi_iova(dmabuf);
		venc_kunmap_kernel_va(dmabuf);
		return -EFAULT;
	}

	entry->stream_buf[indx].shared_fd = shared_fd;
	entry->stream_buf[indx].wifi_iova = wifi_iova;
	entry->stream_buf[indx].kernel_va = kernel_va;
	entry->stream_buf[indx].dma_buf = dmabuf;
	entry->stream_buf[indx].buf_iova = (uint32_t)iova_addr;
	entry->stream_buf[indx].buf_size = (uint32_t)iova_size;

	spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
	entry->stream_buf[indx].status = OWNED_BY_VENC;
	spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
	return 0;
}

void venc_drv_unmap_wireless(int32_t shared_fd)
{
	uint32_t i;
	int32_t wait_count = 0;
	int32_t ret;
	unsigned long flags;
	struct platform_device *venc_dev = NULL;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	if (shared_fd < 0) {
		VCODEC_ERR_VENC("shared_fd(%d)", shared_fd);
		return;
	}

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("venc is not inited");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	for (i = 0; i < MAX_STRM_BUF_SIZE; i++) {
		if (shared_fd == entry->stream_buf[i].shared_fd)
			break;
	}
	if (i == MAX_STRM_BUF_SIZE) {
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	do {
		if (entry->stream_buf[i].status != OWNED_BY_WIFI)
			break;
		if (wait_count == WAIT_RELEASE_TIMEOUT_COUNT) {
			VCODEC_ERR_VENC("venc wireless wait owned timeout");
			break;
		}
		wait_count++;
		vcodec_sleep_ms(WAIT_WIFI_RELEASE_TIMEOUT);
	} while (entry->stream_buf[i].status == OWNED_BY_WIFI);

	venc_dev = venc_get_device();
	ret = kernel_iommu_unmap_dmabuf(
		&venc_dev->dev, entry->stream_buf[i].dma_buf, (unsigned long)entry->stream_buf[i].buf_iova);
	if (ret)
		VCODEC_FATAL_VENC("%s: kernel_iommu_unmap_dmabuf failed", __func__);

	if (unmap_wifi_iova(entry->stream_buf[i].dma_buf))
		VCODEC_WARN_VENC("unmap wifi iova failed");

	venc_kunmap_kernel_va(entry->stream_buf[i].dma_buf);

	entry->stream_buf[i].shared_fd = -1;
	entry->stream_buf[i].wifi_iova = 0;
	entry->stream_buf[i].kernel_va = 0;
	entry->stream_buf[i].dma_buf = NULL;
	entry->stream_buf[i].buf_iova = 0;
	entry->stream_buf[i].buf_size = 0;
	spin_lock_irqsave(&g_venc_wireless_buffer_spinlock, flags);
	entry->stream_buf[i].status = IDLE;
	spin_unlock_irqrestore(&g_venc_wireless_buffer_spinlock, flags);
	mutex_unlock(&g_venc_wireless_mutex);
	return;
}

static void venc_drv_free_ctrl_pool(struct venc_wireless_entry *entry)
{
	int32_t i;

	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		if (entry->ctrl_pool[i])
			vcodec_mem_vfree(entry->ctrl_pool[i]);
		entry->ctrl_pool[i] = NULL;
	}
}

static int32_t venc_drv_alloc_ctrl_pool(struct venc_wireless_entry *entry)
{
	int32_t i;

	for (i = 0; i < MAX_QUEUE_SIZE; i++) {
		entry->ctrl_pool[i] = vcodec_mem_valloc(sizeof(struct _tx_layer_ctrl));
		if (!entry->ctrl_pool[i]) {
			VCODEC_FATAL_VENC("call vmalloc failed");
			venc_drv_free_ctrl_pool(entry);
			return -ENOMEM;
		}
	}

	return 0;
}

int32_t venc_drv_wireless_init(unsigned int cmd, struct venc_file_private *file_private)
{
	int32_t ret;
	int32_t index;
	struct vcodec_venc_callback venc_callback;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENTY_INITED) {
		VCODEC_ERR_VENC("entry init_status error %d", entry->init_status);
		mutex_unlock(&g_venc_wireless_mutex);
		return -EFAULT;
	}

	ret = venc_drv_alloc_ctrl_pool(entry);
	if (ret) {
		VCODEC_FATAL_VENC("create wireless queue failed");
		mutex_unlock(&g_venc_wireless_mutex);
		return ret;
	}

	ret = venc_drv_wireless_create_queue(entry, file_private->encode_done_info);
	if (ret) {
		VCODEC_FATAL_VENC("create wireless queue failed");
		goto create_queue_error;
	}

	ret = venc_drv_wireless_create_thread(entry);
	if (ret) {
		VCODEC_FATAL_VENC("create wireless thread failed");
		goto create_thread_error;
	}

	for (index = 0; index < MAX_STRM_BUF_SIZE; index++)
		entry->stream_buf[index].shared_fd = -1;

	entry->file_private = file_private;
	entry->init_status = VENC_WIRELESS_ENC_INITED;
	entry->retry_send_xps_count = RETRY_SEND_XPS_TIMES;
	entry->last_xps_feedback = true;
	entry->send_xps_success_flag = false;
	mutex_unlock(&g_venc_wireless_mutex);

	venc_callback.wifi_tx_pkg_done = wifi_tx_pkg_done,
	venc_callback.wifi_tx_send_done = wifi_tx_send_done,
	venc_set_callback(&venc_callback);

	VCODEC_INFO_VENC("init venc wireless_lowdelay successfully");
	return 0;

create_thread_error:
	venc_drv_wireless_destroy_queue(entry);
create_queue_error:
	venc_drv_free_ctrl_pool(entry);

	mutex_unlock(&g_venc_wireless_mutex);
	return ret;
}

void venc_wireless_init_entry(void)
{
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENTY_UNINIT) {
		VCODEC_ERR_VENC("entry already inited");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	(void)memset_s(entry, sizeof(struct venc_wireless_entry), 0, sizeof(struct venc_wireless_entry));

	init_waitqueue_head(&entry->pack_thread.wait_queue);
	spin_lock_init(&entry->pack_thread.lock);

	entry->init_status = VENC_WIRELESS_ENTY_INITED;
	mutex_unlock(&g_venc_wireless_mutex);
	VCODEC_INFO_VENC("init enc wireless entry successfully");
}

void venc_drv_wireless_deinit(void)
{
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("entry init_status error %d", entry->init_status);
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	venc_drv_destroy_thread(&entry->pack_thread);
	venc_drv_wireless_destroy_queue(entry);
	venc_drv_free_ctrl_pool(entry);
	venc_drv_unmap_wireless_all();

	entry->file_private = NULL;
	entry->init_status = VENC_WIRELESS_ENTY_INITED;
	mutex_unlock(&g_venc_wireless_mutex);
	VCODEC_INFO_VENC("deinit venc wireless_lowdelay successfully");
}

void venc_wireless_deinit_entry(void)
{
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status == VENC_WIRELESS_ENTY_UNINIT) {
		VCODEC_ERR_VENC("entry not init");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	if (entry->init_status == VENC_WIRELESS_ENC_INITED)
		venc_drv_wireless_deinit();

	entry->file_private = NULL;
	entry->init_status = VENC_WIRELESS_ENTY_UNINIT;
	mutex_unlock(&g_venc_wireless_mutex);
	VCODEC_INFO_VENC("deinit enc wireless entry successfully");
}

int32_t venc_drv_wireless_check_and_map(struct encode_info *encode_info)
{
	int32_t ret = 0;
	int32_t i;
	uint16_t cur_layer_idx;
	uint16_t layer_num;
	struct wireless_lowdelay_info *info = &encode_info->channel.wireless_ldy_info;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	layer_num = info->layer_num;
	cur_layer_idx = info->cur_layer_idx;
	if ((cur_layer_idx >= WIRELESS_LOWDELAY_MAX_LAYER) && (layer_num != (cur_layer_idx + 1))) {
		VCODEC_ERR_VENC("invalid layer idx: %u", cur_layer_idx);
		return -EINVAL;
	}

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("venc is not inited");
		mutex_unlock(&g_venc_wireless_mutex);
		return -EFAULT;
	}

	if (info->pack_enable)
		ret = venc_drv_map_wireless(info->layer_pack[cur_layer_idx].shared_fd);

	for (i = 0; i < WIRELESS_LOWDELAY_MAX_LAYER; i++) {
		if (i == cur_layer_idx)
			continue;
		(void)memset_s(&info->layer_pack[i], sizeof(struct layer_pack_info), 0, sizeof(struct layer_pack_info));
		info->layer_pack[i].shared_fd = -1;
	}
	mutex_unlock(&g_venc_wireless_mutex);
	return ret;
}

int32_t venc_drv_wireless_get_feedback_info(struct venc_file_private *file_private,
	struct wifi_feedback_info *feedback_info)
{
	int32_t ret;
	struct venc_wireless_entry *entry = venc_get_wireless_entry();
	struct venc_feedback_queue *feedback_queue = NULL;

	if (!feedback_info || !file_private) {
		VCODEC_ERR_VENC("feedback_info or file_private is NULL");
		return -EINVAL;
	}

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED) {
		VCODEC_ERR_VENC("venc is not inited");
		mutex_unlock(&g_venc_wireless_mutex);
		return -EFAULT;
	}

	if (!file_private->feedback_queue) {
		VCODEC_ERR_VENC("file_private is NULL or feedback_info is NULL");
		mutex_unlock(&g_venc_wireless_mutex);
		return -EFAULT;
	}
	feedback_queue = file_private->feedback_queue;
	mutex_unlock(&g_venc_wireless_mutex);

	ret = wait_queue_filled(feedback_queue);
	if (ret) {
		VCODEC_ERR_VENC("wait buffer filled failed");
		return ret;
	}

	ret = pop(feedback_queue, feedback_info);
	if (ret) {
		VCODEC_ERR_VENC("read feedback info failed");
		return ret;
	}

	return 0;
}

int32_t venc_drv_wireless_check_file_private(struct venc_file_private *file_private)
{
	struct venc_wireless_entry *entry = venc_get_wireless_entry();

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->file_private == NULL) {
		mutex_unlock(&g_venc_wireless_mutex);
		return -EINVAL;
	}

	if (entry->file_private != file_private) {
		mutex_unlock(&g_venc_wireless_mutex);
		return -EINVAL;
	}
	mutex_unlock(&g_venc_wireless_mutex);

	return EOK;
}

static void venc_drv_wireless_reinit_data(struct wireless_lowdelay_info *wireless_info)
{
	int32_t i;

	(void)memset_s(
		wireless_info->enhance_layer, sizeof(wireless_info->enhance_layer), 0, sizeof(wireless_info->enhance_layer));
	(void)memset_s(
		wireless_info->layer_pack, sizeof(wireless_info->layer_pack), 0, sizeof(wireless_info->layer_pack));
	(void)memset_s(wireless_info->slice_ts, sizeof(wireless_info->slice_ts), 0, sizeof(wireless_info->slice_ts));
	for (i = 0; i < WIRELESS_LOWDELAY_MAX_LAYER; i++)
		wireless_info->layer_pack[i].shared_fd = -1;

	wireless_info->slice_num = 0;
	wireless_info->layer_num = 0;
	wireless_info->cur_slice_idx = 0;
	wireless_info->cur_layer_idx = 0;
	wireless_info->width = 0;
	wireless_info->front_slice_height = 0;
}

void venc_drv_wireless_send_head(struct encode_info *encode_info)
{
	int32_t ret;
	struct encode_done_info info = { 0 };
	struct venc_wireless_entry *entry = venc_get_wireless_entry();
	struct wireless_lowdelay_info *wireless_info = NULL;

	mutex_lock(&g_venc_wireless_mutex);
	if (entry->init_status != VENC_WIRELESS_ENC_INITED || entry->file_private == NULL) {
		VCODEC_ERR_VENC("venc is not inited or file private is NULL");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	wireless_info = &encode_info->channel.wireless_ldy_info;
	venc_drv_wireless_reinit_data(wireless_info);

	VCODEC_INFO_VENC("send stream header");
	entry->channel_id = encode_info->channel.id;
	ret = memcpy_s(&info.channel_info, sizeof(struct channel_info),
		&encode_info->channel, sizeof(struct channel_info));
	if (ret) {
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	if (!wireless_info->pack_enable) {
		ret = push(entry->file_private->encode_done_info, &info);
		if (ret)
			VCODEC_ERR_VENC("push headbuffer failed");
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	if (venc_drv_map_wireless(info.channel_info.wireless_ldy_info.stream_header.sharefd)) {
		mutex_unlock(&g_venc_wireless_mutex);
		return;
	}

	venc_drv_wireless_push_sendbuf(&info);
	mutex_unlock(&g_venc_wireless_mutex);
}