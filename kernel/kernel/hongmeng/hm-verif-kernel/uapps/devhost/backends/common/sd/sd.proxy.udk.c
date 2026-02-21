/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 6 10:45:12 2023
 */

#include <hmkernel/errno.h>
#include <hongmeng/errno.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libsysif/utils.h>
#include <devhost/rpc.h>
#include <libdevhost/devhost.h>
#include <libsysif/devhost/api.h>
#include <udk/block/udk_disk.h>
#ifdef CONFIG_HISI_UDK_MAS_BLK
#include <basicplatform/udk/block/mas_blk/mas_blk_flush_interface.h>
#include <basicplatform/udk/block/mas_blk/mas_blk_dump_interface.h>
#include <basicplatform/udk/block/mas_blk/mas_blk_core_interface.h>
#include <basicplatform/udk/block/mas_blk/mas_bkops_core.h>
#endif
#include <udk/log.h>
#include <udk/mm.h>
#include <udk/module.h>
#include <udk/driver.h>
#include <udk/device.h>
#include <udk/of.h>

#include "include/api.h"

#define SD_PROXY_COMPAT		"sd.proxy"
#define SD_PROXY_DEV		"udk_sdproxy"

#define RESOURCE_UREF_INDEX	0
#define RESOURCE_DEVID_INDEX	1

#define MAX_DTB_MSG_LENGTH	2048

struct sd_proxy_s {
	struct sd_proxy_dev_s proxy;
	uint64_t rref;
	int devid;
	unsigned int cnode_idx;
};

struct sd_export_dev_s {
	struct udk_device *dev;
	devnum_t devt;
};

typedef int (*pal_proxy_func)(unsigned int cmd, void *rd, size_t rd_size,
			      void *wr, size_t wr_size);
static pal_proxy_func pal_proxy_func_callbacks[UDK_PAL_PROXY_MAJOR_MAX];
static struct sd_proxy_s proxy_dev;
static struct sd_export_dev_s export_dev;

static void *block_cmd_arg = NULL;
static block_cmd_cb_t block_cmd_fn = NULL;
void set_block_cmd_callback(block_cmd_cb_t fn, void *arg)
{
	block_cmd_fn = fn;
	block_cmd_arg = arg;
}

static void *encrypt_slot_arg = NULL;
static encrypt_slot_cb_t encrypt_slot_fn = NULL;
void set_encrypt_slot_callback(encrypt_slot_cb_t fn, void *arg)
{
	encrypt_slot_fn = fn;
	encrypt_slot_arg = arg;
}

static udk_call_prepare_cb_t udk_call_prepare_fn = NULL;
void set_udk_call_prepare_callback(udk_call_prepare_cb_t fn)
{
	udk_call_prepare_fn = fn;
}

static bool is_valid_major_cmd(unsigned int major_cmd)
{
	return (major_cmd < UDK_PAL_PROXY_MAJOR_MAX);
}

int sd_proxy_invoke(int cmd, struct rpc_ctx *req, struct rpc_ctx *resp)
{
	/* pass in is_xact=false to let sysif server use this thread as caller */
	return devhost_rpc_invoke(proxy_dev.rref, proxy_dev.devid, cmd,
				  req, resp, NULL, false);
}

int udk_pal_proxy_invoke(int cmd, void *rd, size_t rd_size,
		     void *wr, size_t wr_size)
{
	struct rpc_ctx req = {.buf = rd, .size = rd_size};
	struct rpc_ctx resp = {.buf = wr, .size = wr_size};

	/* pass in is_xact=false to let sysif server use this thread as caller */
	return devhost_rpc_invoke(proxy_dev.rref, proxy_dev.devid, cmd,
				  &req, &resp, NULL, false);
}

int udk_pal_proxy_invoke_xact(int cmd, void *rd, size_t rd_size,
			 void *wr, size_t wr_size)
{
	struct rpc_ctx req = {.buf = rd, .size = rd_size};
	struct rpc_ctx resp = {.buf = wr, .size = wr_size};

	return devhost_rpc_invoke(proxy_dev.rref, proxy_dev.devid, cmd,
				  &req, &resp, NULL, true);
}

struct sd_proxy_dev_s *sd_proxy_dev_fetch(void)
{
	return &proxy_dev.proxy;
}

int udk_pal_proxy_register(unsigned int cmd, pal_proxy_func func)
{
	unsigned int major_cmd = PAL_PROXY_MAJOR_CMD_OF(cmd);

	if (func == NULL) {
		return -EINVAL;
	}
	if (!is_valid_major_cmd(major_cmd)) {
		return -EINVAL;
	}
	if (pal_proxy_func_callbacks[major_cmd] != NULL) {
		return -EEXIST;
	}
	pal_proxy_func_callbacks[major_cmd] = func;

	return 0;
}

