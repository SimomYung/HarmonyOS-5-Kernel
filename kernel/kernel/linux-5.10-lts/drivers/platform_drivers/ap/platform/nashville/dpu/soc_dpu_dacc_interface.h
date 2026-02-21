/******************************************************************************

                 版权所有 (C), 2001-2020, 华为技术有限公司

 ******************************************************************************
  文 件 名   : soc_dacc_interface.h
  版 本 号   : 初稿
  作    者   : Excel2Code
  生成日期   : 2020-10-29 11:49:52
  最近修改   :
  功能描述   : 接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2020年10月29日
    修改内容   : 从《V100 ES SOC寄存器手册_DACC.xml》自动生成

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_DACC_INTERFACE_H__
#define __SOC_DACC_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/3) CFG
 ****************************************************************************/


/* 寄存器说明：SW interrupt registers
   位域定义UNION结构:  SOC_DACC_SW_EXT_INT_REG_UNION */
#define SOC_DACC_SW_EXT_INT_REG_ADDR(base)                    ((base) + (0x0000))

/* 寄存器说明：SW interrupt mask registers
   位域定义UNION结构:  SOC_DACC_SW_EXT_INT_MASK_REG_UNION */
#define SOC_DACC_SW_EXT_INT_MASK_REG_ADDR(base)               ((base) + (0x0004))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_POR_RESET_PC_REG_UNION */
#define SOC_DACC_POR_RESET_PC_REG_ADDR(base)                  ((base) + (0x0008))

/* 寄存器说明：RISC-V CORE
   位域定义UNION结构:  SOC_DACC_CORE_CONFIG_REG_UNION */
#define SOC_DACC_CORE_CONFIG_REG_ADDR(base)                   ((base) + (0x000C))

/* 寄存器说明：RISC-V CORE
   位域定义UNION结构:  SOC_DACC_CORE_STATUS_REG_UNION */
#define SOC_DACC_CORE_STATUS_REG_ADDR(base)                   ((base) + (0x0010))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CORE_MON_PC_REG_UNION */
#define SOC_DACC_CORE_MON_PC_REG_ADDR(base)                   ((base) + (0x0014))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CORE_MON_LR_REG_UNION */
#define SOC_DACC_CORE_MON_LR_REG_ADDR(base)                   ((base) + (0x0018))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_LDI_SEL0_REG_UNION */
#define SOC_DACC_CTL_LDI_SEL0_REG_ADDR(base)                  ((base) + (0x001C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_LDI_SEL1_REG_UNION */
#define SOC_DACC_CTL_LDI_SEL1_REG_ADDR(base)                  ((base) + (0x0020))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_LDI_SEL2_REG_UNION */
#define SOC_DACC_CTL_LDI_SEL2_REG_ADDR(base)                  ((base) + (0x0024))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_LDI_SEL3_REG_UNION */
#define SOC_DACC_CTL_LDI_SEL3_REG_ADDR(base)                  ((base) + (0x0028))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_WCH_SEL0_UNION */
#define SOC_DACC_CTL_WCH_SEL0_ADDR(base)                      ((base) + (0x002C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_WCH_SEL1_UNION */
#define SOC_DACC_CTL_WCH_SEL1_ADDR(base)                      ((base) + (0x0030))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_WCH_SEL2_UNION */
#define SOC_DACC_CTL_WCH_SEL2_ADDR(base)                      ((base) + (0x0034))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_REG0_UNION */
#define SOC_DACC_DBG_REG0_ADDR(base)                          ((base) + (0x0038))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_SEL_REG_UNION */
#define SOC_DACC_DBG_SEL_REG_ADDR(base)                       ((base) + (0x003C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MINIBUS_1T01_CKG_EN_UNION */
#define SOC_DACC_MINIBUS_1T01_CKG_EN_ADDR(base)               ((base) + (0x0040))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MINIBUS_1T01_STATE_UNION */
#define SOC_DACC_MINIBUS_1T01_STATE_ADDR(base)                ((base) + (0x0044))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CKG_EN_UNION */
#define SOC_DACC_CKG_EN_ADDR(base)                            ((base) + (0x0048))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_EN_UNION */
#define SOC_DACC_WLT_EN_ADDR(base)                            ((base) + (0x0100))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_DDR_ADDR0_UNION */
#define SOC_DACC_WLT_SLICE0_DDR_ADDR0_ADDR(base)              ((base) + (0x0104))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_DDR_ADDR1_UNION */
#define SOC_DACC_WLT_SLICE0_DDR_ADDR1_ADDR(base)              ((base) + (0x0108))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_DDR_ADDR0_UNION */
#define SOC_DACC_WLT_SLICE1_DDR_ADDR0_ADDR(base)              ((base) + (0x010c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_DDR_ADDR1_UNION */
#define SOC_DACC_WLT_SLICE1_DDR_ADDR1_ADDR(base)              ((base) + (0x0110))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_DDR_ADDR0_UNION */
#define SOC_DACC_WLT_SLICE2_DDR_ADDR0_ADDR(base)              ((base) + (0x0114))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_DDR_ADDR1_UNION */
#define SOC_DACC_WLT_SLICE2_DDR_ADDR1_ADDR(base)              ((base) + (0x0118))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_DDR_ADDR0_UNION */
#define SOC_DACC_WLT_SLICE3_DDR_ADDR0_ADDR(base)              ((base) + (0x011c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_DDR_ADDR1_UNION */
#define SOC_DACC_WLT_SLICE3_DDR_ADDR1_ADDR(base)              ((base) + (0x0120))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_ADDR0_H_UNION */
#define SOC_DACC_WLT_SLICE0_ADDR0_H_ADDR(base)                ((base) + (0x0124))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_ADDR0_L_UNION */
#define SOC_DACC_WLT_SLICE0_ADDR0_L_ADDR(base)                ((base) + (0x0128))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_ADDR1_L_UNION */
#define SOC_DACC_WLT_SLICE0_ADDR1_L_ADDR(base)                ((base) + (0x012c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_ADDR2_L_UNION */
#define SOC_DACC_WLT_SLICE0_ADDR2_L_ADDR(base)                ((base) + (0x0130))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_ADDR3_L_UNION */
#define SOC_DACC_WLT_SLICE0_ADDR3_L_ADDR(base)                ((base) + (0x0134))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE0_STRIDE_UNION */
#define SOC_DACC_WLT_SLICE0_STRIDE_ADDR(base)                 ((base) + (0x0138))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_ADDR0_H_UNION */
#define SOC_DACC_WLT_SLICE1_ADDR0_H_ADDR(base)                ((base) + (0x013c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_ADDR0_L_UNION */
#define SOC_DACC_WLT_SLICE1_ADDR0_L_ADDR(base)                ((base) + (0x0140))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_ADDR1_L_UNION */
#define SOC_DACC_WLT_SLICE1_ADDR1_L_ADDR(base)                ((base) + (0x0144))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_ADDR2_L_UNION */
#define SOC_DACC_WLT_SLICE1_ADDR2_L_ADDR(base)                ((base) + (0x0148))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_ADDR3_L_UNION */
#define SOC_DACC_WLT_SLICE1_ADDR3_L_ADDR(base)                ((base) + (0x014c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE1_STRIDE_UNION */
#define SOC_DACC_WLT_SLICE1_STRIDE_ADDR(base)                 ((base) + (0x0150))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_ADDR0_H_UNION */
#define SOC_DACC_WLT_SLICE2_ADDR0_H_ADDR(base)                ((base) + (0x0154))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_ADDR0_L_UNION */
#define SOC_DACC_WLT_SLICE2_ADDR0_L_ADDR(base)                ((base) + (0x0158))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_ADDR1_L_UNION */
#define SOC_DACC_WLT_SLICE2_ADDR1_L_ADDR(base)                ((base) + (0x015c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_ADDR2_L_UNION */
#define SOC_DACC_WLT_SLICE2_ADDR2_L_ADDR(base)                ((base) + (0x0160))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_ADDR3_L_UNION */
#define SOC_DACC_WLT_SLICE2_ADDR3_L_ADDR(base)                ((base) + (0x0164))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE2_STRIDE_UNION */
#define SOC_DACC_WLT_SLICE2_STRIDE_ADDR(base)                 ((base) + (0x0168))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_ADDR0_H_UNION */
#define SOC_DACC_WLT_SLICE3_ADDR0_H_ADDR(base)                ((base) + (0x016c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_ADDR0_L_UNION */
#define SOC_DACC_WLT_SLICE3_ADDR0_L_ADDR(base)                ((base) + (0x0170))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_ADDR1_L_UNION */
#define SOC_DACC_WLT_SLICE3_ADDR1_L_ADDR(base)                ((base) + (0x0174))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_ADDR2_L_UNION */
#define SOC_DACC_WLT_SLICE3_ADDR2_L_ADDR(base)                ((base) + (0x0178))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_ADDR3_L_UNION */
#define SOC_DACC_WLT_SLICE3_ADDR3_L_ADDR(base)                ((base) + (0x017c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE3_STRIDE_UNION */
#define SOC_DACC_WLT_SLICE3_STRIDE_ADDR(base)                 ((base) + (0x0180))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_SLICE_ROW_UNION */
#define SOC_DACC_WLT_SLICE_ROW_ADDR(base)                     ((base) + (0x0184))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WLT_CMDLIST_REFRESH_OFFSET_UNION */
#define SOC_DACC_WLT_CMDLIST_REFRESH_OFFSET_ADDR(base)        ((base) + (0x0188))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_COUNTER_CLR_UNION */
#define SOC_DACC_OV_COUNTER_CLR_ADDR(base)                    ((base) + (0x018c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WCH_FRM_BLK_END_UNION */
#define SOC_DACC_WCH_FRM_BLK_END_ADDR(base)                   ((base) + (0x010c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_UVUP_SCENE_ASSIGN_UNION */
#define SOC_DACC_UVUP_SCENE_ASSIGN_ADDR(base)                 ((base) + (0x0110))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ARSR0_SCENE_ASSIGN_UNION */
#define SOC_DACC_ARSR0_SCENE_ASSIGN_ADDR(base)                ((base) + (0x0114))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ARSR1_SCENE_ASSIGN_UNION */
#define SOC_DACC_ARSR1_SCENE_ASSIGN_ADDR(base)                ((base) + (0x0118))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_VSCF0_SCENE_ASSIGN_UNION */
#define SOC_DACC_VSCF0_SCENE_ASSIGN_ADDR(base)                ((base) + (0x011c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_VSCF1_SCENE_ASSIGN_UNION */
#define SOC_DACC_VSCF1_SCENE_ASSIGN_ADDR(base)                ((base) + (0x0120))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_HDR_SCENE_ASSIGN_UNION */
#define SOC_DACC_HDR_SCENE_ASSIGN_ADDR(base)                  ((base) + (0x0124))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLD0_SCENE_ASSIGN_UNION */
#define SOC_DACC_CLD0_SCENE_ASSIGN_ADDR(base)                 ((base) + (0x0128))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLD1_SCENE_ASSIGN_UNION */
#define SOC_DACC_CLD1_SCENE_ASSIGN_ADDR(base)                 ((base) + (0x012c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT0_SCENE_ASSIGN_UNION */
#define SOC_DACC_SROT0_SCENE_ASSIGN_ADDR(base)                ((base) + (0x0130))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT1_SCENE_ASSIGN_UNION */
#define SOC_DACC_SROT1_SCENE_ASSIGN_ADDR(base)                ((base) + (0x0134))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DMA_SEND_AHEAD_OFFSET_UNION */
#define SOC_DACC_DMA_SEND_AHEAD_OFFSET_ADDR(base)             ((base) + (0x0138))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MASK_STA_FLAG_UNION */
#define SOC_DACC_MASK_STA_FLAG_ADDR(base)                     ((base) + (0x0300))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MASK_AREA00_START_IDX_UNION */
#define SOC_DACC_MASK_AREA00_START_IDX_ADDR(base)             ((base) + (0x0304))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MASK_AREA00_END_IDX_UNION */
#define SOC_DACC_MASK_AREA00_END_IDX_ADDR(base)               ((base) + (0x0308))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MASK_AREAFF_START_IDX_UNION */
#define SOC_DACC_MASK_AREAFF_START_IDX_ADDR(base)             ((base) + (0x030c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MASK_AREAFF_END_IDX_UNION */
#define SOC_DACC_MASK_AREAFF_END_IDX_ADDR(base)               ((base) + (0x0310))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLK_SEL_REG_UNION */
#define SOC_DACC_CLK_SEL_REG_ADDR(base, s12)                  ((base) + (0x0340+(s12)*0x8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLK_EN_REG_UNION */
#define SOC_DACC_CLK_EN_REG_ADDR(base, s12)                   ((base) + (0x0344+(s12)*0x8))

