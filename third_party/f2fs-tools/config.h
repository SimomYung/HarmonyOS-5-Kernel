/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

#ifndef CONFIG_H
#define CONFIG_H

/* Major version for f2fs-tools */
#define F2FS_MAJOR_VERSION 1

/* Minor version for f2fs-tools */
#define F2FS_MINOR_VERSION 14

/* f2fs-tools date based on Source releases */
#define F2FS_TOOLS_DATE "2022-05-13"

/* f2fs-tools version */
#define F2FS_TOOLS_VERSION "1.15.0"

/* Define to 1 if you have the `add_key' function. */
/* #undef HAVE_ADD_KEY */

/* Define to 1 if you have the <attr/xattr.h> header file. */
/* #undef HAVE_ATTR_XATTR_H */

/* report zones includes zone capacity */
/* #undef HAVE_BLK_ZONE_REP_V2 */

/* Define to 1 if you have the <byteswap.h> header file. */
#define HAVE_BYTESWAP_H 1

/* Define to 1 if you have the declaration of `bswap_64', and to 0 if you
   don't. */
#define HAVE_DECL_BSWAP_64 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `fallocate' function. */
#define HAVE_FALLOCATE 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fsetxattr' function. */
#define HAVE_FSETXATTR 1

/* Define to 1 if you have the `fstat' function. */
#define HAVE_FSTAT 1

/* Define to 1 if you have the `fstat64' function. */
#define HAVE_FSTAT64 1

/* Define to 1 if you have the `getmntent' function. */
#define HAVE_GETMNTENT 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `keyctl' function. */
/* #undef HAVE_KEYCTL */

/* Use blkid */
/* #undef HAVE_LIBBLKID */

/* Use libselinux */
/* #define HAVE_LIBSELINUX 1 */

/* Define to 1 if you have the <linux/blkzoned.h> header file. */
/* #define HAVE_LINUX_BLKZONED_H 1 */

/* Define to 1 if you have the <linux/falloc.h> header file. */
#define HAVE_LINUX_FALLOC_H 1

/* Define to 1 if you have the <linux/fs.h> header file. */
#define HAVE_LINUX_FS_H 1

/* Define to 1 if you have the <linux/hdreg.h> header file. */
#define HAVE_LINUX_HDREG_H 1

/* Define to 1 if you have the <linux/limits.h> header file. */
#define HAVE_LINUX_LIMITS_H 1

/* Define to 1 if you have the <linux/posix_acl.h> header file. */
#define HAVE_LINUX_POSIX_ACL_H 1

/* Define to 1 if you have the <linux/types.h> header file. */
#define HAVE_LINUX_TYPES_H 1

/* Define to 1 if you have the <linux/xattr.h> header file. */
#define HAVE_LINUX_XATTR_H 1

/* Define to 1 if you have the `llseek' function. */
/* #undef HAVE_LLSEEK */

/* Define to 1 if llseek declared in unistd.h */
/* #undef HAVE_LLSEEK_PROTOTYPE */

/* Define to 1 if you have the `lseek64' function. */
#define HAVE_LSEEK64 1

/* Define to 1 if lseek64 declared in unistd.h */
#define HAVE_LSEEK64_PROTOTYPE 1

/* Define to 1 if you have the <mach/mach_time.h> header file. */
/* #undef HAVE_MACH_MACH_TIME_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <mntent.h> header file. */
#define HAVE_MNTENT_H 1

/* Define to 1 if you have the <scsi/sg.h> header file. */
#define HAVE_SCSI_SG_H 1

/* Define to 1 if you have the `setmntent' function. */
#define HAVE_SETMNTENT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/acl.h> header file. */
/* #undef HAVE_SYS_ACL_H */

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/mount.h> header file. */
#define HAVE_SYS_MOUNT_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/syscall.h> header file. */
#define HAVE_SYS_SYSCALL_H 1

/* Define to 1 if you have the <sys/sysmacros.h> header file. */
#define HAVE_SYS_SYSMACROS_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#define HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <sys/xattr.h> header file. */
#define HAVE_SYS_XATTR_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "f2fs-tools"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "linux-f2fs-devel@lists.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "F2FS tools"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "F2FS tools 1.15.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "f2fs-tools"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.15.0"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.15.0"

#define HAVE_CLOCK_GETTIME 1

#define HAVE_CLOCK_BOOTTIME 1

#define WITH_BLKDISCARD 1

#define HAVE_FSYNC 1

#define HAVE_UUID_UUID_H 1

#define HAVE_LIBUUID 1

#define WITH_OHOS 1

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the type of a signed integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int32_t */

/* Define to the type of a signed integer type of width exactly 8 bits if such
   a type exists and the standard includes do not define it. */
/* #undef int8_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
#endif
