/* Copyright (c) 2022-2023 Huawei Device Co., Ltd.
 *  linux/drivers/video/fbmem.c
 *
 *  Copyright (C) 1994 Martin Schaller
 *
 *	2001 - Documented with DocBook
 *	- Brad Douglas <brad@neruo.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */
#ifndef __DPU_AOD_DEVICE_H_
#define __DPU_AOD_DEVICE_H_

#include <platform_include/smart/linux/base/ap/protocol.h>
#include "contexthub_route.h"


#include <linux/module.h>
#include <linux/compat.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/vt.h>
#include <linux/init.h>
#include <linux/linux_logo.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/console.h>
#include <linux/kmod.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/efi.h>
#include <linux/wait.h>
#include <linux/fb.h>
#include <linux/iommu.h>
#include <linux/of_reserved_mem.h>
#include <linux/delay.h>
#include <asm/fb.h>
#include <linux/dma-buf.h>
#include <basicplatform/drivers/smmu/mm_smmu.h>
#include <platform_include/basicplatform/linux/iommu/mm_svm.h>

#define MAX_BIT_MAP_SIZE 2
#define MAX_DISPLAY_SPACE_COUNT 13
#define DIFF_NUMBER 1
#define MAX_DYNAMIC_ALLOC_FB_COUNT 64

#define ALIGN_UP(val, al)    (((val) + ((al)-1)) & ~((al)-1))

