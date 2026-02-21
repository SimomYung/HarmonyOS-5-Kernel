

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

// fixme!!
#define SOC_DACC_CTL_ST_FRM_SEL0_REG_ADDR(base, s12)      ((base) + (0x041C+(s12)*0x400))


/****************************************************************************
                     (1/5) CFG
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
#define SOC_DACC_CORE_CONFIG_REG_ADDR(base)                   ((base) + (0x000c))

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
   位域定义UNION结构:  SOC_DACC_DMA_TRANS_START_ADDR_UNION */
#define SOC_DACC_DMA_TRANS_START_ADDR_ADDR(base)              ((base) + (0x001c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DMA_TRANS_WORD_NUM_UNION */
#define SOC_DACC_DMA_TRANS_WORD_NUM_ADDR(base)                ((base) + (0x0020))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DMA_TRANS_START_REG_UNION */
#define SOC_DACC_DMA_TRANS_START_REG_ADDR(base)               ((base) + (0x0024))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DSI_PORT_INT_MASK_REG_UNION */
#define SOC_DACC_DSI_PORT_INT_MASK_REG_ADDR(base)             ((base) + (0x0028))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DP_PORT_INT_MASK_REG_UNION */
#define SOC_DACC_DP_PORT_INT_MASK_REG_ADDR(base)              ((base) + (0x002c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MOD_END_INT_MASK_REG_UNION */
#define SOC_DACC_MOD_END_INT_MASK_REG_ADDR(base)              ((base) + (0x0030))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_INT_MASK_REG_UNION */
#define SOC_DACC_OV_INT_MASK_REG_ADDR(base)                   ((base) + (0x0034))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MISC_INT_MASK_REG_UNION */
#define SOC_DACC_MISC_INT_MASK_REG_ADDR(base)                 ((base) + (0x0038))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MOD_EXIT_MEM_DS_REG_UNION */
#define SOC_DACC_MOD_EXIT_MEM_DS_REG_ADDR(base)               ((base) + (0x003c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DSI_PORT_INT_CLR_REG_UNION */
#define SOC_DACC_DSI_PORT_INT_CLR_REG_ADDR(base)              ((base) + (0x0040))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DP_PORT_INT_CLR_REG_UNION */
#define SOC_DACC_DP_PORT_INT_CLR_REG_ADDR(base)               ((base) + (0x0044))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MOD_END_INT_CLR_REG_UNION */
#define SOC_DACC_MOD_END_INT_CLR_REG_ADDR(base)               ((base) + (0x0048))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MISC_INT_CLR_REG_UNION */
#define SOC_DACC_MISC_INT_CLR_REG_ADDR(base)                  ((base) + (0x004c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_2_ACPU_REG_UNION */
#define SOC_DACC_RISCV_2_ACPU_REG_ADDR(base)                  ((base) + (0x0050))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MINIBUS_1T01_CKG_EN_UNION */
#define SOC_DACC_MINIBUS_1T01_CKG_EN_ADDR(base)               ((base) + (0x0054))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_MINIBUS_1T01_STATE_UNION */
#define SOC_DACC_MINIBUS_1T01_STATE_ADDR(base)                ((base) + (0x0058))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CKG_EN_UNION */
#define SOC_DACC_CKG_EN_ADDR(base)                            ((base) + (0x005c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LUT_LAYER_ID_UNION */
#define SOC_DACC_LUT_LAYER_ID_ADDR(base)                      ((base) + (0x0060))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LP_DOZE_CFG_UNION */
#define SOC_DACC_LP_DOZE_CFG_ADDR(base)                       ((base) + (0x0064))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_TE_MASK_INTERVAL_UNION */
#define SOC_DACC_TE_MASK_INTERVAL_ADDR(base)                  ((base) + (0x0068))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_2_RISCV_INTR_TRIG_UNION */
#define SOC_DACC_ACPU_2_RISCV_INTR_TRIG_ADDR(base)            ((base) + (0x006c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_2_RISCV_INTR_MASK_UNION */
#define SOC_DACC_ACPU_2_RISCV_INTR_MASK_ADDR(base)            ((base) + (0x0070))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_2_RISCV_INTR_CLR_UNION */
#define SOC_DACC_ACPU_2_RISCV_INTR_CLR_ADDR(base)             ((base) + (0x0074))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WB_SOURCE_SEL_REG_UNION */
#define SOC_DACC_WB_SOURCE_SEL_REG_ADDR(base)                 ((base) + (0x0078))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_TCXO_CNTER_VALUE_UNION */
#define SOC_DACC_TCXO_CNTER_VALUE_ADDR(base)                  ((base) + (0x007c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_VER_ID_REG_UNION */
#define SOC_DACC_VER_ID_REG_ADDR(base)                        ((base) + (0x00fc))

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
   位域定义UNION结构:  SOC_DACC_LTPO_FRESH_RATE_0_UNION */
#define SOC_DACC_LTPO_FRESH_RATE_0_ADDR(base)                 ((base) + (0x01a0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LTPO_FRESH_RATE_1_UNION */
#define SOC_DACC_LTPO_FRESH_RATE_1_ADDR(base)                 ((base) + (0x01a4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LTPO_DATA_LENGTH_0_UNION */
#define SOC_DACC_LTPO_DATA_LENGTH_0_ADDR(base)                ((base) + (0x01a8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LTPO_DATA_LENGTH_1_UNION */
#define SOC_DACC_LTPO_DATA_LENGTH_1_ADDR(base)                ((base) + (0x01ac))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LTPO_START_ADDR_0_UNION */
#define SOC_DACC_LTPO_START_ADDR_0_ADDR(base)                 ((base) + (0x01b0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LTPO_START_ADDR_1_UNION */
#define SOC_DACC_LTPO_START_ADDR_1_ADDR(base)                 ((base) + (0x01b4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_FRESH_RATE_TRIG_0_UNION */
#define SOC_DACC_ACPU_FRESH_RATE_TRIG_0_ADDR(base)            ((base) + (0x01b8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_FRESH_RATE_TRIG_1_UNION */
#define SOC_DACC_ACPU_FRESH_RATE_TRIG_1_ADDR(base)            ((base) + (0x01bc))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_FRESH_RATE_TRIG_0_UNION */
#define SOC_DACC_RISCV_FRESH_RATE_TRIG_0_ADDR(base)           ((base) + (0x01c0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_FRESH_RATE_TRIG_1_UNION */
#define SOC_DACC_RISCV_FRESH_RATE_TRIG_1_ADDR(base)           ((base) + (0x01c4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LTPO_CFG_STATUS_UNION */
#define SOC_DACC_LTPO_CFG_STATUS_ADDR(base)                   ((base) + (0x01c8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SW_CMD_REG0_UNION */
#define SOC_DACC_SW_CMD_REG0_ADDR(base)                       ((base) + (0x01e0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SW_CMD_REG1_UNION */
#define SOC_DACC_SW_CMD_REG1_ADDR(base)                       ((base) + (0x01e4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CMD_BUF_NUM_UNION */
#define SOC_DACC_CMD_BUF_NUM_ADDR(base)                       ((base) + (0x01e8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SW_CMD_TX_REG_UNION */
#define SOC_DACC_SW_CMD_TX_REG_ADDR(base)                     ((base) + (0x01ec))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_MASK_EN_UNION */
#define SOC_DACC_LAYER_MASK_EN_ADDR(base)                     ((base) + (0x0300))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_MASK_SCL_CFG_UNION */
#define SOC_DACC_LAYER_MASK_SCL_CFG_ADDR(base)                ((base) + (0x0304))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_MASKED_EN_UNION */
#define SOC_DACC_LAYER_MASKED_EN_ADDR(base)                   ((base) + (0x0308))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLK_SEL_REG_UNION */
#define SOC_DACC_CLK_SEL_REG_ADDR(base, s12)                  ((base) + (0x0340+(s12)*0x8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLK_EN_REG_UNION */
#define SOC_DACC_CLK_EN_REG_ADDR(base, s12)                   ((base) + (0x0344+(s12)*0x8))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_CLR_START_UNION */
#define SOC_DACC_CLR_START_REG_ADDR(base, s12)               ((base) + (0x0380+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_CLR_START_UNION */
#define SOC_DACC_CLR_RISCV_START_ADDR(base, s12)              ((base) + (0x0384+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_UNION */
#define SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_ADDR(base, s12)    ((base) + (0x0388+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLR_TIMEOUT_STATUS_REG_UNION */
#define SOC_DACC_CLR_TIMEOUT_STATUS_REG_ADDR(base, s12)       ((base) + (0x038c+(s12)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_NEW_FRAME_RDY_CFG_UNION */
#define SOC_DACC_NEW_FRAME_RDY_CFG_ADDR(base, s12)            ((base) + (0x0404+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_EN_RELOAD_UNION */
#define SOC_DACC_CTL_SW_EN_RELOAD_ADDR(base, s12)             ((base) + (0x0408+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_CFG_OVER_REG_UNION */
#define SOC_DACC_RISCV_CFG_OVER_REG_ADDR(base, s12)           ((base) + (0x0410+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_INTR_EN_REG_UNION */
#define SOC_DACC_CTL_SW_INTR_EN_REG_ADDR(base, s12)           ((base) + (0x0414+(s12)*0x400))

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
   位域定义UNION结构:  SOC_DACC_ACPU_2_RISCV_LOG_REG_UNION */
#define SOC_DACC_ACPU_2_RISCV_LOG_REG_ADDR(base, s12)         ((base) + (0x043C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_2_ACPU_LOG_REG_UNION */
#define SOC_DACC_RISCV_2_ACPU_LOG_REG_ADDR(base, s12)         ((base) + (0x0440+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCMD_START_REG_UNION */
#define SOC_DACC_SCMD_START_REG_ADDR(base, s12)               ((base) + (0x0450+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DCMD_START_REG_UNION */
#define SOC_DACC_DCMD_START_REG_ADDR(base, s12)               ((base) + (0x0454+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ACPU_PARA_ADDR_REG_UNION */
#define SOC_DACC_ACPU_PARA_ADDR_REG_ADDR(base, s12)           ((base) + (0x0458+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_RISCV_PARA_ADDR_REG_UNION */
#define SOC_DACC_RISCV_PARA_ADDR_REG_ADDR(base, s12)          ((base) + (0x045C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_DIM_FLAG_REG_UNION */
#define SOC_DACC_LAYER_DIM_FLAG_REG_ADDR(base, s12)           ((base) + (0x0460+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_GENERAL_CFG_0_REG_UNION */
#define SOC_DACC_GENERAL_CFG_0_REG_ADDR(base, s12)            ((base) + (0x0464+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_GENERAL_CFG_1_REG_UNION */
#define SOC_DACC_GENERAL_CFG_1_REG_ADDR(base, s12)            ((base) + (0x0468+(s12)*0x400))

/* 寄存器说明：DCMD interrupt mask registers
   位域定义UNION结构:  SOC_DACC_DCMD_INTS_MASK_REG_UNION */
#define SOC_DACC_DCMD_INTS_MASK_REG_ADDR(base, s12)           ((base) + (0x0474+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DCMD_STATUS_REG_UNION */
#define SOC_DACC_DCMD_STATUS_REG_ADDR(base, s12)              ((base) + (0x0478+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_NUM_REG_UNION */
#define SOC_DACC_LAYER_NUM_REG_ADDR(base, s12)                ((base) + (0x047C+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CTL_SW_START_REG_UNION */
#define SOC_DACC_CTL_SW_START_REG_ADDR(base, s12)             ((base) + (0x0404+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCEN_SECURE_REG_UNION */
#define SOC_DACC_SCEN_SECURE_REG_ADDR(base, s12)              ((base) + (0x500+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCEN_SECURE_DM_ADDR_UNION */
#define SOC_DACC_SCEN_SECURE_DM_ADDR_ADDR(base, s12)          ((base) + (0x0504+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_TUI_CMDLIST_ADDR_UNION */
#define SOC_DACC_TUI_CMDLIST_ADDR_ADDR(base, s12)             ((base) + (0x0508+(s12)*0x400))

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
   位域定义UNION结构:  SOC_DACC_DBG_0_INFO_UNION */
#define SOC_DACC_DBG_0_INFO_ADDR(base, s12)                   ((base) + (0x052c+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_1_INFO_UNION */
#define SOC_DACC_DBG_1_INFO_ADDR(base, s12)                   ((base) + (0x0530+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_2_INFO_UNION */
#define SOC_DACC_DBG_2_INFO_ADDR(base, s12)                   ((base) + (0x0534+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_3_INFO_UNION */
#define SOC_DACC_DBG_3_INFO_ADDR(base, s12)                   ((base) + (0x0538+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_4_INFO_UNION */
#define SOC_DACC_DBG_4_INFO_ADDR(base, s12)                   ((base) + (0x053c+(s12)*0x400))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DBG_5_INFO_UNION */
#define SOC_DACC_DBG_5_INFO_ADDR(base, s12)                   ((base) + (0x0540+(s12)*0x400))


/****************************************************************************
                     (2/5) Pre_Pipe_Ctrl
 ****************************************************************************/


/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_WR_LINEREG_S0_UNION */
#define SOC_DACC_OV_WR_LINEREG_S0_ADDR(base)                  ((base) + (0x0000))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA0_S0_UNION */
#define SOC_DACC_LAYER_LB_PARA0_S0_ADDR(base, pre_ch0)        ((base) + (0x0004+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA1_S0_UNION */
#define SOC_DACC_LAYER_LB_PARA1_S0_ADDR(base, pre_ch0)        ((base) + (0x0008+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA2_S0_UNION */
#define SOC_DACC_LAYER_LB_PARA2_S0_ADDR(base, pre_ch0)        ((base) + (0x000c+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_IN_S0_UNION */
#define SOC_DACC_LAYER_INFO_IN_S0_ADDR(base, pre_ch0)         ((base) + (0x0010+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_OUT_S0_UNION */
#define SOC_DACC_LAYER_INFO_OUT_S0_ADDR(base, pre_ch0)        ((base) + (0x0014+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_CLIP_INFO_S0_UNION */
#define SOC_DACC_LAYER_CLIP_INFO_S0_ADDR(base, pre_ch0)       ((base) + (0x0018+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_INFO_S0_UNION */
#define SOC_DACC_LAYER_LB_INFO_S0_ADDR(base, pre_ch0)         ((base) + (0x001c+(pre_ch0)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_WR_LINEREG_S1_UNION */
#define SOC_DACC_OV_WR_LINEREG_S1_ADDR(base)                  ((base) + (0x0300))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA0_S1_UNION */
#define SOC_DACC_LAYER_LB_PARA0_S1_ADDR(base, pre_ch1)        ((base) + (0x0304+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA1_S1_UNION */
#define SOC_DACC_LAYER_LB_PARA1_S1_ADDR(base, pre_ch1)        ((base) + (0x0308+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA2_S1_UNION */
#define SOC_DACC_LAYER_LB_PARA2_S1_ADDR(base, pre_ch1)        ((base) + (0x030c+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_IN_S1_UNION */
#define SOC_DACC_LAYER_INFO_IN_S1_ADDR(base, pre_ch1)         ((base) + (0x0310+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_OUT_S1_UNION */
#define SOC_DACC_LAYER_INFO_OUT_S1_ADDR(base, pre_ch1)        ((base) + (0x0314+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_CLIP_INFO_S1_UNION */
#define SOC_DACC_LAYER_CLIP_INFO_S1_ADDR(base, pre_ch1)       ((base) + (0x0318+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_INFO_S1_UNION */
#define SOC_DACC_LAYER_LB_INFO_S1_ADDR(base, pre_ch1)         ((base) + (0x031c+(pre_ch1)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_WR_LINEREG_S2_UNION */
#define SOC_DACC_OV_WR_LINEREG_S2_ADDR(base)                  ((base) + (0x0500))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA0_S2_UNION */
#define SOC_DACC_LAYER_LB_PARA0_S2_ADDR(base, pre_ch2)        ((base) + (0x0504+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA1_S2_UNION */
#define SOC_DACC_LAYER_LB_PARA1_S2_ADDR(base, pre_ch2)        ((base) + (0x0508+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA2_S2_UNION */
#define SOC_DACC_LAYER_LB_PARA2_S2_ADDR(base, pre_ch2)        ((base) + (0x050c+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_IN_S2_UNION */
#define SOC_DACC_LAYER_INFO_IN_S2_ADDR(base, pre_ch2)         ((base) + (0x0510+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_OUT_S2_UNION */
#define SOC_DACC_LAYER_INFO_OUT_S2_ADDR(base, pre_ch2)        ((base) + (0x0514+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_CLIP_INFO_REG_S2_UNION */
#define SOC_DACC_LAYER_CLIP_INFO_REG_S2_ADDR(base, pre_ch2)   ((base) + (0x0518+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_INFO_REG_S2_UNION */
#define SOC_DACC_LAYER_LB_INFO_REG_S2_ADDR(base, pre_ch2)     ((base) + (0x051c+(pre_ch2)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_OV_WR_LINEREG_S4_UNION */
#define SOC_DACC_OV_WR_LINEREG_S4_ADDR(base)                  ((base) + (0x0700))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA0_S4_UNION */
#define SOC_DACC_LAYER_LB_PARA0_S4_ADDR(base, pre_ch4)        ((base) + (0x0704+(pre_ch4)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA1_S4_UNION */
#define SOC_DACC_LAYER_LB_PARA1_S4_ADDR(base, pre_ch4)        ((base) + (0x0708+(pre_ch4)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_PARA2_S4_UNION */
#define SOC_DACC_LAYER_LB_PARA2_S4_ADDR(base, pre_ch4)        ((base) + (0x070c+(pre_ch4)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_IN_S4_UNION */
#define SOC_DACC_LAYER_INFO_IN_S4_ADDR(base, pre_ch4)         ((base) + (0x0710+(pre_ch4)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_INFO_OUT_S4_UNION */
#define SOC_DACC_LAYER_INFO_OUT_S4_ADDR(base, pre_ch4)        ((base) + (0x0714+(pre_ch4)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_CLIP_INFO_REG_S4_UNION */
#define SOC_DACC_LAYER_CLIP_INFO_REG_S4_ADDR(base, pre_ch4)   ((base) + (0x0718+(pre_ch4)*0x1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_LB_INFO_REG_S4_UNION */
#define SOC_DACC_LAYER_LB_INFO_REG_S4_ADDR(base, pre_ch4)     ((base) + (0x071c+(pre_ch4)*0x1c))


