#include <linux/kernel_read_file.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/timekeeping.h>

#include "vcodec_mcu.h"
#include "vcodec_mcu_hal.h"
#include "dbg.h"
#include "vfmw_ipc.h"
#include "vcodec_mcu_ipc.h"
#include "vfmw_pdt.h"
#include "vcodec_mcu_queue.h"
#include "vcodec_vdec_dbg.h"
#include "smmu_tbu.h"

#define LOG_BUFFER_OFFSET 8
#define MCU_LOG_PANIC_LINE_SIZE     (400)
#define MCU_LOG_PANIC_MAX_SIZE      (4000)

// transfer mcu log to kernel, we can get log after power off
static void vcodec_mcu_transfer_log(struct vcodec_dev_mcu *dev)
{
	uint32_t log_size;
	uint32_t *src = NULL;
	char *start = NULL;

	if (!dev->mcu_exec_mem.virt_addr || !dev->transfer_log)
		return;

	src = (uint32_t *)(dev->mcu_exec_mem.virt_addr + MCU_LOG_BUFFER_OFFSET);
	log_size = *(src + 1);
	start = (char *)(src + 2);

	dev->transfer_log(start, log_size);
}

static int32_t ring_buffer_read_linear(struct mcu_log_get_msg *msg, void *addr, uint8_t *src, uint32_t write)
{
	uint32_t cp_size, mcu_log_size, size;

	size = write - msg->mcu_log_read_offset;
	mcu_log_size = MCU_LOG_BUFFER_SIZE - LOG_BUFFER_OFFSET;
	cp_size = size < msg->buf_size ? size : msg->buf_size;

	if (copy_to_user(addr, src + msg->mcu_log_read_offset, cp_size)) {
		dprint(PRN_FATAL, "copy_to_user failed");
		return -EFAULT;
	}

	msg->mcu_log_read_offset = (msg->mcu_log_read_offset + cp_size) % mcu_log_size;
	msg->read_size += cp_size;

	return (int32_t)cp_size;
}


static int32_t ring_buffer_read_loop(struct mcu_log_get_msg *msg, void *addr, uint8_t *src, uint32_t write)
{
	uint32_t right, cp_size, mcu_log_size, read, size;
	int32_t ret = 0;

	mcu_log_size = MCU_LOG_BUFFER_SIZE - LOG_BUFFER_OFFSET;
	read = msg->mcu_log_read_offset;
	right = mcu_log_size - read;
	size = right + write;
	cp_size = right < msg->buf_size ? right : msg->buf_size;

	if (copy_to_user(addr, src + read, cp_size)) {
		dprint(PRN_FATAL, "copy_to_user failed");
		return -EFAULT;
	}

	msg->mcu_log_read_offset = (msg->mcu_log_read_offset + cp_size) % mcu_log_size;
	msg->read_size += cp_size;

	if (cp_size == msg->buf_size || cp_size == size)
		return (int32_t)cp_size;

	msg->buf_size -= cp_size;
	ret = ring_buffer_read_linear(msg, addr + cp_size, src, write);
	if (ret < 0)
		return ret;

	return (int32_t)cp_size + ret;
}

static int32_t vcodec_mcu_dev_get_log(struct vcodec_dev_mcu *dev, void *input_msg)
{
	uint32_t w_offset, r_offset;
	int32_t ret = 0;
	uint8_t *src = NULL;
	struct mcu_log_get_msg *msg = (struct mcu_log_get_msg *)input_msg;

	if (!msg->usr_buf || !msg->buf_size)
		return -EINVAL;

	msg->read_size = 0;

	if (!dev->mcu_exec_mem.virt_addr)
		return -EFAULT;

	src = dev->mcu_exec_mem.virt_addr + MCU_LOG_BUFFER_OFFSET;
	w_offset = *((uint32_t *)src + 1);
	r_offset = msg->mcu_log_read_offset;

	if (msg->clr_sign) {
		msg->mcu_log_read_offset = w_offset;
		return 0;
	}

	if (w_offset == r_offset)
		return 0;

	if (w_offset > r_offset)
		ret = ring_buffer_read_linear(msg, msg->usr_buf, src + LOG_BUFFER_OFFSET, w_offset);
	else
		ret = ring_buffer_read_loop(msg, msg->usr_buf, src + LOG_BUFFER_OFFSET, w_offset);

	return ret < 0 ? ret : 0;
}

