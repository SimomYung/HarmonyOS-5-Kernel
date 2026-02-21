#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/completion.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include "vcodec_mcu_ipc.h"
#include "dbg.h"
#include "vdec/IPC_AO_c_union_define.h"
#include "vcodec_mcu.h"
#include "vdec_memory_map.h"
#include "vfmw_ipc.h"
#ifdef CONFIG_VCODEC_OHOS
#include <trace/hooks/liblinux.h>
#endif

#define IPC_TIMEOUT_MS                (10000)
#define EVERY_LOOP_TIME_MS            (5)
#define MAILBOX_ASYNC_UDELAY_CNT      (5)
#define MAILBOX_IDLE_STATUS           (1 << 4)
#define IPC_KEY_UNLOCK                (0x1acce551)
#define MAX_IPC_MEM_NUM               (300)

typedef enum {
	IPC_ACPU_ID = 0, // AP
	IPC_MCU_ID = 1 // MCU
} vdec_ipc_cpu_id;

typedef enum {
	MBX_0 = 0, // MCU --> AP ,fast ipc
	MBX_1 = 1, // AP --> MCU ,fast ipc
	MBX_2 = 2, // shared mailbox for slow ipc and ipc ack
	MBX_BUTT
} vdec_ipc_mailbox_id;

typedef enum {
	SYNC_MODE = 0,
	ASYNC_MODE = 1
} vdec_ipc_send_mode;

typedef enum {
	IPC_DATA_FROM_ACPU = 0,
	IPC_ACK_FROM_ACPU,
	IPC_DATA_FROM_MCU,
	IPC_SYNC_ACK_FROM_MCU,
} vdec_ipc_msg_type;

typedef struct {
	int32_t year;
	int32_t mon;
	int32_t day;
	int32_t hour;
	int32_t min;
	int32_t sec;
	int64_t nsec;
} mcu_start_time;

typedef struct {
	uint32_t cmd;
	uint32_t type;
	uint8_t *buffer;
	uint32_t buffer_len;
	struct list_head list;
} recv_msg;

typedef struct {
	spinlock_t lock;
	struct list_head head;
} recv_msg_queue;

typedef struct {
	uint32_t cmd;
	void *data;
	uint32_t data_len;
	vdec_ipc_send_mode mode;
	vdec_ipc_msg_type type;
} channel_message;

typedef struct {
	// used for lock ipc send interface
	struct mutex lock;
	// when muti threads are blocked by mutex, used this lock to keep order
	struct mutex seq_lock;
	// this queue is used to save msg send from mcu
	recv_msg_queue recv_queue;
	// ipc thread wait
	wait_queue_head_t wait_queue;
	// ipc thread process msg from mcu，
	struct task_struct *task_kthread;
	struct completion sync_msg_complete;
	// ipc reg base
	volatile S_IPC_AO_REGS_TYPE *dev;
	// the share memory for ipc msg
	void *ap2mcu_memory;
	void *mcu2ap_memory;
	uint32_t ack_data_len;
	uint32_t ack_cmd;
	uint32_t ack_type;
	vdec_ipc_state_type state;
	struct device *device;
	void *owner;
	void (*mcu_startup_done_notify)(void *);
	void (*event_notify)(enum vcodec_dev_event_notify, void*);
} ipc_entry;

static ipc_entry *get_ipc_entry(void)
{
	static ipc_entry g_ipc_entry;
	return &g_ipc_entry;
}

static void free_recv_msg_packet(recv_msg *packet)
{
	if (packet->buffer) {
		kfree(packet->buffer);
		packet->buffer = NULL;
	}
	kfree(packet);
}

static uint32_t get_mailbox_status(int32_t mbx_id)
{
	ipc_entry *ipc = get_ipc_entry();
	return ipc->dev->IPC_MBX_REGS[mbx_id].MBX_MODE.u32;
}

static void release_channel(int32_t mbx_id)
{
	ipc_entry *ipc = get_ipc_entry();

	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_SOURCE.u32 = BIT(IPC_ACPU_ID);
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DSET.u32 = BIT(IPC_MCU_ID);
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_MODE.u32 = 0;

	dprint(PRN_DBG, "send channel status is %u", get_mailbox_status(mbx_id));
}

