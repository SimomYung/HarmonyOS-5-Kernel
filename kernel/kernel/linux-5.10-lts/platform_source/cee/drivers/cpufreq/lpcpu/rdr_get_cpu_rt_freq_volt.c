/*
 * rdr_get_cpu_rt_freq_volt.c
 *
 * rdr_get_cpu_rt_freq_volt module
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/compiler.h>
#include <linux/arm-smccc.h>
#include <asm/compiler.h>
#include <linux/proc_fs.h>
#include <cxpu_profile.h>
#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
#include <rdr_get_cpu_rt_freq_volt_xpu_params.h>
#else
#include <xpu_cpu_sram.h>
#endif
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/cee/linux/rdr_lpm3.h>
#include <cpu_cluster_def.h>
#include "../../../../basicplatform/drivers/mntn/blackbox/rdr_field.h"
#include <m3_rdr_ddr_map.h>
#include <rdr_cpu_rt_freq_volt_params.h>
#include <platform_include/cee/linux/rdr_get_cpu_rt_freq_volt.h>
#include <securec.h>

static unsigned int g_cpu_fv_tzsp_shmem[CPU_RT_FV_SHM_SIZE];
static unsigned char g_cpu_rt_tzsp_freq_info[CPU_RT_FV_BUFFER_SIZE];
static unsigned char g_cpu_rt_tzsp_volt_info[CPU_RT_FV_BUFFER_SIZE];
static int is_cpu_fv_get_from_tzsp_succ = RDR_CPU_RT_FV_GET_FAIL;
static unsigned int g_xpu_prof_num_tzsp[FREQ_DOMAIN_MAX] = { 0 };

static unsigned int g_cpu_fv_rdr_data_buf[CPU_RT_FV_BUFFER_SIZE];
static u64 g_xpu_prof_start_addr[FREQ_DOMAIN_MAX] = { 0 };
static u64 g_xpu_rt_start_addr[FREQ_DOMAIN_MAX] = { 0 };
static unsigned int g_xpu_prof_num[FREQ_DOMAIN_MAX] = { 0 };
static unsigned int g_lowtemp_prof[FREQ_DOMAIN_MAX] = { 0 };
static int is_cpu_fv_get_from_rdr_succ = RDR_CPU_RT_FV_GET_FAIL;
static unsigned short g_cpu_freq_mhz[FREQ_DOMAIN_MAX][CPU_MAX_PROFILE_NUMBER] = {0};
static struct cpu_dvfs_runtime g_cpu_dvfs_runtime[FREQ_DOMAIN_MAX] = {0};

static int get_max_prof_num_from_tzsp(void);

#if defined (CONFIG_PLATDRV_SUPPORT_FFA)
static int get_cpu_rt_fv_data_via_tzsp(void)
{
	int ret;
	int i;
	struct ffa_send_direct_data args = {
		.data0 = FID_LP_CPU_DVFS_GET_FV_INFO,
		.data1 = 0,
		.data2 = 0,
		.data3 = 0,
		.data4 = sizeof(g_cpu_fv_tzsp_shmem),
	};
	ret = memset_s(g_cpu_fv_tzsp_shmem, CPU_RT_FV_SHM_SIZE * sizeof(unsigned int), 0, CPU_RT_FV_SHM_SIZE * sizeof(unsigned int));
	if (ret != EOK) {
		pr_err("memset_s ret:%d, failed\n", ret);
		return ret;
	}
	ret = ffa_platdrv_send_msg_with_shmem(&args, (void *)&g_cpu_fv_tzsp_shmem, SHMEM_INOUT);
	if (ret || args.data4 != sizeof(g_cpu_fv_tzsp_shmem)) {
		pr_err("ret %d\n", ret);
		return ret;
	}
	return ret;
}

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
int set_cpu_improve_yield_data_via_tzsp(u64 cluster, u64 freq_idx_mask, u64 is_all_freq)
{
	int ret;
	struct ffa_send_direct_data args = {
		.data0 = FID_LP_CPU_RTB_YLD_RATE_VOLT,
		.data1 = cluster,
		.data2 = freq_idx_mask,
		.data3 = is_all_freq,
		.data4 = 0,
	};
 
	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("%s: ret:%d, failed\n", __func__, ret);

	return ret;
}
#endif
#else
static inline int get_cpu_rt_fv_data_via_tzsp(void)
{
	return -1;
}
#endif /* CONFIG_PLATDRV_SUPPORT_FFA */

