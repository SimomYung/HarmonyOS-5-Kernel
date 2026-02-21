/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: libdh device helpers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 21 13:43:57 2020
 */
#include <libdevhost/devhost.h>

#include <vfs.h>
#include <errno.h>
#include <unistd.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libhwsecurec/securec.h>

#include "internals.h"

static int __chrdev_create(const char *name, unsigned int devt,
			   unsigned int umode,
			   int *devid_out /* out */)
{
	struct __actvret_drvcall_devmgr_core_device_add drvcall_ret;
	struct hm_actv_buf devname;
	struct hm_actv_buf devnode_name;
	struct hm_actv_buf group;
	struct devmgr_device_data data = { 0 };
	int ret;

	if (!is_valid_device_name(name)) {
		ret = E_HM_INVAL;
	} else {
		hm_actv_buf_init(&devname, name, strlen(name) + 1U);
		hm_actv_buf_init(&devnode_name, name, strlen(name) + 1U);
		hm_actv_buf_init(&group, NULL, 0);
		data.umode = umode;

		ret = actvcapcall_drvcall_devmgr_core_device_add(
					libdh_get_devmgr_ap(),
					devname, devnode_name,
					0, 0, devt, (int)MODE_CHR,
					group, &data, sizeof(data),
					&drvcall_ret);
		if ((ret == E_HM_OK) && (devid_out != NULL)) {
			*devid_out = drvcall_ret.devid;
		}
	}

	return ret;
}

int libdh_chrdev_create(const char *name, unsigned int devt,
			int *devid_out /* out */)
{
	return __chrdev_create(name, devt, S_IRUSR | S_IWUSR, devid_out);
}

int libdh_chrdev_create_simple_ex(const char *name, unsigned int major, unsigned int umode,
				  int *devid_out /* out */,
				  unsigned int *devt_out /* out */)
{
	unsigned int major_out;
	unsigned int devt;
	int devid;
	int ret;

	/* register single chrdev region */
	ret = libdh_register_chrdev_region(major, 0, 1, name, &major_out);
	if (ret < 0) {
		goto err_out;
	}
	devt = MKDEV(major_out, __U(0));

	/* create device */
	ret = __chrdev_create(name, devt, umode, &devid);
	if (ret < 0) {
		libdh_unregister_chrdev_region(major_out, 0, 1);
		goto err_out;
	}

	if (devid_out != NULL) {
		*devid_out = devid;
	}

	if (devt_out != NULL) {
		*devt_out = devt;
	}
err_out:
	return ret;
}

int libdh_chrdev_create_simple(const char *name, unsigned int major,
			       int *devid_out /* out */,
			       unsigned int *devt_out /* out */)
{
	return libdh_chrdev_create_simple_ex(name, major, S_IRUSR | S_IWUSR, devid_out, devt_out);
}
