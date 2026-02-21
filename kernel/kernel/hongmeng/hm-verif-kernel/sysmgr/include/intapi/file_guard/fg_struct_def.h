/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Common struct def for file_guard
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */

#ifndef SYSMGR_INCLUDE_INTAPI_FG_STRUCT_DEF_H
#define SYSMGR_INCLUDE_INTAPI_FG_STRUCT_DEF_H

#include <stdint.h>
#include <string.h>

#include <limits.h>
#include <unistd.h>
#include <netinet/in.h>

#include <securec.h>

#define __packed __attribute__((packed))

#define FG_CHR_WRITE_SIZE		(512 * 1024)

#define FG_MAX_TAG_NAME_LENGTH  (256) // max length of tag name, incuding '\0'
#define FG_MAX_USER_TAG_NUM     (5)
#define FG_MAX_TAGS_LENGTH      (FG_MAX_USER_TAG_NUM * FG_MAX_TAG_NAME_LENGTH) 
#define FG_TAG_XATTR_NAME       "trusted.filetag"
#define FG_TAG_XATTR_SEP		";"

enum fg_message_type {
	FG_COMMAND_MESSAGE,  // from userspace to kernel
	FG_DATA_MESSAGE      // from kernel to userspace
};

enum fg_message_module_type {
	FG_IPC,
	FG_NETWORK,
	FG_FSWORK,
	FG_SERIAL_PORT,
	FG_MODULE_COUNT
};

typedef enum {
	FG_FILE_NORMAL = 0,
	FG_FILE_KIA,
	FG_FILE_KEYWORD,
	FG_FILE_SUFFIX,
	FG_FILE_COMPRESS_SUFFIX,
	FG_FILE_TAG_BEGIN,
	FG_FILE_TAG1 = FG_FILE_TAG_BEGIN,
	FG_FILE_TAG2,
	FG_FILE_TAG3,
	FG_FILE_TAG4,
	FG_FILE_TAG5,
	FG_FILE_TAG_END = FG_FILE_TAG5
} fg_file_type_t;

enum fg_command_type {
	// downstream
	FG_CMD_COMMON_TOGGLE = 0,     // send toggle command, ref: fg_toggle
	FG_CMD_COMMON_RULE = 1,       // send rules, ref

	// upstream
	FG_CMD_COMMON_RECORD = 2,
	FG_CMD_COMMON_REJECT_RECORD = 3,
	FG_CMD_COMMON_OP_RECORD = 4,

	// modules' custom command start from here, e.g. FG_CMD_USB_LIST = 101
	// beware command is uint8_t, value cannot be greater than 256
	FG_CMD_MODULE_START = 100,
	FG_CMD_IPC_PROCESS_LIST = 101,

	FG_CMD_FSWORK_ACTION_SWITCHER = 102,
	FG_CMD_NETWORK_ACTION_SWITCHER = 103,
	FG_CMD_FS_KEYWORD_LIST_RULE = 104,
	FG_CMD_FS_COOBJ_INIT = 105,

	FG_CMD_NETWORK_SET_DMODE = 112,
	FG_CMD_NETWORK_REJECT_CACHETIME = 113,
	FG_CMD_NETWORK_SEGMENT_BOUNDARY = 115,
	FG_CMD_NETWORK_SEGMENT_DEFAULT_POLICY = 116,
	FG_CMD_ASSOCIATED_PROCESS_LIST_RULE = 117,
	FG_CMD_ASSOCIATED_PROCESS_CLEAR_RULE = 118,
	FG_CMD_NETWORK_REWIND_PROCESS_RULE = 119,
	FG_CMD_IPC_KIA_BLOCK_CACHE = 120,
	FG_CMD_IPC_TRUSTED_UID_LIST_RULE = 121,
	FG_CMD_IPC_NOTIFY_CLEAR_CACHE = 122
};

enum fg_file_operation_type {
	FG_FILE_UNKNOWN = 0,
	FG_FILE_OPEN,
	FG_FILE_CREATE,
	FG_FILE_CLOSE,
	FG_FILE_READ,
	FG_FILE_WRITE,
	FG_FILE_REMOVE,
	FG_FILE_RENAME,
	FG_FILE_COPY,
	FG_FILE_COMPRESS,
};

