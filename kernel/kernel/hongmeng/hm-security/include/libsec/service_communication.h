/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: interfaces of security service communication
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 10:04:45 2023
 */
#ifndef ULIBS_LIBSEC_SERVICE_COMMUNICATION_H
#define ULIBS_LIBSEC_SERVICE_COMMUNICATION_H

#include <stddef.h>
#include <hmkernel/capability.h>
#include <hongmeng/types.h>

struct hm_secs_data_desc {
	uint64_t shm_key;
	uint64_t size;
	void *policydb_vaddr;
};

struct hm_secs_data {
	struct hm_secs_data_desc desc;
	int shm_desc;
	unsigned flags;
	void *addr;
	size_t size;
};

static inline void *hm_secs_data_addr(const struct hm_secs_data *data)
{
	return data->addr;
}

static inline size_t hm_secs_data_size(const struct hm_secs_data *data)
{
	return data->size;
}

static inline
struct hm_secs_data_desc hm_secs_data_desc(const struct hm_secs_data *data)
{
	return data->desc;
}

int hm_secs_data_make(struct hm_secs_data *data,
		      const void *buf, size_t size);
int hm_secs_data_sendto(struct hm_secs_data *data, xref_t ref);
int hm_secs_data_recvfrom(struct hm_secs_data *data,
			  const struct hm_secs_data_desc *desc,
			  uint32_t cidx);
void hm_secs_data_destroy(struct hm_secs_data *data);

#endif // !ULIBS_LIBSEC_SERVICE_COMMUNICATION_H
