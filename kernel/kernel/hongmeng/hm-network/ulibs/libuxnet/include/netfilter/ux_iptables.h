/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: iptables head file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 14:21:33 2021
 */

#ifndef _UX_IPTABLES_H_
#define _UX_IPTABLES_H_

#include <net/if.h>
#include "ux_socket.h"
#include "ux_netfilter.h"

#define UX_IPT_BASE_CTL				64

#define UX_IPT_SO_SET_REPLACE			(UX_IPT_BASE_CTL)
#define UX_IPT_SO_SET_ADD_COUNTERS		(UX_IPT_BASE_CTL + 1)
#define UX_IPT_SO_SET_MAX			UX_IPT_SO_SET_ADD_COUNTERS

#define UX_IPT_SO_GET_INFO			(UX_IPT_BASE_CTL)
#define UX_IPT_SO_GET_ENTRIES			(UX_IPT_BASE_CTL + 1)
#define UX_IPT_SO_GET_REVISION_MATCH		(UX_IPT_BASE_CTL + 2)
#define UX_IPT_SO_GET_REVISION_TARGET		(UX_IPT_BASE_CTL + 3)
#define UX_IPT_SO_GET_MAX			UX_IPT_SO_GET_REVISION_TARGET

#define UX_IPT_NAME_MAXLEN 			32
#define UX_IPT_FUNCTION_MAXNAMELEN 		30

#define UX_IPT_FLAG_MASK			0x0

#define UX_IPT_INV_VIA_IN			0x01
#define UX_IPT_INV_VIA_OUT			0x02
#define UX_IPT_INV_SRCIP			0x08
#define UX_IPT_INV_DSTIP			0x10
#define UX_IPT_INV_PROTO			0x40
#define UX_IPT_INV_MASK				0x5B

#define UX_IPT_CONTINUE				0xFFFFFFFF
#define UX_IPT_RETURN				(-UX_NF_REPEAT - 1)

#define UX_IPT_TABLE_MAX_SIZE			(1 * 1024 * 1024)

static inline bool fwinv(bool a, uint8_t invflags, uint8_t invflg)
{
	return (unsigned int)a != (unsigned int)((invflags & invflg) != 0);
}

#define ux_ipt_align(s) (((s) + (__alignof__(struct ux_ipt_entry) - 1)) & ~(__alignof__(struct ux_ipt_entry) - 1))

struct ux_netdev;

struct ux_ipt_counters {
	uint64_t pcnt, bcnt;
};

struct ux_ipt_ip {
	struct in_addr src;
	struct in_addr dst;
	struct in_addr smsk;
	struct in_addr dmsk;
	char iniface[IF_NAMESIZE];
	char outiface[IF_NAMESIZE];
	unsigned char iniface_mask[IF_NAMESIZE];
	unsigned char outiface_mask[IF_NAMESIZE];

	uint16_t proto;
	uint8_t flags;
	uint8_t invflags;
};

struct ux_ipt_entry {
	struct ux_ipt_ip ip;
	unsigned int nfcache;
	uint16_t target_offset;
	uint16_t next_offset;
	unsigned int comefrom;
	struct ux_ipt_counters counters;
	unsigned char elems[];
};

struct ux_ipt_get_entries {
	char name[UX_IPT_NAME_MAXLEN];
	unsigned int size;
	struct ux_ipt_entry entrytable[];
};

struct ux_ipt_table {
	struct dlist_node list;
	char name[UX_IPT_NAME_MAXLEN];
	unsigned int valid_hooks;
	unsigned int size;
	unsigned int hook_entry[UX_NF_NUMHOOKS];
	unsigned int underflow[UX_NF_NUMHOOKS];
	unsigned int num_entries;
	struct ux_ipt_standard *entries;
};

