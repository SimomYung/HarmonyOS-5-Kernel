/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 18 18:48:37 2019
 */
#include "udktest_net.h"
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmucap/ucap.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_io/io.h>
#include <libhmsrv_io/mgmt.h>
#include <libhwsecurec/securec.h>

static int register_devmgr(void)
{
	int rc;

	/* acquire devmgr ap */
	rc = hmsrv_io_init();
	if (rc != E_HM_OK) {
		hm_error("acquire devmgr ap failed: %s\n", hmstrerror(rc));
	}

	return rc;
}

static int test_load_netdrv(void)
{
	int ret = 0;

	ret = hmsrv_io_mgmt_load_module(DM_MGMT_ROOT_TARGET, UDK_TEST_NET_MODNAME, NULL);
	if (ret != E_HM_OK) {
		hm_error("load module %s to devid %d failed: %s\n",
			 UDK_TEST_NET_MODNAME, DEFAULT_DEVID, hmstrerror(ret));
		return ret;
	}

	return 0;
}

int setup_test_process_net(void)
{
	int ret;

	ret = register_devmgr();
	ASSERT_ZERO(ret);

	ret = test_load_netdrv();
	ASSERT_ZERO(ret);

	return 0;
}
