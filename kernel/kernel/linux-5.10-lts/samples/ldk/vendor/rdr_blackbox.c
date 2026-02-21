// SPDX-License-Identifier: GPL-2.0
#include "blackbox/rdr_inner.h"
#include <liblinux/pal.h>
#include <linux/printk.h>
#include <platform_include/basicplatform/linux/dfx_pstore.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>


int __override save_logbuff_memory(const char *dst, const char *src)
{
	/* kernel logbuf is not reserved and will not be saved in liblinux */
	(void)dst;
	(void)src;

	pr_info("kernel logbuff will not be saved\n");

	return 0;
}

void rdr_ap_register_logbuf_ringbuf_dump(void);
void __override rdr_ap_register_logbuf_ringbuf_dump(void)
{
	pr_info("use override function to disable ringbuf dump\n");
}

#ifdef CONFIG_DFX_PSTORE
void __override registe_info_to_mntndump(char *big_oops_buf, size_t big_oops_buf_sz)
{
	(void)big_oops_buf;
	(void)big_oops_buf_sz;
	pr_info("use override function to disable pstore dump\n");
}
#endif

void __override dfx_ap_reset(void)
{
	pr_info("sysmgr will take over, do coredump and snapshot\n");
	liblinux_reboot("no_reason", true);
	return;
}

#ifdef CONFIG_DFX_TIME
int __override logbuf_print_all(char *buf, int size)
{
	int ret = -1;
	size_t ret_len = 0;
	bool err = liblinux_get_hm_log_buffer(buf, size, &ret_len);
	if (!err) {
		pr_err("Failed to dump klog. size:%d, ret_len:%zu\n", size, ret_len);
	} else {
		pr_info("Klog dump success. size:%d, ret_len:%zu\n", size, ret_len);
		ret = (int)ret_len;
	}

	return ret;
}
#endif
