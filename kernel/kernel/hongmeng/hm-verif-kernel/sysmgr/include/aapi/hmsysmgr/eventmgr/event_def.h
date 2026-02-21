/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Event definitions exported to system managers.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 19 16:24:44 2023
 */
#ifndef SYSMGR_INCLUDE_AAPI_EVENTMGR_EVENT_DEF_H
#define SYSMGR_INCLUDE_AAPI_EVENTMGR_EVENT_DEF_H

#include <libhmucap/ucap.h>
#define EVTMGR_THREAD_WL_MAX	32
#define EVTMGR_GROUP_WL_MAX	12

enum evtmgr_etypes {
	EVTMGR_ETYPE_THREAD_CLONE,
	EVTMGR_ETYPE_THREAD_SET_NAME,
	EVTMGR_ETYPE_PROCESS_EXIT,
	EVTMGR_ETYPE_PATH_CLEAN_ENTRY,
	EVTMGR_ETYPE_MAX,
};

enum evtmgr_erule_cmd {
	EVTMGR_ERULE_SET_THREAD_NAME_WL,
	EVTMGR_ERULE_SET_THREAD_GROUP_WL,
	EVTMGR_ERULE_DELETE_THREAD_NAME_WL,
	EVTMGR_ERULE_DELETE_THREAD_GROUP_WL,
	EVTMGR_ERULE_MAX
};

/*
 * Whitelist of thread name.
 * Event handler determine whether to send messages to server which registered event based on the whitelist.
 */
struct evtmgr_thread_name_wl_s {
	unsigned int event_bitmask;
	int count;
	char threads[EVTMGR_THREAD_WL_MAX][HMCAP_THREAD_NAME_SIZE];
};

/*
 * Whitelist of thread group.
 * Event handler determine whether to send messages to server which registered event based on the whitelist.
 */
struct evtmgr_thread_group_wl_s {
	unsigned int event_bitmask;
	int count;
	uint32_t groups[EVTMGR_GROUP_WL_MAX];
};

struct evtcb_thread_msg_s {
	enum evtmgr_etypes event_type;
	pid_t tid;
	pid_t pid;
	uint32_t group;
	char name[HMCAP_THREAD_NAME_SIZE];
	char new_name[HMCAP_THREAD_NAME_SIZE];
};
#endif /* SYSMGR_INCLUDE_AAPI_EVENTMGR_EVENT_DEF_H */