static int32_t vcodec_mcu_alloc_buffer(struct vcodec_dev_mcu* dev)
{
	int32_t ret;
	uint32_t iova;

	DBG_TIMER_SATRT();
	ret = vdec_alloc_iova_with_va(MCU_ENTRY_ADDR, VDEC_MCU_SIZE, &dev->mcu_exec_mem);
	if (ret) {
		dprint(PRN_ERROR, "alloc mcu exec memory failed, size:%d", VDEC_MCU_SIZE);
		dev->event_notify(VCODEC_DEV_MCU_EVENT_ALLOC_ION_DONE, NULL);
		return ret;
	}
	BUG_ON(dev->mcu_exec_mem.iova != MCU_ENTRY_ADDR);
	DBG_TIMER_SHOW("alloc mcu exec memory");

	dev->event_notify(VCODEC_DEV_MCU_EVENT_ALLOC_ION_DONE, NULL);

	DBG_TIMER_RESATRT();
	ret = vdec_alloc_iova(VDEC_MSG_POOL_BASE, VDEC_MSG_POOL_SIZE, &dev->msg_pool_info);
	if (ret) {
		dprint(PRN_ERROR, "alloc msg pool memory failed, size:%d", VDEC_MSG_POOL_SIZE);
		goto msg_pool_fail;
	}
	DBG_TIMER_SHOW("alloc msg pool memory");

	DBG_TIMER_RESATRT();
	ret = vdec_alloc_mcu_heap_iova(&iova);
	if (ret) {
		dprint(PRN_ERROR, "alloc mcu heap failed");
		goto mcu_heap_fail;
	}
	dev->mcu_heap_iova = iova;
	DBG_TIMER_SHOW("alloc mcu heap memory");
	return 0;

mcu_heap_fail:
	vdec_free_iova(&dev->msg_pool_info);
msg_pool_fail:
	vdec_free_iova_with_va(&dev->mcu_exec_mem);
	return ret;
}

static void vcodec_mcu_free_buffer(struct vcodec_dev_mcu* dev)
{
	vdec_free_mcu_heap_iova(dev->mcu_heap_iova);
	vdec_free_iova(&dev->msg_pool_info);
	vdec_free_iova_with_va(&dev->mcu_exec_mem);
}

static int32_t vcodec_mcu_dev_get_rt_log(struct vcodec_dev_mcu* dev, void *msg)
{
	uint32_t log_size;
	uint32_t *src = NULL;
	char *start = NULL;
	struct mcu_real_time_log *log = (struct mcu_real_time_log *)msg;

	if (unlikely(!dev->mcu_exec_mem.virt_addr))
		return -EFAULT;

	src = (uint32_t *)(dev->mcu_exec_mem.virt_addr + MCU_LOG_BUFFER_OFFSET);
	log_size = *(src + 1);
	start = (char *)(src + 2);

	log->buffer = start;
	log->size = log_size;

	return 0;
}

static void vcodec_mcu_dev_print_panic_log(struct vcodec_dev_mcu* dev)
{
	uint32_t log_size;
	uint32_t panic_log_size;
	uint32_t *src = NULL;
	char *start = NULL;
	char *panic_log = NULL;
	char out_line[MCU_LOG_PANIC_LINE_SIZE + 1];

	if (unlikely(!dev->mcu_exec_mem.virt_addr)) {
		dprint(PRN_ERROR, "null ptr");
		return;
	}
	src = (uint32_t *)(dev->mcu_exec_mem.virt_addr + MCU_LOG_BUFFER_OFFSET);
	log_size = *(src + 1);
	start = (char *)(src + 2); /* 2 data offset */
	if (log_size < MCU_LOG_PANIC_MAX_SIZE) {
		panic_log = start;
		panic_log_size = log_size;
	} else {
		panic_log = (char *)(start + (log_size - MCU_LOG_PANIC_MAX_SIZE));
		panic_log_size = MCU_LOG_PANIC_MAX_SIZE;
	}

	dprint(PRN_ALWS, "panic info %u:", panic_log_size);
	out_line[MCU_LOG_PANIC_LINE_SIZE] = '\0';
	while (panic_log_size > MCU_LOG_PANIC_LINE_SIZE) {
		(void)memcpy_s(out_line, MCU_LOG_PANIC_LINE_SIZE, panic_log, MCU_LOG_PANIC_LINE_SIZE);
		dprint(PRN_ALWS, "%s", out_line);
		panic_log_size -= MCU_LOG_PANIC_LINE_SIZE;
		panic_log += MCU_LOG_PANIC_LINE_SIZE;
	}
	if (panic_log_size > 0 && panic_log_size <= MCU_LOG_PANIC_LINE_SIZE) {
		(void)memcpy_s(out_line, MCU_LOG_PANIC_LINE_SIZE, panic_log, panic_log_size);
		out_line[panic_log_size] = '\0';
		dprint(PRN_ALWS, "%s", out_line);
	}
}

