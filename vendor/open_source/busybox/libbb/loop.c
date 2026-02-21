/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 * Copyright (C) 2005 by Rob Landley <rob@landley.net>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */
#include "libbb.h"
#ifndef __hongmeng
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)

/* For 2.6, use the cleaned up header to get the 64 bit API. */
// Commented out per Rob's request
//# include "fix_u32.h" /* some old toolchains need __u64 for linux/loop.h */
# include <linux/loop.h>
typedef struct loop_info64 bb_loop_info;
# define BB_LOOP_SET_STATUS LOOP_SET_STATUS64
# define BB_LOOP_GET_STATUS LOOP_GET_STATUS64

#else

/* For 2.4 and earlier, use the 32 bit API (and don't trust the headers) */
/* Stuff stolen from linux/loop.h for 2.4 and earlier kernels */
# include <linux/posix_types.h>
# define LO_NAME_SIZE        64
# define LO_KEY_SIZE         32
# define LOOP_SET_FD         0x4C00
# define LOOP_CLR_FD         0x4C01
# define BB_LOOP_SET_STATUS  0x4C02
# define BB_LOOP_GET_STATUS  0x4C03
typedef struct {
	int                lo_number;
	__kernel_dev_t     lo_device;
	unsigned long      lo_inode;
	__kernel_dev_t     lo_rdevice;
	int                lo_offset;
	int                lo_encrypt_type;
	int                lo_encrypt_key_size;
	int                lo_flags;
	char               lo_file_name[LO_NAME_SIZE];
	unsigned char      lo_encrypt_key[LO_KEY_SIZE];
	unsigned long      lo_init[2];
	char               reserved[4];
} bb_loop_info;
#endif
#else
#include <hongmeng/types.h>

# define LO_NAME_SIZE        64
# define LO_KEY_SIZE         32
# define LOOP_SET_FD         0x4C00
# define LOOP_CLR_FD         0x4C01
# define BB_LOOP_SET_STATUS  0x4C04
# define BB_LOOP_GET_STATUS  0x4C05
 
typedef struct loop_info64 {
	uint64_t    lo_device;			/* ioctl r/o */
	uint64_t    lo_inode;			/* ioctl r/o */
	uint64_t    lo_rdevice;			/* ioctl r/o */
	uint64_t    lo_offset;
	uint64_t    lo_sizelimit;/* bytes, 0 == max available */
	uint32_t    lo_number;			/* ioctl r/o */
	uint32_t    lo_encrypt_type;
	uint32_t    lo_encrypt_key_size;		/* ioctl w/o */
	uint32_t    lo_flags;
	uint8_t     lo_file_name[LO_NAME_SIZE];
	uint8_t     lo_crypt_name[LO_NAME_SIZE];
	uint8_t     lo_encrypt_key[LO_KEY_SIZE]; /* ioctl w/o */
	uint64_t    lo_init[2];
} bb_loop_info;
#endif

#if ENABLE_FEATURE_LOOP_ERROR_EXIT

/*
 * There are 256 error codes in posix.
 * To prevent the error codes from being extended,
 * the number of error codes is defined as 512.
 */
#ifndef NUM_POSIX
#define NUM_POSIX 512
#endif

/* Repeated mounting is allowed for resource occupation errors. */
static bool is_permitted_try_again[NUM_POSIX] = {
	[EAGAIN] = true,		/* Try again */
	[EBUSY] = true,			/* Device or resource busy */
	[ETXTBSY] = true,		/* Text file busy */
	[EDEADLK] = true,		/* Resource deadlock would occur */
	[ENOLCK] = true,		/* No record locks available */
	[ENOSR] = true,			/* Out of streams resources */
	[EUSERS] = true,		/* Too many users */
	[ENOBUFS] = true,		/* No buffer space available */
	[EALREADY] = true,		/* Operation already in progress */
	[EINPROGRESS] = true,	/* Operation now in progress */
	[EDQUOT] = true,		/* Quota exceeded */
};
#endif

char* FAST_FUNC query_loop(const char *device)
{
	int fd;
	bb_loop_info loopinfo;
	char *dev = NULL;

	fd = open(device, O_RDONLY);
	if (fd >= 0) {
		if (ioctl(fd, BB_LOOP_GET_STATUS, &loopinfo) == 0) {
			dev = xasprintf("%"OFF_FMT"u %s", (off_t) loopinfo.lo_offset,
					(char *)loopinfo.lo_file_name);
		}
		close(fd);
	}

	return dev;
}

int FAST_FUNC del_loop(const char *device)
{
	int fd, rc;

	fd = open(device, O_RDONLY);
	if (fd < 0)
		return 1;
	rc = ioctl(fd, LOOP_CLR_FD, 0);
	close(fd);

	return rc;
}

