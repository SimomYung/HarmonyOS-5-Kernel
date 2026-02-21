/*
 * vcodec_vdec_ioctl_mcu.c
 *
 * This is ioctl implement when use MCU
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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
#include "vcodec_types.h"
#include "vfmw.h"
#include "vfmw_ext.h"
#include "vcodec_vdec.h"
#include "vfmw_ipc.h"
#include "vfmw_queue.h"
#include "dev_mgr/mcu/vcodec_mcu_ipc.h"
#include "vcodec_osal.h"

struct mcore_dump_buf_info {
	uint32_t clear_sign;
	uint32_t buffer_size;
	char *buffer;
};

struct buffer_vctrl {
	uint32_t buffer_num;
	int32_t frm_share_fd[MAX_FRAME_NUM];
	uint32_t frm_phy_addr[MAX_FRAME_NUM];
	uint32_t frm_buf_size[MAX_FRAME_NUM];
	int32_t pmv_share_fd;
	uint32_t pmv_phy_addr;
	uint32_t pmv_buf_size;
	uint32_t ext_len;
	uint8_t ext_data[MAX_BUF_VCTRL_PRM_LEN];
};

static const char *vctrl_cmd_name(vfmw_cid cmd)
{
	const char *name[] = {
		"VFMW_CID_CREATE_CHAN",            /* create channel */
		"VFMW_CID_DESTROY_CHAN",           /* destroy channel */
		"VFMW_CID_CFG_CHAN",               /* config the decode channel */
		"VFMW_CID_START_CHAN",             /* start channel */
		"VFMW_CID_STOP_CHAN",              /* stop channel */
		"VFMW_CID_SET_STREAM_INTF",        /* set the stream access interface for the decode channel */
		"VFMW_CID_RELEASE_STREAM",         /* reset scd to release stream buffers */
		"VFMW_CID_RESET_CHAN_WITH_OPTION", /* reset channel with options to keep some characters of the channel */
		"VFMW_CID_EXTRA_WAKEUP_THREAD",   /* bvt set wake up vfmw thread */
		"VFMW_CID_ALLOC_MEM_TO_CHANNEL",
		"VFMW_CID_SET_FRAME_MEM",
		"VFMW_CID_FLUSH_CHAN",
		"VFMW_CID_SYNC_EXT_BUFFER",
		"VFMW_CID_ACTIVATE_CHANNEL",
		"VFMW_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION",
		"VFMW_CMD_IPC_SUSPEND",
		"VFMW_CMD_IPC_RESUME",
		"VFMW_CID_MCORE_SUSPEND",
		"VFMW_CID_MCORE_RESUME",
	};
	if (cmd >= sizeof(name) / sizeof(name[0])) {
		return "VFMW_CID_UNKNOWN";
	}
	return name[cmd];
}

static int32_t vcodec_vdec_validate_ioctl_msg(const char* cmd_name,
	vdec_ioctl_msg *vdec_msg, size_t in_size, size_t out_size)
{
	dprint(PRN_DBG, "Process: %s", cmd_name);
	if (!vdec_msg) {
		dprint(PRN_FATAL, "%s: parameter is NULL", cmd_name);
		return -EINVAL;
	}
	if (in_size) {
		if (!vdec_msg->in) {
			dprint(PRN_FATAL, "%s: input parameter is NULL", cmd_name);
			return -EINVAL;
		}
		if (vdec_msg->in_size != in_size) {
			dprint(PRN_FATAL, "%s: input parameter is invalid", cmd_name);
			return -EINVAL;
		}
	}

	if (out_size) {
		if (!vdec_msg->out) {
			dprint(PRN_FATAL, "%s: output parameter is NULL", cmd_name);
			return -EINVAL;
		}
		if (vdec_msg->out_size != out_size) {
			dprint(PRN_FATAL, "%s: output parameter is invalid", cmd_name);
			return -EINVAL;
		}
	}
	return 0;
}

