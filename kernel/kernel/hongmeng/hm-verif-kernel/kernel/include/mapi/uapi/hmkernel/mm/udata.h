/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Udata
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 12 20:03:34 2019
 */
#ifndef UAPI_HMKERNEL_UDATA_H
#define UAPI_HMKERNEL_UDATA_H

#include <hmasm/page.h>
#include <hmasm/types.h>
#include <hmkernel/time.h>
#include <hmkernel/errno.h>
#include <hmkernel/seqlock.h>
#include <hmkernel/compiler.h>

#define UDATA_PMEM_SIZE PAGE_SIZE

struct kshare_udata_s {
	struct __seqcnt_s tock_seq;
	__s64 tock_offset;
	__u64 tock_last;
	__u64 tock_mono_nsec;
	__u64 tock_mono_sec;
	__u64 tock_mono_nsec_calib;
	__u64 sec_length;
	__u64 tock_mono_last;
	__u64 offs_boot_tock;
	__u64 offs_boot_sec;
	__u64 rtc_offset;
	__u64 rtc_lastts;
	__u64 pcs4offset;
	__s64 tock_base_sec;
	__u32 tock_base_nsec;
	__u32 freq;
	__u32 mul;
	__u32 mov;
	__u32 offs_boot_nsec;
	__s32 tz_minuteswest;
	__s32 tz_dsttime;
	__bool only_update_timezone;
};

#endif