enum fg_action_ops {
	// network reject
	FG_OP_NET_RECV_REJECT = (1 << 0),
	FG_OP_NET_SEND_REJECT = (1 << 1),

	// network redirect
	FG_OP_NET_RECV_REDIRECT = (1 << 2),
	FG_OP_NET_SEND_REDIRECT = (1 << 3),
};

typedef enum fg_action_type {
	FG_ACTION_NONE = 0,
	FG_ACTION_AUDIT = (1 << 0),
	FG_ACTION_CONTROL = (1 << 1),
	FG_ACTION_ALL = FG_ACTION_AUDIT | FG_ACTION_CONTROL
} fg_action_type_t;

typedef enum action_status {
	PASSED = 0,
	REJECTED = 1,
	KIA_INTERCEPTED = 2,
	KIA_ASSOCIATED = 3
} action_status_t;

typedef enum fg_network_action_switcher_type {
	FG_NETWORK_ACTION_ALL_OFF = 0,
	FG_NETWORK_ACTION_AUDIT_ON = (1 << 0),
	FG_NETWORK_ACTION_IPV4_REJECT_ON = (1 << 1),
	FG_NETWORK_ACTION_IPV6_REJECT_ON = (1 << 2),
	FG_NETWORK_ACTION_OFFLINE_ON = (1 << 3), // all network segments is untrusted
	FG_NETWORK_ACTION_ALL_ON = FG_NETWORK_ACTION_AUDIT_ON | FG_NETWORK_ACTION_IPV4_REJECT_ON
								 | FG_NETWORK_ACTION_IPV6_REJECT_ON | FG_NETWORK_ACTION_OFFLINE_ON
} fg_network_action_switcher_type_t;

typedef enum fg_network_segment_type {
	FG_NETWORK_SEGMENT_TRUSTED = 0,
	FG_NETWORK_SEGMENT_UNTRUSTED = (1 << 0),
	FG_NETWORK_SEGMENT_OTHER_TRUSTED = (1 << 1),
	FG_NETWORK_SEGMENT_OTHER_UNTRUSTED = (1 << 2),
	FG_NETWORK_SEGMENT_OTHER_UNSET = (1 << 3),
	FG_NETWORK_SEGMENT_BOUNDARY = (1 << 4),
} fg_network_segment_type_t;

typedef enum fg_network_segment_policy {
	FG_NETWORK_SEGMENT_POLICY_OTHER_TRUSTED = 0,
	FG_NETWORK_SEGMENT_POLICY_OTHER_UNTRUSTED = (1 << 0),
	FG_NETWORK_SEGMENT_POLICY_OTHER_UNSET = (1 << 1), // relative trusted
} fg_network_segment_policy_t;

typedef enum fg_fs_storage_action_switcher_type {
	FG_USB_STORAGE_ACTION_ALL_OFF = 0,
	FG_USB_STORAGE_ACTION_AUDIT_ON = (1 << 0),
	FG_USB_STORAGE_ACTION_CONTROL_ON = (1 << 1),
	FG_USB_STORAGE_ACTION_ALL_ON = FG_USB_STORAGE_ACTION_AUDIT_ON | FG_USB_STORAGE_ACTION_CONTROL_ON,
	FG_FS_STORAGE_RENAME_AUDIT_ON = (1 << 2),
	FG_FS_STORAGE_COPY_AUDIT_ON = (1 << 3),
	FG_FS_STORAGE_VARIANT_AUDIT_ON = FG_FS_STORAGE_RENAME_AUDIT_ON | FG_FS_STORAGE_COPY_AUDIT_ON,
	FG_FS_STORAGE_NEWFILE_AUDIT_ON = (1 << 4),
	FG_FS_STORAGE_AUDIT_FILTER_ON = (1 << 5),
} fg_fs_storage_action_switcher_type_t;

typedef enum fg_fs_action_switcher_type {
	FG_FS_ACTION_ALL_OFF = 0,
	FG_FS_ACTION_AUDIT_ON = (1 << 0),
	FG_FS_ACTION_CONTROL_ON = (1 << 1),
	FG_FS_ACTION_SWITCHER_OFFLINE_ON = (1 << 2), // all file is kia, not enable now
	FG_FS_ACTION_ALL_ON = FG_FS_ACTION_AUDIT_ON | FG_FS_ACTION_CONTROL_ON | FG_FS_ACTION_SWITCHER_OFFLINE_ON
} fg_fs_action_switcher_type_t;