static int sd_proxy_query_devinfo(void)
{
	int ret;
	char msg[MAX_DTB_MSG_LENGTH] =  { 0 };
	struct rpc_ctx req = {.buf = NULL, .size = 0};
	struct rpc_ctx resp = {.buf = (void *)msg, .size = MAX_DTB_MSG_LENGTH};

	ret = sd_proxy_invoke(PAL_PROXY_COMBINE_CMD(SD_PROXY_MAJOR_CMD, SD_PROXY_MINOR_CMD_QUERY_DEVINFO), &req, &resp);
	if (ret < 0) {
		udk_error("failed to query proxy devinfo, ret=%d\n", ret);
		return ret;
	}

	ret = udk_of_device_create(resp.buf);
	if (ret < 0) {
		udk_error("failed create device by dtb, ret=%d\n", ret);
	}

	return ret;
}

static int sd_proxy_query_dma(void)
{
	int ret;
	struct sd_proxy_dma_s dma = { 0 };
	struct rpc_ctx req = {.buf = NULL, .size = 0};
	struct rpc_ctx resp = {.buf = (void *)&dma, .size = sizeof(dma)};

	ret = sd_proxy_invoke(PAL_PROXY_COMBINE_CMD(SD_PROXY_MAJOR_CMD, SD_PROXY_MINOR_CMD_QUERY_DMA), &req, &resp);
	if (ret == 0) {
		udk_info("sd proxy server resp: dma_size=%lu\n", (unsigned long)dma.size);
		proxy_dev.proxy.dma_paddr = dma.paddr;
		proxy_dev.proxy.dma_size = dma.size;
	}

	return ret;
}

static int sd_proxy_query_status(unsigned int type)
{
	struct rpc_ctx req = {.buf = &type, .size = sizeof(int)};
	struct rpc_ctx resp = {.buf = NULL, .size = 0};

	return sd_proxy_invoke(PAL_PROXY_COMBINE_CMD(SD_PROXY_MAJOR_CMD, SD_PROXY_MINOR_CMD_QUERY_STATUS), &req, &resp);
}

static int sd_proxy_start_error_handler(struct sd_error_handler_s *eh)
{
	struct rpc_ctx req = {.buf = eh, .size = sizeof(*eh)};
	struct rpc_ctx resp = {.buf = NULL, .size = 0};

	return sd_proxy_invoke(PAL_PROXY_COMBINE_CMD(SD_PROXY_MAJOR_CMD, SD_PROXY_MINOR_CMD_START_ERROR_HANDLER), &req, &resp);
}

static void sd_proxy_dev_callback_init(void)
{
	proxy_dev.proxy.query_status = sd_proxy_query_status;
	proxy_dev.proxy.start_error_handler = sd_proxy_start_error_handler;
}

int sd_proxy_bkops(void *buf, unsigned long size)
{
	struct rpc_ctx req = {.buf = buf, .size = size};
	struct rpc_ctx resp = {.buf = NULL, .size = 0};

	return sd_proxy_invoke(
		PAL_PROXY_COMBINE_CMD(SD_PROXY_MAJOR_CMD, SD_PROXY_MINOR_CMD_BKOPS),
		&req, &resp);
}

static int sd_proxy_accept_grant(uint64_t src_uref, uint64_t *accepted_rref)
{
	rref_t recv_rref = hm_ucap_accept_grant(0ULL, (uref_t)src_uref, 0UL);
	if (IS_REF_ERR(recv_rref)) {
		return -hmerrno2posix(REF_TO_ERR(recv_rref));
	}

	*accepted_rref = (uint64_t)recv_rref;
	return 0;
}

/* Make sure that ioctl interface is called by proxy server */
static bool is_from_proxy_server(void)
{
	/* cnode idx == 0 means during initialization phase */
	return (sysif_actv_src_cnode_idx() == proxy_dev.cnode_idx) || (proxy_dev.cnode_idx == 0U);
}

static int sd_proxy_query_info(void)
{
	int ret;
	ret = sd_proxy_query_dma();
	return ret;
}

static void sd_proxy_dev_destroy(void)
{
	if (IS_REF_VALID(proxy_dev.rref)) {
		(void)hm_ucap_reject(0, proxy_dev.rref);
	}
	if (proxy_dev.proxy.dev != NULL) {
		udk_device_destroy(proxy_dev.proxy.dev);
	}
	proxy_dev.devid = 0;
	proxy_dev.rref = 0UL;
	proxy_dev.cnode_idx = 0U;
}

