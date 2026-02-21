/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide xvm perfbuf APIs for xvm clients
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 23 11:41:30 2023
 */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmpsf/xvm/lsyscall.h>
#include <hmkernel/perf_event.h>
#include <hmasm/lsyscall.h>
#include <hmkernel/barrier.h>
#include <lib/perf_rb.h>
#include <libhmpsf/xvm/std/map/perfbuf.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <linux/perf_event.h>
#include <malloc.h>
#include <string.h>
#include <poll.h>
#include <libhmpsf/adapter/lsyscall.h>
#include "../../../adapter/lsyscall.h"


#define CAT_BUF_SIZE			0x10000
#define MSEC_PER_SEC			1000L
#define NSEC_PER_MSEC			1000000L

static void perfbuf_free_cpu_buf(struct perfbuf_s *pb, struct perfbuf_cpu_buf_s *cpu_buf)
{
	int ret;

	if (cpu_buf != NULL) {
		if (cpu_buf->base) {
			ret = hm_mem_munmap(cpu_buf->base, pb->buf_size + pb->page_size);
			if (ret != E_HM_OK) {
				hm_warn("failed to munmap cpu_buf\n");
			}
		}

		if (cpu_buf->fd >= 0) {
			lsyscall_syscall3(__NR_ioctl, (long)cpu_buf->fd, PERF_EVENT_IOC_DISABLE, 0);
			lsyscall_syscall1(__NR_close, (long)cpu_buf->fd);
		}

		free(cpu_buf);
	}
}

static void perfbuf_free(struct perfbuf_s *pb)
{
	unsigned int i;

	if (pb != NULL) {
		if (pb->cpu_bufs) {
			for (i = 0; i < pb->nr_cpus; i++) {
				perfbuf_free_cpu_buf(pb, pb->cpu_bufs[i]);
			}
			free(pb->cpu_bufs);
		}
		free(pb->pollfds);
		free(pb->cat_buf);
		free(pb);
	}
}

static struct perfbuf_cpu_buf_s *
perfbuf_open_cpu_buf(struct perfbuf_s *pb, struct perf_event_attr *attr, int cpu)
{
	pid_t pid;
	struct perfbuf_cpu_buf_s *cpu_buf;

	cpu_buf = calloc(1, sizeof(*cpu_buf));
	if (cpu_buf != NULL) {
		/* flag PERF_FLAG_FD_CLOEXEC is not support */
		if (pb->nr_cpus == 1) {
			pid = lsyscall_syscall0(__NR_gettid);
			cpu_buf->fd = lsyscall_syscall5(__NR_perf_event_open, ptr_to_long(attr), (long)pid, -1, -1, 0);
		} else {
			cpu_buf->fd = lsyscall_syscall5(__NR_perf_event_open, ptr_to_long(attr), -1, cpu, -1, 0);
		}

		if (cpu_buf->fd < 0) {
			hm_warn("perf_event open failed, cpu=%d, err=%d\n", cpu, cpu_buf->fd);
			perfbuf_free_cpu_buf(pb, cpu_buf);
			cpu_buf = NULL;
		} else {
			cpu_buf->base = hm_mem_mmap(NULL, pb->page_size + pb->buf_size,
						PROT_READ | PROT_WRITE, MAP_SHARED,
						cpu_buf->fd, 0);
			if (cpu_buf->base == MAP_FAILED) {
				cpu_buf->base = NULL;
				hm_warn("perf_event mmap failed, cpu=%d\n", cpu);
				perfbuf_free_cpu_buf(pb, cpu_buf);
				cpu_buf = NULL;
			}
		}

		if (cpu_buf != NULL) {
			if (lsyscall_syscall3(__NR_ioctl, (long)cpu_buf->fd, PERF_EVENT_IOC_ENABLE, 0) < 0) {
				hm_warn("perf_event enable failed, cpu=%d\n", cpu);
				perfbuf_free_cpu_buf(pb, cpu_buf);
				cpu_buf = NULL;
			}
		}
	}

	return cpu_buf;
}

