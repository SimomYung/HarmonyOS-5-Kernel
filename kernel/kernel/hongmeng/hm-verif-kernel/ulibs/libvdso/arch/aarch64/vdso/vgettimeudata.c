/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Impelement vgettimeudata
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 17 15:41:05 2023
 */

#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <hmasm/types.h>
#include <hmkernel/mm/udata.h>
#include <hmkernel/barrier.h>
#include <hmkernel/compiler.h>
#include <string.h>
#include "vdso.h"

static int vtimeudata_read(struct kshare_udata_s *src, struct kshare_udata_s *dst)
{
	__u32 seq;
	int ret = 0;

	do {
		seq = __seqcnt_start_read(&src->tock_seq);
		ret = __safe_copy(dst, src, sizeof(struct kshare_udata_s));
		if (ret != 0) {
			ret = -EFAULT;
			break;
		}
	} while (__seqcnt_retry_read(&src->tock_seq, seq) != 0);

	return ret;
}

int __kernel_gettimeudata(struct kshare_udata_s *udata)
{
	int ret = 0;
	struct kshare_udata_s *vdata = NULL;

	if (udata == NULL) {
		ret = -EINVAL;
	}
	if (ret == 0) {
		vdata = (struct kshare_udata_s*)__vdso_get_data();
		if (vdata != NULL) {
			ret = vtimeudata_read(vdata, udata);
		} else {
			ret = -ENOSYS;
		}
	}

	return ret;
}