static int32_t handle_iommu_map(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg)
{
	int32_t ret;
	struct vdec_buffer_record buf_record = {0};
	vdec_entry *vdec = vdec_get_entry_by_file_handle(file_handle);
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_fd_list *list = &handle->fdlist;
	struct dma_buf *dmabuf = VCODEC_NULL;
	check_return(vdec, "vdec is null");

	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IOMMU_MAP",
		vdec_msg, sizeof(buf_record), sizeof(buf_record)))
		return -EINVAL;

	if (copy_from_user(&buf_record, vdec_msg->in, sizeof(buf_record))) {
		dprint(PRN_FATAL, "VDEC_IOCTL_IOMMU_MAP: copy_from_user failed");
		return -EFAULT;
	}

	ret = vdec_mem_iommu_map((void *)vdec->device, &buf_record, file_handle, &dmabuf);
	if (ret) {
		dprint(PRN_FATAL, "VDEC_IOCTL_IOMMU_MAP failed");
		return ret;
	}

	ret = vdec_share_fd_list_add(list, buf_record.share_fd, dmabuf, buf_record.iova, buf_record.iova_size);
	if (ret) {
		dprint(PRN_FATAL, "vdec_share_fd_list_add failed");
		(void)vdec_mem_iommu_unmap((void *)vdec->device, buf_record.share_fd, file_handle,
			dmabuf, buf_record.iova);
		return ret;
	}

	/* Returns a fake iova to the user mode when allocating the buffer,
	   and configures the real iova when in use. */
	buf_record.iova = 0xFFFFFFFF;
	if (copy_to_user(vdec_msg->out, &buf_record, sizeof(buf_record))) {
		dprint(PRN_FATAL, "VDEC_IOCTL_IOMMU_MAP: copy_to_user failed");
		(void)vdec_share_fd_list_remove(list, buf_record.share_fd, &dmabuf, &buf_record.iova);
		(void)vdec_mem_iommu_unmap((void *)vdec->device, buf_record.share_fd, file_handle,
			dmabuf, buf_record.iova);
		return -EFAULT;
	}
	return ret;
}

static int32_t handle_iommu_unmap(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg)
{
	int32_t ret;
	struct vdec_buffer_record buf_record;
	vdec_entry *vdec = vdec_get_entry_by_file_handle(file_handle);
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_fd_list *list = &handle->fdlist;
	struct dma_buf *dmabuf = VCODEC_NULL;
	uint32_t iova_addr;
	check_return(vdec, "vdec is null");

	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IOMMU_UNMAP",
		vdec_msg, sizeof(buf_record), 0))
		return -EINVAL;

	if (copy_from_user(&buf_record, vdec_msg->in, sizeof(buf_record))) {
		dprint(PRN_FATAL, "VDEC_IOCTL_IOMMU_UNMAP: copy_from_user failed");
		return -EFAULT;
	}

	ret = vdec_share_fd_list_remove(list, buf_record.share_fd, &dmabuf, &iova_addr);
	if (ret) {
		dprint(PRN_FATAL, "vdec_share_fd_list_remove failed");
		return ret;
	}

	ret = vdec_mem_iommu_unmap((void *)vdec->device, buf_record.share_fd, file_handle,
		dmabuf, iova_addr);
	if (ret) {
		dprint(PRN_FATAL, "VDEC_IOCTL_IOMMU_UNMAP failed");
		return ret;
	}
	return 0;
}

