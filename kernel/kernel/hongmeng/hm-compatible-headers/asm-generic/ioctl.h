/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add asm-generic/ioctl.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 24 17:29:45 2021
 */

#ifndef ASM_GENERIC_IOCTL_H
#define ASM_GENERIC_IOCTL_H

#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8

#ifndef _IOC_SIZEBITS
#define _IOC_SIZEBITS	14
#endif

#ifndef _IOC_DIRBITS
#define _IOC_DIRBITS	2
#endif

#define _IOC_NRMASK	((1 << _IOC_NRBITS)-1)
#define _IOC_SIZEMASK	((1 << _IOC_SIZEBITS)-1)
#define _IOC_TYPEMASK	((1 << _IOC_TYPEBITS) - 1)
#define _IOC_NRSHIFT	0
#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)

#define _IOC_NR(nr)	(((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)	(((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)
#define _IOC_TYPE(nr)	(((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)

#ifndef _IOC_NONE
#define _IOC_NONE	0U
#endif

#ifndef _IOC_WRITE
#define _IOC_WRITE	1U
#endif

#ifndef _IOC_READ
#define _IOC_READ	2U
#endif

#ifndef _IOC
#define _IOC(a,b,c,d)		( ((a)<<30) | ((b)<<8) | (c) | ((d)<<16) )
#endif

#ifndef _IOC_TYPECHECK
#define _IOC_TYPECHECK(t)	(sizeof(t))
#endif

#ifndef _IO
#define _IO(a,b)		_IOC(_IOC_NONE,(a),(b),0)
#endif

#ifndef _IOW
#define _IOW(a,b,c)		_IOC(_IOC_WRITE,(a),(b),sizeof(c))
#endif

#ifndef _IOR
#define _IOR(a,b,c)		_IOC(_IOC_READ,(a),(b),sizeof(c))
#endif

#ifndef _IOWR
#define _IOWR(a,b,c)		_IOC(_IOC_READ|_IOC_WRITE,(a),(b),sizeof(c))
#endif

#ifndef IOC_IN
#define IOC_IN		(_IOC_WRITE << _IOC_DIRSHIFT)
#endif

#ifndef IOC_OUT
#define IOC_OUT		(_IOC_READ << _IOC_DIRSHIFT)
#endif

#endif
