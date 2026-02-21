/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Define posix cap and check methods
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 07 09:16:20 2019
 */
#ifndef __POSIX_CAP_DEF_H_
#define __POSIX_CAP_DEF_H_

#include <stdint.h>
#include <hmkernel/const.h>
#include <hmkernel/errno.h>

#define POSIX_CAP_CHOWN            0
#define POSIX_CAP_DAC_OVERRIDE     1
#define POSIX_CAP_DAC_READ_SEARCH  2
#define POSIX_CAP_FOWNER           3
#define POSIX_CAP_FSETID           4
#define POSIX_CAP_KILL             5
#define POSIX_CAP_SETGID           6
#define POSIX_CAP_SETUID           7
#define POSIX_CAP_SETPCAP          8
#define POSIX_CAP_LINUX_IMMUTABLE  9
#define POSIX_CAP_NET_BIND_SERVICE 10
#define POSIX_CAP_NET_BROADCAST    11
#define POSIX_CAP_NET_ADMIN        12
#define POSIX_CAP_NET_RAW          13
#define POSIX_CAP_IPC_LOCK         14
#define POSIX_CAP_IPC_OWNER        15
#define POSIX_CAP_SYS_MODULE       16
#define POSIX_CAP_SYS_RAWIO        17
#define POSIX_CAP_SYS_CHROOT       18
#define POSIX_CAP_SYS_PTRACE       19
#define POSIX_CAP_SYS_PACCT        20
#define POSIX_CAP_SYS_ADMIN        21
#define POSIX_CAP_SYS_BOOT         22
#define POSIX_CAP_SYS_NICE         23
#define POSIX_CAP_SYS_RESOURCE     24
#define POSIX_CAP_SYS_TIME         25
#define POSIX_CAP_SYS_TTY_CONFIG   26
#define POSIX_CAP_MKNOD            27
#define POSIX_CAP_LEASE            28
#define POSIX_CAP_AUDIT_WRITE      29
#define POSIX_CAP_AUDIT_CONTROL    30
#define POSIX_CAP_SETFCAP          31
#define POSIX_CAP_MAC_OVERRIDE     32
#define POSIX_CAP_MAC_ADMIN        33
#define POSIX_CAP_SYSLOG           34
#define POSIX_CAP_WAKE_ALARM       35
#define POSIX_CAP_BLOCK_SUSPEND    36
#define POSIX_CAP_AUDIT_READ       37
#define POSIX_CAP_PERFMON          38
#define POSIX_CAP_BPF              39
#define POSIX_CAP_LAST_CAP         POSIX_CAP_BPF

#define posix_cap_valid(x) ((x) <= (unsigned long)POSIX_CAP_LAST_CAP)

#define POSIX_CAP_NR ((unsigned int)POSIX_CAP_LAST_CAP + 1U)

#define COMPAT_CAPABILITY_VERSION_1  0x19980330
#define COMPAT_CAPABILITY_U32S_1     1

#define COMPAT_CAPABILITY_VERSION_2  0x20071026
#define COMPAT_CAPABILITY_U32S_2     2

#define COMPAT_CAPABILITY_VERSION_3  0x20080522
#define COMPAT_CAPABILITY_U32S_3     2

#define COMPAT_CAP_LENGTH 32U

typedef uint64_t posix_cap_t;

typedef struct __posix_cap_header_struct {
	uint32_t version;
	int pid;
} *compat_cap_header_t;

typedef struct __posix_cap_data_struct {
	uint32_t effective;
	uint32_t permitted;
	uint32_t inheritable;
} *compat_cap_data_t;

typedef struct __compat_cap_struct {
	uint32_t cap[COMPAT_CAPABILITY_U32S_3];
} compat_cap_t;

#define HM_VFS_CAP_REV_MASK		__U(0xFF000000)
#define HM_VFS_CAP_REV_SHIFT		__U(24)
#define HM_VFS_CAP_FLAGS_MASK		~HM_VFS_CAP_REV_MASK
#define HM_VFS_CAP_FLAGS_EFFECTIVE	__U(0x000001)

