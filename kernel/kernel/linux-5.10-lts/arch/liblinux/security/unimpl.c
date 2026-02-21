// SPDX-License-Identifier: GPL-2.0-only
#include <linux/err.h>
#include <liblinux/unimpl.h>

/* security/lsm_audit.c */
LIBLINUX_UNIMPL(common_lsm_audit)

/* security/device_cgroup.c */
LIBLINUX_EXPORTED_UNIMPL_RET(devcgroup_check_permission, int, 0)
