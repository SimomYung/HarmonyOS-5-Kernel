/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2028. All rights reserved.
 *
 * hisi_vbat_drop_protect.c
 *
 * driver for vbat drop protect
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "hisi_vbat_drop_protect.h"
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/regmap.h>
#include <linux/platform_device.h>
#include <linux/cpufreq.h>
#include <linux/devfreq.h>
#include <linux/platform_drivers/platform_qos.h>
#include <platform_include/cee/linux/npu_pm.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_drv.h>
#include <linux/slab.h>
#include <linux/power_supply.h>
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif
#ifdef CONFIG_HW_VOTE
#include <platform_include/cee/linux/hw_vote.h>
#endif

static unsigned int g_notifier_enable = 1;
static struct hisi_vbat_drop_protect_dev *g_di;
static int (*g_vbat_to_isp_handler)(unsigned int) = NULL;
static struct platform_qos_request g_npu_pm_qos_req_vbat;

#ifdef CONFIG_NPUFREQ_PLATFORM_QOS
static const int g_npu_pm_qos_class_up = PLATFORM_QOS_NPU_FREQ_UPLIMIT;
#else
static const int g_npu_pm_qos_class_up = PM_QOS_NPU_FREQ_UPLIMIT;
#endif

#define hv_name(hvdev)                                                         \
	(IS_ERR_OR_NULL((hvdev)->name) ? "unknown" : (hvdev)->name)

void vbat_to_isp_callback_register(int (*vbat_to_isp_handler)(unsigned int))
{
	if (vbat_to_isp_handler == NULL) {
		pr_err("vbat to isp callback register failed!\n");
		return;
	}
	g_vbat_to_isp_handler = vbat_to_isp_handler;
}
EXPORT_SYMBOL(vbat_to_isp_callback_register);

void vbat_to_isp_callback_unregister(void)
{
	g_vbat_to_isp_handler = NULL;
}
EXPORT_SYMBOL(vbat_to_isp_callback_unregister);

static void vbat_drop_nputop_freq_uplimit(unsigned long freq_mhz)
{
	if (freq_mhz == 0 && platform_qos_request_active(&g_npu_pm_qos_req_vbat)) {
		pr_err("[%s] freq is 0, unregister qos request\n", __func__);
		platform_qos_remove_request(&g_npu_pm_qos_req_vbat);
		return;
	}
 
	if (!platform_qos_request_active(&g_npu_pm_qos_req_vbat)) {
		pr_err("[%s] register qos request\n", __func__);
		platform_qos_add_request(&g_npu_pm_qos_req_vbat, g_npu_pm_qos_class_up, 0);
		platform_qos_update_request(&g_npu_pm_qos_req_vbat, freq_mhz);
	} else {
		platform_qos_update_request(&g_npu_pm_qos_req_vbat, freq_mhz);
	}
}

static int get_power_supply_property(int *value, enum power_supply_property property)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = {0};

	if (value == NULL)
		return -EINVAL;

	psy = power_supply_get_by_name("Battery");
	if (!psy) {
		pr_err("%s power supply Battery not exist\n", __func__);
		return -EINVAL;
	}

	ret = power_supply_get_property(psy, property, &val);
	power_supply_put(psy);

	*value = val.intval;
	pr_debug("%s property:%u, val:%d\n", __func__, property, *value);
	return ret;
}

#ifdef CONFIG_HW_VOTE
static struct hvdev *cluster_freq_register(struct hisi_vbat_drop_protect_dev *di,
									struct device_node *np, const char *propname)
{
	struct device *dev = di->dev;
	const char *ch_name = NULL;
	const char *vsrc = NULL;
	struct hvdev *freq_hvdev = NULL;
	int ret;

	/* get cpu freq from dts and register to hvdev */
	ret = of_property_read_string_index(np, propname, 0, &ch_name);
	if (ret) {
		dev_err(dev, "[%s]:parse %s fail,not channel!\n", __func__,
			propname);
		return NULL;
	}
	ret = of_property_read_string_index(np, propname, 1, &vsrc);
	if (ret) {
		dev_err(dev, "[%s]:parse %s vote src fail!\n", __func__,
			propname);
		return NULL;
	}
	freq_hvdev = hvdev_register(dev, ch_name, vsrc);
	if (IS_ERR_OR_NULL(freq_hvdev)) {
		dev_err(dev, "[%s]: %s vote register fail!\n", __func__,
			propname);
		return NULL;
	}
	return freq_hvdev;
}

static void limit_cluster_freq_by_vote_channel(struct hvdev *hvdev, int freq)
{
	int ret;

	if (IS_ERR_OR_NULL(hvdev)) {
		pr_err("%s: not register hw vote!\n", __func__);
		return;
	}
	ret = hv_set_freq(hvdev, freq);
	if (!ret)
		pr_err("[%s]%s votes to limit freq\n", __func__,
			hv_name(hvdev));
}
#endif

static int get_passive_perf_limit_percent(struct hisi_vbat_drop_protect_dev *di,
				struct cluster_perf_limit_info * cluster_perf_limit, int batt_cap)
{
	int index, temp_level, limit_para_num;
	unsigned int limit_perf_percent;
	int temp = coul_drv_battery_temperature();

	struct passive_perf_limit_para *perf_passive_limit = NULL;

	if (cluster_perf_limit == NULL || !cluster_perf_limit->passvie_perf_limit_en)
		return FULL_OF_PERCENT;
	
	for (index = 0; index < cluster_perf_limit->temprature_soc_table_num; index++) {
		if (temp >= cluster_perf_limit->temprature_table[index].temp_l &&
			temp < cluster_perf_limit->temprature_table[index].temp_h) {
			temp_level = index;
            break;
		}
	}

	if (index >= cluster_perf_limit->temprature_soc_table_num || !cluster_perf_limit->perf_passive_limit[temp_level])
		return FULL_OF_PERCENT;

