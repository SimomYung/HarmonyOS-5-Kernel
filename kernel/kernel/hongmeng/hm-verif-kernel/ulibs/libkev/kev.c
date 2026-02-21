/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implement four interfaces for reading kev
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 03 11:39:48 2020
 */

#include <stdarg.h>
#include <libkev/kev.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <hongmeng/errno.h>

#define READ_BUF_LEN (4U * 1024U)
#define MAX_ITERATE_DATA_LEN (32 * 1024 * 1024) /* at max 32M */

struct read_param_s {
	uint64_t head;
	unsigned int index;
	uint64_t read_buf;
	size_t read_buf_size;
};

static const char *kev_type_names[(int)KEV_TYPE_NR] = {
	[(int)KEV_TYPE_SCHED_SWITCH] = "sched_switch",
	[(int)KEV_TYPE_SUSPEND] = "suspend",
	[(int)KEV_TYPE_BLOCK] = "block",
	[(int)KEV_TYPE_UNBLOCK] = "unblock",
	[(int)KEV_TYPE_SYSCALL] = "syscall",
	[(int)KEV_TYPE_PAGEFAULT] = "pagefault",
	[(int)KEV_TYPE_IRQ] = "irq",
	[(int)KEV_TYPE_SAMPLE] = "sample",
	[(int)KEV_TYPE_KLOG] = "klog",
	[(int)KEV_TYPE_KPREEMPT] = "kpreempt",
	[(int)KEV_TYPE_FIQ] = "fiq",
	[(int)KEV_TYPE_SCHED_MIGRATE_TASK] = "sched_migrate_task",
	[(int)KEV_TYPE_AUDIT] = "audit",
	[(int)KEV_TYPE_TRAP_EL2] = "trap_el2",
	[(int)KEV_TYPE_EL2_LOG] = "el2_log",
	[(int)KEV_TYPE_VCPU_EXIT] = "vcpu_exit",
	[(int)KEV_TYPE_VCPU_ENTER] = "vcpu_enter",
	[(int)KEV_TYPE_EL2_IRQ] = "el2_irq",
	[(int)KEV_TYPE_VNOTIFY] = "vnotify",
	[(int)KEV_TYPE_SUSPEND_RESUME] = "suspend_resume",
#ifdef CONFIG_VIRTUAL_SMMU_INTERFACE
	[(int)KEV_TYPE_VSMMU] = "vsmmu",
#else
	[(int)KEV_TYPE_VSMMU] = "isolated",
#endif
};
const char *kev_name_of_type(enum kev_type type)
{
	const char *type_name = "unknown";

	if (type < KEV_TYPE_NR) {
		type_name = kev_type_names[type];
	}
	return type_name;
}

enum kev_type kev_type_of_name(const char *name)
{
	unsigned int type = KEV_TYPE_NR;
	unsigned int i;

	if (name == NULL) {
		return KEV_TYPE_NR;
	}
	for (i = 0U; i < KEV_TYPE_NR; i++) {
		if (strcmp(kev_type_names[i], name) == 0) {
			type = i;
			break;
		}
	}
	return (enum kev_type)type;
}

int kev_read_event_config(enum kev_type type, struct kev_config *config)
{
	int err;
	struct __actvret_hmcall_sysctrl_kev_read_event_config ret;

	if (type >= KEV_TYPE_NR || config == NULL) {
		return E_HM_INVAL;
	}
	mem_zero_s(ret);
	err = actvcall_hmcall_sysctrl_kev_read_event_config((unsigned int)type, &ret);
	if (err == 0) {
		config->enable = ret.event_config.enable;
		config->buffer = ret.event_config.config;
		config->paddr = ret.event_config.paddr;
		config->size = ret.event_config.size;
		config->boot_offset = ret.event_config.boot_offset;
	} else {
		hm_warn("read event config failed, err=%s\n", hmstrerror(err));
	}
	return err;
}

int kev_buffer_config_index_of(unsigned int kev_type, struct kev_config *config,
			       unsigned int *buffer_index)
{
	int err = E_HM_OK;
	unsigned int index = 0U;

	if (kev_type >= KEV_TYPE_NR || config == NULL || buffer_index == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK && config->buffer == 0U) {
		hm_info("kev %s is not configured\n",
			kev_name_of_type(kev_type));
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		while ((config->buffer & 1U) == 0U) {
			config->buffer >>= 1U;
			index++;
		}
		*buffer_index = index;
	}

	return err;
}