static int32_t set_frame_store(struct file *file_handle, ipc_vctrl_ioctl *vctrl_info)
{
	int32_t ret;
	uint32_t buf_num;
	uint32_t cnt;
	uint32_t iova_addr;
	uint32_t iova_size;
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_fd_list *list = &handle->fdlist;
	struct buffer_vctrl *buffer_vctrl_info;

	if (vctrl_info->paramLen != sizeof(struct buffer_vctrl))
		return -EINVAL;

	buffer_vctrl_info = (struct buffer_vctrl *)vctrl_info->args;
	if (buffer_vctrl_info->buffer_num > MAX_FRAME_NUM)
		return -ENOMEM;

	buf_num = buffer_vctrl_info->buffer_num;
	for (cnt = 0; cnt < buf_num; cnt++) {
		ret = vdec_share_fd_list_get(list, buffer_vctrl_info->frm_share_fd[cnt], &iova_addr, &iova_size);
		if (ret) {
			dprint(PRN_ERROR, "get frame buffer failed");
			return ret;
		}
		buffer_vctrl_info->frm_phy_addr[cnt] = iova_addr;
		buffer_vctrl_info->frm_buf_size[cnt] = iova_size;
	}

	ret = vdec_share_fd_list_get(list, buffer_vctrl_info->pmv_share_fd, &iova_addr, &iova_size);
	if (ret) {
		dprint(PRN_ERROR, "get pmv buffer failed");
		return ret;
	}
	buffer_vctrl_info->pmv_phy_addr = iova_addr;
	buffer_vctrl_info->pmv_buf_size = iova_size;

	return 0;
}

static int32_t handle_ipc_vctl_cmd(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg)
{
	int32_t ret = -EINVAL;
	vdec_entry *vdec = vdec_get_entry();
	ipc_vctrl_ioctl *vctrl_info;
	struct ipc_vctrl_msg msg;
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	ipc_vctrl_ret_mcu_to_acpu vctrl_ret = {0};
	struct vdec_chan_list *chan_list = &handle->chanlist;

	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IPC_VCTL_CMD",
		vdec_msg, sizeof(ipc_vctrl_ioctl), sizeof(ipc_vctrl_ret_mcu_to_acpu)))
		return -EINVAL;

	vctrl_info = vmalloc(sizeof(ipc_vctrl_ioctl));
	if (!vctrl_info) {
		dprint(PRN_FATAL, "alloc memory failed");
		ret = -ENOMEM;
		goto exit;
	}
	if (copy_from_user(vctrl_info, vdec_msg->in, sizeof(ipc_vctrl_ioctl))) {
		dprint(PRN_FATAL, "copy_from_user failed");
		ret = -EFAULT;
		goto mem_error;
	}
	dprint(PRN_ALWS, "chan %u cmd %s", vctrl_info->chanId, vctrl_cmd_name(vctrl_info->cmdId));

	if (vctrl_info->chanId < VFMW_NO_SEC_CHAN_START || vctrl_info->chanId >= VFMW_CHAN_ID_MAX) {
		dprint(PRN_FATAL, "chanid error %u range is [2, 17]", vctrl_info->chanId);
		ret = -EFAULT;
		goto mem_error;
	}

	if (vctrl_info->cmdId == VFMW_CID_SET_FRAME_MEM) {
		ret = set_frame_store(file_handle, vctrl_info);
		if (ret) {
			dprint(PRN_FATAL, "config output buffer iova failed %d", ret);
			goto mem_error;
		}
		if (!vdec_is_async_open_success(vdec)) {
			dprint(PRN_ERROR, "async open state is not success");
			ret = -EFAULT;
			goto mem_error;
		}
	}

	msg.input = vctrl_info;
	msg.output = &vctrl_ret;
	if (!vdec_chan_list_get(chan_list, vctrl_info->chanId)) {
		dprint(PRN_FATAL, "cmdId %d chanid %d is ilegal", vctrl_info->cmdId, vctrl_info->chanId);
		ret = -EFAULT;
		goto mem_error;
	}

	ret = vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_SEND_CTRL_MSG, &msg, sizeof(struct ipc_vctrl_msg));
	if (ret == IPC_TIMEOUT && (vctrl_info->cmdId == VFMW_CID_STOP_CHAN
		|| vctrl_info->cmdId == VFMW_CID_RELEASE_STREAM || vctrl_info->cmdId == VFMW_CID_RESET_CHAN_WITH_OPTION)) {
		dprint(PRN_FATAL, "ipc timeout but return ok");
		ret = EOK;
	}

	if (ret || vctrl_ret.res) {
		dprint(PRN_FATAL, "send ctrl msg failed %d res is %d", ret, vctrl_ret.res);
		goto error_put_chan;
	}

	if (copy_to_user(vdec_msg->out, &vctrl_ret, sizeof(vctrl_ret))) {
		dprint(PRN_FATAL, "copy_to_user failed");
		ret = -EFAULT;
	}
	dprint(PRN_CTRL, "end VDEC_IOCTL_IPC_VCTL_CMD");

