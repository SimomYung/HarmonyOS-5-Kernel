#include <linux/mutex.h>
#include "vcodec_mcu_queue.h"
#include "vcodec_mcu_ddr_map.h"
#include "dbg.h"
#include "vcodec_osal.h"

#define WAIT_MSG_SHARE_NODE_MS         25
#define RETRY_TIMES_WHEN_NO_FREE_NODE  2

typedef struct {
	// used for lock queue interface
	struct mutex msg_lock;
	// control msg queue
	volatile msg_queue *input_msg_queue;
	// event queue
	volatile event_queue *output_event_queue[VFMW_CHAN_NUM];
	void *owner;
} queue_entry;

static queue_entry *get_queue_entry(void)
{
	static queue_entry g_queue_entry;
	return &g_queue_entry;
}

static void init_input_msg_queue(struct queue_init_info *info, queue_entry *entry)
{
	uint32_t i;
	entry->input_msg_queue = info->mcu_exec_addr + INPUT_MSG_QUEUE_OFFSET;
	entry->input_msg_queue->read = 0;
	entry->input_msg_queue->write = 0;
	entry->input_msg_queue->size = MAX_MSG_CNT;

	for (i = 0; i < entry->input_msg_queue->size; i++) {
		entry->input_msg_queue->msgs[i].used = false;
	}
}

void init_output_event_queue_of_one_chan(int32_t chan_id)
{
	uint32_t j;
	queue_entry *entry = get_queue_entry();
	entry->output_event_queue[chan_id]->read = 0;
	entry->output_event_queue[chan_id]->write = 0;
	entry->output_event_queue[chan_id]->size = MAX_EVENT_CNT;

	for (j = 0; j < entry->output_event_queue[chan_id]->size; j++) {
		entry->output_event_queue[chan_id]->node_list[j].used = false;
	}
}

static void init_output_event_queue(struct queue_init_info *info, queue_entry *entry)
{
	int32_t i;
	for (i = 0; i < VFMW_CHAN_NUM; i++) {
		entry->output_event_queue[i] =
			info->mcu_exec_addr + OUTPUT_MSG_QUEUE_OFFSET + OUTPUT_MSG_QUEUE_ONE_CHAN_SIZE * i;
		init_output_event_queue_of_one_chan(i);
	}
}

int32_t init_vdec_queue(struct queue_init_info *info)
{
	queue_entry *entry = get_queue_entry();
	if (unlikely(!info || !info->mcu_exec_addr || !info->owner))
		return -EINVAL;

	entry->owner = info->owner;
	mutex_init(&(entry->msg_lock));
	init_input_msg_queue(info, entry);
	init_output_event_queue(info, entry);

	dprint(PRN_ALWS, "init vdec_queue success");
	return 0;
}

void deinit_vdec_queue(void)
{
	int32_t index;
	queue_entry *entry = get_queue_entry();

	entry->owner = NULL;
	mutex_destroy(&(entry->msg_lock));
	entry->input_msg_queue = NULL;

	for (index = 0; index < VFMW_CHAN_NUM; index++) {
		entry->output_event_queue[index] = NULL;
	}
	dprint(PRN_ALWS, "deinit vdec_queue success");
}

static int32_t try_write_msg_node(int32_t cmd, void *send_msg, uint32_t send_msg_len)
{
	uint32_t write;
	int32_t ret = 0;
	volatile msg_node *node = NULL;
	queue_entry *entry = get_queue_entry();
	volatile msg_queue *queue = entry->input_msg_queue;

	mb();
	mutex_lock(&(entry->msg_lock));
	write = queue->write;
	// verifying write index to prevent kernel panic caused by share memory abnormal
	if (write >= queue->size) {
		dprint(PRN_ERROR, "the write index:%u of share memory out of bounds", write);
		ret = -EFAULT;
		goto unlock;
	}

	node = &(queue->msgs[write]);
	if (node->used) {
		ret = -ENOSPC;
		goto unlock;
	}

	node->cmd = cmd;
	node->size = send_msg_len;
	if (memcpy_s((void *)node->msg, sizeof(node->msg), send_msg, send_msg_len)) {
		dprint(PRN_ERROR, "copy to msg error");
		ret = -EFAULT;
		goto unlock;
	}

	node->used = true;
	queue->write = (write + 1) % queue->size;

unlock:
	mutex_unlock(&(entry->msg_lock));
	return ret;
}

int32_t write_input_queue_msg(int32_t cmd, void *send_msg, uint32_t send_msg_len)
{
	int32_t ret = 0;
	uint32_t try = 0;

	while (try < RETRY_TIMES_WHEN_NO_FREE_NODE) {
		ret = try_write_msg_node(cmd, send_msg, send_msg_len);
		if (ret == -ENOSPC) {
			dprint(PRN_ALWS, "queue msg memory used out, wait and retry");
			msleep(WAIT_MSG_SHARE_NODE_MS);
			try++;
			continue;
		}

		return ret;
	}

	dprint(PRN_ERROR, "wait free msg node failed");
	return -ENOSPC;
}

int32_t get_output_event_list(event_output_list *output_list)
{
	uint32_t read;
	volatile event_queue *queue = NULL;
	uint32_t event_count = 0;
	queue_entry *entry = get_queue_entry();
	size_t node_size = sizeof(event_node);
	size_t buffer_size = sizeof(output_list->event_output_buffer.upstream_buffer);
	struct vdec_chan_listnode *chan_pos = NULL;
	struct vdec_chan_list *chan_list = output_list->chan_list;

	mb();
	if (unlikely(!chan_list))
		return -EINVAL;
	mutex_lock(&chan_list->lock);
	list_for_each_entry(chan_pos, &chan_list->chan_head, head) {
		int32_t chan_id = chan_pos->chan_id;
		if (chan_id >= VFMW_CHAN_ID_MAX || chan_id < VFMW_NO_SEC_CHAN_START) {
			dprint(PRN_ERROR, "chan id error %d ", chan_id);
			mutex_unlock(&chan_list->lock);
			return -1;
		}
		queue = entry->output_event_queue[chan_id - VFMW_NO_SEC_CHAN_START];
		read = queue->read;
		// verifying read index to prevent kernel panic caused by share memory abnormal
		if (read >= queue->size) {
			mutex_unlock(&chan_list->lock);
			return -1;
		}

		while (buffer_size >= node_size) {
			volatile event_node *node = &(queue->node_list[read]);
			if (!node->used)
				break;

			if (node->param_len > MAX_OUTPUT_EVENT_SIZE) {
				dprint(PRN_ERROR, "param_len exceeds the max event size");
				break;
			}

			// event real len is node->param_len, copy len need reduce unused size
			if (memcpy_s(output_list->event_output_buffer.upstream_buffer + event_count * node_size, buffer_size,
				(void *)node, node_size - MAX_OUTPUT_EVENT_SIZE + node->param_len)) {
				dprint(PRN_ERROR, "copy to upstream_buffer error");
				break;
			}
			node->used = 0;
			buffer_size -= node_size;
			event_count++;
			read = (read + 1) % queue->size;
			queue->read = read;
		}
		output_list->event_output_buffer.event_count = event_count;
	}
	mutex_unlock(&chan_list->lock);
	return 0;
}