/****************************************************************************
                     (3/5) Post_Pipe_Ctrl
 ****************************************************************************/


/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCENE_0_WB_INFO_REG_UNION */
#define SOC_DACC_SCENE_0_WB_INFO_REG_ADDR(base)               ((base) + (0x0e60))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCENE_RD_INFO_REG_UNION */
#define SOC_DACC_SCENE_RD_INFO_REG_ADDR(base, post_ch)        ((base) + (0x0e70+(post_ch)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCENE_WR_INFO_REG_UNION */
#define SOC_DACC_SCENE_WR_INFO_REG_ADDR(base, post_ch)        ((base) + (0x0e74+(post_ch)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCENE_LB_R_INFO_REG_UNION */
#define SOC_DACC_SCENE_LB_R_INFO_REG_ADDR(base, post_ch)      ((base) + (0x0e78+(post_ch)*0x10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_SCENE_LB_W_INFO_REG_UNION */
#define SOC_DACC_SCENE_LB_W_INFO_REG_ADDR(base, post_ch)      ((base) + (0x0e7c+(post_ch)*0x10))


/****************************************************************************
                     (4/5) ITF_WCH_Ctrl
 ****************************************************************************/


/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ITF0_INFO_REG_UNION */
#define SOC_DACC_ITF0_INFO_REG_ADDR(base)                     ((base) + (0x0e00))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ITF0_LB_INFO_REG_UNION */
#define SOC_DACC_ITF0_LB_INFO_REG_ADDR(base)                  ((base) + (0x0e04))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ITF1_INFO_REG_UNION */
#define SOC_DACC_ITF1_INFO_REG_ADDR(base)                     ((base) + (0x0e08))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ITF1_LB_INFO_REG_UNION */
#define SOC_DACC_ITF1_LB_INFO_REG_ADDR(base)                  ((base) + (0x0e0c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ITF2_INFO_REG_UNION */
#define SOC_DACC_ITF2_INFO_REG_ADDR(base)                     ((base) + (0x0e10))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ITF2_LB_INFO_REG_UNION */
#define SOC_DACC_ITF2_LB_INFO_REG_ADDR(base)                  ((base) + (0x0e14))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WCH0_INFO_REG_UNION */
#define SOC_DACC_WCH0_INFO_REG_ADDR(base)                     ((base) + (0x0e18))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WCH0_LB_INFO_REG_UNION */
#define SOC_DACC_WCH0_LB_INFO_REG_ADDR(base)                  ((base) + (0x0e1c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WCH1_INFO_REG_UNION */
#define SOC_DACC_WCH1_INFO_REG_ADDR(base)                     ((base) + (0x0e20))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_WCH1_LB_INFO_REG_UNION */
#define SOC_DACC_WCH1_LB_INFO_REG_ADDR(base)                  ((base) + (0x0e24))


/****************************************************************************
                     (5/5) CrossBar
 ****************************************************************************/


/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ARSR1_CFG_UNION */
#define SOC_DACC_ARSR1_CFG_ADDR(base)                         ((base) + (0x1000))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_VSCF0_CFG_UNION */
#define SOC_DACC_VSCF0_CFG_ADDR(base)                         ((base) + (0x1004))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_VSCF1_CFG_UNION */
#define SOC_DACC_VSCF1_CFG_ADDR(base)                         ((base) + (0x1008))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_UVUP0_CFG_UNION */
#define SOC_DACC_UVUP0_CFG_ADDR(base)                         ((base) + (0x100C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_HDR_CFG_UNION */
#define SOC_DACC_HDR_CFG_ADDR(base)                           ((base) + (0x1010))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_CLD_CFG_UNION */
#define SOC_DACC_CLD_CFG_ADDR(base)                           ((base) + (0x1014))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_ARSR0_CFG_UNION */
#define SOC_DACC_ARSR0_CFG_ADDR(base)                         ((base) + (0x1018))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_UVUP1_CFG_UNION */
#define SOC_DACC_UVUP1_CFG_ADDR(base)                         ((base) + (0x101C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DPP0_CFG_UNION */
#define SOC_DACC_DPP0_CFG_ADDR(base)                          ((base) + (0x1020))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DPP1_CFG_UNION */
#define SOC_DACC_DPP1_CFG_ADDR(base)                          ((base) + (0x1024))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DDIC_CFG_UNION */
#define SOC_DACC_DDIC_CFG_ADDR(base)                          ((base) + (0x1028))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DSC_CFG_UNION */
#define SOC_DACC_DSC_CFG_ADDR(base)                           ((base) + (0x102C))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_OV_Y_S0_UNION */
#define SOC_DACC_LAYER_OV_Y_S0_ADDR(base, pre_ch0)            ((base) + (0x1040+(pre_ch0)*0x4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DIM_FLAG_REG_S0_UNION */
#define SOC_DACC_DIM_FLAG_REG_S0_ADDR(base)                   ((base) + (0x1080))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_OV_Y_S1_UNION */
#define SOC_DACC_LAYER_OV_Y_S1_ADDR(base, pre_ch1)            ((base) + (0x1090+(pre_ch1)*0x4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DIM_FLAG_REG_S1_UNION */
#define SOC_DACC_DIM_FLAG_REG_S1_ADDR(base)                   ((base) + (0x10b0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_OV_Y_S2_UNION */
#define SOC_DACC_LAYER_OV_Y_S2_ADDR(base, pre_ch2)            ((base) + (0x10c0+(pre_ch2)*0x4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DIM_FLAG_REG_S2_UNION */
#define SOC_DACC_DIM_FLAG_REG_S2_ADDR(base)                   ((base) + (0x10e0))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_LAYER_OV_Y_S4_UNION */
#define SOC_DACC_LAYER_OV_Y_S4_ADDR(base, pre_ch4)            ((base) + (0x1120+(pre_ch4)*0x4))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_DIM_FLAG_REG_S4_UNION */
#define SOC_DACC_DIM_FLAG_REG_S4_ADDR(base)                   ((base) + (0x1140))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_INFO_RDY_NEED_DBG_ST0_UNION */
#define SOC_DACC_INFO_RDY_NEED_DBG_ST0_ADDR(base)             ((base) + (0x1144))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_INFO_RDY_NEED_DBG_ST1_UNION */
#define SOC_DACC_INFO_RDY_NEED_DBG_ST1_ADDR(base)             ((base) + (0x1148))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_INFO_LINE_ID_DBG_S01_UNION */
#define SOC_DACC_INFO_LINE_ID_DBG_S01_ADDR(base)              ((base) + (0x114c))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_INFO_LINE_ID_DBG_S23_UNION */
#define SOC_DACC_INFO_LINE_ID_DBG_S23_ADDR(base)              ((base) + (0x1150))

/* 寄存器说明：
   位域定义UNION结构:  SOC_DACC_INFO_LINE_ID_DBG_S45_UNION */
