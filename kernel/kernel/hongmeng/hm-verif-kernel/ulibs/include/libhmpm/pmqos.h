/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Declare interface of pmqos
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 17 17:15:10 CST 2022
 */

#ifndef ULIBS_INCLUDE_LIBHMPM_PMQOS_H
#define ULIBS_INCLUDE_LIBHMPM_PMQOS_H

#define PMQOS_DEFAULT_VALUE			(-1)
#define CPUDMA_LATENCY_DEFAULT_VALUE		(2000 * USEC_PER_SEC)
#define MEMORY_BANDWIDTH_DEFAULT_VALUE		0
#define MEMORY_LATENCY_DEFAULT_VALUE		0
#define MEMORY_THROUGHPUT_DEFAULT_VALUE		0
#define MEMORY_THROUGHPUT_UP_THRESHOLD_DEFAULT_VALUE	INT_MAX
#define L1BUS_LATENCY_DEFAULT_VALUE		0

enum hm_pmqos_type {
	PMQOS_TYPE_CPU_DMA_LATENCY,
	PMQOS_TYPE_MEMORY_BANDWIDTH,
	PMQOS_TYPE_MEMORY_LATENCY,
	PMQOS_TYPE_MEMORY_THROUGHPUT,
	PMQOS_TYPE_MEMORY_THROUGHPUT_UP_THRESHOLD,
	PMQOS_TYPE_L1BUS_LATENCY,
	PMQOS_TYPE_NR,
};

struct hm_pmqos_request {
	enum hm_pmqos_type type;
	int id;
};

int hm_pmqos_request_add(struct hm_pmqos_request *request, int value);
int hm_pmqos_request_remove(struct hm_pmqos_request *request);
int hm_pmqos_request_update(struct hm_pmqos_request *request, int value);

#endif /* ULIBS_INCLUDE_LIBHMPM_PMQOS_H */