#define SOC_DACC_NEW_FRAME_RDY_CFG_ADDR(base, s12)            ((base) + (0x0404+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_EN_RELOAD_UNION */
#define SOC_DACC_CTL_SW_EN_RELOAD_ADDR(base, s12)             ((base) + (0x0408+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_EN_REG_UNION */
#define SOC_DACC_CTL_SW_EN_REG_ADDR(base, s12)                ((base) + (0x040C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_START_REG_UNION */
#define SOC_DACC_CTL_SW_START_REG_ADDR(base, s12)             ((base) + (0x0404+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_INTR_EN_REG_UNION */
#define SOC_DACC_CTL_SW_INTR_EN_REG_ADDR(base, s12)           ((base) + (0x0414+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SELF_REC_EN_REG_UNION */
#define SOC_DACC_CTL_SELF_REC_EN_REG_ADDR(base, s12)          ((base) + (0x0418+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_ST_FRM_SEL0_REG_UNION */
#define SOC_DACC_CTL_ST_FRM_SEL0_REG_ADDR(base, s12)          ((base) + (0x041C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_CFG_MODE0_UNION */
#define SOC_DACC_CTL_CFG_MODE0_ADDR(base, s12)                ((base) + (0x0424+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CFG_RD_SHADOW_UNION */
#define SOC_DACC_CFG_RD_SHADOW_ADDR(base, s12)                ((base) + (0x0428+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_CLEAR_TIMEOUT_EN_REG_UNION */
#define SOC_DACC_CTL_CLEAR_TIMEOUT_EN_REG_ADDR(base, s12)     ((base) + (0x042C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_UNION */
#define SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_ADDR(base, s12)    ((base) + (0x0388+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLR_START_REG_UNION */
#define SOC_DACC_CLR_START_REG_ADDR(base, s12)                ((base) + (0x0380+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DCMD_LAYER_INFO_OFFSET_UNION */
#define SOC_DACC_DCMD_LAYER_INFO_OFFSET_ADDR(base, s12)       ((base) + (0x0440+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLR_RISCV_START_UNION */
#define SOC_DACC_CLR_RISCV_START_ADDR(base, s12)              ((base) + (0x0384+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLR_TIMEOUT_INTS_MASK_REG_UNION */
#define SOC_DACC_CLR_TIMEOUT_INTS_MASK_REG_ADDR(base, s12)    ((base) + (0x0448+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLR_TIMEOUT_STATUS_REG_UNION */
#define SOC_DACC_CLR_TIMEOUT_STATUS_REG_ADDR(base, s12)       ((base) + (0x038C+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCMD_START_REG_UNION */
#define SOC_DACC_SCMD_START_REG_ADDR(base, s12)               ((base) + (0x0450+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DCMD_START_REG_UNION */
#define SOC_DACC_DCMD_START_REG_ADDR(base, s12)               ((base) + (0x0454+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCMD_DM_ADDR_REG_UNION */
#define SOC_DACC_SCMD_DM_ADDR_REG_ADDR(base, s12)             ((base) + (0x0458+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DCMD_DM_ADDR_REG_UNION */
#define SOC_DACC_DCMD_DM_ADDR_REG_ADDR(base, s12)             ((base) + (0x045C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCMD_NUM_REG_UNION */
#define SOC_DACC_SCMD_NUM_REG_ADDR(base, s12)                 ((base) + (0x0460+(s12)*0x400))

/* 寄存器说明：SCMD interrupt mask registers
   位域定义UNION结构:  SOC_DACC_SCMD_INTS_MASK_REG_UNION */
#define SOC_DACC_SCMD_INTS_MASK_REG_ADDR(base, s12)           ((base) + (0x0468+(s12)*0x400))

/* 寄存器说明：SCMD status
   位域定义UNION结构:  SOC_DACC_SCMD_STATUS_REG_UNION */
#define SOC_DACC_SCMD_STATUS_REG_ADDR(base, s12)              ((base) + (0x046C+(s12)*0x400))

/* 寄存器说明：DCMD interrupt mask registers
   位域定义UNION结构:  SOC_DACC_DCMD_INTS_MASK_REG_UNION */
#define SOC_DACC_DCMD_INTS_MASK_REG_ADDR(base, s12)           ((base) + (0x0474+(s12)*0x400))

/* 寄存器说明：DCMD status
   位域定义UNION结构:  SOC_DACC_DCMD_STATUS_REG_UNION */
#define SOC_DACC_DCMD_STATUS_REG_ADDR(base, s12)              ((base) + (0x0478+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_NUM_REG_UNION */
#define SOC_DACC_LAYER_NUM_REG_ADDR(base, s12)                ((base) + (0x047C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SW_CMD_REG0_UNION */
#define SOC_DACC_SW_CMD_REG0_ADDR(base, s12)                  ((base) + (0x0480+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SW_CMD_REG1_UNION */
#define SOC_DACC_SW_CMD_REG1_ADDR(base, s12)                  ((base) + (0x0484+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SW_CMD_TX_REG_UNION */
#define SOC_DACC_SW_CMD_TX_REG_ADDR(base, s12)                ((base) + (0x0488+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_DBG0_REG_UNION */
#define SOC_DACC_CTL_DBG0_REG_ADDR(base, s12)                 ((base) + (0x048C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_DBG1_REG_UNION */
#define SOC_DACC_CTL_DBG1_REG_ADDR(base, s12)                 ((base) + (0x0490+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_DBG2_REG_UNION */
#define SOC_DACC_CTL_DBG2_REG_ADDR(base, s12)                 ((base) + (0x0494+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCEN_SECURE_REG_UNION */
#define SOC_DACC_SCEN_SECURE_REG_ADDR(base, s12)              ((base) + (0x500+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCEN_SECURE_DM_ADDR_UNION */
#define SOC_DACC_SCEN_SECURE_DM_ADDR_ADDR(base, s12)          ((base) + (0x0504+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCEN_SECURE_RSV_UNION */
#define SOC_DACC_SCEN_SECURE_RSV_ADDR(base, s12)              ((base) + (0x0508+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCEN_SECURE_CMD_EN_UNION */
#define SOC_DACC_SCEN_SECURE_CMD_EN_ADDR(base, s12)           ((base) + (0x050c+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_PING_CTL0_LAYER0_SECU_UNION */
#define SOC_DACC_PING_CTL0_LAYER0_SECU_ADDR(base, s12)        ((base) + (0x0510+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_PING_CTL0_LAYER1_SECU_UNION */
#define SOC_DACC_PING_CTL0_LAYER1_SECU_ADDR(base, s12)        ((base) + (0x0514+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SEC_PAY_ABNORMAL_INT_UNION */
#define SOC_DACC_SEC_PAY_ABNORMAL_INT_ADDR(base, s12)         ((base) + (0x0518+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_DEMURA_RLST_CNT_UNION */
#define SOC_DACC_DBG_DEMURA_RLST_CNT_ADDR(base, s12)          ((base) + (0x0520+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_SCL_RLST_CNT_01_UNION */
#define SOC_DACC_DBG_SCL_RLST_CNT_01_ADDR(base, s12)          ((base) + (0x0524+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_SCL_RLST_CNT_23_UNION */
#define SOC_DACC_DBG_SCL_RLST_CNT_23_ADDR(base, s12)          ((base) + (0x0528+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_UVUP_RLST_YCNT_UNION */
#define SOC_DACC_DBG_UVUP_RLST_YCNT_ADDR(base, s12)           ((base) + (0x052c+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_CST_ALL_UNION */
#define SOC_DACC_DBG_CST_ALL_ADDR(base, s12)                  ((base) + (0x0530+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_FSM_MISC_UNION */
#define SOC_DACC_DBG_FSM_MISC_ADDR(base, s12)                 ((base) + (0x0534+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_BUSY_REQ_MISC_UNION */
#define SOC_DACC_DBG_BUSY_REQ_MISC_ADDR(base, s12)            ((base) + (0x0538+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_RESERVED_0_UNION */
#define SOC_DACC_DBG_RESERVED_0_ADDR(base, s12)               ((base) + (0x053c+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_RESERVED_1_UNION */
#define SOC_DACC_DBG_RESERVED_1_ADDR(base, s12)               ((base) + (0x0540+(s12)*0x400))


/****************************************************************************
                     (2/3) DM-0418
 ****************************************************************************/


/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_JOINT_RALATION_0_UNION */
#define SOC_DACC_JOINT_RALATION_0_ADDR(base, ly)              ((base) + (0x0200+0x80*(ly)))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_JOINT_RALATION_1_UNION */
#define SOC_DACC_JOINT_RALATION_1_ADDR(base, ly)              ((base) + (0x0204+0x80*(ly)))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_KEY_FEATURE_UNION */
#define SOC_DACC_LAYER_KEY_FEATURE_ADDR(base, ly)             ((base) + (0x0210+0x80*(ly)))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MOD_IDX_TABLE_0_UNION */
#define SOC_DACC_MOD_IDX_TABLE_0_ADDR(base, ly)               ((base) + (0x0214+0x80*(ly)))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MOD_IDX_TABLE_1_UNION */
#define SOC_DACC_MOD_IDX_TABLE_1_ADDR(base, ly)               ((base) + (0x0218+0x80*(ly)))


/****************************************************************************
                     (3/3) DCMD-0418
 ****************************************************************************/


/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_LINEREG_PARA_UNION */
#define SOC_DACC_OV_LINEREG_PARA_ADDR(base)                   ((base) + (0x0000))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_HDR_LINEREG_PARA0_UNION */
#define SOC_DACC_HDR_LINEREG_PARA0_ADDR(base)                 ((base) + (0x0004))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_HDR_LINEREG_PARA1_UNION */
#define SOC_DACC_HDR_LINEREG_PARA1_ADDR(base)                 ((base) + (0x0008))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_HDR_LINEREG_PARA2_UNION */
#define SOC_DACC_HDR_LINEREG_PARA2_ADDR(base)                 ((base) + (0x000c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_UVUP_LINEREG_PARA0_UNION */
#define SOC_DACC_UVUP_LINEREG_PARA0_ADDR(base)                ((base) + (0x0010))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_UVUP_LINEREG_PARA1_UNION */
#define SOC_DACC_UVUP_LINEREG_PARA1_ADDR(base)                ((base) + (0x0014))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_UVUP_LINEREG_PARA2_UNION */
#define SOC_DACC_UVUP_LINEREG_PARA2_ADDR(base)                ((base) + (0x0018))

/* 寄存器说明：保留寄存器
   位域定义UNION结构:  SOC_DACC_UVUP_LINEREG_PARA3_UNION */
#define SOC_DACC_UVUP_LINEREG_PARA3_ADDR(base)                ((base) + (0x001C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_BLK_WIDTH_UNION */
#define SOC_DACC_SROT_BLK_WIDTH_ADDR(base, sr)                ((base) + (0x0020+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_BLK_HEIGHT_UNION */
#define SOC_DACC_SROT_BLK_HEIGHT_ADDR(base, sr)               ((base) + (0x0024+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_BLOCK_CNT_UNION */
#define SOC_DACC_SROT_BLOCK_CNT_ADDR(base, sr)                ((base) + (0x0028+(sr)*0x20))

/* 寄存器说明：保留寄存器
   位域定义UNION结构:  SOC_DACC_SROT_RESERVED_UNION */