#define HM_VFS_CAP_REV_1		0x01000000
#define HM_VFS_CAP_U32_1		__U(1)
#define HM_VFS_CAP_XATTR_SIZE_1		(sizeof(uint32_t) * (size_t)(__U(1) + __U(2) * HM_VFS_CAP_U32_1))

#define HM_VFS_CAP_REV_2		0x02000000
#define HM_VFS_CAP_U32_2		__U(2)
#define HM_VFS_CAP_XATTR_SIZE_2		(sizeof(uint32_t) * (size_t)(__U(1) + __U(2) * HM_VFS_CAP_U32_2))

#define HM_VFS_CAP_REV_3		0x03000000
#define HM_VFS_CAP_U32_3		__U(2)
#define HM_VFS_CAP_XATTR_SIZE_3		(sizeof(uint32_t) * (size_t)(__U(2) + __U(2) * HM_VFS_CAP_U32_3))

#define HM_VFS_CAP_REV			HM_VFS_CAP_REV_2
#define HM_VFS_CAP_U32			HM_VFS_CAP_U32_2
#define HM_VFS_CAP_XATTR_SIZE		HM_VFS_CAP_XATTR_SIZE_2

#define HM_CAP_OPT_NONE 0
#define HM_CAP_OPT_NOAUDIT 1
#define HM_CAP_OPT_INSETID 2

typedef struct __sec_vfs_cap_data_struct {
	uint32_t mask;
	struct {
		uint32_t permitted;
		uint32_t inheritable;
	} cap_data[HM_VFS_CAP_U32];
} sec_vfs_cap_data_t;

static inline
compat_cap_t posix_cap_to_compat(posix_cap_t cap_set)
{
	compat_cap_t ret;
	ret.cap[0] = (uint32_t)cap_set;
	ret.cap[1] = (uint32_t)(cap_set >> COMPAT_CAP_LENGTH);

	return ret;
}

static inline
posix_cap_t compat_cap_to_posix(compat_cap_t compat_cap)
{
	posix_cap_t ret;
	ret = (posix_cap_t)compat_cap.cap[1];
	ret = ret << COMPAT_CAP_LENGTH;
	ret += (posix_cap_t)compat_cap.cap[0];

	return ret;
}

#define posix_cap_to_mask(x) ((posix_cap_t)(1) << (x))
#define EMPTY_POSIX_CAP ((posix_cap_t)0)
#define FULL_POSIX_CAP_MASK (posix_cap_to_mask(POSIX_CAP_LAST_CAP + 1) - 1U)
#define FULL_POSIX_CAP ((posix_cap_t)FULL_POSIX_CAP_MASK)

#define compat_cap_to_index(x) ((x) >> 5)
#define compat_cap_to_mask(x) ((uint32_t)1 << ((x) & (unsigned int)31))
#define COMPAT_CAP_HIGH_MASK ((compat_cap_to_mask((unsigned int)POSIX_CAP_LAST_CAP + \
						  (unsigned int)1) - (uint32_t)1))
#define EMPTY_COMPAT_CAP ((compat_cap_t){{ 0, 0 }})
#define FULL_COMPAT_CAP ((compat_cap_t){{ ~0, COMPAT_CAP_HIGH_MASK }})

#define posix_cap_add(c, flag)  ((posix_cap_t)((c) |= posix_cap_to_mask(flag)))
#define posix_cap_cut(c, flag)  ((posix_cap_t)((c) &= ~posix_cap_to_mask(flag)))
#define posix_cap_added(c, flag) ((c) & posix_cap_to_mask(flag))

#define posix_sbits_mask(x) (1U << (x))
#define POSIX_SBITS_NOROOT			0U
#define POSIX_SBITS_NOROOT_LOCKED		1U
#define POSIX_SBITS_NO_SETUID_FIXUP		2U
#define POSIX_SBITS_NO_SETUID_FIXUP_LOCKED	3U
#define POSIX_SBITS_KEEP_CAPS			4U
#define POSIX_SBITS_KEEP_CAPS_LOCKED		5U
#define POSIX_SBITS_NO_AMBIENT_ADD		6U
#define POSIX_SBITS_NO_AMBIENT_ADD_LOCKED	7U
#define POSIX_SBITS_ALL_LOCKS (posix_sbits_mask(POSIX_SBITS_NOROOT_LOCKED) | \
			       posix_sbits_mask(POSIX_SBITS_NO_SETUID_FIXUP_LOCKED) | \
			       posix_sbits_mask(POSIX_SBITS_KEEP_CAPS_LOCKED) | \
			       posix_sbits_mask(POSIX_SBITS_NO_AMBIENT_ADD_LOCKED))