/* Obtain an unused loop device number */
int FAST_FUNC get_free_loop(void)
{
	int fd;
	int loopdevno;

	fd = open("/dev/loop-control", O_RDWR | O_CLOEXEC);
	if (fd == -1)
		return fd - 1; /* -2: "no /dev/loop-control" */

#ifndef LOOP_CTL_GET_FREE
# define LOOP_CTL_GET_FREE 0x4C82
#endif
	loopdevno = ioctl(fd, LOOP_CTL_GET_FREE);
	close(fd);
	return loopdevno; /* can be -1 if error */
}

#if ENABLE_FEATURE_LOOP_CONFIGURE
static int get_next_free_loop(char *dev, int id)
{
	int i = get_free_loop();
	if (i >= 0) {
		sprintf(dev, LOOP_FORMAT, i);
		return 1; /* use /dev/loop-control */
	} else if (i == -2) {
		sprintf(dev, LOOP_FORMAT, id);
		return 2;
	} else {
		return -1; /* no free loop devices */
	}
}

static int open_file(const char *file, unsigned flags, int *mode)
{
	int ffd;
	/* open the file.  barf if this doesn't work.  */
	*mode = (flags & BB_LO_FLAGS_READ_ONLY) ? O_RDONLY : O_RDWR;
 retry_open_ffd:
	ffd = open(file, *mode);
	if (ffd < 0) {
		if (*mode != O_RDONLY) {
			*mode = O_RDONLY;
			goto retry_open_ffd;
		}
	}
	return ffd;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,8,0)

/*
 * loop: Add LOOP_CONFIGURE ioctl
 * https://lwn.net/Articles/820408/
 * https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=3448914e8cc5
 */
static int set_loop_configure(int ffd, int lfd, bb_loop_info *loopinfo)
{
	int rc;
	struct loop_config config;

	memset(&config, 0, sizeof(config));
	config.fd = ffd;
	memcpy(&config.info, loopinfo, sizeof(config.info));

	rc = ioctl(lfd, LOOP_CONFIGURE, &config);
	if (rc == 0) {
		return lfd;
	}
	return -1;
}

#else
static int set_loop_configure(int ffd, int lfd, bb_loop_info *loopinfo)
{
	int rc;
	bb_loop_info loopinfo2;
	/* Associate free loop device with file */
	if (ioctl(lfd, LOOP_SET_FD, ffd)) {
		return -1;
	}
	rc = ioctl(lfd, BB_LOOP_SET_STATUS, loopinfo);
	if (rc != 0 && (loopinfo->lo_flags & BB_LO_FLAGS_AUTOCLEAR)) {
		memcpy(&loopinfo2, loopinfo, sizeof(*loopinfo));
		/* Old kernel, does not support LO_FLAGS_AUTOCLEAR? */
		/* (this code path is not tested) */
		loopinfo2.lo_flags -= BB_LO_FLAGS_AUTOCLEAR;
		rc = ioctl(lfd, BB_LOOP_SET_STATUS, &loopinfo2);
	}
	if (rc == 0) {
		return lfd;
	}
	/* failure, undo LOOP_SET_FD */
	ioctl(lfd, LOOP_CLR_FD, 0); // actually, 0 param is unnecessary
	return -1;
}
#endif

static int set_loop_info(int ffd, int lfd, bb_loop_info *loopinfo)
{
	int rc;
	bb_loop_info tmp;

	rc = ioctl(lfd, BB_LOOP_GET_STATUS, &tmp);
	/* If device is free, try to claim it */
	if (rc && errno == ENXIO) {
		return set_loop_configure(ffd, lfd, loopinfo);
	}
	return -1;
}

static void init_bb_loop_info(bb_loop_info *loopinfo, const char *file,
		unsigned long long offset, unsigned long long sizelimit, unsigned flags)
{
	memset(loopinfo, 0, sizeof(*loopinfo));
	safe_strncpy((char *)loopinfo->lo_file_name, file, LO_NAME_SIZE);
	loopinfo->lo_offset = offset;
	loopinfo->lo_sizelimit = sizelimit;
	/*
	 * Used by mount to set LO_FLAGS_AUTOCLEAR.
	 * LO_FLAGS_READ_ONLY is not set because RO is controlled by open type of the file.
	 * Note that closing LO_FLAGS_AUTOCLEARed lfd before mount
	 * is wrong (would free the loop device!)
	 */
	loopinfo->lo_flags = (flags & ~BB_LO_FLAGS_READ_ONLY);
}