#define SOC_DACC_SROT_RESERVED_ADDR(base, sr)                 ((base) + (0x002c+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_LINEREG_PARA0_UNION */
#define SOC_DACC_SROT_LINEREG_PARA0_ADDR(base, sr)            ((base) + (0x0030+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_LINEREG_PARA1_UNION */
#define SOC_DACC_SROT_LINEREG_PARA1_ADDR(base, sr)            ((base) + (0x0034+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_LINEREG_PARA2_UNION */
#define SOC_DACC_SROT_LINEREG_PARA2_ADDR(base, sr)            ((base) + (0x0038+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SROT_LINEREG_PARA3_UNION */
#define SOC_DACC_SROT_LINEREG_PARA3_ADDR(base, sr)            ((base) + (0x003c+(sr)*0x20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLD_LINEREG_PARA0_UNION */
#define SOC_DACC_CLD_LINEREG_PARA0_ADDR(base, cl)             ((base) + (0x0060+(cl)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLD_LINEREG_PARA1_UNION */
#define SOC_DACC_CLD_LINEREG_PARA1_ADDR(base, cl)             ((base) + (0x0064+(cl)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLD_LINEREG_PARA2_UNION */
#define SOC_DACC_CLD_LINEREG_PARA2_ADDR(base, cl)             ((base) + (0x0068+(cl)*0x10))

/* 寄存器说明：保留寄存器
   位域定义UNION结构:  SOC_DACC_CLD_RESERVED_UNION */
#define SOC_DACC_CLD_RESERVED_ADDR(base, cl)                  ((base) + (0x006c+(cl)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCL_LINEREG_PARA0_UNION */
#define SOC_DACC_SCL_LINEREG_PARA0_ADDR(base, scl)            ((base) + (0x0080+(scl)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCL_LINEREG_PARA1_UNION */
#define SOC_DACC_SCL_LINEREG_PARA1_ADDR(base, scl)            ((base) + (0x0084+(scl)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCL_LINEREG_PARA2_UNION */
#define SOC_DACC_SCL_LINEREG_PARA2_ADDR(base, scl)            ((base) + (0x0088+(scl)*0x10))

/* 寄存器说明：保留寄存器
   位域定义UNION结构:  SOC_DACC_SCL_RESERVED_UNION */
#define SOC_DACC_SCL_RESERVED_ADDR(base, scl)                 ((base) + (0x008c+(scl)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA0_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA0_ADDR(base, ly)           ((base) + (0x00d0+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA1_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA1_ADDR(base, ly)           ((base) + (0x00d4+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA2_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA2_ADDR(base, ly)           ((base) + (0x00d8+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA3_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA3_ADDR(base, ly)           ((base) + (0x00dc+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA4_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA4_ADDR(base, ly)           ((base) + (0x00e0+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA5_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA5_ADDR(base, ly)           ((base) + (0x00e4+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA6_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA6_ADDR(base, ly)           ((base) + (0x00e8+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA7_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA7_ADDR(base, ly)           ((base) + (0x00ec+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA8_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA8_ADDR(base, ly)           ((base) + (0x00f0+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA9_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA9_ADDR(base, ly)           ((base) + (0x00f4+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA10_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA10_ADDR(base, ly)          ((base) + (0x00f8+(ly)*0x38))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LINEREG_PARA11_UNION */
#define SOC_DACC_LAYER_LINEREG_PARA11_ADDR(base, ly)          ((base) + (0x00fc+(ly)*0x38))


/*****************************************************************************
  3 枚举定义
*****************************************************************************/



/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/



/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/




