/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide hmtracemgr sysif handler
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <stdbool.h>
#include <lib/utils.h>
#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmtrace/hmtrace.h>

#include <libsysif/hmtracemgr/server.h>
#include "../../hmtrace/core/trace_iter.h"
#include "hmtracemgr_core.h"

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_get_registered_class_list,
		    unsigned long long, sender, unsigned long, credential,
		    struct bunch_ipc_attr, attr)
{
	UNUSED(credential);
	attr.owner_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	return hmtracemgr_get_registered_class_list(&attr);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_get_registered_events_list,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data, struct bunch_ipc_attr, attr)
{
	UNUSED(credential);
	attr.owner_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	const char *class_name = class_data.buffer;
	return hmtracemgr_get_registered_events_list(class_name, &attr);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_get_available_events_list,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data, struct bunch_ipc_attr, attr)
{
	UNUSED(credential);
	attr.owner_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	const char *class_name = class_data.buffer;
	return hmtracemgr_get_available_events_list(class_name, &attr);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_set_event_tracing_on,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    struct hmtrace_eventname, event_data)
{
	const char *class_name = class_data.buffer;
	const char *event_name = event_data.buffer;

	UNUSED(sender, credential);
	return hmtracemgr_set_event_on(class_name, event_name);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_set_event_tracing_off,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    struct hmtrace_eventname, event_data)
{
	const char *class_name = class_data.buffer;
	const char *event_name = event_data.buffer;

	UNUSED(sender, credential);
	return hmtracemgr_set_event_off(class_name, event_name);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_events_tracing_on,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, status)
{
	UNUSED(sender, credential);
	return hm_trace_tracing_on(status);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_get_event_tracing_status,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    struct hmtrace_eventname, event_data)
{
	const char *class_name = class_data.buffer;
	const char *event_name = event_data.buffer;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_status *pret =
		actvhdlr_hmtracehandler_hmtrace_get_event_tracing_status_prepare_ret(
			sender, credential);

	return hmtracemgr_get_event_status(class_name, event_name, &(pret->status));
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_get_event_tracing_eid,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    struct hmtrace_eventname, event_data)
{
	const char *class_name = class_data.buffer;
	const char *event_name = event_data.buffer;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid *pret =
		actvhdlr_hmtracehandler_hmtrace_get_event_tracing_eid_prepare_ret(
			sender, credential);

	return hmtracemgr_get_event_eid(class_name, event_name, &(pret->eid));
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_get_rb_info,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data)
{
	const char *class_name = class_data.buffer;
	struct __actvret_hmtracecall_hmtrace_get_rb_info *pret =
		actvhdlr_hmtracehandler_hmtrace_get_rb_info_prepare_ret(
			sender, credential);
	return hmtracemgr_get_rb_info(class_name, &pret->rb);
}

static bool is_valid_handle(int handle, int sender_src)
{
	unsigned int handle_index = handle_sender_bits((unsigned int)handle);
	unsigned int sender_index = src_sender_bits((unsigned int)sender_src);
	if (handle_index == sender_index) {
		return true;
	}

	return false;
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_ipc_buf_open,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, ipc_data)
{
	int handle = E_HM_INVAL;
	const char *local_name = NULL;
	unsigned int sender_src = __RPC_SENDER_DECODE_SRC_CIDX(sender);

	UNUSED(credential);
	if ((ipc_data.buffer[0] != '\0') &&
	    (ipc_data.buffer[HMTRACE_DATA_SIZE - 1] == '\0')) {
		local_name  = ipc_data.buffer;
		handle = ipc_buf_srv_open(local_name);
	}

	if (handle < 0) {
		return handle;
	}
	return (int)handle_comb(sender_src, (unsigned int)handle);
}

