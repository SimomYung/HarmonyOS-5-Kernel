// SPDX-License-Identifier: GPL-2.0
#include <libhmsrv_sys/hm_security.h>
#include "hmxpm.h"

int xpm_mode_read(void *buf, size_t len)
{
	return hm_xpm_mode_read(buf, len);
}

int xpm_mode_write(const void *buf, size_t len)
{
	return hm_xpm_mode_write(buf, len);
}
