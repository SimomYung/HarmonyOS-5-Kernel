/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: devhost rpc sd headers
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 8 09:40:43 2023
 */

#ifndef __DEVHOST_API_RPC_SD_H__
#define __DEVHOST_API_RPC_SD_H__

#include <devhost/rpc.h>

struct sd_proxy_dma_s {
	uint64_t paddr;
	size_t size;
};

struct sd_error_handler_s {
	unsigned long lun;
	int force_hostreset;
	int pad;
};

struct sd_proxy_dev_s {
#ifdef UDK_MODULE
	struct udk_device *dev;
#else
	struct device *dev;
#endif
	uint64_t dma_paddr;
	size_t dma_size;
	unsigned int reserved_irq_start;
	unsigned int reserved_irq_cnt;
	bool is_created;
	uint64_t rpc_key;
	int (*query_status)(unsigned int type);
	int (*start_error_handler)(struct sd_error_handler_s *eh);
};

#ifdef UDK_MODULE
typedef int (*sd_proxy_func)(unsigned int cmd, unsigned long arg);
#else
typedef int (*sd_proxy_func)(unsigned int cmd, struct devhost_rpc_req *req, struct devhost_rpc_resp *resp);
#endif
int sd_proxy_ops_register(int cmd, sd_proxy_func func);
int sd_proxy_invoke(int cmd, struct rpc_ctx *req, struct rpc_ctx *resp);
struct sd_proxy_dev_s *sd_proxy_dev_fetch(void);

typedef int (*block_cmd_cb_t)(void *arg, unsigned int type);
void set_block_cmd_callback(block_cmd_cb_t fn, void *arg);

typedef int (*encrypt_slot_cb_t)(void *arg, uint32_t lock_slot);
void set_encrypt_slot_callback(encrypt_slot_cb_t fn, void *arg);
int sd_proxy_bkops(void *buf, unsigned long size);

/* for UDK/LDK pal proxy */
#ifndef PAL_PROXY_MAJOR_CMD_OF
#define PAL_PROXY_MAJOR_CMD_OF(x)		(((x) >> 16) & 0xFFFF)
#endif
#ifndef PAL_PROXY_MINOR_CMD_OF
#define PAL_PROXY_MINOR_CMD_OF(x)		((x) & 0xFFFF)
#endif
#ifndef PAL_PROXY_MAJOR
#define PAL_PROXY_MAJOR(x)			((((x) & 0xFFFF) << 16))
#endif
#ifndef PAL_PROXY_COMBINE_CMD
#define PAL_PROXY_COMBINE_CMD(x, y)		((PAL_PROXY_MAJOR(x)) | ((y) & 0xFFFF))
#endif

/* for UDK call LDK prepare */
typedef void (*udk_call_prepare_cb_t)(void);
void set_udk_call_prepare_callback(udk_call_prepare_cb_t fn);

/* for UDK call LDK */
int udk_pal_proxy_invoke(int cmd, void *rd, size_t rd_size, void *wr, size_t wr_size);
int udk_pal_proxy_invoke_xact(int cmd, void *rd, size_t rd_size, void *wr, size_t wr_size);
int liblinux_pal_proxy_register(unsigned int cmd, int (*func)(unsigned int cmd, void *rd,
							      size_t rd_size, void *wr,
							      size_t wr_size));
int liblinux_pal_proxy_handler(unsigned int cmd, void *rd, size_t rd_size,
			       void *wr, size_t wr_size);

enum major_cmd_t {
	SD_PROXY_MAJOR_CMD = 0,
	DMA_BUF_MAJOR_CMD,
	NPU_MAJOR_CMD,
	HTS_MAJOR_CMD,
	SD_PROXY_PCIE_MAJOR_CMD = 6,
	NVME_PROXY_MAJOR_CMD = 7,
	DMA_BUF_EXT_MAJOR_CMD = 8,
	FFTS_MAJOR_CMD = 9,
	PAL_PROXY_MAJOR_MAX,
};

enum minor_cmd_sd_proxy_t {
	SD_PROXY_MINOR_CMD_HELLO = 0,
	SD_PROXY_MINOR_CMD_QUERY_DEVINFO,
	SD_PROXY_MINOR_CMD_QUERY_DMA,
	SD_PROXY_MINOR_CMD_QUERY_STATUS,
	SD_PROXY_MINOR_CMD_BKOPS,
	SD_PROXY_MINOR_CMD_START_ERROR_HANDLER,
	SD_PROXY_MINOR_CMD_INIT_RPC_KEY,
};

enum minor_dma_buf_t {
	DMA_BUF_MINOR_CMD_COPY = 0,
};

enum sd_proxy_status_t {
	SD_PROXY_STATUS_SUSPEND = 0,
};

enum sd_proxy_npu_t {
	SD_PROXY_MINOR_CMD_NPU = 0,
};

/* for LDK call UDK */
struct rdwr_arg_s {
	void *rd;
	size_t rd_size;
	void *wr;
	size_t wr_size;
};

int liblinux_pal_rpc_invoke(uint64_t rpc_key, int cmd, void *rd,
				size_t rd_size, void *wr,
				size_t wr_size, bool is_xact);
int liblinux_pal_proxy_udk_invoke(int cmd, void *rd,
				  size_t rd_size, void *wr, size_t wr_size);
int liblinux_proxy_udk_invoke_xact(int cmd, void *rd,
				   size_t rd_size, void *wr, size_t wr_size);
int udk_pal_proxy_register(unsigned int cmd, int (*func)(unsigned int cmd, void *rd,
							 size_t rd_size, void *wr,
							 size_t wr_size));
int udk_pal_proxy_handler(unsigned int cmd, void *rd, size_t rd_size,
			  void *wr, size_t wr_size);

enum udk_major_cmd_t {
	UDK_SD_PROXY_MAJOR_CMD = 0,
	UDK_DMA_BUF_MAJOR_CMD,
	UDK_FFTS_MAJOR_CMD,
	UDK_UNIFORM_ID_MAJOR_CMD,
	UDK_HTS_MAJOR_CMD,
	UDK_NPU_MAJOR_CMD,
	UDK_SD_PROXY_PCIE_MAJOR_CMD = 6,
	UDK_NVME_PROXY_MAJOR_CMD = 7,
	UDK_NPU_DEBUGFS_MAJOR_CMD,
	UDK_PAL_PROXY_MAJOR_MAX,
};

enum udk_minor_cmd_sd_proxy_t {
	UDK_SD_PROXY_MINOR_CMD_HELLO = 0,
	UDK_SD_PROXY_MINOR_CMD_BLOCK,
	UDK_SD_PROXY_MINOR_CMD_UNBLOCK,
	UDK_SD_PROXY_MINOR_CMD_PANIC,
	UDK_SD_PROXY_MINOR_CMD_POWEROFF,
	UDK_SD_PROXY_MINOR_CMD_SUPPORT_CP,
	UDK_SD_PROXY_MINOR_CMD_ENCRYPT_SLOT,
	UDK_SD_PROXY_MINOR_CMD_SUPPORT_BKOPS,
	UDK_SD_PROXY_MINOR_CMD_GET_DEVICE_NAME,
	UDK_SD_PROXY_MINOR_CMD_MAX,
};

enum udk_minor_cmd_dma_buf_t {
	UDK_DMA_BUF_MINOR_CMD_COPY = 0,
};
#endif