error_put_chan:
	vdec_chan_list_put(chan_list, vctrl_info->chanId);
mem_error:
	vfree(vctrl_info);
exit:
	return ret;
}

static void handle_add_chanlist_failed(vdec_entry *vdec, ipc_vctrl_ioctl *vctrl_info, const int32_t chan_id)
{
	struct ipc_vctrl_msg msg;
	ipc_vctrl_ret_mcu_to_acpu vctrl_ret = {
		.res = -1,
		.chan_id = -1,
		.iova = 0,
	};

	vctrl_info->chanId = (uint32_t)chan_id;
	vctrl_info->cmdId = VFMW_CID_DESTROY_CHAN;
	// the paramLen of destroying chan is 0
	vctrl_info->paramLen = 0;

	msg.input = vctrl_info;
	msg.output = &vctrl_ret;
	(void)vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_SEND_CTRL_MSG, &msg, sizeof(struct ipc_vctrl_msg));
}

static int32_t handle_ipc_create_chan(struct file *file_handle, vdec_ioctl_msg *vdec_msg)
{
	int32_t ret, queue_id;
	uint32_t iova;
	uint64_t temp;
	vdec_entry *vdec = vdec_get_entry();
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_chan_list *chan_list = &handle->chanlist;
	ipc_vctrl_ioctl *vctrl_info;
	struct ipc_vctrl_msg chan_msg;
	struct ipc_segbuf_msg segbuf_msg;
	ipc_vctrl_ret_mcu_to_acpu chan_ret = {.res = -1, .chan_id = -1, .iova = 0};
	ipc_vctrl_ret_mcu_to_acpu segbuf_ret = {.res = -1, .chan_id = -1, .iova = 0};

	dprint(PRN_ALWS, "begin VDEC_IOCTL_IPC_CREATE_CHAN");
	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IPC_CREATE_CHAN",
		vdec_msg, sizeof(ipc_vctrl_ioctl), sizeof(ipc_vctrl_ret_mcu_to_acpu)))
		return -EINVAL;

	vctrl_info = vmalloc(sizeof(ipc_vctrl_ioctl));
	if (!vctrl_info) {
		dprint(PRN_FATAL, "alloc memory failed");
		ret = -ENOMEM;
		goto exit;
	}

	if (copy_from_user(vctrl_info, vdec_msg->in, sizeof(ipc_vctrl_ioctl))) {
		dprint(PRN_FATAL, "copy_from_user failed");
		ret = -EFAULT;
		goto mem_error;
	}

	if (vctrl_info->cmdId != VFMW_CID_CREATE_CHAN) {
		dprint(PRN_FATAL, "cmd %s not VFMW_CID_CREATE_CHAN", vctrl_cmd_name(vctrl_info->cmdId));
		ret = -EFAULT;
		goto mem_error;
	}

	temp = vcodec_osal_get_time_in_us();
	ret = vdec_alloc_segbuffer(&iova);
	if (ret) {
		dprint(PRN_FATAL, "vdec alloc segbuffer failed");
		goto mem_error;
	}

	dprint(PRN_ALWS, "alloc segbuf spend time: %lld us", vcodec_osal_get_time_in_us() - temp);
	chan_msg.input = vctrl_info;
	chan_msg.output = &chan_ret;

	mutex_lock(&handle->chan_create_lock);
	ret = vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_SEND_CTRL_MSG, &chan_msg, sizeof(chan_msg));
	if (ret || chan_ret.res) {
		dprint(PRN_FATAL, "send ctrl msg failed %d res is %d", ret, chan_ret.res);
		ret = -EFAULT;
		goto mcu_create_chan_error;
	}

	if (chan_ret.chan_id < VFMW_NO_SEC_CHAN_START || chan_ret.chan_id >= VFMW_CHAN_ID_MAX) {
		dprint(PRN_FATAL, "chan id error %d range is [2, 17]", chan_ret.chan_id);
		ret = -EFAULT;
		goto mcu_create_chan_error;
	}

	// chan_id 2-17 map to 0-15 shared output event queue buffer id
	queue_id = chan_ret.chan_id - VFMW_NO_SEC_CHAN_START;
	/* init output event queue of the corresponding channel to prevent event residue,
	   this must be done before adding chan to chanlist to avoid event report thread acquiring the event from queue */
	(void)vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_INIT_OUTPUT_EVENT_QUEUE, &queue_id, sizeof(int32_t));

	if (vdec_chan_list_add(chan_list, chan_ret.chan_id)) {
		dprint(PRN_FATAL, "add chan_id % to list failed", chan_ret.chan_id);
		ret = -EFAULT;
		goto add_chan_list_error;
	}

	segbuf_msg.chan_id = chan_ret.chan_id;
	segbuf_msg.iova = iova;
	segbuf_msg.output = &segbuf_ret;
	ret = vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_ASSIGN_SEG_BUFFER, &segbuf_msg, sizeof(segbuf_msg));
	if (ret || segbuf_ret.res) {
		dprint(PRN_ERROR, "assign seg buffer to chan_id:%d failed", segbuf_msg.chan_id);
		ret = -EFAULT;
		goto assign_segbuffer_failed;
	}

	vdec->vdec_chan_state[chan_ret.chan_id] = VFMW_CID_CREATE_CHAN;
	if (copy_to_user(vdec_msg->out, &chan_ret, sizeof(chan_ret))) {
		dprint(PRN_FATAL, "copy_to_user failed");
		ret = -EFAULT;
		goto copy_to_user_failed;
	}

	mutex_unlock(&handle->chan_create_lock);
	dprint(PRN_CTRL, "end VDEC_IOCTL_IPC_CREATE_CHAN");
	vfree(vctrl_info);
	return ret;
