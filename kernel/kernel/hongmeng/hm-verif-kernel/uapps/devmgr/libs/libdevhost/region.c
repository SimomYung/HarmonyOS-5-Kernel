/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: libdh chr/blk device region helpers
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 10 14:45:23 2020
 */
#include <libdevhost/devhost.h>

#include <string.h>
#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libhwsecurec/securec.h>

int libdh_register_chrdev_region(unsigned int major,
				 unsigned int baseminor, int minorct,
				 const char *name,
				 unsigned int *major_out)
{
	struct hm_actv_buf chr_major_name;
	struct __actvret_drvcall_devmgr_chrdev_register_region drvcall_ret;
	int ret = E_HM_OK;

	if (name == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hm_actv_buf_init(&chr_major_name, name, strlen(name) + 1U);

		ret = actvcapcall_drvcall_devmgr_chrdev_register_region(
				libdh_get_devmgr_ap(),
				major, baseminor, minorct, chr_major_name,
				&drvcall_ret);
	}

	if (ret == E_HM_OK) {
		if (major_out != NULL) {
			*major_out = drvcall_ret.major;
		}
	}

	return ret;
}

void libdh_unregister_chrdev_region(unsigned int major,
				    unsigned int baseminor, int minorct)
{
	int ret;

	ret = actvcapcall_drvcall_devmgr_chrdev_unregister_region(
		libdh_get_devmgr_ap(),
		major, baseminor, minorct);
	if (ret < 0) {
		hm_warn("unregister chrdev region %u failed, err=%s\n",
			major, hmstrerror(ret));
	}
}

int libdh_register_blkdev_region(unsigned int major,
				 const char *name,
				 unsigned int *major_out)
{
	struct hm_actv_buf blk_major_name;
	struct __actvret_drvcall_devmgr_blkdev_register_region drvcall_ret;
	int ret = E_HM_OK;

	if (name == NULL) {
		ret = E_HM_INVAL;
		goto err_out;
	}

	hm_actv_buf_init(&blk_major_name, name, strlen(name) + 1U);

	ret = actvcapcall_drvcall_devmgr_blkdev_register_region(
			libdh_get_devmgr_ap(),
			major,
			blk_major_name,
			&drvcall_ret);
	if (ret < 0) {
		goto err_out;
	}

	if (major_out != NULL) {
		*major_out = drvcall_ret.major;
	}
err_out:
	return ret;
}

void libdh_unregister_blkdev_region(unsigned int major, const char *name)
{
	struct hm_actv_buf blk_major_name;
	int ret;

	if (name != NULL) {
		hm_actv_buf_init(&blk_major_name, name, strlen(name) + 1U);

		ret = actvcapcall_drvcall_devmgr_blkdev_unregister_region(
				libdh_get_devmgr_ap(),
				major, blk_major_name);
		if (ret < 0) {
			hm_warn("unregister blkdev region %u failed, err=%s\n",
				major, hmstrerror(ret));
		}
	}
}