int get_cpu_rt_freq_info_via_tzsp(char *out_buffer,  size_t buffer_maxsize)
{
	int ret;
	unsigned int buf_offset = 0;
	int i;
	unsigned int ary_idx;

	if (!out_buffer || buffer_maxsize == 0) {
		pr_err("%s: NULL pointerer\n", __func__);
		return RDR_CPU_RT_FV_GET_FAIL;
	}

	if (is_cpu_fv_get_from_tzsp_succ != RDR_CPU_RT_FV_GET_SUCC) {
		ret = get_cpu_rt_fv_data_via_tzsp();
		if (ret < 0) {
			pr_err("get_cpu_rt_fv_data_via_tzsp ret:%d, failed\n", ret);
			return ret;
		}
		is_cpu_fv_get_from_tzsp_succ = RDR_CPU_RT_FV_GET_SUCC;
	}
	(void)memset_s(out_buffer, buffer_maxsize, 0, buffer_maxsize);
	buf_offset = 0;
	for (i = FREQ_DOMAIN_LIT; i < FREQ_DOMAIN_MAX; i++) {
		ary_idx = i * EACH_CLUSTER_FV_INFO_ITEM;
		g_xpu_prof_num_tzsp[i] = g_cpu_fv_tzsp_shmem[ary_idx + MAX_PROF_NUM_OFFSET];
		ret = snprintf_s(out_buffer + buf_offset, buffer_maxsize, buffer_maxsize - 1,
			"C%d, cur_freq:%dMHz, last_freq:%dMHz, tar_freq:%dMHz, prof_num:%d\n",
			i, g_cpu_fv_tzsp_shmem[ary_idx + CUR_FREQ_OFFSET], g_cpu_fv_tzsp_shmem[ary_idx + LAST_FREQ_OFFSET],
			g_cpu_fv_tzsp_shmem[ary_idx + TAR_FREQ_OFFSET], g_xpu_prof_num_tzsp[i]);
		if (ret < 0) {
			pr_err("[C%d] snprintf_s ret:%d, failed\n", i, ret);
			return ret;
		}
		buf_offset += ret;
		buffer_maxsize -= ret;
	}
	return ret;
}

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
int get_cpu_rt_freq_index_via_tzsp(unsigned int cluster)
{
	int ret = get_cpu_rt_fv_data_via_tzsp();
	if (ret < 0) {
		pr_err("get_cpu_rt_fv_data_via_tzsp ret:%d, failed\n", ret);
		return ret;
	}

	return g_cpu_fv_tzsp_shmem[(cluster * EACH_CLUSTER_FV_INFO_ITEM) + CUR_FREQ_INDEX_OFFSET];
}
#endif

