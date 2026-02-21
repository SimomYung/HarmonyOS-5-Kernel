/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Convert the retval of hm vnotify interfaces to posix errno
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 10 22:32:33 2021
 */
#include <libdevhost/vpipe/vnotify.h>
#include <libvpipe/vnotify.h>
#include <sys/time.h>
#include <hongmeng/errno.h>

void libdh_vnotify_init(struct vnotify_s *vnotify)
{
	hm_vnotify_init(vnotify);
}

int libdh_vnotify_attach_chn(struct vnotify_s *vnotify, const char *name, unsigned int flags)
{
	int err = hm_vnotify_attach_chn(vnotify, name, flags);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

int libdh_vnotify_detach_chn(const struct vnotify_s *vnotify, bool sync,
			     const struct timespec *timeout)
{
	int err = hm_vnotify_detach_chn(vnotify, sync, timeout);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

int libdh_vnotify_notify(const struct vnotify_s *vnotify)
{
	int err = hm_vnotify_notify(vnotify);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

int libdh_vnotify_receive(struct vnotify_s *vnotify,
			     const struct timespec *ts, unsigned int flags)
{
	int err = hm_vnotify_receive(vnotify, ts, flags);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

int libdh_vnotify_update_futex(struct vnotify_s *vnotify,
				  int *futex, bool shared)
{
	int err = hm_vnotify_update_futex(vnotify, futex, shared);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

int libdh_vnotify_read_status(const struct vnotify_s *vnotify,
				 unsigned int *status)
{
	int err = hm_vnotify_read_status(vnotify, status);
	return (err < 0) ? -hmerrno2posix(err) : err;
}