static void __set_udk_call_prepare(bool is_ready)
{
	if (udk_call_prepare_fn != NULL && is_ready)
		udk_call_prepare_fn();
}

static int sd_proxy_dev_create(void *ctx, const char *name,
			   const struct device_create_args *args,
			   struct device_info **info_out)
{
	uint64_t *r = (uint64_t *)args->resource;
	uint64_t uref = r[RESOURCE_UREF_INDEX];
	int devid = (int)r[RESOURCE_DEVID_INDEX];
	int ret = 0;

	UNUSED(ctx, name);
	udk_info("proxy resource devid=%d\n", devid);

	proxy_dev.devid = devid;
	proxy_dev.cnode_idx = __REF_CNODE_IDX_OF(uref);
	proxy_dev.proxy.dev = udk_create_chrdev_duplicated(SD_PROXY_COMPAT, 0, NULL);
	ret = (proxy_dev.proxy.dev == NULL) ? -ENOMEM : 0;

	if (ret == 0) {
		ret = sd_proxy_accept_grant(uref, &proxy_dev.rref);
	}
	if (ret == 0) {
		uref_t dh_uref = libdh_ap_export(__REF_CNODE_IDX_OF(uref));
		ret = udk_pal_proxy_invoke(PAL_PROXY_COMBINE_CMD(SD_PROXY_MAJOR_CMD, SD_PROXY_MINOR_CMD_INIT_RPC_KEY),
					   (void *)&dh_uref, sizeof(uref_t), NULL, 0);
	}

	if (ret == 0) {
		ret = sd_proxy_query_info();
	}
	if (ret == 0) {
		sd_proxy_dev_callback_init();
		*info_out = udk_device_devinfo_of(proxy_dev.proxy.dev);
	}

	if (ret != 0) {
		sd_proxy_dev_destroy();
	}
	proxy_dev.proxy.is_created = true;

	__set_udk_call_prepare(proxy_dev.proxy.is_created);

	return ret;
}

static int __block_cmd(unsigned int cmd, void *wr)
{
	UNUSED(wr);

	if (block_cmd_fn != NULL && block_cmd_arg != NULL) {
		return block_cmd_fn(block_cmd_arg, cmd);
	}

	return 0;
}

static int __set_encrypt_slot(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	uint32_t lock_slot;

	udk_info("enter set encrypt slot, arg=%lx\n", arg);
	if (encrypt_slot_fn != NULL && encrypt_slot_arg != NULL) {
		lock_slot = (uint32_t)arg;
		return encrypt_slot_fn(encrypt_slot_arg, lock_slot);
	}

	return 0;
}

static int __open(struct device_info *dev, void **ctx, unsigned int flags)
{
	UNUSED(dev, ctx, flags);
	return 0;
}

static int __close(void *ctx)
{
	UNUSED(ctx);
	return 0;
}

int udk_pal_proxy_handler(unsigned int cmd, void *rd, size_t rd_size,
				 void *wr, size_t wr_size)
{
	int major_cmd = PAL_PROXY_MAJOR_CMD_OF(cmd);

	if (!is_from_proxy_server()) {
		return -EPERM;
	}
	if (!is_valid_major_cmd(major_cmd)) {
		return -EINVAL;
	}
	if (pal_proxy_func_callbacks[major_cmd] == NULL) {
		return -ENOSYS;
	}

	return pal_proxy_func_callbacks[major_cmd](cmd, rd,
						   rd_size, wr, wr_size);
}

static int __ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	UNUSED(ctx);
	int ret;
	struct rdwr_arg_s rdwr_arg;

	mem_zero_s(rdwr_arg);
	ret = udk_copy_from_user((void *)&rdwr_arg, ulong_to_ptr(arg, struct rdwr_arg_s),
				 sizeof(struct rdwr_arg_s));
	if (ret < 0) {
		return ret;
	}
	return udk_pal_proxy_handler(cmd, (void *)rdwr_arg.rd,
				     (size_t)rdwr_arg.rd_size, (void *)rdwr_arg.wr,
				     (size_t)rdwr_arg.wr_size);
}

static struct devhost_fops export_dev_fops = {
	.open = __open,
	.close = __close,
	.ioctl = __ioctl,
};

static void sd_export_dev_destroy(void)
{
	if (export_dev.devt > 0) {
		udk_unregister_chrdev_region(export_dev.devt, 1);
	}
	if (export_dev.dev != NULL) {
		udk_device_destroy(export_dev.dev);
	}
}