	limit_para_num = cluster_perf_limit->active_perf_limit_para_num / cluster_perf_limit->temprature_soc_table_num;
	for (index = 0; index < limit_para_num; index++) {
		perf_passive_limit = &cluster_perf_limit->perf_passive_limit[temp_level][index];
		if (di->batt_cap >= perf_passive_limit->soc_l && di->batt_cap <= perf_passive_limit->soc_h) {
			limit_perf_percent = cluster_perf_limit->limit_perf_percent - perf_passive_limit->step_val;
			limit_perf_percent = max(limit_perf_percent, perf_passive_limit->perf_lowerest_limit);
			return limit_perf_percent;
		}
	}

	return FULL_OF_PERCENT;
}

static int get_active_perf_limit_percent(struct hisi_vbat_drop_protect_dev *di,
				struct cluster_perf_limit_info * cluster_perf_limit, int batt_cap)
{
	int index, temp_level, limit_para_num, limit_perf_percent;
	int temp = coul_drv_battery_temperature();
	struct active_perf_limit_para *perf_active_limit = NULL;

	if (cluster_perf_limit == NULL || !cluster_perf_limit->active_perf_limit_en)
		return FULL_OF_PERCENT;

	for (index = 0; index < cluster_perf_limit->temprature_soc_table_num; index++) {
		if (temp >= cluster_perf_limit->temprature_table[index].temp_l &&
			temp < cluster_perf_limit->temprature_table[index].temp_h) {
			temp_level = index;
            break;
		}
	}

	if (index >= cluster_perf_limit->temprature_soc_table_num || !cluster_perf_limit->perf_active_limit[temp_level])
		return FULL_OF_PERCENT;

	limit_para_num = cluster_perf_limit->active_perf_limit_para_num / cluster_perf_limit->temprature_soc_table_num;
	for (index = 0; index < limit_para_num; index++) {
		perf_active_limit = &cluster_perf_limit->perf_active_limit[temp_level][index];
		if (di->batt_cap >= perf_active_limit->soc_l && di->batt_cap <= perf_active_limit->soc_h) {
			limit_perf_percent = min(cluster_perf_limit->limit_perf_percent, perf_active_limit->perf_max_limit);
			return limit_perf_percent;
		}
	}

	return FULL_OF_PERCENT;
}

static int get_cpus_max_freq(unsigned long *max_freq_khz, struct device_node *np)
{
	unsigned int i, cpu;
	struct device_node *cpu_np = NULL;
	struct device_node *limit_np = NULL;

	for_each_possible_cpu(cpu) {
		cpu_np = of_get_cpu_node(cpu, NULL);
		if (!cpu_np) {
			pr_err("%s: failed to get cpu%u node\n",
				__func__, cpu);
			return -ENOENT;
		}

		/* Get cluster vbat-drop-limit descriptor node */
		for (i = 0; ; i++) {
			limit_np = of_parse_phandle(cpu_np, "vdrop-drop-limit", i);
			if (!limit_np) {
				pr_debug("%s: failed to get cpu%u vdrop-drop-limit node\n",
				__func__, cpu);
				break;
			}

			/* CPUs are vbat-drop-limit node */
			if (np == limit_np) {
				*max_freq_khz = cpufreq_get_hw_max_freq(cpu);
				if (unlikely(!(*max_freq_khz))) {
					pr_err("[%s]: cluster[%u] max freq is invalid!\n", __func__, cpu);
				}
				of_node_put(limit_np);
			}
			of_node_put(limit_np);
		}

		of_node_put(cpu_np);
	}
	return 0;
}

static int get_cpu_max_freq_by_node(struct device_node *np, int cluster,
									unsigned long *max_freq_khz)
{
	int ret, cluster_id;
	struct device_node *child_np = NULL;
	
	for_each_available_child_of_node(np, child_np) {
		if (of_property_read_u32(child_np, "vbat_drop_core_channel", &cluster_id)) {
			pr_err("[%s]: get vbat_drop_core_channel fail!\n", __func__);
			continue;
		}

		if (cluster == cluster_id) {
			ret = get_cpus_max_freq(max_freq_khz, child_np);
			if (ret) {
				pr_err("[%s]: get_cpus_max_freq fail!\n", __func__);
				return -1;
			}
			break;
		}
	}
	return 0;
}

static int get_max_freq_by_cluster_name(unsigned long *max_freq_khz, const char *name)
{
	struct devfreq *devfreq = NULL;

	devfreq = find_devfreq_by_name(name);
	if (IS_ERR_OR_NULL(devfreq))
		return -1;

	*max_freq_khz = find_available_max_freq(devfreq) / PERMILLAGE;
	if (!(*max_freq_khz))
		return -1;

	return 0;
}

static void set_cpu_freq(struct hisi_vbat_drop_protect_dev *di,
			struct cluster_perf_limit_info *cluster_perf_limit, int perf_limit_percent)
{
	struct device_node *np = di->dev->of_node;
	int ret;

	ret = get_cpu_max_freq_by_node(np, cluster_perf_limit->cluster_id,
							&cluster_perf_limit->max_perf_khz);
	if (ret || unlikely(!cluster_perf_limit->max_perf_khz)) {
		pr_err("[%s]: get_cpu_max_freq fail!\n", __func__);
		return;
	}
	cluster_perf_limit->limit_perf_khz = cluster_perf_limit->max_perf_khz *
										perf_limit_percent / FULL_OF_PERCENT;
#ifdef CONFIG_HW_VOTE
	limit_cluster_freq_by_vote_channel(cluster_perf_limit->limit_freq_hvdev,
				cluster_perf_limit->limit_perf_khz);
#endif
	cluster_perf_limit->limit_perf_percent = perf_limit_percent;
}

static void set_isp_freq(struct cluster_perf_limit_info *cluster_perf_limit,
						int perf_limit_percent)
{
	if (g_vbat_to_isp_handler != NULL) {
		g_vbat_to_isp_handler(perf_limit_percent);
		cluster_perf_limit->limit_perf_percent = perf_limit_percent;
	}
}

