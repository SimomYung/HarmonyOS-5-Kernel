/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SD_PROXY_H
#define _SD_PROXY_H

#include <linux/device.h>

struct sd_error_handler_s {
	unsigned long lun;
	int force_hostreset;
	int pad;
};

struct sd_proxy_dev_s {
	struct device *dev;
	uint64_t dma_paddr;
	size_t dma_size;
	unsigned int reserved_irq_start;
	unsigned int reserved_irq_cnt;
	bool is_created;
	uint64_t rpc_key;
	int (*query_status)(unsigned int type);
	int (*start_error_handler)(struct sd_error_handler_s *eh);
	void (*free_dev_irq)(int linux_irq, int cnt);
	void (*set_its_base)(uint64_t its_base);
};

enum major_cmd_t {
	SD_PROXY_MAJOR_CMD = 0,
	DMA_BUF_MAJOR_CMD,
	NPU_MAJOR_CMD,
	HTS_MAJOR_CMD,
	DMA_BUF_EXT_MAJOR_CMD = 8,
	FFTS_MAJOR_CMD = 9,
	MMC_PROXY_MAJOR_CMD = 10,
	HICRYPTO_PROXY_MAJOR_CMD = 11,
	PAL_PROXY_MAJOR_MAX,
};

enum sd_proxy_status_t {
	SD_PROXY_STATUS_SUSPEND = 0,
};

/* for LDK call UDK */
enum udk_major_cmd_t {
	UDK_SD_PROXY_MAJOR_CMD = 0,
	UDK_DMA_BUF_MAJOR_CMD,
	UDK_FFTS_MAJOR_CMD,
	UDK_UNIFORM_ID_MAJOR_CMD,
	UDK_HTS_MAJOR_CMD,
	UDK_NPU_MAJOR_CMD,
	UDK_NVME_PROXY_MAJOR_CMD = 7,
	UDK_NPU_DEBUGFS_MAJOR_CMD,
	UDK_MMC_PROXY_MAJOR_CMD = 10,
	UDK_HICRYPTO_PROXY_MAJOR_CMD = 11,
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

int sd_proxy_block_cmd(bool is_blocked);
extern struct sd_proxy_dev_s *sd_proxy_dev_fetch(void);
extern void sd_proxy_reserved_irq_init(unsigned int irq_start, unsigned int irq_cnt);
extern int sd_proxy_get_device_name(char *part_name, unsigned int part_len,
				    char *r_dev_name, unsigned int r_dev_size);

#endif /* __SD_PROXY_H__ */
