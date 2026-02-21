// SPDX-License-Identifier: GPL-2.0
#include <libhmsrv_sys/hm_mem.h>
#include "hmextfrag.h"

int extfrag_index_read(void *buf, size_t len)
{
	return hm_mem_extfrag_index_read(buf, len);
}