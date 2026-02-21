// SPDX-License-Identifier: GPL-2.0
#include <hwbootfail/chipsets/common/bootfail_common.h>
#include <linux/kernel.h>
#include <liblinux/pal.h>

#define SIG_TO_INIT 40

void __override dump_init_log(void)
{
	liblinux_pal_process_signal_with_pid(1, SIG_TO_INIT);
	print_info("Init dump: sending signal to init\n");
}
