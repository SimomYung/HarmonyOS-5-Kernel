// SPDX-License-Identifier: GPL-2.0
#include <linux/cpu.h>
#include <linux/platform_drivers/lpcpu_cpufreq_req.h>
#include <linux/pm_qos.h>
#include <linux/slab.h>

#include "dfc.h"

struct cpufreq_req_wrap {
	union {
		struct freq_qos_request *request;
		struct notifier_block nb;
	} data;
	int cpu;
	unsigned int freq;
};

struct ldk_cpufreq_policy {
	struct freq_constraints constraints;
	struct notifier_block nb_min;
	int cpu;
};

#define MAX_NR_CLUSTER	3
struct ldk_cpufreq_policy g_policy[MAX_NR_CLUSTER];

static int ldk_notify_freqmgr_min(struct notifier_block *nb, unsigned long action,
				  void *data)
{
	unsigned int freq;
	struct ldk_cpufreq_policy *policy = NULL;

	policy = container_of(nb, struct ldk_cpufreq_policy, nb_min);
	freq = freq_qos_read_value(&policy->constraints, FREQ_QOS_MIN);
	pm_set_min_cpufreq(policy->cpu, freq);

	return 0;
}

static void cpufreq_policy_init(unsigned int cluster_id)
{
	int ret;
	struct ldk_cpufreq_policy *policy = NULL;

	policy = &g_policy[cluster_id];
	if (policy->nb_min.notifier_call != NULL) {
		return;
	}
	freq_constraints_init(&policy->constraints);
	policy->nb_min.notifier_call = ldk_notify_freqmgr_min;
	ret = freq_qos_add_notifier(&policy->constraints, FREQ_QOS_MIN,
				    &policy->nb_min);
	if (ret != 0) {
		pr_err("cpufreq fail to register min qos notifier:%d\n", ret);
		policy->nb_min.notifier_call = NULL;
		return;
	}
}

void lpcpu_cpufreq_update_req(struct cpufreq_req *req, unsigned int freq)
{
	struct cpufreq_req_wrap *req_wrap = NULL;
	int ret;

	req_wrap = (struct cpufreq_req_wrap *)req;
	ret = freq_qos_update_request(req_wrap->data.request, freq);
	if (ret < 0)
		pr_err("cpufreq update req fail%d\n", ret);
}
EXPORT_SYMBOL(lpcpu_cpufreq_update_req);

int lpcpu_cpufreq_init_req(struct cpufreq_req *req, int cpu)
{
	int ret;
	unsigned int cluster_id;
	struct ldk_cpufreq_policy *policy = NULL;
	struct cpufreq_req_wrap *req_wrap = NULL;

	if (req == NULL || cpu < 0 || (unsigned int)cpu >= nr_cpu_ids) {
		pr_err("cpufreq init req invalid input\n");
		return -EINVAL;
	}
	cluster_id = topology_physical_package_id(cpu);
	if (cluster_id >= MAX_NR_CLUSTER) {
		pr_err("cluster id %u out of range\n", cluster_id);
		return -EINVAL;
	}
	cpufreq_policy_init(cluster_id);
	policy = &g_policy[cluster_id];
	req_wrap = (struct cpufreq_req_wrap *)req;
	req_wrap->data.request = (struct freq_qos_request *)
		kzalloc(sizeof(struct freq_qos_request), GFP_ATOMIC);
	if (IS_ERR_OR_NULL(req_wrap->data.request)) {
		pr_err("cpufreq init req alloc freq qos fail\n");
		return -EINVAL;
	}
	ret = freq_qos_add_request(&policy->constraints,
				   req_wrap->data.request, FREQ_QOS_MIN, 0);
	if (ret < 0) {
		pr_err("cpufreq_ldk: freq qos add req fail\n");
		kfree(req_wrap->data.request);
		return -ENOMEM;
	}
	req->cpu = cpu;
	/*
	 * record any cpu in the cluster is ok, just a symbol
	 * so that freqmgr can convert to the cluster id.
	 */
	policy->cpu = cpu;
	pr_err("cpufreq init req %u-%u succ\n", cluster_id, cpu);

	return 0;
}
EXPORT_SYMBOL(lpcpu_cpufreq_init_req);

void lpcpu_cpufreq_exit_req(struct cpufreq_req *req)
{
	struct cpufreq_req_wrap *req_wrap = NULL;

	if (IS_ERR_OR_NULL(req)) {
		return;
	}
	req_wrap = (struct cpufreq_req_wrap *)req;
	freq_qos_remove_request(req_wrap->data.request);
	kfree(req_wrap->data.request);
}
EXPORT_SYMBOL(lpcpu_cpufreq_exit_req);
