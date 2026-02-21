/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide hmtrace control API for hmtrace.elf
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <libhmtrace/hmtracectrl.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

#include <libsysif/hmtracemgr/server.h>
#include "hmtrace/core/trace_iter.h"
#include <libhmtrace/hmtrace.h>

#ifdef CONFIG_HMTRACE

#define SYSMGR_UEV_LIST_BUNCH_SIZE (0x1U << 18) /* 256K */

enum name_type {
	HMTRACE_CLASS,
	HMTRACE_EVENT,
};

struct hmtrace_ipc_data {
	struct hmtrace_data *class_data;
	rref_t xact_rref;
};

static int write_buffer(char *buffer, unsigned int max_len,
			unsigned int *offset, const char *name)
{
	int rc;
	size_t len;

	len = strlen(name);
	if (*offset + len + 1 > max_len) {
		hm_warn("buffer too small\n");
		return E_HM_INVAL;
	}

	rc = strncpy_s(buffer + (*offset), max_len - (*offset), name, len);
	if (rc != 0) {
		hm_warn("strcpy name failed\n");
		return E_HM_POSIX_FAULT;
	}
	*offset += (unsigned int)len;

	return E_HM_OK;
}

static int buffer_acquire_from_bunch(struct bunch *bunch, const char **buffer)
{
	char *buf = NULL;
	unsigned int total_size = 0;
	unsigned int total_len = 0;
	unsigned int used_len = 0;
	int rc = E_HM_OK;

	total_size = (unsigned int)bunch_read_int32(bunch);
	total_len = (unsigned int)bunch_read_int32(bunch);
	if ((total_size == 0 && total_len != 0) || (total_size != 0 && total_len == 0)) {
		rc = E_HM_INVAL;
	} else if (total_size == 0 || total_len == 0) {
		total_len = 1;
	}

	if (rc == E_HM_OK) {
		buf = (char *)malloc(total_len);
		if (buf == NULL) {
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		for (unsigned int idx = 1; idx <= total_size; idx++ ) {
			const char *str = NULL;
			str = bunch_read_string(bunch);
			rc = write_buffer(buf, total_len, &used_len, str);
			if (rc != E_HM_OK) {
				free(buf);
				break;
			}
			buf[used_len++] = ' ';
		}
	}

	if (rc == E_HM_OK) {
		if (used_len == 0) {
			buf[used_len] = '\0';
		} else {
			buf[used_len - 1] = '\0';
		}
		*buffer = buf;
	}

	return rc;
}

static int get_tracemgr_rref(pid_t pid, rref_t *rref, char *buf, size_t buf_size)
{
	int rc;

	if (pid < 0) {
		hm_warn("hmrace input pid invalid\n");
		return E_HM_INVAL;
	}

	rc = snprintf_s(buf, buf_size,
			buf_size - 1,
			HMTRACEMGR_PATH_FMT, pid);
	if (rc < 0) {
		hm_warn("hmtrace sprintf path failed\n");
		return E_HM_POSIX_FAULT;
	}

	rc = hm_path_acquire(buf, rref);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace acquire path rref failed: %s\n",
			hmstrerror(rc));
		return rc;
	}

	return E_HM_OK;
}

static void shmem_clean(size_t shm_len, int shm_id, void *shm_buf)
{
	(void)hm_mem_munmap(shm_buf, shm_len);
	(void)hm_ashm_close(shm_id);
	(void)hm_ashm_unlink(shm_id);
}

static int shmem_create(size_t shm_len, rref_t rref, uint64_t *ret_key, void **ret_buf, int *id)
{
	int ret;
	int shm_id = 0;
	void *buf = NULL;
	uint64_t key = 0;
	xref_t xref;

	shm_id = hm_ashm_open_auth(0, 0U, shm_len, SHM_O_CREAT | SHM_O_RDWR, NULL);
	if (shm_id < 0) {
		hm_warn("client failed to open ashm\n");
		return E_HM_POSIX_FAULT;
	} else {
		buf = hm_mem_mmap(NULL, shm_len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, shm_id, 0);
		if (buf == MAP_FAILED) {
			hm_error("client failed to mmap\n");
			(void)hm_ashm_close(shm_id);
			return E_HM_POSIX_FAULT;
		}
	}

	xref.rref = rref;
	ret = hm_ashm_grant(shm_id, SHM_GRANT_RDWR, xref, &key);
	if (ret < 0) {
		hm_warn("hm_ashm_grant failed\n");
		shmem_clean(shm_len, shm_id, buf);
	}

	if (ret == E_HM_OK) {
		*ret_key = key;
		*ret_buf = buf;
		*id = shm_id;
	}

	return ret;
}