/*****************************************************************************
  7 UNION定义
*****************************************************************************/
/****************************************************************************
                     (1/3) CFG
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_SW_EXT_INT_REG_UNION
 结构说明  : SW_EXT_INT_REG 寄存器结构定义。地址偏移量:0x0000，初值:0x00000000，宽度:32
 寄存器说明: SW interrupt registers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_ext_ints : 7;  /* bit[0-6] : 软件外发中断寄存器（实际不使用） */
        unsigned int  reserved    : 25; /* bit[7-31]:  */
    } reg;
} SOC_DACC_SW_EXT_INT_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_EXT_INT_MASK_REG_UNION
 结构说明  : SW_EXT_INT_MASK_REG 寄存器结构定义。地址偏移量:0x0004，初值:0x0000007F，宽度:32
 寄存器说明: SW interrupt mask registers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_ext_ints_mask : 7;  /* bit[0-6] : 软件外发中断MASK寄存器（实际不使用） */
        unsigned int  reserved         : 25; /* bit[7-31]:  */
    } reg;
} SOC_DACC_SW_EXT_INT_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_POR_RESET_PC_REG_UNION
 结构说明  : POR_RESET_PC_REG 寄存器结构定义。地址偏移量:0x0008，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  core_rst_pc : 32; /* bit[0-31]: MiDeer reest pc */
    } reg;
} SOC_DACC_POR_RESET_PC_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CORE_CONFIG_REG_UNION
 结构说明  : CORE_CONFIG_REG 寄存器结构定义。地址偏移量:0x000C，初值:0x00000010，宽度:32
 寄存器说明: RISC-V CORE
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  core_hart_id : 4;  /* bit[0-3] : MiDeer Core ID */
        unsigned int  core_wait    : 1;  /* bit[4-4] : 1:Core 等待； 0:CORE 正常工作模式； */
        unsigned int  reserved     : 27; /* bit[5-31]: reserved */
    } reg;
} SOC_DACC_CORE_CONFIG_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CORE_STATUS_REG_UNION
 结构说明  : CORE_STATUS_REG 寄存器结构定义。地址偏移量:0x0010，初值:0x00000000，宽度:32
 寄存器说明: RISC-V CORE
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  core_sleep_mode      : 1;  /* bit[0]   : 1: in sleep mode 0: not in sleep mode */
        unsigned int  core_debug_mode      : 1;  /* bit[1]   : 1: in debug mode 0: not in debug mode */
        unsigned int  core_hard_fault_mode : 1;  /* bit[2]   : 1: in hard fault 0: not in hard fault */
        unsigned int  core_in_nmi_handle   : 1;  /* bit[3]   : 1: in nmi handle 0: not in nmi handle */
        unsigned int  reserved             : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_CORE_STATUS_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CORE_MON_PC_REG_UNION
 结构说明  : CORE_MON_PC_REG 寄存器结构定义。地址偏移量:0x0014，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  core_mon_pc : 32; /* bit[0-31]: dbg寄存器 */
    } reg;
} SOC_DACC_CORE_MON_PC_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CORE_MON_LR_REG_UNION
 结构说明  : CORE_MON_LR_REG 寄存器结构定义。地址偏移量:0x0018，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  core_mon_lr : 32; /* bit[0-31]: dbg寄存器 */
    } reg;
} SOC_DACC_CORE_MON_LR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_LDI_SEL0_REG_UNION
 结构说明  : CTL_LDI_SEL0_REG 寄存器结构定义。地址偏移量:0x001C，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_ldi_sel0 : 3;  /* bit[0-2] : ldi0中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_LDI_SEL0_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_LDI_SEL1_REG_UNION
 结构说明  : CTL_LDI_SEL1_REG 寄存器结构定义。地址偏移量:0x0020，初值:0x00000001，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_ldi_sel1 : 3;  /* bit[0-2] : ldi1中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_LDI_SEL1_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_LDI_SEL2_REG_UNION
 结构说明  : CTL_LDI_SEL2_REG 寄存器结构定义。地址偏移量:0x0024，初值:0x00000002，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_ldi_sel2 : 3;  /* bit[0-2] : ldi2中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_LDI_SEL2_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_LDI_SEL3_REG_UNION
 结构说明  : CTL_LDI_SEL3_REG 寄存器结构定义。地址偏移量:0x0028，初值:0x00000003，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_ldi_sel3 : 3;  /* bit[0-2] : ldi3中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_LDI_SEL3_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_WCH_SEL0_UNION
 结构说明  : CTL_WCH_SEL0 寄存器结构定义。地址偏移量:0x002C，初值:0x00000004，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_wch_sel0 : 3;  /* bit[0-2] : wch0中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_WCH_SEL0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_WCH_SEL1_UNION
 结构说明  : CTL_WCH_SEL1 寄存器结构定义。地址偏移量:0x0030，初值:0x00000005，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_wch_sel1 : 3;  /* bit[0-2] : wch1中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_WCH_SEL1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_WCH_SEL2_UNION
 结构说明  : CTL_WCH_SEL2 寄存器结构定义。地址偏移量:0x0034，初值:0x00000006，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_wch_sel2 : 3;  /* bit[0-2] : wch2中断对应的场景号； */
        unsigned int  reserved     : 29; /* bit[3-31]:  */
    } reg;
} SOC_DACC_CTL_WCH_SEL2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_REG0_UNION
 结构说明  : DBG_REG0 寄存器结构定义。地址偏移量:0x0038，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg : 32; /* bit[0-31]: dbg观测寄存器 */
    } reg;
} SOC_DACC_DBG_REG0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_SEL_REG_UNION
 结构说明  : DBG_SEL_REG 寄存器结构定义。地址偏移量:0x003C，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_sel : 32; /* bit[0-31]: dbg选择寄存器 */
    } reg;
} SOC_DACC_DBG_SEL_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MINIBUS_1T01_CKG_EN_UNION
 结构说明  : MINIBUS_1T01_CKG_EN 寄存器结构定义。地址偏移量:0x0040，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  p2h_ckg_en : 1;  /* bit[0]   :  */
        unsigned int  h2p_ckg_en : 1;  /* bit[1]   : h2p门控使能 */
        unsigned int  reserved   : 30; /* bit[2-31]:  */
    } reg;
} SOC_DACC_MINIBUS_1T01_CKG_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MINIBUS_1T01_STATE_UNION
 结构说明  : MINIBUS_1T01_STATE 寄存器结构定义。地址偏移量:0x0044，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  p2h_idle : 1;  /* bit[0]   :  */
        unsigned int  h2p_idle : 1;  /* bit[1]   : 状态 */
        unsigned int  reserved : 30; /* bit[2-31]:  */
    } reg;
} SOC_DACC_MINIBUS_1T01_STATE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CKG_EN_UNION
 结构说明  : CKG_EN 寄存器结构定义。地址偏移量:0x0048，初值:0x000000FF，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ckg_en   : 8;  /* bit[0-7] : 门控 */
        unsigned int  reserved : 24; /* bit[8-31]:  */
    } reg;
} SOC_DACC_CKG_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_EN_UNION
 结构说明  : WLT_EN 寄存器结构定义。地址偏移量:0x0060，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_en       : 1;  /* bit[0-0]  : 低延迟模式使能 */
        unsigned int  reserved_0   : 7;  /* bit[1-7]  : 保留 */
        unsigned int  wlt_layer_id : 5;  /* bit[8-12] : 低延迟模式图层ID */
        unsigned int  reserved_1   : 19; /* bit[13-31]: 保留 */
    } reg;
} SOC_DACC_WLT_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE0_DDR_ADDR0 寄存器结构定义。地址偏移量:0x0064，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_ddr_addr0 : 32; /* bit[0-31]: Slice0地址信息在DDR中的存放地址低32位 */
    } reg;
} SOC_DACC_WLT_SLICE0_DDR_ADDR0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_DDR_ADDR1_UNION
 结构说明  : WLT_SLICE0_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0068，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_ddr_addr1 : 4;  /* bit[0-3] : Slice0地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE0_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE1_DDR_ADDR0 寄存器结构定义。地址偏移量:0x006c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_ddr_addr0 : 32; /* bit[0-31]: Slice1地址信息在DDR中的存放地址低32位 */
    } reg;
} SOC_DACC_WLT_SLICE1_DDR_ADDR0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_DDR_ADDR1_UNION
 结构说明  : WLT_SLICE1_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0070，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_ddr_addr1 : 4;  /* bit[0-3] : Slice1地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE1_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE2_DDR_ADDR0 寄存器结构定义。地址偏移量:0x0074，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_ddr_addr0 : 32; /* bit[0-31]: Slice2地址信息在DDR中的存放地址低32位 */
    } reg;
} SOC_DACC_WLT_SLICE2_DDR_ADDR0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_DDR_ADDR1_UNION
 结构说明  : WLT_SLICE2_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0078，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_ddr_addr1 : 4;  /* bit[0-3] : Slice2地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE2_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE3_DDR_ADDR0 寄存器结构定义。地址偏移量:0x007c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_ddr_addr0 : 32; /* bit[0-31]: Slice3地址信息在DDR中的存放地址低32位 */
    } reg;
} SOC_DACC_WLT_SLICE3_DDR_ADDR0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_DDR_ADDR1_UNION
 结构说明  : WLT_SLICE3_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0080，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_ddr_addr1 : 4;  /* bit[0-3] : Slice3地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE3_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR0_H_UNION
 结构说明  : WLT_SLICE0_ADDR0_H 寄存器结构定义。地址偏移量:0x0084，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr0_h : 4;  /* bit[0-3]  : Slice0压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址高4bit */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : 保留 */
        unsigned int  wlt_slice0_addr1_h : 4;  /* bit[8-11] : Slice0压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址高4bit */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: 保留 */
        unsigned int  wlt_slice0_addr2_h : 4;  /* bit[16-19]: Slice0压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址高4bit */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: 保留 */
        unsigned int  wlt_slice0_addr3_h : 4;  /* bit[24-27]: Slice0压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR0_L_UNION
 结构说明  : WLT_SLICE0_ADDR0_L 寄存器结构定义。地址偏移量:0x0088，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr0_l : 32; /* bit[0-31]: Slice0压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR1_L_UNION
 结构说明  : WLT_SLICE0_ADDR1_L 寄存器结构定义。地址偏移量:0x008c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr1_l : 32; /* bit[0-31]: Slice0压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR2_L_UNION
 结构说明  : WLT_SLICE0_ADDR2_L 寄存器结构定义。地址偏移量:0x0094，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr2_l : 32; /* bit[0-31]: Slice0压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR3_L_UNION
 结构说明  : WLT_SLICE0_ADDR3_L 寄存器结构定义。地址偏移量:0x009c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr3_l : 32; /* bit[0-31]: Slice0压缩图层：图层C平面 header起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR3_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR0_H_UNION
 结构说明  : WLT_SLICE1_ADDR0_H 寄存器结构定义。地址偏移量:0x00a4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr0_h : 4;  /* bit[0-3]  : Slice1压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址高4bit */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : 保留 */
        unsigned int  wlt_slice1_addr1_h : 4;  /* bit[8-11] : Slice1压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址高4bit */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: 保留 */
        unsigned int  wlt_slice1_addr2_h : 4;  /* bit[16-19]: Slice1压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址高4bit */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: 保留 */
        unsigned int  wlt_slice1_addr3_h : 4;  /* bit[24-27]: Slice1压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR0_L_UNION
 结构说明  : WLT_SLICE1_ADDR0_L 寄存器结构定义。地址偏移量:0x00a8，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr0_l : 32; /* bit[0-31]: Slice1压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR1_L_UNION
 结构说明  : WLT_SLICE1_ADDR1_L 寄存器结构定义。地址偏移量:0x00ac，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr1_l : 32; /* bit[0-31]: Slice1压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR2_L_UNION
 结构说明  : WLT_SLICE1_ADDR2_L 寄存器结构定义。地址偏移量:0x00b4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr2_l : 32; /* bit[0-31]: Slice1压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR3_L_UNION
 结构说明  : WLT_SLICE1_ADDR3_L 寄存器结构定义。地址偏移量:0x00bc，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr3_l : 32; /* bit[0-31]: Slice1压缩图层：图层C平面 header起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR3_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR0_H_UNION
 结构说明  : WLT_SLICE2_ADDR0_H 寄存器结构定义。地址偏移量:0x00c4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr0_h : 4;  /* bit[0-3]  : Slice2压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址高4bit */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : 保留 */
        unsigned int  wlt_slice2_addr1_h : 4;  /* bit[8-11] : Slice2压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址高4bit */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: 保留 */
        unsigned int  wlt_slice2_addr2_h : 4;  /* bit[16-19]: Slice2压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址高4bit */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: 保留 */
        unsigned int  wlt_slice2_addr3_h : 4;  /* bit[24-27]: Slice2压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR0_L_UNION
 结构说明  : WLT_SLICE2_ADDR0_L 寄存器结构定义。地址偏移量:0x00c8，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr0_l : 32; /* bit[0-31]: Slice2压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR1_L_UNION
 结构说明  : WLT_SLICE2_ADDR1_L 寄存器结构定义。地址偏移量:0x00cc，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr1_l : 32; /* bit[0-31]: Slice2压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR2_L_UNION
 结构说明  : WLT_SLICE2_ADDR2_L 寄存器结构定义。地址偏移量:0x00d4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr2_l : 32; /* bit[0-31]: Slice2压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR3_L_UNION
 结构说明  : WLT_SLICE2_ADDR3_L 寄存器结构定义。地址偏移量:0x00dc，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr3_l : 32; /* bit[0-31]: Slice2压缩图层：图层C平面 header起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR3_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR0_H_UNION
 结构说明  : WLT_SLICE3_ADDR0_H 寄存器结构定义。地址偏移量:0x00e4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr0_h : 4;  /* bit[0-3]  : Slice3压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址高4bit */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : 保留 */
        unsigned int  wlt_slice3_addr1_h : 4;  /* bit[8-11] : Slice3压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址高4bit */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: 保留 */
        unsigned int  wlt_slice3_addr2_h : 4;  /* bit[16-19]: Slice3压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址高4bit */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: 保留 */
        unsigned int  wlt_slice3_addr3_h : 4;  /* bit[24-27]: Slice3压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR0_L_UNION
 结构说明  : WLT_SLICE3_ADDR0_L 寄存器结构定义。地址偏移量:0x00e8，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr0_l : 32; /* bit[0-31]: Slice3压缩图层：图层Y平面/RGB payload起始地址，非压缩图层平面0起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR1_L_UNION
 结构说明  : WLT_SLICE3_ADDR1_L 寄存器结构定义。地址偏移量:0x00ec，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr1_l : 32; /* bit[0-31]: Slice3压缩图层：图层Y平面/RGB header起始地址，非压缩图层平面1起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR2_L_UNION
 结构说明  : WLT_SLICE3_ADDR2_L 寄存器结构定义。地址偏移量:0x00f4，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr2_l : 32; /* bit[0-31]: Slice3压缩图层：图层C平面 payload起始地址，非压缩图层平面2起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR3_L_UNION
 结构说明  : WLT_SLICE3_ADDR3_L 寄存器结构定义。地址偏移量:0x00fc，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr3_l : 32; /* bit[0-31]: Slice3压缩图层：图层C平面 header起始地址低32bit */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR3_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE_ROW_UNION
 结构说明  : WLT_SLICE_ROW 寄存器结构定义。地址偏移量:0x0104，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice_row : 32; /* bit[0-31]: 原始图层中，每个slice的行数 */
    } reg;
} SOC_DACC_WLT_SLICE_ROW_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_CMDLIST_REFRESH_OFFSET_UNION
 结构说明  : WLT_CMDLIST_REFRESH_OFFSET 寄存器结构定义。地址偏移量:0x0108，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_cmdlist_refresh_offset : 32; /* bit[0-31]: 原始图层中，发送Cmdlist指令时相比每个slice第一行的提前行数 */
    } reg;
} SOC_DACC_WLT_CMDLIST_REFRESH_OFFSET_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WCH_FRM_BLK_END_UNION
 结构说明  : WCH_FRM_BLK_END 寄存器结构定义。地址偏移量:0x010c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wch_blk_end : 3;  /* bit[0-2] : 3bit中断状态信号，写1清，dacc外部输入到riscv */
        unsigned int  wch_frm_end : 3;  /* bit[3-5] : 3bit中断状态信号，写1清，dacc外部输入到riscv */
        unsigned int  reserved    : 26; /* bit[6-31]:  */
    } reg;
} SOC_DACC_WCH_FRM_BLK_END_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP_SCENE_ASSIGN_UNION
 结构说明  : UVUP_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0110，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uvup_scene_assign : 3;  /* bit[0-2] : [2:0]: UVUP分配场景ID */
        unsigned int  reserved          : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_UVUP_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ARSR0_SCENE_ASSIGN_UNION
 结构说明  : ARSR0_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0114，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  arsr0_scene_assign : 3;  /* bit[0-2] : [2:0]: ARSR0分配场景ID */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_ARSR0_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ARSR1_SCENE_ASSIGN_UNION
 结构说明  : ARSR1_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0118，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  arsr1_scene_assign : 3;  /* bit[0-2] : [2:0]: ARSR1分配场景ID */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_ARSR1_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_VSCF0_SCENE_ASSIGN_UNION
 结构说明  : VSCF0_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x011c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vscf0_scene_assign : 3;  /* bit[0-2] : [2:0]: VSCF0分配场景ID */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_VSCF0_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_VSCF1_SCENE_ASSIGN_UNION
 结构说明  : VSCF1_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0120，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vscf1_scene_assign : 3;  /* bit[0-2] : [2:0]: VSCF1分配场景ID */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_VSCF1_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_HDR_SCENE_ASSIGN_UNION
 结构说明  : HDR_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0124，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  hdr_scene_assign : 3;  /* bit[0-2] : [2:0]: HDR分配场景ID */
        unsigned int  reserved         : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_HDR_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD0_SCENE_ASSIGN_UNION
 结构说明  : CLD0_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0128，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld0_scene_assign : 3;  /* bit[0-2] : [2:0]: CLD0分配场景ID */
        unsigned int  reserved          : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_CLD0_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD1_SCENE_ASSIGN_UNION
 结构说明  : CLD1_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x012c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld1_scene_assign : 3;  /* bit[0-2] : [2:0]: CLD1分配场景ID */
        unsigned int  reserved          : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_CLD1_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT0_SCENE_ASSIGN_UNION
 结构说明  : SROT0_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0130，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot0_scene_assign : 3;  /* bit[0-2] : [2:0]: SROT0分配场景ID */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_SROT0_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT1_SCENE_ASSIGN_UNION
 结构说明  : SROT1_SCENE_ASSIGN 寄存器结构定义。地址偏移量:0x0134，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot1_scene_assign : 3;  /* bit[0-2] : [2:0]: SROT1分配场景ID */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_SROT1_SCENE_ASSIGN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DMA_SEND_AHEAD_OFFSET_UNION
 结构说明  : DMA_SEND_AHEAD_OFFSET 寄存器结构定义。地址偏移量:0x0138，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_send_ahead_offset : 3;  /* bit[0-2] : DMA指令提前下发行偏移（不再使用） */
        unsigned int  reserved              : 29; /* bit[3-31]: 保留（不再使用） */
    } reg;
} SOC_DACC_DMA_SEND_AHEAD_OFFSET_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MASK_STA_FLAG_UNION
 结构说明  : MASK_STA_FLAG 寄存器结构定义。地址偏移量:0x0300，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mask_sta_flag00 : 1;  /* bit[0]   : 统计结果，如果所有alpha均为00，此寄存器为1，否则为0，ROI模式时，代表填充的数据是否为全透明，1是0否 */
        unsigned int  mask_sta_flagff : 1;  /* bit[1]   : 统计结果，如果所有alpha均为FF，此寄存器为1，否则为0，ROI模式时，代表填充的数据是否为全透明，1是0否 */
        unsigned int  reserved        : 30; /* bit[2-31]:  */
    } reg;
} SOC_DACC_MASK_STA_FLAG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MASK_AREA00_START_IDX_UNION
 结构说明  : MASK_AREA00_START_IDX 寄存器结构定义。地址偏移量:0x0304，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mask_area00_start_idx : 22; /* bit[0-21] : 分量最大连续00的区域起始坐标 */
        unsigned int  reserved              : 10; /* bit[22-31]:  */
    } reg;
} SOC_DACC_MASK_AREA00_START_IDX_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MASK_AREA00_END_IDX_UNION
 结构说明  : MASK_AREA00_END_IDX 寄存器结构定义。地址偏移量:0x0308，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mask_area00_end_idx : 22; /* bit[0-21] : 分量最大连续00的区域结束坐标 */
        unsigned int  reserved            : 10; /* bit[22-31]:  */
    } reg;
} SOC_DACC_MASK_AREA00_END_IDX_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MASK_AREAFF_START_IDX_UNION
 结构说明  : MASK_AREAFF_START_IDX 寄存器结构定义。地址偏移量:0x030c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mask_areaff_start_idx : 22; /* bit[0-21] : 分量最大连续FF的区域起始坐标 */
        unsigned int  reserved              : 10; /* bit[22-31]:  */
    } reg;
} SOC_DACC_MASK_AREAFF_START_IDX_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MASK_AREAFF_END_IDX_UNION
 结构说明  : MASK_AREAFF_END_IDX 寄存器结构定义。地址偏移量:0x0310，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mask_areaff_end_idx : 22; /* bit[0-21] : 分量最大连续FF的区域结束坐标 */
        unsigned int  reserved            : 10; /* bit[22-31]:  */
    } reg;
} SOC_DACC_MASK_AREAFF_END_IDX_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLK_SEL_REG_UNION
 结构说明  : CLK_SEL_REG 寄存器结构定义。地址偏移量:0x0400+(s12)*0x400，初值:0x00000001，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dacc_clk_sel : 1;  /* bit[0-0] : 场景时钟选择寄存器 */
        unsigned int  reserved     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CLK_SEL_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLK_EN_REG_UNION
 结构说明  : CLK_EN_REG 寄存器结构定义。地址偏移量:0x0404+(s12)*0x400，初值:0x00000001，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dacc_clk_en : 1;  /* bit[0-0] : 场景时钟使能寄存器 */
        unsigned int  reserved    : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CLK_EN_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_SW_EN_RELOAD_UNION
 结构说明  : CTL_SW_EN_RELOAD 寄存器结构定义。地址偏移量:0x0408+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_sw_reload_en : 1;  /* bit[0-0] : 场景 reload使能 */
        unsigned int  reserved         : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CTL_SW_EN_RELOAD_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_SW_EN_REG_UNION
 结构说明  : CTL_SW_EN_REG 寄存器结构定义。地址偏移量:0x040C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_sw_en : 1;  /* bit[0-0] : 1'h0: 使用ping区域寄存器
                                                    1'h1：使用pong区域寄存器 */
        unsigned int  reserved  : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CTL_SW_EN_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_SW_START_REG_UNION
 结构说明  : CTL_SW_START_REG 寄存器结构定义。地址偏移量:0x0410+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_sw_start : 1;  /* bit[0-0] : 软件配置START中断
                                                       只有场景4~6才有效，该信号接到RISC-V的中断上，中断号为61~63 */
        unsigned int  reserved     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CTL_SW_START_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_SW_INTR_EN_REG_UNION
 结构说明  : CTL_SW_INTR_EN_REG 寄存器结构定义。地址偏移量:0x0414+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_sw_intr_en : 1;  /* bit[0-0] : 软件中断选择使能（实际未使用） */
        unsigned int  reserved       : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CTL_SW_INTR_EN_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_SELF_REC_EN_REG_UNION
 结构说明  : CTL_SELF_REC_EN_REG 寄存器结构定义。地址偏移量:0x0418+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_self_rec_en : 1;  /* bit[0-0] : 自恢复功能使能（实际未使用） */
        unsigned int  reserved        : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CTL_SELF_REC_EN_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_ST_FRM_SEL0_REG_UNION
 结构说明  : CTL_ST_FRM_SEL0_REG 寄存器结构定义。地址偏移量:0x041C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_st_frm_sel : 1;  /* bit[0-0] : vsync选择:（实际未使用）
                                                         0: ldi_frame_start;
                                                         1: ldi_frame_end; */
        unsigned int  reserved       : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CTL_ST_FRM_SEL0_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_CFG_MODE0_UNION
 结构说明  : CTL_CFG_MODE0 寄存器结构定义。地址偏移量:0x0424+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_cfg_mode : 1;  /* bit[0-0] : 配置机制标识：
                                                       0: CPU配置机制；
                                                       1：CMDLIST配置机制； */
        unsigned int  reserved     : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CTL_CFG_MODE0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CFG_RD_SHADOW_UNION
 结构说明  : CFG_RD_SHADOW 寄存器结构定义。地址偏移量:0x0428+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cfg_rd_shadow_en : 1;  /* bit[0-0] : Reload机制
                                                           0：读工作寄存器
                                                           1：读影子寄存器 */
        unsigned int  reserved         : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CFG_RD_SHADOW_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_CLEAR_TIMEOUT_EN_REG_UNION
 结构说明  : CTL_CLEAR_TIMEOUT_EN_REG 寄存器结构定义。地址偏移量:0x042C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_clear_timeout_en : 1;  /* bit[0-0] : 清场超时功能使能，如果使能该bit，RISC-V对ACPU下发的清场不用响应（实际未使用） */
        unsigned int  reserved             : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CTL_CLEAR_TIMEOUT_EN_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_UNION
 结构说明  : CTL_CLEAR_TIMEOUT_THR_REG 寄存器结构定义。地址偏移量:0x0430+(s12)*0x400，初值:0x00000140，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_clear_timeout_thr : 32; /* bit[0-31]: 场景清场超时阈值，19.2MHz时钟的个数，如超时时间设置100ms，该值应该设置为100000us/52ns=1923000 */
    } reg;
} SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLR_START_REG_UNION
 结构说明  : CLR_START_REG 寄存器结构定义。地址偏移量:0x0434+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  clr_start : 1;  /* bit[0-0] : （RISCV）启动DCMD/SCMD清场寄存器 */
        unsigned int  reserved  : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CLR_START_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DCMD_LAYER_INFO_OFFSET_UNION
 结构说明  : DCMD_LAYER_INFO_OFFSET 寄存器结构定义。地址偏移量:0x0440+(s12)*0x400，初值:0x000001A0，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcmd_first_layer_info_offset : 32; /* bit[0-31]: 和0x454寄存器配套使用，454寄存器提供基址，该寄存器提供基于改基址的offset，默认值为32`d416，请勿修改（实际未使用） */
    } reg;
} SOC_DACC_DCMD_LAYER_INFO_OFFSET_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLR_RISCV_START_UNION
 结构说明  : CLR_RISCV_START 寄存器结构定义。地址偏移量:0x0444+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  clr_riscv_start : 1;  /* bit[0-0] : （ACPU）启动RISCV清场寄存器，该使能有效后，RISCV进行判断指令的完整性，然后配置0x434，启动加速器相关的清场使能 */
        unsigned int  reserved        : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CLR_RISCV_START_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLR_TIMEOUT_INTS_MASK_REG_UNION
 结构说明  : CLR_TIMEOUT_INTS_MASK_REG 寄存器结构定义。地址偏移量:0x0448+(s12)*0x400，初值:0x00000001，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  clr_timeout_ints_mask : 1;  /* bit[0-0] : 清场超时中断MASK信号，1表示Mask,0表示不Mask
                                                                只MASK送到ACPU的中断，不MASK到RISCV的中断 */
        unsigned int  reserved              : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CLR_TIMEOUT_INTS_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLR_TIMEOUT_STATUS_REG_UNION
 结构说明  : CLR_TIMEOUT_STATUS_REG 寄存器结构定义。地址偏移量:0x044C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  clr_ack_timeout_status : 1;  /* bit[0-0] : 清场超时或有ACK响应状态，写1清 */
        unsigned int  clr_ack_status         : 1;  /* bit[1-1] : 清场ACK响应状态，写1请，便于ACPU查询 */
        unsigned int  reserved               : 30; /* bit[2-31]:  */
    } reg;
} SOC_DACC_CLR_TIMEOUT_STATUS_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCMD_START_REG_UNION
 结构说明  : SCMD_START_REG 寄存器结构定义。地址偏移量:0x0450+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scmd_start : 1;  /* bit[0-0] : scmd模块启动信号 */
        unsigned int  reserved   : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_SCMD_START_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DCMD_START_REG_UNION
 结构说明  : DCMD_START_REG 寄存器结构定义。地址偏移量:0x0454+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcmd_start : 1;  /* bit[0-0] : dcmd模块启动信号 */
        unsigned int  reserved   : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_DCMD_START_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCMD_DM_ADDR_REG_UNION
 结构说明  : SCMD_DM_ADDR_REG 寄存器结构定义。地址偏移量:0x0458+(s12)*0x400，初值:0x00007400，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scmd_addr : 32; /* bit[0-31]: scmd读取描述符（位于DM内）在DACC空间的偏移地址 */
    } reg;
} SOC_DACC_SCMD_DM_ADDR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DCMD_DM_ADDR_REG_UNION
 结构说明  : DCMD_DM_ADDR_REG 寄存器结构定义。地址偏移量:0x045C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcmd_addr : 32; /* bit[0-31]: dcmd需要的顶层软件接口数据在DACC空间的偏移地址 */
    } reg;
} SOC_DACC_DCMD_DM_ADDR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCMD_NUM_REG_UNION
 结构说明  : SCMD_NUM_REG 寄存器结构定义。地址偏移量:0x0460+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scmd_trans_num : 12; /* bit[0-11] : scmd需要读取描述符个个数 */
        unsigned int  reserved       : 20; /* bit[12-31]: reserved */
    } reg;
} SOC_DACC_SCMD_NUM_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCMD_INTS_MASK_REG_UNION
 结构说明  : SCMD_INTS_MASK_REG 寄存器结构定义。地址偏移量:0x0468+(s12)*0x400，初值:0x00000001，宽度:32
 寄存器说明: SCMD interrupt mask registers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scmd_ints_mask : 1;  /* bit[0-0] : SCMD完成中断MASK信号，1表示Mask,0表示不Mask
                                                         只MASK送到ACPU的中断，不MASK到RISCV的中断 */
        unsigned int  reserved       : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_SCMD_INTS_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCMD_STATUS_REG_UNION
 结构说明  : SCMD_STATUS_REG 寄存器结构定义。地址偏移量:0x046C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: SCMD status
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scmd_status : 1;  /* bit[0-0] : SCMD模块完成指示信号，为1表示本场景指令发送完成，写1清 */
        unsigned int  reserved    : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_SCMD_STATUS_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DCMD_INTS_MASK_REG_UNION
 结构说明  : DCMD_INTS_MASK_REG 寄存器结构定义。地址偏移量:0x0474+(s12)*0x400，初值:0x00000001，宽度:32
 寄存器说明: DCMD interrupt mask registers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcmd_ints_mask : 1;  /* bit[0-0] : DCMD完成中断MASK信号，1表示Mask,0表示不Mask
                                                         只MASK送到ACPU的中断，不MASK到RISCV的中断 */
        unsigned int  reserved       : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_DCMD_INTS_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DCMD_STATUS_REG_UNION
 结构说明  : DCMD_STATUS_REG 寄存器结构定义。地址偏移量:0x0478+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: DCMD status
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcmd_status : 1;  /* bit[0-0] : DCMD模块完成指示信号，为1表示本场景指令发送完成，写1清 */
        unsigned int  reserved    : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_DCMD_STATUS_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_NUM_REG_UNION
 结构说明  : LAYER_NUM_REG 寄存器结构定义。地址偏移量:0x047C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_num : 5;  /* bit[0-4] : 当前场景的layer数 */
        unsigned int  reserved  : 27; /* bit[5-31]: reserved */
    } reg;
} SOC_DACC_LAYER_NUM_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_CMD_REG0_UNION
 结构说明  : SW_CMD_REG0 寄存器结构定义。地址偏移量:0x0480+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_cmd_reg0 : 32; /* bit[0-31]: 描述符寄存器， 存描述符低32bit */
    } reg;
} SOC_DACC_SW_CMD_REG0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_CMD_REG1_UNION
 结构说明  : SW_CMD_REG1 寄存器结构定义。地址偏移量:0x0484+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_cmd_reg1 : 32; /* bit[0-31]: 描述符寄存器， 存描述符高32bit */
    } reg;
} SOC_DACC_SW_CMD_REG1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_CMD_TX_REG_UNION
 结构说明  : SW_CMD_TX_REG 寄存器结构定义。地址偏移量:0x0488+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_cmd_tx_ena : 1;  /* bit[0-0] : 描述符寄存器发送使能 */
        unsigned int  reserved      : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_SW_CMD_TX_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_DBG0_REG_UNION
 结构说明  : CTL_DBG0_REG 寄存器结构定义。地址偏移量:0x048C+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_dbg0 : 32; /* bit[0-31]: （实际未使用） */
    } reg;
} SOC_DACC_CTL_DBG0_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_DBG1_REG_UNION
 结构说明  : CTL_DBG1_REG 寄存器结构定义。地址偏移量:0x0490+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_dbg1 : 32; /* bit[0-31]: （实际未使用） */
    } reg;
} SOC_DACC_CTL_DBG1_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_DBG2_REG_UNION
 结构说明  : CTL_DBG2_REG 寄存器结构定义。地址偏移量:0x0494+(s12)*0x400，初值:0x00000005，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  q_aempty : 1;  /* bit[0-0] : Q 将空标志 */
        unsigned int  q_afull  : 1;  /* bit[1-1] : Q 将满标志 */
        unsigned int  q_empty  : 1;  /* bit[2-2] : Q 空标志 */
        unsigned int  q_full   : 1;  /* bit[3-3] : Q 满标志 */
        unsigned int  reserved : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_CTL_DBG2_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCEN_SECURE_REG_UNION
 结构说明  : SCEN_SECURE_REG 寄存器结构定义。地址偏移量:0x500+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scen_sec_en            : 2;  /* bit[0-1]  : 是否有安全支付图层：
                                                                  0：无安全图层：
                                                                  1：有1个安全图层，放在index0的位置；
                                                                  2：有1个安全图层，放在index1的位置；
                                                                  3：有2个安全图层，安全图层0放index0位置，安全图层1放index1的位置； */
        unsigned int  secure_index0          : 2;  /* bit[2-3]  : 安全支付图层0对应索引 */
        unsigned int  secure_index1          : 2;  /* bit[4-5]  : 安全支付图层1对应索引 */
        unsigned int  secure_layer_id_index0 : 5;  /* bit[6-10] : 安全支付图层0对应的layer/id */
        unsigned int  secure_layer_id_index1 : 5;  /* bit[11-15]: 安全支付图层1对应的layer/id */
        unsigned int  reserved               : 16; /* bit[16-31]: reserved */
    } reg;
} SOC_DACC_SCEN_SECURE_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCEN_SECURE_DM_ADDR_UNION
 结构说明  : SCEN_SECURE_DM_ADDR 寄存器结构定义。地址偏移量:0x0504+(s12)*0x400，初值:0x00018C00，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scen_secure_layer_dm_base_adr : 32; /* bit[0-31]: 安全图层信息在DACC空间的偏移地址 */
    } reg;
} SOC_DACC_SCEN_SECURE_DM_ADDR_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCEN_SECURE_RSV_UNION
 结构说明  : SCEN_SECURE_RSV 寄存器结构定义。地址偏移量:0x0508+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scen_secure_rsv : 32; /* bit[0-31]: scen_secure_rsv */
    } reg;
} SOC_DACC_SCEN_SECURE_RSV_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCEN_SECURE_CMD_EN_UNION
 结构说明  : SCEN_SECURE_CMD_EN 寄存器结构定义。地址偏移量:0x050c+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scen_sec_rot_en      : 1;  /* bit[0]   : 是否能够经过ROT处理
                                                               1：能经过ROT处理
                                                               0：不能经过ROT处理 */
        unsigned int  scen_sec_scl_en      : 1;  /* bit[1]   : 是否能够经过SCL处理
                                                               1：能经过SCL处理
                                                               0：不能经过SCL处理 */
        unsigned int  scen_sec_hdr_en      : 1;  /* bit[2]   : 是否能够经过HDR处理
                                                               1：能经过HDR处理
                                                               0：不能经过HDR处理 */
        unsigned int  scen_sec_uvup_en     : 1;  /* bit[3]   : 是否能够经过UVUP处理
                                                               1：能经过UVUP处理
                                                               0：不能经过UVUP处理 */
        unsigned int  scen_sec_cld_en      : 1;  /* bit[4]   : 是否能够经过CLD处理
                                                               1：能经过CLD处理
                                                               0：不能经过CLD处理 */
        unsigned int  scen_sec_wch_en      : 1;  /* bit[5]   : 是否有WCH算子
                                                               1：有WCH算子
                                                               0：无WCH算子 */
        unsigned int  scen_sec_abnormal    : 1;  /* bit[6]   : 产生安全异常时：
                                                               1：直接退出并产生异常中断
                                                               0：不退出同时产生异常中断 */
        unsigned int  scen_sec_othercmd_en : 25; /* bit[7-31]:  */
    } reg;
} SOC_DACC_SCEN_SECURE_CMD_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_PING_CTL0_LAYER0_SECU_UNION
 结构说明  : PING_CTL0_LAYER0_SECU 寄存器结构定义。地址偏移量:0x0510+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ping_ctl0_layer_secu_en0 : 32; /* bit[0-31]: 31:30 layer15安全属性:2'd0为非安全，2'd1为安全视频
                                                                   29:28 layer14安全属性:2'd0为非安全，2'd1为安全视频
                                                                   27:26 layer13安全属性:2'd0为非安全，2'd1为安全视频
                                                                   25:24 layer12安全属性:2'd0为非安全，2'd1为安全视频
                                                                   23:22 layer11安全属性:2'd0为非安全，2'd1为安全视频
                                                                   21:20 layer10安全属性:2'd0为非安全，2'd1为安全视频
                                                                   19:18 layer9安全属性:2'd0为非安全，2'd1为安全视频
                                                                   17:16 layer8安全属性:2'd0为非安全，2'd1为安全视频
                                                                   15:14 layer7安全属性:2'd0为非安全，2'd1为安全视频
                                                                   13:12 layer6安全属性:2'd0为非安全，2'd1为安全视频
                                                                   11:10 layer5安全属性:2'd0为非安全，2'd1为安全视频
                                                                   9:8 layer4安全属性:2'd0为非安全，2'd1为安全视频
                                                                   7:6 layer3安全属性:2'd0为非安全，2'd1为安全视频
                                                                   5:4 layer2安全属性:2'd0为非安全，2'd1为安全视频
                                                                   3:2 layer1安全属性:2'd0为非安全，2'd1为安全视频
                                                                   1:0 layer0安全属性:2'd0为非安全，2'd1为安全视频 */
    } reg;
} SOC_DACC_PING_CTL0_LAYER0_SECU_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_PING_CTL0_LAYER1_SECU_UNION
 结构说明  : PING_CTL0_LAYER1_SECU 寄存器结构定义。地址偏移量:0x0514+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ping_ctl0_layer_secu_en1 : 4;  /* bit[0-3] : 3:2 layer16安全属性:2'd0为非安全，2'd1为安全视频
                                                                   1:0 layer16安全属性:2'd0为非安全，2'd1为安全视频 */
        unsigned int  reserved                 : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_PING_CTL0_LAYER1_SECU_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SEC_PAY_ABNORMAL_INT_UNION
 结构说明  : SEC_PAY_ABNORMAL_INT 寄存器结构定义。地址偏移量:0x0518+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intr_pay_abnormal_s : 1;  /* bit[0-0] : 安全支付外发异常电平，ACPU收到后可写0清 */
        unsigned int  reserved            : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_SEC_PAY_ABNORMAL_INT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_DEMURA_RLST_CNT_UNION
 结构说明  : DBG_DEMURA_RLST_CNT 寄存器结构定义。地址偏移量:0x0520+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_demura_rlst_cnt : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_DEMURA_RLST_CNT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_SCL_RLST_CNT_01_UNION
 结构说明  : DBG_SCL_RLST_CNT_01 寄存器结构定义。地址偏移量:0x0524+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_scl_rlst_cnt_01 : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_SCL_RLST_CNT_01_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_SCL_RLST_CNT_23_UNION
 结构说明  : DBG_SCL_RLST_CNT_23 寄存器结构定义。地址偏移量:0x0528+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_scl_rlst_cnt_23 : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_SCL_RLST_CNT_23_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_UVUP_RLST_YCNT_UNION
 结构说明  : DBG_UVUP_RLST_YCNT 寄存器结构定义。地址偏移量:0x052c+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_uvup_rlst_ycnt : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_UVUP_RLST_YCNT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_CST_ALL_UNION
 结构说明  : DBG_CST_ALL 寄存器结构定义。地址偏移量:0x0530+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_cst_all : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_CST_ALL_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_FSM_MISC_UNION
 结构说明  : DBG_FSM_MISC 寄存器结构定义。地址偏移量:0x0534+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_fsm_misc : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_FSM_MISC_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_BUSY_REQ_MISC_UNION
 结构说明  : DBG_BUSY_REQ_MISC 寄存器结构定义。地址偏移量:0x0538+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_busy_req_misc : 32; /* bit[0-31]:  */
    } reg;
} SOC_DACC_DBG_BUSY_REQ_MISC_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_RESERVED_0_UNION
 结构说明  : DBG_RESERVED_0 寄存器结构定义。地址偏移量:0x053c+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_reserved_0 : 32; /* bit[0-31]: reserved */
    } reg;
} SOC_DACC_DBG_RESERVED_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_RESERVED_1_UNION
 结构说明  : DBG_RESERVED_1 寄存器结构定义。地址偏移量:0x0540+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_reserved_1 : 32; /* bit[0-31]: reserved */
    } reg;
} SOC_DACC_DBG_RESERVED_1_UNION;
#endif


