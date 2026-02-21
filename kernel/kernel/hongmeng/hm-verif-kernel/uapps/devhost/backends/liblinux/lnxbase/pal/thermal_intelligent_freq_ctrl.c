#include <devhost/log.h>

#include <cee/hmkernel/uapi/sched_ioctl.h>
#include <hongmeng/errno.h>

#include <lib/errno.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <liblinux/pal.h>

static rref_t hisi_sched_ctrl(void)
{
	static rref_t rref = 0ULL;

	if (rref == 0ULL) {
		rref = hm_module_open(HISI_SCHED_CLASS_NAME);
	}
	return rref;
}

int liblinux_pal_cooling_freq_limit(uint32_t freq_limit)
{
	int err;
	rref_t rref = hisi_sched_ctrl();
	if (IS_REF_ERR(rref)) {
		dh_error("module ioctl open failed! err is %s\n", hmstrerror(REF_TO_ERR(rref)));
		return -hmerrno2posix(REF_TO_ERR(rref));
	}
	err = hm_module_ioctl(rref, __SCHED_SET_FREQ_LIMIT, (void *)&freq_limit, sizeof(unsigned int), NULL, 0);
	if (err < 0) {
		dh_warn("module ioctl access failed! err is %s\n", hmstrerror(err));
		return -hmerrno2posix(err);
	}

	return 0;
}
