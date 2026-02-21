// SPDX-License-Identifier: GPL-2.0
#include <liblinux/pal.h>
#include <liblinux/sched.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/of.h>
#include <scsi/scsi_driver.h>
#include <trace/hooks/liblinux.h>
#include <platform_include/basicplatform/linux/ufs/hufs_plat.h>
#include "sd_proxy.h"

#ifdef CONFIG_HUFS_HC_CORE_UTR
/* HM_KERNEL: UDK_UFS use last 7 cores and 56 slots with enable HUFS_HC_CORE_UTR */
#define UTR_RESERVED_CORE_NUM		7
#define CORE_SLOT_RESERVED_NUM (UTR_RESERVED_CORE_NUM * SLOT_NUM_EACH_CORE)

#define UFS_FULL_SLOT_NUM (SPEC_SLOT_NUM + CORE_SLOT_NUM + CORE_SLOT_RESERVED_NUM)
#else
#define UFS_FULL_SLOT_NUM			32
#define UFS_MSI_EVTID_START			0x45
#define UFS_RESERVED_CORE_MSI_IRQ_START		(8192 + UFS_MSI_EVTID_START)
#define UFS1_RESERVED_CORE_MSI_IRQ_START	(10240 + UFS_MSI_EVTID_START)
#define UFS_RESERVED_CORE_MSI_IRQ_NUM		27
#endif
#define SD_PROXY_MSG_MAX_SIZE	64

#define DFA_MSI_EVTID_START             0xA5
#define DFA_RESERVED_CORE_MSI_IRQ_START (8192 + DFA_MSI_EVTID_START)
#define DFA_RESERVED_CORE_MSI_IRQ_NUM   62

#define PAL_PROXY_MAJOR_MAX	64U

static struct sd_proxy_dev_s ufshcd_proxy_dev = {0};
static bool device_exported = false;
static struct mutex g_fd_lock;
static DEFINE_MUTEX(g_fd_lock);

typedef int (*pal_proxy_func)(unsigned int cmd, void *rd, size_t rd_size,
			      void *wr, size_t wr_size);
static pal_proxy_func pal_proxy_func_callbacks[PAL_PROXY_MAJOR_MAX] = {NULL};

static bool is_valid_sd_proxy_cmd(unsigned int major_cmd)
{
	return (major_cmd < PAL_PROXY_MAJOR_MAX);
}

int liblinux_pal_proxy_register(unsigned int cmd, pal_proxy_func func)
{
	int major_cmd = PAL_PROXY_MAJOR_CMD_OF(cmd);

	if (func == NULL) {
		return -EINVAL;
	}
	if (!is_valid_sd_proxy_cmd(major_cmd)) {
		return -EINVAL;
	}
	if (pal_proxy_func_callbacks[major_cmd] != NULL) {
		return -EEXIST;
	}
	pal_proxy_func_callbacks[major_cmd] = func;

	return 0;
}
EXPORT_SYMBOL(liblinux_pal_proxy_register);

int liblinux_pal_proxy_handler(unsigned int cmd, void *rd, size_t rd_size,
			       void *wr, size_t wr_size)
{
	int major_cmd = PAL_PROXY_MAJOR_CMD_OF(cmd);
	if (!is_valid_sd_proxy_cmd(major_cmd)) {
		return -EINVAL;
	}
	if (pal_proxy_func_callbacks[major_cmd] == NULL) {
		return -ENOSYS;
	}
	return pal_proxy_func_callbacks[major_cmd](cmd, rd, rd_size, wr, wr_size);
}
EXPORT_SYMBOL(liblinux_pal_proxy_handler);

struct rdwr_arg_s {
	void *rd;
	size_t rd_size;
	void *wr;
	size_t wr_size;
};

static int fd = -1;
int liblinux_pal_proxy_udk_invoke(int cmd, void *rd,
				  size_t rd_size, void *wr,
				  size_t wr_size)
{
	int ret;

	if (ufshcd_proxy_dev.rpc_key != 0U) {
		return liblinux_pal_rpc_invoke(ufshcd_proxy_dev.rpc_key,
					       cmd, rd, rd_size,
					       wr, wr_size, false);
	} else {
		pr_info("%s, rpc key uninitialized, fallback to ioctl\n", __func__);
	}

	struct rdwr_arg_s rdwr_arg = {
		.rd = rd,
		.rd_size = rd_size,
		.wr = wr,
		.wr_size = wr_size,
	};

	if (fd < 0) {
		mutex_lock(&g_fd_lock);
		if (fd < 0) {
			fd = liblinux_vfs_open("/dev/udk_sdproxy", O_RDONLY, 0);
			if (fd < 0) {
				mutex_unlock(&g_fd_lock);
				return fd;
			}
		}
		mutex_unlock(&g_fd_lock);
	}

	ret = liblinux_vfs_ioctl(fd, cmd, (unsigned long)&rdwr_arg,
				 sizeof(struct rdwr_arg_s));

	return ret;
}
EXPORT_SYMBOL(liblinux_pal_proxy_udk_invoke);

