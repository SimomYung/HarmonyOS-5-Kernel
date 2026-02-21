/*
 * f_fxfer_hisuite.h
 *
 * Header file of common function driver of fxfer hisuite
 *
 * Copyright (C) 2025 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _F_FXFER_HISUITE_H
#define _F_FXFER_HISUITE_H

/*********** Protocol for communicating with user-mode app ***********/
/*
 * bit24-bit31: request number
 * bit20-bit23: result of io trasfer
 * bit19: read(0) or write(1)
 * bit18: reserved
 * bit0-bit17: length
 */
#define MASK_NUMBER  0xFF000000
#define MASK_RESULT  0x00F00000
#define MASK_DIRECT  0x00080000
#define MASK_LENGTH  0x0003FFFF

#define DIRECT_TX    1
#define DIRECT_RX    0

#define GET_NUMBER(v) (((uint32_t )(v) & MASK_NUMBER) >> 24)
#define GET_RESULT(v) (((uint32_t)(v) & MASK_RESULT) >> 20)
#define GET_DIRECT(v) (((uint32_t)(v) & MASK_DIRECT) >> 19)
#define GET_LENGTH(v) ((uint32_t)(v) & MASK_LENGTH)

#define SET_NUMBER(v, p) ((v) = ((v) & ~MASK_NUMBER) | (((uint32_t)(p) & 0xFF) << 24))
#define SET_RESULT(v, p) ((v) = ((v) & ~MASK_RESULT) | (((uint32_t)(p) & 0x0F) << 20))
#define SET_DIRECT(v, p) ((v) = ((v) & ~MASK_DIRECT) | (((uint32_t)(p) & 0x01) << 19))
#define SET_LENGTH(v, p) ((v) = ((v) & ~MASK_LENGTH) | ((uint32_t)(p) & MASK_LENGTH))

#define VALID_NUMBER(v) (((v) <= 0xFF) && ((v) > 0))
#define VALID_RESULT(v) ((v) <= 0x0F)
#define VALID_DIRECT(v) ((v) <= 0x01)
#define VALID_LENGTH(v) ((v) <= MASK_LENGTH)

struct ioctl_mem_info {
	uint64_t mem_addr;
	uint8_t index;
	uint8_t tx;
};

struct ioctl_hisuite_notify {
	uint8_t type;     /* 0:state; 1:string */
	uint8_t length;   /* actual length of data of notify */
	uint8_t data[14]; /* data of notify, max to 14 bytes */
};

#define FXFER_E_OK        0
#define FXFER_E_IO        1
#define FXFER_E_INVAL     2
#define FXFER_E_CANCEL    3
#define FXFER_IO_MAX_REQ  5

#define IOCTL_HISUITE_SET_MEM_INFO         _IOW('g', 1, struct ioctl_mem_info)
#define IOCTL_HISUITE_SUBMIT_IOREQ         _IOW('g', 2, uint32_t)
#define IOCTL_HISUITE_FREE_MEM             _IO('g', 3)
#define IOCTL_HISUITE_SUBMIT_MULTI_IOREQ   _IOWR('g', 4, uint32_t[FXFER_IO_MAX_REQ])
#define IOCTL_HISUITE_IO_CANCEL            _IO('g', 5)
#define IOCTL_HISUITE_SEND_NOTIFY          _IOW('g', 6, struct ioctl_hisuite_notify)

/*************** Protocol for communicating with host ***************/
#define HISUITE_VERSION           1
#define CMD_GET_STATE             0x71

#define NOTIFY_TYPE_STATE         0
#define NOTIFY_TYPE_STRING        1

#define NOTIFY_STATE_UNKNOWN      0
#define NOTIFY_STATE_CONNECT      1
#define NOTIFY_STATE_RESET        2
#define NOTIFY_STATE_DISCONNECT   3

struct fxfer_state {
	uint8_t  version;
	uint8_t  client_connect;
	uint8_t  pad[2]; /* Add 2 bytes for 4-byte aligned */
	uint32_t tx_max;
	uint32_t rx_max;
};

struct fxfer_notify {
	uint8_t type;     /* 0:state; 1:string */
	uint8_t length;   /* actual length of data of notify */
	uint8_t data[14]; /* data of notify, max to 14 bytes */
};

#endif

