// SPDX-License-Identifier: GPL-2.0
#include <hmkernel/types.h>
#include <hongmeng/errno.h>
#include <libsysevent/sysevent.h>

int liblinux_pal_sysevent_write(const char *buf, size_t buf_len)
{
	return hm_sysevent_report_raw_data(buf, buf_len);
}