int liblinux_proxy_udk_invoke_xact(int cmd, void *rd,
				   size_t rd_size, void *wr,
				   size_t wr_size)
{
	if (ufshcd_proxy_dev.rpc_key != 0U) {
		return liblinux_pal_rpc_invoke(ufshcd_proxy_dev.rpc_key,
					       cmd, rd, rd_size,
					       wr, wr_size, true);
	} else {
		pr_info("%s, rpc key uninitialized\n", __func__);
		return -ENOSYS;
	}
}
EXPORT_SYMBOL(liblinux_proxy_udk_invoke_xact);

void sd_proxy_reserved_irq_init(unsigned int irq_start, unsigned int irq_cnt)
{
	ufshcd_proxy_dev.reserved_irq_start = irq_start;
	ufshcd_proxy_dev.reserved_irq_cnt = irq_cnt;
}
EXPORT_SYMBOL(sd_proxy_reserved_irq_init);

struct sd_proxy_dev_s *sd_proxy_dev_fetch(void)
{
	return &ufshcd_proxy_dev;
}
EXPORT_SYMBOL(sd_proxy_dev_fetch);

int sd_proxy_block_cmd(bool is_blocked)
{
	int ret;
	unsigned int cmd;
	char msg[SD_PROXY_MSG_MAX_SIZE];

	if (is_blocked) {
		cmd = PAL_PROXY_COMBINE_CMD(UDK_SD_PROXY_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_BLOCK);
	} else {
		cmd = PAL_PROXY_COMBINE_CMD(UDK_SD_PROXY_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_UNBLOCK);
	}

	ret = liblinux_pal_proxy_udk_invoke(cmd, NULL, 0, (void *)msg, sizeof(msg));

	return ret;
}
EXPORT_SYMBOL(sd_proxy_block_cmd);

int sd_proxy_get_device_name(char *part_name, unsigned int part_len, char *r_dev_name, unsigned int r_dev_size)
{
	unsigned int send_cmd = PAL_PROXY_COMBINE_CMD(UDK_SD_PROXY_MAJOR_CMD, UDK_SD_PROXY_MINOR_CMD_GET_DEVICE_NAME);

	return liblinux_pal_proxy_udk_invoke(send_cmd, (void *)part_name, part_len, (void *)r_dev_name, r_dev_size);
}
EXPORT_SYMBOL(sd_proxy_get_device_name);

void sd_proxy_block_event_cmd(void *args, int event_id)
{
	int ret;
	char msg[SD_PROXY_MSG_MAX_SIZE];
	unsigned int cmd[] = {
		UDK_SD_PROXY_MINOR_CMD_PANIC,
		UDK_SD_PROXY_MINOR_CMD_POWEROFF,
		UDK_SD_PROXY_MINOR_CMD_SUPPORT_CP,
		UDK_SD_PROXY_MINOR_CMD_SUPPORT_BKOPS,
	};
	unsigned int send_cmd;

	if (event_id >= sizeof(cmd)) {
		pr_err("%s event_id %d is err\n", __func__, event_id);
		return;
	}

	send_cmd = PAL_PROXY_COMBINE_CMD(UDK_SD_PROXY_MAJOR_CMD, cmd[event_id]);
	ret = liblinux_pal_proxy_udk_invoke(send_cmd, NULL, 0, (void *)msg, sizeof(msg));
	if (ret)
		pr_err("%s send_cmd %d is err %d\n", __func__, event_id, ret);
	else
		pr_err("%s send_cmd %d success\n", __func__, event_id);
}

INIT_VENDOR_HOOK(ldk_vh_sd_proxy_block_event_cmd, sd_proxy_block_event_cmd);

static unsigned int ufshcd_get_core0_interrupt(struct device_node *of_node)
{
#define LDK_IRQ_CELL_SIZE 3U
#define LDK_IRQ_NO_IDX 1U
#define HM_IRQ_OFFSET 32U
	u32 *interrupts = NULL;

	unsigned int result = 0;
	unsigned int index, offset;

	if (of_property_read_u32(of_node, "core0-interrupt-index", &index) == 0) {
		offset = index * LDK_IRQ_CELL_SIZE + LDK_IRQ_NO_IDX;
		interrupts = kmalloc(sizeof(u32) * (offset + 1), GFP_KERNEL);
		if (of_property_read_u32_array(of_node, "interrupts", &interrupts[0], offset + 1) == 0) {
			result = (unsigned int)interrupts[offset] + HM_IRQ_OFFSET;
		}
		kfree(interrupts);
	}

	return result;
}

static int query_status(unsigned int type)
{
	struct ufs_hba *hba;

	if (ufshcd_proxy_dev.dev == NULL) {
		return -ENODEV;
	}
	hba = (struct ufs_hba *)dev_get_drvdata(ufshcd_proxy_dev.dev);

	switch (type) {
	case SD_PROXY_STATUS_SUSPEND:
		return hba->is_sys_suspended ? 1 : 0;
	default:
		return -ENOSYS;
	}

	return -ENOSYS;
}

void scsi_schedule_eh(struct Scsi_Host *shost);
#define SCSI_HOST_WAIT_EH_TIME	1000
#define SCSI_HOST_WAIT_EH_NR	600