static int __sd_get_device_name(char *u_part_name, size_t u_part_size, char *r_dev_name, size_t r_dev_size)
{
	char *dev_name = NULL;
	char part_name[PARTITION_NAME_LEN] = {0};

	if ((u_part_name == NULL) || (u_part_size == 0) ||
	   (u_part_size >= PARTITION_NAME_LEN) || (r_dev_name == NULL) || (r_dev_size == 0)) {
		return -EINVAL;
	}

	if (udk_copy_from_user(part_name, u_part_name, u_part_size)) {
		return -EFAULT;
	}

	dev_name = udk_disk_get_device_name(part_name);
	if (dev_name == NULL) {
		udk_error("failed to get disk device name\n");
		return -ENODEV;
	}

	if ((strlen(dev_name) + 1) > r_dev_size) {
		udk_error("device name is too long\n");
		free(dev_name);
		return -ENAMETOOLONG;
	}

	if (udk_copy_to_user(r_dev_name, dev_name, strlen(dev_name) + 1)) {
		free(dev_name);
		return -EFAULT;
	}

	free(dev_name);
	return 0;
}

int udk_sd_proxy_common(unsigned int minor_cmd, void *rd, size_t rd_size,
			void *wr, size_t wr_size)
{
	int ret = 0;
	switch (minor_cmd) {
		case UDK_SD_PROXY_MINOR_CMD_BLOCK:
			ret = __block_cmd(minor_cmd, wr);
			break;
		case UDK_SD_PROXY_MINOR_CMD_UNBLOCK:
			ret = __block_cmd(minor_cmd, wr);
			break;
#ifdef CONFIG_HISI_UDK_MAS_BLK
		case UDK_SD_PROXY_MINOR_CMD_PANIC:
			ret = __cfi_mas_blk_panic_notify(minor_cmd, ptr_to_ulong(wr));
			break;
		case UDK_SD_PROXY_MINOR_CMD_POWEROFF:
			ret = __cfi_mas_blk_poweroff_flush_notifier_call(minor_cmd,
									ptr_to_ulong(wr));
			break;
		case UDK_SD_PROXY_MINOR_CMD_SUPPORT_CP:
			ret = mas_blk_cp_enable(minor_cmd, ptr_to_ulong(wr));
			break;
		case UDK_SD_PROXY_MINOR_CMD_SUPPORT_BKOPS:
			ret = mas_blk_bkops_enable(minor_cmd, ptr_to_ulong(wr));
			break;
#endif
		case UDK_SD_PROXY_MINOR_CMD_ENCRYPT_SLOT:
			ret = __set_encrypt_slot(minor_cmd, ptr_to_ulong(wr));
			break;
		case UDK_SD_PROXY_MINOR_CMD_GET_DEVICE_NAME:
			ret = __sd_get_device_name(rd, rd_size, wr, wr_size);
			break;
		default:
			ret = -ENOSYS;
			break;
	}

	return ret;
}

static int sd_export_dev_create(void)
{
	int ret;

	/* never fail during initialization phase */
	(void)udk_pal_proxy_register(PAL_PROXY_MAJOR(UDK_SD_PROXY_MAJOR_CMD), udk_sd_proxy_common);

	ret = udk_alloc_chrdev_region(&export_dev.devt, 0, 1, SD_PROXY_DEV);
	if (ret == 0) {
		export_dev.dev = udk_create_chrdev_duplicated(
			SD_PROXY_DEV, export_dev.devt, &export_dev_fops);
		ret = (export_dev.dev == NULL) ? -ENOMEM : 0;
	}
	if (ret != 0) {
		sd_export_dev_destroy();
	}

	return ret;
}

static struct hm_driver_ops sd_proxy_ops __read_mostly = {
	.bind = NULL,
	.create = sd_proxy_dev_create,
	.owner	= UDK_THIS_MODULE,
};

static int sd_proxy_init(void)
{
	int ret;

	ret = sd_export_dev_create();
	if (ret < 0) {
		udk_error("failed to create udk export device %s\n", SD_PROXY_DEV);
		return ret;
	}

	ret = udk_register_driver(SD_PROXY_COMPAT".udk", SD_PROXY_COMPAT,
				  strlen(SD_PROXY_COMPAT)+1,
				  &sd_proxy_ops, NULL);
	udk_info("%s init ret=%d\n", SD_PROXY_COMPAT, ret);

	if (ret < 0) {
		sd_export_dev_destroy();
	}

	return ret;
}

static int sd_proxy_exit(void)
{
	udk_info("%s exit\n", SD_PROXY_COMPAT);

	sd_proxy_dev_destroy();
	sd_export_dev_destroy();

	return 0;
}

udk_module_init(sd_proxy_init);
udk_module_compat(SD_PROXY_COMPAT);
udk_module_exit(sd_proxy_exit)
udk_module_deps("of.proxy.udk");
