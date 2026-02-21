/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Energy track module for NPU
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 03 15:35:21 2024
 */

#include <linux/devfreq.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <chipset_common/dubai/dubai_plat.h>

#define DTSNODE_NPU_POWER_COMPATIBLE	"hongmeng,npupower"
#define DEVFREQ_NPU_POWER_COMPATIBLE	"lpm,npu-pm"
#define HZ_PER_KHZ			1000

static atomic_t g_npu_stats_enable = ATOMIC_INIT(0);
static DEFINE_MUTEX(npu_stats_lock);

struct npu_freq_data {
	unsigned int freq_num;
	unsigned int *npu_freq;
	unsigned int *npu_power; /* mA */
	unsigned long long *total_time; /* ms */
};

static bool g_initialized = false;
static struct devfreq *g_npu_devfreq = NULL;
static struct npu_freq_data *g_npu_track = NULL;

/* Provide API to get npu energy, unit: mA*S */
static int npu_energy_calc(uint64_t *energy)
{
	unsigned int i;
	uint64_t total_energy = 0; /* mAh */

	if (!g_initialized) {
		pr_warn("npu energy track module is not ready\n");
		return -EINVAL;
	}
	for(i = 0; i < g_npu_track->freq_num; i++) {
		total_energy += g_npu_track->total_time[i] *
				g_npu_track->npu_power[i];
	}
	*energy = total_energy / MSEC_PER_SEC;

	return 0;
}

static int npu_freq_level_of(unsigned long freq, unsigned int *level)
{
	unsigned int index;

	for (index = 0; index < g_npu_track->freq_num; index++) {
		if ((unsigned long)g_npu_track->npu_freq[index] * HZ_PER_KHZ == freq) {
			*level = index;
			return 0;
		}
	}

	return -EINVAL;
}

static int npu_freq_notifier_call(struct notifier_block *nb,
				  unsigned long event, void *data)
{
	unsigned int polling_ms, level;
	unsigned long last_freq;
	u64 running_time, cur_time, last_update_time, last_total_time;
	struct devfreq_freqs *freqs = NULL;

	if (!g_initialized || event != DEVFREQ_POSTCHANGE) {
		return NOTIFY_DONE;
	}
	/*
	 * The POSTCHANGE event is sent after the NPU frequency adjusted
	 * and before the time_in_state updated. Therefore, the obtained
	 * time_in_state does not add the running duration of the previous
	 * frequency. This factor is taken into account here.
	 */
	mutex_lock(&npu_stats_lock);
	cur_time = jiffies64_to_msecs(get_jiffies_64());
	polling_ms = g_npu_devfreq->profile->polling_ms;
	last_update_time = jiffies64_to_msecs(g_npu_devfreq->stats.last_update);
	freqs = (struct devfreq_freqs *)data;
	last_freq = freqs->old;
	running_time = (cur_time - last_update_time) > polling_ms ?
			polling_ms : (cur_time - last_update_time);
 
	if (npu_freq_level_of(last_freq, &level) != 0) {
		mutex_unlock(&npu_stats_lock);
		return NOTIFY_DONE;
	}
	last_total_time = jiffies64_to_msecs(g_npu_devfreq->stats.time_in_state[level]);
	g_npu_track->total_time[level] = last_total_time + running_time;
	mutex_unlock(&npu_stats_lock);

	return NOTIFY_OK;
}

static struct notifier_block npu_freq_notifier = {
	.notifier_call = npu_freq_notifier_call,
};

static void npu_devfreq_unregister_notifier(void)
{
	(void)devfreq_unregister_notifier(g_npu_devfreq, &npu_freq_notifier,
					  DEVFREQ_TRANSITION_NOTIFIER);
}

static int npu_devfreq_register_notifier(void)
{
	int err = 0;

	err = devfreq_register_notifier(g_npu_devfreq, &npu_freq_notifier,
					DEVFREQ_TRANSITION_NOTIFIER);
	if (err) {
		pr_err("devfreq notify register for npu fail, err=%d\n", err);
	}

	return err;
}

static void npu_devfreq_destroy(void)
{
	if (g_npu_devfreq != NULL) {
		kfree(g_npu_devfreq);
		g_npu_devfreq = NULL;
	}
}

static int npu_devfreq_parse(void)
{
	struct device_node *np = NULL;

	/* find npu devfreq node*/
	np = of_find_compatible_node(NULL, NULL, DEVFREQ_NPU_POWER_COMPATIBLE);
	if (np == NULL) {
		pr_warn("failed to find npufreq compitable node%s\n");
		return -ENODEV;
	}
	g_npu_devfreq = devfreq_get_devfreq_by_node(np);
	if (IS_ERR_OR_NULL(g_npu_devfreq)) {
		pr_err("failed to get devfreq device: %s\n", np->full_name);
		of_node_put(np);
		return PTR_ERR(g_npu_devfreq);
	}
	of_node_put(np);

	return 0;
}

