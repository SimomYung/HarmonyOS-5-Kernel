/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: isp ddr map
 * Create: 2019-10-29
 */
#ifndef __ISP_DDR_MAP_H__
#define __ISP_DDR_MAP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/* FPGA INFO */
#define ISP_PCTRL_PERI_STAT_ADDR    (0x000000BC)                /* offset */
#define ISP_PCTRL_PERI_FLAG         (1 << 2)     /* BIT 2 : ISP pipe*/

/* DEBUG MEM */
#define MEM_ISPFW_SIZE              (0x00C00000)
#define MEM_RSCTABLE_ADDR_OFFSET    (0x00014000)
#define MEM_RSCTABLE_SIZE           (0x00004000)
#define MEM_ISPDTS_SIZE             (0x02000000)

/* *end* */

/* ISP MEM INFO */
/* image info */
#define TEXT_BASE                   (0x00A00000)
#define SEC_TEXT_BASE               (0x00A00000)
#define ISP_TEXT_SIZE               (DATA_BASE - TEXT_BASE)

#define DATA_BASE                   (0x01000000)

#define PERF_BASE                   (0x08000000)

/* share mem */
#define ISP_RPMSG_VQ_BASE           0x1E00000
#define ISP_RPMSG_VQ_SIZE           0x40000
#define ISP_RPMSG_VR0_BASE          (ISP_RPMSG_VQ_BASE + ISP_RPMSG_VQ_SIZE)
#define ISP_RPMSG_VR_SIZE           (0x3000)
#define ISP_RPMSG_VR1_BASE          (ISP_RPMSG_VR0_BASE + ISP_RPMSG_VR_SIZE)

#define SHAREDMEM_BASE              (ISP_RPMSG_VR1_BASE + ISP_RPMSG_VR_SIZE)
#define SHAREDMEM_SIZE              (0x1000)

/* mem info */
#define MEM_MDC_DA                  (0x01B00000) /*  the mdc iova addr */
#define MEM_MDC_SIZE                (0x00100000) /*  the mdc iova size */
#define MEM_RAW2YUV_DA              (0x00000000) /*  the raw2yuv iova addr */
#define MEM_RAW2YUV_SIZE            (0x00000000) /*  the raw2yuv size */

/* coredump info */
#define ISPCPU_COREDUMP_ADDR        (0x04000000)
#define ISPCPU_COREDUMP_SIZE        (0x01500000) /*should equal DEFAULT_ROTATE_SIZE*/

#define ISP_BIN_DATA_SIZE           (0x0)
#define ISP_FW_SIZE                 (0x0)
#define ISP_BIN_FW_SIZE             (0x0)

/* reg info */
#define DEVICE_BASE                 (0xE0000000)
#define DEVICE_SIZE                 (0x20000000)
#define ISP_CORE_CFG_BASE_ADDR      (0xE0C00000)
#define ITCM_BASE                   (0xFFFF0000)
#define DTCM0_BASE                  (0xE9B40000)
#define DTCM1_BASE                  (0xE9B80000)

#define MAX_REGION_NUM              (24)
#define STATIC_REGION_NSEC_NUM      (14)
#define STATIC_REGION_SEC_NUM       (14)

#define ISP_NOC_ENABLE              0x00
#define ISP_TARGETFLOW              0x05

#define ISP_MAX_IOVA_MAGIC          (0xE0000000)
#define ISP_MAP_MAX_SIZE            (0x20000000)

#define ISP_MAX_REGTYPE_FOR_ISPCPU  (26)

/* isp iomap reg */
#define ISP_CRGPERI      0
#define ISP_ISPCORE      1
#define ISP_PMCTRL       2
#define ISP_PCTRL        3
#define ISP_SCTRL        4
#define ISP_SUBCTRL      5
#define ISP_WDT          6
#define ISP_MEDIA        7
#define ISP_ACTRL        8
#define ISP_MEDIA_CTRL   9
#define ISP_MAX_REGTYPE  10

/* isp clk type */
#define ISPCPU_CLK       0
#define ISPFUNC_CLK      1
#define ISPFUNC2_CLK     2
#define ISPFUNC3_CLK     3
#define ISPI2C_CLK       4
#define ISPI3C_CLK       4
#define ISP_SYS_CLK      5
#define ISP_OISP_CLK     6
#define ISP_SPI_CLK      7
#define ISP_GIC_CLK      8
#define ISP_PERI_CLK     9
#define ISP_DITAG_CLK    10
#define ISP_CORE_CLK     11
#define ISP_VOTE_PPLL1_CLK 12
#define ISP_CLK_MAX      13
/* unused isp clk type at current platform */
#define ISPFUNC4_CLK     ISP_CLK_MAX
#define ISPFUNC5_CLK     ISP_CLK_MAX
#define VIVOBUS_CLK      ISP_CLK_MAX

/*PLL6 usage num*/
#define PLL6_USAGE_NUM          3

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __ISP_DDR_MAP_H__ */
