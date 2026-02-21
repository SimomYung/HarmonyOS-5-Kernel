/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  ipp_klt_reg_offset.h
 * Project line  :
 * Department    :
 * Author        :  Anthony Sixta
 * Version       :  1.0
 * Date          :  41518
 * Description   :  HIStarISP  top level IPP_KLT description
 * Others        :  Generated automatically by nManager V5.1
 * History       :  Anthony Sixta 2023/09/11 09:43:09 Create file
 */

#ifndef __IPP_KLT_REG_OFFSET_H__
#define __IPP_KLT_REG_OFFSET_H__

/* IPP_KLT Base address of Module's Register */
#define IPP_KLT_BASE (0x0)

/******************************************************************************/
/*                      HiStarISP IPP_KLT Registers' Definitions                            */
/******************************************************************************/

#define IPP_KLT_DS_CONFIG_REG (IPP_KLT_BASE + 0x0) /* Downscaler configuration */
#define IPP_KLT_RD_PREV_BASE_ADDR_0_REG \
    (IPP_KLT_BASE + 0x4) /* Base addresses for downscaled frames ( previous frame) */
#define IPP_KLT_RD_PREV_BASE_ADDR_1_REG \
    (IPP_KLT_BASE + 0x8) /* Base addresses for downscaled frames ( previous frame) */
#define IPP_KLT_RD_PREV_BASE_ADDR_2_REG \
    (IPP_KLT_BASE + 0xC) /* Base addresses for downscaled frames ( previous frame) */
#define IPP_KLT_RD_PREV_BASE_ADDR_3_REG \
    (IPP_KLT_BASE + 0x10) /* Base addresses for downscaled frames ( previous frame) */
#define IPP_KLT_RD_PREV_BASE_ADDR_4_REG \
    (IPP_KLT_BASE + 0x14)                                  /* Base addresses for downscaled frames ( previous frame) */
