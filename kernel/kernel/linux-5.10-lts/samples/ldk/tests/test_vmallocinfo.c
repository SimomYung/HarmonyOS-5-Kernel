// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <securec.h>

#define TESTCASE_NUMBER 6

struct vmalloc_info_testcase {
	void *addr;
	void *(*testcase_vmalloc)(void);
};

#define DEFINE_VMALLOC_TESTCASE(case_number, used_nr_pages, unused_nr_pages)		\
	static noinline void *vmalloc_info_func##case_number(void) {					\
		void *addr = vmalloc(((used_nr_pages) + (unused_nr_pages)) * PAGE_SIZE);	\
		if (addr == NULL) {															\
			return NULL;															\
		}																			\
		if ((used_nr_pages) > 0) {													\
			(void)memset_s(addr, ((used_nr_pages) + (unused_nr_pages)) * PAGE_SIZE,	\
						   0, (used_nr_pages) * PAGE_SIZE);							\
		}																			\
		return addr;																\
	}

#define DEFINE_TESTCASE_ELEMENT(case_number) \
	[case_number] = { .testcase_vmalloc = vmalloc_info_func##case_number }

DEFINE_VMALLOC_TESTCASE(0, 0, 5);
DEFINE_VMALLOC_TESTCASE(1, 1, 4);
DEFINE_VMALLOC_TESTCASE(2, 2, 3);
DEFINE_VMALLOC_TESTCASE(3, 3, 2);
DEFINE_VMALLOC_TESTCASE(4, 4, 1);
DEFINE_VMALLOC_TESTCASE(5, 5, 0);

static struct vmalloc_info_testcase vmalloc_testcases[TESTCASE_NUMBER] = {
	DEFINE_TESTCASE_ELEMENT(0),
	DEFINE_TESTCASE_ELEMENT(1),
	DEFINE_TESTCASE_ELEMENT(2),
	DEFINE_TESTCASE_ELEMENT(3),
	DEFINE_TESTCASE_ELEMENT(4),
	DEFINE_TESTCASE_ELEMENT(5),
};

static int __init test_vmallocinfo_init(void)
{
	int i, j, ret;
	for (i = 0; i < TESTCASE_NUMBER; i++) {
		vmalloc_testcases[i].addr = vmalloc_testcases[i].testcase_vmalloc();
		if (vmalloc_testcases[i].addr == NULL) {
			ret = -ENOMEM;
			goto fail;
		}
	}
	return 0;
fail:
	for (j = 0; j < i; j++) {
		vfree(vmalloc_testcases[j].addr);
	}
	return ret;
}

static void __exit test_vmallocinfo_exit(void)
{
	int i;
	for (i = 0; i < TESTCASE_NUMBER; i++) {
		vfree(vmalloc_testcases[i].addr);
	}
}
module_init(test_vmallocinfo_init);
module_exit(test_vmallocinfo_exit);
