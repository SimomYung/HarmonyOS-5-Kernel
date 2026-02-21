/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Interface for hguard service event
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 10:11:19 2020
 */
#include <hmkernel/const.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>

#include <libhguard/hguard.h>
#include <libhguard/request.h>
#include <libsysif/hguard/api.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_signal.h>

static rref_t hguard_rref = 0ULL;
static struct raw_mutex hguard_mutex = RAW_MUTEX_INITIALIZER;

static int hguard_acquire_rref(void)
{
	int err = E_HM_OK;
	raw_mutex_lock(&hguard_mutex);
	if (IS_REF_ERR(hguard_rref)) {
		err = hm_path_acquire(HGUARD_SERVER_PATH, &hguard_rref);
	}
	raw_mutex_unlock(&hguard_mutex);
	return err;
}

/* Max factor(ms) for delay backoff delay */
#define HGUARD_ACTVCALL_BACKOFF_MAX   64
/* Exponential radix for increasing backoff delay */
#define HGUARD_ACTVCALL_BACKOFF_RADIX 2

static inline int __hguard_actvcall_backoff(int backoff)
{
	struct timespec req = {0};

	/* backoff(ms) must be smaller than 1000 */
	if (backoff >= MSEC_PER_SEC - 1) {
		backoff = MSEC_PER_SEC - 1;
	}
	req.tv_nsec = NSEC_PER_MSEC * backoff;
	return hm_nanosleep(&req, NULL);
}

/*
 * Send notify with exponential backoff
 */
static int __hguard_notify(unsigned int type)
{
	int err = E_HM_OK;
	int backoff = 1;
	int delay_err;

	while (backoff < HGUARD_ACTVCALL_BACKOFF_MAX) {
		err = actvcapcall_hgcall_service_notify(hguard_rref, type);
		if (err == E_HM_CNODE_WRONG_IDX) {
			/* the cached rref is invalid, re-acquire it */
			err = hm_path_acquire(HGUARD_SERVER_PATH, &hguard_rref);
			if (err < 0) {
				break;
			}
		} else if (err == E_HM_ACTIVATIONPOOL_NORES) {
			/* err == NORES, backoff it to not compet others */
			/* Backoff via nanosleep, abort if fail/killed */
			delay_err = __hguard_actvcall_backoff(backoff);
			if (delay_err != E_HM_OK) {
				break;
			}
		} else {
			/* ok or other errors, no retry */
			break;
		}

		backoff *= HGUARD_ACTVCALL_BACKOFF_RADIX;
	}
	return err;
}

static unsigned int __ulib_notify_type_to_hguard_type(unsigned int type)
{
	enum hguard_notify_type ret;
	if (type == NOTIFY_HEARTBEAT) {
		ret = HGUARD_NOTIFY_HEARTBEAT;
	} else if (type == NOTIFY_READY) {
		ret = HGUARD_NOTIFY_READY;
	} else {
		ret = HGUARD_NOTIFY_INVALID;
	}
	return (unsigned int)ret;
}

int hguard_notify(unsigned int ulib_type)
{
	int ret = E_HM_OK;
	unsigned int type = __ulib_notify_type_to_hguard_type(ulib_type);
	if (type != HGUARD_NOTIFY_HEARTBEAT && type != HGUARD_NOTIFY_READY) {
		/* notify other than heartbeat deprecated */
		ret = E_HM_OPNOTSUPP;
	}

	if (ret == E_HM_OK) {
		ret = hguard_acquire_rref();
	}

	if (ret == E_HM_OK) {
		ret = __hguard_notify(type);
	}

	return ret;
}

int hguard_process_exit(rref_t rref, pid_t pid, int status)
{
	int err = E_HM_OK;
	int backoff = 1;
	int delay_err;

	while (backoff < HGUARD_ACTVCALL_BACKOFF_MAX) {
		err = actvcapcall_hgcall_service_process_exit(rref, pid, status);
		if (err == E_HM_ACTIVATIONPOOL_NORES) {
			delay_err = __hguard_actvcall_backoff(backoff);
			if (delay_err != E_HM_OK) {
				break;
			}
		} else {
			/* ok or other errors, no retry */
			break;
		}

		backoff *= HGUARD_ACTVCALL_BACKOFF_RADIX;
	}
	return err;
}

static inline bool is_need_reply_from_hguard(enum hguard_action_type action_type)
{
	return (action_type == HGUARD_ACTION_STATUS ||
		action_type == HGUARD_ACTION_DUMP_STARTUP_TIME) ? true : false;
}

struct service_start_config {
	rref_t server;
	struct hguard_request arg;
};

