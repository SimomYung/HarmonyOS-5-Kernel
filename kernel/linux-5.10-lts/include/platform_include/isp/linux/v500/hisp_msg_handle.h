/*
 * hisp_msg_handle.h
 *
 * Copyright (c) 2023 ISP Technologies CO., Ltd.
 *
 */

#ifndef _HISP_MSG_HANDLE_H
#define _HISP_MSG_HANDLE_H

#include <linux/remoteproc.h>
#include <platform_include/isp/linux/hisp_remoteproc.h>
#include <platform_include/isp/linux/hisp_mempool.h>
#include <linux/rpmsg.h>

struct hisp_msg_req_mem_record_s {
	unsigned int index;
	unsigned int offset;
	unsigned int mem_info;
	unsigned int size;
	unsigned long long cpu_addr;
	unsigned long long cpu_size;
	unsigned int core_addr;
	unsigned int core_size;
	unsigned int oisp_addr;
	unsigned int oisp_size;
	unsigned int prot;
	unsigned int state;
};

struct hisp_msg_ack_mem_record_s {
	unsigned int status;
};

struct hisp_msg_req_mem_unrecord_s {
	unsigned int index;
	unsigned int state;
};

struct hisp_msg_ack_mem_unrecord_s {
	unsigned int status;
};

struct hisp_msg_req_npu_clk_set_s {
    int type;
    unsigned int clk_rate;
};

struct hisp_msg_ack_npu_clk_set_s {
    int state;
};

int hisp_msg_mem_record(void *args);
int hisp_msg_mem_unrecord(struct hisp_mem_type type, void *args);

#endif /* _HISP_MSG_HANDLE_H */