static void acquire_channel(int32_t mbx_id)
{
	uint32_t timeout = 0;
	uint32_t loop = IPC_TIMEOUT_MS / EVERY_LOOP_TIME_MS + MAILBOX_ASYNC_UDELAY_CNT;

	if (get_mailbox_status(mbx_id) & MAILBOX_IDLE_STATUS) {
		return;
	}
	while (timeout < loop) {
		if (timeout < MAILBOX_ASYNC_UDELAY_CNT) {
			msleep(1);
		} else {
			msleep(5);
		}
		/* if the ack status is ready, break out */
		if (get_mailbox_status(mbx_id) & MAILBOX_IDLE_STATUS)
			return;
		timeout++;
	}
	dprint(PRN_ERROR, "qcuire ipc timeout");
	release_channel(mbx_id);
}

static void write_msg_to_channel(int32_t mbx_id, const channel_message *message)
{
	ipc_entry *ipc;
	uint32_t value;

	ipc = get_ipc_entry();

	if (message->data && message->data_len) {
		if(memcpy_s(ipc->ap2mcu_memory, IPC_AP2MCU_MEMORY_SIZE, message->data, message->data_len)) {
			dprint(PRN_ERROR, "copy message from share memory failed");
			return;
		}
	}

	mb();
	ipc->dev->IPC_LOCK.u32 = IPC_KEY_UNLOCK;
	value = ipc->dev->IPC_MBX_REGS[mbx_id].MBX_SOURCE.u32;
	if (value != 0)
		ipc->dev->IPC_MBX_REGS[mbx_id].MBX_SOURCE.u32 = value;

	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_SOURCE.u32 = BIT(IPC_ACPU_ID);
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DSET.u32 = BIT(IPC_MCU_ID);
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_MODE.u32 = 0;

	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA0.u32 = message->mode;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA1.u32 = message->type;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA2.u32 = message->cmd;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA3.u32 = message->data_len;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA4.u32 = 0x12345678;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA5.u32 = 0x87654321;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA6.u32 = 0x89ABCDEF;
	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_DATA7.u32 = 0xFEDCBA98;

	ipc->dev->IPC_MBX_REGS[mbx_id].MBX_SEND.u32 = BIT(IPC_ACPU_ID);
}

static void ipc_ack_to_mcu(uint32_t cmd, void *msg, int32_t len)
{
	ipc_entry *ipc = get_ipc_entry();
	if (ipc->state != VDEC_IPC_STATE_POWER_ON) {
		dprint(PRN_WARN, "error: ipc state %d", ipc->state);
		return;
	}
	(void)memcpy_s(ipc->mcu2ap_memory, IPC_MCU2AP_MEMORY_SIZE, msg, (size_t)len);
	mb();
	ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA0.u32 = SYNC_MODE;
	ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA1.u32 = IPC_ACK_FROM_ACPU;
	ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA2.u32 = cmd;
	ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA3.u32 = (uint32_t)len;
	ipc->dev->IPC_MBX_REGS[MBX_0].MBX_SEND.u32 = BIT(IPC_ACPU_ID);
}

static void ipc_handle_alloc_iova(void)
{
	ipc_alloc_mem mem = { 0 };
	uint32_t iova = 0;
	int32_t ret;
	uint64_t temp = vcodec_osal_get_time_in_us();

	ret = vdec_alloc_mcu_heap_iova(&iova);
	if (ret) {
		dprint(PRN_ERROR, "alloc failed");
	} else {
		mem.iova = iova;
		mem.size = VDEC_MCU_HEAP_ONE_BLOCK_SIZE;
	}
	dprint(PRN_ALWS, "alloc heap buffer spend time: %lld us", vcodec_osal_get_time_in_us() - temp);
	ipc_ack_to_mcu(IPC_MSG_TYPE_ALLOC_MCU_HEAP, &mem, sizeof(ipc_alloc_mem));
}

static void ipc_handle_free_iova(uint32_t iova)
{
	int32_t ret;

	ret = vdec_free_mcu_heap_iova(iova);
	ipc_ack_to_mcu(IPC_MSG_TYPE_FREE_MCU_HEAP, &ret, sizeof(int32_t));
}