/* every upstream/downstream datagram starts with fg_message */
typedef struct fg_message {
	uint64_t timestamp; // timestamp in milliseconds since unix epoch
	uint8_t type; // message type, ref: fg_message_type
	uint8_t module; // message module, ref:fg_message_module_type
	uint8_t command; // command type, common commands defined in fg_command_type,
	uint8_t private_data; // moudle private data filed
	uint32_t size; // message size(include header) = sizeof(fg_message_t) + sizeof(data)
	char data[]; // message data
} __packed fg_message_t;

typedef struct fg_process_data {
	pid_t pid;
	uid_t uid;
} __packed fg_process_data_t;

typedef struct fg_path_data {
	fg_file_type_t type; // match method, ref fg_path_match_method
	char path[PATH_MAX - sizeof(uint32_t)]; // absolute path, "" for wildcard
} __packed fg_path_data_t;

typedef struct fg_action_data {
	uint32_t actions : 2; // actions, ref: fg_action_type
	uint32_t operations : 30; // operations, ref: fg_action_ops
} __packed fg_action_data_t;

typedef struct fg_audit_data {
	uint32_t operations; // fg_operaiton_type, ref: fg_operaiton_type
} __packed fg_audit_data_t;

typedef struct fg_action_switcher {
	uint32_t user_id;
	char tag_name[FG_MAX_TAG_NAME_LENGTH];
	uint32_t switcher;
} __packed fg_action_switcher_t;

typedef struct fg_int_value {
	uint32_t user_id;
	uint32_t value;
} __packed fg_int_value_t;

typedef struct fg_net_reject_cachetime {
	uint32_t cachetime;
} __packed fg_net_reject_cachetime_t;

#define get_rules_size(list_type, rule_type, rules)	(sizeof(list_type) + (rules)->count * sizeof(rule_type))

// process image file list
typedef struct fg_process_list {
	uint32_t count; // processes count
	fg_process_data_t processes[]; // processes list
} __packed fg_process_list_t;

typedef struct fg_ipc_block_cache {
	fg_process_data_t process;
	uint32_t user_mask;
	char file_path[PATH_MAX];
} __packed fg_ipc_block_cache_t;

typedef struct fg_uid_whitelist_rules_list {
	uint32_t uesr_id;
	uint32_t count;
	uid_t uids[];
} __packed fg_uid_whitelist_rules_list_t;

#define get_uid_whitelist_size(rules) get_rules_size(fg_uid_whitelist_rules_list_t, uid_t, rules)

/* network */
typedef struct fg_network_inet_data {
	uint16_t sa_family; // sockaddr family, must be AF_INET
	uint16_t proto; // protocol, eg: IPPROTO_TCP, IPPROTO_UDP; IPPROTO_IP for all IP Traffics
	uint16_t pf; // eg: NFPROTO_IPV4, NFPROTO_IPV6
	uint16_t src_port; // source port, big-endian, 0 for wildcard
	uint16_t dst_port; // destination port, big-endian, 0 for wildcard
	union {
		struct {
			uint32_t src_addr; // source ip address, big-endian, INADDR_ANY for wildcard
			uint32_t dst_addr; // destination ip address, big-endian, INADDR_ANY for wildcard
		} v4;
		struct {
			struct in6_addr src_addr;
			struct in6_addr dst_addr;
		} v6;
	};
} __packed fg_network_inet_data_t;

typedef struct fg_network_record {
	fg_process_data_t process; // 1234, syslogd
	fg_audit_data_t operation; // RECV
	fg_network_inet_data_t connection; // 1.2.3.4:123 -> 8.8.8.8:53 UDP
	// extra data
	uint32_t size;
	fg_path_data_t intercepted_kia_path;
	fg_process_data_t origin_process; // origin process information, used for associated process record
	action_status_t status;
	char data[];
} __packed fg_network_record_t;

typedef struct fg_network_rule {
	fg_process_data_t		process; // syslogd
	fg_action_data_t		action; // CONTROL, RECV_REJECT|SEND_REJECT|RECV_REDIRECT|SEND_REDIRECT
	fg_network_inet_data_t	connection; // 0.0.0.0:0 -> 0.0.0.0:53 UDP
	fg_network_inet_data_t	redirection; // 0.0.0.0:0 -> 0.0.0.0:53 UDP
} __packed fg_network_rule_t;

