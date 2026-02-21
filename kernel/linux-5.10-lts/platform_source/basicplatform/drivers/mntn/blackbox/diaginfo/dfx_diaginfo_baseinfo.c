/*
 * dfx_diaginfo_baseinfo.c
 *
 * dfx diaginfo basic information.
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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
#include <platform_include/basicplatform/linux/dfx_bootup_keypoint.h>
#include <platform_include/basicplatform/linux/dfx_diaginfo_baseinfo.h>
#include <platform_include/basicplatform/linux/dfx_hw_diag.h>
#include <platform_include/cee/linux/rdr_lpm3.h>
#ifdef CONFIG_THERMAL_MNTN_DMD
#include <platform_include/cee/linux/thermal_dmd.h>
#endif
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <platform_include/cee/linux/rdr_get_cpu_rt_freq_volt.h>
#include <linux/vmalloc.h>
#include "bbox_diaginfo.h"
#include "securec.h"
#include "rdr_inner.h"
#include <platform_include/cee/linux/sr_dmd.h>
#include <platform_include/cee/linux/hisi_ddr.h>
#ifdef CONFIG_DFX_SEC_QIC_V300
#include <m3_rdr_ddr_map.h>
#include "rdr_field.h"
#endif

static char g_base_info_str[MAX_DIAGINFO_LEN];
static void *rdr_backup_memory;
static size_t rdr_memory_size;

#ifdef CONFIG_DFX_SEC_QIC_V300
struct cfgbus_timeout_info g_cfgbus_info;
#endif

static bool diaginfo_need_save_baseinfo(void)
{
	u32 reboot_type;
	
	reboot_type = rdr_get_reboot_type();
	if (reboot_type > REBOOT_REASON_LABEL1 && reboot_type < REBOOT_REASON_LABEL3)
		return true;
	return false;
}

int diaginfo_backup_rdr_memory(void *rdr_memory, size_t size)
{
	int ret;

	if (rdr_backup_memory != NULL) {
		pr_err("%s, rdr backup memory already exist\n", __func__);
		return 0;
	}

	if (!diaginfo_need_save_baseinfo()) {
		pr_info("diaginfo don't need to backup rdr memory\n");
		return 0;
	}

	rdr_memory_size = rdr_reserved_phymem_size();
	if (rdr_memory_size == 0) {
		pr_err("%s get rdr reserved memory size failed\n", __func__);
		return -1;
	}

	if (rdr_memory == NULL || size != rdr_memory_size) {
		pr_err("%s invalid param\n", __func__);
		return -1;
	}

	rdr_backup_memory = vmalloc(rdr_memory_size);
	if (rdr_backup_memory == NULL) {
		pr_err("%s malloc memory to backup rdr memory failed\n", __func__);
		return -1;
	}
	ret = memcpy_s(rdr_backup_memory, rdr_memory_size, rdr_memory, size);
	if (ret != EOK) {
		pr_err("%s backup rdr reserved memory failed, ret %d\n", __func__, ret);
		vfree(rdr_backup_memory);
		rdr_backup_memory = NULL;
		rdr_memory_size = 0;
	}

	return 0;
}

static void base_info_free_rdr_memory(void)
{
	if (rdr_backup_memory == NULL)
		return;
	vfree(rdr_backup_memory);
	rdr_backup_memory = NULL;
	rdr_memory_size = 0;
}

#ifdef CONFIG_DFX_SEC_QIC_V300
#define QIC_STATUS_STR_MAX_LEN	16
#define QIC_VERSION_V300	0x12C
#define CFG_BUS_TIMEOUT_INFO_LEN	12
#define CFG_BUS_TIMEOUT_INFO_STR_MAX_LEN	100
static int get_qic_irq_status_from_rdr(const void *rdr_memory, size_t size, char *out_buf, size_t buf_size)
{
	u64 rdr_phy_addr;
	u64 rdr_size;
	u64 qic_dump_offset;
	u32 qic_version;
	u32 qic_status;
	struct rdr_struct_s *rdr_head = NULL;
	char qic_stat_str[QIC_STATUS_STR_MAX_LEN] = {0};
	int ret;
	size_t len;

	if (rdr_memory == NULL || out_buf == NULL) {
		pr_err("get qic irq status from rdr memory failed, memory is NULL\n");
		return -1;
	}

	rdr_phy_addr = rdr_reserved_phymem_addr();
	rdr_size = rdr_reserved_phymem_size();
	if (size != rdr_size) {
		pr_err("%s, invalid rdr memory size %lu\n", __func__, size);
		return -1;
	}

	rdr_head = (struct rdr_struct_s *)rdr_memory;
	if (rdr_head->area_info[RDR_AREA_LPM3].offset <= rdr_phy_addr ||
		rdr_head->area_info[RDR_AREA_LPM3].length == 0 ||
		(rdr_head->area_info[RDR_AREA_LPM3].offset + rdr_head->area_info[RDR_AREA_LPM3].length >= \
		rdr_phy_addr + rdr_size)) {
		pr_err("%s, check LPM3 memory failed, offset 0x%llx, size 0x%x\n",
			__func__,
			rdr_head->area_info[RDR_AREA_LPM3].offset,
			rdr_head->area_info[RDR_AREA_LPM3].length);
		return -1;
	}

	qic_dump_offset = rdr_head->area_info[RDR_AREA_LPM3].offset - rdr_phy_addr + M3_RDR_SYS_CONTEXT_NOC_REG_OFFSET;
	qic_version = *(u32 *)((char *)rdr_memory + qic_dump_offset);
	qic_status = *(u32 *)((char *)rdr_memory + qic_dump_offset + sizeof(qic_version));
	if (qic_version != QIC_VERSION_V300) {
		pr_err("%s check qic version failed, ver 0x%x\n", __func__, qic_version);
		return -1;
	}
	ret = sprintf_s(qic_stat_str, QIC_STATUS_STR_MAX_LEN, "0x%x", qic_status);
	if (ret <= 0) {
		pr_err("get qic status from last rdr memory failed, sprintf error\n");
		return -1;
	}
	len = strnlen(qic_stat_str, QIC_STATUS_STR_MAX_LEN);
	if (buf_size < len + 1) {
		pr_err("get qic status from last rdr memory failed, buf_size %lu is too small\n", buf_size);
		return -1;
	}
	ret = strncpy_s(out_buf, buf_size, qic_stat_str, len);
	if (ret != EOK) {
		pr_err("get qic status from last rdr memory failed, copy string return %d\n", ret);
		return -1;
	}

	return 0;
}

static int save_qic_cfgbus_timeout_info(const void *rdr_memory, size_t size)
{
	int ret;
	u64 rdr_phy_addr;
	u64 rdr_size;
	u64 cfgbus_dump_offset;
	struct rdr_struct_s *rdr_head = NULL;

	if (rdr_memory == NULL) {
		pr_err("get qic cfg bus info from rdr memory failed, memory is NULL\n");
		return -1;
	}

	rdr_phy_addr = rdr_reserved_phymem_addr();
	rdr_size = rdr_reserved_phymem_size();
	if (size != rdr_size) {
		pr_err("%s, invalid rdr memory size %lu\n", __func__, size);
		return -1;
	}
	rdr_head = (struct rdr_struct_s *)rdr_memory;
	if (rdr_head->area_info[RDR_AREA_LPM3].offset <= rdr_phy_addr ||
	    rdr_head->area_info[RDR_AREA_LPM3].length == 0 ||
	    (rdr_head->area_info[RDR_AREA_LPM3].offset + rdr_head->area_info[RDR_AREA_LPM3].length >= \
	    rdr_phy_addr + rdr_size)) {
		pr_err("%s, check LPM3 memory failed, offset 0x%llx, size 0x%x\n",
			__func__,
			rdr_head->area_info[RDR_AREA_LPM3].offset,
			rdr_head->area_info[RDR_AREA_LPM3].length);
		return -1;
	}
	cfgbus_dump_offset = rdr_head->area_info[RDR_AREA_LPM3].offset - rdr_phy_addr + M3_RDR_DDR_MNTN_OFFSET;

	ret = memcpy_s((void *)(uintptr_t)&g_cfgbus_info, CFG_BUS_TIMEOUT_INFO_LEN,
			(void *)(uintptr_t)(rdr_memory + cfgbus_dump_offset), CFG_BUS_TIMEOUT_INFO_LEN);
	if (ret != 0) {
		pr_err("%s get qic cfg bus info fail\n", __func__);
		return -1;
	}

	return 0;
}

int get_qic_cfgbus_timeout_info(char *out_buf, size_t buf_size)
{
	int ret;
	char cfgbus_timeout_str[CFG_BUS_TIMEOUT_INFO_STR_MAX_LEN] = {0};
	size_t len;

	if (out_buf == NULL) {
		pr_err("get qic cfg bus info failed, memory is NULL\n");
		return -1;
	}

	ret = sprintf_s(cfgbus_timeout_str, CFG_BUS_TIMEOUT_INFO_STR_MAX_LEN,
				"window:0x%lx,addr:0x%lx,reason:0x%lx",
				g_cfgbus_info.lpm_sliding_window_addr, g_cfgbus_info.access_addr_from_lpm,
				g_cfgbus_info.cfgbus_timeout_reason);
	if (ret < 0) {
		pr_err("%s():%d snprintf_s ERROR\n", __func__, __LINE__);
		return -1;
	}

	len = strlen(cfgbus_timeout_str);
	if (buf_size < len + 1) {
		pr_err("%s, out_buf size %lu is too small for string len %lu\n", __func__, buf_size, len);
		return -1;
	}
	if (strcpy_s(out_buf, buf_size, cfgbus_timeout_str) != EOK) {
		pr_err("%s, strcpy failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}
#endif

/*
 * callback function type: int (*base_info_get_fn)(const void *rdr_memory, size_t rdr_size, char *buf, size_t len)
 * @param rdr_memory [int] : last rdr backup memory, data from this memory is copied from last rdr reserved memory.
 * @param rdr_size [int]: size of rdr_memory
 * @param buf
 */