static int do_stat_and_mknod(const char *dev, const char *device, int i, int *mknod_flag)
{
	struct stat statbuf;

	IF_FEATURE_MOUNT_LOOP_CREATE(errno = 0;)
	if (stat(dev, &statbuf) != 0 || !S_ISBLK(statbuf.st_mode)) {
		if (ENABLE_FEATURE_MOUNT_LOOP_CREATE
				&& errno == ENOENT
				&& (!device)
			) {
			/* Node doesn't exist, try to create it */
			if (mknod(dev, S_IFBLK|0644, makedev(7, i)) == 0) {
				*mknod_flag = 0;
				return 0;
			}
		}
		/* Ran out of block devices, return failure */
		return -1;
	}
	return 0;
}

static int set_loop_dev(const char *dev, int *mode, int ffd, bb_loop_info *loopinfo)
{
	int rc;
	/* Open the sucker and check its loopiness */
	int lfd = open(dev, *mode);
	if (lfd < 0 && errno == EROFS) {
		*mode = O_RDONLY;
		lfd = open(dev, *mode);
	}
	if (lfd < 0) {
		return lfd;
	}
	rc = set_loop_info(ffd, lfd, loopinfo);
	if (rc == lfd) {
		return lfd;
	}
	close(lfd);
	return -1;
}


/* Returns opened fd to the loop device, <0 on error.
 * *device is loop device to use, or if *device==NULL finds a loop device to
 * mount it on and sets *device to a strdup of that loop device name.
 */
int FAST_FUNC set_loop(char **device, const char *file, unsigned long long offset,
		unsigned long long sizelimit, unsigned flags)
{
	char dev[LOOP_NAMESIZE];
	char *try;
	int i, ffd, mode, rc, mknod_flag;
	bb_loop_info loopinfo;

	mknod_flag = -1;

	ffd = open_file(file, flags, &mode);
	if (ffd < 0) {
		return -errno;
	}

	init_bb_loop_info(&loopinfo, file, offset, sizelimit, flags);
	try = *device;
	if (!try) {
		try = dev;
	}

	/* Find a loop device */
	/* 0xfffff is a max possible minor number in Linux circa 2010 */
	for (i = 0; i <= 0xfffff; i++) {
		if (!*device) {
			rc = get_next_free_loop(dev, i);
			if (rc == -1) {
				break; /* no free loop devices */
			} else if (rc == 1) {
				goto open_lfd;
			}
		}
		rc = do_stat_and_mknod(try, *device, i, &mknod_flag);
		if (rc == -1) {
			break;
		}
 open_lfd:
		rc = set_loop_dev(try, &mode, ffd, &loopinfo);
		if (rc == -1) {
			if (errno == ENXIO) {
				if (mknod_flag == 0) {
					mknod_flag = -1;
					(void)unlink(try);
				}
				break;
			}
			if (*device) {
				break;
			}
		} else {
			/* SUCCESS! */
			if (!*device)
				*device = xstrdup(try);
			break;
		}
	} /* for() */

	close(ffd);
	return rc;
}

#else /* CONFIG_FEATURE_LOOP_CONFIGURE is not set */

/* Returns opened fd to the loop device, <0 on error.
 * *device is loop device to use, or if *device==NULL finds a loop device to
 * mount it on and sets *device to a strdup of that loop device name.
 */