typedef struct fg_network_rules_list {
	uint32_t count; // rules list count
	fg_network_rule_t rules[]; // rules list
}__packed  fg_network_rules_list_t;
#define get_network_rules_size(rules) get_rules_size(fg_network_rules_list_t, fg_network_rule_t, rules)

typedef struct fg_network_segment {
	fg_network_segment_type_t  flag; // trusted or untrusted
	uint32_t start_addr; // 1.1.1.1
	uint32_t end_addr; // 4.4.4.4
} __packed fg_network_segment_t;

typedef struct fg_network_segment_rule_list {
	uint32_t count;
	fg_network_segment_t rules[];
} __packed fg_network_segment_rule_list_t;
#define get_network_segment_rules_size(rules) get_rules_size(fg_network_segment_rule_list_t, \
															fg_network_segment_t, rules)

typedef struct fg_network_segment_addr {
	uint32_t start_addr;
	uint32_t end_addr;
} __packed fg_network_segment_addr_t;

typedef struct fg_network_segment_boundary {
	uint32_t user_id;
	char tag_name[FG_MAX_TAG_NAME_LENGTH];
	uint32_t count;
	fg_network_segment_type_t flag; // trusted or untrusted
	fg_network_segment_addr_t segments[];
} __packed fg_network_segment_boundary_t;

#define get_network_segment_boundary_size(segments) \
	get_rules_size(fg_network_segment_boundary_t, fg_network_segment_addr_t, segments)

typedef struct fg_network_process_data {
	pid_t pid; // process pid, 0 for wildcard
	uint32_t method; // FG_PATH_MATCH_PROCESSPATH / FG_PATH_MATCH_PROCESSCONTAIN
	char exepath[512 - sizeof(pid_t)]; // absolute executable path, "" for wildcard
} __packed fg_network_process_data_t;

typedef struct fg_network_process_segment_rule {
	fg_network_process_data_t process;
	uint32_t count;
	fg_network_segment_t segments[];
} __packed fg_network_process_segment_rule_t;
#define get_process_segment_rule_size(rule)  get_rules_size(fg_network_process_segment_rule_t, \
															fg_network_segment_t, rule)

typedef struct fg_network_whitelist_rule {
	fg_path_data_t path_data; // match data
} __packed fg_network_whitelist_rule_t;

typedef struct fg_network_whitelist_rules_list {
	uint32_t count;
	fg_network_whitelist_rule_t rules[];
} __packed fg_network_whitelist_rules_list_t;
#define get_network_whitelist_rules_size(rules) get_rules_size(fg_network_whitelist_rules_list_t, \
															  fg_network_whitelist_rule_t, rules)

typedef fg_network_whitelist_rule_t fg_network_processlist_rule_t;
typedef fg_network_whitelist_rules_list_t fg_network_processlist_rules_list_t;

#define get_network_processlist_rules_size(rules) get_rules_size(fg_network_processlist_rules_list_t, \
																fg_network_processlist_rule_t, rules)

//////////////////////////////////////////////////////////////////////////
// filesystem
typedef struct fg_fs_keyword_rule {
	char keyword[NAME_MAX];
} __packed fg_fs_keyword_rule_t;

typedef struct fg_fs_keyword_rules_list {
	uint32_t user_id;
	uint32_t count;
	uint32_t keyword_count;
	uint32_t suffix_count;
	uint32_t compress_suffix_count;
	fg_fs_keyword_rule_t rules[];
} __packed fg_fs_keyword_rules_list_t;

#define get_fs_keyword_rules_size(rules) get_rules_size(fg_fs_keyword_rules_list_t, fg_fs_keyword_rule_t, rules)

#define MAX_COMM_NAME 16
typedef struct fg_fs_variant_record {
	uint32_t type;
	pid_t pid;
	uid_t uid;
	char pname[MAX_COMM_NAME];
	char src_file_path[PATH_MAX];
	char dst_file_path[PATH_MAX];
} __packed fg_fs_variant_record_t;

typedef struct {
	unsigned long event_id;
	unsigned int version;
	unsigned int content_len;
	char content[];
} __packed fg_security_event_info;

