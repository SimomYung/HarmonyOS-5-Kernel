/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Libdh prototypes for kernfs support
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 21 19:42:39 2020
 */

#ifndef ULIBS_LIBDEVHOST_KERNFS_H
#define ULIBS_LIBDEVHOST_KERNFS_H

#include <errno.h>
#include <lib/errno.h>
#include <hongmeng/errno.h>
#include <stdint.h>
#include <stdlib.h>

#define TAG_MASK		0x00FFFFFFU
#define REMOTE_TAG_MASK		0x0FFFFFFFU
#define REMOTE_MASK		0x80000000U
#define REMOTE_BACKEND_MASK	0x8000000U

#define LIBDH_KERNFS_TAG_MIN	1
#define LIBDH_KERNFS_TAG_MAX	((int)(TAG_MASK))

#define mask_tag_of(tagid, mask)	\
	(((unsigned int)(tagid)) & (mask))

#define remote_tag_of(tagid)	mask_tag_of(tagid, REMOTE_TAG_MASK)
#define local_tag_of(tagid)	mask_tag_of(tagid, TAG_MASK)

static inline int mk_mask_tag(unsigned int tagid, unsigned int mask)
{
	unsigned int result = tagid | mask;
	return (int)result;
}

#define mk_remote_tag(tagid) mk_mask_tag(tagid, REMOTE_MASK)

#define is_mask_tag(tagid, mask)	\
	((((unsigned int)(tagid)) & (mask)) == (mask))

#define is_remote_tag(tagid) is_mask_tag(tagid, REMOTE_MASK)
/* Internal structure of tagid
 * +------------------------------+-----------------+--------------------+--------------------------+
 * | 1 bits type (devhost/devmgr) | 3 bits reserved | 4 bits fstype (16) | 24 bits tagid (0xffffff) |
 * +------------------------------+-----------------+--------------------+--------------------------+
 */

enum DEVHOST_FS_TYPE {
	DEVHOST_COMMON = 0,
	DEVHOST_SYSFS = 1,
	DEVHOST_SYSFS_NS = 2,
	DEVHOST_PROCFS = 3,
	DEVHOST_FS_MAX,
};

#define DEVHOST_REMOTE_MASK	0xF000000U
#define is_devhost_remote_tag(tagid) ((((unsigned int)(tagid)) & DEVHOST_REMOTE_MASK) != 0U)

#define DEVHOST_SYSFS_MASK	DEVHOST_REMOTE_MASK
#define DEVHOST_SYSFS_BIT	24U
#define DEVHOST_TYPE2MASK(flag)	(((flag) << DEVHOST_SYSFS_BIT) & DEVHOST_SYSFS_MASK)
#define DEVHOST_ID2TYPE(id)	(((id) & DEVHOST_SYSFS_MASK) >> DEVHOST_SYSFS_BIT)

#define DEFINE_DEVHOST_MK_TAG(fstype_name, flag, ...)	\
static const unsigned long fstype_name##_mask = DEVHOST_TYPE2MASK(flag);	\
static inline int mk_##fstype_name##_tag(unsigned int id)	\
{						\
	return mk_mask_tag(id, fstype_name##_mask);		\
}						\
\
static inline int \
devhost_require_##fstype_name##_range(int *min, int *max)	\
{								\
	if ((min == NULL) || (max == NULL) ||			\
	    (DEVHOST_FS_MAX >= (DEVHOST_ID2TYPE(DEVHOST_REMOTE_MASK)))) {	\
		return -EINVAL;					\
	}							\
	*min = LIBDH_KERNFS_TAG_MIN;				\
	*max = LIBDH_KERNFS_TAG_MAX;				\
	return 0;						\
}

typedef int (*liblinux_require_frange_fn_t)(int *min, int *max);

/* fs type tag */
DEFINE_DEVHOST_MK_TAG(sysfs, (unsigned int)DEVHOST_SYSFS)
DEFINE_DEVHOST_MK_TAG(procfs, (unsigned int)DEVHOST_PROCFS)
DEFINE_DEVHOST_MK_TAG(sysfsns, (unsigned int)DEVHOST_SYSFS_NS)

#define FAKE_FD (~0ULL)

struct libdh_kernfs_ops_simple;
int libdh_kernfs_register(int type, void *ops);
struct libdh_kernfs_ops_simple *libdh_get_kernfs_ops(unsigned int tagid, uintptr_t ctx);
int libdh_devhost_flush_async_kernfs(void);


struct kernfs_entry;

struct libdh_kernfs_rw_args {
	unsigned long long pos;
	unsigned long ubuf;
	unsigned long size;
};

struct libdh_kernfs_ops {
	/* CAUSION: this ops should return hm errno */
	int (*read)(const struct kernfs_entry *entry,
		    const struct libdh_kernfs_rw_args *args,
		    unsigned long *rsize);
	int (*write)(const struct kernfs_entry *entry,
		     const struct libdh_kernfs_rw_args *args,
		     unsigned long *wsize);
	int (*poll)(const struct kernfs_entry *entry,
		    bool pollable);
};

struct libdh_kernfs_ops_simple {
	/* CAUSION: this ops should return POSIX errno */
	int (*open)(void *pathname, unsigned long long *fd, unsigned int flags, unsigned int mode);
	int (*close)(unsigned long long fd);
	int (*read)(unsigned long long fd, unsigned long long pos, unsigned long buf,
		    unsigned long size, size_t *rsize);
	int (*write)(unsigned long long fd, unsigned long long pos, unsigned long buf,
		     unsigned long size, size_t *wsize);
	int (*poll)(unsigned int id, bool pollable);
	int (*flush)(void);
};

int libdh_fs_check(uintptr_t cnode_index);

#endif	/* ULIBS_LIBDEVHOST_KERNFS_H */