int kev_buffer_index_of(unsigned int kev_type, unsigned int *buffer_index)
{
	int err;
	struct kev_config config;

	mem_zero_s(config);
	err = kev_read_event_config(kev_type, &config);
	if (err == E_HM_OK) {
		err = kev_buffer_config_index_of(kev_type, &config, buffer_index);
	}

	return err;
}

int kev_buffer_size_of(unsigned int kev_type, unsigned int *buf_size)
{
	int err = E_HM_INVAL;
	struct kev_config config;

	mem_zero_s(config);
	if (buf_size != NULL) {
		err = kev_read_event_config(kev_type, &config);
	}
	if (err == E_HM_OK) {
		*buf_size = config.size;
	}

	return err;
}

/*
 * Here we only want to get the current ringbuffer write head, copy a char is
 * enough.
 */
static int read_current_head(unsigned int index, uint64_t *head)
{
	int err;
	char data;
	ssize_t ret_size;
	uint64_t ret_head = 0ULL;

	ret_size = hm_sysctrl_kev_read_data(index, (void *)(uintptr_t)&data,
					    sizeof(data), &ret_head);
	if (ret_size >= 0) {
		*head = ret_head;
		err = E_HM_OK;
	} else {
		err = (int)ret_size;
		hm_warn("read head of ring buffer failed, err=%s\n",
			 hmstrerror(err));
	}
	return err;
}

int kev_read_head_of_buffer(unsigned int index, uint64_t *head)
{
	if (head == NULL) {
		return E_HM_INVAL;
	}
	return read_current_head(index, head);
}

static inline size_t min(size_t x, size_t y)
{
	return x < y ? x : y;
}

static ssize_t read_data(struct read_param_s *read_param,
			 char *buf, size_t buf_size)
{
	int err = 0;
	uint64_t head;
	ssize_t ret_size;
	size_t read_size;
	size_t copy_size;
	size_t offset = 0UL;

	while (offset < buf_size) {
		head = read_param->head;
		read_size = min(read_param->read_buf_size, buf_size - offset);
		ret_size = hm_sysctrl_kev_read_data(read_param->index,
						    u64_to_ptr(read_param->read_buf, void),
						    read_size, &head);
		if (ret_size < 0) {
			hm_warn("read ring buffer failed, err=%s\n",
				hmstrerror((int)ret_size));
			break;
		}
		if (ret_size == 0) {
			break;
		}
		copy_size = (size_t)ret_size;
		NOFAIL(memcpy_s(&buf[offset], buf_size - offset,
			       u64_to_ptr(read_param->read_buf, void), copy_size));
		offset += copy_size;
		if (read_param->head + copy_size <= read_param->head) {
			break;
		}
		read_param->head += copy_size;
	}
	return offset > 0UL ? (ssize_t)offset : err;
}

static ssize_t do_kev_read_data(unsigned int index, uint64_t head,
				char *buf, size_t size)
{
	int err = 0;
	ssize_t offset = 0;
	char *read_buf = NULL;
	struct read_param_s read_param;

	read_buf = (char *)malloc(READ_BUF_LEN * sizeof(char));
	if (read_buf != NULL) {
		/* clean the read buffer though kpagefault is supported now */
		NOFAIL(memset_s(read_buf, READ_BUF_LEN, 0, READ_BUF_LEN));
		read_param.head = head;
		read_param.index = index;
		read_param.read_buf = (uint64_t)(uintptr_t)read_buf;
		read_param.read_buf_size = READ_BUF_LEN;
		offset = read_data(&read_param, buf, size);
		free(read_buf);
	} else {
		hm_warn("malloc buffer failed, error: %s\n", hmstrerror(err));
		err = E_HM_NOMEM;
	}
	return err >= 0 ? offset : err;
}

ssize_t kev_read_data(unsigned int index, uint64_t head,
		      char *buf, size_t size)
{
	ssize_t rc = 0;

	if (size == 0UL || buf == NULL) {
		return E_HM_INVAL;
	}
	if (head == 0) {
		rc = read_current_head(index, &head);
	}
	if (rc == 0) {
		rc = do_kev_read_data(index, head, buf, size);
	}
	return rc;
}

static int do_visit_event(const struct kev_s **kev_list, size_t kev_count,
			  int (*action)(const struct kev_s *, uintptr_t arg),
			  uintptr_t arg)
{
	size_t i;
	int err = 0;

	for (i = kev_count; i > 0; i--) {
		err = action(kev_list[i - 1], arg);
		if (err < 0) {
			hm_debug("execute callback function failed\n");
			break;
		}
	}
	return err;
}