void vcodec_mcu_dev_close(struct vcodec_dev_mcu* dev)
{
	dprint(PRN_ALWS, "begin close mcu device");
	if (dev->status == VCODEC_MCU_STATUS_POWER_ON) {
		vcodec_mcu_hal_poweroff(dev);
		vcodec_mcu_transfer_log(dev);
	}

	dev->status = VCODEC_MCU_STATUS_POWER_OFF;

	deinit_vdec_ipc();
	deinit_vdec_queue();
	vcodec_mcu_free_buffer(dev);
	vcodec_mcu_deinit_hal(dev);
	dprint(PRN_ALWS, "close mcu device success");
}

static void vcodec_mcu_dev_notify_startup_done(void* owner)
{
	struct vcodec_dev_mcu* dev = (struct vcodec_dev_mcu *)owner;
	if (unlikely(!dev))
		return;

	complete(&dev->mcu_startup_done);
}

static int32_t vcodec_mcu_dev_init_ipc(struct vcodec_dev_mcu* dev)
{
	struct ipc_init_info info;

	if (unlikely(!dev->event_notify))
		return -EINVAL;

	info.owner = dev;
	info.device = dev->device;
	info.mcu_exec_addr = dev->mcu_exec_mem.virt_addr;
	info.reg_base = dev->reg_base_vir_addr;
	info.mcu_startup_done_notify = vcodec_mcu_dev_notify_startup_done;
	info.event_notify = dev->event_notify;

	return init_vdec_ipc(&info);
}

static int32_t vcodec_mcu_dev_init_queue(struct vcodec_dev_mcu* dev)
{
	struct queue_init_info info;

	info.owner = dev;
	info.mcu_exec_addr = dev->mcu_exec_mem.virt_addr;
	return init_vdec_queue(&info);
}

static ipc_error_code vcodec_mcu_send_msg_with_log(int32_t cmd, struct ipc_sync_msg *ipc_msg,
	struct vcodec_dev_mcu *dev)
{
	ipc_error_code ret = send_ipc_sync_msg(cmd, ipc_msg);
	if (ret == IPC_TIMEOUT) {
		dprint(PRN_ERROR, "handle send ipc sync msg timeout");
		dev->event_notify(VCODEC_DEV_MCU_GET_MEDIA1_CRG_INFO, NULL);
		vcodec_mcu_transfer_log(dev);
		vcodec_mcu_dev_print_panic_log(dev);
		vcodec_mcu_hal_show_status(dev);
	}
	return ret;
}

int32_t vcodec_mcu_dev_open(struct vcodec_dev_mcu* dev, uint8_t *reg_base)
{
	uint64_t timeout;

	dprint(PRN_ALWS, "begin open mcu device");

	if (vcodec_mcu_init_hal(dev, reg_base))
		return -EFAULT;

	DBG_TIMER_SATRT();
	if (vcodec_mcu_alloc_buffer(dev))
		goto alloc_buffer_error;
	DBG_TIMER_SHOW("mcu alloc buffer");

	if (vcodec_mcu_dev_init_ipc(dev))
		goto init_ipc_error;

	if (vcodec_mcu_dev_init_queue(dev))
		goto init_queue_error;

	DBG_TIMER_RESATRT();
	if (vcodec_mcu_hal_poweron(dev, true))
		goto power_on_error;
	DBG_TIMER_SHOW("mcu poweron");

	DBG_TIMER_RESATRT();
	timeout = wait_for_completion_timeout(&dev->mcu_startup_done,
		msecs_to_jiffies(1000)); /* 1000 ms */
	if(timeout == 0) {
		dprint(PRN_FATAL, "mcu start timeout");
		vcodec_mcu_hal_show_status(dev);
		show_smmu_pre_intercept();
		goto power_on_timeout;
	}
	DBG_TIMER_SHOW("mcu startup");

	dev->status = VCODEC_MCU_STATUS_POWER_ON;
	dprint(PRN_ALWS, "mcu open success");
	return 0;

power_on_timeout:
	vcodec_mcu_transfer_log(dev);
	vcodec_mcu_hal_poweroff(dev);
power_on_error:
	deinit_vdec_queue();
init_queue_error:
	deinit_vdec_ipc();
init_ipc_error:
	vcodec_mcu_free_buffer(dev);
alloc_buffer_error:
	vcodec_mcu_deinit_hal(dev);
	return -EFAULT;
}