static int scsi_remote_eh_invoke(struct Scsi_Host *shost, struct sd_error_handler_s *eh)
{
	int ret = 0;
	int i = 0;

	pr_info("scsi: host %u(state %u) sched error handler from remote\n",
		shost->host_no, shost->shost_state);

	while (scsi_host_in_recovery(shost)) {
		msleep(SCSI_HOST_WAIT_EH_TIME);
		if (++i > SCSI_HOST_WAIT_EH_NR) {
			pr_info("scsi: host recoverying timed out\n");
			return -ETIMEDOUT;
		}
	}
	shost->remote_error_handler = true;
	shost->host_reseted = false;
	shost->remote_lun = eh->lun;
	shost->force_hostreset = eh->force_hostreset;
	scsi_schedule_eh(shost);

	i = 0;
	while (shost->remote_error_handler) {
		msleep(SCSI_HOST_WAIT_EH_TIME);
		if (++i > SCSI_HOST_WAIT_EH_NR) {
			pr_info("scsi: host error handle timed out\n");
			return -ETIMEDOUT;
		}
	}
	if (shost->host_reseted) {
		shost->host_reseted = false;
		ret = 1;
	}

	pr_info("scsi: error handler finished\n");
	return ret;
}

static int start_error_handler(struct sd_error_handler_s *eh)
{
	struct ufs_hba *hba;

	pr_info("scsi: invoke error handler from remote, lun=%lu force_hostreset=%d\n",
		eh->lun, eh->force_hostreset);

	if (ufshcd_proxy_dev.dev == NULL) {
		return -ENODEV;
	}
	hba = (struct ufs_hba *)dev_get_drvdata(ufshcd_proxy_dev.dev);

	return scsi_remote_eh_invoke(hba->host, eh);
}

static void export_ufschd_proxy_device(struct ufs_hba *hba)
{
	size_t utrdl_size;
	struct hufs_host *host = (struct hufs_host *)hba->priv;

	utrdl_size = ALIGN((sizeof(struct hufs_utp_transfer_req_desc) *
			    (unsigned int)(UFS_FULL_SLOT_NUM)), PAGE_SIZE);

	ufshcd_proxy_dev.dma_paddr = hba->utrdl_dma_addr;
	ufshcd_proxy_dev.dma_size = utrdl_size;
	ufshcd_proxy_dev.dev = hba->dev;
#ifdef CONFIG_HUFS_HC_CORE_UTR
	ufshcd_proxy_dev.reserved_irq_start =
		ufshcd_get_core0_interrupt(hba->dev->of_node) + UTR_CORE_NUM;
	ufshcd_proxy_dev.reserved_irq_cnt = UTR_RESERVED_CORE_NUM;
#else
	if (host->host_id == 1)
		ufshcd_proxy_dev.reserved_irq_start = UFS1_RESERVED_CORE_MSI_IRQ_START;
	else
		ufshcd_proxy_dev.reserved_irq_start = UFS_RESERVED_CORE_MSI_IRQ_START;
	ufshcd_proxy_dev.reserved_irq_cnt = UFS_RESERVED_CORE_MSI_IRQ_NUM;
#endif
	if (is_dfa_enabled(hba)) {
		ufshcd_proxy_dev.reserved_irq_start = DFA_RESERVED_CORE_MSI_IRQ_START;
		ufshcd_proxy_dev.reserved_irq_cnt = DFA_RESERVED_CORE_MSI_IRQ_NUM;
	}
	ufshcd_proxy_dev.query_status = query_status;
	ufshcd_proxy_dev.start_error_handler = start_error_handler;
}

static int sd_helper_probe(struct device *dev)
{
	struct scsi_device *sdp = to_scsi_device(dev);
	struct Scsi_Host *shost = sdp->host;
	struct ufs_hba *hba = NULL;
	int ret = 0;

	if (sdp->type != TYPE_DISK || strcmp(shost->hostt->name, "ufshcd"))
		return -ENODEV;

	hba = shost_priv(shost);
	if (!device_exported) {
		device_exported = true;
		export_ufschd_proxy_device(hba);
		ret = liblinux_pal_request_module("sd.proxy.ko");
		pr_info("ldk request sd.proxy.ko, ret=%d\n", ret);
		BUG_ON(ret != 0);
	}

	return 0;
}

static int sd_helper_remove(struct device *dev)
{
	return 0;
}

static struct scsi_driver sd_helper_template = {
	.gendrv = {
		.name		= "sd_helper",
		.owner		= THIS_MODULE,
		.probe		= sd_helper_probe,
		.remove		= sd_helper_remove,
	},
};

static int __init sd_helper_init(void)
{
	int err;

	err = scsi_register_driver(&sd_helper_template.gendrv);
	if (err)
		return err;

	return 0;
}

static void __exit sd_helper_exit(void)
{
	scsi_unregister_driver(&sd_helper_template.gendrv);
}

subsys_initcall_sync(sd_helper_init);
module_exit(sd_helper_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Huawei OS Kernel Lab");
MODULE_DESCRIPTION("SCSI Device Helper Drive");
MODULE_VERSION("0.1");