int FAST_FUNC set_loop(char **device, const char *file, unsigned long long offset,
		unsigned long long sizelimit, unsigned flags)
{
	char dev[LOOP_NAMESIZE];
	char *try;
	bb_loop_info loopinfo;
	struct stat statbuf;
#if ENABLE_FEATURE_MOUNT_CLEAR_FAILED_DEV
	int mknod_flag;
#endif
	int i, lfd, ffd, mode, rc;

	/* Open the file.  Barf if this doesn't work.  */
	mode = (flags & BB_LO_FLAGS_READ_ONLY) ? O_RDONLY : O_RDWR;
 open_ffd:
	ffd = open(file, mode);
	if (ffd < 0) {
		if (mode != O_RDONLY) {
			mode = O_RDONLY;
			goto open_ffd;
		}
		return -errno;
	}

	try = *device;
	if (!try) {
 get_free_loopN:
		i = get_free_loop();
		if (i == -1) {
			close(ffd);
			return -1; /* no free loop devices */
		}
		if (i >= 0) {
			try = xasprintf(LOOP_FORMAT, i);
			goto open_lfd;
		}
		/* i == -2: no /dev/loop-control. Do an old-style search for a free device */
		try = dev;
	}

	/* Find a loop device */
	/* 0xfffff is a max possible minor number in Linux circa 2010 */
	for (i = 0; i <= 0xfffff; i++) {
		sprintf(dev, LOOP_FORMAT, i);

		IF_FEATURE_MOUNT_LOOP_CREATE(errno = 0;)
		if (stat(try, &statbuf) != 0 || !S_ISBLK(statbuf.st_mode)) {
			if (ENABLE_FEATURE_MOUNT_LOOP_CREATE
			 && errno == ENOENT
			 && try == dev
			) {
				/* Node doesn't exist, try to create it */
				if (mknod(dev, S_IFBLK|0644, makedev(7, i)) == 0) {
#if ENABLE_FEATURE_MOUNT_CLEAR_FAILED_DEV
					mknod_flag = 0;
#endif
					goto open_lfd;
				}
			}
			/* Ran out of block devices, return failure */
			rc = -1;
			break;
		}
 open_lfd:
		/* Open the sucker and check its loopiness */
		lfd = rc = open(try, mode);
		if (lfd < 0 && errno == EROFS) {
			mode = O_RDONLY;
			lfd = rc = open(try, mode);
		}
		if (lfd < 0) {
			if (errno == ENXIO) {
				/* Happens if loop module is not loaded */
#if ENABLE_FEATURE_MOUNT_CLEAR_FAILED_DEV
				if (mknod_flag == 0) {
					mknod_flag = -1;
					(void)unlink(dev);
				}
#endif
				/* rc is -1; */
				break;
			}
			goto try_next_loopN;
		}

		rc = ioctl(lfd, BB_LOOP_GET_STATUS, &loopinfo);

		/* If device is free, try to claim it */
		if (rc && errno == ENXIO) {
			/* Associate free loop device with file */
			if (ioctl(lfd, LOOP_SET_FD, ffd)) {
#if ENABLE_FEATURE_LOOP_ERROR_EXIT
				/* If an error that cannot be rectified by repeated mounting occurs,
				 * exit the process directly.
				 */
				if ((errno < NUM_POSIX) && (!is_permitted_try_again[errno])) {
					rc = -1;
					break;
				}
#endif
				/* Ouch. Are we racing with other mount? */
				if (!*device   /* yes */
				 && try != dev /* tried a _kernel-offered_ loopN? */
				) {
					free(try);
					close(lfd);
//TODO: add "if (--failcount != 0) ..."?
					goto get_free_loopN;
				}
				goto close_and_try_next_loopN;
			}
			memset(&loopinfo, 0, sizeof(loopinfo));
			safe_strncpy((char *)loopinfo.lo_file_name, file, LO_NAME_SIZE);
			loopinfo.lo_offset = offset;
			loopinfo.lo_sizelimit = sizelimit;
			/*
			 * Used by mount to set LO_FLAGS_AUTOCLEAR.
			 * LO_FLAGS_READ_ONLY is not set because RO is controlled by open type of the file.
			 * Note that closing LO_FLAGS_AUTOCLEARed lfd before mount
			 * is wrong (would free the loop device!)
			 */
			loopinfo.lo_flags = (flags & ~BB_LO_FLAGS_READ_ONLY);
			rc = ioctl(lfd, BB_LOOP_SET_STATUS, &loopinfo);
			if (rc != 0 && (loopinfo.lo_flags & BB_LO_FLAGS_AUTOCLEAR)) {
				/* Old kernel, does not support LO_FLAGS_AUTOCLEAR? */
				/* (this code path is not tested) */
				loopinfo.lo_flags -= BB_LO_FLAGS_AUTOCLEAR;
				rc = ioctl(lfd, BB_LOOP_SET_STATUS, &loopinfo);
			}
			if (rc == 0) {
				/* SUCCESS! */
				if (try != dev) /* tried a kernel-offered free loopN? */
					*device = try; /* malloced */
				if (!*device)   /* was looping in search of free "/dev/loopN"? */
					*device = xstrdup(dev);
				rc = lfd; /* return this */
				break;
			}
			/* failure, undo LOOP_SET_FD */
			ioctl(lfd, LOOP_CLR_FD, 0); // actually, 0 param is unnecessary
		} else {
			/* device is not free (rc == 0), or error other than ENXIO */
			if (rc == 0	/* device is not free? */
			 && !*device	/* racing with other mount? */
			 && try != dev	/* tried a _kernel-offered_ loopN? */
			) {
				free(try);
				close(lfd);
				goto get_free_loopN;
			}
		}
 close_and_try_next_loopN:
		close(lfd);
 try_next_loopN:
		rc = -1;
		if (*device) /* was looking for a particular "/dev/loopN"? */
			break; /* yes, do not try other names */
	} /* for() */

	close(ffd);
	return rc;
}
#endif /* CONFIG_FEATURE_LOOP_CONFIGURE */
