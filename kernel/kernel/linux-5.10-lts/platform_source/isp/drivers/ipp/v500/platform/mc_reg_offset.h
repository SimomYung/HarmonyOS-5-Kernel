/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  mc_reg_offset.h
 * Project line  :  
 * Department    :  
 * Author        :  HerveDANIEL
 * Version       :  1.0
 * Date          :  40876
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  HerveDANIEL 2023/09/11 09:43:09 Create file
 */

#ifndef __MC_REG_OFFSET_H__
#define __MC_REG_OFFSET_H__

/* MC Base address of Module's Register */
#define MC_BASE                       (0x0)

/******************************************************************************/
/*                      HiStarISP MC Registers' Definitions                            */
/******************************************************************************/

#define MC_EN_CFG_REG              (MC_BASE + 0x0)    /* Enable Config */
#define MC_IMU_CFG_REG             (MC_BASE + 0x18)   /* IMU config */
#define MC_MC_CFG_REG              (MC_BASE + 0x1C)   /* Basic Config */
#define MC_THRESHOLD_CFG_REG       (MC_BASE + 0x20)   /* Threshold Config */
#define MC_INLIER_NUMBER_REG       (MC_BASE + 0x24)   /* Inlier numbers for FW */
#define MC_KPT_SIZE_REG            (MC_BASE + 0x28)   /* key point size */
#define MC_STATIC_CFG2_REG         (MC_BASE + 0x30)   /* Static config2 */
#define MC_REF_PREFETCH_CFG_REG    (MC_BASE + 0x34)   /* Configure the prefetch for reference */
#define MC_CUR_PREFETCH_CFG_REG    (MC_BASE + 0x38)   /* Configure the prefetch for current */
#define MC_MATCH_POINTS_REG        (MC_BASE + 0x3C)   /* Matched points number */
#define MC_INDEX_CFG_0_REG         (MC_BASE + 0x40)   /* Index pairs from CMP */
#define MC_INDEX_CFG_1_REG         (MC_BASE + 0x44)   /* Index pairs from CMP */
#define MC_INDEX_CFG_2_REG         (MC_BASE + 0x48)   /* Index pairs from CMP */
#define MC_INDEX_CFG_3_REG         (MC_BASE + 0x4C)   /* Index pairs from CMP */
#define MC_INDEX_CFG_4_REG         (MC_BASE + 0x50)   /* Index pairs from CMP */
#define MC_INDEX_CFG_5_REG         (MC_BASE + 0x54)   /* Index pairs from CMP */
#define MC_INDEX_CFG_6_REG         (MC_BASE + 0x58)   /* Index pairs from CMP */
#define MC_INDEX_CFG_7_REG         (MC_BASE + 0x5C)   /* Index pairs from CMP */
#define MC_INDEX_CFG_8_REG         (MC_BASE + 0x60)   /* Index pairs from CMP */
#define MC_INDEX_CFG_9_REG         (MC_BASE + 0x64)   /* Index pairs from CMP */
#define MC_INDEX_CFG_10_REG        (MC_BASE + 0x68)   /* Index pairs from CMP */
#define MC_INDEX_CFG_11_REG        (MC_BASE + 0x6C)   /* Index pairs from CMP */
#define MC_INDEX_CFG_12_REG        (MC_BASE + 0x70)   /* Index pairs from CMP */
#define MC_INDEX_CFG_13_REG        (MC_BASE + 0x74)   /* Index pairs from CMP */
#define MC_INDEX_CFG_14_REG        (MC_BASE + 0x78)   /* Index pairs from CMP */
#define MC_INDEX_CFG_15_REG        (MC_BASE + 0x7C)   /* Index pairs from CMP */
#define MC_INDEX_CFG_16_REG        (MC_BASE + 0x80)   /* Index pairs from CMP */
#define MC_INDEX_CFG_17_REG        (MC_BASE + 0x84)   /* Index pairs from CMP */
#define MC_INDEX_CFG_18_REG        (MC_BASE + 0x88)   /* Index pairs from CMP */
#define MC_INDEX_CFG_19_REG        (MC_BASE + 0x8C)   /* Index pairs from CMP */
#define MC_INDEX_CFG_20_REG        (MC_BASE + 0x90)   /* Index pairs from CMP */
#define MC_INDEX_CFG_21_REG        (MC_BASE + 0x94)   /* Index pairs from CMP */
#define MC_INDEX_CFG_22_REG        (MC_BASE + 0x98)   /* Index pairs from CMP */
#define MC_INDEX_CFG_23_REG        (MC_BASE + 0x9C)   /* Index pairs from CMP */
#define MC_INDEX_CFG_24_REG        (MC_BASE + 0xA0)   /* Index pairs from CMP */
#define MC_INDEX_CFG_25_REG        (MC_BASE + 0xA4)   /* Index pairs from CMP */
#define MC_INDEX_CFG_26_REG        (MC_BASE + 0xA8)   /* Index pairs from CMP */
#define MC_INDEX_CFG_27_REG        (MC_BASE + 0xAC)   /* Index pairs from CMP */
#define MC_INDEX_CFG_28_REG        (MC_BASE + 0xB0)   /* Index pairs from CMP */
#define MC_INDEX_CFG_29_REG        (MC_BASE + 0xB4)   /* Index pairs from CMP */
#define MC_INDEX_CFG_30_REG        (MC_BASE + 0xB8)   /* Index pairs from CMP */
#define MC_INDEX_CFG_31_REG        (MC_BASE + 0xBC)   /* Index pairs from CMP */
#define MC_INDEX_CFG_32_REG        (MC_BASE + 0xC0)   /* Index pairs from CMP */
#define MC_INDEX_CFG_33_REG        (MC_BASE + 0xC4)   /* Index pairs from CMP */
#define MC_INDEX_CFG_34_REG        (MC_BASE + 0xC8)   /* Index pairs from CMP */
#define MC_INDEX_CFG_35_REG        (MC_BASE + 0xCC)   /* Index pairs from CMP */
#define MC_INDEX_CFG_36_REG        (MC_BASE + 0xD0)   /* Index pairs from CMP */
#define MC_INDEX_CFG_37_REG        (MC_BASE + 0xD4)   /* Index pairs from CMP */
#define MC_INDEX_CFG_38_REG        (MC_BASE + 0xD8)   /* Index pairs from CMP */
#define MC_INDEX_CFG_39_REG        (MC_BASE + 0xDC)   /* Index pairs from CMP */
#define MC_INDEX_CFG_40_REG        (MC_BASE + 0xE0)   /* Index pairs from CMP */
#define MC_INDEX_CFG_41_REG        (MC_BASE + 0xE4)   /* Index pairs from CMP */
#define MC_INDEX_CFG_42_REG        (MC_BASE + 0xE8)   /* Index pairs from CMP */
#define MC_INDEX_CFG_43_REG        (MC_BASE + 0xEC)   /* Index pairs from CMP */
#define MC_INDEX_CFG_44_REG        (MC_BASE + 0xF0)   /* Index pairs from CMP */
#define MC_INDEX_CFG_45_REG        (MC_BASE + 0xF4)   /* Index pairs from CMP */
#define MC_INDEX_CFG_46_REG        (MC_BASE + 0xF8)   /* Index pairs from CMP */
#define MC_INDEX_CFG_47_REG        (MC_BASE + 0xFC)   /* Index pairs from CMP */
#define MC_INDEX_CFG_48_REG        (MC_BASE + 0x100)  /* Index pairs from CMP */
#define MC_INDEX_CFG_49_REG        (MC_BASE + 0x104)  /* Index pairs from CMP */
#define MC_INDEX_CFG_50_REG        (MC_BASE + 0x108)  /* Index pairs from CMP */
#define MC_INDEX_CFG_51_REG        (MC_BASE + 0x10C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_52_REG        (MC_BASE + 0x110)  /* Index pairs from CMP */
#define MC_INDEX_CFG_53_REG        (MC_BASE + 0x114)  /* Index pairs from CMP */
#define MC_INDEX_CFG_54_REG        (MC_BASE + 0x118)  /* Index pairs from CMP */
#define MC_INDEX_CFG_55_REG        (MC_BASE + 0x11C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_56_REG        (MC_BASE + 0x120)  /* Index pairs from CMP */
#define MC_INDEX_CFG_57_REG        (MC_BASE + 0x124)  /* Index pairs from CMP */
#define MC_INDEX_CFG_58_REG        (MC_BASE + 0x128)  /* Index pairs from CMP */
#define MC_INDEX_CFG_59_REG        (MC_BASE + 0x12C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_60_REG        (MC_BASE + 0x130)  /* Index pairs from CMP */
#define MC_INDEX_CFG_61_REG        (MC_BASE + 0x134)  /* Index pairs from CMP */
#define MC_INDEX_CFG_62_REG        (MC_BASE + 0x138)  /* Index pairs from CMP */
#define MC_INDEX_CFG_63_REG        (MC_BASE + 0x13C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_64_REG        (MC_BASE + 0x140)  /* Index pairs from CMP */
#define MC_INDEX_CFG_65_REG        (MC_BASE + 0x144)  /* Index pairs from CMP */
#define MC_INDEX_CFG_66_REG        (MC_BASE + 0x148)  /* Index pairs from CMP */
#define MC_INDEX_CFG_67_REG        (MC_BASE + 0x14C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_68_REG        (MC_BASE + 0x150)  /* Index pairs from CMP */
#define MC_INDEX_CFG_69_REG        (MC_BASE + 0x154)  /* Index pairs from CMP */
#define MC_INDEX_CFG_70_REG        (MC_BASE + 0x158)  /* Index pairs from CMP */
#define MC_INDEX_CFG_71_REG        (MC_BASE + 0x15C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_72_REG        (MC_BASE + 0x160)  /* Index pairs from CMP */
#define MC_INDEX_CFG_73_REG        (MC_BASE + 0x164)  /* Index pairs from CMP */
#define MC_INDEX_CFG_74_REG        (MC_BASE + 0x168)  /* Index pairs from CMP */
#define MC_INDEX_CFG_75_REG        (MC_BASE + 0x16C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_76_REG        (MC_BASE + 0x170)  /* Index pairs from CMP */
#define MC_INDEX_CFG_77_REG        (MC_BASE + 0x174)  /* Index pairs from CMP */
#define MC_INDEX_CFG_78_REG        (MC_BASE + 0x178)  /* Index pairs from CMP */
#define MC_INDEX_CFG_79_REG        (MC_BASE + 0x17C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_80_REG        (MC_BASE + 0x180)  /* Index pairs from CMP */
#define MC_INDEX_CFG_81_REG        (MC_BASE + 0x184)  /* Index pairs from CMP */
#define MC_INDEX_CFG_82_REG        (MC_BASE + 0x188)  /* Index pairs from CMP */
#define MC_INDEX_CFG_83_REG        (MC_BASE + 0x18C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_84_REG        (MC_BASE + 0x190)  /* Index pairs from CMP */
#define MC_INDEX_CFG_85_REG        (MC_BASE + 0x194)  /* Index pairs from CMP */
#define MC_INDEX_CFG_86_REG        (MC_BASE + 0x198)  /* Index pairs from CMP */
#define MC_INDEX_CFG_87_REG        (MC_BASE + 0x19C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_88_REG        (MC_BASE + 0x1A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_89_REG        (MC_BASE + 0x1A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_90_REG        (MC_BASE + 0x1A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_91_REG        (MC_BASE + 0x1AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_92_REG        (MC_BASE + 0x1B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_93_REG        (MC_BASE + 0x1B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_94_REG        (MC_BASE + 0x1B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_95_REG        (MC_BASE + 0x1BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_96_REG        (MC_BASE + 0x1C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_97_REG        (MC_BASE + 0x1C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_98_REG        (MC_BASE + 0x1C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_99_REG        (MC_BASE + 0x1CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_100_REG       (MC_BASE + 0x1D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_101_REG       (MC_BASE + 0x1D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_102_REG       (MC_BASE + 0x1D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_103_REG       (MC_BASE + 0x1DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_104_REG       (MC_BASE + 0x1E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_105_REG       (MC_BASE + 0x1E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_106_REG       (MC_BASE + 0x1E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_107_REG       (MC_BASE + 0x1EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_108_REG       (MC_BASE + 0x1F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_109_REG       (MC_BASE + 0x1F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_110_REG       (MC_BASE + 0x1F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_111_REG       (MC_BASE + 0x1FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_112_REG       (MC_BASE + 0x200)  /* Index pairs from CMP */
#define MC_INDEX_CFG_113_REG       (MC_BASE + 0x204)  /* Index pairs from CMP */
#define MC_INDEX_CFG_114_REG       (MC_BASE + 0x208)  /* Index pairs from CMP */
#define MC_INDEX_CFG_115_REG       (MC_BASE + 0x20C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_116_REG       (MC_BASE + 0x210)  /* Index pairs from CMP */
#define MC_INDEX_CFG_117_REG       (MC_BASE + 0x214)  /* Index pairs from CMP */
#define MC_INDEX_CFG_118_REG       (MC_BASE + 0x218)  /* Index pairs from CMP */
#define MC_INDEX_CFG_119_REG       (MC_BASE + 0x21C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_120_REG       (MC_BASE + 0x220)  /* Index pairs from CMP */
#define MC_INDEX_CFG_121_REG       (MC_BASE + 0x224)  /* Index pairs from CMP */
#define MC_INDEX_CFG_122_REG       (MC_BASE + 0x228)  /* Index pairs from CMP */
#define MC_INDEX_CFG_123_REG       (MC_BASE + 0x22C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_124_REG       (MC_BASE + 0x230)  /* Index pairs from CMP */
#define MC_INDEX_CFG_125_REG       (MC_BASE + 0x234)  /* Index pairs from CMP */
#define MC_INDEX_CFG_126_REG       (MC_BASE + 0x238)  /* Index pairs from CMP */
#define MC_INDEX_CFG_127_REG       (MC_BASE + 0x23C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_128_REG       (MC_BASE + 0x240)  /* Index pairs from CMP */
#define MC_INDEX_CFG_129_REG       (MC_BASE + 0x244)  /* Index pairs from CMP */
#define MC_INDEX_CFG_130_REG       (MC_BASE + 0x248)  /* Index pairs from CMP */
#define MC_INDEX_CFG_131_REG       (MC_BASE + 0x24C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_132_REG       (MC_BASE + 0x250)  /* Index pairs from CMP */
#define MC_INDEX_CFG_133_REG       (MC_BASE + 0x254)  /* Index pairs from CMP */
#define MC_INDEX_CFG_134_REG       (MC_BASE + 0x258)  /* Index pairs from CMP */
#define MC_INDEX_CFG_135_REG       (MC_BASE + 0x25C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_136_REG       (MC_BASE + 0x260)  /* Index pairs from CMP */
#define MC_INDEX_CFG_137_REG       (MC_BASE + 0x264)  /* Index pairs from CMP */
#define MC_INDEX_CFG_138_REG       (MC_BASE + 0x268)  /* Index pairs from CMP */
#define MC_INDEX_CFG_139_REG       (MC_BASE + 0x26C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_140_REG       (MC_BASE + 0x270)  /* Index pairs from CMP */
#define MC_INDEX_CFG_141_REG       (MC_BASE + 0x274)  /* Index pairs from CMP */
#define MC_INDEX_CFG_142_REG       (MC_BASE + 0x278)  /* Index pairs from CMP */
#define MC_INDEX_CFG_143_REG       (MC_BASE + 0x27C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_144_REG       (MC_BASE + 0x280)  /* Index pairs from CMP */
#define MC_INDEX_CFG_145_REG       (MC_BASE + 0x284)  /* Index pairs from CMP */
#define MC_INDEX_CFG_146_REG       (MC_BASE + 0x288)  /* Index pairs from CMP */
#define MC_INDEX_CFG_147_REG       (MC_BASE + 0x28C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_148_REG       (MC_BASE + 0x290)  /* Index pairs from CMP */
#define MC_INDEX_CFG_149_REG       (MC_BASE + 0x294)  /* Index pairs from CMP */
#define MC_INDEX_CFG_150_REG       (MC_BASE + 0x298)  /* Index pairs from CMP */
#define MC_INDEX_CFG_151_REG       (MC_BASE + 0x29C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_152_REG       (MC_BASE + 0x2A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_153_REG       (MC_BASE + 0x2A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_154_REG       (MC_BASE + 0x2A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_155_REG       (MC_BASE + 0x2AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_156_REG       (MC_BASE + 0x2B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_157_REG       (MC_BASE + 0x2B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_158_REG       (MC_BASE + 0x2B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_159_REG       (MC_BASE + 0x2BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_160_REG       (MC_BASE + 0x2C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_161_REG       (MC_BASE + 0x2C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_162_REG       (MC_BASE + 0x2C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_163_REG       (MC_BASE + 0x2CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_164_REG       (MC_BASE + 0x2D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_165_REG       (MC_BASE + 0x2D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_166_REG       (MC_BASE + 0x2D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_167_REG       (MC_BASE + 0x2DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_168_REG       (MC_BASE + 0x2E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_169_REG       (MC_BASE + 0x2E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_170_REG       (MC_BASE + 0x2E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_171_REG       (MC_BASE + 0x2EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_172_REG       (MC_BASE + 0x2F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_173_REG       (MC_BASE + 0x2F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_174_REG       (MC_BASE + 0x2F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_175_REG       (MC_BASE + 0x2FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_176_REG       (MC_BASE + 0x300)  /* Index pairs from CMP */
#define MC_INDEX_CFG_177_REG       (MC_BASE + 0x304)  /* Index pairs from CMP */
#define MC_INDEX_CFG_178_REG       (MC_BASE + 0x308)  /* Index pairs from CMP */
#define MC_INDEX_CFG_179_REG       (MC_BASE + 0x30C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_180_REG       (MC_BASE + 0x310)  /* Index pairs from CMP */
#define MC_INDEX_CFG_181_REG       (MC_BASE + 0x314)  /* Index pairs from CMP */
#define MC_INDEX_CFG_182_REG       (MC_BASE + 0x318)  /* Index pairs from CMP */
#define MC_INDEX_CFG_183_REG       (MC_BASE + 0x31C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_184_REG       (MC_BASE + 0x320)  /* Index pairs from CMP */
#define MC_INDEX_CFG_185_REG       (MC_BASE + 0x324)  /* Index pairs from CMP */
#define MC_INDEX_CFG_186_REG       (MC_BASE + 0x328)  /* Index pairs from CMP */
#define MC_INDEX_CFG_187_REG       (MC_BASE + 0x32C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_188_REG       (MC_BASE + 0x330)  /* Index pairs from CMP */
#define MC_INDEX_CFG_189_REG       (MC_BASE + 0x334)  /* Index pairs from CMP */
#define MC_INDEX_CFG_190_REG       (MC_BASE + 0x338)  /* Index pairs from CMP */
#define MC_INDEX_CFG_191_REG       (MC_BASE + 0x33C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_192_REG       (MC_BASE + 0x340)  /* Index pairs from CMP */
#define MC_INDEX_CFG_193_REG       (MC_BASE + 0x344)  /* Index pairs from CMP */
#define MC_INDEX_CFG_194_REG       (MC_BASE + 0x348)  /* Index pairs from CMP */
#define MC_INDEX_CFG_195_REG       (MC_BASE + 0x34C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_196_REG       (MC_BASE + 0x350)  /* Index pairs from CMP */
#define MC_INDEX_CFG_197_REG       (MC_BASE + 0x354)  /* Index pairs from CMP */
#define MC_INDEX_CFG_198_REG       (MC_BASE + 0x358)  /* Index pairs from CMP */
#define MC_INDEX_CFG_199_REG       (MC_BASE + 0x35C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_200_REG       (MC_BASE + 0x360)  /* Index pairs from CMP */
#define MC_INDEX_CFG_201_REG       (MC_BASE + 0x364)  /* Index pairs from CMP */
#define MC_INDEX_CFG_202_REG       (MC_BASE + 0x368)  /* Index pairs from CMP */
#define MC_INDEX_CFG_203_REG       (MC_BASE + 0x36C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_204_REG       (MC_BASE + 0x370)  /* Index pairs from CMP */
#define MC_INDEX_CFG_205_REG       (MC_BASE + 0x374)  /* Index pairs from CMP */
#define MC_INDEX_CFG_206_REG       (MC_BASE + 0x378)  /* Index pairs from CMP */
#define MC_INDEX_CFG_207_REG       (MC_BASE + 0x37C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_208_REG       (MC_BASE + 0x380)  /* Index pairs from CMP */
#define MC_INDEX_CFG_209_REG       (MC_BASE + 0x384)  /* Index pairs from CMP */
#define MC_INDEX_CFG_210_REG       (MC_BASE + 0x388)  /* Index pairs from CMP */
#define MC_INDEX_CFG_211_REG       (MC_BASE + 0x38C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_212_REG       (MC_BASE + 0x390)  /* Index pairs from CMP */
#define MC_INDEX_CFG_213_REG       (MC_BASE + 0x394)  /* Index pairs from CMP */
#define MC_INDEX_CFG_214_REG       (MC_BASE + 0x398)  /* Index pairs from CMP */
#define MC_INDEX_CFG_215_REG       (MC_BASE + 0x39C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_216_REG       (MC_BASE + 0x3A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_217_REG       (MC_BASE + 0x3A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_218_REG       (MC_BASE + 0x3A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_219_REG       (MC_BASE + 0x3AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_220_REG       (MC_BASE + 0x3B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_221_REG       (MC_BASE + 0x3B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_222_REG       (MC_BASE + 0x3B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_223_REG       (MC_BASE + 0x3BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_224_REG       (MC_BASE + 0x3C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_225_REG       (MC_BASE + 0x3C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_226_REG       (MC_BASE + 0x3C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_227_REG       (MC_BASE + 0x3CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_228_REG       (MC_BASE + 0x3D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_229_REG       (MC_BASE + 0x3D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_230_REG       (MC_BASE + 0x3D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_231_REG       (MC_BASE + 0x3DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_232_REG       (MC_BASE + 0x3E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_233_REG       (MC_BASE + 0x3E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_234_REG       (MC_BASE + 0x3E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_235_REG       (MC_BASE + 0x3EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_236_REG       (MC_BASE + 0x3F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_237_REG       (MC_BASE + 0x3F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_238_REG       (MC_BASE + 0x3F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_239_REG       (MC_BASE + 0x3FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_240_REG       (MC_BASE + 0x400)  /* Index pairs from CMP */
#define MC_INDEX_CFG_241_REG       (MC_BASE + 0x404)  /* Index pairs from CMP */
#define MC_INDEX_CFG_242_REG       (MC_BASE + 0x408)  /* Index pairs from CMP */
#define MC_INDEX_CFG_243_REG       (MC_BASE + 0x40C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_244_REG       (MC_BASE + 0x410)  /* Index pairs from CMP */
#define MC_INDEX_CFG_245_REG       (MC_BASE + 0x414)  /* Index pairs from CMP */
#define MC_INDEX_CFG_246_REG       (MC_BASE + 0x418)  /* Index pairs from CMP */
#define MC_INDEX_CFG_247_REG       (MC_BASE + 0x41C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_248_REG       (MC_BASE + 0x420)  /* Index pairs from CMP */
#define MC_INDEX_CFG_249_REG       (MC_BASE + 0x424)  /* Index pairs from CMP */
#define MC_INDEX_CFG_250_REG       (MC_BASE + 0x428)  /* Index pairs from CMP */
#define MC_INDEX_CFG_251_REG       (MC_BASE + 0x42C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_252_REG       (MC_BASE + 0x430)  /* Index pairs from CMP */
#define MC_INDEX_CFG_253_REG       (MC_BASE + 0x434)  /* Index pairs from CMP */
#define MC_INDEX_CFG_254_REG       (MC_BASE + 0x438)  /* Index pairs from CMP */
#define MC_INDEX_CFG_255_REG       (MC_BASE + 0x43C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_256_REG       (MC_BASE + 0x440)  /* Index pairs from CMP */
#define MC_INDEX_CFG_257_REG       (MC_BASE + 0x444)  /* Index pairs from CMP */
#define MC_INDEX_CFG_258_REG       (MC_BASE + 0x448)  /* Index pairs from CMP */
#define MC_INDEX_CFG_259_REG       (MC_BASE + 0x44C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_260_REG       (MC_BASE + 0x450)  /* Index pairs from CMP */
#define MC_INDEX_CFG_261_REG       (MC_BASE + 0x454)  /* Index pairs from CMP */
#define MC_INDEX_CFG_262_REG       (MC_BASE + 0x458)  /* Index pairs from CMP */
#define MC_INDEX_CFG_263_REG       (MC_BASE + 0x45C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_264_REG       (MC_BASE + 0x460)  /* Index pairs from CMP */
#define MC_INDEX_CFG_265_REG       (MC_BASE + 0x464)  /* Index pairs from CMP */
#define MC_INDEX_CFG_266_REG       (MC_BASE + 0x468)  /* Index pairs from CMP */
#define MC_INDEX_CFG_267_REG       (MC_BASE + 0x46C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_268_REG       (MC_BASE + 0x470)  /* Index pairs from CMP */
#define MC_INDEX_CFG_269_REG       (MC_BASE + 0x474)  /* Index pairs from CMP */
#define MC_INDEX_CFG_270_REG       (MC_BASE + 0x478)  /* Index pairs from CMP */
#define MC_INDEX_CFG_271_REG       (MC_BASE + 0x47C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_272_REG       (MC_BASE + 0x480)  /* Index pairs from CMP */
#define MC_INDEX_CFG_273_REG       (MC_BASE + 0x484)  /* Index pairs from CMP */
#define MC_INDEX_CFG_274_REG       (MC_BASE + 0x488)  /* Index pairs from CMP */
#define MC_INDEX_CFG_275_REG       (MC_BASE + 0x48C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_276_REG       (MC_BASE + 0x490)  /* Index pairs from CMP */
#define MC_INDEX_CFG_277_REG       (MC_BASE + 0x494)  /* Index pairs from CMP */
#define MC_INDEX_CFG_278_REG       (MC_BASE + 0x498)  /* Index pairs from CMP */
#define MC_INDEX_CFG_279_REG       (MC_BASE + 0x49C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_280_REG       (MC_BASE + 0x4A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_281_REG       (MC_BASE + 0x4A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_282_REG       (MC_BASE + 0x4A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_283_REG       (MC_BASE + 0x4AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_284_REG       (MC_BASE + 0x4B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_285_REG       (MC_BASE + 0x4B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_286_REG       (MC_BASE + 0x4B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_287_REG       (MC_BASE + 0x4BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_288_REG       (MC_BASE + 0x4C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_289_REG       (MC_BASE + 0x4C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_290_REG       (MC_BASE + 0x4C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_291_REG       (MC_BASE + 0x4CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_292_REG       (MC_BASE + 0x4D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_293_REG       (MC_BASE + 0x4D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_294_REG       (MC_BASE + 0x4D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_295_REG       (MC_BASE + 0x4DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_296_REG       (MC_BASE + 0x4E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_297_REG       (MC_BASE + 0x4E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_298_REG       (MC_BASE + 0x4E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_299_REG       (MC_BASE + 0x4EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_300_REG       (MC_BASE + 0x4F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_301_REG       (MC_BASE + 0x4F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_302_REG       (MC_BASE + 0x4F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_303_REG       (MC_BASE + 0x4FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_304_REG       (MC_BASE + 0x500)  /* Index pairs from CMP */
#define MC_INDEX_CFG_305_REG       (MC_BASE + 0x504)  /* Index pairs from CMP */
#define MC_INDEX_CFG_306_REG       (MC_BASE + 0x508)  /* Index pairs from CMP */
#define MC_INDEX_CFG_307_REG       (MC_BASE + 0x50C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_308_REG       (MC_BASE + 0x510)  /* Index pairs from CMP */
#define MC_INDEX_CFG_309_REG       (MC_BASE + 0x514)  /* Index pairs from CMP */
#define MC_INDEX_CFG_310_REG       (MC_BASE + 0x518)  /* Index pairs from CMP */
#define MC_INDEX_CFG_311_REG       (MC_BASE + 0x51C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_312_REG       (MC_BASE + 0x520)  /* Index pairs from CMP */
#define MC_INDEX_CFG_313_REG       (MC_BASE + 0x524)  /* Index pairs from CMP */
#define MC_INDEX_CFG_314_REG       (MC_BASE + 0x528)  /* Index pairs from CMP */
#define MC_INDEX_CFG_315_REG       (MC_BASE + 0x52C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_316_REG       (MC_BASE + 0x530)  /* Index pairs from CMP */
#define MC_INDEX_CFG_317_REG       (MC_BASE + 0x534)  /* Index pairs from CMP */
#define MC_INDEX_CFG_318_REG       (MC_BASE + 0x538)  /* Index pairs from CMP */
#define MC_INDEX_CFG_319_REG       (MC_BASE + 0x53C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_320_REG       (MC_BASE + 0x540)  /* Index pairs from CMP */
#define MC_INDEX_CFG_321_REG       (MC_BASE + 0x544)  /* Index pairs from CMP */
#define MC_INDEX_CFG_322_REG       (MC_BASE + 0x548)  /* Index pairs from CMP */
#define MC_INDEX_CFG_323_REG       (MC_BASE + 0x54C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_324_REG       (MC_BASE + 0x550)  /* Index pairs from CMP */
#define MC_INDEX_CFG_325_REG       (MC_BASE + 0x554)  /* Index pairs from CMP */
#define MC_INDEX_CFG_326_REG       (MC_BASE + 0x558)  /* Index pairs from CMP */
#define MC_INDEX_CFG_327_REG       (MC_BASE + 0x55C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_328_REG       (MC_BASE + 0x560)  /* Index pairs from CMP */
#define MC_INDEX_CFG_329_REG       (MC_BASE + 0x564)  /* Index pairs from CMP */
#define MC_INDEX_CFG_330_REG       (MC_BASE + 0x568)  /* Index pairs from CMP */
#define MC_INDEX_CFG_331_REG       (MC_BASE + 0x56C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_332_REG       (MC_BASE + 0x570)  /* Index pairs from CMP */
#define MC_INDEX_CFG_333_REG       (MC_BASE + 0x574)  /* Index pairs from CMP */
#define MC_INDEX_CFG_334_REG       (MC_BASE + 0x578)  /* Index pairs from CMP */
#define MC_INDEX_CFG_335_REG       (MC_BASE + 0x57C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_336_REG       (MC_BASE + 0x580)  /* Index pairs from CMP */
#define MC_INDEX_CFG_337_REG       (MC_BASE + 0x584)  /* Index pairs from CMP */
#define MC_INDEX_CFG_338_REG       (MC_BASE + 0x588)  /* Index pairs from CMP */
#define MC_INDEX_CFG_339_REG       (MC_BASE + 0x58C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_340_REG       (MC_BASE + 0x590)  /* Index pairs from CMP */
#define MC_INDEX_CFG_341_REG       (MC_BASE + 0x594)  /* Index pairs from CMP */
#define MC_INDEX_CFG_342_REG       (MC_BASE + 0x598)  /* Index pairs from CMP */
#define MC_INDEX_CFG_343_REG       (MC_BASE + 0x59C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_344_REG       (MC_BASE + 0x5A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_345_REG       (MC_BASE + 0x5A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_346_REG       (MC_BASE + 0x5A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_347_REG       (MC_BASE + 0x5AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_348_REG       (MC_BASE + 0x5B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_349_REG       (MC_BASE + 0x5B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_350_REG       (MC_BASE + 0x5B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_351_REG       (MC_BASE + 0x5BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_352_REG       (MC_BASE + 0x5C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_353_REG       (MC_BASE + 0x5C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_354_REG       (MC_BASE + 0x5C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_355_REG       (MC_BASE + 0x5CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_356_REG       (MC_BASE + 0x5D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_357_REG       (MC_BASE + 0x5D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_358_REG       (MC_BASE + 0x5D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_359_REG       (MC_BASE + 0x5DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_360_REG       (MC_BASE + 0x5E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_361_REG       (MC_BASE + 0x5E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_362_REG       (MC_BASE + 0x5E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_363_REG       (MC_BASE + 0x5EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_364_REG       (MC_BASE + 0x5F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_365_REG       (MC_BASE + 0x5F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_366_REG       (MC_BASE + 0x5F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_367_REG       (MC_BASE + 0x5FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_368_REG       (MC_BASE + 0x600)  /* Index pairs from CMP */
#define MC_INDEX_CFG_369_REG       (MC_BASE + 0x604)  /* Index pairs from CMP */
#define MC_INDEX_CFG_370_REG       (MC_BASE + 0x608)  /* Index pairs from CMP */
#define MC_INDEX_CFG_371_REG       (MC_BASE + 0x60C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_372_REG       (MC_BASE + 0x610)  /* Index pairs from CMP */
#define MC_INDEX_CFG_373_REG       (MC_BASE + 0x614)  /* Index pairs from CMP */
#define MC_INDEX_CFG_374_REG       (MC_BASE + 0x618)  /* Index pairs from CMP */
#define MC_INDEX_CFG_375_REG       (MC_BASE + 0x61C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_376_REG       (MC_BASE + 0x620)  /* Index pairs from CMP */
#define MC_INDEX_CFG_377_REG       (MC_BASE + 0x624)  /* Index pairs from CMP */
#define MC_INDEX_CFG_378_REG       (MC_BASE + 0x628)  /* Index pairs from CMP */
#define MC_INDEX_CFG_379_REG       (MC_BASE + 0x62C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_380_REG       (MC_BASE + 0x630)  /* Index pairs from CMP */
#define MC_INDEX_CFG_381_REG       (MC_BASE + 0x634)  /* Index pairs from CMP */
#define MC_INDEX_CFG_382_REG       (MC_BASE + 0x638)  /* Index pairs from CMP */
#define MC_INDEX_CFG_383_REG       (MC_BASE + 0x63C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_384_REG       (MC_BASE + 0x640)  /* Index pairs from CMP */
#define MC_INDEX_CFG_385_REG       (MC_BASE + 0x644)  /* Index pairs from CMP */
#define MC_INDEX_CFG_386_REG       (MC_BASE + 0x648)  /* Index pairs from CMP */
#define MC_INDEX_CFG_387_REG       (MC_BASE + 0x64C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_388_REG       (MC_BASE + 0x650)  /* Index pairs from CMP */
#define MC_INDEX_CFG_389_REG       (MC_BASE + 0x654)  /* Index pairs from CMP */
#define MC_INDEX_CFG_390_REG       (MC_BASE + 0x658)  /* Index pairs from CMP */
#define MC_INDEX_CFG_391_REG       (MC_BASE + 0x65C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_392_REG       (MC_BASE + 0x660)  /* Index pairs from CMP */
#define MC_INDEX_CFG_393_REG       (MC_BASE + 0x664)  /* Index pairs from CMP */
#define MC_INDEX_CFG_394_REG       (MC_BASE + 0x668)  /* Index pairs from CMP */
#define MC_INDEX_CFG_395_REG       (MC_BASE + 0x66C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_396_REG       (MC_BASE + 0x670)  /* Index pairs from CMP */
#define MC_INDEX_CFG_397_REG       (MC_BASE + 0x674)  /* Index pairs from CMP */
#define MC_INDEX_CFG_398_REG       (MC_BASE + 0x678)  /* Index pairs from CMP */
#define MC_INDEX_CFG_399_REG       (MC_BASE + 0x67C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_400_REG       (MC_BASE + 0x680)  /* Index pairs from CMP */
#define MC_INDEX_CFG_401_REG       (MC_BASE + 0x684)  /* Index pairs from CMP */
#define MC_INDEX_CFG_402_REG       (MC_BASE + 0x688)  /* Index pairs from CMP */
#define MC_INDEX_CFG_403_REG       (MC_BASE + 0x68C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_404_REG       (MC_BASE + 0x690)  /* Index pairs from CMP */
#define MC_INDEX_CFG_405_REG       (MC_BASE + 0x694)  /* Index pairs from CMP */
#define MC_INDEX_CFG_406_REG       (MC_BASE + 0x698)  /* Index pairs from CMP */
#define MC_INDEX_CFG_407_REG       (MC_BASE + 0x69C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_408_REG       (MC_BASE + 0x6A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_409_REG       (MC_BASE + 0x6A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_410_REG       (MC_BASE + 0x6A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_411_REG       (MC_BASE + 0x6AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_412_REG       (MC_BASE + 0x6B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_413_REG       (MC_BASE + 0x6B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_414_REG       (MC_BASE + 0x6B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_415_REG       (MC_BASE + 0x6BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_416_REG       (MC_BASE + 0x6C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_417_REG       (MC_BASE + 0x6C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_418_REG       (MC_BASE + 0x6C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_419_REG       (MC_BASE + 0x6CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_420_REG       (MC_BASE + 0x6D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_421_REG       (MC_BASE + 0x6D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_422_REG       (MC_BASE + 0x6D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_423_REG       (MC_BASE + 0x6DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_424_REG       (MC_BASE + 0x6E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_425_REG       (MC_BASE + 0x6E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_426_REG       (MC_BASE + 0x6E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_427_REG       (MC_BASE + 0x6EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_428_REG       (MC_BASE + 0x6F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_429_REG       (MC_BASE + 0x6F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_430_REG       (MC_BASE + 0x6F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_431_REG       (MC_BASE + 0x6FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_432_REG       (MC_BASE + 0x700)  /* Index pairs from CMP */
#define MC_INDEX_CFG_433_REG       (MC_BASE + 0x704)  /* Index pairs from CMP */
#define MC_INDEX_CFG_434_REG       (MC_BASE + 0x708)  /* Index pairs from CMP */
#define MC_INDEX_CFG_435_REG       (MC_BASE + 0x70C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_436_REG       (MC_BASE + 0x710)  /* Index pairs from CMP */
#define MC_INDEX_CFG_437_REG       (MC_BASE + 0x714)  /* Index pairs from CMP */
#define MC_INDEX_CFG_438_REG       (MC_BASE + 0x718)  /* Index pairs from CMP */
#define MC_INDEX_CFG_439_REG       (MC_BASE + 0x71C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_440_REG       (MC_BASE + 0x720)  /* Index pairs from CMP */
#define MC_INDEX_CFG_441_REG       (MC_BASE + 0x724)  /* Index pairs from CMP */
#define MC_INDEX_CFG_442_REG       (MC_BASE + 0x728)  /* Index pairs from CMP */
#define MC_INDEX_CFG_443_REG       (MC_BASE + 0x72C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_444_REG       (MC_BASE + 0x730)  /* Index pairs from CMP */
#define MC_INDEX_CFG_445_REG       (MC_BASE + 0x734)  /* Index pairs from CMP */
#define MC_INDEX_CFG_446_REG       (MC_BASE + 0x738)  /* Index pairs from CMP */
#define MC_INDEX_CFG_447_REG       (MC_BASE + 0x73C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_448_REG       (MC_BASE + 0x740)  /* Index pairs from CMP */
#define MC_INDEX_CFG_449_REG       (MC_BASE + 0x744)  /* Index pairs from CMP */
#define MC_INDEX_CFG_450_REG       (MC_BASE + 0x748)  /* Index pairs from CMP */
#define MC_INDEX_CFG_451_REG       (MC_BASE + 0x74C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_452_REG       (MC_BASE + 0x750)  /* Index pairs from CMP */
#define MC_INDEX_CFG_453_REG       (MC_BASE + 0x754)  /* Index pairs from CMP */
#define MC_INDEX_CFG_454_REG       (MC_BASE + 0x758)  /* Index pairs from CMP */
#define MC_INDEX_CFG_455_REG       (MC_BASE + 0x75C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_456_REG       (MC_BASE + 0x760)  /* Index pairs from CMP */
#define MC_INDEX_CFG_457_REG       (MC_BASE + 0x764)  /* Index pairs from CMP */
#define MC_INDEX_CFG_458_REG       (MC_BASE + 0x768)  /* Index pairs from CMP */
#define MC_INDEX_CFG_459_REG       (MC_BASE + 0x76C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_460_REG       (MC_BASE + 0x770)  /* Index pairs from CMP */
#define MC_INDEX_CFG_461_REG       (MC_BASE + 0x774)  /* Index pairs from CMP */
#define MC_INDEX_CFG_462_REG       (MC_BASE + 0x778)  /* Index pairs from CMP */
#define MC_INDEX_CFG_463_REG       (MC_BASE + 0x77C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_464_REG       (MC_BASE + 0x780)  /* Index pairs from CMP */
#define MC_INDEX_CFG_465_REG       (MC_BASE + 0x784)  /* Index pairs from CMP */
#define MC_INDEX_CFG_466_REG       (MC_BASE + 0x788)  /* Index pairs from CMP */
#define MC_INDEX_CFG_467_REG       (MC_BASE + 0x78C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_468_REG       (MC_BASE + 0x790)  /* Index pairs from CMP */
#define MC_INDEX_CFG_469_REG       (MC_BASE + 0x794)  /* Index pairs from CMP */
#define MC_INDEX_CFG_470_REG       (MC_BASE + 0x798)  /* Index pairs from CMP */
#define MC_INDEX_CFG_471_REG       (MC_BASE + 0x79C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_472_REG       (MC_BASE + 0x7A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_473_REG       (MC_BASE + 0x7A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_474_REG       (MC_BASE + 0x7A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_475_REG       (MC_BASE + 0x7AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_476_REG       (MC_BASE + 0x7B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_477_REG       (MC_BASE + 0x7B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_478_REG       (MC_BASE + 0x7B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_479_REG       (MC_BASE + 0x7BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_480_REG       (MC_BASE + 0x7C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_481_REG       (MC_BASE + 0x7C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_482_REG       (MC_BASE + 0x7C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_483_REG       (MC_BASE + 0x7CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_484_REG       (MC_BASE + 0x7D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_485_REG       (MC_BASE + 0x7D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_486_REG       (MC_BASE + 0x7D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_487_REG       (MC_BASE + 0x7DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_488_REG       (MC_BASE + 0x7E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_489_REG       (MC_BASE + 0x7E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_490_REG       (MC_BASE + 0x7E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_491_REG       (MC_BASE + 0x7EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_492_REG       (MC_BASE + 0x7F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_493_REG       (MC_BASE + 0x7F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_494_REG       (MC_BASE + 0x7F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_495_REG       (MC_BASE + 0x7FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_496_REG       (MC_BASE + 0x800)  /* Index pairs from CMP */
#define MC_INDEX_CFG_497_REG       (MC_BASE + 0x804)  /* Index pairs from CMP */
#define MC_INDEX_CFG_498_REG       (MC_BASE + 0x808)  /* Index pairs from CMP */
#define MC_INDEX_CFG_499_REG       (MC_BASE + 0x80C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_500_REG       (MC_BASE + 0x810)  /* Index pairs from CMP */
#define MC_INDEX_CFG_501_REG       (MC_BASE + 0x814)  /* Index pairs from CMP */
#define MC_INDEX_CFG_502_REG       (MC_BASE + 0x818)  /* Index pairs from CMP */
#define MC_INDEX_CFG_503_REG       (MC_BASE + 0x81C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_504_REG       (MC_BASE + 0x820)  /* Index pairs from CMP */
#define MC_INDEX_CFG_505_REG       (MC_BASE + 0x824)  /* Index pairs from CMP */
#define MC_INDEX_CFG_506_REG       (MC_BASE + 0x828)  /* Index pairs from CMP */
#define MC_INDEX_CFG_507_REG       (MC_BASE + 0x82C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_508_REG       (MC_BASE + 0x830)  /* Index pairs from CMP */
#define MC_INDEX_CFG_509_REG       (MC_BASE + 0x834)  /* Index pairs from CMP */
#define MC_INDEX_CFG_510_REG       (MC_BASE + 0x838)  /* Index pairs from CMP */
#define MC_INDEX_CFG_511_REG       (MC_BASE + 0x83C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_512_REG       (MC_BASE + 0x840)  /* Index pairs from CMP */
#define MC_INDEX_CFG_513_REG       (MC_BASE + 0x844)  /* Index pairs from CMP */
#define MC_INDEX_CFG_514_REG       (MC_BASE + 0x848)  /* Index pairs from CMP */
#define MC_INDEX_CFG_515_REG       (MC_BASE + 0x84C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_516_REG       (MC_BASE + 0x850)  /* Index pairs from CMP */
#define MC_INDEX_CFG_517_REG       (MC_BASE + 0x854)  /* Index pairs from CMP */
#define MC_INDEX_CFG_518_REG       (MC_BASE + 0x858)  /* Index pairs from CMP */
#define MC_INDEX_CFG_519_REG       (MC_BASE + 0x85C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_520_REG       (MC_BASE + 0x860)  /* Index pairs from CMP */
#define MC_INDEX_CFG_521_REG       (MC_BASE + 0x864)  /* Index pairs from CMP */
#define MC_INDEX_CFG_522_REG       (MC_BASE + 0x868)  /* Index pairs from CMP */
#define MC_INDEX_CFG_523_REG       (MC_BASE + 0x86C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_524_REG       (MC_BASE + 0x870)  /* Index pairs from CMP */
#define MC_INDEX_CFG_525_REG       (MC_BASE + 0x874)  /* Index pairs from CMP */
#define MC_INDEX_CFG_526_REG       (MC_BASE + 0x878)  /* Index pairs from CMP */
#define MC_INDEX_CFG_527_REG       (MC_BASE + 0x87C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_528_REG       (MC_BASE + 0x880)  /* Index pairs from CMP */
#define MC_INDEX_CFG_529_REG       (MC_BASE + 0x884)  /* Index pairs from CMP */
#define MC_INDEX_CFG_530_REG       (MC_BASE + 0x888)  /* Index pairs from CMP */
#define MC_INDEX_CFG_531_REG       (MC_BASE + 0x88C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_532_REG       (MC_BASE + 0x890)  /* Index pairs from CMP */
#define MC_INDEX_CFG_533_REG       (MC_BASE + 0x894)  /* Index pairs from CMP */
#define MC_INDEX_CFG_534_REG       (MC_BASE + 0x898)  /* Index pairs from CMP */
#define MC_INDEX_CFG_535_REG       (MC_BASE + 0x89C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_536_REG       (MC_BASE + 0x8A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_537_REG       (MC_BASE + 0x8A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_538_REG       (MC_BASE + 0x8A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_539_REG       (MC_BASE + 0x8AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_540_REG       (MC_BASE + 0x8B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_541_REG       (MC_BASE + 0x8B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_542_REG       (MC_BASE + 0x8B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_543_REG       (MC_BASE + 0x8BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_544_REG       (MC_BASE + 0x8C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_545_REG       (MC_BASE + 0x8C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_546_REG       (MC_BASE + 0x8C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_547_REG       (MC_BASE + 0x8CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_548_REG       (MC_BASE + 0x8D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_549_REG       (MC_BASE + 0x8D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_550_REG       (MC_BASE + 0x8D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_551_REG       (MC_BASE + 0x8DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_552_REG       (MC_BASE + 0x8E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_553_REG       (MC_BASE + 0x8E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_554_REG       (MC_BASE + 0x8E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_555_REG       (MC_BASE + 0x8EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_556_REG       (MC_BASE + 0x8F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_557_REG       (MC_BASE + 0x8F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_558_REG       (MC_BASE + 0x8F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_559_REG       (MC_BASE + 0x8FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_560_REG       (MC_BASE + 0x900)  /* Index pairs from CMP */
#define MC_INDEX_CFG_561_REG       (MC_BASE + 0x904)  /* Index pairs from CMP */
#define MC_INDEX_CFG_562_REG       (MC_BASE + 0x908)  /* Index pairs from CMP */
#define MC_INDEX_CFG_563_REG       (MC_BASE + 0x90C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_564_REG       (MC_BASE + 0x910)  /* Index pairs from CMP */
#define MC_INDEX_CFG_565_REG       (MC_BASE + 0x914)  /* Index pairs from CMP */
#define MC_INDEX_CFG_566_REG       (MC_BASE + 0x918)  /* Index pairs from CMP */
#define MC_INDEX_CFG_567_REG       (MC_BASE + 0x91C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_568_REG       (MC_BASE + 0x920)  /* Index pairs from CMP */
#define MC_INDEX_CFG_569_REG       (MC_BASE + 0x924)  /* Index pairs from CMP */
#define MC_INDEX_CFG_570_REG       (MC_BASE + 0x928)  /* Index pairs from CMP */
#define MC_INDEX_CFG_571_REG       (MC_BASE + 0x92C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_572_REG       (MC_BASE + 0x930)  /* Index pairs from CMP */
#define MC_INDEX_CFG_573_REG       (MC_BASE + 0x934)  /* Index pairs from CMP */
#define MC_INDEX_CFG_574_REG       (MC_BASE + 0x938)  /* Index pairs from CMP */
#define MC_INDEX_CFG_575_REG       (MC_BASE + 0x93C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_576_REG       (MC_BASE + 0x940)  /* Index pairs from CMP */
#define MC_INDEX_CFG_577_REG       (MC_BASE + 0x944)  /* Index pairs from CMP */
#define MC_INDEX_CFG_578_REG       (MC_BASE + 0x948)  /* Index pairs from CMP */
#define MC_INDEX_CFG_579_REG       (MC_BASE + 0x94C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_580_REG       (MC_BASE + 0x950)  /* Index pairs from CMP */
#define MC_INDEX_CFG_581_REG       (MC_BASE + 0x954)  /* Index pairs from CMP */
#define MC_INDEX_CFG_582_REG       (MC_BASE + 0x958)  /* Index pairs from CMP */
#define MC_INDEX_CFG_583_REG       (MC_BASE + 0x95C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_584_REG       (MC_BASE + 0x960)  /* Index pairs from CMP */
#define MC_INDEX_CFG_585_REG       (MC_BASE + 0x964)  /* Index pairs from CMP */
#define MC_INDEX_CFG_586_REG       (MC_BASE + 0x968)  /* Index pairs from CMP */
#define MC_INDEX_CFG_587_REG       (MC_BASE + 0x96C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_588_REG       (MC_BASE + 0x970)  /* Index pairs from CMP */
#define MC_INDEX_CFG_589_REG       (MC_BASE + 0x974)  /* Index pairs from CMP */
#define MC_INDEX_CFG_590_REG       (MC_BASE + 0x978)  /* Index pairs from CMP */
#define MC_INDEX_CFG_591_REG       (MC_BASE + 0x97C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_592_REG       (MC_BASE + 0x980)  /* Index pairs from CMP */
#define MC_INDEX_CFG_593_REG       (MC_BASE + 0x984)  /* Index pairs from CMP */
#define MC_INDEX_CFG_594_REG       (MC_BASE + 0x988)  /* Index pairs from CMP */
#define MC_INDEX_CFG_595_REG       (MC_BASE + 0x98C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_596_REG       (MC_BASE + 0x990)  /* Index pairs from CMP */
#define MC_INDEX_CFG_597_REG       (MC_BASE + 0x994)  /* Index pairs from CMP */
#define MC_INDEX_CFG_598_REG       (MC_BASE + 0x998)  /* Index pairs from CMP */
#define MC_INDEX_CFG_599_REG       (MC_BASE + 0x99C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_600_REG       (MC_BASE + 0x9A0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_601_REG       (MC_BASE + 0x9A4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_602_REG       (MC_BASE + 0x9A8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_603_REG       (MC_BASE + 0x9AC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_604_REG       (MC_BASE + 0x9B0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_605_REG       (MC_BASE + 0x9B4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_606_REG       (MC_BASE + 0x9B8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_607_REG       (MC_BASE + 0x9BC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_608_REG       (MC_BASE + 0x9C0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_609_REG       (MC_BASE + 0x9C4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_610_REG       (MC_BASE + 0x9C8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_611_REG       (MC_BASE + 0x9CC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_612_REG       (MC_BASE + 0x9D0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_613_REG       (MC_BASE + 0x9D4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_614_REG       (MC_BASE + 0x9D8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_615_REG       (MC_BASE + 0x9DC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_616_REG       (MC_BASE + 0x9E0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_617_REG       (MC_BASE + 0x9E4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_618_REG       (MC_BASE + 0x9E8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_619_REG       (MC_BASE + 0x9EC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_620_REG       (MC_BASE + 0x9F0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_621_REG       (MC_BASE + 0x9F4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_622_REG       (MC_BASE + 0x9F8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_623_REG       (MC_BASE + 0x9FC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_624_REG       (MC_BASE + 0xA00)  /* Index pairs from CMP */
#define MC_INDEX_CFG_625_REG       (MC_BASE + 0xA04)  /* Index pairs from CMP */
#define MC_INDEX_CFG_626_REG       (MC_BASE + 0xA08)  /* Index pairs from CMP */
#define MC_INDEX_CFG_627_REG       (MC_BASE + 0xA0C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_628_REG       (MC_BASE + 0xA10)  /* Index pairs from CMP */
#define MC_INDEX_CFG_629_REG       (MC_BASE + 0xA14)  /* Index pairs from CMP */
#define MC_INDEX_CFG_630_REG       (MC_BASE + 0xA18)  /* Index pairs from CMP */
#define MC_INDEX_CFG_631_REG       (MC_BASE + 0xA1C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_632_REG       (MC_BASE + 0xA20)  /* Index pairs from CMP */
#define MC_INDEX_CFG_633_REG       (MC_BASE + 0xA24)  /* Index pairs from CMP */
#define MC_INDEX_CFG_634_REG       (MC_BASE + 0xA28)  /* Index pairs from CMP */
#define MC_INDEX_CFG_635_REG       (MC_BASE + 0xA2C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_636_REG       (MC_BASE + 0xA30)  /* Index pairs from CMP */
#define MC_INDEX_CFG_637_REG       (MC_BASE + 0xA34)  /* Index pairs from CMP */
#define MC_INDEX_CFG_638_REG       (MC_BASE + 0xA38)  /* Index pairs from CMP */
#define MC_INDEX_CFG_639_REG       (MC_BASE + 0xA3C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_640_REG       (MC_BASE + 0xA40)  /* Index pairs from CMP */
#define MC_INDEX_CFG_641_REG       (MC_BASE + 0xA44)  /* Index pairs from CMP */
#define MC_INDEX_CFG_642_REG       (MC_BASE + 0xA48)  /* Index pairs from CMP */
#define MC_INDEX_CFG_643_REG       (MC_BASE + 0xA4C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_644_REG       (MC_BASE + 0xA50)  /* Index pairs from CMP */
#define MC_INDEX_CFG_645_REG       (MC_BASE + 0xA54)  /* Index pairs from CMP */
#define MC_INDEX_CFG_646_REG       (MC_BASE + 0xA58)  /* Index pairs from CMP */
#define MC_INDEX_CFG_647_REG       (MC_BASE + 0xA5C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_648_REG       (MC_BASE + 0xA60)  /* Index pairs from CMP */
#define MC_INDEX_CFG_649_REG       (MC_BASE + 0xA64)  /* Index pairs from CMP */
#define MC_INDEX_CFG_650_REG       (MC_BASE + 0xA68)  /* Index pairs from CMP */
#define MC_INDEX_CFG_651_REG       (MC_BASE + 0xA6C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_652_REG       (MC_BASE + 0xA70)  /* Index pairs from CMP */
#define MC_INDEX_CFG_653_REG       (MC_BASE + 0xA74)  /* Index pairs from CMP */
#define MC_INDEX_CFG_654_REG       (MC_BASE + 0xA78)  /* Index pairs from CMP */
#define MC_INDEX_CFG_655_REG       (MC_BASE + 0xA7C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_656_REG       (MC_BASE + 0xA80)  /* Index pairs from CMP */
#define MC_INDEX_CFG_657_REG       (MC_BASE + 0xA84)  /* Index pairs from CMP */
#define MC_INDEX_CFG_658_REG       (MC_BASE + 0xA88)  /* Index pairs from CMP */
#define MC_INDEX_CFG_659_REG       (MC_BASE + 0xA8C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_660_REG       (MC_BASE + 0xA90)  /* Index pairs from CMP */
#define MC_INDEX_CFG_661_REG       (MC_BASE + 0xA94)  /* Index pairs from CMP */
#define MC_INDEX_CFG_662_REG       (MC_BASE + 0xA98)  /* Index pairs from CMP */
#define MC_INDEX_CFG_663_REG       (MC_BASE + 0xA9C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_664_REG       (MC_BASE + 0xAA0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_665_REG       (MC_BASE + 0xAA4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_666_REG       (MC_BASE + 0xAA8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_667_REG       (MC_BASE + 0xAAC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_668_REG       (MC_BASE + 0xAB0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_669_REG       (MC_BASE + 0xAB4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_670_REG       (MC_BASE + 0xAB8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_671_REG       (MC_BASE + 0xABC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_672_REG       (MC_BASE + 0xAC0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_673_REG       (MC_BASE + 0xAC4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_674_REG       (MC_BASE + 0xAC8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_675_REG       (MC_BASE + 0xACC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_676_REG       (MC_BASE + 0xAD0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_677_REG       (MC_BASE + 0xAD4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_678_REG       (MC_BASE + 0xAD8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_679_REG       (MC_BASE + 0xADC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_680_REG       (MC_BASE + 0xAE0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_681_REG       (MC_BASE + 0xAE4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_682_REG       (MC_BASE + 0xAE8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_683_REG       (MC_BASE + 0xAEC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_684_REG       (MC_BASE + 0xAF0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_685_REG       (MC_BASE + 0xAF4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_686_REG       (MC_BASE + 0xAF8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_687_REG       (MC_BASE + 0xAFC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_688_REG       (MC_BASE + 0xB00)  /* Index pairs from CMP */
#define MC_INDEX_CFG_689_REG       (MC_BASE + 0xB04)  /* Index pairs from CMP */
#define MC_INDEX_CFG_690_REG       (MC_BASE + 0xB08)  /* Index pairs from CMP */
#define MC_INDEX_CFG_691_REG       (MC_BASE + 0xB0C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_692_REG       (MC_BASE + 0xB10)  /* Index pairs from CMP */
#define MC_INDEX_CFG_693_REG       (MC_BASE + 0xB14)  /* Index pairs from CMP */
#define MC_INDEX_CFG_694_REG       (MC_BASE + 0xB18)  /* Index pairs from CMP */
#define MC_INDEX_CFG_695_REG       (MC_BASE + 0xB1C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_696_REG       (MC_BASE + 0xB20)  /* Index pairs from CMP */
#define MC_INDEX_CFG_697_REG       (MC_BASE + 0xB24)  /* Index pairs from CMP */
#define MC_INDEX_CFG_698_REG       (MC_BASE + 0xB28)  /* Index pairs from CMP */
#define MC_INDEX_CFG_699_REG       (MC_BASE + 0xB2C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_700_REG       (MC_BASE + 0xB30)  /* Index pairs from CMP */
#define MC_INDEX_CFG_701_REG       (MC_BASE + 0xB34)  /* Index pairs from CMP */
#define MC_INDEX_CFG_702_REG       (MC_BASE + 0xB38)  /* Index pairs from CMP */
#define MC_INDEX_CFG_703_REG       (MC_BASE + 0xB3C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_704_REG       (MC_BASE + 0xB40)  /* Index pairs from CMP */
#define MC_INDEX_CFG_705_REG       (MC_BASE + 0xB44)  /* Index pairs from CMP */
#define MC_INDEX_CFG_706_REG       (MC_BASE + 0xB48)  /* Index pairs from CMP */
#define MC_INDEX_CFG_707_REG       (MC_BASE + 0xB4C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_708_REG       (MC_BASE + 0xB50)  /* Index pairs from CMP */
#define MC_INDEX_CFG_709_REG       (MC_BASE + 0xB54)  /* Index pairs from CMP */
#define MC_INDEX_CFG_710_REG       (MC_BASE + 0xB58)  /* Index pairs from CMP */
#define MC_INDEX_CFG_711_REG       (MC_BASE + 0xB5C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_712_REG       (MC_BASE + 0xB60)  /* Index pairs from CMP */
#define MC_INDEX_CFG_713_REG       (MC_BASE + 0xB64)  /* Index pairs from CMP */
#define MC_INDEX_CFG_714_REG       (MC_BASE + 0xB68)  /* Index pairs from CMP */
#define MC_INDEX_CFG_715_REG       (MC_BASE + 0xB6C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_716_REG       (MC_BASE + 0xB70)  /* Index pairs from CMP */
#define MC_INDEX_CFG_717_REG       (MC_BASE + 0xB74)  /* Index pairs from CMP */
#define MC_INDEX_CFG_718_REG       (MC_BASE + 0xB78)  /* Index pairs from CMP */
#define MC_INDEX_CFG_719_REG       (MC_BASE + 0xB7C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_720_REG       (MC_BASE + 0xB80)  /* Index pairs from CMP */
#define MC_INDEX_CFG_721_REG       (MC_BASE + 0xB84)  /* Index pairs from CMP */
#define MC_INDEX_CFG_722_REG       (MC_BASE + 0xB88)  /* Index pairs from CMP */
#define MC_INDEX_CFG_723_REG       (MC_BASE + 0xB8C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_724_REG       (MC_BASE + 0xB90)  /* Index pairs from CMP */
#define MC_INDEX_CFG_725_REG       (MC_BASE + 0xB94)  /* Index pairs from CMP */
#define MC_INDEX_CFG_726_REG       (MC_BASE + 0xB98)  /* Index pairs from CMP */
#define MC_INDEX_CFG_727_REG       (MC_BASE + 0xB9C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_728_REG       (MC_BASE + 0xBA0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_729_REG       (MC_BASE + 0xBA4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_730_REG       (MC_BASE + 0xBA8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_731_REG       (MC_BASE + 0xBAC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_732_REG       (MC_BASE + 0xBB0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_733_REG       (MC_BASE + 0xBB4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_734_REG       (MC_BASE + 0xBB8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_735_REG       (MC_BASE + 0xBBC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_736_REG       (MC_BASE + 0xBC0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_737_REG       (MC_BASE + 0xBC4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_738_REG       (MC_BASE + 0xBC8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_739_REG       (MC_BASE + 0xBCC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_740_REG       (MC_BASE + 0xBD0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_741_REG       (MC_BASE + 0xBD4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_742_REG       (MC_BASE + 0xBD8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_743_REG       (MC_BASE + 0xBDC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_744_REG       (MC_BASE + 0xBE0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_745_REG       (MC_BASE + 0xBE4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_746_REG       (MC_BASE + 0xBE8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_747_REG       (MC_BASE + 0xBEC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_748_REG       (MC_BASE + 0xBF0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_749_REG       (MC_BASE + 0xBF4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_750_REG       (MC_BASE + 0xBF8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_751_REG       (MC_BASE + 0xBFC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_752_REG       (MC_BASE + 0xC00)  /* Index pairs from CMP */
#define MC_INDEX_CFG_753_REG       (MC_BASE + 0xC04)  /* Index pairs from CMP */
#define MC_INDEX_CFG_754_REG       (MC_BASE + 0xC08)  /* Index pairs from CMP */
#define MC_INDEX_CFG_755_REG       (MC_BASE + 0xC0C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_756_REG       (MC_BASE + 0xC10)  /* Index pairs from CMP */
#define MC_INDEX_CFG_757_REG       (MC_BASE + 0xC14)  /* Index pairs from CMP */
#define MC_INDEX_CFG_758_REG       (MC_BASE + 0xC18)  /* Index pairs from CMP */
#define MC_INDEX_CFG_759_REG       (MC_BASE + 0xC1C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_760_REG       (MC_BASE + 0xC20)  /* Index pairs from CMP */
#define MC_INDEX_CFG_761_REG       (MC_BASE + 0xC24)  /* Index pairs from CMP */
#define MC_INDEX_CFG_762_REG       (MC_BASE + 0xC28)  /* Index pairs from CMP */
#define MC_INDEX_CFG_763_REG       (MC_BASE + 0xC2C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_764_REG       (MC_BASE + 0xC30)  /* Index pairs from CMP */
#define MC_INDEX_CFG_765_REG       (MC_BASE + 0xC34)  /* Index pairs from CMP */
#define MC_INDEX_CFG_766_REG       (MC_BASE + 0xC38)  /* Index pairs from CMP */
#define MC_INDEX_CFG_767_REG       (MC_BASE + 0xC3C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_768_REG       (MC_BASE + 0xC40)  /* Index pairs from CMP */
#define MC_INDEX_CFG_769_REG       (MC_BASE + 0xC44)  /* Index pairs from CMP */
#define MC_INDEX_CFG_770_REG       (MC_BASE + 0xC48)  /* Index pairs from CMP */
#define MC_INDEX_CFG_771_REG       (MC_BASE + 0xC4C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_772_REG       (MC_BASE + 0xC50)  /* Index pairs from CMP */
#define MC_INDEX_CFG_773_REG       (MC_BASE + 0xC54)  /* Index pairs from CMP */
#define MC_INDEX_CFG_774_REG       (MC_BASE + 0xC58)  /* Index pairs from CMP */
#define MC_INDEX_CFG_775_REG       (MC_BASE + 0xC5C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_776_REG       (MC_BASE + 0xC60)  /* Index pairs from CMP */
#define MC_INDEX_CFG_777_REG       (MC_BASE + 0xC64)  /* Index pairs from CMP */
#define MC_INDEX_CFG_778_REG       (MC_BASE + 0xC68)  /* Index pairs from CMP */
#define MC_INDEX_CFG_779_REG       (MC_BASE + 0xC6C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_780_REG       (MC_BASE + 0xC70)  /* Index pairs from CMP */
#define MC_INDEX_CFG_781_REG       (MC_BASE + 0xC74)  /* Index pairs from CMP */
#define MC_INDEX_CFG_782_REG       (MC_BASE + 0xC78)  /* Index pairs from CMP */
#define MC_INDEX_CFG_783_REG       (MC_BASE + 0xC7C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_784_REG       (MC_BASE + 0xC80)  /* Index pairs from CMP */
#define MC_INDEX_CFG_785_REG       (MC_BASE + 0xC84)  /* Index pairs from CMP */
#define MC_INDEX_CFG_786_REG       (MC_BASE + 0xC88)  /* Index pairs from CMP */
#define MC_INDEX_CFG_787_REG       (MC_BASE + 0xC8C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_788_REG       (MC_BASE + 0xC90)  /* Index pairs from CMP */
#define MC_INDEX_CFG_789_REG       (MC_BASE + 0xC94)  /* Index pairs from CMP */
#define MC_INDEX_CFG_790_REG       (MC_BASE + 0xC98)  /* Index pairs from CMP */
#define MC_INDEX_CFG_791_REG       (MC_BASE + 0xC9C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_792_REG       (MC_BASE + 0xCA0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_793_REG       (MC_BASE + 0xCA4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_794_REG       (MC_BASE + 0xCA8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_795_REG       (MC_BASE + 0xCAC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_796_REG       (MC_BASE + 0xCB0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_797_REG       (MC_BASE + 0xCB4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_798_REG       (MC_BASE + 0xCB8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_799_REG       (MC_BASE + 0xCBC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_800_REG       (MC_BASE + 0xCC0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_801_REG       (MC_BASE + 0xCC4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_802_REG       (MC_BASE + 0xCC8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_803_REG       (MC_BASE + 0xCCC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_804_REG       (MC_BASE + 0xCD0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_805_REG       (MC_BASE + 0xCD4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_806_REG       (MC_BASE + 0xCD8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_807_REG       (MC_BASE + 0xCDC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_808_REG       (MC_BASE + 0xCE0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_809_REG       (MC_BASE + 0xCE4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_810_REG       (MC_BASE + 0xCE8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_811_REG       (MC_BASE + 0xCEC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_812_REG       (MC_BASE + 0xCF0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_813_REG       (MC_BASE + 0xCF4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_814_REG       (MC_BASE + 0xCF8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_815_REG       (MC_BASE + 0xCFC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_816_REG       (MC_BASE + 0xD00)  /* Index pairs from CMP */
#define MC_INDEX_CFG_817_REG       (MC_BASE + 0xD04)  /* Index pairs from CMP */
#define MC_INDEX_CFG_818_REG       (MC_BASE + 0xD08)  /* Index pairs from CMP */
#define MC_INDEX_CFG_819_REG       (MC_BASE + 0xD0C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_820_REG       (MC_BASE + 0xD10)  /* Index pairs from CMP */
#define MC_INDEX_CFG_821_REG       (MC_BASE + 0xD14)  /* Index pairs from CMP */
#define MC_INDEX_CFG_822_REG       (MC_BASE + 0xD18)  /* Index pairs from CMP */
#define MC_INDEX_CFG_823_REG       (MC_BASE + 0xD1C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_824_REG       (MC_BASE + 0xD20)  /* Index pairs from CMP */
#define MC_INDEX_CFG_825_REG       (MC_BASE + 0xD24)  /* Index pairs from CMP */
#define MC_INDEX_CFG_826_REG       (MC_BASE + 0xD28)  /* Index pairs from CMP */
#define MC_INDEX_CFG_827_REG       (MC_BASE + 0xD2C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_828_REG       (MC_BASE + 0xD30)  /* Index pairs from CMP */
#define MC_INDEX_CFG_829_REG       (MC_BASE + 0xD34)  /* Index pairs from CMP */
#define MC_INDEX_CFG_830_REG       (MC_BASE + 0xD38)  /* Index pairs from CMP */
#define MC_INDEX_CFG_831_REG       (MC_BASE + 0xD3C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_832_REG       (MC_BASE + 0xD40)  /* Index pairs from CMP */
#define MC_INDEX_CFG_833_REG       (MC_BASE + 0xD44)  /* Index pairs from CMP */
#define MC_INDEX_CFG_834_REG       (MC_BASE + 0xD48)  /* Index pairs from CMP */
#define MC_INDEX_CFG_835_REG       (MC_BASE + 0xD4C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_836_REG       (MC_BASE + 0xD50)  /* Index pairs from CMP */
#define MC_INDEX_CFG_837_REG       (MC_BASE + 0xD54)  /* Index pairs from CMP */
#define MC_INDEX_CFG_838_REG       (MC_BASE + 0xD58)  /* Index pairs from CMP */
#define MC_INDEX_CFG_839_REG       (MC_BASE + 0xD5C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_840_REG       (MC_BASE + 0xD60)  /* Index pairs from CMP */
#define MC_INDEX_CFG_841_REG       (MC_BASE + 0xD64)  /* Index pairs from CMP */
#define MC_INDEX_CFG_842_REG       (MC_BASE + 0xD68)  /* Index pairs from CMP */
#define MC_INDEX_CFG_843_REG       (MC_BASE + 0xD6C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_844_REG       (MC_BASE + 0xD70)  /* Index pairs from CMP */
#define MC_INDEX_CFG_845_REG       (MC_BASE + 0xD74)  /* Index pairs from CMP */
#define MC_INDEX_CFG_846_REG       (MC_BASE + 0xD78)  /* Index pairs from CMP */
#define MC_INDEX_CFG_847_REG       (MC_BASE + 0xD7C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_848_REG       (MC_BASE + 0xD80)  /* Index pairs from CMP */
#define MC_INDEX_CFG_849_REG       (MC_BASE + 0xD84)  /* Index pairs from CMP */
#define MC_INDEX_CFG_850_REG       (MC_BASE + 0xD88)  /* Index pairs from CMP */
#define MC_INDEX_CFG_851_REG       (MC_BASE + 0xD8C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_852_REG       (MC_BASE + 0xD90)  /* Index pairs from CMP */
#define MC_INDEX_CFG_853_REG       (MC_BASE + 0xD94)  /* Index pairs from CMP */
#define MC_INDEX_CFG_854_REG       (MC_BASE + 0xD98)  /* Index pairs from CMP */
#define MC_INDEX_CFG_855_REG       (MC_BASE + 0xD9C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_856_REG       (MC_BASE + 0xDA0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_857_REG       (MC_BASE + 0xDA4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_858_REG       (MC_BASE + 0xDA8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_859_REG       (MC_BASE + 0xDAC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_860_REG       (MC_BASE + 0xDB0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_861_REG       (MC_BASE + 0xDB4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_862_REG       (MC_BASE + 0xDB8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_863_REG       (MC_BASE + 0xDBC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_864_REG       (MC_BASE + 0xDC0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_865_REG       (MC_BASE + 0xDC4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_866_REG       (MC_BASE + 0xDC8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_867_REG       (MC_BASE + 0xDCC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_868_REG       (MC_BASE + 0xDD0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_869_REG       (MC_BASE + 0xDD4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_870_REG       (MC_BASE + 0xDD8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_871_REG       (MC_BASE + 0xDDC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_872_REG       (MC_BASE + 0xDE0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_873_REG       (MC_BASE + 0xDE4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_874_REG       (MC_BASE + 0xDE8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_875_REG       (MC_BASE + 0xDEC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_876_REG       (MC_BASE + 0xDF0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_877_REG       (MC_BASE + 0xDF4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_878_REG       (MC_BASE + 0xDF8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_879_REG       (MC_BASE + 0xDFC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_880_REG       (MC_BASE + 0xE00)  /* Index pairs from CMP */
#define MC_INDEX_CFG_881_REG       (MC_BASE + 0xE04)  /* Index pairs from CMP */
#define MC_INDEX_CFG_882_REG       (MC_BASE + 0xE08)  /* Index pairs from CMP */
#define MC_INDEX_CFG_883_REG       (MC_BASE + 0xE0C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_884_REG       (MC_BASE + 0xE10)  /* Index pairs from CMP */
#define MC_INDEX_CFG_885_REG       (MC_BASE + 0xE14)  /* Index pairs from CMP */
#define MC_INDEX_CFG_886_REG       (MC_BASE + 0xE18)  /* Index pairs from CMP */
#define MC_INDEX_CFG_887_REG       (MC_BASE + 0xE1C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_888_REG       (MC_BASE + 0xE20)  /* Index pairs from CMP */
#define MC_INDEX_CFG_889_REG       (MC_BASE + 0xE24)  /* Index pairs from CMP */
#define MC_INDEX_CFG_890_REG       (MC_BASE + 0xE28)  /* Index pairs from CMP */
#define MC_INDEX_CFG_891_REG       (MC_BASE + 0xE2C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_892_REG       (MC_BASE + 0xE30)  /* Index pairs from CMP */
#define MC_INDEX_CFG_893_REG       (MC_BASE + 0xE34)  /* Index pairs from CMP */
#define MC_INDEX_CFG_894_REG       (MC_BASE + 0xE38)  /* Index pairs from CMP */
#define MC_INDEX_CFG_895_REG       (MC_BASE + 0xE3C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_896_REG       (MC_BASE + 0xE40)  /* Index pairs from CMP */
#define MC_INDEX_CFG_897_REG       (MC_BASE + 0xE44)  /* Index pairs from CMP */
#define MC_INDEX_CFG_898_REG       (MC_BASE + 0xE48)  /* Index pairs from CMP */
#define MC_INDEX_CFG_899_REG       (MC_BASE + 0xE4C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_900_REG       (MC_BASE + 0xE50)  /* Index pairs from CMP */
#define MC_INDEX_CFG_901_REG       (MC_BASE + 0xE54)  /* Index pairs from CMP */
#define MC_INDEX_CFG_902_REG       (MC_BASE + 0xE58)  /* Index pairs from CMP */
#define MC_INDEX_CFG_903_REG       (MC_BASE + 0xE5C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_904_REG       (MC_BASE + 0xE60)  /* Index pairs from CMP */
#define MC_INDEX_CFG_905_REG       (MC_BASE + 0xE64)  /* Index pairs from CMP */
#define MC_INDEX_CFG_906_REG       (MC_BASE + 0xE68)  /* Index pairs from CMP */
#define MC_INDEX_CFG_907_REG       (MC_BASE + 0xE6C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_908_REG       (MC_BASE + 0xE70)  /* Index pairs from CMP */
#define MC_INDEX_CFG_909_REG       (MC_BASE + 0xE74)  /* Index pairs from CMP */
#define MC_INDEX_CFG_910_REG       (MC_BASE + 0xE78)  /* Index pairs from CMP */
#define MC_INDEX_CFG_911_REG       (MC_BASE + 0xE7C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_912_REG       (MC_BASE + 0xE80)  /* Index pairs from CMP */
#define MC_INDEX_CFG_913_REG       (MC_BASE + 0xE84)  /* Index pairs from CMP */
#define MC_INDEX_CFG_914_REG       (MC_BASE + 0xE88)  /* Index pairs from CMP */
#define MC_INDEX_CFG_915_REG       (MC_BASE + 0xE8C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_916_REG       (MC_BASE + 0xE90)  /* Index pairs from CMP */
#define MC_INDEX_CFG_917_REG       (MC_BASE + 0xE94)  /* Index pairs from CMP */
#define MC_INDEX_CFG_918_REG       (MC_BASE + 0xE98)  /* Index pairs from CMP */
#define MC_INDEX_CFG_919_REG       (MC_BASE + 0xE9C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_920_REG       (MC_BASE + 0xEA0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_921_REG       (MC_BASE + 0xEA4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_922_REG       (MC_BASE + 0xEA8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_923_REG       (MC_BASE + 0xEAC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_924_REG       (MC_BASE + 0xEB0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_925_REG       (MC_BASE + 0xEB4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_926_REG       (MC_BASE + 0xEB8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_927_REG       (MC_BASE + 0xEBC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_928_REG       (MC_BASE + 0xEC0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_929_REG       (MC_BASE + 0xEC4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_930_REG       (MC_BASE + 0xEC8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_931_REG       (MC_BASE + 0xECC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_932_REG       (MC_BASE + 0xED0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_933_REG       (MC_BASE + 0xED4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_934_REG       (MC_BASE + 0xED8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_935_REG       (MC_BASE + 0xEDC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_936_REG       (MC_BASE + 0xEE0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_937_REG       (MC_BASE + 0xEE4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_938_REG       (MC_BASE + 0xEE8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_939_REG       (MC_BASE + 0xEEC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_940_REG       (MC_BASE + 0xEF0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_941_REG       (MC_BASE + 0xEF4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_942_REG       (MC_BASE + 0xEF8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_943_REG       (MC_BASE + 0xEFC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_944_REG       (MC_BASE + 0xF00)  /* Index pairs from CMP */
#define MC_INDEX_CFG_945_REG       (MC_BASE + 0xF04)  /* Index pairs from CMP */
#define MC_INDEX_CFG_946_REG       (MC_BASE + 0xF08)  /* Index pairs from CMP */
#define MC_INDEX_CFG_947_REG       (MC_BASE + 0xF0C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_948_REG       (MC_BASE + 0xF10)  /* Index pairs from CMP */
#define MC_INDEX_CFG_949_REG       (MC_BASE + 0xF14)  /* Index pairs from CMP */
#define MC_INDEX_CFG_950_REG       (MC_BASE + 0xF18)  /* Index pairs from CMP */
#define MC_INDEX_CFG_951_REG       (MC_BASE + 0xF1C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_952_REG       (MC_BASE + 0xF20)  /* Index pairs from CMP */
#define MC_INDEX_CFG_953_REG       (MC_BASE + 0xF24)  /* Index pairs from CMP */
#define MC_INDEX_CFG_954_REG       (MC_BASE + 0xF28)  /* Index pairs from CMP */
#define MC_INDEX_CFG_955_REG       (MC_BASE + 0xF2C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_956_REG       (MC_BASE + 0xF30)  /* Index pairs from CMP */
#define MC_INDEX_CFG_957_REG       (MC_BASE + 0xF34)  /* Index pairs from CMP */
#define MC_INDEX_CFG_958_REG       (MC_BASE + 0xF38)  /* Index pairs from CMP */
#define MC_INDEX_CFG_959_REG       (MC_BASE + 0xF3C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_960_REG       (MC_BASE + 0xF40)  /* Index pairs from CMP */
#define MC_INDEX_CFG_961_REG       (MC_BASE + 0xF44)  /* Index pairs from CMP */
#define MC_INDEX_CFG_962_REG       (MC_BASE + 0xF48)  /* Index pairs from CMP */
#define MC_INDEX_CFG_963_REG       (MC_BASE + 0xF4C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_964_REG       (MC_BASE + 0xF50)  /* Index pairs from CMP */
#define MC_INDEX_CFG_965_REG       (MC_BASE + 0xF54)  /* Index pairs from CMP */
#define MC_INDEX_CFG_966_REG       (MC_BASE + 0xF58)  /* Index pairs from CMP */
#define MC_INDEX_CFG_967_REG       (MC_BASE + 0xF5C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_968_REG       (MC_BASE + 0xF60)  /* Index pairs from CMP */
#define MC_INDEX_CFG_969_REG       (MC_BASE + 0xF64)  /* Index pairs from CMP */
#define MC_INDEX_CFG_970_REG       (MC_BASE + 0xF68)  /* Index pairs from CMP */
#define MC_INDEX_CFG_971_REG       (MC_BASE + 0xF6C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_972_REG       (MC_BASE + 0xF70)  /* Index pairs from CMP */
#define MC_INDEX_CFG_973_REG       (MC_BASE + 0xF74)  /* Index pairs from CMP */
#define MC_INDEX_CFG_974_REG       (MC_BASE + 0xF78)  /* Index pairs from CMP */
#define MC_INDEX_CFG_975_REG       (MC_BASE + 0xF7C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_976_REG       (MC_BASE + 0xF80)  /* Index pairs from CMP */
#define MC_INDEX_CFG_977_REG       (MC_BASE + 0xF84)  /* Index pairs from CMP */
#define MC_INDEX_CFG_978_REG       (MC_BASE + 0xF88)  /* Index pairs from CMP */
#define MC_INDEX_CFG_979_REG       (MC_BASE + 0xF8C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_980_REG       (MC_BASE + 0xF90)  /* Index pairs from CMP */
#define MC_INDEX_CFG_981_REG       (MC_BASE + 0xF94)  /* Index pairs from CMP */
#define MC_INDEX_CFG_982_REG       (MC_BASE + 0xF98)  /* Index pairs from CMP */
#define MC_INDEX_CFG_983_REG       (MC_BASE + 0xF9C)  /* Index pairs from CMP */
#define MC_INDEX_CFG_984_REG       (MC_BASE + 0xFA0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_985_REG       (MC_BASE + 0xFA4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_986_REG       (MC_BASE + 0xFA8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_987_REG       (MC_BASE + 0xFAC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_988_REG       (MC_BASE + 0xFB0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_989_REG       (MC_BASE + 0xFB4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_990_REG       (MC_BASE + 0xFB8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_991_REG       (MC_BASE + 0xFBC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_992_REG       (MC_BASE + 0xFC0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_993_REG       (MC_BASE + 0xFC4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_994_REG       (MC_BASE + 0xFC8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_995_REG       (MC_BASE + 0xFCC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_996_REG       (MC_BASE + 0xFD0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_997_REG       (MC_BASE + 0xFD4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_998_REG       (MC_BASE + 0xFD8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_999_REG       (MC_BASE + 0xFDC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1000_REG      (MC_BASE + 0xFE0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1001_REG      (MC_BASE + 0xFE4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1002_REG      (MC_BASE + 0xFE8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1003_REG      (MC_BASE + 0xFEC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1004_REG      (MC_BASE + 0xFF0)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1005_REG      (MC_BASE + 0xFF4)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1006_REG      (MC_BASE + 0xFF8)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1007_REG      (MC_BASE + 0xFFC)  /* Index pairs from CMP */
#define MC_INDEX_CFG_1008_REG      (MC_BASE + 0x1000) /* Index pairs from CMP */
#define MC_INDEX_CFG_1009_REG      (MC_BASE + 0x1004) /* Index pairs from CMP */
#define MC_INDEX_CFG_1010_REG      (MC_BASE + 0x1008) /* Index pairs from CMP */
#define MC_INDEX_CFG_1011_REG      (MC_BASE + 0x100C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1012_REG      (MC_BASE + 0x1010) /* Index pairs from CMP */
#define MC_INDEX_CFG_1013_REG      (MC_BASE + 0x1014) /* Index pairs from CMP */
#define MC_INDEX_CFG_1014_REG      (MC_BASE + 0x1018) /* Index pairs from CMP */
#define MC_INDEX_CFG_1015_REG      (MC_BASE + 0x101C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1016_REG      (MC_BASE + 0x1020) /* Index pairs from CMP */
#define MC_INDEX_CFG_1017_REG      (MC_BASE + 0x1024) /* Index pairs from CMP */
#define MC_INDEX_CFG_1018_REG      (MC_BASE + 0x1028) /* Index pairs from CMP */
#define MC_INDEX_CFG_1019_REG      (MC_BASE + 0x102C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1020_REG      (MC_BASE + 0x1030) /* Index pairs from CMP */
#define MC_INDEX_CFG_1021_REG      (MC_BASE + 0x1034) /* Index pairs from CMP */
#define MC_INDEX_CFG_1022_REG      (MC_BASE + 0x1038) /* Index pairs from CMP */
#define MC_INDEX_CFG_1023_REG      (MC_BASE + 0x103C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1024_REG      (MC_BASE + 0x1040) /* Index pairs from CMP */
#define MC_INDEX_CFG_1025_REG      (MC_BASE + 0x1044) /* Index pairs from CMP */
#define MC_INDEX_CFG_1026_REG      (MC_BASE + 0x1048) /* Index pairs from CMP */
#define MC_INDEX_CFG_1027_REG      (MC_BASE + 0x104C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1028_REG      (MC_BASE + 0x1050) /* Index pairs from CMP */
#define MC_INDEX_CFG_1029_REG      (MC_BASE + 0x1054) /* Index pairs from CMP */
#define MC_INDEX_CFG_1030_REG      (MC_BASE + 0x1058) /* Index pairs from CMP */
#define MC_INDEX_CFG_1031_REG      (MC_BASE + 0x105C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1032_REG      (MC_BASE + 0x1060) /* Index pairs from CMP */
#define MC_INDEX_CFG_1033_REG      (MC_BASE + 0x1064) /* Index pairs from CMP */
#define MC_INDEX_CFG_1034_REG      (MC_BASE + 0x1068) /* Index pairs from CMP */
#define MC_INDEX_CFG_1035_REG      (MC_BASE + 0x106C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1036_REG      (MC_BASE + 0x1070) /* Index pairs from CMP */
#define MC_INDEX_CFG_1037_REG      (MC_BASE + 0x1074) /* Index pairs from CMP */
#define MC_INDEX_CFG_1038_REG      (MC_BASE + 0x1078) /* Index pairs from CMP */
#define MC_INDEX_CFG_1039_REG      (MC_BASE + 0x107C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1040_REG      (MC_BASE + 0x1080) /* Index pairs from CMP */
#define MC_INDEX_CFG_1041_REG      (MC_BASE + 0x1084) /* Index pairs from CMP */
#define MC_INDEX_CFG_1042_REG      (MC_BASE + 0x1088) /* Index pairs from CMP */
#define MC_INDEX_CFG_1043_REG      (MC_BASE + 0x108C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1044_REG      (MC_BASE + 0x1090) /* Index pairs from CMP */
#define MC_INDEX_CFG_1045_REG      (MC_BASE + 0x1094) /* Index pairs from CMP */
#define MC_INDEX_CFG_1046_REG      (MC_BASE + 0x1098) /* Index pairs from CMP */
#define MC_INDEX_CFG_1047_REG      (MC_BASE + 0x109C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1048_REG      (MC_BASE + 0x10A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1049_REG      (MC_BASE + 0x10A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1050_REG      (MC_BASE + 0x10A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1051_REG      (MC_BASE + 0x10AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1052_REG      (MC_BASE + 0x10B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1053_REG      (MC_BASE + 0x10B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1054_REG      (MC_BASE + 0x10B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1055_REG      (MC_BASE + 0x10BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1056_REG      (MC_BASE + 0x10C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1057_REG      (MC_BASE + 0x10C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1058_REG      (MC_BASE + 0x10C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1059_REG      (MC_BASE + 0x10CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1060_REG      (MC_BASE + 0x10D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1061_REG      (MC_BASE + 0x10D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1062_REG      (MC_BASE + 0x10D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1063_REG      (MC_BASE + 0x10DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1064_REG      (MC_BASE + 0x10E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1065_REG      (MC_BASE + 0x10E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1066_REG      (MC_BASE + 0x10E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1067_REG      (MC_BASE + 0x10EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1068_REG      (MC_BASE + 0x10F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1069_REG      (MC_BASE + 0x10F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1070_REG      (MC_BASE + 0x10F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1071_REG      (MC_BASE + 0x10FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1072_REG      (MC_BASE + 0x1100) /* Index pairs from CMP */
#define MC_INDEX_CFG_1073_REG      (MC_BASE + 0x1104) /* Index pairs from CMP */
#define MC_INDEX_CFG_1074_REG      (MC_BASE + 0x1108) /* Index pairs from CMP */
#define MC_INDEX_CFG_1075_REG      (MC_BASE + 0x110C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1076_REG      (MC_BASE + 0x1110) /* Index pairs from CMP */
#define MC_INDEX_CFG_1077_REG      (MC_BASE + 0x1114) /* Index pairs from CMP */
#define MC_INDEX_CFG_1078_REG      (MC_BASE + 0x1118) /* Index pairs from CMP */
#define MC_INDEX_CFG_1079_REG      (MC_BASE + 0x111C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1080_REG      (MC_BASE + 0x1120) /* Index pairs from CMP */
#define MC_INDEX_CFG_1081_REG      (MC_BASE + 0x1124) /* Index pairs from CMP */
#define MC_INDEX_CFG_1082_REG      (MC_BASE + 0x1128) /* Index pairs from CMP */
#define MC_INDEX_CFG_1083_REG      (MC_BASE + 0x112C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1084_REG      (MC_BASE + 0x1130) /* Index pairs from CMP */
#define MC_INDEX_CFG_1085_REG      (MC_BASE + 0x1134) /* Index pairs from CMP */
#define MC_INDEX_CFG_1086_REG      (MC_BASE + 0x1138) /* Index pairs from CMP */
#define MC_INDEX_CFG_1087_REG      (MC_BASE + 0x113C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1088_REG      (MC_BASE + 0x1140) /* Index pairs from CMP */
#define MC_INDEX_CFG_1089_REG      (MC_BASE + 0x1144) /* Index pairs from CMP */
#define MC_INDEX_CFG_1090_REG      (MC_BASE + 0x1148) /* Index pairs from CMP */
#define MC_INDEX_CFG_1091_REG      (MC_BASE + 0x114C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1092_REG      (MC_BASE + 0x1150) /* Index pairs from CMP */
#define MC_INDEX_CFG_1093_REG      (MC_BASE + 0x1154) /* Index pairs from CMP */
#define MC_INDEX_CFG_1094_REG      (MC_BASE + 0x1158) /* Index pairs from CMP */
#define MC_INDEX_CFG_1095_REG      (MC_BASE + 0x115C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1096_REG      (MC_BASE + 0x1160) /* Index pairs from CMP */
#define MC_INDEX_CFG_1097_REG      (MC_BASE + 0x1164) /* Index pairs from CMP */
#define MC_INDEX_CFG_1098_REG      (MC_BASE + 0x1168) /* Index pairs from CMP */
#define MC_INDEX_CFG_1099_REG      (MC_BASE + 0x116C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1100_REG      (MC_BASE + 0x1170) /* Index pairs from CMP */
#define MC_INDEX_CFG_1101_REG      (MC_BASE + 0x1174) /* Index pairs from CMP */
#define MC_INDEX_CFG_1102_REG      (MC_BASE + 0x1178) /* Index pairs from CMP */
#define MC_INDEX_CFG_1103_REG      (MC_BASE + 0x117C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1104_REG      (MC_BASE + 0x1180) /* Index pairs from CMP */
#define MC_INDEX_CFG_1105_REG      (MC_BASE + 0x1184) /* Index pairs from CMP */
#define MC_INDEX_CFG_1106_REG      (MC_BASE + 0x1188) /* Index pairs from CMP */
#define MC_INDEX_CFG_1107_REG      (MC_BASE + 0x118C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1108_REG      (MC_BASE + 0x1190) /* Index pairs from CMP */
#define MC_INDEX_CFG_1109_REG      (MC_BASE + 0x1194) /* Index pairs from CMP */
#define MC_INDEX_CFG_1110_REG      (MC_BASE + 0x1198) /* Index pairs from CMP */
#define MC_INDEX_CFG_1111_REG      (MC_BASE + 0x119C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1112_REG      (MC_BASE + 0x11A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1113_REG      (MC_BASE + 0x11A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1114_REG      (MC_BASE + 0x11A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1115_REG      (MC_BASE + 0x11AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1116_REG      (MC_BASE + 0x11B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1117_REG      (MC_BASE + 0x11B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1118_REG      (MC_BASE + 0x11B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1119_REG      (MC_BASE + 0x11BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1120_REG      (MC_BASE + 0x11C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1121_REG      (MC_BASE + 0x11C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1122_REG      (MC_BASE + 0x11C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1123_REG      (MC_BASE + 0x11CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1124_REG      (MC_BASE + 0x11D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1125_REG      (MC_BASE + 0x11D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1126_REG      (MC_BASE + 0x11D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1127_REG      (MC_BASE + 0x11DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1128_REG      (MC_BASE + 0x11E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1129_REG      (MC_BASE + 0x11E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1130_REG      (MC_BASE + 0x11E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1131_REG      (MC_BASE + 0x11EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1132_REG      (MC_BASE + 0x11F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1133_REG      (MC_BASE + 0x11F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1134_REG      (MC_BASE + 0x11F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1135_REG      (MC_BASE + 0x11FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1136_REG      (MC_BASE + 0x1200) /* Index pairs from CMP */
#define MC_INDEX_CFG_1137_REG      (MC_BASE + 0x1204) /* Index pairs from CMP */
#define MC_INDEX_CFG_1138_REG      (MC_BASE + 0x1208) /* Index pairs from CMP */
#define MC_INDEX_CFG_1139_REG      (MC_BASE + 0x120C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1140_REG      (MC_BASE + 0x1210) /* Index pairs from CMP */
#define MC_INDEX_CFG_1141_REG      (MC_BASE + 0x1214) /* Index pairs from CMP */
#define MC_INDEX_CFG_1142_REG      (MC_BASE + 0x1218) /* Index pairs from CMP */
#define MC_INDEX_CFG_1143_REG      (MC_BASE + 0x121C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1144_REG      (MC_BASE + 0x1220) /* Index pairs from CMP */
#define MC_INDEX_CFG_1145_REG      (MC_BASE + 0x1224) /* Index pairs from CMP */
#define MC_INDEX_CFG_1146_REG      (MC_BASE + 0x1228) /* Index pairs from CMP */
#define MC_INDEX_CFG_1147_REG      (MC_BASE + 0x122C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1148_REG      (MC_BASE + 0x1230) /* Index pairs from CMP */
#define MC_INDEX_CFG_1149_REG      (MC_BASE + 0x1234) /* Index pairs from CMP */
#define MC_INDEX_CFG_1150_REG      (MC_BASE + 0x1238) /* Index pairs from CMP */
#define MC_INDEX_CFG_1151_REG      (MC_BASE + 0x123C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1152_REG      (MC_BASE + 0x1240) /* Index pairs from CMP */
#define MC_INDEX_CFG_1153_REG      (MC_BASE + 0x1244) /* Index pairs from CMP */
#define MC_INDEX_CFG_1154_REG      (MC_BASE + 0x1248) /* Index pairs from CMP */
#define MC_INDEX_CFG_1155_REG      (MC_BASE + 0x124C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1156_REG      (MC_BASE + 0x1250) /* Index pairs from CMP */
#define MC_INDEX_CFG_1157_REG      (MC_BASE + 0x1254) /* Index pairs from CMP */
#define MC_INDEX_CFG_1158_REG      (MC_BASE + 0x1258) /* Index pairs from CMP */
#define MC_INDEX_CFG_1159_REG      (MC_BASE + 0x125C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1160_REG      (MC_BASE + 0x1260) /* Index pairs from CMP */
#define MC_INDEX_CFG_1161_REG      (MC_BASE + 0x1264) /* Index pairs from CMP */
#define MC_INDEX_CFG_1162_REG      (MC_BASE + 0x1268) /* Index pairs from CMP */
#define MC_INDEX_CFG_1163_REG      (MC_BASE + 0x126C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1164_REG      (MC_BASE + 0x1270) /* Index pairs from CMP */
#define MC_INDEX_CFG_1165_REG      (MC_BASE + 0x1274) /* Index pairs from CMP */
#define MC_INDEX_CFG_1166_REG      (MC_BASE + 0x1278) /* Index pairs from CMP */
#define MC_INDEX_CFG_1167_REG      (MC_BASE + 0x127C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1168_REG      (MC_BASE + 0x1280) /* Index pairs from CMP */
#define MC_INDEX_CFG_1169_REG      (MC_BASE + 0x1284) /* Index pairs from CMP */
#define MC_INDEX_CFG_1170_REG      (MC_BASE + 0x1288) /* Index pairs from CMP */
#define MC_INDEX_CFG_1171_REG      (MC_BASE + 0x128C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1172_REG      (MC_BASE + 0x1290) /* Index pairs from CMP */
#define MC_INDEX_CFG_1173_REG      (MC_BASE + 0x1294) /* Index pairs from CMP */
#define MC_INDEX_CFG_1174_REG      (MC_BASE + 0x1298) /* Index pairs from CMP */
#define MC_INDEX_CFG_1175_REG      (MC_BASE + 0x129C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1176_REG      (MC_BASE + 0x12A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1177_REG      (MC_BASE + 0x12A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1178_REG      (MC_BASE + 0x12A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1179_REG      (MC_BASE + 0x12AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1180_REG      (MC_BASE + 0x12B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1181_REG      (MC_BASE + 0x12B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1182_REG      (MC_BASE + 0x12B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1183_REG      (MC_BASE + 0x12BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1184_REG      (MC_BASE + 0x12C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1185_REG      (MC_BASE + 0x12C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1186_REG      (MC_BASE + 0x12C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1187_REG      (MC_BASE + 0x12CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1188_REG      (MC_BASE + 0x12D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1189_REG      (MC_BASE + 0x12D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1190_REG      (MC_BASE + 0x12D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1191_REG      (MC_BASE + 0x12DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1192_REG      (MC_BASE + 0x12E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1193_REG      (MC_BASE + 0x12E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1194_REG      (MC_BASE + 0x12E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1195_REG      (MC_BASE + 0x12EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1196_REG      (MC_BASE + 0x12F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1197_REG      (MC_BASE + 0x12F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1198_REG      (MC_BASE + 0x12F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1199_REG      (MC_BASE + 0x12FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1200_REG      (MC_BASE + 0x1300) /* Index pairs from CMP */
#define MC_INDEX_CFG_1201_REG      (MC_BASE + 0x1304) /* Index pairs from CMP */
#define MC_INDEX_CFG_1202_REG      (MC_BASE + 0x1308) /* Index pairs from CMP */
#define MC_INDEX_CFG_1203_REG      (MC_BASE + 0x130C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1204_REG      (MC_BASE + 0x1310) /* Index pairs from CMP */
#define MC_INDEX_CFG_1205_REG      (MC_BASE + 0x1314) /* Index pairs from CMP */
#define MC_INDEX_CFG_1206_REG      (MC_BASE + 0x1318) /* Index pairs from CMP */
#define MC_INDEX_CFG_1207_REG      (MC_BASE + 0x131C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1208_REG      (MC_BASE + 0x1320) /* Index pairs from CMP */
#define MC_INDEX_CFG_1209_REG      (MC_BASE + 0x1324) /* Index pairs from CMP */
#define MC_INDEX_CFG_1210_REG      (MC_BASE + 0x1328) /* Index pairs from CMP */
#define MC_INDEX_CFG_1211_REG      (MC_BASE + 0x132C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1212_REG      (MC_BASE + 0x1330) /* Index pairs from CMP */
#define MC_INDEX_CFG_1213_REG      (MC_BASE + 0x1334) /* Index pairs from CMP */
#define MC_INDEX_CFG_1214_REG      (MC_BASE + 0x1338) /* Index pairs from CMP */
#define MC_INDEX_CFG_1215_REG      (MC_BASE + 0x133C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1216_REG      (MC_BASE + 0x1340) /* Index pairs from CMP */
#define MC_INDEX_CFG_1217_REG      (MC_BASE + 0x1344) /* Index pairs from CMP */
#define MC_INDEX_CFG_1218_REG      (MC_BASE + 0x1348) /* Index pairs from CMP */
#define MC_INDEX_CFG_1219_REG      (MC_BASE + 0x134C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1220_REG      (MC_BASE + 0x1350) /* Index pairs from CMP */
#define MC_INDEX_CFG_1221_REG      (MC_BASE + 0x1354) /* Index pairs from CMP */
#define MC_INDEX_CFG_1222_REG      (MC_BASE + 0x1358) /* Index pairs from CMP */
#define MC_INDEX_CFG_1223_REG      (MC_BASE + 0x135C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1224_REG      (MC_BASE + 0x1360) /* Index pairs from CMP */
#define MC_INDEX_CFG_1225_REG      (MC_BASE + 0x1364) /* Index pairs from CMP */
#define MC_INDEX_CFG_1226_REG      (MC_BASE + 0x1368) /* Index pairs from CMP */
#define MC_INDEX_CFG_1227_REG      (MC_BASE + 0x136C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1228_REG      (MC_BASE + 0x1370) /* Index pairs from CMP */
#define MC_INDEX_CFG_1229_REG      (MC_BASE + 0x1374) /* Index pairs from CMP */
#define MC_INDEX_CFG_1230_REG      (MC_BASE + 0x1378) /* Index pairs from CMP */
#define MC_INDEX_CFG_1231_REG      (MC_BASE + 0x137C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1232_REG      (MC_BASE + 0x1380) /* Index pairs from CMP */
#define MC_INDEX_CFG_1233_REG      (MC_BASE + 0x1384) /* Index pairs from CMP */
#define MC_INDEX_CFG_1234_REG      (MC_BASE + 0x1388) /* Index pairs from CMP */
#define MC_INDEX_CFG_1235_REG      (MC_BASE + 0x138C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1236_REG      (MC_BASE + 0x1390) /* Index pairs from CMP */
#define MC_INDEX_CFG_1237_REG      (MC_BASE + 0x1394) /* Index pairs from CMP */
#define MC_INDEX_CFG_1238_REG      (MC_BASE + 0x1398) /* Index pairs from CMP */
#define MC_INDEX_CFG_1239_REG      (MC_BASE + 0x139C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1240_REG      (MC_BASE + 0x13A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1241_REG      (MC_BASE + 0x13A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1242_REG      (MC_BASE + 0x13A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1243_REG      (MC_BASE + 0x13AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1244_REG      (MC_BASE + 0x13B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1245_REG      (MC_BASE + 0x13B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1246_REG      (MC_BASE + 0x13B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1247_REG      (MC_BASE + 0x13BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1248_REG      (MC_BASE + 0x13C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1249_REG      (MC_BASE + 0x13C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1250_REG      (MC_BASE + 0x13C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1251_REG      (MC_BASE + 0x13CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1252_REG      (MC_BASE + 0x13D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1253_REG      (MC_BASE + 0x13D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1254_REG      (MC_BASE + 0x13D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1255_REG      (MC_BASE + 0x13DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1256_REG      (MC_BASE + 0x13E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1257_REG      (MC_BASE + 0x13E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1258_REG      (MC_BASE + 0x13E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1259_REG      (MC_BASE + 0x13EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1260_REG      (MC_BASE + 0x13F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1261_REG      (MC_BASE + 0x13F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1262_REG      (MC_BASE + 0x13F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1263_REG      (MC_BASE + 0x13FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1264_REG      (MC_BASE + 0x1400) /* Index pairs from CMP */
#define MC_INDEX_CFG_1265_REG      (MC_BASE + 0x1404) /* Index pairs from CMP */
#define MC_INDEX_CFG_1266_REG      (MC_BASE + 0x1408) /* Index pairs from CMP */
#define MC_INDEX_CFG_1267_REG      (MC_BASE + 0x140C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1268_REG      (MC_BASE + 0x1410) /* Index pairs from CMP */
#define MC_INDEX_CFG_1269_REG      (MC_BASE + 0x1414) /* Index pairs from CMP */
#define MC_INDEX_CFG_1270_REG      (MC_BASE + 0x1418) /* Index pairs from CMP */
#define MC_INDEX_CFG_1271_REG      (MC_BASE + 0x141C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1272_REG      (MC_BASE + 0x1420) /* Index pairs from CMP */
#define MC_INDEX_CFG_1273_REG      (MC_BASE + 0x1424) /* Index pairs from CMP */
#define MC_INDEX_CFG_1274_REG      (MC_BASE + 0x1428) /* Index pairs from CMP */
#define MC_INDEX_CFG_1275_REG      (MC_BASE + 0x142C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1276_REG      (MC_BASE + 0x1430) /* Index pairs from CMP */
#define MC_INDEX_CFG_1277_REG      (MC_BASE + 0x1434) /* Index pairs from CMP */
#define MC_INDEX_CFG_1278_REG      (MC_BASE + 0x1438) /* Index pairs from CMP */
#define MC_INDEX_CFG_1279_REG      (MC_BASE + 0x143C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1280_REG      (MC_BASE + 0x1440) /* Index pairs from CMP */
#define MC_INDEX_CFG_1281_REG      (MC_BASE + 0x1444) /* Index pairs from CMP */
#define MC_INDEX_CFG_1282_REG      (MC_BASE + 0x1448) /* Index pairs from CMP */
#define MC_INDEX_CFG_1283_REG      (MC_BASE + 0x144C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1284_REG      (MC_BASE + 0x1450) /* Index pairs from CMP */
#define MC_INDEX_CFG_1285_REG      (MC_BASE + 0x1454) /* Index pairs from CMP */
#define MC_INDEX_CFG_1286_REG      (MC_BASE + 0x1458) /* Index pairs from CMP */
#define MC_INDEX_CFG_1287_REG      (MC_BASE + 0x145C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1288_REG      (MC_BASE + 0x1460) /* Index pairs from CMP */
#define MC_INDEX_CFG_1289_REG      (MC_BASE + 0x1464) /* Index pairs from CMP */
#define MC_INDEX_CFG_1290_REG      (MC_BASE + 0x1468) /* Index pairs from CMP */
#define MC_INDEX_CFG_1291_REG      (MC_BASE + 0x146C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1292_REG      (MC_BASE + 0x1470) /* Index pairs from CMP */
#define MC_INDEX_CFG_1293_REG      (MC_BASE + 0x1474) /* Index pairs from CMP */
#define MC_INDEX_CFG_1294_REG      (MC_BASE + 0x1478) /* Index pairs from CMP */
#define MC_INDEX_CFG_1295_REG      (MC_BASE + 0x147C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1296_REG      (MC_BASE + 0x1480) /* Index pairs from CMP */
#define MC_INDEX_CFG_1297_REG      (MC_BASE + 0x1484) /* Index pairs from CMP */
#define MC_INDEX_CFG_1298_REG      (MC_BASE + 0x1488) /* Index pairs from CMP */
#define MC_INDEX_CFG_1299_REG      (MC_BASE + 0x148C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1300_REG      (MC_BASE + 0x1490) /* Index pairs from CMP */
#define MC_INDEX_CFG_1301_REG      (MC_BASE + 0x1494) /* Index pairs from CMP */
#define MC_INDEX_CFG_1302_REG      (MC_BASE + 0x1498) /* Index pairs from CMP */
#define MC_INDEX_CFG_1303_REG      (MC_BASE + 0x149C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1304_REG      (MC_BASE + 0x14A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1305_REG      (MC_BASE + 0x14A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1306_REG      (MC_BASE + 0x14A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1307_REG      (MC_BASE + 0x14AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1308_REG      (MC_BASE + 0x14B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1309_REG      (MC_BASE + 0x14B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1310_REG      (MC_BASE + 0x14B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1311_REG      (MC_BASE + 0x14BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1312_REG      (MC_BASE + 0x14C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1313_REG      (MC_BASE + 0x14C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1314_REG      (MC_BASE + 0x14C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1315_REG      (MC_BASE + 0x14CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1316_REG      (MC_BASE + 0x14D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1317_REG      (MC_BASE + 0x14D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1318_REG      (MC_BASE + 0x14D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1319_REG      (MC_BASE + 0x14DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1320_REG      (MC_BASE + 0x14E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1321_REG      (MC_BASE + 0x14E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1322_REG      (MC_BASE + 0x14E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1323_REG      (MC_BASE + 0x14EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1324_REG      (MC_BASE + 0x14F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1325_REG      (MC_BASE + 0x14F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1326_REG      (MC_BASE + 0x14F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1327_REG      (MC_BASE + 0x14FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1328_REG      (MC_BASE + 0x1500) /* Index pairs from CMP */
#define MC_INDEX_CFG_1329_REG      (MC_BASE + 0x1504) /* Index pairs from CMP */
#define MC_INDEX_CFG_1330_REG      (MC_BASE + 0x1508) /* Index pairs from CMP */
#define MC_INDEX_CFG_1331_REG      (MC_BASE + 0x150C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1332_REG      (MC_BASE + 0x1510) /* Index pairs from CMP */
#define MC_INDEX_CFG_1333_REG      (MC_BASE + 0x1514) /* Index pairs from CMP */
#define MC_INDEX_CFG_1334_REG      (MC_BASE + 0x1518) /* Index pairs from CMP */
#define MC_INDEX_CFG_1335_REG      (MC_BASE + 0x151C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1336_REG      (MC_BASE + 0x1520) /* Index pairs from CMP */
#define MC_INDEX_CFG_1337_REG      (MC_BASE + 0x1524) /* Index pairs from CMP */
#define MC_INDEX_CFG_1338_REG      (MC_BASE + 0x1528) /* Index pairs from CMP */
#define MC_INDEX_CFG_1339_REG      (MC_BASE + 0x152C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1340_REG      (MC_BASE + 0x1530) /* Index pairs from CMP */
#define MC_INDEX_CFG_1341_REG      (MC_BASE + 0x1534) /* Index pairs from CMP */
#define MC_INDEX_CFG_1342_REG      (MC_BASE + 0x1538) /* Index pairs from CMP */
#define MC_INDEX_CFG_1343_REG      (MC_BASE + 0x153C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1344_REG      (MC_BASE + 0x1540) /* Index pairs from CMP */
#define MC_INDEX_CFG_1345_REG      (MC_BASE + 0x1544) /* Index pairs from CMP */
#define MC_INDEX_CFG_1346_REG      (MC_BASE + 0x1548) /* Index pairs from CMP */
#define MC_INDEX_CFG_1347_REG      (MC_BASE + 0x154C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1348_REG      (MC_BASE + 0x1550) /* Index pairs from CMP */
#define MC_INDEX_CFG_1349_REG      (MC_BASE + 0x1554) /* Index pairs from CMP */
#define MC_INDEX_CFG_1350_REG      (MC_BASE + 0x1558) /* Index pairs from CMP */
#define MC_INDEX_CFG_1351_REG      (MC_BASE + 0x155C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1352_REG      (MC_BASE + 0x1560) /* Index pairs from CMP */
#define MC_INDEX_CFG_1353_REG      (MC_BASE + 0x1564) /* Index pairs from CMP */
#define MC_INDEX_CFG_1354_REG      (MC_BASE + 0x1568) /* Index pairs from CMP */
#define MC_INDEX_CFG_1355_REG      (MC_BASE + 0x156C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1356_REG      (MC_BASE + 0x1570) /* Index pairs from CMP */
#define MC_INDEX_CFG_1357_REG      (MC_BASE + 0x1574) /* Index pairs from CMP */
#define MC_INDEX_CFG_1358_REG      (MC_BASE + 0x1578) /* Index pairs from CMP */
#define MC_INDEX_CFG_1359_REG      (MC_BASE + 0x157C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1360_REG      (MC_BASE + 0x1580) /* Index pairs from CMP */
#define MC_INDEX_CFG_1361_REG      (MC_BASE + 0x1584) /* Index pairs from CMP */
#define MC_INDEX_CFG_1362_REG      (MC_BASE + 0x1588) /* Index pairs from CMP */
#define MC_INDEX_CFG_1363_REG      (MC_BASE + 0x158C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1364_REG      (MC_BASE + 0x1590) /* Index pairs from CMP */
#define MC_INDEX_CFG_1365_REG      (MC_BASE + 0x1594) /* Index pairs from CMP */
#define MC_INDEX_CFG_1366_REG      (MC_BASE + 0x1598) /* Index pairs from CMP */
#define MC_INDEX_CFG_1367_REG      (MC_BASE + 0x159C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1368_REG      (MC_BASE + 0x15A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1369_REG      (MC_BASE + 0x15A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1370_REG      (MC_BASE + 0x15A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1371_REG      (MC_BASE + 0x15AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1372_REG      (MC_BASE + 0x15B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1373_REG      (MC_BASE + 0x15B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1374_REG      (MC_BASE + 0x15B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1375_REG      (MC_BASE + 0x15BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1376_REG      (MC_BASE + 0x15C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1377_REG      (MC_BASE + 0x15C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1378_REG      (MC_BASE + 0x15C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1379_REG      (MC_BASE + 0x15CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1380_REG      (MC_BASE + 0x15D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1381_REG      (MC_BASE + 0x15D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1382_REG      (MC_BASE + 0x15D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1383_REG      (MC_BASE + 0x15DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1384_REG      (MC_BASE + 0x15E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1385_REG      (MC_BASE + 0x15E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1386_REG      (MC_BASE + 0x15E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1387_REG      (MC_BASE + 0x15EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1388_REG      (MC_BASE + 0x15F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1389_REG      (MC_BASE + 0x15F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1390_REG      (MC_BASE + 0x15F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1391_REG      (MC_BASE + 0x15FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1392_REG      (MC_BASE + 0x1600) /* Index pairs from CMP */
#define MC_INDEX_CFG_1393_REG      (MC_BASE + 0x1604) /* Index pairs from CMP */
#define MC_INDEX_CFG_1394_REG      (MC_BASE + 0x1608) /* Index pairs from CMP */
#define MC_INDEX_CFG_1395_REG      (MC_BASE + 0x160C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1396_REG      (MC_BASE + 0x1610) /* Index pairs from CMP */
#define MC_INDEX_CFG_1397_REG      (MC_BASE + 0x1614) /* Index pairs from CMP */
#define MC_INDEX_CFG_1398_REG      (MC_BASE + 0x1618) /* Index pairs from CMP */
#define MC_INDEX_CFG_1399_REG      (MC_BASE + 0x161C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1400_REG      (MC_BASE + 0x1620) /* Index pairs from CMP */
#define MC_INDEX_CFG_1401_REG      (MC_BASE + 0x1624) /* Index pairs from CMP */
#define MC_INDEX_CFG_1402_REG      (MC_BASE + 0x1628) /* Index pairs from CMP */
#define MC_INDEX_CFG_1403_REG      (MC_BASE + 0x162C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1404_REG      (MC_BASE + 0x1630) /* Index pairs from CMP */
#define MC_INDEX_CFG_1405_REG      (MC_BASE + 0x1634) /* Index pairs from CMP */
#define MC_INDEX_CFG_1406_REG      (MC_BASE + 0x1638) /* Index pairs from CMP */
#define MC_INDEX_CFG_1407_REG      (MC_BASE + 0x163C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1408_REG      (MC_BASE + 0x1640) /* Index pairs from CMP */
#define MC_INDEX_CFG_1409_REG      (MC_BASE + 0x1644) /* Index pairs from CMP */
#define MC_INDEX_CFG_1410_REG      (MC_BASE + 0x1648) /* Index pairs from CMP */
#define MC_INDEX_CFG_1411_REG      (MC_BASE + 0x164C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1412_REG      (MC_BASE + 0x1650) /* Index pairs from CMP */
#define MC_INDEX_CFG_1413_REG      (MC_BASE + 0x1654) /* Index pairs from CMP */
#define MC_INDEX_CFG_1414_REG      (MC_BASE + 0x1658) /* Index pairs from CMP */
#define MC_INDEX_CFG_1415_REG      (MC_BASE + 0x165C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1416_REG      (MC_BASE + 0x1660) /* Index pairs from CMP */
#define MC_INDEX_CFG_1417_REG      (MC_BASE + 0x1664) /* Index pairs from CMP */
#define MC_INDEX_CFG_1418_REG      (MC_BASE + 0x1668) /* Index pairs from CMP */
#define MC_INDEX_CFG_1419_REG      (MC_BASE + 0x166C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1420_REG      (MC_BASE + 0x1670) /* Index pairs from CMP */
#define MC_INDEX_CFG_1421_REG      (MC_BASE + 0x1674) /* Index pairs from CMP */
#define MC_INDEX_CFG_1422_REG      (MC_BASE + 0x1678) /* Index pairs from CMP */
#define MC_INDEX_CFG_1423_REG      (MC_BASE + 0x167C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1424_REG      (MC_BASE + 0x1680) /* Index pairs from CMP */
#define MC_INDEX_CFG_1425_REG      (MC_BASE + 0x1684) /* Index pairs from CMP */
#define MC_INDEX_CFG_1426_REG      (MC_BASE + 0x1688) /* Index pairs from CMP */
#define MC_INDEX_CFG_1427_REG      (MC_BASE + 0x168C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1428_REG      (MC_BASE + 0x1690) /* Index pairs from CMP */
#define MC_INDEX_CFG_1429_REG      (MC_BASE + 0x1694) /* Index pairs from CMP */
#define MC_INDEX_CFG_1430_REG      (MC_BASE + 0x1698) /* Index pairs from CMP */
#define MC_INDEX_CFG_1431_REG      (MC_BASE + 0x169C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1432_REG      (MC_BASE + 0x16A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1433_REG      (MC_BASE + 0x16A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1434_REG      (MC_BASE + 0x16A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1435_REG      (MC_BASE + 0x16AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1436_REG      (MC_BASE + 0x16B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1437_REG      (MC_BASE + 0x16B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1438_REG      (MC_BASE + 0x16B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1439_REG      (MC_BASE + 0x16BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1440_REG      (MC_BASE + 0x16C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1441_REG      (MC_BASE + 0x16C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1442_REG      (MC_BASE + 0x16C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1443_REG      (MC_BASE + 0x16CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1444_REG      (MC_BASE + 0x16D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1445_REG      (MC_BASE + 0x16D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1446_REG      (MC_BASE + 0x16D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1447_REG      (MC_BASE + 0x16DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1448_REG      (MC_BASE + 0x16E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1449_REG      (MC_BASE + 0x16E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1450_REG      (MC_BASE + 0x16E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1451_REG      (MC_BASE + 0x16EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1452_REG      (MC_BASE + 0x16F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1453_REG      (MC_BASE + 0x16F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1454_REG      (MC_BASE + 0x16F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1455_REG      (MC_BASE + 0x16FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1456_REG      (MC_BASE + 0x1700) /* Index pairs from CMP */
#define MC_INDEX_CFG_1457_REG      (MC_BASE + 0x1704) /* Index pairs from CMP */
#define MC_INDEX_CFG_1458_REG      (MC_BASE + 0x1708) /* Index pairs from CMP */
#define MC_INDEX_CFG_1459_REG      (MC_BASE + 0x170C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1460_REG      (MC_BASE + 0x1710) /* Index pairs from CMP */
#define MC_INDEX_CFG_1461_REG      (MC_BASE + 0x1714) /* Index pairs from CMP */
#define MC_INDEX_CFG_1462_REG      (MC_BASE + 0x1718) /* Index pairs from CMP */
#define MC_INDEX_CFG_1463_REG      (MC_BASE + 0x171C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1464_REG      (MC_BASE + 0x1720) /* Index pairs from CMP */
#define MC_INDEX_CFG_1465_REG      (MC_BASE + 0x1724) /* Index pairs from CMP */
#define MC_INDEX_CFG_1466_REG      (MC_BASE + 0x1728) /* Index pairs from CMP */
#define MC_INDEX_CFG_1467_REG      (MC_BASE + 0x172C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1468_REG      (MC_BASE + 0x1730) /* Index pairs from CMP */
#define MC_INDEX_CFG_1469_REG      (MC_BASE + 0x1734) /* Index pairs from CMP */
#define MC_INDEX_CFG_1470_REG      (MC_BASE + 0x1738) /* Index pairs from CMP */
#define MC_INDEX_CFG_1471_REG      (MC_BASE + 0x173C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1472_REG      (MC_BASE + 0x1740) /* Index pairs from CMP */
#define MC_INDEX_CFG_1473_REG      (MC_BASE + 0x1744) /* Index pairs from CMP */
#define MC_INDEX_CFG_1474_REG      (MC_BASE + 0x1748) /* Index pairs from CMP */
#define MC_INDEX_CFG_1475_REG      (MC_BASE + 0x174C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1476_REG      (MC_BASE + 0x1750) /* Index pairs from CMP */
#define MC_INDEX_CFG_1477_REG      (MC_BASE + 0x1754) /* Index pairs from CMP */
#define MC_INDEX_CFG_1478_REG      (MC_BASE + 0x1758) /* Index pairs from CMP */
#define MC_INDEX_CFG_1479_REG      (MC_BASE + 0x175C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1480_REG      (MC_BASE + 0x1760) /* Index pairs from CMP */
#define MC_INDEX_CFG_1481_REG      (MC_BASE + 0x1764) /* Index pairs from CMP */
#define MC_INDEX_CFG_1482_REG      (MC_BASE + 0x1768) /* Index pairs from CMP */
#define MC_INDEX_CFG_1483_REG      (MC_BASE + 0x176C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1484_REG      (MC_BASE + 0x1770) /* Index pairs from CMP */
#define MC_INDEX_CFG_1485_REG      (MC_BASE + 0x1774) /* Index pairs from CMP */
#define MC_INDEX_CFG_1486_REG      (MC_BASE + 0x1778) /* Index pairs from CMP */
#define MC_INDEX_CFG_1487_REG      (MC_BASE + 0x177C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1488_REG      (MC_BASE + 0x1780) /* Index pairs from CMP */
#define MC_INDEX_CFG_1489_REG      (MC_BASE + 0x1784) /* Index pairs from CMP */
#define MC_INDEX_CFG_1490_REG      (MC_BASE + 0x1788) /* Index pairs from CMP */
#define MC_INDEX_CFG_1491_REG      (MC_BASE + 0x178C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1492_REG      (MC_BASE + 0x1790) /* Index pairs from CMP */
#define MC_INDEX_CFG_1493_REG      (MC_BASE + 0x1794) /* Index pairs from CMP */
#define MC_INDEX_CFG_1494_REG      (MC_BASE + 0x1798) /* Index pairs from CMP */
#define MC_INDEX_CFG_1495_REG      (MC_BASE + 0x179C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1496_REG      (MC_BASE + 0x17A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1497_REG      (MC_BASE + 0x17A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1498_REG      (MC_BASE + 0x17A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1499_REG      (MC_BASE + 0x17AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1500_REG      (MC_BASE + 0x17B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1501_REG      (MC_BASE + 0x17B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1502_REG      (MC_BASE + 0x17B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1503_REG      (MC_BASE + 0x17BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1504_REG      (MC_BASE + 0x17C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1505_REG      (MC_BASE + 0x17C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1506_REG      (MC_BASE + 0x17C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1507_REG      (MC_BASE + 0x17CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1508_REG      (MC_BASE + 0x17D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1509_REG      (MC_BASE + 0x17D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1510_REG      (MC_BASE + 0x17D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1511_REG      (MC_BASE + 0x17DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1512_REG      (MC_BASE + 0x17E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1513_REG      (MC_BASE + 0x17E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1514_REG      (MC_BASE + 0x17E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1515_REG      (MC_BASE + 0x17EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1516_REG      (MC_BASE + 0x17F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1517_REG      (MC_BASE + 0x17F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1518_REG      (MC_BASE + 0x17F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1519_REG      (MC_BASE + 0x17FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1520_REG      (MC_BASE + 0x1800) /* Index pairs from CMP */
#define MC_INDEX_CFG_1521_REG      (MC_BASE + 0x1804) /* Index pairs from CMP */
#define MC_INDEX_CFG_1522_REG      (MC_BASE + 0x1808) /* Index pairs from CMP */
#define MC_INDEX_CFG_1523_REG      (MC_BASE + 0x180C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1524_REG      (MC_BASE + 0x1810) /* Index pairs from CMP */
#define MC_INDEX_CFG_1525_REG      (MC_BASE + 0x1814) /* Index pairs from CMP */
#define MC_INDEX_CFG_1526_REG      (MC_BASE + 0x1818) /* Index pairs from CMP */
#define MC_INDEX_CFG_1527_REG      (MC_BASE + 0x181C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1528_REG      (MC_BASE + 0x1820) /* Index pairs from CMP */
#define MC_INDEX_CFG_1529_REG      (MC_BASE + 0x1824) /* Index pairs from CMP */
#define MC_INDEX_CFG_1530_REG      (MC_BASE + 0x1828) /* Index pairs from CMP */
#define MC_INDEX_CFG_1531_REG      (MC_BASE + 0x182C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1532_REG      (MC_BASE + 0x1830) /* Index pairs from CMP */
#define MC_INDEX_CFG_1533_REG      (MC_BASE + 0x1834) /* Index pairs from CMP */
#define MC_INDEX_CFG_1534_REG      (MC_BASE + 0x1838) /* Index pairs from CMP */
#define MC_INDEX_CFG_1535_REG      (MC_BASE + 0x183C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1536_REG      (MC_BASE + 0x1840) /* Index pairs from CMP */
#define MC_INDEX_CFG_1537_REG      (MC_BASE + 0x1844) /* Index pairs from CMP */
#define MC_INDEX_CFG_1538_REG      (MC_BASE + 0x1848) /* Index pairs from CMP */
#define MC_INDEX_CFG_1539_REG      (MC_BASE + 0x184C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1540_REG      (MC_BASE + 0x1850) /* Index pairs from CMP */
#define MC_INDEX_CFG_1541_REG      (MC_BASE + 0x1854) /* Index pairs from CMP */
#define MC_INDEX_CFG_1542_REG      (MC_BASE + 0x1858) /* Index pairs from CMP */
#define MC_INDEX_CFG_1543_REG      (MC_BASE + 0x185C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1544_REG      (MC_BASE + 0x1860) /* Index pairs from CMP */
#define MC_INDEX_CFG_1545_REG      (MC_BASE + 0x1864) /* Index pairs from CMP */
#define MC_INDEX_CFG_1546_REG      (MC_BASE + 0x1868) /* Index pairs from CMP */
#define MC_INDEX_CFG_1547_REG      (MC_BASE + 0x186C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1548_REG      (MC_BASE + 0x1870) /* Index pairs from CMP */
#define MC_INDEX_CFG_1549_REG      (MC_BASE + 0x1874) /* Index pairs from CMP */
#define MC_INDEX_CFG_1550_REG      (MC_BASE + 0x1878) /* Index pairs from CMP */
#define MC_INDEX_CFG_1551_REG      (MC_BASE + 0x187C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1552_REG      (MC_BASE + 0x1880) /* Index pairs from CMP */
#define MC_INDEX_CFG_1553_REG      (MC_BASE + 0x1884) /* Index pairs from CMP */
#define MC_INDEX_CFG_1554_REG      (MC_BASE + 0x1888) /* Index pairs from CMP */
#define MC_INDEX_CFG_1555_REG      (MC_BASE + 0x188C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1556_REG      (MC_BASE + 0x1890) /* Index pairs from CMP */
#define MC_INDEX_CFG_1557_REG      (MC_BASE + 0x1894) /* Index pairs from CMP */
#define MC_INDEX_CFG_1558_REG      (MC_BASE + 0x1898) /* Index pairs from CMP */
#define MC_INDEX_CFG_1559_REG      (MC_BASE + 0x189C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1560_REG      (MC_BASE + 0x18A0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1561_REG      (MC_BASE + 0x18A4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1562_REG      (MC_BASE + 0x18A8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1563_REG      (MC_BASE + 0x18AC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1564_REG      (MC_BASE + 0x18B0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1565_REG      (MC_BASE + 0x18B4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1566_REG      (MC_BASE + 0x18B8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1567_REG      (MC_BASE + 0x18BC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1568_REG      (MC_BASE + 0x18C0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1569_REG      (MC_BASE + 0x18C4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1570_REG      (MC_BASE + 0x18C8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1571_REG      (MC_BASE + 0x18CC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1572_REG      (MC_BASE + 0x18D0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1573_REG      (MC_BASE + 0x18D4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1574_REG      (MC_BASE + 0x18D8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1575_REG      (MC_BASE + 0x18DC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1576_REG      (MC_BASE + 0x18E0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1577_REG      (MC_BASE + 0x18E4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1578_REG      (MC_BASE + 0x18E8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1579_REG      (MC_BASE + 0x18EC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1580_REG      (MC_BASE + 0x18F0) /* Index pairs from CMP */
#define MC_INDEX_CFG_1581_REG      (MC_BASE + 0x18F4) /* Index pairs from CMP */
#define MC_INDEX_CFG_1582_REG      (MC_BASE + 0x18F8) /* Index pairs from CMP */
#define MC_INDEX_CFG_1583_REG      (MC_BASE + 0x18FC) /* Index pairs from CMP */
#define MC_INDEX_CFG_1584_REG      (MC_BASE + 0x1900) /* Index pairs from CMP */
#define MC_INDEX_CFG_1585_REG      (MC_BASE + 0x1904) /* Index pairs from CMP */
#define MC_INDEX_CFG_1586_REG      (MC_BASE + 0x1908) /* Index pairs from CMP */
#define MC_INDEX_CFG_1587_REG      (MC_BASE + 0x190C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1588_REG      (MC_BASE + 0x1910) /* Index pairs from CMP */
#define MC_INDEX_CFG_1589_REG      (MC_BASE + 0x1914) /* Index pairs from CMP */
#define MC_INDEX_CFG_1590_REG      (MC_BASE + 0x1918) /* Index pairs from CMP */
#define MC_INDEX_CFG_1591_REG      (MC_BASE + 0x191C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1592_REG      (MC_BASE + 0x1920) /* Index pairs from CMP */
#define MC_INDEX_CFG_1593_REG      (MC_BASE + 0x1924) /* Index pairs from CMP */
#define MC_INDEX_CFG_1594_REG      (MC_BASE + 0x1928) /* Index pairs from CMP */
#define MC_INDEX_CFG_1595_REG      (MC_BASE + 0x192C) /* Index pairs from CMP */
#define MC_INDEX_CFG_1596_REG      (MC_BASE + 0x1930) /* Index pairs from CMP */
#define MC_INDEX_CFG_1597_REG      (MC_BASE + 0x1934) /* Index pairs from CMP */
#define MC_INDEX_CFG_1598_REG      (MC_BASE + 0x1938) /* Index pairs from CMP */
#define MC_INDEX_CFG_1599_REG      (MC_BASE + 0x193C) /* Index pairs from CMP */
#define MC_COORDINATE_CFG_0_REG    (MC_BASE + 0x1A00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1_REG    (MC_BASE + 0x1A04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2_REG    (MC_BASE + 0x1A08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3_REG    (MC_BASE + 0x1A0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_4_REG    (MC_BASE + 0x1A10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_5_REG    (MC_BASE + 0x1A14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_6_REG    (MC_BASE + 0x1A18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_7_REG    (MC_BASE + 0x1A1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_8_REG    (MC_BASE + 0x1A20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_9_REG    (MC_BASE + 0x1A24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_10_REG   (MC_BASE + 0x1A28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_11_REG   (MC_BASE + 0x1A2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_12_REG   (MC_BASE + 0x1A30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_13_REG   (MC_BASE + 0x1A34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_14_REG   (MC_BASE + 0x1A38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_15_REG   (MC_BASE + 0x1A3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_16_REG   (MC_BASE + 0x1A40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_17_REG   (MC_BASE + 0x1A44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_18_REG   (MC_BASE + 0x1A48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_19_REG   (MC_BASE + 0x1A4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_20_REG   (MC_BASE + 0x1A50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_21_REG   (MC_BASE + 0x1A54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_22_REG   (MC_BASE + 0x1A58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_23_REG   (MC_BASE + 0x1A5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_24_REG   (MC_BASE + 0x1A60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_25_REG   (MC_BASE + 0x1A64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_26_REG   (MC_BASE + 0x1A68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_27_REG   (MC_BASE + 0x1A6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_28_REG   (MC_BASE + 0x1A70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_29_REG   (MC_BASE + 0x1A74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_30_REG   (MC_BASE + 0x1A78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_31_REG   (MC_BASE + 0x1A7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_32_REG   (MC_BASE + 0x1A80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_33_REG   (MC_BASE + 0x1A84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_34_REG   (MC_BASE + 0x1A88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_35_REG   (MC_BASE + 0x1A8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_36_REG   (MC_BASE + 0x1A90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_37_REG   (MC_BASE + 0x1A94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_38_REG   (MC_BASE + 0x1A98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_39_REG   (MC_BASE + 0x1A9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_40_REG   (MC_BASE + 0x1AA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_41_REG   (MC_BASE + 0x1AA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_42_REG   (MC_BASE + 0x1AA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_43_REG   (MC_BASE + 0x1AAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_44_REG   (MC_BASE + 0x1AB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_45_REG   (MC_BASE + 0x1AB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_46_REG   (MC_BASE + 0x1AB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_47_REG   (MC_BASE + 0x1ABC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_48_REG   (MC_BASE + 0x1AC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_49_REG   (MC_BASE + 0x1AC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_50_REG   (MC_BASE + 0x1AC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_51_REG   (MC_BASE + 0x1ACC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_52_REG   (MC_BASE + 0x1AD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_53_REG   (MC_BASE + 0x1AD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_54_REG   (MC_BASE + 0x1AD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_55_REG   (MC_BASE + 0x1ADC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_56_REG   (MC_BASE + 0x1AE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_57_REG   (MC_BASE + 0x1AE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_58_REG   (MC_BASE + 0x1AE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_59_REG   (MC_BASE + 0x1AEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_60_REG   (MC_BASE + 0x1AF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_61_REG   (MC_BASE + 0x1AF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_62_REG   (MC_BASE + 0x1AF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_63_REG   (MC_BASE + 0x1AFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_64_REG   (MC_BASE + 0x1B00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_65_REG   (MC_BASE + 0x1B04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_66_REG   (MC_BASE + 0x1B08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_67_REG   (MC_BASE + 0x1B0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_68_REG   (MC_BASE + 0x1B10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_69_REG   (MC_BASE + 0x1B14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_70_REG   (MC_BASE + 0x1B18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_71_REG   (MC_BASE + 0x1B1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_72_REG   (MC_BASE + 0x1B20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_73_REG   (MC_BASE + 0x1B24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_74_REG   (MC_BASE + 0x1B28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_75_REG   (MC_BASE + 0x1B2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_76_REG   (MC_BASE + 0x1B30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_77_REG   (MC_BASE + 0x1B34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_78_REG   (MC_BASE + 0x1B38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_79_REG   (MC_BASE + 0x1B3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_80_REG   (MC_BASE + 0x1B40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_81_REG   (MC_BASE + 0x1B44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_82_REG   (MC_BASE + 0x1B48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_83_REG   (MC_BASE + 0x1B4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_84_REG   (MC_BASE + 0x1B50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_85_REG   (MC_BASE + 0x1B54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_86_REG   (MC_BASE + 0x1B58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_87_REG   (MC_BASE + 0x1B5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_88_REG   (MC_BASE + 0x1B60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_89_REG   (MC_BASE + 0x1B64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_90_REG   (MC_BASE + 0x1B68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_91_REG   (MC_BASE + 0x1B6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_92_REG   (MC_BASE + 0x1B70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_93_REG   (MC_BASE + 0x1B74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_94_REG   (MC_BASE + 0x1B78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_95_REG   (MC_BASE + 0x1B7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_96_REG   (MC_BASE + 0x1B80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_97_REG   (MC_BASE + 0x1B84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_98_REG   (MC_BASE + 0x1B88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_99_REG   (MC_BASE + 0x1B8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_100_REG  (MC_BASE + 0x1B90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_101_REG  (MC_BASE + 0x1B94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_102_REG  (MC_BASE + 0x1B98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_103_REG  (MC_BASE + 0x1B9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_104_REG  (MC_BASE + 0x1BA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_105_REG  (MC_BASE + 0x1BA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_106_REG  (MC_BASE + 0x1BA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_107_REG  (MC_BASE + 0x1BAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_108_REG  (MC_BASE + 0x1BB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_109_REG  (MC_BASE + 0x1BB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_110_REG  (MC_BASE + 0x1BB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_111_REG  (MC_BASE + 0x1BBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_112_REG  (MC_BASE + 0x1BC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_113_REG  (MC_BASE + 0x1BC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_114_REG  (MC_BASE + 0x1BC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_115_REG  (MC_BASE + 0x1BCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_116_REG  (MC_BASE + 0x1BD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_117_REG  (MC_BASE + 0x1BD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_118_REG  (MC_BASE + 0x1BD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_119_REG  (MC_BASE + 0x1BDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_120_REG  (MC_BASE + 0x1BE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_121_REG  (MC_BASE + 0x1BE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_122_REG  (MC_BASE + 0x1BE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_123_REG  (MC_BASE + 0x1BEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_124_REG  (MC_BASE + 0x1BF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_125_REG  (MC_BASE + 0x1BF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_126_REG  (MC_BASE + 0x1BF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_127_REG  (MC_BASE + 0x1BFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_128_REG  (MC_BASE + 0x1C00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_129_REG  (MC_BASE + 0x1C04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_130_REG  (MC_BASE + 0x1C08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_131_REG  (MC_BASE + 0x1C0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_132_REG  (MC_BASE + 0x1C10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_133_REG  (MC_BASE + 0x1C14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_134_REG  (MC_BASE + 0x1C18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_135_REG  (MC_BASE + 0x1C1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_136_REG  (MC_BASE + 0x1C20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_137_REG  (MC_BASE + 0x1C24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_138_REG  (MC_BASE + 0x1C28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_139_REG  (MC_BASE + 0x1C2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_140_REG  (MC_BASE + 0x1C30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_141_REG  (MC_BASE + 0x1C34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_142_REG  (MC_BASE + 0x1C38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_143_REG  (MC_BASE + 0x1C3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_144_REG  (MC_BASE + 0x1C40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_145_REG  (MC_BASE + 0x1C44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_146_REG  (MC_BASE + 0x1C48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_147_REG  (MC_BASE + 0x1C4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_148_REG  (MC_BASE + 0x1C50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_149_REG  (MC_BASE + 0x1C54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_150_REG  (MC_BASE + 0x1C58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_151_REG  (MC_BASE + 0x1C5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_152_REG  (MC_BASE + 0x1C60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_153_REG  (MC_BASE + 0x1C64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_154_REG  (MC_BASE + 0x1C68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_155_REG  (MC_BASE + 0x1C6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_156_REG  (MC_BASE + 0x1C70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_157_REG  (MC_BASE + 0x1C74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_158_REG  (MC_BASE + 0x1C78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_159_REG  (MC_BASE + 0x1C7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_160_REG  (MC_BASE + 0x1C80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_161_REG  (MC_BASE + 0x1C84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_162_REG  (MC_BASE + 0x1C88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_163_REG  (MC_BASE + 0x1C8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_164_REG  (MC_BASE + 0x1C90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_165_REG  (MC_BASE + 0x1C94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_166_REG  (MC_BASE + 0x1C98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_167_REG  (MC_BASE + 0x1C9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_168_REG  (MC_BASE + 0x1CA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_169_REG  (MC_BASE + 0x1CA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_170_REG  (MC_BASE + 0x1CA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_171_REG  (MC_BASE + 0x1CAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_172_REG  (MC_BASE + 0x1CB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_173_REG  (MC_BASE + 0x1CB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_174_REG  (MC_BASE + 0x1CB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_175_REG  (MC_BASE + 0x1CBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_176_REG  (MC_BASE + 0x1CC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_177_REG  (MC_BASE + 0x1CC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_178_REG  (MC_BASE + 0x1CC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_179_REG  (MC_BASE + 0x1CCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_180_REG  (MC_BASE + 0x1CD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_181_REG  (MC_BASE + 0x1CD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_182_REG  (MC_BASE + 0x1CD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_183_REG  (MC_BASE + 0x1CDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_184_REG  (MC_BASE + 0x1CE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_185_REG  (MC_BASE + 0x1CE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_186_REG  (MC_BASE + 0x1CE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_187_REG  (MC_BASE + 0x1CEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_188_REG  (MC_BASE + 0x1CF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_189_REG  (MC_BASE + 0x1CF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_190_REG  (MC_BASE + 0x1CF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_191_REG  (MC_BASE + 0x1CFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_192_REG  (MC_BASE + 0x1D00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_193_REG  (MC_BASE + 0x1D04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_194_REG  (MC_BASE + 0x1D08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_195_REG  (MC_BASE + 0x1D0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_196_REG  (MC_BASE + 0x1D10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_197_REG  (MC_BASE + 0x1D14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_198_REG  (MC_BASE + 0x1D18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_199_REG  (MC_BASE + 0x1D1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_200_REG  (MC_BASE + 0x1D20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_201_REG  (MC_BASE + 0x1D24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_202_REG  (MC_BASE + 0x1D28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_203_REG  (MC_BASE + 0x1D2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_204_REG  (MC_BASE + 0x1D30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_205_REG  (MC_BASE + 0x1D34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_206_REG  (MC_BASE + 0x1D38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_207_REG  (MC_BASE + 0x1D3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_208_REG  (MC_BASE + 0x1D40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_209_REG  (MC_BASE + 0x1D44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_210_REG  (MC_BASE + 0x1D48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_211_REG  (MC_BASE + 0x1D4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_212_REG  (MC_BASE + 0x1D50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_213_REG  (MC_BASE + 0x1D54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_214_REG  (MC_BASE + 0x1D58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_215_REG  (MC_BASE + 0x1D5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_216_REG  (MC_BASE + 0x1D60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_217_REG  (MC_BASE + 0x1D64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_218_REG  (MC_BASE + 0x1D68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_219_REG  (MC_BASE + 0x1D6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_220_REG  (MC_BASE + 0x1D70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_221_REG  (MC_BASE + 0x1D74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_222_REG  (MC_BASE + 0x1D78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_223_REG  (MC_BASE + 0x1D7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_224_REG  (MC_BASE + 0x1D80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_225_REG  (MC_BASE + 0x1D84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_226_REG  (MC_BASE + 0x1D88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_227_REG  (MC_BASE + 0x1D8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_228_REG  (MC_BASE + 0x1D90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_229_REG  (MC_BASE + 0x1D94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_230_REG  (MC_BASE + 0x1D98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_231_REG  (MC_BASE + 0x1D9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_232_REG  (MC_BASE + 0x1DA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_233_REG  (MC_BASE + 0x1DA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_234_REG  (MC_BASE + 0x1DA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_235_REG  (MC_BASE + 0x1DAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_236_REG  (MC_BASE + 0x1DB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_237_REG  (MC_BASE + 0x1DB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_238_REG  (MC_BASE + 0x1DB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_239_REG  (MC_BASE + 0x1DBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_240_REG  (MC_BASE + 0x1DC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_241_REG  (MC_BASE + 0x1DC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_242_REG  (MC_BASE + 0x1DC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_243_REG  (MC_BASE + 0x1DCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_244_REG  (MC_BASE + 0x1DD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_245_REG  (MC_BASE + 0x1DD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_246_REG  (MC_BASE + 0x1DD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_247_REG  (MC_BASE + 0x1DDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_248_REG  (MC_BASE + 0x1DE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_249_REG  (MC_BASE + 0x1DE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_250_REG  (MC_BASE + 0x1DE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_251_REG  (MC_BASE + 0x1DEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_252_REG  (MC_BASE + 0x1DF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_253_REG  (MC_BASE + 0x1DF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_254_REG  (MC_BASE + 0x1DF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_255_REG  (MC_BASE + 0x1DFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_256_REG  (MC_BASE + 0x1E00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_257_REG  (MC_BASE + 0x1E04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_258_REG  (MC_BASE + 0x1E08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_259_REG  (MC_BASE + 0x1E0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_260_REG  (MC_BASE + 0x1E10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_261_REG  (MC_BASE + 0x1E14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_262_REG  (MC_BASE + 0x1E18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_263_REG  (MC_BASE + 0x1E1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_264_REG  (MC_BASE + 0x1E20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_265_REG  (MC_BASE + 0x1E24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_266_REG  (MC_BASE + 0x1E28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_267_REG  (MC_BASE + 0x1E2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_268_REG  (MC_BASE + 0x1E30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_269_REG  (MC_BASE + 0x1E34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_270_REG  (MC_BASE + 0x1E38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_271_REG  (MC_BASE + 0x1E3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_272_REG  (MC_BASE + 0x1E40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_273_REG  (MC_BASE + 0x1E44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_274_REG  (MC_BASE + 0x1E48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_275_REG  (MC_BASE + 0x1E4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_276_REG  (MC_BASE + 0x1E50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_277_REG  (MC_BASE + 0x1E54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_278_REG  (MC_BASE + 0x1E58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_279_REG  (MC_BASE + 0x1E5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_280_REG  (MC_BASE + 0x1E60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_281_REG  (MC_BASE + 0x1E64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_282_REG  (MC_BASE + 0x1E68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_283_REG  (MC_BASE + 0x1E6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_284_REG  (MC_BASE + 0x1E70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_285_REG  (MC_BASE + 0x1E74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_286_REG  (MC_BASE + 0x1E78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_287_REG  (MC_BASE + 0x1E7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_288_REG  (MC_BASE + 0x1E80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_289_REG  (MC_BASE + 0x1E84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_290_REG  (MC_BASE + 0x1E88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_291_REG  (MC_BASE + 0x1E8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_292_REG  (MC_BASE + 0x1E90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_293_REG  (MC_BASE + 0x1E94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_294_REG  (MC_BASE + 0x1E98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_295_REG  (MC_BASE + 0x1E9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_296_REG  (MC_BASE + 0x1EA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_297_REG  (MC_BASE + 0x1EA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_298_REG  (MC_BASE + 0x1EA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_299_REG  (MC_BASE + 0x1EAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_300_REG  (MC_BASE + 0x1EB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_301_REG  (MC_BASE + 0x1EB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_302_REG  (MC_BASE + 0x1EB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_303_REG  (MC_BASE + 0x1EBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_304_REG  (MC_BASE + 0x1EC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_305_REG  (MC_BASE + 0x1EC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_306_REG  (MC_BASE + 0x1EC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_307_REG  (MC_BASE + 0x1ECC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_308_REG  (MC_BASE + 0x1ED0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_309_REG  (MC_BASE + 0x1ED4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_310_REG  (MC_BASE + 0x1ED8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_311_REG  (MC_BASE + 0x1EDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_312_REG  (MC_BASE + 0x1EE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_313_REG  (MC_BASE + 0x1EE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_314_REG  (MC_BASE + 0x1EE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_315_REG  (MC_BASE + 0x1EEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_316_REG  (MC_BASE + 0x1EF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_317_REG  (MC_BASE + 0x1EF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_318_REG  (MC_BASE + 0x1EF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_319_REG  (MC_BASE + 0x1EFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_320_REG  (MC_BASE + 0x1F00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_321_REG  (MC_BASE + 0x1F04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_322_REG  (MC_BASE + 0x1F08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_323_REG  (MC_BASE + 0x1F0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_324_REG  (MC_BASE + 0x1F10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_325_REG  (MC_BASE + 0x1F14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_326_REG  (MC_BASE + 0x1F18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_327_REG  (MC_BASE + 0x1F1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_328_REG  (MC_BASE + 0x1F20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_329_REG  (MC_BASE + 0x1F24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_330_REG  (MC_BASE + 0x1F28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_331_REG  (MC_BASE + 0x1F2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_332_REG  (MC_BASE + 0x1F30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_333_REG  (MC_BASE + 0x1F34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_334_REG  (MC_BASE + 0x1F38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_335_REG  (MC_BASE + 0x1F3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_336_REG  (MC_BASE + 0x1F40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_337_REG  (MC_BASE + 0x1F44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_338_REG  (MC_BASE + 0x1F48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_339_REG  (MC_BASE + 0x1F4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_340_REG  (MC_BASE + 0x1F50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_341_REG  (MC_BASE + 0x1F54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_342_REG  (MC_BASE + 0x1F58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_343_REG  (MC_BASE + 0x1F5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_344_REG  (MC_BASE + 0x1F60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_345_REG  (MC_BASE + 0x1F64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_346_REG  (MC_BASE + 0x1F68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_347_REG  (MC_BASE + 0x1F6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_348_REG  (MC_BASE + 0x1F70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_349_REG  (MC_BASE + 0x1F74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_350_REG  (MC_BASE + 0x1F78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_351_REG  (MC_BASE + 0x1F7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_352_REG  (MC_BASE + 0x1F80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_353_REG  (MC_BASE + 0x1F84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_354_REG  (MC_BASE + 0x1F88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_355_REG  (MC_BASE + 0x1F8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_356_REG  (MC_BASE + 0x1F90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_357_REG  (MC_BASE + 0x1F94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_358_REG  (MC_BASE + 0x1F98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_359_REG  (MC_BASE + 0x1F9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_360_REG  (MC_BASE + 0x1FA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_361_REG  (MC_BASE + 0x1FA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_362_REG  (MC_BASE + 0x1FA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_363_REG  (MC_BASE + 0x1FAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_364_REG  (MC_BASE + 0x1FB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_365_REG  (MC_BASE + 0x1FB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_366_REG  (MC_BASE + 0x1FB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_367_REG  (MC_BASE + 0x1FBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_368_REG  (MC_BASE + 0x1FC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_369_REG  (MC_BASE + 0x1FC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_370_REG  (MC_BASE + 0x1FC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_371_REG  (MC_BASE + 0x1FCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_372_REG  (MC_BASE + 0x1FD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_373_REG  (MC_BASE + 0x1FD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_374_REG  (MC_BASE + 0x1FD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_375_REG  (MC_BASE + 0x1FDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_376_REG  (MC_BASE + 0x1FE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_377_REG  (MC_BASE + 0x1FE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_378_REG  (MC_BASE + 0x1FE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_379_REG  (MC_BASE + 0x1FEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_380_REG  (MC_BASE + 0x1FF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_381_REG  (MC_BASE + 0x1FF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_382_REG  (MC_BASE + 0x1FF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_383_REG  (MC_BASE + 0x1FFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_384_REG  (MC_BASE + 0x2000) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_385_REG  (MC_BASE + 0x2004) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_386_REG  (MC_BASE + 0x2008) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_387_REG  (MC_BASE + 0x200C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_388_REG  (MC_BASE + 0x2010) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_389_REG  (MC_BASE + 0x2014) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_390_REG  (MC_BASE + 0x2018) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_391_REG  (MC_BASE + 0x201C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_392_REG  (MC_BASE + 0x2020) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_393_REG  (MC_BASE + 0x2024) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_394_REG  (MC_BASE + 0x2028) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_395_REG  (MC_BASE + 0x202C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_396_REG  (MC_BASE + 0x2030) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_397_REG  (MC_BASE + 0x2034) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_398_REG  (MC_BASE + 0x2038) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_399_REG  (MC_BASE + 0x203C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_400_REG  (MC_BASE + 0x2040) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_401_REG  (MC_BASE + 0x2044) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_402_REG  (MC_BASE + 0x2048) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_403_REG  (MC_BASE + 0x204C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_404_REG  (MC_BASE + 0x2050) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_405_REG  (MC_BASE + 0x2054) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_406_REG  (MC_BASE + 0x2058) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_407_REG  (MC_BASE + 0x205C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_408_REG  (MC_BASE + 0x2060) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_409_REG  (MC_BASE + 0x2064) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_410_REG  (MC_BASE + 0x2068) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_411_REG  (MC_BASE + 0x206C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_412_REG  (MC_BASE + 0x2070) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_413_REG  (MC_BASE + 0x2074) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_414_REG  (MC_BASE + 0x2078) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_415_REG  (MC_BASE + 0x207C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_416_REG  (MC_BASE + 0x2080) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_417_REG  (MC_BASE + 0x2084) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_418_REG  (MC_BASE + 0x2088) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_419_REG  (MC_BASE + 0x208C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_420_REG  (MC_BASE + 0x2090) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_421_REG  (MC_BASE + 0x2094) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_422_REG  (MC_BASE + 0x2098) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_423_REG  (MC_BASE + 0x209C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_424_REG  (MC_BASE + 0x20A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_425_REG  (MC_BASE + 0x20A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_426_REG  (MC_BASE + 0x20A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_427_REG  (MC_BASE + 0x20AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_428_REG  (MC_BASE + 0x20B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_429_REG  (MC_BASE + 0x20B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_430_REG  (MC_BASE + 0x20B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_431_REG  (MC_BASE + 0x20BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_432_REG  (MC_BASE + 0x20C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_433_REG  (MC_BASE + 0x20C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_434_REG  (MC_BASE + 0x20C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_435_REG  (MC_BASE + 0x20CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_436_REG  (MC_BASE + 0x20D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_437_REG  (MC_BASE + 0x20D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_438_REG  (MC_BASE + 0x20D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_439_REG  (MC_BASE + 0x20DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_440_REG  (MC_BASE + 0x20E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_441_REG  (MC_BASE + 0x20E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_442_REG  (MC_BASE + 0x20E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_443_REG  (MC_BASE + 0x20EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_444_REG  (MC_BASE + 0x20F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_445_REG  (MC_BASE + 0x20F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_446_REG  (MC_BASE + 0x20F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_447_REG  (MC_BASE + 0x20FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_448_REG  (MC_BASE + 0x2100) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_449_REG  (MC_BASE + 0x2104) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_450_REG  (MC_BASE + 0x2108) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_451_REG  (MC_BASE + 0x210C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_452_REG  (MC_BASE + 0x2110) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_453_REG  (MC_BASE + 0x2114) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_454_REG  (MC_BASE + 0x2118) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_455_REG  (MC_BASE + 0x211C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_456_REG  (MC_BASE + 0x2120) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_457_REG  (MC_BASE + 0x2124) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_458_REG  (MC_BASE + 0x2128) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_459_REG  (MC_BASE + 0x212C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_460_REG  (MC_BASE + 0x2130) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_461_REG  (MC_BASE + 0x2134) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_462_REG  (MC_BASE + 0x2138) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_463_REG  (MC_BASE + 0x213C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_464_REG  (MC_BASE + 0x2140) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_465_REG  (MC_BASE + 0x2144) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_466_REG  (MC_BASE + 0x2148) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_467_REG  (MC_BASE + 0x214C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_468_REG  (MC_BASE + 0x2150) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_469_REG  (MC_BASE + 0x2154) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_470_REG  (MC_BASE + 0x2158) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_471_REG  (MC_BASE + 0x215C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_472_REG  (MC_BASE + 0x2160) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_473_REG  (MC_BASE + 0x2164) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_474_REG  (MC_BASE + 0x2168) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_475_REG  (MC_BASE + 0x216C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_476_REG  (MC_BASE + 0x2170) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_477_REG  (MC_BASE + 0x2174) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_478_REG  (MC_BASE + 0x2178) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_479_REG  (MC_BASE + 0x217C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_480_REG  (MC_BASE + 0x2180) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_481_REG  (MC_BASE + 0x2184) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_482_REG  (MC_BASE + 0x2188) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_483_REG  (MC_BASE + 0x218C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_484_REG  (MC_BASE + 0x2190) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_485_REG  (MC_BASE + 0x2194) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_486_REG  (MC_BASE + 0x2198) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_487_REG  (MC_BASE + 0x219C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_488_REG  (MC_BASE + 0x21A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_489_REG  (MC_BASE + 0x21A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_490_REG  (MC_BASE + 0x21A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_491_REG  (MC_BASE + 0x21AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_492_REG  (MC_BASE + 0x21B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_493_REG  (MC_BASE + 0x21B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_494_REG  (MC_BASE + 0x21B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_495_REG  (MC_BASE + 0x21BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_496_REG  (MC_BASE + 0x21C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_497_REG  (MC_BASE + 0x21C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_498_REG  (MC_BASE + 0x21C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_499_REG  (MC_BASE + 0x21CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_500_REG  (MC_BASE + 0x21D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_501_REG  (MC_BASE + 0x21D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_502_REG  (MC_BASE + 0x21D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_503_REG  (MC_BASE + 0x21DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_504_REG  (MC_BASE + 0x21E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_505_REG  (MC_BASE + 0x21E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_506_REG  (MC_BASE + 0x21E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_507_REG  (MC_BASE + 0x21EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_508_REG  (MC_BASE + 0x21F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_509_REG  (MC_BASE + 0x21F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_510_REG  (MC_BASE + 0x21F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_511_REG  (MC_BASE + 0x21FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_512_REG  (MC_BASE + 0x2200) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_513_REG  (MC_BASE + 0x2204) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_514_REG  (MC_BASE + 0x2208) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_515_REG  (MC_BASE + 0x220C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_516_REG  (MC_BASE + 0x2210) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_517_REG  (MC_BASE + 0x2214) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_518_REG  (MC_BASE + 0x2218) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_519_REG  (MC_BASE + 0x221C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_520_REG  (MC_BASE + 0x2220) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_521_REG  (MC_BASE + 0x2224) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_522_REG  (MC_BASE + 0x2228) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_523_REG  (MC_BASE + 0x222C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_524_REG  (MC_BASE + 0x2230) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_525_REG  (MC_BASE + 0x2234) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_526_REG  (MC_BASE + 0x2238) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_527_REG  (MC_BASE + 0x223C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_528_REG  (MC_BASE + 0x2240) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_529_REG  (MC_BASE + 0x2244) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_530_REG  (MC_BASE + 0x2248) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_531_REG  (MC_BASE + 0x224C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_532_REG  (MC_BASE + 0x2250) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_533_REG  (MC_BASE + 0x2254) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_534_REG  (MC_BASE + 0x2258) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_535_REG  (MC_BASE + 0x225C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_536_REG  (MC_BASE + 0x2260) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_537_REG  (MC_BASE + 0x2264) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_538_REG  (MC_BASE + 0x2268) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_539_REG  (MC_BASE + 0x226C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_540_REG  (MC_BASE + 0x2270) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_541_REG  (MC_BASE + 0x2274) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_542_REG  (MC_BASE + 0x2278) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_543_REG  (MC_BASE + 0x227C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_544_REG  (MC_BASE + 0x2280) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_545_REG  (MC_BASE + 0x2284) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_546_REG  (MC_BASE + 0x2288) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_547_REG  (MC_BASE + 0x228C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_548_REG  (MC_BASE + 0x2290) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_549_REG  (MC_BASE + 0x2294) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_550_REG  (MC_BASE + 0x2298) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_551_REG  (MC_BASE + 0x229C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_552_REG  (MC_BASE + 0x22A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_553_REG  (MC_BASE + 0x22A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_554_REG  (MC_BASE + 0x22A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_555_REG  (MC_BASE + 0x22AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_556_REG  (MC_BASE + 0x22B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_557_REG  (MC_BASE + 0x22B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_558_REG  (MC_BASE + 0x22B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_559_REG  (MC_BASE + 0x22BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_560_REG  (MC_BASE + 0x22C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_561_REG  (MC_BASE + 0x22C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_562_REG  (MC_BASE + 0x22C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_563_REG  (MC_BASE + 0x22CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_564_REG  (MC_BASE + 0x22D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_565_REG  (MC_BASE + 0x22D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_566_REG  (MC_BASE + 0x22D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_567_REG  (MC_BASE + 0x22DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_568_REG  (MC_BASE + 0x22E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_569_REG  (MC_BASE + 0x22E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_570_REG  (MC_BASE + 0x22E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_571_REG  (MC_BASE + 0x22EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_572_REG  (MC_BASE + 0x22F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_573_REG  (MC_BASE + 0x22F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_574_REG  (MC_BASE + 0x22F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_575_REG  (MC_BASE + 0x22FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_576_REG  (MC_BASE + 0x2300) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_577_REG  (MC_BASE + 0x2304) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_578_REG  (MC_BASE + 0x2308) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_579_REG  (MC_BASE + 0x230C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_580_REG  (MC_BASE + 0x2310) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_581_REG  (MC_BASE + 0x2314) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_582_REG  (MC_BASE + 0x2318) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_583_REG  (MC_BASE + 0x231C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_584_REG  (MC_BASE + 0x2320) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_585_REG  (MC_BASE + 0x2324) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_586_REG  (MC_BASE + 0x2328) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_587_REG  (MC_BASE + 0x232C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_588_REG  (MC_BASE + 0x2330) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_589_REG  (MC_BASE + 0x2334) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_590_REG  (MC_BASE + 0x2338) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_591_REG  (MC_BASE + 0x233C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_592_REG  (MC_BASE + 0x2340) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_593_REG  (MC_BASE + 0x2344) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_594_REG  (MC_BASE + 0x2348) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_595_REG  (MC_BASE + 0x234C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_596_REG  (MC_BASE + 0x2350) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_597_REG  (MC_BASE + 0x2354) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_598_REG  (MC_BASE + 0x2358) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_599_REG  (MC_BASE + 0x235C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_600_REG  (MC_BASE + 0x2360) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_601_REG  (MC_BASE + 0x2364) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_602_REG  (MC_BASE + 0x2368) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_603_REG  (MC_BASE + 0x236C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_604_REG  (MC_BASE + 0x2370) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_605_REG  (MC_BASE + 0x2374) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_606_REG  (MC_BASE + 0x2378) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_607_REG  (MC_BASE + 0x237C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_608_REG  (MC_BASE + 0x2380) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_609_REG  (MC_BASE + 0x2384) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_610_REG  (MC_BASE + 0x2388) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_611_REG  (MC_BASE + 0x238C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_612_REG  (MC_BASE + 0x2390) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_613_REG  (MC_BASE + 0x2394) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_614_REG  (MC_BASE + 0x2398) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_615_REG  (MC_BASE + 0x239C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_616_REG  (MC_BASE + 0x23A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_617_REG  (MC_BASE + 0x23A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_618_REG  (MC_BASE + 0x23A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_619_REG  (MC_BASE + 0x23AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_620_REG  (MC_BASE + 0x23B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_621_REG  (MC_BASE + 0x23B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_622_REG  (MC_BASE + 0x23B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_623_REG  (MC_BASE + 0x23BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_624_REG  (MC_BASE + 0x23C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_625_REG  (MC_BASE + 0x23C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_626_REG  (MC_BASE + 0x23C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_627_REG  (MC_BASE + 0x23CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_628_REG  (MC_BASE + 0x23D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_629_REG  (MC_BASE + 0x23D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_630_REG  (MC_BASE + 0x23D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_631_REG  (MC_BASE + 0x23DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_632_REG  (MC_BASE + 0x23E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_633_REG  (MC_BASE + 0x23E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_634_REG  (MC_BASE + 0x23E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_635_REG  (MC_BASE + 0x23EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_636_REG  (MC_BASE + 0x23F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_637_REG  (MC_BASE + 0x23F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_638_REG  (MC_BASE + 0x23F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_639_REG  (MC_BASE + 0x23FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_640_REG  (MC_BASE + 0x2400) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_641_REG  (MC_BASE + 0x2404) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_642_REG  (MC_BASE + 0x2408) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_643_REG  (MC_BASE + 0x240C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_644_REG  (MC_BASE + 0x2410) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_645_REG  (MC_BASE + 0x2414) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_646_REG  (MC_BASE + 0x2418) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_647_REG  (MC_BASE + 0x241C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_648_REG  (MC_BASE + 0x2420) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_649_REG  (MC_BASE + 0x2424) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_650_REG  (MC_BASE + 0x2428) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_651_REG  (MC_BASE + 0x242C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_652_REG  (MC_BASE + 0x2430) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_653_REG  (MC_BASE + 0x2434) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_654_REG  (MC_BASE + 0x2438) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_655_REG  (MC_BASE + 0x243C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_656_REG  (MC_BASE + 0x2440) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_657_REG  (MC_BASE + 0x2444) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_658_REG  (MC_BASE + 0x2448) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_659_REG  (MC_BASE + 0x244C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_660_REG  (MC_BASE + 0x2450) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_661_REG  (MC_BASE + 0x2454) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_662_REG  (MC_BASE + 0x2458) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_663_REG  (MC_BASE + 0x245C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_664_REG  (MC_BASE + 0x2460) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_665_REG  (MC_BASE + 0x2464) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_666_REG  (MC_BASE + 0x2468) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_667_REG  (MC_BASE + 0x246C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_668_REG  (MC_BASE + 0x2470) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_669_REG  (MC_BASE + 0x2474) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_670_REG  (MC_BASE + 0x2478) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_671_REG  (MC_BASE + 0x247C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_672_REG  (MC_BASE + 0x2480) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_673_REG  (MC_BASE + 0x2484) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_674_REG  (MC_BASE + 0x2488) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_675_REG  (MC_BASE + 0x248C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_676_REG  (MC_BASE + 0x2490) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_677_REG  (MC_BASE + 0x2494) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_678_REG  (MC_BASE + 0x2498) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_679_REG  (MC_BASE + 0x249C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_680_REG  (MC_BASE + 0x24A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_681_REG  (MC_BASE + 0x24A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_682_REG  (MC_BASE + 0x24A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_683_REG  (MC_BASE + 0x24AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_684_REG  (MC_BASE + 0x24B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_685_REG  (MC_BASE + 0x24B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_686_REG  (MC_BASE + 0x24B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_687_REG  (MC_BASE + 0x24BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_688_REG  (MC_BASE + 0x24C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_689_REG  (MC_BASE + 0x24C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_690_REG  (MC_BASE + 0x24C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_691_REG  (MC_BASE + 0x24CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_692_REG  (MC_BASE + 0x24D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_693_REG  (MC_BASE + 0x24D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_694_REG  (MC_BASE + 0x24D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_695_REG  (MC_BASE + 0x24DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_696_REG  (MC_BASE + 0x24E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_697_REG  (MC_BASE + 0x24E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_698_REG  (MC_BASE + 0x24E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_699_REG  (MC_BASE + 0x24EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_700_REG  (MC_BASE + 0x24F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_701_REG  (MC_BASE + 0x24F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_702_REG  (MC_BASE + 0x24F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_703_REG  (MC_BASE + 0x24FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_704_REG  (MC_BASE + 0x2500) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_705_REG  (MC_BASE + 0x2504) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_706_REG  (MC_BASE + 0x2508) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_707_REG  (MC_BASE + 0x250C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_708_REG  (MC_BASE + 0x2510) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_709_REG  (MC_BASE + 0x2514) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_710_REG  (MC_BASE + 0x2518) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_711_REG  (MC_BASE + 0x251C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_712_REG  (MC_BASE + 0x2520) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_713_REG  (MC_BASE + 0x2524) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_714_REG  (MC_BASE + 0x2528) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_715_REG  (MC_BASE + 0x252C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_716_REG  (MC_BASE + 0x2530) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_717_REG  (MC_BASE + 0x2534) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_718_REG  (MC_BASE + 0x2538) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_719_REG  (MC_BASE + 0x253C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_720_REG  (MC_BASE + 0x2540) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_721_REG  (MC_BASE + 0x2544) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_722_REG  (MC_BASE + 0x2548) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_723_REG  (MC_BASE + 0x254C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_724_REG  (MC_BASE + 0x2550) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_725_REG  (MC_BASE + 0x2554) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_726_REG  (MC_BASE + 0x2558) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_727_REG  (MC_BASE + 0x255C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_728_REG  (MC_BASE + 0x2560) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_729_REG  (MC_BASE + 0x2564) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_730_REG  (MC_BASE + 0x2568) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_731_REG  (MC_BASE + 0x256C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_732_REG  (MC_BASE + 0x2570) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_733_REG  (MC_BASE + 0x2574) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_734_REG  (MC_BASE + 0x2578) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_735_REG  (MC_BASE + 0x257C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_736_REG  (MC_BASE + 0x2580) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_737_REG  (MC_BASE + 0x2584) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_738_REG  (MC_BASE + 0x2588) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_739_REG  (MC_BASE + 0x258C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_740_REG  (MC_BASE + 0x2590) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_741_REG  (MC_BASE + 0x2594) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_742_REG  (MC_BASE + 0x2598) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_743_REG  (MC_BASE + 0x259C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_744_REG  (MC_BASE + 0x25A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_745_REG  (MC_BASE + 0x25A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_746_REG  (MC_BASE + 0x25A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_747_REG  (MC_BASE + 0x25AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_748_REG  (MC_BASE + 0x25B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_749_REG  (MC_BASE + 0x25B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_750_REG  (MC_BASE + 0x25B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_751_REG  (MC_BASE + 0x25BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_752_REG  (MC_BASE + 0x25C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_753_REG  (MC_BASE + 0x25C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_754_REG  (MC_BASE + 0x25C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_755_REG  (MC_BASE + 0x25CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_756_REG  (MC_BASE + 0x25D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_757_REG  (MC_BASE + 0x25D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_758_REG  (MC_BASE + 0x25D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_759_REG  (MC_BASE + 0x25DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_760_REG  (MC_BASE + 0x25E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_761_REG  (MC_BASE + 0x25E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_762_REG  (MC_BASE + 0x25E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_763_REG  (MC_BASE + 0x25EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_764_REG  (MC_BASE + 0x25F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_765_REG  (MC_BASE + 0x25F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_766_REG  (MC_BASE + 0x25F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_767_REG  (MC_BASE + 0x25FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_768_REG  (MC_BASE + 0x2600) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_769_REG  (MC_BASE + 0x2604) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_770_REG  (MC_BASE + 0x2608) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_771_REG  (MC_BASE + 0x260C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_772_REG  (MC_BASE + 0x2610) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_773_REG  (MC_BASE + 0x2614) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_774_REG  (MC_BASE + 0x2618) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_775_REG  (MC_BASE + 0x261C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_776_REG  (MC_BASE + 0x2620) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_777_REG  (MC_BASE + 0x2624) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_778_REG  (MC_BASE + 0x2628) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_779_REG  (MC_BASE + 0x262C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_780_REG  (MC_BASE + 0x2630) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_781_REG  (MC_BASE + 0x2634) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_782_REG  (MC_BASE + 0x2638) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_783_REG  (MC_BASE + 0x263C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_784_REG  (MC_BASE + 0x2640) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_785_REG  (MC_BASE + 0x2644) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_786_REG  (MC_BASE + 0x2648) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_787_REG  (MC_BASE + 0x264C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_788_REG  (MC_BASE + 0x2650) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_789_REG  (MC_BASE + 0x2654) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_790_REG  (MC_BASE + 0x2658) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_791_REG  (MC_BASE + 0x265C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_792_REG  (MC_BASE + 0x2660) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_793_REG  (MC_BASE + 0x2664) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_794_REG  (MC_BASE + 0x2668) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_795_REG  (MC_BASE + 0x266C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_796_REG  (MC_BASE + 0x2670) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_797_REG  (MC_BASE + 0x2674) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_798_REG  (MC_BASE + 0x2678) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_799_REG  (MC_BASE + 0x267C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_800_REG  (MC_BASE + 0x2680) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_801_REG  (MC_BASE + 0x2684) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_802_REG  (MC_BASE + 0x2688) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_803_REG  (MC_BASE + 0x268C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_804_REG  (MC_BASE + 0x2690) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_805_REG  (MC_BASE + 0x2694) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_806_REG  (MC_BASE + 0x2698) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_807_REG  (MC_BASE + 0x269C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_808_REG  (MC_BASE + 0x26A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_809_REG  (MC_BASE + 0x26A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_810_REG  (MC_BASE + 0x26A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_811_REG  (MC_BASE + 0x26AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_812_REG  (MC_BASE + 0x26B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_813_REG  (MC_BASE + 0x26B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_814_REG  (MC_BASE + 0x26B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_815_REG  (MC_BASE + 0x26BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_816_REG  (MC_BASE + 0x26C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_817_REG  (MC_BASE + 0x26C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_818_REG  (MC_BASE + 0x26C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_819_REG  (MC_BASE + 0x26CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_820_REG  (MC_BASE + 0x26D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_821_REG  (MC_BASE + 0x26D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_822_REG  (MC_BASE + 0x26D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_823_REG  (MC_BASE + 0x26DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_824_REG  (MC_BASE + 0x26E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_825_REG  (MC_BASE + 0x26E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_826_REG  (MC_BASE + 0x26E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_827_REG  (MC_BASE + 0x26EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_828_REG  (MC_BASE + 0x26F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_829_REG  (MC_BASE + 0x26F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_830_REG  (MC_BASE + 0x26F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_831_REG  (MC_BASE + 0x26FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_832_REG  (MC_BASE + 0x2700) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_833_REG  (MC_BASE + 0x2704) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_834_REG  (MC_BASE + 0x2708) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_835_REG  (MC_BASE + 0x270C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_836_REG  (MC_BASE + 0x2710) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_837_REG  (MC_BASE + 0x2714) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_838_REG  (MC_BASE + 0x2718) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_839_REG  (MC_BASE + 0x271C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_840_REG  (MC_BASE + 0x2720) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_841_REG  (MC_BASE + 0x2724) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_842_REG  (MC_BASE + 0x2728) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_843_REG  (MC_BASE + 0x272C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_844_REG  (MC_BASE + 0x2730) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_845_REG  (MC_BASE + 0x2734) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_846_REG  (MC_BASE + 0x2738) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_847_REG  (MC_BASE + 0x273C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_848_REG  (MC_BASE + 0x2740) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_849_REG  (MC_BASE + 0x2744) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_850_REG  (MC_BASE + 0x2748) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_851_REG  (MC_BASE + 0x274C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_852_REG  (MC_BASE + 0x2750) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_853_REG  (MC_BASE + 0x2754) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_854_REG  (MC_BASE + 0x2758) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_855_REG  (MC_BASE + 0x275C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_856_REG  (MC_BASE + 0x2760) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_857_REG  (MC_BASE + 0x2764) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_858_REG  (MC_BASE + 0x2768) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_859_REG  (MC_BASE + 0x276C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_860_REG  (MC_BASE + 0x2770) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_861_REG  (MC_BASE + 0x2774) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_862_REG  (MC_BASE + 0x2778) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_863_REG  (MC_BASE + 0x277C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_864_REG  (MC_BASE + 0x2780) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_865_REG  (MC_BASE + 0x2784) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_866_REG  (MC_BASE + 0x2788) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_867_REG  (MC_BASE + 0x278C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_868_REG  (MC_BASE + 0x2790) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_869_REG  (MC_BASE + 0x2794) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_870_REG  (MC_BASE + 0x2798) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_871_REG  (MC_BASE + 0x279C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_872_REG  (MC_BASE + 0x27A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_873_REG  (MC_BASE + 0x27A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_874_REG  (MC_BASE + 0x27A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_875_REG  (MC_BASE + 0x27AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_876_REG  (MC_BASE + 0x27B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_877_REG  (MC_BASE + 0x27B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_878_REG  (MC_BASE + 0x27B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_879_REG  (MC_BASE + 0x27BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_880_REG  (MC_BASE + 0x27C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_881_REG  (MC_BASE + 0x27C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_882_REG  (MC_BASE + 0x27C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_883_REG  (MC_BASE + 0x27CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_884_REG  (MC_BASE + 0x27D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_885_REG  (MC_BASE + 0x27D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_886_REG  (MC_BASE + 0x27D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_887_REG  (MC_BASE + 0x27DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_888_REG  (MC_BASE + 0x27E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_889_REG  (MC_BASE + 0x27E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_890_REG  (MC_BASE + 0x27E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_891_REG  (MC_BASE + 0x27EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_892_REG  (MC_BASE + 0x27F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_893_REG  (MC_BASE + 0x27F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_894_REG  (MC_BASE + 0x27F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_895_REG  (MC_BASE + 0x27FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_896_REG  (MC_BASE + 0x2800) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_897_REG  (MC_BASE + 0x2804) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_898_REG  (MC_BASE + 0x2808) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_899_REG  (MC_BASE + 0x280C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_900_REG  (MC_BASE + 0x2810) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_901_REG  (MC_BASE + 0x2814) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_902_REG  (MC_BASE + 0x2818) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_903_REG  (MC_BASE + 0x281C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_904_REG  (MC_BASE + 0x2820) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_905_REG  (MC_BASE + 0x2824) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_906_REG  (MC_BASE + 0x2828) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_907_REG  (MC_BASE + 0x282C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_908_REG  (MC_BASE + 0x2830) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_909_REG  (MC_BASE + 0x2834) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_910_REG  (MC_BASE + 0x2838) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_911_REG  (MC_BASE + 0x283C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_912_REG  (MC_BASE + 0x2840) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_913_REG  (MC_BASE + 0x2844) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_914_REG  (MC_BASE + 0x2848) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_915_REG  (MC_BASE + 0x284C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_916_REG  (MC_BASE + 0x2850) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_917_REG  (MC_BASE + 0x2854) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_918_REG  (MC_BASE + 0x2858) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_919_REG  (MC_BASE + 0x285C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_920_REG  (MC_BASE + 0x2860) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_921_REG  (MC_BASE + 0x2864) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_922_REG  (MC_BASE + 0x2868) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_923_REG  (MC_BASE + 0x286C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_924_REG  (MC_BASE + 0x2870) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_925_REG  (MC_BASE + 0x2874) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_926_REG  (MC_BASE + 0x2878) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_927_REG  (MC_BASE + 0x287C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_928_REG  (MC_BASE + 0x2880) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_929_REG  (MC_BASE + 0x2884) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_930_REG  (MC_BASE + 0x2888) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_931_REG  (MC_BASE + 0x288C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_932_REG  (MC_BASE + 0x2890) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_933_REG  (MC_BASE + 0x2894) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_934_REG  (MC_BASE + 0x2898) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_935_REG  (MC_BASE + 0x289C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_936_REG  (MC_BASE + 0x28A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_937_REG  (MC_BASE + 0x28A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_938_REG  (MC_BASE + 0x28A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_939_REG  (MC_BASE + 0x28AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_940_REG  (MC_BASE + 0x28B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_941_REG  (MC_BASE + 0x28B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_942_REG  (MC_BASE + 0x28B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_943_REG  (MC_BASE + 0x28BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_944_REG  (MC_BASE + 0x28C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_945_REG  (MC_BASE + 0x28C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_946_REG  (MC_BASE + 0x28C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_947_REG  (MC_BASE + 0x28CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_948_REG  (MC_BASE + 0x28D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_949_REG  (MC_BASE + 0x28D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_950_REG  (MC_BASE + 0x28D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_951_REG  (MC_BASE + 0x28DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_952_REG  (MC_BASE + 0x28E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_953_REG  (MC_BASE + 0x28E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_954_REG  (MC_BASE + 0x28E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_955_REG  (MC_BASE + 0x28EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_956_REG  (MC_BASE + 0x28F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_957_REG  (MC_BASE + 0x28F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_958_REG  (MC_BASE + 0x28F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_959_REG  (MC_BASE + 0x28FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_960_REG  (MC_BASE + 0x2900) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_961_REG  (MC_BASE + 0x2904) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_962_REG  (MC_BASE + 0x2908) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_963_REG  (MC_BASE + 0x290C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_964_REG  (MC_BASE + 0x2910) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_965_REG  (MC_BASE + 0x2914) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_966_REG  (MC_BASE + 0x2918) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_967_REG  (MC_BASE + 0x291C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_968_REG  (MC_BASE + 0x2920) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_969_REG  (MC_BASE + 0x2924) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_970_REG  (MC_BASE + 0x2928) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_971_REG  (MC_BASE + 0x292C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_972_REG  (MC_BASE + 0x2930) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_973_REG  (MC_BASE + 0x2934) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_974_REG  (MC_BASE + 0x2938) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_975_REG  (MC_BASE + 0x293C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_976_REG  (MC_BASE + 0x2940) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_977_REG  (MC_BASE + 0x2944) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_978_REG  (MC_BASE + 0x2948) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_979_REG  (MC_BASE + 0x294C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_980_REG  (MC_BASE + 0x2950) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_981_REG  (MC_BASE + 0x2954) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_982_REG  (MC_BASE + 0x2958) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_983_REG  (MC_BASE + 0x295C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_984_REG  (MC_BASE + 0x2960) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_985_REG  (MC_BASE + 0x2964) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_986_REG  (MC_BASE + 0x2968) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_987_REG  (MC_BASE + 0x296C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_988_REG  (MC_BASE + 0x2970) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_989_REG  (MC_BASE + 0x2974) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_990_REG  (MC_BASE + 0x2978) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_991_REG  (MC_BASE + 0x297C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_992_REG  (MC_BASE + 0x2980) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_993_REG  (MC_BASE + 0x2984) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_994_REG  (MC_BASE + 0x2988) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_995_REG  (MC_BASE + 0x298C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_996_REG  (MC_BASE + 0x2990) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_997_REG  (MC_BASE + 0x2994) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_998_REG  (MC_BASE + 0x2998) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_999_REG  (MC_BASE + 0x299C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1000_REG (MC_BASE + 0x29A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1001_REG (MC_BASE + 0x29A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1002_REG (MC_BASE + 0x29A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1003_REG (MC_BASE + 0x29AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1004_REG (MC_BASE + 0x29B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1005_REG (MC_BASE + 0x29B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1006_REG (MC_BASE + 0x29B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1007_REG (MC_BASE + 0x29BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1008_REG (MC_BASE + 0x29C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1009_REG (MC_BASE + 0x29C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1010_REG (MC_BASE + 0x29C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1011_REG (MC_BASE + 0x29CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1012_REG (MC_BASE + 0x29D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1013_REG (MC_BASE + 0x29D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1014_REG (MC_BASE + 0x29D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1015_REG (MC_BASE + 0x29DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1016_REG (MC_BASE + 0x29E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1017_REG (MC_BASE + 0x29E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1018_REG (MC_BASE + 0x29E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1019_REG (MC_BASE + 0x29EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1020_REG (MC_BASE + 0x29F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1021_REG (MC_BASE + 0x29F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1022_REG (MC_BASE + 0x29F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1023_REG (MC_BASE + 0x29FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1024_REG (MC_BASE + 0x2A00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1025_REG (MC_BASE + 0x2A04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1026_REG (MC_BASE + 0x2A08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1027_REG (MC_BASE + 0x2A0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1028_REG (MC_BASE + 0x2A10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1029_REG (MC_BASE + 0x2A14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1030_REG (MC_BASE + 0x2A18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1031_REG (MC_BASE + 0x2A1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1032_REG (MC_BASE + 0x2A20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1033_REG (MC_BASE + 0x2A24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1034_REG (MC_BASE + 0x2A28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1035_REG (MC_BASE + 0x2A2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1036_REG (MC_BASE + 0x2A30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1037_REG (MC_BASE + 0x2A34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1038_REG (MC_BASE + 0x2A38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1039_REG (MC_BASE + 0x2A3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1040_REG (MC_BASE + 0x2A40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1041_REG (MC_BASE + 0x2A44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1042_REG (MC_BASE + 0x2A48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1043_REG (MC_BASE + 0x2A4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1044_REG (MC_BASE + 0x2A50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1045_REG (MC_BASE + 0x2A54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1046_REG (MC_BASE + 0x2A58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1047_REG (MC_BASE + 0x2A5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1048_REG (MC_BASE + 0x2A60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1049_REG (MC_BASE + 0x2A64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1050_REG (MC_BASE + 0x2A68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1051_REG (MC_BASE + 0x2A6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1052_REG (MC_BASE + 0x2A70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1053_REG (MC_BASE + 0x2A74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1054_REG (MC_BASE + 0x2A78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1055_REG (MC_BASE + 0x2A7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1056_REG (MC_BASE + 0x2A80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1057_REG (MC_BASE + 0x2A84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1058_REG (MC_BASE + 0x2A88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1059_REG (MC_BASE + 0x2A8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1060_REG (MC_BASE + 0x2A90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1061_REG (MC_BASE + 0x2A94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1062_REG (MC_BASE + 0x2A98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1063_REG (MC_BASE + 0x2A9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1064_REG (MC_BASE + 0x2AA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1065_REG (MC_BASE + 0x2AA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1066_REG (MC_BASE + 0x2AA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1067_REG (MC_BASE + 0x2AAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1068_REG (MC_BASE + 0x2AB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1069_REG (MC_BASE + 0x2AB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1070_REG (MC_BASE + 0x2AB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1071_REG (MC_BASE + 0x2ABC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1072_REG (MC_BASE + 0x2AC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1073_REG (MC_BASE + 0x2AC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1074_REG (MC_BASE + 0x2AC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1075_REG (MC_BASE + 0x2ACC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1076_REG (MC_BASE + 0x2AD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1077_REG (MC_BASE + 0x2AD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1078_REG (MC_BASE + 0x2AD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1079_REG (MC_BASE + 0x2ADC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1080_REG (MC_BASE + 0x2AE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1081_REG (MC_BASE + 0x2AE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1082_REG (MC_BASE + 0x2AE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1083_REG (MC_BASE + 0x2AEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1084_REG (MC_BASE + 0x2AF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1085_REG (MC_BASE + 0x2AF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1086_REG (MC_BASE + 0x2AF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1087_REG (MC_BASE + 0x2AFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1088_REG (MC_BASE + 0x2B00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1089_REG (MC_BASE + 0x2B04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1090_REG (MC_BASE + 0x2B08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1091_REG (MC_BASE + 0x2B0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1092_REG (MC_BASE + 0x2B10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1093_REG (MC_BASE + 0x2B14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1094_REG (MC_BASE + 0x2B18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1095_REG (MC_BASE + 0x2B1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1096_REG (MC_BASE + 0x2B20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1097_REG (MC_BASE + 0x2B24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1098_REG (MC_BASE + 0x2B28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1099_REG (MC_BASE + 0x2B2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1100_REG (MC_BASE + 0x2B30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1101_REG (MC_BASE + 0x2B34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1102_REG (MC_BASE + 0x2B38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1103_REG (MC_BASE + 0x2B3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1104_REG (MC_BASE + 0x2B40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1105_REG (MC_BASE + 0x2B44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1106_REG (MC_BASE + 0x2B48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1107_REG (MC_BASE + 0x2B4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1108_REG (MC_BASE + 0x2B50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1109_REG (MC_BASE + 0x2B54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1110_REG (MC_BASE + 0x2B58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1111_REG (MC_BASE + 0x2B5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1112_REG (MC_BASE + 0x2B60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1113_REG (MC_BASE + 0x2B64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1114_REG (MC_BASE + 0x2B68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1115_REG (MC_BASE + 0x2B6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1116_REG (MC_BASE + 0x2B70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1117_REG (MC_BASE + 0x2B74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1118_REG (MC_BASE + 0x2B78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1119_REG (MC_BASE + 0x2B7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1120_REG (MC_BASE + 0x2B80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1121_REG (MC_BASE + 0x2B84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1122_REG (MC_BASE + 0x2B88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1123_REG (MC_BASE + 0x2B8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1124_REG (MC_BASE + 0x2B90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1125_REG (MC_BASE + 0x2B94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1126_REG (MC_BASE + 0x2B98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1127_REG (MC_BASE + 0x2B9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1128_REG (MC_BASE + 0x2BA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1129_REG (MC_BASE + 0x2BA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1130_REG (MC_BASE + 0x2BA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1131_REG (MC_BASE + 0x2BAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1132_REG (MC_BASE + 0x2BB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1133_REG (MC_BASE + 0x2BB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1134_REG (MC_BASE + 0x2BB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1135_REG (MC_BASE + 0x2BBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1136_REG (MC_BASE + 0x2BC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1137_REG (MC_BASE + 0x2BC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1138_REG (MC_BASE + 0x2BC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1139_REG (MC_BASE + 0x2BCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1140_REG (MC_BASE + 0x2BD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1141_REG (MC_BASE + 0x2BD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1142_REG (MC_BASE + 0x2BD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1143_REG (MC_BASE + 0x2BDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1144_REG (MC_BASE + 0x2BE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1145_REG (MC_BASE + 0x2BE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1146_REG (MC_BASE + 0x2BE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1147_REG (MC_BASE + 0x2BEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1148_REG (MC_BASE + 0x2BF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1149_REG (MC_BASE + 0x2BF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1150_REG (MC_BASE + 0x2BF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1151_REG (MC_BASE + 0x2BFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1152_REG (MC_BASE + 0x2C00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1153_REG (MC_BASE + 0x2C04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1154_REG (MC_BASE + 0x2C08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1155_REG (MC_BASE + 0x2C0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1156_REG (MC_BASE + 0x2C10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1157_REG (MC_BASE + 0x2C14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1158_REG (MC_BASE + 0x2C18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1159_REG (MC_BASE + 0x2C1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1160_REG (MC_BASE + 0x2C20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1161_REG (MC_BASE + 0x2C24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1162_REG (MC_BASE + 0x2C28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1163_REG (MC_BASE + 0x2C2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1164_REG (MC_BASE + 0x2C30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1165_REG (MC_BASE + 0x2C34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1166_REG (MC_BASE + 0x2C38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1167_REG (MC_BASE + 0x2C3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1168_REG (MC_BASE + 0x2C40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1169_REG (MC_BASE + 0x2C44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1170_REG (MC_BASE + 0x2C48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1171_REG (MC_BASE + 0x2C4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1172_REG (MC_BASE + 0x2C50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1173_REG (MC_BASE + 0x2C54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1174_REG (MC_BASE + 0x2C58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1175_REG (MC_BASE + 0x2C5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1176_REG (MC_BASE + 0x2C60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1177_REG (MC_BASE + 0x2C64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1178_REG (MC_BASE + 0x2C68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1179_REG (MC_BASE + 0x2C6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1180_REG (MC_BASE + 0x2C70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1181_REG (MC_BASE + 0x2C74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1182_REG (MC_BASE + 0x2C78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1183_REG (MC_BASE + 0x2C7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1184_REG (MC_BASE + 0x2C80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1185_REG (MC_BASE + 0x2C84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1186_REG (MC_BASE + 0x2C88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1187_REG (MC_BASE + 0x2C8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1188_REG (MC_BASE + 0x2C90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1189_REG (MC_BASE + 0x2C94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1190_REG (MC_BASE + 0x2C98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1191_REG (MC_BASE + 0x2C9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1192_REG (MC_BASE + 0x2CA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1193_REG (MC_BASE + 0x2CA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1194_REG (MC_BASE + 0x2CA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1195_REG (MC_BASE + 0x2CAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1196_REG (MC_BASE + 0x2CB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1197_REG (MC_BASE + 0x2CB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1198_REG (MC_BASE + 0x2CB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1199_REG (MC_BASE + 0x2CBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1200_REG (MC_BASE + 0x2CC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1201_REG (MC_BASE + 0x2CC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1202_REG (MC_BASE + 0x2CC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1203_REG (MC_BASE + 0x2CCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1204_REG (MC_BASE + 0x2CD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1205_REG (MC_BASE + 0x2CD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1206_REG (MC_BASE + 0x2CD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1207_REG (MC_BASE + 0x2CDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1208_REG (MC_BASE + 0x2CE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1209_REG (MC_BASE + 0x2CE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1210_REG (MC_BASE + 0x2CE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1211_REG (MC_BASE + 0x2CEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1212_REG (MC_BASE + 0x2CF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1213_REG (MC_BASE + 0x2CF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1214_REG (MC_BASE + 0x2CF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1215_REG (MC_BASE + 0x2CFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1216_REG (MC_BASE + 0x2D00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1217_REG (MC_BASE + 0x2D04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1218_REG (MC_BASE + 0x2D08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1219_REG (MC_BASE + 0x2D0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1220_REG (MC_BASE + 0x2D10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1221_REG (MC_BASE + 0x2D14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1222_REG (MC_BASE + 0x2D18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1223_REG (MC_BASE + 0x2D1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1224_REG (MC_BASE + 0x2D20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1225_REG (MC_BASE + 0x2D24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1226_REG (MC_BASE + 0x2D28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1227_REG (MC_BASE + 0x2D2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1228_REG (MC_BASE + 0x2D30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1229_REG (MC_BASE + 0x2D34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1230_REG (MC_BASE + 0x2D38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1231_REG (MC_BASE + 0x2D3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1232_REG (MC_BASE + 0x2D40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1233_REG (MC_BASE + 0x2D44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1234_REG (MC_BASE + 0x2D48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1235_REG (MC_BASE + 0x2D4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1236_REG (MC_BASE + 0x2D50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1237_REG (MC_BASE + 0x2D54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1238_REG (MC_BASE + 0x2D58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1239_REG (MC_BASE + 0x2D5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1240_REG (MC_BASE + 0x2D60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1241_REG (MC_BASE + 0x2D64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1242_REG (MC_BASE + 0x2D68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1243_REG (MC_BASE + 0x2D6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1244_REG (MC_BASE + 0x2D70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1245_REG (MC_BASE + 0x2D74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1246_REG (MC_BASE + 0x2D78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1247_REG (MC_BASE + 0x2D7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1248_REG (MC_BASE + 0x2D80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1249_REG (MC_BASE + 0x2D84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1250_REG (MC_BASE + 0x2D88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1251_REG (MC_BASE + 0x2D8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1252_REG (MC_BASE + 0x2D90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1253_REG (MC_BASE + 0x2D94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1254_REG (MC_BASE + 0x2D98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1255_REG (MC_BASE + 0x2D9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1256_REG (MC_BASE + 0x2DA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1257_REG (MC_BASE + 0x2DA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1258_REG (MC_BASE + 0x2DA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1259_REG (MC_BASE + 0x2DAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1260_REG (MC_BASE + 0x2DB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1261_REG (MC_BASE + 0x2DB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1262_REG (MC_BASE + 0x2DB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1263_REG (MC_BASE + 0x2DBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1264_REG (MC_BASE + 0x2DC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1265_REG (MC_BASE + 0x2DC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1266_REG (MC_BASE + 0x2DC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1267_REG (MC_BASE + 0x2DCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1268_REG (MC_BASE + 0x2DD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1269_REG (MC_BASE + 0x2DD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1270_REG (MC_BASE + 0x2DD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1271_REG (MC_BASE + 0x2DDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1272_REG (MC_BASE + 0x2DE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1273_REG (MC_BASE + 0x2DE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1274_REG (MC_BASE + 0x2DE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1275_REG (MC_BASE + 0x2DEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1276_REG (MC_BASE + 0x2DF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1277_REG (MC_BASE + 0x2DF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1278_REG (MC_BASE + 0x2DF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1279_REG (MC_BASE + 0x2DFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1280_REG (MC_BASE + 0x2E00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1281_REG (MC_BASE + 0x2E04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1282_REG (MC_BASE + 0x2E08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1283_REG (MC_BASE + 0x2E0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1284_REG (MC_BASE + 0x2E10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1285_REG (MC_BASE + 0x2E14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1286_REG (MC_BASE + 0x2E18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1287_REG (MC_BASE + 0x2E1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1288_REG (MC_BASE + 0x2E20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1289_REG (MC_BASE + 0x2E24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1290_REG (MC_BASE + 0x2E28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1291_REG (MC_BASE + 0x2E2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1292_REG (MC_BASE + 0x2E30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1293_REG (MC_BASE + 0x2E34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1294_REG (MC_BASE + 0x2E38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1295_REG (MC_BASE + 0x2E3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1296_REG (MC_BASE + 0x2E40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1297_REG (MC_BASE + 0x2E44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1298_REG (MC_BASE + 0x2E48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1299_REG (MC_BASE + 0x2E4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1300_REG (MC_BASE + 0x2E50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1301_REG (MC_BASE + 0x2E54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1302_REG (MC_BASE + 0x2E58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1303_REG (MC_BASE + 0x2E5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1304_REG (MC_BASE + 0x2E60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1305_REG (MC_BASE + 0x2E64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1306_REG (MC_BASE + 0x2E68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1307_REG (MC_BASE + 0x2E6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1308_REG (MC_BASE + 0x2E70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1309_REG (MC_BASE + 0x2E74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1310_REG (MC_BASE + 0x2E78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1311_REG (MC_BASE + 0x2E7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1312_REG (MC_BASE + 0x2E80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1313_REG (MC_BASE + 0x2E84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1314_REG (MC_BASE + 0x2E88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1315_REG (MC_BASE + 0x2E8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1316_REG (MC_BASE + 0x2E90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1317_REG (MC_BASE + 0x2E94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1318_REG (MC_BASE + 0x2E98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1319_REG (MC_BASE + 0x2E9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1320_REG (MC_BASE + 0x2EA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1321_REG (MC_BASE + 0x2EA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1322_REG (MC_BASE + 0x2EA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1323_REG (MC_BASE + 0x2EAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1324_REG (MC_BASE + 0x2EB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1325_REG (MC_BASE + 0x2EB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1326_REG (MC_BASE + 0x2EB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1327_REG (MC_BASE + 0x2EBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1328_REG (MC_BASE + 0x2EC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1329_REG (MC_BASE + 0x2EC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1330_REG (MC_BASE + 0x2EC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1331_REG (MC_BASE + 0x2ECC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1332_REG (MC_BASE + 0x2ED0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1333_REG (MC_BASE + 0x2ED4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1334_REG (MC_BASE + 0x2ED8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1335_REG (MC_BASE + 0x2EDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1336_REG (MC_BASE + 0x2EE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1337_REG (MC_BASE + 0x2EE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1338_REG (MC_BASE + 0x2EE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1339_REG (MC_BASE + 0x2EEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1340_REG (MC_BASE + 0x2EF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1341_REG (MC_BASE + 0x2EF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1342_REG (MC_BASE + 0x2EF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1343_REG (MC_BASE + 0x2EFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1344_REG (MC_BASE + 0x2F00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1345_REG (MC_BASE + 0x2F04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1346_REG (MC_BASE + 0x2F08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1347_REG (MC_BASE + 0x2F0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1348_REG (MC_BASE + 0x2F10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1349_REG (MC_BASE + 0x2F14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1350_REG (MC_BASE + 0x2F18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1351_REG (MC_BASE + 0x2F1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1352_REG (MC_BASE + 0x2F20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1353_REG (MC_BASE + 0x2F24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1354_REG (MC_BASE + 0x2F28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1355_REG (MC_BASE + 0x2F2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1356_REG (MC_BASE + 0x2F30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1357_REG (MC_BASE + 0x2F34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1358_REG (MC_BASE + 0x2F38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1359_REG (MC_BASE + 0x2F3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1360_REG (MC_BASE + 0x2F40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1361_REG (MC_BASE + 0x2F44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1362_REG (MC_BASE + 0x2F48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1363_REG (MC_BASE + 0x2F4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1364_REG (MC_BASE + 0x2F50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1365_REG (MC_BASE + 0x2F54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1366_REG (MC_BASE + 0x2F58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1367_REG (MC_BASE + 0x2F5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1368_REG (MC_BASE + 0x2F60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1369_REG (MC_BASE + 0x2F64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1370_REG (MC_BASE + 0x2F68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1371_REG (MC_BASE + 0x2F6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1372_REG (MC_BASE + 0x2F70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1373_REG (MC_BASE + 0x2F74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1374_REG (MC_BASE + 0x2F78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1375_REG (MC_BASE + 0x2F7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1376_REG (MC_BASE + 0x2F80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1377_REG (MC_BASE + 0x2F84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1378_REG (MC_BASE + 0x2F88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1379_REG (MC_BASE + 0x2F8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1380_REG (MC_BASE + 0x2F90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1381_REG (MC_BASE + 0x2F94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1382_REG (MC_BASE + 0x2F98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1383_REG (MC_BASE + 0x2F9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1384_REG (MC_BASE + 0x2FA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1385_REG (MC_BASE + 0x2FA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1386_REG (MC_BASE + 0x2FA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1387_REG (MC_BASE + 0x2FAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1388_REG (MC_BASE + 0x2FB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1389_REG (MC_BASE + 0x2FB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1390_REG (MC_BASE + 0x2FB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1391_REG (MC_BASE + 0x2FBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1392_REG (MC_BASE + 0x2FC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1393_REG (MC_BASE + 0x2FC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1394_REG (MC_BASE + 0x2FC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1395_REG (MC_BASE + 0x2FCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1396_REG (MC_BASE + 0x2FD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1397_REG (MC_BASE + 0x2FD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1398_REG (MC_BASE + 0x2FD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1399_REG (MC_BASE + 0x2FDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1400_REG (MC_BASE + 0x2FE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1401_REG (MC_BASE + 0x2FE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1402_REG (MC_BASE + 0x2FE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1403_REG (MC_BASE + 0x2FEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1404_REG (MC_BASE + 0x2FF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1405_REG (MC_BASE + 0x2FF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1406_REG (MC_BASE + 0x2FF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1407_REG (MC_BASE + 0x2FFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1408_REG (MC_BASE + 0x3000) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1409_REG (MC_BASE + 0x3004) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1410_REG (MC_BASE + 0x3008) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1411_REG (MC_BASE + 0x300C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1412_REG (MC_BASE + 0x3010) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1413_REG (MC_BASE + 0x3014) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1414_REG (MC_BASE + 0x3018) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1415_REG (MC_BASE + 0x301C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1416_REG (MC_BASE + 0x3020) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1417_REG (MC_BASE + 0x3024) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1418_REG (MC_BASE + 0x3028) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1419_REG (MC_BASE + 0x302C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1420_REG (MC_BASE + 0x3030) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1421_REG (MC_BASE + 0x3034) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1422_REG (MC_BASE + 0x3038) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1423_REG (MC_BASE + 0x303C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1424_REG (MC_BASE + 0x3040) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1425_REG (MC_BASE + 0x3044) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1426_REG (MC_BASE + 0x3048) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1427_REG (MC_BASE + 0x304C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1428_REG (MC_BASE + 0x3050) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1429_REG (MC_BASE + 0x3054) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1430_REG (MC_BASE + 0x3058) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1431_REG (MC_BASE + 0x305C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1432_REG (MC_BASE + 0x3060) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1433_REG (MC_BASE + 0x3064) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1434_REG (MC_BASE + 0x3068) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1435_REG (MC_BASE + 0x306C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1436_REG (MC_BASE + 0x3070) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1437_REG (MC_BASE + 0x3074) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1438_REG (MC_BASE + 0x3078) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1439_REG (MC_BASE + 0x307C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1440_REG (MC_BASE + 0x3080) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1441_REG (MC_BASE + 0x3084) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1442_REG (MC_BASE + 0x3088) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1443_REG (MC_BASE + 0x308C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1444_REG (MC_BASE + 0x3090) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1445_REG (MC_BASE + 0x3094) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1446_REG (MC_BASE + 0x3098) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1447_REG (MC_BASE + 0x309C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1448_REG (MC_BASE + 0x30A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1449_REG (MC_BASE + 0x30A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1450_REG (MC_BASE + 0x30A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1451_REG (MC_BASE + 0x30AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1452_REG (MC_BASE + 0x30B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1453_REG (MC_BASE + 0x30B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1454_REG (MC_BASE + 0x30B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1455_REG (MC_BASE + 0x30BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1456_REG (MC_BASE + 0x30C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1457_REG (MC_BASE + 0x30C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1458_REG (MC_BASE + 0x30C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1459_REG (MC_BASE + 0x30CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1460_REG (MC_BASE + 0x30D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1461_REG (MC_BASE + 0x30D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1462_REG (MC_BASE + 0x30D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1463_REG (MC_BASE + 0x30DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1464_REG (MC_BASE + 0x30E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1465_REG (MC_BASE + 0x30E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1466_REG (MC_BASE + 0x30E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1467_REG (MC_BASE + 0x30EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1468_REG (MC_BASE + 0x30F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1469_REG (MC_BASE + 0x30F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1470_REG (MC_BASE + 0x30F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1471_REG (MC_BASE + 0x30FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1472_REG (MC_BASE + 0x3100) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1473_REG (MC_BASE + 0x3104) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1474_REG (MC_BASE + 0x3108) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1475_REG (MC_BASE + 0x310C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1476_REG (MC_BASE + 0x3110) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1477_REG (MC_BASE + 0x3114) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1478_REG (MC_BASE + 0x3118) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1479_REG (MC_BASE + 0x311C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1480_REG (MC_BASE + 0x3120) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1481_REG (MC_BASE + 0x3124) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1482_REG (MC_BASE + 0x3128) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1483_REG (MC_BASE + 0x312C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1484_REG (MC_BASE + 0x3130) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1485_REG (MC_BASE + 0x3134) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1486_REG (MC_BASE + 0x3138) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1487_REG (MC_BASE + 0x313C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1488_REG (MC_BASE + 0x3140) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1489_REG (MC_BASE + 0x3144) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1490_REG (MC_BASE + 0x3148) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1491_REG (MC_BASE + 0x314C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1492_REG (MC_BASE + 0x3150) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1493_REG (MC_BASE + 0x3154) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1494_REG (MC_BASE + 0x3158) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1495_REG (MC_BASE + 0x315C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1496_REG (MC_BASE + 0x3160) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1497_REG (MC_BASE + 0x3164) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1498_REG (MC_BASE + 0x3168) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1499_REG (MC_BASE + 0x316C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1500_REG (MC_BASE + 0x3170) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1501_REG (MC_BASE + 0x3174) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1502_REG (MC_BASE + 0x3178) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1503_REG (MC_BASE + 0x317C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1504_REG (MC_BASE + 0x3180) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1505_REG (MC_BASE + 0x3184) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1506_REG (MC_BASE + 0x3188) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1507_REG (MC_BASE + 0x318C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1508_REG (MC_BASE + 0x3190) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1509_REG (MC_BASE + 0x3194) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1510_REG (MC_BASE + 0x3198) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1511_REG (MC_BASE + 0x319C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1512_REG (MC_BASE + 0x31A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1513_REG (MC_BASE + 0x31A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1514_REG (MC_BASE + 0x31A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1515_REG (MC_BASE + 0x31AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1516_REG (MC_BASE + 0x31B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1517_REG (MC_BASE + 0x31B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1518_REG (MC_BASE + 0x31B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1519_REG (MC_BASE + 0x31BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1520_REG (MC_BASE + 0x31C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1521_REG (MC_BASE + 0x31C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1522_REG (MC_BASE + 0x31C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1523_REG (MC_BASE + 0x31CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1524_REG (MC_BASE + 0x31D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1525_REG (MC_BASE + 0x31D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1526_REG (MC_BASE + 0x31D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1527_REG (MC_BASE + 0x31DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1528_REG (MC_BASE + 0x31E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1529_REG (MC_BASE + 0x31E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1530_REG (MC_BASE + 0x31E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1531_REG (MC_BASE + 0x31EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1532_REG (MC_BASE + 0x31F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1533_REG (MC_BASE + 0x31F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1534_REG (MC_BASE + 0x31F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1535_REG (MC_BASE + 0x31FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1536_REG (MC_BASE + 0x3200) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1537_REG (MC_BASE + 0x3204) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1538_REG (MC_BASE + 0x3208) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1539_REG (MC_BASE + 0x320C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1540_REG (MC_BASE + 0x3210) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1541_REG (MC_BASE + 0x3214) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1542_REG (MC_BASE + 0x3218) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1543_REG (MC_BASE + 0x321C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1544_REG (MC_BASE + 0x3220) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1545_REG (MC_BASE + 0x3224) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1546_REG (MC_BASE + 0x3228) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1547_REG (MC_BASE + 0x322C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1548_REG (MC_BASE + 0x3230) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1549_REG (MC_BASE + 0x3234) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1550_REG (MC_BASE + 0x3238) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1551_REG (MC_BASE + 0x323C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1552_REG (MC_BASE + 0x3240) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1553_REG (MC_BASE + 0x3244) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1554_REG (MC_BASE + 0x3248) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1555_REG (MC_BASE + 0x324C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1556_REG (MC_BASE + 0x3250) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1557_REG (MC_BASE + 0x3254) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1558_REG (MC_BASE + 0x3258) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1559_REG (MC_BASE + 0x325C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1560_REG (MC_BASE + 0x3260) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1561_REG (MC_BASE + 0x3264) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1562_REG (MC_BASE + 0x3268) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1563_REG (MC_BASE + 0x326C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1564_REG (MC_BASE + 0x3270) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1565_REG (MC_BASE + 0x3274) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1566_REG (MC_BASE + 0x3278) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1567_REG (MC_BASE + 0x327C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1568_REG (MC_BASE + 0x3280) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1569_REG (MC_BASE + 0x3284) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1570_REG (MC_BASE + 0x3288) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1571_REG (MC_BASE + 0x328C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1572_REG (MC_BASE + 0x3290) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1573_REG (MC_BASE + 0x3294) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1574_REG (MC_BASE + 0x3298) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1575_REG (MC_BASE + 0x329C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1576_REG (MC_BASE + 0x32A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1577_REG (MC_BASE + 0x32A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1578_REG (MC_BASE + 0x32A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1579_REG (MC_BASE + 0x32AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1580_REG (MC_BASE + 0x32B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1581_REG (MC_BASE + 0x32B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1582_REG (MC_BASE + 0x32B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1583_REG (MC_BASE + 0x32BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1584_REG (MC_BASE + 0x32C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1585_REG (MC_BASE + 0x32C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1586_REG (MC_BASE + 0x32C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1587_REG (MC_BASE + 0x32CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1588_REG (MC_BASE + 0x32D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1589_REG (MC_BASE + 0x32D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1590_REG (MC_BASE + 0x32D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1591_REG (MC_BASE + 0x32DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1592_REG (MC_BASE + 0x32E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1593_REG (MC_BASE + 0x32E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1594_REG (MC_BASE + 0x32E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1595_REG (MC_BASE + 0x32EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1596_REG (MC_BASE + 0x32F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1597_REG (MC_BASE + 0x32F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1598_REG (MC_BASE + 0x32F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1599_REG (MC_BASE + 0x32FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1600_REG (MC_BASE + 0x3300) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1601_REG (MC_BASE + 0x3304) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1602_REG (MC_BASE + 0x3308) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1603_REG (MC_BASE + 0x330C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1604_REG (MC_BASE + 0x3310) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1605_REG (MC_BASE + 0x3314) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1606_REG (MC_BASE + 0x3318) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1607_REG (MC_BASE + 0x331C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1608_REG (MC_BASE + 0x3320) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1609_REG (MC_BASE + 0x3324) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1610_REG (MC_BASE + 0x3328) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1611_REG (MC_BASE + 0x332C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1612_REG (MC_BASE + 0x3330) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1613_REG (MC_BASE + 0x3334) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1614_REG (MC_BASE + 0x3338) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1615_REG (MC_BASE + 0x333C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1616_REG (MC_BASE + 0x3340) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1617_REG (MC_BASE + 0x3344) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1618_REG (MC_BASE + 0x3348) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1619_REG (MC_BASE + 0x334C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1620_REG (MC_BASE + 0x3350) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1621_REG (MC_BASE + 0x3354) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1622_REG (MC_BASE + 0x3358) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1623_REG (MC_BASE + 0x335C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1624_REG (MC_BASE + 0x3360) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1625_REG (MC_BASE + 0x3364) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1626_REG (MC_BASE + 0x3368) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1627_REG (MC_BASE + 0x336C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1628_REG (MC_BASE + 0x3370) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1629_REG (MC_BASE + 0x3374) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1630_REG (MC_BASE + 0x3378) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1631_REG (MC_BASE + 0x337C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1632_REG (MC_BASE + 0x3380) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1633_REG (MC_BASE + 0x3384) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1634_REG (MC_BASE + 0x3388) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1635_REG (MC_BASE + 0x338C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1636_REG (MC_BASE + 0x3390) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1637_REG (MC_BASE + 0x3394) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1638_REG (MC_BASE + 0x3398) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1639_REG (MC_BASE + 0x339C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1640_REG (MC_BASE + 0x33A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1641_REG (MC_BASE + 0x33A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1642_REG (MC_BASE + 0x33A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1643_REG (MC_BASE + 0x33AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1644_REG (MC_BASE + 0x33B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1645_REG (MC_BASE + 0x33B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1646_REG (MC_BASE + 0x33B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1647_REG (MC_BASE + 0x33BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1648_REG (MC_BASE + 0x33C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1649_REG (MC_BASE + 0x33C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1650_REG (MC_BASE + 0x33C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1651_REG (MC_BASE + 0x33CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1652_REG (MC_BASE + 0x33D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1653_REG (MC_BASE + 0x33D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1654_REG (MC_BASE + 0x33D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1655_REG (MC_BASE + 0x33DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1656_REG (MC_BASE + 0x33E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1657_REG (MC_BASE + 0x33E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1658_REG (MC_BASE + 0x33E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1659_REG (MC_BASE + 0x33EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1660_REG (MC_BASE + 0x33F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1661_REG (MC_BASE + 0x33F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1662_REG (MC_BASE + 0x33F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1663_REG (MC_BASE + 0x33FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1664_REG (MC_BASE + 0x3400) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1665_REG (MC_BASE + 0x3404) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1666_REG (MC_BASE + 0x3408) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1667_REG (MC_BASE + 0x340C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1668_REG (MC_BASE + 0x3410) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1669_REG (MC_BASE + 0x3414) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1670_REG (MC_BASE + 0x3418) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1671_REG (MC_BASE + 0x341C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1672_REG (MC_BASE + 0x3420) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1673_REG (MC_BASE + 0x3424) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1674_REG (MC_BASE + 0x3428) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1675_REG (MC_BASE + 0x342C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1676_REG (MC_BASE + 0x3430) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1677_REG (MC_BASE + 0x3434) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1678_REG (MC_BASE + 0x3438) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1679_REG (MC_BASE + 0x343C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1680_REG (MC_BASE + 0x3440) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1681_REG (MC_BASE + 0x3444) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1682_REG (MC_BASE + 0x3448) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1683_REG (MC_BASE + 0x344C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1684_REG (MC_BASE + 0x3450) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1685_REG (MC_BASE + 0x3454) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1686_REG (MC_BASE + 0x3458) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1687_REG (MC_BASE + 0x345C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1688_REG (MC_BASE + 0x3460) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1689_REG (MC_BASE + 0x3464) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1690_REG (MC_BASE + 0x3468) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1691_REG (MC_BASE + 0x346C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1692_REG (MC_BASE + 0x3470) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1693_REG (MC_BASE + 0x3474) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1694_REG (MC_BASE + 0x3478) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1695_REG (MC_BASE + 0x347C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1696_REG (MC_BASE + 0x3480) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1697_REG (MC_BASE + 0x3484) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1698_REG (MC_BASE + 0x3488) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1699_REG (MC_BASE + 0x348C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1700_REG (MC_BASE + 0x3490) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1701_REG (MC_BASE + 0x3494) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1702_REG (MC_BASE + 0x3498) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1703_REG (MC_BASE + 0x349C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1704_REG (MC_BASE + 0x34A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1705_REG (MC_BASE + 0x34A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1706_REG (MC_BASE + 0x34A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1707_REG (MC_BASE + 0x34AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1708_REG (MC_BASE + 0x34B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1709_REG (MC_BASE + 0x34B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1710_REG (MC_BASE + 0x34B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1711_REG (MC_BASE + 0x34BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1712_REG (MC_BASE + 0x34C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1713_REG (MC_BASE + 0x34C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1714_REG (MC_BASE + 0x34C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1715_REG (MC_BASE + 0x34CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1716_REG (MC_BASE + 0x34D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1717_REG (MC_BASE + 0x34D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1718_REG (MC_BASE + 0x34D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1719_REG (MC_BASE + 0x34DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1720_REG (MC_BASE + 0x34E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1721_REG (MC_BASE + 0x34E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1722_REG (MC_BASE + 0x34E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1723_REG (MC_BASE + 0x34EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1724_REG (MC_BASE + 0x34F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1725_REG (MC_BASE + 0x34F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1726_REG (MC_BASE + 0x34F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1727_REG (MC_BASE + 0x34FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1728_REG (MC_BASE + 0x3500) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1729_REG (MC_BASE + 0x3504) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1730_REG (MC_BASE + 0x3508) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1731_REG (MC_BASE + 0x350C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1732_REG (MC_BASE + 0x3510) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1733_REG (MC_BASE + 0x3514) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1734_REG (MC_BASE + 0x3518) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1735_REG (MC_BASE + 0x351C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1736_REG (MC_BASE + 0x3520) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1737_REG (MC_BASE + 0x3524) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1738_REG (MC_BASE + 0x3528) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1739_REG (MC_BASE + 0x352C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1740_REG (MC_BASE + 0x3530) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1741_REG (MC_BASE + 0x3534) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1742_REG (MC_BASE + 0x3538) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1743_REG (MC_BASE + 0x353C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1744_REG (MC_BASE + 0x3540) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1745_REG (MC_BASE + 0x3544) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1746_REG (MC_BASE + 0x3548) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1747_REG (MC_BASE + 0x354C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1748_REG (MC_BASE + 0x3550) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1749_REG (MC_BASE + 0x3554) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1750_REG (MC_BASE + 0x3558) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1751_REG (MC_BASE + 0x355C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1752_REG (MC_BASE + 0x3560) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1753_REG (MC_BASE + 0x3564) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1754_REG (MC_BASE + 0x3568) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1755_REG (MC_BASE + 0x356C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1756_REG (MC_BASE + 0x3570) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1757_REG (MC_BASE + 0x3574) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1758_REG (MC_BASE + 0x3578) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1759_REG (MC_BASE + 0x357C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1760_REG (MC_BASE + 0x3580) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1761_REG (MC_BASE + 0x3584) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1762_REG (MC_BASE + 0x3588) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1763_REG (MC_BASE + 0x358C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1764_REG (MC_BASE + 0x3590) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1765_REG (MC_BASE + 0x3594) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1766_REG (MC_BASE + 0x3598) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1767_REG (MC_BASE + 0x359C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1768_REG (MC_BASE + 0x35A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1769_REG (MC_BASE + 0x35A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1770_REG (MC_BASE + 0x35A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1771_REG (MC_BASE + 0x35AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1772_REG (MC_BASE + 0x35B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1773_REG (MC_BASE + 0x35B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1774_REG (MC_BASE + 0x35B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1775_REG (MC_BASE + 0x35BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1776_REG (MC_BASE + 0x35C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1777_REG (MC_BASE + 0x35C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1778_REG (MC_BASE + 0x35C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1779_REG (MC_BASE + 0x35CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1780_REG (MC_BASE + 0x35D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1781_REG (MC_BASE + 0x35D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1782_REG (MC_BASE + 0x35D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1783_REG (MC_BASE + 0x35DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1784_REG (MC_BASE + 0x35E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1785_REG (MC_BASE + 0x35E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1786_REG (MC_BASE + 0x35E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1787_REG (MC_BASE + 0x35EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1788_REG (MC_BASE + 0x35F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1789_REG (MC_BASE + 0x35F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1790_REG (MC_BASE + 0x35F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1791_REG (MC_BASE + 0x35FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1792_REG (MC_BASE + 0x3600) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1793_REG (MC_BASE + 0x3604) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1794_REG (MC_BASE + 0x3608) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1795_REG (MC_BASE + 0x360C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1796_REG (MC_BASE + 0x3610) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1797_REG (MC_BASE + 0x3614) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1798_REG (MC_BASE + 0x3618) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1799_REG (MC_BASE + 0x361C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1800_REG (MC_BASE + 0x3620) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1801_REG (MC_BASE + 0x3624) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1802_REG (MC_BASE + 0x3628) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1803_REG (MC_BASE + 0x362C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1804_REG (MC_BASE + 0x3630) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1805_REG (MC_BASE + 0x3634) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1806_REG (MC_BASE + 0x3638) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1807_REG (MC_BASE + 0x363C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1808_REG (MC_BASE + 0x3640) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1809_REG (MC_BASE + 0x3644) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1810_REG (MC_BASE + 0x3648) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1811_REG (MC_BASE + 0x364C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1812_REG (MC_BASE + 0x3650) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1813_REG (MC_BASE + 0x3654) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1814_REG (MC_BASE + 0x3658) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1815_REG (MC_BASE + 0x365C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1816_REG (MC_BASE + 0x3660) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1817_REG (MC_BASE + 0x3664) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1818_REG (MC_BASE + 0x3668) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1819_REG (MC_BASE + 0x366C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1820_REG (MC_BASE + 0x3670) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1821_REG (MC_BASE + 0x3674) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1822_REG (MC_BASE + 0x3678) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1823_REG (MC_BASE + 0x367C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1824_REG (MC_BASE + 0x3680) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1825_REG (MC_BASE + 0x3684) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1826_REG (MC_BASE + 0x3688) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1827_REG (MC_BASE + 0x368C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1828_REG (MC_BASE + 0x3690) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1829_REG (MC_BASE + 0x3694) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1830_REG (MC_BASE + 0x3698) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1831_REG (MC_BASE + 0x369C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1832_REG (MC_BASE + 0x36A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1833_REG (MC_BASE + 0x36A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1834_REG (MC_BASE + 0x36A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1835_REG (MC_BASE + 0x36AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1836_REG (MC_BASE + 0x36B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1837_REG (MC_BASE + 0x36B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1838_REG (MC_BASE + 0x36B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1839_REG (MC_BASE + 0x36BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1840_REG (MC_BASE + 0x36C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1841_REG (MC_BASE + 0x36C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1842_REG (MC_BASE + 0x36C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1843_REG (MC_BASE + 0x36CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1844_REG (MC_BASE + 0x36D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1845_REG (MC_BASE + 0x36D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1846_REG (MC_BASE + 0x36D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1847_REG (MC_BASE + 0x36DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1848_REG (MC_BASE + 0x36E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1849_REG (MC_BASE + 0x36E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1850_REG (MC_BASE + 0x36E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1851_REG (MC_BASE + 0x36EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1852_REG (MC_BASE + 0x36F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1853_REG (MC_BASE + 0x36F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1854_REG (MC_BASE + 0x36F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1855_REG (MC_BASE + 0x36FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1856_REG (MC_BASE + 0x3700) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1857_REG (MC_BASE + 0x3704) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1858_REG (MC_BASE + 0x3708) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1859_REG (MC_BASE + 0x370C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1860_REG (MC_BASE + 0x3710) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1861_REG (MC_BASE + 0x3714) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1862_REG (MC_BASE + 0x3718) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1863_REG (MC_BASE + 0x371C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1864_REG (MC_BASE + 0x3720) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1865_REG (MC_BASE + 0x3724) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1866_REG (MC_BASE + 0x3728) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1867_REG (MC_BASE + 0x372C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1868_REG (MC_BASE + 0x3730) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1869_REG (MC_BASE + 0x3734) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1870_REG (MC_BASE + 0x3738) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1871_REG (MC_BASE + 0x373C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1872_REG (MC_BASE + 0x3740) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1873_REG (MC_BASE + 0x3744) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1874_REG (MC_BASE + 0x3748) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1875_REG (MC_BASE + 0x374C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1876_REG (MC_BASE + 0x3750) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1877_REG (MC_BASE + 0x3754) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1878_REG (MC_BASE + 0x3758) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1879_REG (MC_BASE + 0x375C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1880_REG (MC_BASE + 0x3760) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1881_REG (MC_BASE + 0x3764) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1882_REG (MC_BASE + 0x3768) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1883_REG (MC_BASE + 0x376C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1884_REG (MC_BASE + 0x3770) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1885_REG (MC_BASE + 0x3774) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1886_REG (MC_BASE + 0x3778) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1887_REG (MC_BASE + 0x377C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1888_REG (MC_BASE + 0x3780) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1889_REG (MC_BASE + 0x3784) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1890_REG (MC_BASE + 0x3788) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1891_REG (MC_BASE + 0x378C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1892_REG (MC_BASE + 0x3790) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1893_REG (MC_BASE + 0x3794) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1894_REG (MC_BASE + 0x3798) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1895_REG (MC_BASE + 0x379C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1896_REG (MC_BASE + 0x37A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1897_REG (MC_BASE + 0x37A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1898_REG (MC_BASE + 0x37A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1899_REG (MC_BASE + 0x37AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1900_REG (MC_BASE + 0x37B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1901_REG (MC_BASE + 0x37B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1902_REG (MC_BASE + 0x37B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1903_REG (MC_BASE + 0x37BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1904_REG (MC_BASE + 0x37C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1905_REG (MC_BASE + 0x37C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1906_REG (MC_BASE + 0x37C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1907_REG (MC_BASE + 0x37CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1908_REG (MC_BASE + 0x37D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1909_REG (MC_BASE + 0x37D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1910_REG (MC_BASE + 0x37D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1911_REG (MC_BASE + 0x37DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1912_REG (MC_BASE + 0x37E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1913_REG (MC_BASE + 0x37E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1914_REG (MC_BASE + 0x37E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1915_REG (MC_BASE + 0x37EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1916_REG (MC_BASE + 0x37F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1917_REG (MC_BASE + 0x37F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1918_REG (MC_BASE + 0x37F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1919_REG (MC_BASE + 0x37FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1920_REG (MC_BASE + 0x3800) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1921_REG (MC_BASE + 0x3804) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1922_REG (MC_BASE + 0x3808) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1923_REG (MC_BASE + 0x380C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1924_REG (MC_BASE + 0x3810) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1925_REG (MC_BASE + 0x3814) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1926_REG (MC_BASE + 0x3818) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1927_REG (MC_BASE + 0x381C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1928_REG (MC_BASE + 0x3820) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1929_REG (MC_BASE + 0x3824) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1930_REG (MC_BASE + 0x3828) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1931_REG (MC_BASE + 0x382C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1932_REG (MC_BASE + 0x3830) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1933_REG (MC_BASE + 0x3834) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1934_REG (MC_BASE + 0x3838) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1935_REG (MC_BASE + 0x383C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1936_REG (MC_BASE + 0x3840) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1937_REG (MC_BASE + 0x3844) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1938_REG (MC_BASE + 0x3848) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1939_REG (MC_BASE + 0x384C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1940_REG (MC_BASE + 0x3850) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1941_REG (MC_BASE + 0x3854) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1942_REG (MC_BASE + 0x3858) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1943_REG (MC_BASE + 0x385C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1944_REG (MC_BASE + 0x3860) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1945_REG (MC_BASE + 0x3864) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1946_REG (MC_BASE + 0x3868) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1947_REG (MC_BASE + 0x386C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1948_REG (MC_BASE + 0x3870) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1949_REG (MC_BASE + 0x3874) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1950_REG (MC_BASE + 0x3878) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1951_REG (MC_BASE + 0x387C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1952_REG (MC_BASE + 0x3880) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1953_REG (MC_BASE + 0x3884) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1954_REG (MC_BASE + 0x3888) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1955_REG (MC_BASE + 0x388C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1956_REG (MC_BASE + 0x3890) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1957_REG (MC_BASE + 0x3894) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1958_REG (MC_BASE + 0x3898) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1959_REG (MC_BASE + 0x389C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1960_REG (MC_BASE + 0x38A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1961_REG (MC_BASE + 0x38A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1962_REG (MC_BASE + 0x38A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1963_REG (MC_BASE + 0x38AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1964_REG (MC_BASE + 0x38B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1965_REG (MC_BASE + 0x38B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1966_REG (MC_BASE + 0x38B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1967_REG (MC_BASE + 0x38BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1968_REG (MC_BASE + 0x38C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1969_REG (MC_BASE + 0x38C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1970_REG (MC_BASE + 0x38C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1971_REG (MC_BASE + 0x38CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1972_REG (MC_BASE + 0x38D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1973_REG (MC_BASE + 0x38D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1974_REG (MC_BASE + 0x38D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1975_REG (MC_BASE + 0x38DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1976_REG (MC_BASE + 0x38E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1977_REG (MC_BASE + 0x38E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1978_REG (MC_BASE + 0x38E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1979_REG (MC_BASE + 0x38EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1980_REG (MC_BASE + 0x38F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1981_REG (MC_BASE + 0x38F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1982_REG (MC_BASE + 0x38F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1983_REG (MC_BASE + 0x38FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1984_REG (MC_BASE + 0x3900) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1985_REG (MC_BASE + 0x3904) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1986_REG (MC_BASE + 0x3908) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1987_REG (MC_BASE + 0x390C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1988_REG (MC_BASE + 0x3910) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1989_REG (MC_BASE + 0x3914) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1990_REG (MC_BASE + 0x3918) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1991_REG (MC_BASE + 0x391C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1992_REG (MC_BASE + 0x3920) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1993_REG (MC_BASE + 0x3924) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1994_REG (MC_BASE + 0x3928) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1995_REG (MC_BASE + 0x392C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1996_REG (MC_BASE + 0x3930) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1997_REG (MC_BASE + 0x3934) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1998_REG (MC_BASE + 0x3938) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_1999_REG (MC_BASE + 0x393C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2000_REG (MC_BASE + 0x3940) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2001_REG (MC_BASE + 0x3944) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2002_REG (MC_BASE + 0x3948) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2003_REG (MC_BASE + 0x394C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2004_REG (MC_BASE + 0x3950) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2005_REG (MC_BASE + 0x3954) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2006_REG (MC_BASE + 0x3958) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2007_REG (MC_BASE + 0x395C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2008_REG (MC_BASE + 0x3960) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2009_REG (MC_BASE + 0x3964) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2010_REG (MC_BASE + 0x3968) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2011_REG (MC_BASE + 0x396C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2012_REG (MC_BASE + 0x3970) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2013_REG (MC_BASE + 0x3974) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2014_REG (MC_BASE + 0x3978) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2015_REG (MC_BASE + 0x397C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2016_REG (MC_BASE + 0x3980) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2017_REG (MC_BASE + 0x3984) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2018_REG (MC_BASE + 0x3988) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2019_REG (MC_BASE + 0x398C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2020_REG (MC_BASE + 0x3990) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2021_REG (MC_BASE + 0x3994) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2022_REG (MC_BASE + 0x3998) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2023_REG (MC_BASE + 0x399C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2024_REG (MC_BASE + 0x39A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2025_REG (MC_BASE + 0x39A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2026_REG (MC_BASE + 0x39A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2027_REG (MC_BASE + 0x39AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2028_REG (MC_BASE + 0x39B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2029_REG (MC_BASE + 0x39B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2030_REG (MC_BASE + 0x39B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2031_REG (MC_BASE + 0x39BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2032_REG (MC_BASE + 0x39C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2033_REG (MC_BASE + 0x39C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2034_REG (MC_BASE + 0x39C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2035_REG (MC_BASE + 0x39CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2036_REG (MC_BASE + 0x39D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2037_REG (MC_BASE + 0x39D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2038_REG (MC_BASE + 0x39D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2039_REG (MC_BASE + 0x39DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2040_REG (MC_BASE + 0x39E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2041_REG (MC_BASE + 0x39E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2042_REG (MC_BASE + 0x39E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2043_REG (MC_BASE + 0x39EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2044_REG (MC_BASE + 0x39F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2045_REG (MC_BASE + 0x39F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2046_REG (MC_BASE + 0x39F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2047_REG (MC_BASE + 0x39FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2048_REG (MC_BASE + 0x3A00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2049_REG (MC_BASE + 0x3A04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2050_REG (MC_BASE + 0x3A08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2051_REG (MC_BASE + 0x3A0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2052_REG (MC_BASE + 0x3A10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2053_REG (MC_BASE + 0x3A14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2054_REG (MC_BASE + 0x3A18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2055_REG (MC_BASE + 0x3A1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2056_REG (MC_BASE + 0x3A20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2057_REG (MC_BASE + 0x3A24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2058_REG (MC_BASE + 0x3A28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2059_REG (MC_BASE + 0x3A2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2060_REG (MC_BASE + 0x3A30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2061_REG (MC_BASE + 0x3A34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2062_REG (MC_BASE + 0x3A38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2063_REG (MC_BASE + 0x3A3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2064_REG (MC_BASE + 0x3A40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2065_REG (MC_BASE + 0x3A44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2066_REG (MC_BASE + 0x3A48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2067_REG (MC_BASE + 0x3A4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2068_REG (MC_BASE + 0x3A50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2069_REG (MC_BASE + 0x3A54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2070_REG (MC_BASE + 0x3A58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2071_REG (MC_BASE + 0x3A5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2072_REG (MC_BASE + 0x3A60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2073_REG (MC_BASE + 0x3A64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2074_REG (MC_BASE + 0x3A68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2075_REG (MC_BASE + 0x3A6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2076_REG (MC_BASE + 0x3A70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2077_REG (MC_BASE + 0x3A74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2078_REG (MC_BASE + 0x3A78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2079_REG (MC_BASE + 0x3A7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2080_REG (MC_BASE + 0x3A80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2081_REG (MC_BASE + 0x3A84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2082_REG (MC_BASE + 0x3A88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2083_REG (MC_BASE + 0x3A8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2084_REG (MC_BASE + 0x3A90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2085_REG (MC_BASE + 0x3A94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2086_REG (MC_BASE + 0x3A98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2087_REG (MC_BASE + 0x3A9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2088_REG (MC_BASE + 0x3AA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2089_REG (MC_BASE + 0x3AA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2090_REG (MC_BASE + 0x3AA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2091_REG (MC_BASE + 0x3AAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2092_REG (MC_BASE + 0x3AB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2093_REG (MC_BASE + 0x3AB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2094_REG (MC_BASE + 0x3AB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2095_REG (MC_BASE + 0x3ABC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2096_REG (MC_BASE + 0x3AC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2097_REG (MC_BASE + 0x3AC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2098_REG (MC_BASE + 0x3AC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2099_REG (MC_BASE + 0x3ACC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2100_REG (MC_BASE + 0x3AD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2101_REG (MC_BASE + 0x3AD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2102_REG (MC_BASE + 0x3AD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2103_REG (MC_BASE + 0x3ADC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2104_REG (MC_BASE + 0x3AE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2105_REG (MC_BASE + 0x3AE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2106_REG (MC_BASE + 0x3AE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2107_REG (MC_BASE + 0x3AEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2108_REG (MC_BASE + 0x3AF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2109_REG (MC_BASE + 0x3AF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2110_REG (MC_BASE + 0x3AF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2111_REG (MC_BASE + 0x3AFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2112_REG (MC_BASE + 0x3B00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2113_REG (MC_BASE + 0x3B04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2114_REG (MC_BASE + 0x3B08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2115_REG (MC_BASE + 0x3B0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2116_REG (MC_BASE + 0x3B10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2117_REG (MC_BASE + 0x3B14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2118_REG (MC_BASE + 0x3B18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2119_REG (MC_BASE + 0x3B1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2120_REG (MC_BASE + 0x3B20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2121_REG (MC_BASE + 0x3B24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2122_REG (MC_BASE + 0x3B28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2123_REG (MC_BASE + 0x3B2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2124_REG (MC_BASE + 0x3B30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2125_REG (MC_BASE + 0x3B34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2126_REG (MC_BASE + 0x3B38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2127_REG (MC_BASE + 0x3B3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2128_REG (MC_BASE + 0x3B40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2129_REG (MC_BASE + 0x3B44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2130_REG (MC_BASE + 0x3B48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2131_REG (MC_BASE + 0x3B4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2132_REG (MC_BASE + 0x3B50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2133_REG (MC_BASE + 0x3B54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2134_REG (MC_BASE + 0x3B58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2135_REG (MC_BASE + 0x3B5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2136_REG (MC_BASE + 0x3B60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2137_REG (MC_BASE + 0x3B64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2138_REG (MC_BASE + 0x3B68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2139_REG (MC_BASE + 0x3B6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2140_REG (MC_BASE + 0x3B70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2141_REG (MC_BASE + 0x3B74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2142_REG (MC_BASE + 0x3B78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2143_REG (MC_BASE + 0x3B7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2144_REG (MC_BASE + 0x3B80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2145_REG (MC_BASE + 0x3B84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2146_REG (MC_BASE + 0x3B88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2147_REG (MC_BASE + 0x3B8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2148_REG (MC_BASE + 0x3B90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2149_REG (MC_BASE + 0x3B94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2150_REG (MC_BASE + 0x3B98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2151_REG (MC_BASE + 0x3B9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2152_REG (MC_BASE + 0x3BA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2153_REG (MC_BASE + 0x3BA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2154_REG (MC_BASE + 0x3BA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2155_REG (MC_BASE + 0x3BAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2156_REG (MC_BASE + 0x3BB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2157_REG (MC_BASE + 0x3BB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2158_REG (MC_BASE + 0x3BB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2159_REG (MC_BASE + 0x3BBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2160_REG (MC_BASE + 0x3BC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2161_REG (MC_BASE + 0x3BC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2162_REG (MC_BASE + 0x3BC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2163_REG (MC_BASE + 0x3BCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2164_REG (MC_BASE + 0x3BD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2165_REG (MC_BASE + 0x3BD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2166_REG (MC_BASE + 0x3BD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2167_REG (MC_BASE + 0x3BDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2168_REG (MC_BASE + 0x3BE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2169_REG (MC_BASE + 0x3BE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2170_REG (MC_BASE + 0x3BE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2171_REG (MC_BASE + 0x3BEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2172_REG (MC_BASE + 0x3BF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2173_REG (MC_BASE + 0x3BF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2174_REG (MC_BASE + 0x3BF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2175_REG (MC_BASE + 0x3BFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2176_REG (MC_BASE + 0x3C00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2177_REG (MC_BASE + 0x3C04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2178_REG (MC_BASE + 0x3C08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2179_REG (MC_BASE + 0x3C0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2180_REG (MC_BASE + 0x3C10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2181_REG (MC_BASE + 0x3C14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2182_REG (MC_BASE + 0x3C18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2183_REG (MC_BASE + 0x3C1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2184_REG (MC_BASE + 0x3C20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2185_REG (MC_BASE + 0x3C24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2186_REG (MC_BASE + 0x3C28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2187_REG (MC_BASE + 0x3C2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2188_REG (MC_BASE + 0x3C30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2189_REG (MC_BASE + 0x3C34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2190_REG (MC_BASE + 0x3C38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2191_REG (MC_BASE + 0x3C3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2192_REG (MC_BASE + 0x3C40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2193_REG (MC_BASE + 0x3C44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2194_REG (MC_BASE + 0x3C48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2195_REG (MC_BASE + 0x3C4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2196_REG (MC_BASE + 0x3C50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2197_REG (MC_BASE + 0x3C54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2198_REG (MC_BASE + 0x3C58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2199_REG (MC_BASE + 0x3C5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2200_REG (MC_BASE + 0x3C60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2201_REG (MC_BASE + 0x3C64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2202_REG (MC_BASE + 0x3C68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2203_REG (MC_BASE + 0x3C6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2204_REG (MC_BASE + 0x3C70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2205_REG (MC_BASE + 0x3C74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2206_REG (MC_BASE + 0x3C78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2207_REG (MC_BASE + 0x3C7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2208_REG (MC_BASE + 0x3C80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2209_REG (MC_BASE + 0x3C84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2210_REG (MC_BASE + 0x3C88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2211_REG (MC_BASE + 0x3C8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2212_REG (MC_BASE + 0x3C90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2213_REG (MC_BASE + 0x3C94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2214_REG (MC_BASE + 0x3C98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2215_REG (MC_BASE + 0x3C9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2216_REG (MC_BASE + 0x3CA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2217_REG (MC_BASE + 0x3CA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2218_REG (MC_BASE + 0x3CA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2219_REG (MC_BASE + 0x3CAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2220_REG (MC_BASE + 0x3CB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2221_REG (MC_BASE + 0x3CB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2222_REG (MC_BASE + 0x3CB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2223_REG (MC_BASE + 0x3CBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2224_REG (MC_BASE + 0x3CC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2225_REG (MC_BASE + 0x3CC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2226_REG (MC_BASE + 0x3CC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2227_REG (MC_BASE + 0x3CCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2228_REG (MC_BASE + 0x3CD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2229_REG (MC_BASE + 0x3CD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2230_REG (MC_BASE + 0x3CD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2231_REG (MC_BASE + 0x3CDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2232_REG (MC_BASE + 0x3CE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2233_REG (MC_BASE + 0x3CE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2234_REG (MC_BASE + 0x3CE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2235_REG (MC_BASE + 0x3CEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2236_REG (MC_BASE + 0x3CF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2237_REG (MC_BASE + 0x3CF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2238_REG (MC_BASE + 0x3CF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2239_REG (MC_BASE + 0x3CFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2240_REG (MC_BASE + 0x3D00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2241_REG (MC_BASE + 0x3D04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2242_REG (MC_BASE + 0x3D08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2243_REG (MC_BASE + 0x3D0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2244_REG (MC_BASE + 0x3D10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2245_REG (MC_BASE + 0x3D14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2246_REG (MC_BASE + 0x3D18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2247_REG (MC_BASE + 0x3D1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2248_REG (MC_BASE + 0x3D20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2249_REG (MC_BASE + 0x3D24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2250_REG (MC_BASE + 0x3D28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2251_REG (MC_BASE + 0x3D2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2252_REG (MC_BASE + 0x3D30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2253_REG (MC_BASE + 0x3D34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2254_REG (MC_BASE + 0x3D38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2255_REG (MC_BASE + 0x3D3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2256_REG (MC_BASE + 0x3D40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2257_REG (MC_BASE + 0x3D44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2258_REG (MC_BASE + 0x3D48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2259_REG (MC_BASE + 0x3D4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2260_REG (MC_BASE + 0x3D50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2261_REG (MC_BASE + 0x3D54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2262_REG (MC_BASE + 0x3D58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2263_REG (MC_BASE + 0x3D5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2264_REG (MC_BASE + 0x3D60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2265_REG (MC_BASE + 0x3D64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2266_REG (MC_BASE + 0x3D68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2267_REG (MC_BASE + 0x3D6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2268_REG (MC_BASE + 0x3D70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2269_REG (MC_BASE + 0x3D74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2270_REG (MC_BASE + 0x3D78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2271_REG (MC_BASE + 0x3D7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2272_REG (MC_BASE + 0x3D80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2273_REG (MC_BASE + 0x3D84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2274_REG (MC_BASE + 0x3D88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2275_REG (MC_BASE + 0x3D8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2276_REG (MC_BASE + 0x3D90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2277_REG (MC_BASE + 0x3D94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2278_REG (MC_BASE + 0x3D98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2279_REG (MC_BASE + 0x3D9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2280_REG (MC_BASE + 0x3DA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2281_REG (MC_BASE + 0x3DA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2282_REG (MC_BASE + 0x3DA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2283_REG (MC_BASE + 0x3DAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2284_REG (MC_BASE + 0x3DB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2285_REG (MC_BASE + 0x3DB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2286_REG (MC_BASE + 0x3DB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2287_REG (MC_BASE + 0x3DBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2288_REG (MC_BASE + 0x3DC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2289_REG (MC_BASE + 0x3DC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2290_REG (MC_BASE + 0x3DC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2291_REG (MC_BASE + 0x3DCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2292_REG (MC_BASE + 0x3DD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2293_REG (MC_BASE + 0x3DD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2294_REG (MC_BASE + 0x3DD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2295_REG (MC_BASE + 0x3DDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2296_REG (MC_BASE + 0x3DE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2297_REG (MC_BASE + 0x3DE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2298_REG (MC_BASE + 0x3DE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2299_REG (MC_BASE + 0x3DEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2300_REG (MC_BASE + 0x3DF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2301_REG (MC_BASE + 0x3DF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2302_REG (MC_BASE + 0x3DF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2303_REG (MC_BASE + 0x3DFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2304_REG (MC_BASE + 0x3E00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2305_REG (MC_BASE + 0x3E04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2306_REG (MC_BASE + 0x3E08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2307_REG (MC_BASE + 0x3E0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2308_REG (MC_BASE + 0x3E10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2309_REG (MC_BASE + 0x3E14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2310_REG (MC_BASE + 0x3E18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2311_REG (MC_BASE + 0x3E1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2312_REG (MC_BASE + 0x3E20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2313_REG (MC_BASE + 0x3E24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2314_REG (MC_BASE + 0x3E28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2315_REG (MC_BASE + 0x3E2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2316_REG (MC_BASE + 0x3E30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2317_REG (MC_BASE + 0x3E34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2318_REG (MC_BASE + 0x3E38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2319_REG (MC_BASE + 0x3E3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2320_REG (MC_BASE + 0x3E40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2321_REG (MC_BASE + 0x3E44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2322_REG (MC_BASE + 0x3E48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2323_REG (MC_BASE + 0x3E4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2324_REG (MC_BASE + 0x3E50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2325_REG (MC_BASE + 0x3E54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2326_REG (MC_BASE + 0x3E58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2327_REG (MC_BASE + 0x3E5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2328_REG (MC_BASE + 0x3E60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2329_REG (MC_BASE + 0x3E64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2330_REG (MC_BASE + 0x3E68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2331_REG (MC_BASE + 0x3E6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2332_REG (MC_BASE + 0x3E70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2333_REG (MC_BASE + 0x3E74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2334_REG (MC_BASE + 0x3E78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2335_REG (MC_BASE + 0x3E7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2336_REG (MC_BASE + 0x3E80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2337_REG (MC_BASE + 0x3E84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2338_REG (MC_BASE + 0x3E88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2339_REG (MC_BASE + 0x3E8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2340_REG (MC_BASE + 0x3E90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2341_REG (MC_BASE + 0x3E94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2342_REG (MC_BASE + 0x3E98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2343_REG (MC_BASE + 0x3E9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2344_REG (MC_BASE + 0x3EA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2345_REG (MC_BASE + 0x3EA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2346_REG (MC_BASE + 0x3EA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2347_REG (MC_BASE + 0x3EAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2348_REG (MC_BASE + 0x3EB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2349_REG (MC_BASE + 0x3EB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2350_REG (MC_BASE + 0x3EB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2351_REG (MC_BASE + 0x3EBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2352_REG (MC_BASE + 0x3EC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2353_REG (MC_BASE + 0x3EC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2354_REG (MC_BASE + 0x3EC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2355_REG (MC_BASE + 0x3ECC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2356_REG (MC_BASE + 0x3ED0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2357_REG (MC_BASE + 0x3ED4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2358_REG (MC_BASE + 0x3ED8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2359_REG (MC_BASE + 0x3EDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2360_REG (MC_BASE + 0x3EE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2361_REG (MC_BASE + 0x3EE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2362_REG (MC_BASE + 0x3EE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2363_REG (MC_BASE + 0x3EEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2364_REG (MC_BASE + 0x3EF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2365_REG (MC_BASE + 0x3EF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2366_REG (MC_BASE + 0x3EF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2367_REG (MC_BASE + 0x3EFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2368_REG (MC_BASE + 0x3F00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2369_REG (MC_BASE + 0x3F04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2370_REG (MC_BASE + 0x3F08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2371_REG (MC_BASE + 0x3F0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2372_REG (MC_BASE + 0x3F10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2373_REG (MC_BASE + 0x3F14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2374_REG (MC_BASE + 0x3F18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2375_REG (MC_BASE + 0x3F1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2376_REG (MC_BASE + 0x3F20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2377_REG (MC_BASE + 0x3F24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2378_REG (MC_BASE + 0x3F28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2379_REG (MC_BASE + 0x3F2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2380_REG (MC_BASE + 0x3F30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2381_REG (MC_BASE + 0x3F34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2382_REG (MC_BASE + 0x3F38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2383_REG (MC_BASE + 0x3F3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2384_REG (MC_BASE + 0x3F40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2385_REG (MC_BASE + 0x3F44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2386_REG (MC_BASE + 0x3F48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2387_REG (MC_BASE + 0x3F4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2388_REG (MC_BASE + 0x3F50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2389_REG (MC_BASE + 0x3F54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2390_REG (MC_BASE + 0x3F58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2391_REG (MC_BASE + 0x3F5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2392_REG (MC_BASE + 0x3F60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2393_REG (MC_BASE + 0x3F64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2394_REG (MC_BASE + 0x3F68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2395_REG (MC_BASE + 0x3F6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2396_REG (MC_BASE + 0x3F70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2397_REG (MC_BASE + 0x3F74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2398_REG (MC_BASE + 0x3F78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2399_REG (MC_BASE + 0x3F7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2400_REG (MC_BASE + 0x3F80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2401_REG (MC_BASE + 0x3F84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2402_REG (MC_BASE + 0x3F88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2403_REG (MC_BASE + 0x3F8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2404_REG (MC_BASE + 0x3F90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2405_REG (MC_BASE + 0x3F94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2406_REG (MC_BASE + 0x3F98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2407_REG (MC_BASE + 0x3F9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2408_REG (MC_BASE + 0x3FA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2409_REG (MC_BASE + 0x3FA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2410_REG (MC_BASE + 0x3FA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2411_REG (MC_BASE + 0x3FAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2412_REG (MC_BASE + 0x3FB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2413_REG (MC_BASE + 0x3FB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2414_REG (MC_BASE + 0x3FB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2415_REG (MC_BASE + 0x3FBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2416_REG (MC_BASE + 0x3FC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2417_REG (MC_BASE + 0x3FC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2418_REG (MC_BASE + 0x3FC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2419_REG (MC_BASE + 0x3FCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2420_REG (MC_BASE + 0x3FD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2421_REG (MC_BASE + 0x3FD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2422_REG (MC_BASE + 0x3FD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2423_REG (MC_BASE + 0x3FDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2424_REG (MC_BASE + 0x3FE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2425_REG (MC_BASE + 0x3FE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2426_REG (MC_BASE + 0x3FE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2427_REG (MC_BASE + 0x3FEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2428_REG (MC_BASE + 0x3FF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2429_REG (MC_BASE + 0x3FF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2430_REG (MC_BASE + 0x3FF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2431_REG (MC_BASE + 0x3FFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2432_REG (MC_BASE + 0x4000) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2433_REG (MC_BASE + 0x4004) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2434_REG (MC_BASE + 0x4008) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2435_REG (MC_BASE + 0x400C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2436_REG (MC_BASE + 0x4010) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2437_REG (MC_BASE + 0x4014) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2438_REG (MC_BASE + 0x4018) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2439_REG (MC_BASE + 0x401C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2440_REG (MC_BASE + 0x4020) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2441_REG (MC_BASE + 0x4024) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2442_REG (MC_BASE + 0x4028) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2443_REG (MC_BASE + 0x402C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2444_REG (MC_BASE + 0x4030) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2445_REG (MC_BASE + 0x4034) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2446_REG (MC_BASE + 0x4038) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2447_REG (MC_BASE + 0x403C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2448_REG (MC_BASE + 0x4040) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2449_REG (MC_BASE + 0x4044) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2450_REG (MC_BASE + 0x4048) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2451_REG (MC_BASE + 0x404C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2452_REG (MC_BASE + 0x4050) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2453_REG (MC_BASE + 0x4054) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2454_REG (MC_BASE + 0x4058) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2455_REG (MC_BASE + 0x405C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2456_REG (MC_BASE + 0x4060) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2457_REG (MC_BASE + 0x4064) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2458_REG (MC_BASE + 0x4068) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2459_REG (MC_BASE + 0x406C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2460_REG (MC_BASE + 0x4070) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2461_REG (MC_BASE + 0x4074) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2462_REG (MC_BASE + 0x4078) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2463_REG (MC_BASE + 0x407C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2464_REG (MC_BASE + 0x4080) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2465_REG (MC_BASE + 0x4084) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2466_REG (MC_BASE + 0x4088) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2467_REG (MC_BASE + 0x408C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2468_REG (MC_BASE + 0x4090) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2469_REG (MC_BASE + 0x4094) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2470_REG (MC_BASE + 0x4098) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2471_REG (MC_BASE + 0x409C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2472_REG (MC_BASE + 0x40A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2473_REG (MC_BASE + 0x40A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2474_REG (MC_BASE + 0x40A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2475_REG (MC_BASE + 0x40AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2476_REG (MC_BASE + 0x40B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2477_REG (MC_BASE + 0x40B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2478_REG (MC_BASE + 0x40B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2479_REG (MC_BASE + 0x40BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2480_REG (MC_BASE + 0x40C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2481_REG (MC_BASE + 0x40C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2482_REG (MC_BASE + 0x40C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2483_REG (MC_BASE + 0x40CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2484_REG (MC_BASE + 0x40D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2485_REG (MC_BASE + 0x40D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2486_REG (MC_BASE + 0x40D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2487_REG (MC_BASE + 0x40DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2488_REG (MC_BASE + 0x40E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2489_REG (MC_BASE + 0x40E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2490_REG (MC_BASE + 0x40E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2491_REG (MC_BASE + 0x40EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2492_REG (MC_BASE + 0x40F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2493_REG (MC_BASE + 0x40F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2494_REG (MC_BASE + 0x40F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2495_REG (MC_BASE + 0x40FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2496_REG (MC_BASE + 0x4100) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2497_REG (MC_BASE + 0x4104) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2498_REG (MC_BASE + 0x4108) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2499_REG (MC_BASE + 0x410C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2500_REG (MC_BASE + 0x4110) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2501_REG (MC_BASE + 0x4114) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2502_REG (MC_BASE + 0x4118) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2503_REG (MC_BASE + 0x411C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2504_REG (MC_BASE + 0x4120) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2505_REG (MC_BASE + 0x4124) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2506_REG (MC_BASE + 0x4128) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2507_REG (MC_BASE + 0x412C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2508_REG (MC_BASE + 0x4130) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2509_REG (MC_BASE + 0x4134) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2510_REG (MC_BASE + 0x4138) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2511_REG (MC_BASE + 0x413C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2512_REG (MC_BASE + 0x4140) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2513_REG (MC_BASE + 0x4144) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2514_REG (MC_BASE + 0x4148) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2515_REG (MC_BASE + 0x414C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2516_REG (MC_BASE + 0x4150) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2517_REG (MC_BASE + 0x4154) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2518_REG (MC_BASE + 0x4158) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2519_REG (MC_BASE + 0x415C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2520_REG (MC_BASE + 0x4160) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2521_REG (MC_BASE + 0x4164) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2522_REG (MC_BASE + 0x4168) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2523_REG (MC_BASE + 0x416C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2524_REG (MC_BASE + 0x4170) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2525_REG (MC_BASE + 0x4174) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2526_REG (MC_BASE + 0x4178) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2527_REG (MC_BASE + 0x417C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2528_REG (MC_BASE + 0x4180) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2529_REG (MC_BASE + 0x4184) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2530_REG (MC_BASE + 0x4188) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2531_REG (MC_BASE + 0x418C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2532_REG (MC_BASE + 0x4190) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2533_REG (MC_BASE + 0x4194) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2534_REG (MC_BASE + 0x4198) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2535_REG (MC_BASE + 0x419C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2536_REG (MC_BASE + 0x41A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2537_REG (MC_BASE + 0x41A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2538_REG (MC_BASE + 0x41A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2539_REG (MC_BASE + 0x41AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2540_REG (MC_BASE + 0x41B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2541_REG (MC_BASE + 0x41B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2542_REG (MC_BASE + 0x41B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2543_REG (MC_BASE + 0x41BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2544_REG (MC_BASE + 0x41C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2545_REG (MC_BASE + 0x41C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2546_REG (MC_BASE + 0x41C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2547_REG (MC_BASE + 0x41CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2548_REG (MC_BASE + 0x41D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2549_REG (MC_BASE + 0x41D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2550_REG (MC_BASE + 0x41D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2551_REG (MC_BASE + 0x41DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2552_REG (MC_BASE + 0x41E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2553_REG (MC_BASE + 0x41E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2554_REG (MC_BASE + 0x41E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2555_REG (MC_BASE + 0x41EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2556_REG (MC_BASE + 0x41F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2557_REG (MC_BASE + 0x41F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2558_REG (MC_BASE + 0x41F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2559_REG (MC_BASE + 0x41FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2560_REG (MC_BASE + 0x4200) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2561_REG (MC_BASE + 0x4204) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2562_REG (MC_BASE + 0x4208) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2563_REG (MC_BASE + 0x420C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2564_REG (MC_BASE + 0x4210) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2565_REG (MC_BASE + 0x4214) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2566_REG (MC_BASE + 0x4218) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2567_REG (MC_BASE + 0x421C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2568_REG (MC_BASE + 0x4220) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2569_REG (MC_BASE + 0x4224) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2570_REG (MC_BASE + 0x4228) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2571_REG (MC_BASE + 0x422C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2572_REG (MC_BASE + 0x4230) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2573_REG (MC_BASE + 0x4234) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2574_REG (MC_BASE + 0x4238) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2575_REG (MC_BASE + 0x423C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2576_REG (MC_BASE + 0x4240) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2577_REG (MC_BASE + 0x4244) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2578_REG (MC_BASE + 0x4248) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2579_REG (MC_BASE + 0x424C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2580_REG (MC_BASE + 0x4250) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2581_REG (MC_BASE + 0x4254) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2582_REG (MC_BASE + 0x4258) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2583_REG (MC_BASE + 0x425C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2584_REG (MC_BASE + 0x4260) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2585_REG (MC_BASE + 0x4264) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2586_REG (MC_BASE + 0x4268) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2587_REG (MC_BASE + 0x426C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2588_REG (MC_BASE + 0x4270) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2589_REG (MC_BASE + 0x4274) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2590_REG (MC_BASE + 0x4278) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2591_REG (MC_BASE + 0x427C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2592_REG (MC_BASE + 0x4280) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2593_REG (MC_BASE + 0x4284) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2594_REG (MC_BASE + 0x4288) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2595_REG (MC_BASE + 0x428C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2596_REG (MC_BASE + 0x4290) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2597_REG (MC_BASE + 0x4294) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2598_REG (MC_BASE + 0x4298) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2599_REG (MC_BASE + 0x429C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2600_REG (MC_BASE + 0x42A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2601_REG (MC_BASE + 0x42A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2602_REG (MC_BASE + 0x42A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2603_REG (MC_BASE + 0x42AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2604_REG (MC_BASE + 0x42B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2605_REG (MC_BASE + 0x42B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2606_REG (MC_BASE + 0x42B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2607_REG (MC_BASE + 0x42BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2608_REG (MC_BASE + 0x42C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2609_REG (MC_BASE + 0x42C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2610_REG (MC_BASE + 0x42C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2611_REG (MC_BASE + 0x42CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2612_REG (MC_BASE + 0x42D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2613_REG (MC_BASE + 0x42D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2614_REG (MC_BASE + 0x42D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2615_REG (MC_BASE + 0x42DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2616_REG (MC_BASE + 0x42E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2617_REG (MC_BASE + 0x42E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2618_REG (MC_BASE + 0x42E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2619_REG (MC_BASE + 0x42EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2620_REG (MC_BASE + 0x42F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2621_REG (MC_BASE + 0x42F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2622_REG (MC_BASE + 0x42F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2623_REG (MC_BASE + 0x42FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2624_REG (MC_BASE + 0x4300) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2625_REG (MC_BASE + 0x4304) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2626_REG (MC_BASE + 0x4308) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2627_REG (MC_BASE + 0x430C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2628_REG (MC_BASE + 0x4310) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2629_REG (MC_BASE + 0x4314) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2630_REG (MC_BASE + 0x4318) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2631_REG (MC_BASE + 0x431C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2632_REG (MC_BASE + 0x4320) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2633_REG (MC_BASE + 0x4324) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2634_REG (MC_BASE + 0x4328) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2635_REG (MC_BASE + 0x432C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2636_REG (MC_BASE + 0x4330) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2637_REG (MC_BASE + 0x4334) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2638_REG (MC_BASE + 0x4338) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2639_REG (MC_BASE + 0x433C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2640_REG (MC_BASE + 0x4340) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2641_REG (MC_BASE + 0x4344) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2642_REG (MC_BASE + 0x4348) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2643_REG (MC_BASE + 0x434C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2644_REG (MC_BASE + 0x4350) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2645_REG (MC_BASE + 0x4354) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2646_REG (MC_BASE + 0x4358) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2647_REG (MC_BASE + 0x435C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2648_REG (MC_BASE + 0x4360) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2649_REG (MC_BASE + 0x4364) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2650_REG (MC_BASE + 0x4368) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2651_REG (MC_BASE + 0x436C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2652_REG (MC_BASE + 0x4370) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2653_REG (MC_BASE + 0x4374) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2654_REG (MC_BASE + 0x4378) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2655_REG (MC_BASE + 0x437C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2656_REG (MC_BASE + 0x4380) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2657_REG (MC_BASE + 0x4384) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2658_REG (MC_BASE + 0x4388) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2659_REG (MC_BASE + 0x438C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2660_REG (MC_BASE + 0x4390) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2661_REG (MC_BASE + 0x4394) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2662_REG (MC_BASE + 0x4398) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2663_REG (MC_BASE + 0x439C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2664_REG (MC_BASE + 0x43A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2665_REG (MC_BASE + 0x43A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2666_REG (MC_BASE + 0x43A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2667_REG (MC_BASE + 0x43AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2668_REG (MC_BASE + 0x43B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2669_REG (MC_BASE + 0x43B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2670_REG (MC_BASE + 0x43B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2671_REG (MC_BASE + 0x43BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2672_REG (MC_BASE + 0x43C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2673_REG (MC_BASE + 0x43C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2674_REG (MC_BASE + 0x43C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2675_REG (MC_BASE + 0x43CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2676_REG (MC_BASE + 0x43D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2677_REG (MC_BASE + 0x43D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2678_REG (MC_BASE + 0x43D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2679_REG (MC_BASE + 0x43DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2680_REG (MC_BASE + 0x43E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2681_REG (MC_BASE + 0x43E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2682_REG (MC_BASE + 0x43E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2683_REG (MC_BASE + 0x43EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2684_REG (MC_BASE + 0x43F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2685_REG (MC_BASE + 0x43F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2686_REG (MC_BASE + 0x43F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2687_REG (MC_BASE + 0x43FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2688_REG (MC_BASE + 0x4400) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2689_REG (MC_BASE + 0x4404) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2690_REG (MC_BASE + 0x4408) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2691_REG (MC_BASE + 0x440C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2692_REG (MC_BASE + 0x4410) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2693_REG (MC_BASE + 0x4414) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2694_REG (MC_BASE + 0x4418) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2695_REG (MC_BASE + 0x441C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2696_REG (MC_BASE + 0x4420) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2697_REG (MC_BASE + 0x4424) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2698_REG (MC_BASE + 0x4428) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2699_REG (MC_BASE + 0x442C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2700_REG (MC_BASE + 0x4430) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2701_REG (MC_BASE + 0x4434) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2702_REG (MC_BASE + 0x4438) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2703_REG (MC_BASE + 0x443C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2704_REG (MC_BASE + 0x4440) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2705_REG (MC_BASE + 0x4444) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2706_REG (MC_BASE + 0x4448) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2707_REG (MC_BASE + 0x444C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2708_REG (MC_BASE + 0x4450) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2709_REG (MC_BASE + 0x4454) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2710_REG (MC_BASE + 0x4458) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2711_REG (MC_BASE + 0x445C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2712_REG (MC_BASE + 0x4460) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2713_REG (MC_BASE + 0x4464) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2714_REG (MC_BASE + 0x4468) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2715_REG (MC_BASE + 0x446C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2716_REG (MC_BASE + 0x4470) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2717_REG (MC_BASE + 0x4474) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2718_REG (MC_BASE + 0x4478) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2719_REG (MC_BASE + 0x447C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2720_REG (MC_BASE + 0x4480) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2721_REG (MC_BASE + 0x4484) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2722_REG (MC_BASE + 0x4488) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2723_REG (MC_BASE + 0x448C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2724_REG (MC_BASE + 0x4490) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2725_REG (MC_BASE + 0x4494) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2726_REG (MC_BASE + 0x4498) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2727_REG (MC_BASE + 0x449C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2728_REG (MC_BASE + 0x44A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2729_REG (MC_BASE + 0x44A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2730_REG (MC_BASE + 0x44A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2731_REG (MC_BASE + 0x44AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2732_REG (MC_BASE + 0x44B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2733_REG (MC_BASE + 0x44B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2734_REG (MC_BASE + 0x44B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2735_REG (MC_BASE + 0x44BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2736_REG (MC_BASE + 0x44C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2737_REG (MC_BASE + 0x44C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2738_REG (MC_BASE + 0x44C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2739_REG (MC_BASE + 0x44CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2740_REG (MC_BASE + 0x44D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2741_REG (MC_BASE + 0x44D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2742_REG (MC_BASE + 0x44D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2743_REG (MC_BASE + 0x44DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2744_REG (MC_BASE + 0x44E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2745_REG (MC_BASE + 0x44E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2746_REG (MC_BASE + 0x44E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2747_REG (MC_BASE + 0x44EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2748_REG (MC_BASE + 0x44F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2749_REG (MC_BASE + 0x44F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2750_REG (MC_BASE + 0x44F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2751_REG (MC_BASE + 0x44FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2752_REG (MC_BASE + 0x4500) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2753_REG (MC_BASE + 0x4504) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2754_REG (MC_BASE + 0x4508) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2755_REG (MC_BASE + 0x450C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2756_REG (MC_BASE + 0x4510) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2757_REG (MC_BASE + 0x4514) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2758_REG (MC_BASE + 0x4518) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2759_REG (MC_BASE + 0x451C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2760_REG (MC_BASE + 0x4520) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2761_REG (MC_BASE + 0x4524) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2762_REG (MC_BASE + 0x4528) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2763_REG (MC_BASE + 0x452C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2764_REG (MC_BASE + 0x4530) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2765_REG (MC_BASE + 0x4534) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2766_REG (MC_BASE + 0x4538) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2767_REG (MC_BASE + 0x453C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2768_REG (MC_BASE + 0x4540) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2769_REG (MC_BASE + 0x4544) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2770_REG (MC_BASE + 0x4548) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2771_REG (MC_BASE + 0x454C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2772_REG (MC_BASE + 0x4550) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2773_REG (MC_BASE + 0x4554) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2774_REG (MC_BASE + 0x4558) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2775_REG (MC_BASE + 0x455C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2776_REG (MC_BASE + 0x4560) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2777_REG (MC_BASE + 0x4564) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2778_REG (MC_BASE + 0x4568) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2779_REG (MC_BASE + 0x456C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2780_REG (MC_BASE + 0x4570) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2781_REG (MC_BASE + 0x4574) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2782_REG (MC_BASE + 0x4578) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2783_REG (MC_BASE + 0x457C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2784_REG (MC_BASE + 0x4580) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2785_REG (MC_BASE + 0x4584) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2786_REG (MC_BASE + 0x4588) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2787_REG (MC_BASE + 0x458C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2788_REG (MC_BASE + 0x4590) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2789_REG (MC_BASE + 0x4594) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2790_REG (MC_BASE + 0x4598) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2791_REG (MC_BASE + 0x459C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2792_REG (MC_BASE + 0x45A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2793_REG (MC_BASE + 0x45A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2794_REG (MC_BASE + 0x45A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2795_REG (MC_BASE + 0x45AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2796_REG (MC_BASE + 0x45B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2797_REG (MC_BASE + 0x45B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2798_REG (MC_BASE + 0x45B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2799_REG (MC_BASE + 0x45BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2800_REG (MC_BASE + 0x45C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2801_REG (MC_BASE + 0x45C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2802_REG (MC_BASE + 0x45C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2803_REG (MC_BASE + 0x45CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2804_REG (MC_BASE + 0x45D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2805_REG (MC_BASE + 0x45D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2806_REG (MC_BASE + 0x45D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2807_REG (MC_BASE + 0x45DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2808_REG (MC_BASE + 0x45E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2809_REG (MC_BASE + 0x45E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2810_REG (MC_BASE + 0x45E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2811_REG (MC_BASE + 0x45EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2812_REG (MC_BASE + 0x45F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2813_REG (MC_BASE + 0x45F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2814_REG (MC_BASE + 0x45F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2815_REG (MC_BASE + 0x45FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2816_REG (MC_BASE + 0x4600) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2817_REG (MC_BASE + 0x4604) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2818_REG (MC_BASE + 0x4608) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2819_REG (MC_BASE + 0x460C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2820_REG (MC_BASE + 0x4610) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2821_REG (MC_BASE + 0x4614) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2822_REG (MC_BASE + 0x4618) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2823_REG (MC_BASE + 0x461C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2824_REG (MC_BASE + 0x4620) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2825_REG (MC_BASE + 0x4624) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2826_REG (MC_BASE + 0x4628) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2827_REG (MC_BASE + 0x462C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2828_REG (MC_BASE + 0x4630) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2829_REG (MC_BASE + 0x4634) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2830_REG (MC_BASE + 0x4638) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2831_REG (MC_BASE + 0x463C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2832_REG (MC_BASE + 0x4640) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2833_REG (MC_BASE + 0x4644) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2834_REG (MC_BASE + 0x4648) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2835_REG (MC_BASE + 0x464C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2836_REG (MC_BASE + 0x4650) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2837_REG (MC_BASE + 0x4654) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2838_REG (MC_BASE + 0x4658) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2839_REG (MC_BASE + 0x465C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2840_REG (MC_BASE + 0x4660) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2841_REG (MC_BASE + 0x4664) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2842_REG (MC_BASE + 0x4668) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2843_REG (MC_BASE + 0x466C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2844_REG (MC_BASE + 0x4670) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2845_REG (MC_BASE + 0x4674) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2846_REG (MC_BASE + 0x4678) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2847_REG (MC_BASE + 0x467C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2848_REG (MC_BASE + 0x4680) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2849_REG (MC_BASE + 0x4684) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2850_REG (MC_BASE + 0x4688) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2851_REG (MC_BASE + 0x468C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2852_REG (MC_BASE + 0x4690) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2853_REG (MC_BASE + 0x4694) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2854_REG (MC_BASE + 0x4698) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2855_REG (MC_BASE + 0x469C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2856_REG (MC_BASE + 0x46A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2857_REG (MC_BASE + 0x46A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2858_REG (MC_BASE + 0x46A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2859_REG (MC_BASE + 0x46AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2860_REG (MC_BASE + 0x46B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2861_REG (MC_BASE + 0x46B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2862_REG (MC_BASE + 0x46B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2863_REG (MC_BASE + 0x46BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2864_REG (MC_BASE + 0x46C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2865_REG (MC_BASE + 0x46C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2866_REG (MC_BASE + 0x46C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2867_REG (MC_BASE + 0x46CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2868_REG (MC_BASE + 0x46D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2869_REG (MC_BASE + 0x46D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2870_REG (MC_BASE + 0x46D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2871_REG (MC_BASE + 0x46DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2872_REG (MC_BASE + 0x46E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2873_REG (MC_BASE + 0x46E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2874_REG (MC_BASE + 0x46E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2875_REG (MC_BASE + 0x46EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2876_REG (MC_BASE + 0x46F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2877_REG (MC_BASE + 0x46F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2878_REG (MC_BASE + 0x46F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2879_REG (MC_BASE + 0x46FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2880_REG (MC_BASE + 0x4700) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2881_REG (MC_BASE + 0x4704) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2882_REG (MC_BASE + 0x4708) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2883_REG (MC_BASE + 0x470C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2884_REG (MC_BASE + 0x4710) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2885_REG (MC_BASE + 0x4714) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2886_REG (MC_BASE + 0x4718) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2887_REG (MC_BASE + 0x471C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2888_REG (MC_BASE + 0x4720) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2889_REG (MC_BASE + 0x4724) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2890_REG (MC_BASE + 0x4728) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2891_REG (MC_BASE + 0x472C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2892_REG (MC_BASE + 0x4730) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2893_REG (MC_BASE + 0x4734) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2894_REG (MC_BASE + 0x4738) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2895_REG (MC_BASE + 0x473C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2896_REG (MC_BASE + 0x4740) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2897_REG (MC_BASE + 0x4744) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2898_REG (MC_BASE + 0x4748) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2899_REG (MC_BASE + 0x474C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2900_REG (MC_BASE + 0x4750) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2901_REG (MC_BASE + 0x4754) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2902_REG (MC_BASE + 0x4758) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2903_REG (MC_BASE + 0x475C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2904_REG (MC_BASE + 0x4760) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2905_REG (MC_BASE + 0x4764) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2906_REG (MC_BASE + 0x4768) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2907_REG (MC_BASE + 0x476C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2908_REG (MC_BASE + 0x4770) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2909_REG (MC_BASE + 0x4774) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2910_REG (MC_BASE + 0x4778) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2911_REG (MC_BASE + 0x477C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2912_REG (MC_BASE + 0x4780) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2913_REG (MC_BASE + 0x4784) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2914_REG (MC_BASE + 0x4788) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2915_REG (MC_BASE + 0x478C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2916_REG (MC_BASE + 0x4790) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2917_REG (MC_BASE + 0x4794) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2918_REG (MC_BASE + 0x4798) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2919_REG (MC_BASE + 0x479C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2920_REG (MC_BASE + 0x47A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2921_REG (MC_BASE + 0x47A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2922_REG (MC_BASE + 0x47A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2923_REG (MC_BASE + 0x47AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2924_REG (MC_BASE + 0x47B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2925_REG (MC_BASE + 0x47B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2926_REG (MC_BASE + 0x47B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2927_REG (MC_BASE + 0x47BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2928_REG (MC_BASE + 0x47C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2929_REG (MC_BASE + 0x47C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2930_REG (MC_BASE + 0x47C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2931_REG (MC_BASE + 0x47CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2932_REG (MC_BASE + 0x47D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2933_REG (MC_BASE + 0x47D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2934_REG (MC_BASE + 0x47D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2935_REG (MC_BASE + 0x47DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2936_REG (MC_BASE + 0x47E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2937_REG (MC_BASE + 0x47E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2938_REG (MC_BASE + 0x47E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2939_REG (MC_BASE + 0x47EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2940_REG (MC_BASE + 0x47F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2941_REG (MC_BASE + 0x47F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2942_REG (MC_BASE + 0x47F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2943_REG (MC_BASE + 0x47FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2944_REG (MC_BASE + 0x4800) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2945_REG (MC_BASE + 0x4804) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2946_REG (MC_BASE + 0x4808) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2947_REG (MC_BASE + 0x480C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2948_REG (MC_BASE + 0x4810) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2949_REG (MC_BASE + 0x4814) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2950_REG (MC_BASE + 0x4818) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2951_REG (MC_BASE + 0x481C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2952_REG (MC_BASE + 0x4820) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2953_REG (MC_BASE + 0x4824) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2954_REG (MC_BASE + 0x4828) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2955_REG (MC_BASE + 0x482C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2956_REG (MC_BASE + 0x4830) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2957_REG (MC_BASE + 0x4834) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2958_REG (MC_BASE + 0x4838) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2959_REG (MC_BASE + 0x483C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2960_REG (MC_BASE + 0x4840) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2961_REG (MC_BASE + 0x4844) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2962_REG (MC_BASE + 0x4848) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2963_REG (MC_BASE + 0x484C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2964_REG (MC_BASE + 0x4850) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2965_REG (MC_BASE + 0x4854) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2966_REG (MC_BASE + 0x4858) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2967_REG (MC_BASE + 0x485C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2968_REG (MC_BASE + 0x4860) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2969_REG (MC_BASE + 0x4864) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2970_REG (MC_BASE + 0x4868) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2971_REG (MC_BASE + 0x486C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2972_REG (MC_BASE + 0x4870) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2973_REG (MC_BASE + 0x4874) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2974_REG (MC_BASE + 0x4878) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2975_REG (MC_BASE + 0x487C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2976_REG (MC_BASE + 0x4880) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2977_REG (MC_BASE + 0x4884) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2978_REG (MC_BASE + 0x4888) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2979_REG (MC_BASE + 0x488C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2980_REG (MC_BASE + 0x4890) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2981_REG (MC_BASE + 0x4894) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2982_REG (MC_BASE + 0x4898) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2983_REG (MC_BASE + 0x489C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2984_REG (MC_BASE + 0x48A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2985_REG (MC_BASE + 0x48A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2986_REG (MC_BASE + 0x48A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2987_REG (MC_BASE + 0x48AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2988_REG (MC_BASE + 0x48B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2989_REG (MC_BASE + 0x48B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2990_REG (MC_BASE + 0x48B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2991_REG (MC_BASE + 0x48BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2992_REG (MC_BASE + 0x48C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2993_REG (MC_BASE + 0x48C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2994_REG (MC_BASE + 0x48C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2995_REG (MC_BASE + 0x48CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2996_REG (MC_BASE + 0x48D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2997_REG (MC_BASE + 0x48D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2998_REG (MC_BASE + 0x48D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_2999_REG (MC_BASE + 0x48DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3000_REG (MC_BASE + 0x48E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3001_REG (MC_BASE + 0x48E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3002_REG (MC_BASE + 0x48E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3003_REG (MC_BASE + 0x48EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3004_REG (MC_BASE + 0x48F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3005_REG (MC_BASE + 0x48F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3006_REG (MC_BASE + 0x48F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3007_REG (MC_BASE + 0x48FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3008_REG (MC_BASE + 0x4900) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3009_REG (MC_BASE + 0x4904) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3010_REG (MC_BASE + 0x4908) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3011_REG (MC_BASE + 0x490C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3012_REG (MC_BASE + 0x4910) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3013_REG (MC_BASE + 0x4914) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3014_REG (MC_BASE + 0x4918) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3015_REG (MC_BASE + 0x491C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3016_REG (MC_BASE + 0x4920) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3017_REG (MC_BASE + 0x4924) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3018_REG (MC_BASE + 0x4928) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3019_REG (MC_BASE + 0x492C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3020_REG (MC_BASE + 0x4930) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3021_REG (MC_BASE + 0x4934) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3022_REG (MC_BASE + 0x4938) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3023_REG (MC_BASE + 0x493C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3024_REG (MC_BASE + 0x4940) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3025_REG (MC_BASE + 0x4944) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3026_REG (MC_BASE + 0x4948) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3027_REG (MC_BASE + 0x494C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3028_REG (MC_BASE + 0x4950) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3029_REG (MC_BASE + 0x4954) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3030_REG (MC_BASE + 0x4958) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3031_REG (MC_BASE + 0x495C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3032_REG (MC_BASE + 0x4960) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3033_REG (MC_BASE + 0x4964) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3034_REG (MC_BASE + 0x4968) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3035_REG (MC_BASE + 0x496C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3036_REG (MC_BASE + 0x4970) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3037_REG (MC_BASE + 0x4974) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3038_REG (MC_BASE + 0x4978) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3039_REG (MC_BASE + 0x497C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3040_REG (MC_BASE + 0x4980) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3041_REG (MC_BASE + 0x4984) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3042_REG (MC_BASE + 0x4988) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3043_REG (MC_BASE + 0x498C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3044_REG (MC_BASE + 0x4990) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3045_REG (MC_BASE + 0x4994) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3046_REG (MC_BASE + 0x4998) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3047_REG (MC_BASE + 0x499C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3048_REG (MC_BASE + 0x49A0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3049_REG (MC_BASE + 0x49A4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3050_REG (MC_BASE + 0x49A8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3051_REG (MC_BASE + 0x49AC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3052_REG (MC_BASE + 0x49B0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3053_REG (MC_BASE + 0x49B4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3054_REG (MC_BASE + 0x49B8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3055_REG (MC_BASE + 0x49BC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3056_REG (MC_BASE + 0x49C0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3057_REG (MC_BASE + 0x49C4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3058_REG (MC_BASE + 0x49C8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3059_REG (MC_BASE + 0x49CC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3060_REG (MC_BASE + 0x49D0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3061_REG (MC_BASE + 0x49D4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3062_REG (MC_BASE + 0x49D8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3063_REG (MC_BASE + 0x49DC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3064_REG (MC_BASE + 0x49E0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3065_REG (MC_BASE + 0x49E4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3066_REG (MC_BASE + 0x49E8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3067_REG (MC_BASE + 0x49EC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3068_REG (MC_BASE + 0x49F0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3069_REG (MC_BASE + 0x49F4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3070_REG (MC_BASE + 0x49F8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3071_REG (MC_BASE + 0x49FC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3072_REG (MC_BASE + 0x4A00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3073_REG (MC_BASE + 0x4A04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3074_REG (MC_BASE + 0x4A08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3075_REG (MC_BASE + 0x4A0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3076_REG (MC_BASE + 0x4A10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3077_REG (MC_BASE + 0x4A14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3078_REG (MC_BASE + 0x4A18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3079_REG (MC_BASE + 0x4A1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3080_REG (MC_BASE + 0x4A20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3081_REG (MC_BASE + 0x4A24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3082_REG (MC_BASE + 0x4A28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3083_REG (MC_BASE + 0x4A2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3084_REG (MC_BASE + 0x4A30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3085_REG (MC_BASE + 0x4A34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3086_REG (MC_BASE + 0x4A38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3087_REG (MC_BASE + 0x4A3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3088_REG (MC_BASE + 0x4A40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3089_REG (MC_BASE + 0x4A44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3090_REG (MC_BASE + 0x4A48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3091_REG (MC_BASE + 0x4A4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3092_REG (MC_BASE + 0x4A50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3093_REG (MC_BASE + 0x4A54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3094_REG (MC_BASE + 0x4A58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3095_REG (MC_BASE + 0x4A5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3096_REG (MC_BASE + 0x4A60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3097_REG (MC_BASE + 0x4A64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3098_REG (MC_BASE + 0x4A68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3099_REG (MC_BASE + 0x4A6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3100_REG (MC_BASE + 0x4A70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3101_REG (MC_BASE + 0x4A74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3102_REG (MC_BASE + 0x4A78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3103_REG (MC_BASE + 0x4A7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3104_REG (MC_BASE + 0x4A80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3105_REG (MC_BASE + 0x4A84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3106_REG (MC_BASE + 0x4A88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3107_REG (MC_BASE + 0x4A8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3108_REG (MC_BASE + 0x4A90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3109_REG (MC_BASE + 0x4A94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3110_REG (MC_BASE + 0x4A98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3111_REG (MC_BASE + 0x4A9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3112_REG (MC_BASE + 0x4AA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3113_REG (MC_BASE + 0x4AA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3114_REG (MC_BASE + 0x4AA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3115_REG (MC_BASE + 0x4AAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3116_REG (MC_BASE + 0x4AB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3117_REG (MC_BASE + 0x4AB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3118_REG (MC_BASE + 0x4AB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3119_REG (MC_BASE + 0x4ABC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3120_REG (MC_BASE + 0x4AC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3121_REG (MC_BASE + 0x4AC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3122_REG (MC_BASE + 0x4AC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3123_REG (MC_BASE + 0x4ACC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3124_REG (MC_BASE + 0x4AD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3125_REG (MC_BASE + 0x4AD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3126_REG (MC_BASE + 0x4AD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3127_REG (MC_BASE + 0x4ADC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3128_REG (MC_BASE + 0x4AE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3129_REG (MC_BASE + 0x4AE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3130_REG (MC_BASE + 0x4AE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3131_REG (MC_BASE + 0x4AEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3132_REG (MC_BASE + 0x4AF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3133_REG (MC_BASE + 0x4AF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3134_REG (MC_BASE + 0x4AF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3135_REG (MC_BASE + 0x4AFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3136_REG (MC_BASE + 0x4B00) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3137_REG (MC_BASE + 0x4B04) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3138_REG (MC_BASE + 0x4B08) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3139_REG (MC_BASE + 0x4B0C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3140_REG (MC_BASE + 0x4B10) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3141_REG (MC_BASE + 0x4B14) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3142_REG (MC_BASE + 0x4B18) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3143_REG (MC_BASE + 0x4B1C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3144_REG (MC_BASE + 0x4B20) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3145_REG (MC_BASE + 0x4B24) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3146_REG (MC_BASE + 0x4B28) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3147_REG (MC_BASE + 0x4B2C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3148_REG (MC_BASE + 0x4B30) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3149_REG (MC_BASE + 0x4B34) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3150_REG (MC_BASE + 0x4B38) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3151_REG (MC_BASE + 0x4B3C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3152_REG (MC_BASE + 0x4B40) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3153_REG (MC_BASE + 0x4B44) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3154_REG (MC_BASE + 0x4B48) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3155_REG (MC_BASE + 0x4B4C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3156_REG (MC_BASE + 0x4B50) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3157_REG (MC_BASE + 0x4B54) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3158_REG (MC_BASE + 0x4B58) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3159_REG (MC_BASE + 0x4B5C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3160_REG (MC_BASE + 0x4B60) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3161_REG (MC_BASE + 0x4B64) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3162_REG (MC_BASE + 0x4B68) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3163_REG (MC_BASE + 0x4B6C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3164_REG (MC_BASE + 0x4B70) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3165_REG (MC_BASE + 0x4B74) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3166_REG (MC_BASE + 0x4B78) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3167_REG (MC_BASE + 0x4B7C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3168_REG (MC_BASE + 0x4B80) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3169_REG (MC_BASE + 0x4B84) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3170_REG (MC_BASE + 0x4B88) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3171_REG (MC_BASE + 0x4B8C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3172_REG (MC_BASE + 0x4B90) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3173_REG (MC_BASE + 0x4B94) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3174_REG (MC_BASE + 0x4B98) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3175_REG (MC_BASE + 0x4B9C) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3176_REG (MC_BASE + 0x4BA0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3177_REG (MC_BASE + 0x4BA4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3178_REG (MC_BASE + 0x4BA8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3179_REG (MC_BASE + 0x4BAC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3180_REG (MC_BASE + 0x4BB0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3181_REG (MC_BASE + 0x4BB4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3182_REG (MC_BASE + 0x4BB8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3183_REG (MC_BASE + 0x4BBC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3184_REG (MC_BASE + 0x4BC0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3185_REG (MC_BASE + 0x4BC4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3186_REG (MC_BASE + 0x4BC8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3187_REG (MC_BASE + 0x4BCC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3188_REG (MC_BASE + 0x4BD0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3189_REG (MC_BASE + 0x4BD4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3190_REG (MC_BASE + 0x4BD8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3191_REG (MC_BASE + 0x4BDC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3192_REG (MC_BASE + 0x4BE0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3193_REG (MC_BASE + 0x4BE4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3194_REG (MC_BASE + 0x4BE8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3195_REG (MC_BASE + 0x4BEC) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3196_REG (MC_BASE + 0x4BF0) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3197_REG (MC_BASE + 0x4BF4) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3198_REG (MC_BASE + 0x4BF8) /* coordinate pairs from KLT or the thrid-party */
#define MC_COORDINATE_CFG_3199_REG (MC_BASE + 0x4BFC) /* coordinate pairs from KLT or the thrid-party */
#define MC_LOW_KPT_STATUS_REG      (MC_BASE + 0x4C00) /* Low kpt status */
#define MC_DEBUG_0_REG             (MC_BASE + 0x4D00) /* Debug 0 */
#define MC_DEBUG_1_REG             (MC_BASE + 0x4D04) /* Debug 1 */
#define MC_EC_0_REG                (MC_BASE + 0x4D08) /* ec 0 */
#define MC_EC_1_REG                (MC_BASE + 0x4D0C) /* ec 1 */

#endif // __MC_REG_OFFSET_H__
