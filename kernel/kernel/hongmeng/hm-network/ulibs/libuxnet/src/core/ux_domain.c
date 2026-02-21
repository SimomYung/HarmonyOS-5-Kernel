/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network domain
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <libhmsync/raw_mutex.h>

#include "ux_domain.h"
#include "ux_netlog.h"

static struct raw_mutex ux_domain_lock = RAW_MUTEX_INITIALIZER;
static ux_domain_t *ux_domains[MAX_NET_DOMAIN + 1] = { 0 };

int ux_domain_register(ux_domain_t *domain)
{
	int ret = 0;

	if (domain->family < 0 || domain->family > MAX_NET_DOMAIN) {
		return E_HM_INVAL;
	}

	raw_mutex_lock(&ux_domain_lock);
	if (!ux_domains[domain->family]) {
		ux_domains[domain->family] = domain;
	} else {
		ret = E_HM_OBJEXIST;
	}
	raw_mutex_unlock(&ux_domain_lock);

	net_info(HMNET_CORE, "registet family %d\n", domain->family);

	return ret;
}

void ux_domain_unregister(int family)
{
	if (family < 0 || family > MAX_NET_DOMAIN) {
		return;
	}

	raw_mutex_lock(&ux_domain_lock);
	ux_domains[family] = NULL;
	raw_mutex_unlock(&ux_domain_lock);
}

ux_domain_t *ux_domain_get(int family)
{
	if (family < 0 || family > MAX_NET_DOMAIN) {
		return NULL;
	}

	return ux_domains[family];
}
