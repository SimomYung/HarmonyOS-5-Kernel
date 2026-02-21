/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 20 17:49:32 2019
 */

#include <stdlib.h>

#include <libtest/test.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include <udk_tests.h>
#include <udk_test_helper.h>

#include <signal.h>

extern const char *current_test_module;
extern const char *current_test_case;
static char *module_name = NULL;
static char *test_name = NULL;
static struct test_result result = {0};

static void usage(void)
{
	char *elf_name = "udk-test-base.elf";
	printf("\nUsage:\n");
	printf("%s [COMMAND]... [OPTION]...\n", elf_name);
	printf("%s [{help | test | list}] [{-v | -s | -t}]\n", elf_name);
	printf("   help  :  print this message\n");
	printf("   test  :  test specified testcases\n");
	printf("   list  :  list tests info\n");
	printf("   -v    :    for test command, enable debug log\n");
	printf("   -s    :    test or list testcases for specified subsystem\n");
	printf("   -t    :    for test command, run specified testcase\n");
	printf("   -x    :    test func/perf/stab testcase\n");
	printf("Eg:\n");
	printf("./%s                    : run all testcases\n", elf_name);
	printf("./%s test -v            : run all testcases and enable debug mode\n", elf_name);
	printf("./%s list               : list all subsystems\n", elf_name);
	printf("./%s list -s tcb        : list all tcb testcases\n", elf_name);
	printf("./%s test -s tcb        : run all tcb testcases\n", elf_name);
	printf("./%s test -s tcb -t xxx : run single xxx test of tcb module\n", elf_name);
	printf("./%s test -x func       : run all function test\n", elf_name);
}

static void exception_handler(int sig)
{
	UNUSED(sig);

        test_dump_result(module_name, test_name, tests, &result);

        printf("Terminated: udk-base-test may hang or timeout for some reason\n");
        printf("Last test module: %s\n", current_test_module);
        printf("Last test case  : %s\n", current_test_case);
        printf("Done: Finished all udk-base-test\n");
        exit(0);
}

int main(int argc, char *argv[])
{
	int ret;
	struct sigaction act;

	char *test_type_name = NULL;
	int test_type = ALL_TEST;

	__cpuset_t cpus = __cpuset_full;

	hmlog_setoutput(HMLOG_OUTPUT_CONSOLE);
	hm_info("\nStart UDK test\n");

	mem_zero_s(result);

	ret = set_process_affinity(cpus);
	if (ret) {
		return -1;
	}

	int opt;
	while (optind < argc) {
		opt = getopt(argc, argv, "vs:t:x:");
		switch (opt) {
		case 'v':
			hmlog_setlevel(HMLOG_DEBUG);
			break;
		case 's':
			module_name = strdup(optarg);
			break;
		case 't':
			test_name = strdup(optarg);
			break;
		case 'x':
			test_type_name = strdup(optarg);
			break;
		default:
			optind++;
			break;
		}
	}
	act.sa_handler = exception_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);

	if (test_type_name != NULL) {
		if (!strcmp(test_type_name, "func")) {
			test_type = FUNC_TEST;
		} else if (!strcmp(test_type_name, "perf")) {
			test_type = PERF_TEST;
		} else if (!strcmp(test_type_name, "stab")) {
			test_type = STAB_TEST;
		} else {
			usage();
			return ret;
		}
	}
	if (argc == 1) {
		printf("\nStart udk-base-test.\n");
		ret = test_run_alltest(tests, test_type, &result);
	} else if (!strcmp(argv[1], "help")) {
		usage();
		return ret;
	} else if (!strcmp(argv[1], "list")) {
		if (module_name == NULL) {
			test_list_modules(tests);
			return ret;
		} else {
			test_list_tests(module_name, tests);
			return ret;
		}
	} else if (!strcmp(argv[1], "test")) {
		if (module_name == NULL && test_name == NULL) {
			ret = test_run_alltest(tests, test_type, &result);
		} else if (module_name != NULL && test_name == NULL) {
			ret = test_run_module(module_name, tests, test_type, &result);
		} else if (module_name != NULL && test_name != NULL) {
			ret = test_run_one(module_name, test_name, tests, &result);
		} else {
			usage();
			return ret;
		}
	} else {
		usage();
		return ret;
	}

	test_dump_result(module_name, test_name, tests, &result);
	printf("Done: Finished all udk-base-test.\n");

	return ret;
}
