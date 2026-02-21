/* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved. */

#include "ddr_mntn.h"
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/types.h>
#include <ddr/ddr_define_kernel.h>
#include <platform_include/cee/linux/hisi_ddr.h>
#include "securec.h"

#define QICE_INTR_STATUS_STR_MAX_LEN	8
#define QICE_FREQ_INFO_STR_MAX_LEN	8
#define DDR_FREQ_INFO_STR_MAX_LEN	20
#define DDR_VOLT_INFO_STR_MAX_LEN	20
bool g_qice_intr_flag = false;

bool get_ddr_mntn_info(unsigned int *value)
{
	struct ddr_mntn_info_stru *ddr_mntn_info = NULL;
	if (value == NULL) {
		pr_err("get_mntn_info, input value err\n");
		return false;
	}

	ddr_mntn_info = (struct ddr_mntn_info_stru *)ioremap_wc(DDR_MNTN_INFO_BASE, DDR_MNTN_INFO_SIZE);
	if (ddr_mntn_info == NULL) {
		pr_err("get ddr mntn error, addr map failed\n");
		return false;
	}
	*value = ddr_mntn_info->mntn_record;
	iounmap(ddr_mntn_info);
	return true;
}

bool check_qice_intr(void)
{
	struct ddr_mntn_info_stru *ddr_mntn_info = NULL;

	ddr_mntn_info = (struct ddr_mntn_info_stru *)ioremap_wc(DDR_MNTN_INFO_BASE, DDR_MNTN_INFO_SIZE);
	if (ddr_mntn_info == NULL) {
		pr_err("check qice intr error, addr map failed\n");
		return false;
	}
	if (ddr_mntn_info->qice_int_magic == QICE_INT_MAGICNUM) {
		pr_err("check qice intr, value 0x%X\n", ddr_mntn_info->mntn_record);
		ddr_mntn_info->qice_int_magic = 0;
		g_qice_intr_flag = true;
	}

	iounmap(ddr_mntn_info);
	return g_qice_intr_flag;
}

#ifdef CONFIG_DFX_DIAGINFO_BASEINFO
bool get_qice_freq_info(unsigned int *value)
{
	struct ddr_mntn_info_stru *ddr_mntn_info = NULL;

	if (value == NULL) {
		pr_err("get qice freq info, input value err\n");
		return false;
	}
	ddr_mntn_info = (struct ddr_mntn_info_stru *)ioremap_wc(DDR_MNTN_INFO_BASE, DDR_MNTN_INFO_SIZE);
	if (ddr_mntn_info == NULL) {
		pr_err("get_qice_freq_info, addr map failed\n");
		return false;
	}
	*value = ddr_mntn_info->qice_freq_info;
	iounmap(ddr_mntn_info);
	return true;
}

bool get_ddr_freq_info(unsigned int *value)
{
	unsigned int freq_id = 0;
	if (value == NULL) {
		pr_err("get ddr freq info, input value err\n");
		return false;
	}
	if (get_ddr_mntn_info(&freq_id) != true) {
		pr_err("get ddr mntn err\n");
		return false;
	}
	*value = GET_BITS_VALUE(freq_id, DDRMNTN_RECORD_DDR_FREQ_START, DDRMNTN_RECORD_DDR_FREQ_END);
	return true;
}

bool get_ddr_last_freq_info(unsigned int *value)
{
	unsigned int freq_id = 0;
	if (value == NULL) {
		pr_err("get ddr last freq info, input value err\n");
		return false;
	}
	if (get_ddr_mntn_info(&freq_id) != true) {
		pr_err("get ddr mntn err\n");
		return false;
	}
	*value = GET_BITS_VALUE(freq_id, DDRMNTN_RECORD_LAST_FREQ_START, DDRMNTN_RECORD_LAST_FREQ_END);
	return true;
}

bool get_ddr_core_volt_info(unsigned int *value)
{
	unsigned int freq_id = 0;
	if (value == NULL) {
		pr_err("get core volt info, input value err\n");
		return false;
	}
	if (get_ddr_mntn_info(&freq_id) != true) {
		pr_err("get ddr mntn err\n");
		return false;
	}
	*value = GET_BITS_VALUE(freq_id, DDRMNTN_RECORD_CORE_VOLT_START, DDRMNTN_RECORD_CORE_VOLT_END);
	return true;
}

bool get_ddr_last_core_volt_info(unsigned int *value)
{
	unsigned int freq_id = 0;
	if (value == NULL) {
		pr_err("get last core volt info, input value err\n");
		return false;
	}
	if (get_ddr_mntn_info(&freq_id) != true) {
		pr_err("get ddr mntn err\n");
		return false;
	}
	*value = GET_BITS_VALUE(freq_id, DDRMNTN_RECORD_LAST_CORE_VOLT_START, DDRMNTN_RECORD_LAST_CORE_VOLT_END);
	return true;
}