/****************************************************************************
                     (2/3) DM-0418
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_JOINT_RALATION_0_UNION
 结构说明  : JOINT_RALATION_0 寄存器结构定义。地址偏移量:0x0200+0x80*(ly)，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  instr_index_0 : 8;  /* bit[0-7]  : 第1个指令符 */
        unsigned int  instr_index_1 : 8;  /* bit[8-15] : 第2个指令符 */
        unsigned int  instr_index_2 : 8;  /* bit[16-23]: 第3个指令符 */
        unsigned int  instr_index_3 : 8;  /* bit[24-31]: 第4个指令符 */
    } reg;
} SOC_DACC_JOINT_RALATION_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_JOINT_RALATION_1_UNION
 结构说明  : JOINT_RALATION_1 寄存器结构定义。地址偏移量:0x0204+0x80*(ly)，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  instr_index_4 : 8;  /* bit[0-7]  : 第5个指令符 */
        unsigned int  instr_index_5 : 8;  /* bit[8-15] : 第6个指令符 */
        unsigned int  instr_index_6 : 8;  /* bit[16-23]: 第7个指令符 */
        unsigned int  instr_index_7 : 8;  /* bit[24-31]: 第8个指令符 */
    } reg;
} SOC_DACC_JOINT_RALATION_1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_KEY_FEATURE_UNION
 结构说明  : LAYER_KEY_FEATURE 寄存器结构定义。地址偏移量:0x0210+0x80*(ly)，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  connect_type  : 8;  /* bit[0-7]  : 8'd0：DMA+OV类型 
                                                         8'd1：DMA+前处理静态算子+OV类型（无SCl）
                                                         8'd2：DMA+前处理静态算子+OV类型（有SCl）
                                                         8'd3：DMA+ROT+OV类型 
                                                         8'd4：DMA+ROT+前处理静态算子+OV类型（无SCL）
                                                         8'd5：DMA+ROT+前处理静态算子+OV类型（有SCL）
                                                         8'd6：DMA+后处理
                                                         8'd7：OV pattern
                                                         8'd8：DMA+ROT+后处理
                                                         其他值保留 */
        unsigned int  scmd_num      : 8;  /* bit[8-15] : 图层中前处理静态描述符个数 */
        unsigned int  dcmd_num      : 8;  /* bit[16-23]: 图层中动态描述符个数。如果是DMA+后处理静态描述符的这种连接SmartDMA也算数 */
        unsigned int  layer_is_cld  : 1;  /* bit[24-24]: 为1时表示是cld图层，否则不是 */
        unsigned int  layer_is_uvup : 1;  /* bit[25-25]: 为1时表示是uvup图层，否则不是 */
        unsigned int  reserved      : 6;  /* bit[26-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_KEY_FEATURE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MOD_IDX_TABLE_0_UNION
 结构说明  : mod_idx_table_0 寄存器结构定义。地址偏移量:0x0214+0x80*(ly)，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mod_index_0 : 8;  /* bit[0-7]  : 第1个指令符的mod_idx */
        unsigned int  mod_index_1 : 8;  /* bit[8-15] : 第2个指令符的mod_idx */
        unsigned int  mod_index_2 : 8;  /* bit[16-23]: 第3个指令符的mod_idx */
        unsigned int  mod_index_3 : 8;  /* bit[24-31]: 第4个指令符的mod_idx */
    } reg;
} SOC_DACC_MOD_IDX_TABLE_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MOD_IDX_TABLE_1_UNION
 结构说明  : mod_idx_table_1 寄存器结构定义。地址偏移量:0x0218+0x80*(ly)，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mod_index_4 : 8;  /* bit[0-7]  : 第5个指令符的mod_idx */
        unsigned int  mod_index_5 : 8;  /* bit[8-15] : 第6个指令符的mod_idx */
        unsigned int  mod_index_6 : 8;  /* bit[16-23]: 第7个指令符的mod_idx */
        unsigned int  mod_index_7 : 8;  /* bit[24-31]: 第8个指令符的mod_idx */
    } reg;
} SOC_DACC_MOD_IDX_TABLE_1_UNION;
#endif


