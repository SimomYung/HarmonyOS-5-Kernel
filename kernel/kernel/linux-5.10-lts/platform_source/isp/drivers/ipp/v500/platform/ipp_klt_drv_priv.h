/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  ipp_klt_drv_priv.h
 * Project line  :  
 * Department    :  
 * Author        :  Anthony Sixta
 * Version       :  1.0
 * Date          :  41518
 * Description   :  HIStarISP  top level IPP_KLT description
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  Anthony Sixta 2023/11/01 10:09:45 Create file
 */

#ifndef __IPP_KLT_DRV_PRIV_H__
#define __IPP_KLT_DRV_PRIV_H__

/* Define the union U_ds_config */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    ds_width    : 10  ; /* [9:0] */
		unsigned int    ds_height   : 10  ; /* [19:10] */
		unsigned int    ds_nb_layer : 3  ; /* [22:20] */
		unsigned int    rsv_0       : 9  ; /* [31:23] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_ds_config;

/* Define the union U_rd_prev_base_addr */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    rd_prev_base_addr : 27  ; /* [26:0] */
		unsigned int    rsv_1             : 5  ; /* [31:27] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_rd_prev_base_addr;

/* Define the union U_rd_prev_stride */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    rd_prev_line_stride     : 10  ; /* [9:0] */
		unsigned int    rd_prev_line_stridectrl : 1  ; /* [10] */
		unsigned int    rsv_2                   : 21  ; /* [31:11] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_rd_prev_stride;

/* Define the union U_rd_next_base_addr */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    rd_next_base_addr : 27  ; /* [26:0] */
		unsigned int    rsv_3             : 5  ; /* [31:27] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_rd_next_base_addr;

/* Define the union U_rd_next_stride */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    rd_next_line_stride     : 10  ; /* [9:0] */
		unsigned int    rd_next_line_stridectrl : 1  ; /* [10] */
		unsigned int    rsv_4                   : 21  ; /* [31:11] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_rd_next_stride;

/* Define the union U_klt_start */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    start_klt    : 1  ; /* [0] */
		unsigned int    rsv_5        : 7  ; /* [7:1] */
		unsigned int    frame_width  : 10  ; /* [17:8] */
		unsigned int    frame_height : 10  ; /* [27:18] */
		unsigned int    rsv_6        : 4  ; /* [31:28] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_start;

/* Define the union U_klt_fwd_config */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    fwd_pyr_num       : 3  ; /* [2:0] */
		unsigned int    fwd_patch_size    : 5  ; /* [7:3] */
		unsigned int    fwd_iter_accuracy : 10  ; /* [17:8] */
		unsigned int    rsv_7             : 14  ; /* [31:18] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_fwd_config;

/* Define the union U_klt_bwd_config */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    bwd_pyr_num       : 1  ; /* [0] */
		unsigned int    rsv_8             : 2  ; /* [2:1] */
		unsigned int    bwd_patch_size    : 5  ; /* [7:3] */
		unsigned int    bwd_iter_accuracy : 10  ; /* [17:8] */
		unsigned int    rsv_9             : 14  ; /* [31:18] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_bwd_config;

/* Define the union U_klt_iter_config_1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    fwd_iter_guaranteed_nr_4 : 5  ; /* [4:0] */
		unsigned int    fwd_iter_guaranteed_nr_3 : 5  ; /* [9:5] */
		unsigned int    fwd_iter_guaranteed_nr_2 : 5  ; /* [14:10] */
		unsigned int    fwd_iter_guaranteed_nr_1 : 5  ; /* [19:15] */
		unsigned int    fwd_iter_guaranteed_nr_0 : 5  ; /* [24:20] */
		unsigned int    bwd_iter_guaranteed_nr_0 : 4  ; /* [28:25] */
		unsigned int    rsv_10                   : 3  ; /* [31:29] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_iter_config_1;

/* Define the union U_klt_iter_config_2 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    fwd_iter_max_nr_4 : 5  ; /* [4:0] */
		unsigned int    fwd_iter_max_nr_3 : 5  ; /* [9:5] */
		unsigned int    fwd_iter_max_nr_2 : 5  ; /* [14:10] */
		unsigned int    fwd_iter_max_nr_1 : 5  ; /* [19:15] */
		unsigned int    fwd_iter_max_nr_0 : 5  ; /* [24:20] */
		unsigned int    bwd_iter_max_nr_0 : 4  ; /* [28:25] */
		unsigned int    rsv_11            : 3  ; /* [31:29] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_iter_config_2;

/* Define the union U_klt_eval_param */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    min_eig_thresh : 9  ; /* [8:0] */
		unsigned int    rsv_12         : 7  ; /* [15:9] */
		unsigned int    motion_epsilon : 9  ; /* [24:16] */
		unsigned int    rsv_13         : 7  ; /* [31:25] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_eval_param;

/* Define the union U_klt_extra_feature */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    roi_control       : 1  ; /* [0] */
		unsigned int    gyro_pre          : 1  ; /* [1] */
		unsigned int    rsv_14            : 1  ; /* [2] */
		unsigned int    kps_filter_offset : 1  ; /* [3] */
		unsigned int    discard_en        : 1  ; /* [4] */
		unsigned int    rsv_15            : 27  ; /* [31:5] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_klt_extra_feature;

/* Define the union U_roi_config */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    roi_inside : 1  ; /* [0] */
		unsigned int    rsv_16     : 7  ; /* [7:1] */
		unsigned int    roi_num    : 4  ; /* [11:8] */
		unsigned int    rsv_17     : 20  ; /* [31:12] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_roi_config;

/* Define the union U_roi_top_left */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    roi_left : 10  ; /* [9:0] */
		unsigned int    rsv_18   : 6  ; /* [15:10] */
		unsigned int    roi_top  : 10  ; /* [25:16] */
		unsigned int    rsv_19   : 6  ; /* [31:26] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_roi_top_left;

/* Define the union U_roi_bottom_right */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    roi_right  : 10  ; /* [9:0] */
		unsigned int    rsv_20     : 6  ; /* [15:10] */
		unsigned int    roi_bottom : 10  ; /* [25:16] */
		unsigned int    rsv_21     : 6  ; /* [31:26] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_roi_bottom_right;

/* Define the union U_filter_offset_config_1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    rsv_22                 : 1  ; /* [0] */
		unsigned int    outliers_removing      : 1  ; /* [1] */
		unsigned int    moving_object_removing : 1  ; /* [2] */
		unsigned int    dist_avg_value_inv     : 12  ; /* [14:3] */
		unsigned int    dist_avg_value         : 9  ; /* [23:15] */
		unsigned int    rsv_23                 : 8  ; /* [31:24] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_filter_offset_config_1;

/* Define the union U_filter_offset_config_2 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    outlier_bin_num_threshold     : 7  ; /* [6:0] */
		unsigned int    outlier_sum_bin_num_threshold : 7  ; /* [13:7] */
		unsigned int    moving_obj_dist_range         : 8  ; /* [21:14] */
		unsigned int    rsv_24                        : 10  ; /* [31:22] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_filter_offset_config_2;

/* Define the union U_filter_offset_config_3 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    min_remained_corner_num   : 8  ; /* [7:0] */
		unsigned int    min_remained_corner_ratio : 7  ; /* [14:8] */
		unsigned int    main_dist_threshold       : 7  ; /* [21:15] */
		unsigned int    main_dist_threshold_sec   : 7  ; /* [28:22] */
		unsigned int    rsv_25                    : 3  ; /* [31:29] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_filter_offset_config_3;

/* Define the union U_filter_offset_config_4 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    dist_bin_num       : 7  ; /* [6:0] */
		unsigned int    valid_id_range     : 7  ; /* [13:7] */
		unsigned int    valid_id_range_sec : 7  ; /* [20:14] */
		unsigned int    rsv_26             : 11  ; /* [31:21] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_filter_offset_config_4;

/* Define the union U_rsv_future_use */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    rfu_dbg : 32  ; /* [31:0] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_rsv_future_use;

/* Define the union U_gyro_grid_conf */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    gyro_grid_size_x : 4  ; /* [3:0] */
		unsigned int    gyro_grid_size_y : 4  ; /* [7:4] */
		unsigned int    gyro_grid_num_x  : 6  ; /* [13:8] */
		unsigned int    gyro_grid_num_y  : 6  ; /* [19:14] */
		unsigned int    rsv_27           : 12  ; /* [31:20] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_gyro_grid_conf;

/* Define the union U_gyro_grid_offset */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    gyro_grid_offset_x : 12  ; /* [11:0] */
		unsigned int    rsv_28             : 4  ; /* [15:12] */
		unsigned int    gyro_grid_offset_y : 12  ; /* [27:16] */
		unsigned int    rsv_29             : 4  ; /* [31:28] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_gyro_grid_offset;

/* Define the union U_gyro_grid_displ */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    gyro_grid_displ_x : 15  ; /* [14:0] */
		unsigned int    rsv_30            : 1  ; /* [15] */
		unsigned int    gyro_grid_displ_y : 15  ; /* [30:16] */
		unsigned int    rsv_31            : 1  ; /* [31] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_gyro_grid_displ;

/* Define the union U_kp_number */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    kp_numbers : 9  ; /* [8:0] */
		unsigned int    rsv_32     : 23  ; /* [31:9] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_kp_number;

/* Define the union U_kp_prev_buff_x */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    kp_prev_x : 19  ; /* [18:0] */
		unsigned int    rsv_33    : 13  ; /* [31:19] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_kp_prev_buff_x;

/* Define the union U_kp_prev_buff_y */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    kp_prev_y : 19  ; /* [18:0] */
		unsigned int    rsv_34    : 13  ; /* [31:19] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_kp_prev_buff_y;

/* Define the union U_kp_next_buff_x */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    kp_next_x : 19  ; /* [18:0] */
		unsigned int    rsv_35    : 13  ; /* [31:19] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_kp_next_buff_x;

/* Define the union U_kp_next_buff_y */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    kp_next_y : 19  ; /* [18:0] */
		unsigned int    status    : 1  ; /* [19] */
		unsigned int    conf      : 8  ; /* [27:20] */
		unsigned int    rsv_36    : 4  ; /* [31:28] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_kp_next_buff_y;

/* Define the union U_mc_kp_nb */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    val_kp_number     : 9  ; /* [8:0] */
		unsigned int    moving_obj_status : 1  ; /* [9] */
		unsigned int    rsv_37            : 22  ; /* [31:10] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_mc_kp_nb;

/* Define the union U_mc_next_kp */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    mc_cur_x : 16  ; /* [15:0] */
		unsigned int    mc_cur_y : 16  ; /* [31:16] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_mc_next_kp;

/* Define the union U_mc_prev_kp */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    mc_ref_x : 16  ; /* [15:0] */
		unsigned int    mc_ref_y : 16  ; /* [31:16] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} ipp_klt_mc_prev_kp;


// ==============================================================================
/* Define the global struct */
typedef struct {
	ipp_klt_ds_config              ds_config              ; /* 0 */
	ipp_klt_rd_prev_base_addr      rd_prev_base_addr[5]   ; /* 4 */
	ipp_klt_rd_prev_stride         rd_prev_stride[5]      ; /* 18 */
	ipp_klt_rd_next_base_addr      rd_next_base_addr[5]   ; /* 2C */
	ipp_klt_rd_next_stride         rd_next_stride[5]      ; /* 40 */
	ipp_klt_klt_start              klt_start              ; /* 54 */
	ipp_klt_klt_fwd_config         klt_fwd_config         ; /* 58 */
	ipp_klt_klt_bwd_config         klt_bwd_config         ; /* 5C */
	ipp_klt_klt_iter_config_1      klt_iter_config_1      ; /* 60 */
	ipp_klt_klt_iter_config_2      klt_iter_config_2      ; /* 64 */
	ipp_klt_klt_eval_param         klt_eval_param         ; /* 68 */
	ipp_klt_klt_extra_feature      klt_extra_feature      ; /* 6C */
	ipp_klt_roi_config             roi_config             ; /* 70 */
	ipp_klt_roi_top_left           roi_top_left[8]        ; /* 74 */
	ipp_klt_roi_bottom_right       roi_bottom_right[8]    ; /* 94 */
	ipp_klt_filter_offset_config_1 filter_offset_config_1 ; /* B4 */
	ipp_klt_filter_offset_config_2 filter_offset_config_2 ; /* B8 */
	ipp_klt_filter_offset_config_3 filter_offset_config_3 ; /* BC */
	ipp_klt_filter_offset_config_4 filter_offset_config_4 ; /* C0 */
	ipp_klt_rsv_future_use         rsv_future_use         ; /* C4 */
	ipp_klt_gyro_grid_conf         gyro_grid_conf         ; /* C8 */
	ipp_klt_gyro_grid_offset       gyro_grid_offset       ; /* CC */
	ipp_klt_gyro_grid_displ        gyro_grid_displ[1089]  ; /* D0 */
	ipp_klt_kp_number              kp_number              ; /* 11D4 */
	ipp_klt_kp_prev_buff_x         kp_prev_buff_x[500]    ; /* 11DC */
	ipp_klt_kp_prev_buff_y         kp_prev_buff_y[500]    ; /* 11E0 */
	ipp_klt_kp_next_buff_x         kp_next_buff_x[500]    ; /* 217C */
	ipp_klt_kp_next_buff_y         kp_next_buff_y[500]    ; /* 2180 */
	ipp_klt_mc_kp_nb               mc_kp_nb               ; /* 311C */
	ipp_klt_mc_next_kp             mc_next_kp[500]        ; /* 3120 */
	ipp_klt_mc_prev_kp             mc_prev_kp[500]        ; /* 3124 */
} s_ipp_klt_regs_type;


#endif // __IPP_KLT_DRV_PRIV_H__
