/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Fault event lib interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 11 16:19:37 2023
 */

#include <stdarg.h>
#include <stdio.h>

#include <hmkernel/types.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libkbox/kbox.h>

#include <libhmfaultevt/fault_event.h>

#ifdef CONFIG_LIBFAULTEVT

#define FEVT_KBOX_NAME			"fault_event"
#define FEVT_OUTBUF_SIZE		512U

/* dummy event, remove it when the real event is added */
#define DUMMY_FEVT_EVENT			\
	[0] = {					\
		.level = FEVT_LEVEL_DEBUG,	\
		.type  = FEVT_TYPE_FAULT,	\
		.desc  = "dummy event",		\
	}

struct fevt_out_buf {
	char *buf;
	size_t buf_len;
	size_t used_len;
};

struct fevt_module {
	const char *name;
	const struct fevt_event *evt;
	uint32_t evt_nr;
};

static const struct fevt_event fevt_event_hguard[] = {
	[FEVT_EVENT_ID_TO_IDX_RAW(HGUARD, DM_VERITY_FAILED)] = {
		.level = FEVT_LEVEL_IMPORTANT,
		.type  = FEVT_TYPE_FAULT,
		.desc  = "dm-verity: verify sign failed",
	},
	[FEVT_EVENT_ID_TO_IDX_RAW(HGUARD, START_SERVICE_FAILED)] = {
		.level = FEVT_LEVEL_IMPORTANT,
		.type  = FEVT_TYPE_FAULT,
		.desc  = "start service failed",
	},
	[FEVT_EVENT_ID_TO_IDX_RAW(HGUARD, SERVICE_EXITED_ABNORMALLY)] = {
		.level = FEVT_LEVEL_IMPORTANT,
		.type  = FEVT_TYPE_FAULT,
		.desc  = "service exited abnormally",
	},
	[FEVT_EVENT_ID_TO_IDX_RAW(HGUARD, HEARTBEAT_RECOVER_SERVICE)] = {
		.level = FEVT_LEVEL_IMPORTANT,
		.type  = FEVT_TYPE_FAULT,
		.desc  = "heartbeat recover service",
	},
	[FEVT_EVENT_ID_TO_IDX_RAW(HGUARD, SYSTEM_LOWMEMORY)] = {
		.level = FEVT_LEVEL_GENERAL,
		.type  = FEVT_TYPE_STATICS,
		.desc  = "low memory",
	},
};

static const struct fevt_event fevt_event_sysmgr[] = {
	DUMMY_FEVT_EVENT,
};

static const struct fevt_event fevt_event_vfs[] = {
	DUMMY_FEVT_EVENT,
};

static const struct fevt_event fevt_event_uvmm[] = {
	DUMMY_FEVT_EVENT,
};

static const struct fevt_event fevt_event_devhost[] = {
	DUMMY_FEVT_EVENT,
};

static const struct fevt_event fevt_event_devmgr[] = {
	DUMMY_FEVT_EVENT,
};

