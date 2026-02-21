/******************************************************************
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2018. All rights reserved.
 * File name  :
 * Description:
 *
 * Date         2022-06-29
 ********************************************************************/

#ifndef __CFG_TABLE_KLT_CS_H__
#define __CFG_TABLE_KLT_CS_H__

#include "ipp_klt_drv_priv.h"

#define CFG_TAB_KLT_MAX_KP_NUM   (500)
#define KLT_ROI_TOP_LEFT_NUM     (8)
#define KLT_ROI_BOTTOM_RIGHT_NUM (8)
#define KLT_GYRO_GRID_DISPL      (1089)
#define KLT_MAX_FWD_PYR_NUM      (5)
#define KLT_MIN_FWD_PYR_NUM      (1)
#define KLT_MAX_BWD_PYR_NUM      (1)

typedef ipp_klt_ds_config             klt_ds_cfg_t;
typedef ipp_klt_rd_prev_base_addr     klt_rd_prev_base_addr_t;
typedef ipp_klt_rd_prev_stride        klt_rd_prev_stride_t;
typedef ipp_klt_rd_next_base_addr     klt_rd_next_base_addr_t;
typedef ipp_klt_rd_next_stride        klt_rd_next_stride_t;
typedef ipp_klt_klt_start             klt_start_cfg_t;
typedef ipp_klt_klt_fwd_config        klt_fwd_search_cfg_t;
typedef ipp_klt_klt_bwd_config        klt_bwd_search_cfg_t;
typedef ipp_klt_klt_iter_config_1     klt_iter_cfg1_t;
typedef ipp_klt_klt_iter_config_2     klt_iter_cfg2_t;
typedef ipp_klt_klt_eval_param        klt_eval_param_cfg_t;
typedef ipp_klt_klt_extra_feature     klt_extra_feature_t;
typedef ipp_klt_roi_config            klt_roi_config_t;
typedef ipp_klt_roi_top_left          klt_roi_top_left_t;
typedef ipp_klt_roi_bottom_right      klt_roi_bottom_right_t;
typedef ipp_klt_filter_offset_config_1     klt_filter_offset_config_1_t;
typedef ipp_klt_filter_offset_config_2     klt_filter_offset_config_2_t;
typedef ipp_klt_filter_offset_config_3     klt_filter_offset_config_3_t;
typedef ipp_klt_filter_offset_config_4     klt_filter_offset_config_4_t;
typedef ipp_klt_gyro_grid_conf        klt_gyro_grid_conf_t;
typedef ipp_klt_gyro_grid_offset      klt_gyro_grid_offset_t;
typedef ipp_klt_gyro_grid_displ       klt_gyro_grid_displ_t;
typedef ipp_klt_kp_number             klt_kp_number_t;
typedef ipp_klt_kp_prev_buff_x        klt_kp_prev_buff_x_t;
typedef ipp_klt_kp_prev_buff_y        klt_kp_prev_buff_y_t;
typedef ipp_klt_kp_next_buff_x        klt_kp_next_buff_x_t;
typedef ipp_klt_kp_next_buff_y        klt_kp_next_buff_y_t;


typedef struct _klt_iter_cfg_cfg_t {
	klt_iter_cfg1_t iter_cfg1;
	klt_iter_cfg2_t iter_cfg2;
} klt_iter_cfg_cfg_t;

typedef struct _klt_prev_and_next_rd_cfg_t {
	klt_rd_prev_base_addr_t rd_prev_base_addr[KLT_MAX_FWD_PYR_NUM];
	klt_rd_prev_stride_t    rd_prev_line_stride[KLT_MAX_FWD_PYR_NUM];
	klt_rd_next_base_addr_t rd_next_base_addr[KLT_MAX_FWD_PYR_NUM];
	klt_rd_next_stride_t    rd_next_line_stride[KLT_MAX_FWD_PYR_NUM];
} klt_prev_and_next_rd_cfg_t;

typedef struct _klt_roi_config_cfg_t {
    klt_roi_config_t       klt_roi_config;
    klt_roi_top_left_t     klt_roi_top_left[KLT_ROI_TOP_LEFT_NUM];
    klt_roi_bottom_right_t klt_roi_bottom_right[KLT_ROI_BOTTOM_RIGHT_NUM];
} klt_roi_config_cfg_t;

typedef struct _klt_filter_offset_config_cfg_t {
    klt_filter_offset_config_1_t    klt_filter_offset_config_1;
    klt_filter_offset_config_2_t    klt_filter_offset_config_2;
    klt_filter_offset_config_3_t    klt_filter_offset_config_3;
    klt_filter_offset_config_4_t    klt_filter_offset_config_4;
} klt_filter_offset_config_cfg_t;

typedef struct _klt_gyro_grid_cfg_t {
    klt_gyro_grid_conf_t     klt_gyro_grid_conf;
    klt_gyro_grid_offset_t   klt_gyro_grid_offset;
    klt_gyro_grid_displ_t    klt_gyro_grid_displ[KLT_GYRO_GRID_DISPL];
} klt_gyro_grid_cfg_t;

typedef union {
	struct {
		unsigned int    kp_prev_x_y : 18  ; /* [17:0] */
		unsigned int    rsv_15      : 14  ; /* [31:18] */
	} bits;

	unsigned int    u32;
} klt_kp_prev_buff_xy_t;

typedef union {
	struct {
		unsigned int    kp_next_x_y : 18  ; /* [17:0] */
		unsigned int    rsv_15      : 14  ; /* [31:18] */
	} bits;

	unsigned int    u32;
} klt_kp_next_buff_xy_t;

typedef struct _klt_prev_next_kp_coord_cfg_t {
	// cfg order: kp_prev_x/kp_prev_y/kp_prev_x/kp_prev_y ....
	klt_kp_number_t       klt_kp_number;
	klt_kp_prev_buff_xy_t kp_prev_xy[CFG_TAB_KLT_MAX_KP_NUM * 2];
	klt_kp_next_buff_xy_t kp_next_xy[CFG_TAB_KLT_MAX_KP_NUM * 2];
} klt_prev_next_kp_coord_cfg_t;

typedef struct _klt_klt_cfg_t {
	klt_start_cfg_t            klt_start;
	klt_fwd_search_cfg_t       fwd_search;
	klt_bwd_search_cfg_t       bwd_search;
	klt_iter_cfg_cfg_t         iter_param;
	klt_eval_param_cfg_t       eval_param;
	klt_extra_feature_t        extra_feature;
	klt_roi_config_cfg_t       roi_config;
	klt_filter_offset_config_cfg_t    filter_offset_config;
	klt_gyro_grid_cfg_t        gyro_grid;
	klt_prev_and_next_rd_cfg_t rd_cfg;
	klt_prev_next_kp_coord_cfg_t    prev_next_kp_coord;
} klt_klt_cfg_t;

typedef struct _cfg_tab_klt_t {
	klt_ds_cfg_t  klt_ds_cfg;
	klt_klt_cfg_t klt_klt_cfg;

	unsigned int  klt_cascade_en;
} cfg_tab_klt_t;

#endif /* __CFG_TABLE_KLT_CS_H__ */