static int service_start_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct service_start_config *config = NULL;
	config = (struct service_start_config *)data;
	return actvcapcall_hgcall_service_start(config->server, config->arg, *attr);
}

static int __actvcapcall_hgcall_common_with_bunch(struct hguard_request arg,
						  struct bunch *bunch)
{
	int err;
	struct service_start_config config;
	struct bunch_ipc_reader_ctx ctx;

	mem_zero_s(config);
	config.server = hguard_rref;
	config.arg = arg;

	bunch_ipc_ctx_init(&ctx, hguard_rref, service_start_ipc, &config);
	err = bunch_ipc_request(bunch, &ctx);
	if (err < 0) {
		hm_warn("bunch ipc try request data with shm failed: %s\n",
			hmstrerror(err));
	}

	return err;
}

static int __hguard_service_request_by_name(const char *name,
					    enum hguard_action_type action_type,
					    struct bunch *bunch)
{
	int err = E_HM_OK;
	struct hguard_request arg = {
		.name = {0}, .action = action_type, .buf = {0}
	};
	struct bunch_ipc_attr attr = {0};

	err = hguard_acquire_rref();
	if (name != NULL) {
		if (err == E_HM_OK) {
			err = strncpy_s(arg.name, sizeof(arg.name),
					name, strlen(name));
			if (err == ERANGE_AND_RESET) {
				err = E_HM_NAMETOOLONG;
			} else {
				err = (err == EOK) ? E_HM_OK : E_HM_RESFAULT;
			}
		}
	}
	if (err == E_HM_OK) {
		if (is_need_reply_from_hguard(action_type) && bunch != NULL) {
			err = __actvcapcall_hgcall_common_with_bunch(arg, bunch);
		} else {
			err = actvcapcall_hgcall_service_start(hguard_rref, arg, attr);
		}
	}

	return err;
}

#define DEF_HGUARD_API(func, action_type)				   \
int func(const char *name)						   \
{									   \
	return __hguard_service_request_by_name(name, action_type, NULL);  \
}

#define DEF_HGUARD_API_WITH_BUNCH(func, action_type)			   \
int func(const char *name, struct bunch *bunch)				   \
{									   \
	return __hguard_service_request_by_name(name, action_type, bunch); \
}

DEF_HGUARD_API(hguard_service_start, HGUARD_ACTION_START)
DEF_HGUARD_API(hguard_service_stop, HGUARD_ACTION_STOP)
DEF_HGUARD_API(hguard_service_restart, HGUARD_ACTION_RESTART)
DEF_HGUARD_API(hguard_service_reload, HGUARD_ACTION_RELOAD)
DEF_HGUARD_API_WITH_BUNCH(hguard_service_status, HGUARD_ACTION_STATUS)
DEF_HGUARD_API_WITH_BUNCH(hguard_service_dump_startup_time, HGUARD_ACTION_DUMP_STARTUP_TIME)

static int  __hguard_service_request(enum hguard_action_type action_type)
{
	int err = E_HM_OK;
	struct hguard_request arg = {
		.name = {0}, .action = action_type, .buf = {0}
	};
	struct bunch_ipc_attr attr = {0};

	err = hguard_acquire_rref();
	if (err == E_HM_OK) {
		err = actvcapcall_hgcall_service_start(hguard_rref, arg, attr);
	}
	return err;
}

int hguard_daemon_reload(void)
{
	return __hguard_service_request(HGUARD_ACTION_DAEMON_RELOAD);
}

int hguard_reboot(int flag)
{
	enum hguard_action_type action;
	if (flag == HGUARD_REBOOT_FLAG_FORCE) {
		action = HGUARD_ACTION_REBOOT_FORCE;
	} else {
		action = HGUARD_ACTION_REBOOT;
	}
	return __hguard_service_request(action);
}

int hguard_deliver_start(void)
{
	return __hguard_service_request(HGUARD_ACTION_DELIVER_START);
}

static int hguard_subscribe_internal_exec(const struct hguard_subscribe_request *req)
{
	int err = E_HM_OK;

	err = hguard_acquire_rref();
	if (err == E_HM_OK) {
		err = actvcapcall_hgcall_service_subscribe(hguard_rref, *req);
	}

	return err;
}

int hguard_subscribe_record_exit(void)
{
	int err = E_HM_OK;
	struct hguard_subscribe_request req;
	mem_zero_s(req);
	req.action = HGUARD_SUBSCRIBE_RECORD;

	err = hguard_subscribe_internal_exec(&req);
	return err;
}

