

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_DMSS_INTERFACE_H__
#define __SOC_DMSS_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) register_DMSS
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：RTL配置信息0
   位域定义UNION结构:  SOC_DMSS_ASI_RTL_INF0_UNION */
#define SOC_DMSS_ASI_RTL_INF0_ADDR(base, asi_base)            ((base) + (0x000+0x800*(asi_base)))

/* 寄存器说明：RTL配置信息1
   位域定义UNION结构:  SOC_DMSS_ASI_RTL_INF1_UNION */
#define SOC_DMSS_ASI_RTL_INF1_ADDR(base, asi_base)            ((base) + (0x004+0x800*(asi_base)))

/* 寄存器说明：RTL配置信息2
   位域定义UNION结构:  SOC_DMSS_ASI_RTL_INF2_UNION */
#define SOC_DMSS_ASI_RTL_INF2_ADDR(base, asi_base)            ((base) + (0x008+0x800*(asi_base)))

/* 寄存器说明：静态时钟门控
   位域定义UNION结构:  SOC_DMSS_ASI_STA_CKG_UNION */
#define SOC_DMSS_ASI_STA_CKG_ADDR(base, asi_base)             ((base) + (0x010+0x800*(asi_base)))

/* 寄存器说明：动态时钟门控
   位域定义UNION结构:  SOC_DMSS_ASI_DYN_CKG_UNION */
#define SOC_DMSS_ASI_DYN_CKG_ADDR(base, asi_base)             ((base) + (0x014+0x800*(asi_base)))

/* 寄存器说明：异步桥时钟门控
   位域定义UNION结构:  SOC_DMSS_ASI_ASYNC_BRG_UNION */
#define SOC_DMSS_ASI_ASYNC_BRG_ADDR(base, asi_base)           ((base) + (0x018+0x800*(asi_base)))

/* 寄存器说明：地址移位
   位域定义UNION结构:  SOC_DMSS_ASI_ADDR_SHIFT_UNION */
#define SOC_DMSS_ASI_ADDR_SHIFT_ADDR(base, asi_base)          ((base) + (0x020+0x800*(asi_base)))

/* 寄存器说明：写数据速率匹配
   位域定义UNION结构:  SOC_DMSS_ASI_RATE_ADPT_UNION */
#define SOC_DMSS_ASI_RATE_ADPT_ADDR(base, asi_base)           ((base) + (0x030+0x800*(asi_base)))

/* 寄存器说明：防死锁控制
   位域定义UNION结构:  SOC_DMSS_ASI_ANTI_DEADLOCK_UNION */
#define SOC_DMSS_ASI_ANTI_DEADLOCK_ADDR(base, asi_base)       ((base) + (0x038+0x800*(asi_base)))

/* 寄存器说明：读保序控制
   位域定义UNION结构:  SOC_DMSS_ASI_RDR_CTRL_UNION */
#define SOC_DMSS_ASI_RDR_CTRL_ADDR(base, rdr_filters, asi_base)  ((base) + (0x040+0x4*(rdr_filters)+0x800*(asi_base)))

/* 寄存器说明：命令优先级控制
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_CTRL_UNION */
#define SOC_DMSS_ASI_QOS_CTRL_ADDR(base, asi_base)            ((base) + (0x100+0x800*(asi_base)))

/* 寄存器说明：写命令优先级映射表
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_WRPRI_UNION */
#define SOC_DMSS_ASI_QOS_WRPRI_ADDR(base, asi_base)           ((base) + (0x104+0x800*(asi_base)))

/* 寄存器说明：读命令优先级映射表
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RDPRI_UNION */
#define SOC_DMSS_ASI_QOS_RDPRI_ADDR(base, asi_base)           ((base) + (0x108+0x800*(asi_base)))

/* 寄存器说明：命令优先级自适应周期映射表
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_ADPT_UNION */
#define SOC_DMSS_ASI_QOS_ADPT_ADDR(base, asi_base)            ((base) + (0x10C+0x800*(asi_base)))

/* 寄存器说明：读命令的latency控制
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LATENCY_UNION */
#define SOC_DMSS_ASI_QOS_LATENCY_ADDR(base, asi_base)         ((base) + (0x110+0x800*(asi_base)))

/* 寄存器说明：带宽优先级控制0
            （可单独控制读命令优先级，或同时控制读写命令优先级）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR0_UNION */
#define SOC_DMSS_ASI_QOS_RGLR0_ADDR(base, asi_base)           ((base) + (0x120+0x800*(asi_base)))

/* 寄存器说明：带宽配置优先级0
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR0_PRI_UNION */
#define SOC_DMSS_ASI_QOS_RGLR0_PRI_ADDR(base, asi_base)       ((base) + (0x124+0x800*(asi_base)))

/* 寄存器说明：带宽优先级控制1
            （只可单独控制写命令优先级）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR1_UNION */
#define SOC_DMSS_ASI_QOS_RGLR1_ADDR(base, asi_base)           ((base) + (0x128+0x800*(asi_base)))

/* 寄存器说明：带宽配置优先级1
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR1_PRI_UNION */
#define SOC_DMSS_ASI_QOS_RGLR1_PRI_ADDR(base, asi_base)       ((base) + (0x12C+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制0
            （可单独控制读带宽，或同时控制读写总带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR0_UNION */
#define SOC_DMSS_ASI_QOS_LMTR0_ADDR(base, asi_base)           ((base) + (0x140+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制1
            （只可单独控制写带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR1_UNION */
#define SOC_DMSS_ASI_QOS_LMTR1_ADDR(base, asi_base)           ((base) + (0x144+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制2
            （只可同时控制读写总带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR2_UNION */
#define SOC_DMSS_ASI_QOS_LMTR2_ADDR(base, asi_base)           ((base) + (0x148+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制3
            （只可单独控制写带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR3_UNION */
#define SOC_DMSS_ASI_QOS_LMTR3_ADDR(base, asi_base)           ((base) + (0x14C+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制4
            （只可控制SC方向读写总流量）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR4_UNION */
#define SOC_DMSS_ASI_QOS_LMTR4_ADDR(base, asi_base)           ((base) + (0x150+0x800*(asi_base)))

/* 寄存器说明：读命令OSTD限制
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RDOSTD_UNION */
#define SOC_DMSS_ASI_QOS_RDOSTD_ADDR(base, asi_base)          ((base) + (0x160+0x800*(asi_base)))

/* 寄存器说明：写命令同MID命令优先级传递使能
   位域定义UNION结构:  SOC_DMSS_ASI_PUSH_WRMID_UNION */
#define SOC_DMSS_ASI_PUSH_WRMID_ADDR(base, asi_base)          ((base) + (0x180+0x800*(asi_base)))

/* 寄存器说明：读命令同MID命令优先级传递使能
   位域定义UNION结构:  SOC_DMSS_ASI_PUSH_RDMID_UNION */
#define SOC_DMSS_ASI_PUSH_RDMID_ADDR(base, asi_base)          ((base) + (0x184+0x800*(asi_base)))

/* 寄存器说明：统计功能屏蔽
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_MASK_UNION */
#define SOC_DMSS_ASI_STAT_MASK_ADDR(base, asi_base)           ((base) + (0x200+0x800*(asi_base)))

/* 寄存器说明：流量统计过滤配置0
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_FILTER0_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_FILTER0_ADDR(base, asi_base)   ((base) + (0x210+0x800*(asi_base)))

/* 寄存器说明：流量统计过滤配置1
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_FILTER1_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_FILTER1_ADDR(base, asi_base)   ((base) + (0x214+0x800*(asi_base)))

/* 寄存器说明：写流量统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_WR_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_WR_ADDR(base, asi_base)        ((base) + (0x218+0x800*(asi_base)))

/* 寄存器说明：读流量统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_RD_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_RD_ADDR(base, asi_base)        ((base) + (0x21C+0x800*(asi_base)))

/* 寄存器说明：带宽控制器0统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR0_UNION */
#define SOC_DMSS_ASI_STAT_LMTR0_ADDR(base, asi_base)          ((base) + (0x220+0x800*(asi_base)))

/* 寄存器说明：带宽控制器1统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR1_UNION */
#define SOC_DMSS_ASI_STAT_LMTR1_ADDR(base, asi_base)          ((base) + (0x224+0x800*(asi_base)))

/* 寄存器说明：带宽控制器2统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR2_UNION */
#define SOC_DMSS_ASI_STAT_LMTR2_ADDR(base, asi_base)          ((base) + (0x228+0x800*(asi_base)))

/* 寄存器说明：带宽控制器3统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR3_UNION */
#define SOC_DMSS_ASI_STAT_LMTR3_ADDR(base, asi_base)          ((base) + (0x22C+0x800*(asi_base)))

/* 寄存器说明：读latency统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_RDLAT_UNION */
#define SOC_DMSS_ASI_STAT_RDLAT_ADDR(base, asi_base)          ((base) + (0x230+0x800*(asi_base)))

/* 寄存器说明：读命令个数
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_RDNUM_UNION */
#define SOC_DMSS_ASI_STAT_RDNUM_ADDR(base, asi_base)          ((base) + (0x234+0x800*(asi_base)))

/* 寄存器说明：写命令阻塞统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_BLOCK_WR_UNION */
#define SOC_DMSS_ASI_STAT_BLOCK_WR_ADDR(base, asi_base)       ((base) + (0x238+0x800*(asi_base)))

/* 寄存器说明：读命令阻塞统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_BLOCK_RD_UNION */
#define SOC_DMSS_ASI_STAT_BLOCK_RD_ADDR(base, asi_base)       ((base) + (0x23C+0x800*(asi_base)))

/* 寄存器说明：命令优先级0和1统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI01_UNION */
#define SOC_DMSS_ASI_STAT_PRI01_ADDR(base, asi_base)          ((base) + (0x240+0x800*(asi_base)))

/* 寄存器说明：命令优先级2和3统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI23_UNION */
#define SOC_DMSS_ASI_STAT_PRI23_ADDR(base, asi_base)          ((base) + (0x244+0x800*(asi_base)))

/* 寄存器说明：命令优先级4和5统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI45_UNION */
#define SOC_DMSS_ASI_STAT_PRI45_ADDR(base, asi_base)          ((base) + (0x248+0x800*(asi_base)))

/* 寄存器说明：命令优先级6和7统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI67_UNION */
#define SOC_DMSS_ASI_STAT_PRI67_ADDR(base, asi_base)          ((base) + (0x24C+0x800*(asi_base)))

/* 寄存器说明：带宽控制器4统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR4_UNION */
#define SOC_DMSS_ASI_STAT_LMTR4_ADDR(base, asi_base)          ((base) + (0x250+0x800*(asi_base)))

/* 寄存器说明：Pattern detect功能控制0
   位域定义UNION结构:  SOC_DMSS_ASI_DETECT_CFG0_UNION */
#define SOC_DMSS_ASI_DETECT_CFG0_ADDR(base, asi_base)         ((base) + (0x280+0x800*(asi_base)))

/* 寄存器说明：Pattern detect功能控制1
   位域定义UNION结构:  SOC_DMSS_ASI_DETECT_CFG1_UNION */
#define SOC_DMSS_ASI_DETECT_CFG1_ADDR(base, asi_base)         ((base) + (0x284+0x800*(asi_base)))

/* 寄存器说明：Pattern detect信息记录0
   位域定义UNION结构:  SOC_DMSS_ASI_DETECT_INF0_UNION */
#define SOC_DMSS_ASI_DETECT_INF0_ADDR(base, asi_base)         ((base) + (0x288+0x800*(asi_base)))

/* 寄存器说明：Wakeup快速唤醒功能控制
   位域定义UNION结构:  SOC_DMSS_ASI_WAKEUP_CFG_UNION */
#define SOC_DMSS_ASI_WAKEUP_CFG_ADDR(base, asi_base)          ((base) + (0x290+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域0
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN0_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN0_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x300+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域1
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN1_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN1_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x304+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域2
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN2_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN2_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x308+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域3
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN3_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN3_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x30C+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_LD_SEL_UNION */
#define SOC_DMSS_ASI_SEC_LD_SEL_ADDR(base, asi_base)          ((base) + (0x410+0x800*(asi_base)))

/* 寄存器说明：安全中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_INT_EN_UNION */
#define SOC_DMSS_ASI_SEC_INT_EN_ADDR(base, asi_base)          ((base) + (0x420+0x800*(asi_base)))

/* 寄存器说明：安全中断状态
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_INT_STATUS_UNION */
#define SOC_DMSS_ASI_SEC_INT_STATUS_ADDR(base, asi_base)      ((base) + (0x424+0x800*(asi_base)))

/* 寄存器说明：记录首个越权命令的信息0
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_ADDR(base, asi_base)   ((base) + (0x480+0x800*(asi_base)))

/* 寄存器说明：记录首个越权命令的信息1
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_ADDR(base, asi_base)   ((base) + (0x484+0x800*(asi_base)))

/* 寄存器说明：记录首个越权命令的信息2
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_ADDR(base, asi_base)   ((base) + (0x488+0x800*(asi_base)))

/* 寄存器说明：Protected越权访问AMI中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_INT_EN_UNION */
#define SOC_DMSS_ASI_AMI_PROT_INT_EN_ADDR(base, asi_base)     ((base) + (0x4A0+0x800*(asi_base)))

/* 寄存器说明：Protected越权访问AMI中断状态
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_INT_STATUS_UNION */
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_ADDR(base, asi_base)  ((base) + (0x4A4+0x800*(asi_base)))

/* 寄存器说明：记录首个Protected越权访问AMI拦截命令的信息0
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_CMD_INF0_UNION */
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF0_ADDR(base, asi_base)   ((base) + (0x4A8+0x800*(asi_base)))

/* 寄存器说明：记录首个Protected越权访问AMI拦截命令的信息1
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_CMD_INF1_UNION */
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ADDR(base, asi_base)   ((base) + (0x4AC+0x800*(asi_base)))

/* 寄存器说明：Enhance Trusted越权访问AMI中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_INT_EN_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_INT_EN_ADDR(base, asi_base)     ((base) + (0x4B0+0x800*(asi_base)))

/* 寄存器说明：Enhance Trusted越权访问AMI中断状态
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_ADDR(base, asi_base)  ((base) + (0x4B4+0x800*(asi_base)))

/* 寄存器说明：记录首个Enhance Trusted访问AMI命令的信息0
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_ADDR(base, asi_base)   ((base) + (0x4B8+0x800*(asi_base)))

/* 寄存器说明：记录首个Enhance Trusted访问AMI命令命令的信息1
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ADDR(base, asi_base)   ((base) + (0x4BC+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问non-trust地址空间配置0
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_ADDR(base, asi_base)  ((base) + (0x4C0+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问non-trust地址空间配置1
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_ADDR(base, asi_base)  ((base) + (0x4C4+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问protected地址空间配置0
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_ADDR(base, asi_base)  ((base) + (0x4C8+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问protected地址空间配置1
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_ADDR(base, asi_base)  ((base) + (0x4CC+0x800*(asi_base)))

/* 寄存器说明：MPU protected写命令访问non-trust地址空间配置0
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_UNION */
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_ADDR(base, asi_base)  ((base) + (0x4D0+0x800*(asi_base)))

/* 寄存器说明：MPU protected写命令访问non-trust地址空间配置1
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_UNION */
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_ADDR(base, asi_base)  ((base) + (0x4D4+0x800*(asi_base)))

/* 寄存器说明：地址区域映射0
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_RGN_MAP0_UNION */
#define SOC_DMSS_ASI_SEC_RGN_MAP0_ADDR(base, asi_rgns, asi_base)  ((base) + (0x500+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：地址区域映射1
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_RGN_MAP1_UNION */
#define SOC_DMSS_ASI_SEC_RGN_MAP1_ADDR(base, asi_rgns, asi_base)  ((base) + (0x504+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_MID_WR_UNION */
#define SOC_DMSS_ASI_SEC_MID_WR_ADDR(base, asi_rgns, asi_base)  ((base) + (0x508+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_MID_RD_UNION */
#define SOC_DMSS_ASI_SEC_MID_RD_ADDR(base, asi_rgns, asi_base)  ((base) + (0x50C+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：DFX：主要模块工作状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_MODULE_UNION */
#define SOC_DMSS_ASI_DFX_MODULE_ADDR(base, asi_base)          ((base) + (0x700+0x800*(asi_base)))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_FIFO_UNION */
#define SOC_DMSS_ASI_DFX_FIFO_ADDR(base, asi_base)            ((base) + (0x704+0x800*(asi_base)))

/* 寄存器说明：DFX：状态机工作状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_FSM_UNION */
#define SOC_DMSS_ASI_DFX_FSM_ADDR(base, asi_base)             ((base) + (0x708+0x800*(asi_base)))

/* 寄存器说明：DFX：OSTD状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_OSTD_UNION */
#define SOC_DMSS_ASI_DFX_OSTD_ADDR(base, asi_base)            ((base) + (0x70C+0x800*(asi_base)))

/* 寄存器说明：DFX：REORDER模块状态0
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_RDR0_UNION */
#define SOC_DMSS_ASI_DFX_RDR0_ADDR(base, asi_base)            ((base) + (0x710+0x800*(asi_base)))

/* 寄存器说明：DFX：REORDER模块状态1
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_RDR1_UNION */
#define SOC_DMSS_ASI_DFX_RDR1_ADDR(base, asi_base)            ((base) + (0x714+0x800*(asi_base)))

/* 寄存器说明：DFX：WOOO模块状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_WOOO_UNION */
#define SOC_DMSS_ASI_DFX_WOOO_ADDR(base, asi_base)            ((base) + (0x718+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_INT_EN_UNION */
#define SOC_DMSS_ASI_DFX_INT_EN_ADDR(base, asi_base)          ((base) + (0x720+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息中断
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_INT_STATUS_UNION */
#define SOC_DMSS_ASI_DFX_INT_STATUS_ADDR(base, asi_base)      ((base) + (0x724+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息记录0
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_ABNM_INF0_UNION */
#define SOC_DMSS_ASI_DFX_ABNM_INF0_ADDR(base, asi_base)       ((base) + (0x730+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息记录1
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_ABNM_INF1_UNION */
#define SOC_DMSS_ASI_DFX_ABNM_INF1_ADDR(base, asi_base)       ((base) + (0x734+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息记录2
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_ABNM_INF2_UNION */
#define SOC_DMSS_ASI_DFX_ABNM_INF2_ADDR(base, asi_base)       ((base) + (0x738+0x800*(asi_base)))

/* 寄存器说明：DFX：安全中断状态
            （为便于调试，将寄存器ASI_SEC_INT_STATUS复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC0_UNION */
#define SOC_DMSS_ASI_DFX_SEC0_ADDR(base, asi_base)            ((base) + (0x740+0x800*(asi_base)))

/* 寄存器说明：DFX：记录首个越权命令的信息0
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF0复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC1_UNION */
#define SOC_DMSS_ASI_DFX_SEC1_ADDR(base, asi_base)            ((base) + (0x744+0x800*(asi_base)))

/* 寄存器说明：DFX：记录首个越权命令的信息1
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF1复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC2_UNION */
#define SOC_DMSS_ASI_DFX_SEC2_ADDR(base, asi_base)            ((base) + (0x748+0x800*(asi_base)))

/* 寄存器说明：DFX：记录首个越权命令的信息2
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF2复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC3_UNION */
#define SOC_DMSS_ASI_DFX_SEC3_ADDR(base, asi_base)            ((base) + (0x74C+0x800*(asi_base)))

/* 寄存器说明：DFX：仿真流量冲击状态
            （若RTL中配置DMSS_SIM_DFX_EN为0（通常ASIC版本配置为0，FPGA和EMULATOR版本定义为1），则此寄存器不存在）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_LOAD_UNION */
#define SOC_DMSS_ASI_DFX_LOAD_ADDR(base, asi_base)            ((base) + (0x760+0x800*(asi_base)))

/* 寄存器说明：DMI代码配置信息0
   位域定义UNION结构:  SOC_DMSS_DMI_RTL_INF0_UNION */
#define SOC_DMSS_DMI_RTL_INF0_ADDR(base)                      ((base) + (0x10000UL))

/* 寄存器说明：DMI动态时钟门控
   位域定义UNION结构:  SOC_DMSS_DMI_DYN_CKG_UNION */
#define SOC_DMSS_DMI_DYN_CKG_ADDR(base)                       ((base) + (0x10010UL))

/* 寄存器说明：读命令DMC优先级自适应周期
   位域定义UNION结构:  SOC_DMSS_DMI_RD_ADPT_UNION */
#define SOC_DMSS_DMI_RD_ADPT_ADDR(base)                       ((base) + (0x10020UL))

/* 寄存器说明：写命令DMC优先级自适应周期
   位域定义UNION结构:  SOC_DMSS_DMI_WR_ADPT_UNION */
#define SOC_DMSS_DMI_WR_ADPT_ADDR(base)                       ((base) + (0x10024UL))

/* 寄存器说明：读命令DMC TIMEOUT映射表0
   位域定义UNION结构:  SOC_DMSS_DMI_RD_TIMEOUT0_UNION */
#define SOC_DMSS_DMI_RD_TIMEOUT0_ADDR(base)                   ((base) + (0x10030UL))

/* 寄存器说明：读命令DMC TIMEOUT映射表1
   位域定义UNION结构:  SOC_DMSS_DMI_RD_TIMEOUT1_UNION */
#define SOC_DMSS_DMI_RD_TIMEOUT1_ADDR(base)                   ((base) + (0x10034UL))

/* 寄存器说明：写命令DMC TIMEOUT映射表0
   位域定义UNION结构:  SOC_DMSS_DMI_WR_TIMEOUT0_UNION */
#define SOC_DMSS_DMI_WR_TIMEOUT0_ADDR(base)                   ((base) + (0x10038UL))

/* 寄存器说明：写命令DMC TIMEOUT映射表1
   位域定义UNION结构:  SOC_DMSS_DMI_WR_TIMEOUT1_UNION */
#define SOC_DMSS_DMI_WR_TIMEOUT1_ADDR(base)                   ((base) + (0x1003CUL))

/* 寄存器说明：GID地址区域映射0
   位域定义UNION结构:  SOC_DMSS_DMI_GID_RGN_MAP0_UNION */
#define SOC_DMSS_DMI_GID_RGN_MAP0_ADDR(base, gid_rgns)        ((base) + (0x10040+0x8*(gid_rgns)))

/* 寄存器说明：GID地址区域映射1
   位域定义UNION结构:  SOC_DMSS_DMI_GID_RGN_MAP1_UNION */
#define SOC_DMSS_DMI_GID_RGN_MAP1_ADDR(base, gid_rgns)        ((base) + (0x10044+0x8*(gid_rgns)))

/* 寄存器说明：锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_LD_SEL_UNION */
#define SOC_DMSS_DMI_SEC_LD_SEL_ADDR(base)                    ((base) + (0x10080UL))

/* 寄存器说明：DMI安全中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_INT_EN_UNION */
#define SOC_DMSS_DMI_SEC_INT_EN_ADDR(base)                    ((base) + (0x10084UL))

/* 寄存器说明：DMI安全中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_INT_STATUS_UNION */
#define SOC_DMSS_DMI_SEC_INT_STATUS_ADDR(base, dmi_base)      ((base) + (0x10088+0x800*(dmi_base)))

/* 寄存器说明：DMI Enhance安全中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_INT_EN_UNION */
#define SOC_DMSS_DMI_ENHN_INT_EN_ADDR(base)                   ((base) + (0x1008CUL))

/* 寄存器说明：DMI Enhance安全中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_INT_STATUS_UNION */
#define SOC_DMSS_DMI_ENHN_INT_STATUS_ADDR(base, dmi_base)     ((base) + (0x10090+0x800*(dmi_base)))

/* 寄存器说明：记录首个越权命令的信息0（包括DMI安全拦截和DMI Enhance安全拦截）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_ADDR(base, dmi_base)   ((base) + (0x10094+0x800*(dmi_base)))

/* 寄存器说明：记录首个越权命令的信息1（包括DMI安全拦截和DMI Enhance安全拦截）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_ADDR(base, dmi_base)   ((base) + (0x10098+0x800*(dmi_base)))

/* 寄存器说明：记录首个越权命令的信息2（包括DMI安全拦截和DMI Enhance安全拦截）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_ADDR(base, dmi_base)   ((base) + (0x1009C+0x800*(dmi_base)))

/* 寄存器说明：MPU中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_INT_EN_UNION */
#define SOC_DMSS_DMI_MPU_INT_EN_ADDR(base)                    ((base) + (0x100A0UL))

/* 寄存器说明：MPU中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_INT_STATUS_UNION */
#define SOC_DMSS_DMI_MPU_INT_STATUS_ADDR(base, dmi_base)      ((base) + (0x100A4+0x800*(dmi_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息0
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_ADDR(base, dmi_base)   ((base) + (0x100A8+0x800*(dmi_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息1
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_ADDR(base, dmi_base)   ((base) + (0x100AC+0x800*(dmi_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息2
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_ADDR(base, dmi_base)   ((base) + (0x100B0+0x800*(dmi_base)))

/* 寄存器说明：DMI Enhance寄存器配置中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_CFG_INT_EN_UNION */
#define SOC_DMSS_DMI_ENHN_CFG_INT_EN_ADDR(base)               ((base) + (0x100B4UL))

/* 寄存器说明：DMI Enhance寄存器配置中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_UNION */
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_ADDR(base, dmi_base)  ((base) + (0x100B8+0x800*(dmi_base)))

/* 寄存器说明：地址区域映射0
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_RGN_MAP0_UNION */
#define SOC_DMSS_DMI_SEC_RGN_MAP0_ADDR(base, dmi_rgns)        ((base) + (0x10100+0x30*(dmi_rgns)))

/* 寄存器说明：地址区域映射1
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_RGN_MAP1_UNION */
#define SOC_DMSS_DMI_SEC_RGN_MAP1_ADDR(base, dmi_rgns)        ((base) + (0x10104+0x30*(dmi_rgns)))

/* 寄存器说明：地址区域属性
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_RGN_ATTR_UNION */
#define SOC_DMSS_DMI_SEC_RGN_ATTR_ADDR(base, dmi_enhn_rgns)   ((base) + (0x10108+0x30*(dmi_enhn_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR0_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR0_ADDR(base, dmi_mid_rgns)     ((base) + (0x10110+0x30*(dmi_mid_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR1_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR1_ADDR(base, dmi_mid_rgns)     ((base) + (0x10114+0x30*(dmi_mid_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR2_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR2_ADDR(base, dmi_mid_rgns)     ((base) + (0x10118+0x30*(dmi_mid_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR3_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR3_ADDR(base, dmi_mid_rgns)     ((base) + (0x1011C+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD0_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD0_ADDR(base, dmi_mid_rgns)     ((base) + (0x10120+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD1_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD1_ADDR(base, dmi_mid_rgns)     ((base) + (0x10124+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD2_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD2_ADDR(base, dmi_mid_rgns)     ((base) + (0x10128+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD3_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD3_ADDR(base, dmi_mid_rgns)     ((base) + (0x1012C+0x30*(dmi_mid_rgns)))

/* 寄存器说明：DFX：各个主要模块的工作状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_MODULE_UNION */
#define SOC_DMSS_DMI_DFX_MODULE_ADDR(base, dmi_base)          ((base) + (0x10700+0x800*(dmi_base)))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_FIFO_UNION */
#define SOC_DMSS_DMI_DFX_FIFO_ADDR(base, dmi_base)            ((base) + (0x10704+0x800*(dmi_base)))

/* 寄存器说明：DFX：写数据FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_WD_FIFO_UNION */
#define SOC_DMSS_DMI_DFX_WD_FIFO_ADDR(base, dmi_base)         ((base) + (0x10708+0x800*(dmi_base)))

/* 寄存器说明：DFX：DMI的模块状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_ST_UNION */
#define SOC_DMSS_DMI_DFX_ST_ADDR(base, dmi_base)              ((base) + (0x1070C+0x800*(dmi_base)))

/* 寄存器说明：DFX：DCSC的模块状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_DCSC_UNION */
#define SOC_DMSS_DMI_DFX_DCSC_ADDR(base, dmi_base)            ((base) + (0x10710+0x800*(dmi_base)))

/* 寄存器说明：AMI代码配置信息0
   位域定义UNION结构:  SOC_DMSS_AMI_RTL_INF0_UNION */
#define SOC_DMSS_AMI_RTL_INF0_ADDR(base, ami_base)            ((base) + (0x12000+0x800*(ami_base)))

/* 寄存器说明：AMI动态时钟门控
   位域定义UNION结构:  SOC_DMSS_AMI_DYN_CKG_UNION */
#define SOC_DMSS_AMI_DYN_CKG_ADDR(base, ami_base)             ((base) + (0x12010+0x800*(ami_base)))

/* 寄存器说明：AMI GID控制配置
   位域定义UNION结构:  SOC_DMSS_AMI_GID_CTRL_UNION */
#define SOC_DMSS_AMI_GID_CTRL_ADDR(base, ami_base)            ((base) + (0x12020+0x800*(ami_base)))

/* 寄存器说明：锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_LD_SEL_UNION */
#define SOC_DMSS_AMI_SEC_LD_SEL_ADDR(base, ami_base)          ((base) + (0x12080+0x800*(ami_base)))

/* 寄存器说明：安全中断使能
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_INT_EN_UNION */
#define SOC_DMSS_AMI_SEC_INT_EN_ADDR(base, ami_base)          ((base) + (0x12084+0x800*(ami_base)))

/* 寄存器说明：安全中断状态
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_INT_STATUS_UNION */
#define SOC_DMSS_AMI_SEC_INT_STATUS_ADDR(base, ami_base)      ((base) + (0x12088+0x800*(ami_base)))

/* 寄存器说明：记录首个越权命令的信息0
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_ADDR(base, ami_base)   ((base) + (0x12094+0x800*(ami_base)))

/* 寄存器说明：记录首个越权命令的信息1
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_ADDR(base, ami_base)   ((base) + (0x12098+0x800*(ami_base)))

/* 寄存器说明：记录首个越权命令的信息2
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_ADDR(base, ami_base)   ((base) + (0x1209C+0x800*(ami_base)))

/* 寄存器说明：MPU中断使能
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_INT_EN_UNION */
#define SOC_DMSS_AMI_MPU_INT_EN_ADDR(base, ami_base)          ((base) + (0x120A0+0x800*(ami_base)))

/* 寄存器说明：MPU中断状态
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_INT_STATUS_UNION */
#define SOC_DMSS_AMI_MPU_INT_STATUS_ADDR(base, ami_base)      ((base) + (0x120A4+0x800*(ami_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息0
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_ADDR(base, ami_base)   ((base) + (0x120A8+0x800*(ami_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息1
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_ADDR(base, ami_base)   ((base) + (0x120AC+0x800*(ami_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息2
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_ADDR(base, ami_base)   ((base) + (0x120B0+0x800*(ami_base)))

/* 寄存器说明：地址区域映射0
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_RGN_MAP0_UNION */
#define SOC_DMSS_AMI_SEC_RGN_MAP0_ADDR(base, ami_rgns, ami_base)  ((base) + (0x12100+0x30*(ami_rgns)+0x800*(ami_base)))

/* 寄存器说明：地址区域映射1
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_RGN_MAP1_UNION */
#define SOC_DMSS_AMI_SEC_RGN_MAP1_ADDR(base, ami_rgns, ami_base)  ((base) + (0x12104+0x30*(ami_rgns)+0x800*(ami_base)))

/* 寄存器说明：地址区域属性
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_RGN_ATTR_UNION */
#define SOC_DMSS_AMI_SEC_RGN_ATTR_ADDR(base, ami_enhn_rgns, ami_base)  ((base) + (0x12108+0x30*(ami_enhn_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR0_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR0_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12110+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR1_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR1_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12114+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR2_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR2_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12118+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR3_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR3_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x1211C+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD0_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD0_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12120+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD1_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD1_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12124+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD2_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD2_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12128+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD3_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD3_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x1212C+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：DFX：各个主要模块的工作状态
   位域定义UNION结构:  SOC_DMSS_AMI_DFX_MODULE_UNION */
#define SOC_DMSS_AMI_DFX_MODULE_ADDR(base, ami_base)          ((base) + (0x12700+0x800*(ami_base)))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_AMI_DFX_FIFO_UNION */
#define SOC_DMSS_AMI_DFX_FIFO_ADDR(base, ami_base)            ((base) + (0x12704+0x800*(ami_base)))

/* 寄存器说明：DFX：AMI的模块状态
   位域定义UNION结构:  SOC_DMSS_AMI_DFX_ST_UNION */
#define SOC_DMSS_AMI_DFX_ST_ADDR(base, ami_base)              ((base) + (0x12708+0x800*(ami_base)))

/* 寄存器说明：代码版本信息
   位域定义UNION结构:  SOC_DMSS_GLB_RTL_VER_UNION */
#define SOC_DMSS_GLB_RTL_VER_ADDR(base)                       ((base) + (0x16000UL))

/* 寄存器说明：代码配置信息0
   位域定义UNION结构:  SOC_DMSS_GLB_RTL_INF0_UNION */
#define SOC_DMSS_GLB_RTL_INF0_ADDR(base)                      ((base) + (0x16004UL))

/* 寄存器说明：代码配置信息1
   位域定义UNION结构:  SOC_DMSS_GLB_RTL_INF1_UNION */
#define SOC_DMSS_GLB_RTL_INF1_ADDR(base)                      ((base) + (0x16008UL))

/* 寄存器说明：TP-MEMORY配置0
   位域定义UNION结构:  SOC_DMSS_GLB_RAM_TMOD0_UNION */
#define SOC_DMSS_GLB_RAM_TMOD0_ADDR(base)                     ((base) + (0x16010UL))

/* 寄存器说明：TP-MEMORY配置1
   位域定义UNION结构:  SOC_DMSS_GLB_RAM_TMOD1_UNION */
#define SOC_DMSS_GLB_RAM_TMOD1_ADDR(base)                     ((base) + (0x16014UL))

/* 寄存器说明：SP-MEMORY配置
   位域定义UNION结构:  SOC_DMSS_GLB_RAM_SMOD_UNION */
#define SOC_DMSS_GLB_RAM_SMOD_ADDR(base)                      ((base) + (0x16018UL))

/* 寄存器说明：动态时钟门控
   位域定义UNION结构:  SOC_DMSS_GLB_DYN_CKG_UNION */
#define SOC_DMSS_GLB_DYN_CKG_ADDR(base)                       ((base) + (0x16024UL))

/* 寄存器说明：全局中断状态查看
   位域定义UNION结构:  SOC_DMSS_GLB_INT_STATUS0_UNION */
#define SOC_DMSS_GLB_INT_STATUS0_ADDR(base)                   ((base) + (0x16030UL))

/* 寄存器说明：全局中断状态清除
   位域定义UNION结构:  SOC_DMSS_GLB_INT_CLEAR0_UNION */
#define SOC_DMSS_GLB_INT_CLEAR0_ADDR(base)                    ((base) + (0x16034UL))

/* 寄存器说明：全局中断状态查看
   位域定义UNION结构:  SOC_DMSS_GLB_INT_STATUS1_UNION */
#define SOC_DMSS_GLB_INT_STATUS1_ADDR(base)                   ((base) + (0x16038UL))

/* 寄存器说明：全局中断状态清除
   位域定义UNION结构:  SOC_DMSS_GLB_INT_CLEAR1_UNION */
#define SOC_DMSS_GLB_INT_CLEAR1_ADDR(base)                    ((base) + (0x1603CUL))

/* 寄存器说明：exclusive命令配置
   位域定义UNION结构:  SOC_DMSS_GLB_EXCLU_UNION */
#define SOC_DMSS_GLB_EXCLU_ADDR(base)                         ((base) + (0x16040UL))

/* 寄存器说明：全局地址交织模式
   位域定义UNION结构:  SOC_DMSS_GLB_ADDR_INTLV_UNION */
#define SOC_DMSS_GLB_ADDR_INTLV_ADDR(base)                    ((base) + (0x16100UL))

/* 寄存器说明：全局地址加扰模式
   位域定义UNION结构:  SOC_DMSS_GLB_ADDR_SCRMBL_UNION */
#define SOC_DMSS_GLB_ADDR_SCRMBL_ADDR(base)                   ((base) + (0x16110UL))

/* 寄存器说明：RBUF的控制配置
   位域定义UNION结构:  SOC_DMSS_GLB_RBUF_CTRL_UNION */
#define SOC_DMSS_GLB_RBUF_CTRL_ADDR(base)                     ((base) + (0x16148UL))

/* 寄存器说明：RBUF的传输控制配置
   位域定义UNION结构:  SOC_DMSS_GLB_RBUF_TRANS_CTRL_UNION */
#define SOC_DMSS_GLB_RBUF_TRANS_CTRL_ADDR(base)               ((base) + (0x1614CUL))

/* 寄存器说明：RBUF的命令反压配置
   位域定义UNION结构:  SOC_DMSS_GLB_RBUF_BP_UNION */
#define SOC_DMSS_GLB_RBUF_BP_ADDR(base, rbuf_bp_grps)         ((base) + (0x16150+0x4*(rbuf_bp_grps)))

/* 寄存器说明：AMI0返回响应ERROR中断使能
   位域定义UNION结构:  SOC_DMSS_GLB_RSP_ERR_INT_EN_UNION */
#define SOC_DMSS_GLB_RSP_ERR_INT_EN_ADDR(base)                ((base) + (0x16180UL))

/* 寄存器说明：AMI0返回响应ERROR中断状态
   位域定义UNION结构:  SOC_DMSS_GLB_RSP_ERR_INT_STATUS_UNION */
#define SOC_DMSS_GLB_RSP_ERR_INT_STATUS_ADDR(base)            ((base) + (0x16184UL))

/* 寄存器说明：AMI的控制配置
   位域定义UNION结构:  SOC_DMSS_GLB_AMI_CTRL_UNION */
#define SOC_DMSS_GLB_AMI_CTRL_ADDR(base)                      ((base) + (0x161A0UL))

/* 寄存器说明：DDRC的命令队列状态统计控制
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_CTRL_UNION */
#define SOC_DMSS_GLB_DCQ_CTRL_ADDR(base)                      ((base) + (0x16200UL))

/* 寄存器说明：基于DDRC读写总命令队列状态的允许优先级水线0
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_PRILVL0_UNION */
#define SOC_DMSS_GLB_DCQ_PRILVL0_ADDR(base, chans)            ((base) + (0x16220+0x10*(chans)))

/* 寄存器说明：基于DDRC读写总命令队列状态的允许优先级水线1
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_PRILVL1_UNION */
#define SOC_DMSS_GLB_DCQ_PRILVL1_ADDR(base, chans)            ((base) + (0x16224+0x10*(chans)))

/* 寄存器说明：基于DDRC写命令队列状态的允许优先级水线0
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_WR_PRILVL0_UNION */
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_ADDR(base, chans)         ((base) + (0x16228+0x10*(chans)))

/* 寄存器说明：基于DDRC写命令队列状态的允许优先级水线1
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_WR_PRILVL1_UNION */
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_ADDR(base, chans)         ((base) + (0x1622C+0x10*(chans)))

/* 寄存器说明：SystemCache控制
   位域定义UNION结构:  SOC_DMSS_GLB_SC_CTRL_UNION */
#define SOC_DMSS_GLB_SC_CTRL_ADDR(base)                       ((base) + (0x16260UL))

/* 寄存器说明：基于DCQ的SystemCache流控信息
   位域定义UNION结构:  SOC_DMSS_GLB_SC_DCQLVL_UNION */
#define SOC_DMSS_GLB_SC_DCQLVL_ADDR(base)                     ((base) + (0x16264UL))

/* 寄存器说明：外部master的流控功能
   位域定义UNION结构:  SOC_DMSS_GLB_MST_FLUX_UNION */
#define SOC_DMSS_GLB_MST_FLUX_ADDR(base, dcq_msts)            ((base) + (0x16280+0x4*(dcq_msts)))

/* 寄存器说明：TRACE功能控制0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_CTRL0_UNION */
#define SOC_DMSS_GLB_TRACE_CTRL0_ADDR(base)                   ((base) + (0x16300UL))

/* 寄存器说明：TRACE功能控制1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_CTRL1_UNION */
#define SOC_DMSS_GLB_TRACE_CTRL1_ADDR(base)                   ((base) + (0x16304UL))

/* 寄存器说明：TRACE功能控制2
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_CTRL2_UNION */
#define SOC_DMSS_GLB_TRACE_CTRL2_ADDR(base)                   ((base) + (0x16308UL))

/* 寄存器说明：TRACE功能记录配置0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_REC0_UNION */
#define SOC_DMSS_GLB_TRACE_REC0_ADDR(base)                    ((base) + (0x16310UL))

/* 寄存器说明：TRACE功能记录配置1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_REC1_UNION */
#define SOC_DMSS_GLB_TRACE_REC1_ADDR(base)                    ((base) + (0x16314UL))

/* 寄存器说明：TRACE功能监控过滤0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FILTER0_UNION */
#define SOC_DMSS_GLB_TRACE_FILTER0_ADDR(base)                 ((base) + (0x16318UL))

/* 寄存器说明：TRACE功能监控过滤1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FILTER1_UNION */
#define SOC_DMSS_GLB_TRACE_FILTER1_ADDR(base)                 ((base) + (0x1631CUL))

/* 寄存器说明：TRACE功能监控过滤2
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FILTER2_UNION */
#define SOC_DMSS_GLB_TRACE_FILTER2_ADDR(base, mid_grps)       ((base) + (0x16320+0x4*(mid_grps)))

/* 寄存器说明：TRACE功能时钟频率记录
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FREQ_UNION */
#define SOC_DMSS_GLB_TRACE_FREQ_ADDR(base)                    ((base) + (0x16340UL))

/* 寄存器说明：TRACE功能中断控制
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_INT_EN_UNION */
#define SOC_DMSS_GLB_TRACE_INT_EN_ADDR(base)                  ((base) + (0x16350UL))

/* 寄存器说明：TRACE功能锁定控制
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_LOCK_DOWN_UNION */
#define SOC_DMSS_GLB_TRACE_LOCK_DOWN_ADDR(base)               ((base) + (0x16358UL))

/* 寄存器说明：TRACE功能统计信息0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_STAT0_UNION */
#define SOC_DMSS_GLB_TRACE_STAT0_ADDR(base, dmis)             ((base) + (0x16360+0x10*(dmis)))

/* 寄存器说明：TRACE功能统计信息1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_STAT1_UNION */
#define SOC_DMSS_GLB_TRACE_STAT1_ADDR(base, dmis)             ((base) + (0x16364+0x10*(dmis)))

/* 寄存器说明：TRACE功能统计信息2
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_STAT2_UNION */
#define SOC_DMSS_GLB_TRACE_STAT2_ADDR(base, dmis)             ((base) + (0x16368+0x10*(dmis)))

/* 寄存器说明：MPU的全局配置寄存器
   位域定义UNION结构:  SOC_DMSS_GLB_MPU_CFG_UNION */
#define SOC_DMSS_GLB_MPU_CFG_ADDR(base)                       ((base) + (0x16380UL))

/* 寄存器说明：MPU拦截后特征地址配置
   位域定义UNION结构:  SOC_DMSS_GLB_MPU_ERROR_ADDR_UNION */
#define SOC_DMSS_GLB_MPU_ERROR_ADDR_ADDR(base)                ((base) + (0x16384UL))

/* 寄存器说明：MPU的ACPU Stream配置寄存器
   位域定义UNION结构:  SOC_DMSS_GLB_MPU_ACPU_CFG_UNION */
#define SOC_DMSS_GLB_MPU_ACPU_CFG_ADDR(base)                  ((base) + (0x16388UL))

/* 寄存器说明：安全整体功能透传
   位域定义UNION结构:  SOC_DMSS_GLB_SEC_BYPASS_UNION */
#define SOC_DMSS_GLB_SEC_BYPASS_ADDR(base)                    ((base) + (0x1638CUL))

/* 寄存器说明：统计功能控制
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_CTRL_UNION */
#define SOC_DMSS_GLB_STAT_CTRL_ADDR(base)                     ((base) + (0x16400UL))

/* 寄存器说明：统计周期
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_CYCLE_UNION */
#define SOC_DMSS_GLB_STAT_CYCLE_ADDR(base)                    ((base) + (0x16410UL))

/* 寄存器说明：perf_stat模块配置
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_PERFSTAT_UNION */
#define SOC_DMSS_GLB_STAT_PERFSTAT_ADDR(base)                 ((base) + (0x16420UL))

/* 寄存器说明：DCQ队列写命令个数的累积值统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_DCQ_WR_UNION */
#define SOC_DMSS_GLB_STAT_DCQ_WR_ADDR(base, chans)            ((base) + (0x16440+0x4*(chans)))

/* 寄存器说明：DCQ队列所有命令个数的累积值统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_DCQ_UNION */
#define SOC_DMSS_GLB_STAT_DCQ_ADDR(base, chans)               ((base) + (0x16450+0x4*(chans)))

/* 寄存器说明：所有写优先级都允许的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_ADDR(base, chans)      ((base) + (0x16500+0x40*(chans)))

/* 寄存器说明：写允许优先级为1的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_ADDR(base, chans)     ((base) + (0x16504+0x40*(chans)))

/* 寄存器说明：写允许优先级为2的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_ADDR(base, chans)     ((base) + (0x16508+0x40*(chans)))

/* 寄存器说明：写允许优先级为3的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_ADDR(base, chans)     ((base) + (0x1650C+0x40*(chans)))

/* 寄存器说明：写允许优先级为4的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_ADDR(base, chans)     ((base) + (0x16510+0x40*(chans)))

/* 寄存器说明：写允许优先级为5的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_ADDR(base, chans)     ((base) + (0x16514+0x40*(chans)))

/* 寄存器说明：写允许优先级为6的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_ADDR(base, chans)     ((base) + (0x16518+0x40*(chans)))

/* 寄存器说明：写允许优先级为7的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_ADDR(base, chans)     ((base) + (0x1651C+0x40*(chans)))

/* 寄存器说明：所有读优先级都允许的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_ADDR(base, chans)      ((base) + (0x16520+0x40*(chans)))

/* 寄存器说明：读允许优先级为1的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_ADDR(base, chans)     ((base) + (0x16524+0x40*(chans)))

/* 寄存器说明：读允许优先级为2的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_ADDR(base, chans)     ((base) + (0x16528+0x40*(chans)))

/* 寄存器说明：读允许优先级为3的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_ADDR(base, chans)     ((base) + (0x1652C+0x40*(chans)))

/* 寄存器说明：读允许优先级为4的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_ADDR(base, chans)     ((base) + (0x16530+0x40*(chans)))

/* 寄存器说明：读允许优先级为5的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_ADDR(base, chans)     ((base) + (0x16534+0x40*(chans)))

/* 寄存器说明：读允许优先级为6的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_ADDR(base, chans)     ((base) + (0x16538+0x40*(chans)))

/* 寄存器说明：读允许优先级为7的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_ADDR(base, chans)     ((base) + (0x1653C+0x40*(chans)))

/* 寄存器说明：RBUF占用率统计控制
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_CTRL_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_ADDR(base)                ((base) + (0x16600UL))

/* 寄存器说明：RBUF占用率统计结果0
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_OCCUP0_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP0_ADDR(base)              ((base) + (0x16604UL))

/* 寄存器说明：RBUF占用率统计结果1
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_OCCUP1_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP1_ADDR(base)              ((base) + (0x16608UL))

/* 寄存器说明：RBUF占用率统计结果2
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_OCCUP2_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP2_ADDR(base)              ((base) + (0x1660CUL))

/* 寄存器说明：AMI写数据统计结果
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_AMI_WFIFO_UNION */
#define SOC_DMSS_GLB_STAT_AMI_WFIFO_ADDR(base)                ((base) + (0x16640UL))

/* 寄存器说明：DFX：各个主要模块的工作状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_MODULE_UNION */
#define SOC_DMSS_GLB_DFX_MODULE_ADDR(base)                    ((base) + (0x16700UL))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_FIFO_UNION */
#define SOC_DMSS_GLB_DFX_FIFO_ADDR(base)                      ((base) + (0x16704UL))

/* 寄存器说明：DFX：RBUF FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_RBUF_FIFO_UNION */
#define SOC_DMSS_GLB_DFX_RBUF_FIFO_ADDR(base)                 ((base) + (0x16708UL))

/* 寄存器说明：DFX：DCQ的队列实时状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DCQ_ST_UNION */
#define SOC_DMSS_GLB_DFX_DCQ_ST_ADDR(base, chans)             ((base) + (0x16710+0x4*(chans)))

/* 寄存器说明：DFX：RBUF状态0
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_RBUF0_UNION */
#define SOC_DMSS_GLB_DFX_RBUF0_ADDR(base, rbuf_srcs)          ((base) + (0x16720+0x10*(rbuf_srcs)))

/* 寄存器说明：DFX：RBUF状态1
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_RBUF1_UNION */
#define SOC_DMSS_GLB_DFX_RBUF1_ADDR(base, rbuf_srcs)          ((base) + (0x16724+0x10*(rbuf_srcs)))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器0
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY0_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY0_ADDR(base)                    ((base) + (0x167E0UL))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器1
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY1_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY1_ADDR(base)                    ((base) + (0x167E4UL))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器2
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY2_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY2_ADDR(base)                    ((base) + (0x167E8UL))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器3
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY3_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY3_ADDR(base)                    ((base) + (0x167ECUL))

/* 寄存器说明：DFX：仿真流量冲击状态
            （若RTL中配置DMSS_SIM_DFX_EN为0（通常ASIC版本配置为0，FPGA和EMULATOR版本定义为1），则此寄存器不存在）
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_LOAD_UNION */
#define SOC_DMSS_GLB_DFX_LOAD_ADDR(base)                      ((base) + (0x167F0UL))

/* 寄存器说明：MPU的DDR中地址空间属性配置寄存器
   位域定义UNION结构:  SOC_DMSS_MPU_ADDR_ATTR_UNION */
#define SOC_DMSS_MPU_ADDR_ATTR_ADDR(base, mpu_regs)           ((base) + (0x20000+0x4*(mpu_regs)))


#else


/* 寄存器说明：RTL配置信息0
   位域定义UNION结构:  SOC_DMSS_ASI_RTL_INF0_UNION */
#define SOC_DMSS_ASI_RTL_INF0_ADDR(base, asi_base)            ((base) + (0x000+0x800*(asi_base)))

/* 寄存器说明：RTL配置信息1
   位域定义UNION结构:  SOC_DMSS_ASI_RTL_INF1_UNION */
#define SOC_DMSS_ASI_RTL_INF1_ADDR(base, asi_base)            ((base) + (0x004+0x800*(asi_base)))

/* 寄存器说明：RTL配置信息2
   位域定义UNION结构:  SOC_DMSS_ASI_RTL_INF2_UNION */
#define SOC_DMSS_ASI_RTL_INF2_ADDR(base, asi_base)            ((base) + (0x008+0x800*(asi_base)))

/* 寄存器说明：静态时钟门控
   位域定义UNION结构:  SOC_DMSS_ASI_STA_CKG_UNION */
#define SOC_DMSS_ASI_STA_CKG_ADDR(base, asi_base)             ((base) + (0x010+0x800*(asi_base)))

/* 寄存器说明：动态时钟门控
   位域定义UNION结构:  SOC_DMSS_ASI_DYN_CKG_UNION */
#define SOC_DMSS_ASI_DYN_CKG_ADDR(base, asi_base)             ((base) + (0x014+0x800*(asi_base)))

/* 寄存器说明：异步桥时钟门控
   位域定义UNION结构:  SOC_DMSS_ASI_ASYNC_BRG_UNION */
#define SOC_DMSS_ASI_ASYNC_BRG_ADDR(base, asi_base)           ((base) + (0x018+0x800*(asi_base)))

/* 寄存器说明：地址移位
   位域定义UNION结构:  SOC_DMSS_ASI_ADDR_SHIFT_UNION */
#define SOC_DMSS_ASI_ADDR_SHIFT_ADDR(base, asi_base)          ((base) + (0x020+0x800*(asi_base)))

/* 寄存器说明：写数据速率匹配
   位域定义UNION结构:  SOC_DMSS_ASI_RATE_ADPT_UNION */
#define SOC_DMSS_ASI_RATE_ADPT_ADDR(base, asi_base)           ((base) + (0x030+0x800*(asi_base)))

/* 寄存器说明：防死锁控制
   位域定义UNION结构:  SOC_DMSS_ASI_ANTI_DEADLOCK_UNION */
#define SOC_DMSS_ASI_ANTI_DEADLOCK_ADDR(base, asi_base)       ((base) + (0x038+0x800*(asi_base)))

/* 寄存器说明：读保序控制
   位域定义UNION结构:  SOC_DMSS_ASI_RDR_CTRL_UNION */
#define SOC_DMSS_ASI_RDR_CTRL_ADDR(base, rdr_filters, asi_base)  ((base) + (0x040+0x4*(rdr_filters)+0x800*(asi_base)))

/* 寄存器说明：命令优先级控制
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_CTRL_UNION */
#define SOC_DMSS_ASI_QOS_CTRL_ADDR(base, asi_base)            ((base) + (0x100+0x800*(asi_base)))

/* 寄存器说明：写命令优先级映射表
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_WRPRI_UNION */
#define SOC_DMSS_ASI_QOS_WRPRI_ADDR(base, asi_base)           ((base) + (0x104+0x800*(asi_base)))

/* 寄存器说明：读命令优先级映射表
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RDPRI_UNION */
#define SOC_DMSS_ASI_QOS_RDPRI_ADDR(base, asi_base)           ((base) + (0x108+0x800*(asi_base)))

/* 寄存器说明：命令优先级自适应周期映射表
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_ADPT_UNION */
#define SOC_DMSS_ASI_QOS_ADPT_ADDR(base, asi_base)            ((base) + (0x10C+0x800*(asi_base)))

/* 寄存器说明：读命令的latency控制
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LATENCY_UNION */
#define SOC_DMSS_ASI_QOS_LATENCY_ADDR(base, asi_base)         ((base) + (0x110+0x800*(asi_base)))

/* 寄存器说明：带宽优先级控制0
            （可单独控制读命令优先级，或同时控制读写命令优先级）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR0_UNION */
#define SOC_DMSS_ASI_QOS_RGLR0_ADDR(base, asi_base)           ((base) + (0x120+0x800*(asi_base)))

/* 寄存器说明：带宽配置优先级0
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR0_PRI_UNION */
#define SOC_DMSS_ASI_QOS_RGLR0_PRI_ADDR(base, asi_base)       ((base) + (0x124+0x800*(asi_base)))

/* 寄存器说明：带宽优先级控制1
            （只可单独控制写命令优先级）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR1_UNION */
#define SOC_DMSS_ASI_QOS_RGLR1_ADDR(base, asi_base)           ((base) + (0x128+0x800*(asi_base)))

/* 寄存器说明：带宽配置优先级1
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RGLR1_PRI_UNION */
#define SOC_DMSS_ASI_QOS_RGLR1_PRI_ADDR(base, asi_base)       ((base) + (0x12C+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制0
            （可单独控制读带宽，或同时控制读写总带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR0_UNION */
#define SOC_DMSS_ASI_QOS_LMTR0_ADDR(base, asi_base)           ((base) + (0x140+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制1
            （只可单独控制写带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR1_UNION */
#define SOC_DMSS_ASI_QOS_LMTR1_ADDR(base, asi_base)           ((base) + (0x144+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制2
            （只可同时控制读写总带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR2_UNION */
#define SOC_DMSS_ASI_QOS_LMTR2_ADDR(base, asi_base)           ((base) + (0x148+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制3
            （只可单独控制写带宽）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR3_UNION */
#define SOC_DMSS_ASI_QOS_LMTR3_ADDR(base, asi_base)           ((base) + (0x14C+0x800*(asi_base)))

/* 寄存器说明：带宽限流控制4
            （只可控制SC方向读写总流量）
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_LMTR4_UNION */
#define SOC_DMSS_ASI_QOS_LMTR4_ADDR(base, asi_base)           ((base) + (0x150+0x800*(asi_base)))

/* 寄存器说明：读命令OSTD限制
   位域定义UNION结构:  SOC_DMSS_ASI_QOS_RDOSTD_UNION */
#define SOC_DMSS_ASI_QOS_RDOSTD_ADDR(base, asi_base)          ((base) + (0x160+0x800*(asi_base)))

/* 寄存器说明：写命令同MID命令优先级传递使能
   位域定义UNION结构:  SOC_DMSS_ASI_PUSH_WRMID_UNION */
#define SOC_DMSS_ASI_PUSH_WRMID_ADDR(base, asi_base)          ((base) + (0x180+0x800*(asi_base)))

/* 寄存器说明：读命令同MID命令优先级传递使能
   位域定义UNION结构:  SOC_DMSS_ASI_PUSH_RDMID_UNION */
#define SOC_DMSS_ASI_PUSH_RDMID_ADDR(base, asi_base)          ((base) + (0x184+0x800*(asi_base)))

/* 寄存器说明：统计功能屏蔽
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_MASK_UNION */
#define SOC_DMSS_ASI_STAT_MASK_ADDR(base, asi_base)           ((base) + (0x200+0x800*(asi_base)))

/* 寄存器说明：流量统计过滤配置0
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_FILTER0_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_FILTER0_ADDR(base, asi_base)   ((base) + (0x210+0x800*(asi_base)))

/* 寄存器说明：流量统计过滤配置1
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_FILTER1_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_FILTER1_ADDR(base, asi_base)   ((base) + (0x214+0x800*(asi_base)))

/* 寄存器说明：写流量统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_WR_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_WR_ADDR(base, asi_base)        ((base) + (0x218+0x800*(asi_base)))

/* 寄存器说明：读流量统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_FLUX_RD_UNION */
#define SOC_DMSS_ASI_STAT_FLUX_RD_ADDR(base, asi_base)        ((base) + (0x21C+0x800*(asi_base)))

/* 寄存器说明：带宽控制器0统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR0_UNION */
#define SOC_DMSS_ASI_STAT_LMTR0_ADDR(base, asi_base)          ((base) + (0x220+0x800*(asi_base)))

/* 寄存器说明：带宽控制器1统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR1_UNION */
#define SOC_DMSS_ASI_STAT_LMTR1_ADDR(base, asi_base)          ((base) + (0x224+0x800*(asi_base)))

/* 寄存器说明：带宽控制器2统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR2_UNION */
#define SOC_DMSS_ASI_STAT_LMTR2_ADDR(base, asi_base)          ((base) + (0x228+0x800*(asi_base)))

/* 寄存器说明：带宽控制器3统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR3_UNION */
#define SOC_DMSS_ASI_STAT_LMTR3_ADDR(base, asi_base)          ((base) + (0x22C+0x800*(asi_base)))

/* 寄存器说明：读latency统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_RDLAT_UNION */
#define SOC_DMSS_ASI_STAT_RDLAT_ADDR(base, asi_base)          ((base) + (0x230+0x800*(asi_base)))

/* 寄存器说明：读命令个数
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_RDNUM_UNION */
#define SOC_DMSS_ASI_STAT_RDNUM_ADDR(base, asi_base)          ((base) + (0x234+0x800*(asi_base)))

/* 寄存器说明：写命令阻塞统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_BLOCK_WR_UNION */
#define SOC_DMSS_ASI_STAT_BLOCK_WR_ADDR(base, asi_base)       ((base) + (0x238+0x800*(asi_base)))

/* 寄存器说明：读命令阻塞统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_BLOCK_RD_UNION */
#define SOC_DMSS_ASI_STAT_BLOCK_RD_ADDR(base, asi_base)       ((base) + (0x23C+0x800*(asi_base)))

/* 寄存器说明：命令优先级0和1统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI01_UNION */
#define SOC_DMSS_ASI_STAT_PRI01_ADDR(base, asi_base)          ((base) + (0x240+0x800*(asi_base)))

/* 寄存器说明：命令优先级2和3统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI23_UNION */
#define SOC_DMSS_ASI_STAT_PRI23_ADDR(base, asi_base)          ((base) + (0x244+0x800*(asi_base)))

/* 寄存器说明：命令优先级4和5统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI45_UNION */
#define SOC_DMSS_ASI_STAT_PRI45_ADDR(base, asi_base)          ((base) + (0x248+0x800*(asi_base)))

/* 寄存器说明：命令优先级6和7统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_PRI67_UNION */
#define SOC_DMSS_ASI_STAT_PRI67_ADDR(base, asi_base)          ((base) + (0x24C+0x800*(asi_base)))

/* 寄存器说明：带宽控制器4统计
   位域定义UNION结构:  SOC_DMSS_ASI_STAT_LMTR4_UNION */
#define SOC_DMSS_ASI_STAT_LMTR4_ADDR(base, asi_base)          ((base) + (0x250+0x800*(asi_base)))

/* 寄存器说明：Pattern detect功能控制0
   位域定义UNION结构:  SOC_DMSS_ASI_DETECT_CFG0_UNION */
#define SOC_DMSS_ASI_DETECT_CFG0_ADDR(base, asi_base)         ((base) + (0x280+0x800*(asi_base)))

/* 寄存器说明：Pattern detect功能控制1
   位域定义UNION结构:  SOC_DMSS_ASI_DETECT_CFG1_UNION */
#define SOC_DMSS_ASI_DETECT_CFG1_ADDR(base, asi_base)         ((base) + (0x284+0x800*(asi_base)))

/* 寄存器说明：Pattern detect信息记录0
   位域定义UNION结构:  SOC_DMSS_ASI_DETECT_INF0_UNION */
#define SOC_DMSS_ASI_DETECT_INF0_ADDR(base, asi_base)         ((base) + (0x288+0x800*(asi_base)))

/* 寄存器说明：Wakeup快速唤醒功能控制
   位域定义UNION结构:  SOC_DMSS_ASI_WAKEUP_CFG_UNION */
#define SOC_DMSS_ASI_WAKEUP_CFG_ADDR(base, asi_base)          ((base) + (0x290+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域0
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN0_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN0_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x300+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域1
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN1_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN1_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x304+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域2
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN2_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN2_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x308+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：安全地址区域子区域3
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_SUB_RGN3_UNION */
#define SOC_DMSS_ASI_SEC_SUB_RGN3_ADDR(base, asi_sub_rgns, asi_base)  ((base) + (0x30C+0x10*(asi_sub_rgns)+0x800*(asi_base)))

/* 寄存器说明：锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_LD_SEL_UNION */
#define SOC_DMSS_ASI_SEC_LD_SEL_ADDR(base, asi_base)          ((base) + (0x410+0x800*(asi_base)))

/* 寄存器说明：安全中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_INT_EN_UNION */
#define SOC_DMSS_ASI_SEC_INT_EN_ADDR(base, asi_base)          ((base) + (0x420+0x800*(asi_base)))

/* 寄存器说明：安全中断状态
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_INT_STATUS_UNION */
#define SOC_DMSS_ASI_SEC_INT_STATUS_ADDR(base, asi_base)      ((base) + (0x424+0x800*(asi_base)))

/* 寄存器说明：记录首个越权命令的信息0
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_ADDR(base, asi_base)   ((base) + (0x480+0x800*(asi_base)))

/* 寄存器说明：记录首个越权命令的信息1
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_ADDR(base, asi_base)   ((base) + (0x484+0x800*(asi_base)))

/* 寄存器说明：记录首个越权命令的信息2
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_ADDR(base, asi_base)   ((base) + (0x488+0x800*(asi_base)))

/* 寄存器说明：Protected越权访问AMI中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_INT_EN_UNION */
#define SOC_DMSS_ASI_AMI_PROT_INT_EN_ADDR(base, asi_base)     ((base) + (0x4A0+0x800*(asi_base)))

/* 寄存器说明：Protected越权访问AMI中断状态
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_INT_STATUS_UNION */
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_ADDR(base, asi_base)  ((base) + (0x4A4+0x800*(asi_base)))

/* 寄存器说明：记录首个Protected越权访问AMI拦截命令的信息0
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_CMD_INF0_UNION */
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF0_ADDR(base, asi_base)   ((base) + (0x4A8+0x800*(asi_base)))

/* 寄存器说明：记录首个Protected越权访问AMI拦截命令的信息1
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_PROT_CMD_INF1_UNION */
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ADDR(base, asi_base)   ((base) + (0x4AC+0x800*(asi_base)))

/* 寄存器说明：Enhance Trusted越权访问AMI中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_INT_EN_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_INT_EN_ADDR(base, asi_base)     ((base) + (0x4B0+0x800*(asi_base)))

/* 寄存器说明：Enhance Trusted越权访问AMI中断状态
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_ADDR(base, asi_base)  ((base) + (0x4B4+0x800*(asi_base)))

/* 寄存器说明：记录首个Enhance Trusted访问AMI命令的信息0
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_ADDR(base, asi_base)   ((base) + (0x4B8+0x800*(asi_base)))

/* 寄存器说明：记录首个Enhance Trusted访问AMI命令命令的信息1
   位域定义UNION结构:  SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_UNION */
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ADDR(base, asi_base)   ((base) + (0x4BC+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问non-trust地址空间配置0
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_ADDR(base, asi_base)  ((base) + (0x4C0+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问non-trust地址空间配置1
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_ADDR(base, asi_base)  ((base) + (0x4C4+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问protected地址空间配置0
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_ADDR(base, asi_base)  ((base) + (0x4C8+0x800*(asi_base)))

/* 寄存器说明：MPU trust写命令访问protected地址空间配置1
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_UNION */
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_ADDR(base, asi_base)  ((base) + (0x4CC+0x800*(asi_base)))

/* 寄存器说明：MPU protected写命令访问non-trust地址空间配置0
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_UNION */
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_ADDR(base, asi_base)  ((base) + (0x4D0+0x800*(asi_base)))

/* 寄存器说明：MPU protected写命令访问non-trust地址空间配置1
   位域定义UNION结构:  SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_UNION */
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_ADDR(base, asi_base)  ((base) + (0x4D4+0x800*(asi_base)))

/* 寄存器说明：地址区域映射0
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_RGN_MAP0_UNION */
#define SOC_DMSS_ASI_SEC_RGN_MAP0_ADDR(base, asi_rgns, asi_base)  ((base) + (0x500+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：地址区域映射1
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_RGN_MAP1_UNION */
#define SOC_DMSS_ASI_SEC_RGN_MAP1_ADDR(base, asi_rgns, asi_base)  ((base) + (0x504+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_MID_WR_UNION */
#define SOC_DMSS_ASI_SEC_MID_WR_ADDR(base, asi_rgns, asi_base)  ((base) + (0x508+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
   位域定义UNION结构:  SOC_DMSS_ASI_SEC_MID_RD_UNION */
#define SOC_DMSS_ASI_SEC_MID_RD_ADDR(base, asi_rgns, asi_base)  ((base) + (0x50C+0x10*(asi_rgns)+0x800*(asi_base)))

/* 寄存器说明：DFX：主要模块工作状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_MODULE_UNION */
#define SOC_DMSS_ASI_DFX_MODULE_ADDR(base, asi_base)          ((base) + (0x700+0x800*(asi_base)))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_FIFO_UNION */
#define SOC_DMSS_ASI_DFX_FIFO_ADDR(base, asi_base)            ((base) + (0x704+0x800*(asi_base)))

/* 寄存器说明：DFX：状态机工作状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_FSM_UNION */
#define SOC_DMSS_ASI_DFX_FSM_ADDR(base, asi_base)             ((base) + (0x708+0x800*(asi_base)))

/* 寄存器说明：DFX：OSTD状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_OSTD_UNION */
#define SOC_DMSS_ASI_DFX_OSTD_ADDR(base, asi_base)            ((base) + (0x70C+0x800*(asi_base)))

/* 寄存器说明：DFX：REORDER模块状态0
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_RDR0_UNION */
#define SOC_DMSS_ASI_DFX_RDR0_ADDR(base, asi_base)            ((base) + (0x710+0x800*(asi_base)))

/* 寄存器说明：DFX：REORDER模块状态1
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_RDR1_UNION */
#define SOC_DMSS_ASI_DFX_RDR1_ADDR(base, asi_base)            ((base) + (0x714+0x800*(asi_base)))

/* 寄存器说明：DFX：WOOO模块状态
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_WOOO_UNION */
#define SOC_DMSS_ASI_DFX_WOOO_ADDR(base, asi_base)            ((base) + (0x718+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息中断使能
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_INT_EN_UNION */
#define SOC_DMSS_ASI_DFX_INT_EN_ADDR(base, asi_base)          ((base) + (0x720+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息中断
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_INT_STATUS_UNION */
#define SOC_DMSS_ASI_DFX_INT_STATUS_ADDR(base, asi_base)      ((base) + (0x724+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息记录0
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_ABNM_INF0_UNION */
#define SOC_DMSS_ASI_DFX_ABNM_INF0_ADDR(base, asi_base)       ((base) + (0x730+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息记录1
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_ABNM_INF1_UNION */
#define SOC_DMSS_ASI_DFX_ABNM_INF1_ADDR(base, asi_base)       ((base) + (0x734+0x800*(asi_base)))

/* 寄存器说明：DFX：异常命令信息记录2
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_ABNM_INF2_UNION */
#define SOC_DMSS_ASI_DFX_ABNM_INF2_ADDR(base, asi_base)       ((base) + (0x738+0x800*(asi_base)))

/* 寄存器说明：DFX：安全中断状态
            （为便于调试，将寄存器ASI_SEC_INT_STATUS复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC0_UNION */
#define SOC_DMSS_ASI_DFX_SEC0_ADDR(base, asi_base)            ((base) + (0x740+0x800*(asi_base)))

/* 寄存器说明：DFX：记录首个越权命令的信息0
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF0复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC1_UNION */
#define SOC_DMSS_ASI_DFX_SEC1_ADDR(base, asi_base)            ((base) + (0x744+0x800*(asi_base)))

/* 寄存器说明：DFX：记录首个越权命令的信息1
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF1复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC2_UNION */
#define SOC_DMSS_ASI_DFX_SEC2_ADDR(base, asi_base)            ((base) + (0x748+0x800*(asi_base)))

/* 寄存器说明：DFX：记录首个越权命令的信息2
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF2复制到此）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_SEC3_UNION */
#define SOC_DMSS_ASI_DFX_SEC3_ADDR(base, asi_base)            ((base) + (0x74C+0x800*(asi_base)))

/* 寄存器说明：DFX：仿真流量冲击状态
            （若RTL中配置DMSS_SIM_DFX_EN为0（通常ASIC版本配置为0，FPGA和EMULATOR版本定义为1），则此寄存器不存在）
   位域定义UNION结构:  SOC_DMSS_ASI_DFX_LOAD_UNION */
#define SOC_DMSS_ASI_DFX_LOAD_ADDR(base, asi_base)            ((base) + (0x760+0x800*(asi_base)))

/* 寄存器说明：DMI代码配置信息0
   位域定义UNION结构:  SOC_DMSS_DMI_RTL_INF0_UNION */
#define SOC_DMSS_DMI_RTL_INF0_ADDR(base)                      ((base) + (0x10000))

/* 寄存器说明：DMI动态时钟门控
   位域定义UNION结构:  SOC_DMSS_DMI_DYN_CKG_UNION */
#define SOC_DMSS_DMI_DYN_CKG_ADDR(base)                       ((base) + (0x10010))

/* 寄存器说明：读命令DMC优先级自适应周期
   位域定义UNION结构:  SOC_DMSS_DMI_RD_ADPT_UNION */
#define SOC_DMSS_DMI_RD_ADPT_ADDR(base)                       ((base) + (0x10020))

/* 寄存器说明：写命令DMC优先级自适应周期
   位域定义UNION结构:  SOC_DMSS_DMI_WR_ADPT_UNION */
#define SOC_DMSS_DMI_WR_ADPT_ADDR(base)                       ((base) + (0x10024))

/* 寄存器说明：读命令DMC TIMEOUT映射表0
   位域定义UNION结构:  SOC_DMSS_DMI_RD_TIMEOUT0_UNION */
#define SOC_DMSS_DMI_RD_TIMEOUT0_ADDR(base)                   ((base) + (0x10030))

/* 寄存器说明：读命令DMC TIMEOUT映射表1
   位域定义UNION结构:  SOC_DMSS_DMI_RD_TIMEOUT1_UNION */
#define SOC_DMSS_DMI_RD_TIMEOUT1_ADDR(base)                   ((base) + (0x10034))

/* 寄存器说明：写命令DMC TIMEOUT映射表0
   位域定义UNION结构:  SOC_DMSS_DMI_WR_TIMEOUT0_UNION */
#define SOC_DMSS_DMI_WR_TIMEOUT0_ADDR(base)                   ((base) + (0x10038))

/* 寄存器说明：写命令DMC TIMEOUT映射表1
   位域定义UNION结构:  SOC_DMSS_DMI_WR_TIMEOUT1_UNION */
#define SOC_DMSS_DMI_WR_TIMEOUT1_ADDR(base)                   ((base) + (0x1003C))

/* 寄存器说明：GID地址区域映射0
   位域定义UNION结构:  SOC_DMSS_DMI_GID_RGN_MAP0_UNION */
#define SOC_DMSS_DMI_GID_RGN_MAP0_ADDR(base, gid_rgns)        ((base) + (0x10040+0x8*(gid_rgns)))

/* 寄存器说明：GID地址区域映射1
   位域定义UNION结构:  SOC_DMSS_DMI_GID_RGN_MAP1_UNION */
#define SOC_DMSS_DMI_GID_RGN_MAP1_ADDR(base, gid_rgns)        ((base) + (0x10044+0x8*(gid_rgns)))

/* 寄存器说明：锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_LD_SEL_UNION */
#define SOC_DMSS_DMI_SEC_LD_SEL_ADDR(base)                    ((base) + (0x10080))

/* 寄存器说明：DMI安全中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_INT_EN_UNION */
#define SOC_DMSS_DMI_SEC_INT_EN_ADDR(base)                    ((base) + (0x10084))

/* 寄存器说明：DMI安全中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_INT_STATUS_UNION */
#define SOC_DMSS_DMI_SEC_INT_STATUS_ADDR(base, dmi_base)      ((base) + (0x10088+0x800*(dmi_base)))

/* 寄存器说明：DMI Enhance安全中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_INT_EN_UNION */
#define SOC_DMSS_DMI_ENHN_INT_EN_ADDR(base)                   ((base) + (0x1008C))

/* 寄存器说明：DMI Enhance安全中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_INT_STATUS_UNION */
#define SOC_DMSS_DMI_ENHN_INT_STATUS_ADDR(base, dmi_base)     ((base) + (0x10090+0x800*(dmi_base)))

/* 寄存器说明：记录首个越权命令的信息0（包括DMI安全拦截和DMI Enhance安全拦截）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_ADDR(base, dmi_base)   ((base) + (0x10094+0x800*(dmi_base)))

/* 寄存器说明：记录首个越权命令的信息1（包括DMI安全拦截和DMI Enhance安全拦截）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_ADDR(base, dmi_base)   ((base) + (0x10098+0x800*(dmi_base)))

/* 寄存器说明：记录首个越权命令的信息2（包括DMI安全拦截和DMI Enhance安全拦截）
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_ADDR(base, dmi_base)   ((base) + (0x1009C+0x800*(dmi_base)))

/* 寄存器说明：MPU中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_INT_EN_UNION */
#define SOC_DMSS_DMI_MPU_INT_EN_ADDR(base)                    ((base) + (0x100A0))

/* 寄存器说明：MPU中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_INT_STATUS_UNION */
#define SOC_DMSS_DMI_MPU_INT_STATUS_ADDR(base, dmi_base)      ((base) + (0x100A4+0x800*(dmi_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息0
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_ADDR(base, dmi_base)   ((base) + (0x100A8+0x800*(dmi_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息1
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_ADDR(base, dmi_base)   ((base) + (0x100AC+0x800*(dmi_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息2
   位域定义UNION结构:  SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_ADDR(base, dmi_base)   ((base) + (0x100B0+0x800*(dmi_base)))

/* 寄存器说明：DMI Enhance寄存器配置中断使能
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_CFG_INT_EN_UNION */
#define SOC_DMSS_DMI_ENHN_CFG_INT_EN_ADDR(base)               ((base) + (0x100B4))

/* 寄存器说明：DMI Enhance寄存器配置中断状态
   位域定义UNION结构:  SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_UNION */
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_ADDR(base, dmi_base)  ((base) + (0x100B8+0x800*(dmi_base)))

/* 寄存器说明：地址区域映射0
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_RGN_MAP0_UNION */
#define SOC_DMSS_DMI_SEC_RGN_MAP0_ADDR(base, dmi_rgns)        ((base) + (0x10100+0x30*(dmi_rgns)))

/* 寄存器说明：地址区域映射1
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_RGN_MAP1_UNION */
#define SOC_DMSS_DMI_SEC_RGN_MAP1_ADDR(base, dmi_rgns)        ((base) + (0x10104+0x30*(dmi_rgns)))

/* 寄存器说明：地址区域属性
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_RGN_ATTR_UNION */
#define SOC_DMSS_DMI_SEC_RGN_ATTR_ADDR(base, dmi_enhn_rgns)   ((base) + (0x10108+0x30*(dmi_enhn_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR0_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR0_ADDR(base, dmi_mid_rgns)     ((base) + (0x10110+0x30*(dmi_mid_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR1_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR1_ADDR(base, dmi_mid_rgns)     ((base) + (0x10114+0x30*(dmi_mid_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR2_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR2_ADDR(base, dmi_mid_rgns)     ((base) + (0x10118+0x30*(dmi_mid_rgns)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_WR3_UNION */
#define SOC_DMSS_DMI_SEC_MID_WR3_ADDR(base, dmi_mid_rgns)     ((base) + (0x1011C+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD0_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD0_ADDR(base, dmi_mid_rgns)     ((base) + (0x10120+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD1_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD1_ADDR(base, dmi_mid_rgns)     ((base) + (0x10124+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD2_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD2_ADDR(base, dmi_mid_rgns)     ((base) + (0x10128+0x30*(dmi_mid_rgns)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_DMI_SEC_MID_RD3_UNION */
#define SOC_DMSS_DMI_SEC_MID_RD3_ADDR(base, dmi_mid_rgns)     ((base) + (0x1012C+0x30*(dmi_mid_rgns)))

/* 寄存器说明：DFX：各个主要模块的工作状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_MODULE_UNION */
#define SOC_DMSS_DMI_DFX_MODULE_ADDR(base, dmi_base)          ((base) + (0x10700+0x800*(dmi_base)))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_FIFO_UNION */
#define SOC_DMSS_DMI_DFX_FIFO_ADDR(base, dmi_base)            ((base) + (0x10704+0x800*(dmi_base)))

/* 寄存器说明：DFX：写数据FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_WD_FIFO_UNION */
#define SOC_DMSS_DMI_DFX_WD_FIFO_ADDR(base, dmi_base)         ((base) + (0x10708+0x800*(dmi_base)))

/* 寄存器说明：DFX：DMI的模块状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_ST_UNION */
#define SOC_DMSS_DMI_DFX_ST_ADDR(base, dmi_base)              ((base) + (0x1070C+0x800*(dmi_base)))

/* 寄存器说明：DFX：DCSC的模块状态
   位域定义UNION结构:  SOC_DMSS_DMI_DFX_DCSC_UNION */
#define SOC_DMSS_DMI_DFX_DCSC_ADDR(base, dmi_base)            ((base) + (0x10710+0x800*(dmi_base)))

/* 寄存器说明：AMI代码配置信息0
   位域定义UNION结构:  SOC_DMSS_AMI_RTL_INF0_UNION */
#define SOC_DMSS_AMI_RTL_INF0_ADDR(base, ami_base)            ((base) + (0x12000+0x800*(ami_base)))

/* 寄存器说明：AMI动态时钟门控
   位域定义UNION结构:  SOC_DMSS_AMI_DYN_CKG_UNION */
#define SOC_DMSS_AMI_DYN_CKG_ADDR(base, ami_base)             ((base) + (0x12010+0x800*(ami_base)))

/* 寄存器说明：AMI GID控制配置
   位域定义UNION结构:  SOC_DMSS_AMI_GID_CTRL_UNION */
#define SOC_DMSS_AMI_GID_CTRL_ADDR(base, ami_base)            ((base) + (0x12020+0x800*(ami_base)))

/* 寄存器说明：锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_LD_SEL_UNION */
#define SOC_DMSS_AMI_SEC_LD_SEL_ADDR(base, ami_base)          ((base) + (0x12080+0x800*(ami_base)))

/* 寄存器说明：安全中断使能
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_INT_EN_UNION */
#define SOC_DMSS_AMI_SEC_INT_EN_ADDR(base, ami_base)          ((base) + (0x12084+0x800*(ami_base)))

/* 寄存器说明：安全中断状态
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_INT_STATUS_UNION */
#define SOC_DMSS_AMI_SEC_INT_STATUS_ADDR(base, ami_base)      ((base) + (0x12088+0x800*(ami_base)))

/* 寄存器说明：记录首个越权命令的信息0
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_ADDR(base, ami_base)   ((base) + (0x12094+0x800*(ami_base)))

/* 寄存器说明：记录首个越权命令的信息1
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_ADDR(base, ami_base)   ((base) + (0x12098+0x800*(ami_base)))

/* 寄存器说明：记录首个越权命令的信息2
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_ADDR(base, ami_base)   ((base) + (0x1209C+0x800*(ami_base)))

/* 寄存器说明：MPU中断使能
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_INT_EN_UNION */
#define SOC_DMSS_AMI_MPU_INT_EN_ADDR(base, ami_base)          ((base) + (0x120A0+0x800*(ami_base)))

/* 寄存器说明：MPU中断状态
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_INT_STATUS_UNION */
#define SOC_DMSS_AMI_MPU_INT_STATUS_ADDR(base, ami_base)      ((base) + (0x120A4+0x800*(ami_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息0
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_UNION */
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_ADDR(base, ami_base)   ((base) + (0x120A8+0x800*(ami_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息1
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_UNION */
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_ADDR(base, ami_base)   ((base) + (0x120AC+0x800*(ami_base)))

/* 寄存器说明：记录首个MPU拦截命令的信息2
   位域定义UNION结构:  SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_UNION */
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_ADDR(base, ami_base)   ((base) + (0x120B0+0x800*(ami_base)))

/* 寄存器说明：地址区域映射0
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_RGN_MAP0_UNION */
#define SOC_DMSS_AMI_SEC_RGN_MAP0_ADDR(base, ami_rgns, ami_base)  ((base) + (0x12100+0x30*(ami_rgns)+0x800*(ami_base)))

/* 寄存器说明：地址区域映射1
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_RGN_MAP1_UNION */
#define SOC_DMSS_AMI_SEC_RGN_MAP1_ADDR(base, ami_rgns, ami_base)  ((base) + (0x12104+0x30*(ami_rgns)+0x800*(ami_base)))

/* 寄存器说明：地址区域属性
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_RGN_ATTR_UNION */
#define SOC_DMSS_AMI_SEC_RGN_ATTR_ADDR(base, ami_enhn_rgns, ami_base)  ((base) + (0x12108+0x30*(ami_enhn_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR0_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR0_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12110+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR1_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR1_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12114+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR2_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR2_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12118+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：写命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_WR3_UNION */
#define SOC_DMSS_AMI_SEC_MID_WR3_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x1211C+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD0_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD0_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12120+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD1_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD1_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12124+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD2_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD2_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x12128+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：读命令匹配MasterID及其掩码
   位域定义UNION结构:  SOC_DMSS_AMI_SEC_MID_RD3_UNION */
#define SOC_DMSS_AMI_SEC_MID_RD3_ADDR(base, ami_mid_rgns, ami_base)  ((base) + (0x1212C+0x30*(ami_mid_rgns)+0x800*(ami_base)))

/* 寄存器说明：DFX：各个主要模块的工作状态
   位域定义UNION结构:  SOC_DMSS_AMI_DFX_MODULE_UNION */
#define SOC_DMSS_AMI_DFX_MODULE_ADDR(base, ami_base)          ((base) + (0x12700+0x800*(ami_base)))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_AMI_DFX_FIFO_UNION */
#define SOC_DMSS_AMI_DFX_FIFO_ADDR(base, ami_base)            ((base) + (0x12704+0x800*(ami_base)))

/* 寄存器说明：DFX：AMI的模块状态
   位域定义UNION结构:  SOC_DMSS_AMI_DFX_ST_UNION */
#define SOC_DMSS_AMI_DFX_ST_ADDR(base, ami_base)              ((base) + (0x12708+0x800*(ami_base)))

/* 寄存器说明：代码版本信息
   位域定义UNION结构:  SOC_DMSS_GLB_RTL_VER_UNION */
#define SOC_DMSS_GLB_RTL_VER_ADDR(base)                       ((base) + (0x16000))

/* 寄存器说明：代码配置信息0
   位域定义UNION结构:  SOC_DMSS_GLB_RTL_INF0_UNION */
#define SOC_DMSS_GLB_RTL_INF0_ADDR(base)                      ((base) + (0x16004))

/* 寄存器说明：代码配置信息1
   位域定义UNION结构:  SOC_DMSS_GLB_RTL_INF1_UNION */
#define SOC_DMSS_GLB_RTL_INF1_ADDR(base)                      ((base) + (0x16008))

/* 寄存器说明：TP-MEMORY配置0
   位域定义UNION结构:  SOC_DMSS_GLB_RAM_TMOD0_UNION */
#define SOC_DMSS_GLB_RAM_TMOD0_ADDR(base)                     ((base) + (0x16010))

/* 寄存器说明：TP-MEMORY配置1
   位域定义UNION结构:  SOC_DMSS_GLB_RAM_TMOD1_UNION */
#define SOC_DMSS_GLB_RAM_TMOD1_ADDR(base)                     ((base) + (0x16014))

/* 寄存器说明：SP-MEMORY配置
   位域定义UNION结构:  SOC_DMSS_GLB_RAM_SMOD_UNION */
#define SOC_DMSS_GLB_RAM_SMOD_ADDR(base)                      ((base) + (0x16018))

/* 寄存器说明：动态时钟门控
   位域定义UNION结构:  SOC_DMSS_GLB_DYN_CKG_UNION */
#define SOC_DMSS_GLB_DYN_CKG_ADDR(base)                       ((base) + (0x16024))

/* 寄存器说明：全局中断状态查看
   位域定义UNION结构:  SOC_DMSS_GLB_INT_STATUS0_UNION */
#define SOC_DMSS_GLB_INT_STATUS0_ADDR(base)                   ((base) + (0x16030))

/* 寄存器说明：全局中断状态清除
   位域定义UNION结构:  SOC_DMSS_GLB_INT_CLEAR0_UNION */
#define SOC_DMSS_GLB_INT_CLEAR0_ADDR(base)                    ((base) + (0x16034))

/* 寄存器说明：全局中断状态查看
   位域定义UNION结构:  SOC_DMSS_GLB_INT_STATUS1_UNION */
#define SOC_DMSS_GLB_INT_STATUS1_ADDR(base)                   ((base) + (0x16038))

/* 寄存器说明：全局中断状态清除
   位域定义UNION结构:  SOC_DMSS_GLB_INT_CLEAR1_UNION */
#define SOC_DMSS_GLB_INT_CLEAR1_ADDR(base)                    ((base) + (0x1603C))

/* 寄存器说明：exclusive命令配置
   位域定义UNION结构:  SOC_DMSS_GLB_EXCLU_UNION */
#define SOC_DMSS_GLB_EXCLU_ADDR(base)                         ((base) + (0x16040))

/* 寄存器说明：全局地址交织模式
   位域定义UNION结构:  SOC_DMSS_GLB_ADDR_INTLV_UNION */
#define SOC_DMSS_GLB_ADDR_INTLV_ADDR(base)                    ((base) + (0x16100))

/* 寄存器说明：全局地址加扰模式
   位域定义UNION结构:  SOC_DMSS_GLB_ADDR_SCRMBL_UNION */
#define SOC_DMSS_GLB_ADDR_SCRMBL_ADDR(base)                   ((base) + (0x16110))

/* 寄存器说明：RBUF的控制配置
   位域定义UNION结构:  SOC_DMSS_GLB_RBUF_CTRL_UNION */
#define SOC_DMSS_GLB_RBUF_CTRL_ADDR(base)                     ((base) + (0x16148))

/* 寄存器说明：RBUF的传输控制配置
   位域定义UNION结构:  SOC_DMSS_GLB_RBUF_TRANS_CTRL_UNION */
#define SOC_DMSS_GLB_RBUF_TRANS_CTRL_ADDR(base)               ((base) + (0x1614C))

/* 寄存器说明：RBUF的命令反压配置
   位域定义UNION结构:  SOC_DMSS_GLB_RBUF_BP_UNION */
#define SOC_DMSS_GLB_RBUF_BP_ADDR(base, rbuf_bp_grps)         ((base) + (0x16150+0x4*(rbuf_bp_grps)))

/* 寄存器说明：AMI0返回响应ERROR中断使能
   位域定义UNION结构:  SOC_DMSS_GLB_RSP_ERR_INT_EN_UNION */
#define SOC_DMSS_GLB_RSP_ERR_INT_EN_ADDR(base)                ((base) + (0x16180))

/* 寄存器说明：AMI0返回响应ERROR中断状态
   位域定义UNION结构:  SOC_DMSS_GLB_RSP_ERR_INT_STATUS_UNION */
#define SOC_DMSS_GLB_RSP_ERR_INT_STATUS_ADDR(base)            ((base) + (0x16184))

/* 寄存器说明：AMI的控制配置
   位域定义UNION结构:  SOC_DMSS_GLB_AMI_CTRL_UNION */
#define SOC_DMSS_GLB_AMI_CTRL_ADDR(base)                      ((base) + (0x161A0))

/* 寄存器说明：DDRC的命令队列状态统计控制
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_CTRL_UNION */
#define SOC_DMSS_GLB_DCQ_CTRL_ADDR(base)                      ((base) + (0x16200))

/* 寄存器说明：基于DDRC读写总命令队列状态的允许优先级水线0
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_PRILVL0_UNION */
#define SOC_DMSS_GLB_DCQ_PRILVL0_ADDR(base, chans)            ((base) + (0x16220+0x10*(chans)))

/* 寄存器说明：基于DDRC读写总命令队列状态的允许优先级水线1
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_PRILVL1_UNION */
#define SOC_DMSS_GLB_DCQ_PRILVL1_ADDR(base, chans)            ((base) + (0x16224+0x10*(chans)))

/* 寄存器说明：基于DDRC写命令队列状态的允许优先级水线0
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_WR_PRILVL0_UNION */
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_ADDR(base, chans)         ((base) + (0x16228+0x10*(chans)))

/* 寄存器说明：基于DDRC写命令队列状态的允许优先级水线1
   位域定义UNION结构:  SOC_DMSS_GLB_DCQ_WR_PRILVL1_UNION */
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_ADDR(base, chans)         ((base) + (0x1622C+0x10*(chans)))

/* 寄存器说明：SystemCache控制
   位域定义UNION结构:  SOC_DMSS_GLB_SC_CTRL_UNION */
#define SOC_DMSS_GLB_SC_CTRL_ADDR(base)                       ((base) + (0x16260))

/* 寄存器说明：基于DCQ的SystemCache流控信息
   位域定义UNION结构:  SOC_DMSS_GLB_SC_DCQLVL_UNION */
#define SOC_DMSS_GLB_SC_DCQLVL_ADDR(base)                     ((base) + (0x16264))

/* 寄存器说明：外部master的流控功能
   位域定义UNION结构:  SOC_DMSS_GLB_MST_FLUX_UNION */
#define SOC_DMSS_GLB_MST_FLUX_ADDR(base, dcq_msts)            ((base) + (0x16280+0x4*(dcq_msts)))

/* 寄存器说明：TRACE功能控制0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_CTRL0_UNION */
#define SOC_DMSS_GLB_TRACE_CTRL0_ADDR(base)                   ((base) + (0x16300))

/* 寄存器说明：TRACE功能控制1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_CTRL1_UNION */
#define SOC_DMSS_GLB_TRACE_CTRL1_ADDR(base)                   ((base) + (0x16304))

/* 寄存器说明：TRACE功能控制2
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_CTRL2_UNION */
#define SOC_DMSS_GLB_TRACE_CTRL2_ADDR(base)                   ((base) + (0x16308))

/* 寄存器说明：TRACE功能记录配置0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_REC0_UNION */
#define SOC_DMSS_GLB_TRACE_REC0_ADDR(base)                    ((base) + (0x16310))

/* 寄存器说明：TRACE功能记录配置1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_REC1_UNION */
#define SOC_DMSS_GLB_TRACE_REC1_ADDR(base)                    ((base) + (0x16314))

/* 寄存器说明：TRACE功能监控过滤0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FILTER0_UNION */
#define SOC_DMSS_GLB_TRACE_FILTER0_ADDR(base)                 ((base) + (0x16318))

/* 寄存器说明：TRACE功能监控过滤1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FILTER1_UNION */
#define SOC_DMSS_GLB_TRACE_FILTER1_ADDR(base)                 ((base) + (0x1631C))

/* 寄存器说明：TRACE功能监控过滤2
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FILTER2_UNION */
#define SOC_DMSS_GLB_TRACE_FILTER2_ADDR(base, mid_grps)       ((base) + (0x16320+0x4*(mid_grps)))

/* 寄存器说明：TRACE功能时钟频率记录
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_FREQ_UNION */
#define SOC_DMSS_GLB_TRACE_FREQ_ADDR(base)                    ((base) + (0x16340))

/* 寄存器说明：TRACE功能中断控制
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_INT_EN_UNION */
#define SOC_DMSS_GLB_TRACE_INT_EN_ADDR(base)                  ((base) + (0x16350))

/* 寄存器说明：TRACE功能锁定控制
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_LOCK_DOWN_UNION */
#define SOC_DMSS_GLB_TRACE_LOCK_DOWN_ADDR(base)               ((base) + (0x16358))

/* 寄存器说明：TRACE功能统计信息0
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_STAT0_UNION */
#define SOC_DMSS_GLB_TRACE_STAT0_ADDR(base, dmis)             ((base) + (0x16360+0x10*(dmis)))

/* 寄存器说明：TRACE功能统计信息1
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_STAT1_UNION */
#define SOC_DMSS_GLB_TRACE_STAT1_ADDR(base, dmis)             ((base) + (0x16364+0x10*(dmis)))

/* 寄存器说明：TRACE功能统计信息2
   位域定义UNION结构:  SOC_DMSS_GLB_TRACE_STAT2_UNION */
#define SOC_DMSS_GLB_TRACE_STAT2_ADDR(base, dmis)             ((base) + (0x16368+0x10*(dmis)))

/* 寄存器说明：MPU的全局配置寄存器
   位域定义UNION结构:  SOC_DMSS_GLB_MPU_CFG_UNION */
#define SOC_DMSS_GLB_MPU_CFG_ADDR(base)                       ((base) + (0x16380))

/* 寄存器说明：MPU拦截后特征地址配置
   位域定义UNION结构:  SOC_DMSS_GLB_MPU_ERROR_ADDR_UNION */
#define SOC_DMSS_GLB_MPU_ERROR_ADDR_ADDR(base)                ((base) + (0x16384))

/* 寄存器说明：MPU的ACPU Stream配置寄存器
   位域定义UNION结构:  SOC_DMSS_GLB_MPU_ACPU_CFG_UNION */
#define SOC_DMSS_GLB_MPU_ACPU_CFG_ADDR(base)                  ((base) + (0x16388))

/* 寄存器说明：安全整体功能透传
   位域定义UNION结构:  SOC_DMSS_GLB_SEC_BYPASS_UNION */
#define SOC_DMSS_GLB_SEC_BYPASS_ADDR(base)                    ((base) + (0x1638C))

/* 寄存器说明：统计功能控制
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_CTRL_UNION */
#define SOC_DMSS_GLB_STAT_CTRL_ADDR(base)                     ((base) + (0x16400))

/* 寄存器说明：统计周期
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_CYCLE_UNION */
#define SOC_DMSS_GLB_STAT_CYCLE_ADDR(base)                    ((base) + (0x16410))

/* 寄存器说明：perf_stat模块配置
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_PERFSTAT_UNION */
#define SOC_DMSS_GLB_STAT_PERFSTAT_ADDR(base)                 ((base) + (0x16420))

/* 寄存器说明：DCQ队列写命令个数的累积值统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_DCQ_WR_UNION */
#define SOC_DMSS_GLB_STAT_DCQ_WR_ADDR(base, chans)            ((base) + (0x16440+0x4*(chans)))

/* 寄存器说明：DCQ队列所有命令个数的累积值统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_DCQ_UNION */
#define SOC_DMSS_GLB_STAT_DCQ_ADDR(base, chans)               ((base) + (0x16450+0x4*(chans)))

/* 寄存器说明：所有写优先级都允许的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_ADDR(base, chans)      ((base) + (0x16500+0x40*(chans)))

/* 寄存器说明：写允许优先级为1的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_ADDR(base, chans)     ((base) + (0x16504+0x40*(chans)))

/* 寄存器说明：写允许优先级为2的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_ADDR(base, chans)     ((base) + (0x16508+0x40*(chans)))

/* 寄存器说明：写允许优先级为3的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_ADDR(base, chans)     ((base) + (0x1650C+0x40*(chans)))

/* 寄存器说明：写允许优先级为4的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_ADDR(base, chans)     ((base) + (0x16510+0x40*(chans)))

/* 寄存器说明：写允许优先级为5的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_ADDR(base, chans)     ((base) + (0x16514+0x40*(chans)))

/* 寄存器说明：写允许优先级为6的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_ADDR(base, chans)     ((base) + (0x16518+0x40*(chans)))

/* 寄存器说明：写允许优先级为7的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_ADDR(base, chans)     ((base) + (0x1651C+0x40*(chans)))

/* 寄存器说明：所有读优先级都允许的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_ADDR(base, chans)      ((base) + (0x16520+0x40*(chans)))

/* 寄存器说明：读允许优先级为1的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_ADDR(base, chans)     ((base) + (0x16524+0x40*(chans)))

/* 寄存器说明：读允许优先级为2的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_ADDR(base, chans)     ((base) + (0x16528+0x40*(chans)))

/* 寄存器说明：读允许优先级为3的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_ADDR(base, chans)     ((base) + (0x1652C+0x40*(chans)))

/* 寄存器说明：读允许优先级为4的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_ADDR(base, chans)     ((base) + (0x16530+0x40*(chans)))

/* 寄存器说明：读允许优先级为5的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_ADDR(base, chans)     ((base) + (0x16534+0x40*(chans)))

/* 寄存器说明：读允许优先级为6的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_ADDR(base, chans)     ((base) + (0x16538+0x40*(chans)))

/* 寄存器说明：读允许优先级为7的统计
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_UNION */
#define SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_ADDR(base, chans)     ((base) + (0x1653C+0x40*(chans)))

/* 寄存器说明：RBUF占用率统计控制
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_CTRL_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_ADDR(base)                ((base) + (0x16600))

/* 寄存器说明：RBUF占用率统计结果0
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_OCCUP0_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP0_ADDR(base)              ((base) + (0x16604))

/* 寄存器说明：RBUF占用率统计结果1
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_OCCUP1_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP1_ADDR(base)              ((base) + (0x16608))

/* 寄存器说明：RBUF占用率统计结果2
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_RBUF_OCCUP2_UNION */
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP2_ADDR(base)              ((base) + (0x1660C))

/* 寄存器说明：AMI写数据统计结果
   位域定义UNION结构:  SOC_DMSS_GLB_STAT_AMI_WFIFO_UNION */
#define SOC_DMSS_GLB_STAT_AMI_WFIFO_ADDR(base)                ((base) + (0x16640))

/* 寄存器说明：DFX：各个主要模块的工作状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_MODULE_UNION */
#define SOC_DMSS_GLB_DFX_MODULE_ADDR(base)                    ((base) + (0x16700))

/* 寄存器说明：DFX：FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_FIFO_UNION */
#define SOC_DMSS_GLB_DFX_FIFO_ADDR(base)                      ((base) + (0x16704))

/* 寄存器说明：DFX：RBUF FIFO工作状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_RBUF_FIFO_UNION */
#define SOC_DMSS_GLB_DFX_RBUF_FIFO_ADDR(base)                 ((base) + (0x16708))

/* 寄存器说明：DFX：DCQ的队列实时状态
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DCQ_ST_UNION */
#define SOC_DMSS_GLB_DFX_DCQ_ST_ADDR(base, chans)             ((base) + (0x16710+0x4*(chans)))

/* 寄存器说明：DFX：RBUF状态0
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_RBUF0_UNION */
#define SOC_DMSS_GLB_DFX_RBUF0_ADDR(base, rbuf_srcs)          ((base) + (0x16720+0x10*(rbuf_srcs)))

/* 寄存器说明：DFX：RBUF状态1
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_RBUF1_UNION */
#define SOC_DMSS_GLB_DFX_RBUF1_ADDR(base, rbuf_srcs)          ((base) + (0x16724+0x10*(rbuf_srcs)))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器0
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY0_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY0_ADDR(base)                    ((base) + (0x167E0))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器1
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY1_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY1_ADDR(base)                    ((base) + (0x167E4))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器2
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY2_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY2_ADDR(base)                    ((base) + (0x167E8))

/* 寄存器说明：DFX：M536 PTRN_MDM的状态寄存器3
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_DUMMY3_UNION */
#define SOC_DMSS_GLB_DFX_DUMMY3_ADDR(base)                    ((base) + (0x167EC))

/* 寄存器说明：DFX：仿真流量冲击状态
            （若RTL中配置DMSS_SIM_DFX_EN为0（通常ASIC版本配置为0，FPGA和EMULATOR版本定义为1），则此寄存器不存在）
   位域定义UNION结构:  SOC_DMSS_GLB_DFX_LOAD_UNION */
#define SOC_DMSS_GLB_DFX_LOAD_ADDR(base)                      ((base) + (0x167F0))

/* 寄存器说明：MPU的DDR中地址空间属性配置寄存器
   位域定义UNION结构:  SOC_DMSS_MPU_ADDR_ATTR_UNION */
#define SOC_DMSS_MPU_ADDR_ATTR_ADDR(base, mpu_regs)           ((base) + (0x20000+0x4*(mpu_regs)))


#endif




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
                     (1/1) register_DMSS
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_DMSS_ASI_RTL_INF0_UNION
 结构说明  : ASI_RTL_INF0 寄存器结构定义。地址偏移量:0x000+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: RTL配置信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_data_width    : 1;  /* bit[0]    : 对接的AXI总线地址位宽：
                                                             0：64bits
                                                             1：128bits。 */
        unsigned int  reserved_0        : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  rtl_clk_mode      : 2;  /* bit[4-5]  : 对接AXI总线时钟与DMSS主时钟的关系：
                                                             0x0：同频同步；
                                                             0x1：低频同步（低频的AXI总线侧不可以设置multicycle，需以高频时钟收时序）；
                                                             0x2：低频同步（低频的AXI总线侧可按低频时钟收时序）；
                                                             0x3：异步。
                                                             注意：当对接时钟关系不是异步时，异步相关寄存器无效。 */
        unsigned int  reserved_1        : 10; /* bit[6-15] : 保留。 */
        unsigned int  rtl_rdr_que_depth : 4;  /* bit[16-19]: 保序模块的读命令队列监控深度（等效为允许的最大读命令outstanding）：
                                                             0：40 read cmd；
                                                             1：16 read cmd；
                                                             2：24 read cmd；
                                                             3：32 read cmd；
                                                             4: 36 read cmd；
                                                             其他：保留。 */
        unsigned int  rtl_rdr_buf_num   : 2;  /* bit[20-21]: 保序模块的保序buffer深度：
                                                             0： 8 read burst；
                                                             1：16 read burst；
                                                             2：24 read burst；
                                                             3：32 read burst。 */
        unsigned int  reserved_2        : 2;  /* bit[22-23]: 保留。 */
        unsigned int  rtl_rdr_en        : 1;  /* bit[24]   : 同ID读保序功能：
                                                             0：不支持；
                                                             1：支持。
                                                             注意：
                                                             1.只有在系统层面保证当前ASI不会收到outstanding的同ID读命令，且任何读命令也不会被ASI拆分成多个同ID的读命令时，才可以RTL配置为不支持保序功能；
                                                             2.当不支持同ID读保序功能时，读保序相关寄存器无效。 */
        unsigned int  reserved_3        : 7;  /* bit[25-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_RTL_INF0_UNION;
#endif
#define SOC_DMSS_ASI_RTL_INF0_rtl_data_width_START     (0)
#define SOC_DMSS_ASI_RTL_INF0_rtl_data_width_END       (0)
#define SOC_DMSS_ASI_RTL_INF0_rtl_clk_mode_START       (4)
#define SOC_DMSS_ASI_RTL_INF0_rtl_clk_mode_END         (5)
#define SOC_DMSS_ASI_RTL_INF0_rtl_rdr_que_depth_START  (16)
#define SOC_DMSS_ASI_RTL_INF0_rtl_rdr_que_depth_END    (19)
#define SOC_DMSS_ASI_RTL_INF0_rtl_rdr_buf_num_START    (20)
#define SOC_DMSS_ASI_RTL_INF0_rtl_rdr_buf_num_END      (21)
#define SOC_DMSS_ASI_RTL_INF0_rtl_rdr_en_START         (24)
#define SOC_DMSS_ASI_RTL_INF0_rtl_rdr_en_END           (24)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_RTL_INF1_UNION
 结构说明  : ASI_RTL_INF1 寄存器结构定义。地址偏移量:0x004+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: RTL配置信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_wcmd_fifo_depth : 5;  /* bit[0-4]  : 写命令FIFO配置深度：
                                                               0x0： 1 write cmd；
                                                               0x1： 2 write cmd；
                                                               ……
                                                               0x1F：32 write cmd。 */
        unsigned int  reserved_0          : 3;  /* bit[5-7]  : 保留。 */
        unsigned int  rtl_wd_fifo_depth   : 5;  /* bit[8-12] : 写数据FIFO配置深度：
                                                               0x0： 1 write transfer；
                                                               0x1： 2 write transfer；
                                                               ……
                                                               0x1F：32 write transfer。 */
        unsigned int  reserved_1          : 3;  /* bit[13-15]: 保留。 */
        unsigned int  rtl_b_fifo_depth    : 5;  /* bit[16-20]: 写响应FIFO配置深度：
                                                               0x0： 1 write response；
                                                               0x1： 2 write response；
                                                               ……
                                                               0x1F：32 write response。 */
        unsigned int  reserved_2          : 3;  /* bit[21-23]: 保留。 */
        unsigned int  rtl_ra_fifo_depth   : 5;  /* bit[24-28]: 速率匹配写数据FIFO配置深度：
                                                               0x0： 1 write transfer；
                                                               0x1： 2 write transfer；
                                                               ……
                                                               0x1F：32 write transfer。 */
        unsigned int  reserved_3          : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_RTL_INF1_UNION;
#endif
#define SOC_DMSS_ASI_RTL_INF1_rtl_wcmd_fifo_depth_START  (0)
#define SOC_DMSS_ASI_RTL_INF1_rtl_wcmd_fifo_depth_END    (4)
#define SOC_DMSS_ASI_RTL_INF1_rtl_wd_fifo_depth_START    (8)
#define SOC_DMSS_ASI_RTL_INF1_rtl_wd_fifo_depth_END      (12)
#define SOC_DMSS_ASI_RTL_INF1_rtl_b_fifo_depth_START     (16)
#define SOC_DMSS_ASI_RTL_INF1_rtl_b_fifo_depth_END       (20)
#define SOC_DMSS_ASI_RTL_INF1_rtl_ra_fifo_depth_START    (24)
#define SOC_DMSS_ASI_RTL_INF1_rtl_ra_fifo_depth_END      (28)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_RTL_INF2_UNION
 结构说明  : ASI_RTL_INF2 寄存器结构定义。地址偏移量:0x008+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: RTL配置信息2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_sec_rgn_num       : 6;  /* bit[0-5]  : ASI安全模块的regions个数：
                                                                 6'd0：无安全模块；
                                                                 6'd1：1regions；
                                                                 6'd2：2regions；
                                                                 6'd3：3regions；
                                                                 ……
                                                                 注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  reserved_0            : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  rtl_sec_chk_mid_width : 3;  /* bit[8-10] : ASI安全模块的MID检查位宽：
                                                                 0x0：无安全模块；
                                                                 0x1：只对MID[0]进行MID权限检查；
                                                                 0x2：只对MID[1:0]进行MID权限检查；
                                                                 ……
                                                                 0x5：只对MID[4:0]进行MID权限检查；
                                                                 注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  reserved_1            : 1;  /* bit[11]   : 保留。 */
        unsigned int  rtl_mpu_chk_mid_width : 3;  /* bit[12-14]: MPU模块的MID检查位宽：
                                                                 0x0：无安全模块；
                                                                 0x1：只对MID[0]进行MID权限检查；
                                                                 0x2：只对MID[1:0]进行MID权限检查；
                                                                 ……
                                                                 0x5：只对MID[4:0]进行MID权限检查；
                                                                 0x6：只对MID[5:0]进行MID权限检查；
                                                                 注意：当无MPU模块时，MPU功能相关寄存器无效。 */
        unsigned int  reserved_2            : 1;  /* bit[15]   : 保留。 */
        unsigned int  rtl_rd_fifo_depth     : 5;  /* bit[16-20]: 读数据FIFO配置深度：
                                                                 0x0： 1 read transfer；
                                                                 0x1： 2 read transfer；
                                                                 ……
                                                                 0x1F：32 read transfer。 */
        unsigned int  reserved_3            : 3;  /* bit[21-23]: 保留。 */
        unsigned int  rtl_subsec_rgn_num    : 4;  /* bit[24-27]: ASI安全模块中支持subrgn的regions个数：
                                                                 4'd0：无支持subrgn的regions；
                                                                 4'd1：1个支持subrgn的regions；
                                                                 4'd2：2个支持subrgn的regions；
                                                                 4'd3：3个支持subrgn的regions；
                                                                 ……
                                                                 注意：
                                                                 1.当无安全模块时，安全功能相关寄存器无效；
                                                                 2.支持subrgn的regions从region1开始计起，region0固定不支持subrgn。 */
        unsigned int  reserved_4            : 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_RTL_INF2_UNION;
#endif
#define SOC_DMSS_ASI_RTL_INF2_rtl_sec_rgn_num_START        (0)
#define SOC_DMSS_ASI_RTL_INF2_rtl_sec_rgn_num_END          (5)
#define SOC_DMSS_ASI_RTL_INF2_rtl_sec_chk_mid_width_START  (8)
#define SOC_DMSS_ASI_RTL_INF2_rtl_sec_chk_mid_width_END    (10)
#define SOC_DMSS_ASI_RTL_INF2_rtl_mpu_chk_mid_width_START  (12)
#define SOC_DMSS_ASI_RTL_INF2_rtl_mpu_chk_mid_width_END    (14)
#define SOC_DMSS_ASI_RTL_INF2_rtl_rd_fifo_depth_START      (16)
#define SOC_DMSS_ASI_RTL_INF2_rtl_rd_fifo_depth_END        (20)
#define SOC_DMSS_ASI_RTL_INF2_rtl_subsec_rgn_num_START     (24)
#define SOC_DMSS_ASI_RTL_INF2_rtl_subsec_rgn_num_END       (27)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STA_CKG_UNION
 结构说明  : ASI_STA_CKG 寄存器结构定义。地址偏移量:0x010+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 静态时钟门控
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gt_en_asi : 1;  /* bit[0]   : ASI静态时钟门控：
                                                    0：禁止；
                                                    1：使能，关闭当前ASI整体时钟。
                                                    注意：
                                                    只有确保当前ASI空闲且其对接的master设备彻底无访问时，才可以使能ASI静态时钟门控，否则会导致系统挂死。 */
        unsigned int  reserved  : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_STA_CKG_UNION;
#endif
#define SOC_DMSS_ASI_STA_CKG_gt_en_asi_START  (0)
#define SOC_DMSS_ASI_STA_CKG_gt_en_asi_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DYN_CKG_UNION
 结构说明  : ASI_DYN_CKG 寄存器结构定义。地址偏移量:0x014+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 动态时钟门控
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ckg_byp_asi     : 1;  /* bit[0]    : ASI模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_ra      : 1;  /* bit[1]    : RA模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_arcs    : 1;  /* bit[2]    : ARCS模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_awcs    : 1;  /* bit[3]    : AWCS模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_wd      : 1;  /* bit[4]    : WD模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_b       : 1;  /* bit[5]    : B模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_rd      : 1;  /* bit[6]    : RD模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_fkrd    : 1;  /* bit[7]    : FKRD模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_asi_sec : 1;  /* bit[8]    : ASI SEC模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。
                                                           注意：
                                                           当前ASI是否有安全模块，可以通过寄存器ASI_RTL_INF0.rtl_sec_rgn_num确认。 */
        unsigned int  ckg_byp_rdr_est : 1;  /* bit[9]    : RDR_EST模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。
                                                           注意：
                                                           当前ASI是否有保序模块，可以通过寄存器ASI_RTL_INF0.rtl_rdr_en确认。 */
        unsigned int  ckg_byp_rdr_buf : 1;  /* bit[10]   : RDR_BUF模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。
                                                           注意：
                                                           当前ASI是否有保序模块，可以通过寄存器ASI_RTL_INF0.rtl_rdr_en确认。 */
        unsigned int  ckg_byp_async   : 1;  /* bit[11]   : ASYNC模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。
                                                           注意：
                                                           当前ASI是否有异步桥，可以通过寄存器ASI_RTL_INF0.rtl_clk_mode确认。 */
        unsigned int  ckg_byp_ca      : 1;  /* bit[12]   : CA模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_qos_bwc : 1;  /* bit[13]   : QOS模块中的BWC模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_qos_lat : 1;  /* bit[14]   : QOS模块中的LAT模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_stat    : 1;  /* bit[15]   : STAT模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_wooo    : 1;  /* bit[16]   : WOOO模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  ckg_byp_detect  : 1;  /* bit[17]   : DETECT模块的动态时钟门控：
                                                           0：模块idle时，自动关闭时钟；
                                                           1：时钟常开。 */
        unsigned int  reserved        : 14; /* bit[18-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DYN_CKG_UNION;
#endif
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_asi_START      (0)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_asi_END        (0)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_ra_START       (1)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_ra_END         (1)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_arcs_START     (2)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_arcs_END       (2)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_awcs_START     (3)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_awcs_END       (3)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_wd_START       (4)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_wd_END         (4)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_b_START        (5)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_b_END          (5)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_rd_START       (6)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_rd_END         (6)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_fkrd_START     (7)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_fkrd_END       (7)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_asi_sec_START  (8)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_asi_sec_END    (8)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_rdr_est_START  (9)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_rdr_est_END    (9)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_rdr_buf_START  (10)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_rdr_buf_END    (10)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_async_START    (11)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_async_END      (11)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_ca_START       (12)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_ca_END         (12)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_qos_bwc_START  (13)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_qos_bwc_END    (13)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_qos_lat_START  (14)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_qos_lat_END    (14)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_stat_START     (15)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_stat_END       (15)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_wooo_START     (16)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_wooo_END       (16)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_detect_START   (17)
#define SOC_DMSS_ASI_DYN_CKG_ckg_byp_detect_END     (17)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_ASYNC_BRG_UNION
 结构说明  : ASI_ASYNC_BRG 寄存器结构定义。地址偏移量:0x018+0x800*(asi_base)，初值:0x0000007F，宽度:32
 寄存器说明: 异步桥时钟门控
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cactive_wl : 7;  /* bit[0-6] : 异步桥门控计数器水线设置：
                                                     n:当异步桥CACTIVE信号拉低并保持n个时钟周期后，关闭异步桥时钟
                                                     注意：
                                                     若异步桥动态时钟门控设置为常开，则此配置项无效。 */
        unsigned int  reserved   : 25; /* bit[7-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_ASYNC_BRG_UNION;
#endif
#define SOC_DMSS_ASI_ASYNC_BRG_cactive_wl_START  (0)
#define SOC_DMSS_ASI_ASYNC_BRG_cactive_wl_END    (6)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_ADDR_SHIFT_UNION
 结构说明  : ASI_ADDR_SHIFT 寄存器结构定义。地址偏移量:0x020+0x800*(asi_base)，初值:0x00000001，宽度:32
 寄存器说明: 地址移位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  addr_shift_mode : 2;  /* bit[0-1] : 地址移位模式：
                                                          0x0，0x3：不移位；
                                                          0x1：将总线访问的16~16.5GB地址，移为3.5~4GB地址；
                                                          0x2：将总线访问的32~32.5GB地址，移为3.5~4GB地址；
                                                          注意：
                                                          1.安全模块进行命令权限检查时，使用的地址是移位前的原始地址；
                                                          2.此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved        : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_ADDR_SHIFT_UNION;
#endif
#define SOC_DMSS_ASI_ADDR_SHIFT_addr_shift_mode_START  (0)
#define SOC_DMSS_ASI_ADDR_SHIFT_addr_shift_mode_END    (1)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_RATE_ADPT_UNION
 结构说明  : ASI_RATE_ADPT 寄存器结构定义。地址偏移量:0x030+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 写数据速率匹配
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ra_en       : 1;  /* bit[0]   : rate adapter使能：
                                                      0：禁止：不对写命令做任何限制；
                                                      1：使能：只有当接收到指定数目（ra_trsfr_wl+1）的写数据transfer个数或收到wlast之后，才允许其对应的写命令被发往后级模块。
                                                      说明：
                                                      若写数据是从低速时钟域经异步桥传递过来，则可通过rate adapter将间断的写数据进行一定的累积后，再向后级传送，以提高处理效率。
                                                      注意：
                                                      1.此寄存器不可以动态配置，否则可能导致系统挂死；
                                                      2.若使能防死锁anti_deadlock，且总线写命令会提前写数据到达，则建议同时使能ra_en，原理请参考ASI_ANTI_DEADLOCK.anti_deadlock的描述。 */
        unsigned int  reserved_0  : 3;  /* bit[1-3] : 保留。 */
        unsigned int  ra_trsfr_wl : 4;  /* bit[4-7] : rate adapter水线设置：
                                                      n：当接收到的写数据transfer个数达到n+1个时，其对应的写命令才可以被发往后级处理；
                                                      注意：
                                                      此寄存器可配置的最大值，为ra_fifo的深度减一。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_RATE_ADPT_UNION;
#endif
#define SOC_DMSS_ASI_RATE_ADPT_ra_en_START        (0)
#define SOC_DMSS_ASI_RATE_ADPT_ra_en_END          (0)
#define SOC_DMSS_ASI_RATE_ADPT_ra_trsfr_wl_START  (4)
#define SOC_DMSS_ASI_RATE_ADPT_ra_trsfr_wl_END    (7)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_ANTI_DEADLOCK_UNION
 结构说明  : ASI_ANTI_DEADLOCK 寄存器结构定义。地址偏移量:0x038+0x800*(asi_base)，初值:0x00000003，宽度:32
 寄存器说明: 防死锁控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  anti_deadlock    : 1;  /* bit[0]   : 防死锁使能：
                                                           0：禁止：不对写命令做任何限制；
                                                           1：使能：在各个DMI内，只有当此ASI的写数据的首个transfer到达时，其对应写命令才可以提起仲裁请求，以避免出现写死锁。
                                                           说明：
                                                           1.具体的死锁场景需要结合系统进行分析，防死锁功能默认使能。以下是典型的几种场景，供参考：
                                                            a.多个ASI同时向多个DMI或AMI发送写命令和写数据时，必然会出现死锁（蝴蝶锁）；
                                                            b.当同一个master通过多个ASI访问时（例如DMSS外部有1to2的桥），可能会出现死锁；
                                                            c.master仅通过一个ASI访问一个DMI或AMI时，可能不会引入死锁。
                                                           2.若使能防死锁，则对应ASI的wd_fifo深度必须足够大（需仿真确认），否则可能会引入写数据通路上的性能问题；
                                                           3.若使能防死锁，则建议同时使能ASI_RATE_ADPT.ra_en，这样可避免如下场景引入的性能问题：写命令到达DMI或AMI但其写数据迟迟未到达，由于防死锁的原因此时不能提起仲裁情况，则此时写命令会阻塞命令通道，从而对读命令造成影响。 */
        unsigned int  ex_anti_deadlock : 1;  /* bit[1]   : exclusive写命令的防死锁使能：
                                                           0：禁止：不对exclusive写命令做任何限制；
                                                           1：使能：exclusive写命令的防死锁使能。
                                                           说明：
                                                           写exclisive命令的死锁原理和普通命令是一样的，但其防死锁机制有所不同，因此设置单独的使能配置。
                                                           若配置anti_deadlock使能，则anti_deadlock_exclu也应该配置使能。 */
        unsigned int  reserved         : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_ANTI_DEADLOCK_UNION;
#endif
#define SOC_DMSS_ASI_ANTI_DEADLOCK_anti_deadlock_START     (0)
#define SOC_DMSS_ASI_ANTI_DEADLOCK_anti_deadlock_END       (0)
#define SOC_DMSS_ASI_ANTI_DEADLOCK_ex_anti_deadlock_START  (1)
#define SOC_DMSS_ASI_ANTI_DEADLOCK_ex_anti_deadlock_END    (1)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_RDR_CTRL_UNION
 结构说明  : ASI_RDR_CTRL 寄存器结构定义。地址偏移量:0x040+0x4*(rdr_filters)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读保序控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rdr_id_match : 15; /* bit[0-14] : 保序匹配ID：
                                                        与rdr_id_mask配合使用，当cmd_id & rdr_id_mask = rdr_id_match时，认为当前cmd_id被成功匹配。成功匹配的ID可参与或不参与（取决于rdr_id_mode）ID保序的监控。
                                                        注意：
                                                        1.实际可配置位宽取决于RTL的ID位宽配置，可通过GLB_RTL_INF寄存器中查询；
                                                        2.此寄存器不可以动态配置，否则可能导致系统挂死。 */
        unsigned int  reserved     : 1;  /* bit[15]   : 保留。 */
        unsigned int  rdr_id_mask  : 15; /* bit[16-30]: 保序ID掩码：
                                                        与rdr_id_match配合使用，当cmd_id & rdr_id_mask = rdr_id_match时，认为当前cmd_id被成功匹配。成功匹配的ID可参与或不参与（取决于rdr_id_mode）ID保序的监控。
                                                        注意：
                                                        1.实际可配置位宽取决于RTL的ID位宽配置，可通过GLB_RTL_INF寄存器中查询；
                                                        2.此寄存器不可以动态配置，否则可能导致系统挂死。 */
        unsigned int  rdr_id_mode  : 1;  /* bit[31]   : 保序ID模式：
                                                        0:匹配成功的ID，参与保序监控；
                                                        1:匹配失败的ID，参与保序监控。
                                                        注意：多组保序控制寄存器，对于匹配结果参与ID保序的结果是“或”的关系，只要在一组寄存判断结果是参加ID保序监控，则最终此ID参加ID保序监控。 */
    } reg;
} SOC_DMSS_ASI_RDR_CTRL_UNION;
#endif
#define SOC_DMSS_ASI_RDR_CTRL_rdr_id_match_START  (0)
#define SOC_DMSS_ASI_RDR_CTRL_rdr_id_match_END    (14)
#define SOC_DMSS_ASI_RDR_CTRL_rdr_id_mask_START   (16)
#define SOC_DMSS_ASI_RDR_CTRL_rdr_id_mask_END     (30)
#define SOC_DMSS_ASI_RDR_CTRL_rdr_id_mode_START   (31)
#define SOC_DMSS_ASI_RDR_CTRL_rdr_id_mode_END     (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_CTRL_UNION
 结构说明  : ASI_QOS_CTRL 寄存器结构定义。地址偏移量:0x100+0x800*(asi_base)，初值:0x000002C0，宽度:32
 寄存器说明: 命令优先级控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pri_push_en       : 1;  /* bit[0]    : 端口优先级传递使能：
                                                             0：禁止；
                                                             1：使能。
                                                             注意：
                                                             优先级传递使能后，将命令流水线上以及外部送入的多个命令中的最高优先级，传递给当前正参与端口仲裁的命令，以帮助其快速赢得仲裁。是否改变被传递命令的自身优先级取决于配置项arb_pri_use。 */
        unsigned int  push_qos_en       : 1;  /* bit[1]    : Pushed优先级传递使能：
                                                             0：禁止；
                                                             1：使能。
                                                             注意：
                                                             1.NOC总线的Pressure信号，可在命令总线反压时传递优先级，参与DMSS内部的优先级传递；
                                                             2.当pri_push_en==1时，此寄存器有效。 */
        unsigned int  hurry_qos_en      : 1;  /* bit[2]    : Hurry优先级传递使能：
                                                             0：禁止；
                                                             1：使能。
                                                             注意：
                                                             1.NOC总线的Hurry信号，可在任何时刻传递优先级，参与DMSS内部的优先级传递；
                                                             2.当pri_push_en==1时，此寄存器有效。 */
        unsigned int  arb_pri_use       : 1;  /* bit[3]    : 使用仲裁优先级作为命令自身的优先级：
                                                             0：禁止：命令自身优先级只来自于AxQOS信号的映射，或regulator中配置的优先级，或读命令基于latency得到的优先级；
                                                             1：使能：除了上述来源之外，命令自身优先级还可以来自优先级传递和优先级自适应等方式得到的优先级。 */
        unsigned int  ami1_rcmd_dis     : 1;  /* bit[4]    : ASI模块读命令禁止访问AMI1：
                                                             0：读命令可以访问AMI1；
                                                             1：读命令禁止访问AMI1，全部路由至DMI。
                                                             注意：
                                                             1.当AMI1使能且与对应ASI连接时，此寄存器有效。 */
        unsigned int  ami1_wcmd_dis     : 1;  /* bit[5]    : ASI模块写命令禁止访问AMI1：
                                                             0：写命令可以访问AMI1；
                                                             1：写命令禁止访问AMI1，全部路由至DMI。
                                                             注意：
                                                             1.当AMI1使能且与对应ASI连接时，此寄存器有效。 */
        unsigned int  brg_push_en       : 1;  /* bit[6]    : 异步桥内缓存命令的pushed优先级传递使能：
                                                             0：禁止；
                                                             1：使能。
                                                             注意：
                                                             当异步桥存在时，且pri_push_en==1时，此寄存器有效。当前ASI是否有异步桥，可以通过寄存器ASI_RTL_INF0.rtl_clk_mode确认。 */
        unsigned int  rdr_head_byp_en   : 1;  /* bit[7]    : REORDER模块head bypass处理使能：
                                                             0：禁止；
                                                             1：使能。
                                                             注意：
                                                             1.当REORDER使能时，此寄存器有效。
                                                             2.此寄存器必须静态配置。 */
        unsigned int  asi_dis           : 1;  /* bit[8]    : 命令接收禁止：
                                                             0：正常接收命令；
                                                             1：禁止接收命令（反压AXI总线的读写命令，可用于清空当前ASI内全部命令）。 */
        unsigned int  wooo_head_byp_en  : 1;  /* bit[9]    : WOOO模块head bypass处理使能：
                                                             0：禁止；
                                                             1：使能。
                                                             注意：
                                                             1.当WOOO使能时，此寄存器有效;
                                                             2.此寄存器必须静态配置。 */
        unsigned int  ami0_rcmd_dis     : 1;  /* bit[10]   : ASI模块读命令禁止访问AMI0：
                                                             0：读命令可以访问AMI0；
                                                             1：读命令禁止访问AMI0，全部路由至DMI。
                                                             注意：
                                                             1.当AMI0使能且与对应ASI连接时，此寄存器有效。 */
        unsigned int  ami0_wcmd_dis     : 1;  /* bit[11]   : ASI模块写命令禁止访问AMI0：
                                                             0：写命令可以访问AMI0；
                                                             1：写命令禁止访问AMI0，全部路由至DMI。
                                                             注意：
                                                             1.当AMI0使能且与对应ASI连接时，此寄存器有效。 */
        unsigned int  reserved_0        : 2;  /* bit[12-13]: 保留。 */
        unsigned int  wooo_rlast_arb    : 1;  /* bit[14]   : WOOO模块收齐burst后才仲裁输出该burst使能：
                                                             0：收齐burst后才仲裁输出该burst；
                                                             1：收到burst即可仲裁输出该burst。
                                                             注意：
                                                             此寄存器必须静态配置。 */
        unsigned int  wooo_free_disable : 1;  /* bit[15]   : WOOO模块相同ID提前参与仲裁功能禁止：
                                                             0：使能；
                                                             1：禁止。
                                                             注意：
                                                             此寄存器必须静态配置。 */
        unsigned int  dcq_pri_allow_en  : 1;  /* bit[16]   : 通过DCQ得到的允许优先级的流控使能：
                                                             0：忽略DCQ的优先级水线；
                                                             1：依据DCQ的允许优先级对低优先级命令进行流量控制（基于limter2/3）。 */
        unsigned int  mst_pri_allow_en  : 1;  /* bit[17]   : 通过Master得到的允许优先级的流控使能：
                                                             0：忽略MST的优先级水线；
                                                             1：依据MST的允许优先级对低优先级命令进行流量控制（基于limter2/3）。 */
        unsigned int  reserved_1        : 2;  /* bit[18-19]: 保留。 */
        unsigned int  adpt_zoom         : 2;  /* bit[20-21]: 命令优先级自适应功能的放大倍数：
                                                             0：64倍；
                                                             1：256倍；
                                                             2：1024倍；
                                                             3：4096倍；
                                                             举例：
                                                             某命令映射出的adpt是3，adpt_zoom配置为2，则每当命令被阻塞3*1024=3072个时钟周期后，优先级提升一级。 */
        unsigned int  sc_cmo_len        : 1;  /* bit[22]   : 发给SystemCache的ACMPU-CMO读命令，是否将其数据长度强制改为BL1：
                                                             0：禁止；
                                                             1：使能。 */
        unsigned int  reserved_2        : 9;  /* bit[23-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_CTRL_UNION;
#endif
#define SOC_DMSS_ASI_QOS_CTRL_pri_push_en_START        (0)
#define SOC_DMSS_ASI_QOS_CTRL_pri_push_en_END          (0)
#define SOC_DMSS_ASI_QOS_CTRL_push_qos_en_START        (1)
#define SOC_DMSS_ASI_QOS_CTRL_push_qos_en_END          (1)
#define SOC_DMSS_ASI_QOS_CTRL_hurry_qos_en_START       (2)
#define SOC_DMSS_ASI_QOS_CTRL_hurry_qos_en_END         (2)
#define SOC_DMSS_ASI_QOS_CTRL_arb_pri_use_START        (3)
#define SOC_DMSS_ASI_QOS_CTRL_arb_pri_use_END          (3)
#define SOC_DMSS_ASI_QOS_CTRL_ami1_rcmd_dis_START      (4)
#define SOC_DMSS_ASI_QOS_CTRL_ami1_rcmd_dis_END        (4)
#define SOC_DMSS_ASI_QOS_CTRL_ami1_wcmd_dis_START      (5)
#define SOC_DMSS_ASI_QOS_CTRL_ami1_wcmd_dis_END        (5)
#define SOC_DMSS_ASI_QOS_CTRL_brg_push_en_START        (6)
#define SOC_DMSS_ASI_QOS_CTRL_brg_push_en_END          (6)
#define SOC_DMSS_ASI_QOS_CTRL_rdr_head_byp_en_START    (7)
#define SOC_DMSS_ASI_QOS_CTRL_rdr_head_byp_en_END      (7)
#define SOC_DMSS_ASI_QOS_CTRL_asi_dis_START            (8)
#define SOC_DMSS_ASI_QOS_CTRL_asi_dis_END              (8)
#define SOC_DMSS_ASI_QOS_CTRL_wooo_head_byp_en_START   (9)
#define SOC_DMSS_ASI_QOS_CTRL_wooo_head_byp_en_END     (9)
#define SOC_DMSS_ASI_QOS_CTRL_ami0_rcmd_dis_START      (10)
#define SOC_DMSS_ASI_QOS_CTRL_ami0_rcmd_dis_END        (10)
#define SOC_DMSS_ASI_QOS_CTRL_ami0_wcmd_dis_START      (11)
#define SOC_DMSS_ASI_QOS_CTRL_ami0_wcmd_dis_END        (11)
#define SOC_DMSS_ASI_QOS_CTRL_wooo_rlast_arb_START     (14)
#define SOC_DMSS_ASI_QOS_CTRL_wooo_rlast_arb_END       (14)
#define SOC_DMSS_ASI_QOS_CTRL_wooo_free_disable_START  (15)
#define SOC_DMSS_ASI_QOS_CTRL_wooo_free_disable_END    (15)
#define SOC_DMSS_ASI_QOS_CTRL_dcq_pri_allow_en_START   (16)
#define SOC_DMSS_ASI_QOS_CTRL_dcq_pri_allow_en_END     (16)
#define SOC_DMSS_ASI_QOS_CTRL_mst_pri_allow_en_START   (17)
#define SOC_DMSS_ASI_QOS_CTRL_mst_pri_allow_en_END     (17)
#define SOC_DMSS_ASI_QOS_CTRL_adpt_zoom_START          (20)
#define SOC_DMSS_ASI_QOS_CTRL_adpt_zoom_END            (21)
#define SOC_DMSS_ASI_QOS_CTRL_sc_cmo_len_START         (22)
#define SOC_DMSS_ASI_QOS_CTRL_sc_cmo_len_END           (22)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_WRPRI_UNION
 结构说明  : ASI_QOS_WRPRI 寄存器结构定义。地址偏移量:0x104+0x800*(asi_base)，初值:0x76543210，宽度:32
 寄存器说明: 写命令优先级映射表
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_pri0  : 3;  /* bit[0-2]  : 当awqos[2:0]==0时，写命令的优先级配置值：
                                                    0x0：最低优先级；
                                                    ……
                                                    0x7：最高优先级。 */
        unsigned int  reserved_0: 1;  /* bit[3]    : 保留。 */
        unsigned int  wr_pri1  : 3;  /* bit[4-6]  : 类同wr_pri0的配置。 */
        unsigned int  reserved_1: 1;  /* bit[7]    : 保留。 */
        unsigned int  wr_pri2  : 3;  /* bit[8-10] : 类同wr_pri0的配置。 */
        unsigned int  reserved_2: 1;  /* bit[11]   : 保留。 */
        unsigned int  wr_pri3  : 3;  /* bit[12-14]: 类同wr_pri0的配置。 */
        unsigned int  reserved_3: 1;  /* bit[15]   : 保留。 */
        unsigned int  wr_pri4  : 3;  /* bit[16-18]: 类同wr_pri0的配置。 */
        unsigned int  reserved_4: 1;  /* bit[19]   : 保留。 */
        unsigned int  wr_pri5  : 3;  /* bit[20-22]: 类同wr_pri0的配置。 */
        unsigned int  reserved_5: 1;  /* bit[23]   : 保留。 */
        unsigned int  wr_pri6  : 3;  /* bit[24-26]: 类同wr_pri0的配置。 */
        unsigned int  reserved_6: 1;  /* bit[27]   : 保留。 */
        unsigned int  wr_pri7  : 3;  /* bit[28-30]: 类同wr_pri0的配置。 */
        unsigned int  reserved_7: 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_WRPRI_UNION;
#endif
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri0_START   (0)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri0_END     (2)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri1_START   (4)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri1_END     (6)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri2_START   (8)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri2_END     (10)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri3_START   (12)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri3_END     (14)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri4_START   (16)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri4_END     (18)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri5_START   (20)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri5_END     (22)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri6_START   (24)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri6_END     (26)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri7_START   (28)
#define SOC_DMSS_ASI_QOS_WRPRI_wr_pri7_END     (30)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_RDPRI_UNION
 结构说明  : ASI_QOS_RDPRI 寄存器结构定义。地址偏移量:0x108+0x800*(asi_base)，初值:0x76543210，宽度:32
 寄存器说明: 读命令优先级映射表
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_pri0  : 3;  /* bit[0-2]  : 当arqos[2:0]==0时，读命令的优先级配置值：
                                                    0x0：最低优先级；
                                                    ……
                                                    0x7：最高优先级。 */
        unsigned int  reserved_0: 1;  /* bit[3]    : 保留。 */
        unsigned int  rd_pri1  : 3;  /* bit[4-6]  : 类同rd_pri0的配置。 */
        unsigned int  reserved_1: 1;  /* bit[7]    : 保留。 */
        unsigned int  rd_pri2  : 3;  /* bit[8-10] : 类同rd_pri0的配置。 */
        unsigned int  reserved_2: 1;  /* bit[11]   : 保留。 */
        unsigned int  rd_pri3  : 3;  /* bit[12-14]: 类同rd_pri0的配置。 */
        unsigned int  reserved_3: 1;  /* bit[15]   : 保留。 */
        unsigned int  rd_pri4  : 3;  /* bit[16-18]: 类同rd_pri0的配置。 */
        unsigned int  reserved_4: 1;  /* bit[19]   : 保留。 */
        unsigned int  rd_pri5  : 3;  /* bit[20-22]: 类同rd_pri0的配置。 */
        unsigned int  reserved_5: 1;  /* bit[23]   : 保留。 */
        unsigned int  rd_pri6  : 3;  /* bit[24-26]: 类同rd_pri0的配置。 */
        unsigned int  reserved_6: 1;  /* bit[27]   : 保留。 */
        unsigned int  rd_pri7  : 3;  /* bit[28-30]: 类同rd_pri0的配置。 */
        unsigned int  reserved_7: 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_RDPRI_UNION;
#endif
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri0_START   (0)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri0_END     (2)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri1_START   (4)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri1_END     (6)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri2_START   (8)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri2_END     (10)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri3_START   (12)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri3_END     (14)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri4_START   (16)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri4_END     (18)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri5_START   (20)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri5_END     (22)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri6_START   (24)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri6_END     (26)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri7_START   (28)
#define SOC_DMSS_ASI_QOS_RDPRI_rd_pri7_END     (30)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_ADPT_UNION
 结构说明  : ASI_QOS_ADPT 寄存器结构定义。地址偏移量:0x10C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 命令优先级自适应周期映射表
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_adpt_low  : 4;  /* bit[0-3]  : 读命令优先级自适应周期配置（低档位）：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考寄存器ASI_QOS_CTRL.adpt_zoom)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  rd_adpt_lvl  : 3;  /* bit[4-6]  : 读命令优先级自适应周期配置水线：
                                                        0x0～0x6：命令优先级高于此水线时，使用高档位配置，否则使用低档位配置；
                                                        0x7：reserved。
                                                        举例：
                                                        0x3：优先级4~5的读命令使用rd_pri_adpt_high，优先级0～3的读命令使用rd_pri_adpt_low。 */
        unsigned int  reserved_0   : 1;  /* bit[7]    : 保留。 */
        unsigned int  rd_adpt_high : 4;  /* bit[8-11] : 读命令优先级自适应周期配置（高档位）：
                                                        同rd_pri_adpt_low的配置。 */
        unsigned int  reserved_1   : 4;  /* bit[12-15]: 保留。 */
        unsigned int  wr_adpt_low  : 4;  /* bit[16-19]: 写命令优先级自适应周期配置（低档位）：
                                                        同rd_pri_adpt_low的配置。 */
        unsigned int  wr_adpt_lvl  : 3;  /* bit[20-22]: 写命令优先级自适应周期配置水线：
                                                        同rd_pri_adpt_lvl的配置。 */
        unsigned int  reserved_2   : 1;  /* bit[23]   : 保留。 */
        unsigned int  wr_adpt_high : 4;  /* bit[24-27]: 写命令优先级自适应周期配置（高档位）：
                                                        同rd_pri_adpt_low的配置。 */
        unsigned int  reserved_3   : 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_ADPT_UNION;
#endif
#define SOC_DMSS_ASI_QOS_ADPT_rd_adpt_low_START   (0)
#define SOC_DMSS_ASI_QOS_ADPT_rd_adpt_low_END     (3)
#define SOC_DMSS_ASI_QOS_ADPT_rd_adpt_lvl_START   (4)
#define SOC_DMSS_ASI_QOS_ADPT_rd_adpt_lvl_END     (6)
#define SOC_DMSS_ASI_QOS_ADPT_rd_adpt_high_START  (8)
#define SOC_DMSS_ASI_QOS_ADPT_rd_adpt_high_END    (11)
#define SOC_DMSS_ASI_QOS_ADPT_wr_adpt_low_START   (16)
#define SOC_DMSS_ASI_QOS_ADPT_wr_adpt_low_END     (19)
#define SOC_DMSS_ASI_QOS_ADPT_wr_adpt_lvl_START   (20)
#define SOC_DMSS_ASI_QOS_ADPT_wr_adpt_lvl_END     (22)
#define SOC_DMSS_ASI_QOS_ADPT_wr_adpt_high_START  (24)
#define SOC_DMSS_ASI_QOS_ADPT_wr_adpt_high_END    (27)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_LATENCY_UNION
 结构说明  : ASI_QOS_LATENCY 寄存器结构定义。地址偏移量:0x110+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读命令的latency控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lat_target       : 12; /* bit[0-11] : 目标平均latency，单位为clk cycle。
                                                            说明：
                                                            将通过优先级调整和限制所有ASI的低优先级命令流量的方式，使得平均读latency逼近目标平均latency。 */
        unsigned int  lat_factor       : 3;  /* bit[12-14]: 优先级调整速率：
                                                            0~7：数值越大，优先级调整速率越缓慢。相邻两档配置值相比，同等条件下（相同读outstanding）优先级调整速率相差一倍。
                                                            说明：
                                                            当latency超过（未达到）设定的目标平均latency时，读命令优先级会逐级调高（调低），lat_factor决定了优先级连续的逐级调高（调低）时的速率。
                                                            优先级调整速率除了和此配置项有关外，还受到当前读outstanding的影响：outstanding越小，优先级调整速率越缓慢。 */
        unsigned int  reserved_0       : 1;  /* bit[15]   : 保留。 */
        unsigned int  lat_pri_min      : 3;  /* bit[16-18]: 优先级的调整下限。 */
        unsigned int  reserved_1       : 1;  /* bit[19]   : 保留。 */
        unsigned int  lat_pri_max      : 3;  /* bit[20-22]: 优先级的调整上限。 */
        unsigned int  reserved_2       : 1;  /* bit[23]   : 保留。 */
        unsigned int  lat_flux_req_dly : 4;  /* bit[24-27]: 低优先级命令限流的功能的触发延迟周期：
                                                            n：(n+1)*(2^(lat_factor+2))个时钟周期
                                                            举例：
                                                            若lat_factor配为3，n配为5，则触发延迟周期为（5+1）*（2^(3+2)）=196。
                                                            说明：
                                                            若读命令优先级已被调整到上限值，且延迟周期后还不能达到目标latency，则触发低优先级命令的限流功能。 */
        unsigned int  lat_flux_req_en  : 1;  /* bit[28]   : 限制所有ASI端口的低优先级命令流量的功能使能：
                                                            0：禁止；
                                                            1：使能。
                                                            说明：
                                                            当读命令优先级被调整到上限值时，若还不能达到目标latency，则可进一步限制所有ASI的低优先级命令流量。
                                                            流量限制功能复用了外部master的流控功能，需要配置相关的寄存器GLB_MST_FLUX和ASI_QOS_LMTR2/3。
                                                            注意：
                                                            1.需要在RTL的define配置文件中，将当前ASI分配到某组外部master流控功能上，否则此功能无效；
                                                            2.此寄存器只能静态配置。 */
        unsigned int  lat_quiesce      : 1;  /* bit[29]   : Sets the mode for bandwidth regulation:
                                                            0：Normal mode.The QoS value is stable when the master is idle.
                                                            1：Quiesce High mode.The QoS value tends to the maximum when the master is idle。 */
        unsigned int  lat_mode         : 1;  /* bit[30]   : Configures the mode of the QoS value regulator for read transactions:
                                                            0：Latency mode.
                                                            1：Period mode, for bandwidth regulation。 */
        unsigned int  lat_en           : 1;  /* bit[31]   : latency控制使能：
                                                            0：禁止；
                                                            1：使能。
                                                            说明：
                                                            使能后，可统计读的命令平均latency，并通过自动调整命令的优先级（在上下限范围内调整，调整速率可配置）和限制所有ASI端口的低优先级命令流量等方式，使得读命令平均latency尽可能的逼近目标latency。
                                                            注意：
                                                            1.若使能此功能，则读命令的Regulator和优先级映射功能无效；
                                                            (相关寄存器为ASI_QOS_RGLR0/ASI_QOS_RGLR0_PRI/ASI_QOS_RDPRI)
                                                            2.此寄存器只能静态配置。 */
    } reg;
} SOC_DMSS_ASI_QOS_LATENCY_UNION;
#endif
#define SOC_DMSS_ASI_QOS_LATENCY_lat_target_START        (0)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_target_END          (11)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_factor_START        (12)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_factor_END          (14)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_pri_min_START       (16)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_pri_min_END         (18)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_pri_max_START       (20)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_pri_max_END         (22)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_flux_req_dly_START  (24)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_flux_req_dly_END    (27)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_flux_req_en_START   (28)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_flux_req_en_END     (28)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_quiesce_START       (29)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_quiesce_END         (29)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_mode_START          (30)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_mode_END            (30)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_en_START            (31)
#define SOC_DMSS_ASI_QOS_LATENCY_lat_en_END              (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_RGLR0_UNION
 结构说明  : ASI_QOS_RGLR0 寄存器结构定义。地址偏移量:0x120+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽优先级控制0
            （可单独控制读命令优先级，或同时控制读写命令优先级）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rglr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16。
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/设定带宽水线(B/Hz)=滑窗宽度（cycle）。即在配置的设定带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的期望带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           注意：regulator使能时，此处不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  rglr_bandwidth  : 13; /* bit[16-28]: 设定期望带宽：
                                                           配置值=设定期望带宽(B/Hz)*256=期望带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的设定带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           乘256的目的是保留。。设定期望带宽的高8位小数。 */
        unsigned int  reserved_1      : 1;  /* bit[29]   : 保留。 */
        unsigned int  rglr_type       : 1;  /* bit[30]   : 带宽优先级控制的命令类型：
                                                           0：只控制读命令优先级；
                                                           1：同时控制读写命令优先级。
                                                           注意：
                                                           当配置为“同时控制读写命令优先级”时，ASI_QOS_RGLR1将被自动禁止使用。 */
        unsigned int  rglr_en         : 1;  /* bit[31]   : regulator使能：
                                                           0：禁止；
                                                           1：使能。
                                                           注意：
                                                           若使能Regulator功能，则优先级映射功能无效
                                                           (相关寄存器为ASI_QOS_WRPRI/ASI_QOS_RDPRI)。 */
    } reg;
} SOC_DMSS_ASI_QOS_RGLR0_UNION;
#endif
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_type_START        (30)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_type_END          (30)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_en_START          (31)
#define SOC_DMSS_ASI_QOS_RGLR0_rglr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_RGLR0_PRI_UNION
 结构说明  : ASI_QOS_RGLR0_PRI 寄存器结构定义。地址偏移量:0x124+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽配置优先级0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rglr_pri0 : 3;  /* bit[0-2]  : 当带宽未达到ASI_QOS_RGLR0的配置带宽时，命令的优先级配置值：
                                                     0x0：最低优先级；
                                                     ……
                                                     0x7：最高优先级。
                                                     注意：
                                                     若rglr_type配置为“只控制读命令优先级”，则只改变读命令优先级；若配置为“同时控制读写命令优先级”，则同时改变读和写命令优先级。 */
        unsigned int  reserved_0: 5;  /* bit[3-7]  : 保留。 */
        unsigned int  rglr_pri1 : 3;  /* bit[8-10] : 当带宽达到ASI_QOS_RGLR0的配置带宽时，命令的优先级配置值：
                                                     0x0：最低优先级；
                                                     ……
                                                     0x7：最高优先级。
                                                     注意：
                                                     若rglr_type配置为“只控制读命令优先级”，则只改变读命令优先级；若配置为“同时控制读写命令优先级”，则同时改变读和写命令优先级。 */
        unsigned int  reserved_1: 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_RGLR0_PRI_UNION;
#endif
#define SOC_DMSS_ASI_QOS_RGLR0_PRI_rglr_pri0_START  (0)
#define SOC_DMSS_ASI_QOS_RGLR0_PRI_rglr_pri0_END    (2)
#define SOC_DMSS_ASI_QOS_RGLR0_PRI_rglr_pri1_START  (8)
#define SOC_DMSS_ASI_QOS_RGLR0_PRI_rglr_pri1_END    (10)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_RGLR1_UNION
 结构说明  : ASI_QOS_RGLR1 寄存器结构定义。地址偏移量:0x128+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽优先级控制1
            （只可单独控制写命令优先级）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rglr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16。
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/设定带宽水线(B/Hz)=滑窗宽度（cycle）。即在配置的设定带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的期望带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           注意：regulator使能时，此处不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  rglr_bandwidth  : 13; /* bit[16-28]: 设定期望带宽：
                                                           配置值=设定期望带宽(B/Hz)*256=期望带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的设定带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           乘256的目的是保留。。设定期望带宽的高8位小数。 */
        unsigned int  reserved_1      : 2;  /* bit[29-30]: 保留。 */
        unsigned int  rglr_en         : 1;  /* bit[31]   : regulator使能：
                                                           0：禁止；
                                                           1：使能。
                                                           注意：
                                                           1.ASI_QOS_RGLR1只能控制写带宽；
                                                           2.当ASI_QOS_RGLR0配置为“同时控制读写命令带宽”时，ASI_QOS_RGLR1将被自动禁止使用。
                                                           3.注意：
                                                           若使能Regulator功能，则优先级映射功能无效
                                                           (相关寄存器为ASI_QOS_WRPRI)。 */
    } reg;
} SOC_DMSS_ASI_QOS_RGLR1_UNION;
#endif
#define SOC_DMSS_ASI_QOS_RGLR1_rglr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_RGLR1_rglr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_RGLR1_rglr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_RGLR1_rglr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_RGLR1_rglr_en_START          (31)
#define SOC_DMSS_ASI_QOS_RGLR1_rglr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_RGLR1_PRI_UNION
 结构说明  : ASI_QOS_RGLR1_PRI 寄存器结构定义。地址偏移量:0x12C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽配置优先级1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rglr_pri0 : 3;  /* bit[0-2]  : 当带宽未达到ASI_QOS_RGLR0的配置带宽时，写命令的优先级配置值：
                                                     0x0：最低优先级；
                                                     ……
                                                     0x7：最高优先级。 */
        unsigned int  reserved_0: 5;  /* bit[3-7]  : 保留。 */
        unsigned int  rglr_pri1 : 3;  /* bit[8-10] : 当带宽达到ASI_QOS_RGLR0的配置带宽时，写命令的优先级配置值：
                                                     0x0：最低优先级；
                                                     ……
                                                     0x7：最高优先级。 */
        unsigned int  reserved_1: 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_RGLR1_PRI_UNION;
#endif
#define SOC_DMSS_ASI_QOS_RGLR1_PRI_rglr_pri0_START  (0)
#define SOC_DMSS_ASI_QOS_RGLR1_PRI_rglr_pri0_END    (2)
#define SOC_DMSS_ASI_QOS_RGLR1_PRI_rglr_pri1_START  (8)
#define SOC_DMSS_ASI_QOS_RGLR1_PRI_rglr_pri1_END    (10)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_LMTR0_UNION
 结构说明  : ASI_QOS_LMTR0 寄存器结构定义。地址偏移量:0x140+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽限流控制0
            （可单独控制读带宽，或同时控制读写总带宽）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16；
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                           注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                           配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           1.乘256的目的是保留允许平均带宽的高8位小数；
                                                           2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                           3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1      : 1;  /* bit[29]   : 保留。 */
        unsigned int  lmtr_type       : 1;  /* bit[30]   : 带宽限流控制的命令类型：
                                                           0：只控制读带宽；
                                                           1：同时控制读写总带宽。 */
        unsigned int  lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                           0：禁止；
                                                           1：使能。 */
    } reg;
} SOC_DMSS_ASI_QOS_LMTR0_UNION;
#endif
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_type_START        (30)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_type_END          (30)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_en_START          (31)
#define SOC_DMSS_ASI_QOS_LMTR0_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_LMTR1_UNION
 结构说明  : ASI_QOS_LMTR1 寄存器结构定义。地址偏移量:0x144+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽限流控制1
            （只可单独控制写带宽）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16。
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                           注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                           配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           1.乘256的目的是保留允许平均带宽的高8位小数；
                                                           2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                           3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1      : 2;  /* bit[29-30]: 保留。 */
        unsigned int  lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                           0：禁止；
                                                           1：使能。
                                                           注意：
                                                           ASI_QOS_LMTR1只能控制写带宽。 */
    } reg;
} SOC_DMSS_ASI_QOS_LMTR1_UNION;
#endif
#define SOC_DMSS_ASI_QOS_LMTR1_lmtr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_LMTR1_lmtr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_LMTR1_lmtr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_LMTR1_lmtr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_LMTR1_lmtr_en_START          (31)
#define SOC_DMSS_ASI_QOS_LMTR1_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_LMTR2_UNION
 结构说明  : ASI_QOS_LMTR2 寄存器结构定义。地址偏移量:0x148+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽限流控制2
            （只可同时控制读写总带宽）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16。
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                           注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                           配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           1.乘256的目的是保留允许平均带宽的高8位小数；
                                                           2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                           3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1      : 2;  /* bit[29-30]: 保留。 */
        unsigned int  lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                           0：禁止；
                                                           1：使能。
                                                           注意：
                                                           1.ASI_QOS_LMTR2只能控制读写总带宽；
                                                           2.需与GLB_DCQ_PRILVL0/1等寄存器配合使用，实现控制低优先级命令流量的特性。 */
    } reg;
} SOC_DMSS_ASI_QOS_LMTR2_UNION;
#endif
#define SOC_DMSS_ASI_QOS_LMTR2_lmtr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_LMTR2_lmtr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_LMTR2_lmtr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_LMTR2_lmtr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_LMTR2_lmtr_en_START          (31)
#define SOC_DMSS_ASI_QOS_LMTR2_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_LMTR3_UNION
 结构说明  : ASI_QOS_LMTR3 寄存器结构定义。地址偏移量:0x14C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽限流控制3
            （只可单独控制写带宽）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16。
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                           注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                           配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           1.乘256的目的是保留允许平均带宽的高8位小数；
                                                           2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                           3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1      : 2;  /* bit[29-30]: 保留。 */
        unsigned int  lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                           0：禁止；
                                                           1：使能。
                                                           注意：
                                                           1.ASI_QOS_LMTR3只能控制写带宽；
                                                           2.需与GLB_DCQ_PRILVL0/1等寄存器配合使用，实现控制低优先级命令流量的特性。 */
    } reg;
} SOC_DMSS_ASI_QOS_LMTR3_UNION;
#endif
#define SOC_DMSS_ASI_QOS_LMTR3_lmtr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_LMTR3_lmtr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_LMTR3_lmtr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_LMTR3_lmtr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_LMTR3_lmtr_en_START          (31)
#define SOC_DMSS_ASI_QOS_LMTR3_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_LMTR4_UNION
 结构说明  : ASI_QOS_LMTR4 寄存器结构定义。地址偏移量:0x150+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽限流控制4
            （只可控制SC方向读写总流量）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lmtr_saturation : 14; /* bit[0-13] : 带宽容限：
                                                           配置值=带宽容限(B)/16。
                                                           说明：
                                                           可以将本带宽控制方式等效成基于滑动窗口的流量控制，带宽容限决定了滑窗宽度以及滑窗内的允许流量：带宽容限(B)/允许平均带宽(B/Hz)=滑窗宽度（cycle）。即在配置的允许平均带宽固定的情况下，带宽容限越大，滑窗越宽，对控制前的瞬时带宽变化越不敏感，长时间控制后的平均带宽也越接近配置的允许平均带宽，但控制后的瞬时带宽的变化也可能会越剧烈。
                                                           例如，当允许带宽为理论带宽的1/3时（为便于说明，假设一个命令对应一拍数据，数据总线位宽16B。带宽控制实际是控制命令而非数据）：若带宽容限配置值为1，则任意连续的3拍内最多允许1拍数据通过；若带宽容限配置值为2，则任意连续的6拍内最多允许2拍数据通过，而这2拍数据就可能连续通过；以此类推。（由于硬件延迟等因素，建议带宽容限的有效配置值至少为2）
                                                           注意：带宽限流使能时，不可以配置为0。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  lmtr_bandwidth  : 13; /* bit[16-28]: 允许平均带宽：
                                                           配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。
                                                           例如：工作频率为400MHz，当前端口的允许带宽为1400MB/s，则带宽配置值为1400/400*256=896=0x380
                                                           说明：
                                                           1.乘256的目的是保留允许平均带宽的高8位小数；
                                                           2.控制后的瞬时带宽可能会超过允许带宽，超过的范围取决于带宽容限的配置；
                                                           3.控制后的长时间平均带宽小于等于允许平均带宽。 */
        unsigned int  reserved_1      : 2;  /* bit[29-30]: 保留。 */
        unsigned int  lmtr_en         : 1;  /* bit[31]   : limiter使能：
                                                           0：禁止；
                                                           1：使能。
                                                           注意：
                                                           ASI_QOS_LMTR4只能控制SC方向读写总带宽。 */
    } reg;
} SOC_DMSS_ASI_QOS_LMTR4_UNION;
#endif
#define SOC_DMSS_ASI_QOS_LMTR4_lmtr_saturation_START  (0)
#define SOC_DMSS_ASI_QOS_LMTR4_lmtr_saturation_END    (13)
#define SOC_DMSS_ASI_QOS_LMTR4_lmtr_bandwidth_START   (16)
#define SOC_DMSS_ASI_QOS_LMTR4_lmtr_bandwidth_END     (28)
#define SOC_DMSS_ASI_QOS_LMTR4_lmtr_en_START          (31)
#define SOC_DMSS_ASI_QOS_LMTR4_lmtr_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_QOS_RDOSTD_UNION
 结构说明  : ASI_QOS_RDOSTD 寄存器结构定义。地址偏移量:0x160+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读命令OSTD限制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_ostd_lvl : 8;  /* bit[0-7] : 读命令outstanding（OSTD）水线：
                                                      0x00：不限制命令的OSTD个数；
                                                      0x01～0xFC：允许的最大读命令OSTD个数。
                                                      注意：
                                                      1.当读命令outstanding达到设定水线时，阻塞所有读命令；
                                                      2.写命令由于其fast response的特性，无法准确控制其outstanding。 */
        unsigned int  reserved    : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_QOS_RDOSTD_UNION;
#endif
#define SOC_DMSS_ASI_QOS_RDOSTD_rd_ostd_lvl_START  (0)
#define SOC_DMSS_ASI_QOS_RDOSTD_rd_ostd_lvl_END    (7)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_PUSH_WRMID_UNION
 结构说明  : ASI_PUSH_WRMID 寄存器结构定义。地址偏移量:0x180+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 写命令同MID命令优先级传递使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_mid_sel : 32; /* bit[0-31]: 写命令MID（最低五位）的比特匹配选择：
                                                     wr_mid_sel[n]=0：对于MID=n的命令，在QOSBUF中禁止同MID写命令优先级传递功能；
                                                     wr_mid_sel[n]=1：对于MID=n的命令，在QOSBUF中使能同MID写命令优先级传递功能。
                                                     注意：
                                                     同时还需要在QOSBUF中使能同MID的优先级传递功能。 */
    } reg;
} SOC_DMSS_ASI_PUSH_WRMID_UNION;
#endif
#define SOC_DMSS_ASI_PUSH_WRMID_wr_mid_sel_START  (0)
#define SOC_DMSS_ASI_PUSH_WRMID_wr_mid_sel_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_PUSH_RDMID_UNION
 结构说明  : ASI_PUSH_RDMID 寄存器结构定义。地址偏移量:0x184+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读命令同MID命令优先级传递使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_mid_sel : 32; /* bit[0-31]: 读命令MID（最低五位）的比特匹配选择：
                                                     rd_mid_sel[n]=0：对于MID=n的命令，在QOSBUF中禁止同MID读命令优先级传递功能；
                                                     rd_mid_sel[n]=1：对于MID=n的命令，在QOSBUF中使能同MID读命令优先级传递功能。
                                                     注意：
                                                     同时还需要在QOSBUF中使能同MID的优先级传递功能。 */
    } reg;
} SOC_DMSS_ASI_PUSH_RDMID_UNION;
#endif
#define SOC_DMSS_ASI_PUSH_RDMID_rd_mid_sel_START  (0)
#define SOC_DMSS_ASI_PUSH_RDMID_rd_mid_sel_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_MASK_UNION
 结构说明  : ASI_STAT_MASK 寄存器结构定义。地址偏移量:0x200+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 统计功能屏蔽
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_mask_flux_rd  : 1;  /* bit[0]    : 读流量统计屏蔽：
                                                              0：参与统计；
                                                              1：不参与统计。
                                                              说明：
                                                              1.统计结果记录在寄存器ASI_STAT_FLUX_RD中；
                                                              2.只有寄存器ASI_STAT_FLUX_FILTER0/1中的ID和优先级匹配成功的命令，才参与统计；
                                                              3.统计的使能开关在寄存器GLB_STAT_CTRL中。 */
        unsigned int  stat_mask_flux_wr  : 1;  /* bit[1]    : 写流量统计屏蔽：
                                                              配置方式同stat_flux_rd_en。 */
        unsigned int  stat_mask_block_rd : 1;  /* bit[2]    : 读命令阻塞统计屏蔽：
                                                              0：参与统计；
                                                              1：不参与统计。
                                                              说明：
                                                              1.统计结果记录在寄存器ASI_STAT_BLOCK_RD中；
                                                              2.统计的使能开关在寄存器GLB_STAT_CTRL中。 */
        unsigned int  stat_mask_block_wr : 1;  /* bit[3]    : 写命令阻塞统计屏蔽：
                                                              配置方式同stat_block_rd_en。 */
        unsigned int  stat_mask_lmtr0    : 1;  /* bit[4]    : LMTR0的生效周期统计屏蔽：
                                                              0：参与统计；
                                                              1：不参与统计。
                                                              说明：
                                                              1.统计结果记录在寄存器ASI_STAT_LMTR0中；
                                                              2.统计的使能开关在寄存器GLB_STAT_CTRL中。 */
        unsigned int  stat_mask_lmtr1    : 1;  /* bit[5]    : LMTR1的生效周期统计屏蔽：
                                                              配置方式同stat_lmtr0_en。 */
        unsigned int  stat_mask_lmtr2    : 1;  /* bit[6]    : LMTR2的生效周期统计屏蔽：
                                                              配置方式同stat_lmtr0_en。 */
        unsigned int  stat_mask_lmtr3    : 1;  /* bit[7]    : LMTR3的生效周期统计屏蔽：
                                                              配置方式同stat_lmtr0_en。 */
        unsigned int  stat_mask_rdlat    : 1;  /* bit[8]    : 读命令累积latency统计屏蔽：
                                                              0：参与统计；
                                                              1：不参与统计。
                                                              说明：
                                                              1.统计结果记录在寄存器ASI_STAT_RDLAT中；
                                                              2.统计的使能开关在寄存器GLB_STAT_CTRL中。 */
        unsigned int  stat_mask_pri      : 1;  /* bit[9]    : 命令优先级统计屏蔽：
                                                              0：参与统计；
                                                              1：不参与统计。
                                                              说明：
                                                              1.统计结果记录在寄存器ASI_STAT_PRI中；
                                                              2.统计的使能开关在寄存器GLB_STAT_CTRL中。 */
        unsigned int  stat_mask_rdnum    : 1;  /* bit[10]   : 读命令个数统计屏蔽：
                                                              0：参与统计；
                                                              1：不参与统计。
                                                              说明：
                                                              1.统计结果记录在寄存器ASI_STAT_RDNUM中；
                                                              2.统计的使能开关在寄存器GLB_STAT_CTRL中。 */
        unsigned int  stat_mask_dmi      : 1;  /* bit[11]   : DMI（包括Syscache方向）统计屏蔽：
                                                              0：DMI和AMI的命令均参与统计；
                                                              1：DMI命令（包括Syscache方向）不参与统计，只统计AMI的命令。
                                                              说明：
                                                              1.此寄存器配置影响ASI_STAT_RDNUM、ASI_STAT_FLUX_WR、ASI_STAT_FLUX_RD、ASI_STAT_RDLAT统计结果；
                                                              2.AMI命令包括该ASI连接的所有AMI通道的命令。 */
        unsigned int  stat_mask_lmtr4    : 1;  /* bit[12]   : LMTR4的生效周期统计屏蔽：
                                                              配置方式同stat_lmtr0_en。 */
        unsigned int  reserved           : 19; /* bit[13-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_STAT_MASK_UNION;
#endif
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_flux_rd_START   (0)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_flux_rd_END     (0)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_flux_wr_START   (1)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_flux_wr_END     (1)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_block_rd_START  (2)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_block_rd_END    (2)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_block_wr_START  (3)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_block_wr_END    (3)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr0_START     (4)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr0_END       (4)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr1_START     (5)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr1_END       (5)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr2_START     (6)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr2_END       (6)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr3_START     (7)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr3_END       (7)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_rdlat_START     (8)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_rdlat_END       (8)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_pri_START       (9)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_pri_END         (9)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_rdnum_START     (10)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_rdnum_END       (10)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_dmi_START       (11)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_dmi_END         (11)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr4_START     (12)
#define SOC_DMSS_ASI_STAT_MASK_stat_mask_lmtr4_END       (12)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_FLUX_FILTER0_UNION
 结构说明  : ASI_STAT_FLUX_FILTER0 寄存器结构定义。地址偏移量:0x210+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 流量统计过滤配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_flux_id_match  : 20; /* bit[0-19] : 流量统计匹配ID：
                                                               与stat_flux_id_mask配合使用，当cmd_id & stat_flux_id_mask = stat_flux_id_match时，认为当前cmd_id被成功匹配。成功匹配ID及MID的命令才会参与寄存器ASI_STAT_FLUX中的流量统计。
                                                               说明：
                                                               1.ID和MID必须同时匹配，才可参与统计；
                                                               2.ID具体位宽取决于RTL的配置，可在GLB_RTL_INF0寄存器中查询；
                                                               3.若stat_flux_id_match和stat_flux_id_mask都配为0，则可对所有命令进行流量统计；
                                                               4.此寄存器需在统计功能关闭时配置，否则会导致统计结果不准确。 */
        unsigned int  stat_flux_mid_match : 12; /* bit[20-31]: 流量统计匹配MID：
                                                               与stat_flux_mid_mask配合使用，当cmd_mid & stat_flux_mid_mask = stat_flux_mid_match时，认为当前cmd_mid被成功匹配。成功匹配ID及MID的命令才会参与寄存器ASI_STAT_FLUX中的流量统计。
                                                               说明：
                                                               1.ID和MID必须同时匹配，才可参与统计；
                                                               2.MID具体位宽取决于RTL的配置，可在GLB_RTL_INF0寄存器中查询；
                                                               3.若stat_flux_mid_match和stat_flux_mid_mask都配为0，则可对所有命令进行流量统计；
                                                               4.此寄存器需在统计功能关闭时配置，否则会导致统计结果不准确。 */
    } reg;
} SOC_DMSS_ASI_STAT_FLUX_FILTER0_UNION;
#endif
#define SOC_DMSS_ASI_STAT_FLUX_FILTER0_stat_flux_id_match_START   (0)
#define SOC_DMSS_ASI_STAT_FLUX_FILTER0_stat_flux_id_match_END     (19)
#define SOC_DMSS_ASI_STAT_FLUX_FILTER0_stat_flux_mid_match_START  (20)
#define SOC_DMSS_ASI_STAT_FLUX_FILTER0_stat_flux_mid_match_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_FLUX_FILTER1_UNION
 结构说明  : ASI_STAT_FLUX_FILTER1 寄存器结构定义。地址偏移量:0x214+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 流量统计过滤配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_flux_id_mask  : 20; /* bit[0-19] : 流量统计匹配ID：
                                                              与stat_flux_id_match配合使用，当cmd_id & stat_flux_id_mask = stat_flux_id_match时，认为当前cmd_id被成功匹配。成功匹配ID及MID的命令才会参与寄存器ASI_STAT_FLUX中的流量统计。
                                                              说明：
                                                              1.ID和MID必须同时匹配，才可参与统计；
                                                              2.具体位宽取决于RTL的配置，可在GLB_RTL_INF0寄存器中查询；
                                                              3.若stat_flux_id_match和stat_flux_id_mask都配为0，则可对所有命令进行流量统计；
                                                              4.此寄存器需在统计功能关闭时配置，否则会导致统计结果不准确。 */
        unsigned int  stat_flux_mid_mask : 12; /* bit[20-31]: 流量统计匹配MID：
                                                              与stat_flux_mid_match配合使用，当cmd_mid & stat_flux_mid_mask = stat_flux_mid_match时，认为当前cmd_mid被成功匹配。成功匹配ID及MID的命令才会参与寄存器ASI_STAT_FLUX中的流量统计。
                                                              说明：
                                                              1.ID和MID必须同时匹配，才可参与统计；
                                                              2.具体位宽取决于RTL的配置，可在GLB_RTL_INF0寄存器中查询；
                                                              3.若stat_flux_mid_match和stat_flux_mid_mask都配为0，则可对所有命令进行流量统计；
                                                              4.此寄存器需在统计功能关闭时配置，否则会导致统计结果不准确。 */
    } reg;
} SOC_DMSS_ASI_STAT_FLUX_FILTER1_UNION;
#endif
#define SOC_DMSS_ASI_STAT_FLUX_FILTER1_stat_flux_id_mask_START   (0)
#define SOC_DMSS_ASI_STAT_FLUX_FILTER1_stat_flux_id_mask_END     (19)
#define SOC_DMSS_ASI_STAT_FLUX_FILTER1_stat_flux_mid_mask_START  (20)
#define SOC_DMSS_ASI_STAT_FLUX_FILTER1_stat_flux_mid_mask_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_FLUX_WR_UNION
 结构说明  : ASI_STAT_FLUX_WR 寄存器结构定义。地址偏移量:0x218+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 写流量统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_flux_wr : 32; /* bit[0-31]: 统计写流量：
                                                       说明：
                                                       1.累加所有写命令对应的数据流量，单位为Byte；
                                                       2.统计的使能、过滤及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_FLUX_FILTER0/ASI_STAT_FLUX_FILTER1/ASI_STAT_MASK；
                                                       3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数；
                                                       4.32bit计数器，即量程为4GB。 */
    } reg;
} SOC_DMSS_ASI_STAT_FLUX_WR_UNION;
#endif
#define SOC_DMSS_ASI_STAT_FLUX_WR_stat_flux_wr_START  (0)
#define SOC_DMSS_ASI_STAT_FLUX_WR_stat_flux_wr_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_FLUX_RD_UNION
 结构说明  : ASI_STAT_FLUX_RD 寄存器结构定义。地址偏移量:0x21C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读流量统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_flux_rd : 32; /* bit[0-31]: 统计读流量：
                                                       说明：
                                                       1.累加所有读命令对应的数据流量，单位为Byte；
                                                       2.统计的使能、过滤及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_FLUX_FILTER0/ASI_STAT_FLUX_FILTER1/ASI_STAT_MASK；
                                                       3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数；
                                                       4.32bit计数器，即量程为4GB。 */
    } reg;
} SOC_DMSS_ASI_STAT_FLUX_RD_UNION;
#endif
#define SOC_DMSS_ASI_STAT_FLUX_RD_stat_flux_rd_START  (0)
#define SOC_DMSS_ASI_STAT_FLUX_RD_stat_flux_rd_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_LMTR0_UNION
 结构说明  : ASI_STAT_LMTR0 寄存器结构定义。地址偏移量:0x220+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽控制器0统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_lmtr0 : 32; /* bit[0-31]: 统计LMTR0的生效周期：
                                                     说明：
                                                     1.累加LMTR0的生效（反压命令）周期数，单位为cycle；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_LMTR0_UNION;
#endif
#define SOC_DMSS_ASI_STAT_LMTR0_stat_lmtr0_START  (0)
#define SOC_DMSS_ASI_STAT_LMTR0_stat_lmtr0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_LMTR1_UNION
 结构说明  : ASI_STAT_LMTR1 寄存器结构定义。地址偏移量:0x224+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽控制器1统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_lmtr1 : 32; /* bit[0-31]: 统计LMTR1的生效周期：
                                                     说明：
                                                     1.累加LMTR1的生效（反压命令）周期数，单位为cycle；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_LMTR1_UNION;
#endif
#define SOC_DMSS_ASI_STAT_LMTR1_stat_lmtr1_START  (0)
#define SOC_DMSS_ASI_STAT_LMTR1_stat_lmtr1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_LMTR2_UNION
 结构说明  : ASI_STAT_LMTR2 寄存器结构定义。地址偏移量:0x228+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽控制器2统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_lmtr2 : 32; /* bit[0-31]: 统计LMTR2的生效周期：
                                                     说明：
                                                     1.累加LMTR2的生效（反压命令）周期数，单位为cycle；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_LMTR2_UNION;
#endif
#define SOC_DMSS_ASI_STAT_LMTR2_stat_lmtr2_START  (0)
#define SOC_DMSS_ASI_STAT_LMTR2_stat_lmtr2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_LMTR3_UNION
 结构说明  : ASI_STAT_LMTR3 寄存器结构定义。地址偏移量:0x22C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽控制器3统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_lmtr3 : 32; /* bit[0-31]: 统计LMTR3的生效周期：
                                                     说明：
                                                     1.累加LMTR3的生效（反压命令）周期数，单位为cycle；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_LMTR3_UNION;
#endif
#define SOC_DMSS_ASI_STAT_LMTR3_stat_lmtr3_START  (0)
#define SOC_DMSS_ASI_STAT_LMTR3_stat_lmtr3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_RDLAT_UNION
 结构说明  : ASI_STAT_RDLAT 寄存器结构定义。地址偏移量:0x230+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读latency统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rdlat : 32; /* bit[0-31]: 统计累计读latency：
                                                     说明：
                                                     1.逐cycle累加读outstanding：累加结果/读命令累积个数=读平均latency；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_RDLAT_UNION;
#endif
#define SOC_DMSS_ASI_STAT_RDLAT_stat_rdlat_START  (0)
#define SOC_DMSS_ASI_STAT_RDLAT_stat_rdlat_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_RDNUM_UNION
 结构说明  : ASI_STAT_RDNUM 寄存器结构定义。地址偏移量:0x234+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读命令个数
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rdnum : 32; /* bit[0-31]: 统计累计读命令个数：
                                                     说明：
                                                     1.统计累加读命令个数，用于平均读latency的计算；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_RDNUM_UNION;
#endif
#define SOC_DMSS_ASI_STAT_RDNUM_stat_rdnum_START  (0)
#define SOC_DMSS_ASI_STAT_RDNUM_stat_rdnum_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_BLOCK_WR_UNION
 结构说明  : ASI_STAT_BLOCK_WR 寄存器结构定义。地址偏移量:0x238+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 写命令阻塞统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_block_wr : 32; /* bit[0-31]: 统计阻塞写命令的生效周期：
                                                        说明：
                                                        1.累加写命令被阻塞的周期数，单位为cycle；
                                                        2.写命令可能的被阻塞源头：LMTR0/1/2/3；
                                                        3.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                        4.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_BLOCK_WR_UNION;
#endif
#define SOC_DMSS_ASI_STAT_BLOCK_WR_stat_block_wr_START  (0)
#define SOC_DMSS_ASI_STAT_BLOCK_WR_stat_block_wr_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_BLOCK_RD_UNION
 结构说明  : ASI_STAT_BLOCK_RD 寄存器结构定义。地址偏移量:0x23C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 读命令阻塞统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_block_rd : 32; /* bit[0-31]: 统计阻塞读命令的生效周期：
                                                        说明：
                                                        1.累加读命令被阻塞的周期数，单位为cycle；
                                                        2.读命令可能的被阻塞源头：LMTR0/2，rd_ostd；
                                                        3.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                        4.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_BLOCK_RD_UNION;
#endif
#define SOC_DMSS_ASI_STAT_BLOCK_RD_stat_block_rd_START  (0)
#define SOC_DMSS_ASI_STAT_BLOCK_RD_stat_block_rd_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_PRI01_UNION
 结构说明  : ASI_STAT_PRI01 寄存器结构定义。地址偏移量:0x240+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 命令优先级0和1统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_pri0 : 16; /* bit[0-15] : 统计命令优先级为0的命令个数。 */
        unsigned int  stat_pri1 : 16; /* bit[16-31]: 统计优先级为1的命令个数：
                                                     说明：
                                                     1.此处统计的命令是拆分后发给DDRC的命令，非AXI命令；
                                                     2.若寄存器ASI_QOS_CTRL中的arb_pri_use为0，则此处统计的是命令优先级（来源是优先级重映射/Regulator/Latency自适应三种方式之一）；反之则统计的是仲裁优先级（在命令优先级的基础之上，优先级可能受优先级自适应及优先级传递等功能的作用而被提高）。 */
    } reg;
} SOC_DMSS_ASI_STAT_PRI01_UNION;
#endif
#define SOC_DMSS_ASI_STAT_PRI01_stat_pri0_START  (0)
#define SOC_DMSS_ASI_STAT_PRI01_stat_pri0_END    (15)
#define SOC_DMSS_ASI_STAT_PRI01_stat_pri1_START  (16)
#define SOC_DMSS_ASI_STAT_PRI01_stat_pri1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_PRI23_UNION
 结构说明  : ASI_STAT_PRI23 寄存器结构定义。地址偏移量:0x244+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 命令优先级2和3统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_pri2 : 16; /* bit[0-15] : 统计命令优先级为2的命令个数。 */
        unsigned int  stat_pri3 : 16; /* bit[16-31]: 统计命令优先级为3的命令个数。 */
    } reg;
} SOC_DMSS_ASI_STAT_PRI23_UNION;
#endif
#define SOC_DMSS_ASI_STAT_PRI23_stat_pri2_START  (0)
#define SOC_DMSS_ASI_STAT_PRI23_stat_pri2_END    (15)
#define SOC_DMSS_ASI_STAT_PRI23_stat_pri3_START  (16)
#define SOC_DMSS_ASI_STAT_PRI23_stat_pri3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_PRI45_UNION
 结构说明  : ASI_STAT_PRI45 寄存器结构定义。地址偏移量:0x248+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 命令优先级4和5统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_pri4 : 16; /* bit[0-15] : 统计命令优先级为4的命令个数。 */
        unsigned int  stat_pri5 : 16; /* bit[16-31]: 统计命令优先级为5的命令个数。 */
    } reg;
} SOC_DMSS_ASI_STAT_PRI45_UNION;
#endif
#define SOC_DMSS_ASI_STAT_PRI45_stat_pri4_START  (0)
#define SOC_DMSS_ASI_STAT_PRI45_stat_pri4_END    (15)
#define SOC_DMSS_ASI_STAT_PRI45_stat_pri5_START  (16)
#define SOC_DMSS_ASI_STAT_PRI45_stat_pri5_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_PRI67_UNION
 结构说明  : ASI_STAT_PRI67 寄存器结构定义。地址偏移量:0x24C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 命令优先级6和7统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_pri6 : 16; /* bit[0-15] : 统计命令优先级为6的命令个数。 */
        unsigned int  stat_pri7 : 16; /* bit[16-31]: 统计命令优先级为7的命令个数。 */
    } reg;
} SOC_DMSS_ASI_STAT_PRI67_UNION;
#endif
#define SOC_DMSS_ASI_STAT_PRI67_stat_pri6_START  (0)
#define SOC_DMSS_ASI_STAT_PRI67_stat_pri6_END    (15)
#define SOC_DMSS_ASI_STAT_PRI67_stat_pri7_START  (16)
#define SOC_DMSS_ASI_STAT_PRI67_stat_pri7_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_STAT_LMTR4_UNION
 结构说明  : ASI_STAT_LMTR4 寄存器结构定义。地址偏移量:0x250+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 带宽控制器4统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_lmtr4 : 32; /* bit[0-31]: 统计LMTR3的生效周期：
                                                     说明：
                                                     1.累加LMTR4的生效（反压命令）周期数，单位为cycle；
                                                     2.统计的使能及屏蔽，参见寄存器GLB_STAT_CTRL/ASI_STAT_MASK；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_ASI_STAT_LMTR4_UNION;
#endif
#define SOC_DMSS_ASI_STAT_LMTR4_stat_lmtr4_START  (0)
#define SOC_DMSS_ASI_STAT_LMTR4_stat_lmtr4_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DETECT_CFG0_UNION
 结构说明  : ASI_DETECT_CFG0 寄存器结构定义。地址偏移量:0x280+0x800*(asi_base)，初值:0x10000000，宽度:32
 寄存器说明: Pattern detect功能控制0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  detect_timeout : 8;  /* bit[0-7] : 在侦测模式下，读命令Outstanding为0持续时间的timeout配置值；在统计记录模式下，记录读命令Outstanding为0持续时间最大值的上限值。
                                                         配置值 = timeout时钟周期/16
                                                         举例：
                                                         侦测模式下timeout配置值为5，则每当读命令Outstanding为0持续5*16=80个时钟周期后，触发Timeout。 */
        unsigned int  reserved_0     : 20; /* bit[8-27]: 保留。 */
        unsigned int  detect_int_en  : 1;  /* bit[28]  : Pattern detect功能的中断输出使能：
                                                         0：禁止；
                                                         1：使能。 */
        unsigned int  reserved_1     : 1;  /* bit[29]  : 保留。 */
        unsigned int  detect_mode    : 1;  /* bit[30]  : Pattern detect功能模式：
                                                         0：侦测模式：
                                                         当侦测到目标pattern场景出现指定次数时，侦测功能将自动停止且上报中断；
                                                         1：统计记录模式：
                                                         当统计到指定命令个数时，统计功能自动停止并记录pattern场景出现次数，且上报中断。 */
        unsigned int  detect_en      : 1;  /* bit[31]  : Pattern detect功能使能。
                                                         0：Pattern detect功能关闭；
                                                         1：Pattern detect功能使能。
                                                         说明：
                                                         1.若RTL不支持Pattern detect时，则此寄存器无效；
                                                         2.Pattern detect功能自动停止时（侦测模式下侦测到目标pattern场景出现指定次数，或统计记录模式下统计到指定命令个数），此开关会自动关闭;
                                                         3.先完成Pattern detect所有功能配置，最后使能开关。 */
    } reg;
} SOC_DMSS_ASI_DETECT_CFG0_UNION;
#endif
#define SOC_DMSS_ASI_DETECT_CFG0_detect_timeout_START  (0)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_timeout_END    (7)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_int_en_START   (28)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_int_en_END     (28)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_mode_START     (30)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_mode_END       (30)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_en_START       (31)
#define SOC_DMSS_ASI_DETECT_CFG0_detect_en_END         (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DETECT_CFG1_UNION
 结构说明  : ASI_DETECT_CFG1 寄存器结构定义。地址偏移量:0x284+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: Pattern detect功能控制1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  detect_cnt0_wl : 12; /* bit[0-11] : 侦测模式或统计记录模式下，指定的读命令个数的配置水线。
                                                          注意：
                                                          当功能使能时，此寄存器不能配置为0。 */
        unsigned int  detect_cnt1_wl : 12; /* bit[12-23]: 侦测模式下，读Outstanding为1的读命令个数的配置水线。
                                                          注意：
                                                          当侦测模式使能时，此寄存器不能配置为0。 */
        unsigned int  detect_cnt2_wl : 8;  /* bit[24-31]: 侦测模式下，pattern识别个数的配置水线。
                                                          注意：
                                                          当侦测模式使能时，此寄存器不能配置为0。 */
    } reg;
} SOC_DMSS_ASI_DETECT_CFG1_UNION;
#endif
#define SOC_DMSS_ASI_DETECT_CFG1_detect_cnt0_wl_START  (0)
#define SOC_DMSS_ASI_DETECT_CFG1_detect_cnt0_wl_END    (11)
#define SOC_DMSS_ASI_DETECT_CFG1_detect_cnt1_wl_START  (12)
#define SOC_DMSS_ASI_DETECT_CFG1_detect_cnt1_wl_END    (23)
#define SOC_DMSS_ASI_DETECT_CFG1_detect_cnt2_wl_START  (24)
#define SOC_DMSS_ASI_DETECT_CFG1_detect_cnt2_wl_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DETECT_INF0_UNION
 结构说明  : ASI_DETECT_INF0 寄存器结构定义。地址偏移量:0x288+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: Pattern detect信息记录0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  detect_cnt1 : 12; /* bit[0-11] : 统计记录模式下，指定的读命令个数下的读Outstanding为1的读命令个数。 */
        unsigned int  reserved_0  : 4;  /* bit[12-15]: 保留。 */
        unsigned int  detect_time : 8;  /* bit[16-23]: 统计记录模式下，指定的读命令个数中读命令Outstanding为0持续时间，在ASI_DETECT_CFG0.detect_timeout配置水线以内，最长持续时间的时钟周期个数除以16的结果。 */
        unsigned int  reserved_1  : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DETECT_INF0_UNION;
#endif
#define SOC_DMSS_ASI_DETECT_INF0_detect_cnt1_START  (0)
#define SOC_DMSS_ASI_DETECT_INF0_detect_cnt1_END    (11)
#define SOC_DMSS_ASI_DETECT_INF0_detect_time_START  (16)
#define SOC_DMSS_ASI_DETECT_INF0_detect_time_END    (23)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_WAKEUP_CFG_UNION
 结构说明  : ASI_WAKEUP_CFG 寄存器结构定义。地址偏移量:0x290+0x800*(asi_base)，初值:0x00000001，宽度:32
 寄存器说明: Wakeup快速唤醒功能控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wakeup_hold   : 2;  /* bit[0-1] : DDR Power Down快速唤醒信号的持续时钟周期：
                                                        0：16个时钟周期；
                                                        1：32个时钟周期；
                                                        2：64个时钟周期；
                                                        3：256个时钟周期。
                                                        说明：
                                                        若RTL不支持DDR Power Down快速唤醒时，则此寄存器无效。 */
        unsigned int  reserved      : 28; /* bit[2-29]: 保留。 */
        unsigned int  detect_wakeup : 1;  /* bit[30]  : Pattern detect功能的侦测模式下，侦测到目标pattern后，是否触发DDR Power Down快速唤醒：
                                                        0：不触发DDR Power Down快速唤醒功能；
                                                        1：触发DDR Power Down快速唤醒功能。
                                                        说明：
                                                        若RTL不支持DDR Power Down快速唤醒或者不支持Pattern detect时，则此寄存器无效。 */
        unsigned int  wakeup_en     : 1;  /* bit[31]  : DDR Power Down快速唤醒功能使能。
                                                        0：DDR Power Down快速唤醒功能关闭；
                                                        1：DDR Power Down快速唤醒功能使能。
                                                        说明：
                                                        若RTL不支持DDR Power Down快速唤醒时，则此寄存器无效。 */
    } reg;
} SOC_DMSS_ASI_WAKEUP_CFG_UNION;
#endif
#define SOC_DMSS_ASI_WAKEUP_CFG_wakeup_hold_START    (0)
#define SOC_DMSS_ASI_WAKEUP_CFG_wakeup_hold_END      (1)
#define SOC_DMSS_ASI_WAKEUP_CFG_detect_wakeup_START  (30)
#define SOC_DMSS_ASI_WAKEUP_CFG_detect_wakeup_END    (30)
#define SOC_DMSS_ASI_WAKEUP_CFG_wakeup_en_START      (31)
#define SOC_DMSS_ASI_WAKEUP_CFG_wakeup_en_END        (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_SUB_RGN0_UNION
 结构说明  : ASI_SEC_SUB_RGN0 寄存器结构定义。地址偏移量:0x300+0x10*(asi_sub_rgns)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 安全地址区域子区域0
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sub_rgn_disable0 : 32; /* bit[0-31]: 当前安全区域的子区域屏蔽。
                                                           sub_rgn_disable0[n]=0：第（n+96）个子区域n使能（当前子区域内的命令权限，取决于当前rgn）；
                                                           sub_rgn_disable0[n]=1：第（n+96）个子区域n屏蔽（当前子区域内的命令权限，取决于其他rgn）。
                                                           说明：
                                                           1.当前区域是否支持此功能与RTL配置有关，需要根据支持subrgn属性的regions个数判定（从ASI_RTL_INF3.rtl_sec_subrgn_num可读出）。
                                                           2.region0不可配置，默认不支持subrgn。
                                                           3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_SEC_SUB_RGN0_UNION;
#endif
#define SOC_DMSS_ASI_SEC_SUB_RGN0_sub_rgn_disable0_START  (0)
#define SOC_DMSS_ASI_SEC_SUB_RGN0_sub_rgn_disable0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_SUB_RGN1_UNION
 结构说明  : ASI_SEC_SUB_RGN1 寄存器结构定义。地址偏移量:0x304+0x10*(asi_sub_rgns)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 安全地址区域子区域1
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sub_rgn_disable1 : 32; /* bit[0-31]: 当前安全区域的子区域屏蔽。
                                                           sub_rgn_disable1[n]=0：第(n+64)个子区域n使能（当前子区域内的命令权限，取决于当前rgn）；
                                                           sub_rgn_disable1[n]=1：第(n+64)个子区域n屏蔽（当前子区域内的命令权限，取决于其他rgn）。
                                                           说明：
                                                           1.当前区域是否支持此功能与RTL配置有关，需要根据支持subrgn属性的regions个数判定（从ASI_RTL_INF3.rtl_sec_subrgn_num可读出）。
                                                           2.region0不可配置，默认不支持subrgn。
                                                           3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_SEC_SUB_RGN1_UNION;
#endif
#define SOC_DMSS_ASI_SEC_SUB_RGN1_sub_rgn_disable1_START  (0)
#define SOC_DMSS_ASI_SEC_SUB_RGN1_sub_rgn_disable1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_SUB_RGN2_UNION
 结构说明  : ASI_SEC_SUB_RGN2 寄存器结构定义。地址偏移量:0x308+0x10*(asi_sub_rgns)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 安全地址区域子区域2
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sub_rgn_disable2 : 32; /* bit[0-31]: 当前安全区域的子区域屏蔽。
                                                           sub_rgn_disable2[n]=0：第(n+32)个子区域n使能（当前子区域内的命令权限，取决于当前rgn）；
                                                           sub_rgn_disable2[n]=1：第(n+32)个子区域n屏蔽（当前子区域内的命令权限，取决于其他rgn）。
                                                           说明：
                                                           1.当前区域是否支持此功能与RTL配置有关，需要根据支持subrgn属性的regions个数判定（从ASI_RTL_INF3.rtl_sec_subrgn_num可读出）。
                                                           2.region0不可配置，默认不支持subrgn。
                                                           3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_SEC_SUB_RGN2_UNION;
#endif
#define SOC_DMSS_ASI_SEC_SUB_RGN2_sub_rgn_disable2_START  (0)
#define SOC_DMSS_ASI_SEC_SUB_RGN2_sub_rgn_disable2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_SUB_RGN3_UNION
 结构说明  : ASI_SEC_SUB_RGN3 寄存器结构定义。地址偏移量:0x30C+0x10*(asi_sub_rgns)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 安全地址区域子区域3
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sub_rgn_disable3 : 32; /* bit[0-31]: 当前安全区域的子区域屏蔽。
                                                           sub_rgn_disable3[n]=0：第n个子区域n使能（当前子区域内的命令权限，取决于当前rgn）；
                                                           sub_rgn_disable3[n]=1：第n个子区域n屏蔽（当前子区域内的命令权限，取决于其他rgn）。
                                                           说明：
                                                           1.当前区域是否支持此功能与RTL配置有关，需要根据支持subrgn属性的regions个数判定（从ASI_RTL_INF3.rtl_sec_subrgn_num可读出）。
                                                           2.region0不可配置，默认不支持subrgn。
                                                           3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_SEC_SUB_RGN3_UNION;
#endif
#define SOC_DMSS_ASI_SEC_SUB_RGN3_sub_rgn_disable3_START  (0)
#define SOC_DMSS_ASI_SEC_SUB_RGN3_sub_rgn_disable3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_LD_SEL_UNION
 结构说明  : ASI_SEC_LD_SEL 寄存器结构定义。地址偏移量:0x410+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ld_rgns_sel : 32; /* bit[0-31]: 选择需将相关配置寄存器锁定为只读的regions：
                                                      sec_ld_rgns_sel[n]=0：region n未被选定。
                                                      sec_ld_rgns_sel[n]=1：region n被选定。
                                                      注意：
                                                      若某region被选定，则此rgn对应的如下相关配置寄存器皆被选定：
                                                      （1）ASI_SEC_RGN_MAP0；
                                                      （2）ASI_SEC_RGN_MAP1；
                                                      （3）ASI_SEC_MID_WR；
                                                      （4）ASI_SEC_MID_RD；
                                                      若sucure_boot_lock信号被拉高，则所有被选定寄存器及本寄存器，都将被锁定成只读。只有复位DMSS方可解锁。 */
    } reg;
} SOC_DMSS_ASI_SEC_LD_SEL_UNION;
#endif
#define SOC_DMSS_ASI_SEC_LD_SEL_ld_rgns_sel_START  (0)
#define SOC_DMSS_ASI_SEC_LD_SEL_ld_rgns_sel_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_INT_EN_UNION
 结构说明  : ASI_SEC_INT_EN 寄存器结构定义。地址偏移量:0x420+0x800*(asi_base)，初值:0x00000001，宽度:32
 寄存器说明: 安全中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : 越权命令的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有越权命令访问时，可上报中断并记录越权命令信息（请参考ASI_SEC_FAIL_CMD_INF寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_SEC_INT_EN_UNION;
#endif
#define SOC_DMSS_ASI_SEC_INT_EN_int_en_START    (0)
#define SOC_DMSS_ASI_SEC_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_INT_STATUS_UNION
 结构说明  : ASI_SEC_INT_STATUS 寄存器结构定义。地址偏移量:0x424+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 安全中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。
                                                      注意：从GLOBAL模块的GLB_INT_STATUS寄存器中，可识别出中断来自于哪个ASI中的SEC模块。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_SEC_INT_STATUS_UNION;
#endif
#define SOC_DMSS_ASI_SEC_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_ASI_SEC_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_ASI_SEC_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_ASI_SEC_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_ASI_SEC_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_ASI_SEC_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_UNION
 结构说明  : ASI_SEC_FAIL_CMD_INF0 寄存器结构定义。地址偏移量:0x480+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_low : 32; /* bit[0-31]: 中断状态清除前的首个越权命令的低32位地址。 */
    } reg;
} SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_UNION;
#endif
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_address_low_START  (0)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF0_address_low_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_UNION
 结构说明  : ASI_SEC_FAIL_CMD_INF1 寄存器结构定义。地址偏移量:0x484+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_high : 8;  /* bit[0-7]  : 中断状态清除前的首个越权命令的高位地址（如果存在高位地址）。 */
        unsigned int  reserved     : 8;  /* bit[8-15] : 保留。 */
        unsigned int  privileged   : 1;  /* bit[16]   : 中断状态清除前首个越权命令的特权模式信息：
                                                        0：unprivileged访问；
                                                        1：privileged访问。 */
        unsigned int  nonsecure    : 1;  /* bit[17]   : 中断状态清除前首个越权命令的安全模式信息：
                                                        0：secure访问；
                                                        1：non-secure访问。 */
        unsigned int  access_burst : 2;  /* bit[18-19]: 中断状态清除前首个越权命令的burst信息：
                                                        0：FIXED；
                                                        1：INCR；
                                                        2：WRAP；
                                                        3：reserved。 */
        unsigned int  access_type  : 1;  /* bit[20]   : 中断状态清除前首个越权命令的读写模式信息：
                                                        0：read访问；
                                                        1：write访问。 */
        unsigned int  access_size  : 3;  /* bit[21-23]: 中断状态清除前首个越权命令的size信息：
                                                        0：8bits；
                                                        1：16bits；
                                                        2：32bits；
                                                        3：64bits；
                                                        4：128bits。 */
        unsigned int  access_len   : 4;  /* bit[24-27]: 中断状态清除前首个越权命令的length信息：
                                                        0~15：burst1~burst15。 */
        unsigned int  access_cache : 4;  /* bit[28-31]: 中断状态清除前首个越权命令的cache信息。 */
    } reg;
} SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_UNION;
#endif
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_address_high_START  (0)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_address_high_END    (7)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_privileged_START    (16)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_privileged_END      (16)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_nonsecure_START     (17)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_nonsecure_END       (17)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_burst_START  (18)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_burst_END    (19)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_type_START   (20)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_type_END     (20)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_size_START   (21)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_size_END     (23)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_len_START    (24)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_len_END      (27)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_cache_START  (28)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF1_access_cache_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_UNION
 结构说明  : ASI_SEC_FAIL_CMD_INF2 寄存器结构定义。地址偏移量:0x488+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  id  : 24; /* bit[0-23] : 中断状态清除前的首个越权命令的命令ID。 */
        unsigned int  mid : 8;  /* bit[24-31]: 中断状态清除前的首个越权命令的命令MasterID。 */
    } reg;
} SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_UNION;
#endif
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_id_START   (0)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_id_END     (23)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_mid_START  (24)
#define SOC_DMSS_ASI_SEC_FAIL_CMD_INF2_mid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_PROT_INT_EN_UNION
 结构说明  : ASI_AMI_PROT_INT_EN 寄存器结构定义。地址偏移量:0x4A0+0x800*(asi_base)，初值:0x00000001，宽度:32
 寄存器说明: Protected越权访问AMI中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : PMM的S-EL0状态Protected命令越权访问AMI的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有PMM的Protected命令在S-EL0状态越权访问AMI时，可上报中断并记录越权命令信息（请参考ASI_AMI_PROT_CMD_INF寄存器）；
                                                   2.若RTL不支持MPU或AMI时，或者ASI未连接AMI时，则此寄存器无效。
                                                   3.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_AMI_PROT_INT_EN_UNION;
#endif
#define SOC_DMSS_ASI_AMI_PROT_INT_EN_int_en_START    (0)
#define SOC_DMSS_ASI_AMI_PROT_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_PROT_INT_STATUS_UNION
 结构说明  : ASI_AMI_PROT_INT_STATUS 寄存器结构定义。地址偏移量:0x4A4+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: Protected越权访问AMI中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_AMI_PROT_INT_STATUS_UNION;
#endif
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_ASI_AMI_PROT_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_PROT_CMD_INF0_UNION
 结构说明  : ASI_AMI_PROT_CMD_INF0 寄存器结构定义。地址偏移量:0x4A8+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个Protected越权访问AMI拦截命令的信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_prot_low_address : 32; /* bit[0-31]: Protected命令越权访问AMI中断状态清除前的首个拦截命令的低32位地址。 */
    } reg;
} SOC_DMSS_ASI_AMI_PROT_CMD_INF0_UNION;
#endif
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF0_ami_prot_low_address_START  (0)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF0_ami_prot_low_address_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_PROT_CMD_INF1_UNION
 结构说明  : ASI_AMI_PROT_CMD_INF1 寄存器结构定义。地址偏移量:0x4AC+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个Protected越权访问AMI拦截命令的信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_prot_high_address : 8;  /* bit[0-7]  : Protected命令越权访问AMI中断状态清除前的首个拦截命令的高8位地址。 */
        unsigned int  reserved_0            : 8;  /* bit[8-15] : 保留。 */
        unsigned int  ami_prot_mid          : 8;  /* bit[16-23]: Protected命令越权访问AMI中断状态清除前的首个拦截命令的MID信息。 */
        unsigned int  ami_prot_cache        : 4;  /* bit[24-27]: Protected命令越权访问AMI中断状态清除前的首个拦截命令的AxCache信息。 */
        unsigned int  ami_prot_type         : 1;  /* bit[28]   : Protected命令越权访问AMI中断状态清除前的首个拦截命令的读写模式信息：
                                                                 0：read访问；
                                                                 1：write访问。 */
        unsigned int  reserved_1            : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_AMI_PROT_CMD_INF1_UNION;
#endif
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_high_address_START  (0)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_high_address_END    (7)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_mid_START           (16)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_mid_END             (23)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_cache_START         (24)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_cache_END           (27)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_type_START          (28)
#define SOC_DMSS_ASI_AMI_PROT_CMD_INF1_ami_prot_type_END            (28)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_ENHN_INT_EN_UNION
 结构说明  : ASI_AMI_ENHN_INT_EN 寄存器结构定义。地址偏移量:0x4B0+0x800*(asi_base)，初值:0x00000001，宽度:32
 寄存器说明: Enhance Trusted越权访问AMI中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : AMI命令访问禁止后，仍有AMI访问的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当AMI命令访问禁止后，仍有非法AMI访问时，可上报中断并记录越权命令信息（请参考ASI_AMI_ERR_CMD_INF寄存器）；
                                                   2.若RTL不支持MPU或AMI时，或者ASI未连接AMI时，则此寄存器无效。
                                                   3.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_AMI_ENHN_INT_EN_UNION;
#endif
#define SOC_DMSS_ASI_AMI_ENHN_INT_EN_int_en_START    (0)
#define SOC_DMSS_ASI_AMI_ENHN_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_UNION
 结构说明  : ASI_AMI_ENHN_INT_STATUS 寄存器结构定义。地址偏移量:0x4B4+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: Enhance Trusted越权访问AMI中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status : 1;  /* bit[0]   : 中断状态指示：
                                                     0：中断未产生；
                                                     1：中断已产生。 */
        unsigned int  reserved   : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_UNION;
#endif
#define SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_int_status_START  (0)
#define SOC_DMSS_ASI_AMI_ENHN_INT_STATUS_int_status_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_UNION
 结构说明  : ASI_AMI_ENHN_CMD_INF0 寄存器结构定义。地址偏移量:0x4B8+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个Enhance Trusted访问AMI命令的信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_enhn_low_address : 32; /* bit[0-31]: Enhance Trusted命令越权访问AMI中断状态清除前的首个拦截命令的低32位地址。
                                                               说明：
                                                               如果同一时刻出现读写拦截命令，记录读命令。 */
    } reg;
} SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_UNION;
#endif
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_ami_enhn_low_address_START  (0)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF0_ami_enhn_low_address_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_UNION
 结构说明  : ASI_AMI_ENHN_CMD_INF1 寄存器结构定义。地址偏移量:0x4BC+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个Enhance Trusted访问AMI命令命令的信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_enhn_high_address : 8;  /* bit[0-7]  : Enhance Trusted命令越权访问AMI中断状态清除前的首个拦截命令的高8位地址。
                                                                 说明：
                                                                 如果同一时刻出现读写拦截命令，记录读命令。 */
        unsigned int  reserved_0            : 8;  /* bit[8-15] : 保留。 */
        unsigned int  ami_enhn_mid          : 8;  /* bit[16-23]: Enhance Trusted命令越权访问AMI中断状态清除前的首个拦截命令的MID信息。
                                                                 说明：
                                                                 如果同一时刻出现读写拦截命令，记录读命令。 */
        unsigned int  ami_enhn_cache        : 4;  /* bit[24-27]: Enhance Trusted命令越权访问AMI中断状态清除前的首个拦截命令的AxCache信息。
                                                                 说明：
                                                                 如果同一时刻出现读写拦截命令，记录读命令。 */
        unsigned int  ami_enhn_type         : 1;  /* bit[28]   : Enhance Trusted命令越权访问AMI中断状态清除前的首个拦截命令的读写模式信息：
                                                                 0：read访问；
                                                                 1：write访问。
                                                                 说明：
                                                                 如果同一时刻出现读写拦截命令，记录读命令。 */
        unsigned int  reserved_1            : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_UNION;
#endif
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_high_address_START  (0)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_high_address_END    (7)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_mid_START           (16)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_mid_END             (23)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_cache_START         (24)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_cache_END           (27)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_type_START          (28)
#define SOC_DMSS_ASI_AMI_ENHN_CMD_INF1_ami_enhn_type_END            (28)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_UNION
 结构说明  : ASI_MPU_TRUST_WR_NTRUST0 寄存器结构定义。地址偏移量:0x4C0+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU trust写命令访问non-trust地址空间配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_trust_wr_ntrust0 : 32; /* bit[0-31]: MPU trust写命令访问non-trusted地址空间权限根据MID的选择：
                                                               mid_trust_wr_ntrust[n]=0：mid为n的trusted写命令访问non-trusted地址空间会被MPU拦截；
                                                               mid_trust_wr_ntrust[n]=1：mid为n的trusted写命令访问non-trusted地址空间不会被MPU拦截。
                                                               说明：
                                                               1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                               2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_mpu_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_mpu_chk_mid_width=3，则只有mid_trust_wr_ntrust[7:0]可配。
                                                               3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_UNION;
#endif
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_mid_trust_wr_ntrust0_START  (0)
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST0_mid_trust_wr_ntrust0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_UNION
 结构说明  : ASI_MPU_TRUST_WR_NTRUST1 寄存器结构定义。地址偏移量:0x4C4+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU trust写命令访问non-trust地址空间配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_trust_wr_ntrust1 : 32; /* bit[0-31]: MPU trust写命令访问non-trusted地址空间权限根据MID的选择：
                                                               mid_trust_wr_ntrust[n]=0：mid为(n+32)的trusted写命令访问non-trusted地址空间会被MPU拦截；
                                                               mid_trust_wr_ntrust[n]=1：mid为(n+32)的trusted写命令访问non-trusted地址空间不会被MPU拦截。
                                                               说明：
                                                               1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                               2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_mpu_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_mpu_chk_mid_width=3，则只有mid_trust_wr_ntrust[7:0]可配。
                                                               3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_UNION;
#endif
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_mid_trust_wr_ntrust1_START  (0)
#define SOC_DMSS_ASI_MPU_TRUST_WR_NTRUST1_mid_trust_wr_ntrust1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_UNION
 结构说明  : ASI_MPU_TRUST_WR_PROT0 寄存器结构定义。地址偏移量:0x4C8+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU trust写命令访问protected地址空间配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_trust_wr_prot0 : 32; /* bit[0-31]: MPU trusted写命令访问protected地址空间权限根据MID的选择：
                                                             mid_trust_wr_prot[n]=0：mid为n的trusted写命令访问protected地址空间会被MPU拦截；
                                                             mid_trust_wr_prot[n]=1：mid为n的trusted写命令访问protected地址空间不会被MPU拦截。
                                                             说明：
                                                             1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                             2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_mpu_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_mpu_chk_mid_width=3，则只有mid_trust_wr_prot[7:0]可配。
                                                             3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_UNION;
#endif
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_mid_trust_wr_prot0_START  (0)
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT0_mid_trust_wr_prot0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_UNION
 结构说明  : ASI_MPU_TRUST_WR_PROT1 寄存器结构定义。地址偏移量:0x4CC+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU trust写命令访问protected地址空间配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_trust_wr_prot1 : 32; /* bit[0-31]: MPU trusted写命令访问protected地址空间权限根据MID的选择：
                                                             mid_trust_wr_prot[n]=0：mid为(n+32)的trusted写命令访问protected地址空间会被MPU拦截；
                                                             mid_trust_wr_prot[n]=1：mid为(n+32)的trusted写命令访问protected地址空间不会被MPU拦截。
                                                             说明：
                                                             1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                             2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_mpu_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_mpu_chk_mid_width=3，则只有mid_trust_wr_prot[7:0]可配。
                                                             3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_UNION;
#endif
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_mid_trust_wr_prot1_START  (0)
#define SOC_DMSS_ASI_MPU_TRUST_WR_PROT1_mid_trust_wr_prot1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_UNION
 结构说明  : ASI_MPU_PROT_WR_NTRUST0 寄存器结构定义。地址偏移量:0x4D0+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU protected写命令访问non-trust地址空间配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_prot_wr_ntrust0 : 32; /* bit[0-31]: MPU protected写命令访问non-trusted地址空间权限根据MID的选择：
                                                              mid_prot_wr_ntrust[n]=0：mid为n的protected写命令访问non-trusted地址空间会被MPU拦截；
                                                              mid_prot_wr_ntrust[n]=1：mid为n的protected写命令访问non-trusted地址空间不会被MPU拦截。
                                                              说明：
                                                              1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                              2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_mpu_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_mpu_chk_mid_width=3，则只有mid_prot_wr_ntrust[7:0]可配。
                                                              3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_UNION;
#endif
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_mid_prot_wr_ntrust0_START  (0)
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST0_mid_prot_wr_ntrust0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_UNION
 结构说明  : ASI_MPU_PROT_WR_NTRUST1 寄存器结构定义。地址偏移量:0x4D4+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU protected写命令访问non-trust地址空间配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_prot_wr_ntrust1 : 32; /* bit[0-31]: MPU protected写命令访问non-trusted地址空间权限根据MID的选择：
                                                              mid_prot_wr_ntrust[n]=0：mid为(n+32)的protected写命令访问non-trusted地址空间会被MPU拦截；
                                                              mid_prot_wr_ntrust[n]=1：mid为(n+32)的protected写命令访问non-trusted地址空间不会被MPU拦截。
                                                              说明：
                                                              1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                              2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_mpu_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_mpu_chk_mid_width=3，则只有mid_prot_wr_ntrust[7:0]可配。
                                                              3.此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_UNION;
#endif
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_mid_prot_wr_ntrust1_START  (0)
#define SOC_DMSS_ASI_MPU_PROT_WR_NTRUST1_mid_prot_wr_ntrust1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_RGN_MAP0_UNION
 结构说明  : ASI_SEC_RGN_MAP0 寄存器结构定义。地址偏移量:0x500+0x10*(asi_rgns)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域映射0
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_base_addr : 20; /* bit[0-19] : 各个区域的起始地址设置（高位部分）：
                                                         设置地址64KB以上的高20位，高位地址位宽不足则补0。
                                                         注意：
                                                         1.rgn0不可配置，默认覆盖整个地址空间，其余的rgn之间不允许地址重叠；
                                                         2.起始地址不可配置的低位部分为全0，起始地址包含在当前rgn内；
                                                         3.若base_addr配置大于top_addr，则当前rgn不生效；
                                                         4.需保证exclusive命令访问的地址区域有响应的访问权限，否则exclusive命令无法被正常监控；
                                                         5.可设置的地址分辨率固定为64KB；
                                                         6.若支持FAMA功能，则地址高位会再扩展2bit交织模式信息，需一同作为地址识别。 */
        unsigned int  reserved      : 10; /* bit[20-29]: 保留。 */
        unsigned int  sub_rgn_zone  : 1;  /* bit[30]   : 各个subregion地址颗粒大小：
                                                         0：每个subregion为1MB地址空间；
                                                         1：每个subregion为2MB地址空间；
                                                         注意：
                                                         1.当前区域是否支持此功能与RTL配置有关，需要根据支持subrgn属性的regions个数判定（从ASI_RTL_INF3.rtl_sec_subrgn_num可读出）。
                                                         2.region0不可配置，默认不支持subrgn。 */
        unsigned int  rgn_en        : 1;  /* bit[31]   : 各个区域的使能控制：
                                                         0：不使能；
                                                         1：使能；
                                                         注意：region0不可配置，默认使能。 */
    } reg;
} SOC_DMSS_ASI_SEC_RGN_MAP0_UNION;
#endif
#define SOC_DMSS_ASI_SEC_RGN_MAP0_rgn_base_addr_START  (0)
#define SOC_DMSS_ASI_SEC_RGN_MAP0_rgn_base_addr_END    (19)
#define SOC_DMSS_ASI_SEC_RGN_MAP0_sub_rgn_zone_START   (30)
#define SOC_DMSS_ASI_SEC_RGN_MAP0_sub_rgn_zone_END     (30)
#define SOC_DMSS_ASI_SEC_RGN_MAP0_rgn_en_START         (31)
#define SOC_DMSS_ASI_SEC_RGN_MAP0_rgn_en_END           (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_RGN_MAP1_UNION
 结构说明  : ASI_SEC_RGN_MAP1 寄存器结构定义。地址偏移量:0x504+0x10*(asi_rgns)+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域映射1
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_top_addr : 20; /* bit[0-19] : 各个区域的结束地址设置（高位部分）：
                                                        设置地址64KB以上的高20位，高位地址位宽不足则补0。
                                                        注意：
                                                        1.rgn0不可配置，默认覆盖整个地址空间，其余的rgn之间不允许地址重叠；
                                                        2.结束地址不可配置的低位部分为全1，结束地址包含在当前rgn内；
                                                        3.若base_addr大于top_addr，则当前rgn不使能；
                                                        4.需保证exclusive命令访问的地址区域有响应的访问权限，否则exclusive命令无法被正常监控；
                                                        5.可设置的地址分辨率固定为64KB。 */
        unsigned int  reserved     : 8;  /* bit[20-27]: 保留。 */
        unsigned int  sp           : 4;  /* bit[28-31]: 当前区域的安全权限属性。
                                                        sp[3]：安全读属性；
                                                        sp[2]：安全写属性；
                                                        sp[1]：非安全读属性；
                                                        sp[0]：非安全写属性；
                                                        sp[n]=0：禁止访问；
                                                        sp[n]=1：允许访问。 */
    } reg;
} SOC_DMSS_ASI_SEC_RGN_MAP1_UNION;
#endif
#define SOC_DMSS_ASI_SEC_RGN_MAP1_rgn_top_addr_START  (0)
#define SOC_DMSS_ASI_SEC_RGN_MAP1_rgn_top_addr_END    (19)
#define SOC_DMSS_ASI_SEC_RGN_MAP1_sp_START            (28)
#define SOC_DMSS_ASI_SEC_RGN_MAP1_sp_END              (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_MID_WR_UNION
 结构说明  : ASI_SEC_MID_WR 寄存器结构定义。地址偏移量:0x508+0x10*(asi_rgns)+0x800*(asi_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                     mid_sel_wr[n]=0：写命令MID的低若干位为n时，此命令为MID匹配失败；
                                                     mid_sel_wr[n]=1：写命令MID的低若干位为n时，此命令为MID匹配成功。
                                                     说明：
                                                     1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                     2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_sec_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_sec_chk_mid_width=3，则只有mid_sel_wr[7:0]可配。 */
    } reg;
} SOC_DMSS_ASI_SEC_MID_WR_UNION;
#endif
#define SOC_DMSS_ASI_SEC_MID_WR_mid_sel_wr_START  (0)
#define SOC_DMSS_ASI_SEC_MID_WR_mid_sel_wr_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_SEC_MID_RD_UNION
 结构说明  : ASI_SEC_MID_RD 寄存器结构定义。地址偏移量:0x50C+0x10*(asi_rgns)+0x800*(asi_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
            （被锁定条件：ASI_SEC_LD_SEL中配置选定且sucure_boot_lock信号被拉高过。）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                     mid_sel_rd[n]=0：读命令MID的低若干位为n时，此命令为MID匹配失败；
                                                     mid_sel_rd[n]=1：读命令MID的低若干位为n时，此命令为MID匹配成功。
                                                     说明：
                                                     1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                     2.此寄存器实际可配置的位宽和RTL配置有关（从ASI_RTL_INF2.rtl_sec_chk_mid_width可读出），二者为2的幂次方关系。例如：若rtl_sec_chk_mid_width=3，则只有mid_sel_rd[7:0]可配。 */
    } reg;
} SOC_DMSS_ASI_SEC_MID_RD_UNION;
#endif
#define SOC_DMSS_ASI_SEC_MID_RD_mid_sel_rd_START  (0)
#define SOC_DMSS_ASI_SEC_MID_RD_mid_sel_rd_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_MODULE_UNION
 结构说明  : ASI_DFX_MODULE 寄存器结构定义。地址偏移量:0x700+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：主要模块工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_busy      : 1;  /* bit[0]    : ASI的整体状态：
                                                         0：idle；
                                                         1：busy。
                                                         说明：
                                                         idle时，所有子模块均已idle（未考虑异步桥状态），且所有命令和数据均已处理完毕。 */
        unsigned int  asi_temp_busy : 1;  /* bit[1]    : ASI的整体实时状态：
                                                         0：idle；
                                                         1：busy。
                                                         说明：
                                                         idle时，所有子模块均已idle（未考虑异步桥状态），但可能有读数据还未从后级模块返回。 */
        unsigned int  reserved_0    : 2;  /* bit[2-3]  : 保留。 */
        unsigned int  ra_busy       : 1;  /* bit[4]    : ASI_RA模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  aw_busy       : 1;  /* bit[5]    : ASI_AW模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  ar_busy       : 1;  /* bit[6]    : ASI_AR模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  awcs_busy     : 1;  /* bit[7]    : ASI_AWCS模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  arcs_busy     : 1;  /* bit[8]    : ASI_ARCS模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  wd_busy       : 1;  /* bit[9]    : ASI_WD模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  b_busy        : 1;  /* bit[10]   : ASI_B模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  rd_busy       : 1;  /* bit[11]   : ASI_RD模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  fkrd_busy     : 1;  /* bit[12]   : ASI_FKRD模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  awsec_busy    : 1;  /* bit[13]   : ASI_AWSEC模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  arsec_busy    : 1;  /* bit[14]   : ASI_ARSEC模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  rdr_est_busy  : 1;  /* bit[15]   : RDR_EST模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  rdr_buf_busy  : 1;  /* bit[16]   : RDR_BUF模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  ca_busy       : 1;  /* bit[17]   : CA模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  wooo_busy     : 1;  /* bit[18]   : WOOO模块的状态：
                                                         0：idle；
                                                         1：busy。 */
        unsigned int  reserved_1    : 1;  /* bit[19]   : 保留。 */
        unsigned int  w_waiting     : 1;  /* bit[20]   : 写命令对应的写数据接收状态：
                                                         0：写数据已经全部接收；
                                                         1：等待接收写数据。
                                                         说明：若出现挂死等现象，结合w_bp及本寄存器，可快速指示总线写数据是否有缺失。 */
        unsigned int  reserved_2    : 3;  /* bit[21-23]: 保留。 */
        unsigned int  aw_bp         : 1;  /* bit[24]   : AW通道的实时状态：
                                                         0：非反压；
                                                         1：反压。
                                                         注意：若有异步桥，则此处为异步桥之后的总线状态。 */
        unsigned int  w_bp          : 1;  /* bit[25]   : W通道的实时状态：
                                                         0：非反压；
                                                         1：反压。
                                                         注意：若有异步桥，则此处为异步桥之后的总线状态。 */
        unsigned int  b_bp          : 1;  /* bit[26]   : B通道的实时状态：
                                                         0：非反压；
                                                         1：反压。
                                                         注意：若有异步桥，则此处为异步桥之后的总线状态。 */
        unsigned int  ar_bp         : 1;  /* bit[27]   : AR通道的实时状态：
                                                         0：非反压；
                                                         1：反压。
                                                         注意：若有异步桥，则此处为异步桥之后的总线状态。 */
        unsigned int  r_bp          : 1;  /* bit[28]   : R通道的实时状态：
                                                         0：非反压；
                                                         1：反压。
                                                         注意：若有异步桥，则此处为异步桥之后的总线状态。 */
        unsigned int  reserved_3    : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_MODULE_UNION;
#endif
#define SOC_DMSS_ASI_DFX_MODULE_asi_busy_START       (0)
#define SOC_DMSS_ASI_DFX_MODULE_asi_busy_END         (0)
#define SOC_DMSS_ASI_DFX_MODULE_asi_temp_busy_START  (1)
#define SOC_DMSS_ASI_DFX_MODULE_asi_temp_busy_END    (1)
#define SOC_DMSS_ASI_DFX_MODULE_ra_busy_START        (4)
#define SOC_DMSS_ASI_DFX_MODULE_ra_busy_END          (4)
#define SOC_DMSS_ASI_DFX_MODULE_aw_busy_START        (5)
#define SOC_DMSS_ASI_DFX_MODULE_aw_busy_END          (5)
#define SOC_DMSS_ASI_DFX_MODULE_ar_busy_START        (6)
#define SOC_DMSS_ASI_DFX_MODULE_ar_busy_END          (6)
#define SOC_DMSS_ASI_DFX_MODULE_awcs_busy_START      (7)
#define SOC_DMSS_ASI_DFX_MODULE_awcs_busy_END        (7)
#define SOC_DMSS_ASI_DFX_MODULE_arcs_busy_START      (8)
#define SOC_DMSS_ASI_DFX_MODULE_arcs_busy_END        (8)
#define SOC_DMSS_ASI_DFX_MODULE_wd_busy_START        (9)
#define SOC_DMSS_ASI_DFX_MODULE_wd_busy_END          (9)
#define SOC_DMSS_ASI_DFX_MODULE_b_busy_START         (10)
#define SOC_DMSS_ASI_DFX_MODULE_b_busy_END           (10)
#define SOC_DMSS_ASI_DFX_MODULE_rd_busy_START        (11)
#define SOC_DMSS_ASI_DFX_MODULE_rd_busy_END          (11)
#define SOC_DMSS_ASI_DFX_MODULE_fkrd_busy_START      (12)
#define SOC_DMSS_ASI_DFX_MODULE_fkrd_busy_END        (12)
#define SOC_DMSS_ASI_DFX_MODULE_awsec_busy_START     (13)
#define SOC_DMSS_ASI_DFX_MODULE_awsec_busy_END       (13)
#define SOC_DMSS_ASI_DFX_MODULE_arsec_busy_START     (14)
#define SOC_DMSS_ASI_DFX_MODULE_arsec_busy_END       (14)
#define SOC_DMSS_ASI_DFX_MODULE_rdr_est_busy_START   (15)
#define SOC_DMSS_ASI_DFX_MODULE_rdr_est_busy_END     (15)
#define SOC_DMSS_ASI_DFX_MODULE_rdr_buf_busy_START   (16)
#define SOC_DMSS_ASI_DFX_MODULE_rdr_buf_busy_END     (16)
#define SOC_DMSS_ASI_DFX_MODULE_ca_busy_START        (17)
#define SOC_DMSS_ASI_DFX_MODULE_ca_busy_END          (17)
#define SOC_DMSS_ASI_DFX_MODULE_wooo_busy_START      (18)
#define SOC_DMSS_ASI_DFX_MODULE_wooo_busy_END        (18)
#define SOC_DMSS_ASI_DFX_MODULE_w_waiting_START      (20)
#define SOC_DMSS_ASI_DFX_MODULE_w_waiting_END        (20)
#define SOC_DMSS_ASI_DFX_MODULE_aw_bp_START          (24)
#define SOC_DMSS_ASI_DFX_MODULE_aw_bp_END            (24)
#define SOC_DMSS_ASI_DFX_MODULE_w_bp_START           (25)
#define SOC_DMSS_ASI_DFX_MODULE_w_bp_END             (25)
#define SOC_DMSS_ASI_DFX_MODULE_b_bp_START           (26)
#define SOC_DMSS_ASI_DFX_MODULE_b_bp_END             (26)
#define SOC_DMSS_ASI_DFX_MODULE_ar_bp_START          (27)
#define SOC_DMSS_ASI_DFX_MODULE_ar_bp_END            (27)
#define SOC_DMSS_ASI_DFX_MODULE_r_bp_START           (28)
#define SOC_DMSS_ASI_DFX_MODULE_r_bp_END             (28)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_FIFO_UNION
 结构说明  : ASI_DFX_FIFO 寄存器结构定义。地址偏移量:0x704+0x800*(asi_base)，初值:0x00540555，宽度:32
 寄存器说明: DFX：FIFO工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ra_wfifo_st    : 2;  /* bit[0-1]  : ASI_RA模块中写数据FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  wd_cmdfifo_st  : 2;  /* bit[2-3]  : ASI_WD模块中写命令FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  b_respfifo_st  : 2;  /* bit[4-5]  : ASI_B模块中写响应FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  rd_rfifo_st    : 2;  /* bit[6-7]  : ASI_RD模块中读数据FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  b_mififo_st    : 2;  /* bit[8-9]  : ASI_B模块中写命令在DMI或AMI获得仲裁标志的FIFO的状态：
                                                          0x0：busy；
                                                          0x1：full；
                                                          0x2：emtpy；
                                                          0x3：reserved。 */
        unsigned int  fkrd_fkfifo_st : 2;  /* bit[10-11]: ASI_FKRD模块中假读命令FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  reserved_0     : 6;  /* bit[12-17]: 保留。 */
        unsigned int  ami_wfifo_st   : 2;  /* bit[18-19]: AMI模块中的写数据FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。
                                                          注意：
                                                          PTRN内ASI中此寄存器域段固定为0，实际状态可查询AMI_DFX_WD_FIFO。 */
        unsigned int  dmi0_wfifo_st  : 2;  /* bit[20-21]: DMI0模块中的写数据FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。
                                                          注意：
                                                          PTRN内ASI中此寄存器域段固定为0，实际状态可查询DMI_DFX_WD_FIFO。 */
        unsigned int  dmi1_wfifo_st  : 2;  /* bit[22-23]: DMI1模块中的写数据FIFO的状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。
                                                          注意：
                                                          PTRN内ASI中此寄存器域段固定为0，实际状态可查询DMI_DFX_WD_FIFO。 */
        unsigned int  reserved_1     : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_FIFO_UNION;
#endif
#define SOC_DMSS_ASI_DFX_FIFO_ra_wfifo_st_START     (0)
#define SOC_DMSS_ASI_DFX_FIFO_ra_wfifo_st_END       (1)
#define SOC_DMSS_ASI_DFX_FIFO_wd_cmdfifo_st_START   (2)
#define SOC_DMSS_ASI_DFX_FIFO_wd_cmdfifo_st_END     (3)
#define SOC_DMSS_ASI_DFX_FIFO_b_respfifo_st_START   (4)
#define SOC_DMSS_ASI_DFX_FIFO_b_respfifo_st_END     (5)
#define SOC_DMSS_ASI_DFX_FIFO_rd_rfifo_st_START     (6)
#define SOC_DMSS_ASI_DFX_FIFO_rd_rfifo_st_END       (7)
#define SOC_DMSS_ASI_DFX_FIFO_b_mififo_st_START     (8)
#define SOC_DMSS_ASI_DFX_FIFO_b_mififo_st_END       (9)
#define SOC_DMSS_ASI_DFX_FIFO_fkrd_fkfifo_st_START  (10)
#define SOC_DMSS_ASI_DFX_FIFO_fkrd_fkfifo_st_END    (11)
#define SOC_DMSS_ASI_DFX_FIFO_ami_wfifo_st_START    (18)
#define SOC_DMSS_ASI_DFX_FIFO_ami_wfifo_st_END      (19)
#define SOC_DMSS_ASI_DFX_FIFO_dmi0_wfifo_st_START   (20)
#define SOC_DMSS_ASI_DFX_FIFO_dmi0_wfifo_st_END     (21)
#define SOC_DMSS_ASI_DFX_FIFO_dmi1_wfifo_st_START   (22)
#define SOC_DMSS_ASI_DFX_FIFO_dmi1_wfifo_st_END     (23)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_FSM_UNION
 结构说明  : ASI_DFX_FSM 寄存器结构定义。地址偏移量:0x708+0x800*(asi_base)，初值:0x00010101，宽度:32
 寄存器说明: DFX：状态机工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  arcs_fsm_st : 8;  /* bit[0-7]  : ASI_ARCS模块中命令状态机的跳转状态：
                                                       0x001：st_idle；
                                                       0x002：st_ex_chk；
                                                       0x004：st_ex_wait；
                                                       0x008：st_ex_rw；
                                                       0x010：st_nar；
                                                       0x020：st_wrap；
                                                       0x040：st_realign；
                                                       其他：reserve。 */
        unsigned int  awcs_fsm_st : 8;  /* bit[8-15] : ASI_AWCS模块中命令状态机的跳转状态：
                                                       0x001：st_idle；
                                                       0x002：st_ex_chk；
                                                       0x004：st_ex_wait；
                                                       0x008：st_ex_rw；
                                                       0x010：st_nar；
                                                       0x020：st_wrap；
                                                       0x040：st_realign；
                                                       其他：reserve。 */
        unsigned int  rd_fsm_st   : 5;  /* bit[16-20]: ASI_RD模块中命令状态机的跳转状态：
                                                       0x01：st_idle；
                                                       0x02：st_normal；
                                                       0x04：st_low；
                                                       0x08：st_high；
                                                       0x10：st_wrap；
                                                       其他：reserve。 */
        unsigned int  reserved_0  : 3;  /* bit[21-23]: 保留。 */
        unsigned int  wd_fsm_st   : 6;  /* bit[24-29]: ASI_WD模块中命令状态机的跳转状态：
                                                       0x00：st_idle；
                                                       0x01：st_normal；
                                                       0x02：st_low；
                                                       0x04：st_high；
                                                       0x08：st_error；
                                                       0x10：st_exfail；
                                                       其他：reserve。 */
        unsigned int  reserved_1  : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_FSM_UNION;
#endif
#define SOC_DMSS_ASI_DFX_FSM_arcs_fsm_st_START  (0)
#define SOC_DMSS_ASI_DFX_FSM_arcs_fsm_st_END    (7)
#define SOC_DMSS_ASI_DFX_FSM_awcs_fsm_st_START  (8)
#define SOC_DMSS_ASI_DFX_FSM_awcs_fsm_st_END    (15)
#define SOC_DMSS_ASI_DFX_FSM_rd_fsm_st_START    (16)
#define SOC_DMSS_ASI_DFX_FSM_rd_fsm_st_END      (20)
#define SOC_DMSS_ASI_DFX_FSM_wd_fsm_st_START    (24)
#define SOC_DMSS_ASI_DFX_FSM_wd_fsm_st_END      (29)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_OSTD_UNION
 结构说明  : ASI_DFX_OSTD 寄存器结构定义。地址偏移量:0x70C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：OSTD状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_ostd_st     : 6;  /* bit[0-5]  : 写命令outstanding统计：
                                                          注意：
                                                          由于DMSS中DMI方向是以fast-response方式处理写命令的，因而此处统计的outstanding不能代表真正的写命令及写数据的处理状态。例如，此处统计为零时，不代表所有写命令或写数据已被后级模块处理完毕。 */
        unsigned int  reserved       : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  rd_ostd_st     : 8;  /* bit[8-15] : 读命令outstanding统计。 */
        unsigned int  wr_dmi_ostd_st : 8;  /* bit[16-23]: DMI方向(包括Syscache方向)的写命令outstanding统计：
                                                          注意：
                                                          由于DMSS中DMI方向是以fast-response方式处理写命令的，因而此处统计的outstanding不能代表真正的写命令及写数据的处理状态。例如，此处统计为零时，不代表所有写命令或写数据已被后级模块处理完毕。 */
        unsigned int  rd_dmi_ostd_st : 8;  /* bit[24-31]: DMI方向(包括Syscache方向)的读命令outstanding统计。 */
    } reg;
} SOC_DMSS_ASI_DFX_OSTD_UNION;
#endif
#define SOC_DMSS_ASI_DFX_OSTD_wr_ostd_st_START      (0)
#define SOC_DMSS_ASI_DFX_OSTD_wr_ostd_st_END        (5)
#define SOC_DMSS_ASI_DFX_OSTD_rd_ostd_st_START      (8)
#define SOC_DMSS_ASI_DFX_OSTD_rd_ostd_st_END        (15)
#define SOC_DMSS_ASI_DFX_OSTD_wr_dmi_ostd_st_START  (16)
#define SOC_DMSS_ASI_DFX_OSTD_wr_dmi_ostd_st_END    (23)
#define SOC_DMSS_ASI_DFX_OSTD_rd_dmi_ostd_st_START  (24)
#define SOC_DMSS_ASI_DFX_OSTD_rd_dmi_ostd_st_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_RDR0_UNION
 结构说明  : ASI_DFX_RDR0 寄存器结构定义。地址偏移量:0x710+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：REORDER模块状态0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rdr_cmd_que_cnt_st : 8;  /* bit[0-7]  : REORDER模块命令队列中的命令个数。 */
        unsigned int  rdr_buf_cnt_st     : 8;  /* bit[8-15] : REORDER模块数据buffer中的数据burst个数。 */
        unsigned int  rdr_bid_use_cnt_st : 8;  /* bit[16-23]: REORDER模块已分配的buffer ID个数。 */
        unsigned int  reserved           : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_RDR0_UNION;
#endif
#define SOC_DMSS_ASI_DFX_RDR0_rdr_cmd_que_cnt_st_START  (0)
#define SOC_DMSS_ASI_DFX_RDR0_rdr_cmd_que_cnt_st_END    (7)
#define SOC_DMSS_ASI_DFX_RDR0_rdr_buf_cnt_st_START      (8)
#define SOC_DMSS_ASI_DFX_RDR0_rdr_buf_cnt_st_END        (15)
#define SOC_DMSS_ASI_DFX_RDR0_rdr_bid_use_cnt_st_START  (16)
#define SOC_DMSS_ASI_DFX_RDR0_rdr_bid_use_cnt_st_END    (23)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_RDR1_UNION
 结构说明  : ASI_DFX_RDR1 寄存器结构定义。地址偏移量:0x714+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：REORDER模块状态1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rdr_buf_rcv_done_st : 32; /* bit[0-31]: REORDER模块数据buffer中的数据burst是否接收完毕：
                                                              rdr_buf_rcv_done_st[n]=0：数据buffer第n个entry未分配burst或者对应burst数据未接收完毕；
                                                              rdr_buf_rcv_done_st[n]=1：数据buffer第n个entry对应burst数据接收完毕。 */
    } reg;
} SOC_DMSS_ASI_DFX_RDR1_UNION;
#endif
#define SOC_DMSS_ASI_DFX_RDR1_rdr_buf_rcv_done_st_START  (0)
#define SOC_DMSS_ASI_DFX_RDR1_rdr_buf_rcv_done_st_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_WOOO_UNION
 结构说明  : ASI_DFX_WOOO 寄存器结构定义。地址偏移量:0x718+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：WOOO模块状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wooo_que_cnt : 8;  /* bit[0-7]  : WOOO模块命令队列中的命令个数。 */
        unsigned int  wooo_buf_cnt : 8;  /* bit[8-15] : WOOO模块中已缓存数据buffer entry个数。 */
        unsigned int  reserved     : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_WOOO_UNION;
#endif
#define SOC_DMSS_ASI_DFX_WOOO_wooo_que_cnt_START  (0)
#define SOC_DMSS_ASI_DFX_WOOO_wooo_que_cnt_END    (7)
#define SOC_DMSS_ASI_DFX_WOOO_wooo_buf_cnt_START  (8)
#define SOC_DMSS_ASI_DFX_WOOO_wooo_buf_cnt_END    (15)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_INT_EN_UNION
 结构说明  : ASI_DFX_INT_EN 寄存器结构定义。地址偏移量:0x720+0x800*(asi_base)，初值:0x00000001，宽度:32
 寄存器说明: DFX：异常命令信息中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dfx_int_en : 1;  /* bit[0]   : 异常命令的中断输出使能：
                                                     0：禁止；
                                                     1：使能。
                                                     说明：
                                                     1.当有异常访问时，可上报中断并记录异常命令的相关信息(查询ASI_DFX_ABNM_INFO寄存器)；
                                                     2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及异常命令信息。 */
        unsigned int  reserved   : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_INT_EN_UNION;
#endif
#define SOC_DMSS_ASI_DFX_INT_EN_dfx_int_en_START  (0)
#define SOC_DMSS_ASI_DFX_INT_EN_dfx_int_en_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_INT_STATUS_UNION
 结构说明  : ASI_DFX_INT_STATUS 寄存器结构定义。地址偏移量:0x724+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：异常命令信息中断
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dfx_int_status : 1;  /* bit[0]   : 中断状态指示：
                                                         0：中断未产生；
                                                         1：中断已产生。 */
        unsigned int  reserved       : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_INT_STATUS_UNION;
#endif
#define SOC_DMSS_ASI_DFX_INT_STATUS_dfx_int_status_START  (0)
#define SOC_DMSS_ASI_DFX_INT_STATUS_dfx_int_status_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_ABNM_INF0_UNION
 结构说明  : ASI_DFX_ABNM_INF0 寄存器结构定义。地址偏移量:0x730+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：异常命令信息记录0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  abnm_link_err  : 1;  /* bit[0]    : 路由错误：命令被发往了未连接的DMI（可能会导致系统挂死）。 */
        unsigned int  abnm_rdr_split : 1;  /* bit[1]    : 保序错误：未例化保序模块的情况下，读命令被拆分（可能会导致系统挂死）。 */
        unsigned int  abnm_ex_len    : 1;  /* bit[2]    : 违反命令约束：收到了非INCR1的exclusive命令（可能会导致exclusive命令无效）。 */
        unsigned int  abnm_nar_wrap  : 1;  /* bit[3]    : 违反命令约束：收到了narrow类型的WRAP命令（可能会导致系统挂死）。 */
        unsigned int  abnm_fix       : 1;  /* bit[4]    : 违反命令约束：收到了FIX类型的命令（强制转换成INCR命令处理）。 */
        unsigned int  abnm_len16     : 1;  /* bit[5]    : 违反命令约束：收到了大于burst16的命令（可能会导致系统挂死）。 */
        unsigned int  reserved_0     : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  abnm_cmd_len   : 4;  /* bit[8-11] : 首个异常命令的len：
                                                          注意：
                                                          1.当中断清除，并产生下一次新的中断时，此寄存器信息才会被更新；
                                                          2.abnm_rdr_split/link_err/len16/wd_mismatch类型的错误，不记录len信息。 */
        unsigned int  abnm_cmd_size  : 3;  /* bit[12-14]: 首个异常命令的size：
                                                          注意：
                                                          1.当中断清除，并产生下一次新的中断时，此寄存器信息才会被更新；
                                                          2.abnm_rdr_split/link_err/wd_mismatch类型的错误，不记录size信息。 */
        unsigned int  reserved_1     : 1;  /* bit[15]   : 保留。 */
        unsigned int  abnm_cmd_mid   : 15; /* bit[16-30]: 首个异常命令的MasterID：
                                                          注意：当中断清除，并产生下一次新的中断时，此寄存器信息才会被更新。 */
        unsigned int  abnm_cmd_type  : 1;  /* bit[31]   : 首个异常命令的读写类型：
                                                          0：读命令；
                                                          1：写命令。
                                                          注意：
                                                          当中断清除，并产生下一次新的中断时，此寄存器信息才会被更新。 */
    } reg;
} SOC_DMSS_ASI_DFX_ABNM_INF0_UNION;
#endif
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_link_err_START   (0)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_link_err_END     (0)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_rdr_split_START  (1)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_rdr_split_END    (1)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_ex_len_START     (2)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_ex_len_END       (2)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_nar_wrap_START   (3)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_nar_wrap_END     (3)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_fix_START        (4)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_fix_END          (4)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_len16_START      (5)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_len16_END        (5)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_len_START    (8)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_len_END      (11)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_size_START   (12)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_size_END     (14)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_mid_START    (16)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_mid_END      (30)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_type_START   (31)
#define SOC_DMSS_ASI_DFX_ABNM_INF0_abnm_cmd_type_END     (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_ABNM_INF1_UNION
 结构说明  : ASI_DFX_ABNM_INF1 寄存器结构定义。地址偏移量:0x734+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：异常命令信息记录1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  abnm_wd_mismatch   : 1;  /* bit[0]    : 违反命令约束：写数据个数和写命令awlen不匹配（可能会导致系统挂死）。
                                                              说明：
                                                              可结合target_awlen和cur_wd_cnt查看写命令的length和当前已接收的写数据个数。 */
        unsigned int  reserved_0         : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  target_awlen       : 4;  /* bit[4-7]  : 当前正在接受的写数据对应写命令awlen。
                                                              说明：
                                                              若abnm_wd_mismatch拉高，则可通过比较target_awlen和cur_wd_cnt，确认是写数据是漏发还是多发。 */
        unsigned int  cur_wd_cnt         : 4;  /* bit[8-11] : 当前正在接受的写数据的transaction个数。
                                                              说明：
                                                              1.从零计数，正常情况下最大值为对应写命令的awlen；
                                                              2.若abnm_wd_mismatch拉高，则可通过比较target_awlen和cur_wd_cnt，确认是写数据是漏发还是多发。 */
        unsigned int  reserved_1         : 4;  /* bit[12-15]: 保留。 */
        unsigned int  abnm_cmd_addr_high : 16; /* bit[16-31]: 首个异常命令的地址（这里只记录地址的高位部分，低32位信息在寄存器ASI_DFX_ABNM_INF2中）
                                                              注意：
                                                              当中断清除，并产生下一次新的中断时，此寄存器信息才会被更新。 */
    } reg;
} SOC_DMSS_ASI_DFX_ABNM_INF1_UNION;
#endif
#define SOC_DMSS_ASI_DFX_ABNM_INF1_abnm_wd_mismatch_START    (0)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_abnm_wd_mismatch_END      (0)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_target_awlen_START        (4)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_target_awlen_END          (7)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_cur_wd_cnt_START          (8)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_cur_wd_cnt_END            (11)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_abnm_cmd_addr_high_START  (16)
#define SOC_DMSS_ASI_DFX_ABNM_INF1_abnm_cmd_addr_high_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_ABNM_INF2_UNION
 结构说明  : ASI_DFX_ABNM_INF2 寄存器结构定义。地址偏移量:0x738+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：异常命令信息记录2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  abnm_cmd_addr_low : 32; /* bit[0-31]: 首个异常命令的地址（这里只记录地址的低32bits，高位信息在寄存器ASI_DFX_ABNM_INF1中）
                                                            注意：
                                                            当中断清除，并产生下一次新的中断时，此寄存器信息才会被更新。 */
    } reg;
} SOC_DMSS_ASI_DFX_ABNM_INF2_UNION;
#endif
#define SOC_DMSS_ASI_DFX_ABNM_INF2_abnm_cmd_addr_low_START  (0)
#define SOC_DMSS_ASI_DFX_ABNM_INF2_abnm_cmd_addr_low_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_SEC0_UNION
 结构说明  : ASI_DFX_SEC0 寄存器结构定义。地址偏移量:0x740+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：安全中断状态
            （为便于调试，将寄存器ASI_SEC_INT_STATUS复制到此）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。
                                                      注意：从GLOBAL模块的GLB_INT_STATUS寄存器中，可识别出中断来自于哪个ASI中的SEC模块。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_SEC0_UNION;
#endif
#define SOC_DMSS_ASI_DFX_SEC0_int_status_START   (0)
#define SOC_DMSS_ASI_DFX_SEC0_int_status_END     (0)
#define SOC_DMSS_ASI_DFX_SEC0_int_overrun_START  (1)
#define SOC_DMSS_ASI_DFX_SEC0_int_overrun_END    (1)
#define SOC_DMSS_ASI_DFX_SEC0_int_cnt_START      (4)
#define SOC_DMSS_ASI_DFX_SEC0_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_SEC1_UNION
 结构说明  : ASI_DFX_SEC1 寄存器结构定义。地址偏移量:0x744+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：记录首个越权命令的信息0
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF0复制到此）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_low : 32; /* bit[0-31]: 中断状态清除前的首个越权命令的低32位地址。 */
    } reg;
} SOC_DMSS_ASI_DFX_SEC1_UNION;
#endif
#define SOC_DMSS_ASI_DFX_SEC1_address_low_START  (0)
#define SOC_DMSS_ASI_DFX_SEC1_address_low_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_SEC2_UNION
 结构说明  : ASI_DFX_SEC2 寄存器结构定义。地址偏移量:0x748+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：记录首个越权命令的信息1
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF1复制到此）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_high : 8;  /* bit[0-7]  : 中断状态清除前的首个越权命令的高位地址（如果存在高位地址）。 */
        unsigned int  reserved     : 8;  /* bit[8-15] : 保留。 */
        unsigned int  privileged   : 1;  /* bit[16]   : 中断状态清除前首个越权命令的特权模式信息：
                                                        0：unprivileged访问；
                                                        1：privileged访问。 */
        unsigned int  nonsecure    : 1;  /* bit[17]   : 中断状态清除前首个越权命令的安全模式信息：
                                                        0：secure访问；
                                                        1：non-secure访问。 */
        unsigned int  access_burst : 2;  /* bit[18-19]: 中断状态清除前首个越权命令的burst信息：
                                                        0：FIXED；
                                                        1：INCR；
                                                        2：WRAP；
                                                        3：reserved。 */
        unsigned int  access_type  : 1;  /* bit[20]   : 中断状态清除前首个越权命令的读写模式信息：
                                                        0：read访问；
                                                        1：write访问。 */
        unsigned int  access_size  : 3;  /* bit[21-23]: 中断状态清除前首个越权命令的size信息：
                                                        0：8bits；
                                                        1：16bits；
                                                        2：32bits；
                                                        3：64bits；
                                                        4：128bits。 */
        unsigned int  access_len   : 4;  /* bit[24-27]: 中断状态清除前首个越权命令的length信息：
                                                        0~15：burst1~burst15。 */
        unsigned int  access_cache : 4;  /* bit[28-31]: 中断状态清除前首个越权命令的cache信息。 */
    } reg;
} SOC_DMSS_ASI_DFX_SEC2_UNION;
#endif
#define SOC_DMSS_ASI_DFX_SEC2_address_high_START  (0)
#define SOC_DMSS_ASI_DFX_SEC2_address_high_END    (7)
#define SOC_DMSS_ASI_DFX_SEC2_privileged_START    (16)
#define SOC_DMSS_ASI_DFX_SEC2_privileged_END      (16)
#define SOC_DMSS_ASI_DFX_SEC2_nonsecure_START     (17)
#define SOC_DMSS_ASI_DFX_SEC2_nonsecure_END       (17)
#define SOC_DMSS_ASI_DFX_SEC2_access_burst_START  (18)
#define SOC_DMSS_ASI_DFX_SEC2_access_burst_END    (19)
#define SOC_DMSS_ASI_DFX_SEC2_access_type_START   (20)
#define SOC_DMSS_ASI_DFX_SEC2_access_type_END     (20)
#define SOC_DMSS_ASI_DFX_SEC2_access_size_START   (21)
#define SOC_DMSS_ASI_DFX_SEC2_access_size_END     (23)
#define SOC_DMSS_ASI_DFX_SEC2_access_len_START    (24)
#define SOC_DMSS_ASI_DFX_SEC2_access_len_END      (27)
#define SOC_DMSS_ASI_DFX_SEC2_access_cache_START  (28)
#define SOC_DMSS_ASI_DFX_SEC2_access_cache_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_SEC3_UNION
 结构说明  : ASI_DFX_SEC3 寄存器结构定义。地址偏移量:0x74C+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：记录首个越权命令的信息2
            （为便于调试，将寄存器ASI_SEC_FAIL_CMD_INF2复制到此）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  id  : 24; /* bit[0-23] : 中断状态清除前的首个越权命令的命令ID。 */
        unsigned int  mid : 8;  /* bit[24-31]: 中断状态清除前的首个越权命令的命令MasterID。 */
    } reg;
} SOC_DMSS_ASI_DFX_SEC3_UNION;
#endif
#define SOC_DMSS_ASI_DFX_SEC3_id_START   (0)
#define SOC_DMSS_ASI_DFX_SEC3_id_END     (23)
#define SOC_DMSS_ASI_DFX_SEC3_mid_START  (24)
#define SOC_DMSS_ASI_DFX_SEC3_mid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_ASI_DFX_LOAD_UNION
 结构说明  : ASI_DFX_LOAD 寄存器结构定义。地址偏移量:0x760+0x800*(asi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：仿真流量冲击状态
            （若RTL中配置DMSS_SIM_DFX_EN为0（通常ASIC版本配置为0，FPGA和EMULATOR版本定义为1），则此寄存器不存在）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ra_wfifo_load_st    : 1;  /* bit[0]    : ASI_RA模块中写数据FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  wd_cmdfifo_load_st  : 1;  /* bit[1]    : ASI_WD模块中写命令FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  b_respfifo_load_st  : 1;  /* bit[2]    : ASI_B模块中写响应FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  rd_rfifo_load_st    : 1;  /* bit[3]    : ASI_RD模块中读数据FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  reserved_0          : 1;  /* bit[4]    : 保留。 */
        unsigned int  fkrd_fkfifo_load_st : 1;  /* bit[5]    : ASI_FKRD模块中假读命令FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  rdr_cmd_que_load_st : 1;  /* bit[6]    : REORDER模块中命令队列的仿真负载状态：
                                                               0x0：命令队列未达到过full状态；
                                                               0x1：命令队列达到过full状态。 */
        unsigned int  rdr_bid_use_load_st : 1;  /* bit[7]    : REORDER模块中已分配的buffer ID个数的仿真负载状态：
                                                               0x0：未达到过buffer ID分配耗尽状态；
                                                               0x1：达到过buffer ID分配耗尽状态。 */
        unsigned int  dmi_wfifo0_load_st  : 1;  /* bit[8]    : DMI0模块中通道0的写数据FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  dmi_wfifo1_load_st  : 1;  /* bit[9]    : DMI0模块中通道1的写数据FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  dmi_wfifo2_load_st  : 1;  /* bit[10]   : DMI1模块中通道0的写数据FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  dmi_wfifo3_load_st  : 1;  /* bit[11]   : DMI1模块中通道1的写数据FIFO的仿真负载状态：
                                                               0x0：fifo未达到过full状态；
                                                               0x1：fifo达到过full状态。 */
        unsigned int  reserved_1          : 20; /* bit[12-31]: 保留。 */
    } reg;
} SOC_DMSS_ASI_DFX_LOAD_UNION;
#endif
#define SOC_DMSS_ASI_DFX_LOAD_ra_wfifo_load_st_START     (0)
#define SOC_DMSS_ASI_DFX_LOAD_ra_wfifo_load_st_END       (0)
#define SOC_DMSS_ASI_DFX_LOAD_wd_cmdfifo_load_st_START   (1)
#define SOC_DMSS_ASI_DFX_LOAD_wd_cmdfifo_load_st_END     (1)
#define SOC_DMSS_ASI_DFX_LOAD_b_respfifo_load_st_START   (2)
#define SOC_DMSS_ASI_DFX_LOAD_b_respfifo_load_st_END     (2)
#define SOC_DMSS_ASI_DFX_LOAD_rd_rfifo_load_st_START     (3)
#define SOC_DMSS_ASI_DFX_LOAD_rd_rfifo_load_st_END       (3)
#define SOC_DMSS_ASI_DFX_LOAD_fkrd_fkfifo_load_st_START  (5)
#define SOC_DMSS_ASI_DFX_LOAD_fkrd_fkfifo_load_st_END    (5)
#define SOC_DMSS_ASI_DFX_LOAD_rdr_cmd_que_load_st_START  (6)
#define SOC_DMSS_ASI_DFX_LOAD_rdr_cmd_que_load_st_END    (6)
#define SOC_DMSS_ASI_DFX_LOAD_rdr_bid_use_load_st_START  (7)
#define SOC_DMSS_ASI_DFX_LOAD_rdr_bid_use_load_st_END    (7)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo0_load_st_START   (8)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo0_load_st_END     (8)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo1_load_st_START   (9)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo1_load_st_END     (9)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo2_load_st_START   (10)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo2_load_st_END     (10)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo3_load_st_START   (11)
#define SOC_DMSS_ASI_DFX_LOAD_dmi_wfifo3_load_st_END     (11)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_RTL_INF0_UNION
 结构说明  : DMI_RTL_INF0 寄存器结构定义。地址偏移量:0x10000，初值:0x00000000，宽度:32
 寄存器说明: DMI代码配置信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_sec_rgn_num      : 8;  /* bit[0-7]  : DMI安全模块的所有regions个数：
                                                                8'd0：无安全模块；
                                                                8'd1：1regions；
                                                                8'd2：2regions；
                                                                8'd3：3regions；
                                                                ……
                                                                注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  rtl_mid_sec_rgn_num  : 8;  /* bit[8-15] : DMI安全模块的支持MID匹配的regions个数：
                                                                8'd0：无安全模块；
                                                                8'd1：1regions；
                                                                8'd2：2regions；
                                                                8'd3：3regions；
                                                                ……
                                                                注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  rtl_enhn_sec_rgn_num : 4;  /* bit[16-19]: DMI安全模块的支持enhance trusted属性的regions个数：
                                                                8'd0：无安全模块；
                                                                8'd1：1regions；
                                                                8'd2：2regions；
                                                                8'd3：3regions；
                                                                ……
                                                                注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  reserved_0           : 4;  /* bit[20-23]: 保留。 */
        unsigned int  rtl_mpu_en           : 1;  /* bit[24]   : MPU鉴权拦截功能：
                                                                0：不支持；
                                                                1：支持。
                                                                注意：
                                                                当不支持MPU鉴权拦截功能时，MPU相关寄存器无效。 */
        unsigned int  reserved_1           : 7;  /* bit[25-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_RTL_INF0_UNION;
#endif
#define SOC_DMSS_DMI_RTL_INF0_rtl_sec_rgn_num_START       (0)
#define SOC_DMSS_DMI_RTL_INF0_rtl_sec_rgn_num_END         (7)
#define SOC_DMSS_DMI_RTL_INF0_rtl_mid_sec_rgn_num_START   (8)
#define SOC_DMSS_DMI_RTL_INF0_rtl_mid_sec_rgn_num_END     (15)
#define SOC_DMSS_DMI_RTL_INF0_rtl_enhn_sec_rgn_num_START  (16)
#define SOC_DMSS_DMI_RTL_INF0_rtl_enhn_sec_rgn_num_END    (19)
#define SOC_DMSS_DMI_RTL_INF0_rtl_mpu_en_START            (24)
#define SOC_DMSS_DMI_RTL_INF0_rtl_mpu_en_END              (24)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_DYN_CKG_UNION
 结构说明  : DMI_DYN_CKG 寄存器结构定义。地址偏移量:0x10010，初值:0x00000000，宽度:32
 寄存器说明: DMI动态时钟门控
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ckg_byp_dmi        : 1;  /* bit[0]   : DMI模块的动态时钟门控：
                                                             0：模块idle时，自动关闭时钟；
                                                             1：时钟常开。 */
        unsigned int  ckg_byp_dmi_wdfifo : 1;  /* bit[1]   : DMI模块中写数据fifo的动态时钟门控：
                                                             0：模块idle时，自动关闭时钟；
                                                             1：时钟常开。 */
        unsigned int  ckg_byp_dmi_sec    : 1;  /* bit[2]   : DMI SEC模块的动态时钟门控：
                                                             0：模块idle时，自动关闭时钟；
                                                             1：时钟常开。 */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_DYN_CKG_UNION;
#endif
#define SOC_DMSS_DMI_DYN_CKG_ckg_byp_dmi_START         (0)
#define SOC_DMSS_DMI_DYN_CKG_ckg_byp_dmi_END           (0)
#define SOC_DMSS_DMI_DYN_CKG_ckg_byp_dmi_wdfifo_START  (1)
#define SOC_DMSS_DMI_DYN_CKG_ckg_byp_dmi_wdfifo_END    (1)
#define SOC_DMSS_DMI_DYN_CKG_ckg_byp_dmi_sec_START     (2)
#define SOC_DMSS_DMI_DYN_CKG_ckg_byp_dmi_sec_END       (2)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_RD_ADPT_UNION
 结构说明  : DMI_RD_ADPT 寄存器结构定义。地址偏移量:0x10020，初值:0x00000000，宽度:32
 寄存器说明: 读命令DMC优先级自适应周期
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_adpt_pri0 : 4;  /* bit[0-3]  : 优先级为0的DMC读命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  rd_adpt_pri1 : 4;  /* bit[4-7]  : 优先级为1的DMC读命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  rd_adpt_pri2 : 4;  /* bit[8-11] : 优先级为2的DMC读命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  rd_adpt_pri3 : 4;  /* bit[12-15]: 优先级为3的DMC读命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  rd_adpt_pri4 : 4;  /* bit[16-19]: 优先级为4的DMC读命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  rd_adpt_pri5 : 4;  /* bit[20-23]: 优先级为5的DMC读命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  reserved     : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_RD_ADPT_UNION;
#endif
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri0_START  (0)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri0_END    (3)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri1_START  (4)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri1_END    (7)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri2_START  (8)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri2_END    (11)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri3_START  (12)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri3_END    (15)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri4_START  (16)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri4_END    (19)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri5_START  (20)
#define SOC_DMSS_DMI_RD_ADPT_rd_adpt_pri5_END    (23)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_WR_ADPT_UNION
 结构说明  : DMI_WR_ADPT 寄存器结构定义。地址偏移量:0x10024，初值:0x00000000，宽度:32
 寄存器说明: 写命令DMC优先级自适应周期
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_adpt_pri0 : 4;  /* bit[0-3]  : 优先级为0的DMC写命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  wr_adpt_pri1 : 4;  /* bit[4-7]  : 优先级为1的DMC写命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  wr_adpt_pri2 : 4;  /* bit[8-11] : 优先级为2的DMC写命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  wr_adpt_pri3 : 4;  /* bit[12-15]: 优先级为3的DMC写命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  wr_adpt_pri4 : 4;  /* bit[16-19]: 优先级为4的DMC写命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  wr_adpt_pri5 : 4;  /* bit[20-23]: 优先级为5的DMC写命令自适应周期配置：
                                                        0x0：禁止优先级自适应功能；
                                                        0x1～0xF：n*zoom个时钟周期(zoom是放大倍数，请参考DMC寄存器)。
                                                        说明：
                                                        命令被阻塞时，每当自适应周期到达后，命令优先级自动提升一级，直到提升至次最高优先级（优先级6）。 */
        unsigned int  reserved     : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_WR_ADPT_UNION;
#endif
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri0_START  (0)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri0_END    (3)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri1_START  (4)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri1_END    (7)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri2_START  (8)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri2_END    (11)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri3_START  (12)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri3_END    (15)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri4_START  (16)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri4_END    (19)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri5_START  (20)
#define SOC_DMSS_DMI_WR_ADPT_wr_adpt_pri5_END    (23)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_RD_TIMEOUT0_UNION
 结构说明  : DMI_RD_TIMEOUT0 寄存器结构定义。地址偏移量:0x10030，初值:0x00000000，宽度:32
 寄存器说明: 读命令DMC TIMEOUT映射表0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_timeout_pri0 : 6;  /* bit[0-5]  : 优先级为0的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_0      : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  rd_timeout_pri1 : 6;  /* bit[8-13] : 优先级为1的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  rd_timeout_pri2 : 6;  /* bit[16-21]: 优先级为2的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_2      : 2;  /* bit[22-23]: 保留。 */
        unsigned int  rd_timeout_pri3 : 6;  /* bit[24-29]: 优先级为3的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_3      : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_RD_TIMEOUT0_UNION;
#endif
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri0_START  (0)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri0_END    (5)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri1_START  (8)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri1_END    (13)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri2_START  (16)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri2_END    (21)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri3_START  (24)
#define SOC_DMSS_DMI_RD_TIMEOUT0_rd_timeout_pri3_END    (29)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_RD_TIMEOUT1_UNION
 结构说明  : DMI_RD_TIMEOUT1 寄存器结构定义。地址偏移量:0x10034，初值:0x00000000，宽度:32
 寄存器说明: 读命令DMC TIMEOUT映射表1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rd_timeout_pri4 : 6;  /* bit[0-5]  : 优先级为4的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_0      : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  rd_timeout_pri5 : 6;  /* bit[8-13] : 优先级为5的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  rd_timeout_pri6 : 6;  /* bit[16-21]: 优先级为6的读命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_2      : 10; /* bit[22-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_RD_TIMEOUT1_UNION;
#endif
#define SOC_DMSS_DMI_RD_TIMEOUT1_rd_timeout_pri4_START  (0)
#define SOC_DMSS_DMI_RD_TIMEOUT1_rd_timeout_pri4_END    (5)
#define SOC_DMSS_DMI_RD_TIMEOUT1_rd_timeout_pri5_START  (8)
#define SOC_DMSS_DMI_RD_TIMEOUT1_rd_timeout_pri5_END    (13)
#define SOC_DMSS_DMI_RD_TIMEOUT1_rd_timeout_pri6_START  (16)
#define SOC_DMSS_DMI_RD_TIMEOUT1_rd_timeout_pri6_END    (21)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_WR_TIMEOUT0_UNION
 结构说明  : DMI_WR_TIMEOUT0 寄存器结构定义。地址偏移量:0x10038，初值:0x00000000，宽度:32
 寄存器说明: 写命令DMC TIMEOUT映射表0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_timeout_pri0 : 6;  /* bit[0-5]  : 优先级为0的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_0      : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  wr_timeout_pri1 : 6;  /* bit[8-13] : 优先级为1的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  wr_timeout_pri2 : 6;  /* bit[16-21]: 优先级为2的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_2      : 2;  /* bit[22-23]: 保留。 */
        unsigned int  wr_timeout_pri3 : 6;  /* bit[24-29]: 优先级为3的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_3      : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_WR_TIMEOUT0_UNION;
#endif
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri0_START  (0)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri0_END    (5)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri1_START  (8)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri1_END    (13)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri2_START  (16)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri2_END    (21)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri3_START  (24)
#define SOC_DMSS_DMI_WR_TIMEOUT0_wr_timeout_pri3_END    (29)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_WR_TIMEOUT1_UNION
 结构说明  : DMI_WR_TIMEOUT1 寄存器结构定义。地址偏移量:0x1003C，初值:0x00000000，宽度:32
 寄存器说明: 写命令DMC TIMEOUT映射表1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wr_timeout_pri4 : 6;  /* bit[0-5]  : 优先级为4的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_0      : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  wr_timeout_pri5 : 6;  /* bit[8-13] : 优先级为5的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  wr_timeout_pri6 : 6;  /* bit[16-21]: 优先级为6的写命令Timeout配置：
                                                           0x0：禁止Timeout功能；
                                                           0x1～0x3F：Timeout配置系数；
                                                           说明：
                                                           QOSBUF/DMC的timeout配置系数，实际timeout时钟周期需要结合timeout放大倍数计算得到。 */
        unsigned int  reserved_2      : 10; /* bit[22-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_WR_TIMEOUT1_UNION;
#endif
#define SOC_DMSS_DMI_WR_TIMEOUT1_wr_timeout_pri4_START  (0)
#define SOC_DMSS_DMI_WR_TIMEOUT1_wr_timeout_pri4_END    (5)
#define SOC_DMSS_DMI_WR_TIMEOUT1_wr_timeout_pri5_START  (8)
#define SOC_DMSS_DMI_WR_TIMEOUT1_wr_timeout_pri5_END    (13)
#define SOC_DMSS_DMI_WR_TIMEOUT1_wr_timeout_pri6_START  (16)
#define SOC_DMSS_DMI_WR_TIMEOUT1_wr_timeout_pri6_END    (21)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_GID_RGN_MAP0_UNION
 结构说明  : DMI_GID_RGN_MAP0 寄存器结构定义。地址偏移量:0x10040+0x8*(gid_rgns)，初值:0x00000000，宽度:32
 寄存器说明: GID地址区域映射0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_base_addr : 24; /* bit[0-23] : 各个地址区域的起始地址设置（高位部分）：
                                                         设置地址4KB以上的高24位，高位地址位宽不足则补0。
                                                         注意：
                                                         1.gid rgn之间不允许地址重叠；
                                                         2.起始地址不可配置的低位部分为全0，起始地址包含在当前rgn内；
                                                         3.若base_addr配置大于top_addr，则当前rgn不生效；
                                                         4.可设置的地址分辨率固定为4KB。 */
        unsigned int  reserved      : 7;  /* bit[24-30]: 保留。 */
        unsigned int  rgn_en        : 1;  /* bit[31]   : 各个区域的使能控制：
                                                         0：不使能；
                                                         1：使能。 */
    } reg;
} SOC_DMSS_DMI_GID_RGN_MAP0_UNION;
#endif
#define SOC_DMSS_DMI_GID_RGN_MAP0_rgn_base_addr_START  (0)
#define SOC_DMSS_DMI_GID_RGN_MAP0_rgn_base_addr_END    (23)
#define SOC_DMSS_DMI_GID_RGN_MAP0_rgn_en_START         (31)
#define SOC_DMSS_DMI_GID_RGN_MAP0_rgn_en_END           (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_GID_RGN_MAP1_UNION
 结构说明  : DMI_GID_RGN_MAP1 寄存器结构定义。地址偏移量:0x10044+0x8*(gid_rgns)，初值:0x00000000，宽度:32
 寄存器说明: GID地址区域映射1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_top_addr : 24; /* bit[0-23] : 各个地址区域的结束地址设置（高位部分）：
                                                        设置地址4KB以上的高24位，高位地址位宽不足则补0。
                                                        注意：
                                                        1.gid rgn之间不允许地址重叠；
                                                        2.结束地址不可配置的低位部分为全1，结束地址包含在当前rgn内；
                                                        3.若base_addr配置大于top_addr，则当前rgn不生效；
                                                        4.可设置的地址分辨率固定为4KB。 */
        unsigned int  reserved     : 4;  /* bit[24-27]: 保留。 */
        unsigned int  remap_gid    : 4;  /* bit[28-31]: 重映射后的GID，对于命中地址区域且GID为0的命令GID重映射。 */
    } reg;
} SOC_DMSS_DMI_GID_RGN_MAP1_UNION;
#endif
#define SOC_DMSS_DMI_GID_RGN_MAP1_rgn_top_addr_START  (0)
#define SOC_DMSS_DMI_GID_RGN_MAP1_rgn_top_addr_END    (23)
#define SOC_DMSS_DMI_GID_RGN_MAP1_remap_gid_START     (28)
#define SOC_DMSS_DMI_GID_RGN_MAP1_remap_gid_END       (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_LD_SEL_UNION
 结构说明  : DMI_SEC_LD_SEL 寄存器结构定义。地址偏移量:0x10080，初值:0x00000000，宽度:32
 寄存器说明: 锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ld_rgns_sel : 32; /* bit[0-31]: 选择需将相关配置寄存器锁定为只读的regions：
                                                      sec_ld_rgns_sel[n]=0：region n未被选定。
                                                      sec_ld_rgns_sel[n]=1：region n被选定。
                                                      注意：
                                                      若某region被选定，则此rgn对应的如下相关配置寄存器皆被选定：
                                                      （1）DMI_SEC_RGN_MAP0；
                                                      （2）DMI_SEC_RGN_MAP1；
                                                      （3）DMI_SEC_RGN_ATTR；
                                                      （4）DMI_SEC_MID_WR；
                                                      （5）DMI_SEC_MID_RD；
                                                      若secure_boot_lock信号被拉高，则所有被选定寄存器及本寄存器，都将被锁定成只读。只有复位DMSS方可解锁。 */
    } reg;
} SOC_DMSS_DMI_SEC_LD_SEL_UNION;
#endif
#define SOC_DMSS_DMI_SEC_LD_SEL_ld_rgns_sel_START  (0)
#define SOC_DMSS_DMI_SEC_LD_SEL_ld_rgns_sel_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_INT_EN_UNION
 结构说明  : DMI_SEC_INT_EN 寄存器结构定义。地址偏移量:0x10084，初值:0x00000001，宽度:32
 寄存器说明: DMI安全中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : 越权命令的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有越权命令访问时，可上报中断并记录越权命令信息（请参考DMI_SEC_FAIL_CMD_INF寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_SEC_INT_EN_UNION;
#endif
#define SOC_DMSS_DMI_SEC_INT_EN_int_en_START    (0)
#define SOC_DMSS_DMI_SEC_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_INT_STATUS_UNION
 结构说明  : DMI_SEC_INT_STATUS 寄存器结构定义。地址偏移量:0x10088+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: DMI安全中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。
                                                      注意：从GLOBAL模块的GLB_INT_STATUS1寄存器中，可识别出中断来自于哪个DMI中的SEC模块。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_SEC_INT_STATUS_UNION;
#endif
#define SOC_DMSS_DMI_SEC_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_DMI_SEC_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_DMI_SEC_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_DMI_SEC_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_DMI_SEC_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_DMI_SEC_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_ENHN_INT_EN_UNION
 结构说明  : DMI_ENHN_INT_EN 寄存器结构定义。地址偏移量:0x1008C，初值:0x00000001，宽度:32
 寄存器说明: DMI Enhance安全中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : Enhance Trusted地址区域越权命令的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有越权命令访问时，可上报中断并记录越权命令信息（请参考DMI_SEC_FAIL_CMD_INF寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_ENHN_INT_EN_UNION;
#endif
#define SOC_DMSS_DMI_ENHN_INT_EN_int_en_START    (0)
#define SOC_DMSS_DMI_ENHN_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_ENHN_INT_STATUS_UNION
 结构说明  : DMI_ENHN_INT_STATUS 寄存器结构定义。地址偏移量:0x10090+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: DMI Enhance安全中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : Enhance Trusted地址区域越权中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。
                                                      注意：从GLOBAL模块的GLB_INT_STATUS1寄存器中，可识别出中断来自于哪个DMI中的Enhance SEC模块。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_ENHN_INT_STATUS_UNION;
#endif
#define SOC_DMSS_DMI_ENHN_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_DMI_ENHN_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_DMI_ENHN_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_DMI_ENHN_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_DMI_ENHN_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_DMI_ENHN_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_UNION
 结构说明  : DMI_SEC_FAIL_CMD_INF0 寄存器结构定义。地址偏移量:0x10094+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息0（包括DMI安全拦截和DMI Enhance安全拦截）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_low : 32; /* bit[0-31]: 中断状态清除前的首个越权命令的低32位地址。
                                                      说明：
                                                      此处的地址为抽取交织位之后的通道内地址。 */
    } reg;
} SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_UNION;
#endif
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_address_low_START  (0)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF0_address_low_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_UNION
 结构说明  : DMI_SEC_FAIL_CMD_INF1 寄存器结构定义。地址偏移量:0x10098+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息1（包括DMI安全拦截和DMI Enhance安全拦截）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_high  : 8;  /* bit[0-7]  : 中断状态清除前的首个越权命令的高位地址（如果存在高位地址）。
                                                         说明：
                                                         此处的地址为抽取交织位之后的通道内地址。 */
        unsigned int  reserved      : 8;  /* bit[8-15] : 保留。 */
        unsigned int  privileged    : 1;  /* bit[16]   : 中断状态清除前首个越权命令的特权模式信息：
                                                         0：unprivileged访问；
                                                         1：privileged访问。 */
        unsigned int  nonsecure     : 1;  /* bit[17]   : 中断状态清除前首个越权命令的安全模式信息：
                                                         0：secure访问；
                                                         1：non-secure访问。 */
        unsigned int  access_burst  : 2;  /* bit[18-19]: 中断状态清除前首个越权命令的burst信息：
                                                         0：FIXED；
                                                         1：INCR；
                                                         2：WRAP；
                                                         3：reserved。 */
        unsigned int  access_type   : 1;  /* bit[20]   : 中断状态清除前首个越权命令的读写模式信息：
                                                         0：read访问；
                                                         1：write访问。 */
        unsigned int  access_nsaid  : 3;  /* bit[21-23]: 中断状态清除前首个越权命令的nsaid信息。 */
        unsigned int  access_len    : 4;  /* bit[24-27]: 中断状态清除前首个越权命令的length信息：
                                                         0~15：burst1~burst15。 */
        unsigned int  access_portid : 4;  /* bit[28-31]: 中断状态清除前首个越权命令的ASI编号。 */
    } reg;
} SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_UNION;
#endif
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_address_high_START   (0)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_address_high_END     (7)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_privileged_START     (16)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_privileged_END       (16)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_nonsecure_START      (17)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_nonsecure_END        (17)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_burst_START   (18)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_burst_END     (19)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_type_START    (20)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_type_END      (20)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_nsaid_START   (21)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_nsaid_END     (23)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_len_START     (24)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_len_END       (27)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_portid_START  (28)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF1_access_portid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_UNION
 结构说明  : DMI_SEC_FAIL_CMD_INF2 寄存器结构定义。地址偏移量:0x1009C+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息2（包括DMI安全拦截和DMI Enhance安全拦截）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  id  : 24; /* bit[0-23] : 中断状态清除前的首个越权命令的命令ID。 */
        unsigned int  mid : 8;  /* bit[24-31]: 中断状态清除前的首个越权命令的命令MasterID。 */
    } reg;
} SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_UNION;
#endif
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_id_START   (0)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_id_END     (23)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_mid_START  (24)
#define SOC_DMSS_DMI_SEC_FAIL_CMD_INF2_mid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_MPU_INT_EN_UNION
 结构说明  : DMI_MPU_INT_EN 寄存器结构定义。地址偏移量:0x100A0，初值:0x00000001，宽度:32
 寄存器说明: MPU中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : MPU拦截命令的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有MPU拦截命令访问时，可上报中断并记录MPU拦截命令信息（请参考DMI_MPU_FAIL_CMD_INF寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及MPU拦截命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_MPU_INT_EN_UNION;
#endif
#define SOC_DMSS_DMI_MPU_INT_EN_int_en_START    (0)
#define SOC_DMSS_DMI_MPU_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_MPU_INT_STATUS_UNION
 结构说明  : DMI_MPU_INT_STATUS 寄存器结构定义。地址偏移量:0x100A4+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_MPU_INT_STATUS_UNION;
#endif
#define SOC_DMSS_DMI_MPU_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_DMI_MPU_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_DMI_MPU_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_DMI_MPU_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_DMI_MPU_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_DMI_MPU_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_UNION
 结构说明  : DMI_MPU_FAIL_CMD_INF0 寄存器结构定义。地址偏移量:0x100A8+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个MPU拦截命令的信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_fail_low_address : 32; /* bit[0-31]: MPU中断状态清除前的首个MPU拦截命令的低32位地址。
                                                               说明：
                                                               此处的地址为抽取交织位之后的通道内地址。 */
    } reg;
} SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_UNION;
#endif
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_mpu_fail_low_address_START  (0)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF0_mpu_fail_low_address_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_UNION
 结构说明  : DMI_MPU_FAIL_CMD_INF1 寄存器结构定义。地址偏移量:0x100AC+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个MPU拦截命令的信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_fail_high_address : 8;  /* bit[0-7] : MPU中断状态清除前的首个MPU拦截命令的高8位地址。
                                                                说明：
                                                                此处的地址为抽取交织位之后的通道内地址。 */
        unsigned int  reserved              : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_UNION;
#endif
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_mpu_fail_high_address_START  (0)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF1_mpu_fail_high_address_END    (7)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_UNION
 结构说明  : DMI_MPU_FAIL_CMD_INF2 寄存器结构定义。地址偏移量:0x100B0+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个MPU拦截命令的信息2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_fail_mid    : 8;  /* bit[0-7]  : 中断状态清除前的首个MPU拦截命令的MasterID。 */
        unsigned int  mpu_fail_chmod  : 2;  /* bit[8-9]  : 中断状态清除前的首个MPU拦截命令的通道映射模式信息：
                                                           0x0：四通道交织；
                                                           0x1：3+1通道交织；
                                                           0x2：双通道交织；
                                                           0x3：单通道交织。 */
        unsigned int  reserved_0      : 2;  /* bit[10-11]: 保留。 */
        unsigned int  mpu_fail_nsaid  : 2;  /* bit[12-13]: 中断状态清除前的首个MPU拦截命令的NSAID。 */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  mpu_fail_attr   : 1;  /* bit[16]   : 中断状态清除前的首个MPU拦截命令所在的64KB地址属性信息：
                                                           0：non-trusted；
                                                           1：protected。 */
        unsigned int  reserved_2      : 5;  /* bit[17-21]: 保留。 */
        unsigned int  mpu_fail_ch     : 1;  /* bit[22]   : 中断状态清除前的首个MPU拦截命令所在DMI的通道编号信息：
                                                           0：dmi中第一个通道；
                                                           1：dmi中第二个通道。 */
        unsigned int  reserved_3      : 1;  /* bit[23]   : 保留。 */
        unsigned int  mpu_fail_type   : 1;  /* bit[24]   : 中断状态清除前首个MPU拦截命令的读写模式信息：
                                                           0：read访问；
                                                           1：write访问。 */
        unsigned int  reserved_4      : 3;  /* bit[25-27]: 保留。 */
        unsigned int  mpu_fail_portid : 4;  /* bit[28-31]: 中断状态清除前的首个MPU拦截命令的ASI编号。 */
    } reg;
} SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_UNION;
#endif
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_mid_START     (0)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_mid_END       (7)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_chmod_START   (8)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_chmod_END     (9)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_nsaid_START   (12)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_nsaid_END     (13)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_attr_START    (16)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_attr_END      (16)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_ch_START      (22)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_ch_END        (22)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_type_START    (24)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_type_END      (24)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_portid_START  (28)
#define SOC_DMSS_DMI_MPU_FAIL_CMD_INF2_mpu_fail_portid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_ENHN_CFG_INT_EN_UNION
 结构说明  : DMI_ENHN_CFG_INT_EN 寄存器结构定义。地址偏移量:0x100B4，初值:0x00000001，宽度:32
 寄存器说明: DMI Enhance寄存器配置中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : Enhance Trusted配置寄存器越权配置的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有越权命令访问时，可上报中断并记录越权命令信息（请参考DMI_ENHN_CFG_INT_STATUS寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_ENHN_CFG_INT_EN_UNION;
#endif
#define SOC_DMSS_DMI_ENHN_CFG_INT_EN_int_en_START    (0)
#define SOC_DMSS_DMI_ENHN_CFG_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_UNION
 结构说明  : DMI_ENHN_CFG_INT_STATUS 寄存器结构定义。地址偏移量:0x100B8+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: DMI Enhance寄存器配置中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]    : Enhance Trusted配置寄存器越权配置的中断状态指示：
                                                       0：中断未产生；
                                                       1：中断已产生。 */
        unsigned int  reserved_0  : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  cfg_type    : 1;  /* bit[4]    : 中断状态清除前的首个enhance trutst越权配置操的读写模式信息：
                                                       0：read访问；
                                                       1：write访问。 */
        unsigned int  reserved_1  : 7;  /* bit[5-11] : 保留。 */
        unsigned int  cfg_address : 20; /* bit[12-31]: 中断状态清除前的首个enhance trutst越权配置操作的低20位地址。 */
    } reg;
} SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_UNION;
#endif
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_cfg_type_START     (4)
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_cfg_type_END       (4)
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_cfg_address_START  (12)
#define SOC_DMSS_DMI_ENHN_CFG_INT_STATUS_cfg_address_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_RGN_MAP0_UNION
 结构说明  : DMI_SEC_RGN_MAP0 寄存器结构定义。地址偏移量:0x10100+0x30*(dmi_rgns)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域映射0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_base_addr : 20; /* bit[0-19] : 各个地址区域的起始地址设置（高位部分）：
                                                         设置地址64KB以上的高20位，高位地址位宽不足则补0。
                                                         注意：
                                                         1.rgn之间（包括与ASI的SEC Region之间）不允许地址重叠；
                                                         2.起始地址不可配置的低位部分为全0，起始地址包含在当前rgn内；
                                                         3.若base_addr配置大于top_addr，则当前rgn不生效；
                                                         4.需保证exclusive命令访问的地址区域有响应的访问权限，否则exclusive命令无法被正常监控；
                                                         5.可设置的地址分辨率固定为64KB。 */
        unsigned int  reserved      : 11; /* bit[20-30]: 保留。 */
        unsigned int  rgn_en        : 1;  /* bit[31]   : 各个区域的使能控制：
                                                         0：不使能；
                                                         1：使能。 */
    } reg;
} SOC_DMSS_DMI_SEC_RGN_MAP0_UNION;
#endif
#define SOC_DMSS_DMI_SEC_RGN_MAP0_rgn_base_addr_START  (0)
#define SOC_DMSS_DMI_SEC_RGN_MAP0_rgn_base_addr_END    (19)
#define SOC_DMSS_DMI_SEC_RGN_MAP0_rgn_en_START         (31)
#define SOC_DMSS_DMI_SEC_RGN_MAP0_rgn_en_END           (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_RGN_MAP1_UNION
 结构说明  : DMI_SEC_RGN_MAP1 寄存器结构定义。地址偏移量:0x10104+0x30*(dmi_rgns)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域映射1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_top_addr : 20; /* bit[0-19] : 各个地址区域的结束地址设置（高位部分）：
                                                        设置地址64KB以上的高20位，高位地址位宽不足则补0。
                                                        注意：
                                                        1.rgn之间（包括与ASI的SEC Region之间）不允许地址重叠；
                                                        2.结束地址不可配置的低位部分为全1，结束地址包含在当前rgn内；
                                                        3.若base_addr大于top_addr，则当前rgn不使能；
                                                        4.需保证exclusive命令访问的地址区域有响应的访问权限，否则exclusive命令无法被正常监控；
                                                        5.可设置的地址分辨率固定为64KB。 */
        unsigned int  reserved     : 8;  /* bit[20-27]: 保留。 */
        unsigned int  sp           : 4;  /* bit[28-31]: 当前区域的安全权限属性。
                                                        sp[3]：安全读属性；
                                                        sp[2]：安全写属性；
                                                        sp[1]：非安全读属性；
                                                        sp[0]：非安全写属性；
                                                        sp[n]=0：禁止访问；
                                                        sp[n]=1：允许访问。
                                                        说明：
                                                        1.当前区域enhance trusted属性使能时，安全属性特指enhance trusted属性，非安全属性特指非enhance trusted属性；
                                                        2.当前区域enhance trusted属性是否使能，通过DMI_SEC_RGN_ATTR可以读取。 */
    } reg;
} SOC_DMSS_DMI_SEC_RGN_MAP1_UNION;
#endif
#define SOC_DMSS_DMI_SEC_RGN_MAP1_rgn_top_addr_START  (0)
#define SOC_DMSS_DMI_SEC_RGN_MAP1_rgn_top_addr_END    (19)
#define SOC_DMSS_DMI_SEC_RGN_MAP1_sp_START            (28)
#define SOC_DMSS_DMI_SEC_RGN_MAP1_sp_END              (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_RGN_ATTR_UNION
 结构说明  : DMI_SEC_RGN_ATTR 寄存器结构定义。地址偏移量:0x10108+0x30*(dmi_enhn_rgns)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域属性
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  etrust   : 1;  /* bit[0]   : 当前区域的enhance trusted属性使能：
                                                   0：不使能，当前区域是普通安全区域；
                                                   1：使能，当前区域是enhance trusted区域。
                                                   注意：
                                                   当前区域是否支持此功能与RTL配置有关，需要根据enhance trusted属性的regions个数判定（从DMI_RTL_INF0.rtl_enhn_sec_rgn_num可读出）。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_SEC_RGN_ATTR_UNION;
#endif
#define SOC_DMSS_DMI_SEC_RGN_ATTR_etrust_START    (0)
#define SOC_DMSS_DMI_SEC_RGN_ATTR_etrust_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_WR0_UNION
 结构说明  : DMI_SEC_MID_WR0 寄存器结构定义。地址偏移量:0x10110+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr0 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_WR0_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_WR0_mid_sel_wr0_START  (0)
#define SOC_DMSS_DMI_SEC_MID_WR0_mid_sel_wr0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_WR1_UNION
 结构说明  : DMI_SEC_MID_WR1 寄存器结构定义。地址偏移量:0x10114+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr1 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为32+n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为32+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_WR1_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_WR1_mid_sel_wr1_START  (0)
#define SOC_DMSS_DMI_SEC_MID_WR1_mid_sel_wr1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_WR2_UNION
 结构说明  : DMI_SEC_MID_WR2 寄存器结构定义。地址偏移量:0x10118+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr2 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为64+n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为64+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_WR2_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_WR2_mid_sel_wr2_START  (0)
#define SOC_DMSS_DMI_SEC_MID_WR2_mid_sel_wr2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_WR3_UNION
 结构说明  : DMI_SEC_MID_WR3 寄存器结构定义。地址偏移量:0x1011C+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr3 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为96+n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为96+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_WR3_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_WR3_mid_sel_wr3_START  (0)
#define SOC_DMSS_DMI_SEC_MID_WR3_mid_sel_wr3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_RD0_UNION
 结构说明  : DMI_SEC_MID_RD0 寄存器结构定义。地址偏移量:0x10120+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd0 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_RD0_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_RD0_mid_sel_rd0_START  (0)
#define SOC_DMSS_DMI_SEC_MID_RD0_mid_sel_rd0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_RD1_UNION
 结构说明  : DMI_SEC_MID_RD1 寄存器结构定义。地址偏移量:0x10124+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd1 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为32+n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为32+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_RD1_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_RD1_mid_sel_rd1_START  (0)
#define SOC_DMSS_DMI_SEC_MID_RD1_mid_sel_rd1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_RD2_UNION
 结构说明  : DMI_SEC_MID_RD2 寄存器结构定义。地址偏移量:0x10128+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd2 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为64+n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为64+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_RD2_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_RD2_mid_sel_rd2_START  (0)
#define SOC_DMSS_DMI_SEC_MID_RD2_mid_sel_rd2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_SEC_MID_RD3_UNION
 结构说明  : DMI_SEC_MID_RD3 寄存器结构定义。地址偏移量:0x1012C+0x30*(dmi_mid_rgns)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd3 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为96+n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为96+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从DMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_DMI_SEC_MID_RD3_UNION;
#endif
#define SOC_DMSS_DMI_SEC_MID_RD3_mid_sel_rd3_START  (0)
#define SOC_DMSS_DMI_SEC_MID_RD3_mid_sel_rd3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_DFX_MODULE_UNION
 结构说明  : DMI_DFX_MODULE 寄存器结构定义。地址偏移量:0x10700+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：各个主要模块的工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dmi_busy     : 1;  /* bit[0]   : DMI的工作状态：
                                                       0：DMI处于idle状态；
                                                       1：DMI处于busy状态。 */
        unsigned int  dmi_sec_busy : 1;  /* bit[1]   : DMI的SEC模块工作状态：
                                                       0：DMI SEC模块处于idle状态；
                                                       1：DMI SEC模块处于busy状态。 */
        unsigned int  dcsc_busy    : 1;  /* bit[2]   : DCSC模块工作状态：
                                                       0：DCSC模块处于idle状态；
                                                       1：DCSC模块处于busy状态。 */
        unsigned int  reserved     : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_DFX_MODULE_UNION;
#endif
#define SOC_DMSS_DMI_DFX_MODULE_dmi_busy_START      (0)
#define SOC_DMSS_DMI_DFX_MODULE_dmi_busy_END        (0)
#define SOC_DMSS_DMI_DFX_MODULE_dmi_sec_busy_START  (1)
#define SOC_DMSS_DMI_DFX_MODULE_dmi_sec_busy_END    (1)
#define SOC_DMSS_DMI_DFX_MODULE_dcsc_busy_START     (2)
#define SOC_DMSS_DMI_DFX_MODULE_dcsc_busy_END       (2)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_DFX_FIFO_UNION
 结构说明  : DMI_DFX_FIFO 寄存器结构定义。地址偏移量:0x10704+0x800*(dmi_base)，初值:0x00000155，宽度:32
 寄存器说明: DFX：FIFO工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wd_mux0_fifo_st : 2;  /* bit[0-1]  : DMI模块中的写数据通道0汇聚FIFO的状态：
                                                           0x0：busy；
                                                           0x1：emtpy；
                                                           0x2：full；
                                                           0x3：reserved。 */
        unsigned int  wd_mux1_fifo_st : 2;  /* bit[2-3]  : DMI模块中的写数据通道1汇聚FIFO的状态：
                                                           0x0：busy；
                                                           0x1：emtpy；
                                                           0x2：full；
                                                           0x3：reserved。 */
        unsigned int  wd_arb0_fifo_st : 2;  /* bit[4-5]  : DMI模块中的写数据通道0仲裁FIFO的状态：
                                                           0x0：busy；
                                                           0x1：emtpy；
                                                           0x2：full；
                                                           0x3：reserved。 */
        unsigned int  wd_arb1_fifo_st : 2;  /* bit[6-7]  : DMI模块中的写数据通道1仲裁FIFO的状态：
                                                           0x0：busy；
                                                           0x1：emtpy；
                                                           0x2：full；
                                                           0x3：reserved。 */
        unsigned int  wrsp_fifo_st    : 2;  /* bit[8-9]  : DMI模块中的写响应FIFO的状态：
                                                           0x0：busy；
                                                           0x1：emtpy；
                                                           0x2：full；
                                                           0x3：reserved。 */
        unsigned int  reserved        : 22; /* bit[10-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_DFX_FIFO_UNION;
#endif
#define SOC_DMSS_DMI_DFX_FIFO_wd_mux0_fifo_st_START  (0)
#define SOC_DMSS_DMI_DFX_FIFO_wd_mux0_fifo_st_END    (1)
#define SOC_DMSS_DMI_DFX_FIFO_wd_mux1_fifo_st_START  (2)
#define SOC_DMSS_DMI_DFX_FIFO_wd_mux1_fifo_st_END    (3)
#define SOC_DMSS_DMI_DFX_FIFO_wd_arb0_fifo_st_START  (4)
#define SOC_DMSS_DMI_DFX_FIFO_wd_arb0_fifo_st_END    (5)
#define SOC_DMSS_DMI_DFX_FIFO_wd_arb1_fifo_st_START  (6)
#define SOC_DMSS_DMI_DFX_FIFO_wd_arb1_fifo_st_END    (7)
#define SOC_DMSS_DMI_DFX_FIFO_wrsp_fifo_st_START     (8)
#define SOC_DMSS_DMI_DFX_FIFO_wrsp_fifo_st_END       (9)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_DFX_WD_FIFO_UNION
 结构说明  : DMI_DFX_WD_FIFO 寄存器结构定义。地址偏移量:0x10708+0x800*(dmi_base)，初值:0x00000FFF，宽度:32
 寄存器说明: DFX：写数据FIFO工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wfifo_empty : 14; /* bit[0-13] : DMI模块中的写数据FIFO状态：
                                                       burstfifo_st[n]=0：DSTn的burstfifo为非空；
                                                       burstfifo_st[n]=1：DSTn的burstfifo为空。 */
        unsigned int  reserved_0  : 2;  /* bit[14-15]: 保留。 */
        unsigned int  wfifo_full  : 14; /* bit[16-29]: DMI模块中的写数据FIFO状态：
                                                       burstfifo_st[n]=0：DSTn的burstfifo为非满；
                                                       burstfifo_st[n]=1：DSTn的burstfifo为满。 */
        unsigned int  reserved_1  : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_DFX_WD_FIFO_UNION;
#endif
#define SOC_DMSS_DMI_DFX_WD_FIFO_wfifo_empty_START  (0)
#define SOC_DMSS_DMI_DFX_WD_FIFO_wfifo_empty_END    (13)
#define SOC_DMSS_DMI_DFX_WD_FIFO_wfifo_full_START   (16)
#define SOC_DMSS_DMI_DFX_WD_FIFO_wfifo_full_END     (29)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_DFX_ST_UNION
 结构说明  : DMI_DFX_ST 寄存器结构定义。地址偏移量:0x1070C+0x800*(dmi_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：DMI的模块状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dmi_grant : 16; /* bit[0-15] : DMI的命令仲裁状态：
                                                     dmi_grant[n]=0：ASIn未获得DMI的命令仲裁；
                                                     dmi_grant[n]=1：ASIn获得了DMI的命令仲裁。 */
        unsigned int  reserved_0: 8;  /* bit[16-23]: 保留。 */
        unsigned int  cmd_bp    : 1;  /* bit[24]   : DMI下发命令的被反压状态：
                                                     ar_bp=0：DMI没有下发命令，或下发命令被接收；
                                                     ar_bp=1：DMI有下发命令且被下游反压。 */
        unsigned int  w_bp      : 1;  /* bit[25]   : DMI下发写数据的被反压状态：
                                                     0：DMI没有下发写数据，或下发写数据被接收；
                                                     1：DMI有下发写数据且被下游反压。 */
        unsigned int  r_bp      : 1;  /* bit[26]   : DMI接收读数据的被反压状态：
                                                     0：DMI没有收到读数据，或读数据被接收；
                                                     1：DMI有收到读数据且被DMI反压。 */
        unsigned int  b_bp      : 1;  /* bit[27]   : DMI接收写响应的被反压状态：
                                                     0：DMI没有收到写响应，或写响应被接收；
                                                     1：DMI有收到写响应且被DMI反压。 */
        unsigned int  reserved_1: 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_DFX_ST_UNION;
#endif
#define SOC_DMSS_DMI_DFX_ST_dmi_grant_START  (0)
#define SOC_DMSS_DMI_DFX_ST_dmi_grant_END    (15)
#define SOC_DMSS_DMI_DFX_ST_cmd_bp_START     (24)
#define SOC_DMSS_DMI_DFX_ST_cmd_bp_END       (24)
#define SOC_DMSS_DMI_DFX_ST_w_bp_START       (25)
#define SOC_DMSS_DMI_DFX_ST_w_bp_END         (25)
#define SOC_DMSS_DMI_DFX_ST_r_bp_START       (26)
#define SOC_DMSS_DMI_DFX_ST_r_bp_END         (26)
#define SOC_DMSS_DMI_DFX_ST_b_bp_START       (27)
#define SOC_DMSS_DMI_DFX_ST_b_bp_END         (27)


/*****************************************************************************
 结构名    : SOC_DMSS_DMI_DFX_DCSC_UNION
 结构说明  : DMI_DFX_DCSC 寄存器结构定义。地址偏移量:0x10710+0x800*(dmi_base)，初值:0x15550000，宽度:32
 寄存器说明: DFX：DCSC的模块状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcsc_m_r_bp                : 1;  /* bit[0]    : DCSC->DDRC接口读数据通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  dcsc_m_w_bp                : 1;  /* bit[1]    : DCSC->DDRC接口写数据通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  dcsc_m_cmd_bp              : 1;  /* bit[2]    : DCSC->DDRC接口命令通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_m_r_bp                  : 1;  /* bit[3]    : DCSC->SC接口读数据通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_m_b_bp                  : 1;  /* bit[4]    : DCSC->SC接口写响应通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_m_w_bp                  : 1;  /* bit[5]    : DCSC->SC接口写数据通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_m_cmd_bp                : 1;  /* bit[6]    : DCSC->SC接口命令通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_s_r_bp                  : 1;  /* bit[7]    : SC->DCSC接口读数据通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_s_b_bp                  : 1;  /* bit[8]    : SC->DCSC接口写响应通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_s_w_bp                  : 1;  /* bit[9]    : SC->DCSC接口写数据通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  sc_s_cmd_bp                : 1;  /* bit[10]   : SC->DCSC接口命令通路反压状态：
                                                                      0：无反压；
                                                                      1：有反压。 */
        unsigned int  reserved_0                 : 5;  /* bit[11-15]: 保留。 */
        unsigned int  dcsc_brsp_fifo_st          : 2;  /* bit[16-17]: DCSC模块中的写响应FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  dcsc_wd_mux1_fifo_st       : 2;  /* bit[18-19]: DCSC模块中的写数据汇聚FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  dcsc_wd_mux0_fifo_st       : 2;  /* bit[20-21]: DCSC模块中的写数据汇聚FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  dcsc_wd_dmux1_fifo_st      : 2;  /* bit[22-23]: DCSC模块中的写数据分发FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  dcsc_wd_dmux0_fifo_st      : 2;  /* bit[24-25]: DCSC模块中的写数据分发FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  dcsc_trace_wd_mux1_fifo_st : 2;  /* bit[26-27]: TRACE模块和主通路的写数据分发FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  dcsc_trace_wd_mux0_fifo_st : 2;  /* bit[28-29]: TRACE模块和主通路的写数据分发FIFO的状态：
                                                                      0x0：busy；
                                                                      0x1：emtpy；
                                                                      0x2：full；
                                                                      0x3：reserved。 */
        unsigned int  reserved_1                 : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_DMI_DFX_DCSC_UNION;
#endif
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_m_r_bp_START                 (0)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_m_r_bp_END                   (0)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_m_w_bp_START                 (1)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_m_w_bp_END                   (1)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_m_cmd_bp_START               (2)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_m_cmd_bp_END                 (2)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_r_bp_START                   (3)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_r_bp_END                     (3)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_b_bp_START                   (4)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_b_bp_END                     (4)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_w_bp_START                   (5)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_w_bp_END                     (5)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_cmd_bp_START                 (6)
#define SOC_DMSS_DMI_DFX_DCSC_sc_m_cmd_bp_END                   (6)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_r_bp_START                   (7)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_r_bp_END                     (7)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_b_bp_START                   (8)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_b_bp_END                     (8)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_w_bp_START                   (9)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_w_bp_END                     (9)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_cmd_bp_START                 (10)
#define SOC_DMSS_DMI_DFX_DCSC_sc_s_cmd_bp_END                   (10)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_brsp_fifo_st_START           (16)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_brsp_fifo_st_END             (17)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_mux1_fifo_st_START        (18)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_mux1_fifo_st_END          (19)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_mux0_fifo_st_START        (20)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_mux0_fifo_st_END          (21)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_dmux1_fifo_st_START       (22)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_dmux1_fifo_st_END         (23)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_dmux0_fifo_st_START       (24)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_wd_dmux0_fifo_st_END         (25)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_trace_wd_mux1_fifo_st_START  (26)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_trace_wd_mux1_fifo_st_END    (27)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_trace_wd_mux0_fifo_st_START  (28)
#define SOC_DMSS_DMI_DFX_DCSC_dcsc_trace_wd_mux0_fifo_st_END    (29)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_RTL_INF0_UNION
 结构说明  : AMI_RTL_INF0 寄存器结构定义。地址偏移量:0x12000+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: AMI代码配置信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_sec_rgn_num      : 8;  /* bit[0-7]  : AMI安全模块的所有regions个数：
                                                                8'd0：无安全模块；
                                                                8'd1：1regions；
                                                                8'd2：2regions；
                                                                8'd3：3regions；
                                                                ……
                                                                注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  rtl_mid_sec_rgn_num  : 8;  /* bit[8-15] : AMI安全模块的支持MID匹配的regions个数：
                                                                8'd0：无安全模块；
                                                                8'd1：1regions；
                                                                8'd2：2regions；
                                                                8'd3：3regions；
                                                                ……
                                                                注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  rtl_enhn_sec_rgn_num : 4;  /* bit[16-19]: AMI安全模块的支持enhance trusted属性的regions个数：
                                                                8'd0：无安全模块；
                                                                8'd1：1regions；
                                                                8'd2：2regions；
                                                                8'd3：3regions；
                                                                ……
                                                                注意：当无安全模块时，安全功能相关寄存器无效。 */
        unsigned int  reserved_0           : 4;  /* bit[20-23]: 保留。 */
        unsigned int  rtl_mpu_en           : 1;  /* bit[24]   : MPU鉴权拦截功能：
                                                                0：不支持；
                                                                1：支持。
                                                                注意：
                                                                当不支持MPU鉴权拦截功能时，MPU相关寄存器无效。 */
        unsigned int  reserved_1           : 7;  /* bit[25-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_RTL_INF0_UNION;
#endif
#define SOC_DMSS_AMI_RTL_INF0_rtl_sec_rgn_num_START       (0)
#define SOC_DMSS_AMI_RTL_INF0_rtl_sec_rgn_num_END         (7)
#define SOC_DMSS_AMI_RTL_INF0_rtl_mid_sec_rgn_num_START   (8)
#define SOC_DMSS_AMI_RTL_INF0_rtl_mid_sec_rgn_num_END     (15)
#define SOC_DMSS_AMI_RTL_INF0_rtl_enhn_sec_rgn_num_START  (16)
#define SOC_DMSS_AMI_RTL_INF0_rtl_enhn_sec_rgn_num_END    (19)
#define SOC_DMSS_AMI_RTL_INF0_rtl_mpu_en_START            (24)
#define SOC_DMSS_AMI_RTL_INF0_rtl_mpu_en_END              (24)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_DYN_CKG_UNION
 结构说明  : AMI_DYN_CKG 寄存器结构定义。地址偏移量:0x12010+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: AMI动态时钟门控
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ckg_byp_ami        : 1;  /* bit[0]   : AMI模块的动态时钟门控：
                                                             0：模块idle时，自动关闭时钟；
                                                             1：时钟常开。 */
        unsigned int  ckg_byp_ami_wdfifo : 1;  /* bit[1]   : AMI模块中写数据fifo的动态时钟门控：
                                                             0：模块idle时，自动关闭时钟；
                                                             1：时钟常开。 */
        unsigned int  ckg_byp_ami_sec    : 1;  /* bit[2]   : AMI SEC模块的动态时钟门控：
                                                             0：模块idle时，自动关闭时钟；
                                                             1：时钟常开。 */
        unsigned int  reserved           : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_DYN_CKG_UNION;
#endif
#define SOC_DMSS_AMI_DYN_CKG_ckg_byp_ami_START         (0)
#define SOC_DMSS_AMI_DYN_CKG_ckg_byp_ami_END           (0)
#define SOC_DMSS_AMI_DYN_CKG_ckg_byp_ami_wdfifo_START  (1)
#define SOC_DMSS_AMI_DYN_CKG_ckg_byp_ami_wdfifo_END    (1)
#define SOC_DMSS_AMI_DYN_CKG_ckg_byp_ami_sec_START     (2)
#define SOC_DMSS_AMI_DYN_CKG_ckg_byp_ami_sec_END       (2)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_GID_CTRL_UNION
 结构说明  : AMI_GID_CTRL 寄存器结构定义。地址偏移量:0x12020+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: AMI GID控制配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  gid_disable : 16; /* bit[0-15] : AMI通路GID透传配置：
                                                       gid_disable[n]=0：gid n被透传。
                                                       gid_disable[n]=1：gid n未被透传，随路命令gid为0。 */
        unsigned int  reserved    : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_GID_CTRL_UNION;
#endif
#define SOC_DMSS_AMI_GID_CTRL_gid_disable_START  (0)
#define SOC_DMSS_AMI_GID_CTRL_gid_disable_END    (15)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_LD_SEL_UNION
 结构说明  : AMI_SEC_LD_SEL 寄存器结构定义。地址偏移量:0x12080+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 锁定安全属性相关配置寄存器
            （当secur_boot_lock信号被拉高超过一个时钟周期时，此寄存器自身及其选定的相关寄存器被锁定为只读，只可通过系统复位解除锁定）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ld_rgns_sel : 32; /* bit[0-31]: 选择需将相关配置寄存器锁定为只读的regions：
                                                      sec_ld_rgns_sel[n]=0：region n未被选定。
                                                      sec_ld_rgns_sel[n]=1：region n被选定。
                                                      注意：
                                                      若某region被选定，则此rgn对应的如下相关配置寄存器皆被选定：
                                                      （1）AMI_SEC_RGN_MAP0；
                                                      （2）AMI_SEC_RGN_MAP1；
                                                      （3）AMI_SEC_MID_WR；1
                                                      （4）AMI_SEC_MID_RD；
                                                      若secure_boot_lock信号被拉高，则所有被选定寄存器及本寄存器，都将被锁定成只读。只有复位DMSS方可解锁。 */
    } reg;
} SOC_DMSS_AMI_SEC_LD_SEL_UNION;
#endif
#define SOC_DMSS_AMI_SEC_LD_SEL_ld_rgns_sel_START  (0)
#define SOC_DMSS_AMI_SEC_LD_SEL_ld_rgns_sel_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_INT_EN_UNION
 结构说明  : AMI_SEC_INT_EN 寄存器结构定义。地址偏移量:0x12084+0x800*(ami_base)，初值:0x00000001，宽度:32
 寄存器说明: 安全中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : 越权命令的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有越权命令访问时，可上报中断并记录越权命令信息（请参考DMI_SEC_FAIL_CMD_INF寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及越权命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_SEC_INT_EN_UNION;
#endif
#define SOC_DMSS_AMI_SEC_INT_EN_int_en_START    (0)
#define SOC_DMSS_AMI_SEC_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_INT_STATUS_UNION
 结构说明  : AMI_SEC_INT_STATUS 寄存器结构定义。地址偏移量:0x12088+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 安全中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。
                                                      注意：从GLOBAL模块的GLB_INT_STATUS寄存器中，可识别出中断来自于哪个DMI中的SEC模块。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_SEC_INT_STATUS_UNION;
#endif
#define SOC_DMSS_AMI_SEC_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_AMI_SEC_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_AMI_SEC_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_AMI_SEC_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_AMI_SEC_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_AMI_SEC_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_UNION
 结构说明  : AMI_SEC_FAIL_CMD_INF0 寄存器结构定义。地址偏移量:0x12094+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_low : 32; /* bit[0-31]: 中断状态清除前的首个越权命令的低32位地址。 */
    } reg;
} SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_UNION;
#endif
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_address_low_START  (0)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF0_address_low_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_UNION
 结构说明  : AMI_SEC_FAIL_CMD_INF1 寄存器结构定义。地址偏移量:0x12098+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  address_high  : 8;  /* bit[0-7]  : 中断状态清除前的首个越权命令的高位地址（如果存在高位地址）。 */
        unsigned int  reserved      : 8;  /* bit[8-15] : 保留。 */
        unsigned int  privileged    : 1;  /* bit[16]   : 中断状态清除前首个越权命令的特权模式信息：
                                                         0：unprivileged访问；
                                                         1：privileged访问。 */
        unsigned int  nonsecure     : 1;  /* bit[17]   : 中断状态清除前首个越权命令的安全模式信息：
                                                         0：secure访问；
                                                         1：non-secure访问。 */
        unsigned int  access_burst  : 2;  /* bit[18-19]: 中断状态清除前首个越权命令的burst信息：
                                                         0：FIXED；
                                                         1：INCR；
                                                         2：WRAP；
                                                         3：reserved。 */
        unsigned int  access_type   : 1;  /* bit[20]   : 中断状态清除前首个越权命令的读写模式信息：
                                                         0：read访问；
                                                         1：write访问。 */
        unsigned int  access_size   : 3;  /* bit[21-23]: 中断状态清除前首个越权命令的size信息：
                                                         0：8bits；
                                                         1：16bits；
                                                         2：32bits；
                                                         3：64bits；
                                                         4：128bits。 */
        unsigned int  access_len    : 4;  /* bit[24-27]: 中断状态清除前首个越权命令的length信息：
                                                         0~15：burst1~burst15。 */
        unsigned int  access_portid : 4;  /* bit[28-31]: 中断状态清除前首个越权命令的ASI编号。 */
    } reg;
} SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_UNION;
#endif
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_address_high_START   (0)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_address_high_END     (7)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_privileged_START     (16)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_privileged_END       (16)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_nonsecure_START      (17)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_nonsecure_END        (17)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_burst_START   (18)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_burst_END     (19)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_type_START    (20)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_type_END      (20)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_size_START    (21)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_size_END      (23)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_len_START     (24)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_len_END       (27)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_portid_START  (28)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF1_access_portid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_UNION
 结构说明  : AMI_SEC_FAIL_CMD_INF2 寄存器结构定义。地址偏移量:0x1209C+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个越权命令的信息2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  id  : 24; /* bit[0-23] : 中断状态清除前的首个越权命令的命令ID。 */
        unsigned int  mid : 8;  /* bit[24-31]: 中断状态清除前的首个越权命令的命令MasterID。 */
    } reg;
} SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_UNION;
#endif
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_id_START   (0)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_id_END     (23)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_mid_START  (24)
#define SOC_DMSS_AMI_SEC_FAIL_CMD_INF2_mid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_MPU_INT_EN_UNION
 结构说明  : AMI_MPU_INT_EN 寄存器结构定义。地址偏移量:0x120A0+0x800*(ami_base)，初值:0x00000001，宽度:32
 寄存器说明: MPU中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : MPU拦截命令的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有MPU拦截命令访问时，可上报中断并记录MPU拦截命令信息（请参考DMI_MPU_FAIL_CMD_INF寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及MPU拦截命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_MPU_INT_EN_UNION;
#endif
#define SOC_DMSS_AMI_MPU_INT_EN_int_en_START    (0)
#define SOC_DMSS_AMI_MPU_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_MPU_INT_STATUS_UNION
 结构说明  : AMI_MPU_INT_STATUS 寄存器结构定义。地址偏移量:0x120A4+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: MPU中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status  : 1;  /* bit[0]   : 中断状态指示：
                                                      0：中断未产生；
                                                      1：中断已产生。 */
        unsigned int  int_overrun : 1;  /* bit[1]   : 多次中断指示：
                                                      0：中断未产生或只产生了一次；
                                                      1：中断已产生多次。 */
        unsigned int  reserved_0  : 2;  /* bit[2-3] : 保留。 */
        unsigned int  int_cnt     : 4;  /* bit[4-7] : 多次中断计数：
                                                      n：中断产生次数。
                                                      说明：
                                                      1.若达到最大计数，则保持；
                                                      2.清除中断后此计数器清零。 */
        unsigned int  reserved_1  : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_MPU_INT_STATUS_UNION;
#endif
#define SOC_DMSS_AMI_MPU_INT_STATUS_int_status_START   (0)
#define SOC_DMSS_AMI_MPU_INT_STATUS_int_status_END     (0)
#define SOC_DMSS_AMI_MPU_INT_STATUS_int_overrun_START  (1)
#define SOC_DMSS_AMI_MPU_INT_STATUS_int_overrun_END    (1)
#define SOC_DMSS_AMI_MPU_INT_STATUS_int_cnt_START      (4)
#define SOC_DMSS_AMI_MPU_INT_STATUS_int_cnt_END        (7)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_UNION
 结构说明  : AMI_MPU_FAIL_CMD_INF0 寄存器结构定义。地址偏移量:0x120A8+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个MPU拦截命令的信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_fail_low_address : 32; /* bit[0-31]: MPU中断状态清除前的首个MPU拦截命令的低32位地址。
                                                               说明：
                                                               1.此处的地址为命令原始地址；
                                                               2.当同一时刻出现读写拦截命令，记录写命令。 */
    } reg;
} SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_UNION;
#endif
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_mpu_fail_low_address_START  (0)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF0_mpu_fail_low_address_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_UNION
 结构说明  : AMI_MPU_FAIL_CMD_INF1 寄存器结构定义。地址偏移量:0x120AC+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个MPU拦截命令的信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_fail_high_address : 8;  /* bit[0-7] : MPU中断状态清除前的首个MPU拦截命令的高8位地址。
                                                                说明：
                                                                1.此处的地址为命令原始地址；
                                                                2.当同一时刻出现读写拦截命令，记录写命令。 */
        unsigned int  reserved              : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_UNION;
#endif
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_mpu_fail_high_address_START  (0)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF1_mpu_fail_high_address_END    (7)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_UNION
 结构说明  : AMI_MPU_FAIL_CMD_INF2 寄存器结构定义。地址偏移量:0x120B0+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 记录首个MPU拦截命令的信息2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_fail_mid    : 8;  /* bit[0-7]  : 中断状态清除前的首个MPU拦截命令的MasterID。
                                                           说明：
                                                           当同一时刻出现读写拦截命令，记录写命令。 */
        unsigned int  mpu_fail_chmod  : 2;  /* bit[8-9]  : 中断状态清除前的首个MPU拦截命令的通道映射模式信息：
                                                           0x0：四通道交织；
                                                           0x1：3+1通道交织；
                                                           0x2：双通道交织；
                                                           0x3：单通道交织。
                                                           说明：
                                                           当同一时刻出现读写拦截命令，记录写命令。 */
        unsigned int  reserved_0      : 2;  /* bit[10-11]: 保留。 */
        unsigned int  mpu_fail_nsaid  : 2;  /* bit[12-13]: 中断状态清除前的首个MPU拦截命令的NSAID。
                                                           说明：
                                                           当同一时刻出现读写拦截命令，记录写命令。 */
        unsigned int  reserved_1      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  mpu_fail_attr   : 1;  /* bit[16]   : 中断状态清除前的首个MPU拦截命令所在的64KB地址属性信息：
                                                           0：non-trusted；
                                                           1：protected。
                                                           说明：
                                                           当同一时刻出现读写拦截命令，记录写命令。 */
        unsigned int  reserved_2      : 7;  /* bit[17-23]: 保留。 */
        unsigned int  mpu_fail_type   : 1;  /* bit[24]   : 中断状态清除前首个MPU拦截命令的读写模式信息：
                                                           0：read访问；
                                                           1：write访问。
                                                           说明：
                                                           当同一时刻出现读写拦截命令，记录写命令。 */
        unsigned int  reserved_3      : 3;  /* bit[25-27]: 保留。 */
        unsigned int  mpu_fail_portid : 4;  /* bit[28-31]: 中断状态清除前的首个MPU拦截命令的ASI编号。
                                                           说明：
                                                           当同一时刻出现读写拦截命令，记录写命令。 */
    } reg;
} SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_UNION;
#endif
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_mid_START     (0)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_mid_END       (7)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_chmod_START   (8)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_chmod_END     (9)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_nsaid_START   (12)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_nsaid_END     (13)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_attr_START    (16)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_attr_END      (16)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_type_START    (24)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_type_END      (24)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_portid_START  (28)
#define SOC_DMSS_AMI_MPU_FAIL_CMD_INF2_mpu_fail_portid_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_RGN_MAP0_UNION
 结构说明  : AMI_SEC_RGN_MAP0 寄存器结构定义。地址偏移量:0x12100+0x30*(ami_rgns)+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域映射0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_base_addr : 20; /* bit[0-19] : 各个地址区域的起始地址设置（高位部分）：
                                                         设置地址64KB以上的高20位，高位地址位宽不足则补0。
                                                         注意：
                                                         1.rgn之间（包括与ASI的SEC Region之间）不允许地址重叠；
                                                         2.起始地址不可配置的低位部分为全0，起始地址包含在当前rgn内；
                                                         3.若base_addr配置大于top_addr，则当前rgn不生效；
                                                         4.需保证exclusive命令访问的地址区域有响应的访问权限，否则exclusive命令无法被正常监控；
                                                         5.可设置的地址分辨率固定为64KB。 */
        unsigned int  reserved      : 11; /* bit[20-30]: 保留。 */
        unsigned int  rgn_en        : 1;  /* bit[31]   : 各个区域的使能控制：
                                                         0：不使能；
                                                         1：使能。 */
    } reg;
} SOC_DMSS_AMI_SEC_RGN_MAP0_UNION;
#endif
#define SOC_DMSS_AMI_SEC_RGN_MAP0_rgn_base_addr_START  (0)
#define SOC_DMSS_AMI_SEC_RGN_MAP0_rgn_base_addr_END    (19)
#define SOC_DMSS_AMI_SEC_RGN_MAP0_rgn_en_START         (31)
#define SOC_DMSS_AMI_SEC_RGN_MAP0_rgn_en_END           (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_RGN_MAP1_UNION
 结构说明  : AMI_SEC_RGN_MAP1 寄存器结构定义。地址偏移量:0x12104+0x30*(ami_rgns)+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域映射1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rgn_top_addr : 20; /* bit[0-19] : 各个地址区域的结束地址设置（高位部分）：
                                                        设置地址64KB以上的高20位，高位地址位宽不足则补0。
                                                        注意：
                                                        1.rgn之间（包括与ASI的SEC Region之间）不允许地址重叠；
                                                        2.结束地址不可配置的低位部分为全1，结束地址包含在当前rgn内；
                                                        3.若base_addr大于top_addr，则当前rgn不使能；
                                                        4.需保证exclusive命令访问的地址区域有响应的访问权限，否则exclusive命令无法被正常监控；
                                                        5.可设置的地址分辨率固定为64KB。 */
        unsigned int  reserved     : 8;  /* bit[20-27]: 保留。 */
        unsigned int  sp           : 4;  /* bit[28-31]: 当前区域的安全权限属性。
                                                        sp[3]：安全读属性；
                                                        sp[2]：安全写属性；
                                                        sp[1]：非安全读属性；
                                                        sp[0]：非安全写属性；
                                                        sp[n]=0：禁止访问；
                                                        sp[n]=1：允许访问。 */
    } reg;
} SOC_DMSS_AMI_SEC_RGN_MAP1_UNION;
#endif
#define SOC_DMSS_AMI_SEC_RGN_MAP1_rgn_top_addr_START  (0)
#define SOC_DMSS_AMI_SEC_RGN_MAP1_rgn_top_addr_END    (19)
#define SOC_DMSS_AMI_SEC_RGN_MAP1_sp_START            (28)
#define SOC_DMSS_AMI_SEC_RGN_MAP1_sp_END              (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_RGN_ATTR_UNION
 结构说明  : AMI_SEC_RGN_ATTR 寄存器结构定义。地址偏移量:0x12108+0x30*(ami_enhn_rgns)+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: 地址区域属性
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  etrust   : 1;  /* bit[0]   : 当前区域的enhance trusted属性使能：
                                                   0：不使能；
                                                   1：使能。
                                                   注意：
                                                   当前区域是否支持此功能与RTL配置有关，需要根据enhance trusted属性的regions个数判定（从AMI_RTL_INF0.rtl_enhn_sec_rgn_num可读出）。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_SEC_RGN_ATTR_UNION;
#endif
#define SOC_DMSS_AMI_SEC_RGN_ATTR_etrust_START    (0)
#define SOC_DMSS_AMI_SEC_RGN_ATTR_etrust_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_WR0_UNION
 结构说明  : AMI_SEC_MID_WR0 寄存器结构定义。地址偏移量:0x12110+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr0 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_WR0_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_WR0_mid_sel_wr0_START  (0)
#define SOC_DMSS_AMI_SEC_MID_WR0_mid_sel_wr0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_WR1_UNION
 结构说明  : AMI_SEC_MID_WR1 寄存器结构定义。地址偏移量:0x12114+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr1 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为32+n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为32+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_WR1_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_WR1_mid_sel_wr1_START  (0)
#define SOC_DMSS_AMI_SEC_MID_WR1_mid_sel_wr1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_WR2_UNION
 结构说明  : AMI_SEC_MID_WR2 寄存器结构定义。地址偏移量:0x12118+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr2 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为64+n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为64+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_WR2_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_WR2_mid_sel_wr2_START  (0)
#define SOC_DMSS_AMI_SEC_MID_WR2_mid_sel_wr2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_WR3_UNION
 结构说明  : AMI_SEC_MID_WR3 寄存器结构定义。地址偏移量:0x1211C+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 写命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_wr3 : 32; /* bit[0-31]: MID的比特匹配选择（写命令）：
                                                      mid_sel_wr[n]=0：写命令MID为96+n时，此命令为MID匹配失败；
                                                      mid_sel_wr[n]=1：写命令MID为96+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_WR3_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_WR3_mid_sel_wr3_START  (0)
#define SOC_DMSS_AMI_SEC_MID_WR3_mid_sel_wr3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_RD0_UNION
 结构说明  : AMI_SEC_MID_RD0 寄存器结构定义。地址偏移量:0x12120+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd0 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_RD0_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_RD0_mid_sel_rd0_START  (0)
#define SOC_DMSS_AMI_SEC_MID_RD0_mid_sel_rd0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_RD1_UNION
 结构说明  : AMI_SEC_MID_RD1 寄存器结构定义。地址偏移量:0x12124+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd1 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为32+n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为32+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_RD1_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_RD1_mid_sel_rd1_START  (0)
#define SOC_DMSS_AMI_SEC_MID_RD1_mid_sel_rd1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_RD2_UNION
 结构说明  : AMI_SEC_MID_RD2 寄存器结构定义。地址偏移量:0x12128+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd2 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为64+n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为64+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_RD2_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_RD2_mid_sel_rd2_START  (0)
#define SOC_DMSS_AMI_SEC_MID_RD2_mid_sel_rd2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_SEC_MID_RD3_UNION
 结构说明  : AMI_SEC_MID_RD3 寄存器结构定义。地址偏移量:0x1212C+0x30*(ami_mid_rgns)+0x800*(ami_base)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: 读命令匹配MasterID及其掩码
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mid_sel_rd3 : 32; /* bit[0-31]: MID的比特匹配选择（读命令）：
                                                      mid_sel_rd[n]=0：读命令MID为96+n时，此命令为MID匹配失败；
                                                      mid_sel_rd[n]=1：读命令MID为96+n时，此命令为MID匹配成功。
                                                      说明：
                                                      1.只有当MID成功匹配且获得安全权限时，当前region才有访问权限；
                                                      2.当前区域是否支持此功能与RTL配置有关，需要根据mid匹配regions个数判定（从AMI_RTL_INF0.rtl_mid_sec_rgn_num可读出）。 */
    } reg;
} SOC_DMSS_AMI_SEC_MID_RD3_UNION;
#endif
#define SOC_DMSS_AMI_SEC_MID_RD3_mid_sel_rd3_START  (0)
#define SOC_DMSS_AMI_SEC_MID_RD3_mid_sel_rd3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_DFX_MODULE_UNION
 结构说明  : AMI_DFX_MODULE 寄存器结构定义。地址偏移量:0x12700+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：各个主要模块的工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_busy     : 1;  /* bit[0]   : AMI的工作状态：
                                                       0：AMI处于idle状态；
                                                       1：AMI处于busy状态。 */
        unsigned int  ami_sec_busy : 1;  /* bit[1]   : AMI的SEC模块工作状态：
                                                       0：AMI SEC模块处于idle状态；
                                                       1：AMI SEC模块处于busy状态。 */
        unsigned int  reserved     : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_DFX_MODULE_UNION;
#endif
#define SOC_DMSS_AMI_DFX_MODULE_ami_busy_START      (0)
#define SOC_DMSS_AMI_DFX_MODULE_ami_busy_END        (0)
#define SOC_DMSS_AMI_DFX_MODULE_ami_sec_busy_START  (1)
#define SOC_DMSS_AMI_DFX_MODULE_ami_sec_busy_END    (1)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_DFX_FIFO_UNION
 结构说明  : AMI_DFX_FIFO 寄存器结构定义。地址偏移量:0x12704+0x800*(ami_base)，初值:0x00055555，宽度:32
 寄存器说明: DFX：FIFO工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_cmd_fifo_st   : 2;  /* bit[0-1]  : AMI内的读写命令FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x11：reserved。 */
        unsigned int  ami_rcmd_fifo_st  : 2;  /* bit[2-3]  : AMI内的读命令FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x10：reserved。 */
        unsigned int  ami_wcmd_fifo_st  : 2;  /* bit[4-5]  : AMI内的读命令FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x9：reserved。 */
        unsigned int  ami_warb_fifo_st  : 2;  /* bit[6-7]  : AMI内的写仲裁结果FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x8：reserved。 */
        unsigned int  ami_wd_fifo_st    : 2;  /* bit[8-9]  : AMI内的写数据FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x7：reserved。 */
        unsigned int  mpu_rcmd_fifo_st  : 2;  /* bit[10-11]: AMI内的读命令MPU鉴权结果的FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x6：reserved。 */
        unsigned int  mpu_wcmd_fifo_st  : 2;  /* bit[12-13]: AMI内的写命令MPU鉴权结果的FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x5：reserved。 */
        unsigned int  mpu_wd_fifo_st    : 2;  /* bit[14-15]: AMI内的写数据MPU鉴权结果的FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x4：reserved。 */
        unsigned int  mpu_rfail_fifo_st : 2;  /* bit[16-17]: AMI内的MPU拦截的读命令FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x4：reserved。 */
        unsigned int  mpu_wfail_fifo_st : 2;  /* bit[18-19]: AMI内的MPU拦截的写命令FIFO状态：
                                                             0x0：busy；
                                                             0x1：emtpy；
                                                             0x2：full；
                                                             0x4：reserved。 */
        unsigned int  reserved          : 12; /* bit[20-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_DFX_FIFO_UNION;
#endif
#define SOC_DMSS_AMI_DFX_FIFO_ami_cmd_fifo_st_START    (0)
#define SOC_DMSS_AMI_DFX_FIFO_ami_cmd_fifo_st_END      (1)
#define SOC_DMSS_AMI_DFX_FIFO_ami_rcmd_fifo_st_START   (2)
#define SOC_DMSS_AMI_DFX_FIFO_ami_rcmd_fifo_st_END     (3)
#define SOC_DMSS_AMI_DFX_FIFO_ami_wcmd_fifo_st_START   (4)
#define SOC_DMSS_AMI_DFX_FIFO_ami_wcmd_fifo_st_END     (5)
#define SOC_DMSS_AMI_DFX_FIFO_ami_warb_fifo_st_START   (6)
#define SOC_DMSS_AMI_DFX_FIFO_ami_warb_fifo_st_END     (7)
#define SOC_DMSS_AMI_DFX_FIFO_ami_wd_fifo_st_START     (8)
#define SOC_DMSS_AMI_DFX_FIFO_ami_wd_fifo_st_END       (9)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_rcmd_fifo_st_START   (10)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_rcmd_fifo_st_END     (11)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_wcmd_fifo_st_START   (12)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_wcmd_fifo_st_END     (13)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_wd_fifo_st_START     (14)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_wd_fifo_st_END       (15)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_rfail_fifo_st_START  (16)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_rfail_fifo_st_END    (17)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_wfail_fifo_st_START  (18)
#define SOC_DMSS_AMI_DFX_FIFO_mpu_wfail_fifo_st_END    (19)


/*****************************************************************************
 结构名    : SOC_DMSS_AMI_DFX_ST_UNION
 结构说明  : AMI_DFX_ST 寄存器结构定义。地址偏移量:0x12708+0x800*(ami_base)，初值:0x00000000，宽度:32
 寄存器说明: DFX：AMI的模块状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ami_grant     : 16; /* bit[0-15] : AMI的命令仲裁状态：
                                                         ami_grant[n]=0：ASIn未获得AMI的命令仲裁；
                                                         ami_grant[n]=1：ASIn获得了AMI的命令仲裁。 */
        unsigned int  ami_table_cnt : 8;  /* bit[16-23]: AMI模块中命令table中的命令个数。 */
        unsigned int  ar_bp         : 1;  /* bit[24]   : AMI下发读命令的被反压状态：
                                                         ar_bp=0：AMI没有下发读命令，或下发读命令被接收；
                                                         ar_bp=1：AMI有下发读命令且被下游反压。 */
        unsigned int  aw_bp         : 1;  /* bit[25]   : AMI下发写命令的被反压状态：
                                                         ar_bp=0：AMI没有下发写命令，或下发写命令被接收；
                                                         ar_bp=1：AMI有下发写命令且被下游反压。 */
        unsigned int  w_bp          : 1;  /* bit[26]   : AMI下发写数据的被反压状态：
                                                         ar_bp=0：AMI没有下发写数据，或下发写数据被接收；
                                                         ar_bp=1：AMI有下发写数据且被下游反压。 */
        unsigned int  r_bp          : 1;  /* bit[27]   : AMI接收读数据的被反压状态：
                                                         ar_bp=0：AMI没有收到读数据，或读数据被接收；
                                                         ar_bp=1：AMI有收到读数据且被AMI反压。 */
        unsigned int  b_bp          : 1;  /* bit[28]   : AMI接收写响应的被反压状态：
                                                         ar_bp=0：AMI没有收到写响应，或写响应被接收；
                                                         ar_bp=1：AMI有收到写响应且被AMI反压。 */
        unsigned int  reserved      : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_DMSS_AMI_DFX_ST_UNION;
#endif
#define SOC_DMSS_AMI_DFX_ST_ami_grant_START      (0)
#define SOC_DMSS_AMI_DFX_ST_ami_grant_END        (15)
#define SOC_DMSS_AMI_DFX_ST_ami_table_cnt_START  (16)
#define SOC_DMSS_AMI_DFX_ST_ami_table_cnt_END    (23)
#define SOC_DMSS_AMI_DFX_ST_ar_bp_START          (24)
#define SOC_DMSS_AMI_DFX_ST_ar_bp_END            (24)
#define SOC_DMSS_AMI_DFX_ST_aw_bp_START          (25)
#define SOC_DMSS_AMI_DFX_ST_aw_bp_END            (25)
#define SOC_DMSS_AMI_DFX_ST_w_bp_START           (26)
#define SOC_DMSS_AMI_DFX_ST_w_bp_END             (26)
#define SOC_DMSS_AMI_DFX_ST_r_bp_START           (27)
#define SOC_DMSS_AMI_DFX_ST_r_bp_END             (27)
#define SOC_DMSS_AMI_DFX_ST_b_bp_START           (28)
#define SOC_DMSS_AMI_DFX_ST_b_bp_END             (28)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RTL_VER_UNION
 结构说明  : GLB_RTL_VER 寄存器结构定义。地址偏移量:0x16000，初值:0x03000000，宽度:32
 寄存器说明: 代码版本信息
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_tag  : 8;  /* bit[0-7]  : RTL的设计TAG编号。 */
        unsigned int  reserved_0: 8;  /* bit[8-15] : 保留。 */
        unsigned int  version  : 12; /* bit[16-27]: 版本编号：
                                                    0x100：HiDMSSV100；
                                                    0x110：HiDMSSV110；
                                                    0x200：HiDMSSV200；
                                                    0x210：HiDMSSV210；
                                                    0x220：HiDMSSV220；
                                                    0x230：HiDMSSV230；
                                                    0x300: HiDMSSV300。 */
        unsigned int  reserved_1: 4;  /* bit[28-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RTL_VER_UNION;
#endif
#define SOC_DMSS_GLB_RTL_VER_rtl_tag_START   (0)
#define SOC_DMSS_GLB_RTL_VER_rtl_tag_END     (7)
#define SOC_DMSS_GLB_RTL_VER_version_START   (16)
#define SOC_DMSS_GLB_RTL_VER_version_END     (27)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RTL_INF0_UNION
 结构说明  : GLB_RTL_INF0 寄存器结构定义。地址偏移量:0x16004，初值:0x00000000，宽度:32
 寄存器说明: 代码配置信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_addr_width     : 4;  /* bit[0-3]  : AXI总线地址位宽：
                                                              0:32bits；
                                                              1:33bits；
                                                              2:34bits；
                                                              3:35bits；
                                                              4:36bits；
                                                              5:37bits；
                                                              6:38bits；
                                                              7:39bits；
                                                              8:40bits；
                                                              其它：reserved。 */
        unsigned int  rtl_id_width       : 5;  /* bit[4-8]  : AXI命令ID位宽：
                                                              0:reserved；
                                                              1:1bits；
                                                              2:2bits；
                                                              ……
                                                              31:31bits。 */
        unsigned int  reserved_0         : 3;  /* bit[9-11] : 保留。 */
        unsigned int  rtl_mid_width      : 4;  /* bit[12-15]: Master ID位宽：
                                                              0:reserved；
                                                              1:1bits；
                                                              2:2bits；
                                                              ……
                                                              15:15bits。 */
        unsigned int  rtl_dmi_num        : 1;  /* bit[16]   : DMI(DDRC Master Interface)个数：
                                                              0：1个DMI；
                                                              1：2个DMI。 */
        unsigned int  reserved_1         : 1;  /* bit[17]   : 保留。 */
        unsigned int  rtl_ch_num_pre_dmi : 1;  /* bit[18]   : 每个DMI对接的通道个数：
                                                              0：1 channel per DMI；
                                                              1：2 channel per DMI。 */
        unsigned int  reserved_2         : 1;  /* bit[19]   : 保留。 */
        unsigned int  rtl_asi_num        : 4;  /* bit[20-23]: ASI（AXI Slave Interface）个数：
                                                              0x0： 1个ASI；
                                                              ……
                                                              0xB：12个ASI；
                                                              其它：reserved。 */
        unsigned int  rtl_exclusive_num  : 6;  /* bit[24-29]: 可同时监视的不同exclusive命令的最大个数：
                                                              0x00：不支持exclusive命令；
                                                              0x01：1个；
                                                              0x02：2个；
                                                              ……
                                                              0x1F：31个；
                                                              0x20：32个；
                                                              其它：reserved。 */
        unsigned int  reserved_3         : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RTL_INF0_UNION;
#endif
#define SOC_DMSS_GLB_RTL_INF0_rtl_addr_width_START      (0)
#define SOC_DMSS_GLB_RTL_INF0_rtl_addr_width_END        (3)
#define SOC_DMSS_GLB_RTL_INF0_rtl_id_width_START        (4)
#define SOC_DMSS_GLB_RTL_INF0_rtl_id_width_END          (8)
#define SOC_DMSS_GLB_RTL_INF0_rtl_mid_width_START       (12)
#define SOC_DMSS_GLB_RTL_INF0_rtl_mid_width_END         (15)
#define SOC_DMSS_GLB_RTL_INF0_rtl_dmi_num_START         (16)
#define SOC_DMSS_GLB_RTL_INF0_rtl_dmi_num_END           (16)
#define SOC_DMSS_GLB_RTL_INF0_rtl_ch_num_pre_dmi_START  (18)
#define SOC_DMSS_GLB_RTL_INF0_rtl_ch_num_pre_dmi_END    (18)
#define SOC_DMSS_GLB_RTL_INF0_rtl_asi_num_START         (20)
#define SOC_DMSS_GLB_RTL_INF0_rtl_asi_num_END           (23)
#define SOC_DMSS_GLB_RTL_INF0_rtl_exclusive_num_START   (24)
#define SOC_DMSS_GLB_RTL_INF0_rtl_exclusive_num_END     (29)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RTL_INF1_UNION
 结构说明  : GLB_RTL_INF1 寄存器结构定义。地址偏移量:0x16008，初值:0x00000000，宽度:32
 寄存器说明: 代码配置信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rtl_rbuf_buf_num : 6;  /* bit[0-5]  : RBUF模块的数据buffer深度：
                                                            0：未例化RBUF，使用了cross bar的交换结构；
                                                            1：2 read burst；
                                                            2：3 read burst；
                                                            ……
                                                            63：64 read burst。 */
        unsigned int  reserved_0       : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  rtl_mst_flux_num : 4;  /* bit[8-11] : 闭环流控的MST_FLUX配置：
                                                            0：不支持MST_FLUX相关功能；
                                                            1：1组MST_FLUX控制信号；
                                                            ……
                                                            15：15组MST_FLUX控制信号。 */
        unsigned int  rtl_perf_stat    : 1;  /* bit[12]   : 性能监控模块perf_stat配置：
                                                            0：不支持perf_stat；
                                                            1：支持perf_stat。 */
        unsigned int  reserved_1       : 19; /* bit[13-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RTL_INF1_UNION;
#endif
#define SOC_DMSS_GLB_RTL_INF1_rtl_rbuf_buf_num_START  (0)
#define SOC_DMSS_GLB_RTL_INF1_rtl_rbuf_buf_num_END    (5)
#define SOC_DMSS_GLB_RTL_INF1_rtl_mst_flux_num_START  (8)
#define SOC_DMSS_GLB_RTL_INF1_rtl_mst_flux_num_END    (11)
#define SOC_DMSS_GLB_RTL_INF1_rtl_perf_stat_START     (12)
#define SOC_DMSS_GLB_RTL_INF1_rtl_perf_stat_END       (12)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RAM_TMOD0_UNION
 结构说明  : GLB_RAM_TMOD0 寄存器结构定义。地址偏移量:0x16010，初值:0x00000000，宽度:32
 寄存器说明: TP-MEMORY配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ram_tmod0 : 16; /* bit[0-15] : 根据TP-MEMORY的不同类型及工艺配置。 */
        unsigned int  ram_tmod1 : 16; /* bit[16-31]: 根据TP-MEMORY的不同类型及工艺配置。 */
    } reg;
} SOC_DMSS_GLB_RAM_TMOD0_UNION;
#endif
#define SOC_DMSS_GLB_RAM_TMOD0_ram_tmod0_START  (0)
#define SOC_DMSS_GLB_RAM_TMOD0_ram_tmod0_END    (15)
#define SOC_DMSS_GLB_RAM_TMOD0_ram_tmod1_START  (16)
#define SOC_DMSS_GLB_RAM_TMOD0_ram_tmod1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RAM_TMOD1_UNION
 结构说明  : GLB_RAM_TMOD1 寄存器结构定义。地址偏移量:0x16014，初值:0x00000000，宽度:32
 寄存器说明: TP-MEMORY配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ram_tmod2 : 16; /* bit[0-15] : 根据TP-MEMORY的不同类型及工艺配置。 */
        unsigned int  ram_tmod3 : 16; /* bit[16-31]: 根据TP-MEMORY的不同类型及工艺配置。 */
    } reg;
} SOC_DMSS_GLB_RAM_TMOD1_UNION;
#endif
#define SOC_DMSS_GLB_RAM_TMOD1_ram_tmod2_START  (0)
#define SOC_DMSS_GLB_RAM_TMOD1_ram_tmod2_END    (15)
#define SOC_DMSS_GLB_RAM_TMOD1_ram_tmod3_START  (16)
#define SOC_DMSS_GLB_RAM_TMOD1_ram_tmod3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RAM_SMOD_UNION
 结构说明  : GLB_RAM_SMOD 寄存器结构定义。地址偏移量:0x16018，初值:0x00000000，宽度:32
 寄存器说明: SP-MEMORY配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ram_smod : 26; /* bit[0-25] : 根据SP-MEMORY的不同类型及工艺配置。 */
        unsigned int  reserved : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RAM_SMOD_UNION;
#endif
#define SOC_DMSS_GLB_RAM_SMOD_ram_smod_START  (0)
#define SOC_DMSS_GLB_RAM_SMOD_ram_smod_END    (25)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DYN_CKG_UNION
 结构说明  : GLB_DYN_CKG 寄存器结构定义。地址偏移量:0x16024，初值:0x00000200，宽度:32
 寄存器说明: 动态时钟门控
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ckg_byp_rbuf_src : 1;  /* bit[0]    : RBUF_SRC模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_rbuf_dst : 1;  /* bit[1]    : RBUF_DST模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_ra_rs    : 1;  /* bit[2]    : RBUF到ASI读数据RS模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_rbuf_rs  : 1;  /* bit[3]    : RBUF输入端RS模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_dcq      : 1;  /* bit[4]    : DCQ模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_mst_flux : 1;  /* bit[5]    : MST_FLUX模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_dcq_stat : 1;  /* bit[6]    : DCQ模块中STAT模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_ram      : 1;  /* bit[7]    : RBUF和RDR模块中RAM的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_trace    : 1;  /* bit[8]    : TRACE模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  ckg_byp_cfg      : 1;  /* bit[9]    : 配置寄存器模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。
                                                            注意：配置寄存器模块的动态时钟门控默认关闭，时钟常开，初始化流程需要使能。 */
        unsigned int  ckg_byp_ptrn_rt  : 1;  /* bit[10]   : PTRN RT模块的动态时钟门控：
                                                            0：模块idle时，自动关闭时钟；
                                                            1：时钟常开。 */
        unsigned int  reserved         : 21; /* bit[11-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_DYN_CKG_UNION;
#endif
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_rbuf_src_START  (0)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_rbuf_src_END    (0)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_rbuf_dst_START  (1)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_rbuf_dst_END    (1)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_ra_rs_START     (2)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_ra_rs_END       (2)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_rbuf_rs_START   (3)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_rbuf_rs_END     (3)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_dcq_START       (4)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_dcq_END         (4)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_mst_flux_START  (5)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_mst_flux_END    (5)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_dcq_stat_START  (6)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_dcq_stat_END    (6)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_ram_START       (7)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_ram_END         (7)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_trace_START     (8)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_trace_END       (8)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_cfg_START       (9)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_cfg_END         (9)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_ptrn_rt_START   (10)
#define SOC_DMSS_GLB_DYN_CKG_ckg_byp_ptrn_rt_END     (10)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_INT_STATUS0_UNION
 结构说明  : GLB_INT_STATUS0 寄存器结构定义。地址偏移量:0x16030，初值:0x00000000，宽度:32
 寄存器说明: 全局中断状态查看
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_dfx_int  : 16; /* bit[0-15] : DFX功能中断状态：
                                                        asi_dfx_int[n]=1：DFX中断来自ASIn。
                                                        说明：
                                                        1.中断状态可在对应ASI的寄存器ASI_DFX_INT_STATUS查看；
                                                        2.中断使能可在对应ASI的寄存器ASI_DFX_INT_EN中配置。 */
        unsigned int  ami_prot_int : 1;  /* bit[16]   : Protected命令越权访问AMI中断。
                                                        说明：
                                                        中断使能可在ASI寄存器ASI_AMI_PROT_INT_EN中配置。 */
        unsigned int  ami_enhn_int : 1;  /* bit[17]   : Enhance Trusted命令越权访问AMI中断。
                                                        说明：
                                                        中断使能可在ASI寄存器ASI_AMI_ENHN_INT_EN中配置。 */
        unsigned int  rsp_err_int  : 1;  /* bit[18]   : AMI0收到ERROR响应的中断。
                                                        说明：
                                                        中断使能可在寄存器GLB_RSP_ERR_INT_EN中配置。 */
        unsigned int  reserved     : 13; /* bit[19-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_INT_STATUS0_UNION;
#endif
#define SOC_DMSS_GLB_INT_STATUS0_asi_dfx_int_START   (0)
#define SOC_DMSS_GLB_INT_STATUS0_asi_dfx_int_END     (15)
#define SOC_DMSS_GLB_INT_STATUS0_ami_prot_int_START  (16)
#define SOC_DMSS_GLB_INT_STATUS0_ami_prot_int_END    (16)
#define SOC_DMSS_GLB_INT_STATUS0_ami_enhn_int_START  (17)
#define SOC_DMSS_GLB_INT_STATUS0_ami_enhn_int_END    (17)
#define SOC_DMSS_GLB_INT_STATUS0_rsp_err_int_START   (18)
#define SOC_DMSS_GLB_INT_STATUS0_rsp_err_int_END     (18)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_INT_CLEAR0_UNION
 结构说明  : GLB_INT_CLEAR0 寄存器结构定义。地址偏移量:0x16034，初值:0x00000000，宽度:32
 寄存器说明: 全局中断状态清除
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_dfx_int_clear  : 16; /* bit[0-15] : 对应bit写1，可清除对应ASI的DFX功能中断状态：
                                                              asi_dfx_int_clear[n]=1：清除的DFX中断状态来自ASIn。 */
        unsigned int  ami_prot_int_clear : 1;  /* bit[16]   : 对应bit写1，可清除Protected命令越权访问AMI中断状态。 */
        unsigned int  ami_enhn_int_clear : 1;  /* bit[17]   : 对应bit写1，可清除Enhance Trusted命令越权访问AMI中断状态。 */
        unsigned int  rsp_err_int_clear  : 1;  /* bit[18]   : 对应bit写1，可清除AMI0收到ERROR响应的中断。 */
        unsigned int  reserved           : 13; /* bit[19-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_INT_CLEAR0_UNION;
#endif
#define SOC_DMSS_GLB_INT_CLEAR0_asi_dfx_int_clear_START   (0)
#define SOC_DMSS_GLB_INT_CLEAR0_asi_dfx_int_clear_END     (15)
#define SOC_DMSS_GLB_INT_CLEAR0_ami_prot_int_clear_START  (16)
#define SOC_DMSS_GLB_INT_CLEAR0_ami_prot_int_clear_END    (16)
#define SOC_DMSS_GLB_INT_CLEAR0_ami_enhn_int_clear_START  (17)
#define SOC_DMSS_GLB_INT_CLEAR0_ami_enhn_int_clear_END    (17)
#define SOC_DMSS_GLB_INT_CLEAR0_rsp_err_int_clear_START   (18)
#define SOC_DMSS_GLB_INT_CLEAR0_rsp_err_int_clear_END     (18)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_INT_STATUS1_UNION
 结构说明  : GLB_INT_STATUS1 寄存器结构定义。地址偏移量:0x16038，初值:0x00000000，宽度:32
 寄存器说明: 全局中断状态查看
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_sec_int      : 16; /* bit[0-15] : ASI安全功能中断状态：
                                                            asi_sec_int[n]=1：安全中断来自ASIn。
                                                            说明：
                                                            1.中断状态可在对应ASI的寄存器ASI_SEC_INT_STATUS查看；
                                                            2.中断使能可在对应ASI的寄存器ASI_SEC_INT_EN中配置。 */
        unsigned int  dmi_sec_int      : 2;  /* bit[16-17]: DMI安全功能中断状态：
                                                            dmi_sec_int[n]=1：安全中断来自DMIn。
                                                            说明：
                                                            1.中断状态可在对应DMI的寄存器DMI_SEC_INT_STATUS查看；
                                                            2.中断使能可在对应DMI的寄存器DMI_SEC_INT_EN中配置。 */
        unsigned int  dmi_enhn_int     : 2;  /* bit[18-19]: DMI enhance tursted功能中断状态：
                                                            dmi_enhn_int[n]=1：安全中断来自DMIn。
                                                            说明：
                                                            1.中断状态可在对应DMI的寄存器DMI_SEC_INT_STATUS查看；
                                                            2.中断使能可在对应DMI的寄存器DMI_SEC_INT_EN中配置。 */
        unsigned int  dmi_enhn_cfg_int : 1;  /* bit[20]   : DMI enhance配置中断。
                                                            说明：
                                                            中断使能可在DMI寄存器DMI_ENHN_INT_EN中配置。 */
        unsigned int  ami_sec_int      : 1;  /* bit[21]   : AMI安全功能中断。
                                                            说明：
                                                            中断使能可在AMI寄存器AMI_SEC_INT_EN中配置。 */
        unsigned int  mpu_int          : 1;  /* bit[22]   : MPU中断。
                                                            说明：
                                                            中断使能可在寄存器GLB_MPU_INT_EN中配置。 */
        unsigned int  detect_int       : 1;  /* bit[23]   : Patter Detect中断。
                                                            说明：
                                                            中断使能可在ASI寄存器ASI_DETECT_CFG0中配置。 */
        unsigned int  trace_int        : 1;  /* bit[24]   : TRACE中断。
                                                            说明：
                                                            中断使能可在寄存器GLB_TRACE_INT_EN中配置。 */
        unsigned int  rbuf_stat_int    : 1;  /* bit[25]   : RBUF统计中断。
                                                            说明：
                                                            中断使能可在寄存器GLB_STAT_RBUF_CTRL中配置。 */
        unsigned int  reserved         : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_INT_STATUS1_UNION;
#endif
#define SOC_DMSS_GLB_INT_STATUS1_asi_sec_int_START       (0)
#define SOC_DMSS_GLB_INT_STATUS1_asi_sec_int_END         (15)
#define SOC_DMSS_GLB_INT_STATUS1_dmi_sec_int_START       (16)
#define SOC_DMSS_GLB_INT_STATUS1_dmi_sec_int_END         (17)
#define SOC_DMSS_GLB_INT_STATUS1_dmi_enhn_int_START      (18)
#define SOC_DMSS_GLB_INT_STATUS1_dmi_enhn_int_END        (19)
#define SOC_DMSS_GLB_INT_STATUS1_dmi_enhn_cfg_int_START  (20)
#define SOC_DMSS_GLB_INT_STATUS1_dmi_enhn_cfg_int_END    (20)
#define SOC_DMSS_GLB_INT_STATUS1_ami_sec_int_START       (21)
#define SOC_DMSS_GLB_INT_STATUS1_ami_sec_int_END         (21)
#define SOC_DMSS_GLB_INT_STATUS1_mpu_int_START           (22)
#define SOC_DMSS_GLB_INT_STATUS1_mpu_int_END             (22)
#define SOC_DMSS_GLB_INT_STATUS1_detect_int_START        (23)
#define SOC_DMSS_GLB_INT_STATUS1_detect_int_END          (23)
#define SOC_DMSS_GLB_INT_STATUS1_trace_int_START         (24)
#define SOC_DMSS_GLB_INT_STATUS1_trace_int_END           (24)
#define SOC_DMSS_GLB_INT_STATUS1_rbuf_stat_int_START     (25)
#define SOC_DMSS_GLB_INT_STATUS1_rbuf_stat_int_END       (25)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_INT_CLEAR1_UNION
 结构说明  : GLB_INT_CLEAR1 寄存器结构定义。地址偏移量:0x1603C，初值:0x00000000，宽度:32
 寄存器说明: 全局中断状态清除
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_sec_int_clear      : 16; /* bit[0-15] : 对应bit写1，可清除对应ASI的安全功能中断状态：
                                                                  asi_sec_int_clear[n]=1：清除的安全功能中断状态来自ASIn。 */
        unsigned int  dmi_sec_int_clear      : 2;  /* bit[16-17]: 对应bit写1，可清除对应DMI的安全功能中断状态：
                                                                  dmi_sec_int_clear[n]=1：清除的安全功能中断状态来自DMIn。 */
        unsigned int  dmi_enhn_int_clear     : 2;  /* bit[18-19]: 对应bit写1，可清除对应DMI的enhance tursted功能中断状态：
                                                                  dmi_sec_int_clear[n]=1：清除的enhance tursted功能中断状态来自DMIn。 */
        unsigned int  dmi_enhn_cfg_int_clear : 1;  /* bit[20]   : 对应bit写1，可清除DMI enhance配置功能中断状态 */
        unsigned int  ami_sec_int_clear      : 1;  /* bit[21]   : 对应bit写1，可清除AMI安全功能中断状态 */
        unsigned int  mpu_int_clear          : 1;  /* bit[22]   : 对应bit写1，可清除MPU功能中断状态。 */
        unsigned int  detect_int_clear       : 1;  /* bit[23]   : 对应bit写1，可清除DETECT功能中断状态。 */
        unsigned int  trace_int_clear        : 1;  /* bit[24]   : 对应bit写1，可清除TRACE功能中断状态。 */
        unsigned int  rbuf_stat_int_clear    : 1;  /* bit[25]   : 对应bit写1，可清除RBUF统计中断状态。 */
        unsigned int  reserved               : 6;  /* bit[26-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_INT_CLEAR1_UNION;
#endif
#define SOC_DMSS_GLB_INT_CLEAR1_asi_sec_int_clear_START       (0)
#define SOC_DMSS_GLB_INT_CLEAR1_asi_sec_int_clear_END         (15)
#define SOC_DMSS_GLB_INT_CLEAR1_dmi_sec_int_clear_START       (16)
#define SOC_DMSS_GLB_INT_CLEAR1_dmi_sec_int_clear_END         (17)
#define SOC_DMSS_GLB_INT_CLEAR1_dmi_enhn_int_clear_START      (18)
#define SOC_DMSS_GLB_INT_CLEAR1_dmi_enhn_int_clear_END        (19)
#define SOC_DMSS_GLB_INT_CLEAR1_dmi_enhn_cfg_int_clear_START  (20)
#define SOC_DMSS_GLB_INT_CLEAR1_dmi_enhn_cfg_int_clear_END    (20)
#define SOC_DMSS_GLB_INT_CLEAR1_ami_sec_int_clear_START       (21)
#define SOC_DMSS_GLB_INT_CLEAR1_ami_sec_int_clear_END         (21)
#define SOC_DMSS_GLB_INT_CLEAR1_mpu_int_clear_START           (22)
#define SOC_DMSS_GLB_INT_CLEAR1_mpu_int_clear_END             (22)
#define SOC_DMSS_GLB_INT_CLEAR1_detect_int_clear_START        (23)
#define SOC_DMSS_GLB_INT_CLEAR1_detect_int_clear_END          (23)
#define SOC_DMSS_GLB_INT_CLEAR1_trace_int_clear_START         (24)
#define SOC_DMSS_GLB_INT_CLEAR1_trace_int_clear_END           (24)
#define SOC_DMSS_GLB_INT_CLEAR1_rbuf_stat_int_clear_START     (25)
#define SOC_DMSS_GLB_INT_CLEAR1_rbuf_stat_int_clear_END       (25)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_EXCLU_UNION
 结构说明  : GLB_EXCLU 寄存器结构定义。地址偏移量:0x16040，初值:0x00000121，宽度:32
 寄存器说明: exclusive命令配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  exclu_en          : 1;  /* bit[0]   : exclusive命令使能：
                                                            0：禁止：忽略exclusive命令，当做普通命令处理；
                                                            1：使能：支持exclusive命令。 */
        unsigned int  reserved_0        : 3;  /* bit[1-3] : 保留。 */
        unsigned int  exclu_id_sel      : 3;  /* bit[4-6] : exclusive命令监控ID选择：
                                                            exclu_id_sel[2]：选择命令的Master_ID；
                                                            exclu_id_sel[1]：选择命令的AXI_ID；
                                                            exclu_id_sel[0]：选择命令的Port_ID；
                                                            0：不参与exclusive-monitor的ID监控；
                                                            1：参与exclusive-monitor的ID监控。 */
        unsigned int  reserved_1        : 1;  /* bit[7]   : 保留。 */
        unsigned int  exclu_clrmonhk_en : 1;  /* bit[8]   : exclusive clrexmon硬件握手使能：
                                                            0：禁止；
                                                            1：使能。 */
        unsigned int  reserved_2        : 23; /* bit[9-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_EXCLU_UNION;
#endif
#define SOC_DMSS_GLB_EXCLU_exclu_en_START           (0)
#define SOC_DMSS_GLB_EXCLU_exclu_en_END             (0)
#define SOC_DMSS_GLB_EXCLU_exclu_id_sel_START       (4)
#define SOC_DMSS_GLB_EXCLU_exclu_id_sel_END         (6)
#define SOC_DMSS_GLB_EXCLU_exclu_clrmonhk_en_START  (8)
#define SOC_DMSS_GLB_EXCLU_exclu_clrmonhk_en_END    (8)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_ADDR_INTLV_UNION
 结构说明  : GLB_ADDR_INTLV 寄存器结构定义。地址偏移量:0x16100，初值:0x00000010，宽度:32
 寄存器说明: 全局地址交织模式
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  intlv_mode : 4;  /* bit[0-3]  : 通道映射模式：
                                                      0x0：全通道交织；
                                                      0x4：通道0映射（命令仅访问通道0）；
                                                      0x5：通道1映射（命令仅访问通道1）；
                                                      0x6：通道2映射（命令仅访问通道2）；
                                                      0x7：通道3映射（命令仅访问通道3）；
                                                      0x8：通道0和1交织（命令仅访问通道0和1）；
                                                      0x9：通道2和3交织（命令仅访问通道2和3）；
                                                      0xA：通道0和2交织（命令仅访问通道0和2）；
                                                      0xB：通道1和3交织（命令仅访问通道1和3）；
                                                      其他：reserved。
                                                      注意：
                                                      1.此寄存器只能在系统初始化时静态配置；
                                                      2.此配置需考虑RTL的最大通道配置数目；
                                                      3.除了全通道交织模式外，其他通道映射模式仅用于测试且ch4_order固定为性能优化模式；
                                                      4.通道0和2在同一个DMI内，通道1和3在同一个DMI内。 */
        unsigned int  intlv_gran : 2;  /* bit[4-5]  : 地址交织粒度：
                                                      0x0：128Byte；
                                                      0x1：256Byte；
                                                      0x2：512Byte；
                                                      0x3：1024Byte。
                                                      注意：
                                                      1.命令以此粒度为边界，被交替分发到多个通道；
                                                      2.地址交织粒度必须大于等于地址对齐粒度(由RTL宏定义)。
                                                      3.此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved_0 : 6;  /* bit[6-11] : 保留。 */
        unsigned int  ch4_order  : 1;  /* bit[12]   : 物理上有四个通道时的通道排列编号顺序：
                                                      0x0：功耗优化模式（四个通道的编号顺序：0-2-1-3，即先DMI内交织后DMI间交织）；
                                                      0x1：性能优化模式（四个通道的编号顺序：0-1-2-3，即先DMI间交织后DMI内交织）。
                                                      注意：
                                                      1.通道0和2在同一个DMI内，通道1和3在同一个DMI内；
                                                      2.此寄存器只能在系统初始化时静态配置；
                                                      3.物理上非四个通道时，或通道映射模式为非全通道交织模式时，此寄存器无意义。 */
        unsigned int  reserved_1 : 19; /* bit[13-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_ADDR_INTLV_UNION;
#endif
#define SOC_DMSS_GLB_ADDR_INTLV_intlv_mode_START  (0)
#define SOC_DMSS_GLB_ADDR_INTLV_intlv_mode_END    (3)
#define SOC_DMSS_GLB_ADDR_INTLV_intlv_gran_START  (4)
#define SOC_DMSS_GLB_ADDR_INTLV_intlv_gran_END    (5)
#define SOC_DMSS_GLB_ADDR_INTLV_ch4_order_START   (12)
#define SOC_DMSS_GLB_ADDR_INTLV_ch4_order_END     (12)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_ADDR_SCRMBL_UNION
 结构说明  : GLB_ADDR_SCRMBL 寄存器结构定义。地址偏移量:0x16110，初值:0x00000000，宽度:32
 寄存器说明: 全局地址加扰模式
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  bnk_scrmbl : 4;  /* bit[0-3] : BANK地址扰码模式：
                                                     0：无扰码；
                                                     1：addr[14:12] = addr[14:12] xor addr[19:17]（DDR pagesize=4KB）；
                                                     2：addr[14:12] = addr[14:12] xor addr[20:18]（DDR pagesize=4KB）；
                                                     3：addr[14:12] = addr[14:12] xor addr[21:19]（DDR pagesize=4KB）；
                                                     4：addr[14:12] = addr[14:12] xor addr[22:20]（DDR pagesize=4KB）；
                                                     5：addr[13:11] = addr[13:11] xor addr[18:16]（DDR pagesize=2KB）；
                                                     6：addr[13:11] = addr[13:11] xor addr[19:17]（DDR pagesize=2KB）；
                                                     7：addr[13:11] = addr[13:11] xor addr[20:18]（DDR pagesize=2KB）；
                                                     8：addr[13:11] = addr[13:11] xor addr[21:19]（DDR pagesize=2KB）；
                                                     9~15：reserved。
                                                     说明：
                                                     1.此处的addr，是抽取交织位之后的通道内地址；
                                                     2.加扰规则为RTL定制。
                                                     3.此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved   : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_ADDR_SCRMBL_UNION;
#endif
#define SOC_DMSS_GLB_ADDR_SCRMBL_bnk_scrmbl_START  (0)
#define SOC_DMSS_GLB_ADDR_SCRMBL_bnk_scrmbl_END    (3)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RBUF_CTRL_UNION
 结构说明  : GLB_RBUF_CTRL 寄存器结构定义。地址偏移量:0x16148，初值:0x00000001，宽度:32
 寄存器说明: RBUF的控制配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rbuf_byp_path_en : 1;  /* bit[0]    : RBUF数据bypass通路的使能开关：
                                                            0：禁止：所有返回数据都必须经过数据BUF，RBUF的latency最小为4cycle；
                                                            1：使能：当数据BUF中无对应ASI的数据且通路不阻塞，且对应ASI的RTL为配置为Bypass使能时，返回数据可绕过数据BUF直接返回ASI，RBUF的latency最小为1； */
        unsigned int  reserved         : 15; /* bit[1-15] : 保留。 */
        unsigned int  rbuf_cnt_wl      : 8;  /* bit[16-23]: RBUF缓存的水线：
                                                            0：关闭RBUF缓存的水线功能；
                                                            ……
                                                            n：当前RBUF缓存的水线。
                                                            说明：
                                                            1.当前RBUF所有数据burst总数达到rbuf_cnt_wl时，送出反压信号给上游模块（DDRC），不再接受新的数据burst；
                                                            2.水线最大配置值为RBUF中的BUFFER深度。
                                                            注意：
                                                            此寄存器只能在系统初始化时静态配置。 */
        unsigned int  rbuf_urg_wl      : 8;  /* bit[24-31]: RBUF缓存的水线：
                                                            0：关闭RBUF缓存的水线功能；
                                                            1,2,3：reserve；
                                                            ……
                                                            n：当前RBUF达到紧急调度状态的缓存水线。
                                                            说明：
                                                            1.当前RBUF所有数据burst总数达到rbuf_urg_wl时，进入紧急调度状态，在调度获得更高的优先级；
                                                            2.水线最大配置值为RBUF中的BUFFER深度。
                                                            注意：
                                                            此寄存器只能在系统初始化时静态配置。 */
    } reg;
} SOC_DMSS_GLB_RBUF_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_RBUF_CTRL_rbuf_byp_path_en_START  (0)
#define SOC_DMSS_GLB_RBUF_CTRL_rbuf_byp_path_en_END    (0)
#define SOC_DMSS_GLB_RBUF_CTRL_rbuf_cnt_wl_START       (16)
#define SOC_DMSS_GLB_RBUF_CTRL_rbuf_cnt_wl_END         (23)
#define SOC_DMSS_GLB_RBUF_CTRL_rbuf_urg_wl_START       (24)
#define SOC_DMSS_GLB_RBUF_CTRL_rbuf_urg_wl_END         (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RBUF_TRANS_CTRL_UNION
 结构说明  : GLB_RBUF_TRANS_CTRL 寄存器结构定义。地址偏移量:0x1614C，初值:0x00000000，宽度:32
 寄存器说明: RBUF的传输控制配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rbuf_trans_req : 16; /* bit[0-15] : RBUF开始提传输请求的配置选择：
                                                          rbuf_trans_req[n]=0：RBUF对于ASIn的读命令，收到读数据Burst的第一个transfer后便请求调度；
                                                          rbuf_trans_req[n]=1：RBUF对于ASIn的读命令，收到完整的读数据Burst，或buf中存储当前读数据burst的entry已满（RBUF中每entry可存储8个transfer的数据）。
                                                          注意：
                                                          1.此寄存器只能在系统初始化时静态配置；
                                                          2.此寄存器实际可配置的位宽为ASI的个数，从GLB_RTL_INF0.rtl_asi_num可读出。 */
        unsigned int  reserved       : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RBUF_TRANS_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_RBUF_TRANS_CTRL_rbuf_trans_req_START  (0)
#define SOC_DMSS_GLB_RBUF_TRANS_CTRL_rbuf_trans_req_END    (15)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RBUF_BP_UNION
 结构说明  : GLB_RBUF_BP 寄存器结构定义。地址偏移量:0x16150+0x4*(rbuf_bp_grps)，初值:0x00000000，宽度:32
 寄存器说明: RBUF的命令反压配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rbuf_bp_grp : 16; /* bit[0-15] : 命令反压分组的选择：
                                                       rbuf_bp_grp[n]=0：ASIn不包含到分组0；
                                                       rbuf_bp_grp[n]=1：ASIn包含到分组0。
                                                       说明：
                                                       被包含到同一个分组的ASI，当其在RBUF中的数据burst总数达到对应rbuf_bp_wl配置的水线时，送出反压信号给上游模块（DDRC），进行对应ASI的命令反压。
                                                       注意：
                                                       1.此寄存器只能在系统初始化时静态配置；
                                                       2.当命令反压分组的水线rbuf_bp_wl为0时即关闭命令反压功能时，命令反压分组无效，此域段需要配置为0。 */
        unsigned int  rbuf_bp_wl  : 16; /* bit[16-31]: 命令反压分组的水线：
                                                       0：关闭命令反压功能；
                                                       ……
                                                       n：当前分组的命令反压水线。
                                                       说明：
                                                       1.当前分组中所有ASI对应的数据burst总数达到rbuf_bp_wl时，送出反压信号给上游模块（DDRC），进行对应ASI的命令反压；
                                                       2.水线最大配置值为RBUF中的BUFFER深度。
                                                       注意：
                                                       不能动态开关命令反压功能，但可以在使能的情况下，动态调整水线。 */
    } reg;
} SOC_DMSS_GLB_RBUF_BP_UNION;
#endif
#define SOC_DMSS_GLB_RBUF_BP_rbuf_bp_grp_START  (0)
#define SOC_DMSS_GLB_RBUF_BP_rbuf_bp_grp_END    (15)
#define SOC_DMSS_GLB_RBUF_BP_rbuf_bp_wl_START   (16)
#define SOC_DMSS_GLB_RBUF_BP_rbuf_bp_wl_END     (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RSP_ERR_INT_EN_UNION
 结构说明  : GLB_RSP_ERR_INT_EN 寄存器结构定义。地址偏移量:0x16180，初值:0x00000001，宽度:32
 寄存器说明: AMI0返回响应ERROR中断使能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_en   : 1;  /* bit[0]   : AMI返回响应ERROR的中断输出使能。
                                                   0：禁止；
                                                   1：使能。
                                                   说明：
                                                   1.当有AMI返回响应ERROR时，可上报中断并记录命令读写类型信息（请参考GLB_RSP_ERR_INT_STATUS寄存器）；
                                                   2.只能禁止中断输出，但相关寄存器还是会正常记录中断状态及命令信息。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RSP_ERR_INT_EN_UNION;
#endif
#define SOC_DMSS_GLB_RSP_ERR_INT_EN_int_en_START    (0)
#define SOC_DMSS_GLB_RSP_ERR_INT_EN_int_en_END      (0)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_RSP_ERR_INT_STATUS_UNION
 结构说明  : GLB_RSP_ERR_INT_STATUS 寄存器结构定义。地址偏移量:0x16184，初值:0x00000000，宽度:32
 寄存器说明: AMI0返回响应ERROR中断状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_status   : 1;  /* bit[0]   : 中断状态指示：
                                                       0：中断未产生；
                                                       1：中断已产生。 */
        unsigned int  reserved_0   : 3;  /* bit[1-3] : 保留。 */
        unsigned int  rsp_err_type : 1;  /* bit[4]   : 中断状态清除前首个AMI返回响应ERROR的读写类型信息：
                                                       0：read访问；
                                                       1：write访问。
                                                       说明：
                                                       如果同一时刻出现读写响应ERROR，记录读访问。 */
        unsigned int  reserved_1   : 27; /* bit[5-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_RSP_ERR_INT_STATUS_UNION;
#endif
#define SOC_DMSS_GLB_RSP_ERR_INT_STATUS_int_status_START    (0)
#define SOC_DMSS_GLB_RSP_ERR_INT_STATUS_int_status_END      (0)
#define SOC_DMSS_GLB_RSP_ERR_INT_STATUS_rsp_err_type_START  (4)
#define SOC_DMSS_GLB_RSP_ERR_INT_STATUS_rsp_err_type_END    (4)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_AMI_CTRL_UNION
 结构说明  : GLB_AMI_CTRL 寄存器结构定义。地址偏移量:0x161A0，初值:0x00000000，宽度:32
 寄存器说明: AMI的控制配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  wd_fifo_wl : 8;  /* bit[0-7] : AMI写数据fifo缓存的水线：
                                                     0：关闭AMI写数据fifo缓存的水线功能；
                                                     ……
                                                     n：AMI写数据fifo缓存的水线。
                                                     说明：
                                                     1.当前写数据fifo所有数据总数达到wd_fifo_wl时，送出反压信号给上游模块，不再接受新的写数据；
                                                     2.水线最大配置值为AMI写数据fifo深度。
                                                     注意：
                                                     此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved   : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_AMI_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_AMI_CTRL_wd_fifo_wl_START  (0)
#define SOC_DMSS_GLB_AMI_CTRL_wd_fifo_wl_END    (7)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DCQ_CTRL_UNION
 结构说明  : GLB_DCQ_CTRL 寄存器结构定义。地址偏移量:0x16200，初值:0x00000001，宽度:32
 寄存器说明: DDRC的命令队列状态统计控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcq_en   : 1;  /* bit[0]   : 统计使能：
                                                   0：禁止；
                                                   1：使能。
                                                   注意：
                                                   1.若配置为“禁止”，则所有基于DDRC命令队列状态统计的功能同时被禁止，即寄存器ASI_QOS_LMTR2/ASI_QOS_LMTR3/ASI_QOS_DCQx/GLB_DCQ_x等均无效；
                                                   2.此寄存器只能静态配置。 */
        unsigned int  dcq_mode : 1;  /* bit[1]   : 统计模式选择：
                                                   0：统计QOSBUF对应通道和对应DMC的命令；
                                                   1：统计QOSBUF所有通道和对应DMC的命令。
                                                   注意：
                                                   1.此寄存器只能静态配置；
                                                   2.当QOSBUF被静态Bypass时，只能配置为0。 */
        unsigned int  reserved : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_DCQ_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_DCQ_CTRL_dcq_en_START    (0)
#define SOC_DMSS_GLB_DCQ_CTRL_dcq_en_END      (0)
#define SOC_DMSS_GLB_DCQ_CTRL_dcq_mode_START  (1)
#define SOC_DMSS_GLB_DCQ_CTRL_dcq_mode_END    (1)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DCQ_PRILVL0_UNION
 结构说明  : GLB_DCQ_PRILVL0 寄存器结构定义。地址偏移量:0x16220+0x10*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 基于DDRC读写总命令队列状态的允许优先级水线0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved     : 8;  /* bit[0-7]  : 保留。 */
        unsigned int  dcq_pri1_lvl : 8;  /* bit[8-15] : 优先级1的DDRC命令队列状态允许水线：
                                                        0x00：此水线关闭；
                                                        0x01～0xFF：DDRC命令队列中读写总命令数达到此水线时，对低于优先级1的读或写命令进行流控。
                                                        注意：
                                                        1.对于读命令，只受dcq_pri_lvl的控制；
                                                        2.对于写命令，同时受dcq_pri_lvl和dcq_wr_pri_lvl的控制。 */
        unsigned int  dcq_pri2_lvl : 8;  /* bit[16-23]: 优先级2的DDRC命令队列状态允许水线：
                                                        配置方式同dcq_pri1_lvl。 */
        unsigned int  dcq_pri3_lvl : 8;  /* bit[24-31]: 优先级3的DDRC命令队列状态允许水线：
                                                        配置方式同dcq_pri1_lvl。 */
    } reg;
} SOC_DMSS_GLB_DCQ_PRILVL0_UNION;
#endif
#define SOC_DMSS_GLB_DCQ_PRILVL0_dcq_pri1_lvl_START  (8)
#define SOC_DMSS_GLB_DCQ_PRILVL0_dcq_pri1_lvl_END    (15)
#define SOC_DMSS_GLB_DCQ_PRILVL0_dcq_pri2_lvl_START  (16)
#define SOC_DMSS_GLB_DCQ_PRILVL0_dcq_pri2_lvl_END    (23)
#define SOC_DMSS_GLB_DCQ_PRILVL0_dcq_pri3_lvl_START  (24)
#define SOC_DMSS_GLB_DCQ_PRILVL0_dcq_pri3_lvl_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DCQ_PRILVL1_UNION
 结构说明  : GLB_DCQ_PRILVL1 寄存器结构定义。地址偏移量:0x16224+0x10*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 基于DDRC读写总命令队列状态的允许优先级水线1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcq_pri4_lvl : 8;  /* bit[0-7]  : 优先级4的DDRC命令队列状态允许水线：
                                                        配置方式同dcq_pri1_lvl。 */
        unsigned int  dcq_pri5_lvl : 8;  /* bit[8-15] : 优先级5的DDRC命令队列状态允许水线：
                                                        配置方式同dcq_pri1_lvl。 */
        unsigned int  dcq_pri6_lvl : 8;  /* bit[16-23]: 优先级6的DDRC命令队列状态允许水线：
                                                        配置方式同dcq_pri1_lvl。 */
        unsigned int  dcq_pri7_lvl : 8;  /* bit[24-31]: 优先级7的DDRC命令队列状态允许水线：
                                                        配置方式同dcq_pri1_lvl。 */
    } reg;
} SOC_DMSS_GLB_DCQ_PRILVL1_UNION;
#endif
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri4_lvl_START  (0)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri4_lvl_END    (7)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri5_lvl_START  (8)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri5_lvl_END    (15)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri6_lvl_START  (16)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri6_lvl_END    (23)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri7_lvl_START  (24)
#define SOC_DMSS_GLB_DCQ_PRILVL1_dcq_pri7_lvl_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DCQ_WR_PRILVL0_UNION
 结构说明  : GLB_DCQ_WR_PRILVL0 寄存器结构定义。地址偏移量:0x16228+0x10*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 基于DDRC写命令队列状态的允许优先级水线0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  reserved        : 8;  /* bit[0-7]  : 保留。 */
        unsigned int  dcq_wr_pri1_lvl : 8;  /* bit[8-15] : 优先级1的DDRC写命令队列状态允许水线：
                                                           0x00：此水线关闭；
                                                           0x01～0xFF：DDRC命令队列中写命令数达到此水线时，对低于优先级1的写命令进行流控。
                                                           注意：
                                                           1.对于读命令，只受dcq_pri_lvl的控制；
                                                           2.对于写命令，同时受dcq_pri_lvl和dcq_wr_pri_lvl的控制。 */
        unsigned int  dcq_wr_pri2_lvl : 8;  /* bit[16-23]: 优先级2的DDRC写命令队列状态允许水线：
                                                           配置方式同dcq_wr_pri1_lvl。 */
        unsigned int  dcq_wr_pri3_lvl : 8;  /* bit[24-31]: 优先级3的DDRC写命令队列状态允许水线：
                                                           配置方式同dcq_wr_pri1_lvl。 */
    } reg;
} SOC_DMSS_GLB_DCQ_WR_PRILVL0_UNION;
#endif
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_dcq_wr_pri1_lvl_START  (8)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_dcq_wr_pri1_lvl_END    (15)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_dcq_wr_pri2_lvl_START  (16)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_dcq_wr_pri2_lvl_END    (23)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_dcq_wr_pri3_lvl_START  (24)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL0_dcq_wr_pri3_lvl_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DCQ_WR_PRILVL1_UNION
 结构说明  : GLB_DCQ_WR_PRILVL1 寄存器结构定义。地址偏移量:0x1622C+0x10*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 基于DDRC写命令队列状态的允许优先级水线1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcq_wr_pri4_lvl : 8;  /* bit[0-7]  : 优先级4的DDRC写命令队列状态允许水线：
                                                           配置方式同dcq_wr_pri1_lvl。 */
        unsigned int  dcq_wr_pri5_lvl : 8;  /* bit[8-15] : 优先级5的DDRC写命令队列状态允许水线：
                                                           配置方式同dcq_wr_pri1_lvl。 */
        unsigned int  dcq_wr_pri6_lvl : 8;  /* bit[16-23]: 优先级6的DDRC写命令队列状态允许水线：
                                                           配置方式同dcq_wr_pri1_lvl。 */
        unsigned int  dcq_wr_pri7_lvl : 8;  /* bit[24-31]: 优先级7的DDRC写命令队列状态允许水线：
                                                           配置方式同dcq_wr_pri1_lvl。 */
    } reg;
} SOC_DMSS_GLB_DCQ_WR_PRILVL1_UNION;
#endif
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri4_lvl_START  (0)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri4_lvl_END    (7)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri5_lvl_START  (8)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri5_lvl_END    (15)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri6_lvl_START  (16)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri6_lvl_END    (23)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri7_lvl_START  (24)
#define SOC_DMSS_GLB_DCQ_WR_PRILVL1_dcq_wr_pri7_lvl_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_SC_CTRL_UNION
 结构说明  : GLB_SC_CTRL 寄存器结构定义。地址偏移量:0x16260，初值:0x00000000，宽度:32
 寄存器说明: SystemCache控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_en            : 1;  /* bit[0]   : SystemCache使能：
                                                           0：禁止，DMSS访问只能被路由到DMI/AMI；
                                                           1：使能，按相关路由规则，DMSS可以访问SystemCache。
                                                           注意：
                                                           只能初始化静态配置开关。 */
        unsigned int  sc_prot_alloc_en : 1;  /* bit[1]   : Protected加密命令是否访问System cache：
                                                           0x0：Protected加密命令Bypass SystemCache，直接访问DDR；
                                                           0x1：Protected加密命令可以访问SystemCache。
                                                           注意：
                                                           需要联合其他路由规格来判断Protected加密命令最终是否访问SystemCache。 */
        unsigned int  sc_gid_byp       : 2;  /* bit[2-3] : GID0命令的bypass policy：
                                                           0x0：GID0命令按照GID非0路由规格决定是否Bypass SystemCache；
                                                           0x1：GID0 Cacheable命令按照GID非0路由规格决定是否Bypass SystemCache，GID0 非Cacheable命令Bypass SystemCache，直接访问DDR；
                                                           0x2：GID0命令Bypass SystemCache，直接访问DDR；
                                                           0x3：GID0命令Bypass SystemCache，直接访问DDR。
                                                           注意：
                                                           Cacheable命令即Axcache为4'b1111、4’b0111、4'b1011的命令，其他为非Cacheable命令。 */
        unsigned int  reserved         : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_SC_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_SC_CTRL_sc_en_START             (0)
#define SOC_DMSS_GLB_SC_CTRL_sc_en_END               (0)
#define SOC_DMSS_GLB_SC_CTRL_sc_prot_alloc_en_START  (1)
#define SOC_DMSS_GLB_SC_CTRL_sc_prot_alloc_en_END    (1)
#define SOC_DMSS_GLB_SC_CTRL_sc_gid_byp_START        (2)
#define SOC_DMSS_GLB_SC_CTRL_sc_gid_byp_END          (3)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_SC_DCQLVL_UNION
 结构说明  : GLB_SC_DCQLVL 寄存器结构定义。地址偏移量:0x16264，初值:0x00000000，宽度:32
 寄存器说明: 基于DCQ的SystemCache流控信息
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sc_dcq_lvl0 : 8;  /* bit[0-7]  : SystemCache的DCQ预警水线0：
                                                       0：关闭SystemCache的DCQ预警0功能；
                                                       n：当DdrcCmdQue中的命令超过此水线时，DMSS输出告警信号给SystemCache，以便其对下发DDRC的命令进行相关的流量控制。 */
        unsigned int  sc_dcq_lvl1 : 8;  /* bit[8-15] : SystemCache的DCQ预警水线1：
                                                       0：关闭SystemCache的DCQ预警1功能；
                                                       n：当DdrcCmdQue中的命令超过此水线时，DMSS输出告警信号给SystemCache，以便其对下发DDRC的命令进行相关的流量控制。 */
        unsigned int  reserved    : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_SC_DCQLVL_UNION;
#endif
#define SOC_DMSS_GLB_SC_DCQLVL_sc_dcq_lvl0_START  (0)
#define SOC_DMSS_GLB_SC_DCQLVL_sc_dcq_lvl0_END    (7)
#define SOC_DMSS_GLB_SC_DCQLVL_sc_dcq_lvl1_START  (8)
#define SOC_DMSS_GLB_SC_DCQLVL_sc_dcq_lvl1_END    (15)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_MST_FLUX_UNION
 结构说明  : GLB_MST_FLUX 寄存器结构定义。地址偏移量:0x16280+0x4*(dcq_msts)，初值:0x00770000，宽度:32
 寄存器说明: 外部master的流控功能
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mst_flux_req_rd_pri  : 3;  /* bit[0-2]  : master请求流控的读允许优先级：
                                                                0：此功能关闭；
                                                                1～7：当外部master送入的mst_flux_req信号使能且通过mst_flux_req_sel选中某些通道时，所有ASI的发往这些通道的低于此允许优先级的读命令，都将因limter2的带宽限制而可能被阻塞。 */
        unsigned int  reserved_0           : 1;  /* bit[3]    : 保留。 */
        unsigned int  mst_flux_req_wr_pri  : 3;  /* bit[4-6]  : master请求流控的写允许优先级：
                                                                0：此功能关闭；
                                                                1～7：当外部master送入的mst_flux_req信号使能且通过mst_flux_req_sel选中某些通道时，所有ASI的发往这些通道的低于此允许优先级的写命令，都将因limter2的带宽限制而可能被阻塞。 */
        unsigned int  reserved_1           : 1;  /* bit[7]    : 保留。 */
        unsigned int  mst_flux_req_sel     : 4;  /* bit[8-11] : master请求流控的目标通道选择：
                                                                mst_flux_req_sel[n]=0：不选择通道n；
                                                                mst_flux_req_sel[n]=1：选择通道n。
                                                                说明：
                                                                选择当前master可能访问的目的通道：当mst_flux_req信号使能时，则所有ASI的发往这些选中通道的低优先级命令，可能被限流。 */
        unsigned int  mst_flux_req_en      : 1;  /* bit[12]   : master请求流控功能的使能：
                                                                0：禁止；
                                                                1：使能。 */
        unsigned int  reserved_2           : 3;  /* bit[13-15]: 保留。 */
        unsigned int  mst_flux_ctrl_rd_pri : 3;  /* bit[16-18]: master源端流控的读触发优先级：
                                                                0：reserved；
                                                                1～7：若mst_flux_ctrl_sel选中的通道当前读允许优先级高于等于此处设定的优先级时，将拉高送给外部master的mst_flux_ctrl_r信号，通知其进行源端流控，以避免再发送低优先级读命令而被对应的ASI阻塞。 */
        unsigned int  reserved_3           : 1;  /* bit[19]   : 保留。 */
        unsigned int  mst_flux_ctrl_wr_pri : 3;  /* bit[20-22]: master源端流控的写触发优先级：
                                                                0：reserved；
                                                                1～7：若mst_flux_ctrl_sel选中的通道当前写允许优先级高于等于此处设定的优先级时，将拉高送给外部master的mst_flux_ctrl_w信号，通知其进行源端流控，以避免再发送低优先级写命令而被对应的ASI阻塞。 */
        unsigned int  reserved_4           : 1;  /* bit[23]   : 保留。 */
        unsigned int  mst_flux_ctrl_sel    : 4;  /* bit[24-27]: master源端流控的目标通道选择：
                                                                mst_flux_ctrl_sel[n]=0：不选择通道n；
                                                                mst_flux_ctrl_sel[n]=1：选择通道n。
                                                                说明：
                                                                选择当前master可能访问的目的通道：当选中通道允许优先级高于设定值时，则拉高mst_flux_ctrl_w/_r信号，提示master进行源端限流。 */
        unsigned int  mst_flux_ctrl_en     : 1;  /* bit[28]   : master源端流控功能的使能：
                                                                0：禁止；
                                                                1：使能。 */
        unsigned int  reserved_5           : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_MST_FLUX_UNION;
#endif
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_rd_pri_START   (0)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_rd_pri_END     (2)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_wr_pri_START   (4)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_wr_pri_END     (6)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_sel_START      (8)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_sel_END        (11)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_en_START       (12)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_req_en_END         (12)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_rd_pri_START  (16)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_rd_pri_END    (18)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_wr_pri_START  (20)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_wr_pri_END    (22)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_sel_START     (24)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_sel_END       (27)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_en_START      (28)
#define SOC_DMSS_GLB_MST_FLUX_mst_flux_ctrl_en_END        (28)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_CTRL0_UNION
 结构说明  : GLB_TRACE_CTRL0 寄存器结构定义。地址偏移量:0x16300，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能控制0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  trace_en : 1;  /* bit[0]   : TRACE功能使能。
                                                   0：TRACE功能关闭；
                                                   1：TRACE功能使能。
                                                   注意：
                                                   GLB_TRACE_LOCK_DOWN.trace_lock_down配置为1时（复位默认值即为1），trace_en强制清零，TRACE功能不可用。
                                                   说明：
                                                   1.一个DMI对应一个TRACE模块，分别记录各个DMI的命令pattern；多个TRACE模块被统一控制；
                                                   2.命令pattern被打包为TRACE命令和数据，以Burst8格式被写入到DDR内预留的TRACE空间内;
                                                   3.TRACE功能自动停止时（已记录达到指定数目的pattern/已达到设定的TRACE时间/非卷绕模式下TRACE空间已满），此配置会自动清零。 */
        unsigned int  reserved : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_TRACE_CTRL0_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_CTRL0_trace_en_START  (0)
#define SOC_DMSS_GLB_TRACE_CTRL0_trace_en_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_CTRL1_UNION
 结构说明  : GLB_TRACE_CTRL1 寄存器结构定义。地址偏移量:0x16304，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能控制1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  max_pattern_num : 28; /* bit[0-27] : 命令pattern的最大记录个数。
                                                           0：不对记录的命令pattern个数进行限制；
                                                           n：任意一个DMI中，记录了n*16个命令pattern后，自动停止TRACE功能；
                                                           注意：
                                                           当达到了TRACE时间，或在非卷绕模式下TRACE空间已被写满，则TRACE功能也会自动停止。 */
        unsigned int  unaligned_mode  : 1;  /* bit[28]   : 对非对齐写命令的对齐判断粒度。
                                                           0：识别32B地址非对齐写命令；
                                                           1：识别64B地址非对齐写命令；
                                                           说明：
                                                           识别非对齐写命令并记录到cmd_pattern中，结合命令len信息，可用于识别mask write命令。 */
        unsigned int  reserved        : 2;  /* bit[29-30]: 保留。 */
        unsigned int  roll_en         : 1;  /* bit[31]   : 卷绕记录使能：
                                                           0：非卷绕记录模式：
                                                           当TRACE空间被写满时，TRACE功能将自动停止；
                                                           1：卷绕记录模式：
                                                           当TRACE空间写满后，卷绕到起始地址继续记录TRACE数据，之前信息将被覆盖。 */
    } reg;
} SOC_DMSS_GLB_TRACE_CTRL1_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_CTRL1_max_pattern_num_START  (0)
#define SOC_DMSS_GLB_TRACE_CTRL1_max_pattern_num_END    (27)
#define SOC_DMSS_GLB_TRACE_CTRL1_unaligned_mode_START   (28)
#define SOC_DMSS_GLB_TRACE_CTRL1_unaligned_mode_END     (28)
#define SOC_DMSS_GLB_TRACE_CTRL1_roll_en_START          (31)
#define SOC_DMSS_GLB_TRACE_CTRL1_roll_en_END            (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_CTRL2_UNION
 结构说明  : GLB_TRACE_CTRL2 寄存器结构定义。地址偏移量:0x16308，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能控制2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  trace_prd : 32; /* bit[0-31]: TRACE功能的最长工作时间。
                                                    0：不对记录的TRACE时间进行限制；
                                                    n：记录了n*64个cycle后，自动停止TRACE功能；
                                                    注意：
                                                    1.当达到了命令pattern的最大记录个数，或在非卷绕模式下TRACE空间已被写满，则TRACE功能也会自动停止；
                                                    2.在933MHz下，最大设置时间约为4.9分钟，分辨率为68ns。 */
    } reg;
} SOC_DMSS_GLB_TRACE_CTRL2_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_CTRL2_trace_prd_START  (0)
#define SOC_DMSS_GLB_TRACE_CTRL2_trace_prd_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_REC0_UNION
 结构说明  : GLB_TRACE_REC0 寄存器结构定义。地址偏移量:0x16310，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能记录配置0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rec_base_addr : 16; /* bit[0-15] : TRACE空间的通道内起始地址（地址高位部分）：
                                                         说明：
                                                         1.此地址是单个通道内物理地址，多通道配置下，会在同DMI内的多通道之间，以可配置的粒度交织记录TRACE数据（交织粒度配置：GLB_TRACE_REC1.rec_intlv_gran）；
                                                         2.配置地址以MB为单位，即此处配置的是从地址[20]为起点的高位部分，低20bits地址默认全0。 */
        unsigned int  rec_top_addr  : 16; /* bit[16-31]: TRACE空间的通道内结束地址（地址高位部分）：
                                                         说明：
                                                         1.此地址是单个通道内物理地址，多通道配置下，会在同DMI内的多通道之间，以可配置的粒度交织记录TRACE数据（交织粒度配置：GLB_TRACE_REC1.rec_intlv_gran）；
                                                         2.配置地址以MB为单位，即此处配置的是从地址[20]为起点的高位部分，低20bits地址默认全1； */
    } reg;
} SOC_DMSS_GLB_TRACE_REC0_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_REC0_rec_base_addr_START  (0)
#define SOC_DMSS_GLB_TRACE_REC0_rec_base_addr_END    (15)
#define SOC_DMSS_GLB_TRACE_REC0_rec_top_addr_START   (16)
#define SOC_DMSS_GLB_TRACE_REC0_rec_top_addr_END     (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_REC1_UNION
 结构说明  : GLB_TRACE_REC1 寄存器结构定义。地址偏移量:0x16314，初值:0x00100000，宽度:32
 寄存器说明: TRACE功能记录配置1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rec_mid           : 8;  /* bit[0-7]  : TRACE命令的MID配置。 */
        unsigned int  rec_pri           : 3;  /* bit[8-10] : TRACE命令的优先级配置。 */
        unsigned int  reserved_0        : 5;  /* bit[11-15]: 保留。 */
        unsigned int  rec_ts_max_intrvl : 4;  /* bit[16-19]: TRACE数据中timestamp的最大间隔：
                                                             0：不强制插入timestamp；
                                                             n：若连续记录(2^n)*16个命令pattern都没有产生timestamp，则强制插入一个timestamp。 */
        unsigned int  rec_intlv_gran    : 3;  /* bit[20-22]: TRACE数据写入时的通道交织粒度：
                                                             0x1：128Byte；
                                                             0x2：256Byte；
                                                             0x3：512Byte；
                                                             0x4： 1KByte；
                                                             0x5： 2KByte；
                                                             0x6： 4KByte；
                                                             0x0,0x7：reserved。
                                                             注意：
                                                             1.TRACE数据以此粒度为边界，被交替分发到同DMI内的多个通道(若DMI内只有一个通道，则此配置无效)；
                                                             2.此寄存器只能在TRACE使能前静态配置。 */
        unsigned int  reserved_1        : 9;  /* bit[23-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_TRACE_REC1_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_REC1_rec_mid_START            (0)
#define SOC_DMSS_GLB_TRACE_REC1_rec_mid_END              (7)
#define SOC_DMSS_GLB_TRACE_REC1_rec_pri_START            (8)
#define SOC_DMSS_GLB_TRACE_REC1_rec_pri_END              (10)
#define SOC_DMSS_GLB_TRACE_REC1_rec_ts_max_intrvl_START  (16)
#define SOC_DMSS_GLB_TRACE_REC1_rec_ts_max_intrvl_END    (19)
#define SOC_DMSS_GLB_TRACE_REC1_rec_intlv_gran_START     (20)
#define SOC_DMSS_GLB_TRACE_REC1_rec_intlv_gran_END       (22)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_FILTER0_UNION
 结构说明  : GLB_TRACE_FILTER0 寄存器结构定义。地址偏移量:0x16318，初值:0xFFFF0000，宽度:32
 寄存器说明: TRACE功能监控过滤0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  filter_base_addr : 16; /* bit[0-15] : 被TRACE命令的下限地址过滤（地址高位部分）：
                                                            说明：
                                                            1.此地址是从DMI内捕获命令的通道内地址；
                                                            2.配置地址以MB为单位，即此处配置的是从地址[20]为起点的高位部分，低位20bits地址默认全0。
                                                            3.在地址范围内命令的pattern，可能被TRACE功能记录（需同时满足其它过滤条件）。 */
        unsigned int  filter_top_addr  : 16; /* bit[16-31]: 被TRACE命令的上限地址过滤（地址高位部分）：
                                                            说明：
                                                            1.此地址是从DMI内捕获命令的通道内地址；
                                                            2.配置地址以MB为单位，即此处配置的是从地址[20]为起点的高位部分，低位20bits地址默认全1。
                                                            3.在地址范围内命令的pattern，可能被TRACE功能记录（需同时满足其它过滤条件）。 */
    } reg;
} SOC_DMSS_GLB_TRACE_FILTER0_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_FILTER0_filter_base_addr_START  (0)
#define SOC_DMSS_GLB_TRACE_FILTER0_filter_base_addr_END    (15)
#define SOC_DMSS_GLB_TRACE_FILTER0_filter_top_addr_START   (16)
#define SOC_DMSS_GLB_TRACE_FILTER0_filter_top_addr_END     (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_FILTER1_UNION
 结构说明  : GLB_TRACE_FILTER1 寄存器结构定义。地址偏移量:0x1631C，初值:0x000FFFFF，宽度:32
 寄存器说明: TRACE功能监控过滤1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  filter_asi     : 14; /* bit[0-13] : 被TRACE命令的ASI过滤：
                                                          filter_asi[n]=0：来自ASIn命令的pattern不会被TRACE功能记录；
                                                          filter_asi[n]=1：来自ASIn命令的pattern可能被TRACE功能记录（需同时满足其它过滤条件）。 */
        unsigned int  filter_type_rd : 1;  /* bit[14]   : 被TRACE命令的读类型过滤：
                                                          0：读命令的pattern不会被TRACE功能记录；
                                                          1：读命令的pattern可能被TRACE功能记录（需同时满足其它过滤条件）。 */
        unsigned int  filter_type_wr : 1;  /* bit[15]   : 被TRACE命令的写类型过滤：
                                                          0：写命令的pattern不会被TRACE功能记录；
                                                          1：写命令的pattern可能被TRACE功能记录（需同时满足其它过滤条件）。 */
        unsigned int  filter_ch      : 4;  /* bit[16-19]: 被TRACE命令的通道过滤：
                                                          filter_ch[n]=0：发往通道n命令的pattern不会被TRACE功能记录；
                                                          filter_ch[n]=1：发往通道n命令的pattern可能被TRACE功能记录（需同时满足其它过滤条件）。
                                                          说明：
                                                          无论RTL如何配置，filter_ch[n]中n的含义固定如下：
                                                          0：DMI0中的通道0；
                                                          1：DMI0中的通道1；
                                                          2：DMI1中的通道0；
                                                          3：DMI1中的通道1。 */
        unsigned int  filter_src     : 1;  /* bit[20]   : 被TRACE命令的监控源端选择：
                                                          0：记录从DMSS直接发往DDR，以及从System cache发往DDR的命令；
                                                          1：保留。 */
        unsigned int  reserved       : 11; /* bit[21-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_TRACE_FILTER1_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_asi_START      (0)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_asi_END        (13)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_type_rd_START  (14)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_type_rd_END    (14)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_type_wr_START  (15)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_type_wr_END    (15)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_ch_START       (16)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_ch_END         (19)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_src_START      (20)
#define SOC_DMSS_GLB_TRACE_FILTER1_filter_src_END        (20)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_FILTER2_UNION
 结构说明  : GLB_TRACE_FILTER2 寄存器结构定义。地址偏移量:0x16320+0x4*(mid_grps)，初值:0xFFFFFFFF，宽度:32
 寄存器说明: TRACE功能监控过滤2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  filter_mid0 : 32; /* bit[0-31]: 被TRACE命令的MID过滤：
                                                      filter_mid0[n]=0：命令MID的为n时，此命令的pattern不会被TRACE功能记录；
                                                      filter_mid0[n]=1：命令MID的为n时，此命令的pattern可能被TRACE功能记录（需同时满足其它过滤条件）。 */
    } reg;
} SOC_DMSS_GLB_TRACE_FILTER2_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_FILTER2_filter_mid0_START  (0)
#define SOC_DMSS_GLB_TRACE_FILTER2_filter_mid0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_FREQ_UNION
 结构说明  : GLB_TRACE_FREQ 寄存器结构定义。地址偏移量:0x16340，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能时钟频率记录
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  cur_freq : 4;  /* bit[0-3] : 当前时钟频率信息记录：
                                                   说明：
                                                   1.TRACE功能使能期间，每写此寄存器一次，TRACE数据中将插入一个timestamp，记录被更新前后的cur_freq；
                                                   2.此配置的具体数值所代表的时钟频率，由软件方案约定。 */
        unsigned int  reserved : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_TRACE_FREQ_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_FREQ_cur_freq_START  (0)
#define SOC_DMSS_GLB_TRACE_FREQ_cur_freq_END    (3)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_INT_EN_UNION
 结构说明  : GLB_TRACE_INT_EN 寄存器结构定义。地址偏移量:0x16350，初值:0x00000001，宽度:32
 寄存器说明: TRACE功能中断控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  trace_int_en : 1;  /* bit[0]    : TRACE功能的中断输出使能：
                                                        0：禁止；
                                                        1：使能。
                                                        说明：
                                                        中断触发条件：
                                                        1.TRACE功能自动停止时（达到最大pattern记录个数或记录时间，或非卷绕模式下TRACE空间被写满）；
                                                        2.使能中断trigger功能，且达到中断触发条件时。
                                                        中断清除条件：
                                                        寄存器GLB_INT_CLEAR.trace_int_clear置1。
                                                        中断状态指示：
                                                        在寄存器GLB_INT_STATUS.trace_int中。 */
        unsigned int  reserved     : 15; /* bit[1-15] : 保留。 */
        unsigned int  int_trigger  : 16; /* bit[16-31]: 中断的trigger功能：
                                                        0：禁止trigger功能；
                                                        n：当记录了n*1024个TRACE命令时，主动触发中断。
                                                        说明：
                                                        1.若有两个DMI时，有两套TRACE机制，则只有当TRACE记录较少的那个达到trigger条件时，才触发中断；
                                                        2.一个TRACE命令对应128B的TRACE数据，包含16个cmd pattern或timestamp信息；
                                                        3.此功能可配合统计信息GLB_TRACE_STAT0.stat_cur_addr等，由CPU间歇性的对TRACE数据进行搬移，实现长时间的连续TRACE。 */
    } reg;
} SOC_DMSS_GLB_TRACE_INT_EN_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_INT_EN_trace_int_en_START  (0)
#define SOC_DMSS_GLB_TRACE_INT_EN_trace_int_en_END    (0)
#define SOC_DMSS_GLB_TRACE_INT_EN_int_trigger_START   (16)
#define SOC_DMSS_GLB_TRACE_INT_EN_int_trigger_END     (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_LOCK_DOWN_UNION
 结构说明  : GLB_TRACE_LOCK_DOWN 寄存器结构定义。地址偏移量:0x16358，初值:0x00000001，宽度:32
 寄存器说明: TRACE功能锁定控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  trace_lock_down : 1;  /* bit[0]   : TRACE功能锁定：
                                                          0：TRACE功能可正常使用；
                                                          1：TRACE功能被锁定（GLB_TRACE_CTRL0.trace_en被强制清零）。
                                                          说明：
                                                          1.此寄存器只能被安全APB访问（外部电路保证，非DMSS自身特性）；
                                                          2.TRACE其它配置寄存器可能可被非安全APB访问（外部电路保证，取决于APB访问权限的配置，非DMSS自身特性）。 */
        unsigned int  reserved        : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_TRACE_LOCK_DOWN_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_LOCK_DOWN_trace_lock_down_START  (0)
#define SOC_DMSS_GLB_TRACE_LOCK_DOWN_trace_lock_down_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_STAT0_UNION
 结构说明  : GLB_TRACE_STAT0 寄存器结构定义。地址偏移量:0x16360+0x10*(dmis)，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能统计信息0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_cur_addr    : 31; /* bit[0-30]: 统计TRACE空间的当前TRACE命令写入地址（不包含低7bits地址）：
                                                           说明：
                                                           1.此地址是通道内地址；
                                                           2.TRACE命令是128B对齐的，故此处不包含低7bits的全零地址。 */
        unsigned int  stat_cur_addr_ch : 1;  /* bit[31]  : 统计TRACE空间的当前TRACE命令写入的DMI内的通道：
                                                           0：DMI内的通道0；
                                                           1：DMI内的通达1。 */
    } reg;
} SOC_DMSS_GLB_TRACE_STAT0_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_STAT0_stat_cur_addr_START     (0)
#define SOC_DMSS_GLB_TRACE_STAT0_stat_cur_addr_END       (30)
#define SOC_DMSS_GLB_TRACE_STAT0_stat_cur_addr_ch_START  (31)
#define SOC_DMSS_GLB_TRACE_STAT0_stat_cur_addr_ch_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_STAT1_UNION
 结构说明  : GLB_TRACE_STAT1 寄存器结构定义。地址偏移量:0x16364+0x10*(dmis)，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能统计信息1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_pattern_cnt : 32; /* bit[0-31]: 统计被TRACE的命令pattern个数：
                                                           说明：
                                                           1.此处记录的是TRACE功能使能后，所有被TRACE的命令pattern个数。若发生记录卷绕，则此统计值会大于TRACE空间中实际记录的命令pattern个数；
                                                           2.重新使能TRACE功能时自动清零，溢出后保持最大值。 */
    } reg;
} SOC_DMSS_GLB_TRACE_STAT1_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_STAT1_stat_pattern_cnt_START  (0)
#define SOC_DMSS_GLB_TRACE_STAT1_stat_pattern_cnt_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_TRACE_STAT2_UNION
 结构说明  : GLB_TRACE_STAT2 寄存器结构定义。地址偏移量:0x16368+0x10*(dmis)，初值:0x00000000，宽度:32
 寄存器说明: TRACE功能统计信息2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_roll_cnt : 12; /* bit[0-11] : 统计TRACE空间的卷绕记录次数：
                                                         说明：
                                                         1.绕记录模式下(GLB_TRACE_CTRL1.roll_en=1)，发生TRACE空间记录卷绕的次数；
                                                         2.重新使能TRACE功能时自动清零，溢出后保持最大值。 */
        unsigned int  reserved      : 20; /* bit[12-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_TRACE_STAT2_UNION;
#endif
#define SOC_DMSS_GLB_TRACE_STAT2_stat_roll_cnt_START  (0)
#define SOC_DMSS_GLB_TRACE_STAT2_stat_roll_cnt_END    (11)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_MPU_CFG_UNION
 结构说明  : GLB_MPU_CFG 寄存器结构定义。地址偏移量:0x16380，初值:0x00010001，宽度:32
 寄存器说明: MPU的全局配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_bypass    : 1;  /* bit[0]    : MPU bypass配置：
                                                         0：MPU使能，命令需要进行MPU鉴权检查；
                                                         1：MPU bypass，命令不需要进行MPU鉴权检查。
                                                         说明：
                                                         1.若RTL不支持MPU，则此寄存器无效。
                                                         2.此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved_0    : 3;  /* bit[1-3]  : 保留。 */
        unsigned int  mpu_addr_zone : 2;  /* bit[4-5]  : MPU中DDR地址空间区域选择：
                                                         0x0：选择地址转移后的0~4G地址空间，MPU中DDR地址空间属性配置寄存器最小粒度为64KB；
                                                         0x1：选择地址转移后的0~8G地址空间，MPU中DDR地址空间属性配置寄存器最小粒度为128KB；
                                                         0x2：选择地址转移后的0~16G地址空间，MPU中DDR地址空间属性配置寄存器最小粒度为256KB；
                                                         0x3：reserved。
                                                         说明：
                                                         1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                         2.此寄存器只能在系统初始化时静态配置。
                                                         3.若访问命令地址不处于MPU中DDR地址空间区域，则统一看做访问non-trusted地址空间属性。
                                                         4.此处地址是指将总线访问转移后的地址空间，地址转移规则可从寄存器ASI_ADDR_SHIFT查询得到。 */
        unsigned int  reserved_1    : 2;  /* bit[6-7]  : 保留。 */
        unsigned int  mpu_apb_sel   : 2;  /* bit[8-9]  : MPU中DDR地址空间属性配置寄存器的读访问选择：
                                                         0x0：从MPU0处即DMI0 MPU读取DDR地址空间属性配置寄存器；
                                                         0x1：从MPU1处即DMI1 MPU读取DDR地址空间属性配置寄存器；
                                                         0x2：从MPU2处即AMI0 MPU读取DDR地址空间属性配置寄存器；
                                                         0x3：reserved。
                                                         说明：
                                                         1.若RTL不支持MPU，则此寄存器无效。
                                                         2.MPU中DDR地址空间属性配置寄存器的写访问不受此寄存器影响，对MPU0、MPU1和MPU2都有效。
                                                         3.此寄存器只在RTL两个DMI都配置时有效。 */
        unsigned int  reserved_2    : 2;  /* bit[10-11]: 保留。 */
        unsigned int  mpu_apb_be    : 1;  /* bit[12]   : MPU中DDR地址空间属性配置寄存器的写数据bit使能开关：
                                                         0：禁止写数据bit使能，MPU_ADDR_ATTR寄存器写入完整的32bit数据；
                                                         1：写数据bit使能，MPU_ADDR_ATTR寄存器写入低16bit数据，另外高16bit作为对应的写有效信号，有效bit为高时写入对应数据bit，有效bit为低时不写入对应数据bit。
                                                         说明：
                                                         1.若RTL不支持MPU，则此寄存器无效。
                                                         2.写数据bit使能时,那么MPU_ADDR_ATTR每一个32bit寄存器地址空间实质只有16bit寄存器数据。 */
        unsigned int  reserved_3    : 3;  /* bit[13-15]: 保留。 */
        unsigned int  mpu_fix_allow : 1;  /* bit[16]   : MPU鉴权固定通过配置：
                                                         0：MPU正常鉴权检查；
                                                         1：MPU鉴权检查必定通过，不会发生MPU命令拦截。
                                                         说明：
                                                         1.若RTL不支持MPU，则此寄存器无效。
                                                         2.此寄存器只能在LB_MPU_CFG.mpu_bypass为0时生效。 */
        unsigned int  reserved_4    : 15; /* bit[17-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_MPU_CFG_UNION;
#endif
#define SOC_DMSS_GLB_MPU_CFG_mpu_bypass_START     (0)
#define SOC_DMSS_GLB_MPU_CFG_mpu_bypass_END       (0)
#define SOC_DMSS_GLB_MPU_CFG_mpu_addr_zone_START  (4)
#define SOC_DMSS_GLB_MPU_CFG_mpu_addr_zone_END    (5)
#define SOC_DMSS_GLB_MPU_CFG_mpu_apb_sel_START    (8)
#define SOC_DMSS_GLB_MPU_CFG_mpu_apb_sel_END      (9)
#define SOC_DMSS_GLB_MPU_CFG_mpu_apb_be_START     (12)
#define SOC_DMSS_GLB_MPU_CFG_mpu_apb_be_END       (12)
#define SOC_DMSS_GLB_MPU_CFG_mpu_fix_allow_START  (16)
#define SOC_DMSS_GLB_MPU_CFG_mpu_fix_allow_END    (16)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_MPU_ERROR_ADDR_UNION
 结构说明  : GLB_MPU_ERROR_ADDR 寄存器结构定义。地址偏移量:0x16384，初值:0xFFFFFFFF，宽度:32
 寄存器说明: MPU拦截后特征地址配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_err_addr : 32; /* bit[0-31]: MPU拦截访问后，被拦截的读命令仍发向DDRC，但地址被改写，此为被改写的特征地址
                                                       说明：
                                                       1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效；
                                                       2.写命令被拦截，直接返回写错误响应，不再发向DDRC。 */
    } reg;
} SOC_DMSS_GLB_MPU_ERROR_ADDR_UNION;
#endif
#define SOC_DMSS_GLB_MPU_ERROR_ADDR_mpu_err_addr_START  (0)
#define SOC_DMSS_GLB_MPU_ERROR_ADDR_mpu_err_addr_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_MPU_ACPU_CFG_UNION
 结构说明  : GLB_MPU_ACPU_CFG 寄存器结构定义。地址偏移量:0x16388，初值:0x00000000，宽度:32
 寄存器说明: MPU的ACPU Stream配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  pmm_el2           : 1;  /* bit[0]   : MPU PMM是否工作于EL2：
                                                            0：PMM工作于S-EL0/S-EL1；
                                                            1：PMM工作于EL2。
                                                            说明：
                                                            1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                            2.此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved_0        : 3;  /* bit[1-3] : 保留。 */
        unsigned int  mpu_acpu_wr_ntrst : 1;  /* bit[4]   : MPU acpu secure写命令访问non-trusted地址空间权限：
                                                            0： acpu secure写命令访问non-trusted地址空间会被MPU拦截；
                                                            1： acpu secure写命令访问non-trusted地址空间不会被MPU拦截。
                                                            说明：
                                                            1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效。
                                                            2.此寄存器只能在系统初始化时静态配置。 */
        unsigned int  reserved_1        : 27; /* bit[5-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_MPU_ACPU_CFG_UNION;
#endif
#define SOC_DMSS_GLB_MPU_ACPU_CFG_pmm_el2_START            (0)
#define SOC_DMSS_GLB_MPU_ACPU_CFG_pmm_el2_END              (0)
#define SOC_DMSS_GLB_MPU_ACPU_CFG_mpu_acpu_wr_ntrst_START  (4)
#define SOC_DMSS_GLB_MPU_ACPU_CFG_mpu_acpu_wr_ntrst_END    (4)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_SEC_BYPASS_UNION
 结构说明  : GLB_SEC_BYPASS 寄存器结构定义。地址偏移量:0x1638C，初值:0x00000000，宽度:32
 寄存器说明: 安全整体功能透传
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  sec_rgn_byp : 1;  /* bit[0]   : ASI/DMI/AMI安全Region的Bypass开关。
                                                      0：安全Region功能使能；
                                                      1：安全Region功能关闭。
                                                      说明：
                                                      1.控制非enhance trusted外所有安全Region，包括ASI的安全Region，DMI/AMI的安全Region，但不包括enhance trusted Region；
                                                      2.此功能仅供测试模式使用。 */
        unsigned int  reserved    : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_SEC_BYPASS_UNION;
#endif
#define SOC_DMSS_GLB_SEC_BYPASS_sec_rgn_byp_START  (0)
#define SOC_DMSS_GLB_SEC_BYPASS_sec_rgn_byp_END    (0)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_CTRL_UNION
 结构说明  : GLB_STAT_CTRL 寄存器结构定义。地址偏移量:0x16400，初值:0x00000000，宽度:32
 寄存器说明: 统计功能控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_stat_en       : 16; /* bit[0-15] : ASI统计功能使能。
                                                             asi_stat_en[n]=0：ASIn的统计功能关闭；
                                                             asi_stat_en[n]=1：ASIn的统计功能使能。
                                                             说明：
                                                             1.统计相关的配置在寄存器ASI_STAT_MASK/ASI_STAT_FLUX_FILTER0/1中；
                                                             2.若关闭，则统计结果保持；若重新使能，则统计结果清零并重新统计。 */
        unsigned int  pri_allow_stat_en : 1;  /* bit[16]   : PRI_ALLOW统计功能使能。
                                                             0：关闭；
                                                             1：使能。
                                                             说明：
                                                             1.需使能DCQ相关功能，此统计功能才有意义；
                                                             2.统计相关的结果在寄存器GLB_STAT_PRI1/2/3/4/5/6/7_ALLOW中；
                                                             3.若关闭，则统计结果保持；若重新使能，则统计结果清零并重新统计。 */
        unsigned int  reserved_0        : 3;  /* bit[17-19]: 保留。 */
        unsigned int  rbuf_stat_en      : 1;  /* bit[20]   : RBUF占用率统计功能使能。
                                                             0：关闭；
                                                             1：使能。
                                                             说明：
                                                             1.统计相关控制在寄存器GLB_STAT_RBUF_CTRL中，统计相关的结果在寄存器GLB_STAT_RBUF_OCCUP0/1/2中；
                                                             2.若关闭，则统计结果保持；若重新使能，则统计结果清零并重新统计。 */
        unsigned int  reserved_1        : 3;  /* bit[21-23]: 保留。 */
        unsigned int  ami_wfifo_stat_en : 1;  /* bit[24]   : AMI写数据FIFO统计功能使能。
                                                             0：关闭；
                                                             1：使能。
                                                             说明：
                                                             1.统计相关的结果在寄存器GLB_STAT_AMI_WFIFO中；
                                                             2.若关闭，则统计结果保持；若重新使能，则统计结果清零并重新统计。 */
        unsigned int  reserved_2        : 7;  /* bit[25-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_STAT_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_STAT_CTRL_asi_stat_en_START        (0)
#define SOC_DMSS_GLB_STAT_CTRL_asi_stat_en_END          (15)
#define SOC_DMSS_GLB_STAT_CTRL_pri_allow_stat_en_START  (16)
#define SOC_DMSS_GLB_STAT_CTRL_pri_allow_stat_en_END    (16)
#define SOC_DMSS_GLB_STAT_CTRL_rbuf_stat_en_START       (20)
#define SOC_DMSS_GLB_STAT_CTRL_rbuf_stat_en_END         (20)
#define SOC_DMSS_GLB_STAT_CTRL_ami_wfifo_stat_en_START  (24)
#define SOC_DMSS_GLB_STAT_CTRL_ami_wfifo_stat_en_END    (24)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_CYCLE_UNION
 结构说明  : GLB_STAT_CYCLE 寄存器结构定义。地址偏移量:0x16410，初值:0x00000000，宽度:32
 寄存器说明: 统计周期
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_cycle : 32; /* bit[0-31]: 统计周期：
                                                     说明：
                                                     1.统计功能的统计时钟周期数；
                                                     2.统计的使能，参见寄存器GLB_STAT_CTRL.pri_allow_stat_en；
                                                     3.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数；
                                                     4.32bit计数器，400MHz下的量程为2.5ns*2^32=10.74s。 */
    } reg;
} SOC_DMSS_GLB_STAT_CYCLE_UNION;
#endif
#define SOC_DMSS_GLB_STAT_CYCLE_stat_cycle_START  (0)
#define SOC_DMSS_GLB_STAT_CYCLE_stat_cycle_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_PERFSTAT_UNION
 结构说明  : GLB_STAT_PERFSTAT 寄存器结构定义。地址偏移量:0x16420，初值:0x66666666，宽度:32
 寄存器说明: perf_stat模块配置
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ch0_wr_pri_allow_set : 3;  /* bit[0-2]  : 通道0的wr_pri_allow统计结果选择：
                                                                0：选择统计结果stat_wr_all_allow到perf_stat模块；
                                                                ……
                                                                7：选择统计结果stat_wr_pri7_allow到perf_stat模块。 */
        unsigned int  reserved_0           : 1;  /* bit[3]    : 保留。 */
        unsigned int  ch1_wr_pri_allow_set : 3;  /* bit[4-6]  : 同ch0_wr_pri_allow_set。 */
        unsigned int  reserved_1           : 1;  /* bit[7]    : 保留。 */
        unsigned int  ch2_wr_pri_allow_set : 3;  /* bit[8-10] : 同ch0_wr_pri_allow_set。 */
        unsigned int  reserved_2           : 1;  /* bit[11]   : 保留。 */
        unsigned int  ch3_wr_pri_allow_set : 3;  /* bit[12-14]: 同ch0_wr_pri_allow_set。 */
        unsigned int  reserved_3           : 1;  /* bit[15]   : 保留。 */
        unsigned int  ch0_rd_pri_allow_set : 3;  /* bit[16-18]: 通道0的rd_pri_allow统计结果选择：
                                                                0：选择统计结果stat_rd_all_allow到perf_stat模块；
                                                                ……
                                                                7：选择统计结果stat_rd_pri7_allow到perf_stat模块。 */
        unsigned int  reserved_4           : 1;  /* bit[19]   : 保留。 */
        unsigned int  ch1_rd_pri_allow_set : 3;  /* bit[20-22]: 同ch0_rd_pri_allow_set。 */
        unsigned int  reserved_5           : 1;  /* bit[23]   : 保留。 */
        unsigned int  ch2_rd_pri_allow_set : 3;  /* bit[24-26]: 同ch0_rd_pri_allow_set。 */
        unsigned int  reserved_6           : 1;  /* bit[27]   : 保留。 */
        unsigned int  ch3_rd_pri_allow_set : 3;  /* bit[28-30]: 同ch0_rd_pri_allow_set。 */
        unsigned int  reserved_7           : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_DMSS_GLB_STAT_PERFSTAT_UNION;
#endif
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch0_wr_pri_allow_set_START  (0)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch0_wr_pri_allow_set_END    (2)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch1_wr_pri_allow_set_START  (4)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch1_wr_pri_allow_set_END    (6)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch2_wr_pri_allow_set_START  (8)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch2_wr_pri_allow_set_END    (10)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch3_wr_pri_allow_set_START  (12)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch3_wr_pri_allow_set_END    (14)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch0_rd_pri_allow_set_START  (16)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch0_rd_pri_allow_set_END    (18)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch1_rd_pri_allow_set_START  (20)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch1_rd_pri_allow_set_END    (22)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch2_rd_pri_allow_set_START  (24)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch2_rd_pri_allow_set_END    (26)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch3_rd_pri_allow_set_START  (28)
#define SOC_DMSS_GLB_STAT_PERFSTAT_ch3_rd_pri_allow_set_END    (30)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_DCQ_WR_UNION
 结构说明  : GLB_STAT_DCQ_WR 寄存器结构定义。地址偏移量:0x16440+0x4*(chans)，初值:0x00000000，宽度:32
 寄存器说明: DCQ队列写命令个数的累积值统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_dcq_wr_que_acc : 32; /* bit[0-31]: 统计dcq_wr_que的累积值：
                                                              说明：
                                                              1.按cycle累加dcq_wr_que的状态，此值除统计周期，可得到DCQ队列中写命令数目的均值；
                                                              2.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_GLB_STAT_DCQ_WR_UNION;
#endif
#define SOC_DMSS_GLB_STAT_DCQ_WR_stat_dcq_wr_que_acc_START  (0)
#define SOC_DMSS_GLB_STAT_DCQ_WR_stat_dcq_wr_que_acc_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_DCQ_UNION
 结构说明  : GLB_STAT_DCQ 寄存器结构定义。地址偏移量:0x16450+0x4*(chans)，初值:0x00000000，宽度:32
 寄存器说明: DCQ队列所有命令个数的累积值统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_dcq_que_acc : 32; /* bit[0-31]: 统计dcq_que的累积值：
                                                           说明：
                                                           1.按cycle累加dcq_que的状态，此值除统计周期，可得到DCQ队列中读写总命令数目的均值；
                                                           2.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_GLB_STAT_DCQ_UNION;
#endif
#define SOC_DMSS_GLB_STAT_DCQ_stat_dcq_que_acc_START  (0)
#define SOC_DMSS_GLB_STAT_DCQ_stat_dcq_que_acc_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_UNION
 结构说明  : GLB_STAT_WR_ALL_ALLOW 寄存器结构定义。地址偏移量:0x16500+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 所有写优先级都允许的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_all_allow : 32; /* bit[0-31]: 统计当dcq_wr_pri_allow为0的生效周期：
                                                            说明：
                                                            1.累加dcq_wr_pri_allow为0的周期数，单位为cycle；
                                                            2.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_stat_wr_all_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_ALL_ALLOW_stat_wr_all_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI1_ALLOW 寄存器结构定义。地址偏移量:0x16504+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为1的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri1_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为1的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_stat_wr_pri1_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI1_ALLOW_stat_wr_pri1_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI2_ALLOW 寄存器结构定义。地址偏移量:0x16508+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为2的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri2_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为2的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_stat_wr_pri2_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI2_ALLOW_stat_wr_pri2_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI3_ALLOW 寄存器结构定义。地址偏移量:0x1650C+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为3的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri3_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为3的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_stat_wr_pri3_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI3_ALLOW_stat_wr_pri3_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI4_ALLOW 寄存器结构定义。地址偏移量:0x16510+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为4的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri4_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为4的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_stat_wr_pri4_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI4_ALLOW_stat_wr_pri4_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI5_ALLOW 寄存器结构定义。地址偏移量:0x16514+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为5的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri5_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为5的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_stat_wr_pri5_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI5_ALLOW_stat_wr_pri5_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI6_ALLOW 寄存器结构定义。地址偏移量:0x16518+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为6的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri6_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为6的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_stat_wr_pri6_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI6_ALLOW_stat_wr_pri6_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_UNION
 结构说明  : GLB_STAT_WR_PRI7_ALLOW 寄存器结构定义。地址偏移量:0x1651C+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 写允许优先级为7的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_wr_pri7_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_wr_pri_allow为7的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_stat_wr_pri7_allow_START  (0)
#define SOC_DMSS_GLB_STAT_WR_PRI7_ALLOW_stat_wr_pri7_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_UNION
 结构说明  : GLB_STAT_RD_ALL_ALLOW 寄存器结构定义。地址偏移量:0x16520+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 所有读优先级都允许的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_all_allow : 32; /* bit[0-31]: 统计当dcq_rd_pri_allow为0的生效周期：
                                                            说明：
                                                            1.累加dcq_rd_pri_allow为0的周期数，单位为cycle；
                                                            2.统计停止后结果保持，统计再次开始时自动清零，溢出后卷绕计数。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_stat_rd_all_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_ALL_ALLOW_stat_rd_all_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI1_ALLOW 寄存器结构定义。地址偏移量:0x16524+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为1的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri1_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为1的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_stat_rd_pri1_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI1_ALLOW_stat_rd_pri1_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI2_ALLOW 寄存器结构定义。地址偏移量:0x16528+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为2的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri2_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为2的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_stat_rd_pri2_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI2_ALLOW_stat_rd_pri2_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI3_ALLOW 寄存器结构定义。地址偏移量:0x1652C+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为3的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri3_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为3的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_stat_rd_pri3_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI3_ALLOW_stat_rd_pri3_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI4_ALLOW 寄存器结构定义。地址偏移量:0x16530+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为4的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri4_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为4的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_stat_rd_pri4_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI4_ALLOW_stat_rd_pri4_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI5_ALLOW 寄存器结构定义。地址偏移量:0x16534+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为5的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri5_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为5的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_stat_rd_pri5_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI5_ALLOW_stat_rd_pri5_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI6_ALLOW 寄存器结构定义。地址偏移量:0x16538+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为6的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri6_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为6的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_stat_rd_pri6_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI6_ALLOW_stat_rd_pri6_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_UNION
 结构说明  : GLB_STAT_RD_PRI7_ALLOW 寄存器结构定义。地址偏移量:0x1653C+0x40*(chans)，初值:0x00000000，宽度:32
 寄存器说明: 读允许优先级为7的统计
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rd_pri7_allow : 32; /* bit[0-31]: 统计当优先级水线dcq_rd_pri_allow为7的生效周期。 */
    } reg;
} SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_stat_rd_pri7_allow_START  (0)
#define SOC_DMSS_GLB_STAT_RD_PRI7_ALLOW_stat_rd_pri7_allow_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RBUF_CTRL_UNION
 结构说明  : GLB_STAT_RBUF_CTRL 寄存器结构定义。地址偏移量:0x16600，初值:0x00000010，宽度:32
 寄存器说明: RBUF占用率统计控制
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_buf_sel   : 4;  /* bit[0-3]  : RBUF中要统计占用率的SRC（通道）对应buf选择：
                                                          只统计选定通道n的SRC中buf占用率。 */
        unsigned int  stat_int_en    : 1;  /* bit[4]    : RBUB统计的中断输出使能：
                                                          0：禁止；
                                                          1：使能。
                                                          说明：
                                                          中断触发条件：
                                                          当GLB_STAT_RBUF_OCCUP0/1/2其中一个达到饱和计数时。
                                                          中断清除条件：
                                                          寄存器GLB_INT_CLEAR1.rbuf_stat_int_clear置1。
                                                          中断状态指示：
                                                          在寄存器GLB_INT_STATUS1.rbuf_stat_int中。 */
        unsigned int  reserved_0     : 3;  /* bit[5-7]  : 保留。 */
        unsigned int  stat_splitter0 : 8;  /* bit[8-15] : RBUF中占用率统计分割水线0：
                                                          说明：
                                                          1.选定buf中读数据burst数低于stat_splitter0但非零时，计数器stat_buf_occup0按cycle累加；
                                                          2.选定buf中读数据burst数高于等于stat_splitter0且低于stat_splitter1时，计数器stat_buf_occup1按cycle累加；
                                                          3.统计使能控制为寄存器GLB_STAT_CTRL.rbuf_stat_en。 */
        unsigned int  stat_splitter1 : 8;  /* bit[16-23]: RBUF中占用率统计分割水线1：
                                                          说明：
                                                          1.选定buf中读数据burst数高于等于stat_splitter0且低于stat_splitter1时，计数器stat_buf_occup1按cycle累加；
                                                          2.选定buf中读数据burst数高于等于stat_splitter1时，计数器stat_buf_occup2按cycle累加；
                                                          3.统计使能控制为寄存器GLB_STAT_CTRL.rbuf_stat_en。 */
        unsigned int  reserved_1     : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_STAT_RBUF_CTRL_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_buf_sel_START    (0)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_buf_sel_END      (3)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_int_en_START     (4)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_int_en_END       (4)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_splitter0_START  (8)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_splitter0_END    (15)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_splitter1_START  (16)
#define SOC_DMSS_GLB_STAT_RBUF_CTRL_stat_splitter1_END    (23)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RBUF_OCCUP0_UNION
 结构说明  : GLB_STAT_RBUF_OCCUP0 寄存器结构定义。地址偏移量:0x16604，初值:0x00000000，宽度:32
 寄存器说明: RBUF占用率统计结果0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rbuf_occup0 : 32; /* bit[0-31]: RBUF占用率统计结果0。 */
    } reg;
} SOC_DMSS_GLB_STAT_RBUF_OCCUP0_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP0_stat_rbuf_occup0_START  (0)
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP0_stat_rbuf_occup0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RBUF_OCCUP1_UNION
 结构说明  : GLB_STAT_RBUF_OCCUP1 寄存器结构定义。地址偏移量:0x16608，初值:0x00000000，宽度:32
 寄存器说明: RBUF占用率统计结果1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rbuf_occup1 : 32; /* bit[0-31]: RBUF占用率统计结果1。 */
    } reg;
} SOC_DMSS_GLB_STAT_RBUF_OCCUP1_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP1_stat_rbuf_occup1_START  (0)
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP1_stat_rbuf_occup1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_RBUF_OCCUP2_UNION
 结构说明  : GLB_STAT_RBUF_OCCUP2 寄存器结构定义。地址偏移量:0x1660C，初值:0x00000000，宽度:32
 寄存器说明: RBUF占用率统计结果2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_rbuf_occup2 : 32; /* bit[0-31]: RBUF占用率统计结果2。 */
    } reg;
} SOC_DMSS_GLB_STAT_RBUF_OCCUP2_UNION;
#endif
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP2_stat_rbuf_occup2_START  (0)
#define SOC_DMSS_GLB_STAT_RBUF_OCCUP2_stat_rbuf_occup2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_STAT_AMI_WFIFO_UNION
 结构说明  : GLB_STAT_AMI_WFIFO 寄存器结构定义。地址偏移量:0x16640，初值:0x00000000，宽度:32
 寄存器说明: AMI写数据统计结果
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  stat_ami_wfifo_busy : 20; /* bit[0-19] : 统计AMI的写数据fifo不满不空的时钟周期。
                                                               说明：
                                                               1.如果GLB_AMI_CTRL中水线功能打开，AMI的写数据fifo数据个数超过缓存水线即为满；
                                                               2.当stat_ami_wfifo_busy计数达最大值时保持；或者当stat_ami_wfifo_full计数达最大值时，此寄存器保持当前值。 */
        unsigned int  stat_ami_wfifo_full : 12; /* bit[20-31]: 统计AMI的写数据fifo为满的时钟周期。
                                                               说明：
                                                               1.如果GLB_AMI_CTRL中水线功能打开，AMI的写数据fifo数据个数超过缓存水线即为满；
                                                               2.当stat_ami_wfifo_full计数达最大值时保持；或者当stat_ami_wfifo_busy计数达最大值时，此寄存器保持当前值。 */
    } reg;
} SOC_DMSS_GLB_STAT_AMI_WFIFO_UNION;
#endif
#define SOC_DMSS_GLB_STAT_AMI_WFIFO_stat_ami_wfifo_busy_START  (0)
#define SOC_DMSS_GLB_STAT_AMI_WFIFO_stat_ami_wfifo_busy_END    (19)
#define SOC_DMSS_GLB_STAT_AMI_WFIFO_stat_ami_wfifo_full_START  (20)
#define SOC_DMSS_GLB_STAT_AMI_WFIFO_stat_ami_wfifo_full_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_MODULE_UNION
 结构说明  : GLB_DFX_MODULE 寄存器结构定义。地址偏移量:0x16700，初值:0x00000000，宽度:32
 寄存器说明: DFX：各个主要模块的工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  asi_busy      : 14; /* bit[0-13] : 各个ASI的工作状态：
                                                         asi_busy[n]=0：ASIn处于idle状态；
                                                         asi_busy[n]=1：ASIn处于busy状态。
                                                         说明：
                                                         这里的idle是指：ASI所有outstanding命令及其数据和响应，都已经彻底处理完毕。 */
        unsigned int  reserved_0    : 2;  /* bit[14-15]: 保留。 */
        unsigned int  dmi_busy      : 2;  /* bit[16-17]: 各个DMI的工作状态：
                                                         dmi_busy[n]=0：DMIn处于idle状态；
                                                         dmi_busy[n]=1：DMIn处于busy状态。 */
        unsigned int  reserved_1    : 2;  /* bit[18-19]: 保留。 */
        unsigned int  ami_busy      : 2;  /* bit[20-21]: 各个AMI的工作状态：
                                                         ami_busy[n]=0：AMIn处于idle状态；
                                                         ami_busy[n]=1：AMIn处于busy状态。 */
        unsigned int  reserved_2    : 2;  /* bit[22-23]: 保留。 */
        unsigned int  trace_busy    : 2;  /* bit[24-25]: 各个TRACE模块的工作状态：
                                                         trace_busy[n]=0：TRACEn处于idle状态；
                                                         trace_busy[n]=1：TRACEn处于busy状态。 */
        unsigned int  dmss_ddr_busy : 1;  /* bit[26]   : DMSS中访问DDR通路的工作状态：
                                                         dmss_ddr_busy=0：DMSS中访问DDR的通路处于idle状态；
                                                         dmss_ddr_busy=1：DMSS中访问DDR的通路处于busy状态。 */
        unsigned int  ddr_wake_up   : 1;  /* bit[27]   : DDR Power Down快速唤醒功能是否被触发：
                                                         ami_busy=0：DDR Power Down快速唤醒功能未被触发；
                                                         ami_busy=1：DDR Power Down快速唤醒功能已经被触发。
                                                         说明：
                                                         若RTL不支持DDR Power Down快速唤醒时，则此寄存器无效。 */
        unsigned int  dmi_cmd_bp    : 2;  /* bit[28-29]: 各个DMI下发命令的被反压状态：
                                                         dmi_cmd_bp[n]=0：DMIn没有下发命令，或下发命令被接收；
                                                         dmi_cmd_by[n]=1：DMIn有下发命令且被下游反压。 */
        unsigned int  reserved_3    : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_DFX_MODULE_UNION;
#endif
#define SOC_DMSS_GLB_DFX_MODULE_asi_busy_START       (0)
#define SOC_DMSS_GLB_DFX_MODULE_asi_busy_END         (13)
#define SOC_DMSS_GLB_DFX_MODULE_dmi_busy_START       (16)
#define SOC_DMSS_GLB_DFX_MODULE_dmi_busy_END         (17)
#define SOC_DMSS_GLB_DFX_MODULE_ami_busy_START       (20)
#define SOC_DMSS_GLB_DFX_MODULE_ami_busy_END         (21)
#define SOC_DMSS_GLB_DFX_MODULE_trace_busy_START     (24)
#define SOC_DMSS_GLB_DFX_MODULE_trace_busy_END       (25)
#define SOC_DMSS_GLB_DFX_MODULE_dmss_ddr_busy_START  (26)
#define SOC_DMSS_GLB_DFX_MODULE_dmss_ddr_busy_END    (26)
#define SOC_DMSS_GLB_DFX_MODULE_ddr_wake_up_START    (27)
#define SOC_DMSS_GLB_DFX_MODULE_ddr_wake_up_END      (27)
#define SOC_DMSS_GLB_DFX_MODULE_dmi_cmd_bp_START     (28)
#define SOC_DMSS_GLB_DFX_MODULE_dmi_cmd_bp_END       (29)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_FIFO_UNION
 结构说明  : GLB_DFX_FIFO 寄存器结构定义。地址偏移量:0x16704，初值:0x00000005，宽度:32
 寄存器说明: DFX：FIFO工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  trace0_wfifo_st : 2;  /* bit[0-1] : TRACE0内的写数据FIFO状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  trace1_wfifo_st : 2;  /* bit[2-3] : TRACE1内的写数据FIFO状态：
                                                          0x0：busy；
                                                          0x1：emtpy；
                                                          0x2：full；
                                                          0x3：reserved。 */
        unsigned int  reserved        : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_DFX_FIFO_UNION;
#endif
#define SOC_DMSS_GLB_DFX_FIFO_trace0_wfifo_st_START  (0)
#define SOC_DMSS_GLB_DFX_FIFO_trace0_wfifo_st_END    (1)
#define SOC_DMSS_GLB_DFX_FIFO_trace1_wfifo_st_START  (2)
#define SOC_DMSS_GLB_DFX_FIFO_trace1_wfifo_st_END    (3)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_RBUF_FIFO_UNION
 结构说明  : GLB_DFX_RBUF_FIFO 寄存器结构定义。地址偏移量:0x16708，初值:0x00003FFF，宽度:32
 寄存器说明: DFX：RBUF FIFO工作状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  burstfifo_empty : 14; /* bit[0-13] : RBUF中DST模块的数据FIFO状态：
                                                           burstfifo_st[n]=0：DSTn的burstfifo为非空；
                                                           burstfifo_st[n]=1：DSTn的burstfifo为空。 */
        unsigned int  reserved_0      : 2;  /* bit[14-15]: 保留。 */
        unsigned int  burstfifo_full  : 14; /* bit[16-29]: RBUF中DST模块的数据FIFO状态：
                                                           burstfifo_st[n]=0：DSTn的burstfifo为非满；
                                                           burstfifo_st[n]=1：DSTn的burstfifo为满。 */
        unsigned int  reserved_1      : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_DMSS_GLB_DFX_RBUF_FIFO_UNION;
#endif
#define SOC_DMSS_GLB_DFX_RBUF_FIFO_burstfifo_empty_START  (0)
#define SOC_DMSS_GLB_DFX_RBUF_FIFO_burstfifo_empty_END    (13)
#define SOC_DMSS_GLB_DFX_RBUF_FIFO_burstfifo_full_START   (16)
#define SOC_DMSS_GLB_DFX_RBUF_FIFO_burstfifo_full_END     (29)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_DCQ_ST_UNION
 结构说明  : GLB_DFX_DCQ_ST 寄存器结构定义。地址偏移量:0x16710+0x4*(chans)，初值:0x00000000，宽度:32
 寄存器说明: DFX：DCQ的队列实时状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dcq_wr_que_st    : 8;  /* bit[0-7]  : 当前通道命令队列中停留的写命令个数。 */
        unsigned int  dcq_que_st       : 8;  /* bit[8-15] : 当前通道命令队列中停留的读写命令总数。 */
        unsigned int  dcq_rd_pri_allow : 3;  /* bit[16-18]: DCQ的读命令允许优先级的实时状态。 */
        unsigned int  reserved_0       : 1;  /* bit[19]   : 保留。 */
        unsigned int  dcq_wr_pri_allow : 3;  /* bit[20-22]: DCQ的写命令允许优先级的实时状态。 */
        unsigned int  reserved_1       : 1;  /* bit[23]   : 保留。 */
        unsigned int  mst_rd_pri_allow : 3;  /* bit[24-26]: MST_FLUX的读命令允许优先级的实时状态。 */
        unsigned int  reserved_2       : 1;  /* bit[27]   : 保留。 */
        unsigned int  mst_wr_pri_allow : 3;  /* bit[28-30]: MST_FLUX的写命令允许优先级的实时状态。 */
        unsigned int  reserved_3       : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_DMSS_GLB_DFX_DCQ_ST_UNION;
#endif
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_wr_que_st_START     (0)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_wr_que_st_END       (7)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_que_st_START        (8)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_que_st_END          (15)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_rd_pri_allow_START  (16)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_rd_pri_allow_END    (18)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_wr_pri_allow_START  (20)
#define SOC_DMSS_GLB_DFX_DCQ_ST_dcq_wr_pri_allow_END    (22)
#define SOC_DMSS_GLB_DFX_DCQ_ST_mst_rd_pri_allow_START  (24)
#define SOC_DMSS_GLB_DFX_DCQ_ST_mst_rd_pri_allow_END    (26)
#define SOC_DMSS_GLB_DFX_DCQ_ST_mst_wr_pri_allow_START  (28)
#define SOC_DMSS_GLB_DFX_DCQ_ST_mst_wr_pri_allow_END    (30)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_RBUF0_UNION
 结构说明  : GLB_DFX_RBUF0 寄存器结构定义。地址偏移量:0x16720+0x10*(rbuf_srcs)，初值:0x00000000，宽度:32
 寄存器说明: DFX：RBUF状态0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vque_st : 32; /* bit[0-31]: RBUF的buffer中待发往ASI的数据burst状态：
                                                  vque_st[n]=0：RBUF的buffer中不存在待发往ASIn的数据burst，当前ASIn读数据burst已读空；
                                                  vque_st[n]=1：RBUF的buffer中存在待发往ASIn的数据burst，当前ASIn读数据burst未被读空（可能有多个读数据burst）。 */
    } reg;
} SOC_DMSS_GLB_DFX_RBUF0_UNION;
#endif
#define SOC_DMSS_GLB_DFX_RBUF0_vque_st_START  (0)
#define SOC_DMSS_GLB_DFX_RBUF0_vque_st_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_RBUF1_UNION
 结构说明  : GLB_DFX_RBUF1 寄存器结构定义。地址偏移量:0x16724+0x10*(rbuf_srcs)，初值:0x00000000，宽度:32
 寄存器说明: DFX：RBUF状态1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  vque_grp0_cnt_st : 8;  /* bit[0-7]  : RBUF的buffer中，命令反压分组0中的数据burst个数。 */
        unsigned int  vque_grp1_cnt_st : 8;  /* bit[8-15] : 同vque_grp0_cnt。 */
        unsigned int  vque_grp2_cnt_st : 8;  /* bit[16-23]: 同vque_grp0_cnt。 */
        unsigned int  vque_grp3_cnt_st : 8;  /* bit[24-31]: 同vque_grp0_cnt。 */
    } reg;
} SOC_DMSS_GLB_DFX_RBUF1_UNION;
#endif
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp0_cnt_st_START  (0)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp0_cnt_st_END    (7)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp1_cnt_st_START  (8)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp1_cnt_st_END    (15)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp2_cnt_st_START  (16)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp2_cnt_st_END    (23)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp3_cnt_st_START  (24)
#define SOC_DMSS_GLB_DFX_RBUF1_vque_grp3_cnt_st_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_DUMMY0_UNION
 结构说明  : GLB_DFX_DUMMY0 寄存器结构定义。地址偏移量:0x167E0，初值:0x00000000，宽度:32
 寄存器说明: DFX：M536 PTRN_MDM的状态寄存器0
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dfx_dummy0 : 32; /* bit[0-31]: M536 PTRN_MDM的状态寄存器0 */
    } reg;
} SOC_DMSS_GLB_DFX_DUMMY0_UNION;
#endif
#define SOC_DMSS_GLB_DFX_DUMMY0_dfx_dummy0_START  (0)
#define SOC_DMSS_GLB_DFX_DUMMY0_dfx_dummy0_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_DUMMY1_UNION
 结构说明  : GLB_DFX_DUMMY1 寄存器结构定义。地址偏移量:0x167E4，初值:0x00000000，宽度:32
 寄存器说明: DFX：M536 PTRN_MDM的状态寄存器1
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dfx_dummy1 : 32; /* bit[0-31]: M536 PTRN_MDM的状态寄存器1 */
    } reg;
} SOC_DMSS_GLB_DFX_DUMMY1_UNION;
#endif
#define SOC_DMSS_GLB_DFX_DUMMY1_dfx_dummy1_START  (0)
#define SOC_DMSS_GLB_DFX_DUMMY1_dfx_dummy1_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_DUMMY2_UNION
 结构说明  : GLB_DFX_DUMMY2 寄存器结构定义。地址偏移量:0x167E8，初值:0x00000000，宽度:32
 寄存器说明: DFX：M536 PTRN_MDM的状态寄存器2
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dfx_dummy2 : 32; /* bit[0-31]: M536 PTRN_MDM的状态寄存器2 */
    } reg;
} SOC_DMSS_GLB_DFX_DUMMY2_UNION;
#endif
#define SOC_DMSS_GLB_DFX_DUMMY2_dfx_dummy2_START  (0)
#define SOC_DMSS_GLB_DFX_DUMMY2_dfx_dummy2_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_DUMMY3_UNION
 结构说明  : GLB_DFX_DUMMY3 寄存器结构定义。地址偏移量:0x167EC，初值:0x00000000，宽度:32
 寄存器说明: DFX：M536 PTRN_MDM的状态寄存器3
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dfx_dummy3 : 32; /* bit[0-31]: M536 PTRN_MDM的状态寄存器3 */
    } reg;
} SOC_DMSS_GLB_DFX_DUMMY3_UNION;
#endif
#define SOC_DMSS_GLB_DFX_DUMMY3_dfx_dummy3_START  (0)
#define SOC_DMSS_GLB_DFX_DUMMY3_dfx_dummy3_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_GLB_DFX_LOAD_UNION
 结构说明  : GLB_DFX_LOAD 寄存器结构定义。地址偏移量:0x167F0，初值:0x00000000，宽度:32
 寄存器说明: DFX：仿真流量冲击状态
            （若RTL中配置DMSS_SIM_DFX_EN为0（通常ASIC版本配置为0，FPGA和EMULATOR版本定义为1），则此寄存器不存在）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  rbuf0_load_cnt_st : 8;  /* bit[0-7]  : RBUF的buffer0的仿真负载情况：
                                                             n：缓存到的数据burst达到过的最大个数。 */
        unsigned int  rbuf1_load_cnt_st : 8;  /* bit[8-15] : 同rbuf0_load_cnt_st。 */
        unsigned int  rbuf2_load_cnt_st : 8;  /* bit[16-23]: 同rbuf0_load_cnt_st。 */
        unsigned int  rbuf3_load_cnt_st : 8;  /* bit[24-31]: 同rbuf0_load_cnt_st。 */
    } reg;
} SOC_DMSS_GLB_DFX_LOAD_UNION;
#endif
#define SOC_DMSS_GLB_DFX_LOAD_rbuf0_load_cnt_st_START  (0)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf0_load_cnt_st_END    (7)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf1_load_cnt_st_START  (8)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf1_load_cnt_st_END    (15)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf2_load_cnt_st_START  (16)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf2_load_cnt_st_END    (23)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf3_load_cnt_st_START  (24)
#define SOC_DMSS_GLB_DFX_LOAD_rbuf3_load_cnt_st_END    (31)


/*****************************************************************************
 结构名    : SOC_DMSS_MPU_ADDR_ATTR_UNION
 结构说明  : MPU_ADDR_ATTR 寄存器结构定义。地址偏移量:0x20000+0x4*(mpu_regs)，初值:0x00000000，宽度:32
 寄存器说明: MPU的DDR中地址空间属性配置寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mpu_addr_attr : 32; /* bit[0-31]: MPU中DDR的64KB/128KB/256KB地址空间的地址属性：
                                                        mpu_addr_attr[n]=0：由（mpu_regs*32+n）*（64KB/128KB/256KB)地址对齐的64KB/128KB/256KB地址空间为non-trusted；
                                                        mpu_addr_attr[n]=1：由（mpu_regs*32+n）*（64KB/128KB/256KB)地址对齐的64KB/128KB/256KB地址空间为protected。
                                                        说明：
                                                        1.若RTL不支持MPU或GLB_MPU_CFG.mpu_bypass为1，则此寄存器无效；
                                                        2.若GLB_MPU_CFG.mpu_apb_be为1，那么写此寄存器高16bit为写bit有效信号，低16bit为数据，有效bit为高时写入对应数据bit，有效bit为低时不写入对应数据bit；
                                                        3.若GLB_MPU_CFG.mpu_apb_be为1，那么每一个16bit有效配置占用32bit寄存器地址空间；
                                                        4.根据GLB_MPU_CFG.mpu_addr_zone配置选择地址空间粒度为64KB、128KB还是256KB。 */
    } reg;
} SOC_DMSS_MPU_ADDR_ATTR_UNION;
#endif
#define SOC_DMSS_MPU_ADDR_ATTR_mpu_addr_attr_START  (0)
#define SOC_DMSS_MPU_ADDR_ATTR_mpu_addr_attr_END    (31)






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

#endif /* end of soc_dmss_interface.h */