static void set_gpu_freq(struct cluster_perf_limit_info *cluster_perf_limit,
						int perf_limit_percent)
{
	int ret;

	ret = get_max_freq_by_cluster_name(&cluster_perf_limit->max_perf_khz,
									GPU_FREQ_NAME);
	if (ret || unlikely(!cluster_perf_limit->max_perf_khz)) {
		pr_err("[%s]: get_gpu_max_freq fail!\n", __func__);
		return;
	}
	cluster_perf_limit->limit_perf_khz = cluster_perf_limit->max_perf_khz *
										perf_limit_percent / FULL_OF_PERCENT;
#ifdef CONFIG_HW_VOTE
	limit_cluster_freq_by_vote_channel(cluster_perf_limit->limit_freq_hvdev,
				cluster_perf_limit->limit_perf_khz);
#endif
	cluster_perf_limit->limit_perf_percent = perf_limit_percent;
}

static void set_npu_freq(struct cluster_perf_limit_info *cluster_perf_limit,
						int perf_limit_percent)
{
	int ret;

	ret = get_max_freq_by_cluster_name(&cluster_perf_limit->max_perf_khz,
									NPU_FREQ_NAME);
	if (ret) {
		pr_err("[%s]: get_npu_max_freq fail!\n", __func__);
		return;
	}
	cluster_perf_limit->limit_perf_khz = cluster_perf_limit->max_perf_khz *
										perf_limit_percent / FULL_OF_PERCENT;
	vbat_drop_nputop_freq_uplimit(cluster_perf_limit->limit_perf_khz / PERMILLAGE);

	cluster_perf_limit->limit_perf_percent = perf_limit_percent;
}

static void hisi_vbat_drop_cluster_freq_set(struct hisi_vbat_drop_protect_dev *di,
				struct cluster_perf_limit_info *cluster_perf_limit, int perf_limit_percent)
{
	if (cluster_perf_limit == NULL) {
		pr_debug("[%s]: cluster_perf_limit is NULL!\n", __func__);
		return;
	}

	pr_err("[%s]limit cluster%d perf %d of percent\n", __func__,
			cluster_perf_limit->cluster_id, perf_limit_percent);

	switch (cluster_perf_limit->cluster_id) {
		case MIDDLE_CPU:
		case MIDDLE_CPU_PE:
		case BIG_CPU:
			set_cpu_freq(di, cluster_perf_limit, perf_limit_percent);
			break;
		case GPU_CPU:
			set_gpu_freq(cluster_perf_limit, perf_limit_percent);
			break;
		case NPU_CPU:
			set_npu_freq(cluster_perf_limit, perf_limit_percent);
			break;
		case ISP_CPU:
			set_isp_freq(cluster_perf_limit, perf_limit_percent);
			break;
		default:
			pr_err("[%s]limit cluster%d is invalid\n", __func__,
					cluster_perf_limit->cluster_id);
			break;
	}
}

static unsigned int pmic_get_vbat_drop_irq_status(struct hisi_vbat_drop_protect_dev *di)
{
	return HISI_VBAT_DROP_PMIC_REG_READ(di->vbat_drop_pmic_config.irq_addr) &
		di->vbat_drop_pmic_config.irq_mask;
}

static void pmic_vbat_drop_irq_clear(struct hisi_vbat_drop_protect_dev *di)
{
	HISI_VBAT_DROP_PMIC_REG_WRITE(
		di->vbat_drop_pmic_config.irq_addr,
		di->vbat_drop_pmic_config.irq_mask);
}

static void pmic_vbat_drop_irq_mask(struct hisi_vbat_drop_protect_dev *di)
{
	HISI_VBAT_DROP_PMIC_REG_WRITE(
		di->vbat_drop_pmic_config.irq_ctrl_addr,
		di->vbat_drop_pmic_config.irq_ctrl_mask);
}

static void pmic_vbat_drop_irq_unmask(struct hisi_vbat_drop_protect_dev *di)
{
	HISI_VBAT_DROP_PMIC_REG_WRITE(
		di->vbat_drop_pmic_config.irq_ctrl_addr,
		~di->vbat_drop_pmic_config.irq_ctrl_mask);
}

static void hisi_vbat_drop_dmd_report(struct hisi_vbat_drop_protect_dev *di, const char *tag)
{
#if defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM)
	int tmp_len, cluster_id;
	char buf[POWER_DSM_BUF_SIZE_0512] = { 0 };
	int vol = coul_drv_battery_voltage();
	int curr = -coul_drv_battery_current();
	int cap = coul_drv_battery_capacity();
	int temp = coul_drv_battery_temperature();
	char *brand = coul_drv_battery_brand();
	unsigned int irq_status = pmic_get_vbat_drop_irq_status(di);
	struct cluster_perf_limit_info *cluster_perf_limit = NULL;

	tmp_len = snprintf_s(buf, POWER_DSM_BUF_SIZE_0512, POWER_DSM_BUF_SIZE_0512 - 1,
		"[%s] brand:%s,voltage:%dmV,current:%dmA,capacity:%d,temperature:%d,chg_status:%d,irq_cnt:%d,irq_status:%d\n",
		tag, brand, vol, curr, cap, temp, di->charge_state, di->vbat_drop_count, irq_status);
	
	for (cluster_id = 0; cluster_id < ALL; cluster_id++) {
		cluster_perf_limit = di->cluster_perf_limit[cluster_id];
		if (!cluster_perf_limit)
			continue;
		tmp_len += snprintf_s(buf + tmp_len, POWER_DSM_BUF_SIZE_0512 - tmp_len,
			POWER_DSM_BUF_SIZE_0512 - tmp_len - 1, "core: %u, limit_pc: %u%%, limit_freq: %ukhz\n",
			cluster_perf_limit->cluster_id, cluster_perf_limit->limit_perf_percent,
			cluster_perf_limit->limit_perf_khz);
	}

	pr_info("[%s] %s\n", __func__, buf);
	power_dsm_report_dmd(POWER_DSM_BATTERY, ERROR_LOW_VOL_INT, buf);
#endif
}