copy_to_user_failed:
	vdec->vdec_chan_state[chan_ret.chan_id] = VFMW_CID_MAX;
assign_segbuffer_failed:
	(void)vdec_chan_list_remove(chan_list, chan_ret.chan_id);
add_chan_list_error:
	handle_add_chanlist_failed(vdec, vctrl_info, chan_ret.chan_id);
mcu_create_chan_error:
	(void)vdec_free_segbuffer(iova);
	mutex_unlock(&handle->chan_create_lock);
mem_error:
	vfree(vctrl_info);
exit:
	return ret;
}

static int32_t handle_ipc_destory_chan(struct file *file_handle, vdec_ioctl_msg *vdec_msg)
{
	int32_t ret = -1;
	vdec_entry *vdec = vdec_get_entry();
	ipc_vctrl_ioctl *vctrl_info;
	struct ipc_vctrl_msg msg;
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_chan_list *chan_list = &handle->chanlist;
	ipc_vctrl_ret_mcu_to_acpu vctrl_ret = {
		.res = -1,
		.chan_id = -1,
		.iova = 0,
	};

	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IPC_DESTORY_CHAN",
		vdec_msg, sizeof(ipc_vctrl_ioctl), sizeof(ipc_vctrl_ret_mcu_to_acpu)))
		return -EINVAL;

	vctrl_info = vmalloc(sizeof(ipc_vctrl_ioctl));
	if (!vctrl_info) {
		dprint(PRN_FATAL, "alloc memory failed");
		ret = -ENOMEM;
		goto exit;
	}

	if (copy_from_user(vctrl_info, vdec_msg->in, sizeof(ipc_vctrl_ioctl))) {
		dprint(PRN_FATAL, "copy_from_user failed");
		ret = -EFAULT;
		goto mem_error;
	}

	dprint(PRN_ALWS, "chanid is %u begin VDEC_IOCTL_IPC_DESTROY_CHAN", vctrl_info->chanId);
	if (vctrl_info->cmdId != VFMW_CID_DESTROY_CHAN) {
		dprint(PRN_FATAL, "cmd %s not VFMW_CID_DESTROY_CHAN", vctrl_cmd_name(vctrl_info->cmdId));
		ret = -EFAULT;
		goto mem_error;
	}

	if (vctrl_info->chanId < VFMW_NO_SEC_CHAN_START || vctrl_info->chanId >= VFMW_CHAN_ID_MAX) {
		dprint(PRN_FATAL, "chan id error %d range is [2, 17]", vctrl_info->chanId);
		ret = -EFAULT;
		goto mem_error;
	}

	if (!vdec_chan_list_destroy(chan_list, vctrl_info->chanId)) {
		dprint(PRN_FATAL, "VFMW_CID_DESTROY_CHAN chanid %d is ilegal", vctrl_info->chanId);
		ret = -EFAULT;
		goto mem_error;
	}

	msg.input = vctrl_info;
	msg.output = &vctrl_ret;

	mutex_lock(&handle->chan_create_lock);
	ret = vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_SEND_CTRL_MSG, &msg, sizeof(struct ipc_vctrl_msg));
	if (ret == IPC_TIMEOUT) {
		/* destroy chan needs to return ok in ipc time out to avoid memory leakage
	   caused by the upper layer not invoking the exit process. */
		dprint(PRN_FATAL, "destory chan ipc timeout but return ok");
		ret = EOK;
		vctrl_ret.res = EOK;
	}

	// not handle destroy error
	if (ret || vctrl_ret.res) {
		dprint(PRN_FATAL, "send destroy cmd msg failed %d res is %d", ret, vctrl_ret.res);
		ret = -EFAULT;
	}

	if (vdec_chan_list_remove(chan_list, (int32_t)vctrl_info->chanId)) {
		dprint(PRN_FATAL, "remove chan_id %u to list failed", vctrl_info->chanId);
		ret = -EFAULT;
		mutex_unlock(&handle->chan_create_lock);
		goto mem_error;
	}

	mutex_unlock(&handle->chan_create_lock);
	if (!ret && !vctrl_ret.res && vctrl_ret.iova) {
		if (vdec_free_segbuffer(vctrl_ret.iova))
			dprint(PRN_ERROR, "free seg buffer of chan_id:%d failed", vctrl_info->chanId);
	}

	vdec->vdec_chan_state[vctrl_info->chanId] = VFMW_CID_DESTROY_CHAN;
	if (copy_to_user(vdec_msg->out, &vctrl_ret, sizeof(vctrl_ret))) {
		dprint(PRN_FATAL, "copy_to_user failed");
		ret = -EFAULT;
		goto mem_error;
	}
	dprint(PRN_CTRL, "end VDEC_IOCTL_IPC_DESTROY_CHAN");

