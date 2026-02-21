/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 20 20:17:25 2021
 */

#ifndef _NSS_NSSWITCH_H
#define _NSS_NSSWITCH_H

#define        _PATH_HOSTS                "/etc/hosts"
#define        _PATH_NSSWITCH_CONF        "/etc/nsswitch.conf"

#define NSSWITCH_MSG_TYPE        4

/*
 * nss database design
 *
 * etc/nsswitch.conf
 *
 *    netgroup:    files   [NOTFOUND=retuern]   nis
 *    ---+----     -----------+--------------   -+-
 *       +                    +                  +
 *       |                    |                  |
 *       |                    |                  |
 *       |                    |                  +----------------+
 *       |                    |                                   |
 *       v          source    v                       source      v
 *  entry_name     +----------------------------+    +---------------------------+
 *                 | source_name: files         |    | source_name: nis          |
 *                 | action[NOTFOUND]=return    |    | action default            |
 *                 | handler: read etc/netgroup |    | handler: send nis server  |
 *                 +----------------------------+    +---------------------------+
 */

typedef struct nss_netgroup_params {
	const char *group;
	const char *host;
	const char *name;
	const char *domain;
} nss_netgroup_params;

typedef struct nss_source_params {
	union {
		struct nss_netgroup_params netgroup_param;
	};
} nss_source_params;

typedef enum lookup_action {
	NSS_SOURCE_ACTION_CONTINUE = 0,
	NSS_SOURCE_ACTION_RETURN = 1
} lookup_action;

typedef enum nss_source_status {
	NSS_SOURCE_STATUS_UNAVAIL = -2,
	NSS_SOURCE_STATUS_NOTFOUND = -1,
	NSS_SOURCE_STATUS_TRY_AGAIN = 0,
	NSS_SOURCE_STATUS_SUCCESS = 1
} nss_source_status;

typedef nss_source_status (*nss_source_handle)(const struct nss_source_params);

typedef struct nss_source {
	struct nss_source *next;
	lookup_action actions[NSSWITCH_MSG_TYPE];
	nss_source_handle handle;
	char name[0];
} nss_source;

typedef struct nss_database_row {
	struct nss_database_row *next;
	struct nss_source *source;
	struct nss_source *last_source;
	char name[0];
} nss_database_row;

typedef struct nss_database {
	struct nss_database_row *row;
	struct nss_database_row *last;
} nss_database;

int innetgr(const char *, const char *, const char *, const char *);

#endif