struct base_info_item g_base_info_items[] = {
	{"Keypoint", get_last_boot_keypoint_str},
	{"Sr-position", get_sr_info_for_dmd},
	{"Acpu-freq", get_cpu_freq_info_for_diag},
	{"Acpu-vol", get_cpu_volt_info_for_diag},
#ifdef CONFIG_THERMAL_MNTN_DMD
	{"TMP", parse_tempinfo_from_rdr},
#endif
	{"Ddr-freq", get_ddr_freq_info_str},
	{"Ddr-vol", get_ddr_volt_info_str},
	{"Qice-freq", get_qice_freq_info_str},
	{"Peri-vol", parse_peri_info_from_rdr},
	{"Media-vote", ip_regulator_mntn_parse},
	{"Qice-status", get_qice_status_str},
#ifdef CONFIG_DFX_SEC_QIC_V300
	{"Qic-status", get_qic_irq_status_from_rdr},
#endif
	{"RT-flag", NULL},
};

static int dfx_diaginfo_base_info_add(const char *info, size_t len)
{
	size_t cur_len;
	errno_t ret;

	if (info == NULL || len == 0)
		return -1;

	cur_len = strnlen(g_base_info_str, MAX_DIAGINFO_LEN);
	if (MAX_DIAGINFO_LEN - cur_len <= len) {
		pr_err("add bbox diaginfo base info failed, cur_len:%lu (MAX_DIAGINFO_LEN - cur_len):%lu len:%lu is too long\n", cur_len, (MAX_DIAGINFO_LEN - cur_len), len);
		return -1;
	}
	ret = strncat_s(g_base_info_str, MAX_DIAGINFO_LEN, info, len);
	if (ret != EOK) {
		pr_err("add bbox diaginfo base info failed, cat string return %d\n", ret);
		return -1;
	}

	return 0;
}