#define IPP_KLT_RD_PREV_STRIDE_0_REG (IPP_KLT_BASE + 0x18) /* Read prev line stride param */
#define IPP_KLT_RD_PREV_STRIDE_1_REG (IPP_KLT_BASE + 0x1C) /* Read prev line stride param */
#define IPP_KLT_RD_PREV_STRIDE_2_REG (IPP_KLT_BASE + 0x20) /* Read prev line stride param */
#define IPP_KLT_RD_PREV_STRIDE_3_REG (IPP_KLT_BASE + 0x24) /* Read prev line stride param */
#define IPP_KLT_RD_PREV_STRIDE_4_REG (IPP_KLT_BASE + 0x28) /* Read prev line stride param */
#define IPP_KLT_RD_NEXT_BASE_ADDR_0_REG (IPP_KLT_BASE + 0x2C) /* Base addresses for downscaled frames ( next frame) */
#define IPP_KLT_RD_NEXT_BASE_ADDR_1_REG (IPP_KLT_BASE + 0x30) /* Base addresses for downscaled frames ( next frame) */
#define IPP_KLT_RD_NEXT_BASE_ADDR_2_REG (IPP_KLT_BASE + 0x34) /* Base addresses for downscaled frames ( next frame) */
#define IPP_KLT_RD_NEXT_BASE_ADDR_3_REG (IPP_KLT_BASE + 0x38) /* Base addresses for downscaled frames ( next frame) */
#define IPP_KLT_RD_NEXT_BASE_ADDR_4_REG (IPP_KLT_BASE + 0x3C) /* Base addresses for downscaled frames ( next frame) */
#define IPP_KLT_RD_NEXT_STRIDE_0_REG (IPP_KLT_BASE + 0x40)    /* Read next line stride param */
#define IPP_KLT_RD_NEXT_STRIDE_1_REG (IPP_KLT_BASE + 0x44)    /* Read next line stride param */
#define IPP_KLT_RD_NEXT_STRIDE_2_REG (IPP_KLT_BASE + 0x48)    /* Read next line stride param */
#define IPP_KLT_RD_NEXT_STRIDE_3_REG (IPP_KLT_BASE + 0x4C)    /* Read next line stride param */
#define IPP_KLT_RD_NEXT_STRIDE_4_REG (IPP_KLT_BASE + 0x50)    /* Read next line stride param */
#define IPP_KLT_KLT_START_REG (IPP_KLT_BASE + 0x54)           /* Start trigger */
#define IPP_KLT_KLT_FWD_CONFIG_REG (IPP_KLT_BASE + 0x58)      /* Forward Search parameters */
#define IPP_KLT_KLT_BWD_CONFIG_REG (IPP_KLT_BASE + 0x5C)      /* Backward Search parameters */
#define IPP_KLT_KLT_ITER_CONFIG_1_REG (IPP_KLT_BASE + 0x60)   /* Iteration configuration */
#define IPP_KLT_KLT_ITER_CONFIG_2_REG (IPP_KLT_BASE + 0x64)   /* Iteration configuration */
#define IPP_KLT_KLT_EVAL_PARAM_REG (IPP_KLT_BASE + 0x68)      /* KP evaluation */
#define IPP_KLT_KLT_EXTRA_FEATURE_REG (IPP_KLT_BASE + 0x6C)   /* KP pre & post processing */
#define IPP_KLT_ROI_CONFIG_REG (IPP_KLT_BASE + 0x70)          /* ROI configuration */
#define IPP_KLT_ROI_TOP_LEFT_0_REG (IPP_KLT_BASE + 0x74)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_1_REG (IPP_KLT_BASE + 0x78)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_2_REG (IPP_KLT_BASE + 0x7C)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_3_REG (IPP_KLT_BASE + 0x80)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_4_REG (IPP_KLT_BASE + 0x84)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_5_REG (IPP_KLT_BASE + 0x88)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_6_REG (IPP_KLT_BASE + 0x8C)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_TOP_LEFT_7_REG (IPP_KLT_BASE + 0x90)      /* ROI top-left coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_0_REG (IPP_KLT_BASE + 0x94)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_1_REG (IPP_KLT_BASE + 0x98)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_2_REG (IPP_KLT_BASE + 0x9C)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_3_REG (IPP_KLT_BASE + 0xA0)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_4_REG (IPP_KLT_BASE + 0xA4)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_5_REG (IPP_KLT_BASE + 0xA8)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_6_REG (IPP_KLT_BASE + 0xAC)  /* ROI bottom-right coordinates */
#define IPP_KLT_ROI_BOTTOM_RIGHT_7_REG (IPP_KLT_BASE + 0xB0)  /* ROI bottom-right coordinates */
#define IPP_KLT_FILTER_OFFSET_CONFIG_1_REG (IPP_KLT_BASE + 0xB4) /* KPS_FILTER_OFFSET configuration */
#define IPP_KLT_FILTER_OFFSET_CONFIG_2_REG (IPP_KLT_BASE + 0xB8) /* KPS_FILTER_OFFSET configuration */
#define IPP_KLT_FILTER_OFFSET_CONFIG_3_REG (IPP_KLT_BASE + 0xBC) /* KPS_FILTER_OFFSET configuration */
#define IPP_KLT_FILTER_OFFSET_CONFIG_4_REG (IPP_KLT_BASE + 0xC0) /* KPS_FILTER_OFFSET configuration */
#define IPP_KLT_RSV_FUTURE_USE_REG (IPP_KLT_BASE + 0xC4)         /* Reserved for future use or debug */
#define IPP_KLT_GYRO_GRID_CONF_REG (IPP_KLT_BASE + 0xC8)         /* Size of GYRO Cgrid */
#define IPP_KLT_GYRO_GRID_OFFSET_REG (IPP_KLT_BASE + 0xCC)       /* Size of GYRO Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_0_REG (IPP_KLT_BASE + 0xD0)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1_REG (IPP_KLT_BASE + 0xD4)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_2_REG (IPP_KLT_BASE + 0xD8)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_3_REG (IPP_KLT_BASE + 0xDC)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_4_REG (IPP_KLT_BASE + 0xE0)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_5_REG (IPP_KLT_BASE + 0xE4)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_6_REG (IPP_KLT_BASE + 0xE8)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_7_REG (IPP_KLT_BASE + 0xEC)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_8_REG (IPP_KLT_BASE + 0xF0)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_9_REG (IPP_KLT_BASE + 0xF4)      /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_10_REG (IPP_KLT_BASE + 0xF8)     /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_11_REG (IPP_KLT_BASE + 0xFC)     /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_12_REG (IPP_KLT_BASE + 0x100)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_13_REG (IPP_KLT_BASE + 0x104)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_14_REG (IPP_KLT_BASE + 0x108)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_15_REG (IPP_KLT_BASE + 0x10C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_16_REG (IPP_KLT_BASE + 0x110)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_17_REG (IPP_KLT_BASE + 0x114)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_18_REG (IPP_KLT_BASE + 0x118)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_19_REG (IPP_KLT_BASE + 0x11C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_20_REG (IPP_KLT_BASE + 0x120)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_21_REG (IPP_KLT_BASE + 0x124)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_22_REG (IPP_KLT_BASE + 0x128)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_23_REG (IPP_KLT_BASE + 0x12C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_24_REG (IPP_KLT_BASE + 0x130)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_25_REG (IPP_KLT_BASE + 0x134)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_26_REG (IPP_KLT_BASE + 0x138)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_27_REG (IPP_KLT_BASE + 0x13C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_28_REG (IPP_KLT_BASE + 0x140)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_29_REG (IPP_KLT_BASE + 0x144)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_30_REG (IPP_KLT_BASE + 0x148)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_31_REG (IPP_KLT_BASE + 0x14C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_32_REG (IPP_KLT_BASE + 0x150)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_33_REG (IPP_KLT_BASE + 0x154)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_34_REG (IPP_KLT_BASE + 0x158)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_35_REG (IPP_KLT_BASE + 0x15C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_36_REG (IPP_KLT_BASE + 0x160)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_37_REG (IPP_KLT_BASE + 0x164)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_38_REG (IPP_KLT_BASE + 0x168)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_39_REG (IPP_KLT_BASE + 0x16C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_40_REG (IPP_KLT_BASE + 0x170)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_41_REG (IPP_KLT_BASE + 0x174)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_42_REG (IPP_KLT_BASE + 0x178)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_43_REG (IPP_KLT_BASE + 0x17C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_44_REG (IPP_KLT_BASE + 0x180)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_45_REG (IPP_KLT_BASE + 0x184)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_46_REG (IPP_KLT_BASE + 0x188)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_47_REG (IPP_KLT_BASE + 0x18C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_48_REG (IPP_KLT_BASE + 0x190)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_49_REG (IPP_KLT_BASE + 0x194)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_50_REG (IPP_KLT_BASE + 0x198)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_51_REG (IPP_KLT_BASE + 0x19C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_52_REG (IPP_KLT_BASE + 0x1A0)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_53_REG (IPP_KLT_BASE + 0x1A4)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_54_REG (IPP_KLT_BASE + 0x1A8)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_55_REG (IPP_KLT_BASE + 0x1AC)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_56_REG (IPP_KLT_BASE + 0x1B0)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_57_REG (IPP_KLT_BASE + 0x1B4)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_58_REG (IPP_KLT_BASE + 0x1B8)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_59_REG (IPP_KLT_BASE + 0x1BC)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_60_REG (IPP_KLT_BASE + 0x1C0)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_61_REG (IPP_KLT_BASE + 0x1C4)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_62_REG (IPP_KLT_BASE + 0x1C8)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_63_REG (IPP_KLT_BASE + 0x1CC)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_64_REG (IPP_KLT_BASE + 0x1D0)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_65_REG (IPP_KLT_BASE + 0x1D4)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_66_REG (IPP_KLT_BASE + 0x1D8)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_67_REG (IPP_KLT_BASE + 0x1DC)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_68_REG (IPP_KLT_BASE + 0x1E0)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_69_REG (IPP_KLT_BASE + 0x1E4)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_70_REG (IPP_KLT_BASE + 0x1E8)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_71_REG (IPP_KLT_BASE + 0x1EC)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_72_REG (IPP_KLT_BASE + 0x1F0)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_73_REG (IPP_KLT_BASE + 0x1F4)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_74_REG (IPP_KLT_BASE + 0x1F8)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_75_REG (IPP_KLT_BASE + 0x1FC)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_76_REG (IPP_KLT_BASE + 0x200)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_77_REG (IPP_KLT_BASE + 0x204)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_78_REG (IPP_KLT_BASE + 0x208)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_79_REG (IPP_KLT_BASE + 0x20C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_80_REG (IPP_KLT_BASE + 0x210)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_81_REG (IPP_KLT_BASE + 0x214)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_82_REG (IPP_KLT_BASE + 0x218)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_83_REG (IPP_KLT_BASE + 0x21C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_84_REG (IPP_KLT_BASE + 0x220)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_85_REG (IPP_KLT_BASE + 0x224)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_86_REG (IPP_KLT_BASE + 0x228)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_87_REG (IPP_KLT_BASE + 0x22C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_88_REG (IPP_KLT_BASE + 0x230)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_89_REG (IPP_KLT_BASE + 0x234)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_90_REG (IPP_KLT_BASE + 0x238)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_91_REG (IPP_KLT_BASE + 0x23C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_92_REG (IPP_KLT_BASE + 0x240)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_93_REG (IPP_KLT_BASE + 0x244)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_94_REG (IPP_KLT_BASE + 0x248)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_95_REG (IPP_KLT_BASE + 0x24C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_96_REG (IPP_KLT_BASE + 0x250)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_97_REG (IPP_KLT_BASE + 0x254)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_98_REG (IPP_KLT_BASE + 0x258)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_99_REG (IPP_KLT_BASE + 0x25C)    /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_100_REG (IPP_KLT_BASE + 0x260)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_101_REG (IPP_KLT_BASE + 0x264)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_102_REG (IPP_KLT_BASE + 0x268)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_103_REG (IPP_KLT_BASE + 0x26C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_104_REG (IPP_KLT_BASE + 0x270)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_105_REG (IPP_KLT_BASE + 0x274)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_106_REG (IPP_KLT_BASE + 0x278)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_107_REG (IPP_KLT_BASE + 0x27C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_108_REG (IPP_KLT_BASE + 0x280)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_109_REG (IPP_KLT_BASE + 0x284)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_110_REG (IPP_KLT_BASE + 0x288)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_111_REG (IPP_KLT_BASE + 0x28C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_112_REG (IPP_KLT_BASE + 0x290)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_113_REG (IPP_KLT_BASE + 0x294)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_114_REG (IPP_KLT_BASE + 0x298)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_115_REG (IPP_KLT_BASE + 0x29C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_116_REG (IPP_KLT_BASE + 0x2A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_117_REG (IPP_KLT_BASE + 0x2A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_118_REG (IPP_KLT_BASE + 0x2A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_119_REG (IPP_KLT_BASE + 0x2AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_120_REG (IPP_KLT_BASE + 0x2B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_121_REG (IPP_KLT_BASE + 0x2B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_122_REG (IPP_KLT_BASE + 0x2B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_123_REG (IPP_KLT_BASE + 0x2BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_124_REG (IPP_KLT_BASE + 0x2C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_125_REG (IPP_KLT_BASE + 0x2C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_126_REG (IPP_KLT_BASE + 0x2C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_127_REG (IPP_KLT_BASE + 0x2CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_128_REG (IPP_KLT_BASE + 0x2D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_129_REG (IPP_KLT_BASE + 0x2D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_130_REG (IPP_KLT_BASE + 0x2D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_131_REG (IPP_KLT_BASE + 0x2DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_132_REG (IPP_KLT_BASE + 0x2E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_133_REG (IPP_KLT_BASE + 0x2E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_134_REG (IPP_KLT_BASE + 0x2E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_135_REG (IPP_KLT_BASE + 0x2EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_136_REG (IPP_KLT_BASE + 0x2F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_137_REG (IPP_KLT_BASE + 0x2F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_138_REG (IPP_KLT_BASE + 0x2F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_139_REG (IPP_KLT_BASE + 0x2FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_140_REG (IPP_KLT_BASE + 0x300)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_141_REG (IPP_KLT_BASE + 0x304)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_142_REG (IPP_KLT_BASE + 0x308)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_143_REG (IPP_KLT_BASE + 0x30C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_144_REG (IPP_KLT_BASE + 0x310)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_145_REG (IPP_KLT_BASE + 0x314)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_146_REG (IPP_KLT_BASE + 0x318)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_147_REG (IPP_KLT_BASE + 0x31C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_148_REG (IPP_KLT_BASE + 0x320)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_149_REG (IPP_KLT_BASE + 0x324)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_150_REG (IPP_KLT_BASE + 0x328)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_151_REG (IPP_KLT_BASE + 0x32C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_152_REG (IPP_KLT_BASE + 0x330)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_153_REG (IPP_KLT_BASE + 0x334)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_154_REG (IPP_KLT_BASE + 0x338)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_155_REG (IPP_KLT_BASE + 0x33C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_156_REG (IPP_KLT_BASE + 0x340)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_157_REG (IPP_KLT_BASE + 0x344)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_158_REG (IPP_KLT_BASE + 0x348)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_159_REG (IPP_KLT_BASE + 0x34C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_160_REG (IPP_KLT_BASE + 0x350)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_161_REG (IPP_KLT_BASE + 0x354)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_162_REG (IPP_KLT_BASE + 0x358)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_163_REG (IPP_KLT_BASE + 0x35C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_164_REG (IPP_KLT_BASE + 0x360)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_165_REG (IPP_KLT_BASE + 0x364)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_166_REG (IPP_KLT_BASE + 0x368)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_167_REG (IPP_KLT_BASE + 0x36C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_168_REG (IPP_KLT_BASE + 0x370)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_169_REG (IPP_KLT_BASE + 0x374)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_170_REG (IPP_KLT_BASE + 0x378)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_171_REG (IPP_KLT_BASE + 0x37C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_172_REG (IPP_KLT_BASE + 0x380)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_173_REG (IPP_KLT_BASE + 0x384)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_174_REG (IPP_KLT_BASE + 0x388)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_175_REG (IPP_KLT_BASE + 0x38C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_176_REG (IPP_KLT_BASE + 0x390)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_177_REG (IPP_KLT_BASE + 0x394)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_178_REG (IPP_KLT_BASE + 0x398)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_179_REG (IPP_KLT_BASE + 0x39C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_180_REG (IPP_KLT_BASE + 0x3A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_181_REG (IPP_KLT_BASE + 0x3A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_182_REG (IPP_KLT_BASE + 0x3A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_183_REG (IPP_KLT_BASE + 0x3AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_184_REG (IPP_KLT_BASE + 0x3B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_185_REG (IPP_KLT_BASE + 0x3B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_186_REG (IPP_KLT_BASE + 0x3B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_187_REG (IPP_KLT_BASE + 0x3BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_188_REG (IPP_KLT_BASE + 0x3C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_189_REG (IPP_KLT_BASE + 0x3C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_190_REG (IPP_KLT_BASE + 0x3C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_191_REG (IPP_KLT_BASE + 0x3CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_192_REG (IPP_KLT_BASE + 0x3D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_193_REG (IPP_KLT_BASE + 0x3D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_194_REG (IPP_KLT_BASE + 0x3D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_195_REG (IPP_KLT_BASE + 0x3DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_196_REG (IPP_KLT_BASE + 0x3E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_197_REG (IPP_KLT_BASE + 0x3E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_198_REG (IPP_KLT_BASE + 0x3E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_199_REG (IPP_KLT_BASE + 0x3EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_200_REG (IPP_KLT_BASE + 0x3F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_201_REG (IPP_KLT_BASE + 0x3F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_202_REG (IPP_KLT_BASE + 0x3F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_203_REG (IPP_KLT_BASE + 0x3FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_204_REG (IPP_KLT_BASE + 0x400)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_205_REG (IPP_KLT_BASE + 0x404)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_206_REG (IPP_KLT_BASE + 0x408)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_207_REG (IPP_KLT_BASE + 0x40C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_208_REG (IPP_KLT_BASE + 0x410)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_209_REG (IPP_KLT_BASE + 0x414)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_210_REG (IPP_KLT_BASE + 0x418)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_211_REG (IPP_KLT_BASE + 0x41C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_212_REG (IPP_KLT_BASE + 0x420)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_213_REG (IPP_KLT_BASE + 0x424)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_214_REG (IPP_KLT_BASE + 0x428)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_215_REG (IPP_KLT_BASE + 0x42C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_216_REG (IPP_KLT_BASE + 0x430)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_217_REG (IPP_KLT_BASE + 0x434)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_218_REG (IPP_KLT_BASE + 0x438)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_219_REG (IPP_KLT_BASE + 0x43C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_220_REG (IPP_KLT_BASE + 0x440)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_221_REG (IPP_KLT_BASE + 0x444)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_222_REG (IPP_KLT_BASE + 0x448)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_223_REG (IPP_KLT_BASE + 0x44C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_224_REG (IPP_KLT_BASE + 0x450)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_225_REG (IPP_KLT_BASE + 0x454)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_226_REG (IPP_KLT_BASE + 0x458)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_227_REG (IPP_KLT_BASE + 0x45C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_228_REG (IPP_KLT_BASE + 0x460)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_229_REG (IPP_KLT_BASE + 0x464)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_230_REG (IPP_KLT_BASE + 0x468)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_231_REG (IPP_KLT_BASE + 0x46C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_232_REG (IPP_KLT_BASE + 0x470)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_233_REG (IPP_KLT_BASE + 0x474)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_234_REG (IPP_KLT_BASE + 0x478)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_235_REG (IPP_KLT_BASE + 0x47C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_236_REG (IPP_KLT_BASE + 0x480)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_237_REG (IPP_KLT_BASE + 0x484)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_238_REG (IPP_KLT_BASE + 0x488)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_239_REG (IPP_KLT_BASE + 0x48C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_240_REG (IPP_KLT_BASE + 0x490)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_241_REG (IPP_KLT_BASE + 0x494)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_242_REG (IPP_KLT_BASE + 0x498)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_243_REG (IPP_KLT_BASE + 0x49C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_244_REG (IPP_KLT_BASE + 0x4A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_245_REG (IPP_KLT_BASE + 0x4A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_246_REG (IPP_KLT_BASE + 0x4A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_247_REG (IPP_KLT_BASE + 0x4AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_248_REG (IPP_KLT_BASE + 0x4B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_249_REG (IPP_KLT_BASE + 0x4B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_250_REG (IPP_KLT_BASE + 0x4B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_251_REG (IPP_KLT_BASE + 0x4BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_252_REG (IPP_KLT_BASE + 0x4C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_253_REG (IPP_KLT_BASE + 0x4C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_254_REG (IPP_KLT_BASE + 0x4C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_255_REG (IPP_KLT_BASE + 0x4CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_256_REG (IPP_KLT_BASE + 0x4D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_257_REG (IPP_KLT_BASE + 0x4D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_258_REG (IPP_KLT_BASE + 0x4D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_259_REG (IPP_KLT_BASE + 0x4DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_260_REG (IPP_KLT_BASE + 0x4E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_261_REG (IPP_KLT_BASE + 0x4E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_262_REG (IPP_KLT_BASE + 0x4E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_263_REG (IPP_KLT_BASE + 0x4EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_264_REG (IPP_KLT_BASE + 0x4F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_265_REG (IPP_KLT_BASE + 0x4F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_266_REG (IPP_KLT_BASE + 0x4F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_267_REG (IPP_KLT_BASE + 0x4FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_268_REG (IPP_KLT_BASE + 0x500)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_269_REG (IPP_KLT_BASE + 0x504)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_270_REG (IPP_KLT_BASE + 0x508)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_271_REG (IPP_KLT_BASE + 0x50C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_272_REG (IPP_KLT_BASE + 0x510)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_273_REG (IPP_KLT_BASE + 0x514)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_274_REG (IPP_KLT_BASE + 0x518)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_275_REG (IPP_KLT_BASE + 0x51C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_276_REG (IPP_KLT_BASE + 0x520)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_277_REG (IPP_KLT_BASE + 0x524)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_278_REG (IPP_KLT_BASE + 0x528)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_279_REG (IPP_KLT_BASE + 0x52C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_280_REG (IPP_KLT_BASE + 0x530)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_281_REG (IPP_KLT_BASE + 0x534)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_282_REG (IPP_KLT_BASE + 0x538)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_283_REG (IPP_KLT_BASE + 0x53C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_284_REG (IPP_KLT_BASE + 0x540)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_285_REG (IPP_KLT_BASE + 0x544)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_286_REG (IPP_KLT_BASE + 0x548)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_287_REG (IPP_KLT_BASE + 0x54C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_288_REG (IPP_KLT_BASE + 0x550)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_289_REG (IPP_KLT_BASE + 0x554)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_290_REG (IPP_KLT_BASE + 0x558)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_291_REG (IPP_KLT_BASE + 0x55C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_292_REG (IPP_KLT_BASE + 0x560)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_293_REG (IPP_KLT_BASE + 0x564)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_294_REG (IPP_KLT_BASE + 0x568)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_295_REG (IPP_KLT_BASE + 0x56C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_296_REG (IPP_KLT_BASE + 0x570)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_297_REG (IPP_KLT_BASE + 0x574)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_298_REG (IPP_KLT_BASE + 0x578)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_299_REG (IPP_KLT_BASE + 0x57C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_300_REG (IPP_KLT_BASE + 0x580)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_301_REG (IPP_KLT_BASE + 0x584)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_302_REG (IPP_KLT_BASE + 0x588)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_303_REG (IPP_KLT_BASE + 0x58C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_304_REG (IPP_KLT_BASE + 0x590)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_305_REG (IPP_KLT_BASE + 0x594)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_306_REG (IPP_KLT_BASE + 0x598)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_307_REG (IPP_KLT_BASE + 0x59C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_308_REG (IPP_KLT_BASE + 0x5A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_309_REG (IPP_KLT_BASE + 0x5A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_310_REG (IPP_KLT_BASE + 0x5A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_311_REG (IPP_KLT_BASE + 0x5AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_312_REG (IPP_KLT_BASE + 0x5B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_313_REG (IPP_KLT_BASE + 0x5B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_314_REG (IPP_KLT_BASE + 0x5B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_315_REG (IPP_KLT_BASE + 0x5BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_316_REG (IPP_KLT_BASE + 0x5C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_317_REG (IPP_KLT_BASE + 0x5C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_318_REG (IPP_KLT_BASE + 0x5C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_319_REG (IPP_KLT_BASE + 0x5CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_320_REG (IPP_KLT_BASE + 0x5D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_321_REG (IPP_KLT_BASE + 0x5D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_322_REG (IPP_KLT_BASE + 0x5D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_323_REG (IPP_KLT_BASE + 0x5DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_324_REG (IPP_KLT_BASE + 0x5E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_325_REG (IPP_KLT_BASE + 0x5E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_326_REG (IPP_KLT_BASE + 0x5E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_327_REG (IPP_KLT_BASE + 0x5EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_328_REG (IPP_KLT_BASE + 0x5F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_329_REG (IPP_KLT_BASE + 0x5F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_330_REG (IPP_KLT_BASE + 0x5F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_331_REG (IPP_KLT_BASE + 0x5FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_332_REG (IPP_KLT_BASE + 0x600)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_333_REG (IPP_KLT_BASE + 0x604)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_334_REG (IPP_KLT_BASE + 0x608)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_335_REG (IPP_KLT_BASE + 0x60C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_336_REG (IPP_KLT_BASE + 0x610)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_337_REG (IPP_KLT_BASE + 0x614)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_338_REG (IPP_KLT_BASE + 0x618)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_339_REG (IPP_KLT_BASE + 0x61C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_340_REG (IPP_KLT_BASE + 0x620)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_341_REG (IPP_KLT_BASE + 0x624)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_342_REG (IPP_KLT_BASE + 0x628)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_343_REG (IPP_KLT_BASE + 0x62C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_344_REG (IPP_KLT_BASE + 0x630)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_345_REG (IPP_KLT_BASE + 0x634)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_346_REG (IPP_KLT_BASE + 0x638)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_347_REG (IPP_KLT_BASE + 0x63C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_348_REG (IPP_KLT_BASE + 0x640)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_349_REG (IPP_KLT_BASE + 0x644)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_350_REG (IPP_KLT_BASE + 0x648)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_351_REG (IPP_KLT_BASE + 0x64C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_352_REG (IPP_KLT_BASE + 0x650)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_353_REG (IPP_KLT_BASE + 0x654)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_354_REG (IPP_KLT_BASE + 0x658)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_355_REG (IPP_KLT_BASE + 0x65C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_356_REG (IPP_KLT_BASE + 0x660)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_357_REG (IPP_KLT_BASE + 0x664)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_358_REG (IPP_KLT_BASE + 0x668)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_359_REG (IPP_KLT_BASE + 0x66C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_360_REG (IPP_KLT_BASE + 0x670)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_361_REG (IPP_KLT_BASE + 0x674)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_362_REG (IPP_KLT_BASE + 0x678)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_363_REG (IPP_KLT_BASE + 0x67C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_364_REG (IPP_KLT_BASE + 0x680)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_365_REG (IPP_KLT_BASE + 0x684)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_366_REG (IPP_KLT_BASE + 0x688)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_367_REG (IPP_KLT_BASE + 0x68C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_368_REG (IPP_KLT_BASE + 0x690)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_369_REG (IPP_KLT_BASE + 0x694)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_370_REG (IPP_KLT_BASE + 0x698)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_371_REG (IPP_KLT_BASE + 0x69C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_372_REG (IPP_KLT_BASE + 0x6A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_373_REG (IPP_KLT_BASE + 0x6A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_374_REG (IPP_KLT_BASE + 0x6A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_375_REG (IPP_KLT_BASE + 0x6AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_376_REG (IPP_KLT_BASE + 0x6B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_377_REG (IPP_KLT_BASE + 0x6B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_378_REG (IPP_KLT_BASE + 0x6B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_379_REG (IPP_KLT_BASE + 0x6BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_380_REG (IPP_KLT_BASE + 0x6C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_381_REG (IPP_KLT_BASE + 0x6C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_382_REG (IPP_KLT_BASE + 0x6C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_383_REG (IPP_KLT_BASE + 0x6CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_384_REG (IPP_KLT_BASE + 0x6D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_385_REG (IPP_KLT_BASE + 0x6D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_386_REG (IPP_KLT_BASE + 0x6D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_387_REG (IPP_KLT_BASE + 0x6DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_388_REG (IPP_KLT_BASE + 0x6E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_389_REG (IPP_KLT_BASE + 0x6E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_390_REG (IPP_KLT_BASE + 0x6E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_391_REG (IPP_KLT_BASE + 0x6EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_392_REG (IPP_KLT_BASE + 0x6F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_393_REG (IPP_KLT_BASE + 0x6F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_394_REG (IPP_KLT_BASE + 0x6F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_395_REG (IPP_KLT_BASE + 0x6FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_396_REG (IPP_KLT_BASE + 0x700)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_397_REG (IPP_KLT_BASE + 0x704)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_398_REG (IPP_KLT_BASE + 0x708)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_399_REG (IPP_KLT_BASE + 0x70C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_400_REG (IPP_KLT_BASE + 0x710)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_401_REG (IPP_KLT_BASE + 0x714)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_402_REG (IPP_KLT_BASE + 0x718)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_403_REG (IPP_KLT_BASE + 0x71C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_404_REG (IPP_KLT_BASE + 0x720)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_405_REG (IPP_KLT_BASE + 0x724)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_406_REG (IPP_KLT_BASE + 0x728)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_407_REG (IPP_KLT_BASE + 0x72C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_408_REG (IPP_KLT_BASE + 0x730)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_409_REG (IPP_KLT_BASE + 0x734)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_410_REG (IPP_KLT_BASE + 0x738)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_411_REG (IPP_KLT_BASE + 0x73C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_412_REG (IPP_KLT_BASE + 0x740)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_413_REG (IPP_KLT_BASE + 0x744)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_414_REG (IPP_KLT_BASE + 0x748)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_415_REG (IPP_KLT_BASE + 0x74C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_416_REG (IPP_KLT_BASE + 0x750)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_417_REG (IPP_KLT_BASE + 0x754)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_418_REG (IPP_KLT_BASE + 0x758)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_419_REG (IPP_KLT_BASE + 0x75C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_420_REG (IPP_KLT_BASE + 0x760)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_421_REG (IPP_KLT_BASE + 0x764)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_422_REG (IPP_KLT_BASE + 0x768)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_423_REG (IPP_KLT_BASE + 0x76C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_424_REG (IPP_KLT_BASE + 0x770)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_425_REG (IPP_KLT_BASE + 0x774)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_426_REG (IPP_KLT_BASE + 0x778)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_427_REG (IPP_KLT_BASE + 0x77C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_428_REG (IPP_KLT_BASE + 0x780)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_429_REG (IPP_KLT_BASE + 0x784)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_430_REG (IPP_KLT_BASE + 0x788)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_431_REG (IPP_KLT_BASE + 0x78C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_432_REG (IPP_KLT_BASE + 0x790)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_433_REG (IPP_KLT_BASE + 0x794)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_434_REG (IPP_KLT_BASE + 0x798)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_435_REG (IPP_KLT_BASE + 0x79C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_436_REG (IPP_KLT_BASE + 0x7A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_437_REG (IPP_KLT_BASE + 0x7A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_438_REG (IPP_KLT_BASE + 0x7A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_439_REG (IPP_KLT_BASE + 0x7AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_440_REG (IPP_KLT_BASE + 0x7B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_441_REG (IPP_KLT_BASE + 0x7B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_442_REG (IPP_KLT_BASE + 0x7B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_443_REG (IPP_KLT_BASE + 0x7BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_444_REG (IPP_KLT_BASE + 0x7C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_445_REG (IPP_KLT_BASE + 0x7C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_446_REG (IPP_KLT_BASE + 0x7C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_447_REG (IPP_KLT_BASE + 0x7CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_448_REG (IPP_KLT_BASE + 0x7D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_449_REG (IPP_KLT_BASE + 0x7D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_450_REG (IPP_KLT_BASE + 0x7D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_451_REG (IPP_KLT_BASE + 0x7DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_452_REG (IPP_KLT_BASE + 0x7E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_453_REG (IPP_KLT_BASE + 0x7E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_454_REG (IPP_KLT_BASE + 0x7E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_455_REG (IPP_KLT_BASE + 0x7EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_456_REG (IPP_KLT_BASE + 0x7F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_457_REG (IPP_KLT_BASE + 0x7F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_458_REG (IPP_KLT_BASE + 0x7F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_459_REG (IPP_KLT_BASE + 0x7FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_460_REG (IPP_KLT_BASE + 0x800)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_461_REG (IPP_KLT_BASE + 0x804)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_462_REG (IPP_KLT_BASE + 0x808)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_463_REG (IPP_KLT_BASE + 0x80C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_464_REG (IPP_KLT_BASE + 0x810)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_465_REG (IPP_KLT_BASE + 0x814)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_466_REG (IPP_KLT_BASE + 0x818)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_467_REG (IPP_KLT_BASE + 0x81C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_468_REG (IPP_KLT_BASE + 0x820)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_469_REG (IPP_KLT_BASE + 0x824)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_470_REG (IPP_KLT_BASE + 0x828)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_471_REG (IPP_KLT_BASE + 0x82C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_472_REG (IPP_KLT_BASE + 0x830)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_473_REG (IPP_KLT_BASE + 0x834)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_474_REG (IPP_KLT_BASE + 0x838)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_475_REG (IPP_KLT_BASE + 0x83C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_476_REG (IPP_KLT_BASE + 0x840)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_477_REG (IPP_KLT_BASE + 0x844)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_478_REG (IPP_KLT_BASE + 0x848)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_479_REG (IPP_KLT_BASE + 0x84C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_480_REG (IPP_KLT_BASE + 0x850)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_481_REG (IPP_KLT_BASE + 0x854)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_482_REG (IPP_KLT_BASE + 0x858)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_483_REG (IPP_KLT_BASE + 0x85C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_484_REG (IPP_KLT_BASE + 0x860)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_485_REG (IPP_KLT_BASE + 0x864)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_486_REG (IPP_KLT_BASE + 0x868)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_487_REG (IPP_KLT_BASE + 0x86C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_488_REG (IPP_KLT_BASE + 0x870)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_489_REG (IPP_KLT_BASE + 0x874)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_490_REG (IPP_KLT_BASE + 0x878)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_491_REG (IPP_KLT_BASE + 0x87C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_492_REG (IPP_KLT_BASE + 0x880)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_493_REG (IPP_KLT_BASE + 0x884)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_494_REG (IPP_KLT_BASE + 0x888)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_495_REG (IPP_KLT_BASE + 0x88C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_496_REG (IPP_KLT_BASE + 0x890)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_497_REG (IPP_KLT_BASE + 0x894)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_498_REG (IPP_KLT_BASE + 0x898)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_499_REG (IPP_KLT_BASE + 0x89C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_500_REG (IPP_KLT_BASE + 0x8A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_501_REG (IPP_KLT_BASE + 0x8A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_502_REG (IPP_KLT_BASE + 0x8A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_503_REG (IPP_KLT_BASE + 0x8AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_504_REG (IPP_KLT_BASE + 0x8B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_505_REG (IPP_KLT_BASE + 0x8B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_506_REG (IPP_KLT_BASE + 0x8B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_507_REG (IPP_KLT_BASE + 0x8BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_508_REG (IPP_KLT_BASE + 0x8C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_509_REG (IPP_KLT_BASE + 0x8C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_510_REG (IPP_KLT_BASE + 0x8C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_511_REG (IPP_KLT_BASE + 0x8CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_512_REG (IPP_KLT_BASE + 0x8D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_513_REG (IPP_KLT_BASE + 0x8D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_514_REG (IPP_KLT_BASE + 0x8D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_515_REG (IPP_KLT_BASE + 0x8DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_516_REG (IPP_KLT_BASE + 0x8E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_517_REG (IPP_KLT_BASE + 0x8E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_518_REG (IPP_KLT_BASE + 0x8E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_519_REG (IPP_KLT_BASE + 0x8EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_520_REG (IPP_KLT_BASE + 0x8F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_521_REG (IPP_KLT_BASE + 0x8F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_522_REG (IPP_KLT_BASE + 0x8F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_523_REG (IPP_KLT_BASE + 0x8FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_524_REG (IPP_KLT_BASE + 0x900)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_525_REG (IPP_KLT_BASE + 0x904)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_526_REG (IPP_KLT_BASE + 0x908)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_527_REG (IPP_KLT_BASE + 0x90C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_528_REG (IPP_KLT_BASE + 0x910)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_529_REG (IPP_KLT_BASE + 0x914)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_530_REG (IPP_KLT_BASE + 0x918)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_531_REG (IPP_KLT_BASE + 0x91C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_532_REG (IPP_KLT_BASE + 0x920)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_533_REG (IPP_KLT_BASE + 0x924)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_534_REG (IPP_KLT_BASE + 0x928)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_535_REG (IPP_KLT_BASE + 0x92C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_536_REG (IPP_KLT_BASE + 0x930)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_537_REG (IPP_KLT_BASE + 0x934)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_538_REG (IPP_KLT_BASE + 0x938)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_539_REG (IPP_KLT_BASE + 0x93C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_540_REG (IPP_KLT_BASE + 0x940)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_541_REG (IPP_KLT_BASE + 0x944)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_542_REG (IPP_KLT_BASE + 0x948)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_543_REG (IPP_KLT_BASE + 0x94C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_544_REG (IPP_KLT_BASE + 0x950)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_545_REG (IPP_KLT_BASE + 0x954)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_546_REG (IPP_KLT_BASE + 0x958)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_547_REG (IPP_KLT_BASE + 0x95C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_548_REG (IPP_KLT_BASE + 0x960)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_549_REG (IPP_KLT_BASE + 0x964)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_550_REG (IPP_KLT_BASE + 0x968)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_551_REG (IPP_KLT_BASE + 0x96C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_552_REG (IPP_KLT_BASE + 0x970)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_553_REG (IPP_KLT_BASE + 0x974)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_554_REG (IPP_KLT_BASE + 0x978)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_555_REG (IPP_KLT_BASE + 0x97C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_556_REG (IPP_KLT_BASE + 0x980)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_557_REG (IPP_KLT_BASE + 0x984)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_558_REG (IPP_KLT_BASE + 0x988)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_559_REG (IPP_KLT_BASE + 0x98C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_560_REG (IPP_KLT_BASE + 0x990)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_561_REG (IPP_KLT_BASE + 0x994)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_562_REG (IPP_KLT_BASE + 0x998)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_563_REG (IPP_KLT_BASE + 0x99C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_564_REG (IPP_KLT_BASE + 0x9A0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_565_REG (IPP_KLT_BASE + 0x9A4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_566_REG (IPP_KLT_BASE + 0x9A8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_567_REG (IPP_KLT_BASE + 0x9AC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_568_REG (IPP_KLT_BASE + 0x9B0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_569_REG (IPP_KLT_BASE + 0x9B4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_570_REG (IPP_KLT_BASE + 0x9B8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_571_REG (IPP_KLT_BASE + 0x9BC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_572_REG (IPP_KLT_BASE + 0x9C0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_573_REG (IPP_KLT_BASE + 0x9C4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_574_REG (IPP_KLT_BASE + 0x9C8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_575_REG (IPP_KLT_BASE + 0x9CC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_576_REG (IPP_KLT_BASE + 0x9D0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_577_REG (IPP_KLT_BASE + 0x9D4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_578_REG (IPP_KLT_BASE + 0x9D8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_579_REG (IPP_KLT_BASE + 0x9DC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_580_REG (IPP_KLT_BASE + 0x9E0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_581_REG (IPP_KLT_BASE + 0x9E4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_582_REG (IPP_KLT_BASE + 0x9E8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_583_REG (IPP_KLT_BASE + 0x9EC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_584_REG (IPP_KLT_BASE + 0x9F0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_585_REG (IPP_KLT_BASE + 0x9F4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_586_REG (IPP_KLT_BASE + 0x9F8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_587_REG (IPP_KLT_BASE + 0x9FC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_588_REG (IPP_KLT_BASE + 0xA00)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_589_REG (IPP_KLT_BASE + 0xA04)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_590_REG (IPP_KLT_BASE + 0xA08)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_591_REG (IPP_KLT_BASE + 0xA0C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_592_REG (IPP_KLT_BASE + 0xA10)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_593_REG (IPP_KLT_BASE + 0xA14)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_594_REG (IPP_KLT_BASE + 0xA18)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_595_REG (IPP_KLT_BASE + 0xA1C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_596_REG (IPP_KLT_BASE + 0xA20)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_597_REG (IPP_KLT_BASE + 0xA24)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_598_REG (IPP_KLT_BASE + 0xA28)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_599_REG (IPP_KLT_BASE + 0xA2C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_600_REG (IPP_KLT_BASE + 0xA30)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_601_REG (IPP_KLT_BASE + 0xA34)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_602_REG (IPP_KLT_BASE + 0xA38)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_603_REG (IPP_KLT_BASE + 0xA3C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_604_REG (IPP_KLT_BASE + 0xA40)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_605_REG (IPP_KLT_BASE + 0xA44)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_606_REG (IPP_KLT_BASE + 0xA48)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_607_REG (IPP_KLT_BASE + 0xA4C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_608_REG (IPP_KLT_BASE + 0xA50)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_609_REG (IPP_KLT_BASE + 0xA54)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_610_REG (IPP_KLT_BASE + 0xA58)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_611_REG (IPP_KLT_BASE + 0xA5C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_612_REG (IPP_KLT_BASE + 0xA60)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_613_REG (IPP_KLT_BASE + 0xA64)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_614_REG (IPP_KLT_BASE + 0xA68)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_615_REG (IPP_KLT_BASE + 0xA6C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_616_REG (IPP_KLT_BASE + 0xA70)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_617_REG (IPP_KLT_BASE + 0xA74)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_618_REG (IPP_KLT_BASE + 0xA78)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_619_REG (IPP_KLT_BASE + 0xA7C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_620_REG (IPP_KLT_BASE + 0xA80)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_621_REG (IPP_KLT_BASE + 0xA84)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_622_REG (IPP_KLT_BASE + 0xA88)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_623_REG (IPP_KLT_BASE + 0xA8C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_624_REG (IPP_KLT_BASE + 0xA90)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_625_REG (IPP_KLT_BASE + 0xA94)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_626_REG (IPP_KLT_BASE + 0xA98)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_627_REG (IPP_KLT_BASE + 0xA9C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_628_REG (IPP_KLT_BASE + 0xAA0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_629_REG (IPP_KLT_BASE + 0xAA4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_630_REG (IPP_KLT_BASE + 0xAA8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_631_REG (IPP_KLT_BASE + 0xAAC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_632_REG (IPP_KLT_BASE + 0xAB0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_633_REG (IPP_KLT_BASE + 0xAB4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_634_REG (IPP_KLT_BASE + 0xAB8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_635_REG (IPP_KLT_BASE + 0xABC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_636_REG (IPP_KLT_BASE + 0xAC0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_637_REG (IPP_KLT_BASE + 0xAC4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_638_REG (IPP_KLT_BASE + 0xAC8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_639_REG (IPP_KLT_BASE + 0xACC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_640_REG (IPP_KLT_BASE + 0xAD0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_641_REG (IPP_KLT_BASE + 0xAD4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_642_REG (IPP_KLT_BASE + 0xAD8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_643_REG (IPP_KLT_BASE + 0xADC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_644_REG (IPP_KLT_BASE + 0xAE0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_645_REG (IPP_KLT_BASE + 0xAE4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_646_REG (IPP_KLT_BASE + 0xAE8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_647_REG (IPP_KLT_BASE + 0xAEC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_648_REG (IPP_KLT_BASE + 0xAF0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_649_REG (IPP_KLT_BASE + 0xAF4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_650_REG (IPP_KLT_BASE + 0xAF8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_651_REG (IPP_KLT_BASE + 0xAFC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_652_REG (IPP_KLT_BASE + 0xB00)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_653_REG (IPP_KLT_BASE + 0xB04)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_654_REG (IPP_KLT_BASE + 0xB08)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_655_REG (IPP_KLT_BASE + 0xB0C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_656_REG (IPP_KLT_BASE + 0xB10)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_657_REG (IPP_KLT_BASE + 0xB14)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_658_REG (IPP_KLT_BASE + 0xB18)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_659_REG (IPP_KLT_BASE + 0xB1C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_660_REG (IPP_KLT_BASE + 0xB20)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_661_REG (IPP_KLT_BASE + 0xB24)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_662_REG (IPP_KLT_BASE + 0xB28)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_663_REG (IPP_KLT_BASE + 0xB2C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_664_REG (IPP_KLT_BASE + 0xB30)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_665_REG (IPP_KLT_BASE + 0xB34)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_666_REG (IPP_KLT_BASE + 0xB38)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_667_REG (IPP_KLT_BASE + 0xB3C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_668_REG (IPP_KLT_BASE + 0xB40)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_669_REG (IPP_KLT_BASE + 0xB44)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_670_REG (IPP_KLT_BASE + 0xB48)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_671_REG (IPP_KLT_BASE + 0xB4C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_672_REG (IPP_KLT_BASE + 0xB50)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_673_REG (IPP_KLT_BASE + 0xB54)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_674_REG (IPP_KLT_BASE + 0xB58)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_675_REG (IPP_KLT_BASE + 0xB5C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_676_REG (IPP_KLT_BASE + 0xB60)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_677_REG (IPP_KLT_BASE + 0xB64)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_678_REG (IPP_KLT_BASE + 0xB68)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_679_REG (IPP_KLT_BASE + 0xB6C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_680_REG (IPP_KLT_BASE + 0xB70)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_681_REG (IPP_KLT_BASE + 0xB74)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_682_REG (IPP_KLT_BASE + 0xB78)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_683_REG (IPP_KLT_BASE + 0xB7C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_684_REG (IPP_KLT_BASE + 0xB80)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_685_REG (IPP_KLT_BASE + 0xB84)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_686_REG (IPP_KLT_BASE + 0xB88)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_687_REG (IPP_KLT_BASE + 0xB8C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_688_REG (IPP_KLT_BASE + 0xB90)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_689_REG (IPP_KLT_BASE + 0xB94)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_690_REG (IPP_KLT_BASE + 0xB98)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_691_REG (IPP_KLT_BASE + 0xB9C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_692_REG (IPP_KLT_BASE + 0xBA0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_693_REG (IPP_KLT_BASE + 0xBA4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_694_REG (IPP_KLT_BASE + 0xBA8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_695_REG (IPP_KLT_BASE + 0xBAC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_696_REG (IPP_KLT_BASE + 0xBB0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_697_REG (IPP_KLT_BASE + 0xBB4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_698_REG (IPP_KLT_BASE + 0xBB8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_699_REG (IPP_KLT_BASE + 0xBBC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_700_REG (IPP_KLT_BASE + 0xBC0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_701_REG (IPP_KLT_BASE + 0xBC4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_702_REG (IPP_KLT_BASE + 0xBC8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_703_REG (IPP_KLT_BASE + 0xBCC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_704_REG (IPP_KLT_BASE + 0xBD0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_705_REG (IPP_KLT_BASE + 0xBD4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_706_REG (IPP_KLT_BASE + 0xBD8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_707_REG (IPP_KLT_BASE + 0xBDC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_708_REG (IPP_KLT_BASE + 0xBE0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_709_REG (IPP_KLT_BASE + 0xBE4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_710_REG (IPP_KLT_BASE + 0xBE8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_711_REG (IPP_KLT_BASE + 0xBEC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_712_REG (IPP_KLT_BASE + 0xBF0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_713_REG (IPP_KLT_BASE + 0xBF4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_714_REG (IPP_KLT_BASE + 0xBF8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_715_REG (IPP_KLT_BASE + 0xBFC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_716_REG (IPP_KLT_BASE + 0xC00)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_717_REG (IPP_KLT_BASE + 0xC04)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_718_REG (IPP_KLT_BASE + 0xC08)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_719_REG (IPP_KLT_BASE + 0xC0C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_720_REG (IPP_KLT_BASE + 0xC10)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_721_REG (IPP_KLT_BASE + 0xC14)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_722_REG (IPP_KLT_BASE + 0xC18)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_723_REG (IPP_KLT_BASE + 0xC1C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_724_REG (IPP_KLT_BASE + 0xC20)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_725_REG (IPP_KLT_BASE + 0xC24)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_726_REG (IPP_KLT_BASE + 0xC28)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_727_REG (IPP_KLT_BASE + 0xC2C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_728_REG (IPP_KLT_BASE + 0xC30)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_729_REG (IPP_KLT_BASE + 0xC34)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_730_REG (IPP_KLT_BASE + 0xC38)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_731_REG (IPP_KLT_BASE + 0xC3C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_732_REG (IPP_KLT_BASE + 0xC40)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_733_REG (IPP_KLT_BASE + 0xC44)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_734_REG (IPP_KLT_BASE + 0xC48)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_735_REG (IPP_KLT_BASE + 0xC4C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_736_REG (IPP_KLT_BASE + 0xC50)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_737_REG (IPP_KLT_BASE + 0xC54)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_738_REG (IPP_KLT_BASE + 0xC58)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_739_REG (IPP_KLT_BASE + 0xC5C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_740_REG (IPP_KLT_BASE + 0xC60)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_741_REG (IPP_KLT_BASE + 0xC64)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_742_REG (IPP_KLT_BASE + 0xC68)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_743_REG (IPP_KLT_BASE + 0xC6C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_744_REG (IPP_KLT_BASE + 0xC70)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_745_REG (IPP_KLT_BASE + 0xC74)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_746_REG (IPP_KLT_BASE + 0xC78)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_747_REG (IPP_KLT_BASE + 0xC7C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_748_REG (IPP_KLT_BASE + 0xC80)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_749_REG (IPP_KLT_BASE + 0xC84)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_750_REG (IPP_KLT_BASE + 0xC88)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_751_REG (IPP_KLT_BASE + 0xC8C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_752_REG (IPP_KLT_BASE + 0xC90)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_753_REG (IPP_KLT_BASE + 0xC94)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_754_REG (IPP_KLT_BASE + 0xC98)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_755_REG (IPP_KLT_BASE + 0xC9C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_756_REG (IPP_KLT_BASE + 0xCA0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_757_REG (IPP_KLT_BASE + 0xCA4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_758_REG (IPP_KLT_BASE + 0xCA8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_759_REG (IPP_KLT_BASE + 0xCAC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_760_REG (IPP_KLT_BASE + 0xCB0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_761_REG (IPP_KLT_BASE + 0xCB4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_762_REG (IPP_KLT_BASE + 0xCB8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_763_REG (IPP_KLT_BASE + 0xCBC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_764_REG (IPP_KLT_BASE + 0xCC0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_765_REG (IPP_KLT_BASE + 0xCC4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_766_REG (IPP_KLT_BASE + 0xCC8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_767_REG (IPP_KLT_BASE + 0xCCC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_768_REG (IPP_KLT_BASE + 0xCD0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_769_REG (IPP_KLT_BASE + 0xCD4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_770_REG (IPP_KLT_BASE + 0xCD8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_771_REG (IPP_KLT_BASE + 0xCDC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_772_REG (IPP_KLT_BASE + 0xCE0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_773_REG (IPP_KLT_BASE + 0xCE4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_774_REG (IPP_KLT_BASE + 0xCE8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_775_REG (IPP_KLT_BASE + 0xCEC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_776_REG (IPP_KLT_BASE + 0xCF0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_777_REG (IPP_KLT_BASE + 0xCF4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_778_REG (IPP_KLT_BASE + 0xCF8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_779_REG (IPP_KLT_BASE + 0xCFC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_780_REG (IPP_KLT_BASE + 0xD00)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_781_REG (IPP_KLT_BASE + 0xD04)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_782_REG (IPP_KLT_BASE + 0xD08)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_783_REG (IPP_KLT_BASE + 0xD0C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_784_REG (IPP_KLT_BASE + 0xD10)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_785_REG (IPP_KLT_BASE + 0xD14)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_786_REG (IPP_KLT_BASE + 0xD18)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_787_REG (IPP_KLT_BASE + 0xD1C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_788_REG (IPP_KLT_BASE + 0xD20)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_789_REG (IPP_KLT_BASE + 0xD24)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_790_REG (IPP_KLT_BASE + 0xD28)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_791_REG (IPP_KLT_BASE + 0xD2C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_792_REG (IPP_KLT_BASE + 0xD30)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_793_REG (IPP_KLT_BASE + 0xD34)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_794_REG (IPP_KLT_BASE + 0xD38)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_795_REG (IPP_KLT_BASE + 0xD3C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_796_REG (IPP_KLT_BASE + 0xD40)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_797_REG (IPP_KLT_BASE + 0xD44)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_798_REG (IPP_KLT_BASE + 0xD48)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_799_REG (IPP_KLT_BASE + 0xD4C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_800_REG (IPP_KLT_BASE + 0xD50)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_801_REG (IPP_KLT_BASE + 0xD54)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_802_REG (IPP_KLT_BASE + 0xD58)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_803_REG (IPP_KLT_BASE + 0xD5C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_804_REG (IPP_KLT_BASE + 0xD60)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_805_REG (IPP_KLT_BASE + 0xD64)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_806_REG (IPP_KLT_BASE + 0xD68)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_807_REG (IPP_KLT_BASE + 0xD6C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_808_REG (IPP_KLT_BASE + 0xD70)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_809_REG (IPP_KLT_BASE + 0xD74)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_810_REG (IPP_KLT_BASE + 0xD78)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_811_REG (IPP_KLT_BASE + 0xD7C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_812_REG (IPP_KLT_BASE + 0xD80)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_813_REG (IPP_KLT_BASE + 0xD84)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_814_REG (IPP_KLT_BASE + 0xD88)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_815_REG (IPP_KLT_BASE + 0xD8C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_816_REG (IPP_KLT_BASE + 0xD90)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_817_REG (IPP_KLT_BASE + 0xD94)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_818_REG (IPP_KLT_BASE + 0xD98)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_819_REG (IPP_KLT_BASE + 0xD9C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_820_REG (IPP_KLT_BASE + 0xDA0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_821_REG (IPP_KLT_BASE + 0xDA4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_822_REG (IPP_KLT_BASE + 0xDA8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_823_REG (IPP_KLT_BASE + 0xDAC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_824_REG (IPP_KLT_BASE + 0xDB0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_825_REG (IPP_KLT_BASE + 0xDB4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_826_REG (IPP_KLT_BASE + 0xDB8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_827_REG (IPP_KLT_BASE + 0xDBC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_828_REG (IPP_KLT_BASE + 0xDC0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_829_REG (IPP_KLT_BASE + 0xDC4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_830_REG (IPP_KLT_BASE + 0xDC8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_831_REG (IPP_KLT_BASE + 0xDCC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_832_REG (IPP_KLT_BASE + 0xDD0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_833_REG (IPP_KLT_BASE + 0xDD4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_834_REG (IPP_KLT_BASE + 0xDD8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_835_REG (IPP_KLT_BASE + 0xDDC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_836_REG (IPP_KLT_BASE + 0xDE0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_837_REG (IPP_KLT_BASE + 0xDE4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_838_REG (IPP_KLT_BASE + 0xDE8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_839_REG (IPP_KLT_BASE + 0xDEC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_840_REG (IPP_KLT_BASE + 0xDF0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_841_REG (IPP_KLT_BASE + 0xDF4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_842_REG (IPP_KLT_BASE + 0xDF8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_843_REG (IPP_KLT_BASE + 0xDFC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_844_REG (IPP_KLT_BASE + 0xE00)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_845_REG (IPP_KLT_BASE + 0xE04)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_846_REG (IPP_KLT_BASE + 0xE08)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_847_REG (IPP_KLT_BASE + 0xE0C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_848_REG (IPP_KLT_BASE + 0xE10)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_849_REG (IPP_KLT_BASE + 0xE14)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_850_REG (IPP_KLT_BASE + 0xE18)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_851_REG (IPP_KLT_BASE + 0xE1C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_852_REG (IPP_KLT_BASE + 0xE20)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_853_REG (IPP_KLT_BASE + 0xE24)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_854_REG (IPP_KLT_BASE + 0xE28)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_855_REG (IPP_KLT_BASE + 0xE2C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_856_REG (IPP_KLT_BASE + 0xE30)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_857_REG (IPP_KLT_BASE + 0xE34)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_858_REG (IPP_KLT_BASE + 0xE38)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_859_REG (IPP_KLT_BASE + 0xE3C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_860_REG (IPP_KLT_BASE + 0xE40)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_861_REG (IPP_KLT_BASE + 0xE44)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_862_REG (IPP_KLT_BASE + 0xE48)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_863_REG (IPP_KLT_BASE + 0xE4C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_864_REG (IPP_KLT_BASE + 0xE50)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_865_REG (IPP_KLT_BASE + 0xE54)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_866_REG (IPP_KLT_BASE + 0xE58)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_867_REG (IPP_KLT_BASE + 0xE5C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_868_REG (IPP_KLT_BASE + 0xE60)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_869_REG (IPP_KLT_BASE + 0xE64)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_870_REG (IPP_KLT_BASE + 0xE68)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_871_REG (IPP_KLT_BASE + 0xE6C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_872_REG (IPP_KLT_BASE + 0xE70)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_873_REG (IPP_KLT_BASE + 0xE74)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_874_REG (IPP_KLT_BASE + 0xE78)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_875_REG (IPP_KLT_BASE + 0xE7C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_876_REG (IPP_KLT_BASE + 0xE80)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_877_REG (IPP_KLT_BASE + 0xE84)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_878_REG (IPP_KLT_BASE + 0xE88)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_879_REG (IPP_KLT_BASE + 0xE8C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_880_REG (IPP_KLT_BASE + 0xE90)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_881_REG (IPP_KLT_BASE + 0xE94)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_882_REG (IPP_KLT_BASE + 0xE98)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_883_REG (IPP_KLT_BASE + 0xE9C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_884_REG (IPP_KLT_BASE + 0xEA0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_885_REG (IPP_KLT_BASE + 0xEA4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_886_REG (IPP_KLT_BASE + 0xEA8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_887_REG (IPP_KLT_BASE + 0xEAC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_888_REG (IPP_KLT_BASE + 0xEB0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_889_REG (IPP_KLT_BASE + 0xEB4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_890_REG (IPP_KLT_BASE + 0xEB8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_891_REG (IPP_KLT_BASE + 0xEBC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_892_REG (IPP_KLT_BASE + 0xEC0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_893_REG (IPP_KLT_BASE + 0xEC4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_894_REG (IPP_KLT_BASE + 0xEC8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_895_REG (IPP_KLT_BASE + 0xECC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_896_REG (IPP_KLT_BASE + 0xED0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_897_REG (IPP_KLT_BASE + 0xED4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_898_REG (IPP_KLT_BASE + 0xED8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_899_REG (IPP_KLT_BASE + 0xEDC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_900_REG (IPP_KLT_BASE + 0xEE0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_901_REG (IPP_KLT_BASE + 0xEE4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_902_REG (IPP_KLT_BASE + 0xEE8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_903_REG (IPP_KLT_BASE + 0xEEC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_904_REG (IPP_KLT_BASE + 0xEF0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_905_REG (IPP_KLT_BASE + 0xEF4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_906_REG (IPP_KLT_BASE + 0xEF8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_907_REG (IPP_KLT_BASE + 0xEFC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_908_REG (IPP_KLT_BASE + 0xF00)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_909_REG (IPP_KLT_BASE + 0xF04)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_910_REG (IPP_KLT_BASE + 0xF08)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_911_REG (IPP_KLT_BASE + 0xF0C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_912_REG (IPP_KLT_BASE + 0xF10)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_913_REG (IPP_KLT_BASE + 0xF14)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_914_REG (IPP_KLT_BASE + 0xF18)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_915_REG (IPP_KLT_BASE + 0xF1C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_916_REG (IPP_KLT_BASE + 0xF20)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_917_REG (IPP_KLT_BASE + 0xF24)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_918_REG (IPP_KLT_BASE + 0xF28)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_919_REG (IPP_KLT_BASE + 0xF2C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_920_REG (IPP_KLT_BASE + 0xF30)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_921_REG (IPP_KLT_BASE + 0xF34)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_922_REG (IPP_KLT_BASE + 0xF38)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_923_REG (IPP_KLT_BASE + 0xF3C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_924_REG (IPP_KLT_BASE + 0xF40)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_925_REG (IPP_KLT_BASE + 0xF44)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_926_REG (IPP_KLT_BASE + 0xF48)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_927_REG (IPP_KLT_BASE + 0xF4C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_928_REG (IPP_KLT_BASE + 0xF50)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_929_REG (IPP_KLT_BASE + 0xF54)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_930_REG (IPP_KLT_BASE + 0xF58)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_931_REG (IPP_KLT_BASE + 0xF5C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_932_REG (IPP_KLT_BASE + 0xF60)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_933_REG (IPP_KLT_BASE + 0xF64)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_934_REG (IPP_KLT_BASE + 0xF68)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_935_REG (IPP_KLT_BASE + 0xF6C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_936_REG (IPP_KLT_BASE + 0xF70)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_937_REG (IPP_KLT_BASE + 0xF74)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_938_REG (IPP_KLT_BASE + 0xF78)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_939_REG (IPP_KLT_BASE + 0xF7C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_940_REG (IPP_KLT_BASE + 0xF80)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_941_REG (IPP_KLT_BASE + 0xF84)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_942_REG (IPP_KLT_BASE + 0xF88)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_943_REG (IPP_KLT_BASE + 0xF8C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_944_REG (IPP_KLT_BASE + 0xF90)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_945_REG (IPP_KLT_BASE + 0xF94)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_946_REG (IPP_KLT_BASE + 0xF98)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_947_REG (IPP_KLT_BASE + 0xF9C)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_948_REG (IPP_KLT_BASE + 0xFA0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_949_REG (IPP_KLT_BASE + 0xFA4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_950_REG (IPP_KLT_BASE + 0xFA8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_951_REG (IPP_KLT_BASE + 0xFAC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_952_REG (IPP_KLT_BASE + 0xFB0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_953_REG (IPP_KLT_BASE + 0xFB4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_954_REG (IPP_KLT_BASE + 0xFB8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_955_REG (IPP_KLT_BASE + 0xFBC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_956_REG (IPP_KLT_BASE + 0xFC0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_957_REG (IPP_KLT_BASE + 0xFC4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_958_REG (IPP_KLT_BASE + 0xFC8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_959_REG (IPP_KLT_BASE + 0xFCC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_960_REG (IPP_KLT_BASE + 0xFD0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_961_REG (IPP_KLT_BASE + 0xFD4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_962_REG (IPP_KLT_BASE + 0xFD8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_963_REG (IPP_KLT_BASE + 0xFDC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_964_REG (IPP_KLT_BASE + 0xFE0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_965_REG (IPP_KLT_BASE + 0xFE4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_966_REG (IPP_KLT_BASE + 0xFE8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_967_REG (IPP_KLT_BASE + 0xFEC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_968_REG (IPP_KLT_BASE + 0xFF0)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_969_REG (IPP_KLT_BASE + 0xFF4)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_970_REG (IPP_KLT_BASE + 0xFF8)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_971_REG (IPP_KLT_BASE + 0xFFC)   /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_972_REG (IPP_KLT_BASE + 0x1000)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_973_REG (IPP_KLT_BASE + 0x1004)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_974_REG (IPP_KLT_BASE + 0x1008)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_975_REG (IPP_KLT_BASE + 0x100C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_976_REG (IPP_KLT_BASE + 0x1010)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_977_REG (IPP_KLT_BASE + 0x1014)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_978_REG (IPP_KLT_BASE + 0x1018)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_979_REG (IPP_KLT_BASE + 0x101C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_980_REG (IPP_KLT_BASE + 0x1020)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_981_REG (IPP_KLT_BASE + 0x1024)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_982_REG (IPP_KLT_BASE + 0x1028)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_983_REG (IPP_KLT_BASE + 0x102C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_984_REG (IPP_KLT_BASE + 0x1030)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_985_REG (IPP_KLT_BASE + 0x1034)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_986_REG (IPP_KLT_BASE + 0x1038)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_987_REG (IPP_KLT_BASE + 0x103C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_988_REG (IPP_KLT_BASE + 0x1040)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_989_REG (IPP_KLT_BASE + 0x1044)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_990_REG (IPP_KLT_BASE + 0x1048)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_991_REG (IPP_KLT_BASE + 0x104C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_992_REG (IPP_KLT_BASE + 0x1050)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_993_REG (IPP_KLT_BASE + 0x1054)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_994_REG (IPP_KLT_BASE + 0x1058)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_995_REG (IPP_KLT_BASE + 0x105C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_996_REG (IPP_KLT_BASE + 0x1060)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_997_REG (IPP_KLT_BASE + 0x1064)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_998_REG (IPP_KLT_BASE + 0x1068)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_999_REG (IPP_KLT_BASE + 0x106C)  /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1000_REG (IPP_KLT_BASE + 0x1070) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1001_REG (IPP_KLT_BASE + 0x1074) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1002_REG (IPP_KLT_BASE + 0x1078) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1003_REG (IPP_KLT_BASE + 0x107C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1004_REG (IPP_KLT_BASE + 0x1080) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1005_REG (IPP_KLT_BASE + 0x1084) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1006_REG (IPP_KLT_BASE + 0x1088) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1007_REG (IPP_KLT_BASE + 0x108C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1008_REG (IPP_KLT_BASE + 0x1090) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1009_REG (IPP_KLT_BASE + 0x1094) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1010_REG (IPP_KLT_BASE + 0x1098) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1011_REG (IPP_KLT_BASE + 0x109C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1012_REG (IPP_KLT_BASE + 0x10A0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1013_REG (IPP_KLT_BASE + 0x10A4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1014_REG (IPP_KLT_BASE + 0x10A8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1015_REG (IPP_KLT_BASE + 0x10AC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1016_REG (IPP_KLT_BASE + 0x10B0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1017_REG (IPP_KLT_BASE + 0x10B4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1018_REG (IPP_KLT_BASE + 0x10B8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1019_REG (IPP_KLT_BASE + 0x10BC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1020_REG (IPP_KLT_BASE + 0x10C0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1021_REG (IPP_KLT_BASE + 0x10C4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1022_REG (IPP_KLT_BASE + 0x10C8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1023_REG (IPP_KLT_BASE + 0x10CC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1024_REG (IPP_KLT_BASE + 0x10D0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1025_REG (IPP_KLT_BASE + 0x10D4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1026_REG (IPP_KLT_BASE + 0x10D8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1027_REG (IPP_KLT_BASE + 0x10DC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1028_REG (IPP_KLT_BASE + 0x10E0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1029_REG (IPP_KLT_BASE + 0x10E4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1030_REG (IPP_KLT_BASE + 0x10E8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1031_REG (IPP_KLT_BASE + 0x10EC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1032_REG (IPP_KLT_BASE + 0x10F0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1033_REG (IPP_KLT_BASE + 0x10F4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1034_REG (IPP_KLT_BASE + 0x10F8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1035_REG (IPP_KLT_BASE + 0x10FC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1036_REG (IPP_KLT_BASE + 0x1100) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1037_REG (IPP_KLT_BASE + 0x1104) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1038_REG (IPP_KLT_BASE + 0x1108) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1039_REG (IPP_KLT_BASE + 0x110C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1040_REG (IPP_KLT_BASE + 0x1110) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1041_REG (IPP_KLT_BASE + 0x1114) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1042_REG (IPP_KLT_BASE + 0x1118) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1043_REG (IPP_KLT_BASE + 0x111C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1044_REG (IPP_KLT_BASE + 0x1120) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1045_REG (IPP_KLT_BASE + 0x1124) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1046_REG (IPP_KLT_BASE + 0x1128) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1047_REG (IPP_KLT_BASE + 0x112C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1048_REG (IPP_KLT_BASE + 0x1130) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1049_REG (IPP_KLT_BASE + 0x1134) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1050_REG (IPP_KLT_BASE + 0x1138) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1051_REG (IPP_KLT_BASE + 0x113C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1052_REG (IPP_KLT_BASE + 0x1140) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1053_REG (IPP_KLT_BASE + 0x1144) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1054_REG (IPP_KLT_BASE + 0x1148) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1055_REG (IPP_KLT_BASE + 0x114C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1056_REG (IPP_KLT_BASE + 0x1150) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1057_REG (IPP_KLT_BASE + 0x1154) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1058_REG (IPP_KLT_BASE + 0x1158) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1059_REG (IPP_KLT_BASE + 0x115C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1060_REG (IPP_KLT_BASE + 0x1160) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1061_REG (IPP_KLT_BASE + 0x1164) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1062_REG (IPP_KLT_BASE + 0x1168) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1063_REG (IPP_KLT_BASE + 0x116C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1064_REG (IPP_KLT_BASE + 0x1170) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1065_REG (IPP_KLT_BASE + 0x1174) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1066_REG (IPP_KLT_BASE + 0x1178) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1067_REG (IPP_KLT_BASE + 0x117C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1068_REG (IPP_KLT_BASE + 0x1180) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1069_REG (IPP_KLT_BASE + 0x1184) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1070_REG (IPP_KLT_BASE + 0x1188) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1071_REG (IPP_KLT_BASE + 0x118C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1072_REG (IPP_KLT_BASE + 0x1190) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1073_REG (IPP_KLT_BASE + 0x1194) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1074_REG (IPP_KLT_BASE + 0x1198) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1075_REG (IPP_KLT_BASE + 0x119C) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1076_REG (IPP_KLT_BASE + 0x11A0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1077_REG (IPP_KLT_BASE + 0x11A4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1078_REG (IPP_KLT_BASE + 0x11A8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1079_REG (IPP_KLT_BASE + 0x11AC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1080_REG (IPP_KLT_BASE + 0x11B0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1081_REG (IPP_KLT_BASE + 0x11B4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1082_REG (IPP_KLT_BASE + 0x11B8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1083_REG (IPP_KLT_BASE + 0x11BC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1084_REG (IPP_KLT_BASE + 0x11C0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1085_REG (IPP_KLT_BASE + 0x11C4) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1086_REG (IPP_KLT_BASE + 0x11C8) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1087_REG (IPP_KLT_BASE + 0x11CC) /* Displacement of nodes of Cgrid */
#define IPP_KLT_GYRO_GRID_DISPL_1088_REG (IPP_KLT_BASE + 0x11D0) /* Displacement of nodes of Cgrid */
#define IPP_KLT_KP_NUMBER_REG (IPP_KLT_BASE + 0x11D4)            /* Number of KP */
#define IPP_KLT_KP_PREV_BUFF_X_0_REG (IPP_KLT_BASE + 0x11DC)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_1_REG (IPP_KLT_BASE + 0x11E4)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_2_REG (IPP_KLT_BASE + 0x11EC)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_3_REG (IPP_KLT_BASE + 0x11F4)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_4_REG (IPP_KLT_BASE + 0x11FC)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_5_REG (IPP_KLT_BASE + 0x1204)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_6_REG (IPP_KLT_BASE + 0x120C)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_7_REG (IPP_KLT_BASE + 0x1214)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_8_REG (IPP_KLT_BASE + 0x121C)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_9_REG (IPP_KLT_BASE + 0x1224)     /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_10_REG (IPP_KLT_BASE + 0x122C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_11_REG (IPP_KLT_BASE + 0x1234)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_12_REG (IPP_KLT_BASE + 0x123C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_13_REG (IPP_KLT_BASE + 0x1244)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_14_REG (IPP_KLT_BASE + 0x124C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_15_REG (IPP_KLT_BASE + 0x1254)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_16_REG (IPP_KLT_BASE + 0x125C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_17_REG (IPP_KLT_BASE + 0x1264)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_18_REG (IPP_KLT_BASE + 0x126C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_19_REG (IPP_KLT_BASE + 0x1274)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_20_REG (IPP_KLT_BASE + 0x127C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_21_REG (IPP_KLT_BASE + 0x1284)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_22_REG (IPP_KLT_BASE + 0x128C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_23_REG (IPP_KLT_BASE + 0x1294)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_24_REG (IPP_KLT_BASE + 0x129C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_25_REG (IPP_KLT_BASE + 0x12A4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_26_REG (IPP_KLT_BASE + 0x12AC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_27_REG (IPP_KLT_BASE + 0x12B4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_28_REG (IPP_KLT_BASE + 0x12BC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_29_REG (IPP_KLT_BASE + 0x12C4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_30_REG (IPP_KLT_BASE + 0x12CC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_31_REG (IPP_KLT_BASE + 0x12D4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_32_REG (IPP_KLT_BASE + 0x12DC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_33_REG (IPP_KLT_BASE + 0x12E4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_34_REG (IPP_KLT_BASE + 0x12EC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_35_REG (IPP_KLT_BASE + 0x12F4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_36_REG (IPP_KLT_BASE + 0x12FC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_37_REG (IPP_KLT_BASE + 0x1304)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_38_REG (IPP_KLT_BASE + 0x130C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_39_REG (IPP_KLT_BASE + 0x1314)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_40_REG (IPP_KLT_BASE + 0x131C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_41_REG (IPP_KLT_BASE + 0x1324)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_42_REG (IPP_KLT_BASE + 0x132C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_43_REG (IPP_KLT_BASE + 0x1334)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_44_REG (IPP_KLT_BASE + 0x133C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_45_REG (IPP_KLT_BASE + 0x1344)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_46_REG (IPP_KLT_BASE + 0x134C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_47_REG (IPP_KLT_BASE + 0x1354)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_48_REG (IPP_KLT_BASE + 0x135C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_49_REG (IPP_KLT_BASE + 0x1364)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_50_REG (IPP_KLT_BASE + 0x136C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_51_REG (IPP_KLT_BASE + 0x1374)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_52_REG (IPP_KLT_BASE + 0x137C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_53_REG (IPP_KLT_BASE + 0x1384)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_54_REG (IPP_KLT_BASE + 0x138C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_55_REG (IPP_KLT_BASE + 0x1394)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_56_REG (IPP_KLT_BASE + 0x139C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_57_REG (IPP_KLT_BASE + 0x13A4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_58_REG (IPP_KLT_BASE + 0x13AC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_59_REG (IPP_KLT_BASE + 0x13B4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_60_REG (IPP_KLT_BASE + 0x13BC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_61_REG (IPP_KLT_BASE + 0x13C4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_62_REG (IPP_KLT_BASE + 0x13CC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_63_REG (IPP_KLT_BASE + 0x13D4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_64_REG (IPP_KLT_BASE + 0x13DC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_65_REG (IPP_KLT_BASE + 0x13E4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_66_REG (IPP_KLT_BASE + 0x13EC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_67_REG (IPP_KLT_BASE + 0x13F4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_68_REG (IPP_KLT_BASE + 0x13FC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_69_REG (IPP_KLT_BASE + 0x1404)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_70_REG (IPP_KLT_BASE + 0x140C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_71_REG (IPP_KLT_BASE + 0x1414)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_72_REG (IPP_KLT_BASE + 0x141C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_73_REG (IPP_KLT_BASE + 0x1424)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_74_REG (IPP_KLT_BASE + 0x142C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_75_REG (IPP_KLT_BASE + 0x1434)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_76_REG (IPP_KLT_BASE + 0x143C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_77_REG (IPP_KLT_BASE + 0x1444)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_78_REG (IPP_KLT_BASE + 0x144C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_79_REG (IPP_KLT_BASE + 0x1454)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_80_REG (IPP_KLT_BASE + 0x145C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_81_REG (IPP_KLT_BASE + 0x1464)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_82_REG (IPP_KLT_BASE + 0x146C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_83_REG (IPP_KLT_BASE + 0x1474)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_84_REG (IPP_KLT_BASE + 0x147C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_85_REG (IPP_KLT_BASE + 0x1484)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_86_REG (IPP_KLT_BASE + 0x148C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_87_REG (IPP_KLT_BASE + 0x1494)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_88_REG (IPP_KLT_BASE + 0x149C)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_89_REG (IPP_KLT_BASE + 0x14A4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_90_REG (IPP_KLT_BASE + 0x14AC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_91_REG (IPP_KLT_BASE + 0x14B4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_92_REG (IPP_KLT_BASE + 0x14BC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_93_REG (IPP_KLT_BASE + 0x14C4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_94_REG (IPP_KLT_BASE + 0x14CC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_95_REG (IPP_KLT_BASE + 0x14D4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_96_REG (IPP_KLT_BASE + 0x14DC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_97_REG (IPP_KLT_BASE + 0x14E4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_98_REG (IPP_KLT_BASE + 0x14EC)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_99_REG (IPP_KLT_BASE + 0x14F4)    /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_100_REG (IPP_KLT_BASE + 0x14FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_101_REG (IPP_KLT_BASE + 0x1504)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_102_REG (IPP_KLT_BASE + 0x150C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_103_REG (IPP_KLT_BASE + 0x1514)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_104_REG (IPP_KLT_BASE + 0x151C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_105_REG (IPP_KLT_BASE + 0x1524)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_106_REG (IPP_KLT_BASE + 0x152C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_107_REG (IPP_KLT_BASE + 0x1534)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_108_REG (IPP_KLT_BASE + 0x153C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_109_REG (IPP_KLT_BASE + 0x1544)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_110_REG (IPP_KLT_BASE + 0x154C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_111_REG (IPP_KLT_BASE + 0x1554)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_112_REG (IPP_KLT_BASE + 0x155C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_113_REG (IPP_KLT_BASE + 0x1564)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_114_REG (IPP_KLT_BASE + 0x156C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_115_REG (IPP_KLT_BASE + 0x1574)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_116_REG (IPP_KLT_BASE + 0x157C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_117_REG (IPP_KLT_BASE + 0x1584)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_118_REG (IPP_KLT_BASE + 0x158C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_119_REG (IPP_KLT_BASE + 0x1594)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_120_REG (IPP_KLT_BASE + 0x159C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_121_REG (IPP_KLT_BASE + 0x15A4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_122_REG (IPP_KLT_BASE + 0x15AC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_123_REG (IPP_KLT_BASE + 0x15B4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_124_REG (IPP_KLT_BASE + 0x15BC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_125_REG (IPP_KLT_BASE + 0x15C4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_126_REG (IPP_KLT_BASE + 0x15CC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_127_REG (IPP_KLT_BASE + 0x15D4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_128_REG (IPP_KLT_BASE + 0x15DC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_129_REG (IPP_KLT_BASE + 0x15E4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_130_REG (IPP_KLT_BASE + 0x15EC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_131_REG (IPP_KLT_BASE + 0x15F4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_132_REG (IPP_KLT_BASE + 0x15FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_133_REG (IPP_KLT_BASE + 0x1604)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_134_REG (IPP_KLT_BASE + 0x160C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_135_REG (IPP_KLT_BASE + 0x1614)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_136_REG (IPP_KLT_BASE + 0x161C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_137_REG (IPP_KLT_BASE + 0x1624)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_138_REG (IPP_KLT_BASE + 0x162C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_139_REG (IPP_KLT_BASE + 0x1634)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_140_REG (IPP_KLT_BASE + 0x163C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_141_REG (IPP_KLT_BASE + 0x1644)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_142_REG (IPP_KLT_BASE + 0x164C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_143_REG (IPP_KLT_BASE + 0x1654)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_144_REG (IPP_KLT_BASE + 0x165C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_145_REG (IPP_KLT_BASE + 0x1664)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_146_REG (IPP_KLT_BASE + 0x166C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_147_REG (IPP_KLT_BASE + 0x1674)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_148_REG (IPP_KLT_BASE + 0x167C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_149_REG (IPP_KLT_BASE + 0x1684)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_150_REG (IPP_KLT_BASE + 0x168C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_151_REG (IPP_KLT_BASE + 0x1694)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_152_REG (IPP_KLT_BASE + 0x169C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_153_REG (IPP_KLT_BASE + 0x16A4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_154_REG (IPP_KLT_BASE + 0x16AC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_155_REG (IPP_KLT_BASE + 0x16B4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_156_REG (IPP_KLT_BASE + 0x16BC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_157_REG (IPP_KLT_BASE + 0x16C4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_158_REG (IPP_KLT_BASE + 0x16CC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_159_REG (IPP_KLT_BASE + 0x16D4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_160_REG (IPP_KLT_BASE + 0x16DC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_161_REG (IPP_KLT_BASE + 0x16E4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_162_REG (IPP_KLT_BASE + 0x16EC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_163_REG (IPP_KLT_BASE + 0x16F4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_164_REG (IPP_KLT_BASE + 0x16FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_165_REG (IPP_KLT_BASE + 0x1704)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_166_REG (IPP_KLT_BASE + 0x170C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_167_REG (IPP_KLT_BASE + 0x1714)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_168_REG (IPP_KLT_BASE + 0x171C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_169_REG (IPP_KLT_BASE + 0x1724)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_170_REG (IPP_KLT_BASE + 0x172C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_171_REG (IPP_KLT_BASE + 0x1734)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_172_REG (IPP_KLT_BASE + 0x173C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_173_REG (IPP_KLT_BASE + 0x1744)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_174_REG (IPP_KLT_BASE + 0x174C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_175_REG (IPP_KLT_BASE + 0x1754)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_176_REG (IPP_KLT_BASE + 0x175C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_177_REG (IPP_KLT_BASE + 0x1764)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_178_REG (IPP_KLT_BASE + 0x176C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_179_REG (IPP_KLT_BASE + 0x1774)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_180_REG (IPP_KLT_BASE + 0x177C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_181_REG (IPP_KLT_BASE + 0x1784)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_182_REG (IPP_KLT_BASE + 0x178C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_183_REG (IPP_KLT_BASE + 0x1794)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_184_REG (IPP_KLT_BASE + 0x179C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_185_REG (IPP_KLT_BASE + 0x17A4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_186_REG (IPP_KLT_BASE + 0x17AC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_187_REG (IPP_KLT_BASE + 0x17B4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_188_REG (IPP_KLT_BASE + 0x17BC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_189_REG (IPP_KLT_BASE + 0x17C4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_190_REG (IPP_KLT_BASE + 0x17CC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_191_REG (IPP_KLT_BASE + 0x17D4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_192_REG (IPP_KLT_BASE + 0x17DC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_193_REG (IPP_KLT_BASE + 0x17E4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_194_REG (IPP_KLT_BASE + 0x17EC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_195_REG (IPP_KLT_BASE + 0x17F4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_196_REG (IPP_KLT_BASE + 0x17FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_197_REG (IPP_KLT_BASE + 0x1804)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_198_REG (IPP_KLT_BASE + 0x180C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_199_REG (IPP_KLT_BASE + 0x1814)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_200_REG (IPP_KLT_BASE + 0x181C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_201_REG (IPP_KLT_BASE + 0x1824)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_202_REG (IPP_KLT_BASE + 0x182C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_203_REG (IPP_KLT_BASE + 0x1834)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_204_REG (IPP_KLT_BASE + 0x183C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_205_REG (IPP_KLT_BASE + 0x1844)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_206_REG (IPP_KLT_BASE + 0x184C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_207_REG (IPP_KLT_BASE + 0x1854)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_208_REG (IPP_KLT_BASE + 0x185C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_209_REG (IPP_KLT_BASE + 0x1864)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_210_REG (IPP_KLT_BASE + 0x186C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_211_REG (IPP_KLT_BASE + 0x1874)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_212_REG (IPP_KLT_BASE + 0x187C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_213_REG (IPP_KLT_BASE + 0x1884)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_214_REG (IPP_KLT_BASE + 0x188C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_215_REG (IPP_KLT_BASE + 0x1894)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_216_REG (IPP_KLT_BASE + 0x189C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_217_REG (IPP_KLT_BASE + 0x18A4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_218_REG (IPP_KLT_BASE + 0x18AC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_219_REG (IPP_KLT_BASE + 0x18B4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_220_REG (IPP_KLT_BASE + 0x18BC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_221_REG (IPP_KLT_BASE + 0x18C4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_222_REG (IPP_KLT_BASE + 0x18CC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_223_REG (IPP_KLT_BASE + 0x18D4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_224_REG (IPP_KLT_BASE + 0x18DC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_225_REG (IPP_KLT_BASE + 0x18E4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_226_REG (IPP_KLT_BASE + 0x18EC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_227_REG (IPP_KLT_BASE + 0x18F4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_228_REG (IPP_KLT_BASE + 0x18FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_229_REG (IPP_KLT_BASE + 0x1904)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_230_REG (IPP_KLT_BASE + 0x190C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_231_REG (IPP_KLT_BASE + 0x1914)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_232_REG (IPP_KLT_BASE + 0x191C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_233_REG (IPP_KLT_BASE + 0x1924)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_234_REG (IPP_KLT_BASE + 0x192C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_235_REG (IPP_KLT_BASE + 0x1934)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_236_REG (IPP_KLT_BASE + 0x193C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_237_REG (IPP_KLT_BASE + 0x1944)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_238_REG (IPP_KLT_BASE + 0x194C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_239_REG (IPP_KLT_BASE + 0x1954)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_240_REG (IPP_KLT_BASE + 0x195C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_241_REG (IPP_KLT_BASE + 0x1964)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_242_REG (IPP_KLT_BASE + 0x196C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_243_REG (IPP_KLT_BASE + 0x1974)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_244_REG (IPP_KLT_BASE + 0x197C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_245_REG (IPP_KLT_BASE + 0x1984)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_246_REG (IPP_KLT_BASE + 0x198C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_247_REG (IPP_KLT_BASE + 0x1994)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_248_REG (IPP_KLT_BASE + 0x199C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_249_REG (IPP_KLT_BASE + 0x19A4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_250_REG (IPP_KLT_BASE + 0x19AC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_251_REG (IPP_KLT_BASE + 0x19B4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_252_REG (IPP_KLT_BASE + 0x19BC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_253_REG (IPP_KLT_BASE + 0x19C4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_254_REG (IPP_KLT_BASE + 0x19CC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_255_REG (IPP_KLT_BASE + 0x19D4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_256_REG (IPP_KLT_BASE + 0x19DC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_257_REG (IPP_KLT_BASE + 0x19E4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_258_REG (IPP_KLT_BASE + 0x19EC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_259_REG (IPP_KLT_BASE + 0x19F4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_260_REG (IPP_KLT_BASE + 0x19FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_261_REG (IPP_KLT_BASE + 0x1A04)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_262_REG (IPP_KLT_BASE + 0x1A0C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_263_REG (IPP_KLT_BASE + 0x1A14)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_264_REG (IPP_KLT_BASE + 0x1A1C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_265_REG (IPP_KLT_BASE + 0x1A24)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_266_REG (IPP_KLT_BASE + 0x1A2C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_267_REG (IPP_KLT_BASE + 0x1A34)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_268_REG (IPP_KLT_BASE + 0x1A3C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_269_REG (IPP_KLT_BASE + 0x1A44)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_270_REG (IPP_KLT_BASE + 0x1A4C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_271_REG (IPP_KLT_BASE + 0x1A54)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_272_REG (IPP_KLT_BASE + 0x1A5C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_273_REG (IPP_KLT_BASE + 0x1A64)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_274_REG (IPP_KLT_BASE + 0x1A6C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_275_REG (IPP_KLT_BASE + 0x1A74)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_276_REG (IPP_KLT_BASE + 0x1A7C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_277_REG (IPP_KLT_BASE + 0x1A84)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_278_REG (IPP_KLT_BASE + 0x1A8C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_279_REG (IPP_KLT_BASE + 0x1A94)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_280_REG (IPP_KLT_BASE + 0x1A9C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_281_REG (IPP_KLT_BASE + 0x1AA4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_282_REG (IPP_KLT_BASE + 0x1AAC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_283_REG (IPP_KLT_BASE + 0x1AB4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_284_REG (IPP_KLT_BASE + 0x1ABC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_285_REG (IPP_KLT_BASE + 0x1AC4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_286_REG (IPP_KLT_BASE + 0x1ACC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_287_REG (IPP_KLT_BASE + 0x1AD4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_288_REG (IPP_KLT_BASE + 0x1ADC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_289_REG (IPP_KLT_BASE + 0x1AE4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_290_REG (IPP_KLT_BASE + 0x1AEC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_291_REG (IPP_KLT_BASE + 0x1AF4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_292_REG (IPP_KLT_BASE + 0x1AFC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_293_REG (IPP_KLT_BASE + 0x1B04)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_294_REG (IPP_KLT_BASE + 0x1B0C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_295_REG (IPP_KLT_BASE + 0x1B14)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_296_REG (IPP_KLT_BASE + 0x1B1C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_297_REG (IPP_KLT_BASE + 0x1B24)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_298_REG (IPP_KLT_BASE + 0x1B2C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_299_REG (IPP_KLT_BASE + 0x1B34)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_300_REG (IPP_KLT_BASE + 0x1B3C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_301_REG (IPP_KLT_BASE + 0x1B44)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_302_REG (IPP_KLT_BASE + 0x1B4C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_303_REG (IPP_KLT_BASE + 0x1B54)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_304_REG (IPP_KLT_BASE + 0x1B5C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_305_REG (IPP_KLT_BASE + 0x1B64)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_306_REG (IPP_KLT_BASE + 0x1B6C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_307_REG (IPP_KLT_BASE + 0x1B74)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_308_REG (IPP_KLT_BASE + 0x1B7C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_309_REG (IPP_KLT_BASE + 0x1B84)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_310_REG (IPP_KLT_BASE + 0x1B8C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_311_REG (IPP_KLT_BASE + 0x1B94)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_312_REG (IPP_KLT_BASE + 0x1B9C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_313_REG (IPP_KLT_BASE + 0x1BA4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_314_REG (IPP_KLT_BASE + 0x1BAC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_315_REG (IPP_KLT_BASE + 0x1BB4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_316_REG (IPP_KLT_BASE + 0x1BBC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_317_REG (IPP_KLT_BASE + 0x1BC4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_318_REG (IPP_KLT_BASE + 0x1BCC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_319_REG (IPP_KLT_BASE + 0x1BD4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_320_REG (IPP_KLT_BASE + 0x1BDC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_321_REG (IPP_KLT_BASE + 0x1BE4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_322_REG (IPP_KLT_BASE + 0x1BEC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_323_REG (IPP_KLT_BASE + 0x1BF4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_324_REG (IPP_KLT_BASE + 0x1BFC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_325_REG (IPP_KLT_BASE + 0x1C04)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_326_REG (IPP_KLT_BASE + 0x1C0C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_327_REG (IPP_KLT_BASE + 0x1C14)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_328_REG (IPP_KLT_BASE + 0x1C1C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_329_REG (IPP_KLT_BASE + 0x1C24)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_330_REG (IPP_KLT_BASE + 0x1C2C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_331_REG (IPP_KLT_BASE + 0x1C34)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_332_REG (IPP_KLT_BASE + 0x1C3C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_333_REG (IPP_KLT_BASE + 0x1C44)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_334_REG (IPP_KLT_BASE + 0x1C4C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_335_REG (IPP_KLT_BASE + 0x1C54)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_336_REG (IPP_KLT_BASE + 0x1C5C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_337_REG (IPP_KLT_BASE + 0x1C64)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_338_REG (IPP_KLT_BASE + 0x1C6C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_339_REG (IPP_KLT_BASE + 0x1C74)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_340_REG (IPP_KLT_BASE + 0x1C7C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_341_REG (IPP_KLT_BASE + 0x1C84)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_342_REG (IPP_KLT_BASE + 0x1C8C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_343_REG (IPP_KLT_BASE + 0x1C94)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_344_REG (IPP_KLT_BASE + 0x1C9C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_345_REG (IPP_KLT_BASE + 0x1CA4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_346_REG (IPP_KLT_BASE + 0x1CAC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_347_REG (IPP_KLT_BASE + 0x1CB4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_348_REG (IPP_KLT_BASE + 0x1CBC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_349_REG (IPP_KLT_BASE + 0x1CC4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_350_REG (IPP_KLT_BASE + 0x1CCC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_351_REG (IPP_KLT_BASE + 0x1CD4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_352_REG (IPP_KLT_BASE + 0x1CDC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_353_REG (IPP_KLT_BASE + 0x1CE4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_354_REG (IPP_KLT_BASE + 0x1CEC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_355_REG (IPP_KLT_BASE + 0x1CF4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_356_REG (IPP_KLT_BASE + 0x1CFC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_357_REG (IPP_KLT_BASE + 0x1D04)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_358_REG (IPP_KLT_BASE + 0x1D0C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_359_REG (IPP_KLT_BASE + 0x1D14)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_360_REG (IPP_KLT_BASE + 0x1D1C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_361_REG (IPP_KLT_BASE + 0x1D24)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_362_REG (IPP_KLT_BASE + 0x1D2C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_363_REG (IPP_KLT_BASE + 0x1D34)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_364_REG (IPP_KLT_BASE + 0x1D3C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_365_REG (IPP_KLT_BASE + 0x1D44)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_366_REG (IPP_KLT_BASE + 0x1D4C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_367_REG (IPP_KLT_BASE + 0x1D54)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_368_REG (IPP_KLT_BASE + 0x1D5C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_369_REG (IPP_KLT_BASE + 0x1D64)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_370_REG (IPP_KLT_BASE + 0x1D6C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_371_REG (IPP_KLT_BASE + 0x1D74)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_372_REG (IPP_KLT_BASE + 0x1D7C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_373_REG (IPP_KLT_BASE + 0x1D84)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_374_REG (IPP_KLT_BASE + 0x1D8C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_375_REG (IPP_KLT_BASE + 0x1D94)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_376_REG (IPP_KLT_BASE + 0x1D9C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_377_REG (IPP_KLT_BASE + 0x1DA4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_378_REG (IPP_KLT_BASE + 0x1DAC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_379_REG (IPP_KLT_BASE + 0x1DB4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_380_REG (IPP_KLT_BASE + 0x1DBC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_381_REG (IPP_KLT_BASE + 0x1DC4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_382_REG (IPP_KLT_BASE + 0x1DCC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_383_REG (IPP_KLT_BASE + 0x1DD4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_384_REG (IPP_KLT_BASE + 0x1DDC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_385_REG (IPP_KLT_BASE + 0x1DE4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_386_REG (IPP_KLT_BASE + 0x1DEC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_387_REG (IPP_KLT_BASE + 0x1DF4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_388_REG (IPP_KLT_BASE + 0x1DFC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_389_REG (IPP_KLT_BASE + 0x1E04)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_390_REG (IPP_KLT_BASE + 0x1E0C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_391_REG (IPP_KLT_BASE + 0x1E14)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_392_REG (IPP_KLT_BASE + 0x1E1C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_393_REG (IPP_KLT_BASE + 0x1E24)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_394_REG (IPP_KLT_BASE + 0x1E2C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_395_REG (IPP_KLT_BASE + 0x1E34)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_396_REG (IPP_KLT_BASE + 0x1E3C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_397_REG (IPP_KLT_BASE + 0x1E44)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_398_REG (IPP_KLT_BASE + 0x1E4C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_399_REG (IPP_KLT_BASE + 0x1E54)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_400_REG (IPP_KLT_BASE + 0x1E5C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_401_REG (IPP_KLT_BASE + 0x1E64)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_402_REG (IPP_KLT_BASE + 0x1E6C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_403_REG (IPP_KLT_BASE + 0x1E74)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_404_REG (IPP_KLT_BASE + 0x1E7C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_405_REG (IPP_KLT_BASE + 0x1E84)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_406_REG (IPP_KLT_BASE + 0x1E8C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_407_REG (IPP_KLT_BASE + 0x1E94)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_408_REG (IPP_KLT_BASE + 0x1E9C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_409_REG (IPP_KLT_BASE + 0x1EA4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_410_REG (IPP_KLT_BASE + 0x1EAC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_411_REG (IPP_KLT_BASE + 0x1EB4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_412_REG (IPP_KLT_BASE + 0x1EBC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_413_REG (IPP_KLT_BASE + 0x1EC4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_414_REG (IPP_KLT_BASE + 0x1ECC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_415_REG (IPP_KLT_BASE + 0x1ED4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_416_REG (IPP_KLT_BASE + 0x1EDC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_417_REG (IPP_KLT_BASE + 0x1EE4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_418_REG (IPP_KLT_BASE + 0x1EEC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_419_REG (IPP_KLT_BASE + 0x1EF4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_420_REG (IPP_KLT_BASE + 0x1EFC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_421_REG (IPP_KLT_BASE + 0x1F04)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_422_REG (IPP_KLT_BASE + 0x1F0C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_423_REG (IPP_KLT_BASE + 0x1F14)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_424_REG (IPP_KLT_BASE + 0x1F1C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_425_REG (IPP_KLT_BASE + 0x1F24)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_426_REG (IPP_KLT_BASE + 0x1F2C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_427_REG (IPP_KLT_BASE + 0x1F34)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_428_REG (IPP_KLT_BASE + 0x1F3C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_429_REG (IPP_KLT_BASE + 0x1F44)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_430_REG (IPP_KLT_BASE + 0x1F4C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_431_REG (IPP_KLT_BASE + 0x1F54)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_432_REG (IPP_KLT_BASE + 0x1F5C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_433_REG (IPP_KLT_BASE + 0x1F64)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_434_REG (IPP_KLT_BASE + 0x1F6C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_435_REG (IPP_KLT_BASE + 0x1F74)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_436_REG (IPP_KLT_BASE + 0x1F7C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_437_REG (IPP_KLT_BASE + 0x1F84)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_438_REG (IPP_KLT_BASE + 0x1F8C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_439_REG (IPP_KLT_BASE + 0x1F94)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_440_REG (IPP_KLT_BASE + 0x1F9C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_441_REG (IPP_KLT_BASE + 0x1FA4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_442_REG (IPP_KLT_BASE + 0x1FAC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_443_REG (IPP_KLT_BASE + 0x1FB4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_444_REG (IPP_KLT_BASE + 0x1FBC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_445_REG (IPP_KLT_BASE + 0x1FC4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_446_REG (IPP_KLT_BASE + 0x1FCC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_447_REG (IPP_KLT_BASE + 0x1FD4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_448_REG (IPP_KLT_BASE + 0x1FDC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_449_REG (IPP_KLT_BASE + 0x1FE4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_450_REG (IPP_KLT_BASE + 0x1FEC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_451_REG (IPP_KLT_BASE + 0x1FF4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_452_REG (IPP_KLT_BASE + 0x1FFC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_453_REG (IPP_KLT_BASE + 0x2004)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_454_REG (IPP_KLT_BASE + 0x200C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_455_REG (IPP_KLT_BASE + 0x2014)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_456_REG (IPP_KLT_BASE + 0x201C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_457_REG (IPP_KLT_BASE + 0x2024)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_458_REG (IPP_KLT_BASE + 0x202C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_459_REG (IPP_KLT_BASE + 0x2034)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_460_REG (IPP_KLT_BASE + 0x203C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_461_REG (IPP_KLT_BASE + 0x2044)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_462_REG (IPP_KLT_BASE + 0x204C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_463_REG (IPP_KLT_BASE + 0x2054)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_464_REG (IPP_KLT_BASE + 0x205C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_465_REG (IPP_KLT_BASE + 0x2064)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_466_REG (IPP_KLT_BASE + 0x206C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_467_REG (IPP_KLT_BASE + 0x2074)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_468_REG (IPP_KLT_BASE + 0x207C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_469_REG (IPP_KLT_BASE + 0x2084)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_470_REG (IPP_KLT_BASE + 0x208C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_471_REG (IPP_KLT_BASE + 0x2094)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_472_REG (IPP_KLT_BASE + 0x209C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_473_REG (IPP_KLT_BASE + 0x20A4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_474_REG (IPP_KLT_BASE + 0x20AC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_475_REG (IPP_KLT_BASE + 0x20B4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_476_REG (IPP_KLT_BASE + 0x20BC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_477_REG (IPP_KLT_BASE + 0x20C4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_478_REG (IPP_KLT_BASE + 0x20CC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_479_REG (IPP_KLT_BASE + 0x20D4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_480_REG (IPP_KLT_BASE + 0x20DC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_481_REG (IPP_KLT_BASE + 0x20E4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_482_REG (IPP_KLT_BASE + 0x20EC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_483_REG (IPP_KLT_BASE + 0x20F4)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_484_REG (IPP_KLT_BASE + 0x20FC)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_485_REG (IPP_KLT_BASE + 0x2104)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_486_REG (IPP_KLT_BASE + 0x210C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_487_REG (IPP_KLT_BASE + 0x2114)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_488_REG (IPP_KLT_BASE + 0x211C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_489_REG (IPP_KLT_BASE + 0x2124)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_490_REG (IPP_KLT_BASE + 0x212C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_491_REG (IPP_KLT_BASE + 0x2134)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_492_REG (IPP_KLT_BASE + 0x213C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_493_REG (IPP_KLT_BASE + 0x2144)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_494_REG (IPP_KLT_BASE + 0x214C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_495_REG (IPP_KLT_BASE + 0x2154)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_496_REG (IPP_KLT_BASE + 0x215C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_497_REG (IPP_KLT_BASE + 0x2164)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_498_REG (IPP_KLT_BASE + 0x216C)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_X_499_REG (IPP_KLT_BASE + 0x2174)   /* Buffer of KP previous x coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_0_REG (IPP_KLT_BASE + 0x11E0)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_1_REG (IPP_KLT_BASE + 0x11E8)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_2_REG (IPP_KLT_BASE + 0x11F0)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_3_REG (IPP_KLT_BASE + 0x11F8)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_4_REG (IPP_KLT_BASE + 0x1200)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_5_REG (IPP_KLT_BASE + 0x1208)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_6_REG (IPP_KLT_BASE + 0x1210)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_7_REG (IPP_KLT_BASE + 0x1218)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_8_REG (IPP_KLT_BASE + 0x1220)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_9_REG (IPP_KLT_BASE + 0x1228)     /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_10_REG (IPP_KLT_BASE + 0x1230)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_11_REG (IPP_KLT_BASE + 0x1238)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_12_REG (IPP_KLT_BASE + 0x1240)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_13_REG (IPP_KLT_BASE + 0x1248)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_14_REG (IPP_KLT_BASE + 0x1250)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_15_REG (IPP_KLT_BASE + 0x1258)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_16_REG (IPP_KLT_BASE + 0x1260)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_17_REG (IPP_KLT_BASE + 0x1268)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_18_REG (IPP_KLT_BASE + 0x1270)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_19_REG (IPP_KLT_BASE + 0x1278)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_20_REG (IPP_KLT_BASE + 0x1280)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_21_REG (IPP_KLT_BASE + 0x1288)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_22_REG (IPP_KLT_BASE + 0x1290)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_23_REG (IPP_KLT_BASE + 0x1298)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_24_REG (IPP_KLT_BASE + 0x12A0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_25_REG (IPP_KLT_BASE + 0x12A8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_26_REG (IPP_KLT_BASE + 0x12B0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_27_REG (IPP_KLT_BASE + 0x12B8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_28_REG (IPP_KLT_BASE + 0x12C0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_29_REG (IPP_KLT_BASE + 0x12C8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_30_REG (IPP_KLT_BASE + 0x12D0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_31_REG (IPP_KLT_BASE + 0x12D8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_32_REG (IPP_KLT_BASE + 0x12E0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_33_REG (IPP_KLT_BASE + 0x12E8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_34_REG (IPP_KLT_BASE + 0x12F0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_35_REG (IPP_KLT_BASE + 0x12F8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_36_REG (IPP_KLT_BASE + 0x1300)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_37_REG (IPP_KLT_BASE + 0x1308)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_38_REG (IPP_KLT_BASE + 0x1310)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_39_REG (IPP_KLT_BASE + 0x1318)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_40_REG (IPP_KLT_BASE + 0x1320)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_41_REG (IPP_KLT_BASE + 0x1328)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_42_REG (IPP_KLT_BASE + 0x1330)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_43_REG (IPP_KLT_BASE + 0x1338)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_44_REG (IPP_KLT_BASE + 0x1340)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_45_REG (IPP_KLT_BASE + 0x1348)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_46_REG (IPP_KLT_BASE + 0x1350)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_47_REG (IPP_KLT_BASE + 0x1358)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_48_REG (IPP_KLT_BASE + 0x1360)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_49_REG (IPP_KLT_BASE + 0x1368)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_50_REG (IPP_KLT_BASE + 0x1370)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_51_REG (IPP_KLT_BASE + 0x1378)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_52_REG (IPP_KLT_BASE + 0x1380)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_53_REG (IPP_KLT_BASE + 0x1388)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_54_REG (IPP_KLT_BASE + 0x1390)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_55_REG (IPP_KLT_BASE + 0x1398)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_56_REG (IPP_KLT_BASE + 0x13A0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_57_REG (IPP_KLT_BASE + 0x13A8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_58_REG (IPP_KLT_BASE + 0x13B0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_59_REG (IPP_KLT_BASE + 0x13B8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_60_REG (IPP_KLT_BASE + 0x13C0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_61_REG (IPP_KLT_BASE + 0x13C8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_62_REG (IPP_KLT_BASE + 0x13D0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_63_REG (IPP_KLT_BASE + 0x13D8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_64_REG (IPP_KLT_BASE + 0x13E0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_65_REG (IPP_KLT_BASE + 0x13E8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_66_REG (IPP_KLT_BASE + 0x13F0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_67_REG (IPP_KLT_BASE + 0x13F8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_68_REG (IPP_KLT_BASE + 0x1400)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_69_REG (IPP_KLT_BASE + 0x1408)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_70_REG (IPP_KLT_BASE + 0x1410)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_71_REG (IPP_KLT_BASE + 0x1418)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_72_REG (IPP_KLT_BASE + 0x1420)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_73_REG (IPP_KLT_BASE + 0x1428)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_74_REG (IPP_KLT_BASE + 0x1430)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_75_REG (IPP_KLT_BASE + 0x1438)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_76_REG (IPP_KLT_BASE + 0x1440)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_77_REG (IPP_KLT_BASE + 0x1448)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_78_REG (IPP_KLT_BASE + 0x1450)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_79_REG (IPP_KLT_BASE + 0x1458)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_80_REG (IPP_KLT_BASE + 0x1460)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_81_REG (IPP_KLT_BASE + 0x1468)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_82_REG (IPP_KLT_BASE + 0x1470)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_83_REG (IPP_KLT_BASE + 0x1478)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_84_REG (IPP_KLT_BASE + 0x1480)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_85_REG (IPP_KLT_BASE + 0x1488)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_86_REG (IPP_KLT_BASE + 0x1490)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_87_REG (IPP_KLT_BASE + 0x1498)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_88_REG (IPP_KLT_BASE + 0x14A0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_89_REG (IPP_KLT_BASE + 0x14A8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_90_REG (IPP_KLT_BASE + 0x14B0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_91_REG (IPP_KLT_BASE + 0x14B8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_92_REG (IPP_KLT_BASE + 0x14C0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_93_REG (IPP_KLT_BASE + 0x14C8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_94_REG (IPP_KLT_BASE + 0x14D0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_95_REG (IPP_KLT_BASE + 0x14D8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_96_REG (IPP_KLT_BASE + 0x14E0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_97_REG (IPP_KLT_BASE + 0x14E8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_98_REG (IPP_KLT_BASE + 0x14F0)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_99_REG (IPP_KLT_BASE + 0x14F8)    /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_100_REG (IPP_KLT_BASE + 0x1500)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_101_REG (IPP_KLT_BASE + 0x1508)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_102_REG (IPP_KLT_BASE + 0x1510)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_103_REG (IPP_KLT_BASE + 0x1518)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_104_REG (IPP_KLT_BASE + 0x1520)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_105_REG (IPP_KLT_BASE + 0x1528)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_106_REG (IPP_KLT_BASE + 0x1530)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_107_REG (IPP_KLT_BASE + 0x1538)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_108_REG (IPP_KLT_BASE + 0x1540)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_109_REG (IPP_KLT_BASE + 0x1548)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_110_REG (IPP_KLT_BASE + 0x1550)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_111_REG (IPP_KLT_BASE + 0x1558)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_112_REG (IPP_KLT_BASE + 0x1560)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_113_REG (IPP_KLT_BASE + 0x1568)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_114_REG (IPP_KLT_BASE + 0x1570)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_115_REG (IPP_KLT_BASE + 0x1578)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_116_REG (IPP_KLT_BASE + 0x1580)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_117_REG (IPP_KLT_BASE + 0x1588)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_118_REG (IPP_KLT_BASE + 0x1590)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_119_REG (IPP_KLT_BASE + 0x1598)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_120_REG (IPP_KLT_BASE + 0x15A0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_121_REG (IPP_KLT_BASE + 0x15A8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_122_REG (IPP_KLT_BASE + 0x15B0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_123_REG (IPP_KLT_BASE + 0x15B8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_124_REG (IPP_KLT_BASE + 0x15C0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_125_REG (IPP_KLT_BASE + 0x15C8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_126_REG (IPP_KLT_BASE + 0x15D0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_127_REG (IPP_KLT_BASE + 0x15D8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_128_REG (IPP_KLT_BASE + 0x15E0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_129_REG (IPP_KLT_BASE + 0x15E8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_130_REG (IPP_KLT_BASE + 0x15F0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_131_REG (IPP_KLT_BASE + 0x15F8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_132_REG (IPP_KLT_BASE + 0x1600)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_133_REG (IPP_KLT_BASE + 0x1608)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_134_REG (IPP_KLT_BASE + 0x1610)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_135_REG (IPP_KLT_BASE + 0x1618)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_136_REG (IPP_KLT_BASE + 0x1620)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_137_REG (IPP_KLT_BASE + 0x1628)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_138_REG (IPP_KLT_BASE + 0x1630)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_139_REG (IPP_KLT_BASE + 0x1638)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_140_REG (IPP_KLT_BASE + 0x1640)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_141_REG (IPP_KLT_BASE + 0x1648)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_142_REG (IPP_KLT_BASE + 0x1650)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_143_REG (IPP_KLT_BASE + 0x1658)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_144_REG (IPP_KLT_BASE + 0x1660)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_145_REG (IPP_KLT_BASE + 0x1668)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_146_REG (IPP_KLT_BASE + 0x1670)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_147_REG (IPP_KLT_BASE + 0x1678)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_148_REG (IPP_KLT_BASE + 0x1680)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_149_REG (IPP_KLT_BASE + 0x1688)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_150_REG (IPP_KLT_BASE + 0x1690)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_151_REG (IPP_KLT_BASE + 0x1698)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_152_REG (IPP_KLT_BASE + 0x16A0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_153_REG (IPP_KLT_BASE + 0x16A8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_154_REG (IPP_KLT_BASE + 0x16B0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_155_REG (IPP_KLT_BASE + 0x16B8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_156_REG (IPP_KLT_BASE + 0x16C0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_157_REG (IPP_KLT_BASE + 0x16C8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_158_REG (IPP_KLT_BASE + 0x16D0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_159_REG (IPP_KLT_BASE + 0x16D8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_160_REG (IPP_KLT_BASE + 0x16E0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_161_REG (IPP_KLT_BASE + 0x16E8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_162_REG (IPP_KLT_BASE + 0x16F0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_163_REG (IPP_KLT_BASE + 0x16F8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_164_REG (IPP_KLT_BASE + 0x1700)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_165_REG (IPP_KLT_BASE + 0x1708)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_166_REG (IPP_KLT_BASE + 0x1710)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_167_REG (IPP_KLT_BASE + 0x1718)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_168_REG (IPP_KLT_BASE + 0x1720)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_169_REG (IPP_KLT_BASE + 0x1728)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_170_REG (IPP_KLT_BASE + 0x1730)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_171_REG (IPP_KLT_BASE + 0x1738)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_172_REG (IPP_KLT_BASE + 0x1740)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_173_REG (IPP_KLT_BASE + 0x1748)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_174_REG (IPP_KLT_BASE + 0x1750)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_175_REG (IPP_KLT_BASE + 0x1758)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_176_REG (IPP_KLT_BASE + 0x1760)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_177_REG (IPP_KLT_BASE + 0x1768)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_178_REG (IPP_KLT_BASE + 0x1770)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_179_REG (IPP_KLT_BASE + 0x1778)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_180_REG (IPP_KLT_BASE + 0x1780)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_181_REG (IPP_KLT_BASE + 0x1788)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_182_REG (IPP_KLT_BASE + 0x1790)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_183_REG (IPP_KLT_BASE + 0x1798)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_184_REG (IPP_KLT_BASE + 0x17A0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_185_REG (IPP_KLT_BASE + 0x17A8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_186_REG (IPP_KLT_BASE + 0x17B0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_187_REG (IPP_KLT_BASE + 0x17B8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_188_REG (IPP_KLT_BASE + 0x17C0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_189_REG (IPP_KLT_BASE + 0x17C8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_190_REG (IPP_KLT_BASE + 0x17D0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_191_REG (IPP_KLT_BASE + 0x17D8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_192_REG (IPP_KLT_BASE + 0x17E0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_193_REG (IPP_KLT_BASE + 0x17E8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_194_REG (IPP_KLT_BASE + 0x17F0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_195_REG (IPP_KLT_BASE + 0x17F8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_196_REG (IPP_KLT_BASE + 0x1800)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_197_REG (IPP_KLT_BASE + 0x1808)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_198_REG (IPP_KLT_BASE + 0x1810)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_199_REG (IPP_KLT_BASE + 0x1818)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_200_REG (IPP_KLT_BASE + 0x1820)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_201_REG (IPP_KLT_BASE + 0x1828)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_202_REG (IPP_KLT_BASE + 0x1830)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_203_REG (IPP_KLT_BASE + 0x1838)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_204_REG (IPP_KLT_BASE + 0x1840)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_205_REG (IPP_KLT_BASE + 0x1848)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_206_REG (IPP_KLT_BASE + 0x1850)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_207_REG (IPP_KLT_BASE + 0x1858)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_208_REG (IPP_KLT_BASE + 0x1860)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_209_REG (IPP_KLT_BASE + 0x1868)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_210_REG (IPP_KLT_BASE + 0x1870)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_211_REG (IPP_KLT_BASE + 0x1878)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_212_REG (IPP_KLT_BASE + 0x1880)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_213_REG (IPP_KLT_BASE + 0x1888)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_214_REG (IPP_KLT_BASE + 0x1890)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_215_REG (IPP_KLT_BASE + 0x1898)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_216_REG (IPP_KLT_BASE + 0x18A0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_217_REG (IPP_KLT_BASE + 0x18A8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_218_REG (IPP_KLT_BASE + 0x18B0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_219_REG (IPP_KLT_BASE + 0x18B8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_220_REG (IPP_KLT_BASE + 0x18C0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_221_REG (IPP_KLT_BASE + 0x18C8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_222_REG (IPP_KLT_BASE + 0x18D0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_223_REG (IPP_KLT_BASE + 0x18D8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_224_REG (IPP_KLT_BASE + 0x18E0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_225_REG (IPP_KLT_BASE + 0x18E8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_226_REG (IPP_KLT_BASE + 0x18F0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_227_REG (IPP_KLT_BASE + 0x18F8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_228_REG (IPP_KLT_BASE + 0x1900)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_229_REG (IPP_KLT_BASE + 0x1908)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_230_REG (IPP_KLT_BASE + 0x1910)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_231_REG (IPP_KLT_BASE + 0x1918)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_232_REG (IPP_KLT_BASE + 0x1920)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_233_REG (IPP_KLT_BASE + 0x1928)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_234_REG (IPP_KLT_BASE + 0x1930)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_235_REG (IPP_KLT_BASE + 0x1938)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_236_REG (IPP_KLT_BASE + 0x1940)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_237_REG (IPP_KLT_BASE + 0x1948)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_238_REG (IPP_KLT_BASE + 0x1950)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_239_REG (IPP_KLT_BASE + 0x1958)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_240_REG (IPP_KLT_BASE + 0x1960)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_241_REG (IPP_KLT_BASE + 0x1968)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_242_REG (IPP_KLT_BASE + 0x1970)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_243_REG (IPP_KLT_BASE + 0x1978)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_244_REG (IPP_KLT_BASE + 0x1980)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_245_REG (IPP_KLT_BASE + 0x1988)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_246_REG (IPP_KLT_BASE + 0x1990)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_247_REG (IPP_KLT_BASE + 0x1998)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_248_REG (IPP_KLT_BASE + 0x19A0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_249_REG (IPP_KLT_BASE + 0x19A8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_250_REG (IPP_KLT_BASE + 0x19B0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_251_REG (IPP_KLT_BASE + 0x19B8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_252_REG (IPP_KLT_BASE + 0x19C0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_253_REG (IPP_KLT_BASE + 0x19C8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_254_REG (IPP_KLT_BASE + 0x19D0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_255_REG (IPP_KLT_BASE + 0x19D8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_256_REG (IPP_KLT_BASE + 0x19E0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_257_REG (IPP_KLT_BASE + 0x19E8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_258_REG (IPP_KLT_BASE + 0x19F0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_259_REG (IPP_KLT_BASE + 0x19F8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_260_REG (IPP_KLT_BASE + 0x1A00)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_261_REG (IPP_KLT_BASE + 0x1A08)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_262_REG (IPP_KLT_BASE + 0x1A10)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_263_REG (IPP_KLT_BASE + 0x1A18)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_264_REG (IPP_KLT_BASE + 0x1A20)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_265_REG (IPP_KLT_BASE + 0x1A28)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_266_REG (IPP_KLT_BASE + 0x1A30)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_267_REG (IPP_KLT_BASE + 0x1A38)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_268_REG (IPP_KLT_BASE + 0x1A40)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_269_REG (IPP_KLT_BASE + 0x1A48)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_270_REG (IPP_KLT_BASE + 0x1A50)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_271_REG (IPP_KLT_BASE + 0x1A58)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_272_REG (IPP_KLT_BASE + 0x1A60)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_273_REG (IPP_KLT_BASE + 0x1A68)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_274_REG (IPP_KLT_BASE + 0x1A70)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_275_REG (IPP_KLT_BASE + 0x1A78)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_276_REG (IPP_KLT_BASE + 0x1A80)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_277_REG (IPP_KLT_BASE + 0x1A88)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_278_REG (IPP_KLT_BASE + 0x1A90)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_279_REG (IPP_KLT_BASE + 0x1A98)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_280_REG (IPP_KLT_BASE + 0x1AA0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_281_REG (IPP_KLT_BASE + 0x1AA8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_282_REG (IPP_KLT_BASE + 0x1AB0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_283_REG (IPP_KLT_BASE + 0x1AB8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_284_REG (IPP_KLT_BASE + 0x1AC0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_285_REG (IPP_KLT_BASE + 0x1AC8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_286_REG (IPP_KLT_BASE + 0x1AD0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_287_REG (IPP_KLT_BASE + 0x1AD8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_288_REG (IPP_KLT_BASE + 0x1AE0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_289_REG (IPP_KLT_BASE + 0x1AE8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_290_REG (IPP_KLT_BASE + 0x1AF0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_291_REG (IPP_KLT_BASE + 0x1AF8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_292_REG (IPP_KLT_BASE + 0x1B00)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_293_REG (IPP_KLT_BASE + 0x1B08)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_294_REG (IPP_KLT_BASE + 0x1B10)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_295_REG (IPP_KLT_BASE + 0x1B18)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_296_REG (IPP_KLT_BASE + 0x1B20)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_297_REG (IPP_KLT_BASE + 0x1B28)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_298_REG (IPP_KLT_BASE + 0x1B30)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_299_REG (IPP_KLT_BASE + 0x1B38)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_300_REG (IPP_KLT_BASE + 0x1B40)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_301_REG (IPP_KLT_BASE + 0x1B48)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_302_REG (IPP_KLT_BASE + 0x1B50)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_303_REG (IPP_KLT_BASE + 0x1B58)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_304_REG (IPP_KLT_BASE + 0x1B60)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_305_REG (IPP_KLT_BASE + 0x1B68)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_306_REG (IPP_KLT_BASE + 0x1B70)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_307_REG (IPP_KLT_BASE + 0x1B78)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_308_REG (IPP_KLT_BASE + 0x1B80)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_309_REG (IPP_KLT_BASE + 0x1B88)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_310_REG (IPP_KLT_BASE + 0x1B90)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_311_REG (IPP_KLT_BASE + 0x1B98)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_312_REG (IPP_KLT_BASE + 0x1BA0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_313_REG (IPP_KLT_BASE + 0x1BA8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_314_REG (IPP_KLT_BASE + 0x1BB0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_315_REG (IPP_KLT_BASE + 0x1BB8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_316_REG (IPP_KLT_BASE + 0x1BC0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_317_REG (IPP_KLT_BASE + 0x1BC8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_318_REG (IPP_KLT_BASE + 0x1BD0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_319_REG (IPP_KLT_BASE + 0x1BD8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_320_REG (IPP_KLT_BASE + 0x1BE0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_321_REG (IPP_KLT_BASE + 0x1BE8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_322_REG (IPP_KLT_BASE + 0x1BF0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_323_REG (IPP_KLT_BASE + 0x1BF8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_324_REG (IPP_KLT_BASE + 0x1C00)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_325_REG (IPP_KLT_BASE + 0x1C08)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_326_REG (IPP_KLT_BASE + 0x1C10)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_327_REG (IPP_KLT_BASE + 0x1C18)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_328_REG (IPP_KLT_BASE + 0x1C20)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_329_REG (IPP_KLT_BASE + 0x1C28)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_330_REG (IPP_KLT_BASE + 0x1C30)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_331_REG (IPP_KLT_BASE + 0x1C38)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_332_REG (IPP_KLT_BASE + 0x1C40)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_333_REG (IPP_KLT_BASE + 0x1C48)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_334_REG (IPP_KLT_BASE + 0x1C50)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_335_REG (IPP_KLT_BASE + 0x1C58)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_336_REG (IPP_KLT_BASE + 0x1C60)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_337_REG (IPP_KLT_BASE + 0x1C68)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_338_REG (IPP_KLT_BASE + 0x1C70)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_339_REG (IPP_KLT_BASE + 0x1C78)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_340_REG (IPP_KLT_BASE + 0x1C80)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_341_REG (IPP_KLT_BASE + 0x1C88)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_342_REG (IPP_KLT_BASE + 0x1C90)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_343_REG (IPP_KLT_BASE + 0x1C98)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_344_REG (IPP_KLT_BASE + 0x1CA0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_345_REG (IPP_KLT_BASE + 0x1CA8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_346_REG (IPP_KLT_BASE + 0x1CB0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_347_REG (IPP_KLT_BASE + 0x1CB8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_348_REG (IPP_KLT_BASE + 0x1CC0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_349_REG (IPP_KLT_BASE + 0x1CC8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_350_REG (IPP_KLT_BASE + 0x1CD0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_351_REG (IPP_KLT_BASE + 0x1CD8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_352_REG (IPP_KLT_BASE + 0x1CE0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_353_REG (IPP_KLT_BASE + 0x1CE8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_354_REG (IPP_KLT_BASE + 0x1CF0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_355_REG (IPP_KLT_BASE + 0x1CF8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_356_REG (IPP_KLT_BASE + 0x1D00)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_357_REG (IPP_KLT_BASE + 0x1D08)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_358_REG (IPP_KLT_BASE + 0x1D10)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_359_REG (IPP_KLT_BASE + 0x1D18)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_360_REG (IPP_KLT_BASE + 0x1D20)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_361_REG (IPP_KLT_BASE + 0x1D28)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_362_REG (IPP_KLT_BASE + 0x1D30)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_363_REG (IPP_KLT_BASE + 0x1D38)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_364_REG (IPP_KLT_BASE + 0x1D40)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_365_REG (IPP_KLT_BASE + 0x1D48)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_366_REG (IPP_KLT_BASE + 0x1D50)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_367_REG (IPP_KLT_BASE + 0x1D58)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_368_REG (IPP_KLT_BASE + 0x1D60)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_369_REG (IPP_KLT_BASE + 0x1D68)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_370_REG (IPP_KLT_BASE + 0x1D70)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_371_REG (IPP_KLT_BASE + 0x1D78)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_372_REG (IPP_KLT_BASE + 0x1D80)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_373_REG (IPP_KLT_BASE + 0x1D88)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_374_REG (IPP_KLT_BASE + 0x1D90)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_375_REG (IPP_KLT_BASE + 0x1D98)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_376_REG (IPP_KLT_BASE + 0x1DA0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_377_REG (IPP_KLT_BASE + 0x1DA8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_378_REG (IPP_KLT_BASE + 0x1DB0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_379_REG (IPP_KLT_BASE + 0x1DB8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_380_REG (IPP_KLT_BASE + 0x1DC0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_381_REG (IPP_KLT_BASE + 0x1DC8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_382_REG (IPP_KLT_BASE + 0x1DD0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_383_REG (IPP_KLT_BASE + 0x1DD8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_384_REG (IPP_KLT_BASE + 0x1DE0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_385_REG (IPP_KLT_BASE + 0x1DE8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_386_REG (IPP_KLT_BASE + 0x1DF0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_387_REG (IPP_KLT_BASE + 0x1DF8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_388_REG (IPP_KLT_BASE + 0x1E00)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_389_REG (IPP_KLT_BASE + 0x1E08)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_390_REG (IPP_KLT_BASE + 0x1E10)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_391_REG (IPP_KLT_BASE + 0x1E18)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_392_REG (IPP_KLT_BASE + 0x1E20)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_393_REG (IPP_KLT_BASE + 0x1E28)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_394_REG (IPP_KLT_BASE + 0x1E30)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_395_REG (IPP_KLT_BASE + 0x1E38)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_396_REG (IPP_KLT_BASE + 0x1E40)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_397_REG (IPP_KLT_BASE + 0x1E48)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_398_REG (IPP_KLT_BASE + 0x1E50)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_399_REG (IPP_KLT_BASE + 0x1E58)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_400_REG (IPP_KLT_BASE + 0x1E60)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_401_REG (IPP_KLT_BASE + 0x1E68)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_402_REG (IPP_KLT_BASE + 0x1E70)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_403_REG (IPP_KLT_BASE + 0x1E78)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_404_REG (IPP_KLT_BASE + 0x1E80)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_405_REG (IPP_KLT_BASE + 0x1E88)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_406_REG (IPP_KLT_BASE + 0x1E90)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_407_REG (IPP_KLT_BASE + 0x1E98)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_408_REG (IPP_KLT_BASE + 0x1EA0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_409_REG (IPP_KLT_BASE + 0x1EA8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_410_REG (IPP_KLT_BASE + 0x1EB0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_411_REG (IPP_KLT_BASE + 0x1EB8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_412_REG (IPP_KLT_BASE + 0x1EC0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_413_REG (IPP_KLT_BASE + 0x1EC8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_414_REG (IPP_KLT_BASE + 0x1ED0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_415_REG (IPP_KLT_BASE + 0x1ED8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_416_REG (IPP_KLT_BASE + 0x1EE0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_417_REG (IPP_KLT_BASE + 0x1EE8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_418_REG (IPP_KLT_BASE + 0x1EF0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_419_REG (IPP_KLT_BASE + 0x1EF8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_420_REG (IPP_KLT_BASE + 0x1F00)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_421_REG (IPP_KLT_BASE + 0x1F08)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_422_REG (IPP_KLT_BASE + 0x1F10)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_423_REG (IPP_KLT_BASE + 0x1F18)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_424_REG (IPP_KLT_BASE + 0x1F20)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_425_REG (IPP_KLT_BASE + 0x1F28)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_426_REG (IPP_KLT_BASE + 0x1F30)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_427_REG (IPP_KLT_BASE + 0x1F38)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_428_REG (IPP_KLT_BASE + 0x1F40)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_429_REG (IPP_KLT_BASE + 0x1F48)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_430_REG (IPP_KLT_BASE + 0x1F50)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_431_REG (IPP_KLT_BASE + 0x1F58)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_432_REG (IPP_KLT_BASE + 0x1F60)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_433_REG (IPP_KLT_BASE + 0x1F68)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_434_REG (IPP_KLT_BASE + 0x1F70)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_435_REG (IPP_KLT_BASE + 0x1F78)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_436_REG (IPP_KLT_BASE + 0x1F80)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_437_REG (IPP_KLT_BASE + 0x1F88)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_438_REG (IPP_KLT_BASE + 0x1F90)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_439_REG (IPP_KLT_BASE + 0x1F98)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_440_REG (IPP_KLT_BASE + 0x1FA0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_441_REG (IPP_KLT_BASE + 0x1FA8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_442_REG (IPP_KLT_BASE + 0x1FB0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_443_REG (IPP_KLT_BASE + 0x1FB8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_444_REG (IPP_KLT_BASE + 0x1FC0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_445_REG (IPP_KLT_BASE + 0x1FC8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_446_REG (IPP_KLT_BASE + 0x1FD0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_447_REG (IPP_KLT_BASE + 0x1FD8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_448_REG (IPP_KLT_BASE + 0x1FE0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_449_REG (IPP_KLT_BASE + 0x1FE8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_450_REG (IPP_KLT_BASE + 0x1FF0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_451_REG (IPP_KLT_BASE + 0x1FF8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_452_REG (IPP_KLT_BASE + 0x2000)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_453_REG (IPP_KLT_BASE + 0x2008)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_454_REG (IPP_KLT_BASE + 0x2010)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_455_REG (IPP_KLT_BASE + 0x2018)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_456_REG (IPP_KLT_BASE + 0x2020)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_457_REG (IPP_KLT_BASE + 0x2028)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_458_REG (IPP_KLT_BASE + 0x2030)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_459_REG (IPP_KLT_BASE + 0x2038)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_460_REG (IPP_KLT_BASE + 0x2040)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_461_REG (IPP_KLT_BASE + 0x2048)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_462_REG (IPP_KLT_BASE + 0x2050)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_463_REG (IPP_KLT_BASE + 0x2058)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_464_REG (IPP_KLT_BASE + 0x2060)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_465_REG (IPP_KLT_BASE + 0x2068)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_466_REG (IPP_KLT_BASE + 0x2070)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_467_REG (IPP_KLT_BASE + 0x2078)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_468_REG (IPP_KLT_BASE + 0x2080)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_469_REG (IPP_KLT_BASE + 0x2088)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_470_REG (IPP_KLT_BASE + 0x2090)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_471_REG (IPP_KLT_BASE + 0x2098)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_472_REG (IPP_KLT_BASE + 0x20A0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_473_REG (IPP_KLT_BASE + 0x20A8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_474_REG (IPP_KLT_BASE + 0x20B0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_475_REG (IPP_KLT_BASE + 0x20B8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_476_REG (IPP_KLT_BASE + 0x20C0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_477_REG (IPP_KLT_BASE + 0x20C8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_478_REG (IPP_KLT_BASE + 0x20D0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_479_REG (IPP_KLT_BASE + 0x20D8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_480_REG (IPP_KLT_BASE + 0x20E0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_481_REG (IPP_KLT_BASE + 0x20E8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_482_REG (IPP_KLT_BASE + 0x20F0)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_483_REG (IPP_KLT_BASE + 0x20F8)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_484_REG (IPP_KLT_BASE + 0x2100)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_485_REG (IPP_KLT_BASE + 0x2108)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_486_REG (IPP_KLT_BASE + 0x2110)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_487_REG (IPP_KLT_BASE + 0x2118)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_488_REG (IPP_KLT_BASE + 0x2120)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_489_REG (IPP_KLT_BASE + 0x2128)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_490_REG (IPP_KLT_BASE + 0x2130)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_491_REG (IPP_KLT_BASE + 0x2138)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_492_REG (IPP_KLT_BASE + 0x2140)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_493_REG (IPP_KLT_BASE + 0x2148)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_494_REG (IPP_KLT_BASE + 0x2150)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_495_REG (IPP_KLT_BASE + 0x2158)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_496_REG (IPP_KLT_BASE + 0x2160)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_497_REG (IPP_KLT_BASE + 0x2168)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_498_REG (IPP_KLT_BASE + 0x2170)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_PREV_BUFF_Y_499_REG (IPP_KLT_BASE + 0x2178)   /* Buffer of KP previous y coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_0_REG (IPP_KLT_BASE + 0x217C)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_1_REG (IPP_KLT_BASE + 0x2184)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_2_REG (IPP_KLT_BASE + 0x218C)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_3_REG (IPP_KLT_BASE + 0x2194)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_4_REG (IPP_KLT_BASE + 0x219C)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_5_REG (IPP_KLT_BASE + 0x21A4)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_6_REG (IPP_KLT_BASE + 0x21AC)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_7_REG (IPP_KLT_BASE + 0x21B4)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_8_REG (IPP_KLT_BASE + 0x21BC)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_9_REG (IPP_KLT_BASE + 0x21C4)     /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_10_REG (IPP_KLT_BASE + 0x21CC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_11_REG (IPP_KLT_BASE + 0x21D4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_12_REG (IPP_KLT_BASE + 0x21DC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_13_REG (IPP_KLT_BASE + 0x21E4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_14_REG (IPP_KLT_BASE + 0x21EC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_15_REG (IPP_KLT_BASE + 0x21F4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_16_REG (IPP_KLT_BASE + 0x21FC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_17_REG (IPP_KLT_BASE + 0x2204)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_18_REG (IPP_KLT_BASE + 0x220C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_19_REG (IPP_KLT_BASE + 0x2214)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_20_REG (IPP_KLT_BASE + 0x221C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_21_REG (IPP_KLT_BASE + 0x2224)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_22_REG (IPP_KLT_BASE + 0x222C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_23_REG (IPP_KLT_BASE + 0x2234)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_24_REG (IPP_KLT_BASE + 0x223C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_25_REG (IPP_KLT_BASE + 0x2244)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_26_REG (IPP_KLT_BASE + 0x224C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_27_REG (IPP_KLT_BASE + 0x2254)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_28_REG (IPP_KLT_BASE + 0x225C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_29_REG (IPP_KLT_BASE + 0x2264)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_30_REG (IPP_KLT_BASE + 0x226C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_31_REG (IPP_KLT_BASE + 0x2274)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_32_REG (IPP_KLT_BASE + 0x227C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_33_REG (IPP_KLT_BASE + 0x2284)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_34_REG (IPP_KLT_BASE + 0x228C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_35_REG (IPP_KLT_BASE + 0x2294)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_36_REG (IPP_KLT_BASE + 0x229C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_37_REG (IPP_KLT_BASE + 0x22A4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_38_REG (IPP_KLT_BASE + 0x22AC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_39_REG (IPP_KLT_BASE + 0x22B4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_40_REG (IPP_KLT_BASE + 0x22BC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_41_REG (IPP_KLT_BASE + 0x22C4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_42_REG (IPP_KLT_BASE + 0x22CC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_43_REG (IPP_KLT_BASE + 0x22D4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_44_REG (IPP_KLT_BASE + 0x22DC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_45_REG (IPP_KLT_BASE + 0x22E4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_46_REG (IPP_KLT_BASE + 0x22EC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_47_REG (IPP_KLT_BASE + 0x22F4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_48_REG (IPP_KLT_BASE + 0x22FC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_49_REG (IPP_KLT_BASE + 0x2304)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_50_REG (IPP_KLT_BASE + 0x230C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_51_REG (IPP_KLT_BASE + 0x2314)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_52_REG (IPP_KLT_BASE + 0x231C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_53_REG (IPP_KLT_BASE + 0x2324)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_54_REG (IPP_KLT_BASE + 0x232C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_55_REG (IPP_KLT_BASE + 0x2334)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_56_REG (IPP_KLT_BASE + 0x233C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_57_REG (IPP_KLT_BASE + 0x2344)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_58_REG (IPP_KLT_BASE + 0x234C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_59_REG (IPP_KLT_BASE + 0x2354)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_60_REG (IPP_KLT_BASE + 0x235C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_61_REG (IPP_KLT_BASE + 0x2364)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_62_REG (IPP_KLT_BASE + 0x236C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_63_REG (IPP_KLT_BASE + 0x2374)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_64_REG (IPP_KLT_BASE + 0x237C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_65_REG (IPP_KLT_BASE + 0x2384)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_66_REG (IPP_KLT_BASE + 0x238C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_67_REG (IPP_KLT_BASE + 0x2394)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_68_REG (IPP_KLT_BASE + 0x239C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_69_REG (IPP_KLT_BASE + 0x23A4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_70_REG (IPP_KLT_BASE + 0x23AC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_71_REG (IPP_KLT_BASE + 0x23B4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_72_REG (IPP_KLT_BASE + 0x23BC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_73_REG (IPP_KLT_BASE + 0x23C4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_74_REG (IPP_KLT_BASE + 0x23CC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_75_REG (IPP_KLT_BASE + 0x23D4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_76_REG (IPP_KLT_BASE + 0x23DC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_77_REG (IPP_KLT_BASE + 0x23E4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_78_REG (IPP_KLT_BASE + 0x23EC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_79_REG (IPP_KLT_BASE + 0x23F4)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_80_REG (IPP_KLT_BASE + 0x23FC)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_81_REG (IPP_KLT_BASE + 0x2404)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_82_REG (IPP_KLT_BASE + 0x240C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_83_REG (IPP_KLT_BASE + 0x2414)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_84_REG (IPP_KLT_BASE + 0x241C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_85_REG (IPP_KLT_BASE + 0x2424)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_86_REG (IPP_KLT_BASE + 0x242C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_87_REG (IPP_KLT_BASE + 0x2434)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_88_REG (IPP_KLT_BASE + 0x243C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_89_REG (IPP_KLT_BASE + 0x2444)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_90_REG (IPP_KLT_BASE + 0x244C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_91_REG (IPP_KLT_BASE + 0x2454)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_92_REG (IPP_KLT_BASE + 0x245C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_93_REG (IPP_KLT_BASE + 0x2464)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_94_REG (IPP_KLT_BASE + 0x246C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_95_REG (IPP_KLT_BASE + 0x2474)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_96_REG (IPP_KLT_BASE + 0x247C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_97_REG (IPP_KLT_BASE + 0x2484)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_98_REG (IPP_KLT_BASE + 0x248C)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_99_REG (IPP_KLT_BASE + 0x2494)    /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_100_REG (IPP_KLT_BASE + 0x249C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_101_REG (IPP_KLT_BASE + 0x24A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_102_REG (IPP_KLT_BASE + 0x24AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_103_REG (IPP_KLT_BASE + 0x24B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_104_REG (IPP_KLT_BASE + 0x24BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_105_REG (IPP_KLT_BASE + 0x24C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_106_REG (IPP_KLT_BASE + 0x24CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_107_REG (IPP_KLT_BASE + 0x24D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_108_REG (IPP_KLT_BASE + 0x24DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_109_REG (IPP_KLT_BASE + 0x24E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_110_REG (IPP_KLT_BASE + 0x24EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_111_REG (IPP_KLT_BASE + 0x24F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_112_REG (IPP_KLT_BASE + 0x24FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_113_REG (IPP_KLT_BASE + 0x2504)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_114_REG (IPP_KLT_BASE + 0x250C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_115_REG (IPP_KLT_BASE + 0x2514)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_116_REG (IPP_KLT_BASE + 0x251C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_117_REG (IPP_KLT_BASE + 0x2524)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_118_REG (IPP_KLT_BASE + 0x252C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_119_REG (IPP_KLT_BASE + 0x2534)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_120_REG (IPP_KLT_BASE + 0x253C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_121_REG (IPP_KLT_BASE + 0x2544)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_122_REG (IPP_KLT_BASE + 0x254C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_123_REG (IPP_KLT_BASE + 0x2554)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_124_REG (IPP_KLT_BASE + 0x255C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_125_REG (IPP_KLT_BASE + 0x2564)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_126_REG (IPP_KLT_BASE + 0x256C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_127_REG (IPP_KLT_BASE + 0x2574)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_128_REG (IPP_KLT_BASE + 0x257C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_129_REG (IPP_KLT_BASE + 0x2584)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_130_REG (IPP_KLT_BASE + 0x258C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_131_REG (IPP_KLT_BASE + 0x2594)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_132_REG (IPP_KLT_BASE + 0x259C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_133_REG (IPP_KLT_BASE + 0x25A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_134_REG (IPP_KLT_BASE + 0x25AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_135_REG (IPP_KLT_BASE + 0x25B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_136_REG (IPP_KLT_BASE + 0x25BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_137_REG (IPP_KLT_BASE + 0x25C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_138_REG (IPP_KLT_BASE + 0x25CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_139_REG (IPP_KLT_BASE + 0x25D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_140_REG (IPP_KLT_BASE + 0x25DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_141_REG (IPP_KLT_BASE + 0x25E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_142_REG (IPP_KLT_BASE + 0x25EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_143_REG (IPP_KLT_BASE + 0x25F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_144_REG (IPP_KLT_BASE + 0x25FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_145_REG (IPP_KLT_BASE + 0x2604)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_146_REG (IPP_KLT_BASE + 0x260C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_147_REG (IPP_KLT_BASE + 0x2614)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_148_REG (IPP_KLT_BASE + 0x261C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_149_REG (IPP_KLT_BASE + 0x2624)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_150_REG (IPP_KLT_BASE + 0x262C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_151_REG (IPP_KLT_BASE + 0x2634)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_152_REG (IPP_KLT_BASE + 0x263C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_153_REG (IPP_KLT_BASE + 0x2644)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_154_REG (IPP_KLT_BASE + 0x264C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_155_REG (IPP_KLT_BASE + 0x2654)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_156_REG (IPP_KLT_BASE + 0x265C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_157_REG (IPP_KLT_BASE + 0x2664)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_158_REG (IPP_KLT_BASE + 0x266C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_159_REG (IPP_KLT_BASE + 0x2674)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_160_REG (IPP_KLT_BASE + 0x267C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_161_REG (IPP_KLT_BASE + 0x2684)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_162_REG (IPP_KLT_BASE + 0x268C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_163_REG (IPP_KLT_BASE + 0x2694)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_164_REG (IPP_KLT_BASE + 0x269C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_165_REG (IPP_KLT_BASE + 0x26A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_166_REG (IPP_KLT_BASE + 0x26AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_167_REG (IPP_KLT_BASE + 0x26B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_168_REG (IPP_KLT_BASE + 0x26BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_169_REG (IPP_KLT_BASE + 0x26C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_170_REG (IPP_KLT_BASE + 0x26CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_171_REG (IPP_KLT_BASE + 0x26D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_172_REG (IPP_KLT_BASE + 0x26DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_173_REG (IPP_KLT_BASE + 0x26E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_174_REG (IPP_KLT_BASE + 0x26EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_175_REG (IPP_KLT_BASE + 0x26F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_176_REG (IPP_KLT_BASE + 0x26FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_177_REG (IPP_KLT_BASE + 0x2704)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_178_REG (IPP_KLT_BASE + 0x270C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_179_REG (IPP_KLT_BASE + 0x2714)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_180_REG (IPP_KLT_BASE + 0x271C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_181_REG (IPP_KLT_BASE + 0x2724)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_182_REG (IPP_KLT_BASE + 0x272C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_183_REG (IPP_KLT_BASE + 0x2734)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_184_REG (IPP_KLT_BASE + 0x273C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_185_REG (IPP_KLT_BASE + 0x2744)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_186_REG (IPP_KLT_BASE + 0x274C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_187_REG (IPP_KLT_BASE + 0x2754)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_188_REG (IPP_KLT_BASE + 0x275C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_189_REG (IPP_KLT_BASE + 0x2764)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_190_REG (IPP_KLT_BASE + 0x276C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_191_REG (IPP_KLT_BASE + 0x2774)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_192_REG (IPP_KLT_BASE + 0x277C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_193_REG (IPP_KLT_BASE + 0x2784)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_194_REG (IPP_KLT_BASE + 0x278C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_195_REG (IPP_KLT_BASE + 0x2794)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_196_REG (IPP_KLT_BASE + 0x279C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_197_REG (IPP_KLT_BASE + 0x27A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_198_REG (IPP_KLT_BASE + 0x27AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_199_REG (IPP_KLT_BASE + 0x27B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_200_REG (IPP_KLT_BASE + 0x27BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_201_REG (IPP_KLT_BASE + 0x27C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_202_REG (IPP_KLT_BASE + 0x27CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_203_REG (IPP_KLT_BASE + 0x27D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_204_REG (IPP_KLT_BASE + 0x27DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_205_REG (IPP_KLT_BASE + 0x27E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_206_REG (IPP_KLT_BASE + 0x27EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_207_REG (IPP_KLT_BASE + 0x27F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_208_REG (IPP_KLT_BASE + 0x27FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_209_REG (IPP_KLT_BASE + 0x2804)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_210_REG (IPP_KLT_BASE + 0x280C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_211_REG (IPP_KLT_BASE + 0x2814)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_212_REG (IPP_KLT_BASE + 0x281C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_213_REG (IPP_KLT_BASE + 0x2824)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_214_REG (IPP_KLT_BASE + 0x282C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_215_REG (IPP_KLT_BASE + 0x2834)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_216_REG (IPP_KLT_BASE + 0x283C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_217_REG (IPP_KLT_BASE + 0x2844)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_218_REG (IPP_KLT_BASE + 0x284C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_219_REG (IPP_KLT_BASE + 0x2854)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_220_REG (IPP_KLT_BASE + 0x285C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_221_REG (IPP_KLT_BASE + 0x2864)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_222_REG (IPP_KLT_BASE + 0x286C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_223_REG (IPP_KLT_BASE + 0x2874)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_224_REG (IPP_KLT_BASE + 0x287C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_225_REG (IPP_KLT_BASE + 0x2884)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_226_REG (IPP_KLT_BASE + 0x288C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_227_REG (IPP_KLT_BASE + 0x2894)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_228_REG (IPP_KLT_BASE + 0x289C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_229_REG (IPP_KLT_BASE + 0x28A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_230_REG (IPP_KLT_BASE + 0x28AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_231_REG (IPP_KLT_BASE + 0x28B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_232_REG (IPP_KLT_BASE + 0x28BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_233_REG (IPP_KLT_BASE + 0x28C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_234_REG (IPP_KLT_BASE + 0x28CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_235_REG (IPP_KLT_BASE + 0x28D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_236_REG (IPP_KLT_BASE + 0x28DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_237_REG (IPP_KLT_BASE + 0x28E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_238_REG (IPP_KLT_BASE + 0x28EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_239_REG (IPP_KLT_BASE + 0x28F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_240_REG (IPP_KLT_BASE + 0x28FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_241_REG (IPP_KLT_BASE + 0x2904)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_242_REG (IPP_KLT_BASE + 0x290C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_243_REG (IPP_KLT_BASE + 0x2914)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_244_REG (IPP_KLT_BASE + 0x291C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_245_REG (IPP_KLT_BASE + 0x2924)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_246_REG (IPP_KLT_BASE + 0x292C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_247_REG (IPP_KLT_BASE + 0x2934)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_248_REG (IPP_KLT_BASE + 0x293C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_249_REG (IPP_KLT_BASE + 0x2944)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_250_REG (IPP_KLT_BASE + 0x294C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_251_REG (IPP_KLT_BASE + 0x2954)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_252_REG (IPP_KLT_BASE + 0x295C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_253_REG (IPP_KLT_BASE + 0x2964)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_254_REG (IPP_KLT_BASE + 0x296C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_255_REG (IPP_KLT_BASE + 0x2974)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_256_REG (IPP_KLT_BASE + 0x297C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_257_REG (IPP_KLT_BASE + 0x2984)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_258_REG (IPP_KLT_BASE + 0x298C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_259_REG (IPP_KLT_BASE + 0x2994)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_260_REG (IPP_KLT_BASE + 0x299C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_261_REG (IPP_KLT_BASE + 0x29A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_262_REG (IPP_KLT_BASE + 0x29AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_263_REG (IPP_KLT_BASE + 0x29B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_264_REG (IPP_KLT_BASE + 0x29BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_265_REG (IPP_KLT_BASE + 0x29C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_266_REG (IPP_KLT_BASE + 0x29CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_267_REG (IPP_KLT_BASE + 0x29D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_268_REG (IPP_KLT_BASE + 0x29DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_269_REG (IPP_KLT_BASE + 0x29E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_270_REG (IPP_KLT_BASE + 0x29EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_271_REG (IPP_KLT_BASE + 0x29F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_272_REG (IPP_KLT_BASE + 0x29FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_273_REG (IPP_KLT_BASE + 0x2A04)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_274_REG (IPP_KLT_BASE + 0x2A0C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_275_REG (IPP_KLT_BASE + 0x2A14)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_276_REG (IPP_KLT_BASE + 0x2A1C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_277_REG (IPP_KLT_BASE + 0x2A24)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_278_REG (IPP_KLT_BASE + 0x2A2C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_279_REG (IPP_KLT_BASE + 0x2A34)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_280_REG (IPP_KLT_BASE + 0x2A3C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_281_REG (IPP_KLT_BASE + 0x2A44)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_282_REG (IPP_KLT_BASE + 0x2A4C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_283_REG (IPP_KLT_BASE + 0x2A54)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_284_REG (IPP_KLT_BASE + 0x2A5C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_285_REG (IPP_KLT_BASE + 0x2A64)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_286_REG (IPP_KLT_BASE + 0x2A6C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_287_REG (IPP_KLT_BASE + 0x2A74)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_288_REG (IPP_KLT_BASE + 0x2A7C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_289_REG (IPP_KLT_BASE + 0x2A84)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_290_REG (IPP_KLT_BASE + 0x2A8C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_291_REG (IPP_KLT_BASE + 0x2A94)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_292_REG (IPP_KLT_BASE + 0x2A9C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_293_REG (IPP_KLT_BASE + 0x2AA4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_294_REG (IPP_KLT_BASE + 0x2AAC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_295_REG (IPP_KLT_BASE + 0x2AB4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_296_REG (IPP_KLT_BASE + 0x2ABC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_297_REG (IPP_KLT_BASE + 0x2AC4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_298_REG (IPP_KLT_BASE + 0x2ACC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_299_REG (IPP_KLT_BASE + 0x2AD4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_300_REG (IPP_KLT_BASE + 0x2ADC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_301_REG (IPP_KLT_BASE + 0x2AE4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_302_REG (IPP_KLT_BASE + 0x2AEC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_303_REG (IPP_KLT_BASE + 0x2AF4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_304_REG (IPP_KLT_BASE + 0x2AFC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_305_REG (IPP_KLT_BASE + 0x2B04)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_306_REG (IPP_KLT_BASE + 0x2B0C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_307_REG (IPP_KLT_BASE + 0x2B14)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_308_REG (IPP_KLT_BASE + 0x2B1C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_309_REG (IPP_KLT_BASE + 0x2B24)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_310_REG (IPP_KLT_BASE + 0x2B2C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_311_REG (IPP_KLT_BASE + 0x2B34)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_312_REG (IPP_KLT_BASE + 0x2B3C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_313_REG (IPP_KLT_BASE + 0x2B44)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_314_REG (IPP_KLT_BASE + 0x2B4C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_315_REG (IPP_KLT_BASE + 0x2B54)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_316_REG (IPP_KLT_BASE + 0x2B5C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_317_REG (IPP_KLT_BASE + 0x2B64)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_318_REG (IPP_KLT_BASE + 0x2B6C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_319_REG (IPP_KLT_BASE + 0x2B74)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_320_REG (IPP_KLT_BASE + 0x2B7C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_321_REG (IPP_KLT_BASE + 0x2B84)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_322_REG (IPP_KLT_BASE + 0x2B8C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_323_REG (IPP_KLT_BASE + 0x2B94)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_324_REG (IPP_KLT_BASE + 0x2B9C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_325_REG (IPP_KLT_BASE + 0x2BA4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_326_REG (IPP_KLT_BASE + 0x2BAC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_327_REG (IPP_KLT_BASE + 0x2BB4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_328_REG (IPP_KLT_BASE + 0x2BBC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_329_REG (IPP_KLT_BASE + 0x2BC4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_330_REG (IPP_KLT_BASE + 0x2BCC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_331_REG (IPP_KLT_BASE + 0x2BD4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_332_REG (IPP_KLT_BASE + 0x2BDC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_333_REG (IPP_KLT_BASE + 0x2BE4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_334_REG (IPP_KLT_BASE + 0x2BEC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_335_REG (IPP_KLT_BASE + 0x2BF4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_336_REG (IPP_KLT_BASE + 0x2BFC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_337_REG (IPP_KLT_BASE + 0x2C04)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_338_REG (IPP_KLT_BASE + 0x2C0C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_339_REG (IPP_KLT_BASE + 0x2C14)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_340_REG (IPP_KLT_BASE + 0x2C1C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_341_REG (IPP_KLT_BASE + 0x2C24)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_342_REG (IPP_KLT_BASE + 0x2C2C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_343_REG (IPP_KLT_BASE + 0x2C34)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_344_REG (IPP_KLT_BASE + 0x2C3C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_345_REG (IPP_KLT_BASE + 0x2C44)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_346_REG (IPP_KLT_BASE + 0x2C4C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_347_REG (IPP_KLT_BASE + 0x2C54)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_348_REG (IPP_KLT_BASE + 0x2C5C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_349_REG (IPP_KLT_BASE + 0x2C64)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_350_REG (IPP_KLT_BASE + 0x2C6C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_351_REG (IPP_KLT_BASE + 0x2C74)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_352_REG (IPP_KLT_BASE + 0x2C7C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_353_REG (IPP_KLT_BASE + 0x2C84)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_354_REG (IPP_KLT_BASE + 0x2C8C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_355_REG (IPP_KLT_BASE + 0x2C94)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_356_REG (IPP_KLT_BASE + 0x2C9C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_357_REG (IPP_KLT_BASE + 0x2CA4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_358_REG (IPP_KLT_BASE + 0x2CAC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_359_REG (IPP_KLT_BASE + 0x2CB4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_360_REG (IPP_KLT_BASE + 0x2CBC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_361_REG (IPP_KLT_BASE + 0x2CC4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_362_REG (IPP_KLT_BASE + 0x2CCC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_363_REG (IPP_KLT_BASE + 0x2CD4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_364_REG (IPP_KLT_BASE + 0x2CDC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_365_REG (IPP_KLT_BASE + 0x2CE4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_366_REG (IPP_KLT_BASE + 0x2CEC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_367_REG (IPP_KLT_BASE + 0x2CF4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_368_REG (IPP_KLT_BASE + 0x2CFC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_369_REG (IPP_KLT_BASE + 0x2D04)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_370_REG (IPP_KLT_BASE + 0x2D0C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_371_REG (IPP_KLT_BASE + 0x2D14)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_372_REG (IPP_KLT_BASE + 0x2D1C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_373_REG (IPP_KLT_BASE + 0x2D24)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_374_REG (IPP_KLT_BASE + 0x2D2C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_375_REG (IPP_KLT_BASE + 0x2D34)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_376_REG (IPP_KLT_BASE + 0x2D3C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_377_REG (IPP_KLT_BASE + 0x2D44)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_378_REG (IPP_KLT_BASE + 0x2D4C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_379_REG (IPP_KLT_BASE + 0x2D54)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_380_REG (IPP_KLT_BASE + 0x2D5C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_381_REG (IPP_KLT_BASE + 0x2D64)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_382_REG (IPP_KLT_BASE + 0x2D6C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_383_REG (IPP_KLT_BASE + 0x2D74)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_384_REG (IPP_KLT_BASE + 0x2D7C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_385_REG (IPP_KLT_BASE + 0x2D84)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_386_REG (IPP_KLT_BASE + 0x2D8C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_387_REG (IPP_KLT_BASE + 0x2D94)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_388_REG (IPP_KLT_BASE + 0x2D9C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_389_REG (IPP_KLT_BASE + 0x2DA4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_390_REG (IPP_KLT_BASE + 0x2DAC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_391_REG (IPP_KLT_BASE + 0x2DB4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_392_REG (IPP_KLT_BASE + 0x2DBC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_393_REG (IPP_KLT_BASE + 0x2DC4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_394_REG (IPP_KLT_BASE + 0x2DCC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_395_REG (IPP_KLT_BASE + 0x2DD4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_396_REG (IPP_KLT_BASE + 0x2DDC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_397_REG (IPP_KLT_BASE + 0x2DE4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_398_REG (IPP_KLT_BASE + 0x2DEC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_399_REG (IPP_KLT_BASE + 0x2DF4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_400_REG (IPP_KLT_BASE + 0x2DFC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_401_REG (IPP_KLT_BASE + 0x2E04)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_402_REG (IPP_KLT_BASE + 0x2E0C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_403_REG (IPP_KLT_BASE + 0x2E14)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_404_REG (IPP_KLT_BASE + 0x2E1C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_405_REG (IPP_KLT_BASE + 0x2E24)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_406_REG (IPP_KLT_BASE + 0x2E2C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_407_REG (IPP_KLT_BASE + 0x2E34)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_408_REG (IPP_KLT_BASE + 0x2E3C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_409_REG (IPP_KLT_BASE + 0x2E44)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_410_REG (IPP_KLT_BASE + 0x2E4C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_411_REG (IPP_KLT_BASE + 0x2E54)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_412_REG (IPP_KLT_BASE + 0x2E5C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_413_REG (IPP_KLT_BASE + 0x2E64)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_414_REG (IPP_KLT_BASE + 0x2E6C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_415_REG (IPP_KLT_BASE + 0x2E74)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_416_REG (IPP_KLT_BASE + 0x2E7C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_417_REG (IPP_KLT_BASE + 0x2E84)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_418_REG (IPP_KLT_BASE + 0x2E8C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_419_REG (IPP_KLT_BASE + 0x2E94)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_420_REG (IPP_KLT_BASE + 0x2E9C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_421_REG (IPP_KLT_BASE + 0x2EA4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_422_REG (IPP_KLT_BASE + 0x2EAC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_423_REG (IPP_KLT_BASE + 0x2EB4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_424_REG (IPP_KLT_BASE + 0x2EBC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_425_REG (IPP_KLT_BASE + 0x2EC4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_426_REG (IPP_KLT_BASE + 0x2ECC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_427_REG (IPP_KLT_BASE + 0x2ED4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_428_REG (IPP_KLT_BASE + 0x2EDC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_429_REG (IPP_KLT_BASE + 0x2EE4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_430_REG (IPP_KLT_BASE + 0x2EEC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_431_REG (IPP_KLT_BASE + 0x2EF4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_432_REG (IPP_KLT_BASE + 0x2EFC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_433_REG (IPP_KLT_BASE + 0x2F04)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_434_REG (IPP_KLT_BASE + 0x2F0C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_435_REG (IPP_KLT_BASE + 0x2F14)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_436_REG (IPP_KLT_BASE + 0x2F1C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_437_REG (IPP_KLT_BASE + 0x2F24)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_438_REG (IPP_KLT_BASE + 0x2F2C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_439_REG (IPP_KLT_BASE + 0x2F34)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_440_REG (IPP_KLT_BASE + 0x2F3C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_441_REG (IPP_KLT_BASE + 0x2F44)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_442_REG (IPP_KLT_BASE + 0x2F4C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_443_REG (IPP_KLT_BASE + 0x2F54)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_444_REG (IPP_KLT_BASE + 0x2F5C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_445_REG (IPP_KLT_BASE + 0x2F64)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_446_REG (IPP_KLT_BASE + 0x2F6C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_447_REG (IPP_KLT_BASE + 0x2F74)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_448_REG (IPP_KLT_BASE + 0x2F7C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_449_REG (IPP_KLT_BASE + 0x2F84)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_450_REG (IPP_KLT_BASE + 0x2F8C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_451_REG (IPP_KLT_BASE + 0x2F94)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_452_REG (IPP_KLT_BASE + 0x2F9C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_453_REG (IPP_KLT_BASE + 0x2FA4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_454_REG (IPP_KLT_BASE + 0x2FAC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_455_REG (IPP_KLT_BASE + 0x2FB4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_456_REG (IPP_KLT_BASE + 0x2FBC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_457_REG (IPP_KLT_BASE + 0x2FC4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_458_REG (IPP_KLT_BASE + 0x2FCC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_459_REG (IPP_KLT_BASE + 0x2FD4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_460_REG (IPP_KLT_BASE + 0x2FDC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_461_REG (IPP_KLT_BASE + 0x2FE4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_462_REG (IPP_KLT_BASE + 0x2FEC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_463_REG (IPP_KLT_BASE + 0x2FF4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_464_REG (IPP_KLT_BASE + 0x2FFC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_465_REG (IPP_KLT_BASE + 0x3004)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_466_REG (IPP_KLT_BASE + 0x300C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_467_REG (IPP_KLT_BASE + 0x3014)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_468_REG (IPP_KLT_BASE + 0x301C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_469_REG (IPP_KLT_BASE + 0x3024)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_470_REG (IPP_KLT_BASE + 0x302C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_471_REG (IPP_KLT_BASE + 0x3034)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_472_REG (IPP_KLT_BASE + 0x303C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_473_REG (IPP_KLT_BASE + 0x3044)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_474_REG (IPP_KLT_BASE + 0x304C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_475_REG (IPP_KLT_BASE + 0x3054)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_476_REG (IPP_KLT_BASE + 0x305C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_477_REG (IPP_KLT_BASE + 0x3064)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_478_REG (IPP_KLT_BASE + 0x306C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_479_REG (IPP_KLT_BASE + 0x3074)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_480_REG (IPP_KLT_BASE + 0x307C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_481_REG (IPP_KLT_BASE + 0x3084)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_482_REG (IPP_KLT_BASE + 0x308C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_483_REG (IPP_KLT_BASE + 0x3094)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_484_REG (IPP_KLT_BASE + 0x309C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_485_REG (IPP_KLT_BASE + 0x30A4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_486_REG (IPP_KLT_BASE + 0x30AC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_487_REG (IPP_KLT_BASE + 0x30B4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_488_REG (IPP_KLT_BASE + 0x30BC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_489_REG (IPP_KLT_BASE + 0x30C4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_490_REG (IPP_KLT_BASE + 0x30CC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_491_REG (IPP_KLT_BASE + 0x30D4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_492_REG (IPP_KLT_BASE + 0x30DC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_493_REG (IPP_KLT_BASE + 0x30E4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_494_REG (IPP_KLT_BASE + 0x30EC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_495_REG (IPP_KLT_BASE + 0x30F4)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_496_REG (IPP_KLT_BASE + 0x30FC)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_497_REG (IPP_KLT_BASE + 0x3104)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_498_REG (IPP_KLT_BASE + 0x310C)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_X_499_REG (IPP_KLT_BASE + 0x3114)   /* Buffer of KP next x coordinates */
#define IPP_KLT_KP_NEXT_BUFF_Y_0_REG (IPP_KLT_BASE + 0x2180)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_1_REG (IPP_KLT_BASE + 0x2188)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_2_REG (IPP_KLT_BASE + 0x2190)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_3_REG (IPP_KLT_BASE + 0x2198)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_4_REG (IPP_KLT_BASE + 0x21A0)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_5_REG (IPP_KLT_BASE + 0x21A8)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_6_REG (IPP_KLT_BASE + 0x21B0)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_7_REG (IPP_KLT_BASE + 0x21B8)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_8_REG (IPP_KLT_BASE + 0x21C0)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_9_REG (IPP_KLT_BASE + 0x21C8)     /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_10_REG (IPP_KLT_BASE + 0x21D0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_11_REG (IPP_KLT_BASE + 0x21D8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_12_REG (IPP_KLT_BASE + 0x21E0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_13_REG (IPP_KLT_BASE + 0x21E8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_14_REG (IPP_KLT_BASE + 0x21F0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_15_REG (IPP_KLT_BASE + 0x21F8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_16_REG (IPP_KLT_BASE + 0x2200)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_17_REG (IPP_KLT_BASE + 0x2208)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_18_REG (IPP_KLT_BASE + 0x2210)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_19_REG (IPP_KLT_BASE + 0x2218)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_20_REG (IPP_KLT_BASE + 0x2220)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_21_REG (IPP_KLT_BASE + 0x2228)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_22_REG (IPP_KLT_BASE + 0x2230)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_23_REG (IPP_KLT_BASE + 0x2238)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_24_REG (IPP_KLT_BASE + 0x2240)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_25_REG (IPP_KLT_BASE + 0x2248)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_26_REG (IPP_KLT_BASE + 0x2250)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_27_REG (IPP_KLT_BASE + 0x2258)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_28_REG (IPP_KLT_BASE + 0x2260)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_29_REG (IPP_KLT_BASE + 0x2268)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_30_REG (IPP_KLT_BASE + 0x2270)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_31_REG (IPP_KLT_BASE + 0x2278)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_32_REG (IPP_KLT_BASE + 0x2280)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_33_REG (IPP_KLT_BASE + 0x2288)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_34_REG (IPP_KLT_BASE + 0x2290)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_35_REG (IPP_KLT_BASE + 0x2298)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_36_REG (IPP_KLT_BASE + 0x22A0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_37_REG (IPP_KLT_BASE + 0x22A8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_38_REG (IPP_KLT_BASE + 0x22B0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_39_REG (IPP_KLT_BASE + 0x22B8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_40_REG (IPP_KLT_BASE + 0x22C0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_41_REG (IPP_KLT_BASE + 0x22C8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_42_REG (IPP_KLT_BASE + 0x22D0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_43_REG (IPP_KLT_BASE + 0x22D8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_44_REG (IPP_KLT_BASE + 0x22E0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_45_REG (IPP_KLT_BASE + 0x22E8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_46_REG (IPP_KLT_BASE + 0x22F0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_47_REG (IPP_KLT_BASE + 0x22F8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_48_REG (IPP_KLT_BASE + 0x2300)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_49_REG (IPP_KLT_BASE + 0x2308)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_50_REG (IPP_KLT_BASE + 0x2310)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_51_REG (IPP_KLT_BASE + 0x2318)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_52_REG (IPP_KLT_BASE + 0x2320)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_53_REG (IPP_KLT_BASE + 0x2328)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_54_REG (IPP_KLT_BASE + 0x2330)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_55_REG (IPP_KLT_BASE + 0x2338)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_56_REG (IPP_KLT_BASE + 0x2340)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_57_REG (IPP_KLT_BASE + 0x2348)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_58_REG (IPP_KLT_BASE + 0x2350)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_59_REG (IPP_KLT_BASE + 0x2358)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_60_REG (IPP_KLT_BASE + 0x2360)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_61_REG (IPP_KLT_BASE + 0x2368)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_62_REG (IPP_KLT_BASE + 0x2370)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_63_REG (IPP_KLT_BASE + 0x2378)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_64_REG (IPP_KLT_BASE + 0x2380)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_65_REG (IPP_KLT_BASE + 0x2388)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_66_REG (IPP_KLT_BASE + 0x2390)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_67_REG (IPP_KLT_BASE + 0x2398)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_68_REG (IPP_KLT_BASE + 0x23A0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_69_REG (IPP_KLT_BASE + 0x23A8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_70_REG (IPP_KLT_BASE + 0x23B0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_71_REG (IPP_KLT_BASE + 0x23B8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_72_REG (IPP_KLT_BASE + 0x23C0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_73_REG (IPP_KLT_BASE + 0x23C8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_74_REG (IPP_KLT_BASE + 0x23D0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_75_REG (IPP_KLT_BASE + 0x23D8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_76_REG (IPP_KLT_BASE + 0x23E0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_77_REG (IPP_KLT_BASE + 0x23E8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_78_REG (IPP_KLT_BASE + 0x23F0)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_79_REG (IPP_KLT_BASE + 0x23F8)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_80_REG (IPP_KLT_BASE + 0x2400)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_81_REG (IPP_KLT_BASE + 0x2408)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_82_REG (IPP_KLT_BASE + 0x2410)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_83_REG (IPP_KLT_BASE + 0x2418)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_84_REG (IPP_KLT_BASE + 0x2420)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_85_REG (IPP_KLT_BASE + 0x2428)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_86_REG (IPP_KLT_BASE + 0x2430)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_87_REG (IPP_KLT_BASE + 0x2438)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_88_REG (IPP_KLT_BASE + 0x2440)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_89_REG (IPP_KLT_BASE + 0x2448)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_90_REG (IPP_KLT_BASE + 0x2450)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_91_REG (IPP_KLT_BASE + 0x2458)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_92_REG (IPP_KLT_BASE + 0x2460)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_93_REG (IPP_KLT_BASE + 0x2468)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_94_REG (IPP_KLT_BASE + 0x2470)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_95_REG (IPP_KLT_BASE + 0x2478)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_96_REG (IPP_KLT_BASE + 0x2480)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_97_REG (IPP_KLT_BASE + 0x2488)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_98_REG (IPP_KLT_BASE + 0x2490)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_99_REG (IPP_KLT_BASE + 0x2498)    /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_100_REG (IPP_KLT_BASE + 0x24A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_101_REG (IPP_KLT_BASE + 0x24A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_102_REG (IPP_KLT_BASE + 0x24B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_103_REG (IPP_KLT_BASE + 0x24B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_104_REG (IPP_KLT_BASE + 0x24C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_105_REG (IPP_KLT_BASE + 0x24C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_106_REG (IPP_KLT_BASE + 0x24D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_107_REG (IPP_KLT_BASE + 0x24D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_108_REG (IPP_KLT_BASE + 0x24E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_109_REG (IPP_KLT_BASE + 0x24E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_110_REG (IPP_KLT_BASE + 0x24F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_111_REG (IPP_KLT_BASE + 0x24F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_112_REG (IPP_KLT_BASE + 0x2500)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_113_REG (IPP_KLT_BASE + 0x2508)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_114_REG (IPP_KLT_BASE + 0x2510)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_115_REG (IPP_KLT_BASE + 0x2518)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_116_REG (IPP_KLT_BASE + 0x2520)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_117_REG (IPP_KLT_BASE + 0x2528)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_118_REG (IPP_KLT_BASE + 0x2530)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_119_REG (IPP_KLT_BASE + 0x2538)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_120_REG (IPP_KLT_BASE + 0x2540)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_121_REG (IPP_KLT_BASE + 0x2548)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_122_REG (IPP_KLT_BASE + 0x2550)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_123_REG (IPP_KLT_BASE + 0x2558)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_124_REG (IPP_KLT_BASE + 0x2560)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_125_REG (IPP_KLT_BASE + 0x2568)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_126_REG (IPP_KLT_BASE + 0x2570)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_127_REG (IPP_KLT_BASE + 0x2578)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_128_REG (IPP_KLT_BASE + 0x2580)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_129_REG (IPP_KLT_BASE + 0x2588)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_130_REG (IPP_KLT_BASE + 0x2590)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_131_REG (IPP_KLT_BASE + 0x2598)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_132_REG (IPP_KLT_BASE + 0x25A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_133_REG (IPP_KLT_BASE + 0x25A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_134_REG (IPP_KLT_BASE + 0x25B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_135_REG (IPP_KLT_BASE + 0x25B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_136_REG (IPP_KLT_BASE + 0x25C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_137_REG (IPP_KLT_BASE + 0x25C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_138_REG (IPP_KLT_BASE + 0x25D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_139_REG (IPP_KLT_BASE + 0x25D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_140_REG (IPP_KLT_BASE + 0x25E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_141_REG (IPP_KLT_BASE + 0x25E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_142_REG (IPP_KLT_BASE + 0x25F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_143_REG (IPP_KLT_BASE + 0x25F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_144_REG (IPP_KLT_BASE + 0x2600)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_145_REG (IPP_KLT_BASE + 0x2608)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_146_REG (IPP_KLT_BASE + 0x2610)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_147_REG (IPP_KLT_BASE + 0x2618)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_148_REG (IPP_KLT_BASE + 0x2620)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_149_REG (IPP_KLT_BASE + 0x2628)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_150_REG (IPP_KLT_BASE + 0x2630)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_151_REG (IPP_KLT_BASE + 0x2638)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_152_REG (IPP_KLT_BASE + 0x2640)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_153_REG (IPP_KLT_BASE + 0x2648)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_154_REG (IPP_KLT_BASE + 0x2650)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_155_REG (IPP_KLT_BASE + 0x2658)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_156_REG (IPP_KLT_BASE + 0x2660)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_157_REG (IPP_KLT_BASE + 0x2668)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_158_REG (IPP_KLT_BASE + 0x2670)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_159_REG (IPP_KLT_BASE + 0x2678)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_160_REG (IPP_KLT_BASE + 0x2680)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_161_REG (IPP_KLT_BASE + 0x2688)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_162_REG (IPP_KLT_BASE + 0x2690)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_163_REG (IPP_KLT_BASE + 0x2698)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_164_REG (IPP_KLT_BASE + 0x26A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_165_REG (IPP_KLT_BASE + 0x26A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_166_REG (IPP_KLT_BASE + 0x26B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_167_REG (IPP_KLT_BASE + 0x26B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_168_REG (IPP_KLT_BASE + 0x26C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_169_REG (IPP_KLT_BASE + 0x26C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_170_REG (IPP_KLT_BASE + 0x26D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_171_REG (IPP_KLT_BASE + 0x26D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_172_REG (IPP_KLT_BASE + 0x26E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_173_REG (IPP_KLT_BASE + 0x26E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_174_REG (IPP_KLT_BASE + 0x26F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_175_REG (IPP_KLT_BASE + 0x26F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_176_REG (IPP_KLT_BASE + 0x2700)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_177_REG (IPP_KLT_BASE + 0x2708)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_178_REG (IPP_KLT_BASE + 0x2710)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_179_REG (IPP_KLT_BASE + 0x2718)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_180_REG (IPP_KLT_BASE + 0x2720)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_181_REG (IPP_KLT_BASE + 0x2728)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_182_REG (IPP_KLT_BASE + 0x2730)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_183_REG (IPP_KLT_BASE + 0x2738)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_184_REG (IPP_KLT_BASE + 0x2740)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_185_REG (IPP_KLT_BASE + 0x2748)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_186_REG (IPP_KLT_BASE + 0x2750)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_187_REG (IPP_KLT_BASE + 0x2758)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_188_REG (IPP_KLT_BASE + 0x2760)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_189_REG (IPP_KLT_BASE + 0x2768)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_190_REG (IPP_KLT_BASE + 0x2770)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_191_REG (IPP_KLT_BASE + 0x2778)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_192_REG (IPP_KLT_BASE + 0x2780)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_193_REG (IPP_KLT_BASE + 0x2788)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_194_REG (IPP_KLT_BASE + 0x2790)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_195_REG (IPP_KLT_BASE + 0x2798)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_196_REG (IPP_KLT_BASE + 0x27A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_197_REG (IPP_KLT_BASE + 0x27A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_198_REG (IPP_KLT_BASE + 0x27B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_199_REG (IPP_KLT_BASE + 0x27B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_200_REG (IPP_KLT_BASE + 0x27C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_201_REG (IPP_KLT_BASE + 0x27C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_202_REG (IPP_KLT_BASE + 0x27D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_203_REG (IPP_KLT_BASE + 0x27D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_204_REG (IPP_KLT_BASE + 0x27E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_205_REG (IPP_KLT_BASE + 0x27E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_206_REG (IPP_KLT_BASE + 0x27F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_207_REG (IPP_KLT_BASE + 0x27F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_208_REG (IPP_KLT_BASE + 0x2800)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_209_REG (IPP_KLT_BASE + 0x2808)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_210_REG (IPP_KLT_BASE + 0x2810)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_211_REG (IPP_KLT_BASE + 0x2818)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_212_REG (IPP_KLT_BASE + 0x2820)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_213_REG (IPP_KLT_BASE + 0x2828)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_214_REG (IPP_KLT_BASE + 0x2830)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_215_REG (IPP_KLT_BASE + 0x2838)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_216_REG (IPP_KLT_BASE + 0x2840)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_217_REG (IPP_KLT_BASE + 0x2848)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_218_REG (IPP_KLT_BASE + 0x2850)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_219_REG (IPP_KLT_BASE + 0x2858)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_220_REG (IPP_KLT_BASE + 0x2860)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_221_REG (IPP_KLT_BASE + 0x2868)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_222_REG (IPP_KLT_BASE + 0x2870)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_223_REG (IPP_KLT_BASE + 0x2878)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_224_REG (IPP_KLT_BASE + 0x2880)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_225_REG (IPP_KLT_BASE + 0x2888)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_226_REG (IPP_KLT_BASE + 0x2890)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_227_REG (IPP_KLT_BASE + 0x2898)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_228_REG (IPP_KLT_BASE + 0x28A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_229_REG (IPP_KLT_BASE + 0x28A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_230_REG (IPP_KLT_BASE + 0x28B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_231_REG (IPP_KLT_BASE + 0x28B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_232_REG (IPP_KLT_BASE + 0x28C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_233_REG (IPP_KLT_BASE + 0x28C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_234_REG (IPP_KLT_BASE + 0x28D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_235_REG (IPP_KLT_BASE + 0x28D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_236_REG (IPP_KLT_BASE + 0x28E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_237_REG (IPP_KLT_BASE + 0x28E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_238_REG (IPP_KLT_BASE + 0x28F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_239_REG (IPP_KLT_BASE + 0x28F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_240_REG (IPP_KLT_BASE + 0x2900)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_241_REG (IPP_KLT_BASE + 0x2908)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_242_REG (IPP_KLT_BASE + 0x2910)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_243_REG (IPP_KLT_BASE + 0x2918)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_244_REG (IPP_KLT_BASE + 0x2920)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_245_REG (IPP_KLT_BASE + 0x2928)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_246_REG (IPP_KLT_BASE + 0x2930)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_247_REG (IPP_KLT_BASE + 0x2938)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_248_REG (IPP_KLT_BASE + 0x2940)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_249_REG (IPP_KLT_BASE + 0x2948)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_250_REG (IPP_KLT_BASE + 0x2950)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_251_REG (IPP_KLT_BASE + 0x2958)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_252_REG (IPP_KLT_BASE + 0x2960)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_253_REG (IPP_KLT_BASE + 0x2968)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_254_REG (IPP_KLT_BASE + 0x2970)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_255_REG (IPP_KLT_BASE + 0x2978)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_256_REG (IPP_KLT_BASE + 0x2980)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_257_REG (IPP_KLT_BASE + 0x2988)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_258_REG (IPP_KLT_BASE + 0x2990)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_259_REG (IPP_KLT_BASE + 0x2998)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_260_REG (IPP_KLT_BASE + 0x29A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_261_REG (IPP_KLT_BASE + 0x29A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_262_REG (IPP_KLT_BASE + 0x29B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_263_REG (IPP_KLT_BASE + 0x29B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_264_REG (IPP_KLT_BASE + 0x29C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_265_REG (IPP_KLT_BASE + 0x29C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_266_REG (IPP_KLT_BASE + 0x29D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_267_REG (IPP_KLT_BASE + 0x29D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_268_REG (IPP_KLT_BASE + 0x29E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_269_REG (IPP_KLT_BASE + 0x29E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_270_REG (IPP_KLT_BASE + 0x29F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_271_REG (IPP_KLT_BASE + 0x29F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_272_REG (IPP_KLT_BASE + 0x2A00)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_273_REG (IPP_KLT_BASE + 0x2A08)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_274_REG (IPP_KLT_BASE + 0x2A10)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_275_REG (IPP_KLT_BASE + 0x2A18)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_276_REG (IPP_KLT_BASE + 0x2A20)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_277_REG (IPP_KLT_BASE + 0x2A28)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_278_REG (IPP_KLT_BASE + 0x2A30)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_279_REG (IPP_KLT_BASE + 0x2A38)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_280_REG (IPP_KLT_BASE + 0x2A40)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_281_REG (IPP_KLT_BASE + 0x2A48)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_282_REG (IPP_KLT_BASE + 0x2A50)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_283_REG (IPP_KLT_BASE + 0x2A58)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_284_REG (IPP_KLT_BASE + 0x2A60)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_285_REG (IPP_KLT_BASE + 0x2A68)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_286_REG (IPP_KLT_BASE + 0x2A70)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_287_REG (IPP_KLT_BASE + 0x2A78)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_288_REG (IPP_KLT_BASE + 0x2A80)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_289_REG (IPP_KLT_BASE + 0x2A88)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_290_REG (IPP_KLT_BASE + 0x2A90)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_291_REG (IPP_KLT_BASE + 0x2A98)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_292_REG (IPP_KLT_BASE + 0x2AA0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_293_REG (IPP_KLT_BASE + 0x2AA8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_294_REG (IPP_KLT_BASE + 0x2AB0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_295_REG (IPP_KLT_BASE + 0x2AB8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_296_REG (IPP_KLT_BASE + 0x2AC0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_297_REG (IPP_KLT_BASE + 0x2AC8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_298_REG (IPP_KLT_BASE + 0x2AD0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_299_REG (IPP_KLT_BASE + 0x2AD8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_300_REG (IPP_KLT_BASE + 0x2AE0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_301_REG (IPP_KLT_BASE + 0x2AE8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_302_REG (IPP_KLT_BASE + 0x2AF0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_303_REG (IPP_KLT_BASE + 0x2AF8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_304_REG (IPP_KLT_BASE + 0x2B00)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_305_REG (IPP_KLT_BASE + 0x2B08)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_306_REG (IPP_KLT_BASE + 0x2B10)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_307_REG (IPP_KLT_BASE + 0x2B18)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_308_REG (IPP_KLT_BASE + 0x2B20)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_309_REG (IPP_KLT_BASE + 0x2B28)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_310_REG (IPP_KLT_BASE + 0x2B30)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_311_REG (IPP_KLT_BASE + 0x2B38)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_312_REG (IPP_KLT_BASE + 0x2B40)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_313_REG (IPP_KLT_BASE + 0x2B48)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_314_REG (IPP_KLT_BASE + 0x2B50)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_315_REG (IPP_KLT_BASE + 0x2B58)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_316_REG (IPP_KLT_BASE + 0x2B60)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_317_REG (IPP_KLT_BASE + 0x2B68)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_318_REG (IPP_KLT_BASE + 0x2B70)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_319_REG (IPP_KLT_BASE + 0x2B78)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_320_REG (IPP_KLT_BASE + 0x2B80)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_321_REG (IPP_KLT_BASE + 0x2B88)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_322_REG (IPP_KLT_BASE + 0x2B90)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_323_REG (IPP_KLT_BASE + 0x2B98)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_324_REG (IPP_KLT_BASE + 0x2BA0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_325_REG (IPP_KLT_BASE + 0x2BA8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_326_REG (IPP_KLT_BASE + 0x2BB0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_327_REG (IPP_KLT_BASE + 0x2BB8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_328_REG (IPP_KLT_BASE + 0x2BC0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_329_REG (IPP_KLT_BASE + 0x2BC8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_330_REG (IPP_KLT_BASE + 0x2BD0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_331_REG (IPP_KLT_BASE + 0x2BD8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_332_REG (IPP_KLT_BASE + 0x2BE0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_333_REG (IPP_KLT_BASE + 0x2BE8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_334_REG (IPP_KLT_BASE + 0x2BF0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_335_REG (IPP_KLT_BASE + 0x2BF8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_336_REG (IPP_KLT_BASE + 0x2C00)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_337_REG (IPP_KLT_BASE + 0x2C08)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_338_REG (IPP_KLT_BASE + 0x2C10)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_339_REG (IPP_KLT_BASE + 0x2C18)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_340_REG (IPP_KLT_BASE + 0x2C20)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_341_REG (IPP_KLT_BASE + 0x2C28)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_342_REG (IPP_KLT_BASE + 0x2C30)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_343_REG (IPP_KLT_BASE + 0x2C38)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_344_REG (IPP_KLT_BASE + 0x2C40)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_345_REG (IPP_KLT_BASE + 0x2C48)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_346_REG (IPP_KLT_BASE + 0x2C50)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_347_REG (IPP_KLT_BASE + 0x2C58)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_348_REG (IPP_KLT_BASE + 0x2C60)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_349_REG (IPP_KLT_BASE + 0x2C68)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_350_REG (IPP_KLT_BASE + 0x2C70)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_351_REG (IPP_KLT_BASE + 0x2C78)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_352_REG (IPP_KLT_BASE + 0x2C80)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_353_REG (IPP_KLT_BASE + 0x2C88)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_354_REG (IPP_KLT_BASE + 0x2C90)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_355_REG (IPP_KLT_BASE + 0x2C98)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_356_REG (IPP_KLT_BASE + 0x2CA0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_357_REG (IPP_KLT_BASE + 0x2CA8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_358_REG (IPP_KLT_BASE + 0x2CB0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_359_REG (IPP_KLT_BASE + 0x2CB8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_360_REG (IPP_KLT_BASE + 0x2CC0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_361_REG (IPP_KLT_BASE + 0x2CC8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_362_REG (IPP_KLT_BASE + 0x2CD0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_363_REG (IPP_KLT_BASE + 0x2CD8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_364_REG (IPP_KLT_BASE + 0x2CE0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_365_REG (IPP_KLT_BASE + 0x2CE8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_366_REG (IPP_KLT_BASE + 0x2CF0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_367_REG (IPP_KLT_BASE + 0x2CF8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_368_REG (IPP_KLT_BASE + 0x2D00)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_369_REG (IPP_KLT_BASE + 0x2D08)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_370_REG (IPP_KLT_BASE + 0x2D10)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_371_REG (IPP_KLT_BASE + 0x2D18)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_372_REG (IPP_KLT_BASE + 0x2D20)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_373_REG (IPP_KLT_BASE + 0x2D28)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_374_REG (IPP_KLT_BASE + 0x2D30)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_375_REG (IPP_KLT_BASE + 0x2D38)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_376_REG (IPP_KLT_BASE + 0x2D40)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_377_REG (IPP_KLT_BASE + 0x2D48)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_378_REG (IPP_KLT_BASE + 0x2D50)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_379_REG (IPP_KLT_BASE + 0x2D58)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_380_REG (IPP_KLT_BASE + 0x2D60)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_381_REG (IPP_KLT_BASE + 0x2D68)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_382_REG (IPP_KLT_BASE + 0x2D70)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_383_REG (IPP_KLT_BASE + 0x2D78)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_384_REG (IPP_KLT_BASE + 0x2D80)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_385_REG (IPP_KLT_BASE + 0x2D88)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_386_REG (IPP_KLT_BASE + 0x2D90)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_387_REG (IPP_KLT_BASE + 0x2D98)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_388_REG (IPP_KLT_BASE + 0x2DA0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_389_REG (IPP_KLT_BASE + 0x2DA8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_390_REG (IPP_KLT_BASE + 0x2DB0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_391_REG (IPP_KLT_BASE + 0x2DB8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_392_REG (IPP_KLT_BASE + 0x2DC0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_393_REG (IPP_KLT_BASE + 0x2DC8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_394_REG (IPP_KLT_BASE + 0x2DD0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_395_REG (IPP_KLT_BASE + 0x2DD8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_396_REG (IPP_KLT_BASE + 0x2DE0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_397_REG (IPP_KLT_BASE + 0x2DE8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_398_REG (IPP_KLT_BASE + 0x2DF0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_399_REG (IPP_KLT_BASE + 0x2DF8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_400_REG (IPP_KLT_BASE + 0x2E00)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_401_REG (IPP_KLT_BASE + 0x2E08)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_402_REG (IPP_KLT_BASE + 0x2E10)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_403_REG (IPP_KLT_BASE + 0x2E18)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_404_REG (IPP_KLT_BASE + 0x2E20)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_405_REG (IPP_KLT_BASE + 0x2E28)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_406_REG (IPP_KLT_BASE + 0x2E30)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_407_REG (IPP_KLT_BASE + 0x2E38)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_408_REG (IPP_KLT_BASE + 0x2E40)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_409_REG (IPP_KLT_BASE + 0x2E48)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_410_REG (IPP_KLT_BASE + 0x2E50)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_411_REG (IPP_KLT_BASE + 0x2E58)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_412_REG (IPP_KLT_BASE + 0x2E60)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_413_REG (IPP_KLT_BASE + 0x2E68)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_414_REG (IPP_KLT_BASE + 0x2E70)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_415_REG (IPP_KLT_BASE + 0x2E78)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_416_REG (IPP_KLT_BASE + 0x2E80)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_417_REG (IPP_KLT_BASE + 0x2E88)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_418_REG (IPP_KLT_BASE + 0x2E90)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_419_REG (IPP_KLT_BASE + 0x2E98)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_420_REG (IPP_KLT_BASE + 0x2EA0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_421_REG (IPP_KLT_BASE + 0x2EA8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_422_REG (IPP_KLT_BASE + 0x2EB0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_423_REG (IPP_KLT_BASE + 0x2EB8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_424_REG (IPP_KLT_BASE + 0x2EC0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_425_REG (IPP_KLT_BASE + 0x2EC8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_426_REG (IPP_KLT_BASE + 0x2ED0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_427_REG (IPP_KLT_BASE + 0x2ED8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_428_REG (IPP_KLT_BASE + 0x2EE0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_429_REG (IPP_KLT_BASE + 0x2EE8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_430_REG (IPP_KLT_BASE + 0x2EF0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_431_REG (IPP_KLT_BASE + 0x2EF8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_432_REG (IPP_KLT_BASE + 0x2F00)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_433_REG (IPP_KLT_BASE + 0x2F08)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_434_REG (IPP_KLT_BASE + 0x2F10)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_435_REG (IPP_KLT_BASE + 0x2F18)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_436_REG (IPP_KLT_BASE + 0x2F20)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_437_REG (IPP_KLT_BASE + 0x2F28)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_438_REG (IPP_KLT_BASE + 0x2F30)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_439_REG (IPP_KLT_BASE + 0x2F38)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_440_REG (IPP_KLT_BASE + 0x2F40)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_441_REG (IPP_KLT_BASE + 0x2F48)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_442_REG (IPP_KLT_BASE + 0x2F50)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_443_REG (IPP_KLT_BASE + 0x2F58)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_444_REG (IPP_KLT_BASE + 0x2F60)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_445_REG (IPP_KLT_BASE + 0x2F68)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_446_REG (IPP_KLT_BASE + 0x2F70)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_447_REG (IPP_KLT_BASE + 0x2F78)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_448_REG (IPP_KLT_BASE + 0x2F80)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_449_REG (IPP_KLT_BASE + 0x2F88)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_450_REG (IPP_KLT_BASE + 0x2F90)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_451_REG (IPP_KLT_BASE + 0x2F98)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_452_REG (IPP_KLT_BASE + 0x2FA0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_453_REG (IPP_KLT_BASE + 0x2FA8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_454_REG (IPP_KLT_BASE + 0x2FB0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_455_REG (IPP_KLT_BASE + 0x2FB8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_456_REG (IPP_KLT_BASE + 0x2FC0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_457_REG (IPP_KLT_BASE + 0x2FC8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_458_REG (IPP_KLT_BASE + 0x2FD0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_459_REG (IPP_KLT_BASE + 0x2FD8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_460_REG (IPP_KLT_BASE + 0x2FE0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_461_REG (IPP_KLT_BASE + 0x2FE8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_462_REG (IPP_KLT_BASE + 0x2FF0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_463_REG (IPP_KLT_BASE + 0x2FF8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_464_REG (IPP_KLT_BASE + 0x3000)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_465_REG (IPP_KLT_BASE + 0x3008)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_466_REG (IPP_KLT_BASE + 0x3010)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_467_REG (IPP_KLT_BASE + 0x3018)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_468_REG (IPP_KLT_BASE + 0x3020)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_469_REG (IPP_KLT_BASE + 0x3028)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_470_REG (IPP_KLT_BASE + 0x3030)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_471_REG (IPP_KLT_BASE + 0x3038)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_472_REG (IPP_KLT_BASE + 0x3040)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_473_REG (IPP_KLT_BASE + 0x3048)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_474_REG (IPP_KLT_BASE + 0x3050)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_475_REG (IPP_KLT_BASE + 0x3058)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_476_REG (IPP_KLT_BASE + 0x3060)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_477_REG (IPP_KLT_BASE + 0x3068)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_478_REG (IPP_KLT_BASE + 0x3070)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_479_REG (IPP_KLT_BASE + 0x3078)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_480_REG (IPP_KLT_BASE + 0x3080)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_481_REG (IPP_KLT_BASE + 0x3088)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_482_REG (IPP_KLT_BASE + 0x3090)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_483_REG (IPP_KLT_BASE + 0x3098)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_484_REG (IPP_KLT_BASE + 0x30A0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_485_REG (IPP_KLT_BASE + 0x30A8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_486_REG (IPP_KLT_BASE + 0x30B0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_487_REG (IPP_KLT_BASE + 0x30B8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_488_REG (IPP_KLT_BASE + 0x30C0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_489_REG (IPP_KLT_BASE + 0x30C8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_490_REG (IPP_KLT_BASE + 0x30D0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_491_REG (IPP_KLT_BASE + 0x30D8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_492_REG (IPP_KLT_BASE + 0x30E0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_493_REG (IPP_KLT_BASE + 0x30E8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_494_REG (IPP_KLT_BASE + 0x30F0)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_495_REG (IPP_KLT_BASE + 0x30F8)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_496_REG (IPP_KLT_BASE + 0x3100)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_497_REG (IPP_KLT_BASE + 0x3108)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_498_REG (IPP_KLT_BASE + 0x3110)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_KP_NEXT_BUFF_Y_499_REG (IPP_KLT_BASE + 0x3118)   /* Buffer of KP next y coordinates + status and conf */
#define IPP_KLT_MC_KP_NB_REG (IPP_KLT_BASE + 0x311C)             /* Number of valid KP */
#define IPP_KLT_MC_NEXT_KP_0_REG (IPP_KLT_BASE + 0x3120)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_1_REG (IPP_KLT_BASE + 0x3128)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_2_REG (IPP_KLT_BASE + 0x3130)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_3_REG (IPP_KLT_BASE + 0x3138)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_4_REG (IPP_KLT_BASE + 0x3140)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_5_REG (IPP_KLT_BASE + 0x3148)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_6_REG (IPP_KLT_BASE + 0x3150)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_7_REG (IPP_KLT_BASE + 0x3158)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_8_REG (IPP_KLT_BASE + 0x3160)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_9_REG (IPP_KLT_BASE + 0x3168)         /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_10_REG (IPP_KLT_BASE + 0x3170)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_11_REG (IPP_KLT_BASE + 0x3178)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_12_REG (IPP_KLT_BASE + 0x3180)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_13_REG (IPP_KLT_BASE + 0x3188)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_14_REG (IPP_KLT_BASE + 0x3190)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_15_REG (IPP_KLT_BASE + 0x3198)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_16_REG (IPP_KLT_BASE + 0x31A0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_17_REG (IPP_KLT_BASE + 0x31A8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_18_REG (IPP_KLT_BASE + 0x31B0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_19_REG (IPP_KLT_BASE + 0x31B8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_20_REG (IPP_KLT_BASE + 0x31C0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_21_REG (IPP_KLT_BASE + 0x31C8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_22_REG (IPP_KLT_BASE + 0x31D0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_23_REG (IPP_KLT_BASE + 0x31D8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_24_REG (IPP_KLT_BASE + 0x31E0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_25_REG (IPP_KLT_BASE + 0x31E8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_26_REG (IPP_KLT_BASE + 0x31F0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_27_REG (IPP_KLT_BASE + 0x31F8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_28_REG (IPP_KLT_BASE + 0x3200)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_29_REG (IPP_KLT_BASE + 0x3208)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_30_REG (IPP_KLT_BASE + 0x3210)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_31_REG (IPP_KLT_BASE + 0x3218)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_32_REG (IPP_KLT_BASE + 0x3220)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_33_REG (IPP_KLT_BASE + 0x3228)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_34_REG (IPP_KLT_BASE + 0x3230)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_35_REG (IPP_KLT_BASE + 0x3238)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_36_REG (IPP_KLT_BASE + 0x3240)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_37_REG (IPP_KLT_BASE + 0x3248)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_38_REG (IPP_KLT_BASE + 0x3250)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_39_REG (IPP_KLT_BASE + 0x3258)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_40_REG (IPP_KLT_BASE + 0x3260)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_41_REG (IPP_KLT_BASE + 0x3268)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_42_REG (IPP_KLT_BASE + 0x3270)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_43_REG (IPP_KLT_BASE + 0x3278)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_44_REG (IPP_KLT_BASE + 0x3280)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_45_REG (IPP_KLT_BASE + 0x3288)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_46_REG (IPP_KLT_BASE + 0x3290)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_47_REG (IPP_KLT_BASE + 0x3298)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_48_REG (IPP_KLT_BASE + 0x32A0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_49_REG (IPP_KLT_BASE + 0x32A8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_50_REG (IPP_KLT_BASE + 0x32B0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_51_REG (IPP_KLT_BASE + 0x32B8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_52_REG (IPP_KLT_BASE + 0x32C0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_53_REG (IPP_KLT_BASE + 0x32C8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_54_REG (IPP_KLT_BASE + 0x32D0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_55_REG (IPP_KLT_BASE + 0x32D8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_56_REG (IPP_KLT_BASE + 0x32E0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_57_REG (IPP_KLT_BASE + 0x32E8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_58_REG (IPP_KLT_BASE + 0x32F0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_59_REG (IPP_KLT_BASE + 0x32F8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_60_REG (IPP_KLT_BASE + 0x3300)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_61_REG (IPP_KLT_BASE + 0x3308)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_62_REG (IPP_KLT_BASE + 0x3310)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_63_REG (IPP_KLT_BASE + 0x3318)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_64_REG (IPP_KLT_BASE + 0x3320)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_65_REG (IPP_KLT_BASE + 0x3328)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_66_REG (IPP_KLT_BASE + 0x3330)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_67_REG (IPP_KLT_BASE + 0x3338)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_68_REG (IPP_KLT_BASE + 0x3340)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_69_REG (IPP_KLT_BASE + 0x3348)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_70_REG (IPP_KLT_BASE + 0x3350)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_71_REG (IPP_KLT_BASE + 0x3358)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_72_REG (IPP_KLT_BASE + 0x3360)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_73_REG (IPP_KLT_BASE + 0x3368)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_74_REG (IPP_KLT_BASE + 0x3370)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_75_REG (IPP_KLT_BASE + 0x3378)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_76_REG (IPP_KLT_BASE + 0x3380)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_77_REG (IPP_KLT_BASE + 0x3388)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_78_REG (IPP_KLT_BASE + 0x3390)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_79_REG (IPP_KLT_BASE + 0x3398)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_80_REG (IPP_KLT_BASE + 0x33A0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_81_REG (IPP_KLT_BASE + 0x33A8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_82_REG (IPP_KLT_BASE + 0x33B0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_83_REG (IPP_KLT_BASE + 0x33B8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_84_REG (IPP_KLT_BASE + 0x33C0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_85_REG (IPP_KLT_BASE + 0x33C8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_86_REG (IPP_KLT_BASE + 0x33D0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_87_REG (IPP_KLT_BASE + 0x33D8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_88_REG (IPP_KLT_BASE + 0x33E0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_89_REG (IPP_KLT_BASE + 0x33E8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_90_REG (IPP_KLT_BASE + 0x33F0)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_91_REG (IPP_KLT_BASE + 0x33F8)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_92_REG (IPP_KLT_BASE + 0x3400)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_93_REG (IPP_KLT_BASE + 0x3408)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_94_REG (IPP_KLT_BASE + 0x3410)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_95_REG (IPP_KLT_BASE + 0x3418)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_96_REG (IPP_KLT_BASE + 0x3420)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_97_REG (IPP_KLT_BASE + 0x3428)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_98_REG (IPP_KLT_BASE + 0x3430)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_99_REG (IPP_KLT_BASE + 0x3438)        /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_100_REG (IPP_KLT_BASE + 0x3440)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_101_REG (IPP_KLT_BASE + 0x3448)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_102_REG (IPP_KLT_BASE + 0x3450)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_103_REG (IPP_KLT_BASE + 0x3458)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_104_REG (IPP_KLT_BASE + 0x3460)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_105_REG (IPP_KLT_BASE + 0x3468)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_106_REG (IPP_KLT_BASE + 0x3470)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_107_REG (IPP_KLT_BASE + 0x3478)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_108_REG (IPP_KLT_BASE + 0x3480)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_109_REG (IPP_KLT_BASE + 0x3488)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_110_REG (IPP_KLT_BASE + 0x3490)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_111_REG (IPP_KLT_BASE + 0x3498)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_112_REG (IPP_KLT_BASE + 0x34A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_113_REG (IPP_KLT_BASE + 0x34A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_114_REG (IPP_KLT_BASE + 0x34B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_115_REG (IPP_KLT_BASE + 0x34B8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_116_REG (IPP_KLT_BASE + 0x34C0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_117_REG (IPP_KLT_BASE + 0x34C8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_118_REG (IPP_KLT_BASE + 0x34D0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_119_REG (IPP_KLT_BASE + 0x34D8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_120_REG (IPP_KLT_BASE + 0x34E0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_121_REG (IPP_KLT_BASE + 0x34E8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_122_REG (IPP_KLT_BASE + 0x34F0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_123_REG (IPP_KLT_BASE + 0x34F8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_124_REG (IPP_KLT_BASE + 0x3500)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_125_REG (IPP_KLT_BASE + 0x3508)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_126_REG (IPP_KLT_BASE + 0x3510)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_127_REG (IPP_KLT_BASE + 0x3518)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_128_REG (IPP_KLT_BASE + 0x3520)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_129_REG (IPP_KLT_BASE + 0x3528)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_130_REG (IPP_KLT_BASE + 0x3530)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_131_REG (IPP_KLT_BASE + 0x3538)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_132_REG (IPP_KLT_BASE + 0x3540)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_133_REG (IPP_KLT_BASE + 0x3548)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_134_REG (IPP_KLT_BASE + 0x3550)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_135_REG (IPP_KLT_BASE + 0x3558)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_136_REG (IPP_KLT_BASE + 0x3560)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_137_REG (IPP_KLT_BASE + 0x3568)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_138_REG (IPP_KLT_BASE + 0x3570)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_139_REG (IPP_KLT_BASE + 0x3578)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_140_REG (IPP_KLT_BASE + 0x3580)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_141_REG (IPP_KLT_BASE + 0x3588)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_142_REG (IPP_KLT_BASE + 0x3590)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_143_REG (IPP_KLT_BASE + 0x3598)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_144_REG (IPP_KLT_BASE + 0x35A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_145_REG (IPP_KLT_BASE + 0x35A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_146_REG (IPP_KLT_BASE + 0x35B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_147_REG (IPP_KLT_BASE + 0x35B8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_148_REG (IPP_KLT_BASE + 0x35C0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_149_REG (IPP_KLT_BASE + 0x35C8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_150_REG (IPP_KLT_BASE + 0x35D0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_151_REG (IPP_KLT_BASE + 0x35D8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_152_REG (IPP_KLT_BASE + 0x35E0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_153_REG (IPP_KLT_BASE + 0x35E8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_154_REG (IPP_KLT_BASE + 0x35F0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_155_REG (IPP_KLT_BASE + 0x35F8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_156_REG (IPP_KLT_BASE + 0x3600)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_157_REG (IPP_KLT_BASE + 0x3608)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_158_REG (IPP_KLT_BASE + 0x3610)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_159_REG (IPP_KLT_BASE + 0x3618)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_160_REG (IPP_KLT_BASE + 0x3620)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_161_REG (IPP_KLT_BASE + 0x3628)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_162_REG (IPP_KLT_BASE + 0x3630)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_163_REG (IPP_KLT_BASE + 0x3638)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_164_REG (IPP_KLT_BASE + 0x3640)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_165_REG (IPP_KLT_BASE + 0x3648)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_166_REG (IPP_KLT_BASE + 0x3650)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_167_REG (IPP_KLT_BASE + 0x3658)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_168_REG (IPP_KLT_BASE + 0x3660)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_169_REG (IPP_KLT_BASE + 0x3668)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_170_REG (IPP_KLT_BASE + 0x3670)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_171_REG (IPP_KLT_BASE + 0x3678)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_172_REG (IPP_KLT_BASE + 0x3680)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_173_REG (IPP_KLT_BASE + 0x3688)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_174_REG (IPP_KLT_BASE + 0x3690)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_175_REG (IPP_KLT_BASE + 0x3698)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_176_REG (IPP_KLT_BASE + 0x36A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_177_REG (IPP_KLT_BASE + 0x36A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_178_REG (IPP_KLT_BASE + 0x36B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_179_REG (IPP_KLT_BASE + 0x36B8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_180_REG (IPP_KLT_BASE + 0x36C0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_181_REG (IPP_KLT_BASE + 0x36C8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_182_REG (IPP_KLT_BASE + 0x36D0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_183_REG (IPP_KLT_BASE + 0x36D8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_184_REG (IPP_KLT_BASE + 0x36E0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_185_REG (IPP_KLT_BASE + 0x36E8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_186_REG (IPP_KLT_BASE + 0x36F0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_187_REG (IPP_KLT_BASE + 0x36F8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_188_REG (IPP_KLT_BASE + 0x3700)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_189_REG (IPP_KLT_BASE + 0x3708)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_190_REG (IPP_KLT_BASE + 0x3710)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_191_REG (IPP_KLT_BASE + 0x3718)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_192_REG (IPP_KLT_BASE + 0x3720)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_193_REG (IPP_KLT_BASE + 0x3728)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_194_REG (IPP_KLT_BASE + 0x3730)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_195_REG (IPP_KLT_BASE + 0x3738)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_196_REG (IPP_KLT_BASE + 0x3740)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_197_REG (IPP_KLT_BASE + 0x3748)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_198_REG (IPP_KLT_BASE + 0x3750)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_199_REG (IPP_KLT_BASE + 0x3758)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_200_REG (IPP_KLT_BASE + 0x3760)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_201_REG (IPP_KLT_BASE + 0x3768)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_202_REG (IPP_KLT_BASE + 0x3770)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_203_REG (IPP_KLT_BASE + 0x3778)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_204_REG (IPP_KLT_BASE + 0x3780)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_205_REG (IPP_KLT_BASE + 0x3788)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_206_REG (IPP_KLT_BASE + 0x3790)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_207_REG (IPP_KLT_BASE + 0x3798)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_208_REG (IPP_KLT_BASE + 0x37A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_209_REG (IPP_KLT_BASE + 0x37A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_210_REG (IPP_KLT_BASE + 0x37B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_211_REG (IPP_KLT_BASE + 0x37B8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_212_REG (IPP_KLT_BASE + 0x37C0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_213_REG (IPP_KLT_BASE + 0x37C8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_214_REG (IPP_KLT_BASE + 0x37D0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_215_REG (IPP_KLT_BASE + 0x37D8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_216_REG (IPP_KLT_BASE + 0x37E0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_217_REG (IPP_KLT_BASE + 0x37E8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_218_REG (IPP_KLT_BASE + 0x37F0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_219_REG (IPP_KLT_BASE + 0x37F8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_220_REG (IPP_KLT_BASE + 0x3800)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_221_REG (IPP_KLT_BASE + 0x3808)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_222_REG (IPP_KLT_BASE + 0x3810)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_223_REG (IPP_KLT_BASE + 0x3818)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_224_REG (IPP_KLT_BASE + 0x3820)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_225_REG (IPP_KLT_BASE + 0x3828)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_226_REG (IPP_KLT_BASE + 0x3830)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_227_REG (IPP_KLT_BASE + 0x3838)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_228_REG (IPP_KLT_BASE + 0x3840)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_229_REG (IPP_KLT_BASE + 0x3848)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_230_REG (IPP_KLT_BASE + 0x3850)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_231_REG (IPP_KLT_BASE + 0x3858)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_232_REG (IPP_KLT_BASE + 0x3860)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_233_REG (IPP_KLT_BASE + 0x3868)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_234_REG (IPP_KLT_BASE + 0x3870)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_235_REG (IPP_KLT_BASE + 0x3878)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_236_REG (IPP_KLT_BASE + 0x3880)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_237_REG (IPP_KLT_BASE + 0x3888)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_238_REG (IPP_KLT_BASE + 0x3890)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_239_REG (IPP_KLT_BASE + 0x3898)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_240_REG (IPP_KLT_BASE + 0x38A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_241_REG (IPP_KLT_BASE + 0x38A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_242_REG (IPP_KLT_BASE + 0x38B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_243_REG (IPP_KLT_BASE + 0x38B8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_244_REG (IPP_KLT_BASE + 0x38C0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_245_REG (IPP_KLT_BASE + 0x38C8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_246_REG (IPP_KLT_BASE + 0x38D0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_247_REG (IPP_KLT_BASE + 0x38D8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_248_REG (IPP_KLT_BASE + 0x38E0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_249_REG (IPP_KLT_BASE + 0x38E8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_250_REG (IPP_KLT_BASE + 0x38F0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_251_REG (IPP_KLT_BASE + 0x38F8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_252_REG (IPP_KLT_BASE + 0x3900)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_253_REG (IPP_KLT_BASE + 0x3908)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_254_REG (IPP_KLT_BASE + 0x3910)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_255_REG (IPP_KLT_BASE + 0x3918)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_256_REG (IPP_KLT_BASE + 0x3920)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_257_REG (IPP_KLT_BASE + 0x3928)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_258_REG (IPP_KLT_BASE + 0x3930)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_259_REG (IPP_KLT_BASE + 0x3938)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_260_REG (IPP_KLT_BASE + 0x3940)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_261_REG (IPP_KLT_BASE + 0x3948)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_262_REG (IPP_KLT_BASE + 0x3950)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_263_REG (IPP_KLT_BASE + 0x3958)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_264_REG (IPP_KLT_BASE + 0x3960)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_265_REG (IPP_KLT_BASE + 0x3968)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_266_REG (IPP_KLT_BASE + 0x3970)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_267_REG (IPP_KLT_BASE + 0x3978)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_268_REG (IPP_KLT_BASE + 0x3980)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_269_REG (IPP_KLT_BASE + 0x3988)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_270_REG (IPP_KLT_BASE + 0x3990)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_271_REG (IPP_KLT_BASE + 0x3998)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_272_REG (IPP_KLT_BASE + 0x39A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_273_REG (IPP_KLT_BASE + 0x39A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_274_REG (IPP_KLT_BASE + 0x39B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_275_REG (IPP_KLT_BASE + 0x39B8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_276_REG (IPP_KLT_BASE + 0x39C0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_277_REG (IPP_KLT_BASE + 0x39C8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_278_REG (IPP_KLT_BASE + 0x39D0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_279_REG (IPP_KLT_BASE + 0x39D8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_280_REG (IPP_KLT_BASE + 0x39E0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_281_REG (IPP_KLT_BASE + 0x39E8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_282_REG (IPP_KLT_BASE + 0x39F0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_283_REG (IPP_KLT_BASE + 0x39F8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_284_REG (IPP_KLT_BASE + 0x3A00)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_285_REG (IPP_KLT_BASE + 0x3A08)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_286_REG (IPP_KLT_BASE + 0x3A10)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_287_REG (IPP_KLT_BASE + 0x3A18)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_288_REG (IPP_KLT_BASE + 0x3A20)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_289_REG (IPP_KLT_BASE + 0x3A28)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_290_REG (IPP_KLT_BASE + 0x3A30)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_291_REG (IPP_KLT_BASE + 0x3A38)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_292_REG (IPP_KLT_BASE + 0x3A40)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_293_REG (IPP_KLT_BASE + 0x3A48)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_294_REG (IPP_KLT_BASE + 0x3A50)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_295_REG (IPP_KLT_BASE + 0x3A58)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_296_REG (IPP_KLT_BASE + 0x3A60)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_297_REG (IPP_KLT_BASE + 0x3A68)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_298_REG (IPP_KLT_BASE + 0x3A70)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_299_REG (IPP_KLT_BASE + 0x3A78)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_300_REG (IPP_KLT_BASE + 0x3A80)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_301_REG (IPP_KLT_BASE + 0x3A88)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_302_REG (IPP_KLT_BASE + 0x3A90)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_303_REG (IPP_KLT_BASE + 0x3A98)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_304_REG (IPP_KLT_BASE + 0x3AA0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_305_REG (IPP_KLT_BASE + 0x3AA8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_306_REG (IPP_KLT_BASE + 0x3AB0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_307_REG (IPP_KLT_BASE + 0x3AB8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_308_REG (IPP_KLT_BASE + 0x3AC0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_309_REG (IPP_KLT_BASE + 0x3AC8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_310_REG (IPP_KLT_BASE + 0x3AD0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_311_REG (IPP_KLT_BASE + 0x3AD8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_312_REG (IPP_KLT_BASE + 0x3AE0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_313_REG (IPP_KLT_BASE + 0x3AE8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_314_REG (IPP_KLT_BASE + 0x3AF0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_315_REG (IPP_KLT_BASE + 0x3AF8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_316_REG (IPP_KLT_BASE + 0x3B00)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_317_REG (IPP_KLT_BASE + 0x3B08)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_318_REG (IPP_KLT_BASE + 0x3B10)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_319_REG (IPP_KLT_BASE + 0x3B18)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_320_REG (IPP_KLT_BASE + 0x3B20)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_321_REG (IPP_KLT_BASE + 0x3B28)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_322_REG (IPP_KLT_BASE + 0x3B30)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_323_REG (IPP_KLT_BASE + 0x3B38)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_324_REG (IPP_KLT_BASE + 0x3B40)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_325_REG (IPP_KLT_BASE + 0x3B48)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_326_REG (IPP_KLT_BASE + 0x3B50)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_327_REG (IPP_KLT_BASE + 0x3B58)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_328_REG (IPP_KLT_BASE + 0x3B60)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_329_REG (IPP_KLT_BASE + 0x3B68)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_330_REG (IPP_KLT_BASE + 0x3B70)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_331_REG (IPP_KLT_BASE + 0x3B78)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_332_REG (IPP_KLT_BASE + 0x3B80)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_333_REG (IPP_KLT_BASE + 0x3B88)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_334_REG (IPP_KLT_BASE + 0x3B90)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_335_REG (IPP_KLT_BASE + 0x3B98)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_336_REG (IPP_KLT_BASE + 0x3BA0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_337_REG (IPP_KLT_BASE + 0x3BA8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_338_REG (IPP_KLT_BASE + 0x3BB0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_339_REG (IPP_KLT_BASE + 0x3BB8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_340_REG (IPP_KLT_BASE + 0x3BC0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_341_REG (IPP_KLT_BASE + 0x3BC8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_342_REG (IPP_KLT_BASE + 0x3BD0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_343_REG (IPP_KLT_BASE + 0x3BD8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_344_REG (IPP_KLT_BASE + 0x3BE0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_345_REG (IPP_KLT_BASE + 0x3BE8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_346_REG (IPP_KLT_BASE + 0x3BF0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_347_REG (IPP_KLT_BASE + 0x3BF8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_348_REG (IPP_KLT_BASE + 0x3C00)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_349_REG (IPP_KLT_BASE + 0x3C08)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_350_REG (IPP_KLT_BASE + 0x3C10)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_351_REG (IPP_KLT_BASE + 0x3C18)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_352_REG (IPP_KLT_BASE + 0x3C20)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_353_REG (IPP_KLT_BASE + 0x3C28)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_354_REG (IPP_KLT_BASE + 0x3C30)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_355_REG (IPP_KLT_BASE + 0x3C38)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_356_REG (IPP_KLT_BASE + 0x3C40)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_357_REG (IPP_KLT_BASE + 0x3C48)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_358_REG (IPP_KLT_BASE + 0x3C50)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_359_REG (IPP_KLT_BASE + 0x3C58)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_360_REG (IPP_KLT_BASE + 0x3C60)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_361_REG (IPP_KLT_BASE + 0x3C68)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_362_REG (IPP_KLT_BASE + 0x3C70)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_363_REG (IPP_KLT_BASE + 0x3C78)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_364_REG (IPP_KLT_BASE + 0x3C80)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_365_REG (IPP_KLT_BASE + 0x3C88)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_366_REG (IPP_KLT_BASE + 0x3C90)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_367_REG (IPP_KLT_BASE + 0x3C98)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_368_REG (IPP_KLT_BASE + 0x3CA0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_369_REG (IPP_KLT_BASE + 0x3CA8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_370_REG (IPP_KLT_BASE + 0x3CB0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_371_REG (IPP_KLT_BASE + 0x3CB8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_372_REG (IPP_KLT_BASE + 0x3CC0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_373_REG (IPP_KLT_BASE + 0x3CC8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_374_REG (IPP_KLT_BASE + 0x3CD0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_375_REG (IPP_KLT_BASE + 0x3CD8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_376_REG (IPP_KLT_BASE + 0x3CE0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_377_REG (IPP_KLT_BASE + 0x3CE8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_378_REG (IPP_KLT_BASE + 0x3CF0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_379_REG (IPP_KLT_BASE + 0x3CF8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_380_REG (IPP_KLT_BASE + 0x3D00)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_381_REG (IPP_KLT_BASE + 0x3D08)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_382_REG (IPP_KLT_BASE + 0x3D10)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_383_REG (IPP_KLT_BASE + 0x3D18)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_384_REG (IPP_KLT_BASE + 0x3D20)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_385_REG (IPP_KLT_BASE + 0x3D28)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_386_REG (IPP_KLT_BASE + 0x3D30)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_387_REG (IPP_KLT_BASE + 0x3D38)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_388_REG (IPP_KLT_BASE + 0x3D40)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_389_REG (IPP_KLT_BASE + 0x3D48)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_390_REG (IPP_KLT_BASE + 0x3D50)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_391_REG (IPP_KLT_BASE + 0x3D58)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_392_REG (IPP_KLT_BASE + 0x3D60)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_393_REG (IPP_KLT_BASE + 0x3D68)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_394_REG (IPP_KLT_BASE + 0x3D70)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_395_REG (IPP_KLT_BASE + 0x3D78)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_396_REG (IPP_KLT_BASE + 0x3D80)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_397_REG (IPP_KLT_BASE + 0x3D88)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_398_REG (IPP_KLT_BASE + 0x3D90)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_399_REG (IPP_KLT_BASE + 0x3D98)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_400_REG (IPP_KLT_BASE + 0x3DA0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_401_REG (IPP_KLT_BASE + 0x3DA8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_402_REG (IPP_KLT_BASE + 0x3DB0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_403_REG (IPP_KLT_BASE + 0x3DB8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_404_REG (IPP_KLT_BASE + 0x3DC0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_405_REG (IPP_KLT_BASE + 0x3DC8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_406_REG (IPP_KLT_BASE + 0x3DD0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_407_REG (IPP_KLT_BASE + 0x3DD8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_408_REG (IPP_KLT_BASE + 0x3DE0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_409_REG (IPP_KLT_BASE + 0x3DE8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_410_REG (IPP_KLT_BASE + 0x3DF0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_411_REG (IPP_KLT_BASE + 0x3DF8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_412_REG (IPP_KLT_BASE + 0x3E00)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_413_REG (IPP_KLT_BASE + 0x3E08)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_414_REG (IPP_KLT_BASE + 0x3E10)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_415_REG (IPP_KLT_BASE + 0x3E18)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_416_REG (IPP_KLT_BASE + 0x3E20)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_417_REG (IPP_KLT_BASE + 0x3E28)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_418_REG (IPP_KLT_BASE + 0x3E30)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_419_REG (IPP_KLT_BASE + 0x3E38)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_420_REG (IPP_KLT_BASE + 0x3E40)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_421_REG (IPP_KLT_BASE + 0x3E48)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_422_REG (IPP_KLT_BASE + 0x3E50)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_423_REG (IPP_KLT_BASE + 0x3E58)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_424_REG (IPP_KLT_BASE + 0x3E60)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_425_REG (IPP_KLT_BASE + 0x3E68)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_426_REG (IPP_KLT_BASE + 0x3E70)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_427_REG (IPP_KLT_BASE + 0x3E78)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_428_REG (IPP_KLT_BASE + 0x3E80)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_429_REG (IPP_KLT_BASE + 0x3E88)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_430_REG (IPP_KLT_BASE + 0x3E90)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_431_REG (IPP_KLT_BASE + 0x3E98)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_432_REG (IPP_KLT_BASE + 0x3EA0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_433_REG (IPP_KLT_BASE + 0x3EA8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_434_REG (IPP_KLT_BASE + 0x3EB0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_435_REG (IPP_KLT_BASE + 0x3EB8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_436_REG (IPP_KLT_BASE + 0x3EC0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_437_REG (IPP_KLT_BASE + 0x3EC8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_438_REG (IPP_KLT_BASE + 0x3ED0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_439_REG (IPP_KLT_BASE + 0x3ED8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_440_REG (IPP_KLT_BASE + 0x3EE0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_441_REG (IPP_KLT_BASE + 0x3EE8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_442_REG (IPP_KLT_BASE + 0x3EF0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_443_REG (IPP_KLT_BASE + 0x3EF8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_444_REG (IPP_KLT_BASE + 0x3F00)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_445_REG (IPP_KLT_BASE + 0x3F08)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_446_REG (IPP_KLT_BASE + 0x3F10)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_447_REG (IPP_KLT_BASE + 0x3F18)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_448_REG (IPP_KLT_BASE + 0x3F20)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_449_REG (IPP_KLT_BASE + 0x3F28)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_450_REG (IPP_KLT_BASE + 0x3F30)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_451_REG (IPP_KLT_BASE + 0x3F38)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_452_REG (IPP_KLT_BASE + 0x3F40)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_453_REG (IPP_KLT_BASE + 0x3F48)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_454_REG (IPP_KLT_BASE + 0x3F50)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_455_REG (IPP_KLT_BASE + 0x3F58)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_456_REG (IPP_KLT_BASE + 0x3F60)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_457_REG (IPP_KLT_BASE + 0x3F68)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_458_REG (IPP_KLT_BASE + 0x3F70)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_459_REG (IPP_KLT_BASE + 0x3F78)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_460_REG (IPP_KLT_BASE + 0x3F80)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_461_REG (IPP_KLT_BASE + 0x3F88)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_462_REG (IPP_KLT_BASE + 0x3F90)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_463_REG (IPP_KLT_BASE + 0x3F98)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_464_REG (IPP_KLT_BASE + 0x3FA0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_465_REG (IPP_KLT_BASE + 0x3FA8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_466_REG (IPP_KLT_BASE + 0x3FB0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_467_REG (IPP_KLT_BASE + 0x3FB8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_468_REG (IPP_KLT_BASE + 0x3FC0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_469_REG (IPP_KLT_BASE + 0x3FC8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_470_REG (IPP_KLT_BASE + 0x3FD0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_471_REG (IPP_KLT_BASE + 0x3FD8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_472_REG (IPP_KLT_BASE + 0x3FE0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_473_REG (IPP_KLT_BASE + 0x3FE8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_474_REG (IPP_KLT_BASE + 0x3FF0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_475_REG (IPP_KLT_BASE + 0x3FF8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_476_REG (IPP_KLT_BASE + 0x4000)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_477_REG (IPP_KLT_BASE + 0x4008)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_478_REG (IPP_KLT_BASE + 0x4010)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_479_REG (IPP_KLT_BASE + 0x4018)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_480_REG (IPP_KLT_BASE + 0x4020)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_481_REG (IPP_KLT_BASE + 0x4028)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_482_REG (IPP_KLT_BASE + 0x4030)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_483_REG (IPP_KLT_BASE + 0x4038)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_484_REG (IPP_KLT_BASE + 0x4040)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_485_REG (IPP_KLT_BASE + 0x4048)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_486_REG (IPP_KLT_BASE + 0x4050)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_487_REG (IPP_KLT_BASE + 0x4058)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_488_REG (IPP_KLT_BASE + 0x4060)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_489_REG (IPP_KLT_BASE + 0x4068)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_490_REG (IPP_KLT_BASE + 0x4070)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_491_REG (IPP_KLT_BASE + 0x4078)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_492_REG (IPP_KLT_BASE + 0x4080)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_493_REG (IPP_KLT_BASE + 0x4088)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_494_REG (IPP_KLT_BASE + 0x4090)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_495_REG (IPP_KLT_BASE + 0x4098)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_496_REG (IPP_KLT_BASE + 0x40A0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_497_REG (IPP_KLT_BASE + 0x40A8)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_498_REG (IPP_KLT_BASE + 0x40B0)       /* Current KP coordinates */
#define IPP_KLT_MC_NEXT_KP_499_REG (IPP_KLT_BASE + 0x40B8)       /* Current KP coordinates */
#define IPP_KLT_MC_PREV_KP_0_REG (IPP_KLT_BASE + 0x3124)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_1_REG (IPP_KLT_BASE + 0x312C)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_2_REG (IPP_KLT_BASE + 0x3134)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_3_REG (IPP_KLT_BASE + 0x313C)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_4_REG (IPP_KLT_BASE + 0x3144)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_5_REG (IPP_KLT_BASE + 0x314C)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_6_REG (IPP_KLT_BASE + 0x3154)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_7_REG (IPP_KLT_BASE + 0x315C)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_8_REG (IPP_KLT_BASE + 0x3164)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_9_REG (IPP_KLT_BASE + 0x316C)         /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_10_REG (IPP_KLT_BASE + 0x3174)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_11_REG (IPP_KLT_BASE + 0x317C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_12_REG (IPP_KLT_BASE + 0x3184)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_13_REG (IPP_KLT_BASE + 0x318C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_14_REG (IPP_KLT_BASE + 0x3194)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_15_REG (IPP_KLT_BASE + 0x319C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_16_REG (IPP_KLT_BASE + 0x31A4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_17_REG (IPP_KLT_BASE + 0x31AC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_18_REG (IPP_KLT_BASE + 0x31B4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_19_REG (IPP_KLT_BASE + 0x31BC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_20_REG (IPP_KLT_BASE + 0x31C4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_21_REG (IPP_KLT_BASE + 0x31CC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_22_REG (IPP_KLT_BASE + 0x31D4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_23_REG (IPP_KLT_BASE + 0x31DC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_24_REG (IPP_KLT_BASE + 0x31E4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_25_REG (IPP_KLT_BASE + 0x31EC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_26_REG (IPP_KLT_BASE + 0x31F4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_27_REG (IPP_KLT_BASE + 0x31FC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_28_REG (IPP_KLT_BASE + 0x3204)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_29_REG (IPP_KLT_BASE + 0x320C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_30_REG (IPP_KLT_BASE + 0x3214)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_31_REG (IPP_KLT_BASE + 0x321C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_32_REG (IPP_KLT_BASE + 0x3224)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_33_REG (IPP_KLT_BASE + 0x322C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_34_REG (IPP_KLT_BASE + 0x3234)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_35_REG (IPP_KLT_BASE + 0x323C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_36_REG (IPP_KLT_BASE + 0x3244)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_37_REG (IPP_KLT_BASE + 0x324C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_38_REG (IPP_KLT_BASE + 0x3254)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_39_REG (IPP_KLT_BASE + 0x325C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_40_REG (IPP_KLT_BASE + 0x3264)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_41_REG (IPP_KLT_BASE + 0x326C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_42_REG (IPP_KLT_BASE + 0x3274)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_43_REG (IPP_KLT_BASE + 0x327C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_44_REG (IPP_KLT_BASE + 0x3284)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_45_REG (IPP_KLT_BASE + 0x328C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_46_REG (IPP_KLT_BASE + 0x3294)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_47_REG (IPP_KLT_BASE + 0x329C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_48_REG (IPP_KLT_BASE + 0x32A4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_49_REG (IPP_KLT_BASE + 0x32AC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_50_REG (IPP_KLT_BASE + 0x32B4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_51_REG (IPP_KLT_BASE + 0x32BC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_52_REG (IPP_KLT_BASE + 0x32C4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_53_REG (IPP_KLT_BASE + 0x32CC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_54_REG (IPP_KLT_BASE + 0x32D4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_55_REG (IPP_KLT_BASE + 0x32DC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_56_REG (IPP_KLT_BASE + 0x32E4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_57_REG (IPP_KLT_BASE + 0x32EC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_58_REG (IPP_KLT_BASE + 0x32F4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_59_REG (IPP_KLT_BASE + 0x32FC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_60_REG (IPP_KLT_BASE + 0x3304)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_61_REG (IPP_KLT_BASE + 0x330C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_62_REG (IPP_KLT_BASE + 0x3314)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_63_REG (IPP_KLT_BASE + 0x331C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_64_REG (IPP_KLT_BASE + 0x3324)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_65_REG (IPP_KLT_BASE + 0x332C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_66_REG (IPP_KLT_BASE + 0x3334)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_67_REG (IPP_KLT_BASE + 0x333C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_68_REG (IPP_KLT_BASE + 0x3344)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_69_REG (IPP_KLT_BASE + 0x334C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_70_REG (IPP_KLT_BASE + 0x3354)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_71_REG (IPP_KLT_BASE + 0x335C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_72_REG (IPP_KLT_BASE + 0x3364)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_73_REG (IPP_KLT_BASE + 0x336C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_74_REG (IPP_KLT_BASE + 0x3374)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_75_REG (IPP_KLT_BASE + 0x337C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_76_REG (IPP_KLT_BASE + 0x3384)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_77_REG (IPP_KLT_BASE + 0x338C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_78_REG (IPP_KLT_BASE + 0x3394)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_79_REG (IPP_KLT_BASE + 0x339C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_80_REG (IPP_KLT_BASE + 0x33A4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_81_REG (IPP_KLT_BASE + 0x33AC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_82_REG (IPP_KLT_BASE + 0x33B4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_83_REG (IPP_KLT_BASE + 0x33BC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_84_REG (IPP_KLT_BASE + 0x33C4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_85_REG (IPP_KLT_BASE + 0x33CC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_86_REG (IPP_KLT_BASE + 0x33D4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_87_REG (IPP_KLT_BASE + 0x33DC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_88_REG (IPP_KLT_BASE + 0x33E4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_89_REG (IPP_KLT_BASE + 0x33EC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_90_REG (IPP_KLT_BASE + 0x33F4)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_91_REG (IPP_KLT_BASE + 0x33FC)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_92_REG (IPP_KLT_BASE + 0x3404)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_93_REG (IPP_KLT_BASE + 0x340C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_94_REG (IPP_KLT_BASE + 0x3414)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_95_REG (IPP_KLT_BASE + 0x341C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_96_REG (IPP_KLT_BASE + 0x3424)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_97_REG (IPP_KLT_BASE + 0x342C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_98_REG (IPP_KLT_BASE + 0x3434)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_99_REG (IPP_KLT_BASE + 0x343C)        /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_100_REG (IPP_KLT_BASE + 0x3444)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_101_REG (IPP_KLT_BASE + 0x344C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_102_REG (IPP_KLT_BASE + 0x3454)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_103_REG (IPP_KLT_BASE + 0x345C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_104_REG (IPP_KLT_BASE + 0x3464)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_105_REG (IPP_KLT_BASE + 0x346C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_106_REG (IPP_KLT_BASE + 0x3474)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_107_REG (IPP_KLT_BASE + 0x347C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_108_REG (IPP_KLT_BASE + 0x3484)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_109_REG (IPP_KLT_BASE + 0x348C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_110_REG (IPP_KLT_BASE + 0x3494)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_111_REG (IPP_KLT_BASE + 0x349C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_112_REG (IPP_KLT_BASE + 0x34A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_113_REG (IPP_KLT_BASE + 0x34AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_114_REG (IPP_KLT_BASE + 0x34B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_115_REG (IPP_KLT_BASE + 0x34BC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_116_REG (IPP_KLT_BASE + 0x34C4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_117_REG (IPP_KLT_BASE + 0x34CC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_118_REG (IPP_KLT_BASE + 0x34D4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_119_REG (IPP_KLT_BASE + 0x34DC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_120_REG (IPP_KLT_BASE + 0x34E4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_121_REG (IPP_KLT_BASE + 0x34EC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_122_REG (IPP_KLT_BASE + 0x34F4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_123_REG (IPP_KLT_BASE + 0x34FC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_124_REG (IPP_KLT_BASE + 0x3504)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_125_REG (IPP_KLT_BASE + 0x350C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_126_REG (IPP_KLT_BASE + 0x3514)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_127_REG (IPP_KLT_BASE + 0x351C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_128_REG (IPP_KLT_BASE + 0x3524)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_129_REG (IPP_KLT_BASE + 0x352C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_130_REG (IPP_KLT_BASE + 0x3534)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_131_REG (IPP_KLT_BASE + 0x353C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_132_REG (IPP_KLT_BASE + 0x3544)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_133_REG (IPP_KLT_BASE + 0x354C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_134_REG (IPP_KLT_BASE + 0x3554)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_135_REG (IPP_KLT_BASE + 0x355C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_136_REG (IPP_KLT_BASE + 0x3564)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_137_REG (IPP_KLT_BASE + 0x356C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_138_REG (IPP_KLT_BASE + 0x3574)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_139_REG (IPP_KLT_BASE + 0x357C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_140_REG (IPP_KLT_BASE + 0x3584)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_141_REG (IPP_KLT_BASE + 0x358C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_142_REG (IPP_KLT_BASE + 0x3594)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_143_REG (IPP_KLT_BASE + 0x359C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_144_REG (IPP_KLT_BASE + 0x35A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_145_REG (IPP_KLT_BASE + 0x35AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_146_REG (IPP_KLT_BASE + 0x35B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_147_REG (IPP_KLT_BASE + 0x35BC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_148_REG (IPP_KLT_BASE + 0x35C4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_149_REG (IPP_KLT_BASE + 0x35CC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_150_REG (IPP_KLT_BASE + 0x35D4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_151_REG (IPP_KLT_BASE + 0x35DC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_152_REG (IPP_KLT_BASE + 0x35E4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_153_REG (IPP_KLT_BASE + 0x35EC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_154_REG (IPP_KLT_BASE + 0x35F4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_155_REG (IPP_KLT_BASE + 0x35FC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_156_REG (IPP_KLT_BASE + 0x3604)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_157_REG (IPP_KLT_BASE + 0x360C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_158_REG (IPP_KLT_BASE + 0x3614)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_159_REG (IPP_KLT_BASE + 0x361C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_160_REG (IPP_KLT_BASE + 0x3624)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_161_REG (IPP_KLT_BASE + 0x362C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_162_REG (IPP_KLT_BASE + 0x3634)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_163_REG (IPP_KLT_BASE + 0x363C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_164_REG (IPP_KLT_BASE + 0x3644)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_165_REG (IPP_KLT_BASE + 0x364C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_166_REG (IPP_KLT_BASE + 0x3654)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_167_REG (IPP_KLT_BASE + 0x365C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_168_REG (IPP_KLT_BASE + 0x3664)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_169_REG (IPP_KLT_BASE + 0x366C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_170_REG (IPP_KLT_BASE + 0x3674)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_171_REG (IPP_KLT_BASE + 0x367C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_172_REG (IPP_KLT_BASE + 0x3684)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_173_REG (IPP_KLT_BASE + 0x368C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_174_REG (IPP_KLT_BASE + 0x3694)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_175_REG (IPP_KLT_BASE + 0x369C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_176_REG (IPP_KLT_BASE + 0x36A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_177_REG (IPP_KLT_BASE + 0x36AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_178_REG (IPP_KLT_BASE + 0x36B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_179_REG (IPP_KLT_BASE + 0x36BC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_180_REG (IPP_KLT_BASE + 0x36C4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_181_REG (IPP_KLT_BASE + 0x36CC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_182_REG (IPP_KLT_BASE + 0x36D4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_183_REG (IPP_KLT_BASE + 0x36DC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_184_REG (IPP_KLT_BASE + 0x36E4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_185_REG (IPP_KLT_BASE + 0x36EC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_186_REG (IPP_KLT_BASE + 0x36F4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_187_REG (IPP_KLT_BASE + 0x36FC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_188_REG (IPP_KLT_BASE + 0x3704)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_189_REG (IPP_KLT_BASE + 0x370C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_190_REG (IPP_KLT_BASE + 0x3714)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_191_REG (IPP_KLT_BASE + 0x371C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_192_REG (IPP_KLT_BASE + 0x3724)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_193_REG (IPP_KLT_BASE + 0x372C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_194_REG (IPP_KLT_BASE + 0x3734)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_195_REG (IPP_KLT_BASE + 0x373C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_196_REG (IPP_KLT_BASE + 0x3744)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_197_REG (IPP_KLT_BASE + 0x374C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_198_REG (IPP_KLT_BASE + 0x3754)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_199_REG (IPP_KLT_BASE + 0x375C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_200_REG (IPP_KLT_BASE + 0x3764)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_201_REG (IPP_KLT_BASE + 0x376C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_202_REG (IPP_KLT_BASE + 0x3774)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_203_REG (IPP_KLT_BASE + 0x377C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_204_REG (IPP_KLT_BASE + 0x3784)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_205_REG (IPP_KLT_BASE + 0x378C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_206_REG (IPP_KLT_BASE + 0x3794)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_207_REG (IPP_KLT_BASE + 0x379C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_208_REG (IPP_KLT_BASE + 0x37A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_209_REG (IPP_KLT_BASE + 0x37AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_210_REG (IPP_KLT_BASE + 0x37B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_211_REG (IPP_KLT_BASE + 0x37BC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_212_REG (IPP_KLT_BASE + 0x37C4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_213_REG (IPP_KLT_BASE + 0x37CC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_214_REG (IPP_KLT_BASE + 0x37D4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_215_REG (IPP_KLT_BASE + 0x37DC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_216_REG (IPP_KLT_BASE + 0x37E4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_217_REG (IPP_KLT_BASE + 0x37EC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_218_REG (IPP_KLT_BASE + 0x37F4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_219_REG (IPP_KLT_BASE + 0x37FC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_220_REG (IPP_KLT_BASE + 0x3804)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_221_REG (IPP_KLT_BASE + 0x380C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_222_REG (IPP_KLT_BASE + 0x3814)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_223_REG (IPP_KLT_BASE + 0x381C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_224_REG (IPP_KLT_BASE + 0x3824)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_225_REG (IPP_KLT_BASE + 0x382C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_226_REG (IPP_KLT_BASE + 0x3834)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_227_REG (IPP_KLT_BASE + 0x383C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_228_REG (IPP_KLT_BASE + 0x3844)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_229_REG (IPP_KLT_BASE + 0x384C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_230_REG (IPP_KLT_BASE + 0x3854)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_231_REG (IPP_KLT_BASE + 0x385C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_232_REG (IPP_KLT_BASE + 0x3864)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_233_REG (IPP_KLT_BASE + 0x386C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_234_REG (IPP_KLT_BASE + 0x3874)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_235_REG (IPP_KLT_BASE + 0x387C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_236_REG (IPP_KLT_BASE + 0x3884)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_237_REG (IPP_KLT_BASE + 0x388C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_238_REG (IPP_KLT_BASE + 0x3894)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_239_REG (IPP_KLT_BASE + 0x389C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_240_REG (IPP_KLT_BASE + 0x38A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_241_REG (IPP_KLT_BASE + 0x38AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_242_REG (IPP_KLT_BASE + 0x38B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_243_REG (IPP_KLT_BASE + 0x38BC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_244_REG (IPP_KLT_BASE + 0x38C4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_245_REG (IPP_KLT_BASE + 0x38CC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_246_REG (IPP_KLT_BASE + 0x38D4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_247_REG (IPP_KLT_BASE + 0x38DC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_248_REG (IPP_KLT_BASE + 0x38E4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_249_REG (IPP_KLT_BASE + 0x38EC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_250_REG (IPP_KLT_BASE + 0x38F4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_251_REG (IPP_KLT_BASE + 0x38FC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_252_REG (IPP_KLT_BASE + 0x3904)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_253_REG (IPP_KLT_BASE + 0x390C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_254_REG (IPP_KLT_BASE + 0x3914)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_255_REG (IPP_KLT_BASE + 0x391C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_256_REG (IPP_KLT_BASE + 0x3924)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_257_REG (IPP_KLT_BASE + 0x392C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_258_REG (IPP_KLT_BASE + 0x3934)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_259_REG (IPP_KLT_BASE + 0x393C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_260_REG (IPP_KLT_BASE + 0x3944)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_261_REG (IPP_KLT_BASE + 0x394C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_262_REG (IPP_KLT_BASE + 0x3954)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_263_REG (IPP_KLT_BASE + 0x395C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_264_REG (IPP_KLT_BASE + 0x3964)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_265_REG (IPP_KLT_BASE + 0x396C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_266_REG (IPP_KLT_BASE + 0x3974)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_267_REG (IPP_KLT_BASE + 0x397C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_268_REG (IPP_KLT_BASE + 0x3984)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_269_REG (IPP_KLT_BASE + 0x398C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_270_REG (IPP_KLT_BASE + 0x3994)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_271_REG (IPP_KLT_BASE + 0x399C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_272_REG (IPP_KLT_BASE + 0x39A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_273_REG (IPP_KLT_BASE + 0x39AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_274_REG (IPP_KLT_BASE + 0x39B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_275_REG (IPP_KLT_BASE + 0x39BC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_276_REG (IPP_KLT_BASE + 0x39C4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_277_REG (IPP_KLT_BASE + 0x39CC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_278_REG (IPP_KLT_BASE + 0x39D4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_279_REG (IPP_KLT_BASE + 0x39DC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_280_REG (IPP_KLT_BASE + 0x39E4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_281_REG (IPP_KLT_BASE + 0x39EC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_282_REG (IPP_KLT_BASE + 0x39F4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_283_REG (IPP_KLT_BASE + 0x39FC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_284_REG (IPP_KLT_BASE + 0x3A04)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_285_REG (IPP_KLT_BASE + 0x3A0C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_286_REG (IPP_KLT_BASE + 0x3A14)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_287_REG (IPP_KLT_BASE + 0x3A1C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_288_REG (IPP_KLT_BASE + 0x3A24)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_289_REG (IPP_KLT_BASE + 0x3A2C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_290_REG (IPP_KLT_BASE + 0x3A34)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_291_REG (IPP_KLT_BASE + 0x3A3C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_292_REG (IPP_KLT_BASE + 0x3A44)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_293_REG (IPP_KLT_BASE + 0x3A4C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_294_REG (IPP_KLT_BASE + 0x3A54)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_295_REG (IPP_KLT_BASE + 0x3A5C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_296_REG (IPP_KLT_BASE + 0x3A64)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_297_REG (IPP_KLT_BASE + 0x3A6C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_298_REG (IPP_KLT_BASE + 0x3A74)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_299_REG (IPP_KLT_BASE + 0x3A7C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_300_REG (IPP_KLT_BASE + 0x3A84)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_301_REG (IPP_KLT_BASE + 0x3A8C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_302_REG (IPP_KLT_BASE + 0x3A94)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_303_REG (IPP_KLT_BASE + 0x3A9C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_304_REG (IPP_KLT_BASE + 0x3AA4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_305_REG (IPP_KLT_BASE + 0x3AAC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_306_REG (IPP_KLT_BASE + 0x3AB4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_307_REG (IPP_KLT_BASE + 0x3ABC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_308_REG (IPP_KLT_BASE + 0x3AC4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_309_REG (IPP_KLT_BASE + 0x3ACC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_310_REG (IPP_KLT_BASE + 0x3AD4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_311_REG (IPP_KLT_BASE + 0x3ADC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_312_REG (IPP_KLT_BASE + 0x3AE4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_313_REG (IPP_KLT_BASE + 0x3AEC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_314_REG (IPP_KLT_BASE + 0x3AF4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_315_REG (IPP_KLT_BASE + 0x3AFC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_316_REG (IPP_KLT_BASE + 0x3B04)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_317_REG (IPP_KLT_BASE + 0x3B0C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_318_REG (IPP_KLT_BASE + 0x3B14)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_319_REG (IPP_KLT_BASE + 0x3B1C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_320_REG (IPP_KLT_BASE + 0x3B24)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_321_REG (IPP_KLT_BASE + 0x3B2C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_322_REG (IPP_KLT_BASE + 0x3B34)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_323_REG (IPP_KLT_BASE + 0x3B3C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_324_REG (IPP_KLT_BASE + 0x3B44)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_325_REG (IPP_KLT_BASE + 0x3B4C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_326_REG (IPP_KLT_BASE + 0x3B54)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_327_REG (IPP_KLT_BASE + 0x3B5C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_328_REG (IPP_KLT_BASE + 0x3B64)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_329_REG (IPP_KLT_BASE + 0x3B6C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_330_REG (IPP_KLT_BASE + 0x3B74)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_331_REG (IPP_KLT_BASE + 0x3B7C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_332_REG (IPP_KLT_BASE + 0x3B84)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_333_REG (IPP_KLT_BASE + 0x3B8C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_334_REG (IPP_KLT_BASE + 0x3B94)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_335_REG (IPP_KLT_BASE + 0x3B9C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_336_REG (IPP_KLT_BASE + 0x3BA4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_337_REG (IPP_KLT_BASE + 0x3BAC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_338_REG (IPP_KLT_BASE + 0x3BB4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_339_REG (IPP_KLT_BASE + 0x3BBC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_340_REG (IPP_KLT_BASE + 0x3BC4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_341_REG (IPP_KLT_BASE + 0x3BCC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_342_REG (IPP_KLT_BASE + 0x3BD4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_343_REG (IPP_KLT_BASE + 0x3BDC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_344_REG (IPP_KLT_BASE + 0x3BE4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_345_REG (IPP_KLT_BASE + 0x3BEC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_346_REG (IPP_KLT_BASE + 0x3BF4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_347_REG (IPP_KLT_BASE + 0x3BFC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_348_REG (IPP_KLT_BASE + 0x3C04)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_349_REG (IPP_KLT_BASE + 0x3C0C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_350_REG (IPP_KLT_BASE + 0x3C14)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_351_REG (IPP_KLT_BASE + 0x3C1C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_352_REG (IPP_KLT_BASE + 0x3C24)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_353_REG (IPP_KLT_BASE + 0x3C2C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_354_REG (IPP_KLT_BASE + 0x3C34)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_355_REG (IPP_KLT_BASE + 0x3C3C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_356_REG (IPP_KLT_BASE + 0x3C44)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_357_REG (IPP_KLT_BASE + 0x3C4C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_358_REG (IPP_KLT_BASE + 0x3C54)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_359_REG (IPP_KLT_BASE + 0x3C5C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_360_REG (IPP_KLT_BASE + 0x3C64)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_361_REG (IPP_KLT_BASE + 0x3C6C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_362_REG (IPP_KLT_BASE + 0x3C74)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_363_REG (IPP_KLT_BASE + 0x3C7C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_364_REG (IPP_KLT_BASE + 0x3C84)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_365_REG (IPP_KLT_BASE + 0x3C8C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_366_REG (IPP_KLT_BASE + 0x3C94)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_367_REG (IPP_KLT_BASE + 0x3C9C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_368_REG (IPP_KLT_BASE + 0x3CA4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_369_REG (IPP_KLT_BASE + 0x3CAC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_370_REG (IPP_KLT_BASE + 0x3CB4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_371_REG (IPP_KLT_BASE + 0x3CBC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_372_REG (IPP_KLT_BASE + 0x3CC4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_373_REG (IPP_KLT_BASE + 0x3CCC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_374_REG (IPP_KLT_BASE + 0x3CD4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_375_REG (IPP_KLT_BASE + 0x3CDC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_376_REG (IPP_KLT_BASE + 0x3CE4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_377_REG (IPP_KLT_BASE + 0x3CEC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_378_REG (IPP_KLT_BASE + 0x3CF4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_379_REG (IPP_KLT_BASE + 0x3CFC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_380_REG (IPP_KLT_BASE + 0x3D04)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_381_REG (IPP_KLT_BASE + 0x3D0C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_382_REG (IPP_KLT_BASE + 0x3D14)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_383_REG (IPP_KLT_BASE + 0x3D1C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_384_REG (IPP_KLT_BASE + 0x3D24)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_385_REG (IPP_KLT_BASE + 0x3D2C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_386_REG (IPP_KLT_BASE + 0x3D34)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_387_REG (IPP_KLT_BASE + 0x3D3C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_388_REG (IPP_KLT_BASE + 0x3D44)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_389_REG (IPP_KLT_BASE + 0x3D4C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_390_REG (IPP_KLT_BASE + 0x3D54)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_391_REG (IPP_KLT_BASE + 0x3D5C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_392_REG (IPP_KLT_BASE + 0x3D64)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_393_REG (IPP_KLT_BASE + 0x3D6C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_394_REG (IPP_KLT_BASE + 0x3D74)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_395_REG (IPP_KLT_BASE + 0x3D7C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_396_REG (IPP_KLT_BASE + 0x3D84)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_397_REG (IPP_KLT_BASE + 0x3D8C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_398_REG (IPP_KLT_BASE + 0x3D94)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_399_REG (IPP_KLT_BASE + 0x3D9C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_400_REG (IPP_KLT_BASE + 0x3DA4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_401_REG (IPP_KLT_BASE + 0x3DAC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_402_REG (IPP_KLT_BASE + 0x3DB4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_403_REG (IPP_KLT_BASE + 0x3DBC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_404_REG (IPP_KLT_BASE + 0x3DC4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_405_REG (IPP_KLT_BASE + 0x3DCC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_406_REG (IPP_KLT_BASE + 0x3DD4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_407_REG (IPP_KLT_BASE + 0x3DDC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_408_REG (IPP_KLT_BASE + 0x3DE4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_409_REG (IPP_KLT_BASE + 0x3DEC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_410_REG (IPP_KLT_BASE + 0x3DF4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_411_REG (IPP_KLT_BASE + 0x3DFC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_412_REG (IPP_KLT_BASE + 0x3E04)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_413_REG (IPP_KLT_BASE + 0x3E0C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_414_REG (IPP_KLT_BASE + 0x3E14)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_415_REG (IPP_KLT_BASE + 0x3E1C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_416_REG (IPP_KLT_BASE + 0x3E24)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_417_REG (IPP_KLT_BASE + 0x3E2C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_418_REG (IPP_KLT_BASE + 0x3E34)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_419_REG (IPP_KLT_BASE + 0x3E3C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_420_REG (IPP_KLT_BASE + 0x3E44)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_421_REG (IPP_KLT_BASE + 0x3E4C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_422_REG (IPP_KLT_BASE + 0x3E54)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_423_REG (IPP_KLT_BASE + 0x3E5C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_424_REG (IPP_KLT_BASE + 0x3E64)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_425_REG (IPP_KLT_BASE + 0x3E6C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_426_REG (IPP_KLT_BASE + 0x3E74)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_427_REG (IPP_KLT_BASE + 0x3E7C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_428_REG (IPP_KLT_BASE + 0x3E84)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_429_REG (IPP_KLT_BASE + 0x3E8C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_430_REG (IPP_KLT_BASE + 0x3E94)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_431_REG (IPP_KLT_BASE + 0x3E9C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_432_REG (IPP_KLT_BASE + 0x3EA4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_433_REG (IPP_KLT_BASE + 0x3EAC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_434_REG (IPP_KLT_BASE + 0x3EB4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_435_REG (IPP_KLT_BASE + 0x3EBC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_436_REG (IPP_KLT_BASE + 0x3EC4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_437_REG (IPP_KLT_BASE + 0x3ECC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_438_REG (IPP_KLT_BASE + 0x3ED4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_439_REG (IPP_KLT_BASE + 0x3EDC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_440_REG (IPP_KLT_BASE + 0x3EE4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_441_REG (IPP_KLT_BASE + 0x3EEC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_442_REG (IPP_KLT_BASE + 0x3EF4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_443_REG (IPP_KLT_BASE + 0x3EFC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_444_REG (IPP_KLT_BASE + 0x3F04)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_445_REG (IPP_KLT_BASE + 0x3F0C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_446_REG (IPP_KLT_BASE + 0x3F14)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_447_REG (IPP_KLT_BASE + 0x3F1C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_448_REG (IPP_KLT_BASE + 0x3F24)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_449_REG (IPP_KLT_BASE + 0x3F2C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_450_REG (IPP_KLT_BASE + 0x3F34)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_451_REG (IPP_KLT_BASE + 0x3F3C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_452_REG (IPP_KLT_BASE + 0x3F44)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_453_REG (IPP_KLT_BASE + 0x3F4C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_454_REG (IPP_KLT_BASE + 0x3F54)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_455_REG (IPP_KLT_BASE + 0x3F5C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_456_REG (IPP_KLT_BASE + 0x3F64)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_457_REG (IPP_KLT_BASE + 0x3F6C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_458_REG (IPP_KLT_BASE + 0x3F74)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_459_REG (IPP_KLT_BASE + 0x3F7C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_460_REG (IPP_KLT_BASE + 0x3F84)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_461_REG (IPP_KLT_BASE + 0x3F8C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_462_REG (IPP_KLT_BASE + 0x3F94)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_463_REG (IPP_KLT_BASE + 0x3F9C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_464_REG (IPP_KLT_BASE + 0x3FA4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_465_REG (IPP_KLT_BASE + 0x3FAC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_466_REG (IPP_KLT_BASE + 0x3FB4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_467_REG (IPP_KLT_BASE + 0x3FBC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_468_REG (IPP_KLT_BASE + 0x3FC4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_469_REG (IPP_KLT_BASE + 0x3FCC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_470_REG (IPP_KLT_BASE + 0x3FD4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_471_REG (IPP_KLT_BASE + 0x3FDC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_472_REG (IPP_KLT_BASE + 0x3FE4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_473_REG (IPP_KLT_BASE + 0x3FEC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_474_REG (IPP_KLT_BASE + 0x3FF4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_475_REG (IPP_KLT_BASE + 0x3FFC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_476_REG (IPP_KLT_BASE + 0x4004)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_477_REG (IPP_KLT_BASE + 0x400C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_478_REG (IPP_KLT_BASE + 0x4014)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_479_REG (IPP_KLT_BASE + 0x401C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_480_REG (IPP_KLT_BASE + 0x4024)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_481_REG (IPP_KLT_BASE + 0x402C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_482_REG (IPP_KLT_BASE + 0x4034)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_483_REG (IPP_KLT_BASE + 0x403C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_484_REG (IPP_KLT_BASE + 0x4044)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_485_REG (IPP_KLT_BASE + 0x404C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_486_REG (IPP_KLT_BASE + 0x4054)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_487_REG (IPP_KLT_BASE + 0x405C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_488_REG (IPP_KLT_BASE + 0x4064)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_489_REG (IPP_KLT_BASE + 0x406C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_490_REG (IPP_KLT_BASE + 0x4074)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_491_REG (IPP_KLT_BASE + 0x407C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_492_REG (IPP_KLT_BASE + 0x4084)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_493_REG (IPP_KLT_BASE + 0x408C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_494_REG (IPP_KLT_BASE + 0x4094)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_495_REG (IPP_KLT_BASE + 0x409C)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_496_REG (IPP_KLT_BASE + 0x40A4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_497_REG (IPP_KLT_BASE + 0x40AC)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_498_REG (IPP_KLT_BASE + 0x40B4)       /* Reference KP coordinates */
#define IPP_KLT_MC_PREV_KP_499_REG (IPP_KLT_BASE + 0x40BC)       /* Reference KP coordinates */

#endif  // __IPP_KLT_REG_OFFSET_H__