/****************************************************************************
                     (3/3) DCMD-0418
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_OV_LINEREG_PARA_UNION
 结构说明  : OV_LINEREG_PARA 寄存器结构定义。地址偏移量:0x0000，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_w_linereg_cnt  : 8;  /* bit[0-7]  : [7:0] ov_linereg_cnt */
        unsigned int  ov_w_linereg_flag : 8;  /* bit[8-15] : [7:2] ov_write_for_mod
                                                             [1] reserved
                                                             [0] ov_linereg_flag */
        unsigned int  ov_wtotal         : 8;  /* bit[16-23]: [7:6] reserved
                                                             [5:0] ov_wtotal */
        unsigned int  ov_w_maxline      : 8;  /* bit[24-31]: ov_w_maxline */
    } reg;
} SOC_DACC_OV_LINEREG_PARA_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_HDR_LINEREG_PARA0_UNION
 结构说明  : HDR_LINEREG_PARA0 寄存器结构定义。地址偏移量:0x0004，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  hdr_w_linereg_cnt   : 8;  /* bit[0-7]  : [7:0] hdr_linereg_cnt */
        unsigned int  hdr_w_linereg_flag  : 8;  /* bit[8-15] : [7:1] reserved
                                                               [0] hdr_linereg_flag */
        unsigned int  hdr_wtotal          : 8;  /* bit[16-23]: [7:6] reserved
                                                               [5:0] hdr_wtotal */
        unsigned int  hdr_r_linereg_cnt_y : 8;  /* bit[24-31]: [7:0] hdr_linereg_cnt */
    } reg;
} SOC_DACC_HDR_LINEREG_PARA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_HDR_LINEREG_PARA1_UNION
 结构说明  : HDR_LINEREG_PARA1 寄存器结构定义。地址偏移量:0x0008，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  hdr_r_linereg_flag_y : 8;  /* bit[0-7]  : [7:1] reserved
                                                                [0] hdr_linereg_flag */
        unsigned int  hdr_rtotal_y         : 8;  /* bit[8-15] : [7:6] reserved
                                                                [5:0] hdr_total */
        unsigned int  hdr_r_linereg_cnt_u  : 8;  /* bit[16-23]: [7:0] hdr_linereg_cnt */
        unsigned int  hdr_r_linereg_flag_u : 8;  /* bit[24-31]: [7:1] reserved
                                                                [0] hdr_linereg_flag */
    } reg;
} SOC_DACC_HDR_LINEREG_PARA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_HDR_LINEREG_PARA2_UNION
 结构说明  : HDR_LINEREG_PARA2 寄存器结构定义。地址偏移量:0x000c，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  hdr_rtotal_u         : 8;  /* bit[0-7]  : [7:6] reserved
                                                                [5:0] hdr_total */
        unsigned int  hdr_r_linereg_cnt_v  : 8;  /* bit[8-15] : [7:0] hdr_linereg_cnt */
        unsigned int  hdr_r_linereg_flag_v : 8;  /* bit[16-23]: [7:1] reserved
                                                                [0] hdr_linereg_flag */
        unsigned int  hdr_rtotal_v         : 8;  /* bit[24-31]: [7:6] reserved
                                                                [5:0] hdr_total */
    } reg;
} SOC_DACC_HDR_LINEREG_PARA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP_LINEREG_PARA0_UNION
 结构说明  : UVUP_LINEREG_PARA0 寄存器结构定义。地址偏移量:0x0010，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uvup_w_linereg_cnt   : 8;  /* bit[0-7]  : [7:0] uvup_linereg_cnt */
        unsigned int  uvup_w_linereg_flag  : 8;  /* bit[8-15] : [7:1] reserved
                                                                [0] uvup_linereg_flag */
        unsigned int  uvup_wtotal          : 8;  /* bit[16-23]: [7:6] reserved
                                                                [5:0] uvup_wtotal */
        unsigned int  uvup_r_linereg_cnt_y : 8;  /* bit[24-31]: [7:0] uvup_linereg_cnt */
    } reg;
} SOC_DACC_UVUP_LINEREG_PARA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP_LINEREG_PARA1_UNION
 结构说明  : UVUP_LINEREG_PARA1 寄存器结构定义。地址偏移量:0x0014，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uvup_r_linereg_flag_y : 8;  /* bit[0-7]  : [7:1] reserved
                                                                 [0] uvup_linereg_flag */
        unsigned int  uvup_rtotal_y         : 8;  /* bit[8-15] : [7:6] reserved
                                                                 [5:0] uvup_total */
        unsigned int  uvup_r_linereg_cnt_u  : 8;  /* bit[16-23]: [7:0] uvup_linereg_cnt */
        unsigned int  uvup_r_linereg_flag_u : 8;  /* bit[24-31]: [7:1] reserved
                                                                 [0] uvup_linereg_flag */
    } reg;
} SOC_DACC_UVUP_LINEREG_PARA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP_LINEREG_PARA2_UNION
 结构说明  : UVUP_LINEREG_PARA2 寄存器结构定义。地址偏移量:0x0018，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uvup_rtotal_u         : 8;  /* bit[0-7]  : [7:6] reserved
                                                                 [5:0] uvup_total */
        unsigned int  uvup_r_linereg_cnt_v  : 8;  /* bit[8-15] : [7:0] uvup_linereg_cnt */
        unsigned int  uvup_r_linereg_flag_v : 8;  /* bit[16-23]: [7:1] reserved
                                                                 [0] uvup_linereg_flag */
        unsigned int  uvup_rtotal_v         : 8;  /* bit[24-31]: [7:6] reserved
                                                                 [5:0] uvup_total */
    } reg;
} SOC_DACC_UVUP_LINEREG_PARA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP_LINEREG_PARA3_UNION
 结构说明  : UVUP_LINEREG_PARA3 寄存器结构定义。地址偏移量:0x001C，初值:0x00000000，宽度:32
 寄存器说明: 保留寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_DACC_UVUP_LINEREG_PARA3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_BLK_WIDTH_UNION
 结构说明  : SROT_BLK_WIDTH 寄存器结构定义。地址偏移量:0x0020+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_blk_width : 32; /* bit[0-31]: 当前宏块旋转前的宽度，最大值为8192，单位为pix，实际值减1
                                                         bit0~12：Y平面 bit13~15 reserved
                                                         bit16~28：C平面 */
    } reg;
} SOC_DACC_SROT_BLK_WIDTH_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_BLK_HEIGHT_UNION
 结构说明  : SROT_BLK_HEIGHT 寄存器结构定义。地址偏移量:0x0024+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_blk_height : 32; /* bit[0-31]: 当前宏块旋转前的高度，最大值为8192，单位为pix，实际值减1
                                                          bit0~12：Y平面 bit13~15 reserved
                                                          bit16~28：C平面 */
    } reg;
} SOC_DACC_SROT_BLK_HEIGHT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_BLOCK_CNT_UNION
 结构说明  : SROT_BLOCK_CNT 寄存器结构定义。地址偏移量:0x0028+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_block_cnt : 32; /* bit[0-31]: [31:24] reserved
                                                         [23:12]:C平面rot的block_cnt计数
                                                         [11:0]:Y平面rot的block_cnt计数 */
    } reg;
} SOC_DACC_SROT_BLOCK_CNT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_RESERVED_UNION
 结构说明  : SROT_RESERVED 寄存器结构定义。地址偏移量:0x002c+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 保留寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_DACC_SROT_RESERVED_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_LINEREG_PARA0_UNION
 结构说明  : SROT_LINEREG_PARA0 寄存器结构定义。地址偏移量:0x0030+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_write_for_mod     : 16; /* bit[0-15] : 指示此次写之后，哪个mst读
                                                                  bit0~5：Y平面 bit6~7 reserved
                                                                  bit8~13：C平面 */
        unsigned int  rot_post_static_or_dyn : 8;  /* bit[16-23]: 不再使用
                                                                  bit7~1 reserved
                                                                  1:表示ROT后连接是动态
                                                                  0:表示ROT后连接是静态 */
        unsigned int  srot_w_linereg_cnt_y   : 8;  /* bit[24-31]: [7:0] srot_linereg_cnt */
    } reg;
} SOC_DACC_SROT_LINEREG_PARA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_LINEREG_PARA1_UNION
 结构说明  : SROT_LINEREG_PARA1 寄存器结构定义。地址偏移量:0x0034+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_w_linereg_flag_y : 8;  /* bit[0-7]  : [7:1] reserved
                                                                 [0] srot_linereg_flag */
        unsigned int  srot_wtotal_y         : 8;  /* bit[8-15] : [7:6] reserved
                                                                 [5:0] srot_wtotal */
        unsigned int  srot_w_linereg_cnt_c  : 8;  /* bit[16-23]: [7:0] srot_linereg_cnt */
        unsigned int  srot_w_linereg_flag_c : 8;  /* bit[24-31]: [7:1] reserved
                                                                 [0] srot_linereg_flag */
    } reg;
} SOC_DACC_SROT_LINEREG_PARA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_LINEREG_PARA2_UNION
 结构说明  : SROT_LINEREG_PARA2 寄存器结构定义。地址偏移量:0x0038+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_wtotal_c         : 8;  /* bit[0-7]  : [7:6] reserved
                                                                 [5:0] srot_wtotal */
        unsigned int  srot_r_linereg_cnt_y  : 8;  /* bit[8-15] : [7:0] srot_linereg_cnt */
        unsigned int  srot_r_linereg_flag_y : 8;  /* bit[16-23]: [7:1] reserved
                                                                 [0] srot_linereg_flag */
        unsigned int  srot_rtotal_y         : 8;  /* bit[24-31]: [7:6] reserved
                                                                 [5:0] srot_total */
    } reg;
} SOC_DACC_SROT_LINEREG_PARA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SROT_LINEREG_PARA3_UNION
 结构说明  : SROT_LINEREG_PARA3 寄存器结构定义。地址偏移量:0x003c+(sr)*0x20，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  srot_r_linereg_cnt_c  : 8;  /* bit[0-7]  : [7:0] srot_linereg_cnt */
        unsigned int  srot_r_linereg_flag_c : 8;  /* bit[8-15] : [7:1] reserved
                                                                 [0] srot_linereg_flag */
        unsigned int  srot_rtotal_c         : 8;  /* bit[16-23]: [7:6] reserved
                                                                 [5:0] srot_total */
        unsigned int  rot_rsv               : 8;  /* bit[24-31]: 保留 */
    } reg;
} SOC_DACC_SROT_LINEREG_PARA3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD_LINEREG_PARA0_UNION
 结构说明  : CLD_LINEREG_PARA0 寄存器结构定义。地址偏移量:0x0060+(cl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld_w_linereg_cnt   : 8;  /* bit[0-7]  : [7:0] cld_linereg_cnt */
        unsigned int  cld_w_linereg_flag  : 8;  /* bit[8-15] : [7:1] reserved
                                                               [0] cld_linereg_flag */
        unsigned int  cld_wtotal          : 8;  /* bit[16-23]: [7:6] reserved
                                                               [5:0] cld_wtotal */
        unsigned int  cld_r_linereg_cnt_y : 8;  /* bit[24-31]: [7:0] cld_linereg_cnt */
    } reg;
} SOC_DACC_CLD_LINEREG_PARA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD_LINEREG_PARA1_UNION
 结构说明  : CLD_LINEREG_PARA1 寄存器结构定义。地址偏移量:0x0064+(cl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld_r_linereg_flag_y : 8;  /* bit[0-7]  : [7:1] reserved
                                                                [0] cld_linereg_flag */
        unsigned int  cld_rtotal_y         : 8;  /* bit[8-15] : [7:6] reserved
                                                                [5:0] cld_total */
        unsigned int  cld_r_linereg_cnt_u  : 8;  /* bit[16-23]: [7:0] cld_linereg_cnt */
        unsigned int  cld_r_linereg_flag_u : 8;  /* bit[24-31]: [7:1] reserved
                                                                [0] cld_linereg_flag */
    } reg;
} SOC_DACC_CLD_LINEREG_PARA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD_LINEREG_PARA2_UNION
 结构说明  : CLD_LINEREG_PARA2 寄存器结构定义。地址偏移量:0x0068+(cl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld_rtotal_u         : 8;  /* bit[0-7]  : [7:6] reserved
                                                                [5:0] cld_total */
        unsigned int  cld_r_linereg_cnt_v  : 8;  /* bit[8-15] : [7:0] cld_linereg_cnt */
        unsigned int  cld_r_linereg_flag_v : 8;  /* bit[16-23]: [7:1] reserved
                                                                [0] cld_linereg_flag */
        unsigned int  cld_rtotal_v         : 8;  /* bit[24-31]: [7:6] reserved
                                                                [5:0] cld_total */
    } reg;
} SOC_DACC_CLD_LINEREG_PARA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD_RESERVED_UNION
 结构说明  : CLD_RESERVED 寄存器结构定义。地址偏移量:0x006c+(cl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 保留寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld_reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_DACC_CLD_RESERVED_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCL_LINEREG_PARA0_UNION
 结构说明  : SCL_LINEREG_PARA0 寄存器结构定义。地址偏移量:0x0080+(scl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scl_w_linereg_cnt   : 8;  /* bit[0-7]  : [7:0] scl_linereg_cnt */
        unsigned int  scl_w_linereg_flag  : 8;  /* bit[8-15] : [7:1] reserved
                                                               [0] scl_linereg_flag */
        unsigned int  scl_wtotal          : 8;  /* bit[16-23]: [7:6] reserved
                                                               [5:0] scl_wtotal */
        unsigned int  scl_r_linereg_cnt_y : 8;  /* bit[24-31]: [7:0] scl_linereg_cnt */
    } reg;
} SOC_DACC_SCL_LINEREG_PARA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCL_LINEREG_PARA1_UNION
 结构说明  : SCL_LINEREG_PARA1 寄存器结构定义。地址偏移量:0x0084+(scl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scl_r_linereg_flag_y : 8;  /* bit[0-7]  : [7:1] reserved
                                                                [0] scl_linereg_flag */
        unsigned int  scl_rtotal_y         : 8;  /* bit[8-15] : [7:6] reserved
                                                                [5:0] scl_total */
        unsigned int  scl_r_linereg_cnt_u  : 8;  /* bit[16-23]: [7:0] scl_linereg_cnt */
        unsigned int  scl_r_linereg_flag_u : 8;  /* bit[24-31]: [7:1] reserved
                                                                [0] scl_linereg_flag */
    } reg;
} SOC_DACC_SCL_LINEREG_PARA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCL_LINEREG_PARA2_UNION
 结构说明  : SCL_LINEREG_PARA2 寄存器结构定义。地址偏移量:0x0088+(scl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scl_rtotal_u         : 8;  /* bit[0-7]  : [7:6] reserved
                                                                [5:0] scl_total */
        unsigned int  scl_r_linereg_cnt_v  : 8;  /* bit[8-15] : [7:0] scl_linereg_cnt */
        unsigned int  scl_r_linereg_flag_v : 8;  /* bit[16-23]: [7:1] reserved
                                                                [0] scl_linereg_flag */
        unsigned int  scl_rtotal_v         : 8;  /* bit[24-31]: [7:6] reserved
                                                                [5:0] scl_total */
    } reg;
} SOC_DACC_SCL_LINEREG_PARA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCL_RESERVED_UNION
 结构说明  : SCL_RESERVED 寄存器结构定义。地址偏移量:0x008c+(scl)*0x10，初值:0x00000000，宽度:32
 寄存器说明: 保留寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld_reserved0 : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_DACC_SCL_RESERVED_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA0_UNION
 结构说明  : LAYER_LINEREG_PARA0 寄存器结构定义。地址偏移量:0x00d0+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_w_linereg_cnt_y  : 8;  /* bit[0-7]  : [7:0] dma_linereg_cnt */
        unsigned int  dma_w_linereg_flag_y : 8;  /* bit[8-15] : [7:1] reserved
                                                                [0] dma_linereg_flag */
        unsigned int  dma_wtotal_y         : 8;  /* bit[16-23]: [7:6] reserved
                                                                [5:0] dma_wtotal */
        unsigned int  dma_w_linereg_cnt_u  : 8;  /* bit[24-31]: [7:0] dma_linereg_cnt */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA1_UNION
 结构说明  : LAYER_LINEREG_PARA1 寄存器结构定义。地址偏移量:0x00d4+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_w_linereg_flag_u : 8;  /* bit[0-7]  : [7:1] reserved
                                                                [0] dma_linereg_flag */
        unsigned int  dma_wtotal_u         : 8;  /* bit[8-15] : [7:6] reserved
                                                                [5:0] dma_wtotal */
        unsigned int  dma_w_linereg_cnt_v  : 8;  /* bit[16-23]: [7:0] dma_linereg_cnt */
        unsigned int  dma_w_linereg_flag_v : 8;  /* bit[24-31]: [7:1] reserved
                                                                [0] dma_linereg_flag */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA2_UNION
 结构说明  : LAYER_LINEREG_PARA2 寄存器结构定义。地址偏移量:0x00d8+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_wtotal_v        : 8;  /* bit[0-7]  : [7:6] reserved
                                                               [5:0] dma_wtotal */
        unsigned int  ov_r_linereg_cnt_y  : 8;  /* bit[8-15] : [7:0] ov_linereg_cnt */
        unsigned int  ov_r_linereg_flag_y : 8;  /* bit[16-23]: [7:1] reserved
                                                               [0] ov_linereg_flag */
        unsigned int  ov_r_linereg_cnt_u  : 8;  /* bit[24-31]: [7:0] ov_linereg_cnt */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA3_UNION
 结构说明  : LAYER_LINEREG_PARA3 寄存器结构定义。地址偏移量:0x00dc+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_r_linereg_flag_u    : 8;  /* bit[0-7]  : [7:1] reserved
                                                                  [0] ov_linereg_flag */
        unsigned int  ov_r_linereg_cnt_v     : 8;  /* bit[8-15] : [7:0] ov_linereg_cnt */
        unsigned int  ov_r_linereg_flag_v    : 8;  /* bit[16-23]: [7:1] reserved
                                                                  [0] ov_linereg_flag */
        unsigned int  dma_post_static_or_dyn : 8;  /* bit[24-31]: 不再使用
                                                                  bit7:~1 reserved
                                                                  1:表示DMA后连接是动态
                                                                  0:表示DMA后连接是静态 */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA3_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA4_UNION
 结构说明  : LAYER_LINEREG_PARA4 寄存器结构定义。地址偏移量:0x00e0+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_read_pre_static_or_dyn : 8;  /* bit[0-7]  : 不再使用
                                                                     bit7:~1 reserved
                                                                     1:表示OV前连接是动态
                                                                     0:表示OV前连接是静态 */
        unsigned int  max_line_y0               : 8;  /* bit[8-15] : 和连接关系对应的指令符0的y平面max_line */
        unsigned int  max_line_u0               : 8;  /* bit[16-23]: 和连接关系对应的指令符0的u平面max_line */
        unsigned int  max_line_v0               : 8;  /* bit[24-31]: 和连接关系对应的指令符0的v平面max_line */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA5_UNION
 结构说明  : LAYER_LINEREG_PARA5 寄存器结构定义。地址偏移量:0x00e4+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_line_y1 : 8;  /* bit[0-7]  : 和连接关系对应的指令符1的y平面max_line */
        unsigned int  max_line_u1 : 8;  /* bit[8-15] : 和连接关系对应的指令符1的u平面max_line */
        unsigned int  max_line_v1 : 8;  /* bit[16-23]: 和连接关系对应的指令符1的v平面max_line */
        unsigned int  max_line_y2 : 8;  /* bit[24-31]: 和连接关系对应的指令符2的y平面max_line */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA5_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA6_UNION
 结构说明  : LAYER_LINEREG_PARA6 寄存器结构定义。地址偏移量:0x00e8+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_line_u2 : 8;  /* bit[0-7]  : 和连接关系对应的指令符2的u平面max_line */
        unsigned int  max_line_v2 : 8;  /* bit[8-15] : 和连接关系对应的指令符2的v平面max_line */
        unsigned int  max_line_y3 : 8;  /* bit[16-23]: 和连接关系对应的指令符3的y平面max_line */
        unsigned int  max_line_u3 : 8;  /* bit[24-31]: 和连接关系对应的指令符3的u平面max_line */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA6_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA7_UNION
 结构说明  : LAYER_LINEREG_PARA7 寄存器结构定义。地址偏移量:0x00ec+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_line_v3 : 8;  /* bit[0-7]  : 和连接关系对应的指令符3的v平面max_line */
        unsigned int  max_line_y4 : 8;  /* bit[8-15] : 和连接关系对应的指令符4的y平面max_line */
        unsigned int  max_line_u4 : 8;  /* bit[16-23]: 和连接关系对应的指令符4的u平面max_line */
        unsigned int  max_line_v4 : 8;  /* bit[24-31]: 和连接关系对应的指令符4的v平面max_line */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA7_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA8_UNION
 结构说明  : LAYER_LINEREG_PARA8 寄存器结构定义。地址偏移量:0x00f0+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_line_y5 : 8;  /* bit[0-7]  : 和连接关系对应的指令符5的y平面max_line */
        unsigned int  max_line_u5 : 8;  /* bit[8-15] : 和连接关系对应的指令符5的u平面max_line */
        unsigned int  max_line_v5 : 8;  /* bit[16-23]: 和连接关系对应的指令符5的v平面max_line */
        unsigned int  max_line_y6 : 8;  /* bit[24-31]: 和连接关系对应的指令符6的y平面max_line */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA8_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA9_UNION
 结构说明  : LAYER_LINEREG_PARA9 寄存器结构定义。地址偏移量:0x00f4+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_line_u6 : 8;  /* bit[0-7]  : 和连接关系对应的指令符6的u平面max_line */
        unsigned int  max_line_v6 : 8;  /* bit[8-15] : 和连接关系对应的指令符6的v平面max_line */
        unsigned int  max_line_y7 : 8;  /* bit[16-23]: 和连接关系对应的指令符7的y平面max_line */
        unsigned int  max_line_u7 : 8;  /* bit[24-31]: 和连接关系对应的指令符7的u平面max_line */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA9_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA10_UNION
 结构说明  : LAYER_LINEREG_PARA10 寄存器结构定义。地址偏移量:0x00f8+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_line_v7         : 8;  /* bit[0-7]  : 和连接关系对应的指令符7的v平面max_line */
        unsigned int  dma_block_high_last : 8;  /* bit[8-15] : DMA搬运块高度尾值，不再使用 */
        unsigned int  dma_block_width     : 16; /* bit[16-31]: DMA搬运块宽度 */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA10_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LINEREG_PARA11_UNION
 结构说明  : LAYER_LINEREG_PARA11 寄存器结构定义。地址偏移量:0x00fc+(ly)*0x38，初值:0x00000000，宽度:32
 寄存器说明: 
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_block_high       : 16; /* bit[0-15] : DMA搬运块高度 */
        unsigned int  dma_block_width_last : 16; /* bit[16-31]: DMA搬运块宽度尾值，不再使用 */
    } reg;
} SOC_DACC_LAYER_LINEREG_PARA11_UNION;
#endif


/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of soc_dacc_interface.h */