#define DPU_AOD_ERR(msg, ...)    \
	do { if (dpu_aod_msg_level > 0)  \
		printk(KERN_ERR "[dpu_aod]%s: "msg, __func__, ## __VA_ARGS__); } while (0)

#define DPU_AOD_INFO(msg, ...)    \
	do { if (dpu_aod_msg_level > 1)  \
		printk(KERN_INFO "[dpu_aod]%s: "msg, __func__, ## __VA_ARGS__); } while (0)

#define DPU_AOD_DEBUG(msg, ...)                                 \
	do {                                                     \
		if (dpu_aod_msg_level > 2)                      \
			printk(KERN_INFO "[dpu_aod]%s: "msg, __func__, ## __VA_ARGS__);  \
	} while (0)

#define AOD_IOCTL_AOD_START		_IOW(0xB2, 0x01, unsigned long)
#define AOD_IOCTL_AOD_STOP		_IO(0xB2, 0x02)
#define AOD_IOCTL_AOD_PAUSE 	_IOR(0xB2, 0x03, unsigned long)
#define AOD_IOCTL_AOD_RESUME 	_IOW (0xB2, 0x04, unsigned long)
#define AOD_IOCTL_AOD_START_UPDATING 	_IOR(0xB2, 0x05, unsigned long)
#define AOD_IOCTL_AOD_END_UPDATING	_IOW(0xB2, 0x06, unsigned long)
#define AOD_IOCTL_AOD_SET_TIME	_IOW(0xB2, 0x07, unsigned long)
#define AOD_IOCTL_AOD_SET_BITMAP_SIZE	_IOW(0xB2, 0x08, unsigned long)
#define AOD_IOCTL_AOD_SET_DISPLAY_SPACE		_IOW(0xB2, 0x09, unsigned long)
#define AOD_IOCTL_AOD_GET_PANEL_INFO		_IOW(0xB2, 0x0A, unsigned long)
#define AOD_IOCTL_AOD_FREE_BUF	_IOW(0xB2, 0x0B, unsigned long)
#define AOD_IOCTL_AOD_SET_FINGER_STATUS _IOW(0xB2, 0x0C, unsigned long)
#define AOD_IOCTL_AOD_GET_DYNAMIC_FB	_IOW(0xB2, 0x0D, unsigned long)
#define AOD_IOCTL_AOD_FREE_DYNAMIC_FB   _IOW(0xB2, 0x0E, unsigned long)
#define AOD_IOCTL_AOD_SET_MAX_AND_MIN_BACKLIGHT   _IOW(0xB2, 0x0F, unsigned long)
#define AOD_IOCTL_AOD_SET_COMMON_SENSORHUB _IOW(0xB2, 0x10, unsigned long)
#define AOD_IOCTL_AOD_SET_GMP _IOW(0xB2, 0x14, unsigned long)
#define AOD_IOCTL_AOD_SET_GENERAL_SENSORHUB _IOW(0xB2, 0x15, unsigned long)
#define AOD_IOCTL_AOD_GET_DYNAMIC_FB_NEW _IOW(0xB2, 0x16, unsigned long)
#define AOD_IOCTL_AOD_FREE_DYNAMIC_FB_NEW _IOW(0xB2, 0x17, unsigned long)
#define AOD_IOCTL_AOD_SET_FOLD_INFO _IOW(0xB2, 0x18, unsigned long)
#define AOD_IOCTL_AOD_PAUSE_NEW _IOR(0xB2, 0x19, unsigned long)
#define AOD_IOCTL_AOD_GET_POS _IOW(0xB2, 0x1A, unsigned long)
#define AOD_IOCTL_AOD_SET_MULTI_GMP _IOW(0xB2, 0x1B, unsigned long)
#define AOD_IOCTL_AOD_SET_SAFE_FRAME_RATE _IOW(0xB2, 0x1C, unsigned long)
#define AOD_IOCTL_AOD_FULL_SCREEN_BACKLIGHT _IOW(0xB2, 0x1D, unsigned long)
#define AOD_IOCTL_AOD_SET_DISPLAY_PARAM _IOW(0xB2, 0x1E, unsigned long)
#define AOD_IOCTL_AOD_SET_LCD_ALWAYS_ON _IOW(0xB2, 0x1F, unsigned long)
#define AOD_IOCTL_AOD_SET_VIVOBUS_LEVEL _IOW(0xB2, 0x20, unsigned long)
#define AOD_IOCTL_AOD_FULL_SCREEN_STATUS _IOW(0xB2, 0x21, unsigned long)
#define AOD_IOCTL_SET_RENDER_BUFFER _IOW(0xB2, 0x22, unsigned long)
#define AOD_IOCTL_AOD_POST_HANDLE _IOW(0xB2, 0x23, unsigned long)


#define DPU_AOD_OK 0
#define DPU_AOD_FAIL (-1)
#define STATUS_FINGER_DOWN 1
#define STATUS_FINGER_CHECK_OK 2
#define DISPALY_SCREEN_ON 0
#define DISPALY_SCREEN_OFF 1
#define AOD_LPM_OFF 0
#define LCD_TYPE_UNKNOWN 0
#define LCD_TYPE_SAMSUNG_S6E3HF4 1

#define MAX_ADDR_FOR_SENSORHUB     0xFFFFFFFF

#define SHMEM_START_MSG_CMD_TYPE	1
#if defined(CONFIG_DPU_FB_V510) || defined(CONFIG_DPU_FB_V350) || \
	defined(CONFIG_DPU_FB_V501) || defined(CONFIG_DPU_FB_V600) || \
	defined(CONFIG_DKMD_DPU_AOD) || defined(CONFIG_DPU_FB_V346)
#define SHMEM_START_CONFIG	1
#else
#define SHMEM_START_CONFIG	0
#endif
enum aod_pause_data_index {
	SUB_CMD_TYPE,
	SCREEN_STATE,
	LPM_ON,
	DEFAULT_PAUSE_DATA_NUM,
};

enum aod_fb_pixel_format {
	AOD_FB_PIXEL_FORMAT_RGB_565 = 0,
	AOD_FB_PIXEL_FORMAT_RGBX_4444,
	AOD_FB_PIXEL_FORMAT_RGBA_4444,
	AOD_FB_PIXEL_FORMAT_RGBX_5551,
	AOD_FB_PIXEL_FORMAT_RGBA_5551,
	AOD_FB_PIXEL_FORMAT_RGBX_8888,
	AOD_FB_PIXEL_FORMAT_RGBA_8888,

	AOD_FB_PIXEL_FORMAT_BGR_565,
	AOD_FB_PIXEL_FORMAT_BGRX_4444,
	AOD_FB_PIXEL_FORMAT_BGRA_4444,
	AOD_FB_PIXEL_FORMAT_BGRX_5551,
	AOD_FB_PIXEL_FORMAT_BGRA_5551,
	AOD_FB_PIXEL_FORMAT_BGRX_8888,
	AOD_FB_PIXEL_FORMAT_BGRA_8888,

	AOD_FB_PIXEL_FORMAT_YUV_422_I,

	/* YUV Semi-planar */
	AOD_FB_PIXEL_FORMAT_YCbCr_422_SP, /* NV16 */
	AOD_FB_PIXEL_FORMAT_YCrCb_422_SP,
	AOD_FB_PIXEL_FORMAT_YCbCr_420_SP,
	AOD_FB_PIXEL_FORMAT_YCrCb_420_SP, /* NV21 */

	/* YUV Planar */
	AOD_FB_PIXEL_FORMAT_YCbCr_422_P,
	AOD_FB_PIXEL_FORMAT_YCrCb_422_P,
	AOD_FB_PIXEL_FORMAT_YCbCr_420_P,
	AOD_FB_PIXEL_FORMAT_YCrCb_420_P, /* AOD_FB_PIXEL_FORMAT_YV12 */

	/* YUV Package */
	AOD_FB_PIXEL_FORMAT_YUYV_422_Pkg,
	AOD_FB_PIXEL_FORMAT_UYVY_422_Pkg,
	AOD_FB_PIXEL_FORMAT_YVYU_422_Pkg,
	AOD_FB_PIXEL_FORMAT_VYUY_422_Pkg,
};

enum buffer_type {
	RENDER_BUFFER_0,
	RENDER_BUFFER_1,
	RENDER_HEAP,
	FONT_CACHE,
	JPEGD_BUFFER_0,
	JPEGD_BUFFER_1,
	ALT_BUFFER,
	BUFFER_TYPE_MAX,
};

uint32_t get_lcd_always_on(void);
uint32_t get_lpm_on(void);
#endif
