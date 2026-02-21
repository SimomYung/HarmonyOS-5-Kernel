/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/fd.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 12:13:30 2021
 */

#ifndef LINUX_FD_H
#define LINUX_FD_H

#include <linux/ioctl.h>

struct floppy_struct {
	unsigned int		size,
				sect,
				head,
				track,
				stretch;
#define FD_STRETCH		1
#define FD_SWAPSIDES		2
#define FD_ZEROBASED		4
#define FD_SECTBASEMASK		0x3FC
#define FD_MKSECTBASE(s)	(((s) ^ 1) << 2)
#define FD_SECTBASE(floppy)	((((floppy)->stretch & FD_SECTBASEMASK) >> 2) ^ 1)
	unsigned char		gap,
				rate,
#define FD_2M			0x4
#define FD_SIZECODEMASK		0x38
#define FD_SIZECODE(floppy)	(((((floppy)->rate&FD_SIZECODEMASK)>> 3)+ 2) %8)
#define FD_SECTSIZE(floppy)	( (floppy)->rate & FD_2M ? \
				512 : 128 << FD_SIZECODE(floppy) )
#define FD_PERP			0x40
				spec1,
				fmt_gap;
	const char		* name;
};

#define FDGETPRM		_IOR(2, 0x04, struct floppy_struct)

#define FD_FILL_BYTE		0xF6

struct format_descr {
	unsigned int		device,
				head,
				track;
};

#define FDFMTBEG		_IO(2,0x47)
#define FDFMTTRK		_IOW(2,0x48, struct format_descr)
#define FDFMTEND		_IO(2,0x49)

#define FDEJECT			_IO(2, 0x5a)

#endif
