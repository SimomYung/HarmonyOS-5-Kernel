/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 20 20:17:25 2021
 */

#include <stdio.h>
#include <stdio_ext.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <nss/netgroup.h>

static bool nss_is_space(char str);
static struct net_group *net_group_parse_group(char *line);
static struct net_group_entry *net_group_parse_entry(char *line);
static char *net_group_consume_space(char *str);
static char *net_group_consume_not_space(char *str);
static char *net_group_consume_until(char *str, char ch);
static int count_symbol(const char *str, const char symbol);
static struct net_group *net_group_create_group(const char *name);
static struct net_groups *net_group_parse_data(FILE *file, struct net_groups *net_grps);
static struct net_group *net_group_parse_group_data(char *line, const char *netgrpname);
static struct net_group_entry *netgroup_make_triple(char *name, struct net_group_entry *entry);

#define COMMA_NUM_DEFULT 2

struct net_groups *net_group_parse_file(void)
{
	struct net_groups *net_grps = NULL;
	FILE *file = fopen(_PATH_NETGROUP, "r");
	if (file != (FILE*)NULL) {
		net_grps = malloc(sizeof(struct net_groups));
		if (net_grps != NULL) {
			net_grps = net_group_parse_data(file, net_grps);
		}
		(void)fclose(file);
	}
	return net_grps;
}

static struct net_groups *net_group_parse_data(FILE *file, struct net_groups *net_grps)
{
	char *line = NULL;

	net_grps->last = NULL;
	net_grps->group = NULL;
	while (feof(file) == 0) {
		size_t len = 0;
		ssize_t size = getline(&line, &len, file);
		if (size < 0) {
			break;
		}
		if (*line == '\0' || nss_is_space(*line) || *line == '#') {
			continue;
		}
		struct net_group *grp = net_group_parse_group(line);
		if (grp != NULL) {
			if (net_grps->group == NULL) {
				net_grps->group = grp;
			} else {
				net_grps->last->next = grp;
			}
			net_grps->last = grp;
		}
	}
	if (line != NULL) {
		free(line);
	}
	return net_grps;
}

static struct net_group *net_group_parse_group(char *line)
{
	char *l = line;
	struct net_group *netgrp = NULL;
	l = net_group_consume_space(l);
	char *name = l;
	l = net_group_consume_not_space(l);
	*l = '\0';
	if (l != name) {
		l++;
		netgrp = net_group_parse_group_data(l, name);
	}
	return netgrp;
}

static struct net_group *net_group_parse_group_data(char *line, const char *netgrpname)
{
	char *l = line;
	struct net_group *netgrp = net_group_create_group(netgrpname);
	if (netgrp != NULL) {
		l = net_group_consume_space(l);
		struct net_group_entry *entry = net_group_parse_entry(l);
		if (entry != NULL) {
			if (netgrp->entry == NULL) {
				netgrp->entry = entry;
			} else {
				netgrp->last->next = entry;
			}
			netgrp->last = entry;
		}
	}
	return netgrp;
}

static struct net_group_entry *netgroup_make_triple(char *name, struct net_group_entry *entry)
{
	char *n = name;
	int comma_count = count_symbol(n, ',');
	int close_brace = count_symbol(n, ')');
	struct net_group_entry *e = entry;
	if (comma_count == COMMA_NUM_DEFULT && close_brace == 1) {
		n++;
		char* user = net_group_consume_until(n, ',');
		(void)strncpy(e->triple.host, n, sizeof(e->triple.host) - 1UL);
		e->triple.host[sizeof(e->triple.host) - 1UL] = '\0';

		char* domain = net_group_consume_until(user, ',');
		(void)strncpy(e->triple.name, user, sizeof(e->triple.name) - 1UL);
		e->triple.name[sizeof(e->triple.name) - 1UL] = '\0';

		(void)net_group_consume_until(domain, ')');
		(void)strncpy(e->triple.domain, domain, sizeof(e->triple.domain) - 1UL);
		e->triple.domain[sizeof(e->triple.domain) - 1UL] = '\0';

		e->type = TRIPLE;
		if (strlen(n) >= sizeof(e->triple.host) ||
		   strlen(user) >= sizeof(e->triple.name) ||
		   strlen(domain) >= sizeof(e->triple.domain)) {
			free(e);
			e = NULL;
		}
	} else {
		free(e);
		e = NULL;
	}
	return e;
}

static struct net_group_entry *net_group_parse_entry(char *line)
{
	char *l = line;
	struct net_group_entry *result = NULL;
	char *name = l;
	l = net_group_consume_not_space(l);
	if (*l != '\0') {
		*l = '\0';
		struct net_group_entry *entry = malloc(sizeof(struct net_group_entry));
		if (entry == NULL) {
			goto out;
		}
		entry->next = NULL;
		(void)net_group_consume_space(l);
		if (*name == '(') { /* triple */
			entry = netgroup_make_triple(name, entry);
		} else { /* group */
			entry->type = GROUP;
			(void)strncpy(entry->group, name, sizeof(entry->group) - 1UL);
			entry->group[sizeof(entry->group) - 1UL] = '\0';
			if (strlen(name) >= sizeof(entry->group)) {
				free(entry);
				entry = NULL;
			}
		}
		result = entry;
	}
out:
	return result;
}

static struct net_group *net_group_create_group(const char *name)
{
	size_t len = strlen(name);
	struct net_group *netgrp = malloc(sizeof(struct net_group) + len + 1UL);
	if (netgrp != NULL) {
		(void)strcpy(netgrp->name, name);
		netgrp->entry = NULL;
		netgrp->next = NULL;
		netgrp->last = NULL;
	}

	return netgrp;
}

static bool nss_is_space(char str)
{
	return isspace((int)str) != 0;
}

static char *net_group_consume_condition(char *str, bool cond)
{
	char *s = str;
	while (*s != '\0' && (cond ? nss_is_space(*s) : !nss_is_space(*s))) {
		s++;
	}
	return s;
}

static char *net_group_consume_space(char *str)
{
	return net_group_consume_condition(str, true);
}

static char *net_group_consume_not_space(char *str)
{
	return net_group_consume_condition(str, false);
}

static char *net_group_consume_until(char *str, char ch)
{
	char *s = str;
	while (*s != '\0' && *s != ch) {
		s++;
	}
	*s = '\0';
	s++;
	return s;
}

static int count_symbol(const char *str, const char symbol)
{
	const char *s = str;
	int count = 0;
	while (*s != '\0') {
		if (*s == symbol) {
			count++;
		}
		s++;
	}
	return count;
}