#define ENERGY_NODE_PARSE_DEFINE(node)							\
int et_##node##_parse(struct device_node *np, const char *name, unsigned int num)	\
{											\
	int err;									\
	unsigned int *amplist_node = NULL;						\
											\
	amplist_node = kzalloc(sizeof(unsigned int) * num, GFP_KERNEL);			\
	if (amplist_node == NULL) {							\
		pr_err("npu "#node" memory allocation failed\n");			\
		return -ENOMEM;								\
	}										\
	err = of_property_read_u32_array(np, name, amplist_node, num);			\
	if (err != 0) {									\
		pr_err("failed to parse "#node" of npu\n");				\
		kfree(amplist_node);							\
		return -ENOENT;								\
	}										\
	g_npu_track->node = amplist_node;						\
											\
	return 0;									\
}

ENERGY_NODE_PARSE_DEFINE(npu_freq);
ENERGY_NODE_PARSE_DEFINE(npu_power);

static void npu_energy_info_destroy(void)
{
	if (g_npu_track->total_time != NULL) {
		kfree(g_npu_track->total_time);
		g_npu_track->total_time = NULL;
	}
	if (g_npu_track->npu_power != NULL) {
		kfree(g_npu_track->npu_power);
		g_npu_track->npu_power = NULL;
	}
	if (g_npu_track->npu_freq != NULL) {
		kfree(g_npu_track->npu_freq);
		g_npu_track->npu_freq = NULL;
	}
}

static int npu_energy_info_parse(struct device_node *np)
{
	int err = 0;
	unsigned long long *time = NULL;

	err = of_property_read_u32(np, "nr_freq", &g_npu_track->freq_num);
	if (err != 0) {
		pr_err("failed to parse freq number of npu\n");
		return -ENOENT;
	}
	err = et_npu_freq_parse(np, "freq_list", g_npu_track->freq_num);
	if (err != 0) {
		pr_err("failed to parse freq list of npu\n");
		return err;
	}
	err = et_npu_power_parse(np, "amp_list", g_npu_track->freq_num);
	if (err != 0) {
		pr_err("failed to parse amp list of npu\n");
		npu_energy_info_destroy();
		return err;
	}
	/* store total time of each freq point of NPU */
	time = kzalloc(sizeof(unsigned long long) * g_npu_track->freq_num, GFP_KERNEL);
	if (time == NULL) {
		pr_err("memory allocation for npu time failed\n");
		npu_energy_info_destroy();
		return -ENOMEM;
	}
	g_npu_track->total_time = time;

	return 0;
}

static void npu_track_data_destroy(void)
{
	if (g_npu_track != NULL) {
		kfree(g_npu_track);
		g_npu_track = NULL;
	}
}

static void npu_freq_data_destroy(void)
{
	npu_energy_info_destroy();
	npu_track_data_destroy();
}

static int npu_freq_data_init(void)
{
	int err = 0;
	struct device_node *np = NULL;

	g_npu_track = kzalloc(sizeof(struct npu_freq_data), GFP_KERNEL);
	if (g_npu_track == NULL) {
		pr_err("no memory to create npu energy tracking\n");
		return -ENOMEM;
	}
	/* find compatible node*/
	np = of_find_compatible_node(NULL, NULL, DTSNODE_NPU_POWER_COMPATIBLE);
	if (np == NULL) {
		pr_err("failed to find device node of npu\n");
		npu_track_data_destroy();
		return -ENODEV;
	}
	err = npu_energy_info_parse(np);
	if (err != 0) {
		pr_err("failed to parse energy info of npu\n");
		npu_track_data_destroy();
		of_node_put(np);
		return err;
	}
	of_node_put(np);

	return 0;
}

static int dubai_enable_npu(bool enable)
{
	if (enable && !atomic_read(&g_npu_stats_enable)) {
		atomic_set(&g_npu_stats_enable, 1);
	} else if (!enable && atomic_read(&g_npu_stats_enable)) {
		atomic_set(&g_npu_stats_enable, 0);
	} else {
		pr_err("duplicate settings: NPU stats: %d, setting: %d\n",
			atomic_read(&g_npu_stats_enable), enable);
		return -EEXIST;
	}
	pr_info("NPU stats enable: %d\n", enable ? 1 : 0);

	return 0;
}

static int dubai_npu_freq_num_parse(void)
{
	if (!atomic_read(&g_npu_stats_enable)) {
		return -EPERM;
	}

	return g_npu_track->freq_num;
}

static int dubai_npu_info_parse(struct dubai_npu_freq_info *data, uint64_t *energy, int num)
{
	int err = 0;
	unsigned int i;

	if (!atomic_read(&g_npu_stats_enable)) {
		return -EPERM;
	}
	if (!data || (num != g_npu_track->freq_num)) {
		pr_err("invalid param to get npu info\n");
		return -EINVAL;
	}
	mutex_lock(&npu_stats_lock);
	for (i = 0; i < num; i++) {
		data[i].freq = g_npu_track->npu_freq[i];
		data[i].run_time = g_npu_track->total_time[i];
	}
	err = npu_energy_calc(energy);
	if (err < 0) {
		pr_err("failed to get npu energy\n");
		*energy = 0;
	}
	mutex_unlock(&npu_stats_lock);

	return 0;
}

static struct dubai_npu_stats_ops npu_ops = {
	.enable = dubai_enable_npu,
	.get_num = dubai_npu_freq_num_parse,
	.get_stats = dubai_npu_info_parse,
};

void dubai_npu_freq_stats_init(void)
{
	int err;

	err = npu_freq_data_init();
	if (err != 0) {
		pr_err("NPU freq data init failed: %d\n", err);
		return;
	}
	err = npu_devfreq_parse();
	if (err != 0) {
		pr_err("NPU devfreq parse failed: %d\n", err);
		npu_freq_data_destroy();
		return;
	}
	err = npu_devfreq_register_notifier();
	if (err != 0) {
		pr_err("NPU devfreq register notifier failed: %d\n", err);
		npu_devfreq_destroy();
		npu_freq_data_destroy();
		return;
	}
	err = dubai_register_module_ops(DUBAI_MODULE_NPU, &npu_ops);
	if (err < 0) {
		pr_err("failed to register dubai module ops of NPU\n");
		npu_devfreq_unregister_notifier();
		npu_devfreq_destroy();
		npu_freq_data_destroy();
		return;
	}
	g_initialized = true;
}

void dubai_npu_freq_stats_exit(void)
{
	dubai_unregister_module_ops(DUBAI_MODULE_NPU);
}
