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
#include <nss/nsswitch.h>
#include <nss/netgroup.h>

static struct nss_database *g_source_table;
static struct net_groups *g_net_grps;

static struct nss_database *nss_parse_conf(void);
static struct nss_database_row *nss_parse_line(char *line);
static void nss_parse_sources(struct nss_database_row *row, char *line);
static bool nss_is_space(char str);
static void nss_db_append_row(struct nss_database *db, struct nss_database_row *row);
static void nss_row_append_source(struct nss_database_row *row, struct nss_source *source);
static struct nss_source *nss_create_source(const char *info_name, const char *name);
static struct nss_source *nss_parse_action(struct nss_source *source, char *name);
static char *nss_consume_space(char *str);
static char *nss_consume_not_space(char *str);
static bool net_group_compare_entry(struct net_group_entry *entry,
				    const char *groupname,
				    const char *host, const char *name,
				    const char *domain);
static void nss_parse_data(FILE *file, struct nss_database *db);
static bool net_group_compare_entry_str(const char *entry_str, const char *str);
static nss_source_status net_grp_find_item(const char *groupname, const char *host,
					   const char *name, const char *domain);
static struct nss_database_row *nss_lookup(const char *name);
static struct nss_database_row *nss_lookup_in_table(const char *name);

#define free_list(type)								\
static void nss_free_##type(struct type *list);					\
static void nss_free_##type(struct type *list)					\
{										\
	struct type *l = list;							\
	struct type *next = NULL;						\
	while (l != NULL) {							\
		next = l->next;							\
		free(l);							\
		l = next;							\
	}									\
}

free_list(nss_source)

free_list(net_group_entry)

#define free_list_parent(type, child_type, child) 				\
static void nss_free_##type(struct type *list);					\
static void nss_free_##type(struct type *list)					\
{										\
	struct type *l = list;							\
	struct type *next = NULL;						\
	while (l != NULL) {							\
		next = l->next;							\
		nss_free_##child_type(l->child);				\
		free(l);							\
		l = next;							\
	}									\
}

free_list_parent(nss_database_row, nss_source, source)

free_list_parent(net_group, net_group_entry, entry)

#define nss_free(resource, child_type, child)					\
static void nss_free_##resource();						\
static void nss_free_##resource()						\
{										\
	if ((resource) != NULL) {						\
		nss_free_##child_type((resource)->child);			\
		free(resource);							\
		(resource) = NULL;						\
	}									\
}

nss_free(g_source_table, nss_database_row, row)

nss_free(g_net_grps, net_group, group)

static nss_source_status __nss_netgroup_files(const struct nss_source_params params)
{
	nss_source_status result = NSS_SOURCE_STATUS_UNAVAIL;

	nss_free_g_net_grps();
	if (g_net_grps == NULL) {
		g_net_grps = net_group_parse_file();
	}
	if (g_net_grps != NULL) {
		result = net_grp_find_item(params.netgroup_param.group,
					   params.netgroup_param.host,
					   params.netgroup_param.name,
					   params.netgroup_param.domain);
	}
	return result;
}

static nss_source_status net_grp_find_item(const char *groupname, const char *host,
					   const char *name, const char *domain)
{
	nss_source_status result = NSS_SOURCE_STATUS_NOTFOUND;
	struct net_group *group = g_net_grps->group;
	while (group != NULL) {
		struct net_group_entry *entry = group->entry;
		if (entry != NULL) {
			if (strcmp((const char *)(group->name), groupname) == 0 
			    && net_group_compare_entry(entry, groupname, host, name, domain)) {
				result = NSS_SOURCE_STATUS_SUCCESS;
				break;
			}
		}
		group = group->next;
	}
	return result;
}

static bool net_group_compare_entry(struct net_group_entry *entry, const char *groupname,
				    const char *host, const char *name, const char *domain)
{
	bool result = false;
	if (entry->type != GROUP) {
		bool is_host_match = net_group_compare_entry_str(entry->triple.host, host);
		bool is_name_match = net_group_compare_entry_str(entry->triple.name, name);
		bool is_domain_match = net_group_compare_entry_str(entry->triple.domain, domain);
		result = is_host_match && is_name_match && is_domain_match;
	}
	return result;
}

static bool net_group_compare_entry_str(const char *entry_str, const char *str)
{
	bool result = false;
	if (strcmp(entry_str, "") == 0) {
		result = true;
	} else if (strcmp(entry_str, "-") == 0) {
		result = false;
	} else {
		if (str != NULL) {
			if (strcmp(entry_str, str) == 0) {
				result = true;
			}
		}
	}

	return result;
}

