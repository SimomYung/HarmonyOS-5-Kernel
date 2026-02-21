// SPDX-License-Identifier: GPL-2.0
#include <stdint.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <liblinux/pal.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libmem/utils.h>
#include <libbunch/bunch_ipc.h>
#include <libsysif/sysmgr/api.h>

/*
 * 16 contexts, each context metadata string need about 1466~1471 bytes,
 * size need 4*5 bytes, bunch_write_string also write size(uint32_t)
 */
#define ELOG_METADATA_BUNCH_SIZE	PAGE_ALIGN_UP(PAGE_ALIGN_UP((1471 + 4 * 5 + 4 * 5) * 16) + 1)

static int get_metadata_ipc(struct bunch_ipc_attr *attr, void *data)
{
	(void)data;
	return actvcall_hmcall_elog_metadata(*attr);
}

int liblinux_get_elog_metadata(int (*callback)(const char* buf, size_t buf_size))
{
	int err;
	char *buf;
	uint32_t size;
	struct bunch *bunch = NULL;
	struct bunch_ipc_reader_ctx ctx = {0};

	bunch = bunch_ipc_create(ELOG_METADATA_BUNCH_SIZE);
	if (bunch == NULL) {
		hm_error("elog: bunch_ipc_create failed\n");
		return -1;
	}

	ctx.ipc = get_metadata_ipc;
	ctx.private_data = NULL;
	ctx.target_info = (uint64_t)hmsrv_sysmgr_cnode_idx();
	err = bunch_ipc_request(bunch, &ctx);
	if (err < 0) {
		hm_error("bunch ipc try request data with shm failed: %s\n",
			hmstrerror(err));
		bunch_ipc_destroy(bunch);
		return err;
	}

	size = bunch_read_uint32(bunch);
	while (size != 0) {
		buf = bunch_read_string(bunch);
		err = callback(buf, size);
		/* next size */
		size = bunch_read_uint32(bunch);
	}

	bunch_ipc_destroy(bunch);

	return err;
}
