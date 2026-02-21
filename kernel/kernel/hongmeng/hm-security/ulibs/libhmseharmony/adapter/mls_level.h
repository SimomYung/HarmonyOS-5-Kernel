/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Hmseharmony header file for mls adapter service
 * Author: Huawei OS Kernel Lab
 * Create: Sunday April 7 15:21:00 2024
 */

#ifndef HMSEL_ADAPTER_MLSLEVEL_H
#define HMSEL_ADAPTER_MLSLEVEL_H
#include <sepol/policydb/policydb.h>
#include <libhmseharmony/services.h>

static inline size_t hmsel_level_size(const struct hmmls_level *level)
{
	return sizeof(struct hmmls_level) + level->cat_num * sizeof(struct hm_catmap_node) - sizeof(struct hm_catmap_node);
}

static inline struct hmmls_level *get_high_level(const struct hmmls_level *level)
{
	return (struct hmmls_level *)(((uintptr_t)level) + hmsel_level_size(level));
}

static inline size_t hmsel_context_size(struct hmsel_context *ctx)
{
	if (ctx->len != 0) {
	    return sizeof(struct hmsel_context);
	}
	return sizeof(struct hmsel_context) - sizeof(struct hmmls_level) + hmsel_level_size(ctx->levels) +
		hmsel_level_size(get_high_level(ctx->levels));
}
#endif