#define SOC_DACC_INFO_LINE_ID_DBG_S45_ADDR(base)              ((base) + (0x1154))


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
                     (1/5) CFG
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
        unsigned int  sw_ext_ints : 16; /* bit[0-15] : [15]: 保留
                                                       [14:0]: 软件外发中断寄存器，RISCV可配置该寄存器向ACPU发送中断 */
        unsigned int  reserved    : 16; /* bit[16-31]:  */
    } reg;
} SOC_DACC_SW_EXT_INT_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_EXT_INT_MASK_REG_UNION
 结构说明  : SW_EXT_INT_MASK_REG 寄存器结构定义。地址偏移量:0x0004，初值:0x0000FFFF，宽度:32
 寄存器说明: SW interrupt mask registers
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_ext_ints_mask : 16; /* bit[0-15] : [15]:RISCV配置cfg_sctrl_doze1_wake触发ACPU中断mask
                                                            [14:0]: 软件外发中断MASK寄存器 */
        unsigned int  reserved         : 16; /* bit[16-31]:  */
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
 结构说明  : CORE_CONFIG_REG 寄存器结构定义。地址偏移量:0x000c，初值:0x00000010，宽度:32
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
 结构名    : SOC_DACC_DMA_TRANS_START_ADDR_UNION
 结构说明  : DMA_TRANS_START_ADDR 寄存器结构定义。地址偏移量:0x001c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_trans_start_addr : 18; /* bit[0-17] : 参数DMA搬运DM数据到local RAM时的起始地址（相对DACC空间） */
        unsigned int  reserved             : 14; /* bit[18-31]: 保留 */
    } reg;
} SOC_DACC_DMA_TRANS_START_ADDR_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DMA_TRANS_WORD_NUM_UNION
 结构说明  : DMA_TRANS_WORD_NUM 寄存器结构定义。地址偏移量:0x0020，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_trans_word_num : 18; /* bit[0-17] : 参数DMA搬运DM数据到local RAM时的数据量（单位Word） */
        unsigned int  reserved           : 14; /* bit[18-31]: 保留 */
    } reg;
} SOC_DACC_DMA_TRANS_WORD_NUM_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DMA_TRANS_START_REG_UNION
 结构说明  : DMA_TRANS_START_REG 寄存器结构定义。地址偏移量:0x0024，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_trans_start : 1;  /* bit[0-0] : RISCV配置，启动DACC内DMA传输 */
        unsigned int  reserved        : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_DMA_TRANS_START_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DSI_PORT_INT_MASK_REG_UNION
 结构说明  : DSI_PORT_INT_MASK_REG 寄存器结构定义。地址偏移量:0x0028，初值:0xFFFFFFFF，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dsi_port_int_mask : 32; /* bit[0-31]: [31:13]:保留
                                                            [12]:dsi1_frm_underflow中断mask
                                                            [11]:dsi1_frm_start中断mask
                                                            [10]:dsi1_frm_end中断mask
                                                            [9]:dsi1_frm_vactive中断mask
                                                            [8]:dsi1_cmd_event中断mask
                                                            [7:5]:保留
                                                            [4]:dsi0_frm_underflow中断mask
                                                            [3]:dsi0_frm_start中断mask
                                                            [2]:dsi0_frm_end中断mask
                                                            [1]:dsi0_frm_vactive中断mask
                                                            [0]:dsi0_cmd_event中断mask */
    } reg;
} SOC_DACC_DSI_PORT_INT_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DP_PORT_INT_MASK_REG_UNION
 结构说明  : DP_PORT_INT_MASK_REG 寄存器结构定义。地址偏移量:0x002c，初值:0xFFFFFFFF，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dp_port_int_mask : 32; /* bit[0-31]: [31:13]:保留
                                                           [12]:dp1_frm_underflow中断mask
                                                           [11]:dp1_frm_start中断mask
                                                           [10]:dp1_frm_end中断mask
                                                           [9]:dp1_frm_vactive中断mask
                                                           [8]:dp1_cmd_event中断mask
                                                           [7:5]:保留
                                                           [4]:dp0_frm_underflow中断mask
                                                           [3]:dp0_frm_start中断mask
                                                           [2]:dp0_frm_end中断mask
                                                           [1]:dp0_frm_vactive中断mask
                                                           [0]:dp0_cmd_event中断mask */
    } reg;
} SOC_DACC_DP_PORT_INT_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MOD_END_INT_MASK_REG_UNION
 结构说明  : MOD_END_INT_MASK_REG 寄存器结构定义。地址偏移量:0x0030，初值:0xFFFFFFFF，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mod_end_int_mask : 32; /* bit[0-31]: [31:6]:保留
                                                           [5]:离线场景4底部clip结束中断mask
                                                           [4]:UVUP完成中断mask
                                                           [3]:VSCF1完成中断mask
                                                           [2]:VSCF0完成中断mask
                                                           [1]:ARSR1完成中断mask
                                                           [0]:ARSR0完成中断mask */
    } reg;
} SOC_DACC_MOD_END_INT_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_OV_INT_MASK_REG_UNION
 结构说明  : OV_INT_MASK_REG 寄存器结构定义。地址偏移量:0x0034，初值:0x0000000F，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_int_mask_0 : 1;  /* bit[0-0] : OV输出中断0 mask（低延迟接收使用） */
        unsigned int  ov_int_mask_1 : 1;  /* bit[1-1] : OV输出中断0 mask（低延迟接收使用） */
        unsigned int  ov_int_mask_2 : 1;  /* bit[2-2] : OV输出中断2 mask（低延迟接收使用） */
        unsigned int  ov_int_mask_3 : 1;  /* bit[3-3] : OV输出中断3 mask（低延迟接收使用） */
        unsigned int  reserved      : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_DACC_OV_INT_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MISC_INT_MASK_REG_UNION
 结构说明  : MISC_INT_MASK_REG 寄存器结构定义。地址偏移量:0x0038，初值:0x0003FFFF，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  te_0_mask                 : 1;  /* bit[0-0]  : TE0中断mask(静态配置，和屏幕类型强相关) */
        unsigned int  te_1_mask                 : 1;  /* bit[1-1]  : TE1中断mask(静态配置，和屏幕类型强相关) */
        unsigned int  dcmd_para_dma_mask        : 1;  /* bit[2-2]  : DCMD参数搬运DMA完成中断mask */
        unsigned int  hts_slave_err_mask        : 1;  /* bit[3-3]  : hts slave adapter异常中断mask */
        unsigned int  hts_master_err_mask       : 1;  /* bit[4-4]  : hts master adapter异常中断mask */
        unsigned int  hts_slave_int_mask        : 1;  /* bit[5-5]  : hts slave event中断mask */
        unsigned int  acpu_clear_mask           : 1;  /* bit[6-6]  : ACPU配置启动场景清场的RISCV中断mask(所有场景共用1个mask) */
        unsigned int  clear_ack_mask            : 1;  /* bit[7-7]  : 场景完成清场clear_ack的中断mask(所有场景共用1个mask) */
        unsigned int  doze_lfc_quit_mask        : 1;  /* bit[8-8]  : Doze2状态退出lfc_mode_quit中断mask */
        unsigned int  online_cld_int_mask       : 4;  /* bit[9-12] : 4个在线场景cld图层参数刷新中断mask(只有场景0有效) */
        unsigned int  online_underflow_int_mask : 4;  /* bit[13-16]: 4个在线场景underflow中断mask */
        unsigned int  scl_end_int_mask          : 1;  /* bit[17]   : SCL算子结束中断mask（保留） */
        unsigned int  reserved                  : 14; /* bit[18-31]: 保留 */
    } reg;
} SOC_DACC_MISC_INT_MASK_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MOD_EXIT_MEM_DS_REG_UNION
 结构说明  : MOD_EXIT_MEM_DS_REG 寄存器结构定义。地址偏移量:0x003c，初值:0x00000001，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lb_shutdown_cfg : 1;  /* bit[0-0] : LB shutdown配置 1：掉电 0：上电 */
        unsigned int  mod_exit_mem_ds : 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_DACC_MOD_EXIT_MEM_DS_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DSI_PORT_INT_CLR_REG_UNION
 结构说明  : DSI_PORT_INT_CLR_REG 寄存器结构定义。地址偏移量:0x0040，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dsi0_cmd_event_clr     : 1;  /* bit[0]    : dsi0_cmd_event_clr，清中断 */
        unsigned int  dsi0_frm_vactive_clr   : 1;  /* bit[1]    : dsi0_frm_vactive_clr，清中断 */
        unsigned int  dsi0_frm_end_clr       : 1;  /* bit[2]    : dsi0_frm_end_clr，清中断 */
        unsigned int  dsi0_frm_start_clr     : 1;  /* bit[3]    : dsi0_frm_start_clr，清中断 */
        unsigned int  dsi0_frm_underflow_clr : 1;  /* bit[4]    : dsi0_frm_underflow_clr，清中断 */
        unsigned int  dsi0_rsv               : 3;  /* bit[5-7]  : 保留 */
        unsigned int  dsi1_cmd_event_clr     : 1;  /* bit[8]    : dsi1_cmd_event_clr，清中断 */
        unsigned int  dsi1_frm_vactive_clr   : 1;  /* bit[9]    : dsi1_frm_vactive_clr，清中断 */
        unsigned int  dsi1_frm_end_clr       : 1;  /* bit[10]   : dsi1_frm_end_clr，清中断 */
        unsigned int  dsi1_frm_start_clr     : 1;  /* bit[11]   : dsi1_frm_start_clr，清中断 */
        unsigned int  dsi1_frm_underflow_clr : 1;  /* bit[12]   : dsi1_frm_underflow_clr，清中断 */
        unsigned int  dsi1_rsv               : 3;  /* bit[13-15]: 保留 */
        unsigned int  reserved               : 16; /* bit[16-31]: 保留(本寄存器全部用INT_WC) */
    } reg;
} SOC_DACC_DSI_PORT_INT_CLR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DP_PORT_INT_CLR_REG_UNION
 结构说明  : DP_PORT_INT_CLR_REG 寄存器结构定义。地址偏移量:0x0044，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dp0_cmd_event_clr     : 1;  /* bit[0]    : dp0_cmd_event_clr，清中断 */
        unsigned int  dp0_frm_vactive_clr   : 1;  /* bit[1]    : dp0_frm_vactive_clr，清中断 */
        unsigned int  dp0_frm_end_clr       : 1;  /* bit[2]    : dp0_frm_end_clr，清中断 */
        unsigned int  dp0_frm_start_clr     : 1;  /* bit[3]    : dp0_frm_start_clr，清中断 */
        unsigned int  dp0_frm_underflow_clr : 1;  /* bit[4]    : dp0_frm_underflow_clr，清中断 */
        unsigned int  dp0_rsv               : 3;  /* bit[5-7]  : 保留 */
        unsigned int  dp1_cmd_event_clr     : 1;  /* bit[8]    : dp1_cmd_event_clr，清中断 */
        unsigned int  dp1_frm_vactive_clr   : 1;  /* bit[9]    : dp1_frm_vactive_clr，清中断 */
        unsigned int  dp1_frm_end_clr       : 1;  /* bit[10]   : dp1_frm_end_clr，清中断 */
        unsigned int  dp1_frm_start_clr     : 1;  /* bit[11]   : dp1_frm_start_clr，清中断 */
        unsigned int  dp1_frm_underflow_clr : 1;  /* bit[12]   : dp1_frm_underflow_clr，清中断 */
        unsigned int  dp1_rsv               : 3;  /* bit[13-15]: 保留 */
        unsigned int  reserved              : 16; /* bit[16-31]: 保留(本寄存器全部用INT_WC) */
    } reg;
} SOC_DACC_DP_PORT_INT_CLR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MOD_END_INT_CLR_REG_UNION
 结构说明  : MOD_END_INT_CLR_REG 寄存器结构定义。地址偏移量:0x0048，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  arsr0_end_int_clr       : 1;  /* bit[0]   : ARSR0 运行结束清中断 */
        unsigned int  arsr1_end_int_clr       : 1;  /* bit[1]   : ARSR1 运行结束清中断 */
        unsigned int  vscf0_end_int_clr       : 1;  /* bit[2]   : VSCF0 运行结束清中断 */
        unsigned int  vscf1_end_int_clr       : 1;  /* bit[3]   : VSCF1 运行结束清中断 */
        unsigned int  uvup_end_int_clr        : 1;  /* bit[4]   : UVUP 运行结束清中断 */
        unsigned int  s4_bot_clip_end_int_clr : 1;  /* bit[5]   : 场景4底部clip结束清中断 */
        unsigned int  reserved                : 26; /* bit[6-31]: 保留(本寄存器全部用INT_WC) */
    } reg;
} SOC_DACC_MOD_END_INT_CLR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MISC_INT_CLR_REG_UNION
 结构说明  : MISC_INT_CLR_REG 寄存器结构定义。地址偏移量:0x004c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_int_0_clr             : 1;  /* bit[0-0]  : OV输出中断0清中断（低延迟接收使用） */
        unsigned int  ov_int_1_clr             : 1;  /* bit[1-1]  : OV输出中断1清中断（低延迟接收使用） */
        unsigned int  ov_int_2_clr             : 1;  /* bit[2-2]  : OV输出中断2清中断（低延迟接收使用） */
        unsigned int  ov_int_3_clr             : 1;  /* bit[3-3]  : OV输出中断3清中断（低延迟接收使用） */
        unsigned int  timer_0_clr              : 1;  /* bit[4-4]  : Timer 0清中断，Timer的中断mask由Timer IP专用表单配置实现；清除该中断时需优先清除Timer IP里的中断。 */
        unsigned int  timer_1_clr              : 1;  /* bit[5-5]  : Timer 1清中断，Timer的中断mask由Timer IP专用表单配置实现；清除该中断时需优先清除Timer IP里的中断。 */
        unsigned int  timer_2_clr              : 1;  /* bit[6-6]  : Timer 2清中断，Timer的中断mask由Timer IP专用表单配置实现；清除该中断时需优先清除Timer IP里的中断。 */
        unsigned int  timer_3_clr              : 1;  /* bit[7-7]  : Timer 3清中断，Timer的中断mask由Timer IP专用表单配置实现；清除该中断时需优先清除Timer IP里的中断。 */
        unsigned int  scene_0_clear_clr        : 1;  /* bit[8-8]  : ACPU产生的场景0清场中断清理 */
        unsigned int  scene_1_clear_clr        : 1;  /* bit[9-9]  : ACPU产生的场景1清场中断清理 */
        unsigned int  scene_2_clear_clr        : 1;  /* bit[10-10]: ACPU产生的场景2清场中断清理 */
        unsigned int  scene_3_clear_clr        : 1;  /* bit[11-11]: ACPU产生的场景3清场中断清理（保留） */
        unsigned int  scene_4_clear_clr        : 1;  /* bit[12-12]: ACPU产生的场景4清场中断清理 */
        unsigned int  scene_5_clear_clr        : 1;  /* bit[13-13]: ACPU产生的场景5清场中断清理（保留） */
        unsigned int  scene_6_clear_clr        : 1;  /* bit[14-14]: ACPU产生的场景6清场中断清理（保留） */
        unsigned int  hts_slave_err_clr        : 1;  /* bit[15-15]: hts slave adapter异常清中断 */
        unsigned int  hts_master_err_clr       : 1;  /* bit[16-16]: hts master adapter异常清中断 */
        unsigned int  hts_slave_int_clr        : 1;  /* bit[17-17]: hts slave event清中断 */
        unsigned int  online_cld_int_clr       : 4;  /* bit[18-21]: 4个在线场景cld图层参数刷新中断清理(只有场景0有效) */
        unsigned int  online_underflow_int_clr : 4;  /* bit[22-25]: 4个在线场景underflow中断清理 */
        unsigned int  reserved                 : 6;  /* bit[26-31]: 保留 */
    } reg;
} SOC_DACC_MISC_INT_CLR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_2_ACPU_REG_UNION
 结构说明  : RISCV_2_ACPU_REG 寄存器结构定义。地址偏移量:0x0050，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_2_acpu_info : 32; /* bit[0-31]: 该寄存器开放给RISCV核，用于RISCV核向ACPU传递与场景无关的状态信息。
                                                            [31:1]: reserved
                                                            [0]:为1表示RISCV已经完成所有初始化工作，ACPU可以启动业务；为0则表示初始化工作未完成 */
    } reg;
} SOC_DACC_RISCV_2_ACPU_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_MINIBUS_1T01_CKG_EN_UNION
 结构说明  : MINIBUS_1T01_CKG_EN 寄存器结构定义。地址偏移量:0x0054，初值:0x00000000，宽度:32
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
 结构说明  : MINIBUS_1T01_STATE 寄存器结构定义。地址偏移量:0x0058，初值:0x00000000，宽度:32
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
 结构说明  : CKG_EN 寄存器结构定义。地址偏移量:0x005c，初值:0x00000001，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ckg_en              : 1;  /* bit[0-0] : 门控 */
        unsigned int  tcxo_dbg_counter_en : 1;  /* bit[1-1] : TCXO时钟计数使能（维测使用） 1：计数 0：不计数 */
        unsigned int  reserved            : 30; /* bit[2-31]:  */
    } reg;
} SOC_DACC_CKG_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LUT_LAYER_ID_UNION
 结构说明  : LUT_LAYER_ID 寄存器结构定义。地址偏移量:0x0060，初值:0x20000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  demura_lut_layer_id   : 5;  /* bit[0-4]  : demura lut图层ID */
        unsigned int  demura_lut_en         : 1;  /* bit[5-5]  : demura lut图层使能 */
        unsigned int  deburnin_lut_layer_id : 5;  /* bit[6-10] : deburnin lut图层ID */
        unsigned int  deburnin_lut_en       : 1;  /* bit[11-11]: deburnin lut图层使能 */
        unsigned int  od_lut_layer_id       : 5;  /* bit[12-16]: OD lut图层ID */
        unsigned int  od_lut_en             : 1;  /* bit[17-17]: OD lut图层使能 */
        unsigned int  idle_display_en       : 1;  /* bit[18-18]: Idle送显使能 */
        unsigned int  cld_refresh_line_num  : 13; /* bit[19-31]: CLD图层参数切换行数 */
    } reg;
} SOC_DACC_LUT_LAYER_ID_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LP_DOZE_CFG_UNION
 结构说明  : LP_DOZE_CFG 寄存器结构定义。地址偏移量:0x0064，初值:0x00000004，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cfg_sctrl_doze1_entry : 1;  /* bit[0-0] : 投票DSS可进入doze1状态（RISCV配置） */
        unsigned int  cfg_sctrl_doze1_wake  : 1;  /* bit[1-1] : 投票DSS需从doze1状态唤醒（RISCV配置） */
        unsigned int  dss_clk_gate_on       : 1;  /* bit[2-2] : 使能除DACC和配置通路之外其他模块时钟（RISCV配置）
                                                                1：开钟 0：关钟 */
        unsigned int  reserved              : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_LP_DOZE_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_TE_MASK_INTERVAL_UNION
 结构说明  : TE_MASK_INTERVAL 寄存器结构定义。地址偏移量:0x0068，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  te_mask_interval_0 : 4;  /* bit[0-3] : TE0信号RSICV中断mask间隔
                                                             0：无过滤
                                                             1：每2个TE过滤1个，即RISCV只收到1/2的TE0中断
                                                             2：每3个TE过滤2个，即RISCV只收到1/3的TE0中断
                                                             ……
                                                             15:每16个TE过滤15个，即RISCV只收到1/16的TE0中断 */
        unsigned int  te_mask_interval_1 : 4;  /* bit[4-7] : 功能同te_mask_interval_0 */
        unsigned int  reserved           : 24; /* bit[8-31]: 保留 */
    } reg;
} SOC_DACC_TE_MASK_INTERVAL_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_2_RISCV_INTR_TRIG_UNION
 结构说明  : ACPU_2_RISCV_INTR_TRIG 寄存器结构定义。地址偏移量:0x006c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_2_riscv_intr_trig : 16; /* bit[0-15] : ACPU配置触发RISCV中断 */
        unsigned int  reserved               : 16; /* bit[16-31]: 保留 */
    } reg;
} SOC_DACC_ACPU_2_RISCV_INTR_TRIG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_2_RISCV_INTR_MASK_UNION
 结构说明  : ACPU_2_RISCV_INTR_MASK 寄存器结构定义。地址偏移量:0x0070，初值:0x0000FFFF，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_2_riscv_intr_mask : 16; /* bit[0-15] : ACPU配置触发RISCV中断mask */
        unsigned int  reserved               : 16; /* bit[16-31]: 保留 */
    } reg;
} SOC_DACC_ACPU_2_RISCV_INTR_MASK_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_2_RISCV_INTR_CLR_UNION
 结构说明  : ACPU_2_RISCV_INTR_CLR 寄存器结构定义。地址偏移量:0x0074，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_2_riscv_intr_clr : 16; /* bit[0-15] : ACPU配置触发RISCV中断清理 */
        unsigned int  reserved              : 16; /* bit[16-31]: 保留 */
    } reg;
} SOC_DACC_ACPU_2_RISCV_INTR_CLR_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WB_SOURCE_SEL_REG_UNION
 结构说明  : WB_SOURCE_SEL_REG 寄存器结构定义。地址偏移量:0x0078，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wb_source_sel : 5;  /* bit[0-4] : 回写源选择（RISCV配置）
                                                        0：无回写
                                                        1：ODC统计回写
                                                        2：DDIC/DSC回写
                                                        3：OV回写
                                                        4：ODC info回写（维测）
                                                        5：Demura info回写（维测）
                                                        6：Debernin info回写（维测）
                                                        …… */
        unsigned int  reserved      : 27; /* bit[5-31]: 保留 */
    } reg;
} SOC_DACC_WB_SOURCE_SEL_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_TCXO_CNTER_VALUE_UNION
 结构说明  : TCXO_CNTER_VALUE 寄存器结构定义。地址偏移量:0x007c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  tcxo_dbg_counter   : 28; /* bit[0-27] : TCXO时钟计数值（维测使用） */
        unsigned int  reserved           : 3;  /* bit[28-30]: 保留 */
        unsigned int  lb_shutdown_status : 1;  /* bit[31-31]: LB shutdown状态 1：掉电状态 0：上电状态 */
    } reg;
} SOC_DACC_TCXO_CNTER_VALUE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_VER_ID_REG_UNION
 结构说明  : VER_ID_REG 寄存器结构定义。地址偏移量:0x00fc，初值:0x00000011，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  version_id : 8;  /* bit[0-7] : [7:4]: DSS架构ID（0x1代表Nashville架构）
                                                     [3:0]: DSS某一架构ID的子版本ID（0x0对应ES，0x1对应CS） */
        unsigned int  reserved   : 24; /* bit[8-31]: 保留 */
    } reg;
} SOC_DACC_VER_ID_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_EN_UNION
 结构说明  : WLT_EN 寄存器结构定义。地址偏移量:0x0100，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_en       : 1;  /* bit[0-0]  : 低延迟模式使能(只支持压缩格式) */
        unsigned int  reserved_0   : 7;  /* bit[1-7]  : reserved */
        unsigned int  wlt_layer_id : 5;  /* bit[8-12] : 低延迟模式图层ID */
        unsigned int  reserved_1   : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_DACC_WLT_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE0_DDR_ADDR0 寄存器结构定义。地址偏移量:0x0104，初值:0x00000000，宽度:32
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
 结构说明  : WLT_SLICE0_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0108，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_ddr_addr1 : 4;  /* bit[0-3] : Slice0地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_WLT_SLICE0_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE1_DDR_ADDR0 寄存器结构定义。地址偏移量:0x010c，初值:0x00000000，宽度:32
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
 结构说明  : WLT_SLICE1_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0110，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_ddr_addr1 : 4;  /* bit[0-3] : Slice1地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_WLT_SLICE1_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE2_DDR_ADDR0 寄存器结构定义。地址偏移量:0x0114，初值:0x00000000，宽度:32
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
 结构说明  : WLT_SLICE2_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0118，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_ddr_addr1 : 4;  /* bit[0-3] : Slice2地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_WLT_SLICE2_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_DDR_ADDR0_UNION
 结构说明  : WLT_SLICE3_DDR_ADDR0 寄存器结构定义。地址偏移量:0x011c，初值:0x00000000，宽度:32
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
 结构说明  : WLT_SLICE3_DDR_ADDR1 寄存器结构定义。地址偏移量:0x0120，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_ddr_addr1 : 4;  /* bit[0-3] : Slice3地址信息在DDR中的存放地址高4位 */
        unsigned int  reserved             : 28; /* bit[4-31]: reserved */
    } reg;
} SOC_DACC_WLT_SLICE3_DDR_ADDR1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR0_H_UNION
 结构说明  : WLT_SLICE0_ADDR0_H 寄存器结构定义。地址偏移量:0x0124，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr0_h : 4;  /* bit[0-3]  : Slice0压缩图层：图层Y平面/RGB payload起始地址 */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : reserved */
        unsigned int  wlt_slice0_addr1_h : 4;  /* bit[8-11] : Slice0压缩图层：图层Y平面/RGB header起始地址 */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: reserved */
        unsigned int  wlt_slice0_addr2_h : 4;  /* bit[16-19]: Slice0压缩图层：图层C平面 payload起始地址 */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: reserved */
        unsigned int  wlt_slice0_addr3_h : 4;  /* bit[24-27]: Slice0压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: reserved */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR0_L_UNION
 结构说明  : WLT_SLICE0_ADDR0_L 寄存器结构定义。地址偏移量:0x0128，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr0_l : 32; /* bit[0-31]: Slice0压缩图层：图层Y平面/RGB payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR1_L_UNION
 结构说明  : WLT_SLICE0_ADDR1_L 寄存器结构定义。地址偏移量:0x012c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr1_l : 32; /* bit[0-31]: Slice0压缩图层：图层Y平面/RGB header起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR2_L_UNION
 结构说明  : WLT_SLICE0_ADDR2_L 寄存器结构定义。地址偏移量:0x0130，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_addr2_l : 32; /* bit[0-31]: Slice0压缩图层：图层C平面 payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE0_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE0_ADDR3_L_UNION
 结构说明  : WLT_SLICE0_ADDR3_L 寄存器结构定义。地址偏移量:0x0134，初值:0x00000000，宽度:32
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
 结构名    : SOC_DACC_WLT_SLICE0_STRIDE_UNION
 结构说明  : WLT_SLICE0_STRIDE 寄存器结构定义。地址偏移量:0x0138，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice0_stride : 32; /* bit[0-31]: Slice0的YUV444 planer时 U平面的slice_addr相对于y平面slice_addr的偏移 */
    } reg;
} SOC_DACC_WLT_SLICE0_STRIDE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR0_H_UNION
 结构说明  : WLT_SLICE1_ADDR0_H 寄存器结构定义。地址偏移量:0x013c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr0_h : 4;  /* bit[0-3]  : Slice1压缩图层：图层Y平面/RGB payload起始地址 */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : reserved */
        unsigned int  wlt_slice1_addr1_h : 4;  /* bit[8-11] : Slice1压缩图层：图层Y平面/RGB header起始地址 */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: reserved */
        unsigned int  wlt_slice1_addr2_h : 4;  /* bit[16-19]: Slice1压缩图层：图层C平面 payload起始地址 */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: reserved */
        unsigned int  wlt_slice1_addr3_h : 4;  /* bit[24-27]: Slice1压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR0_L_UNION
 结构说明  : WLT_SLICE1_ADDR0_L 寄存器结构定义。地址偏移量:0x0140，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr0_l : 32; /* bit[0-31]: Slice1压缩图层：图层Y平面/RGB payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR1_L_UNION
 结构说明  : WLT_SLICE1_ADDR1_L 寄存器结构定义。地址偏移量:0x0144，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr1_l : 32; /* bit[0-31]: Slice1压缩图层：图层Y平面/RGB header起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR2_L_UNION
 结构说明  : WLT_SLICE1_ADDR2_L 寄存器结构定义。地址偏移量:0x0148，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_addr2_l : 32; /* bit[0-31]: Slice1压缩图层：图层C平面 payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE1_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE1_ADDR3_L_UNION
 结构说明  : WLT_SLICE1_ADDR3_L 寄存器结构定义。地址偏移量:0x014c，初值:0x00000000，宽度:32
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
 结构名    : SOC_DACC_WLT_SLICE1_STRIDE_UNION
 结构说明  : WLT_SLICE1_STRIDE 寄存器结构定义。地址偏移量:0x0150，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice1_stride : 32; /* bit[0-31]: Slice1的YUV444 planer时 U平面的slice_addr相对于y平面slice_addr的偏移 */
    } reg;
} SOC_DACC_WLT_SLICE1_STRIDE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR0_H_UNION
 结构说明  : WLT_SLICE2_ADDR0_H 寄存器结构定义。地址偏移量:0x0154，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr0_h : 4;  /* bit[0-3]  : Slice2压缩图层：图层Y平面/RGB payload起始地址 */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : reserved */
        unsigned int  wlt_slice2_addr1_h : 4;  /* bit[8-11] : Slice2压缩图层：图层Y平面/RGB header起始地址 */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: reserved */
        unsigned int  wlt_slice2_addr2_h : 4;  /* bit[16-19]: Slice2压缩图层：图层C平面 payload起始地址 */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: reserved */
        unsigned int  wlt_slice2_addr3_h : 4;  /* bit[24-27]: Slice2压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR0_L_UNION
 结构说明  : WLT_SLICE2_ADDR0_L 寄存器结构定义。地址偏移量:0x0158，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr0_l : 32; /* bit[0-31]: Slice2压缩图层：图层Y平面/RGB payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR1_L_UNION
 结构说明  : WLT_SLICE2_ADDR1_L 寄存器结构定义。地址偏移量:0x015c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr1_l : 32; /* bit[0-31]: Slice2压缩图层：图层Y平面/RGB header起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR2_L_UNION
 结构说明  : WLT_SLICE2_ADDR2_L 寄存器结构定义。地址偏移量:0x0160，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_addr2_l : 32; /* bit[0-31]: Slice2压缩图层：图层C平面 payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE2_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE2_ADDR3_L_UNION
 结构说明  : WLT_SLICE2_ADDR3_L 寄存器结构定义。地址偏移量:0x0164，初值:0x00000000，宽度:32
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
 结构名    : SOC_DACC_WLT_SLICE2_STRIDE_UNION
 结构说明  : WLT_SLICE2_STRIDE 寄存器结构定义。地址偏移量:0x0168，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice2_stride : 32; /* bit[0-31]: Slice2的YUV444 planer时 U平面的slice_addr相对于y平面slice_addr的偏移 */
    } reg;
} SOC_DACC_WLT_SLICE2_STRIDE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR0_H_UNION
 结构说明  : WLT_SLICE3_ADDR0_H 寄存器结构定义。地址偏移量:0x016c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr0_h : 4;  /* bit[0-3]  : Slice3压缩图层：图层Y平面/RGB payload起始地址 */
        unsigned int  reserved_0         : 4;  /* bit[4-7]  : reserved */
        unsigned int  wlt_slice3_addr1_h : 4;  /* bit[8-11] : Slice3压缩图层：图层Y平面/RGB header起始地址 */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: reserved */
        unsigned int  wlt_slice3_addr2_h : 4;  /* bit[16-19]: Slice3压缩图层：图层C平面 payload起始地址 */
        unsigned int  reserved_2         : 4;  /* bit[20-23]: reserved */
        unsigned int  wlt_slice3_addr3_h : 4;  /* bit[24-27]: Slice3压缩图层：图层C平面 header起始地址高4bit */
        unsigned int  reserved_3         : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR0_H_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR0_L_UNION
 结构说明  : WLT_SLICE3_ADDR0_L 寄存器结构定义。地址偏移量:0x0170，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr0_l : 32; /* bit[0-31]: Slice3压缩图层：图层Y平面/RGB payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR0_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR1_L_UNION
 结构说明  : WLT_SLICE3_ADDR1_L 寄存器结构定义。地址偏移量:0x0174，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr1_l : 32; /* bit[0-31]: Slice3压缩图层：图层Y平面/RGB header起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR1_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR2_L_UNION
 结构说明  : WLT_SLICE3_ADDR2_L 寄存器结构定义。地址偏移量:0x0178，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_addr2_l : 32; /* bit[0-31]: Slice3压缩图层：图层C平面 payload起始地址 */
    } reg;
} SOC_DACC_WLT_SLICE3_ADDR2_L_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE3_ADDR3_L_UNION
 结构说明  : WLT_SLICE3_ADDR3_L 寄存器结构定义。地址偏移量:0x017c，初值:0x00000000，宽度:32
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
 结构名    : SOC_DACC_WLT_SLICE3_STRIDE_UNION
 结构说明  : WLT_SLICE3_STRIDE 寄存器结构定义。地址偏移量:0x0180，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wlt_slice3_stride : 32; /* bit[0-31]: Slice3的YUV444 planer时 U平面的slice_addr相对于y平面slice_addr的偏移 */
    } reg;
} SOC_DACC_WLT_SLICE3_STRIDE_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WLT_SLICE_ROW_UNION
 结构说明  : WLT_SLICE_ROW 寄存器结构定义。地址偏移量:0x0184，初值:0x00000000，宽度:32
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
 结构说明  : WLT_CMDLIST_REFRESH_OFFSET 寄存器结构定义。地址偏移量:0x0188，初值:0x00000000，宽度:32
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
 结构名    : SOC_DACC_OV_COUNTER_CLR_UNION
 结构说明  : OV_COUNTER_CLR 寄存器结构定义。地址偏移量:0x018c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_counter_clr : 1;  /* bit[0-0] : 低延迟场景，每帧起始对OV行计数器清0 */
        unsigned int  reserved       : 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_DACC_OV_COUNTER_CLR_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_FRESH_RATE_0_UNION
 结构说明  : LTPO_FRESH_RATE_0 寄存器结构定义。地址偏移量:0x01a0，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_fresh_rate_0 : 5;  /* bit[0-4] : LTPO帧率配置0（最大支持32种帧率，ACPU配置） */
        unsigned int  reserved          : 27; /* bit[5-31]: reserved */
    } reg;
} SOC_DACC_LTPO_FRESH_RATE_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_FRESH_RATE_1_UNION
 结构说明  : LTPO_FRESH_RATE_1 寄存器结构定义。地址偏移量:0x01a4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_fresh_rate_1 : 5;  /* bit[0-4] : LTPO帧率配置1（最大支持32种帧率，ACPU配置） */
        unsigned int  reserved          : 27; /* bit[5-31]: reserved */
    } reg;
} SOC_DACC_LTPO_FRESH_RATE_1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_DATA_LENGTH_0_UNION
 结构说明  : LTPO_DATA_LENGTH_0 寄存器结构定义。地址偏移量:0x01a8，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_fresh_length_0 : 10; /* bit[0-9]  : LTPO帧率配置0参数长度(单位：32bit) */
        unsigned int  reserved            : 22; /* bit[10-31]: reserved */
    } reg;
} SOC_DACC_LTPO_DATA_LENGTH_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_DATA_LENGTH_1_UNION
 结构说明  : LTPO_DATA_LENGTH_1 寄存器结构定义。地址偏移量:0x01ac，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_fresh_length_1 : 10; /* bit[0-9]  : LTPO帧率配置1参数长度（单位：32bit） */
        unsigned int  reserved            : 22; /* bit[10-31]: reserved */
    } reg;
} SOC_DACC_LTPO_DATA_LENGTH_1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_START_ADDR_0_UNION
 结构说明  : LTPO_START_ADDR_0 寄存器结构定义。地址偏移量:0x01b0，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_start_addr_0 : 10; /* bit[0-9]  : LTPO帧率参数0在LTPO RAM内起始地址（单位：32bit） */
        unsigned int  reserved          : 22; /* bit[10-31]: reserved */
    } reg;
} SOC_DACC_LTPO_START_ADDR_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_START_ADDR_1_UNION
 结构说明  : LTPO_START_ADDR_1 寄存器结构定义。地址偏移量:0x01b4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_start_addr_1 : 10; /* bit[0-9]  : LTPO帧率参数1在LTPO RAM内起始地址（单位：32bit） */
        unsigned int  reserved          : 22; /* bit[10-31]: reserved */
    } reg;
} SOC_DACC_LTPO_START_ADDR_1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_FRESH_RATE_TRIG_0_UNION
 结构说明  : ACPU_FRESH_RATE_TRIG_0 寄存器结构定义。地址偏移量:0x01b8，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_fresh_rate_trig_0 : 1;  /* bit[0-0] : ACPU配置启动变帧率流程（LTPO_0） */
        unsigned int  reserved               : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_ACPU_FRESH_RATE_TRIG_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_FRESH_RATE_TRIG_1_UNION
 结构说明  : ACPU_FRESH_RATE_TRIG_1 寄存器结构定义。地址偏移量:0x01bc，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_fresh_rate_trig_1 : 1;  /* bit[0-0] : ACPU配置启动变帧率流程（LTPO_1） */
        unsigned int  reserved               : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_ACPU_FRESH_RATE_TRIG_1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_FRESH_RATE_TRIG_0_UNION
 结构说明  : RISCV_FRESH_RATE_TRIG_0 寄存器结构定义。地址偏移量:0x01c0，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_fresh_rate_trig_0 : 1;  /* bit[0-0] : RISCV配置启动变帧率流程（LTPO_0） */
        unsigned int  reserved                : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_RISCV_FRESH_RATE_TRIG_0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_FRESH_RATE_TRIG_1_UNION
 结构说明  : RISCV_FRESH_RATE_TRIG_1 寄存器结构定义。地址偏移量:0x01c4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_fresh_rate_trig_1 : 1;  /* bit[0-0] : RISCV配置启动变帧率流程（LTPO_1） */
        unsigned int  reserved                : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_RISCV_FRESH_RATE_TRIG_1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LTPO_CFG_STATUS_UNION
 结构说明  : LTPO_CFG_STATUS 寄存器结构定义。地址偏移量:0x01c8，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ltpo_cfg_status_0 : 1;  /* bit[0-0] : RISCV配置流程0状态
                                                            1：配置过程中 0：未配置或已配置完 */
        unsigned int  ltpo_cfg_status_1 : 1;  /* bit[1-1] : RISCV配置流程1状态
                                                            1：配置过程中 0：未配置或已配置完 */
        unsigned int  reserved          : 30; /* bit[2-31]: reserved */
    } reg;
} SOC_DACC_LTPO_CFG_STATUS_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_CMD_REG0_UNION
 结构说明  : SW_CMD_REG0 寄存器结构定义。地址偏移量:0x01e0，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_cmd_reg0 : 32; /* bit[0-31]: 存描述符低32bit(下发cmdlist指令) */
    } reg;
} SOC_DACC_SW_CMD_REG0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_CMD_REG1_UNION
 结构说明  : SW_CMD_REG1 寄存器结构定义。地址偏移量:0x01e4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_cmd_reg1 : 32; /* bit[0-31]: 存描述符高32bit(下发cmdlist指令) */
    } reg;
} SOC_DACC_SW_CMD_REG1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CMD_BUF_NUM_UNION
 结构说明  : CMD_BUF_NUM 寄存器结构定义。地址偏移量:0x01e8，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cmd_buf_num : 5;  /* bit[0-4] : Cmdlist buffer剩余指令数据量（单位：64bit指令）
                                                      Buffer深度16，RISCV软件判断剩余数据量为16时（已满），不能再下发指令。 */
        unsigned int  reserved    : 27; /* bit[5-31]: reserved */
    } reg;
} SOC_DACC_CMD_BUF_NUM_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SW_CMD_TX_REG_UNION
 结构说明  : SW_CMD_TX_REG 寄存器结构定义。地址偏移量:0x01ec，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sw_cmd_tx : 1;  /* bit[0-0] : 启动Cmdlist描述符下发 */
        unsigned int  reserved  : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_SW_CMD_TX_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_MASK_EN_UNION
 结构说明  : LAYER_MASK_EN 寄存器结构定义。地址偏移量:0x0300，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mask_layer_id       : 4;  /* bit[0-3]  : 动态mask图层的layer id */
        unsigned int  mask_layer_format   : 6;  /* bit[4-9]  : 动态mask图层图像格式 */
        unsigned int  mask_layer_en       : 1;  /* bit[10]   : 动态mask图层使能 */
        unsigned int  layer_ax_swap       : 1;  /* bit[11]   : 对应DM页的layer_ov_dfc_ax_swap(mask图层) */
        unsigned int  mask_min_line       : 4;  /* bit[12-15]: mask图层全覆盖区域最小行数设置 */
        unsigned int  mask_alpha_value    : 10; /* bit[16-25]: 动态mask图层的alpha分量 */
        unsigned int  mask_ahead_line_cfg : 6;  /* bit[26-31]: mask图层提前搬运行数 */
    } reg;
} SOC_DACC_LAYER_MASK_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_MASK_SCL_CFG_UNION
 结构说明  : LAYER_MASK_SCL_CFG 寄存器结构定义。地址偏移量:0x0304，初值:0x00010000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  scl_value_fraction : 16; /* bit[0-15] : Mask缩放图层缩放倍数的小数部分 */
        unsigned int  scl_value_integer  : 6;  /* bit[16-21]: Mask缩放图层缩放倍数的整数部分（缩小最大2:1，放大最大1:60） */
        unsigned int  reserved           : 2;  /* bit[22-23]: reserved */
        unsigned int  scl_protect_value  : 8;  /* bit[24-31]: [7:4]: 缩放图层上端保护区域
                                                              [3:0]: 缩放图层下端保护区域 */
    } reg;
} SOC_DACC_LAYER_MASK_SCL_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_MASKED_EN_UNION
 结构说明  : LAYER_MASKED_EN 寄存器结构定义。地址偏移量:0x0308，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  masked_layer_en   : 16; /* bit[0-15] : 动态mask特性使能时，layer_id小于mask图层layer_id的图层，无旋转无缩放无静态mask且无strench_en时，对应bit设置为1（表示可参与mask区域计算），否则设置为0. */
        unsigned int  mask_layer_height : 13; /* bit[16-28]: mask图层高度（减1处理） */
        unsigned int  mask_dma_block_h  : 3;  /* bit[29-31]: hebc格式：对应superblock_h（减1处理）
                                                             linear格式：对应dma_block_h（减1处理） */
    } reg;
} SOC_DACC_LAYER_MASKED_EN_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLK_SEL_REG_UNION
 结构说明  : CLK_SEL_REG 寄存器结构定义。地址偏移量:0x0340+(s12)*0x8，初值:0x00000001，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dacc_clk_sel : 1;  /* bit[0-0] : 场景时钟强开选择寄存器 */
        unsigned int  reserved     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CLK_SEL_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLK_EN_REG_UNION
 结构说明  : CLK_EN_REG 寄存器结构定义。地址偏移量:0x0344+(s12)*0x8，初值:0x00000001，宽度:32
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
 结构名    : SOC_DACC_ACPU_CLR_START_UNION
 结构说明  : ACPU_CLR_START 寄存器结构定义。地址偏移量:0x0380+(s12)*0x10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_clr_start : 1;  /* bit[0-0] : ACPU配置，触发RISCV清场中断，之后RISCV启动清场流程
                                                         连接到RISCV中断管脚 */
        unsigned int  reserved       : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_ACPU_CLR_START_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_CLR_START_UNION
 结构说明  : RISCV_CLR_START 寄存器结构定义。地址偏移量:0x0384+(s12)*0x10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_clr_start : 1;  /* bit[0-0] : RISCV配置，启动子系统清场流程；清场结束后该寄存器配置为0 */
        unsigned int  reserved        : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_RISCV_CLR_START_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_UNION
 结构说明  : CTL_CLEAR_TIMEOUT_THR_REG 寄存器结构定义。地址偏移量:0x0388+(s12)*0x10，初值:0x8004B1EC，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_clear_timeout_thr : 31; /* bit[0-30] : 场景清场超时阈值，19.2MHz时钟的个数，如超时时间设置16ms，该值应该设置为16000us/52ns = 307692 = 0x4B1EC */
        unsigned int  clr_timeout_ints_mask : 1;  /* bit[31-31]: 清场超时中断MASK信号，1表示Mask,0表示不Mask
                                                                 只MASK送到ACPU的中断，不MASK到RISCV的中断 */
    } reg;
} SOC_DACC_CTL_CLEAR_TIMEOUT_THR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLR_TIMEOUT_STATUS_REG_UNION
 结构说明  : CLR_TIMEOUT_STATUS_REG 寄存器结构定义。地址偏移量:0x038c+(s12)*0x10，初值:0x00000000，宽度:32
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
 结构名    : SOC_DACC_NEW_FRAME_RDY_CFG_UNION
 结构说明  : NEW_FRAME_RDY_CFG 寄存器结构定义。地址偏移量:0x0404+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  new_frame_rdy_cfg : 1;  /* bit[0-0] : （ACPU配置）帧配置参数在DDR内准备标识；送显当帧过程中，下帧参数ready则配置该寄存机器，使RISCV启动下帧参数计算流程 */
        unsigned int  reserved          : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_NEW_FRAME_RDY_CFG_UNION;
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
        unsigned int  ctl_sw_reload_en : 1;  /* bit[0-0] : 场景安全配置信息reload使能 */
        unsigned int  reserved         : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CTL_SW_EN_RELOAD_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_CFG_OVER_REG_UNION
 结构说明  : RISCV_CFG_OVER_REG 寄存器结构定义。地址偏移量:0x0410+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_cfg_over : 1;  /* bit[0-0] : （RISCV配置）通知下游所有参数已ready，开始下发首行指令 */
        unsigned int  reserved       : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_RISCV_CFG_OVER_REG_UNION;
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
        unsigned int  ctl_sw_intr_en : 1;  /* bit[0-0] : reserved */
        unsigned int  reserved       : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CTL_SW_INTR_EN_REG_UNION;
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
        unsigned int  ctl_cfg_mode : 1;  /* bit[0-0] : 配置模式：
                                                       0: CPU配置模式；
                                                       1：CMDLIST配置模式； */
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
        unsigned int  ctl_clear_timeout_en : 1;  /* bit[0-0] : reserved */
        unsigned int  reserved             : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_CTL_CLEAR_TIMEOUT_EN_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_2_RISCV_LOG_REG_UNION
 结构说明  : ACPU_2_RISCV_LOG_REG 寄存器结构定义。地址偏移量:0x043C+(s12)*0x400，初值:0x000001A0，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_2_riscv_log_info : 32; /* bit[0-31]: 开放给ACPU，用于ACPU与RISCV核进行交互；ACPU可向该寄存器写入状态信息，由RISCV核读取进行判断（每场景1个寄存器） */
    } reg;
} SOC_DACC_ACPU_2_RISCV_LOG_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_2_ACPU_LOG_REG_UNION
 结构说明  : RISCV_2_ACPU_LOG_REG 寄存器结构定义。地址偏移量:0x0440+(s12)*0x400，初值:0x000001A0，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_2_acpu_log_info : 32; /* bit[0-31]: 开放给RISCV核，用于RISCV核与ACPU进行交互；RISCV核可向该寄存器写入状态信息，由ACPU读取进行判断（每场景1个寄存器） */
    } reg;
} SOC_DACC_RISCV_2_ACPU_LOG_REG_UNION;
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
        unsigned int  scmd_start : 1;  /* bit[0-0] : 保留 */
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
        unsigned int  dcmd_start : 1;  /* bit[0-0] : (RISCV配置)dcmd模块启动信号，开始下发指令 */
        unsigned int  reserved   : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_DCMD_START_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ACPU_PARA_ADDR_REG_UNION
 结构说明  : ACPU_PARA_ADDR_REG 寄存器结构定义。地址偏移量:0x0458+(s12)*0x400，初值:0x00014000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  acpu_para_addr : 18; /* bit[0-17] : 本场景软件顶层接口参数在DACC空间的起始偏移地址 */
        unsigned int  reserved       : 14; /* bit[18-31]:  */
    } reg;
} SOC_DACC_ACPU_PARA_ADDR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_RISCV_PARA_ADDR_REG_UNION
 结构说明  : RISCV_PARA_ADDR_REG 寄存器结构定义。地址偏移量:0x045C+(s12)*0x400，初值:0x00016000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  riscv_para_addr : 18; /* bit[0-17] : 本场景RISCV计算的中间参数在DACC空间的起始偏移地址 */
        unsigned int  reserved        : 14; /* bit[18-31]:  */
    } reg;
} SOC_DACC_RISCV_PARA_ADDR_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_DIM_FLAG_REG_UNION
 结构说明  : LAYER_DIM_FLAG_REG 寄存器结构定义。地址偏移量:0x0460+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_dim_flag : 16; /* bit[0-15] : bit0~15分别对应图层0到图层15是否为DIM图层的标识。RISCV配置DCMD使用。安全图层对DIM图层设置不感知。
                                                          1：该图层为DIM图层 0：该图层不是DIM图层 */
        unsigned int  reserved       : 16; /* bit[16-31]:  */
    } reg;
} SOC_DACC_LAYER_DIM_FLAG_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_GENERAL_CFG_0_REG_UNION
 结构说明  : GENERAL_CFG_0_REG 寄存器结构定义。地址偏移量:0x0464+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  general_cfg_0 : 32; /* bit[0-31]: 通用配置参数0（用于ACPU和RISCV信息交互） */
    } reg;
} SOC_DACC_GENERAL_CFG_0_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_GENERAL_CFG_1_REG_UNION
 结构说明  : GENERAL_CFG_1_REG 寄存器结构定义。地址偏移量:0x0468+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  general_cfg_1 : 32; /* bit[0-31]: 通用配置参数1（用于ACPU和RISCV信息交互） */
    } reg;
} SOC_DACC_GENERAL_CFG_1_REG_UNION;
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
 寄存器说明:
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
        unsigned int  pixel_layer_num : 5;  /* bit[0-4]  : 当前场景的pixel图层数 */
        unsigned int  reserved_0      : 3;  /* bit[5-7]  : 保留 */
        unsigned int  lut_layer_num   : 2;  /* bit[8-9]  : 当前场景的LUT图层数 */
        unsigned int  reserved_1      : 22; /* bit[10-31]: reserved */
    } reg;
} SOC_DACC_LAYER_NUM_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CTL_SW_START_REG_UNION
 结构说明  : CTL_SW_START_REG 寄存器结构定义。地址偏移量:0x1480，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ctl_sw_start : 1;  /* bit[0-0] : （ACPU）配置离线场景启动中断
                                                       只有离线场景4有效，该信号接到RISC-V的中断 */
        unsigned int  reserved     : 31; /* bit[1-31]: reserved */
    } reg;
} SOC_DACC_CTL_SW_START_REG_UNION;
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
        unsigned int  scen_sec_en            : 2;  /* bit[0-1]  : 安全支付图层配置：
                                                                  0：无安全图层；
                                                                  1：有1个安全图层，即安全支付图层0；（如果只有1个安全支付图层，要求使用安全支付图层0）
                                                                  2：无效配置；
                                                                  3：有2个安全图层，安全支付图层0和1；（2个安全图层时，安全支付图层1对应的layer id一定比安全支付图层0对应的layer id大） */
        unsigned int  secure_index0          : 2;  /* bit[2-3]  : 安全支付图层0对应索引（保留，NSV不支持4个TUI图层乒乓） */
        unsigned int  secure_index1          : 2;  /* bit[4-5]  : 安全支付图层1对应索引（保留，NSV不支持4个TUI图层乒乓） */
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
        unsigned int  scen_secure_layer_dm_base_adr : 32; /* bit[0-31]: 场景0~3的该寄存器保留
                                                                        场景4的该寄存器，[31:16]保留
                                                                        [15:0]：离线单图层的DDR原始图层高度（clip前）*图层平面个数（无减1配置） */
    } reg;
} SOC_DACC_SCEN_SECURE_DM_ADDR_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_TUI_CMDLIST_ADDR_UNION
 结构说明  : TUI_CMDLIST_ADDR 寄存器结构定义。地址偏移量:0x0508+(s12)*0x400，初值:0xFFFFFFFF，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  tui_cmdlist_addr : 32; /* bit[0-31]: 安全支付图层对应的Cmdlist节点地址 */
    } reg;
} SOC_DACC_TUI_CMDLIST_ADDR_UNION;
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
        unsigned int  scen_sec_wch_en      : 1;  /* bit[5]   : 是否能够经过WCH处理
                                                               1：能经过WCH处理
                                                               0：不能经过WCH处理 */
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
        unsigned int  ping_ctl0_layer_secu_en0 : 32; /* bit[0-31]: 31:30 layer15安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   29:28 layer14安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   27:26 layer13安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   25:24 layer12安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   23:22 layer11安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   21:20 layer10安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   19:18 layer9安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   17:16 layer8安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   15:14 layer7安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   13:12 layer6安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   11:10 layer5安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   9:8 layer4安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   7:6 layer3安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   5:4 layer2安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   3:2 layer1安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频
                                                                   1:0 layer0安全属性:2'd0为非安全，2'd1为安全支付图层，2'd2为安全视频 */
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
        unsigned int  ping_ctl0_layer_secu_en1 : 4;  /* bit[0-3] : 保留 */
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
        unsigned int  intr_pay_abnormal_s : 1;  /* bit[0-0] : 场景0：安全支付外发异常电平，ACPU收到后可写0清
                                                              场景1&2:保留
                                                              场景3：hw_dvfs功能使能时的帧配置变化配置信号
                                                              场景4：上报给media区的dacc idle状态信号 1：idle 0:busy */
        unsigned int  reserved            : 31; /* bit[1-31]:  */
    } reg;
} SOC_DACC_SEC_PAY_ABNORMAL_INT_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_0_INFO_UNION
 结构说明  : DBG_0_INFO 寄存器结构定义。地址偏移量:0x052c+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_0_info : 32; /* bit[0-31]: [31]:para_extr_rdy
                                                     [30:27]:req_layer_id
                                                     [25]:req_sdma_ov_flag
                                                     [24]:layer_para_req
                                                     [23]:instr_ov_need
                                                     [22]:instr_ov_rdy
                                                     [21]:instr_dma_need
                                                     [20]:instr_dma_rdy
                                                     [19]:dacc_info_out_rdy
                                                     [18]:dacc_info_out_need
                                                     [17]:dma_info_rdy
                                                     [16]:dma_info_need
                                                     [15]:固接0
                                                     [14:12]:dma_send_cst
                                                     [11]:固接0
                                                     [10:8]:dma_calc_cst
                                                     [7:4]:ov_send_cst
                                                     [3]:固接0
                                                     [2:0]:ov_calc_cst */
    } reg;
} SOC_DACC_DBG_0_INFO_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_1_INFO_UNION
 结构说明  : DBG_1_INFO 寄存器结构定义。地址偏移量:0x0530+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_1_info : 32; /* bit[0-31]: [31:16]:info_out_cnt
                                                     [15:13]:固接0
                                                     [12:0]:info_ack_cnt */
    } reg;
} SOC_DACC_DBG_1_INFO_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_2_INFO_UNION
 结构说明  : DBG_2_INFO 寄存器结构定义。地址偏移量:0x0534+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_2_info : 32; /* bit[0-31]: [31:27]:固接0
                                                     [26]:dma_info_fifo_nempty
                                                     [25]:dma_instr_gen_over_1d
                                                     [24]:ov_instr_gen_over_1d
                                                     [23:21]:固接0
                                                     [20:16]:layer_id
                                                     [15:13]:固接0
                                                     [12:0]:ycnt */
    } reg;
} SOC_DACC_DBG_2_INFO_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_3_INFO_UNION
 结构说明  : DBG_3_INFO 寄存器结构定义。地址偏移量:0x0538+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_3_info : 32; /* bit[0-31]:
                                                     dma_info_fifo_rdata
                                                     [13:0]: line_id
                                                     [18:14]:layer_id
                                                     [22:19]: scene_id
                                                     [31:23]:固接0 */
    } reg;
} SOC_DACC_DBG_3_INFO_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_4_INFO_UNION
 结构说明  : DBG_4_INFO 寄存器结构定义。地址偏移量:0x053c+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_4_info : 32; /* bit[0-31]: reserved */
    } reg;
} SOC_DACC_DBG_4_INFO_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DBG_5_INFO_UNION
 结构说明  : DBG_5_INFO 寄存器结构定义。地址偏移量:0x0540+(s12)*0x400，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dbg_5_info : 32; /* bit[0-31]: 场景0：
                                                     [31]: TUI校验错误标识 1：有校验错误 0：校验无错
                                                     [30:0]: reserved
                                                     场景1~4：reserved */
    } reg;
} SOC_DACC_DBG_5_INFO_UNION;
#endif


