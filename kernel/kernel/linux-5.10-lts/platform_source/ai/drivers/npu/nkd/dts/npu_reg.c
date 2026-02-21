/*
 * npu_reg.c
 *
 * about npu reg
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "npu_reg.h"

#include <linux/io.h>
#include <securec.h>
#include "npu_log.h"

#define NPU_DUMP_REGION_NAME     "dump_region"

static u32 *npu_plat_get_reg_vaddr_offset(u32 reg_idx, u32 offset)
{
	struct npu_platform_info *plat_info = NULL;

	if (reg_idx >= NPU_REG_MAX_RESOURCE) {
		npu_drv_err("reg_idx = %x is error\n", reg_idx);
		return NULL;
	}

	plat_info = npu_plat_get_info();
	if (plat_info == NULL) {
		npu_drv_err("get plat_info failed\n");
		return NULL;
	}
	return (u32 *)((u8 *)plat_info->dts_info.reg_vaddr[reg_idx] + offset);
}

int npu_plat_map_reg(struct platform_device *pdev,
	struct npu_platform_info *plat_info)
{
	int i;
	void __iomem *base = NULL;
	struct npu_mem_desc *mem_desc = NULL;
	int max_regs = NPU_REG_MAX_RESOURCE;

	if (plat_info->dts_info.feature_switch[NPU_FEATURE_HWTS] == 0)
		max_regs = NPU_REG_POWER_STATUS_BASE + 1;
	for (i = 0; i < max_regs; i++) {
		mem_desc = &plat_info->dts_info.reg_desc[i];
		npu_drv_debug("resource: base %pK\n",
			(void *)(uintptr_t)(u64)mem_desc->base);
		npu_drv_debug("resource: mem_desc.len %llx\n", mem_desc->len);

		if (mem_desc->base == 0 || mem_desc->len == 0) {
			plat_info->dts_info.reg_vaddr[i] = NULL;
			continue;
		}

		base = devm_ioremap(&pdev->dev, mem_desc->base, mem_desc->len);
		if (base == NULL) {
			npu_drv_err("platform_get_resource failed i = %d\n", i);
			goto map_platform_reg_fail;
		}
		plat_info->dts_info.reg_vaddr[i] = base;
	}

	return 0;
map_platform_reg_fail:
	(void)memset_s(plat_info->dts_info.reg_desc,
		sizeof(plat_info->dts_info.reg_desc),
		0, sizeof(plat_info->dts_info.reg_desc));
	(void)npu_plat_unmap_reg(pdev, plat_info);
	return -1;
}

int npu_plat_unmap_reg(struct platform_device *pdev,
	struct npu_platform_info *plat_info)
{
	int i;
	int max_regs = NPU_REG_MAX_RESOURCE;

	if (plat_info->dts_info.feature_switch[NPU_FEATURE_HWTS] == 0)
		max_regs = NPU_REG_POWER_STATUS_BASE + 1;

	for (i = 0; i < max_regs; i++) {
		if (plat_info->dts_info.reg_vaddr[i] == NULL)
			continue;

		devm_iounmap(&pdev->dev, plat_info->dts_info.reg_vaddr[i]);
	}
	return 0;
}

int npu_plat_parse_dump_region_desc(const struct device_node *root,
	struct npu_platform_info *plat_info)
{
	int ret;
	struct npu_mem_desc *mem_desc = &plat_info->dts_info.dump_region_desc[0];

	if (mem_desc == NULL) {
		npu_drv_err("dump_region desc is NULL");
		return -1;
	}
	ret = of_property_read_u64_array(root, NPU_DUMP_REGION_NAME,
		(u64 *)mem_desc,
		NPU_DUMP_REGION_MAX * (sizeof(struct npu_mem_desc) / sizeof(u64)));
	if (ret < 0) {
		npu_drv_warn("dump_regs base is not set, dump_regs is disabled\n");
		return ret;
	}
	npu_drv_debug("dump region: base=0x%pK len=0x%llx", mem_desc->base,
		mem_desc->len);
	return 0;
}
#ifdef PLATFORM_GET_RESMEM_FROM_DTS
int npu_plat_parse_resmem_desc(struct platform_device *pdev,
	struct npu_platform_info *plat_info)
{
	int ret;
	struct npu_mem_desc *remem_desc = NULL;

	uint64_t remem_buffer_base = 0;
	uint64_t remem_buffer_size = 0;
	struct device_node *np = NULL;

	npu_drv_warn("start parse ");
	unused(pdev);
	np = of_find_node_by_path("/reserved-memory/npu_ai_server");
	if (!np) {
		npu_drv_warn("NOT FOUND dts path: %s!\n", "/reserved-memory/npu_ai_server");
	}
	ret = of_property_read_u64_index(np, "reg", 0, &remem_buffer_base);
	if (ret != 0) {
		npu_drv_warn("failed to get remem_buffer_base resource! ret=%d\n", ret);
		remem_buffer_base = 0;
	}
	ret = of_property_read_u64_index(np, "reg", 1, &remem_buffer_size);
	if (ret != 0) {
		npu_drv_warn("failed to get remem_buffer_base resource! ret=%d\n", ret);
		remem_buffer_size = 0;
	}

	remem_desc = &plat_info->resmem_info.resmem_desc[NPU_CTRL_INFO_IDX];
	remem_desc->base = remem_buffer_base;
	remem_desc->len = remem_buffer_size;
	return ret;
}

#endif

int npu_plat_parse_ipcbuffer_resmem_desc(uint64_t *ipcbuffer_base_addr, uint32_t *ipcbuffer_size)
{
#ifdef PLATFORM_GET_RESMEM_FROM_DTS
	int ret = -1;
	uint64_t remem_buffer_base = 0;
	struct device_node *np = NULL;
	if (ipcbuffer_base_addr == NULL || ipcbuffer_size == NULL) {
		npu_drv_err("input param is NULL!\n");
		return -1;
	}
 
	np = of_find_node_by_path("/reserved-memory/npu_ai_server");
	if (!np) {
		npu_drv_err("NOT FOUND dts path: %s!\n", "/reserved-memory/npu_ai_server");
		return -1;
	}
 
	ret = of_property_read_u64_index(np, "reg", 0, &remem_buffer_base);
	if (ret != 0) {
		npu_drv_err("failed to get remem_buffer_base resource! ret=%d\n", ret);
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

EXPORT_SYMBOL(npu_plat_parse_ipcbuffer_resmem_desc);
int npu_plat_parse_reg_desc(struct platform_device *pdev,
	struct npu_platform_info *plat_info)
{
	unsigned int i;
	struct resource *info = NULL;
	struct npu_mem_desc *mem_desc = NULL;
	unsigned int max_regs = NPU_REG_MAX_RESOURCE;

	if (plat_info->dts_info.feature_switch[NPU_FEATURE_HWTS] == 0)
		max_regs = NPU_REG_POWER_STATUS_BASE + 1;
	for (i = 0; i < max_regs; i++) {
#ifndef CONFIG_NPU_SWTS
#ifdef  CONFIG_NPU_SINK_UDK
		if (i >= NPU_REG_HWTS_BASE && i <= NPU_REG_AIC1_BASE)
			continue;
#endif
#endif

#if defined(CONFIG_NPU_SINK_UDK) && defined(CONFIG_NPU_SWTS)
		if (i >= NPU_REG_AIC0_BASE && i <= NPU_REG_AIC1_BASE)
			continue;
#endif
		info = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (info == NULL || info->end <= info->start) {
			npu_drv_err("info is invalid, i = %u\n", i);
			return -1;
		}
		mem_desc = &plat_info->dts_info.reg_desc[i];
		mem_desc->base = info->start;
		mem_desc->len = info->end - info->start + 1;
	}

	return 0;
}

u32 npu_plat_get_npu_power_status(void)
{
	u32 *addr = npu_plat_get_reg_vaddr_offset(NPU_REG_POWER_STATUS_BASE, 0);

	if (addr != NULL) {
		return (u32)readl(addr);
	} else {
		npu_drv_warn("invalid power status addr\n");
		return 0;
	}
}

void npu_plat_set_npu_power_status(u32 status)
{
	u32 *addr = npu_plat_get_reg_vaddr_offset(NPU_REG_POWER_STATUS_BASE, 0);

	if (addr != NULL)
		writel(status, addr);
	else
		npu_drv_warn("invalid power status addr\n");
}
EXPORT_SYMBOL(npu_plat_set_npu_power_status);
