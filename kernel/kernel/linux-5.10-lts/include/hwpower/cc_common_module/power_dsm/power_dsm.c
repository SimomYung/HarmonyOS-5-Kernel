// SPDX-License-Identifier: GPL-2.0
/*
 * power_dsm.c
 *
 * dsm for power module
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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <linux/ctype.h>
#include <dfx/hiview_hisysevent.h>
#include <chipset_common/hwpower/common_module/power_dsm_config_single.h>
#else
#include <log/hiview_hievent.h>
#endif
#include <chipset_common/hwpower/common_module/power_sysfs.h>
#include <chipset_common/hwpower/common_module/power_dsm.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <securec.h>

#define HWLOG_TAG power_dsm
HWLOG_REGIST();

static struct device *g_power_dsm_dev;

#ifdef CONFIG_HUAWEI_OHOS_DSM
static LIST_HEAD(dmd_map_list);
static DEFINE_SPINLOCK(g_power_dsm_list_lock);

struct error_no_map {
	int error_no;
	char *name;
};
#endif  /* CONFIG_HUAWEI_OHOS_DSM */

static struct dsm_dev g_power_dsm_dev_cpu_buck = {
	.name = "dsm_cpu_buck",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_usb = {
	.name = "dsm_usb",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_0256,
};

static struct dsm_dev g_power_dsm_dev_battery_detect = {
	.name = "dsm_battery_detect",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_battery = {
	.name = "dsm_battery",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_2048,
};

static struct dsm_dev g_power_dsm_dev_charge_monitor = {
	.name = "dsm_charge_monitor",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_superswitch = {
	.name = "superswitch",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_smpl = {
	.name = "dsm_smpl",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_pd_richtek = {
	.name = "dsm_pd_richtek",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_pd = {
	.name = "dsm_pd",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_0256,
};

static struct dsm_dev g_power_dsm_dev_uscp = {
	.name = "dsm_usb_short_circuit_protect",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_pmu_ocp = {
	.name = "dsm_pmu_ocp",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_pmu_bms = {
	.name = "dsm_pmu_bms",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_pmic_irq = {
	.name = "dsm_pmu_irq",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_sc = {
	.name = "dsm_direct_charge_sc",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_1024,
};

static struct dsm_dev g_power_dsm_dev_lightstrap = {
	.name = "dsm_lightstrap",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = POWER_DSM_BUF_SIZE_0128,
};

/* define power dsm struct data */
static struct power_dsm_client g_power_dsm_client_data[] = {
	{ POWER_DSM_CPU_BUCK, "dsm_cpu_buck", NULL,
		&g_power_dsm_dev_cpu_buck },
	{ POWER_DSM_USB, "dsm_usb", NULL,
		&g_power_dsm_dev_usb },
	{ POWER_DSM_BATTERY_DETECT, "dsm_battery_detect", NULL,
		&g_power_dsm_dev_battery_detect },
	{ POWER_DSM_BATTERY, "dsm_battery", NULL,
		&g_power_dsm_dev_battery },
	{ POWER_DSM_CHARGE_MONITOR, "dsm_charge_monitor", NULL,
		&g_power_dsm_dev_charge_monitor },
	{ POWER_DSM_SUPERSWITCH, "superswitch", NULL,
		&g_power_dsm_dev_superswitch },
	{ POWER_DSM_SMPL, "dsm_smpl", NULL,
		&g_power_dsm_dev_smpl },
	{ POWER_DSM_PD_RICHTEK, "dsm_pd_richtek", NULL,
		&g_power_dsm_dev_pd_richtek },
	{ POWER_DSM_PD, "dsm_pd", NULL,
		&g_power_dsm_dev_pd },
	{ POWER_DSM_USCP, "dsm_usb_short_circuit_protect", NULL,
		&g_power_dsm_dev_uscp },
	{ POWER_DSM_PMU_OCP, "dsm_pmu_ocp", NULL,
		&g_power_dsm_dev_pmu_ocp },
	{ POWER_DSM_PMU_BMS, "dsm_pmu_bms", NULL,
		&g_power_dsm_dev_pmu_bms },
	{ POWER_DSM_PMU_IRQ, "dsm_pmu_irq", NULL,
		&g_power_dsm_dev_pmic_irq },
	{ POWER_DSM_DIRECT_CHARGE_SC, "dsm_direct_charge_sc", NULL,
		&g_power_dsm_dev_sc },
	{ POWER_DSM_LIGHTSTRAP, "dsm_lightstrap", NULL,
		&g_power_dsm_dev_lightstrap },
};

static int power_dsm_check_type(unsigned int type)
{
	if ((type >= POWER_DSM_TYPE_BEGIN) && (type < POWER_DSM_TYPE_END))
		return 0;

	hwlog_err("invalid type=%d\n", type);
	return -EPERM;
}

struct dsm_client *power_dsm_get_dclient(unsigned int type)
{
	int i;
	struct power_dsm_client *p_data = g_power_dsm_client_data;
	int size = ARRAY_SIZE(g_power_dsm_client_data);
	struct dsm_client *client = NULL;

	if (power_dsm_check_type(type))
		return NULL;

	hwlog_info("type=%d\n", type);

	for (i = 0; i < size; i++) {
		if (type == p_data[i].type) {
			client = p_data[i].client;
			break;
		}
	}

	if (!client) {
		hwlog_err("client is null\n");
		return NULL;
	}

	return client;
}

#ifdef CONFIG_HUAWEI_OHOS_DSM
struct power_dsm_err_map *power_dsm_single_map_register(const char *domain)
{
	struct power_dsm_err_map *temp = NULL;
	struct power_dsm_err_map *node = NULL;
	unsigned long flags;
	bool found = false;

	if (!domain)
		return NULL;

	if (!power_dsm_single_domain_check(domain)) {
		hwlog_err("unable to report other field's event:%s\n", domain);
		return NULL;
	}

	node = kzalloc(sizeof(*temp), GFP_KERNEL);
	if (!node)
		return NULL;

	spin_lock_irqsave(&g_power_dsm_list_lock, flags);
	list_for_each_entry(temp, &dmd_map_list, map_node) {
		if (!strncmp(domain, temp->map[0].domain, POWER_DSM_BUF_SIZE_0016)) {
			hwlog_err("domain : %s is already registered\n", domain);
			found = true;
			goto out;
		}
	}

	list_add_tail(&node->map_node, &dmd_map_list);
	hwlog_err("domain : %s is registered\n", domain);
out:
	spin_unlock_irqrestore(&g_power_dsm_list_lock, flags);
	if (found) {
		kfree(node);
		node = NULL;
		return NULL;
	}
	return node;
}
EXPORT_SYMBOL_GPL(power_dsm_single_map_register);

static void power_dsm_single_map_release(void)
{
	struct power_dsm_err_map *temp = NULL;
	struct power_dsm_err_map *next = NULL;
	unsigned long flags;

	spin_lock_irqsave(&g_power_dsm_list_lock, flags);
	list_for_each_entry_safe(temp, next, &dmd_map_list, map_node) {
		list_del(&temp->map_node);
		kfree(temp);
		temp = NULL;
	}
	spin_unlock_irqrestore(&g_power_dsm_list_lock, flags);
}

int power_dsm_single_hisysevent_check(char *dsm_error_domain, char *dsm_error_name)
{
	int i;
	struct power_dsm_err_map *temp = NULL;
	unsigned long flags;
	bool found = false;

	if (!dsm_error_domain || !dsm_error_name)
		return -EINVAL;

	spin_lock_irqsave(&g_power_dsm_list_lock, flags);
	list_for_each_entry(temp, &dmd_map_list, map_node) {
		if (strncmp(dsm_error_domain, temp->map[0].domain, POWER_DSM_BUF_SIZE_0016))
			continue;
		for (i = 0; i < temp->event_num; i++) {
			if (!strncmp(temp->map[i].name, dsm_error_name, POWER_DSM_BUF_SIZE_0032)) {
				found = true;
				goto out;
			}
		}
	}

	hwlog_err("event %s is not configured\n", dsm_error_name);
out:
	spin_unlock_irqrestore(&g_power_dsm_list_lock, flags);
	return found ? POWER_DSM_OK : POWER_DSM_ERR;
}

static int power_dsm_single_convert_dsm_to_hisysevent(struct hisysevent_error_no_map *dmd_msg)
{
	int ret = POWER_DSM_ERR;
	int i;
	struct power_dsm_err_map *temp = NULL;
	unsigned long flags;

	if (!dmd_msg)
		return -EINVAL;

	spin_lock_irqsave(&g_power_dsm_list_lock, flags);
	list_for_each_entry(temp, &dmd_map_list, map_node) {
		for (i = 0; i < temp->event_num; i++) {
			if (dmd_msg->error_no == temp->map[i].error_no) {
				dmd_msg->level = temp->map[i].level;
				ret = strncpy_s(dmd_msg->name, POWER_DSM_BUF_SIZE_0032, temp->map[i].name, strlen(temp->map[i].name));
				ret += strncpy_s(dmd_msg->domain, POWER_DSM_BUF_SIZE_0016, temp->map[i].domain, strlen(temp->map[i].domain));
				ret += strncpy_s(dmd_msg->key, POWER_DSM_BUF_SIZE_0048, temp->map[i].key, strlen(temp->map[i].key));
				dmd_msg->running_test_switch = temp->map[i].running_test_switch;
				hwlog_err("%s event match: level %d name %s domain %s key %s running_test_switch %d, \
					 map level %d map name %s, map domain %s map key %s,map running_test_switch %d, ret %d\n", __func__, 
					dmd_msg->level, dmd_msg->name, dmd_msg->domain, dmd_msg->key, dmd_msg->running_test_switch,
					temp->map[i].level, temp->map[i].name, temp->map[i].domain, temp->map[i].key,temp->map[i].running_test_switch, ret);
				break;
			}
		}
	}
	spin_unlock_irqrestore(&g_power_dsm_list_lock, flags);
	return ret;
}

static int power_dsm_single_report_hiview(const char *key, const char *value, struct hisysevent_error_no_map *dmd_msg)
{
	int ret;
	struct hiview_hisysevent *hi_event = NULL;

	if(!dmd_msg || !value || !key)
		return -EINVAL;

	hi_event = hisysevent_create(dmd_msg->domain, dmd_msg->name, dmd_msg->level);
	if (!hi_event) {
		hwlog_err("%s create hievent fail\n", __func__);
		return -EPERM;
	}

	hisysevent_put_string(hi_event, key, value);
	hisysevent_put_integer(hi_event, "ERROR_NO", dmd_msg->error_no);
	hisysevent_put_integer(hi_event, "RUNNING_TEST_SWITCH", dmd_msg->running_test_switch);
	ret = hisysevent_write(hi_event);
	if (ret < 0) {
		hwlog_err("%s hievent report fail\n", __func__);
		hisysevent_destroy(&hi_event);
		return -EPERM;
	}

	hisysevent_destroy(&hi_event);
	hwlog_info("%s domain=%s err_no=%s key=%s value=%s level=%d running_test_switch=%d\n",
		__func__, dmd_msg->domain, dmd_msg->name, key, value, dmd_msg->level, dmd_msg->running_test_switch);
	return POWER_DSM_OK;
}

static int power_dsm_single_report_multi_hiview(const char *domain, const char *error_name,
	struct hiview_info *info, unsigned int num, unsigned int err_level)
{
	int ret;
	int i;
	struct hiview_hisysevent *hi_event = NULL;

	if (!info || !error_name || !domain)
		return -EINVAL;

	hi_event = hisysevent_create(domain, error_name, err_level);
	if (!hi_event) {
		hwlog_err("%s create hievent fail\n", __func__);
		return -EPERM;
	}

	for (i = 0; i < num; i++) {
		if (!(info[i].key) || !(info[i].value)) {
			hisysevent_destroy(&hi_event);
			return -EINVAL;
		}
		hisysevent_put_string(hi_event, info[i].key, info[i].value);
	}

	ret = hisysevent_write(hi_event);
	if (ret < 0) {
		hwlog_err("%s hievent report fail\n", __func__);
		hisysevent_destroy(&hi_event);
		return -EPERM;
	}

	hisysevent_destroy(&hi_event);
	for (i = 0; i < num; i++)
		hwlog_info("%s domain=%s err_no=%s key[%d]=%s value[%d]=%s level=%d\n",
			__func__, domain, error_name, i, info[i].key, i, info[i].value, err_level);
	return POWER_DSM_OK;
}
#endif  /* CONFIG_HUAWEI_OHOS_DSM */

int power_dsm_report_hiview(unsigned int err_no, const char *key, const char *value)
{
	if(!key || !value)
		return -EINVAL;

#ifdef CONFIG_HUAWEI_OHOS_DSM
	struct hisysevent_error_no_map dmd_msg = { 0 };
	dmd_msg.error_no = err_no;

	if (power_dsm_single_convert_dsm_to_hisysevent(&dmd_msg) == POWER_DSM_OK) {
		power_dsm_single_report_hiview(key, value, &dmd_msg);
		return POWER_DSM_OK;
	}

	return POWER_DSM_ERR;
#else
	int ret;
	struct hiview_hievent *hi_event = NULL;

	hi_event = hiview_hievent_create(err_no);
	if (!hi_event) {
		hwlog_err("%s create hievent fail\n", __func__);
		return -EPERM;
	}

	hiview_hievent_put_string(hi_event, key, value);
	ret = hiview_hievent_report(hi_event);
	if (ret <= 0) {
		hwlog_err("%s hievent report fail\n", __func__);
		hiview_hievent_destroy(hi_event);
		return -EPERM;
	}

	hiview_hievent_destroy(hi_event);
	hwlog_info("%s err_no=%d key=%s value=%s\n", __func__, err_no, key, value);
	return 0;
#endif /* CONFIG_HUAWEI_OHOS_DSM */
}
EXPORT_SYMBOL(power_dsm_report_hiview);

#ifdef CONFIG_HUAWEI_OHOS_DSM
int power_dsm_report_multi_hiview(unsigned int err_no, struct hiview_info *info, unsigned int num)
{
	struct hisysevent_error_no_map dmd_msg = { 0 };
	dmd_msg.error_no = err_no;

	if (power_dsm_single_convert_dsm_to_hisysevent(&dmd_msg) == POWER_DSM_OK) {
		power_dsm_single_report_multi_hiview(dmd_msg.domain, dmd_msg.name, info, num, dmd_msg.level);
		return POWER_DSM_OK;
	}

	return POWER_DSM_ERR;
}
EXPORT_SYMBOL(power_dsm_report_multi_hiview);
#else
int power_dsm_report_multi_hiview(unsigned int err_no, struct hiview_info *info, unsigned int num)
{
	int ret;
	int i;
	struct hiview_hievent *hi_event = NULL;

	hi_event = hiview_hievent_create(err_no);
	if (!hi_event) {
		hwlog_err("%s create hievent fail\n", __func__);
		return -EPERM;
	}

	for (i = 0; i < num; i++) {
		if (!(info[i].key) || !(info[i].value)) {
			hiview_hievent_destroy(hi_event);
			return -EPERM;
		}
		hiview_hievent_put_string(hi_event, info[i].key, info[i].value);
	}

	ret = hiview_hievent_report(hi_event);
	if (ret <= 0) {
		hwlog_err("%s hievent report fail\n", __func__);
		hiview_hievent_destroy(hi_event);
		return -EPERM;
	}

	hiview_hievent_destroy(hi_event);
	for (i = 0; i < num; i++)
		hwlog_info("%s err_no=%d key[%d]=%s value[%d]=%s\n", __func__, err_no, i, info[i].key, i, info[i].value);
	return 0;
}
EXPORT_SYMBOL(power_dsm_report_multi_hiview);
#endif /* CONFIG_HUAWEI_OHOS_DSM */

int power_dsm_report_dmd(unsigned int type, int err_no, const char *buf)
{
#ifdef CONFIG_HUAWEI_OHOS_DSM
	struct hisysevent_error_no_map dmd_msg = { 0 };
	dmd_msg.error_no = err_no;

	if (power_dsm_single_convert_dsm_to_hisysevent(&dmd_msg) == POWER_DSM_OK)
		return power_dsm_single_report_hiview(dmd_msg.key, buf, &dmd_msg);

	hwlog_err("event %d is not register yet\n", err_no);
	return -EPERM;
#else
	struct dsm_client *client = power_dsm_get_dclient(type);

	if (!client || !buf) {
		hwlog_err("client or buf is null\n");
		return -EPERM;
	}

	if (!dsm_client_ocuppy(client)) {
		dsm_client_record(client, "%s", buf);
		dsm_client_notify(client, err_no);
		hwlog_info("report type:%d, err_no:%d\n", type, err_no);
		return 0;
	}

	hwlog_err("power dsm client is busy\n");
	return -EPERM;
#endif /* CONFIG_HUAWEI_OHOS_DSM */
}
EXPORT_SYMBOL(power_dsm_report_dmd);

#ifdef CONFIG_HUAWEI_OHOS_DSM
int power_dsm_report_format_dmd(unsigned int type, int err_no, const char *fmt, ...)
{
	int32_t ret;
	char outstr[POWER_DSM_BUF_SIZE_1024] = { 0 };
	va_list arg;

	va_start(arg, fmt);
	ret = vsprintf_s(outstr, sizeof(outstr), fmt, arg);
	va_end(arg);
	if (ret < 0)
		return -EINVAL;

	return power_dsm_report_dmd(type, err_no, outstr);
}
#endif /* CONFIG_HUAWEI_OHOS_DSM */
#ifdef CONFIG_HUAWEI_DATA_ACQUISITION
int power_dsm_report_fmd(unsigned int type, int err_no, const void *msg)
{
	int ret;
	struct dsm_client *client = power_dsm_get_dclient(type);

	if (!client || !msg) {
		hwlog_err("client or msg is null\n");
		return -EPERM;
	}

	if (!dsm_client_ocuppy(client)) {
		ret = dsm_client_copy_ext(client, (struct message *)msg,
			sizeof(struct message));
		dsm_client_notify(client, err_no);
		hwlog_info("report type:%d, err_no:%d\n", type, err_no);
		return ret;
	}

	hwlog_err("power dsm client is busy\n");
	return -EPERM;
}
#endif /* CONFIG_HUAWEI_DATA_ACQUISITION */

static void power_dsm_dump_specified_data(struct power_dsm_dump *data)
{
	int ret;
	char *buf = NULL;
	unsigned int max_size = POWER_DSM_BUF_SIZE_2048;
	unsigned int used;
	unsigned int unused;

	/* skip when dump disabled */
	if (!data->dump_enable)
		return;

	/* skip when platform not support */
	if (data->support && !data->support())
		return;

	buf = kzalloc(max_size + POWER_DSM_RESERVED_SIZE, GFP_KERNEL);
	if (!buf)
		return;

	if (data->pre_buf) {
		used = strlen(buf);
		unused = max_size - used;
		snprintf(buf, unused, "%s: ", data->pre_buf);
	}

	if (data->check_error) {
		used = strlen(buf);
		unused = max_size - used;
		if (!data->check_error(buf + used, unused))
			goto end;
	}

	if (data->dump) {
		used = strlen(buf);
		unused = max_size - used;
		data->dump(buf + used, unused);
	}

	ret = power_dsm_report_dmd(data->type, data->error_no, buf);
	if (!ret) {
		/* dump will disabeld when no need always report */
		if (!data->dump_always)
			data->dump_enable = false;
	}

end:
	kfree(buf);
}

void power_dsm_reset_dump_enable(struct power_dsm_dump *data, unsigned int size)
{
	unsigned int i;

	if (!data)
		return;

	for (i = 0; i < size; i++) {
		if (data[i].dump_always)
			data[i].dump_enable = true;
	}
}

void power_dsm_dump_data(struct power_dsm_dump *data, unsigned int size)
{
	unsigned int i;

	if (!data)
		return;

	for (i = 0; i < size; i++)
		power_dsm_dump_specified_data(&data[i]);
}

void power_dsm_dump_data_with_error_no(struct power_dsm_dump *data,
	unsigned int size, int err_no)
{
	unsigned int i;

	if (!data)
		return;

	for (i = 0; i < size; i++) {
		if (err_no != data[i].error_no)
			continue;

		power_dsm_dump_specified_data(&data[i]);
		break;
	}
}

#if ((defined(CONFIG_HUAWEI_POWER_DEBUG) || defined(CONFIG_HUAWEI_OHOS_DSM)) && defined(CONFIG_SYSFS))
static unsigned int g_power_dsm_type = POWER_DSM_TYPE_END;
static int g_power_dsm_errno;

static ssize_t power_dsm_set_info(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int dsm_type = 0;
	unsigned int dsm_errno = 0;

	if (count >= POWER_DSM_BUF_SIZE_0016) {
		hwlog_err("input too long\n");
		return -EINVAL;
	}

	/* 2: numbers for dsm_type and dsm_errno */
	if (sscanf(buf, "%u %u", &dsm_type, &dsm_errno) != 2) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	if (power_dsm_check_type(dsm_type))
		return -EINVAL;

	g_power_dsm_type = dsm_type;
	g_power_dsm_errno = dsm_errno;
	power_dsm_report_dmd(dsm_type, dsm_errno, buf);

	return count;
}

static ssize_t power_dsm_show_info(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "type:%u, err_no:%d",
		g_power_dsm_type, g_power_dsm_errno);
}

#ifdef CONFIG_HUAWEI_OHOS_DSM
static ssize_t power_dsm_single_set_hisys_dsm(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	char dsm_error_content[POWER_DSM_BUF_SIZE_1024] = { '\0' };
	char dsm_error_level_tmp[POWER_DSM_BUF_SIZE_0016] = { '\0' };
	struct hisysevent_error_no_map dmd_msg = { 0 };

	if (count >= POWER_HISYSENVENT_BUF_MAX) {
		hwlog_err("input too long count %d\n", count);
		return -EINVAL;
	}

	/* format:domain~name~level~key~content */
	if (sscanf_s(buf, "%[^:]:%[^:]:%[0-9]:%[^:]:%[^\n]", dmd_msg.domain, POWER_DSM_BUF_SIZE_0016,
	   dmd_msg.name, POWER_DSM_BUF_SIZE_0032, dsm_error_level_tmp, POWER_DSM_BUF_SIZE_0016,
	   dmd_msg.key, POWER_DSM_BUF_SIZE_0048, dsm_error_content, POWER_DSM_BUF_SIZE_2048)
	   != POWER_HISYSENVENT_PARAM_NUM) {
		hwlog_err("unable to parse input:%s\n", buf);
		return -EINVAL;
	}

	/* check is BmsDmdWriteInfo report or not, eg:BATTERY:920008000:1:CONTENT:cc_no_ibus */
	if (isdigit(dmd_msg.name[0])) {
		/* convert dsm_error_name from str to int */
		dmd_msg.error_no = simple_strtoul(dmd_msg.name, NULL, POWER_HISYSENVENT_DECIMAL);
		
		if (power_dsm_single_convert_dsm_to_hisysevent(&dmd_msg) == POWER_DSM_OK) {
			power_dsm_single_report_hiview(dmd_msg.key, dsm_error_content, &dmd_msg);
			return count;
		}
	}

	if (!power_dsm_single_domain_check(dmd_msg.domain)) {
		hwlog_err("unable to report other field's event:%s\n", dmd_msg.domain);
		return -EINVAL;
	}

	dmd_msg.level = simple_strtoul(dsm_error_level_tmp, NULL, POWER_HISYSENVENT_DECIMAL);
	if (dmd_msg.level >= POERT_DSM_LEVEL_MAX || dmd_msg.level < POWER_DSM_LEVEL_FAULT) {
		hwlog_err("invalid event level:%d\n", dmd_msg.level);
		return -EPERM;
	}

	if (power_dsm_single_hisysevent_check(dmd_msg.domain, dmd_msg.name) == POWER_DSM_OK) {
		power_dsm_single_report_hiview(dmd_msg.key, dsm_error_content, &dmd_msg);
		return count;
	}

	return -EPERM;
}
static DEVICE_ATTR(hisys_dsm, 0220, NULL, power_dsm_single_set_hisys_dsm);
#endif /* CONFIG_HUAWEI_OHOS_DSM */
static DEVICE_ATTR(info, 0640, power_dsm_show_info, power_dsm_set_info);

static struct attribute *power_dsm_attributes[] = {
	&dev_attr_info.attr,
#ifdef CONFIG_HUAWEI_OHOS_DSM
	&dev_attr_hisys_dsm.attr,
#endif /* CONFIG_HUAWEI_OHOS_DSM */
	NULL,
};

static const struct attribute_group power_dsm_attr_group = {
	.attrs = power_dsm_attributes,
};

static struct device *power_dsm_create_group(void)
{
	return power_sysfs_create_group("hw_power", "power_dsm",
		&power_dsm_attr_group);
}

static void power_dsm_remove_group(struct device *dev)
{
	power_sysfs_remove_group(dev, &power_dsm_attr_group);
}
#else
static inline struct device *power_dsm_create_group(void)
{
	return NULL;
}

static inline void power_dsm_remove_group(struct device *dev)
{
}
#endif /* (CONFIG_HUAWEI_POWER_DEBUG || CONFIG_HUAWEI_OHOS_DSM) && CONFIG_SYSFS */

static int __init power_dsm_init(void)
{
	int i;
	struct power_dsm_client *p_data = g_power_dsm_client_data;
	int size = ARRAY_SIZE(g_power_dsm_client_data);

	for (i = 0; i < size; i++) {
		if (!p_data[i].client)
			p_data[i].client = dsm_register_client(p_data[i].dev);

		if (!p_data[i].client)
			hwlog_err("%s dsm register fail\n", p_data[i].name);
		else
			hwlog_info("%s dsm register ok\n", p_data[i].name);
	}

	g_power_dsm_dev = power_dsm_create_group();
#ifdef CONFIG_HUAWEI_OHOS_DSM
	power_dsm_single_battery_register();
#endif /* CONFIG_HUAWEI_OHOS_DSM */
	return 0;
}

static void __exit power_dsm_exit(void)
{
	int i;
	struct power_dsm_client *p_data = g_power_dsm_client_data;
	int size = ARRAY_SIZE(g_power_dsm_client_data);

	for (i = 0; i < size; i++) {
		p_data[i].client = NULL;
		hwlog_info("dsm %s un-register ok\n", p_data[i].name);
	}

#ifdef CONFIG_HUAWEI_OHOS_DSM
	power_dsm_single_map_release();
#endif /* CONFIG_HUAWEI_OHOS_DSM */
	power_dsm_remove_group(g_power_dsm_dev);
	g_power_dsm_dev = NULL;
}

subsys_initcall_sync(power_dsm_init);
module_exit(power_dsm_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("dsm for power module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