#define fevt_module_field(module, module_name)		\
[FEVT_MODULE_##module_name] = {				\
	.name = #module_name,				\
	.evt = fevt_event_##module,			\
	.evt_nr = __ARRAY_SIZE(fevt_event_##module),	\
}

#define fevt_module_field_dummy(module, module_name)	\
[FEVT_MODULE_##module_name] = {			\
	.name = #module_name,				\
	.evt = NULL,						\
	.evt_nr = 0,						\
}

static struct fevt_module fevt_modules[] = {
	fevt_module_field(hguard, HGUARD),
	fevt_module_field(sysmgr, SYSMGR),
	fevt_module_field(vfs, VFS),
	fevt_module_field(uvmm, UVMM),
	fevt_module_field(devhost, DEVHOST),
	fevt_module_field(devmgr, DEVMGR),
	fevt_module_field_dummy(vhost, VHOST),
};

int hm_fevt_module_register(enum fevt_module_id module_id, const struct fevt_event *events, uint32_t evt_nr)
{
	int err = E_HM_OK;

	if (events == NULL || evt_nr == 0) {
		hm_error("events is null\n");
		err = E_HM_INVAL;
	} else if (module_id >= FEVT_MODULE_NR) {
		hm_error("invalid module_id=%u, should be less than %u\n",
				(uint32_t)module_id, (uint32_t)FEVT_MODULE_NR);
		err = E_HM_INVAL;
	} else if (fevt_modules[module_id].evt != NULL || fevt_modules[module_id].evt_nr != 0) {
		hm_error("the module %u has been registered\n", (uint32_t)module_id);
		err = E_HM_OBJEXIST;
	} else {
		fevt_modules[module_id].evt = events;
		fevt_modules[module_id].evt_nr = evt_nr;
	}

	return err;
}

static int fevt_buf_vprint(struct fevt_out_buf *out_buf,
			   const char *fmt, va_list args)
{
	int err = E_HM_OK;
	char *buf = ptr_add(out_buf->buf, out_buf->used_len);
	size_t usable_len = out_buf->buf_len - out_buf->used_len;
	int wlen;

	wlen = vsprintf_s(buf, usable_len, fmt, args);
	if (wlen < 0) {
		hm_error("failed to print to fevt buf, buf_len=%zu, used_len=%zu\n",
			 out_buf->buf_len, out_buf->used_len);
		err = E_HM_INVAL;
	} else {
		out_buf->used_len += (size_t)wlen;
	}

	return err;
}

static int __attribute__((format(printf, 2, 3)))
fevt_buf_print(struct fevt_out_buf *out_buf, const char *fmt, ...)
{
	int err;
	va_list args;

	va_start(args, fmt);
	err = fevt_buf_vprint(out_buf, fmt, args);
	va_end(args);

	return err;
}

static int fevt_print_time(struct fevt_out_buf *out_buf)
{
	struct timeval tv = {};
	int err;

	err = hm_gettimeofday(&tv, NULL);
	if (err != E_HM_OK) {
		hm_warn("failed to gettimeofday, err=%s\n", hmstrerror(err));
		mem_zero_s(tv);
	}

	return fevt_buf_print(out_buf, "%ld.%06ld;", (long)tv.tv_sec, (long)tv.tv_usec);
}

static int fevt_print_prefix(struct fevt_out_buf *out_buf, uint32_t event_id,
			     const struct fevt_event *event, const char *module_name)
{
	int err;

	err = fevt_print_time(out_buf);
	if (err == E_HM_OK) {
		err = fevt_buf_print(out_buf, "%u;%d;%d;%s;", event_id,
				     (int)event->level, (int)event->type,
				     module_name);
	}

	if (err != E_HM_OK) {
		hm_error("failed to print fevt prefix\n");
	}
	return err;
}

static int fevt_print_kv_pairs(struct fevt_out_buf *out_buf,
			       uint32_t kv_nr, va_list args)
{
	int err = E_HM_OK;

	if (kv_nr == 0) {
		/* use "-" to indicate no key-value pairs */
		err = fevt_buf_print(out_buf, "-;");
	} else {
		for (uint32_t i = 0U; i < kv_nr; i++) {
			const char *fmt = va_arg(args, const char *);
			void *arg = va_arg(args, void *);

			err = fevt_buf_print(out_buf, fmt, arg);
			if (err != E_HM_OK) {
				break;
			}

			/* each key-value pair is separated by "," */
			err = fevt_buf_print(out_buf, (i + 1U < kv_nr) ? "," : ";");
			if (err != E_HM_OK) {
				break;
			}
		}
	}

	if (err != E_HM_OK) {
		hm_error("failed to print fevt kv pairs\n");
	}
	return err;
}

static int fevt_print_event_desc(struct fevt_out_buf *out_buf,
				 const char *event_desc)
{
	int err;

	err = fevt_buf_print(out_buf, "%s\n", event_desc);
	if (err != E_HM_OK) {
		hm_error("failed to print fevt event desc\n");
	}

	return err;
}

static void fevt_do_output(const struct fevt_out_buf *out_buf)
{
	int kbox_id;

	kbox_id = kbox_open(FEVT_KBOX_NAME);
	if (kbox_id < 0) {
		hm_error("failed to open kbox %s\n", FEVT_KBOX_NAME);
	} else {
		(void)kbox_write(kbox_id, out_buf->buf, out_buf->used_len);
		kbox_close(kbox_id);
	}
}

#ifdef __HOST_UT__
static const struct fevt_event *ut_test_event = NULL;

void ut_fevt_set_test_event(const struct fevt_event *event)
{
	ut_test_event = event;
}
#endif

static const struct fevt_event *
find_fevt_event(uint32_t event_id, const char **module_name)
{
	const struct fevt_event *event = NULL;
	const struct fevt_module *module = NULL;
	enum fevt_module_id module_id;

	module_id = FEVT_EVENT_ID_TO_MODULE(event_id);
	if (module_id >= FEVT_MODULE_NR) {
		hm_error("invalid module_id=%d, should be less than %d. event_id=%u\n",
			 (int)module_id, (int)FEVT_MODULE_NR, event_id);
	} else {
		uint32_t evt_idx = FEVT_EVENT_ID_TO_IDX(event_id, module_id);

		module = &fevt_modules[module_id];
		if (evt_idx >= module->evt_nr) {
			hm_error("invalid evt_idx=%u, should be less than %u. event_id=%u\n",
				 evt_idx, module->evt_nr, event_id);
		} else {
			*module_name = module->name;
			event = &module->evt[evt_idx];
		}
	}

#ifdef __HOST_UT__
	/* for ut, force return test_event */
	*module_name = UT_FEVT_TEST_MODULE;
	event = ut_test_event;
#endif

	return event;
}

void hm_fevt_report_raw(uint32_t event_id, uint32_t kv_nr, ...)
{
	char buf[FEVT_OUTBUF_SIZE] = {};
	struct fevt_out_buf out_buf = {
		.buf = buf,
		.buf_len = sizeof(buf),
		.used_len = 0U,
	};
	const struct fevt_event *event = NULL;
	const char *module_name = NULL;
	int err = E_HM_OK;

	event = find_fevt_event(event_id, &module_name);
	if (event == NULL || module_name == NULL) {
		err = E_HM_INVAL;
	} else {
		err = fevt_print_prefix(&out_buf, event_id, event, module_name);
	}

	if (err == E_HM_OK) {
		va_list args;

		va_start(args, kv_nr);
		err = fevt_print_kv_pairs(&out_buf, kv_nr, args);
		va_end(args);
	}

	if (err == E_HM_OK) {
		err = fevt_print_event_desc(&out_buf, event->desc);
	}

	if (err == E_HM_OK) {
		fevt_do_output(&out_buf);
	}
}

#else /* CONFIG_LIBFAULTEVT */

void hm_fevt_report_raw(uint32_t event_id, uint32_t kv_nr, ...)
{
	UNUSED(event_id, kv_nr);
}

int hm_fevt_module_register(enum fevt_module_id module_id, const struct fevt_event *events, uint32_t evt_nr)
{
	UNUSED(module_id, events, evt_nr);

	return E_HM_NOSYS;
}

#endif /* CONFIG_LIBFAULTEVT */