/****************************************************************************
                     (2/5) Pre_Pipe_Ctrl
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_OV_WR_LINEREG_S0_UNION
 结构说明  : OV_WR_LINEREG_S0 寄存器结构定义。地址偏移量:0x0000，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_w_linereg_cnt_s0  : 3;  /* bit[0-2]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_cnt */
        unsigned int  ov_w_linereg_flag_s0 : 1;  /* bit[3-3]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_flag */
        unsigned int  ov_wtotal_s0         : 6;  /* bit[4-9]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_wtotal */
        unsigned int  reserved             : 22; /* bit[10-31]: 保留 本寄存器DCMD同时使用。 */
    } reg;
} SOC_DACC_OV_WR_LINEREG_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA0_S0_UNION
 结构说明  : LAYER_LB_PARA0_S0 寄存器结构定义。地址偏移量:0x0004+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_r_linereg_cnt_s0     : 3;  /* bit[0-2]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_cnt */
        unsigned int  ov_r_linereg_flag_s0    : 1;  /* bit[3-3]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_flag */
        unsigned int  ov_rtotal_s0            : 6;  /* bit[4-9]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_rtotal */
        unsigned int  layer_dfc_alpha0_l_s0   : 6;  /* bit[10-15]: layer_dfc_alpha0[5:0] */
        unsigned int  dma_w_linereg_cnt_y_s0  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_y（Y平面） */
        unsigned int  dma_w_linereg_flag_y_s0 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_y（Y平面） */
        unsigned int  dma_wtotal_y_s0         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_y（Y平面） */
        unsigned int  layer_dfc_alpha0_h_s0   : 4;  /* bit[26-29]: layer_dfc_alpha0[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA0_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA1_S0_UNION
 结构说明  : LAYER_LB_PARA1_S0 寄存器结构定义。地址偏移量:0x0008+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_w_linereg_cnt_v_s0  : 3;  /* bit[0-2]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_v（V平面） */
        unsigned int  dma_w_linereg_flag_v_s0 : 1;  /* bit[3-3]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_v（V平面） */
        unsigned int  dma_wtotal_v_s0         : 6;  /* bit[4-9]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_v（V平面） */
        unsigned int  layer_dfc_alpha1_l_s0   : 6;  /* bit[10-15]: layer_dfc_alpha1[5:0] */
        unsigned int  dma_w_linereg_cnt_u_s0  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_u（U平面） */
        unsigned int  dma_w_linereg_flag_u_s0 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_u（U平面） */
        unsigned int  dma_wtotal_u_s0         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_u（U平面） */
        unsigned int  layer_dfc_alpha1_h_s0   : 4;  /* bit[26-29]: layer_dfc_alpha1[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA1_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA2_S0_UNION
 结构说明  : LAYER_LB_PARA2_S0 寄存器结构定义。地址偏移量:0x000c+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_block_high_s0  : 13; /* bit[0-12] : DMA搬运块高度,减1配置 */
        unsigned int  reserved_0         : 3;  /* bit[13-15]: 保留 */
        unsigned int  dma_block_width_s0 : 13; /* bit[16-28]: DMA搬运块宽度,减1配置 */
        unsigned int  reserved_1         : 3;  /* bit[29-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA2_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_IN_S0_UNION
 结构说明  : LAYER_INFO_IN_S0 寄存器结构定义。地址偏移量:0x0010+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_input_img_height_s0 : 14; /* bit[0-13] : 前处理通路输入原始图像高度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  reserved                : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_input_img_width_s0  : 13; /* bit[16-28]: 前处理通路输入原始图像宽度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  layer_dfc_swap_s0       : 3;  /* bit[29-31]: [31]:layer_dfc_rb_swap；R和B交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [30]:layer_dfc_uv_swap；U和V交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [29]:layer_dfc_ax_swap；AX 高低位切换标志：0表不交换，1表交换 */
    } reg;
} SOC_DACC_LAYER_INFO_IN_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_OUT_S0_UNION
 结构说明  : LAYER_INFO_OUT_S0 寄存器结构定义。地址偏移量:0x0014+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_output_img_height_s0 : 14; /* bit[0-13] : 前处理通路输出图像高度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_0               : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_output_img_width_s0  : 13; /* bit[16-28]: 前处理通路输出图像宽度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_1               : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_INFO_OUT_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_CLIP_INFO_S0_UNION
 结构说明  : LAYER_CLIP_INFO_S0 寄存器结构定义。地址偏移量:0x0018+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bottom_clip_value_s0 : 6;  /* bit[0-5]  : 底部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行底端clip */
        unsigned int  reserved_0           : 2;  /* bit[6-7]  : 保留 */
        unsigned int  top_clip_value_s0    : 6;  /* bit[8-13] : 顶部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行顶部clip；无减1配置 */
        unsigned int  reserved_1           : 2;  /* bit[14-15]: 保留 */
        unsigned int  right_clip_value_s0  : 6;  /* bit[16-21]: 右侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行右侧clip；无减1配置 */
        unsigned int  reserved_2           : 2;  /* bit[22-23]: 保留 */
        unsigned int  left_clip_value_s0   : 6;  /* bit[24-29]: 左侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行左侧clip；无减1配置 */
        unsigned int  reserved_3           : 2;  /* bit[30-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_CLIP_INFO_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_INFO_S0_UNION
 结构说明  : LAYER_LB_INFO_S0 寄存器结构定义。地址偏移量:0x001c+(pre_ch0)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_format_s0      : 6;  /* bit[0-5]  : 前处理算子通路/bypass通路/LUT图层/idle送显通路输入格式 */
        unsigned int  output_format_s0     : 6;  /* bit[6-11] : 前处理算子通路/bypass通路输出格式 */
        unsigned int  first_mod_idx_s0     : 6;  /* bit[12-17]: 图层前处理首个算子的模块索引 */
        unsigned int  linereg_start_idx_s0 : 11; /* bit[18-28]: linereg起始索引 */
        unsigned int  reserved             : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_LB_INFO_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_OV_WR_LINEREG_S1_UNION
 结构说明  : OV_WR_LINEREG_S1 寄存器结构定义。地址偏移量:0x0300，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_w_linereg_cnt_s1  : 3;  /* bit[0-2]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_cnt */
        unsigned int  ov_w_linereg_flag_s1 : 1;  /* bit[3-3]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_flag */
        unsigned int  ov_wtotal_s1         : 6;  /* bit[4-9]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_wtotal */
        unsigned int  reserved             : 22; /* bit[10-31]: 保留 本寄存器DCMD同时使用。 */
    } reg;
} SOC_DACC_OV_WR_LINEREG_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA0_S1_UNION
 结构说明  : LAYER_LB_PARA0_S1 寄存器结构定义。地址偏移量:0x0304+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_r_linereg_cnt_s1     : 3;  /* bit[0-2]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_cnt */
        unsigned int  ov_r_linereg_flag_s1    : 1;  /* bit[3-3]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_flag */
        unsigned int  ov_rtotal_s1            : 6;  /* bit[4-9]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_rtotal */
        unsigned int  layer_dfc_alpha0_l_s1   : 6;  /* bit[10-15]: layer_dfc_alpha0[5:0] */
        unsigned int  dma_w_linereg_cnt_y_s1  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_y（Y平面） */
        unsigned int  dma_w_linereg_flag_y_s1 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_y（Y平面） */
        unsigned int  dma_wtotal_y_s1         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_y（Y平面） */
        unsigned int  layer_dfc_alpha0_h_s1   : 4;  /* bit[26-29]: layer_dfc_alpha0[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA0_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA1_S1_UNION
 结构说明  : LAYER_LB_PARA1_S1 寄存器结构定义。地址偏移量:0x0308+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_w_linereg_cnt_v_s1  : 3;  /* bit[0-2]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_v（V平面） */
        unsigned int  dma_w_linereg_flag_v_s1 : 1;  /* bit[3-3]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_v（V平面） */
        unsigned int  dma_wtotal_v_s1         : 6;  /* bit[4-9]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_v（V平面） */
        unsigned int  layer_dfc_alpha1_l_s1   : 6;  /* bit[10-15]: layer_dfc_alpha1[5:0] */
        unsigned int  dma_w_linereg_cnt_u_s1  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_u（U平面） */
        unsigned int  dma_w_linereg_flag_u_s1 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_u（U平面） */
        unsigned int  dma_wtotal_u_s1         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_u（U平面） */
        unsigned int  layer_dfc_alpha1_h_s1   : 4;  /* bit[26-29]: layer_dfc_alpha1[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA1_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA2_S1_UNION
 结构说明  : LAYER_LB_PARA2_S1 寄存器结构定义。地址偏移量:0x030c+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_block_high_s1  : 13; /* bit[0-12] : DMA搬运块高度,减1配置 */
        unsigned int  reserved_0         : 3;  /* bit[13-15]: 保留 */
        unsigned int  dma_block_width_s1 : 13; /* bit[16-28]: DMA搬运块宽度,减1配置 */
        unsigned int  reserved_1         : 3;  /* bit[29-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA2_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_IN_S1_UNION
 结构说明  : LAYER_INFO_IN_S1 寄存器结构定义。地址偏移量:0x0310+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_input_img_height_s1 : 14; /* bit[0-13] : 原始图像高度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  reserved                : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_input_img_width_s1  : 13; /* bit[16-28]: 原始图像宽度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  layer_dfc_swap_s1       : 3;  /* bit[29-31]: [31]:layer_dfc_rb_swap；R和B交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [30]:layer_dfc_uv_swap；U和V交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [29]:layer_dfc_ax_swap；AX 高低位切换标志：0表不交换，1表交换 */
    } reg;
} SOC_DACC_LAYER_INFO_IN_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_OUT_S1_UNION
 结构说明  : LAYER_INFO_OUT_S1 寄存器结构定义。地址偏移量:0x0314+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_output_img_height_s1 : 14; /* bit[0-13] : 图像高度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_0               : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_output_img_width_s1  : 13; /* bit[16-28]: 图像宽度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_1               : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_INFO_OUT_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_CLIP_INFO_S1_UNION
 结构说明  : LAYER_CLIP_INFO_S1 寄存器结构定义。地址偏移量:0x0318+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bottom_clip_value_s1 : 6;  /* bit[0-5]  : 底部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行底端clip；无减1配置 */
        unsigned int  reserved_0           : 2;  /* bit[6-7]  : 保留 */
        unsigned int  top_clip_value_s1    : 6;  /* bit[8-13] : 顶部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行顶部clip；无减1配置 */
        unsigned int  reserved_1           : 2;  /* bit[14-15]: 保留 */
        unsigned int  right_clip_value_s1  : 6;  /* bit[16-21]: 右侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行右侧clip；无减1配置 */
        unsigned int  reserved_2           : 2;  /* bit[22-23]: 保留 */
        unsigned int  left_clip_value_s1   : 6;  /* bit[24-29]: 左侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行左侧clip；无减1配置 */
        unsigned int  reserved_3           : 2;  /* bit[30-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_CLIP_INFO_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_INFO_S1_UNION
 结构说明  : LAYER_LB_INFO_S1 寄存器结构定义。地址偏移量:0x031c+(pre_ch1)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_format_s1      : 6;  /* bit[0-5]  : 前处理算子通路/bypass通路/LUT图层/idle送显通路输入格式 */
        unsigned int  output_format_s1     : 6;  /* bit[6-11] : 前处理算子通路/bypass通路输出格式 */
        unsigned int  first_mod_idx_s1     : 6;  /* bit[12-17]: 图层前处理首个算子的模块索引 */
        unsigned int  linereg_start_idx_s1 : 11; /* bit[18-28]: linereg起始索引 */
        unsigned int  reserved             : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_LB_INFO_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_OV_WR_LINEREG_S2_UNION
 结构说明  : OV_WR_LINEREG_S2 寄存器结构定义。地址偏移量:0x0500，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_w_linereg_cnt_s2  : 3;  /* bit[0-2]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_cnt */
        unsigned int  ov_w_linereg_flag_s2 : 1;  /* bit[3-3]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_flag */
        unsigned int  ov_wtotal_s2         : 6;  /* bit[4-9]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_wtotal */
        unsigned int  reserved             : 22; /* bit[10-31]: 保留 本寄存器DCMD同时使用。 */
    } reg;
} SOC_DACC_OV_WR_LINEREG_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA0_S2_UNION
 结构说明  : LAYER_LB_PARA0_S2 寄存器结构定义。地址偏移量:0x0504+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_r_linereg_cnt_s2     : 3;  /* bit[0-2]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_cnt */
        unsigned int  ov_r_linereg_flag_s2    : 1;  /* bit[3-3]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_flag */
        unsigned int  ov_rtotal_s2            : 6;  /* bit[4-9]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_rtotal */
        unsigned int  layer_dfc_alpha0_l_s2   : 6;  /* bit[10-15]: layer_dfc_alpha0[5:0] */
        unsigned int  dma_w_linereg_cnt_y_s2  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_y（Y平面） */
        unsigned int  dma_w_linereg_flag_y_s2 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_y（Y平面） */
        unsigned int  dma_wtotal_y_s2         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_y（Y平面） */
        unsigned int  layer_dfc_alpha0_h_s2   : 4;  /* bit[26-29]: layer_dfc_alpha0[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA0_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA1_S2_UNION
 结构说明  : LAYER_LB_PARA1_S2 寄存器结构定义。地址偏移量:0x0508+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_w_linereg_cnt_v_s2  : 3;  /* bit[0-2]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_v（V平面） */
        unsigned int  dma_w_linereg_flag_v_s2 : 1;  /* bit[3-3]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_v（V平面） */
        unsigned int  dma_wtotal_v_s2         : 6;  /* bit[4-9]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_v（V平面） */
        unsigned int  layer_dfc_alpha1_l_s2   : 6;  /* bit[10-15]: layer_dfc_alpha1[5:0] */
        unsigned int  dma_w_linereg_cnt_u_s2  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_u（U平面） */
        unsigned int  dma_w_linereg_flag_u_s2 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_u（U平面） */
        unsigned int  dma_wtotal_u_s2         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_u（U平面） */
        unsigned int  layer_dfc_alpha1_h_s2   : 4;  /* bit[26-29]: layer_dfc_alpha1[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA1_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA2_S2_UNION
 结构说明  : LAYER_LB_PARA2_S2 寄存器结构定义。地址偏移量:0x050c+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_block_high_s2  : 13; /* bit[0-12] : DMA搬运块高度,减1配置 */
        unsigned int  reserved_0         : 3;  /* bit[13-15]: 保留 */
        unsigned int  dma_block_width_s2 : 13; /* bit[16-28]: DMA搬运块宽度,减1配置 */
        unsigned int  reserved_1         : 3;  /* bit[29-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA2_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_IN_S2_UNION
 结构说明  : LAYER_INFO_IN_S2 寄存器结构定义。地址偏移量:0x0510+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_input_img_height_s2 : 14; /* bit[0-13] : 原始图像高度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  reserved                : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_input_img_width_s2  : 13; /* bit[16-28]: 原始图像宽度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  layer_dfc_swap_s2       : 3;  /* bit[29-31]: [31]:layer_dfc_rb_swap；R和B交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [30]:layer_dfc_uv_swap；U和V交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [29]:layer_dfc_ax_swap；AX 高低位切换标志：0表不交换，1表交换 */
    } reg;
} SOC_DACC_LAYER_INFO_IN_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_OUT_S2_UNION
 结构说明  : LAYER_INFO_OUT_S2 寄存器结构定义。地址偏移量:0x0514+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_output_img_height_s2 : 14; /* bit[0-13] : 图像高度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_0               : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_output_img_width_s2  : 13; /* bit[16-28]: 图像宽度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_1               : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_INFO_OUT_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_CLIP_INFO_REG_S2_UNION
 结构说明  : LAYER_CLIP_INFO_REG_S2 寄存器结构定义。地址偏移量:0x0518+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bottom_clip_value_s2 : 6;  /* bit[0-5]  : 底部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行底端clip；无减1配置 */
        unsigned int  reserved_0           : 2;  /* bit[6-7]  : 保留 */
        unsigned int  top_clip_value_s2    : 6;  /* bit[8-13] : 顶部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行顶部clip；无减1配置 */
        unsigned int  reserved_1           : 2;  /* bit[14-15]: 保留 */
        unsigned int  right_clip_value_s2  : 6;  /* bit[16-21]: 右侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行右侧clip；无减1配置 */
        unsigned int  reserved_2           : 2;  /* bit[22-23]: 保留 */
        unsigned int  left_clip_value_s2   : 6;  /* bit[24-29]: 左侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行左侧clip；无减1配置 */
        unsigned int  reserved_3           : 2;  /* bit[30-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_CLIP_INFO_REG_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_INFO_REG_S2_UNION
 结构说明  : LAYER_LB_INFO_REG_S2 寄存器结构定义。地址偏移量:0x051c+(pre_ch2)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_format_s2      : 6;  /* bit[0-5]  : 前处理算子通路/bypass通路/LUT图层/idle送显通路输入格式 */
        unsigned int  output_format_s2     : 6;  /* bit[6-11] : 前处理算子通路/bypass通路输出格式 */
        unsigned int  first_mod_idx_s2     : 6;  /* bit[12-17]: 图层前处理首个算子的模块索引 */
        unsigned int  linereg_start_idx_s2 : 11; /* bit[18-28]: linereg起始索引 */
        unsigned int  reserved             : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_LB_INFO_REG_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_OV_WR_LINEREG_S4_UNION
 结构说明  : OV_WR_LINEREG_S4 寄存器结构定义。地址偏移量:0x0700，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_w_linereg_cnt_s4  : 3;  /* bit[0-2]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_cnt */
        unsigned int  ov_w_linereg_flag_s4 : 1;  /* bit[3-3]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_w_linereg_flag */
        unsigned int  ov_wtotal_s4         : 6;  /* bit[4-9]  : OV写LB参数，后处理算子通路读LB也使用该参数
                                                                ov_wtotal */
        unsigned int  reserved             : 22; /* bit[10-31]: 保留 本寄存器DCMD同时使用。 */
    } reg;
} SOC_DACC_OV_WR_LINEREG_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA0_S4_UNION
 结构说明  : LAYER_LB_PARA0_S4 寄存器结构定义。地址偏移量:0x0704+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ov_r_linereg_cnt_s4     : 3;  /* bit[0-2]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_cnt */
        unsigned int  ov_r_linereg_flag_s4    : 1;  /* bit[3-3]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_r_linereg_flag */
        unsigned int  ov_rtotal_s4            : 6;  /* bit[4-9]  : OV读LB参数，前处理算子通路/bypass通路 写LB也使用该参数
                                                                   ov_rtotal */
        unsigned int  layer_dfc_alpha0_l_s4   : 6;  /* bit[10-15]: layer_dfc_alpha0[5:0] */
        unsigned int  dma_w_linereg_cnt_y_s4  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_y（Y平面） */
        unsigned int  dma_w_linereg_flag_y_s4 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_y（Y平面） */
        unsigned int  dma_wtotal_y_s4         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_y（Y平面） */
        unsigned int  layer_dfc_alpha0_h_s4   : 4;  /* bit[26-29]: layer_dfc_alpha0[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA0_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA1_S4_UNION
 结构说明  : LAYER_LB_PARA1_S4 寄存器结构定义。地址偏移量:0x0708+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_w_linereg_cnt_v_s4  : 3;  /* bit[0-2]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_v（V平面） */
        unsigned int  dma_w_linereg_flag_v_s4 : 1;  /* bit[3-3]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_v（V平面） */
        unsigned int  dma_wtotal_v_s4         : 6;  /* bit[4-9]  : SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_v（V平面） */
        unsigned int  layer_dfc_alpha1_l_s4   : 6;  /* bit[10-15]: layer_dfc_alpha1[5:0] */
        unsigned int  dma_w_linereg_cnt_u_s4  : 3;  /* bit[16-18]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_cnt_u（U平面） */
        unsigned int  dma_w_linereg_flag_u_s4 : 1;  /* bit[19-19]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_linereg_flag_u（U平面） */
        unsigned int  dma_wtotal_u_s4         : 6;  /* bit[20-25]: SDMA写LB参数，前处理算子通路/bypass通路/LUT图层/idle送显通路读LB也使用该参数
                                                                   dma_wtotal_u（U平面） */
        unsigned int  layer_dfc_alpha1_h_s4   : 4;  /* bit[26-29]: layer_dfc_alpha1[9:6] */
        unsigned int  reserved                : 2;  /* bit[30-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA1_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_PARA2_S4_UNION
 结构说明  : LAYER_LB_PARA2_S4 寄存器结构定义。地址偏移量:0x070c+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dma_block_high_s4  : 13; /* bit[0-12] : DMA搬运块高度,减1配置 */
        unsigned int  reserved_0         : 3;  /* bit[13-15]: 保留 */
        unsigned int  dma_block_width_s4 : 13; /* bit[16-28]: DMA搬运块宽度,减1配置 */
        unsigned int  reserved_1         : 3;  /* bit[29-31]: 保留（本寄存器DCMD同时使用） */
    } reg;
} SOC_DACC_LAYER_LB_PARA2_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_IN_S4_UNION
 结构说明  : LAYER_INFO_IN_S4 寄存器结构定义。地址偏移量:0x0710+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_input_img_height_s4 : 14; /* bit[0-13] : 原始图像高度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  reserved                : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_input_img_width_s4  : 13; /* bit[16-28]: 原始图像宽度（旋转/镜像之后，clip之前）,减1配置 */
        unsigned int  layer_dfc_swap_s4       : 3;  /* bit[29-31]: [31]:layer_dfc_rb_swap；R和B交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [30]:layer_dfc_uv_swap；U和V交换标志(用于10bit格式)： 0表不交换，1表交换
                                                                   [29]:layer_dfc_ax_swap；AX 高低位切换标志：0表不交换，1表交换 */
    } reg;
} SOC_DACC_LAYER_INFO_IN_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_INFO_OUT_S4_UNION
 结构说明  : LAYER_INFO_OUT_S4 寄存器结构定义。地址偏移量:0x0714+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pre_output_img_height_s4 : 14; /* bit[0-13] : 图像高度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_0               : 2;  /* bit[14-15]: 保留 */
        unsigned int  pre_output_img_width_s4  : 13; /* bit[16-28]: 图像宽度（旋转/镜像/clip之后）,减1配置 */
        unsigned int  reserved_1               : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_INFO_OUT_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_CLIP_INFO_REG_S4_UNION
 结构说明  : LAYER_CLIP_INFO_REG_S4 寄存器结构定义。地址偏移量:0x0718+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bottom_clip_value_s4 : 6;  /* bit[0-5]  : 底部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行底端clip；无减1配置 */
        unsigned int  reserved_0           : 2;  /* bit[6-7]  : 保留 */
        unsigned int  top_clip_value_s4    : 6;  /* bit[8-13] : 顶部clip值（单位：行）；如果本图层有旋转/镜像，则是旋转/镜像后进行顶部clip；无减1配置 */
        unsigned int  reserved_1           : 2;  /* bit[14-15]: 保留 */
        unsigned int  right_clip_value_s4  : 6;  /* bit[16-21]: 右侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行右侧clip；无减1配置 */
        unsigned int  reserved_2           : 2;  /* bit[22-23]: 保留 */
        unsigned int  left_clip_value_s4   : 6;  /* bit[24-29]: 左侧clip值（单位：pixel）；如果本图层有旋转/镜像，则是旋转/镜像后进行左侧clip；无减1配置 */
        unsigned int  reserved_3           : 2;  /* bit[30-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_CLIP_INFO_REG_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_LB_INFO_REG_S4_UNION
 结构说明  : LAYER_LB_INFO_REG_S4 寄存器结构定义。地址偏移量:0x071c+(pre_ch4)*0x1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_format_s4      : 6;  /* bit[0-5]  : 前处理算子通路/bypass通路/LUT图层/idle送显通路输入格式 */
        unsigned int  output_format_s4     : 6;  /* bit[6-11] : 前处理算子通路/bypass通路输出格式 */
        unsigned int  first_mod_idx_s4     : 6;  /* bit[12-17]: 图层前处理首个算子的模块索引 */
        unsigned int  linereg_start_idx_s4 : 11; /* bit[18-28]: linereg起始索引 */
        unsigned int  reserved             : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_LAYER_LB_INFO_REG_S4_UNION;