mem_error:
	vfree(vctrl_info);
exit:
	return ret;
}

static int32_t handle_ipc_config_inputbuffer(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg)
{
	int32_t ret;
	uint32_t iova_addr;
	uint32_t iova_size;
	stream_ioctl stm_info;
	vdec_entry* vdec = vdec_get_entry();
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_fd_list *list = &handle->fdlist;
	struct vdec_chan_list *chan_list = &handle->chanlist;

	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IPC_CONFIG_INPUTBUFFER",
		vdec_msg, sizeof(stream_ioctl), 0))
		return -EINVAL;

	if (copy_from_user(&stm_info, vdec_msg->in, sizeof(stm_info))) {
		dprint(PRN_FATAL, "copy_from_user failed");
		return -EFAULT;
	}

	dprint(PRN_CTRL, "begin chan id is %u VDEC_IOCTL_IPC_CONFIG_INPUTBUFFER", stm_info.chanId);
	if (!vdec_chan_list_get(chan_list, stm_info.chanId)) {
		dprint(PRN_FATAL, "chanid %u is ilegal", stm_info.chanId);
		return -EFAULT;
	}

	ret = vdec_share_fd_list_get(list, stm_info.data.share_fd, &iova_addr, &iova_size);
	if (ret) {
		dprint(PRN_ERROR, "get input buffer failed");
		goto error_chan_list_put;
	}
	if (iova_size < stm_info.data.vfmw_raw_ext.buf_len) {
		dprint(PRN_ERROR, "invalid input buffer");
		ret = -ENOMEM;
		goto error_chan_list_put;
	}
	stm_info.data.phy_addr = iova_addr;

	dprint(PRN_CTRL, "send QUEUE_MSG_TYPE_STREAM_SYNC, in size is %d", sizeof(stm_info));
	ret = vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_SEND_STREAM_MSG, &stm_info, sizeof(stream_ioctl));
	dprint(PRN_CTRL, "end VDEC_IOCTL_IPC_CONFIG_INPUTBUFFER");
