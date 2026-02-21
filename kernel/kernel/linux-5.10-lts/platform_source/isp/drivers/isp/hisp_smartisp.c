/*
 * isp smartisp driver
 *
 * Copyright (c) 2024- ISP Technologies CO., Ltd.
 */

#include <linux/of.h>
#include "npu_ddr_map.h"
#include "hisp_internel.h"
#define DTS_PATH_ISP_NPU_BUFFER "/reserved-memory/npu_ai_server"

int hisp_parse_ipcbuffer_resmem_desc(uint64_t *ipcbuffer_base_addr, uint32_t *ipcbuffer_size)
{
#ifdef HISP_SMARTISP_ENABLE
	int ret = -1;
	uint64_t remem_buffer_base = 0;
	struct device_node *np = NULL;
	if (ipcbuffer_base_addr == NULL || ipcbuffer_size == NULL) {
		pr_err("input param is NULL!\n");
		return -1;
	}

	np = of_find_node_by_path(DTS_PATH_ISP_NPU_BUFFER);
	if (!np) {
		pr_err("NOT FOUND dts path: %s!\n", DTS_PATH_ISP_NPU_BUFFER);
		return -1;
	}

	ret = of_property_read_u64_index(np, "reg", 0, &remem_buffer_base);
	if (ret != 0) {
		pr_err("failed to get remem_buffer_base resource! ret=%d\n", ret);
		return ret;
	}

	*ipcbuffer_base_addr = remem_buffer_base + NPU_NS_ISPNN_IPC_BUFF_BASE_OFFSET;
	*ipcbuffer_size = NPU_NS_ISPNN_IPC_BUFF_SIZE;
	return ret;
#else
	(void)ipcbuffer_base_addr;
	(void)ipcbuffer_size;
	return -1;
#endif
}
