// SPDX-License-Identifier: GPL-2.0

#include <hmkernel/mm/udata.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_vdso.h>
#include <devhost/log.h>

#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/pac.h>
#endif
#include "time.h"

long long tock_to_ns(__u64 tock, __u64 freq)
{
	return ((tock / freq) * NSEC_PER_SEC) + ((tock % freq) * NSEC_PER_SEC / freq);
}

static int do_gettimeudata_vdso(struct kshare_udata_s *udata)
{
	int ret = E_HM_NOSYS;
	static int (*fun)(struct kshare_udata_s*) = NULL;

	if (fun == NULL) {
		void *fun_addr = hm_lookup_vdso_sym("__kernel_gettimeudata");
		if (fun_addr != NULL) {
			fun = fun_addr;
		}
	}
	if (fun != NULL) {
#ifdef PAC_FWCFI_SYSSERVICE
		PAC_FECFI_XPAC_SIGN(fun, int(*)(struct kshare_udata_s*));
#endif
		ret = fun(udata);
		ret = posix2hmerrno(ret);
	}

	return ret;
}

int liblinux_read_time_udata(struct liblinux_time_udata *data)
{
	int ret;
	struct kshare_udata_s udata;

	mem_zero_s(udata);
	ret = do_gettimeudata_vdso(&udata);
	if (ret < 0) {
		dh_warn("get time udata failed: %s\n", hmstrerror(ret));
		return ret;
	}
	data->offs_real = tock_to_ns(udata.tock_offset, udata.freq);
	data->real = udata.tock_mono_sec * NSEC_PER_SEC + udata.tock_mono_nsec + data->offs_real;
	data->cycle_last = udata.tock_last;
	data->offs_boot = tock_to_ns(udata.offs_boot_tock, udata.freq);
	data->rtc_offset = udata.rtc_offset;
	data->rtc_lastts = data->real / NSEC_PER_SEC;
	data->tz_minuteswest = udata.tz_minuteswest;
	data->tz_dsttime = udata.tz_dsttime;
	data->only_update_timezone = udata.only_update_timezone;
	data->mono_sec = udata.tock_mono_sec;
	data->mono_nsec = udata.tock_mono_nsec;

	return 0;
}