#endif


/****************************************************************************
                     (3/5) Post_Pipe_Ctrl
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_SCENE_0_WB_INFO_REG_UNION
 结构说明  : SCENE_0_WB_INFO_REG 寄存器结构定义。地址偏移量:0x0e60，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  idle_display_en : 1;  /* bit[0-0] : 保留 */
        unsigned int  od_wb_en        : 1;  /* bit[1-1] : 保留 */
        unsigned int  ov_wb_en        : 1;  /* bit[2-2] : 保留 */
        unsigned int  reserved        : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_DACC_SCENE_0_WB_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCENE_RD_INFO_REG_UNION
 结构说明  : SCENE_RD_INFO_REG 寄存器结构定义。地址偏移量:0x0e70+(post_ch)*0x10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  post_input_img_height : 14; /* bit[0-13] : 后处理通路输入图像高度（减1处理）,减1配置 */
        unsigned int  reserved_0            : 2;  /* bit[14-15]: 保留 */
        unsigned int  post_input_img_width  : 13; /* bit[16-28]: 后处理通路输入图像宽度（减1处理）,减1配置 */
        unsigned int  reserved_1            : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_SCENE_RD_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCENE_WR_INFO_REG_UNION
 结构说明  : SCENE_WR_INFO_REG 寄存器结构定义。地址偏移量:0x0e74+(post_ch)*0x10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  post_output_img_height : 14; /* bit[0-13] : 后处理通路输出图像高度（减1处理）,减1配置 */
        unsigned int  reserved_0             : 2;  /* bit[14-15]: 保留 */
        unsigned int  post_output_img_width  : 13; /* bit[16-28]: 后处理通路输出图像宽度（减1处理）,减1配置 */
        unsigned int  reserved_1             : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_SCENE_WR_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCENE_LB_R_INFO_REG_UNION
 结构说明  : SCENE_LB_R_INFO_REG 寄存器结构定义。地址偏移量:0x0e78+(post_ch)*0x10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_r_cnt        : 3;  /* bit[0-2]  : linereg_r_cnt */
        unsigned int  linereg_r_flag       : 1;  /* bit[3-3]  : linereg_r_flag */
        unsigned int  linereg_rtotal       : 6;  /* bit[4-9]  : linereg_rtotal */
        unsigned int  input_format         : 6;  /* bit[10-15]: 输入格式 */
        unsigned int  linereg_rd_start_idx : 11; /* bit[16-26]: 读前级LB的linereg起始索引 */
        unsigned int  ov_wb_en_flag        : 1;  /* bit[27-27]: 保留 */
        unsigned int  reserved             : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_SCENE_LB_R_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_SCENE_LB_W_INFO_REG_UNION
 结构说明  : SCENE_LB_W_INFO_REG 寄存器结构定义。地址偏移量:0x0e7c+(post_ch)*0x10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_w_cnt        : 3;  /* bit[0-2]  : linereg_w_cnt */
        unsigned int  linereg_w_flag       : 1;  /* bit[3-3]  : linereg_w_flag */
        unsigned int  linereg_wtotal       : 6;  /* bit[4-9]  : linereg_wtotal */
        unsigned int  output_format        : 6;  /* bit[10-15]: 输出格式 */
        unsigned int  linereg_wr_start_idx : 11; /* bit[16-26]: 写后级LB的linereg起始索引 */
        unsigned int  ddic_dsc_wb_en_flag  : 1;  /* bit[27-27]: 保留 */
        unsigned int  reserved             : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_DACC_SCENE_LB_W_INFO_REG_UNION;