#define POSIX_SBITS_BITS (POSIX_SBITS_ALL_LOCKS >> 1U)

static int pack_cap_data_from_compat(compat_cap_data_t datap, compat_cap_t eff,
	compat_cap_t per, compat_cap_t inh, uint32_t version);
static int unpack_cap_data_to_compat(compat_cap_t *eff, compat_cap_t *per,
	compat_cap_t *inh, compat_cap_data_t datap, uint32_t version);

static int pack_cap_data_from_compat(
	compat_cap_data_t datap,
	compat_cap_t eff,
	compat_cap_t per,
	compat_cap_t inh,
	uint32_t version)
{
	if (version == COMPAT_CAPABILITY_VERSION_1) {
		datap[0].effective = eff.cap[0];
		datap[0].permitted = per.cap[0];
		datap[0].inheritable = inh.cap[0];
	} else if (version == COMPAT_CAPABILITY_VERSION_2 ||
			   version == COMPAT_CAPABILITY_VERSION_3) {
		datap[0].effective = eff.cap[0];
		datap[1].effective = eff.cap[1];
		datap[0].permitted = per.cap[0];
		datap[1].permitted = per.cap[1];
		datap[0].inheritable = inh.cap[0];
		datap[1].inheritable = inh.cap[1];
	} else {
		return E_HM_INVAL;
	}
	return E_HM_OK;
}

static inline int pack_cap_data_from_posix(
	compat_cap_data_t datap,
	posix_cap_t eff,
	posix_cap_t per,
	posix_cap_t inh,
	uint32_t version)
{
	return pack_cap_data_from_compat(
		datap,
		posix_cap_to_compat(eff),
		posix_cap_to_compat(per),
		posix_cap_to_compat(inh),
		version
	);
}

static int unpack_cap_data_to_compat(
	compat_cap_t *eff,
	compat_cap_t *per,
	compat_cap_t *inh,
	compat_cap_data_t datap,
	uint32_t version)
{
	if (version == COMPAT_CAPABILITY_VERSION_1) {
		eff->cap[0] = datap[0].effective;
		eff->cap[1] = 0;
		per->cap[0] = datap[0].permitted;
		per->cap[1] = 0;
		inh->cap[0] = datap[0].inheritable;
		inh->cap[1] = 0;
	} else if (version == COMPAT_CAPABILITY_VERSION_2 ||
		   version == COMPAT_CAPABILITY_VERSION_3) {
		eff->cap[0] = datap[0].effective;
		eff->cap[1] = datap[1].effective;
		per->cap[0] = datap[0].permitted;
		per->cap[1] = datap[1].permitted;
		inh->cap[0] = datap[0].inheritable;
		inh->cap[1] = datap[1].inheritable;

		eff->cap[1] &= COMPAT_CAP_HIGH_MASK;
		per->cap[1] &= COMPAT_CAP_HIGH_MASK;
		inh->cap[1] &= COMPAT_CAP_HIGH_MASK;
	} else {
		return E_HM_INVAL;
	}
	return E_HM_OK;
}

static inline int unpack_cap_data_to_posix(
	posix_cap_t *eff,
	posix_cap_t *per,
	posix_cap_t *inh,
	compat_cap_data_t datap,
	uint32_t version)
{
	compat_cap_t com_eff, com_per, com_inh;
	int err = unpack_cap_data_to_compat(
		&com_eff, &com_per, &com_inh,
		datap, version);
	if (err == E_HM_OK) {
		*eff = compat_cap_to_posix(com_eff);
		*per = compat_cap_to_posix(com_per);
		*inh = compat_cap_to_posix(com_inh);
	}
	return err;
}

#endif
