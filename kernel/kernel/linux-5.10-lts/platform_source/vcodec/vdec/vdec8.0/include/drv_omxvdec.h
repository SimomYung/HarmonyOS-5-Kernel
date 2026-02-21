/*
 * drv_omxvdec.c
 *
 * This is for omxvdec export header file.
 *
 * Copyright (c) 2014-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __DRV_OMXVDEC_H__
#define __DRV_OMXVDEC_H__

#include <linux/ioctl.h>
#include "vcodec_types.h"

/*
 * IOCTL for interaction with omx components
 * For Compat USER 32 -> KER 64, all pointer menbers set in last in structure.
 * NOTICE: MUST NOT ALTER the member sequence of this structure
 */
typedef struct vcodec_vdec_ioctl_msg {
	int32_t chan_num;
	uint32_t in_size;
	uint32_t out_size;
	void *in;
	void *out;
} vdec_ioctl_msg;

// Modified for 64-bit platform
typedef struct vcodec_compat_ioctl_msg {
	int32_t chan_num;
	int32_t in_size;
	int32_t out_size;
	compat_ulong_t in;
	compat_ulong_t out;
} compat_ioctl_msg;

#define VDEC_IOCTL_MAGIC 'v'
#define VDEC_IOCTL_CANCEL_DECODE		_IO(VDEC_IOCTL_MAGIC, 19)
#define VDEC_IOCTL_SET_CLK_RATE			_IO(VDEC_IOCTL_MAGIC, 20)
#define VDEC_IOCTL_GET_VDM_HWSTATE		_IO(VDEC_IOCTL_MAGIC, 21)
#define VDEC_IOCTL_SCD_PROC				_IO(VDEC_IOCTL_MAGIC, 22)
#define VDEC_IOCTL_VDM_PROC				_IO(VDEC_IOCTL_MAGIC, 23)
#define VDEC_IOCTL_LOCK_HW				_IO(VDEC_IOCTL_MAGIC, 24)
#define VDEC_IOCTL_UNLOCK_HW			_IO(VDEC_IOCTL_MAGIC, 25)
#define VDEC_IOCTL_DEC_IRQ_QUERY		_IO(VDEC_IOCTL_MAGIC, 29)
#define VDEC_IOCTL_IOMMU_MAP			_IO(VDEC_IOCTL_MAGIC, 33)
#define VDEC_IOCTL_IOMMU_UNMAP			_IO(VDEC_IOCTL_MAGIC, 34)
#define VDEC_IOCTL_FREE_IONBUF			_IO(VDEC_IOCTL_MAGIC, 35)
#define VDEC_IOCTL_RESTORE_IONBUF		_IO(VDEC_IOCTL_MAGIC, 36)

#define VDEC_IOCTL_BSP_DECODE			_IO(VDEC_IOCTL_MAGIC, 40)
#define VDEC_IOCTL_BSP0_DECODE_DONE		_IO(VDEC_IOCTL_MAGIC, 41)
#define VDEC_IOCTL_BSP1_DECODE_DONE		_IO(VDEC_IOCTL_MAGIC, 42)
#define VDEC_IOCTL_PXP_DECODE			_IO(VDEC_IOCTL_MAGIC, 43)
#define VDEC_IOCTL_PXP_DECODE_DONE		_IO(VDEC_IOCTL_MAGIC, 44)
#define VDEC_IOCTL_IPC_VCTL_CMD			_IO(VDEC_IOCTL_MAGIC, 47)
#define VDEC_IOCTL_IPC_CONFIG_INPUTBUFFER	_IO(VDEC_IOCTL_MAGIC, 48)
#define VDEC_IOCTL_IPC_PROCESS_EVENT	_IO(VDEC_IOCTL_MAGIC, 49)
#define VDEC_IOCTL_GET_MCU_LOG 			_IO(VDEC_IOCTL_MAGIC, 50)
#define VDEC_IOCTL_PROC_IPC_MSG         _IO(VDEC_IOCTL_MAGIC, 51)
#define VDEC_IOCTL_IPC_RELEASE_IMG      _IO(VDEC_IOCTL_MAGIC, 52)
/* 53 - 58 used by RESET_xxx */
#define VDEC_IOCTL_IPC_CREATE_CHAN	_IO(VDEC_IOCTL_MAGIC, 59)
#define VDEC_IOCTL_IPC_DESTORY_CHAN	_IO(VDEC_IOCTL_MAGIC, 60)

#define VDEC_IOCTL_IMPROVE_YIELD  _IO(VDEC_IOCTL_MAGIC, 61)
#endif