int get_qice_status_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size)
{
	bool status = 0;
	char qice_status_str[QICE_INTR_STATUS_STR_MAX_LEN] = {0};
	int ret;
	size_t len;

	(void)(rdr_memory);
	(void)(rdr_size);

	if (buf == NULL) {
		pr_err("%s:buf null\n", __func__);
		return -1;
	}

	status = check_qice_intr();

	ret = sprintf_s(qice_status_str, QICE_INTR_STATUS_STR_MAX_LEN, "%u", status);
	if (ret < 0) {
		pr_err("%s: print qice status info to str failed\n", __func__);
		return -1;
	}
	len = strlen(qice_status_str);
	if (size < len + 1) {
		pr_err("%s: no enough buffer for qice status, len %u, size %u\n", __func__, len, size);
		return -1;
	}
	ret = strncpy_s(buf, size, qice_status_str, len);
	if (ret != 0) {
		pr_err("copy qice status failed, ret %d\n", ret);
		return -1;
	}

	return 0;
}

int get_qice_freq_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size)
{
	unsigned int freq = 0;
	char freq_info_str[QICE_FREQ_INFO_STR_MAX_LEN] = {0};
	int ret;
	size_t len;

	(void)(rdr_memory);
	(void)(rdr_size);

	if (buf == NULL) {
		pr_err("%s:buf null\n", __func__);
		return -1;
	}

	if (!get_qice_freq_info(&freq))
		return -1;

	ret = sprintf_s(freq_info_str, QICE_FREQ_INFO_STR_MAX_LEN, "%u", freq);
	if (ret < 0) {
		pr_err("%s: print qice freq info to str failed\n", __func__);
		return -1;
	}
	len = strlen(freq_info_str);
	if (size < len + 1) {
		pr_err("%s: no enough buffer for qice freq info, len %u, size %u\n", __func__, len, size);
		return -1;
	}
	ret = strncpy_s(buf, size, freq_info_str, len);
	if (ret != 0) {
		pr_err("copy qice freq info failed, ret %d\n", ret);
		return -1;
	}

	return 0;
}

int get_ddr_freq_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size)
{
	unsigned int cur_freq = 0;
	unsigned int last_freq = 0;
	char freq_info_str[DDR_FREQ_INFO_STR_MAX_LEN] = {0};
	int ret;
	size_t len;

	(void)(rdr_memory);
	(void)(rdr_size);

	if (buf == NULL) {
		pr_err("%s:buf null\n", __func__);
		return -1;
	}
	get_ddr_freq_info(&cur_freq);
	get_ddr_last_freq_info(&last_freq);
	ret = sprintf_s(freq_info_str, DDR_FREQ_INFO_STR_MAX_LEN, "last=%u,cur=%u", last_freq, cur_freq);
	if (ret < 0) {
		pr_err("%s: print ddr freq info to str failed\n", __func__);
		return -1;
	}
	len = strlen(freq_info_str);
	if (size < len + 1) {
		pr_err("%s: no enough buffer for ddr freq info, len %u, size %u\n", __func__, len, size);
		return -1;
	}
	ret = strncpy_s(buf, size, freq_info_str, len);
	if (ret != 0) {
		pr_err("copy ddr freq info failed, ret %d\n", ret);
		return -1;
	}

	return 0;
}

int get_ddr_volt_info_str(const void *rdr_memory, size_t rdr_size, char *buf, size_t size)
{
	unsigned int cur_volt = 0;
	unsigned int last_volt = 0;
	char volt_info_str[DDR_VOLT_INFO_STR_MAX_LEN] = {0};
	int ret;
	size_t len;

	(void)(rdr_memory);
	(void)(rdr_size);

	if (buf == NULL) {
		pr_err("%s:buf null\n", __func__);
		return -1;
	}
	get_ddr_core_volt_info(&cur_volt);
	get_ddr_last_core_volt_info(&last_volt);
	ret = sprintf_s(volt_info_str, DDR_VOLT_INFO_STR_MAX_LEN, "last=%u,cur=%u", last_volt, cur_volt);
	if (ret < 0) {
		pr_err("%s: print ddr volt info to str failed\n", __func__);
		return -1;
	}
	len = strlen(volt_info_str);
	if (size < len + 1) {
		pr_err("%s: no enough buffer for ddr volt info, len %u, size %u\n", __func__, len, size);
		return -1;
	}
	ret = strncpy_s(buf, size, volt_info_str, len);
	if (ret != 0) {
		pr_err("copy ddr volt info failed, ret %d\n", ret);
		return -1;
	}

	return 0;
}
#endif