/* count the events reside in the buf */
static size_t count_event(const char *buf, size_t size)
{
	size_t end;
	size_t count = 0;
	size_t cur = 0;
	size_t evt_size;
	const struct kev_s *evt = NULL;

	end = size - sizeof(struct kev_s);
	while (cur < end) {
		evt = (struct kev_s *)(uintptr_t)ptr_add(buf, cur);
		evt_size = evt->header.evt_size;
		if (evt_size % sizeof(uint32_t) != 0 ||
		    cur + evt_size > size || evt_size < sizeof(struct kev_s)) {
			break;
		}
		cur += evt_size;
		count++;
	}
	return count;
}

/* each event is pointed by the list */
static int prepare_kev_list(const char *buf, size_t size,
			    const struct kev_s ***kev_list,
			    size_t *kev_count)
{
	size_t i;
	size_t count;
	int err = E_HM_OK;
	unsigned int cur = 0U;
	const struct kev_s **list = NULL;

	count = count_event(buf, size);
	if (count != 0) {
		list = (const struct kev_s **)calloc(count, sizeof(struct kev_s *));
		if (list != NULL) {
			for (i = 0; i < count; i++) {
				list[i] = (struct kev_s *)(uintptr_t)ptr_add(buf, cur);
				cur += list[i]->header.evt_size;
			}
			*kev_list = list;
			*kev_count = count;
		} else {
			hm_warn("malloc buffer failed, error: %s\n", hmstrerror(err));
			err = E_HM_NOMEM;
		}
	}
	return err;
}

static int visit_event(const char *buf, size_t size, kev_iter_func_t action,
		       uintptr_t arg, size_t *ret_count)
{
	int err;
	size_t kev_count = 0;
	const struct kev_s **kev_list = NULL;

	err = prepare_kev_list(buf, size, &kev_list, &kev_count);
	if (err == E_HM_OK && kev_count != 0) {
		err = do_visit_event(kev_list, kev_count, action, arg);
		free(kev_list);
		if (ret_count != NULL) {
			*ret_count = kev_count;
		}
	}
	return err;
}

int kev_for_each(const char *buf, size_t size, kev_iter_func_t action, uintptr_t arg)
{
	int err;

	if (action == NULL || buf == NULL) {
		err = E_HM_INVAL;
	} else if (size < sizeof(struct kev_s)) {
		err = E_HM_OK;
	} else {
		err = visit_event(buf, size, action, arg, NULL);
	}
	return err;
}

static int do_kev_set_enable(const char *event_name, unsigned int enable)
{
	int err = E_HM_INVAL;
	struct trace_event event;

	if ((event_name != NULL) && (kev_type_of_name(event_name) < KEV_TYPE_NR)) {
		event.enable = enable;
		NOFAIL(strcpy_s(event.name, sizeof(event.name), event_name));
		err = actvcall_hmcall_sysctrl_kev_set_enable(event);
	}

	return err;
}

int kev_set_filter(const char *event_name, const char *filter)
{
	int err = E_HM_INVAL;
	struct trace_event event;

	if ((event_name != NULL) && (kev_type_of_name(event_name) < KEV_TYPE_NR)) {
		NOFAIL(strcpy_s(event.name, sizeof(event.name), event_name));
		if (filter != NULL) {
			NOFAIL(strcpy_s(event.filter, sizeof(event.filter), filter));
		}
		err = actvcall_hmcall_sysctrl_kev_set_filter(event);
	}

	return err;
}

int kev_set_enable(unsigned int enable, int num, ...)
{
	int err = E_HM_INVAL;
	va_list args;
	const char *event_name = NULL;

	va_start(args, num);
	if (num < 1) {
		hm_error("event num should bigger than 0\n");
	} else {
		for (int i = 0; i < num; i++) {
			event_name = va_arg(args, char *);
			/* do not return, just set next event */
			err = do_kev_set_enable(event_name, enable);
			if (err != E_HM_OK) {
				hm_warn("kev ctrl event %s failed, err is %s\n", event_name, hmstrerror(err));
				break;
			}
		}
	}
	va_end(args);

	return err;
}

static ssize_t do_iter_kev_data(unsigned int rb_index, uint64_t offset, size_t max_read_len,
		kev_iter_func_t it_func, uintptr_t arg)
{
	int err = E_HM_OK;
	size_t event_len = 0;
	char *buffer = NULL;

	if (it_func == NULL || max_read_len >= MAX_ITERATE_DATA_LEN) {
		err = E_HM_INVAL;
	} else {
		buffer = (char *)malloc(max_read_len);
		if (buffer == NULL) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		ssize_t ret_len = kev_read_data(rb_index, offset, buffer, max_read_len);
		if (ret_len < 0) {
			err = (int)ret_len;
		} else {
			err = visit_event(buffer, (size_t)ret_len, it_func, arg, &event_len);
		}
	}

	free(buffer);

	return (err < 0) ? (ssize_t)err : (ssize_t)event_len;
}