int32_t vcodec_mcu_dev_suspend(struct vcodec_dev_mcu* dev)
{
	int32_t ret;
	uint32_t cmd_id = VFMW_CID_MCORE_SUSPEND;
	struct ipc_sync_msg ipc_msg = { &cmd_id, sizeof(cmd_id), NULL, MCU_IPC_SR_WAIT_MS };

	if (dev->status != VCODEC_MCU_STATUS_POWER_ON)
		return 0;

	dprint(PRN_ALWS, "begin suspend mcu device");
	ret = send_ipc_sync_msg(IPC_MSG_TYPE_VCTRL_SR_CMD, &ipc_msg);
	if (ret) {
		dprint(PRN_FATAL, "ipc error, ret %d", ret);
		dev->event_notify(VCODEC_DEV_MCU_GET_MEDIA1_CRG_INFO, NULL);
		goto error;
	}
	ipc_set_state(VDEC_IPC_STATE_POWER_OFF);
	ret = vcodec_mcu_hal_wait_suspend_done(dev);
	if (ret)
		goto error;
	dev->subctrl_vir_addr->MCU_RST0.bits.mcu_wdt_rst = 1;
	dev->status = VCODEC_MCU_STATUS_SUSPEND;
	dprint(PRN_ALWS, "suspend mcu device success");
	vdec_suspend_iova_smmu_pre();
	return 0;
error:
	dev->event_notify(VCODEC_DEV_MCU_RESET_GLOBAL, NULL);
	ipc_set_state(VDEC_IPC_STATE_POWER_OFF);
	dev->status = VCODEC_MCU_STATUS_POWER_OFF;
	vcodec_mcu_hal_show_status(dev);
	vcodec_mcu_dev_print_panic_log(dev);
	return -EFAULT;
}

int32_t vcodec_mcu_dev_resume(struct vcodec_dev_mcu* dev)
{
	int32_t ret;
	uint32_t cmd_id = VFMW_CID_MCORE_RESUME;
	struct ipc_sync_msg ipc_msg = { &cmd_id, sizeof(cmd_id), NULL, MCU_IPC_SR_WAIT_MS };

	if (dev->status != VCODEC_MCU_STATUS_SUSPEND)
		return 0;

	vdec_resume_iova_smmu_pre();
	dprint(PRN_ALWS, "begin resume mcu device");
	if (vcodec_mcu_hal_poweron(dev, false)) {
		dprint(PRN_FATAL, "power on mcu fail");
		goto error;
	}
	ipc_set_state(VDEC_IPC_STATE_POWER_ON);
	ret = send_ipc_sync_msg(IPC_MSG_TYPE_VCTRL_SR_CMD, &ipc_msg);
	if (ret) {
		vcodec_mcu_hal_poweroff(dev);
		dprint(PRN_FATAL, "ipc error, ret %d", ret);
		dev->event_notify(VCODEC_DEV_MCU_GET_MEDIA1_CRG_INFO, NULL);
		goto error;
	}
	dev->status = VCODEC_MCU_STATUS_POWER_ON;
	dprint(PRN_ALWS, "resume mcu device success");
	return 0;
error:
	dev->event_notify(VCODEC_DEV_MCU_RESET_GLOBAL, NULL);
	ipc_set_state(VDEC_IPC_STATE_POWER_OFF);
	dev->status = VCODEC_MCU_STATUS_POWER_OFF;
	vcodec_mcu_hal_show_status(dev);
	vcodec_mcu_dev_print_panic_log(dev);
	return -EFAULT;
}