static void cancel_clusters_freq_limit(struct hisi_vbat_drop_protect_dev *di)
{
	int cluster;
	struct cluster_perf_limit_info * cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = di->cluster_perf_limit[cluster];
		hisi_vbat_drop_cluster_freq_set(di, cluster_perf_limit, FULL_OF_PERCENT);
	}
}

static void active_limit_clusters_freq_by_batt_cap(struct hisi_vbat_drop_protect_dev *di)
{
	int cluster, perf_limit_percent;
	struct cluster_perf_limit_info * cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = di->cluster_perf_limit[cluster];
		perf_limit_percent = get_active_perf_limit_percent(
								di, cluster_perf_limit, di->batt_cap);
		hisi_vbat_drop_cluster_freq_set(di, cluster_perf_limit, perf_limit_percent);
	}
}

static void passive_limit_clusters_freq_by_batt_cap(struct hisi_vbat_drop_protect_dev *di)
{
	int cluster, perf_limit_percent;
	struct cluster_perf_limit_info * cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = di->cluster_perf_limit[cluster];
		perf_limit_percent = get_passive_perf_limit_percent(
									di, cluster_perf_limit, di->batt_cap);
		hisi_vbat_drop_cluster_freq_set(di, cluster_perf_limit, perf_limit_percent);
	}
}

static bool is_limit_freq_happend(struct hisi_vbat_drop_protect_dev *di) {
	int cluster;
	struct cluster_perf_limit_info * cluster_perf_limit = NULL;
	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = di->cluster_perf_limit[cluster];
		if (cluster_perf_limit && cluster_perf_limit->limit_perf_percent != FULL_OF_PERCENT)
			return true;
	}
	return false;
}

static void power_supply_changed_process_work(struct work_struct *work)
{
	int ret;
	static int last_batt_cap = FULL_OF_PERCENT;
	static int last_charge_state = POWER_SUPPLY_STATUS_UNKNOWN;
	struct hisi_vbat_drop_protect_dev *di =
		container_of(work, struct hisi_vbat_drop_protect_dev, power_supply_changed_work);

	mutex_lock(&di->lock);
	ret = get_power_supply_property(&di->batt_cap, POWER_SUPPLY_PROP_CAPACITY);
	ret += get_power_supply_property(&di->charge_state, POWER_SUPPLY_PROP_STATUS);
	if (ret != 0) {
		mutex_unlock(&di->lock);
		pr_err("get capacity from power_supply failed: %d\n", ret);
		return;
	}

	pr_err("%s charger state: %d\n", __func__, di->charge_state);
	if (di->charge_state == POWER_SUPPLY_STATUS_CHARGING &&
		last_charge_state != di->charge_state && g_notifier_enable) {
		if (is_limit_freq_happend(di))
			hisi_vbat_drop_dmd_report(di, VBAT_DROP_CHG);
		cancel_clusters_freq_limit(di);
	}

	pr_err("%s last soc = %d, curr_soc = %d\n", __func__, last_batt_cap, di->batt_cap);
	if (di->charge_state != POWER_SUPPLY_STATUS_CHARGING &&
		last_batt_cap != di->batt_cap && g_notifier_enable) {
		active_limit_clusters_freq_by_batt_cap(di);
		if (is_limit_freq_happend(di)) {
			hisi_vbat_drop_dmd_report(di, VBAT_DROP_SOC);
			di->vbat_drop_count = 0;
		}
	}

	last_batt_cap = di->batt_cap;
	last_charge_state = di->charge_state;

	if(pmic_get_vbat_drop_irq_status(di)) {
		pmic_vbat_drop_irq_clear(di);
		pr_err("[%s] vbat drop irq is missed and need to clear again\n", __func__);
		hisi_vbat_drop_dmd_report(di, VBAT_DROP_ERR);
	}
	mutex_unlock(&di->lock);

	return;
}

static int power_supply_callback(
	struct notifier_block *nb, unsigned long event, void *data)
{
	struct hisi_vbat_drop_protect_dev *di =
		container_of(nb, struct hisi_vbat_drop_protect_dev, power_supply_nf);

	queue_work(system_highpri_wq, &di->power_supply_changed_work);

	return NOTIFY_DONE;
}

static void hisi_vbat_drop_interrupt_work(struct work_struct *work)
{
	struct hisi_vbat_drop_protect_dev *di = container_of(work,
		struct hisi_vbat_drop_protect_dev, vbat_drop_irq_work.work);

	di->vbat_drop_count++;

	if (di->charge_state != POWER_SUPPLY_STATUS_CHARGING)
		passive_limit_clusters_freq_by_batt_cap(di);

	hisi_vbat_drop_dmd_report(di, VBAT_DROP_IRQ);

	/* clear interrupt status and cancel auto 2 div */
	pmic_vbat_drop_irq_mask(di);
	pmic_vbat_drop_irq_clear(di);
	enable_irq(di->vbat_drop_irq);
	pmic_vbat_drop_irq_unmask(di);

	pr_err("[%s] exit work, cancel auto 2 div!\n", __func__);

	__pm_relax(di->vbatt_check_lock);
}

static irqreturn_t hisi_vbat_drop_irq_handler(
	int irq, void *data)
{
	struct hisi_vbat_drop_protect_dev *di = (struct hisi_vbat_drop_protect_dev *)data;

	__pm_stay_awake(di->vbatt_check_lock);
	pr_err("[%s] enter vbat drop handle!\n", __func__);

	/* interrupt mask */
	disable_irq_nosync(di->vbat_drop_irq);

	/* delayed work: check battery voltage */
	queue_delayed_work(system_highpri_wq, &di->vbat_drop_irq_work,
		msecs_to_jiffies(di->delayed_work_time));
	return IRQ_HANDLED; /* work unlock */
}

static int hisi_vbat_drop_temp_para_parse_dts(struct hisi_vbat_drop_protect_dev *di,
										struct cluster_perf_limit_info *limit_info,
										struct device_node *np)
{
	const char *temp_info = NULL;
	int temp_para_info[2] = {0};
	int index, count_temp, ret;

	if (limit_info == NULL || np == NULL)
		return -1;