#endif


/****************************************************************************
                     (4/5) ITF_WCH_Ctrl
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_ITF0_INFO_REG_UNION
 结构说明  : ITF0_INFO_REG 寄存器结构定义。地址偏移量:0x0e00，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_img_height_s0 : 14; /* bit[0-13] : 图像高度（减1处理） */
        unsigned int  reserved_0          : 2;  /* bit[14-15]: 保留 */
        unsigned int  input_img_width_s0  : 13; /* bit[16-28]: 图像宽度（减1处理） */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_ITF0_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ITF0_LB_INFO_REG_UNION
 结构说明  : ITF0_LB_INFO_REG 寄存器结构定义。地址偏移量:0x0e04，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_r_cnt_s0     : 3;  /* bit[0-2]  : linereg_r_cnt */
        unsigned int  linereg_r_flag_s0    : 1;  /* bit[3-3]  : linereg_r_flag */
        unsigned int  linereg_rtotal_s0    : 6;  /* bit[4-9]  : linereg_rtotal */
        unsigned int  input_format_s0      : 6;  /* bit[10-15]: 输入格式 */
        unsigned int  linereg_start_idx_s0 : 11; /* bit[16-26]: linereg起始索引 */
        unsigned int  reserved             : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_DACC_ITF0_LB_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ITF1_INFO_REG_UNION
 结构说明  : ITF1_INFO_REG 寄存器结构定义。地址偏移量:0x0e08，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_img_height_s1 : 14; /* bit[0-13] : 图像高度（减1处理） */
        unsigned int  reserved_0          : 2;  /* bit[14-15]: 保留 */
        unsigned int  input_img_width_s1  : 13; /* bit[16-28]: 图像宽度（减1处理） */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_ITF1_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ITF1_LB_INFO_REG_UNION
 结构说明  : ITF1_LB_INFO_REG 寄存器结构定义。地址偏移量:0x0e0c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_r_cnt_s1     : 3;  /* bit[0-2]  : linereg_r_cnt */
        unsigned int  linereg_r_flag_s1    : 1;  /* bit[3-3]  : linereg_r_flag */
        unsigned int  linereg_rtotal_s1    : 6;  /* bit[4-9]  : linereg_rtotal */
        unsigned int  input_format_s1      : 6;  /* bit[10-15]: 输入格式 */
        unsigned int  linereg_start_idx_s1 : 11; /* bit[16-26]: linereg起始索引 */
        unsigned int  reserved             : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_DACC_ITF1_LB_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ITF2_INFO_REG_UNION
 结构说明  : ITF2_INFO_REG 寄存器结构定义。地址偏移量:0x0e10，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_img_height_s2 : 14; /* bit[0-13] : 图像高度（减1处理） */
        unsigned int  reserved_0          : 2;  /* bit[14-15]: 保留 */
        unsigned int  input_img_width_s2  : 13; /* bit[16-28]: 图像宽度（减1处理） */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_ITF2_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ITF2_LB_INFO_REG_UNION
 结构说明  : ITF2_LB_INFO_REG 寄存器结构定义。地址偏移量:0x0e14，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_r_cnt_s2     : 3;  /* bit[0-2]  : linereg_r_cnt */
        unsigned int  linereg_r_flag_s2    : 1;  /* bit[3-3]  : linereg_r_flag */
        unsigned int  linereg_rtotal_s2    : 6;  /* bit[4-9]  : linereg_rtotal */
        unsigned int  input_format_s2      : 6;  /* bit[10-15]: 输入格式 */
        unsigned int  linereg_start_idx_s2 : 11; /* bit[16-26]: linereg起始索引 */
        unsigned int  reserved             : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_DACC_ITF2_LB_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WCH0_INFO_REG_UNION
 结构说明  : WCH0_INFO_REG 寄存器结构定义。地址偏移量:0x0e18，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_img_height_w0 : 14; /* bit[0-13] : 保留 */
        unsigned int  reserved_0          : 2;  /* bit[14-15]: 保留 */
        unsigned int  input_img_width_w0  : 13; /* bit[16-28]: 保留 */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_WCH0_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WCH0_LB_INFO_REG_UNION
 结构说明  : WCH0_LB_INFO_REG 寄存器结构定义。地址偏移量:0x0e1c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_r_cnt_w0     : 3;  /* bit[0-2]  : 保留 */
        unsigned int  linereg_r_flag_w0    : 1;  /* bit[3-3]  : 保留 */
        unsigned int  linereg_rtotal_w0    : 6;  /* bit[4-9]  : 保留 */
        unsigned int  input_format_w0      : 6;  /* bit[10-15]: 保留 */
        unsigned int  linereg_start_idx_w0 : 11; /* bit[16-26]: 保留 */
        unsigned int  wr_1_rd_2_flag_w0    : 1;  /* bit[27-27]: 保留 */
        unsigned int  wr_back_src_sel_w0   : 1;  /* bit[28-28]: 保留 */
        unsigned int  reserved             : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_WCH0_LB_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WCH1_INFO_REG_UNION
 结构说明  : WCH1_INFO_REG 寄存器结构定义。地址偏移量:0x0e20，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  input_img_height_w1 : 14; /* bit[0-13] : 图像高度（减1处理） */
        unsigned int  reserved_0          : 2;  /* bit[14-15]: 保留 */
        unsigned int  input_img_width_w1  : 13; /* bit[16-28]: 图像宽度（减1处理） */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_DACC_WCH1_INFO_REG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_WCH1_LB_INFO_REG_UNION
 结构说明  : WCH1_LB_INFO_REG 寄存器结构定义。地址偏移量:0x0e24，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  linereg_r_cnt_w1     : 3;  /* bit[0-2]  : linereg_r_cnt */
        unsigned int  linereg_r_flag_w1    : 1;  /* bit[3-3]  : linereg_r_flag */
        unsigned int  linereg_rtotal_w1    : 6;  /* bit[4-9]  : linereg_rtotal */
        unsigned int  input_format_w1      : 6;  /* bit[10-15]: 输入格式 */
        unsigned int  linereg_start_idx_w1 : 11; /* bit[16-26]: linereg起始索引 */
        unsigned int  reserved             : 5;  /* bit[27-31]: 保留 */
    } reg;
} SOC_DACC_WCH1_LB_INFO_REG_UNION;
#endif


