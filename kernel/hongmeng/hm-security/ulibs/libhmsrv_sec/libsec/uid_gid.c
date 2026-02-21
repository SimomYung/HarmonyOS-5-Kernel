/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: uid & gid convertion implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 12 14:35:21 2023
 */

#include <libsec/uid_gid.h>
#include <libsec/cred.h>

#ifdef CONFIG_USER_NS

static struct ugid_item *map_id_up_base(unsigned int nr, struct ugid_map *map, uint32_t id)
{
	unsigned int idx;
	uint32_t first, last;
	struct ugid_item *item = NULL;

	/* Find the matching items */
	for (idx = 0; idx < nr; idx++) {
		first = map->items[idx].global_first;
		last = first + map->items[idx].count - 1;
		if (id >= first && id <= last) {
			item = &map->items[idx];
			break;
		}
	}
	return item;
}

static uint32_t map_id_up(struct ugid_map *map, uint32_t id)
{
	struct ugid_item *item;
	unsigned int nr = map->nr;

	item = map_id_up_base(nr, map, id);
	/* Map the id or note failure */
	if (item != NULL) {
		id = (id - item->global_first) + item->inner_first;
	} else {
		id = (uint32_t)-1;
	}

	return id;
}

static struct ugid_item *map_id_range_down_base(struct ugid_map *map, uint32_t id, uint32_t count)
{
	uint32_t idx;
	uint32_t inner_first, inner_last;
	uint32_t nr = map->nr;
	struct ugid_item *item = NULL;

	for (idx = 0; idx < nr; idx++) {
		inner_first = map->items[idx].inner_first;
		inner_last = map->items[idx].inner_first + map->items[idx].count - 1;
		if (id >= inner_first && id <= inner_last &&
		    (id + count - 1) >= inner_first && (id + count - 1) <= inner_last) {
			item = &map->items[idx];
			break;
		}
	}

	return item;
}

/* From id in user namespace to id in kernel */
uint32_t map_id_range_down(struct ugid_map *map, uint32_t id, uint32_t count)
{
	struct ugid_item *item = NULL;

	if (map != NULL) {
		item = map_id_range_down_base(map, id, count);
		if (item != NULL) {
			id = (id - item->inner_first) + item->global_first;
		} else {
			id = UINT32_MAX;
		}
	} else {
		id = UINT32_MAX;
	}

	return id;
}

static uint32_t map_id_down(struct ugid_map *map, uint32_t id)
{
	return map_id_range_down(map, id, 1);
}

uid_t convert_to_inner_uid(struct usr_ns_cred *usrns_cred, uid_t global_uid)
{
	/* Map the uid from a global kernel uid */
	return map_id_up(&usrns_cred->uid_map, (uint32_t)(global_uid));
}

gid_t convert_to_inner_gid(struct usr_ns_cred *usrns_cred, gid_t global_gid)
{
	/* Map the gid from a global kernel gid */
	return map_id_up(&usrns_cred->gid_map, (uint32_t)(global_gid));
}

uid_t convert_to_inner_uid_munged(struct usr_ns_cred *usrns_cred, uid_t global_uid)
{
	/* Map the uid from a global kernel uid and convert to overflowuid if abnormal */
	uid_t uid = convert_to_inner_uid(usrns_cred, global_uid);
	if (uid == (uid_t)-1) {
		uid = *hm_sec_shconf_kernel_overflowuid();
	}
	return uid;
}

gid_t convert_to_inner_gid_munged(struct usr_ns_cred *usrns_cred, uid_t global_gid)
{
	/* Map the gid from a global kernel gid and convert to overflowgid if abnormal */
	gid_t gid = convert_to_inner_gid(usrns_cred, global_gid);
	if (gid == (gid_t)-1) {
		gid = *hm_sec_shconf_kernel_overflowgid();
	}
	return gid;
}

uid_t convert_to_global_uid(struct usr_ns_cred *usrns_cred, uid_t inner_uid)
{
	/* Map the global kernel uid to the uid in user namespace */
	return map_id_down(&usrns_cred->uid_map, inner_uid);
}

gid_t convert_to_global_gid(struct usr_ns_cred *usrns_cred, gid_t inner_gid)
{
	/* Map the global kernel gid to the gid in user namespace */
	return map_id_down(&usrns_cred->gid_map, inner_gid);
}

#endif