struct ux_ipt_entry_target {
	union {
		struct {
			uint16_t target_size;
			char name[UX_IPT_FUNCTION_MAXNAMELEN - 1];
			uint8_t revision;
		} user;
		struct {
			uint16_t target_size;
			struct ux_ipt_target *target;
		} kernel;
		uint16_t target_size;
	} u;
	unsigned char data[];
};

struct ux_ipt_entry_match {
	union {
		struct {
			uint16_t match_size;
			char name[UX_IPT_FUNCTION_MAXNAMELEN - 1];
			uint8_t revision;
		} user;
		struct {
			uint16_t match_size;
			struct ux_ipt_match *match;
		} kernel;
		uint16_t match_size;
	} u;

	unsigned char data[];
};

struct ux_ipt_standard_target {
	struct ux_ipt_entry_target target;
	int verdict;
};

struct ux_ipt_standard {
	struct ux_ipt_entry entry;
	struct ux_ipt_standard_target target;
};

struct ux_ipt_error_target {
	struct ux_ipt_entry_target target;
	char errorname[UX_IPT_FUNCTION_MAXNAMELEN];
};

struct ux_ipt_error {
	struct ux_ipt_entry entry;
	struct ux_ipt_error_target target;
};

struct ux_ipt_getinfo {
	char name[UX_IPT_NAME_MAXLEN];
	unsigned int valid_hooks;
	unsigned int hook_entry[UX_NF_NUMHOOKS];
	unsigned int underflow[UX_NF_NUMHOOKS];
	unsigned int num_entries;
	unsigned int size;
};

struct ux_ipt_replace {
	char name[UX_IPT_NAME_MAXLEN];
	unsigned int valid_hooks;
	unsigned int num_entries;
	unsigned int size;
	unsigned int hook_entry[UX_NF_NUMHOOKS];
	unsigned int underflow[UX_NF_NUMHOOKS];
	unsigned int num_counters;
	struct ux_ipt_counters *counters;
	struct ux_ipt_entry entries[];
};

struct ux_ipt_match {
	struct dlist_node list;
	const char name[UX_IPT_FUNCTION_MAXNAMELEN - 1];
	bool (*match)(const struct ux_pktbuf *skb,
		      const struct ux_netdev *in,
		      const struct ux_netdev *out,
		      const struct ux_ipt_match *match,
		      const void *matchinfo);
	int (*checkentry)(const char *tablename,
			  const struct ux_ipt_ip *ip,
			  const void *matchinfo,
			  unsigned int matchinfo_size);
	unsigned int matchsize;
	unsigned short family;
	uint8_t revision;
};

struct ux_ipt_target {
	struct dlist_node list;
	const char name[UX_IPT_FUNCTION_MAXNAMELEN - 1];
	unsigned int (*target)(const struct ux_pktbuf *skb,
			       const struct ux_netdev *in,
			       const struct ux_netdev *out,
			       const struct ux_ipt_target *target,
			       const void *targetinfo);
	int (*checkentry)(const char *tablename,
			  const struct ux_ipt_ip *ip,
			  const void *targetinfo,
			  unsigned int target_size);
	unsigned int targetsize;
	unsigned short family;
	uint8_t revision;
};

struct ux_ipt_get_revision {
	char name[UX_IPT_FUNCTION_MAXNAMELEN - 1];
	uint8_t revision;
};

struct ux_ipt_netdev {
	struct ux_netdev *indev;
	struct ux_netdev *outdev;
};

void ux_add_ipt_match(struct ux_ipt_match *match);

int do_ipt_set_ctl(int opt, void *user, unsigned int len);

int do_ipt_get_ctl(int opt, void *user, int *len);

struct ux_ipt_table *ipt_find_table_by_name(const char *name);

int ipt_init(void);

int ux_ipt_do_table(unsigned int hook,
		    struct ux_pktbuf *pskbuf,
		    struct ux_ipt_netdev netdev,
		    const char *name);

void ux_ipt_register_target(struct ux_ipt_target *target);

void ipt_add_table(struct dlist_node *new_table);
#endif