/****************************************************************************
                     (5/5) CrossBar
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DACC_ARSR1_CFG_UNION
 结构说明  : ARSR1_CFG 寄存器结构定义。地址偏移量:0x1000，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  arsr1_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  arsr1_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  arsr1_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id */
        unsigned int  arsr1_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否 */
        unsigned int  arsr1_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号 */
        unsigned int  arsr1_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否 */
        unsigned int  arsr1_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号 */
        unsigned int  reserved          : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_ARSR1_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_VSCF0_CFG_UNION
 结构说明  : VSCF0_CFG 寄存器结构定义。地址偏移量:0x1004，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vscf0_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  vscf0_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  vscf0_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id */
        unsigned int  vscf0_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否 */
        unsigned int  vscf0_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号 */
        unsigned int  vscf0_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否 */
        unsigned int  vscf0_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号 */
        unsigned int  reserved          : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_VSCF0_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_VSCF1_CFG_UNION
 结构说明  : VSCF1_CFG 寄存器结构定义。地址偏移量:0x1008，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vscf1_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  vscf1_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  vscf1_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id */
        unsigned int  vscf1_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否 */
        unsigned int  vscf1_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号 */
        unsigned int  vscf1_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否 */
        unsigned int  vscf1_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号 */
        unsigned int  reserved          : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_VSCF1_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP0_CFG_UNION
 结构说明  : UVUP0_CFG 寄存器结构定义。地址偏移量:0x100C，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uvup0_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  uvup0_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  uvup0_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id */
        unsigned int  uvup0_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否 */
        unsigned int  uvup0_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号 */
        unsigned int  uvup0_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否 */
        unsigned int  uvup0_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号 */
        unsigned int  reserved          : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_UVUP0_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_HDR_CFG_UNION
 结构说明  : HDR_CFG 寄存器结构定义。地址偏移量:0x1010，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  hdr_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  hdr_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  hdr_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id */
        unsigned int  hdr_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否 */
        unsigned int  hdr_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号 */
        unsigned int  hdr_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否 */
        unsigned int  hdr_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号 */
        unsigned int  reserved        : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_HDR_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_CLD_CFG_UNION
 结构说明  : CLD_CFG 寄存器结构定义。地址偏移量:0x1014，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cld_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  cld_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  cld_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id */
        unsigned int  cld_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否 */
        unsigned int  cld_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号 */
        unsigned int  cld_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否 */
        unsigned int  cld_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号 */
        unsigned int  reserved        : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_CLD_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_ARSR0_CFG_UNION
 结构说明  : ARSR0_CFG 寄存器结构定义。地址偏移量:0x1018，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  arsr0_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  arsr0_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  arsr0_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id（保留） */
        unsigned int  arsr0_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否（保留） */
        unsigned int  arsr0_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号（保留） */
        unsigned int  arsr0_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否（保留） */
        unsigned int  arsr0_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号（保留） */
        unsigned int  reserved          : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_ARSR0_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_UVUP1_CFG_UNION
 结构说明  : UVUP1_CFG 寄存器结构定义。地址偏移量:0x101C，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  uvup1_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能（保留） */
        unsigned int  uvup1_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id（保留） */
        unsigned int  uvup1_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id（保留） */
        unsigned int  uvup1_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否（保留） */
        unsigned int  uvup1_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号（保留） */
        unsigned int  uvup1_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否（保留） */
        unsigned int  uvup1_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号（保留） */
        unsigned int  reserved          : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_UVUP1_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DPP0_CFG_UNION
 结构说明  : DPP0_CFG 寄存器结构定义。地址偏移量:0x1020，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dpp0_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  dpp0_scene_id    : 4;  /* bit[1-4]  : [4]：DPP0 colorbar测试模式使能 1：使能 0：不使能
                                                            [3:1]:使用此算子的场景id（DPP0只有场景0使用） */
        unsigned int  dpp0_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id（保留） */
        unsigned int  dpp0_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否（保留） */
        unsigned int  dpp0_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号（保留） */
        unsigned int  dpp0_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否（保留） */
        unsigned int  dpp0_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号（保留） */
        unsigned int  reserved         : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_DPP0_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DPP1_CFG_UNION
 结构说明  : DPP1_CFG 寄存器结构定义。地址偏移量:0x1024，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dpp1_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  dpp1_scene_id    : 4;  /* bit[1-4]  : [4]：DPP1 colorbar测试模式使能 1：使能 0：不使能
                                                            [3:1]:使用此算子的场景id（DPP1只有场景1使用） */
        unsigned int  dpp1_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id（保留） */
        unsigned int  dpp1_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否（保留） */
        unsigned int  dpp1_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号（保留） */
        unsigned int  dpp1_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否（保留） */
        unsigned int  dpp1_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号（保留） */
        unsigned int  reserved         : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_DPP1_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DDIC_CFG_UNION
 结构说明  : DDIC_CFG 寄存器结构定义。地址偏移量:0x1028，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ddic_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  ddic_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  ddic_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id（保留） */
        unsigned int  ddic_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否（保留） */
        unsigned int  ddic_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号（保留） */
        unsigned int  ddic_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否（保留） */
        unsigned int  ddic_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号（保留） */
        unsigned int  reserved         : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_DDIC_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DSC_CFG_UNION
 结构说明  : DSC_CFG 寄存器结构定义。地址偏移量:0x102C，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dsc_enable      : 1;  /* bit[0]    : 算子使能，1使能；0不使能 */
        unsigned int  dsc_scene_id    : 4;  /* bit[1-4]  : 使用此算子的场景id */
        unsigned int  dsc_layer_id    : 5;  /* bit[5-9]  : 使用此算子的图层id（保留） */
        unsigned int  dsc_start_order : 1;  /* bit[10]   : 是否是链路的首算子，1是；0否（保留） */
        unsigned int  dsc_next_order  : 4;  /* bit[11-14]: 算子链路的后级算子编号（保留） */
        unsigned int  dsc_end_order   : 1;  /* bit[15]   : 是否是链路的尾算子，1是；0否（保留） */
        unsigned int  dsc_pre_order   : 4;  /* bit[16-19]: 算子链路的前级算子编号（保留） */
        unsigned int  reserved        : 12; /* bit[20-31]: reserve */
    } reg;
} SOC_DACC_DSC_CFG_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_OV_Y_S0_UNION
 结构说明  : LAYER_OV_Y_S0 寄存器结构定义。地址偏移量:0x1040+(pre_ch0)*0x4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_ov_start_y_s0 : 13; /* bit[0-12] : 场景0的Pixel图层在屏幕（OV处）的矩形区域首行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  layer_ov_end_y_s0   : 13; /* bit[16-28]: 场景0的Pixel图层在屏幕（OV处）的矩形区域尾行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_LAYER_OV_Y_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DIM_FLAG_REG_S0_UNION
 结构说明  : DIM_FLAG_REG_S0 寄存器结构定义。地址偏移量:0x1080，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_dim_flag_s0  : 16; /* bit[0-15] : 场景0的dim图层标识
                                                              1：该图层为DIM图层0：该图层不是DIM图层 */
        unsigned int  pixel_layer_num_s0 : 5;  /* bit[16-20]: 场景0的pixel图层数量 */
        unsigned int  reserved           : 10; /* bit[21-30]: reserve */
        unsigned int  ov_pattern_en_s0   : 1;  /* bit[31-31]: 场景0的ov pattern测试模式使能1：使能0：不使能 */
    } reg;
} SOC_DACC_DIM_FLAG_REG_S0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_OV_Y_S1_UNION
 结构说明  : LAYER_OV_Y_S1 寄存器结构定义。地址偏移量:0x1090+(pre_ch1)*0x4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_ov_start_y_s1 : 13; /* bit[0-12] : 场景1的Pixel图层在屏幕（OV处）的矩形区域首行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  layer_ov_end_y_s1   : 13; /* bit[16-28]: 场景1的Pixel图层在屏幕（OV处）的矩形区域尾行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_LAYER_OV_Y_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DIM_FLAG_REG_S1_UNION
 结构说明  : DIM_FLAG_REG_S1 寄存器结构定义。地址偏移量:0x10b0，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_dim_flag_s1  : 16; /* bit[0-15] : 场景1的dim图层标识
                                                              1：该图层为DIM图层0：该图层不是DIM图层 */
        unsigned int  pixel_layer_num_s1 : 5;  /* bit[16-20]: 场景1的pixel图层数量 */
        unsigned int  reserved           : 10; /* bit[21-30]: reserve */
        unsigned int  ov_pattern_en_s1   : 1;  /* bit[31-31]: 场景1的ov pattern测试模式使能1：使能0：不使能 */
    } reg;
} SOC_DACC_DIM_FLAG_REG_S1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_OV_Y_S2_UNION
 结构说明  : LAYER_OV_Y_S2 寄存器结构定义。地址偏移量:0x10c0+(pre_ch2)*0x4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_ov_start_y_s2 : 13; /* bit[0-12] : 场景2的Pixel图层在屏幕（OV处）的矩形区域首行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  layer_ov_end_y_s2   : 13; /* bit[16-28]: 场景2的Pixel图层在屏幕（OV处）的矩形区域尾行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_LAYER_OV_Y_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DIM_FLAG_REG_S2_UNION
 结构说明  : DIM_FLAG_REG_S2 寄存器结构定义。地址偏移量:0x10e0，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_dim_flag_s2  : 16; /* bit[0-15] : 场景2的dim图层标识
                                                              1：该图层为DIM图层0：该图层不是DIM图层 */
        unsigned int  pixel_layer_num_s2 : 5;  /* bit[16-20]: 场景2的pixel图层数量 */
        unsigned int  reserved           : 10; /* bit[21-30]: reserve */
        unsigned int  ov_pattern_en_s2   : 1;  /* bit[31-31]: 场景2的ov pattern测试模式使能1：使能0：不使能 */
    } reg;
} SOC_DACC_DIM_FLAG_REG_S2_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_LAYER_OV_Y_S4_UNION
 结构说明  : LAYER_OV_Y_S4 寄存器结构定义。地址偏移量:0x1120+(pre_ch4)*0x4，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_ov_start_y_s4 : 13; /* bit[0-12] : 场景4的Pixel图层在屏幕（OV处）的矩形区域首行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  layer_ov_end_y_s4   : 13; /* bit[16-28]: 场景4的Pixel图层在屏幕（OV处）的矩形区域尾行Y坐标；lut图层该值不感知。 */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_LAYER_OV_Y_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_DIM_FLAG_REG_S4_UNION
 结构说明  : DIM_FLAG_REG_S4 寄存器结构定义。地址偏移量:0x1140，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  layer_dim_flag_s4  : 16; /* bit[0-15] : 场景4的dim图层标识
                                                              1：该图层为DIM图层0：该图层不是DIM图层 */
        unsigned int  pixel_layer_num_s4 : 5;  /* bit[16-20]: 场景4的pixel图层数量 */
        unsigned int  reserved           : 10; /* bit[21-30]: reserve */
        unsigned int  ov_pattern_en_s4   : 1;  /* bit[31-31]: 场景4的ov pattern测试模式使能1：使能0：不使能 */
    } reg;
} SOC_DACC_DIM_FLAG_REG_S4_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_INFO_RDY_NEED_DBG_ST0_UNION
 结构说明  : INFO_RDY_NEED_DBG_ST0 寄存器结构定义。地址偏移量:0x1144，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  info_rdy_need_dbg_st0 : 32; /* bit[0-31]: dgp和上下游模块info接口的rdy/need状态
                                                                [31:16]: 保留
                                                                [15:14]: DGP输出给ov_pipe_ctrl出口的状态机状态；
                                                                [13:12]: DGP输出给DACC出口的状态机状态；
                                                                [11:10]: DGP输出给pre_pipe_ctrl写侧状态机状态；
                                                                [9:8]: 输出给ov_pipe_ctrl的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [7:6]: 输出给pre_pipe_ctrl读侧的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [5:4]: 输出给pre_pipe_ctrl写侧的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [3:2]: DGP接收后处理info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [1:0]: DGP输出给DACC的rdy/need接口状态高bit对应rdy，低bit对应need； */
    } reg;
} SOC_DACC_INFO_RDY_NEED_DBG_ST0_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_INFO_RDY_NEED_DBG_ST1_UNION
 结构说明  : INFO_RDY_NEED_DBG_ST1 寄存器结构定义。地址偏移量:0x1148，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  info_rdy_need_dbg_st1 : 32; /* bit[0-31]: dgp和前处理算子info接口的rdy/need状态
                                                                [31:24]: 保留
                                                                [23:22]: UVUP输出给DGP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [21:20]: DGP输出给UVUP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [19:18]: CLD输出给DGP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [17:16]: DGP输出给CLD的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [15:14]: HDR输出给DGP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [13:12]: DGP输出给HDR的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [11:10]: ARSR1输出给DGP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [9:8]: DGP输出给ARSR1的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [7:6]: VSCF1输出给DGP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [5:4]: DGP输出给VSCF1的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [3:2]: VSCF0输出给DGP的info的rdy/need接口状态高bit对应rdy，低bit对应need；
                                                                [1:0]: DGP输出给VSCF0的info的rdy/need接口状态高bit对应rdy，低bit对应need； */
    } reg;
} SOC_DACC_INFO_RDY_NEED_DBG_ST1_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_INFO_LINE_ID_DBG_S01_UNION
 结构说明  : INFO_LINE_ID_DBG_S01 寄存器结构定义。地址偏移量:0x114c，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  info_line_id_dbg_s0 : 13; /* bit[0-12] : 场景0已接收的info行号 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  info_line_id_dbg_s1 : 13; /* bit[16-28]: 场景1已接收的info行号 */
        unsigned int  reserved_1          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_INFO_LINE_ID_DBG_S01_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_INFO_LINE_ID_DBG_S23_UNION
 结构说明  : INFO_LINE_ID_DBG_S23 寄存器结构定义。地址偏移量:0x1150，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  info_line_id_dbg_s2 : 13; /* bit[0-12] : 场景2已接收的info行号 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  reserved_1          : 13; /* bit[16-28]: reserve */
        unsigned int  reserved_2          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_INFO_LINE_ID_DBG_S23_UNION;
#endif


/*****************************************************************************
 结构名    : SOC_DACC_INFO_LINE_ID_DBG_S45_UNION
 结构说明  : INFO_LINE_ID_DBG_S45 寄存器结构定义。地址偏移量:0x1154，初值:0x00000000，宽度:32
 寄存器说明:
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  info_line_id_dbg_s4 : 13; /* bit[0-12] : 场景4已接收的info行号 */
        unsigned int  reserved_0          : 3;  /* bit[13-15]: reserve */
        unsigned int  reserved_1          : 13; /* bit[16-28]: reserve */
        unsigned int  reserved_2          : 3;  /* bit[29-31]: reserve */
    } reg;
} SOC_DACC_INFO_LINE_ID_DBG_S45_UNION;
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