int dfx_diaginfo_collect_base_info(void)
{
	int index, ret;
	char buf[BASE_INFO_SINGLE_MAX_LEN] = {0};
	size_t len;

	if (!diaginfo_need_save_baseinfo()) {
		pr_info("no need to collect diaginfo baseinfo\n");
		return 0;
	}

	if (rdr_backup_memory == NULL || rdr_memory_size == 0) {
		pr_err("%s rdr backup memory is null\n", __func__);
		return -1;
	}

	for (index = 0; index < sizeof(g_base_info_items) / sizeof(g_base_info_items[0]); index++) {
		if (g_base_info_items[index].keyword == NULL || g_base_info_items[index].base_info_get_fn == NULL)
			continue;
		pr_info("deal item with keyword %s\n", g_base_info_items[index].keyword);
		(void)memset_s(buf, BASE_INFO_SINGLE_MAX_LEN, 0, BASE_INFO_SINGLE_MAX_LEN);
		ret = sprintf_s(buf, BASE_INFO_SINGLE_MAX_LEN, "%s:", g_base_info_items[index].keyword);
		if (ret < 0) {
			pr_err("%s():%d snprintf_s ERROR\n", __func__, __LINE__);
			return -1;
		}
		len = strnlen(buf, BASE_INFO_SINGLE_MAX_LEN);
		if (g_base_info_items[index].base_info_get_fn(rdr_backup_memory, rdr_memory_size, \
			buf + len, BASE_INFO_SINGLE_MAX_LEN - len) < 0)
			continue;
		(void)strncat_s(buf, BASE_INFO_SINGLE_MAX_LEN, " ", strlen(" "));
		len = strnlen(buf, BASE_INFO_SINGLE_MAX_LEN);
		if (dfx_diaginfo_base_info_add(buf, len) < 0) {
			pr_err("bbox diaginfo base info add failed\n");
			return -1;
		}
		pr_info("base info: %s\n", buf);
	}
#ifdef CONFIG_DFX_SEC_QIC_V300
	save_qic_cfgbus_timeout_info(rdr_backup_memory, rdr_memory_size);
#endif
	base_info_free_rdr_memory();

	return 0;
}

int dfx_diaginfo_get_base_info(char *buf, size_t size)
{
	size_t len;
	errno_t ret;

	if (buf == NULL || size == 0)
		return -1;

	len = strnlen(g_base_info_str, MAX_DIAGINFO_LEN);
	if (size <= len) {
		pr_err("bbox diaginfo get base info failed, buffer size %lu is too small\n", size);
		return -1;
	}
	ret = strncpy_s(buf, size, g_base_info_str, len);
	if (ret != EOK) {
		pr_err("bbox diaginfo get base info failed, copy string return %u\n", ret);
		return -1;
	}

	return 0;
}