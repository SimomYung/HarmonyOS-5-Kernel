/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: UDK unload test
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 05 17:22:55 2020
 */
#include <libhmlog/hmlog.h>
#include <libhmsrv_io/io.h>
#include <libsysif/sysmgr/api.h>

#include <libtest/test.h>
#include "udktest_unload.h"

static int udk_test_unload_setup(void)
{
	int ret;

	ret = hmsrv_io_init();
	if (ret) {
		hm_error("acquire devmgr ap failed: %s\n", hmstrerror(ret));
	}

	return ret;
}

int main(int argc, char **argv)
{
	int ret = 0;
	struct test_result result;
	__cpuset_t cpus = __cpuset_full;

	hm_info("\nStart UDK unload test\n");

	mem_zero_s(result);

	ret = set_process_affinity(cpus);
	if (ret) {
		return -1;
	}

	ret = udk_test_unload_setup();
	if (ret < 0) {
		ret = -1;
		goto test_out;
	}

	test_run_all(argc, &argv[0], udk_unload_tests, &result);
	if (result.failed == 0) {
		hm_info("\nFinish udk-test-unload, "
			"result: [total: %d, all test success].\n",
			result.total);
		ret = 0;
	} else {
		hm_error("\nFinish udk-test-unload, "
			 "result: [total: %d, failed: %d].\n",
			 result.total, result.failed);
		ret = -1;
	}

test_out:
	hm_info("\nEnd UDK unload test.\n");
	return ret;
}