static void ipc_handle_init_done(void)
{
	struct timespec64 ts;
	struct tm t;
	mcu_start_time time;
	ipc_entry *ipc = get_ipc_entry();

	ktime_get_real_ts64(&ts);
	time64_to_tm(ts.tv_sec, 0, &t);
	dprint(PRN_ALWS, "%ld.%ld, %ld-%d-%d %d:%d:%d", ts.tv_sec, ts.tv_nsec, t.tm_year + 1900,
		t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	time.year = (int32_t)t.tm_year;
	time.mon = t.tm_mon;
	time.day = t.tm_mday;
	time.hour = t.tm_hour;
	time.min = t.tm_min;
	time.sec = t.tm_sec;
	time.nsec = ts.tv_nsec;
	ipc_ack_to_mcu(IPC_LITEOS_INIT_DONE, &time, sizeof(time));
	ipc->mcu_startup_done_notify(ipc->owner);
}

static void process_msg(recv_msg *packet)
{
	ipc_entry *ipc = get_ipc_entry();
	switch (packet->cmd) {
	case IPC_LITEOS_INIT_DONE: {
		dprint(PRN_DBG, "process cmd IPC_LITEOS_INIT_DONE");
		ipc_handle_init_done();
		break;
	}
	case IPC_MSG_TYPE_RESET_BSP: {
		void *info = (void*)(packet->buffer);
		dprint(PRN_DBG, "process cmd IPC_MSG_TYPE_RESET_BSP");
		ipc->event_notify(VCODEC_DEV_MCU_RESET_BSP, info);
		ipc_ack_to_mcu(IPC_MSG_TYPE_RESET_BSP, NULL, 0);
		break;
	}
	case IPC_MSG_TYPE_RESET_PXP: {
		void *info = (void*)(packet->buffer);
		dprint(PRN_DBG, "process cmd IPC_MSG_TYPE_RESET_PXP");
		ipc->event_notify(VCODEC_DEV_MCU_RESET_PXP, info);
		ipc_ack_to_mcu(IPC_MSG_TYPE_RESET_PXP, NULL, 0);
		break;
	}
	case IPC_MSG_TYPE_EVENT_REPORT: {
		void *info = (void*)(packet->buffer);
		dprint(PRN_DBG, "process cmd IPC_MSG_TYPE_EVENT_REPORT");
		ipc->event_notify(VCODEC_DEV_MCU_EVENT_REPORT, info);
		break;
	}
	case IPC_MSG_TYPE_ALLOC_MCU_HEAP: {
		dprint(PRN_DBG, "process cmd IPC_MSG_TYPE_ALLOC_MCU_HEAP");
		ipc_handle_alloc_iova();
		break;
	}
	case IPC_MSG_TYPE_FREE_MCU_HEAP: {
		uint32_t free_iova = *(uint32_t *)(packet->buffer);
		ipc_handle_free_iova(free_iova);
		break;
	}
	case IPC_MSG_TYPE_STREAM_ERROR: {
		bsp_error error_info = *(bsp_error *)(packet->buffer);
		dprint(PRN_ALWS,
		"chan_id %u frm_num %u stream error, early_end_cnt is %u bs_over_cnt is %u first_slice_lost %u",
		error_info.chan_id, error_info.frm_num, error_info.early_end_cnt, error_info.bs_over_cnt,
		error_info.first_slice_lost);
		break;
	}
	default: {
		dprint(PRN_ERROR, "unsurpport cmd %d", packet->cmd);
		ipc_ack_to_mcu(packet->cmd, NULL, 0);
	}
	}
}

static int ipc_recv_thread(void *data)
{
	int ret;
	recv_msg *packet = NULL;
	ipc_entry *ipc = get_ipc_entry();
	unsigned long flag;

	while (!kthread_should_stop()) {
		ret = wait_event_interruptible(
			ipc->wait_queue, (!list_empty(&ipc->recv_queue.head)) ||
						 kthread_should_stop());
		if (ret) {
			dprint(PRN_ERROR, "wait event %d", ret);
			continue;
		}

		spin_lock_irqsave(&ipc->recv_queue.lock, flag);
		packet = list_first_entry_or_null(&ipc->recv_queue.head, recv_msg, list);
		if (!packet) {
			spin_unlock_irqrestore(&ipc->recv_queue.lock, flag);
			dprint(PRN_ERROR, "get packet NULL");
			continue;
		} else {
			list_del(&packet->list);
			spin_unlock_irqrestore(&ipc->recv_queue.lock, flag);
		}
		process_msg(packet);
		free_recv_msg_packet(packet);
	}
	dprint(PRN_ALWS, "ipc thread exit");
	return 0;
}

static recv_msg *alloc_recv_msg_packet(uint32_t msg_len)
{
	recv_msg *packet = NULL;

	packet = kzalloc(sizeof(recv_msg), GFP_ATOMIC);
	if (!packet) {
		dprint(PRN_ERROR, "malloc packet fail");
		return NULL;
	}

	if (msg_len == 0) {
		packet->buffer_len = 0;
		return packet;
	}

	packet->buffer = kzalloc(msg_len, GFP_ATOMIC);
	if (!packet->buffer) {
		dprint(PRN_ERROR, "malloc packet buf fail");
		kfree(packet);
		return NULL;
	}

	packet->buffer_len = msg_len;

	return packet;
}

// this irq is ack from mcu
irqreturn_t vdec_ipc_ack_isr(int irq, void *id)
{
	ipc_entry *ipc;
	dprint(PRN_DBG, "enter vdec_ipc_ack_isr");
	ipc = get_ipc_entry();
	if (!ipc->dev) {
		dprint(PRN_ERROR, "ipc dev is null");
		return IRQ_HANDLED;
	}
	ipc->dev->IPC_LOCK.u32 = IPC_KEY_UNLOCK;

	ipc->ack_type = ipc->dev->IPC_MBX_REGS[MBX_1].MBX_DATA1.u32;
	ipc->ack_cmd = ipc->dev->IPC_MBX_REGS[MBX_1].MBX_DATA2.u32;
	ipc->ack_data_len = ipc->dev->IPC_MBX_REGS[MBX_1].MBX_DATA3.u32;

	ipc->dev->IPC_MBX_REGS[MBX_1].MBX_ICLR.u32 = BIT(IPC_ACPU_ID);
	complete(&ipc->sync_msg_complete);
	return IRQ_HANDLED;
}

// this irq is msg send by mcu
irqreturn_t vdec_ipc_mbx0_isr(int irq, void *id)
{
	recv_msg *packet = NULL;
	ipc_entry *ipc = get_ipc_entry();
	channel_message msg = { 0 };
	unsigned long flag;

	if (!ipc->dev) {
		dprint(PRN_ERROR, "ipc dev is null");
		return IRQ_HANDLED;
	}
	dprint(PRN_DBG, "enter vdec_ipc_mbx0_isr");
	ipc->dev->IPC_LOCK.u32 = IPC_KEY_UNLOCK;
	msg.mode = ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA0.u32;
	msg.type = ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA1.u32;
	msg.cmd = ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA2.u32;
	msg.data_len = ipc->dev->IPC_MBX_REGS[MBX_0].MBX_DATA3.u32;
	ipc->dev->IPC_MBX_REGS[MBX_0].MBX_ICLR.u32 = BIT(IPC_ACPU_ID);

	packet = alloc_recv_msg_packet(msg.data_len);
	if (!packet)
		return IRQ_HANDLED;

	packet->cmd = (uint16_t)msg.cmd;
	packet->type = msg.type;
	mb();
	(void)memcpy_s(packet->buffer, packet->buffer_len, ipc->mcu2ap_memory, msg.data_len);

	spin_lock_irqsave(&ipc->recv_queue.lock, flag);
	list_add_tail(&packet->list, &ipc->recv_queue.head);
	spin_unlock_irqrestore(&ipc->recv_queue.lock, flag);

	wake_up_interruptible(&ipc->wait_queue);

	return IRQ_HANDLED;
}

void ipc_set_state(vdec_ipc_state_type state)
{
	ipc_entry *ipc = get_ipc_entry();
	mutex_lock(&ipc->lock);
	ipc->state = state;
	mutex_unlock(&ipc->lock);
	dprint(PRN_ALWS, "set vdec ipc state %d", state);
}

int32_t init_vdec_ipc(struct ipc_init_info *info)
{
	ipc_entry *ipc = get_ipc_entry();
	if (unlikely(!info || !info->device || !info->mcu_exec_addr ||
		!info->mcu_startup_done_notify || !info->event_notify ||
		!info->reg_base || !info->owner))
		return -EINVAL;

	ipc->dev = (S_IPC_AO_REGS_TYPE *)(info->reg_base + OFFSET_OF_VDEC_BASE(IPC_REG_BASE));
	ipc->ap2mcu_memory = info->mcu_exec_addr + IPC_AP2MCU_MEMORY_OFFSET;
	ipc->mcu2ap_memory = info->mcu_exec_addr + IPC_MCU2AP_MEMORY_OFFSET;
	ipc->ack_data_len = 0;
	ipc->device = info->device;
	ipc->mcu_startup_done_notify = info->mcu_startup_done_notify;
	ipc->event_notify = info->event_notify;
	ipc->owner = info->owner;

	mutex_init(&ipc->lock);
	mutex_init(&ipc->seq_lock);
	init_completion(&ipc->sync_msg_complete);
	init_waitqueue_head(&ipc->wait_queue);
	spin_lock_init(&ipc->recv_queue.lock);
	INIT_LIST_HEAD(&ipc->recv_queue.head);
	ipc->state = VDEC_IPC_STATE_POWER_ON;

	ipc->task_kthread = kthread_run(ipc_recv_thread, NULL, "vdec_ipc");
	if (IS_ERR(ipc->task_kthread)) {
		dprint(PRN_ERROR, "create kthread failed!");
		return -EINVAL;
	}
#ifdef CONFIG_VCODEC_OHOS
	trace_ldk_rvh_set_vip_prio(ipc->task_kthread, 1); /* prio 41 */
#endif
	dprint(PRN_ALWS, "init vdec ipc success");

	return 0;
}

void deinit_vdec_ipc(void)
{
	struct list_head *p = NULL;
	struct list_head *n = NULL;
	recv_msg *packet = NULL;
	ipc_entry *ipc = get_ipc_entry();
	unsigned long flag;

	kthread_stop(ipc->task_kthread);
	dprint(PRN_ALWS, "ipc thread exit success");

	spin_lock_irqsave(&ipc->recv_queue.lock, flag);
	list_for_each_safe(p, n, &ipc->recv_queue.head) {
		packet = list_entry(p, recv_msg, list);
		if (packet) {
			list_del(&packet->list);
			free_recv_msg_packet(packet);
		}
	}
	spin_unlock_irqrestore(&ipc->recv_queue.lock, flag);

	mutex_destroy(&ipc->seq_lock);
	mutex_destroy(&ipc->lock);
	ipc->dev = NULL;
	ipc->ap2mcu_memory = NULL;
	ipc->mcu2ap_memory = NULL;
	ipc->ack_data_len = 0;
	ipc->device = NULL;
	ipc->mcu_startup_done_notify = NULL;
	ipc->event_notify = NULL;
	ipc->owner = NULL;
	ipc->state = VDEC_IPC_STATE_POWER_OFF;
	dprint(PRN_ALWS, "deinit vdec ipc success");
}

ipc_error_code send_ipc_sync_msg(int32_t cmd, struct ipc_sync_msg *ipc_msg)
{
	int temp;
	uint64_t timeout = 0;
	static int flag0 = 0;
	static int flag1 = 1;
	ipc_entry *ipc = get_ipc_entry();
	ipc_error_code ret = IPC_FAILURE;
	channel_message message = { 0 };

	mutex_lock(&ipc->seq_lock);
	flag0++;
	temp = flag0;
	mutex_unlock(&ipc->seq_lock);
	while (temp != flag1) {
		msleep(1);
	}

	mutex_lock(&ipc->lock);
	if (ipc->state != VDEC_IPC_STATE_POWER_ON) {
		ret = IPC_FAILURE;
		dprint(PRN_WARN, "error: ipc state %d", ipc->state);
		goto ipc_exit;
	}
	// Check whether the MBX1 channel is in the idle state.
	// If the MBX1 channel is not in the idle state,
	// wait for a period of time and force it to enter the idle state.
	acquire_channel(MBX_1);

	message.mode = SYNC_MODE;
	message.type = IPC_DATA_FROM_ACPU;
	message.cmd = (uint32_t)cmd;
	message.data = ipc_msg->send_msg;
	message.data_len = ipc_msg->send_msg_len;
	write_msg_to_channel(MBX_1, &message);

	timeout = wait_for_completion_timeout(&ipc->sync_msg_complete, msecs_to_jiffies(ipc_msg->wait_time));
	if (unlikely(timeout == 0)) {
		// need add fail handle
		dprint(PRN_ERROR, "process ipc sync msg timeout,ACPU_IRST:%d,MCU_IRST:%d",
			ipc->dev->IRQ_STATUS_REGS[IPC_ACPU_ID].CPU_IRST.u32,
			ipc->dev->IRQ_STATUS_REGS[IPC_MCU_ID].CPU_IRST.u32);
		ret = IPC_TIMEOUT;
	} else {
		dprint(PRN_DBG, "process ipc sync msg success");
		ret = IPC_SUCCESS;
		if (ipc_msg->ack_msg) {
			if (ipc->ap2mcu_memory) {
				mb();
				if (memcpy_s(ipc_msg->ack_msg, sizeof(ipc_vctrl_ret_mcu_to_acpu), ipc->ap2mcu_memory, sizeof(ipc_vctrl_ret_mcu_to_acpu))) {
					dprint(PRN_ERROR, "copy ipc mcu to acpu msg failed");
					ret = IPC_FAILURE;
				}
			} else {
				dprint(PRN_ERROR, "MCU does not return any result");
				ret = IPC_FAILURE;
			}
		}
	}
	release_channel(MBX_1);
ipc_exit:
	mutex_unlock(&ipc->lock);
	flag1++;
	return ret;
}