static int shmem_read(size_t shm_len, const void *shm_buf, void *ret_buf)
{
	int ret = E_HM_OK;

	if (memcpy_s(ret_buf, shm_len, shm_buf, shm_len) != 0) {
		hm_warn("copy shm_buf failed\n");
		ret = E_HM_POSIX_FAULT;
	}

	return ret;
}

static int __hmtrace_get_registered_class_list_ipc(struct bunch_ipc_attr *attr, void *data)
{
	rref_t server = *((rref_t *)data);
	return actvxactcapcall_hmtracecall_hmtrace_get_registered_class_list(false, true,
									     server, *attr);
}

int hm_trace_get_registered_class_list_by_pid(pid_t pid, const char **buffer)
{
	int rc;
	int err;
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	struct bunch_ipc_reader_ctx ctx;
	struct bunch *bunch = NULL;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	mem_zero_s(ctx);
	mem_zero_a(path_name);

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		bunch = bunch_ipc_create(SYSMGR_UEV_LIST_BUNCH_SIZE);
		if (bunch == NULL) {
			rc = E_HM_POSIX_FAULT;
			hm_warn("bunch_ipc_create failed\n");
		}
	}

	if (rc == E_HM_OK) {
		ctx.ipc = __hmtrace_get_registered_class_list_ipc;
		ctx.private_data = ptr_to_void(&rref);
		ctx.target_info = rref;

		rc = bunch_ipc_request(bunch, &ctx);
		if (rc != E_HM_OK) {
			hm_warn("bunch_ipc_request failed:%s\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		rc = buffer_acquire_from_bunch(bunch, buffer);
		if (rc != E_HM_OK) {
			hm_warn("buffer_acquire_from_bunch failed, ret=%s\n",
				hmstrerror(rc));
		}
	}

	/* allow NULL bunch pointer */
	bunch_ipc_destroy(bunch);

	if (rref != ERR_TO_REF(E_HM_INVAL)) {
		/* using err only to check warnings */
		err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("hm_path_release failed, ret=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

static int actvcall_prepare(const char *name, enum name_type type,
			    char *dest_buffer, unsigned int buffer_size)
{
	int rc;
	size_t max_name_len;
	char *type_str = NULL;

	if (type == HMTRACE_CLASS) {
		type_str = "class";
		max_name_len = HMTRACE_MAX_CLASS_NAME_SIZE;
	} else if (type == HMTRACE_EVENT) {
		type_str = "event";
		max_name_len = HMTRACE_MAX_EVENT_NAME_SIZE_RESTRICTED;
	} else {
		hm_warn("invalid name_type\n");
		return E_HM_INVAL;
	}

	if ((name == NULL) || (strlen(name) == 0)) {
		hm_warn("hmtrace access %s must have a name\n",
			type_str);
		return E_HM_INVAL;
	}

	if (strlen(name) + 1 > max_name_len) {
		hm_warn("%s name beyond the max name len\n", type_str);
		return E_HM_INVAL;
	}

	rc = strncpy_s(dest_buffer, buffer_size, name, strlen(name));
	if (rc != 0) {
		hm_warn("hmtrace copy %s name failed\n", type_str);
		return E_HM_POSIX_FAULT;
	}

	return E_HM_OK;
}

static int __hmtrace_get_registered_events_list_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct hmtrace_ipc_data *ipc_data = ptr_from_void(data, struct hmtrace_ipc_data);
	return actvxactcapcall_hmtracecall_hmtrace_get_registered_events_list(false, true,
		      ipc_data->xact_rref, *ipc_data->class_data, *attr);
}

int hm_trace_get_registered_events_list_by_pid(pid_t pid, const char *class_name,
					       const char **buffer)
{
	int rc;
	int err;
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	struct bunch *bunch = NULL;
	struct hmtrace_data class_data;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		rc = actvcall_prepare(class_name, HMTRACE_CLASS,
				      class_data.buffer, sizeof(class_data.buffer));
	}

	if (rc == E_HM_OK) {
		bunch = bunch_ipc_create(0);
		if (bunch == NULL) {
			rc = E_HM_POSIX_FAULT;
			hm_warn("bunch_ipc_create failed\n");
		}
	}

	if (rc == E_HM_OK) {
		struct hmtrace_ipc_data ipc_data;
		struct bunch_ipc_reader_ctx ctx;
		mem_zero_s(ipc_data);
		mem_zero_s(ctx);

		ipc_data.class_data = &class_data;
		ipc_data.xact_rref = rref;
		ctx.ipc = __hmtrace_get_registered_events_list_ipc;
		ctx.private_data = ptr_to_void(&ipc_data);
		ctx.target_info = rref;

		rc = bunch_ipc_request(bunch, &ctx);
		if (rc != E_HM_OK) {
			hm_warn("bunch_ipc_request failed:%s\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		rc = buffer_acquire_from_bunch(bunch, buffer);
		if (rc != E_HM_OK) {
			hm_warn("buffer_acquire_from_bunch failed, ret=%s\n",
				hmstrerror(rc));
		}
	}

	bunch_ipc_destroy(bunch);
	if (rref != ERR_TO_REF(E_HM_INVAL)) {
		err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

static int __hmtrace_get_available_events_list_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct hmtrace_ipc_data *ipc_data = ptr_from_void(data, struct hmtrace_ipc_data);
	return actvxactcapcall_hmtracecall_hmtrace_get_available_events_list(false, true,
		      ipc_data->xact_rref, *ipc_data->class_data, *attr);
}

int hm_trace_get_available_events_list_by_pid(pid_t pid, const char *class_name,
					      const char **buffer)
{
	int rc;
	int err;
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	struct bunch *bunch = NULL;
	struct hmtrace_data class_data;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		rc = actvcall_prepare(class_name, HMTRACE_CLASS,
				      class_data.buffer, sizeof(class_data.buffer));
	}

	if (rc == E_HM_OK) {
		bunch = bunch_ipc_create(SYSMGR_UEV_LIST_BUNCH_SIZE);
		if (bunch == NULL) {
			rc = E_HM_POSIX_FAULT;
			hm_warn("bunch_ipc_create failed\n");
		}
	}

	if (rc == E_HM_OK) {
		struct hmtrace_ipc_data ipc_data;
		struct bunch_ipc_reader_ctx ctx;
		mem_zero_s(ipc_data);
		mem_zero_s(ctx);

		ipc_data.class_data = &class_data;
		ipc_data.xact_rref = rref;
		ctx.ipc = __hmtrace_get_available_events_list_ipc;
		ctx.private_data = ptr_to_void(&ipc_data);
		ctx.target_info = rref;

		rc = bunch_ipc_request(bunch, &ctx);
		if (rc != E_HM_OK) {
			hm_warn("bunch_ipc_request failed:%s\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		rc = buffer_acquire_from_bunch(bunch, buffer);
		if (rc != E_HM_OK) {
			hm_warn("buffer_acquire_from_bunch failed, ret=%s\n",
				hmstrerror(rc));
		}
	}

	bunch_ipc_destroy(bunch);
	if (rref != ERR_TO_REF(E_HM_INVAL)) {
		err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

static int do_register_event_by_pid(pid_t pid, const char *class_name,
				    const void *buffer, unsigned int buffer_size,
				    bool is_register_event)
{
	int rc = E_HM_OK;
	rref_t rref = ERR_TO_REF(E_HM_INVAL);
	uint64_t key = 0ULL;
	int shm_id = 0;
	void *shm_buf = NULL;
	struct hmtrace_data class_data;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		rc = shmem_create((size_t)buffer_size, rref, &key, &shm_buf, &shm_id);
	}

	if ((rc == E_HM_OK) && (shm_buf != NULL)) {
		if (memcpy_s(shm_buf, buffer_size, buffer, buffer_size) != 0) {
			hm_warn("copy shm_buf failed\n");
			rc = E_HM_POSIX_FAULT;
		}
	}

	if (rc == E_HM_OK) {
		rc = actvcall_prepare(class_name, HMTRACE_CLASS,
				      class_data.buffer, sizeof(class_data.buffer));
	}
	if (rc == E_HM_OK) {
		if (is_register_event) {
			rc = actvcapcall_hmtracecall_hmtrace_register_event(rref, class_data,
									    key);
		} else {
			rc = actvcapcall_hmtracecall_hmtrace_unregister_event(rref, class_data,
									      key);
		}
	}

	if (shm_buf != NULL) {
		shmem_clean((size_t)buffer_size, shm_id, shm_buf);
	}

	if (rref != ERR_TO_REF(E_HM_INVAL)) {
		int err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

int hm_trace_register_event_by_pid(pid_t pid, const char *class_name,
				   const void *buffer, unsigned int buffer_size)
{
	if ((buffer == NULL) || (buffer_size == 0)) {
		hm_warn("hmtrace register event buffer or buffer_size invalid\n");
		return E_HM_INVAL;
	}

	return do_register_event_by_pid(pid, class_name, buffer, buffer_size, true);
}

int hm_trace_unregister_event_by_pid(pid_t pid, const char *class_name,
				     const void *buffer, unsigned int buffer_size)
{
	if ((buffer == NULL) || (buffer_size == 0)) {
		hm_warn("hmtrace unregister event buffer or buffer_size invalid\n");
		return E_HM_INVAL;
	}

	return do_register_event_by_pid(pid, class_name, buffer, buffer_size, false);
}

int hm_trace_set_event_tracing_on_by_pid(pid_t pid, const char *class_name,
					 const char *event_name)
{
	int rc;
	int ret;
	rref_t rref;
	struct hmtrace_data class_data;
	struct hmtrace_eventname event_data;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc != E_HM_OK) {
		return rc;
	}

	mem_zero_s(class_data);
	mem_zero_s(event_data);
	rc = actvcall_prepare(class_name, HMTRACE_CLASS,
			      class_data.buffer, sizeof(class_data.buffer));
	if (rc == E_HM_OK && event_name != NULL) {
		rc = actvcall_prepare(event_name, HMTRACE_EVENT,
				      event_data.buffer, sizeof(event_data.buffer));
	}
	if (rc == E_HM_OK) {
		rc = actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(rref,
									  class_data, event_data);
	}

	ret = hm_path_release(path_name, rref);
	if (ret != E_HM_OK) {
		hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(ret));
	}

	return rc;
}

int hm_trace_set_event_tracing_off_by_pid(pid_t pid, const char *class_name,
					  const char *event_name)
{
	int rc;
	int ret;
	rref_t rref;
	struct hmtrace_data class_data;
	struct hmtrace_eventname event_data;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	mem_zero_s(class_data);
	mem_zero_s(event_data);
	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc != E_HM_OK) {
		return rc;
	}

	rc = actvcall_prepare(class_name, HMTRACE_CLASS,
			      class_data.buffer, sizeof(class_data.buffer));
	if (rc == E_HM_OK && event_name != NULL) {
		rc = actvcall_prepare(event_name, HMTRACE_EVENT,
				      event_data.buffer, sizeof(event_data.buffer));
	}

	if (rc == E_HM_OK) {
		rc = actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(rref,
				class_data, event_data);
	}

	ret = hm_path_release(path_name, rref);
	if (ret != E_HM_OK) {
		hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(ret));
	}

	return rc;
}

int hm_trace_get_event_tracing_status_by_pid(pid_t pid,
					     const char *class_name,
					     const char *event_name,
					     int *status)
{
	int rc;
	int err;
	rref_t rref;
	struct hmtrace_data class_data;
	struct hmtrace_eventname event_data;
	struct  __actvret_hmtracecall_hmtrace_get_event_tracing_status ret;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	mem_zero_s(ret);
	if (status == NULL) {
		hm_warn("hmtrace server input status is NULL\n");
		return E_HM_INVAL;
	}

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc != E_HM_OK) {
		return rc;
	}

	rc = actvcall_prepare(class_name, HMTRACE_CLASS,
			      class_data.buffer, sizeof(class_data.buffer));
	if (rc == E_HM_OK) {
		rc = actvcall_prepare(event_name, HMTRACE_EVENT,
				      event_data.buffer, sizeof(event_data.buffer));
	}

	if (rc == E_HM_OK) {
		rc = actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(rref,
				class_data, event_data, &ret);
		if (rc != E_HM_OK) {
			hm_warn("hmtrace server get event status failed: %s\n",
				 hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		*status = ret.status;
	}

	err = hm_path_release(path_name, rref);
	if (err != E_HM_OK) {
		hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(err));
	}

	return rc;
}

int hm_trace_tracing_on_all_events(pid_t pid, unsigned int status)
{
	int rc = E_HM_OK;
	int ret;
	rref_t rref = 0;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];
	int need_release = 0;

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		need_release = 1;
		rc = actvcapcall_hmtracecall_hmtrace_events_tracing_on(rref, status);
	}

	if (need_release == 1) {
		ret = hm_path_release(path_name, rref);
		if (ret != E_HM_OK) {
			hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(ret));
		}
	}

	return rc;
}

int hm_trace_get_event_tracing_eid_by_pid(pid_t pid,
					  const char *class_name,
					  const char *event_name,
					  int *eid)
{
	int rc = E_HM_OK;
	int err;
	rref_t rref = 0;
	struct hmtrace_data class_data;
	struct hmtrace_eventname event_data;
	struct  __actvret_hmtracecall_hmtrace_get_event_tracing_eid ret;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	mem_zero_s(ret);
	if (eid == NULL) {
		hm_warn("hmtrace server input status is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	}

	if (rc == E_HM_OK) {
		rc = actvcall_prepare(class_name, HMTRACE_CLASS,
				      class_data.buffer, sizeof(class_data.buffer));
		if (rc == E_HM_OK) {
			rc = actvcall_prepare(event_name, HMTRACE_EVENT,
					      event_data.buffer, sizeof(event_data.buffer));
		}

		if (rc == E_HM_OK) {
			rc = actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(rref,
					class_data, event_data, &ret);
			if (rc != E_HM_OK) {
				hm_warn("hmtrace server get event eid failed: %s\n",
					 hmstrerror(rc));
			}
		}

		if (rc == E_HM_OK) {
			*eid = ret.eid;
		}

		err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("clean tracemgr_rref failed, ret=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

int hm_trace_get_rb_info_by_pid(pid_t pid, const char *class_name, struct rb *rb)
{
	int rc;
	int err;
	rref_t rref;
	struct hmtrace_data class_data;
	struct __actvret_hmtracecall_hmtrace_get_rb_info ret;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	mem_zero_s(ret);
	if (rb == NULL) {
		hm_warn("hmtrace get rb info with invalid input\n");
		return E_HM_INVAL;
	}

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc != E_HM_OK) {
		return rc;
	}

	rc = actvcall_prepare(class_name, HMTRACE_CLASS,
			      class_data.buffer, sizeof(class_data.buffer));
	if (rc == E_HM_OK) {
		rc = actvxactcapcall_hmtracecall_hmtrace_get_rb_info(false, /* not fwd */
								     true, /* allow fwd */
								     rref, class_data, &ret);
		if (rc != E_HM_OK) {
			hm_warn("hmtrace get shm id failed: %s\n",
				hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		*rb = ret.rb;
	}

	err = hm_path_release(path_name, rref);
	if (err != E_HM_OK) {
		hm_warn("hm_path_release failed, ret=%s\n", hmstrerror(err));
	}

	return rc;
}

int hm_trace_ipc_buf_open(pid_t pid, const char *name)
{
	int ret;
	rref_t rref;
	int handle = 0;
	struct hmtrace_data ipc_name;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];
	int rc;

	if (name == NULL) {
		return E_HM_INVAL;
	}

	ret = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (ret < 0) {
		return ret;
	}

	ret = strcpy_s(ipc_name.buffer, HMTRACE_DATA_SIZE, name);
	if (ret != 0) {
		hm_warn("name is too long %zu larger than %d\n", strlen(name) + 1,
			HMTRACE_DATA_SIZE);
		rc = hm_path_release(path_name, rref);
		if (rc != E_HM_OK) {
			hm_warn("hm_path_release failed, rc=%s\n", hmstrerror(rc));
		}
		return E_HM_INVAL;
	}

	ipc_name.buffer[HMTRACE_DATA_SIZE - 1] = '\0';
	handle = actvcapcall_hmtracecall_hmtrace_ipc_buf_open(rref, ipc_name);
	if (handle < 0) {
		hm_warn("open server %s failed: %s\n", name, hmstrerror(handle));
		rc = hm_path_release(path_name, rref);
		if (rc != E_HM_OK) {
			hm_warn("hm_path_release failed, rc=%s\n", hmstrerror(rc));
		}
		return handle;
	}

	rc = hm_path_release(path_name, rref);
	if (rc != E_HM_OK) {
		hm_warn("hm_path_release failed, rc=%s\n", hmstrerror(rc));
	}

	ret = ipc_buf_client_open(name, handle);
	if (ret < 0) {
		hm_warn("open client %s failed: %s\n", name, hmstrerror(ret));
		int err = actvcapcall_hmtracecall_hmtrace_ipc_buf_close(rref, handle);
		if (err < 0) {
			hm_debug("close server %s failed: %s\n", name, hmstrerror(err));
		}
		return ret;
	}

	return handle;
}

static int ipc_buf_actvcall_read_buf(rref_t rref, int handle, void *buf, size_t size)
{
	int err;
	uint64_t key = 0;
	void *shm_buf = NULL;
	int shm_id = 0;
	int offset = -1;

	if (buf == NULL) {
		hm_warn("buffer is NULL\n");
		return E_HM_INVAL;
	}

	err = shmem_create(size, rref, &key, &shm_buf, &shm_id);
	if (err != E_HM_OK) {
		return err;
	}

	err = actvcapcall_hmtracecall_hmtrace_ipc_buf_read(rref, handle,
							   (unsigned long long)key);
	if (err >= 0) {
		offset = err;
		err = E_HM_OK;
	} else {
		hm_warn("hmtrace server ipc_buf_read failed: %s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = shmem_read(size, shm_buf, buf);
	}

	shmem_clean(size, shm_id, shm_buf);
	return err < 0 ? err : (int)offset;
}

int hm_trace_ipc_buf_read_item(pid_t pid, int handle, void **item)
{
	int err;
	rref_t rref = 0ULL;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];
	int ret;

	if (item == NULL) {
		return E_HM_INVAL;
	}

	err = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (err < 0) {
		return err;
	}

	err = ipc_buf_client_read_item(rref, handle, item, ipc_buf_actvcall_read_buf);

	ret = hm_path_release(path_name, rref);
	if (ret != E_HM_OK) {
		hm_warn("hm_path_release failed, ret=%s\n", hmstrerror(ret));
	}

	return err;
}

int hm_trace_ipc_buf_close(pid_t pid, int handle)
{
	int err;
	rref_t rref = 0ULL;
	int ret;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];

	err = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (err < 0) {
		return err;
	}

	err = actvcapcall_hmtracecall_hmtrace_ipc_buf_close(rref, handle);
	if (err == E_HM_OK) {
		hm_debug("close server %d success\n", handle);
		err = ipc_buf_client_close(handle);
	}

	ret = hm_path_release(path_name, rref);
	if (ret != E_HM_OK) {
		hm_warn("hm_path_release failed, err=%s\n", hmstrerror(ret));
	}

	return err;
}

int hm_trace_realloc_trace_buffer_by_pid(pid_t pid, const char *class_name, unsigned int buffer_size)
{
	int rc;
	rref_t rref = 0ULL;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];
	struct hmtrace_data class_data;

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		rc = actvcall_prepare(class_name, HMTRACE_CLASS,
				       class_data.buffer, sizeof(class_data.buffer));
		if (rc == E_HM_OK) {
			rc = actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(rref, class_data,
										   buffer_size);
		}
		int err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("hm_path_release failed, err=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

int hm_trace_buffer_size_of_pid(pid_t pid, const char *class_name, size_t *buffer_size)
{
	int rc;
	rref_t rref = 0ULL;
	char path_name[HMTRACEMGR_PATH_MAX_SIZE];
	struct hmtrace_data class_data;
	struct  __actvret_hmtracecall_hmtrace_trace_buffer_size actv_ret;

	rc = get_tracemgr_rref(pid, &rref, path_name, (size_t)HMTRACEMGR_PATH_MAX_SIZE);
	if (rc == E_HM_OK) {
		rc = actvcall_prepare(class_name, HMTRACE_CLASS,
				       class_data.buffer, sizeof(class_data.buffer));
		if (rc == E_HM_OK) {
			rc = actvcapcall_hmtracecall_hmtrace_trace_buffer_size(rref, class_data,
										&actv_ret);
		}

		if (rc == E_HM_OK) {
			*buffer_size = actv_ret.buffer_size;
		}

		int err = hm_path_release(path_name, rref);
		if (err != E_HM_OK) {
			hm_warn("hm_path_release failed, err=%s\n", hmstrerror(err));
		}
	}

	return rc;
}

#else /* CONFIG_HMTRACE */

int hm_trace_get_registered_class_list_by_pid(pid_t pid, const char **buffer)
{
	UNUSED(pid, buffer);
	return E_HM_NOSYS;
}

int hm_trace_get_registered_events_list_by_pid(pid_t pid, const char *class_name,
					       const char **buffer)
{
	UNUSED(pid, class_name, buffer);
	return E_HM_NOSYS;
}

int hm_trace_get_available_events_list_by_pid(pid_t pid, const char *class_name,
					      const char **buffer)
{
	UNUSED(pid, class_name, buffer);
	return E_HM_NOSYS;
}

int hm_trace_register_event_by_pid(pid_t pid, const char *class_name,
				   const void *buffer, unsigned int buffer_size)
{
	UNUSED(pid, class_name, buffer, buffer_size);
	return E_HM_NOSYS;
}

int hm_trace_unregister_event_by_pid(pid_t pid, const char *class_name,
				     const void *buffer, unsigned int buffer_size)
{
	UNUSED(pid, class_name, buffer, buffer_size);
	return E_HM_NOSYS;
}

int hm_trace_set_event_tracing_on_by_pid(pid_t pid, const char *class_name,
					 const char *event_name)
{
	UNUSED(pid, class_name, event_name);
	return E_HM_NOSYS;
}

int hm_trace_set_event_tracing_off_by_pid(pid_t pid, const char *class_name,
					  const char *event_name)
{
	UNUSED(pid, class_name, event_name);
	return E_HM_NOSYS;
}

int hm_trace_get_event_tracing_status_by_pid(pid_t pid,
					     const char *class_name,
					     const char *event_name,
					     int *status)
{
	UNUSED(pid, class_name, event_name, status);
	return E_HM_NOSYS;
}

int hm_trace_get_event_tracing_eid_by_pid(pid_t pid,
					  const char *class_name,
					  const char *event_name,
					  int *eid)
{
	UNUSED(pid, class_name, event_name, eid);
	return E_HM_NOSYS;
}

int hm_trace_get_rb_info_by_pid(pid_t pid, const char *class_name,
				struct rb *rb)
{
	UNUSED(pid, class_name, rb);
	return E_HM_NOSYS;
}

int hm_trace_ipc_buf_open(pid_t pid, const char *name)
{
	UNUSED(pid, name);
	return E_HM_NOSYS;
}

int hm_trace_ipc_buf_read_item(pid_t pid, int handle, void **item)
{
	UNUSED(pid, handle, item);
	return E_HM_NOSYS;
}

int hm_trace_ipc_buf_close(pid_t pid, int handle)
{
	UNUSED(pid, handle);
	return E_HM_NOSYS;
}

int hm_trace_tracing_on_all_events(pid_t pid, unsigned int status)
{
	UNUSED(pid, status);
	return E_HM_NOSYS;
}

int hm_trace_realloc_trace_buffer_by_pid(pid_t pid, const char *class_name, unsigned int buffer_size)
{
	UNUSED(pid, class_name, buffer_size);
	return E_HM_NOSYS;
}

int hm_trace_buffer_size_of_pid(pid_t pid, const char *class_name, size_t *buffer_size)
{
	UNUSED(pid, class_name, buffer_size);
	return E_HM_NOSYS;
}
#endif
