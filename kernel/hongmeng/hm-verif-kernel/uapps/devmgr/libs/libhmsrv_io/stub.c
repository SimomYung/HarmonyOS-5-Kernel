/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: stub for libdh IPC to selfcall functions
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */

#include <hongmeng/errno.h>

/*
 * Following definitions are the same as libhmsrv_io/blk.h
 *
 * These interfaces are used to transform IPC to selfcall when libdh
 * and libhmsrv_io are linked together.
 *
 * libdh_blk is a stub that returns E_HM_NOSYS as default.
 */
static inline int __libdh_stub(void)
{
	return E_HM_NODEV;
}

#define DEFINE_LIBDH_STUB(func) int func(void) __attribute__((weak, alias("__libdh_stub")))

DEFINE_LIBDH_STUB(libdh_blk_bio_polling);
DEFINE_LIBDH_STUB(libdh_blk_submit_bio);
DEFINE_LIBDH_STUB(libdh_blk_submit_cmd);
DEFINE_LIBDH_STUB(libdh_dev_open);
DEFINE_LIBDH_STUB(libdh_dev_close);
DEFINE_LIBDH_STUB(libdh_dev_ioctl);
DEFINE_LIBDH_STUB(libdh_dev_read);
DEFINE_LIBDH_STUB(libdh_dev_write);
DEFINE_LIBDH_STUB(libdh_dev_read_iter);
DEFINE_LIBDH_STUB(libdh_dev_write_iter);
DEFINE_LIBDH_STUB(libdh_dev_llseek);
DEFINE_LIBDH_STUB(libdh_dev_poll);
DEFINE_LIBDH_STUB(libdh_dev_epoll_ctl);
DEFINE_LIBDH_STUB(libdh_dev_fcntl);
DEFINE_LIBDH_STUB(libdh_dev_mmap);
DEFINE_LIBDH_STUB(libdh_dev_munmap);
