#ifndef VCODEC_MCU_QUEUEQ_H
#define VCODEC_MCU_QUEUEQ_H

#include <linux/types.h>
#include <linux/cdev.h>
#include "vcodec_types.h"
#include "dev_mgr.h"
#include "vfmw_queue.h"

typedef enum {
	QUEUE_MSG_TYPE_STREAM_SYNC,
	QUEUE_MSG_TYPE_RELEASE_IMG,
	QUEUEQ_MSG_TYPE_BUTT,
} queue_msg_type;

struct queue_init_info {
	void *owner;
	void *mcu_exec_addr;
};

int32_t init_vdec_queue(struct queue_init_info *info);
void deinit_vdec_queue(void);
void init_output_event_queue_of_one_chan(int32_t chan_id);
int32_t write_input_queue_msg(int32_t cmd, void *send_msg, uint32_t send_msg_len);
int32_t get_output_event_list(event_output_list *output_list);

#endif