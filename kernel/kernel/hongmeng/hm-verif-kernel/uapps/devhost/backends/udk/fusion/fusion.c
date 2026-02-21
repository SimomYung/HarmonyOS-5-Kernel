/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK actvpool implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */
#include <vfs.h>
#include <libdevhost/devhost.h>
#include <libdevhost/sysfs.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/types.h>
#include <udk/delay.h>
#include <udk/char.h>
#include <udk/device.h>
#include <udk/block/udk_block_stats.h>
#ifdef CONFIG_VFS_BLK_HMCRYPTO
#include <udk/vfs.h>
#include <udk/block/udk_block_fallback.h>
#endif

#include <internal/workqueue.h>
#include <internal/sysfs.h>
#include <internal/init.h>

#include "of.h"
#include "irq.h"
#include "fusion.h"
#include "../modules/of/of_dev.h"

#define UDK_DEVHOST_AP			"udk_ap"
#define UDK_DEVHOST_ADD_WATER		8u
#define UDK_DEVHOST_DEL_WATER		128u
#define UDK_DEVHOST_ACTV_STACK_SIZE	(1UL << (2UL + (unsigned long)PAGE_SHIFT))

static cref_t udk_ap;
static struct actvpool_configs udk_ap_cfg;
static struct udk_workqueue *udk_wq;

static int udk_ap_init(void)
{
	int ret;

	libdh_apconfig_set_default(&udk_ap_cfg, UDK_DEVHOST_AP);
	/* override stack size, default is too small */
	udk_ap_cfg.stack_size = UDK_DEVHOST_ACTV_STACK_SIZE;
	NOFAIL(hm_apconfig_private_init(&udk_ap_cfg));
	NOFAIL(hm_apconfig_private_set_workqueue(&udk_ap_cfg,
				(struct raw_workqueue *)udk_wq, NULL));
	NOFAIL(hm_apconfig_private_set_elasticparam(&udk_ap_cfg, true,
						    UDK_DEVHOST_ADD_WATER,
						    UDK_DEVHOST_DEL_WATER,
						    UDK_DEVHOST_AP));
	ret = libdh_ap_create(&udk_ap_cfg, &udk_ap);
	if (ret < 0) {
		hm_error("failed to create udk ap, ret=%s\n", hmstrerror(ret));
	}

	ret = libdh_init_ap(UDK_DEVHOST_NAME, udk_ap, DH_FLAGS_ENABLE_BACKEND);
	if (ret < 0) {
		hm_error("failed to register libdh ap, ret=%s\n", hmstrerror(ret));
	}

	ret = libdh_sysfs_init();
	if (ret < 0) {
		hm_error("failed to init libdh sysfs, ret=%s\n", hmstrerror(ret));
	}

	return ret;
}

static int udk_ap_wait(void)
{
	return libdh_ap_elastic(&udk_ap_cfg);
}

static int udk_fusion_sysfs_init(void)
{
#define MAX_TRY_TIMES	50
#define WAIT_MSEC	1000
	int try_times = 0;

	while(vfs_access("/sys/block", F_OK) != 0 && try_times < MAX_TRY_TIMES) {
		udk_mdelay(WAIT_MSEC);
		try_times++;
	}

	return (try_times == MAX_TRY_TIMES) ? E_HM_NOENT : posix2hmerrno(-udk_sysfs_block_init());
}

static int udk_fusion_register_to_sysmgr(void)
{
	int ret = E_HM_OK;

/* register udk fusion uref to sysmgr */
	ret = actvxactcall_hmcall_mem_register_devhost(false, false, UDK_FUSION_MAGIC_RREF);
	if (ret != E_HM_OK) {
		hm_error("export uref to sysmgr failed, err=%s\n", hmstrerror(ret));
	}

	return ret;
}

#ifdef CONFIG_VFS_BLK_HMCRYPTO
static int udk_fusion_register_to_fs(void)
{
	int ret = E_HM_OK;

	/* register udk fusion endecrypt to fs */
	ret = udk_vfs_encrypt_decrypt_alg_register(crypto_aes_endecrypt_for_fs, "hm_crypto");
	if (ret != E_HM_OK) {
		hm_error("block crypto register to fs failed\n");
	}

	return ret;
}
#endif

/*
 * Linker gc-sections mode doesn't keep symbols that're not referred from entry,
 * so only refer it after NULL
 */
typedef int (*__init_func)(void);
static __init_func __init_func_arr[] = {
	udk_ap_init,
	udk_fusion_irq_server_init,
	udk_fusion_udk_pm_init,
	udk_fusion_udk_timer_process_init,
	udk_fusion_trace_init,
	udk_fusion_udk_anon_file_init,
	udk_special_device_create,
	udk_fusion_sd_proxy_init,
	udk_fusion_probe_resources_init, /* init probe resources */
	udk_fusion_block_workqueue_init,
	udk_drivers_builtin_init, /* from extmodules */
	udk_fusion_of_init,
	udk_fusion_sysfs_init,
	udk_bio_register_hungtask_notifier,
	udk_fusion_register_to_sysmgr,
	udk_chrdev_init,
#ifdef CONFIG_VFS_BLK_HMCRYPTO
	udk_fusion_register_to_fs,
#endif
	udk_ap_wait,
	NULL, /* end of init func */
};

/* init func in sync mode, irq_init modify sysif_devhost_table before hkip */
static __init_func __init_func_sync_arr[] = {
	udk_fusion_irq_ap_table_init,
	NULL, /* end of init func */
};

static void *__start_udk(void *args)
{
	UNUSED(args);
	hm_info("Welcome to udk fusion world\n");

	BUG_ON(udk_workqueue_default_init(&udk_wq) != 0);
	for (unsigned int i = 0; i < sizeof(__init_func_arr)/sizeof(__init_func); i++) {
		if (__init_func_arr[i] == NULL) {
			break;
		}
		BUG_ON(__init_func_arr[i]() != 0);
	}

	return NULL;
}

/* in sysmgr main process */
static void __start_udk_sync(void)
{
	for (unsigned int i = 0; i < sizeof(__init_func_sync_arr)/sizeof(__init_func); i++) {
		if (__init_func_sync_arr[i] == NULL) {
			break;
		}
		BUG_ON(__init_func_sync_arr[i]() != 0);
	}
}

int udk_fusion_entry(int argc, char **argv)
{
	UNUSED(argc, argv);

	int ret;
	raw_thread_t thread = NULL;
	raw_thread_attr_t attr = { 0 };
	char name[HMCAP_THREAD_NAME_SIZE];

	hm_info("start udk in fusion mode\n");

	/* start_sync func should finish in main process */
	__start_udk_sync();

	ret = raw_thread_attr_init(&attr);
	if (ret == E_HM_OK) {
		ret = raw_thread_create(&thread, &attr, __start_udk, NULL);
	}

	if (ret == E_HM_OK) {
		mem_zero_a(name);
		ret = snprintf_s(name, HMCAP_THREAD_NAME_SIZE, HMCAP_THREAD_NAME_SIZE - 1UL,
				 "udk-ap-%x", hm_ucap_self_cnode_idx());
		BUG_ON(ret < 0);
		ret = hm_thread_setname(raw_thread_cref_of(thread), name, NULL);
	}

	return ret;
}