	count_temp = of_property_count_strings(np, "temprature_table");
	if (count_temp > 0 && !(count_temp % TEMP_RARA_SIZE)) {
		limit_info->temprature_soc_table_num = count_temp / TEMP_RARA_SIZE;
		for (index = 0; index < limit_info->temprature_soc_table_num; index++) {
			if (of_property_read_string_index(np, "temprature_table",
							index * TEMP_RARA_SIZE,
							&temp_info))
				return -ENODEV;
			ret = kstrtoint(temp_info, 10, &temp_para_info[0]);
			if (ret != 0) {
				pr_err("%s kstortoint is failed\n", __func__);
				return ret;
			}
			if (of_property_read_string_index(np, "temprature_table",
							index * TEMP_RARA_SIZE + 1,
							&temp_info))
				return -ENODEV;
			ret = kstrtoint(temp_info, 10, &temp_para_info[1]);
			if (ret != 0) {
				pr_err("%s kstortoint is failed\n", __func__);
				return ret;
			}

			limit_info->temprature_table[index].temp_l = temp_para_info[0];
			limit_info->temprature_table[index].temp_h = temp_para_info[1];
		}
	} else {
		pr_err("[%s]: temprature_table is invalid!\n", __func__);
        return -1;
	}
	return 0;
}

static int hisi_vbat_drop_limit_perf_parse_dts(struct hisi_vbat_drop_protect_dev *di,
										struct cluster_perf_limit_info *limit_info,
										struct device_node *np)
{
	u32 perf_max_info[3] = {0};
	u32 perf_step_info[4] = {0};
	int index, index1, count, count_step, ret;

	if (limit_info == NULL || np == NULL)
		return -1;

	ret = hisi_vbat_drop_temp_para_parse_dts(di, limit_info, np);
	if (ret) {
		pr_err("[%s]: temp_para_parse_dts is failed!\n", __func__);
        return ret;
	}