void vcodec_mcu_dev_init(struct vcodec_dev_mcu* dev)
{
	init_completion(&dev->mcu_startup_done);
	dev->status = VCODEC_MCU_STATUS_POWER_OFF;
}

void vcodec_mcu_dev_deinit(struct vcodec_dev_mcu* dev)
{
	dev->status = VCODEC_MCU_STATUS_POWER_OFF;
	return;
}

static int32_t vcodec_mcu_set_recv_event_notify(struct vcodec_dev_mcu* dev, void *msg)
{
	dev->event_notify = (void (*)(enum vcodec_dev_event_notify, void *))msg;
	return 0;
}

static int32_t vcodec_mcu_dev_send_ctrl_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	struct ipc_vctrl_msg *ctrl = (struct ipc_vctrl_msg *)msg;
	struct ipc_sync_msg ipc_msg;
	if (unlikely(!ctrl->input || !ctrl->output))
		return -EINVAL;
	ipc_msg.send_msg = ctrl->input;
	ipc_msg.send_msg_len = sizeof(ipc_vctrl_ioctl);
	ipc_msg.ack_msg = ctrl->output;
	ipc_msg.wait_time = MCU_IPC_WAIT_MS;
	return vcodec_mcu_send_msg_with_log(IPC_MSG_TYPE_VCTRL_COMMAND, &ipc_msg, dev);
}

static int32_t vcodec_mcu_dev_send_stream_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	stream_ioctl *stream = (stream_ioctl *)msg;
	if (unlikely(!stream))
		return -EINVAL;

	return write_input_queue_msg(QUEUE_MSG_TYPE_STREAM_SYNC, stream, sizeof(stream_ioctl));
}

static int32_t vcodec_mcu_dev_send_image_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	image_ioctl *image = (image_ioctl *)msg;
	if (unlikely(!image))
		return -EINVAL;

	return write_input_queue_msg(QUEUE_MSG_TYPE_RELEASE_IMG, image, sizeof(image_ioctl));
}

static int32_t vcodec_mcu_dev_send_proc_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	struct ipc_proc_msg *proc = (struct ipc_proc_msg *)msg;
	struct ipc_sync_msg ipc_msg = { proc->input, proc->str_len, &proc->output, MCU_IPC_WAIT_MS };
	return vcodec_mcu_send_msg_with_log(IPC_MSG_TYPE_PROCMSG, &ipc_msg, dev);
}

static int32_t vcodec_mcu_send_lock_scd_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	struct ipc_sync_msg ipc_msg = { NULL, 0, NULL, MCU_IPC_WAIT_MS };
	return vcodec_mcu_send_msg_with_log(IPC_MSG_TYPE_LOCK_SCD_HW, &ipc_msg, dev);
}

static int32_t vcodec_mcu_send_unlock_scd_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	struct ipc_sync_msg ipc_msg = { NULL, 0, NULL, MCU_IPC_WAIT_MS };
	return vcodec_mcu_send_msg_with_log(IPC_MSG_TYPE_UNLOCK_SCD_HW, &ipc_msg, dev);
}

static int32_t vcodec_mcu_dev_get_event_list(struct vcodec_dev_mcu* dev, void *msg)
{
	event_output_list *output = (event_output_list *)msg;
	return get_output_event_list(output);
}

static int32_t vcodec_mcu_set_log_transfer(struct vcodec_dev_mcu* dev, void *msg)
{
	dev->transfer_log = (void (*)(uint8_t*, uint32_t))msg;
	return 0;
}

static int32_t vcodec_mcu_dev_send_destory_msg(struct vcodec_dev_mcu* dev, void *msg)
{
	struct ipc_destroy_msg *destroy_msg = (struct ipc_destroy_msg *)msg;
	struct ipc_sync_msg ipc_msg;

	ipc_msg.send_msg = &(destroy_msg->chan_id);
	ipc_msg.send_msg_len = (uint32_t)sizeof(destroy_msg->chan_id);
	ipc_msg.ack_msg = &(destroy_msg->output);
	ipc_msg.wait_time = MCU_IPC_WAIT_MS;
	return vcodec_mcu_send_msg_with_log(IPC_MSG_TYPE_ABNORMAL_CLOSE, &ipc_msg, dev);
}

static int32_t vcodec_mcu_dev_init_output_queue_of_one_chan(struct vcodec_dev_mcu* dev, void *msg)
{
	init_output_event_queue_of_one_chan(*(int32_t *)msg);
	return 0;
}

