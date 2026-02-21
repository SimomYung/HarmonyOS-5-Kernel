/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: sysevent lib interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 26 08:08:32 PM 2023
 */

#include <ctype.h>
#include <stdarg.h>
#include <hongmeng/errno.h>
#include <hmkernel/types.h>
#include <hongmeng/macro.h>
#include <hongmeng/syscall.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>
#include <libsysevent/sysevent.h>

#ifdef CONFIG_SYSEVENT

static int sysevent_get_realtime(__u64 *timestamp)
{
	struct timespec time_t;
	int err = E_HM_OK;

	err = hm_clock_gettime(CLOCK_REALTIME, &time_t);
	if (err == E_HM_OK) {
		*timestamp = (__u64)((time_t.tv_sec * MSEC_PER_SEC) +
				     (time_t.tv_nsec / NSEC_PER_MSEC));
	} else {
		hm_debug("failed to get current time\n");
	}

	return err;
}

#define SYSEVENT_CURRENT_PID() (__u32)hm_getpid()
#define SYSEVENT_CURRENT_TID() (__u32)hm_thread_gettid()
#define SYSEVENT_CURRENT_UID() hm_getuid()
#define SYSEVENT_CURRENT_REALTIME(timestamp_addr) sysevent_get_realtime(timestamp_addr)

#include <lib/sysevent/sysevent.h>

static int raw_data_init(struct raw_data *data, size_t size)
{
	int err = E_HM_OK;

	data->buf = calloc(1, size);
	if (data->buf != NULL) {
		data->capacity = size;
		data->pos = 0;
	} else {
		err = E_HM_NOMEM;
	}

	return err;
}

static void raw_data_destroy(struct raw_data *data)
{
	free(data->buf);
	data->buf = NULL;
	data->capacity = 0;
	data->pos = 0;
}

int hm_sysevent_report_raw_data(const char *data, size_t data_len)
{
	static vatomic32_t kobj_sysevent_inited = VATOMIC_INIT(0U);
	static struct __ctrlmem_kobj_sysevent_s *kobj_sysevent = NULL;
	int err = E_HM_OK;

	if (data == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (vatomic32_cmpxchg(&kobj_sysevent_inited, 0U, 1U) == 0U) {
			kobj_sysevent = hm_ctrlmem_kobjpool_spawn_sysevent();
			if (kobj_sysevent == NULL) {
				hm_error("sysevent kobj spawn failed.\n");
			}
		}
		if (kobj_sysevent != NULL) {
			err = sysctrlmem_kobjctnr_sysevent_write(kobj_sysevent, data, (unsigned int)data_len);
		} else {
			err = E_HM_NOSYS;
		}
    }

	return err;
}

int hm_sysevent_report_raw(const char *domain, const char *evt_name,
			   enum sysevent_type evt_type, __u32 params_count, ...)
{
	struct raw_data data = {0};
	__u32 block_size = 0;
	size_t block_size_pos = 0;

	int err = raw_data_init(&data, SYSEVT_BUF_SIZE);
	if (err == E_HM_OK) {
		/* reserve block_size and save its position for update later */
		block_size_pos = data.pos;
		err = raw_data_append(&data, (__u8 *)&block_size, sizeof(__u32));
	}

	if (err == E_HM_OK) {
		err = fill_header_to_data(&data, domain, evt_name, evt_type);
	}

	if (err == E_HM_OK) {
		va_list args;

		va_start(args, params_count);
		err = fill_args_to_data(&data, params_count, args);
		va_end(args);
	}

	if (err == E_HM_OK) {
		block_size = (__u32)data.pos;
		err = raw_data_update(&data, block_size_pos, (__u8 *)&block_size, sizeof(__u32));
	}

	if (err == E_HM_OK) {
		err = hm_sysevent_report_raw_data(data.buf, data.pos);
		if (err != E_HM_OK) {
			hm_error("Failed to sysevent_write: %s\n", hmstrerror(err));
		}
	}

	raw_data_destroy(&data);

	return err;
}

#else /* !CONFIG_SYSEVENT */

int hm_sysevent_report_raw_data(const char *data, size_t data_len)
{
	UNUSED(data, data_len);
	return E_HM_NOSYS;
}

int hm_sysevent_report_raw(const char *domain, const char *evt_name,
			   enum sysevent_type evt_type, __u32 params_count, ...)
{
	UNUSED(domain, evt_name, evt_type, params_count);
	return E_HM_NOSYS;
}

#endif /* CONFIG_SYSEVENT */
