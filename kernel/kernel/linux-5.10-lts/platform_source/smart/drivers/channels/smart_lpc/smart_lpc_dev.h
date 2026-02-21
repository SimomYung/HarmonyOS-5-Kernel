/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2021. All rights reserved.
 * Description: dev drvier to communicate with sensorhub lpc app
 * Create: 2024-11-30
 */
 
#ifndef __LINUX_CONTEXTHUB_LPC_H__
#define __LINUX_CONTEXTHUB_LPC_H__
#include <linux/types.h>
 
/* ioctl cmd define */
#define LPC_IO                         0xBB
 
#define LPC_IOCTL_LPC_OPEN         _IOW(LPC_IO, 0xD1, short)
#define LPC_IOCTL_LPC_CLOSE        _IOW(LPC_IO, 0xD2, short)
 
#define LPC_IOCTL_ION_REF_ATTACH     _IOW(LPC_IO, 0xD7, short)
#define LPC_IOCTL_ION_REF_DETACH     _IOW(LPC_IO, 0xD8, short)
 
struct lpc_ion_info_t {
	struct dma_buf *buf;
	u64 id;
	int fd;
};
 
struct lpc_upload_t {
	u32 session_id;
	u32 notify_type;
	u32 notify_len;
	u32 reserved;
};
 
#endif