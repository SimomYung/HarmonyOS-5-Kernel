
#ifndef VFMW_IPC_H
#define VFMW_IPC_H

#include "vcodec_types.h"

#define MAX_VCTRL_PRM_LEN (6U * 1024U)
#define MAX_BUF_VCTRL_PRM_LEN (5U * 1024U + 512U)
#define MAX_PROC_STR_LEN  (255)

typedef struct {
	uint32_t chanId;
	uint32_t cmdId;
	uint32_t paramLen;
	uint8_t args[MAX_VCTRL_PRM_LEN];
} ipc_vctrl_ioctl;

typedef struct {
    int32_t res;
    int32_t chan_id;
} ipc_vctrl_ret_mcu_to_acpu;

struct ipc_vctrl_msg {
	ipc_vctrl_ioctl *input;
	ipc_vctrl_ret_mcu_to_acpu *output;
};

struct ipc_proc_msg {
	char input[MAX_PROC_STR_LEN];
	ipc_vctrl_ret_mcu_to_acpu output;
	uint32_t str_len;
};

struct ipc_destroy_msg {
	uint32_t chan_id;
	ipc_vctrl_ret_mcu_to_acpu output;
};

#endif