typedef struct fg_fs_operate_record {
	uint32_t type;
	pid_t pid;
	uid_t uid;
	char pname[MAX_COMM_NAME];
	char file_path[PATH_MAX];
} __packed fg_fs_operate_record_t;

typedef struct fg_fs_whitelist_rule {
	fg_path_data_t path_data; // match data
} __packed fg_fs_customlist_rule_t;

typedef struct fg_fs_whitelist_rules_list {
	uint32_t count;
	fg_fs_customlist_rule_t rules[];
} __packed fg_fs_customlist_rules_list_t;

typedef struct fg_associated_process_rule_list {
	fg_process_data_t origin_process;
	fg_process_list_t associated_processes;
} __packed fg_associated_process_rule_list_t;

typedef fg_fs_customlist_rule_t fg_fs_whitelist_rule_t;
typedef fg_fs_whitelist_rule_t fg_fs_kialist_rule_t;
typedef fg_fs_customlist_rules_list_t fg_fs_whitelist_rules_list_t;
typedef fg_fs_whitelist_rules_list_t fg_fs_kialist_rules_list_t;
typedef fg_fs_kialist_rule_t fg_fs_kiapermitlist_rule_t;
typedef fg_fs_kialist_rules_list_t fg_fs_kiapermitlist_rules_t;
typedef fg_fs_customlist_rule_t fg_fs_blacklist_rule_t;
typedef fg_fs_whitelist_rules_list_t fg_fs_blacklist_rules_list_t;

#define get_fs_whitelist_rules_size(rules)		get_rules_size(fg_fs_whitelist_rules_list_t, \
												fg_fs_whitelist_rule_t, rules)
#define get_fs_kialist_rules_size(rules)	get_rules_size(fg_fs_kialist_rules_list_t, fg_fs_kialist_rule_t, rules)
#define get_fs_kiapermitlist_rules_size(rules)	get_rules_size(fg_fs_kiapermitlist_rules_t, \
															  fg_fs_kiapermitlist_rule_t, rules)
#define get_fs_blacklist_rules_size(rules)		get_rules_size(fg_fs_blacklist_rules_list_t, \
															  fg_fs_blacklist_rule_t, rules)

//////////////////////////////////////////////////////////////////////////
// utils
enum {
	BLOCK_CACHE_KEYWORD = 0,
	BLOCK_CACHE_KIA_FILE,
	BLOCK_CACHE_FILE_TAG_BEGIN,
	BLOCK_CACHE_FILE_TAG1 = BLOCK_CACHE_FILE_TAG_BEGIN,
	BLOCK_CACHE_FILE_TAG2,
	BLOCK_CACHE_FILE_TAG3,
	BLOCK_CACHE_FILE_TAG4,
	BLOCK_CACHE_FILE_TAG5,
	BLOCK_CACHE_FILE_TAG_END = BLOCK_CACHE_FILE_TAG5,
	BLOCK_CACHE_MAX
};

enum {
	FG_BLOCK_RULE_INVALID = 0,
	FG_BLOCK_RULE_NET_VALID = (1 << 0),
	FG_BLOCK_RULE_USB_VALID = (1 << 1),
	FG_BLOCK_RULE_VALID = FG_BLOCK_RULE_NET_VALID | FG_BLOCK_RULE_USB_VALID,
};

typedef struct block_rule_profile {
	fg_process_data_t process;
	fg_path_data_t file_path[BLOCK_CACHE_MAX];
	uint64_t time_stamp[BLOCK_CACHE_MAX];
	uint32_t status[BLOCK_CACHE_MAX];
} block_rule_profile_t;

typedef struct effective_block_rule {
	block_rule_profile_t block_rule;
	fg_file_type_t first_matched_type;
	uint32_t first_matched_idx;
} effective_block_rule_t;

#define for_each_rule(rules_list, rule) \
	for ((rule) = (rules_list)->rules; (rule) - (rules_list)->rules < (rules_list)->count; (rule)++)
#define current_rule_index(rules_list, rule) ((rule) - (rules_list)->rules)

#define get_message_data(message, type) ((type*)(message)->data)
#define get_message_data_size(message) ((message)->size - sizeof(fg_message_t))
#define set_message_data_size(message, datasize) ((message)->size = sizeof(fg_message_t) + (datasize))
#define set_message_timestamp(message, ts) ((message)->timestamp = (ts).tv_sec * 1000UL + (ts).tv_nsec / 1000000UL)