int get_cpu_rt_volt_info_via_tzsp(char *out_buffer,  size_t buffer_maxsize)
{
	int ret;
	unsigned int buf_offset = 0;
	int i;
	unsigned int ary_idx;

	if (!out_buffer || buffer_maxsize == 0) {
		pr_err("%s: NULL pointer\n", __func__);
		return RDR_CPU_RT_FV_GET_FAIL;
	}

	if (is_cpu_fv_get_from_tzsp_succ != RDR_CPU_RT_FV_GET_SUCC) {
		ret = get_cpu_rt_fv_data_via_tzsp();
		if (ret < 0) {
			pr_err("get_cpu_rt_fv_data_via_tzsp ret:%d, failed\n", ret);
			return ret;
		}
		is_cpu_fv_get_from_tzsp_succ = RDR_CPU_RT_FV_GET_SUCC;
	}
	(void)memset_s(out_buffer, buffer_maxsize, 0, buffer_maxsize);
	buf_offset = 0;
	for (i = FREQ_DOMAIN_LIT; i < FREQ_DOMAIN_MAX; i++) {
		ary_idx = i * EACH_CLUSTER_FV_INFO_ITEM;
		g_xpu_prof_num_tzsp[i] = g_cpu_fv_tzsp_shmem[ary_idx + MAX_PROF_NUM_OFFSET];
		ret = snprintf_s(out_buffer + buf_offset, buffer_maxsize, buffer_maxsize - 1,
			"C%d, cur_logic:%dmV, cur_mem:%dmV, prof_num:%d\n",
			i, g_cpu_fv_tzsp_shmem[ary_idx + CUR_LOGIC_OFFSET], g_cpu_fv_tzsp_shmem[ary_idx + CUR_MEM_OFFSET],
			g_xpu_prof_num_tzsp[i]);
		if (ret < 0) {
			pr_err("[C%d] snprintf_s ret:%d, failed\n", i, ret);
			return ret;
		}
		buf_offset += ret;
		buffer_maxsize -= ret;
	}
	return ret;
}

static int get_cpu_rt_fv_mntn_offset(const void *bbox_base, size_t size, u64 *offset)
{
	u64 rdr_phy_addr;
	u64 rdr_size;
	struct rdr_struct_s *rdr_head = NULL;

	if (bbox_base == NULL) {
		pr_err("bbox_base memory is NULL\n");
		return -EINVAL;
	}

	rdr_phy_addr = rdr_reserved_phymem_addr();
	rdr_size = rdr_reserved_phymem_size();
	if (size != rdr_size) {
		pr_err("invalid rdr memory size %lu\n", size);
		return -EINVAL;
	}

	rdr_head = (struct rdr_struct_s *)bbox_base;
	if (rdr_head->area_info[RDR_AREA_LPM3].offset <= rdr_phy_addr ||
		rdr_head->area_info[RDR_AREA_LPM3].length == 0 ||
		(rdr_head->area_info[RDR_AREA_LPM3].offset + rdr_head->area_info[RDR_AREA_LPM3].length >= \
		rdr_phy_addr + rdr_size)) {
		pr_err("check LPM3 memory failed, offset 0x%llx, size 0x%x\n",
			rdr_head->area_info[RDR_AREA_LPM3].offset,
			rdr_head->area_info[RDR_AREA_LPM3].length);
		return -EINVAL;
	}
	*offset = rdr_head->area_info[RDR_AREA_LPM3].offset - rdr_phy_addr + M3_RDR_CXPU_RAM_OFFSET;
	return RDR_CPU_RT_FV_GET_SUCC;
}

