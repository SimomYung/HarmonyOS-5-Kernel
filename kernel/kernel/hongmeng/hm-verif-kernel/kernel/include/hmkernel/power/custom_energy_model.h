/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Export header file of custom energy model.
 * Create: Fri Feb 21 17:12:12 2025
 */

#ifndef CUSTOM_ENERGY_MODEL_H
#define CUSTOM_ENERGY_MODEL_H

#include <lib/hook/hm_lite_vendor_hooks.h>
#include <hmkernel/devicetree.h>
#include <hmkernel/power/energy_model.h>

struct em_cluster_s;
struct em_cpu_s;

struct em_cluster_custom_energy_model_s {
	unsigned int type;
	void *data;
};

DECLARE_HM_HCK_LITE_HOOK(em_custom_init,
	TP_PROTO(struct em_cluster_s *cem, unsigned int nr_cluster, struct dtb_node_s *dt_node),
	TP_ARGS(cem, nr_cluster, dt_node));

DECLARE_HM_HCK_LITE_HOOK(em_custom_update_cap,
	TP_PROTO(unsigned int cpu_id),
	TP_ARGS(cpu_id));

DECLARE_HM_HCK_LITE_HOOK(em_custom_cluster_cap_of,
	TP_PROTO(unsigned int cluster, unsigned int freq, unsigned int *cap),
	TP_ARGS(cluster, freq, cap));

DECLARE_HM_HCK_LITE_HOOK(em_custom_cpu_avail_cap_of,
	TP_PROTO(unsigned int cpu, unsigned int cap, unsigned int *avail_cap),
	TP_ARGS(cpu, cap, avail_cap));

#endif