#define init_message_data(message, msgtype, mod, cmd, dataclass) ({	\
	(message)->type = (msgtype);									\
	(message)->module = (mod);										\
	(message)->command = (cmd);										\
	set_message_data_size((message), sizeof(dataclass));			\
	get_message_data((message), dataclass);							\
})

static inline const char *get_message_name(uint32_t type)
{
	switch (type) {
	case FG_COMMAND_MESSAGE:
		return "command";
	case FG_DATA_MESSAGE:
		return "data";
	default:
		return "undefined";
	}
}

static inline const char *get_module_name(uint32_t module)
{
	switch (module) {
	case FG_IPC:
		return "ipc";
	case FG_NETWORK:
		return "net";
	case FG_FSWORK:
		return "fs";
	default:
		return "undefined";
	}
}

static inline const char *get_command_name(uint32_t cmd)
{
	switch (cmd) {
	case FG_CMD_COMMON_TOGGLE:
		return "common toggle";
	case FG_CMD_COMMON_RULE:
		return "common rule";
	case FG_CMD_COMMON_REJECT_RECORD:
		return "reject record";
	case FG_CMD_MODULE_START:
		return "start module";
	case FG_CMD_IPC_PROCESS_LIST:
		return "ipc process list";
	case FG_CMD_FSWORK_ACTION_SWITCHER:
		return "fs action switcher";
	case FG_CMD_FS_KEYWORD_LIST_RULE:
		return "fs kia keyword list";
	case FG_CMD_NETWORK_ACTION_SWITCHER:
		return "net action switcher";
	case FG_CMD_NETWORK_REJECT_CACHETIME:
		return "net process reject cachetime";
	case FG_CMD_NETWORK_SEGMENT_BOUNDARY:
		return "net segment boundary";
	case FG_CMD_NETWORK_SEGMENT_DEFAULT_POLICY:
		return "net segment other";
	case FG_CMD_ASSOCIATED_PROCESS_LIST_RULE:
		return "associated processes list";
	case FG_CMD_ASSOCIATED_PROCESS_CLEAR_RULE:
		return "clear associated processes";
	case FG_CMD_IPC_KIA_BLOCK_CACHE:
		return "ipc kia block cache";
	case FG_CMD_IPC_TRUSTED_UID_LIST_RULE:
		return "trusted uid list";
	default:
		return "undefined";
	}
}

static inline const char *get_operation_name(uint32_t operation)
{
	if (operation == 0)
		return "nop";
	if (operation & FG_FILE_OPEN)
		return "open";
	if (operation & FG_FILE_CLOSE)
		return "close";
	if (operation & FG_FILE_READ)
		return "read";
	if (operation & FG_FILE_WRITE)
		return "write";
	if (operation & FG_FILE_REMOVE)
		return "remove";
	if (operation & FG_FILE_RENAME)
		return "rename";
	return "undefined";
}

static inline const char *get_action_name(fg_action_type_t action)
{
	switch ((uint32_t)action & 0x3) {
	case FG_ACTION_NONE:
		return "none";
	case FG_ACTION_AUDIT:
		return "audit";
	case FG_ACTION_CONTROL:
		return "control";
	case FG_ACTION_ALL:
		return "all";
	default:
		return "undefined";
	}
}

static inline const char *get_action_status_name(action_status_t status)
{
	switch (status) {
	case PASSED:
		return "PASSED";
	case REJECTED:
		return "REJECTED";
	case KIA_INTERCEPTED:
		return "KIA_INTERCEPTED";
	case KIA_ASSOCIATED:
		return "KIA_ASSOCIATED";
	default:
		return "UNDEFINED";
	}
}

static inline bool has_valid_tag(const fg_action_switcher_t *switcher)
{
	return switcher->tag_name[0] != '\0';
}

static inline bool is_file_type_tag(fg_file_type_t type)
{
	return (FG_FILE_TAG_BEGIN <= type) && (type <= FG_FILE_TAG_END);
}

static inline bool is_block_cache_type_tag(uint32_t type)
{
	return (BLOCK_CACHE_FILE_TAG_BEGIN <= type) && (type <= BLOCK_CACHE_FILE_TAG_END);
}

#endif // SYSMGR_INCLUDE_INTAPI_FG_STRUCT_DEF_H