static int parse_rdr_xpu_profile_data(const void *bbox_base, size_t size)
{
	u64 addr = 0;
	u64 addr_limit = 0;
	unsigned int cluster, prof_num;
	unsigned short last_freq;
	const struct cpu_profile *p = NULL;
	u64 offset = 0;
	int ret;

	/* get cpu mntn data begin offset */
	ret = get_cpu_rt_fv_mntn_offset(bbox_base, size, &offset);
	if (ret != 0 || (offset > (u64)size)) {
		pr_err("get offset failed, offset:%lu, size:%lu\n", offset, size);
		return RDR_CPU_RT_FV_GET_FAIL;
	}
	addr = offset + bbox_base + CXPU_PROG_HEAD_SIZE;
	addr_limit = offset + bbox_base + CXPU_PROG_HEAD_SIZE + CXPU_PROFILE_SIZE;
	is_struct_aligned_int(struct cpu_profile);
	for (cluster = FREQ_DOMAIN_LIT; cluster < FREQ_DOMAIN_MAX; cluster++) {
		g_xpu_prof_start_addr[cluster] = addr;
		prof_num = 0;
		last_freq = 0;
		addr = offset + bbox_base + CXPU_PROG_HEAD_SIZE + sizeof(struct cpu_profile) * get_max_prof_num_from_tzsp() * cluster;
		addr_limit = offset + bbox_base + CXPU_PROG_HEAD_SIZE + sizeof(struct cpu_profile) * get_max_prof_num_from_tzsp() * (cluster + 1);
		while (addr < addr_limit) {
			p = (struct cpu_profile *)(uintptr_t)addr;
			if (p->freq < last_freq || p->freq == 0)
				break;
			if (p->lowtemp_flag != 0)
				g_lowtemp_prof[cluster] = prof_num;
			g_cpu_freq_mhz[cluster][prof_num] = *((unsigned int *)(uintptr_t)addr) & 0xFFFF;
			last_freq = p->freq;
			addr += CPU_PROFILE_BLOCK;
			prof_num++;
		}
		/* check whether addr has run out of range */
		if (addr > addr_limit) {
			pr_err("C%u p[%u] freq:%u, addr:%llx, parse xpu cpu profile fail\n", cluster, prof_num, p->freq, addr);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		g_xpu_prof_num[cluster] = prof_num;
		/* find the final profile struct, skip all the space until next cluster */
		while (addr < addr_limit && *((unsigned int *)(uintptr_t)addr) == 0)
			/* section must aligned 4 */
			addr += sizeof(unsigned int);
	}

	return RDR_CPU_RT_FV_GET_SUCC;
}

static int parse_rdr_xpu_runtime_data(const void *bbox_base, size_t size)
{
	u64 addr;
	u64 addr_limit;
	unsigned int cluster;
	const struct cpu_dvfs_runtime *rt = NULL;
	int ret;
	u64 offset = 0;

	/* get cpu mntn data begin offset */
	ret = get_cpu_rt_fv_mntn_offset(bbox_base, size, &offset);
	if (ret != 0 || (offset > (u64)size)) {
		pr_err("get offset failed, offset:%lu, size:%lu\n", offset, size);
		return RDR_CPU_RT_FV_GET_FAIL;
	}
	addr = offset + bbox_base + CXPU_SEC_DATA_BASE;
	addr_limit = offset + bbox_base + CXPU_SEC_DATA_BASE + CXPU_SEC_DATA_SIZE;
	/* find the starting position of LITTLE cluster */
	while (addr < addr_limit && *((unsigned int *)(uintptr_t)addr) != CPU_MNT_RT_MAGIC_NUM)
		/* section must aligned 4 */
		addr += sizeof(unsigned int);
	for (cluster = FREQ_DOMAIN_LIT; cluster < FREQ_DOMAIN_MAX; cluster++) {
		g_xpu_rt_start_addr[cluster] = addr;
		rt = (struct cpu_dvfs_runtime *)(uintptr_t)addr;
		(void)memcpy_s(&g_cpu_dvfs_runtime[cluster].name, sizeof(u8) * MAX_RT_NAME, rt->name, sizeof(u8) * MAX_RT_NAME);
		g_cpu_dvfs_runtime[cluster].cur_prof = rt->cur_prof;
		g_cpu_dvfs_runtime[cluster].last_prof = rt->last_prof;
		g_cpu_dvfs_runtime[cluster].tar_prof = rt->tar_prof;
		g_cpu_dvfs_runtime[cluster].cur_logic_cfg = rt->cur_logic_cfg;
		g_cpu_dvfs_runtime[cluster].cur_mem_cfg = rt->cur_mem_cfg;
		g_cpu_dvfs_runtime[cluster].last_logic_cfg = rt->last_logic_cfg;
		/* check whether addr has run out of range */
		if (addr > addr_limit) {
			pr_err("addr=%llx, addr_limit=%llx, parse_rdr_xpu_runtime_data failed\n", addr, addr_limit);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		addr += CPU_RUNTIME_BLOCK;
	}

	return RDR_CPU_RT_FV_GET_SUCC;
}

static int get_max_prof_num_from_tzsp(void)
{
	int ret = 0;
	int i;

	for (i = 0; i < FREQ_DOMAIN_MAX; i++) {
		if (g_xpu_prof_num_tzsp[i] > ret)
			ret = g_xpu_prof_num_tzsp[i];
	}
	return ret;
}

/*
 * For each cluster, the information format recorded is as follows:
 * cur_prof(MHz) cur_logic(mV) cur_mem(mV)
 * last_prof(MHz) tar_prof(MHz)
 */
static int get_cpu_fv_rdr_data(unsigned int *array, unsigned int size)
{
	unsigned int cluster;
	struct cpu_dvfs_runtime *rt = NULL;
	unsigned int ary_idx;

	if (array == NULL || size < FREQ_DOMAIN_MAX)
		return RDR_CPU_RT_FV_GET_FAIL;

	for (cluster = FREQ_DOMAIN_LIT; cluster < FREQ_DOMAIN_MAX; cluster++) {
		ary_idx = cluster * EACH_CLUSTER_FV_INFO_ITEM;
		/* cur_freq_MHz */
		array[ary_idx + CUR_FREQ_OFFSET] = g_cpu_freq_mhz[cluster][g_cpu_dvfs_runtime[cluster].cur_prof];
		/* cur_logic_mv */
		array[ary_idx + CUR_LOGIC_OFFSET] = g_cpu_dvfs_runtime[cluster].cur_logic_cfg * CPU_FV_INFO_LOGIC_STEP + CPU_FV_INFO_LOGIC_BASE;
		/* little cur_mem_mv fixed to 800mV */
		if (cluster == FREQ_DOMAIN_LIT)
			array[ary_idx + CUR_MEM_OFFSET] = 800;
		/* mid cur_mem_mv */
		else if (cluster == FREQ_DOMAIN_MID)
			array[ary_idx + CUR_MEM_OFFSET] = g_cpu_dvfs_runtime[cluster].cur_mem_cfg * CPU_FV_INFO_MEM_STEP + CPU_FV_INFO_MEM_BASE;
		/* big cur_mem_mv = cur_logic_mv */
		else
			array[ary_idx + CUR_MEM_OFFSET] = array[ary_idx + CUR_LOGIC_OFFSET];
		/* last_freq_MHz */
		array[ary_idx + LAST_FREQ_OFFSET] = g_cpu_freq_mhz[cluster][g_cpu_dvfs_runtime[cluster].last_prof];
		/* tar_freq_MHz */
		array[ary_idx + TAR_FREQ_OFFSET] = g_cpu_freq_mhz[cluster][g_cpu_dvfs_runtime[cluster].tar_prof];
	}
	return RDR_CPU_RT_FV_GET_SUCC;
}

int get_cpu_freq_info_for_diag(const void *bbox_base, size_t size, char *out_buffer,  size_t buffer_maxsize)
{
	int ret;
	int i;
	int ary_idx, buf_offset;

	if (!bbox_base || !out_buffer) {
		pr_err("%s: NULL pointer\n", __func__);
		return RDR_CPU_RT_FV_GET_FAIL;
	}

	if (buffer_maxsize == 0) {
		pr_err("%s: buf_size-%u is invalid\n", __func__, buffer_maxsize);
		return RDR_CPU_RT_FV_GET_FAIL;
	}
	if (is_cpu_fv_get_from_rdr_succ != RDR_CPU_RT_FV_GET_SUCC) {
		ret = get_cpu_rt_freq_info_via_tzsp(g_cpu_rt_tzsp_freq_info,  sizeof(char) * CPU_RT_FV_BUFFER_SIZE);
		if (ret < 0) {
			pr_err("get_cpu_rt_freq_info_via_tzsp, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		ret = parse_rdr_xpu_profile_data(bbox_base, size);
		if (ret != RDR_CPU_RT_FV_GET_SUCC) {
			pr_err("parse_rdr_xpu_profile_data, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		ret = parse_rdr_xpu_runtime_data(bbox_base, size);
		if (ret != RDR_CPU_RT_FV_GET_SUCC) {
			pr_err("parse_rdr_xpu_runtime_data, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		ret = get_cpu_fv_rdr_data(g_cpu_fv_rdr_data_buf, sizeof(g_cpu_fv_rdr_data_buf));
		if (ret != RDR_CPU_RT_FV_GET_SUCC) {
			pr_err("get_cpu_fv_rdr_data, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		is_cpu_fv_get_from_rdr_succ = RDR_CPU_RT_FV_GET_SUCC;
	}
	(void)memset_s(out_buffer, buffer_maxsize, 0, buffer_maxsize);
	buf_offset = 0;
	for (i = FREQ_DOMAIN_LIT; i < FREQ_DOMAIN_MAX; i++) {
		ary_idx = i * EACH_CLUSTER_FV_INFO_ITEM;
		ret = snprintf_s(out_buffer + buf_offset, buffer_maxsize, buffer_maxsize - 1,
			"C%d cur=%u-%d,last=%u-%d;",
			i, g_cpu_dvfs_runtime[i].cur_prof, g_cpu_fv_rdr_data_buf[ary_idx + CUR_FREQ_OFFSET],
			g_cpu_dvfs_runtime[i].last_prof, g_cpu_fv_rdr_data_buf[ary_idx + LAST_FREQ_OFFSET]);
		if (ret < 0) {
			pr_err("[C%d] snprintf_s ret:%d, failed\n", i, ret);
			return ret;
		}
		buf_offset += ret;
		buffer_maxsize -= ret;
	}
	return ret;
}

int get_cpu_volt_info_for_diag(const void *bbox_base, size_t size, char *out_buffer,  size_t buffer_maxsize)
{
	int ret;
	int i;
	int ary_idx, buf_offset;

	if (!bbox_base || !out_buffer) {
		pr_err("%s: NULL pointer\n", __func__);
		return RDR_CPU_RT_FV_GET_FAIL;
	}

	if (buffer_maxsize == 0) {
		pr_err("%s: buffer_maxsize-%u is invalid\n", __func__, buffer_maxsize);
		return RDR_CPU_RT_FV_GET_FAIL;
	}
	if (is_cpu_fv_get_from_rdr_succ != RDR_CPU_RT_FV_GET_SUCC) {
		ret = get_cpu_rt_freq_info_via_tzsp(g_cpu_rt_tzsp_freq_info,  sizeof(char) * CPU_RT_FV_BUFFER_SIZE);
		if (ret < 0) {
			pr_err("get_cpu_rt_freq_info_via_tzsp, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		ret = parse_rdr_xpu_profile_data(bbox_base, size);
		if (ret != RDR_CPU_RT_FV_GET_SUCC) {
			pr_err("parse_rdr_xpu_profile_data, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		ret = parse_rdr_xpu_runtime_data(bbox_base, size);
		if (ret != RDR_CPU_RT_FV_GET_SUCC) {
			pr_err("parse_rdr_xpu_runtime_data, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		ret = get_cpu_fv_rdr_data(g_cpu_fv_rdr_data_buf, sizeof(g_cpu_fv_rdr_data_buf));
		if (ret != RDR_CPU_RT_FV_GET_SUCC) {
			pr_err("get_cpu_fv_rdr_data, failed:%d\n", ret);
			return RDR_CPU_RT_FV_GET_FAIL;
		}
		is_cpu_fv_get_from_rdr_succ = RDR_CPU_RT_FV_GET_SUCC;
	}

	(void)memset_s(out_buffer, buffer_maxsize, 0, buffer_maxsize);
	buf_offset = 0;
	for (i = FREQ_DOMAIN_LIT; i < FREQ_DOMAIN_MAX; i++) {
		ary_idx = i * EACH_CLUSTER_FV_INFO_ITEM;
		ret = snprintf_s(out_buffer + buf_offset, buffer_maxsize, buffer_maxsize - 1,
			"C%d Vp=%d,Vc=%d;",
			i, g_cpu_fv_rdr_data_buf[ary_idx + CUR_LOGIC_OFFSET], g_cpu_fv_rdr_data_buf[ary_idx + CUR_MEM_OFFSET]);
		if (ret < 0) {
			pr_err("[C%d] snprintf_s ret:%d, failed\n", i, ret);
			return ret;
		}
		buf_offset += ret;
		buffer_maxsize -= ret;
	}
	return ret;
}
