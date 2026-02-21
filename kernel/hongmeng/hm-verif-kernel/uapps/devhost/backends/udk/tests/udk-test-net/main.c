/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 18 18:48:37 2019
 */
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>

#include <libtest/test.h>
#include "udktest_net.h"

int main(int argc, char **argv)
{
	int ret = 0;
	struct test_result result = {0};
	__cpuset_t cpus = __cpuset_full;

	hm_info("\nStart Net-test.\n");

	ret = set_process_affinity(cpus);
	if (ret) {
		return -1;
	}

	ret = setup_test_process_net();
	if (ret != 0) {
		ret = -1;
		hm_error("\ninit_test_process failed.\n");
		goto test_out;
	}

	test_run_all(argc - 1, &argv[1], net_tests, &result);
	if (result.failed == 0) {
		hm_info("\nFinish Net-test, result: [total: %d, all test success].\n",
			result.total);
		ret = 0;
	} else {
		hm_error("\nFinish Net-test, result: [total: %d, failed: %d].\n", result.total,
			 result.failed);
		ret = -1;
	}

test_out:
	hm_info("\nEnd Net-test.\n");
	return ret;
}
