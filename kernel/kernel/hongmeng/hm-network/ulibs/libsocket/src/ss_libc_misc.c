/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation for getifaddrs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 10:51:24 2019
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <libmem/utils.h>
#include <libsysif_net/api.h>

#include "hm_ifaddrs.h"
#include "ss_shmem.h"
#include "hm_socket.h"
#include "ss_channel.h"

static void hm_net_freeifaddrs(struct ifaddrs *ifp)
{
	struct ifaddrs *head = ifp;
	struct ifaddrs *next = NULL;
	while(head != NULL) {
		next = head->ifa_next;
		free(head);
		head = next;
	}
}

static struct ifaddrs* getifaddrs_from_storage(const struct ifaddrs_storage_internal *ifsp, int cnt)
{
	struct ifaddrs_storage *ifs = NULL;
	struct ifaddrs *ifapp = NULL;
	struct ifaddrs *ifap_last = NULL;
	int i;

	for (i = 0; i < cnt; i++) {
		ifs = calloc(1, sizeof(struct ifaddrs_storage));
		if (ifs == NULL) {
			hm_net_freeifaddrs(ifapp);
			ifapp = NULL;
			return ifapp;
		}

		ifs->storage = *ifsp;
		ifs->ifa.ifa_flags = ifs->storage.ifa_flags;
		ifs->ifa.ifa_name = ifs->storage.name;
		ifs->ifa.ifa_addr = &(ifs->storage.ifa_addr);
		ifs->ifa.ifa_netmask = &(ifs->storage.ifa_netmask);
		ifs->ifa.ifa_ifu.ifu_dstaddr = &(ifs->storage.ifa_ifu.ifu_dstaddr);
		ifs->ifa.ifa_data = &(ifs->storage.stats);
		if (ifapp == NULL) {
			ifapp = &ifs->ifa;
			ifap_last = &ifs->ifa;
		} else {
			ifap_last->ifa_next = &ifs->ifa;
			ifap_last = &ifs->ifa;
		}
		ifsp++;
	}

	return ifapp;
}

int hm_net_getifaddrs(struct ifaddrs **ifap)
{
	struct ifaddrs *ifapp = NULL;
	struct ss_shmem_info shm = {0};
	rref_t rref;
	int ret;

	if (ifap == NULL) {
		return E_HM_INVAL;
	}

	ret = ss_shmem_alloc(&shm);
	if (ret < 0) {
		hm_error("alloc shmem failed, ret=%d\n", ret);
		return E_HM_NOMEM;
	}

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_cfg_getifaddrs(rref,
						 shm.server_id);
	if (ret < 0) {
		hm_error("actvcapcall_netcall_cfg_getifaddrs failed, ret=%d\n", ret);
		goto out;
	}

	ifapp = getifaddrs_from_storage((struct ifaddrs_storage_internal *)shm.vaddr, ret);
	if (ifapp == NULL) {
		hm_error("getifaddrs from storage failed\n");
		ret = E_HM_NOMEM;
		goto out;
	}

	*ifap = ifapp;

out:
	ss_shmem_free(&shm);
	hm_net_put_rref(&rref);
	return ret > 0 ? 0 : ret;
}