static struct nss_database *nss_parse_conf(void)
{
	FILE *file = fopen(_PATH_NSSWITCH_CONF, "r");

	struct nss_database *result = NULL;
	if (file != (FILE*)NULL) {
		struct nss_database *db = malloc(sizeof(struct nss_database));
		if (db != NULL) {
			db->row = NULL;
			nss_parse_data(file, db);
		}
		result = db;
		(void)fclose(file);
	}
	return result;
}

static void nss_parse_data(FILE *file, struct nss_database *db)
{
	char *line = NULL;
	while (feof(file) == 0) {
		size_t len = 0;
		ssize_t size = getline(&line, &len, file);
		if (size < 0) {
			break;
		}
		if (*line == '\0' || nss_is_space(*line) || *line == '#') {
			continue;
		}
		struct nss_database_row *row = nss_parse_line(line);
		nss_db_append_row(db, row);
	}
	if (line != NULL) {
		free(line);
	}
}

static void nss_db_append_row(struct nss_database *db, struct nss_database_row *row)
{
	if (row != NULL) {
		if (db->row == NULL) {
			db->row = row;
		} else {
			db->last->next = row;
		}
		db->last = row;
	}
}

static struct nss_database_row *nss_parse_line(char *line)
{
	char *l = line;
	char *name = l;
	struct nss_database_row *result;

	while (*l != '\0' && !nss_is_space(*l) && *l != ':') {
		l++;
	}
	*l = '\0';
	size_t len = strlen(name);
	struct nss_database_row *row = malloc(sizeof(struct nss_database_row) + len + 1UL);
	if (row == NULL) {
		result = NULL;
	} else {
		row->next = NULL;
		row->source = NULL;
		(void)memcpy(row->name, name, len + 1UL);
		l++;
		nss_parse_sources(row, l);
		result = row;
	}

	return result;
}

static void nss_parse_sources(struct nss_database_row *row, char *line)
{
	char *l = line;
	struct nss_source *prev = NULL;
	while (*l != '\0') {
		l = nss_consume_space(l);
		if (*l == '\0') {
			break;
		}
		char *name = l;
		l = nss_consume_not_space(l);
		if (*l == '\0') {
			break;
		}
		*l = '\0';
		if (*name == '[') { /* action like '[NOTFOUND=return]' */
			name++;
			struct nss_source *source = nss_parse_action(prev, name);
			if (source == NULL) {
				break;
			}
		} else { /* just source like 'files' */
			struct nss_source *source = nss_create_source((const char *)(row->name), name);
			if (source == NULL) {
				break;
			}
			prev = source;
			nss_row_append_source(row, source);
		}
		l++;
	}
}

static void nss_row_append_source(struct nss_database_row *row, struct nss_source *source)
{
	if (row->source == NULL) {
		row->source = source;
	} else {
		row->last_source->next = source;
	}
	row->last_source = source;
}

#define nss_set_handle(source, info, info_name, source_, name) do {		\
	if (strcmp(#info, info_name) == 0 && strcmp(name, #source_) == 0) {	\
		(source)->handle = __nss_##info##_##source_;			\
	}									\
} while (0)

#define NSS_SOURCE_STATUS_OFFSET 2

static inline void set_nss_source_action(struct nss_source *source, nss_source_status status, lookup_action action)
{
	source->actions[(int)status + NSS_SOURCE_STATUS_OFFSET] = action;
	return;
}

static struct nss_source *nss_create_source(const char *info_name, const char *name)
{
	size_t len = strlen(name);
	struct nss_source *source = malloc(sizeof(struct nss_source) + len + 1UL);
	if (source != NULL) {
		source->next = NULL;
		nss_set_handle(source, netgroup, info_name, files, name);
		set_nss_source_action(source, NSS_SOURCE_STATUS_TRY_AGAIN, NSS_SOURCE_ACTION_CONTINUE);
		set_nss_source_action(source, NSS_SOURCE_STATUS_UNAVAIL, NSS_SOURCE_ACTION_CONTINUE);
		set_nss_source_action(source, NSS_SOURCE_STATUS_NOTFOUND, NSS_SOURCE_ACTION_CONTINUE);
		set_nss_source_action(source, NSS_SOURCE_STATUS_SUCCESS, NSS_SOURCE_ACTION_RETURN);
		(void)memcpy(source->name, name, len + 1UL);
	}
	return source;
}

