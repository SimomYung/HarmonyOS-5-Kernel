/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Memory query interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 27 16:20:09 2019
 */
#ifndef SYSMGR_MEM_MEMQUERY_H
#define SYSMGR_MEM_MEMQUERY_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <libbunch/bunch_ipc.h>
#include <intapi/mem/memstat.h>
#include <intapi/process/process.h>

enum {
	MEMQUERY_VSPACE_LAYOUT,
	MEMQUERY_VSPACE_VREGION,
};

int memquery_entrance(unsigned int flags, struct bunch_ipc_attr *attr);
void memquery_process(unsigned int flags, struct process_s *process,
		      struct bunch *bunch);
void memquery_process_nolock(unsigned int flags, struct process_s *process,
			     struct bunch *bunch);

#endif