error_chan_list_put:
	vdec_chan_list_put(chan_list, stm_info.chanId);
	return ret;
}

static int32_t vdec_get_unsupport_event(struct dec_file_handle *handle, event_output_list *output_list)
{
	unsigned long flag;
	bool is_need_report = false;
	vdec_entry* vdec = vdec_get_entry();
	struct vdec_chan_listnode *chan_pos = NULL;
	struct vdec_chan_list *chan_list = output_list->chan_list;
	event_node *node = (event_node *)(output_list->event_output_buffer.upstream_buffer);

	mutex_lock(&chan_list->lock);
	list_for_each_entry(chan_pos, &chan_list->chan_head, head) {
		if (vdec->vdec_chan_state[chan_pos->chan_id] == VFMW_CID_CREATE_CHAN) {
			is_need_report = true;
			dprint(PRN_ALWS, "cur process report global reset event to user");
			break;
		}
	}

	if (is_need_report) {
		/* cur process reports one global reset event, chan_id is set to 0 by default,
		and then each instance in user mode reports unsupport. */
		node->chan_id = 0;
		node->type = FIRMWARE_UP_MSG_GLOBAL_RESET;
		node->param_len = 0;
		node->used = true;
	}

	output_list->event_output_buffer.event_count = 1;
	mutex_unlock(&chan_list->lock);

	spin_lock_irqsave(&handle->global_reset_spin_lock, flag);
	handle->global_reset = false;
	spin_unlock_irqrestore(&handle->global_reset_spin_lock, flag);

	return 0;
}