static void status_case(char* name, struct nss_source *source, const char *status_str,
			int status, const char *status_name)
{
	char* n = name;
	if (strcmp(status_name, status_str) == 0) {
		n++;
		char *action_name = n;
		while (*n != '\0' && *n != ']') {
			n++;
		}
		*n = '\0';
		if (strcmp(action_name, "return") == 0) {
			source->actions[NSS_SOURCE_STATUS_OFFSET + status] = NSS_SOURCE_ACTION_RETURN;
		} else if (strcmp(action_name, "continue") == 0) {
			source->actions[NSS_SOURCE_STATUS_OFFSET + status] = NSS_SOURCE_ACTION_CONTINUE;
		} else {
			source->actions[NSS_SOURCE_STATUS_OFFSET + status] = NSS_SOURCE_ACTION_CONTINUE;
		}
	}
	return;
}

static struct nss_source *nss_parse_action(struct nss_source *source, char *name)
{
	char *n = name;
	struct nss_source *result = NULL;
	if (source != NULL && n != NULL) {
		char *status_name = n;
		while (*n != '\0' && *n != '=') { /* from '[' to '=' */
			n++;
		}
		*n = '\0';
		status_case(n, source, "NOTFOUND", (int)NSS_SOURCE_STATUS_NOTFOUND, status_name);
		status_case(n, source, "UNAVAIL", (int)NSS_SOURCE_STATUS_UNAVAIL, status_name);
		status_case(n, source, "TRYAGAIN", (int)NSS_SOURCE_STATUS_TRY_AGAIN, status_name);
		status_case(n, source, "SUCCESS", (int)NSS_SOURCE_STATUS_SUCCESS, status_name);

		result = source;
	}
	return result;
}

static struct nss_database_row *nss_lookup(const char *name)
{
	struct nss_database_row *result = NULL;
	nss_free_g_source_table();
	if (g_source_table == NULL) {
		g_source_table = nss_parse_conf();
		result = nss_lookup_in_table(name);
	}
	return result;
}

static struct nss_database_row *nss_lookup_in_table(const char *name)
{
	struct nss_database_row *result = NULL;
	if (g_source_table != NULL) {
		struct nss_database_row *row = g_source_table->row;
		while (row != NULL) {
			if (strcmp((const char *)(row->name), name) == 0) {
				result = row;
				break;
			}
			row = row->next;
		}
	}
	return result;
}

static bool source_status_case(int *result, int status, int val, const struct nss_source *source)
{
	bool ret = false;
	if (source->actions[status + NSS_SOURCE_STATUS_OFFSET] == NSS_SOURCE_ACTION_RETURN) {
		*result = val;
		ret = true;
	} else {
		if (source->actions[status + NSS_SOURCE_STATUS_OFFSET] == NSS_SOURCE_ACTION_CONTINUE) {
			if (source->next == NULL) {
				*result = val;
				ret = true;
			}
		}
	}
	return ret;
}

int innetgr(const char *netgroup, const char *host, const char *user,
	    const char *domain)
{
	struct nss_database_row *row = nss_lookup("netgroup");
	int result = 0;
	nss_source_status st = NSS_SOURCE_STATUS_UNAVAIL;
	struct nss_source *source = NULL;
	bool ret = false;
	const struct nss_source_params params = {
		.netgroup_param = {
			.group = netgroup,
			.host = host,
			.name = user,
			.domain = domain,
		}
	};

	if (row != NULL) {
		source = row->source;
	}
	while (source != NULL) {
		if (source->handle != NULL) {
			st = (source->handle)(params);
		}
		switch (st) {
		case NSS_SOURCE_STATUS_TRY_AGAIN:
			ret = source_status_case(&result, (int)NSS_SOURCE_STATUS_TRY_AGAIN, 0, source);
			break;
		case NSS_SOURCE_STATUS_UNAVAIL:
			ret = source_status_case(&result, (int)NSS_SOURCE_STATUS_UNAVAIL, 0, source);
			break;
		case NSS_SOURCE_STATUS_NOTFOUND:
			ret = source_status_case(&result, (int)NSS_SOURCE_STATUS_NOTFOUND, 0, source);
			break;
		case NSS_SOURCE_STATUS_SUCCESS:
			ret = source_status_case(&result, (int)NSS_SOURCE_STATUS_SUCCESS, 1, source);
			break;
		default:
			break;
		}
		if (ret) {
			break;
		}
		source = source->next;
	}

	return result;
}

static bool nss_is_space(char str)
{
	return isspace((int)str) != 0;
}

static char *nss_consume_condition(char *str, bool cond)
{
	char *s = str;
	while (*s != '\0' && (cond ? nss_is_space(*s) : !nss_is_space(*s))) {
		s++;
	}
	return s;
}

static char *nss_consume_space(char *str)
{
	return nss_consume_condition(str, true);
}

static char *nss_consume_not_space(char *str)
{
	return nss_consume_condition(str, false);
}
