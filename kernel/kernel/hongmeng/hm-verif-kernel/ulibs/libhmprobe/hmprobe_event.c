/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implement four interfaces for hmprobe
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 03 11:39:48 2020
 */

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libhmprobe/hmprobe.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <lib/hmpsf/hmpsf_def.h>
#include <lib/hmprobe/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/hm_lsyscall.h>
#include <hmasm/lsyscall.h>
#include <unistd.h>

static int hmprobe_init_kev_attr(struct hmprobe_event_host_attr_s *host_attr,
				 struct hmprobe_event_attr_s *attr)
{
	int err = E_HM_OK;

	host_attr->vspaceid = HMPSF_VSPACE_KERNEL;
	host_attr->event_type = HMPROBE_TRACEPOINT_EVENT;
	if (attr->event_descriptor.trace_point.tp_name != NULL) {
		err = strcpy_s(host_attr->ev_point.kev_point.kev_name,
			       HMPROBE_EVENT_NAME_MAX_SIZE,
			       attr->event_descriptor.trace_point.tp_name);
	} else {
		err = E_HM_INVAL;
	}

	return err;
}

static int hmprobe_init_uev_attr(struct hmprobe_event_host_attr_s *host_attr,
				 struct hmprobe_event_attr_s *attr)
{
	int err = E_HM_OK;

	host_attr->vspaceid = HMPSF_VSPACE_SYSMGR;
	host_attr->event_type = HMPROBE_TRACEPOINT_EVENT;
	if (attr->event_descriptor.trace_point.tp_name != NULL) {
		err = strcpy_s(host_attr->ev_point.uev_point.uev_name,
			       HMPROBE_EVENT_NAME_MAX_SIZE,
			       attr->event_descriptor.trace_point.tp_name);
	} else {
		err = E_HM_INVAL;
	}

	return err;
}


static int hmprobe_init_uprobe_host_attr(struct hmprobe_event_host_attr_s *host_attr,
					 struct hmprobe_event_attr_s *attr)
{
	int err = E_HM_OK;

	host_attr->vspaceid = HMPSF_VSPACE_SYSMGR;
	host_attr->event_type = HMPROBE_PROBE_EVENT;
	host_attr->ev_point.uprobe_point.pid = attr->event_descriptor.uprobe.pid;

	if (attr->event_descriptor.uprobe.file_path != NULL) {
		err = strcpy_s(host_attr->ev_point.uprobe_point.file_path,
				HMPROBE_FILE_PATH_MAX_SIZE,
				attr->event_descriptor.uprobe.file_path);
		if (err == E_HM_OK) {
			if (attr->event_descriptor.uprobe.symbol_name != NULL) {
				host_attr->ev_point.uprobe_point.file_offset = 0;
				err = strcpy_s(host_attr->ev_point.uprobe_point.symbol_name,
					       HMPROBE_SYMBOL_NAME_MAX_SIZE,
					       attr->event_descriptor.uprobe.symbol_name);
			} else if (attr->event_descriptor.uprobe.file_offset != 0) {
				   host_attr->ev_point.uprobe_point.file_offset = attr->event_descriptor.uprobe.file_offset;
			} else {
				err = E_HM_INVAL;
			}
		}
	} else {
		err = E_HM_INVAL;
	}

	return err;
}

static int hmprobe_init_uprobe_attr(struct hmprobe_event_host_attr_s *host_attr,
				    struct hmprobe_event_attr_s *attr)
{
	host_attr->ev_point.uprobe_point.is_ret = false;

	return hmprobe_init_uprobe_host_attr(host_attr, attr);
}

static int hmprobe_init_ret_uprobe_attr(struct hmprobe_event_host_attr_s *host_attr,
					struct hmprobe_event_attr_s *attr)
{
	host_attr->ev_point.uprobe_point.is_ret = true;

	return hmprobe_init_uprobe_host_attr(host_attr, attr);
}

int hmprobe_open(struct hmprobe_event_attr_s *attr, unsigned int *event_id)
{
	int err = E_HM_OK;
	struct hmprobe_event_host_attr_s host_attr;

	NOFAIL(memset_s(&host_attr, sizeof(struct hmprobe_event_host_attr_s),
			0, sizeof(struct hmprobe_event_host_attr_s)));

	if (attr == NULL || event_id == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		switch (attr->type) {
		case HMPROBE_KEV_TYPE:
			err = hmprobe_init_kev_attr(&host_attr, attr);
			break;
		case HMPROBE_UEV_TYPE:
			err = hmprobe_init_uev_attr(&host_attr, attr);
			break;
		case HMPROBE_UPROBE_TYPE:
			err = hmprobe_init_uprobe_attr(&host_attr, attr);
			break;
		case HMPROBE_RET_UPROBE_TYPE:
			err = hmprobe_init_ret_uprobe_attr(&host_attr, attr);
			break;
		default:
			err = E_HM_INVAL;
			break;
		}
	}

	if (err == E_HM_OK) {
		err = lsyscall_syscall4(__NR_hmprobe_event_ctl, HMPROBE_EVENT_OPEN,
					(long)&host_attr, -1, 0);
		if (err >= 0) {
			*event_id = err;
			err = 0;
		} else {
			err = posix2hmerrno(-err);
		}
	}

	return err;
}

int hmprobe_close(unsigned int event_id)
{
	int err = lsyscall_syscall4(__NR_hmprobe_event_ctl, HMPROBE_EVENT_CLOSE,
				    0, event_id, 0);

	return posix2hmerrno(-err);
}

int hmprobe_attach(unsigned int module_id, const char *func_name, unsigned int event_id, unsigned int *prog_fd)
{
	int err = E_HM_OK;

	if (func_name == NULL || prog_fd == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		struct hmprobe_prog_attr_s prog_attr;

		NOFAIL(memset_s(&prog_attr, sizeof(prog_attr), 0, sizeof(prog_attr)));
		prog_attr.module_id = module_id;
		prog_attr.func_name = func_name;
		prog_attr.name_len = strlen(func_name);
		prog_attr.prog_fd = -1;
		err = lsyscall_syscall4(__NR_hmprobe_event_ctl, HMPROBE_EVENT_ATTACH,
					0, event_id, (long)&prog_attr);
		if (err >= 0) {
			*prog_fd = err;
			err = 0;
		} else {
			err = posix2hmerrno(-err);
		}
	}
	return err;
}

int hmprobe_detach(unsigned int prog_fd)
{
	struct hmprobe_prog_attr_s prog_attr;
	int err;

	NOFAIL(memset_s(&prog_attr, sizeof(prog_attr), 0, sizeof(prog_attr)));
	prog_attr.prog_fd = prog_fd;
	err = lsyscall_syscall4(__NR_hmprobe_event_ctl, HMPROBE_EVENT_DETACH,
				0, -1, (long)&prog_attr);

	return posix2hmerrno(-err);
}
