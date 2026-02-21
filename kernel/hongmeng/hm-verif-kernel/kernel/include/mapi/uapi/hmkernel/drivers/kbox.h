/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Kbox device definition
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 25 21:46:06 2021
 */

#ifndef UAPI_HMKERNEL_DRIVERS_KBOX_H
#define UAPI_HMKERNEL_DRIVERS_KBOX_H

#include <hmkernel/types.h>
#include <hmkernel/const.h>

#define __KBOX_DEVICE_HEAD_SIZE		(__U(1) << 12U)
#define __KBOX_REGION_NAME_SIZE		32U

#define __KBOX_REGION_CBITS_EMPTY_SHIFT		30U
#define __KBOX_REGION_CBITS_EMPTY_MASK		(__U(1) << __KBOX_REGION_CBITS_EMPTY_SHIFT)
#define __KBOX_REGION_CBITS_SEQ_TOP		(__U(1) << 30U)
#define __KBOX_REGION_CBITS_SEQ_MASK		(__KBOX_REGION_CBITS_SEQ_TOP - 1U)

#define __KBOX_REGION_CBITS_DECODE_EMPTY(v)	(__u32)(((v) >> __KBOX_REGION_CBITS_EMPTY_SHIFT) & 1U)
#define __KBOX_REGION_CBITS_DECODE_SEQ(v)	(__u32)((v) & __KBOX_REGION_CBITS_SEQ_MASK)

struct kbox_device_region {
	/*
	 * 'cbits' is short for 'control bits'. It's read and set as an
	 * atomic integer.
	 *
	 * The internal layout of control bits is as follows:
	 *
	 *  3 3             2               1               0             0
	 *  1 0             3               5               7             0
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 * |R|E|                           SEQ                             |
	 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	 *
	 * R: Reserved bit
	 * 	- Reserved. This bit was used to avoid concurrent writing
	 * 	  during data reading.
	 *
	 * E: Empty flag
	 * 	- If this field is set, the data contained in the region is
	 * 	  less than the region size.
	 *
	 * SEQ: SEQuence
	 * 	- The next sequence number for bytes to be written. Every byte
	 * 	  written to the kbox region has a sequence number.
	 * 	  For example, if the current SEQ is 128 and there are two
	 * 	  64-byte buffers A and B to be written in order, then 128~191
	 * 	  are sequence numbers for buffer A and 192~255 for buffer B.
	 */
	__u32 cbits;
	__u32 clr_seq; /* sequence number of last read_clear */
	__u32 index; /* region index in this kbox */
	__u8 data[0];
} __attribute__((packed));

#endif /* !UAPI_HMKERNEL_DRIVERS_KBOX_H */