static int32_t vcodec_mcu_dev_assign_segbuffer_to_one_chan(struct vcodec_dev_mcu* dev, void *msg)
{
	struct ipc_segbuf_msg *segbuf_msg = (struct ipc_segbuf_msg *)msg;
	ipc_segbuf_info info = {0};
	struct ipc_sync_msg ipc_msg;

	info.iova = segbuf_msg->iova;
	info.size = VDEC_SEG_BUF_ONE_BLOCK_SIZE;
	info.chan_id = segbuf_msg->chan_id;

	ipc_msg.send_msg = &info;
	ipc_msg.send_msg_len = (uint32_t)sizeof(ipc_segbuf_info);
	ipc_msg.ack_msg = segbuf_msg->output;
	ipc_msg.wait_time = MCU_IPC_WAIT_MS;
	return vcodec_mcu_send_msg_with_log(IPC_MSG_TYPE_ALLOC_SEG_BUFFER, &ipc_msg, dev);
}

const struct vcodec_mcu_control_proc_tbl *vcodec_mcu_dev_get_control_proc_tbl(void)
{
	static struct vcodec_mcu_control_proc_tbl tbl[] = {
		/* set recv event callback */
		{ VCODEC_DEV_MCU_SET_EVENT_NOTIFY, sizeof(void *),
			VCODEC_MCU_STATUS_POWER_OFF, vcodec_mcu_set_recv_event_notify },

		/* send ctrl message to mcore */
		{ VCODEC_DEV_MCU_SEND_CTRL_MSG, sizeof(struct ipc_vctrl_msg),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_send_ctrl_msg },

		/* wirte stream msg to msg input queue */
		{ VCODEC_DEV_MCU_SEND_STREAM_MSG, sizeof(stream_ioctl),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_send_stream_msg },

		/* get event from event output queue */
		{ VCODEC_DEV_MCU_GET_EVENT, sizeof(event_output_list),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_get_event_list },

		/* wirte stream msg to msg input queue */
		{ VCODEC_DEV_MCU_SEND_IMAGE_MSG, sizeof(image_ioctl),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_send_image_msg },

		/* send proc sync message to mcore */
		{ VCODEC_DEV_MCU_SEND_PROC_MSG, sizeof(struct ipc_proc_msg),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_send_proc_msg },

		/* write realtime mcu log to file */
		{ VCODEC_DEV_MCU_DUMP_REALTIME_LOG, sizeof(struct mcu_real_time_log),
            VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_get_rt_log },

		/* get realtime mcu log */
		{ VCODEC_DEV_MCU_GET_REALTIME_LOG, sizeof(struct mcu_log_get_msg),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_get_log },

		/* set log transfer callback */
		{ VCODEC_DEV_MCU_SET_LOG_TRANSFER, sizeof(void *),
			VCODEC_MCU_STATUS_POWER_OFF, vcodec_mcu_set_log_transfer },

		/* send lock scd sync message to mcore, for drm */
		{ VCODEC_DEV_MCU_SEND_LOCK_SCD_MSG, sizeof(uint32_t),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_send_lock_scd_msg },

		/* send unlock scd sync message to mcore, for drm */
		{ VCODEC_DEV_MCU_SEND_UNLOCK_SCD_MSG, sizeof(uint32_t),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_send_unlock_scd_msg },

		/* send chan destory message to mcore, for vdec abnormal close */
		{ VCODEC_DEV_MCU_SEND_DESTORY_MSG, sizeof(struct ipc_destroy_msg),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_send_destory_msg },
		
		/* init output event queue of one chan to prevent data remanence in shared memory */
		{ VCODEC_DEV_MCU_INIT_OUTPUT_EVENT_QUEUE, sizeof(int32_t),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_init_output_queue_of_one_chan },

		/* assign seg buffer to one chan */
		{ VCODEC_DEV_MCU_ASSIGN_SEG_BUFFER, sizeof(struct ipc_segbuf_msg),
			VCODEC_MCU_STATUS_POWER_ON, vcodec_mcu_dev_assign_segbuffer_to_one_chan },

		/* tbl end flag */
		{ MCU_INVALID_CMD_ID, 0, VCODEC_MCU_STATUS_BUTT, NULL },
	};

	return tbl;
}