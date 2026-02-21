/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: User interface of dump function
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 11 22:10:47 2020
 */
#include <hongmeng/errno.h>
#include <libbunch/bunch_ipc.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_dump.h>
#include <libsysif/sysmgr/api.h>
#include <hmsysmgr/dumpstack.h>
#include <hmsysmgr/process/pid.h>
#include <libhwsecurec/securec.h>

struct dump_thread_config {
	pid_t tid;
	unsigned int flags;
};

static int dump_thread_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct dump_thread_config *config = NULL;
	config = (struct dump_thread_config *)data;
	return actvcall_hmcall_dump_thread(config->tid, config->flags,
					   DUMP_OUTPUT_TYPE_BUNCH, *attr);
}

int hm_dump_thread_to_bunch(pid_t tid, unsigned int flags, struct bunch *bunch)
{
	int err = E_HM_OK;
	struct dump_thread_config config;
	struct bunch_ipc_reader_ctx ctx;
	uint32_t cnode_idx;

	if ((tid < PID_NONE) ||
	    (flags >= dumpstack_flag_mask_of((unsigned int)DUMPSTACK_FLAGS_MAX)) ||
	    (bunch == NULL)) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		mem_zero_s(config);
		config.tid = tid;
		config.flags = flags;
		cnode_idx = hmsrv_sysmgr_cnode_idx();

		ctx.ipc = dump_thread_ipc;
		ctx.private_data = ptr_to_void(&config);
		ctx.target_info = (uint64_t)cnode_idx;
		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_warn("bunch ipc try request data with shm failed: %s\n",
				hmstrerror(err));
		}
	}

	return err;
}

int hm_dump_thread_to_klog(pid_t tid, unsigned int flags)
{
	int err = E_HM_OK;
	struct bunch_ipc_attr attr;

	if (tid < PID_NONE) {
		err = E_HM_INVAL;
	} else if (flags >= dumpstack_flag_mask_of((unsigned int)DUMPSTACK_FLAGS_MAX)) {
		err = E_HM_INVAL;
	} else {
		mem_zero_s(attr);
		err = actvcall_hmcall_dump_thread(tid, flags, DUMP_OUTPUT_TYPE_KLOG, attr);
	}
	return err;
}

int hm_dump_addr2sym_by_cnode_idx(__u32 cnode_idx, unsigned long addr, struct symbol_s *symbol)
{
	int err;
	struct __actvret_hmcall_dump_addr2sym ret;

	mem_zero_s(ret);
	err = actvcall_hmcall_dump_addr2sym(cnode_idx, (const void *)(uintptr_t)addr, &ret);
	if (err == E_HM_OK && symbol != NULL) {
		mem_assign_s(*symbol, ret.symbol);
	}

	return err;
}
