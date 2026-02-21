/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define hpi driver macro and data struct
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 06 17:52:45 2019
 */

#ifndef UAPI_DRIVERS_HPI_DRV_H
#define UAPI_DRIVERS_HPI_DRV_H

#include <hmkernel/types.h>

/* Note that this file is synced from products, should not be modified.
 * Latest sync time: 2020-05-13 */
#define HPI_CMD_WRITEREG		0xFF010000U
#define HPI_CMD_READREG			0xFF010001U

#define HPI_CMD_READ_I			0xFF010100U
#define HPI_CMD_WRITE_I			0xFF010101U
#define HPI_CMD_OR_I			0xFF010102U
#define HPI_CMD_AND_I			0xFF010103U
#define HPI_CMD_SETN2M_BIT_I		0xFF010104U
#define HPI_CMD_SET_BIT_I		0xFF010105U
#define HPI_CMD_CLRBIT_I		0xFF010106U
#define HPI_CMD_MAX_I			0xFF010107U

#define HPI_MAX_BUFF_LEN		128U
#define HPI_BLOCK_BYTE_BEFORE_TIMEOUT	128U

/* HPI rw mode */
enum __hpi_rw_mode {
	HPI_AUTO_BITS16_MODE = 0,	/* 16bit self-increasing mode */
	HPI_AUTO_BITS32_MODE,
	HPI_SINGLE_BITS8_MODE,		/* 8 bit single mode */
	HPI_SINGLE_BITS16_MODE,
	HPI_SINGLE_BITS32_MODE
};

/* HPI support access with 8/16/32bit */
enum __hpi_bits_mode {
	EN_BITS_8 = 0,
	EN_BITS_16,
	EN_BITS_32
};

/* set mode of bit operation */
enum __hpi_set_bit_mode {
	HPI_SET_BIT_BY_NO = 0,		/* set bit mode */
	HPI_SET_BIT_BY_MASK		/* mask operation */
};

struct __hpi_regwrite_arg {
	__u32	drvid;			/* kernel does't need this arg, use it to adapt rru-call */
	__u32	regaddr;
	__u32	*srcbuf;
	__u32	srclen;
};

struct __hpi_regread_arg {
	__u32	drvid;
	__u32	regaddr;
	__u8	*dstbuf;
	__u32	dstlen;
};

struct __hpi_ioctl_arg {
	__u32			busindex;
	__u32			startaddr;
	__u8			*dstbuf;
	__u32			bufdata;
	__u32			buflen;			/* Buffer length */
	enum __hpi_rw_mode	rwmode;			/* hpi_rw_mode: 8 16 32 */
	__u32			slowmode;
	__u8			*setvalue;		/* set value of buffer */
	enum __hpi_bits_mode	bitsmode;
	enum __hpi_set_bit_mode	setbitmode;
	__u32			srcmask;
};

#ifdef __HOST_LLT__
int llt_hpi_driver_init(void);
int llt_hpi_driver_handler(unsigned long cmd, const void __user * args,
			   unsigned long args_size);
#endif

#endif
