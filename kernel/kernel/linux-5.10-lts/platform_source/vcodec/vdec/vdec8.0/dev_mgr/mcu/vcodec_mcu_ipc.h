#ifndef VCODEC_MCU_IPC_H
#define VCODEC_MCU_IPC_H

#include <linux/types.h>
#include <linux/cdev.h>
#include "vcodec_types.h"
#include "dev_mgr.h"
#include "vfmw_ipc.h"

typedef enum {
	IPC_MSG_TYPE_VCTRL_COMMAND,
	IPC_LITEOS_INIT_DONE,
	IPC_MSG_TYPE_PROCMSG,
	IPC_MSG_TYPE_RESET_SCD,
	IPC_MSG_TYPE_CLEAR_BSP0,
	IPC_MSG_TYPE_CLEAR_BSP1,
	IPC_MSG_TYPE_RESET_BSP,
	IPC_MSG_TYPE_RESET_PXP,
	IPC_MSG_TYPE_RESET_VDEC,
	IPC_MSG_TYPE_EVENT_REPORT,
	IPC_MSG_TYPE_VCTRL_SR_CMD,
	IPC_MSG_TYPE_LOCK_SCD_HW,
	IPC_MSG_TYPE_UNLOCK_SCD_HW,
	IPC_MSG_TYPE_ALLOC_MCU_HEAP,
	IPC_MSG_TYPE_FREE_MCU_HEAP,
	IPC_MSG_TYPE_ABNORMAL_CLOSE,
	IPC_MSG_TYPE_STREAM_ERROR,
	IPC_MSG_TYPE_ALLOC_SEG_BUFFER,
	IPC_MSG_TYPE_BUTT,
} ipc_msg_type;

typedef enum {
	IPC_TIMEOUT = -2,
	IPC_FAILURE = -1,
	IPC_SUCCESS = 0
} ipc_error_code;

typedef struct {
	uint32_t size;
	uint32_t iova;
} ipc_alloc_mem;

struct ipc_init_info {
	struct device *device;
	uint8_t *reg_base;
	void *owner;
	void *mcu_exec_addr;
	void (*mcu_startup_done_notify)(void *);
	void (*event_notify)(enum vcodec_dev_event_notify, void *);
};

typedef enum {
	VDEC_IPC_STATE_POWER_ON,
	VDEC_IPC_STATE_POWER_OFF,
	VDEC_IPC_STATE_BUTT,
} vdec_ipc_state_type;

#define MCU_IPC_WAIT_MS     1000
#define MCU_IPC_SR_WAIT_MS  200
struct ipc_sync_msg {
	void *send_msg;
	uint32_t send_msg_len;
	ipc_vctrl_ret_mcu_to_acpu *ack_msg;
	uint32_t wait_time;
};

int32_t init_vdec_ipc(struct ipc_init_info *info);
void deinit_vdec_ipc(void);
ipc_error_code send_ipc_sync_msg(int32_t cmd, struct ipc_sync_msg *ipc_msg);
void ipc_set_state(vdec_ipc_state_type state);
#endif
