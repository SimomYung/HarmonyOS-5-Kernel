/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service APIs type
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#ifndef ULIBS_LIBHMSRV_IO_TYPE_H
#define ULIBS_LIBHMSRV_IO_TYPE_H

#include <stdint.h>
#include <hmkernel/const.h>
#include <hmkernel/capability.h>

struct hmsrv_io_ctx {
	rref_t dh_ap_rref;
	unsigned int flags;
	union {
		unsigned long long filp;    /* for generic device */
		int ifindex;                /* for network interface */
		int domain;                 /* for socket domain */
	};
	/*
	 * though `filp` was filp_node,
	 * file_id also be needed, i.e poll/epoll.
	 * now file_id is only used to netd.
	 */
	int file_id;
	__u32 dh_cnode_idx;
	unsigned int fastpath;
	unsigned long long kshm_key; /* for ctrlmem page Kshare */
	unsigned long kobj_offset;  /* for ctrlmem kobjctnr */
	unsigned int nonvapre; /* for mmap: don't prepare va at sysmgr ahead of time */
	unsigned int dev_fops_flags; /* for dev fops mask flags */
};

typedef struct hmsrv_io_ctx hmsrv_io_domain_ctx;

#define HMSRV_IO_CTX_FLAGS_OPEN_FORWARD		0x80000000  /* use forward for open/close */
#define HMSRV_IO_CTX_FLAGS_RW_FORWARD		0x40000000  /* use forward for read/write */
#define HMSRV_IO_CTX_FLAGS_IOCTL_FORWARD	0x20000000  /* use forward for ioctl      */
#define HMSRV_IO_CTX_FLAGS_CLOCK_FORWARD	0x10000000  /* use forward for posix-clock */
#define HMSRV_IO_CTX_FLAGS_CLOSE_NOREJECT	0x08000000  /* don't reject dh_ap_rref in close */

/* use forward actv transaction */
#define HMSRV_IO_CTX_FLAGS_ALL_FORWARD	((HMSRV_IO_CTX_FLAGS_OPEN_FORWARD) |	\
					 (HMSRV_IO_CTX_FLAGS_RW_FORWARD) |	\
					 (HMSRV_IO_CTX_FLAGS_IOCTL_FORWARD) |	\
					 (HMSRV_IO_CTX_FLAGS_CLOCK_FORWARD))

/* XXX Keep consistent with Linux */
typedef uint32_t devnum_t;
#define MINORBITS	20U
#define MINORMASK	((__U(1) << MINORBITS) - 1U)
#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma, mi)	(((ma) << MINORBITS) | (mi))

#define DEV_KMSG_MAJOR		1U
#define DEV_KMSG_MINOR		11U

#define BD_FEATURE_FLUSH_REDUCE	(1ULL << 25)

/* hyperhold device */
#define BD_FEATURE_HYPERHOLD	(1ULL << 26)

#define BD_FEATURE_MAS_BLK	(1ULL << 27)

/* read only device */
#define BD_FEATURE_RODEV	(1ULL << 28)

#define BD_FEATURE_DIRECTMAP	(1ULL << 29)
#define BD_FEATURE_DAS		(1ULL << 30)

/* for /dev/tty */
#define DEV_CTTY_MAJOR 5U
#define DEV_CTTY_MINOR 0U

#define HMSRV_IO_CTX_FASTPATH_FLAG		0x1
#define HMSRV_IO_CTX_FASTPATH_FLAG_SHIFT	1
#define HMSRV_IO_CTX_FASTPATH_RPCCNT_MASK	0xfffffffe

#define HMSRV_IO_CTX_DEVFOPS_NO_FLUSH_FLAG	0x00000001
#define HMSRV_IO_CTX_DEV_ZERO_FLAG		0x00000004

#define SMARTIO_BIO_TYPE_BG     0x00000001
#define SMARTIO_BIO_TYPE_KBG    0x00000002
#define SMARTIO_BIO_TYPE_SBG    0x00000004
#define SMARTIO_BIO_TYPE_FG     0x00000008
#define SMARTIO_BIO_TYPE_TA     0x00000010
#define BIO_HOT_DATA            0x00000100

#endif	/* ULIBS_LIBHMSRV_IO_TYPE_H */
