/*
 * drv_venc_ioctl.h
 *
 * This is for drv_venc_ioctl.
 *
 * Copyright (c) 2010-2020 Huawei Technologies CO., Ltd.
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

#ifndef __DRV_VENC_IOCTL_H__
#define __DRV_VENC_IOCTL_H__

#include "soc_venc_reg_interface.h"
#include "vcodec_type.h"
#include "wireless_lowdelay_info.h"
#include "venc_freq.h"

#define MAX_SLICE_NUM  16
#define QP_HISTOGRAM_NUM 52
#define CMDLIST_BUFFER_NUM 32

enum reg_cfg_mode {
	VENC_SET_CFGREG = 100,
	VENC_SET_CFGREGSIMPLE
};

/*
 * IOCTL for interaction with omx components
 * For Compat USER 32 -> KER 64, all pointer menbers set in last in structure.
 * NOTICE: MUST NOT ALTER the member sequence of this structure
 */
typedef struct {
	int32_t chan_num;
	int32_t in_size;
	int32_t out_size;
	void *in;
	void *out;
} venc_ioctl_msg;

/* Modified for 64-bit platform */
typedef struct {
	int32_t chan_num;
	int32_t in_size;
	int32_t out_size;
	compat_ulong_t in;
	compat_ulong_t out;
} compat_ioctl_msg;

typedef struct {
	int32_t shared_fd;
	uint32_t iova;
	uint64_t virt_addr;
	uint32_t iova_size;
} venc_buffer_record_t;

typedef struct {
	int32_t   interal_shared_fd;
	int32_t   image_shared_fd;
	int32_t   stream_shared_fd[MAX_SLICE_NUM];
	int32_t   stream_head_shared_fd;
	int32_t   watermark_shared_fd;
} venc_fd_info_t;

typedef struct {
	uint32_t rec_luma_size;
	uint32_t rec_chroma_size;
	uint32_t rec_luma_head_size;
	uint32_t rec_chroma_head_size;
	uint32_t qpgld_size;
	uint32_t nbi_size;
	uint32_t pme_size;
	uint32_t pme_info_size;
	uint32_t vedu_src_y_length;
	uint32_t vedu_src_c_length;
	uint32_t vedu_src_v_length;
	uint32_t vedu_src_yh_length;
	uint32_t vedu_src_ch_length;
	UADDR master_stream_start;
	UADDR master_stream_end;
} venc_buffer_alloc_info_t;

struct muti_ref_rc_info {
	uint32_t gop_type;
	uint32_t skip_ratio;
	uint32_t low_mv_ratio;
};

enum venc_frame_type{
    PREVIEW_FRAME = 0,
    SUPER_SHORT_FRAME, // 1 E
    SHORT_FRAME,
    NORMAL_FRAME,
    LONG_FRAME,
};
 
enum camera_mode{
    MAIN_BINNING,
    MAIN_QUAD,
    WIDE_BINNING,
    TELE_BINNING,
    TELE_QUAD,
};
 
enum color_type{
    G0 = 0,
    B,
    R,
    G1,
};

struct qp_rate_control_params {
	bool is_enable_mcu_rc;
    uint32_t iso;
    uint32_t exposure;
    bool is_ref;
    enum venc_frame_type frame_type; // long, short, normal, d;
    enum camera_mode pixel_mode;     // : wide, quad, binning;
    enum color_type plane;           // rggb
    uint32_t r_greater_b_plus_one;   // (rGain / grGain > bGain / gbGain)? + 1
};

struct channel_info {
	uint32_t id;          // channel id of this frame
	uint32_t frame_number;     // the frame number of this channel
	uint32_t frame_type;   // the frame type of this frame
	uint32_t buf_index;   // the buffer index of this frame
	uint32_t gop;
	uint32_t framerate;
	uint32_t bitrate; // targetQP for CRF, bitrate for ABR/CBR
	uint32_t rcmode;
	uint32_t is_skip_frame; // for EBR
	uint32_t is_external_rc_frame; // for EBR
	uint32_t slice_chroma_qp_offsets_en;
	uint32_t scenemode;
	uint32_t is_reset_rc;
	venc_fd_info_t mem_info;
	struct wireless_lowdelay_info wireless_ldy_info;
	struct muti_ref_rc_info muti_ref_info;
	struct qp_rate_control_params qp_rc_params;
};