int hguard_subscribe_register(int signum)
{
	int err = E_HM_OK;
	struct hguard_subscribe_request req = {
		HGUARD_SUBSCRIBE_REGISTER,
		{ .signum = signum }
	};

	if (signum < 0 || signum > (int)SIGNAL_MAX_RT) {
		err = E_HM_RANGE;
	}
	if (err == E_HM_OK) {
		err = hguard_subscribe_internal_exec(&req);
	}
	return err;
}

int hguard_subscribe_fetchall(const struct hguard_exit_data *buf, int buf_size)
{
	int err = E_HM_OK;
	struct hguard_subscribe_request req = {
		HGUARD_SUBSCRIBE_FETCHALL,
		{ .buf = { .ptr = ptr_to_void(buf), .size = buf_size } }
	};

	if (buf == NULL || buf_size <= 0) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = hguard_subscribe_internal_exec(&req);
	}
	return err;
}

int hguard_subscribe_clear(int pid)
{
	int err = E_HM_OK;
	struct hguard_subscribe_request req = {
		HGUARD_SUBSCRIBE_CLEAR,
		{ .pid = pid }
	};

	err = hguard_subscribe_internal_exec(&req);
	return err;
}

int hguard_service_socket_all_names_of(char *buf, int buf_size)
{
	int err = E_HM_OK;
	struct hguard_request arg = {
		.name = {0},
		.action = HGUARD_ACTION_ALL_SOCKET_SERVICE_NAMES,
		.buf = { .ptr = ptr_to_void(buf), .size = buf_size }
	};
	struct bunch_ipc_attr attr = {0};

	if (buf == NULL || buf_size <= 0) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = hguard_acquire_rref();
	}
	if (err == E_HM_OK) {
		err = actvcapcall_hgcall_service_start(hguard_rref, arg, attr);
	}

	return err;
}

#define SERVICE_CONF_SUFFIX	".service"

static int  __hguard_service_request_by_name_and_buf(const char *name,
					    enum hguard_action_type action_type,
					    char *buf, int buf_size)
{
	int err = E_HM_OK;
	struct hguard_request arg = {
		.name = {0},
		.action = action_type,
		.buf = { .ptr = ptr_to_void(buf), .size = buf_size }
	};
	struct bunch_ipc_attr attr = {0};

	if (name != NULL && (strlen(name) > strlen(SERVICE_CONF_SUFFIX))) {
		char *c = strrchr(name, (int)'.');
		if (c == NULL || strcmp(c, SERVICE_CONF_SUFFIX) != 0) {
			err = E_HM_INVAL;
		}
	} else {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (buf == NULL || buf_size <= 0) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		err = strncpy_s(arg.name, sizeof(arg.name), name, strlen(name));
		if (err == ERANGE_AND_RESET) {
			err = E_HM_NAMETOOLONG;
		} else {
			err = (err == EOK) ? E_HM_OK : E_HM_RESFAULT;
		}
	}

	if (err == E_HM_OK) {
		err = hguard_acquire_rref();
	}
	if (err == E_HM_OK) {
		err = actvcapcall_hgcall_service_start(hguard_rref, arg, attr);
	}

	return err;
}

int hguard_service_state_of(const char *service_name, char *buf, int buf_size)
{
	return __hguard_service_request_by_name_and_buf(service_name,
						 HGUARD_ACTION_SERVICE_STATE, buf, buf_size);
}

int hguard_service_memory_current_of(const char *service_name, char *buf, int buf_size)
{
	return __hguard_service_request_by_name_and_buf(service_name,
						 HGUARD_ACTION_SERVICE_MEMORY_CURRENT, buf, buf_size);
}

int hguard_procfs_get_config(int config_id, char *config_buf, int config_size)
{
	int ret;

	struct hguard_config_arg args = {
		.id = config_id,
		.buf = config_buf,
		.size = config_size,
	};
	ret = hguard_acquire_rref();
	if (ret == E_HM_OK) {
		ret = actvcapcall_hgcall_procfs_get_hungtask(hguard_rref, args);
	}

	return ret;
}

int hguard_procfs_set_config(int config_id, char *config_buf, int config_size)
{
	int ret;

	struct hguard_config_arg args = {
		.id = config_id,
		.buf = config_buf,
		.size = config_size,
	};

	ret = hguard_acquire_rref();
	if (ret == E_HM_OK) {
		ret = actvcapcall_hgcall_procfs_set_hungtask(hguard_rref, args);
	}

	return ret;
}

int hguard_procfs_stop_watchdog_for_reboot(unsigned int timeout_sec)
{
	int ret;

	if (timeout_sec == 0U) {
		ret = E_HM_INVAL;
	} else {
		ret = hguard_acquire_rref();
	}

	if (ret == E_HM_OK) {
		ret = actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(hguard_rref, timeout_sec);
	}

	return ret;
}