static int read_event_config(unsigned int kev_type,
		unsigned int *ret_rb_index, size_t *ret_rb_size, uint64_t *ret_boot_offset)
{
	int err = E_HM_OK;
	unsigned int rb_index;
	struct kev_config config;
	err = kev_read_event_config(kev_type, &config);
	if (err == E_HM_OK) {
		err = kev_buffer_config_index_of(kev_type, &config, &rb_index);
	}

	if (err == E_HM_OK) {
		if (ret_rb_index != NULL) {
			*ret_rb_index = rb_index;
		}
		if (ret_rb_size != NULL) {
			*ret_rb_size = (size_t)config.size;
		}
		if (ret_boot_offset != NULL) {
			*ret_boot_offset = (uint64_t)config.boot_offset;
		}
	}

	return err;
}

int kev_iter_last_data(unsigned int kev_type, size_t max_read_len,
		kev_iter_func_t it_func, uintptr_t arg, size_t *kev_data_len)
{
	int err = E_HM_OK;
	unsigned int rb_index = 0U;
	size_t rb_size = 0U;
	uint64_t rb_boot_offset = 0U;
	ssize_t ret_len = 0;

	err = read_event_config(kev_type, &rb_index, &rb_size, &rb_boot_offset);
	if (err == E_HM_OK) {
		ret_len = do_iter_kev_data(rb_index, rb_boot_offset, min(max_read_len, rb_size), it_func, arg);
		if (ret_len < 0) {
			err = (int)ret_len;
			hm_warn("Failed to iter kev data. err:%s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK && kev_data_len != NULL) {
		*kev_data_len = ret_len;
	}

	return err;
}

int kev_iter_current_data(unsigned int kev_type, size_t max_read_len,
		kev_iter_func_t it_func, uintptr_t arg, size_t *kev_data_len)
{
	int err = E_HM_OK;
	unsigned int rb_index = 0U;
	size_t rb_size = 0U;
	uint64_t read_offset = 0U;
	ssize_t ret_len = 0;

	err = read_event_config(kev_type, &rb_index, &rb_size, NULL);
	if (err == E_HM_OK) {
		err = read_current_head(rb_index, &read_offset);
	}

	if (err == E_HM_OK) {
		ret_len = do_iter_kev_data(rb_index, read_offset, min(max_read_len, rb_size), it_func, arg);
		if (ret_len < 0) {
			err = (int)ret_len;
		}
	}
	if (err == E_HM_OK && kev_data_len != NULL) {
		*kev_data_len = ret_len;
	}

	return err;
}

static int format_klog_prefix(const struct kev_klog_s *klog, char *buf, unsigned int buf_len)
{
	int err = E_HM_OK;
	int len = 0;

	if (buf_len > 0U) {
#ifdef CONFIG_KEV_KLOG_TCB_INFO
		const struct kev_tcb_info_s *tcb_info = ptr_cast(klog->data, const struct kev_tcb_info_s);
		len = snprintf_s(buf, buf_len, buf_len - 1U, "[%u:pid=%-4d tid=%-4d comm=%s]",
				 klog->header.cpuid, tcb_info->pid, tcb_info->tid, tcb_info->tcb_name);
#else
		len = snprintf_s(buf, buf_len, buf_len - 1U, "[%u]", klog->header.cpuid);
#endif
	}

	if (len <= 0) {
		err = E_HM_INVAL;
	}

	return (err == E_HM_OK) ? len : err;
}

static int format_klog_data(const char *log_content, char *buf, unsigned int buf_len)
{
	int err = E_HM_OK;
	int log_len = strlen(log_content);
	int ret = memcpy_s(buf, buf_len, log_content, log_len);
	if (ret != 0) {
		err = E_HM_NOMEM;
	}

	return (err == E_HM_OK) ? log_len : err;
}

/*
 * format the klog info in struct to final klog item.
 *
 * the klog item format as follows:
 * [$cpuid:pid=$pid tid=$tid comm=$comm][$timestamp][$level_text]$msgbody
 *  <--- klog_prefix ------------------><------ klog_data -------------->
 */
int kev_klog_format(const struct kev_klog_s *klog, char *buf, unsigned int buf_len)
{
#define advance(__buf, __remain_len, __total, __ret_len) \
	if ((__ret_len) >= 0) { \
		(__total) += (__ret_len); \
		(__buf) += (__ret_len); \
		(__remain_len) -= ((typeof(__remain_len))(__ret_len)); \
	}
	int ret;
	int total = 0;
	unsigned int remain_len = buf_len;

	ret = format_klog_prefix(klog, buf, remain_len);
	if (ret >= 0) {
		advance(buf, remain_len, total, ret);
		ret = format_klog_data(KEV_KLOG_TEXT(klog), buf, remain_len);
	}
	if (ret >= 0) {
		advance(buf, remain_len, total, ret);
		if (remain_len > 0 && buf[-1] != '\n') {
			/* append tail \n if missed */
			*buf = '\n';
			total++;
		}
	}

#undef advance

	return (ret >= 0) ? total : ret;
}

/* 128 for prefix, 512 for content, 1 for trailing newline */
#define LOG_BUF_SIZE (512U + 128U + 1U)
struct klog_iter_arg_s {
	klog_dump_func_t dump_func;
	uintptr_t dump_arg;
};

static int parse_one_klog(const struct kev_s *event, uintptr_t arg)
{
	int err = E_HM_OK;
	unsigned int evt_type = event->header.type;
	struct klog_iter_arg_s *iter_arg = (struct klog_iter_arg_s *)arg;
	char buffer[LOG_BUF_SIZE] = {};
	if (evt_type == KEV_TYPE_KLOG) {
		const struct kev_header_s *header = &event->header;
		const struct kev_klog_s *klog = container_of(header, struct kev_klog_s, header);
		int len = kev_klog_format(klog, buffer, sizeof(buffer));
		if (len < 0) {
			hm_warn("Failed to format klog. seq=%u\n", klog->sequence);
			err = E_HM_OK; /* skip this log when it can not be formatted */
		} else {
			int dump_ret = iter_arg->dump_func(buffer, len, iter_arg->dump_arg);
			if (dump_ret != 0) {
				err = E_HM_INVAL;
			}
		}
	}

	return err;
}

int kev_iter_last_klog(unsigned long max_read_len, klog_dump_func_t dump_func, uintptr_t arg)
{
	struct klog_iter_arg_s iter_arg = {
		.dump_func = dump_func,
		.dump_arg = arg
	};

	return kev_iter_last_data(KEV_TYPE_KLOG, max_read_len, parse_one_klog, (uintptr_t)&iter_arg, NULL);
}

int kev_iter_current_klog(unsigned long max_read_len, klog_dump_func_t dump_func, uintptr_t arg)
{
	struct klog_iter_arg_s iter_arg = {
		.dump_func = dump_func,
		.dump_arg = arg
	};

	return kev_iter_current_data(KEV_TYPE_KLOG, max_read_len, parse_one_klog, (uintptr_t)&iter_arg, NULL);
}

int kev_iter_on_klog_data(const char *buffer, size_t buflen, klog_dump_func_t dump_func, uintptr_t arg)
{
	int err;
	size_t event_len = 0;

	struct klog_iter_arg_s iter_arg = {
		.dump_func = dump_func,
		.dump_arg = arg
	};

	err = visit_event(buffer, buflen, parse_one_klog, (uintptr_t)(void *)&iter_arg, &event_len);
	return (err == E_HM_OK) ? (int)event_len : err;
}

int kev_read_klog_data(char *buffer, size_t buflen, ssize_t offset, size_t *ret_read_len)
{
	int err = E_HM_OK;
	struct kev_config config;
	size_t target_read_len = buflen;
	unsigned int rb_index;
	uint64_t real_offset;
	ssize_t read_len = 0;

	err = kev_read_event_config(KEV_TYPE_KLOG, &config);
	if (err == E_HM_OK) {
		if (target_read_len > config.size) {
			target_read_len = config.size;
		}
	}
	if (err == E_HM_OK) {
		err = kev_buffer_config_index_of(KEV_TYPE_KLOG, &config, &rb_index);
	}
	if (err == E_HM_OK) {
		if (offset < 0) {
			real_offset = (uint64_t)config.boot_offset;
		} else {
			real_offset = (uint64_t)offset;
		}
		read_len = kev_read_data(rb_index, real_offset, buffer, target_read_len);
		if (read_len < 0) {
			hm_error("Failed to read klog. err:%s\n", hmstrerror(read_len));
			err = read_len;
		}
	}

	if (ret_read_len != NULL) {
		*ret_read_len = (size_t)read_len;
	}

	return err;
}