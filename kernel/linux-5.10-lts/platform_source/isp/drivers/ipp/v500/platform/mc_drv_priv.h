/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  mc_drv_priv.h
 * Project line  :  
 * Department    :  
 * Author        :  HerveDANIEL
 * Version       :  1.0
 * Date          :  40876
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  HerveDANIEL 2023/11/01 10:09:46 Create file
 */

#ifndef __MC_DRV_PRIV_H__
#define __MC_DRV_PRIV_H__

/* Define the union U_en_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    mc_en : 1  ; /* [0] */
		unsigned int    rsv_0 : 31  ; /* [31:1] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_en_cfg;

/* Define the union U_imu_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    imu_en     : 1  ; /* [0] */
		unsigned int    imu_static : 1  ; /* [1] */
		unsigned int    rsv_1      : 30  ; /* [31:2] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_imu_cfg;

/* Define the union U_mc_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    cfg_mode        : 1  ; /* [0] */
		unsigned int    rsv_2           : 1  ; /* [1] */
		unsigned int    push_inliers_en : 1  ; /* [2] */
		unsigned int    rsv_3           : 1  ; /* [3] */
		unsigned int    max_iterations  : 12  ; /* [15:4] */
		unsigned int    svd_iterations  : 5  ; /* [20:16] */
		unsigned int    rsv_4           : 11  ; /* [31:21] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_mc_cfg;

/* Define the union U_threshold_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    inlier_th : 20  ; /* [19:0] */
		unsigned int    rsv_5     : 12  ; /* [31:20] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_threshold_cfg;

/* Define the union U_inlier_number */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    inlier_num_th : 11  ; /* [10:0] */
		unsigned int    rsv_6         : 21  ; /* [31:11] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_inlier_number;

/* Define the union U_kpt_size */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    kpt_size : 10  ; /* [9:0] */
		unsigned int    rsv_7    : 22  ; /* [31:10] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_kpt_size;

/* Define the union U_static_cfg2 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    matched_num_th : 11  ; /* [10:0] */
		unsigned int    rsv_8          : 21  ; /* [31:11] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_static_cfg2;

/* Define the union U_ref_prefetch_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    ref_first_32k_page  : 19  ; /* [18:0] */
		unsigned int    rsv_9               : 1  ; /* [19] */
		unsigned int    ref_prefetch_enable : 1  ; /* [20] */
		unsigned int    rsv_10              : 11  ; /* [31:21] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_ref_prefetch_cfg;

/* Define the union U_cur_prefetch_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    cur_first_32k_page  : 19  ; /* [18:0] */
		unsigned int    rsv_11              : 1  ; /* [19] */
		unsigned int    cur_prefetch_enable : 1  ; /* [20] */
		unsigned int    rsv_12              : 11  ; /* [31:21] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_cur_prefetch_cfg;

/* Define the union U_match_points */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    matched_kpts : 11  ; /* [10:0] */
		unsigned int    rsv_13       : 21  ; /* [31:11] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_match_points;

/* Define the union U_index_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    cur_index : 12  ; /* [11:0] */
		unsigned int    ref_index : 12  ; /* [23:12] */
		unsigned int    rsv_14    : 8  ; /* [31:24] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_index_cfg;

/* Define the union U_coordinate_cfg */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    coordinate_x : 16  ; /* [15:0] */
		unsigned int    coordinate_y : 16  ; /* [31:16] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_coordinate_cfg;

/* Define the union U_low_kpt_status */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    low_kpt : 1  ; /* [0] */
		unsigned int    rsv_15  : 31  ; /* [31:1] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_low_kpt_status;

/* Define the union U_debug_0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    debug_0 : 32  ; /* [31:0] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_debug_0;

/* Define the union U_debug_1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    debug_1 : 32  ; /* [31:0] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_debug_1;

/* Define the union U_ec_0 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    ec_0 : 32  ; /* [31:0] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_ec_0;

/* Define the union U_ec_1 */
typedef union {
	/* Define the struct bits */
	struct {
		unsigned int    ec_1 : 32  ; /* [31:0] */
	} bits;

	/* Define an unsigned member */
	unsigned int    u32;
} mc_ec_1;


// ==============================================================================
/* Define the global struct */
typedef struct {
	mc_en_cfg           en_cfg               ; /* 0 */
	mc_imu_cfg          imu_cfg              ; /* 18 */
	mc_mc_cfg           mc_cfg               ; /* 1C */
	mc_threshold_cfg    threshold_cfg        ; /* 20 */
	mc_inlier_number    inlier_number        ; /* 24 */
	mc_kpt_size         kpt_size             ; /* 28 */
	mc_static_cfg2      static_cfg2          ; /* 30 */
	mc_ref_prefetch_cfg ref_prefetch_cfg     ; /* 34 */
	mc_cur_prefetch_cfg cur_prefetch_cfg     ; /* 38 */
	mc_match_points     match_points         ; /* 3C */
	mc_index_cfg        index_cfg[1600]      ; /* 40 */
	mc_coordinate_cfg   coordinate_cfg[3200] ; /* 1A00 */
	mc_low_kpt_status   low_kpt_status       ; /* 4C00 */
	mc_debug_0          debug_0              ; /* 4D00 */
	mc_debug_1          debug_1              ; /* 4D04 */
	mc_ec_0             ec_0                 ; /* 4D08 */
	mc_ec_1             ec_1                 ; /* 4D0C */
} s_mc_regs_type;


#endif // __MC_DRV_PRIV_H__