static int perfbuf_prepare_pb(const char *name, struct xvm_perfbuf_params_s *params, struct perfbuf_s **o_pb)
{
	int ret = E_HM_OK;
	struct perfbuf_s *pb;

	pb = calloc(1, sizeof(*pb));
	if (pb == NULL) {
		ret = E_HM_NOMEM;
	}

	if (ret == E_HM_OK) {
		ret = strcpy_s(pb->base.name, XVM_MAP_MAX_NAME_LEN, name);
		if (ret != E_HM_OK) {
			free(pb);
			pb = NULL;
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		pb->sample_cb = params->sample_cb;
		pb->ctx = params->ctx;
		pb->page_size = PAGE_SIZE;
		pb->buf_size = params->buf_size;
		pb->nr_cpus = params->nr_cpus;
		pb->base.type = XVM_MAP_TYPE_PERFBUF;
		pb->base.scope = XVM_MAP_SCOPE_GLOBAL_SHARED;
		pb->base.participants = XVM_PARTICIPANT_SYSMGR_FLAG | XVM_PARTICIPANT_KERNEL_FLAG;

		pb->cpu_bufs = calloc(pb->nr_cpus, sizeof(*pb->cpu_bufs));
		pb->pollfds = calloc(pb->nr_cpus, sizeof(*pb->pollfds));
		pb->cat_buf = malloc(CAT_BUF_SIZE);
		if (pb->cpu_bufs == NULL || pb->pollfds == NULL || pb->cat_buf == NULL) {
			perfbuf_free(pb);
			pb = NULL;
			ret = E_HM_NOMEM;
		}
	}

	*o_pb = pb;

	return ret;
}

static void xvm_init_perf_event_attr(struct perf_event_attr *attr, struct xvm_perfbuf_params_s *params)
{
	attr->size = sizeof(struct perf_event_attr);
	attr->config = PERF_COUNT_SW_BPF_OUTPUT;
	attr->type = PERF_TYPE_SOFTWARE;
	attr->sample_type = PERF_SAMPLE_RAW;
	attr->sample_period = 1;
	attr->disabled = 1;
	attr->watermark = 1;
	attr->wakeup_watermark = params->wakeup_watermark;
}

static int xvm_call_perfbuf_elem_update(unsigned int map_id, unsigned int cpu, int fd)
{
	int ret = E_HM_OK;
	if (xvm_is_using_lsyscall()) {
		struct xvm_sc_perfbuf_elem_update_args_s args = {
			.map_id = map_id,
			.cpu = cpu,
			.fd = fd,
		};
		ret = xvm_lsyscall(XVM_SC_CMD_PERFBUF_ELEM_UPDATE, (unsigned long)&args);
	} else {
		ret = actvcall_hmcall_hmpsf_perfbuf_elem_update(map_id, cpu, fd);
	}

	return ret;
}

int xvm_perfbuf_create(xvm_map_handle_t *map, const char *name, struct xvm_perfbuf_params_s *params)
{
	int ret;
	unsigned int i;
	struct perf_event_attr attr = {};
	struct perfbuf_s *pb = NULL;

	if (map == NULL || name == NULL || params == NULL ||
	    params->nr_cpus == 0 || params->sample_cb == NULL ||
	    params->buf_size == 0 || (params->buf_size & (PAGE_SIZE - 1)) != 0) {
		ret = E_HM_INVAL;
	} else {
		ret = perfbuf_prepare_pb(name, params, &pb);
	}

	if (ret == E_HM_OK) {
		if (xvm_is_using_lsyscall()) {
			ret = xvm_lsyscall(XVM_SC_CMD_PERFBUF_CREATE, ptr_to_ulong(pb));
		} else {
			ret = actvcall_hmcall_hmpsf_perfbuf_create(pb);
		}

		if (ret != E_HM_OK) {
			 perfbuf_free(pb);
		}
	}

	if (ret == E_HM_OK) {
		xvm_init_perf_event_attr(&attr, params);
		for (i = 0; i < pb->nr_cpus; i++) {
			struct perfbuf_cpu_buf_s *cpu_buf;

			cpu_buf = perfbuf_open_cpu_buf(pb, &attr, i);
			if (cpu_buf == NULL) {
				hm_warn("failed to open cpu info, cpu=%d\n", i);
				ret = E_HM_NOMEM;
				break;
			}

			pb->cpu_bufs[i] = cpu_buf;
			pb->pollfds[i].fd = cpu_buf->fd;
			pb->pollfds[i].events = POLLIN;

			ret = xvm_call_perfbuf_elem_update(pb->base.id, i, cpu_buf->fd);
			if (ret != E_HM_OK) {
				hm_warn("failed to update cpu info, cpu=%d\n", i);
				break;
			}
		}

		if (ret != E_HM_OK) {
			 perfbuf_free(pb);
		} else {
			*map = pb;
		}
	}

	return ret;
}

static void perfbuf_process_events(struct perfbuf_s *pb, unsigned int cpu)
{
	struct perf_rb_mmap_page_s *user_page = (struct perf_rb_mmap_page_s *)pb->cpu_bufs[cpu]->base;
	void *data_base = (char *)user_page + pb->page_size;
	struct __perf_event_header *ehdr;
	__u64 data_head, data_tail;
	__u32 size;

	data_head = vatomic64_read(&user_page->data_head);
	data_tail = vatomic64_read(&user_page->data_tail);

	while (data_head != data_tail) {
		smp_rmb();
		ehdr = (struct __perf_event_header *)((char *)data_base + (data_tail & (pb->buf_size - 1)));
		size = ehdr->size;

		if ((char *)ehdr + size > (char *)data_base + pb->buf_size) {
			void *copy_start = ehdr;
			__u32 len_first = (__u32)((char *)data_base + pb->buf_size - (char *)copy_start);
			__u32 len_secnd = size - len_first;

			memcpy_s(pb->cat_buf, CAT_BUF_SIZE, copy_start, len_first);
			memcpy_s((void *)((long)pb->cat_buf + len_first), CAT_BUF_SIZE, data_base, len_secnd);
			ehdr = pb->cat_buf;
		}

		pb->sample_cb(pb->ctx, cpu, (void *)(ehdr + 1), size - sizeof(*ehdr) - ehdr->misc);
		smp_mb();
		vatomic64_add(&user_page->data_tail, size);
		data_head = vatomic64_read(&user_page->data_head);
		data_tail = vatomic64_read(&user_page->data_tail);
	}
}

int xvm_perfbuf_poll(xvm_map_handle_t map, int timeout)
{
	int ret = E_HM_INVAL;
	unsigned int i;
	struct timespec ts, *pts = NULL;
	struct perfbuf_s *pb = (struct perfbuf_s *)map;

	if (pb != NULL) {
		if (timeout >= 0) {
			ts.tv_sec  = timeout / MSEC_PER_SEC;
			ts.tv_nsec = (timeout % MSEC_PER_SEC) * NSEC_PER_MSEC;
			pts = &ts;
		}
		ret = lsyscall_syscall4(__NR_ppoll, ptr_to_long(pb->pollfds), (long)pb->nr_cpus, ptr_to_long(pts), 0);
		if (ret > 0) {
			ret = E_HM_OK;
		} else if (ret == 0) {
			ret = E_HM_TIMEDOUT;
		}
	}

	if (ret == E_HM_OK) {
		for (i = 0; i < pb->nr_cpus; i++) {
			if (!pb->pollfds[i].revents) {
				continue;
			}

			if (pb->pollfds[i].revents & POLLIN) {
				perfbuf_process_events(pb, i);
			}

			pb->pollfds[i].revents = 0;
		}
	}

	return ret;
}

void xvm_perfbuf_free(xvm_map_handle_t map)
{
	int ret;
	struct perfbuf_s *pb = (struct perfbuf_s *)map;

	if (pb != NULL) {
		if (xvm_is_using_lsyscall()) {
			ret = xvm_lsyscall(XVM_SC_CMD_PERFBUF_REMOVE, (unsigned long)pb->base.id);
		} else {
			ret = actvcall_hmcall_hmpsf_perfbuf_remove(pb->base.id);
		}

		if (ret != E_HM_OK) {
			hm_warn("invalid xvm map handle\n");
		} else {
			perfbuf_free((struct perfbuf_s *)pb);
		}
	}
}