	count = of_property_count_elems_of_size(np, "active_perf_limit", sizeof(int));
	if (count > 0 && !(count % PERF_MAX_LIMIT_CNT) && !(count % limit_info->temprature_soc_table_num)) {
		limit_info->active_perf_limit_para_num = count / PERF_MAX_LIMIT_CNT;

		for (index = 0; index < limit_info->temprature_soc_table_num; index++) {
			limit_info->perf_active_limit[index] = devm_kzalloc(di->dev,
						limit_info->active_perf_limit_para_num / limit_info->temprature_soc_table_num *
						sizeof(struct active_perf_limit_para),
						GFP_KERNEL);
			if(!limit_info->perf_active_limit[index])
				return -ENOMEM;
	
			for (index1 = 0; index1 < limit_info->active_perf_limit_para_num / limit_info->temprature_soc_table_num; index1++) {
				if (of_property_read_u32_index(np, "active_perf_limit",
								(index * limit_info->active_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_MAX_LIMIT_CNT,
								&perf_max_info[0]))
					return -ENODEV;
				if (of_property_read_u32_index(np, "active_perf_limit",
								(index * limit_info->active_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_MAX_LIMIT_CNT + 1,
								&perf_max_info[1]))
					return -ENODEV;
				if (of_property_read_u32_index(np, "active_perf_limit",
								(index * limit_info->active_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_MAX_LIMIT_CNT + 2,
								&perf_max_info[2]))
					return -ENODEV;
				limit_info->perf_active_limit[index][index1].soc_l = perf_max_info[0];
				limit_info->perf_active_limit[index][index1].soc_h = perf_max_info[1];
				limit_info->perf_active_limit[index][index1].perf_max_limit = perf_max_info[2];
			}
		}
	} else {
		pr_err("[%s]: active_perf_limit is invalid!\n", __func__);
		return -1;
	}

	count_step = of_property_count_elems_of_size(np, "passive_perf_limit", sizeof(int));
	if (count_step > 0 && !(count_step % PERF_STEP_LIMIT_CNT) && !(count_step % limit_info->temprature_soc_table_num)) {
		limit_info->passive_perf_limit_para_num = count_step / PERF_STEP_LIMIT_CNT;
		for (index = 0; index < limit_info->temprature_soc_table_num; index++) {
			limit_info->perf_passive_limit[index] = devm_kzalloc(di->dev,
						limit_info->passive_perf_limit_para_num * sizeof(struct passive_perf_limit_para),
						GFP_KERNEL);
			if(!limit_info->perf_passive_limit[index])
				return -ENOMEM;
	
			for (index1 = 0; index1 < limit_info->passive_perf_limit_para_num / limit_info->temprature_soc_table_num; index1++) {
				if (of_property_read_u32_index(np, "passive_perf_limit",
								(index * limit_info->passive_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_STEP_LIMIT_CNT,
								&perf_step_info[0]))
					return -ENODEV;
				if (of_property_read_u32_index(np, "passive_perf_limit",
								(index * limit_info->passive_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_STEP_LIMIT_CNT + 1,
								&perf_step_info[1]))
					return -ENODEV;
				if (of_property_read_u32_index(np, "passive_perf_limit",
								(index * limit_info->passive_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_STEP_LIMIT_CNT + 2,
								&perf_step_info[2]))
					return -ENODEV;
				if (of_property_read_u32_index(np, "passive_perf_limit",
								(index * limit_info->passive_perf_limit_para_num / limit_info->temprature_soc_table_num + index1) *
								PERF_STEP_LIMIT_CNT + 3,
								&perf_step_info[3]))
					return -ENODEV;
				limit_info->perf_passive_limit[index][index1].soc_l = perf_step_info[0];
				limit_info->perf_passive_limit[index][index1].soc_h = perf_step_info[1];
				limit_info->perf_passive_limit[index][index1].step_val = perf_step_info[2];
				limit_info->perf_passive_limit[index][index1].perf_lowerest_limit = perf_step_info[3];
			}
		}
	} else {
		pr_err("[%s]: passive_perf_limit is invalid!\n", __func__);
		return -1;
	}
	return 0;
}

static int hisi_vbat_drop_parse_cluster_dts(struct hisi_vbat_drop_protect_dev *di)
{
	struct device_node *np = di->dev->of_node;
	struct device_node *child_np = NULL;
	struct cluster_perf_limit_info *perf_limit_info = NULL;
	int ret, cluster_id;

	for_each_available_child_of_node(np, child_np) {
		perf_limit_info = devm_kzalloc(di->dev, sizeof(struct cluster_perf_limit_info),
									GFP_KERNEL);
		if(!perf_limit_info)
			return -ENOMEM;

		if (of_property_read_u32(child_np, "vbat_drop_core_channel", &cluster_id)) {
			dev_err(di->dev, "[%s]: get vbat_drop_core_channel fail!\n", __func__);
			return -1;
		}

		if (cluster_id >= ALL)
			dev_err(di->dev, "[%s]: vbat_drop_core_channel %d is invalid!\n",
					__func__, cluster_id);

		if (of_property_read_u32(
		    child_np, "active_perf_limit_en", &perf_limit_info->active_perf_limit_en)) {
			dev_err(di->dev, "[%s]: get active_perf_limit_en fail!\n", __func__);
		}

		if (of_property_read_u32(
		    child_np, "passvie_perf_limit_en", &perf_limit_info->passvie_perf_limit_en)) {
			dev_err(di->dev, "[%s]: get passvie_perf_limit_en fail!\n", __func__);
		}

		ret = hisi_vbat_drop_limit_perf_parse_dts(di, perf_limit_info, child_np);
		if (ret) {
			dev_err(di->dev, "[%s]: vbat_drop_limit_perf_parse fail!\n", __func__);
			return -1;
		}

		perf_limit_info->cluster_id = cluster_id;
		perf_limit_info->limit_perf_percent = FULL_OF_PERCENT;
#ifdef CONFIG_HW_VOTE
		perf_limit_info->limit_freq_hvdev = cluster_freq_register(di, child_np, "limit_channel");
#endif

		di->cluster_perf_limit[cluster_id] = perf_limit_info;
	}

	return 0;
}

static int hisi_vbat_drop_parse_pmic_dts(struct hisi_vbat_drop_protect_dev *di)
{
	struct device_node *np = di->dev->of_node;
	struct vbat_drop_pmic_config_info *pmic_config = &di->vbat_drop_pmic_config;
	unsigned int reg_info[2] = {0};

	if (of_property_read_u32_array(np, "vbat-drop-irq-addr", reg_info, 2)) {
		dev_err(di->dev, "[%s]: get vbat-drop-irq-addr fail!\n", __func__);
		return -1;
	}
	pmic_config->irq_addr = reg_info[0];
	pmic_config->irq_mask = reg_info[1];

	if (of_property_read_u32_array(np, "vbat-drop-irq-mask-addr", reg_info, 2)) {
		dev_err(di->dev, "[%s]: get vbat-drop-irq-mask-addr fail!\n", __func__);
		return -1;
	}
	pmic_config->irq_ctrl_addr = reg_info[0];
	pmic_config->irq_ctrl_mask = reg_info[1];

	if (of_property_read_u32_array(np, "vbat-drop-vset", reg_info, 2)) {
		dev_err(di->dev, "[%s]: get vbat-drop-vset fail!\n", __func__);
		return -1;
	}
	pmic_config->vset_addr = reg_info[0];
	pmic_config->vset_mask = reg_info[1];

	if (of_property_read_u32(np, "vbat-drop-voltages", &pmic_config->vset_num)) {
		dev_err(di->dev, "[%s]: get vbat-drop-voltages fail!\n", __func__);
		return -1;
	}
	
	pmic_config->vset_table = devm_kzalloc(di->dev, pmic_config->vset_num * sizeof(int),
										GFP_KERNEL);
	if (!pmic_config->vset_table)
		return -ENOMEM;

	if (of_property_read_u32_array(
		    np, "vbat-drop-vset-table", pmic_config->vset_table, pmic_config->vset_num)) {
		dev_err(di->dev, "[%s]: get vbat-drop-vset-table fail!\n", __func__);
		return -1;
	}

	return 0;
}

static int hisi_vbat_drop_parse_dts(struct hisi_vbat_drop_protect_dev *di)
{
	struct device_node *np = di->dev->of_node;
	int ret;

	/* get vbat drop vol mv */
	if (of_property_read_u32(np, "vbat_drop_vol_mv", &di->vbat_drop_vol_mv)) {
		di->vbat_drop_vol_mv = VBAT_DROP_VOL_DEFAULT;
		dev_err(di->dev, "[%s]: get vbat drop fail!\n", __func__);
	}
	dev_err(di->dev, "[%s]: vbat_drop_vol_mv = %u mv!\n", __func__,
		di->vbat_drop_vol_mv);

	/* get vbat drop delayed work time ms */
	if (of_property_read_u32(np, "delayed-work-time", &di->delayed_work_time)) {
		di->delayed_work_time = DELAYED_WORK_TIME_DEFAULT;
		dev_err(di->dev, "[%s]: get vbat drop work time fail!\n", __func__);
	}
	dev_err(di->dev, "[%s]: delayed_work_time = %u ms!\n", __func__,
		di->delayed_work_time);

	ret = hisi_vbat_drop_parse_pmic_dts(di);
	if (ret) {
		dev_err(di->dev, "[%s]: vbat_drop_config_parse_dts fail\n", __func__);
		return -1;
	}

	ret = hisi_vbat_drop_parse_cluster_dts(di);
	if (ret) {
		dev_err(di->dev, "[%s]: cluster_limit_perf_parse_dts fail\n", __func__);
		return -1;
	}

	return 0;
}

static void hisi_vbat_drop_vol_set(struct hisi_vbat_drop_protect_dev *di, int mv)
{
	int index;
	unsigned int reg_val = 0;
	struct vbat_drop_pmic_config_info *config = &di->vbat_drop_pmic_config;

	for (index = 0; index < config->vset_num; index++) {
		if (mv <= config->vset_table[index]) {
			reg_val = index;
			break;
		}
		reg_val = index;
	}

	HISI_VBAT_DROP_PMIC_REG_WRITE(di->vbat_drop_pmic_config.vset_addr, reg_val);
	pr_info("[%s]:set vol [%d]mv reg %u!\n", __func__, mv, reg_val);
}

static int hisi_vbat_drop_protect_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct hisi_vbat_drop_protect_dev *di = NULL;
	int ret;

	di = devm_kzalloc(dev, sizeof(*di), GFP_KERNEL);
	if (di == NULL)
		return -ENOMEM;

	di->dev = dev;
	platform_set_drvdata(pdev, di);

	/* get dts info */
	ret = hisi_vbat_drop_parse_dts(di);
	if (ret)
		goto vbat_drop_err;

	/* mask && clear old IRQ status */
	pmic_vbat_drop_irq_mask(di);
	pmic_vbat_drop_irq_clear(di);

	/* request vbat drop vol irq */
	di->vbat_drop_irq = platform_get_irq(pdev, 0);
	if (di->vbat_drop_irq < 0) {
		dev_err(dev, "IRQ resource %d is not available\n",
			di->vbat_drop_irq);
		ret = -ENOMEM;
		goto vbat_drop_err;
	}

	ret = devm_request_irq(dev, di->vbat_drop_irq,
		hisi_vbat_drop_irq_handler, IRQF_TRIGGER_RISING, "vbat_drop", di);
	if (ret) {
		dev_err(dev, "could not claim vbat drop irq with err:%d\n", ret);
		ret = -1;
		goto vbat_drop_err;
	}
	/* init vbat drop work */
	INIT_DELAYED_WORK(&di->vbat_drop_irq_work, hisi_vbat_drop_interrupt_work);
	INIT_WORK(&di->power_supply_changed_work, power_supply_changed_process_work);

	mutex_init(&di->lock);
	di->vbatt_check_lock = wakeup_source_register(dev, "vbatt_drop_check_wake");

	di->power_supply_nf.notifier_call = power_supply_callback;
	power_supply_reg_notifier(&di->power_supply_nf);

	/* set vbat drop vol */
	hisi_vbat_drop_vol_set(di, di->vbat_drop_vol_mv);

	/* unmask pmu vsys_drop interrupt */
	pmic_vbat_drop_irq_unmask(di);
	
	g_di = di;
	dev_err(dev, "[%s] probe success!\n", __func__);
	return 0;

vbat_drop_err:
	platform_set_drvdata(pdev, NULL);
	dev_err(dev, "[%s] probe fail!\n", __func__);
	return ret;
}

static const struct of_device_id hisi_vbat_drop_protect_of_match[] = {
	{
		.compatible = "hisilicon,vbat-drop-protect-v2",
	},
	{},
};

MODULE_DEVICE_TABLE(of, hisi_vbat_drop_protect_of_match);

static struct platform_driver hisi_vbat_drop_protect_v2_driver = {
	.probe  = hisi_vbat_drop_protect_probe,
	.driver = {
			.owner = THIS_MODULE,
			.name = "hisi_vbat_drop_prot_v2",
			.of_match_table = hisi_vbat_drop_protect_of_match,
		},
};

module_platform_driver(hisi_vbat_drop_protect_v2_driver);

MODULE_DESCRIPTION("Vbat drop protect driver v2.0");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");

#ifdef CONFIG_DFX_DEBUG_FS
void get_all_cluster_max_freq(void)
{
	int cluster;
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = g_di->cluster_perf_limit[cluster];
		if (cluster_perf_limit == NULL)
			continue;
		pr_err("%s: cluster[%d]= %luKhz\n", __func__, cluster,
			cluster_perf_limit->max_perf_khz);
	}
}
 
void set_cluster_max_freq(int cluster_id, int freq)
{
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	if (cluster_id >= ALL)
		return;

	cluster_perf_limit = g_di->cluster_perf_limit[cluster_id];
	if (cluster_perf_limit == NULL)
		return;
	cluster_perf_limit->max_perf_khz = freq;
}

void get_all_cluster_active_limit_en(void)
{
	int cluster;
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = g_di->cluster_perf_limit[cluster];
		if (cluster_perf_limit == NULL)
			continue;
		pr_err("%s: cluster[%d]= %d\n", __func__, cluster,
			cluster_perf_limit->active_perf_limit_en);
	}
}
 
void set_cluster_active_limit_en(int cluster_id, int en)
{
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	if (cluster_id >= ALL)
		return;

	cluster_perf_limit = g_di->cluster_perf_limit[cluster_id];
	if (cluster_perf_limit == NULL)
		return;
	cluster_perf_limit->active_perf_limit_en = en;
}

void get_all_cluster_passive_limit_en(void)
{
	int cluster;
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = g_di->cluster_perf_limit[cluster];
		if (cluster_perf_limit == NULL)
			continue;
		pr_err("%s: cluster[%d]= %d\n", __func__, cluster,
				cluster_perf_limit->passvie_perf_limit_en);
	}
}
 
void set_cluster_passive_limit_en(int cluster_id, int en)
{
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	if (cluster_id >= ALL)
		return;

	cluster_perf_limit = g_di->cluster_perf_limit[cluster_id];
	if (cluster_perf_limit == NULL)
		return;

	cluster_perf_limit->passvie_perf_limit_en = en;
}
 
void set_active_limit_freq_para(int cluster_id, int temp_level, int index,
					int soc_l, int soc_h, int percent)
{  
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	if (cluster_id >= ALL)
		return;

	cluster_perf_limit = g_di->cluster_perf_limit[cluster_id];
	if (cluster_perf_limit == NULL ||
		temp_level >= cluster_perf_limit->temprature_soc_table_num ||
		index >= cluster_perf_limit->active_perf_limit_para_num)
		return;

	cluster_perf_limit->perf_active_limit[temp_level][index].soc_l = soc_l;
	cluster_perf_limit->perf_active_limit[temp_level][index].soc_h = soc_h;
	cluster_perf_limit->perf_active_limit[temp_level][index].perf_max_limit = percent;
}

void get_all_active_limit_freq_para(void)
{  
	int cluster, index, temp_level, limit_para_count;
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	for (cluster = 0; cluster < ALL; cluster++) {
		cluster_perf_limit = g_di->cluster_perf_limit[cluster];
		if (cluster_perf_limit == NULL)
			continue;
		for (temp_level = 0; temp_level < cluster_perf_limit->temprature_soc_table_num; temp_level++) {
			limit_para_count = cluster_perf_limit->active_perf_limit_para_num / cluster_perf_limit->temprature_soc_table_num;
			for (index = 0; index < limit_para_count; index++) {
				pr_err("%s: cluster%d, level%d:temp %d-%d, level%d: soc %d-%d, percent:%d\n", __func__, cluster, temp_level,
						cluster_perf_limit->temprature_table[temp_level].temp_l,
						cluster_perf_limit->temprature_table[temp_level].temp_h,
						index,
						cluster_perf_limit->perf_active_limit[temp_level][index].soc_l,
						cluster_perf_limit->perf_active_limit[temp_level][index].soc_h,
						cluster_perf_limit->perf_active_limit[temp_level][index].perf_max_limit);
			}
		}
	}
}

void set_passive_limit_freq_para(int cluster_id, int temp_level, int index, int soc_l,
						int soc_h, int step_val, int percent)
{  
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	if (cluster_id >= ALL)
		return;

	cluster_perf_limit = g_di->cluster_perf_limit[cluster_id];
	if (cluster_perf_limit == NULL ||
		temp_level >= cluster_perf_limit->temprature_soc_table_num ||
		index >= cluster_perf_limit->passive_perf_limit_para_num)
		return;

	cluster_perf_limit->perf_passive_limit[temp_level][index].soc_l = soc_l;
	cluster_perf_limit->perf_passive_limit[temp_level][index].soc_h = soc_h;
	cluster_perf_limit->perf_passive_limit[temp_level][index].step_val = step_val;
	cluster_perf_limit->perf_passive_limit[temp_level][index].perf_lowerest_limit = percent;
}

void get_all_passvie_limit_freq_para(void)
{  
	int id, index, temp_level, limit_para_count;
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	for (id = 0; id < ALL; id++) {
		cluster_perf_limit = g_di->cluster_perf_limit[id];
		if (cluster_perf_limit == NULL)
			continue;
		for (temp_level = 0; temp_level < cluster_perf_limit->temprature_soc_table_num; temp_level++) {
			limit_para_count = cluster_perf_limit->active_perf_limit_para_num / cluster_perf_limit->temprature_soc_table_num;
			for (index = 0; index < limit_para_count; index++)
				pr_err("%s: cluster%d, level%d:temp %d-%d, level%d: soc %d-%d, step_val:%d, percent:%d\n",
				__func__, id, temp_level,
				cluster_perf_limit->temprature_table[temp_level].temp_l,
				cluster_perf_limit->temprature_table[temp_level].temp_h,
				index,
				cluster_perf_limit->perf_passive_limit[temp_level][index].soc_l,
				cluster_perf_limit->perf_passive_limit[temp_level][index].soc_h,
				cluster_perf_limit->perf_passive_limit[temp_level][index].step_val,
				cluster_perf_limit->perf_passive_limit[temp_level][index].perf_lowerest_limit);
		}
	}
}

void set_vbat_drop_mv(int val) {
	g_di->vbat_drop_vol_mv = val;
}

void get_vbat_drop_mv(void) {
	pr_err("vbat_drop_vol_mv:%d\n", g_di->vbat_drop_vol_mv);
}

void set_delayed_work_time(int val_ms) {
	g_di->delayed_work_time = val_ms;
}

void get_delayed_work_time(void) {
	pr_err("delayed_work_time:%dms\n", g_di->delayed_work_time);
}

irqreturn_t test_hisi_vbat_drop_irq_handler(void)
{
	struct hisi_vbat_drop_protect_dev *di = g_di;
 
	__pm_stay_awake(di->vbatt_check_lock);
	pr_err("[%s] enter vbat handle!\n", __func__);
 
	/* interrupt mask */
	disable_irq_nosync(di->vbat_drop_irq);

	/* delayed work: check battery voltage */
	queue_delayed_work(system_highpri_wq, &di->vbat_drop_irq_work,
		msecs_to_jiffies(di->delayed_work_time));
	return IRQ_HANDLED; /* work unlock */
}

void test_hisi_vbat_drop_cluster_freq_set(int cluster, int perf_limit_percent)
{
	struct cluster_perf_limit_info  *cluster_perf_limit = NULL;

	if (cluster >= ALL)
        return;

	cluster_perf_limit = g_di->cluster_perf_limit[cluster];
	if (cluster_perf_limit == NULL) {
		pr_err("[%s]: cluster_perf_limit is NULL!\n", __func__);
		return;
	}

	pr_err("[%s]limit cluster%d perf %d of percent\n", __func__,
			cluster_perf_limit->cluster_id, perf_limit_percent);

	switch (cluster_perf_limit->cluster_id) {
		case MIDDLE_CPU:
		case MIDDLE_CPU_PE:
		case BIG_CPU:
			set_cpu_freq(g_di, cluster_perf_limit, perf_limit_percent);
			break;
		case GPU_CPU:
			set_gpu_freq(cluster_perf_limit, perf_limit_percent);
			break;
		case NPU_CPU:
			set_npu_freq(cluster_perf_limit, perf_limit_percent);
			break;
		case ISP_CPU:
			set_isp_freq(cluster_perf_limit, perf_limit_percent);
			break;
		default:
			pr_err("[%s]limit cluster%d is invalid\n", __func__,
					cluster_perf_limit->cluster_id);
			break;
	}
}

void test_hisi_vbat_drop_vol_set(int mv)
{
	hisi_vbat_drop_vol_set(g_di, mv);

	pr_info("[%s]:set vol [%d]mv!\n", __func__, mv);
}

void enable_charger_notifier(void)
{
	g_notifier_enable = 1;
}

void disable_charger_notifier(void)
{
	g_notifier_enable = 0;
}
#endif
