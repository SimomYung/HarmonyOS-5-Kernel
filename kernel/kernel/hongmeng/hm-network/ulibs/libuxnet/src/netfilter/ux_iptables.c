/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Iptables impl code
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 14:22:56 2021
 */
#include <stdio.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_rwlock.h>

#include "ux_socket.h"
#include "ux_netlog.h"
#include "ux_netdev.h"
#include "ux_iptables.h"
#include "ux_filter_table.h"
#include "ux_netfilter.h"
#include "ux_ipt_tcpudp.h"
#include "ux_ipt_icmp.h"
#include "ux_ipt_conntrack.h"
#include "ux_ipt_audit.h"
#include "ux_ipt_reject.h"

#define ERROR_ENTRY "ERROR"

struct dlist_node ipt_list;
struct dlist_node ipt_match_list;
struct dlist_node ipt_target_list;

#define ux_ipt_match_iterate(type, entry, fn, args...)				\
({										\
	int __ret = 0;								\
	unsigned int __i;							\
	struct ux_ipt_entry_match *__match;					\
	for (__i = sizeof(type); __i < (entry)->target_offset && __ret == 0;	\
	     __i += __match->u.match_size) {					\
		__match = (struct ux_ipt_entry_match *)((char *)(entry) + __i);	\
		__ret = fn(__match, ## args);					\
	}									\
	__ret;									\
})

#define ux_ipt_entry_iterate(type, entries, size, fn, args...)				\
({											\
	int __ret = E_HM_OK;								\
	unsigned int __i;								\
	type *entry;									\
	for (__i = 0; __i < size && __ret == E_HM_OK; __i += entry->next_offset) {	\
		entry = (type *)((char *)entries + __i);				\
		__ret = fn(entry, ## args);						\
	}										\
	__ret;										\
})

STATIC_DEFINE_RAW_RWLOCK(ipt_list_lock);
STATIC_DEFINE_RAW_RWLOCK(ipt_match_list_lock);
STATIC_DEFINE_RAW_RWLOCK(ipt_target_list_lock);

void ux_add_ipt_match(struct ux_ipt_match *match)
{
	if (match != NULL) {
		(void)raw_rwlock_wrlock(&ipt_match_list_lock);
		dlist_insert_head(&ipt_match_list, &match->list);
		(void)raw_rwlock_unlock(&ipt_match_list_lock);
	}
}

static struct ux_ipt_match *ux_ipt_find_match(const char *name,
					      unsigned char revision)
{
	struct ux_ipt_match *match;
	struct dlist_node *pos;

	(void)raw_rwlock_rdlock(&ipt_match_list_lock);
	dlist_for_each(pos, &ipt_match_list) {
		match = dlist_entry(pos, struct ux_ipt_match, list);
		if (strcmp(match->name, name) == 0) {
			if (match->revision == revision) {
				(void)raw_rwlock_unlock(&ipt_match_list_lock);
				return match;
			}
		}
	}
	(void)raw_rwlock_unlock(&ipt_match_list_lock);
	return NULL;
}

static struct ux_ipt_target *ux_ipt_find_target(const char *name,
						unsigned char revision)
{
	struct ux_ipt_target *target = NULL;
	struct dlist_node *pos = NULL;

	raw_rwlock_rdlock(&ipt_target_list_lock);
	dlist_for_each(pos, &ipt_target_list) {
		target = dlist_entry(pos, struct ux_ipt_target, list);
		if (strcmp(target->name, name) == 0) {
			if (target->revision == revision) {
				raw_rwlock_unlock(&ipt_target_list_lock);
				return target;
			}
		}
	}
	raw_rwlock_unlock(&ipt_target_list_lock);
	return NULL;
}

static int ux_ipt_replace_match(struct ux_ipt_entry_match *match,
				struct ux_ipt_match *k_match)
{
	match->u.kernel.match = k_match;

	return 0;
}

static inline struct ux_ipt_entry *ux_ipt_get_entry(void *base, unsigned int offset)
{
	return (struct ux_ipt_entry *)((char *)base + offset);
}

static inline struct ux_ipt_standard_target *ux_ipt_get_target(struct ux_ipt_entry *entry)
{
	return (struct ux_ipt_standard_target *)((char *)entry + entry->target_offset);
}

static int ux_ipt_check_match(const char *name,
			      const struct ux_ipt_ip *ip,
			      const struct ux_ipt_entry_match *match,
			      const struct ux_ipt_match *k_match)
{
	if (k_match->checkentry != NULL) {
		if (k_match->checkentry(name, ip, match->data,
					match->u.match_size - sizeof(*match)) != E_HM_OK) {
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

static int ux_ipt_check_target(const char *name,
			       const struct ux_ipt_ip *ip,
			       const struct ux_ipt_entry_target *target,
			       const struct ux_ipt_target *k_target)
{
	if (k_target->checkentry != NULL) {
		if (k_target->checkentry(name, ip, target->data,
					 target->u.target_size - sizeof(*target)) != E_HM_OK) {
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

static int ux_ipt_check_replace_match(struct ux_ipt_entry *entry,
				      const char *name)
{
	int ret = E_HM_OK;
	unsigned int i;
	struct ux_ipt_entry_match *match;
	struct ux_ipt_match *k_match;
	for (i = sizeof(struct ux_ipt_entry); i < entry->target_offset;
	     i += match->u.match_size) {
		match = (struct ux_ipt_entry_match *)((char *)entry + i);
		k_match = ux_ipt_find_match(match->u.user.name, match->u.user.revision);
		if (k_match == NULL) {
			net_error(HMNET_CORE, "ux_ipt_check_replace_match dose not has match name = %s, revision = %d\n",
				  match->u.user.name,
				  (int)match->u.user.revision);
			return E_HM_POSIX_NOENT;
		}

		ret = ux_ipt_check_match(name, &entry->ip, match, k_match);
		if (ret != E_HM_OK) {
			net_warn(HMNET_CORE, "ux_ipt_check_replace_match: check match failed\n");
			return E_HM_INVAL;
		}

		ret = ux_ipt_replace_match(match, k_match);
		if (ret != E_HM_OK) {
			return E_HM_INVAL;
		}
	}
	return E_HM_OK;
}

static int ux_ipt_check_replace_target(struct ux_ipt_entry *entry, const char *name)
{
	int ret;
	struct ux_ipt_entry_target *t;
	struct ux_ipt_target *kernel_target;

	t = (struct ux_ipt_entry_target *)ux_ipt_get_target(entry);
	if ((strcmp(t->u.user.name, "") != 0) && (strcmp(t->u.user.name, "ERROR") != 0)) {
		kernel_target = ux_ipt_find_target(t->u.user.name, t->u.user.revision);
		if (!kernel_target) {
			net_error(HMNET_CORE, "ux_ipt_replace_each_target dose not has target name = %s, revision = %d\n",
				  t->u.user.name,
				  (int)t->u.user.revision);
			return E_HM_POSIX_NOENT;
		}
		ret = ux_ipt_check_target(name, &entry->ip, t, kernel_target);
		if (ret != E_HM_OK) {
			net_warn(HMNET_CORE, "ux_ipt_check_replace_target: check target failed\n");
			return E_HM_INVAL;
		}

		t->u.kernel.target = kernel_target;
	}

	return E_HM_OK;
}

static int ux_ipt_do_match(struct ux_pktbuf *skb,
			   struct ux_netdev *indev,
			   struct ux_netdev *outdev,
			   struct ux_ipt_entry *entry)
{
	unsigned int ret = 0;
	unsigned int i = 0;
	char *indev_name;
	char *outdev_name;
	char nulldev_name[IF_NAMESIZE] __attribute__((aligned(sizeof(long)))) = {0};
	struct iphdr *ip;
	struct ux_ipt_ip ipinfo;

	if (skb->len < sizeof(struct iphdr)) {
		return 0;
	}

	ipinfo = entry->ip;
	ip = (struct iphdr *)skb->payload;
	indev_name = indev ? indev->name : nulldev_name;
	outdev_name = outdev ? outdev->name : nulldev_name;

	for (i = 0, ret = 0; i < IF_NAMESIZE / sizeof(unsigned long); i++) {
		ret |= (((const unsigned long *)indev_name)[i] ^
			((const unsigned long *)ipinfo.iniface)[i]) &
			((const unsigned long *)ipinfo.iniface_mask)[i];
	}
	if (fwinv(ret != 0, ipinfo.invflags, UX_IPT_INV_VIA_IN)) {
		return 0;
	}

	for (i = 0, ret = 0; i < IF_NAMESIZE / sizeof(unsigned long); i++) {
		ret |= (((const unsigned long *)outdev_name)[i] ^
			((const unsigned long *)ipinfo.outiface)[i]) &
			((const unsigned long *)ipinfo.outiface_mask)[i];
	}
	if (fwinv(ret != 0, ipinfo.invflags, UX_IPT_INV_VIA_OUT)) {
		return 0;
	}

	if (fwinv((ip->saddr & ipinfo.smsk.s_addr) != ipinfo.src.s_addr, ipinfo.invflags, UX_IPT_INV_SRCIP) ||
	    fwinv((ip->daddr & ipinfo.dmsk.s_addr) != ipinfo.dst.s_addr, ipinfo.invflags, UX_IPT_INV_DSTIP)) {
		return 0;
	}

	if (ipinfo.proto != 0 && fwinv(ip->protocol != ipinfo.proto, ipinfo.invflags, UX_IPT_INV_PROTO)) {
		return 0;
	}

	return 1;
}

static bool ux_ipt_do_extmatch(struct ux_ipt_entry_match *match,
			       struct ux_pktbuf *pskbuf,
			       const struct ux_netdev *in,
			       const struct ux_netdev *out)
{
	if (!match->u.kernel.match->match(pskbuf, in, out,
					  match->u.kernel.match,
					  match->data)) {
		return true;
	}
	return false;
}

static bool ux_ipt_match_ext(struct ux_pktbuf *pskbuf,
			     struct ux_netdev *indev,
			     struct ux_netdev *outdev,
			     struct ux_ipt_entry *entry)
{
	if (ux_ipt_match_iterate(struct ux_ipt_entry, entry,
				  ux_ipt_do_extmatch, pskbuf,
				  indev, outdev) == 0) {
		return true;
	}
	return false;
}

int ux_ipt_do_table(unsigned int hook,
			     struct ux_pktbuf *pskbuf,
			     struct ux_ipt_netdev netdev,
			     const char *name)
{
	struct ux_netdev *indev;
	struct ux_netdev *outdev;
	struct ux_ipt_table *table;
	struct ux_ipt_entry *entry;
	struct ux_ipt_standard_target *target;
	unsigned int verdict = 0;
	struct ux_ipt_entry_target *t;

	indev = netdev.indev;
	outdev = netdev.outdev;

	(void)raw_rwlock_rdlock(&ipt_list_lock);
	table = ipt_find_table_by_name(name);
	if (table == NULL) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_NOENT;
	}

	entry = ux_ipt_get_entry(table->entries, table->hook_entry[hook]);
	target = ux_ipt_get_target(entry);

	char *entry_base = (char *)table->entries;

	while ((unsigned int)((char *)entry - entry_base) <= table->underflow[hook]) {
		if (ux_ipt_do_match(pskbuf, indev, outdev, entry) != 0 &&
		    ux_ipt_match_ext(pskbuf, indev, outdev, entry)) {
			if (target->target.u.kernel.target != NULL && target->target.u.kernel.target->target != NULL) {
				t = (struct ux_ipt_entry_target *)((char *)entry + entry->target_offset);
				verdict = target->target.u.kernel.target->target(pskbuf, indev, outdev,
										 t->u.kernel.target, t->data);
			} else {
				verdict = (unsigned int)((-target->verdict) - 1);
			}
			if (verdict != UX_IPT_CONTINUE) {
				break;
			}
		}
		/* When we has not config any rule in iptables, the default Rule need return with verdict way */
		if ((unsigned int)((char *)entry - entry_base) == table->underflow[hook]) {
			verdict = (unsigned int)((-target->verdict) - 1);
			break;
		}
		entry = (struct ux_ipt_entry *)((char *)entry + entry->next_offset);
		target = ux_ipt_get_target(entry);
	}
	raw_rwlock_unlock(&ipt_list_lock);

	return (int)verdict;
}

struct ux_ipt_table *ipt_find_table_by_name(const char *name)
{
	struct ux_ipt_table *table;
	struct dlist_node *pos;

	dlist_for_each(pos, &ipt_list) {
		table = dlist_entry(pos, struct ux_ipt_table, list);
		if (strcmp(table->name, name) == 0) {
			return table;
		}
	}

	return NULL;
}

static void ipt_clean_entries(struct ux_ipt_standard *entries)
{
	free(entries);
	entries = NULL;

	return;
}

static struct ux_ipt_standard *ipt_alloc_entries(unsigned int size)
{
	struct ux_ipt_standard *new_entries = NULL;

	if (size < sizeof(*new_entries) || size > UX_IPT_TABLE_MAX_SIZE) {
		net_error(HMNET_CORE, "ipt_alloc_entries: size: %u is invalid\n", size);
		return NULL;
	}

	new_entries = (struct ux_ipt_standard *)malloc(size);
	if (new_entries == NULL) {
		return NULL;
	}

	return new_entries;
}

static int ux_ip_checkentry(const struct ux_ipt_ip *entry_ip)
{
	if ((entry_ip->flags & ~UX_IPT_FLAG_MASK) != 0U) {
		return E_HM_INVAL;
	}

	if ((entry_ip->invflags & ~UX_IPT_INV_MASK) != 0U) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static int ipt_precheck_entry_offset(struct ux_ipt_entry *entry,
				     const unsigned char *limit,
				     unsigned int *num_entries)
{
	struct ux_ipt_standard_target *target;

	if (ptr_to_ulong(entry) % __alignof__(struct ux_ipt_entry) != 0 ||
	    (unsigned char *)entry >= limit - sizeof(struct ux_ipt_entry) ||
	    (unsigned char *)entry > limit - entry->next_offset) {
		return E_HM_INVAL;
	}

	if (entry->next_offset <
	    sizeof(struct ux_ipt_entry) + sizeof(struct ux_ipt_entry_target)) {
		return E_HM_INVAL;
	}

	if (entry->target_offset >
	    entry->next_offset - sizeof(struct ux_ipt_entry_target)) {
		return E_HM_INVAL;
	}

	if (ux_ip_checkentry(&entry->ip) != E_HM_OK) {
		return E_HM_INVAL;
	}

	target = ux_ipt_get_target(entry);
	if (entry->target_offset >
	    entry->next_offset - target->target.u.target_size) {
		return E_HM_INVAL;
	}

	if (target->target.u.target_size < sizeof(struct ux_ipt_standard_target)) {
		return E_HM_INVAL;
	}

	entry->counters.pcnt = 0;
	entry->counters.bcnt = 0;
	entry->comefrom = 0;

	(*num_entries)++;

	return E_HM_OK;
}

static int ipt_precheck_entry_hook(const struct ux_ipt_entry *entry,
				   const unsigned char *base,
				   const struct ux_ipt_replace *repl,
				   unsigned int hook_entry[UX_NF_NUMHOOKS],
				   unsigned int underflow[UX_NF_NUMHOOKS])
{
	int i;

	for (i = 0; i < UX_NF_NUMHOOKS; i++) {
		if ((unsigned int)((unsigned char *)entry - base) == repl->hook_entry[i]) {
			hook_entry[i] = repl->hook_entry[i];
		}
		if ((unsigned int)((unsigned char *)entry - base) == repl->underflow[i]) {
			underflow[i] = repl->underflow[i];
		}
	}

	return E_HM_OK;
}

static int ipt_precheck_entries(const struct ux_ipt_standard *entries,
				const struct ux_ipt_replace *repl,
				unsigned int hook_entry[UX_NF_NUMHOOKS],
				unsigned int underflow[UX_NF_NUMHOOKS])
{
	unsigned int i;
	int ret;
	unsigned int num_entries = 0;

	for (i = 0; i < UX_NF_NUMHOOKS; i++) {
		hook_entry[i] = 0xFFFFFFFF;
		underflow[i] = 0xFFFFFFFF;
	}

	ret = ux_ipt_entry_iterate(struct ux_ipt_entry, entries, repl->size,
				   ipt_precheck_entry_offset,
				   (unsigned char *)entries + repl->size,
				   &num_entries);
	if (num_entries != repl->num_entries || ret != 0) {
		net_warn(HMNET_CORE, "check entry size failed\n");
		return E_HM_INVAL;
	}

	ret = ux_ipt_entry_iterate(struct ux_ipt_entry, entries, repl->size,
				   ipt_precheck_entry_hook,
				   (unsigned char *)entries,
				   repl, hook_entry, underflow);
	if (ret != E_HM_OK) {
		net_warn(HMNET_CORE, "check entry hook failed\n");
		return E_HM_INVAL;
	}

	for (i = 0; i < UX_NF_NUMHOOKS; i++) {
		if ((repl->valid_hooks & (1U << i)) == 0) {
			continue;
		}
		if (hook_entry[i] == 0xFFFFFFFF) {
			net_warn(HMNET_CORE, "hook_entry[%u] is 0xffffffff\n", i);
			return E_HM_INVAL;
		}
		if (underflow[i] == 0xFFFFFFFF) {
			net_warn(HMNET_CORE, "underflow[%u] is 0xffffffff\n", i);
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

static int ipt_replace_entries(const char *name,
			       const struct ux_ipt_standard *entries,
			       unsigned int size)
{
	int ret;

	ret = ux_ipt_entry_iterate(struct ux_ipt_entry, entries, size, ux_ipt_check_replace_match, name);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = ux_ipt_entry_iterate(struct ux_ipt_entry, entries, size, ux_ipt_check_replace_target, name);
	if (ret != E_HM_OK) {
		return ret;
	}

	return E_HM_OK;
}

static int ipt_do_replace(const void *user, unsigned int len)
{
	int ret = 0;
	struct ux_ipt_table *table;
	struct ux_ipt_replace repl;
	struct ux_ipt_standard *entries;
	unsigned int hook_entry[UX_NF_NUMHOOKS];
	unsigned int underflow[UX_NF_NUMHOOKS];

	if (hm_actv_read_xact_vm(&repl, user, sizeof(struct ux_ipt_replace)) != 0) {
		return E_HM_POSIX_FAULT;
	}

	repl.name[sizeof(repl.name) - 1] = 0;

	(void)raw_rwlock_wrlock(&ipt_list_lock);
	table = ipt_find_table_by_name(repl.name);
	if (table == NULL) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_NOENT;
	}

	entries = ipt_alloc_entries(repl.size);
	if (entries == NULL) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_NOMEM;
	}

	if (hm_actv_read_xact_vm(entries, (char *)user + sizeof(struct ux_ipt_replace), repl.size) != 0) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		free(entries);
		return E_HM_POSIX_FAULT;
	}

	ret = ipt_precheck_entries(entries, &repl, hook_entry, underflow);
	if (ret != E_HM_OK) {
		raw_rwlock_unlock(&ipt_list_lock);
		free(entries);
		return ret;
	}

	ret = ipt_replace_entries(repl.name, entries, repl.size);
	if (ret != E_HM_OK) {
		raw_rwlock_unlock(&ipt_list_lock);
		free(entries);
		return ret;
	}

	ipt_clean_entries(table->entries);
	table->entries = entries;
	table->size = repl.size;
	table->valid_hooks = repl.valid_hooks;
	table->num_entries = repl.num_entries;
	NOFAIL(memcpy_s(table->underflow, sizeof(table->underflow), underflow, sizeof(underflow)));
	NOFAIL(memcpy_s(table->hook_entry, sizeof(table->hook_entry), hook_entry, sizeof(hook_entry)));

	raw_rwlock_unlock(&ipt_list_lock);

	return ret;
}

int do_ipt_set_ctl(int opt, void *user, unsigned int len)
{
	int ret = E_HM_OK;

	switch (opt) {
	case UX_IPT_SO_SET_REPLACE:
		ret = ipt_do_replace(user, len);
		break;
	case UX_IPT_SO_SET_ADD_COUNTERS:
		break;
	default:
		ret = E_HM_INVAL;
	}

	return ret;
}

static int ipt_get_info(void *user, int *len)
{
	unsigned int user_len;
	char name[UX_IPT_NAME_MAXLEN];
	struct ux_ipt_table *table;
	struct ux_ipt_getinfo info;

	if (hm_actv_read_xact_vm(&user_len, len, sizeof(unsigned int)) != 0) {
		return E_HM_POSIX_FAULT;
	}

	if (user_len != sizeof(struct ux_ipt_getinfo)) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(name, user, UX_IPT_NAME_MAXLEN) != 0) {
		return E_HM_POSIX_FAULT;
	}

	name[sizeof(name) - 1] = 0;

	(void)raw_rwlock_rdlock(&ipt_list_lock);
	table = ipt_find_table_by_name(name);
	if (table == NULL) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_NOENT;
	}

	info.size = table->size;
	info.num_entries = table->num_entries;
	info.valid_hooks = table->valid_hooks;
	NOFAIL(memcpy_s(info.underflow, sizeof(info.underflow), table->underflow, sizeof(table->underflow)));
	NOFAIL(memcpy_s(info.hook_entry, sizeof(info.hook_entry), table->hook_entry, sizeof(table->hook_entry)));
	if (memset_s(info.name, sizeof(info.name), 0, sizeof(info.name)) != EOK) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_FAULT;
	}

	if (strcpy_s(info.name, sizeof(info.name), name) != 0) {
		raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_FAULT;
	}

	raw_rwlock_unlock(&ipt_list_lock);

	if (hm_actv_write_xact_vm(user, &info, user_len) != 0) {
		return E_HM_POSIX_FAULT;
	}

	return E_HM_OK;
}

static int ipt_copy_entries_to_user(struct ux_ipt_table *table,
				    struct ux_ipt_get_entries *user_entry)
{
	struct ux_ipt_entry *entry;
	struct ux_ipt_entry_match *match;
	struct ux_ipt_entry_target *target;
	struct ux_ipt_standard *ret_entries;
	unsigned int i = 0;
	unsigned int j = 0;

	/* Note:
	 *	Becasue of kernel/user match rule use one union struct,
	 *	so we need copy match rule from kernel struct to user
	 *	struct. See ux_ipt_entry_match to know more details.
	 */
	ret_entries = (struct ux_ipt_standard *)malloc(table->size);
	if (ret_entries == NULL) {
		return E_HM_POSIX_FAULT;
	}
	NOFAIL(memcpy_s(ret_entries, table->size, table->entries, table->size));

	for (i = 0; i < table->size; i += entry->next_offset) {
		entry = (struct ux_ipt_entry *)((char *)ret_entries + i);
		for (j = sizeof(struct ux_ipt_entry); j < entry->target_offset;
		     j += match->u.match_size) {
			match = (struct ux_ipt_entry_match *)((char *)entry + j);
			if (strcpy_s(match->u.user.name, sizeof(match->u.user.name),
				     match->u.kernel.match->name) != 0) {
				free(ret_entries);
				ret_entries = NULL;
				return E_HM_POSIX_FAULT;
			}
		}
		target = (struct ux_ipt_entry_target *)((char *)entry + entry->target_offset);
		if (target->u.kernel.target != NULL && strcmp(target->u.user.name, ERROR_ENTRY) != 0) {
			if (strcpy_s(target->u.user.name, sizeof(target->u.user.name),
				     target->u.kernel.target->name) != 0) {
				free(ret_entries);
				ret_entries = NULL;
				return E_HM_POSIX_FAULT;
			}
		}
	}

	/*
	 * Note:
	 * 	The entrytable of struct ux_ipt_get_entries is a Elastic array,
	 * 	which means user_entry->entrytable equal user_entry + sizeof(user_entry).
	 */
	if (hm_actv_write_xact_vm(user_entry->entrytable,
				  ret_entries,
				  table->size) != 0) {
		free(ret_entries);
		ret_entries = NULL;
		return E_HM_POSIX_FAULT;
	};
	free(ret_entries);
	ret_entries = NULL;

	return E_HM_OK;
}

static int ipt_get_entries(void *user, int *len)
{
	struct ux_ipt_table *table;
	struct ux_ipt_get_entries entries;
	struct ux_ipt_get_entries *user_entry;
	int user_len;
	int ret;

	if (hm_actv_read_xact_vm(&user_len, len, sizeof(int)) != 0) {
		return E_HM_POSIX_FAULT;
	}

	if ((size_t)user_len < sizeof(struct ux_ipt_get_entries)) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&entries, user, sizeof(struct ux_ipt_get_entries)) != 0) {
		return E_HM_POSIX_FAULT;
	}

	if ((size_t)user_len != sizeof(struct ux_ipt_get_entries) + entries.size) {
		return E_HM_INVAL;
	}

	entries.name[sizeof(entries.name) - 1] = 0;

	(void)raw_rwlock_rdlock(&ipt_list_lock);
	table = ipt_find_table_by_name(entries.name);
	if (table == NULL) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_NOENT;
	}
	if (entries.size < table->size) {
		(void)raw_rwlock_unlock(&ipt_list_lock);
		return E_HM_POSIX_FAULT;
	}

	user_entry = (struct ux_ipt_get_entries *)user;

	ret = ipt_copy_entries_to_user(table, user_entry);
	if (ret != E_HM_OK) {
		net_error(HMNET_CORE, "ipt_get_entries: copy entries to user failed\n");
		raw_rwlock_unlock(&ipt_list_lock);
		return ret;
	}
	raw_rwlock_unlock(&ipt_list_lock);

	return 0;
}

static int ipt_get_revision(void *user, int *len, bool is_match)
{
	struct ux_ipt_get_revision rev;

	if (hm_actv_read_xact_vm(&rev, user, sizeof(struct ux_ipt_get_revision)) != 0) {
		return E_HM_POSIX_FAULT;
	}

	rev.name[sizeof(rev.name) - 1] = 0;

	if (is_match) {
		if (ux_ipt_find_match(rev.name, rev.revision)) {
			return 0;
		}
	} else {
		if (ux_ipt_find_target(rev.name, rev.revision)) {
			return 0;
		}
	}

	return E_HM_PROTONOSUPPORT;
}

int do_ipt_get_ctl(int opt, void *user, int *len)
{
	int ret = E_HM_OK;

	switch (opt) {
	case UX_IPT_SO_GET_INFO: {
		ret = ipt_get_info(user, len);
		break;
	}

	case UX_IPT_SO_GET_ENTRIES: {
		ret = ipt_get_entries(user, len);
		break;
	}

	case UX_IPT_SO_GET_REVISION_MATCH: {
		ret = ipt_get_revision(user, len, true);
		break;
	}

	case UX_IPT_SO_GET_REVISION_TARGET: {
		ret = ipt_get_revision(user, len, false);
		break;
	}
	default:
		ret = E_HM_INVAL;
	}

	return ret;
}

int ipt_init(void)
{
	int ret = 0;

	dlist_init(&ipt_list);
	dlist_init(&ipt_match_list);
	dlist_init(&ipt_target_list);

	ret = init_filter_table();
	if (ret != 0) {
		net_error(HMNET_CORE, "ux_iptables: init filter table failed\n");
		return ret;
	}

	ret = init_tcpudp_mt();
	if (ret != 0) {
		net_error(HMNET_CORE, "ux_iptables: init tcpudp match failed\n");
		return ret;
	}

	init_icmp_mt();

#ifdef CONFIG_UXNET_CTRACK
	ret = init_conntrack_mt();
	if (ret) {
		net_error(HMNET_CORE, "ux_iptables: init conntrack match failed\n");
		return ret;
	}
#endif

	ret = init_audit_tg();
	if (ret != 0) {
		net_error(HMNET_CORE, "ux_iptables: init audit target failed\n");
		return ret;
	}

	ret = init_reject_tg();
	if (ret != 0) {
		net_error(HMNET_CORE, "ux_iptables: init reject target failed\n");
		return ret;
	}

	return ret;
}

void ux_ipt_register_target(struct ux_ipt_target *target)
{
	raw_rwlock_wrlock(&ipt_target_list_lock);
	dlist_insert_head(&ipt_target_list, &target->list);
	raw_rwlock_unlock(&ipt_target_list_lock);
}

void ipt_add_table(struct dlist_node *new_table)
{
	dlist_insert_head(&ipt_list, new_table);
}