static int shmem_open(uint64_t key, void **ret_buf, int *ret_shm_id, size_t *size)
{
	void *buf = NULL;
	int shm_id;
	int ret = E_HM_OK;
	size_t ret_len;

	/* shm key belongs to client */
	shm_id = hm_ashm_open_auth(key, sysif_actv_src_cnode_idx(), 0,
				   SHM_O_RDWR, &ret_len);
	if (shm_id < 0) {
		ret = E_HM_POSIX_FAULT;
	}

	if (ret == E_HM_OK) {
		buf = hm_mem_mmap(NULL, ret_len, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, shm_id, 0);
		if (buf == MAP_FAILED) {
			hm_error("failed to mmap\n");
			(void)hm_ashm_close(shm_id);
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		*ret_buf = buf;
		*ret_shm_id = shm_id;
		if (size != NULL) {
			*size = ret_len;
		}
	}

	return ret;
}

static void shm_clear(void *buf, size_t size, int shm_id)
{
	(void)hm_mem_munmap(buf, size);
	(void)hm_ashm_close(shm_id);
}

static int hmtrace_ipc_buf_read(int handle, uint64_t key)
{
	int ret;
	size_t offset = 0;
	int shm_id;
	void *buf = NULL;
	size_t local_buf_sz = 0;
	void *local_buf = NULL;
	size_t size = 0;

	/* shm key belongs to client */
	ret = shmem_open(key, &buf, &shm_id, &size);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (size < IPC_BUF_MAX_SIZE) {
		local_buf_sz = size;
	} else  {
		local_buf_sz = IPC_BUF_MAX_SIZE;
	}

	if (local_buf_sz == 0) {
		return E_HM_INVAL;
	}

	size_t buf_sz = size;
	local_buf = malloc(local_buf_sz);
	if (local_buf == NULL) {
		ret = E_HM_NOMEM;
		goto end;
	}
	while (ALIGN_UP(offset, IPC_BUF_MAX_SIZE) < size) {
		offset = ALIGN_UP(offset, IPC_BUF_MAX_SIZE);
		int count = ipc_buf_srv_write(handle_index_bits(handle), local_buf, local_buf_sz);
		if (count <= 0) {
			ret = count;
			break;
		}
		ret = memcpy_s((char *)buf + offset, buf_sz, local_buf, (unsigned long)(unsigned int)count);
		if (ret != 0) {
			ret = E_HM_POSIX_FAULT;
			break;
		}
		offset += (size_t)(uint32_t)count;
		buf_sz -= (size_t)(uint32_t)count;
	}

	free(local_buf);

end:
	shm_clear(buf, size, shm_id);

	return ret < 0 ? ret : (int)offset;
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_ipc_buf_read,
		    unsigned long long, sender, unsigned long, credential,
		    int, handle, unsigned long long, key)
{
	int ret;
	int sender_src = (int)__RPC_SENDER_DECODE_SRC_CIDX(sender);

	UNUSED(credential);
	if (!is_valid_handle(handle, sender_src)) {
		return E_HM_INVAL;
	}

	ret = hmtrace_ipc_buf_read(handle, (uint64_t)key);
	return ret;
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_ipc_buf_close,
		    unsigned long long, sender, unsigned long, credential,
		    int, handle)
{
	int sender_src = (int)__RPC_SENDER_DECODE_SRC_CIDX(sender);

	UNUSED(credential);
	if (!is_valid_handle(handle, sender_src)) {
		return E_HM_INVAL;
	}

	return ipc_buf_srv_close(handle_index_bits(handle));
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_register_event,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    unsigned long long, key)
{
	UNUSED(sender, credential);

	const char *class_name = class_data.buffer;
	int shm_id;
	void *buffer = NULL;
	size_t size = 0;

	int err = shmem_open((uint64_t)key, &buffer, &shm_id, &size);
	if (err == E_HM_OK) {
		err = hmtracemgr_register_event(class_name, buffer, (unsigned int)size);
		shm_clear(buffer, size, shm_id);
	}
	return err;
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_unregister_event,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    unsigned long long, key)
{
	UNUSED(sender, credential);

	const char *class_name = class_data.buffer;
	int shm_id;
	void *buf = NULL;
	size_t size = 0;

	int err = shmem_open((uint64_t)key, &buf, &shm_id, &size);
	if (err == E_HM_OK) {
		err = hmtracemgr_unregister_event(class_name, buf, (unsigned int)size);
		shm_clear(buf, size, shm_id);
	}
	return err;
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_realloc_trace_buffer,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data,
		    unsigned long, buffer_size)
{
	UNUSED(sender, credential);

	const char *class_name = class_data.buffer;

	return hmtracemgr_realloc_trace_buffer(class_name, buffer_size);
}

DEFINE_ACTVHDLR_ALS(hmtracehandler_hmtrace_trace_buffer_size,
		    unsigned long long, sender, unsigned long, credential,
		    struct hmtrace_data, class_data)
{
	UNUSED(sender, credential);

	const char *class_name = class_data.buffer;
	struct __actvret_hmtracecall_hmtrace_trace_buffer_size *pret =
		actvhdlr_hmtracehandler_hmtrace_trace_buffer_size_prepare_ret(sender, credential);

	return hmtracemgr_trace_buffer_size(class_name, (size_t *)(void *)&(pret->buffer_size));
}
