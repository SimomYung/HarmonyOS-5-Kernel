/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Convert the retval of hm vnotify interfaces to posix errno
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 10 22:34:01 2021
 */
#ifndef LIBDEVHOST_VPIPE_VNOTIFY_H
#define LIBDEVHOST_VPIPE_VNOTIFY_H

/*
 * NOTE: Now the <libdevhost/vpipe/vnotify.h> might be included when
 * compiling a liblinux module. To support this, cannot include any
 * unsupported-for-liblinux-module header files here.
 */
#include <libvpipe/vnotify_common.h>
#include <stdbool.h>

void libdh_vnotify_init(struct vnotify_s *vnotify);
int libdh_vnotify_attach_chn(struct vnotify_s *vnotify, const char *name, unsigned int flags);
int libdh_vnotify_detach_chn(const struct vnotify_s *vnotify, bool sync,
			     const struct timespec *timeout);
int libdh_vnotify_notify(const struct vnotify_s *vnotify);
int libdh_vnotify_receive(struct vnotify_s *vnotify,
			  const struct timespec *ts, unsigned int flags);
int libdh_vnotify_update_futex(struct vnotify_s *vnotify,
			       int *futex, bool shared);
int libdh_vnotify_read_status(const struct vnotify_s *vnotify,
			      unsigned int *status);

#endif /* LIBDEVHOST_VPIPE_VNOTIFY_H */