static int32_t handle_ipc_process_event(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg)
{
	int32_t ret = 0;
	event_output_list *event_out_list;
	vdec_entry* vdec = vdec_get_entry();
	bool global_reset;
	unsigned long flag;
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_chan_list *chan_list = &handle->chanlist;

	dprint(PRN_EVENT, "begin VDEC_IOCTL_IPC_PROCESS_EVENT");
	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IPC_PROCESS_EVENT",
		vdec_msg, 0, sizeof(event_output)))
		return -EINVAL;

	/* waiting for events sent by the MCU, timeout is 8 ms,
	   obtaining events after timeout to prevent events of a process from being missed. */
	(void)wait_for_completion_timeout(&vdec->mcu_report_event, msecs_to_jiffies(8));
	event_out_list = vzalloc(sizeof(event_output_list));
	if (!event_out_list) {
		dprint(PRN_FATAL, "alloc memory failed");
		ret = -ENOMEM;
		goto exit;
	}

	event_out_list->chan_list = chan_list;
	spin_lock_irqsave(&handle->global_reset_spin_lock, flag);
	global_reset = handle->global_reset;
	spin_unlock_irqrestore(&handle->global_reset_spin_lock, flag);
	if (global_reset) {
		ret = vdec_get_unsupport_event(handle, event_out_list);
	} else {
		ret = vdec->mcu_dev->control(vdec->mcu_dev,
			VCODEC_DEV_MCU_GET_EVENT, event_out_list, sizeof(event_output_list));
	}
	if (ret) {
		ret = -EFAULT;
		goto mem_error;
	}
	if (copy_to_user(vdec_msg->out, &event_out_list->event_output_buffer, sizeof(event_output))){
		dprint(PRN_FATAL, "copy_to_user failed");
		ret = -EFAULT;
		goto mem_error;
	}
	dprint(PRN_EVENT, "end VDEC_IOCTL_IPC_PROCESS_EVENT, event_cnt:%d",
		event_out_list->event_output_buffer.event_count);

mem_error:
	vfree(event_out_list);
exit:
	return ret;
}

static int32_t handle_ipc_release_img(
	struct file *file_handle, vdec_ioctl_msg *vdec_msg)
{
	int32_t ret;
	image_ioctl in;
	vdec_entry* vdec = vdec_get_entry();
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_chan_list *chan_list = &handle->chanlist;

	if (vcodec_vdec_validate_ioctl_msg("VDEC_IOCTL_IPC_RELEASE_IMG",
		vdec_msg, sizeof(image_ioctl), 0))
		return -EINVAL;

	if (copy_from_user(&in, vdec_msg->in, sizeof(in))) {
		dprint(PRN_FATAL, "copy_from_user failed");
		return -EFAULT;
	}
	dprint(PRN_CTRL, "chanid %u begin VDEC_IOCTL_IPC_RELEASE_IMG", in.chan_id);
	if (!vdec_chan_list_get(chan_list, in.chan_id)) {
		dprint(PRN_FATAL, "chanid %d is ilegal", in.chan_id);
		return -EFAULT;
	}
	ret = vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_SEND_IMAGE_MSG, &in, sizeof(image_ioctl));

	vdec_chan_list_put(chan_list, in.chan_id);

	dprint(PRN_CTRL, "end VDEC_IOCTL_IPC_RELEASE_IMG");

	return ret;
}



const ioctl_command_node *vcodec_vdec_get_ioctl_tbl(void)
{
	static ioctl_command_node ioctl_command_table[] = {
		{ VDEC_IOCTL_SET_CLK_RATE,           vdec_ioctl_set_clk_rate },
		{ VDEC_IOCTL_LOCK_HW,                handle_lock_hw },
		{ VDEC_IOCTL_UNLOCK_HW,              handle_unlock_hw },
		{ VDEC_IOCTL_IOMMU_MAP,              handle_iommu_map },
		{ VDEC_IOCTL_IOMMU_UNMAP,            handle_iommu_unmap },
		{ VDEC_IOCTL_IPC_VCTL_CMD,           handle_ipc_vctl_cmd },
		{ VDEC_IOCTL_IPC_CREATE_CHAN,        handle_ipc_create_chan },
		{ VDEC_IOCTL_IPC_DESTORY_CHAN,       handle_ipc_destory_chan },
		{ VDEC_IOCTL_IPC_CONFIG_INPUTBUFFER, handle_ipc_config_inputbuffer },
		{ VDEC_IOCTL_IPC_PROCESS_EVENT,      handle_ipc_process_event },
		{ VDEC_IOCTL_IPC_RELEASE_IMG,        handle_ipc_release_img },
		{ 0, NULL },
	};

	return ioctl_command_table;
}