// channel_info will be passed to mcore by the two union types below
typedef union {
	struct {
		unsigned int id                         : 4      ;
		unsigned int reserved_0                 : 2      ;
		unsigned int is_skip_frame              : 1      ;
		unsigned int is_extrc_frame             : 1      ;
		unsigned int rcmode                     : 8      ;
		unsigned int is_reset_rc                : 1      ;
		unsigned int slice_chroma_qp_offsets_en : 1      ;
		unsigned int reserved_1                 : 6      ;
		unsigned int scenemode                  : 8      ;
	} bits;
	unsigned int u32;
} bgll_reg_type;

typedef union {
	struct {
		unsigned int framerate      : 16     ;
		unsigned int gop            : 16     ;
	} bits;
	unsigned int u32;
} bglh_reg_type;


enum bref_rc_gop_type {
	GOP_DISABLE_B = 0,
	GOP_STATIC,
	GOP_DYNAMIC,
};

typedef union {
	struct {
		unsigned int gop_rc_type     : 4     ;
		unsigned int skip_ratio      : 8     ;
		unsigned int low_mv_ratio    : 8     ;
		unsigned int reserved        : 4     ;
	} bits;
	unsigned int u32;
} bref_reg_type;

typedef union {
	struct {
		unsigned int is_ref     		: 8     ;
		unsigned int frame_type    		: 8     ;
		unsigned int pixel_mode     	: 8     ;
		unsigned int is_enable_mcu_rc   : 8     ;
	} bits;
	unsigned int u32;
} raw_rc_params;

typedef union {
	struct {
		unsigned int plane        			: 8     ;
		unsigned int r_greater_b_plus_one   : 8     ;
		unsigned int reserved   			: 16    ;
	} bits;
	unsigned int u32;
} raw_rc_rggb_params;

enum block_size_type {
	BLOCK_4X4,
	BLOCK_8X8,
	BLOCK_16X16,
	BLOCK_32X32,
	BLOCK_BUTT
};

struct stream_info {
	uint8_t is_buf_full;
	uint8_t is_eop;
	uint8_t reserved[2]; /* 2 bytes reserve */
	uint32_t pic_stream_size;
	uint32_t block_cnt[BLOCK_BUTT];
	uint32_t ave_madi;
	uint32_t frame_madp;
	uint32_t slice_len[MAX_SLICE_NUM];
	uint32_t aligned_slice_len[MAX_SLICE_NUM];
	uint32_t slice_num;
	uint32_t luma_qp_cnt[QP_HISTOGRAM_NUM];
	uint32_t average_luma_qp;
	uint32_t average_lcu_mse;
};

struct clock_info {
	uint8_t is_set_clock;
	uint8_t low_power_enable_flag;
	uint8_t reserved[2]; /* 2 bytes reserve */
	venc_clk_t clock_type;
	uint32_t core_num;
	uint64_t loads;
};

struct picture_stats {
	uint32_t skip_mode_acc;
	uint32_t low_mv_magnitude_sum;
	uint32_t all_mv_magnitude_sum;
};

struct encode_done_info {
	uint8_t is_timeout;
	uint8_t reserved[3]; /* 3 bytes reserve */
	struct channel_info channel_info;   // the channel info of this frame
	struct stream_info stream_info;  // the register info for encode done process
	struct picture_stats picture_info; // skip and mv info, for muti ref encode
};

struct encode_info {
	uint8_t is_block;            // true: sync mode(waiting for encoding done); false: async mode
	uint8_t is_reset;            // whether reset the hardware
	uint8_t is_protected;
	uint8_t enable_power_control_per_frame;
	struct channel_info channel; // the channel info of this frame
	enum reg_cfg_mode reg_cfg_mode;
	S_HEVC_AVC_REGS_TYPE_CFG all_reg;
	venc_buffer_alloc_info_t venc_inter_buffer;
	struct clock_info clock_info;
	struct encode_done_info encode_done_info;  // the read back register info
};

#define CMD_VENC_ENCODE                 _IOWR(IOC_TYPE_VENC, 32, struct encode_info)
#define CMD_VENC_GET_ENCODE_DONE_INFO   _IOR(IOC_TYPE_VENC, 36, struct encode_done_info)

#define CMD_VENC_IOMMU_MAP              _IO(IOC_TYPE_VENC, 33)
#define CMD_VENC_IOMMU_UNMAP            _IO(IOC_TYPE_VENC, 34)

#define CMD_VENC_WIRELESS_LOWDELAY_INIT  _IOW(IOC_TYPE_VENC, 0x37, struct wireless_lowdelay_init_param)
#define CMD_VENC_WIRELESS_LOWDELAY_GET_FEEDBACK_INFO _IOR(IOC_TYPE_VENC, 0x38, struct wifi_feedback_info)

#endif

