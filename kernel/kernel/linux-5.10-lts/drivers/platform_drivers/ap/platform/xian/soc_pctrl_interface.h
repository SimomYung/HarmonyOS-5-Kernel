

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_PCTRL_INTERFACE_H__
#define __SOC_PCTRL_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) reg_define
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* Register description: G3D光栅控制寄存器。
   Bit domain definition UNION:  SOC_PCTRL_G3D_RASTER_UNION */
#define SOC_PCTRL_G3D_RASTER_ADDR(base)                       (base)

/* Register description: USB2 HOST控制寄存器0。
   Bit domain definition UNION:  SOC_PCTRL_USB2_HOST_CTRL0_UNION */
#define SOC_PCTRL_USB2_HOST_CTRL0_ADDR(base)                  ((base) + 0x0004UL)

/* Register description: USB2 HOST控制寄存器1。
   Bit domain definition UNION:  SOC_PCTRL_USB2_HOST_CTRL1_UNION */
#define SOC_PCTRL_USB2_HOST_CTRL1_ADDR(base)                  ((base) + 0x0008UL)

/* Register description: USB2 HOST控制寄存器2。
   Bit domain definition UNION:  SOC_PCTRL_USB2_HOST_CTRL2_UNION */
#define SOC_PCTRL_USB2_HOST_CTRL2_ADDR(base)                  ((base) + 0x000CUL)

/* Register description: 通用外设控制寄存器4。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL4_UNION */
#define SOC_PCTRL_PERI_CTRL4_ADDR(base)                       ((base) + 0x0010UL)

/* Register description: 通用外设控制寄存器98。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL98_UNION */
#define SOC_PCTRL_PERI_CTRL98_ADDR(base)                      ((base) + 0x0014UL)

/* Register description: 通用外设控制寄存器99。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL99_UNION */
#define SOC_PCTRL_PERI_CTRL99_ADDR(base)                      ((base) + 0x0018UL)

/* Register description: 通用外设控制寄存器100。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL100_UNION */
#define SOC_PCTRL_PERI_CTRL100_ADDR(base)                     ((base) + 0x001CUL)

/* Register description: 通用外设控制寄存器101。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL101_UNION */
#define SOC_PCTRL_PERI_CTRL101_ADDR(base)                     ((base) + 0x0020UL)

/* Register description: 通用外设控制寄存器104。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL104_UNION */
#define SOC_PCTRL_PERI_CTRL104_ADDR(base)                     ((base) + 0x0024UL)

/* Register description: 通用外设控制寄存器12。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL12_UNION */
#define SOC_PCTRL_PERI_CTRL12_ADDR(base)                      ((base) + 0x0028UL)

/* Register description: 通用外设控制寄存器13。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL13_UNION */
#define SOC_PCTRL_PERI_CTRL13_ADDR(base)                      ((base) + 0x002CUL)

/* Register description: 通用外设控制寄存器14。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL14_UNION */
#define SOC_PCTRL_PERI_CTRL14_ADDR(base)                      ((base) + 0x0030UL)

/* Register description: 通用外设控制寄存器15。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL15_UNION */
#define SOC_PCTRL_PERI_CTRL15_ADDR(base)                      ((base) + 0x0034UL)

/* Register description: 通用外设控制寄存器16。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL16_UNION */
#define SOC_PCTRL_PERI_CTRL16_ADDR(base)                      ((base) + 0x0038UL)

/* Register description: 通用外设控制寄存器17。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL17_UNION */
#define SOC_PCTRL_PERI_CTRL17_ADDR(base)                      ((base) + 0x003CUL)

/* Register description: 通用外设控制寄存器18。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL18_UNION */
#define SOC_PCTRL_PERI_CTRL18_ADDR(base)                      ((base) + 0x0040UL)

/* Register description: 通用外设控制寄存器19。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL19_UNION */
#define SOC_PCTRL_PERI_CTRL19_ADDR(base)                      ((base) + 0x0044UL)

/* Register description: 通用外设控制寄存器20。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL20_UNION */
#define SOC_PCTRL_PERI_CTRL20_ADDR(base)                      ((base) + 0x0048UL)

/* Register description: 通用外设控制寄存器21。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL21_UNION */
#define SOC_PCTRL_PERI_CTRL21_ADDR(base)                      ((base) + 0x004CUL)

/* Register description: 通用外设控制寄存器22。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL22_UNION */
#define SOC_PCTRL_PERI_CTRL22_ADDR(base)                      ((base) + 0x0050UL)

/* Register description: 通用外设控制寄存器23。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL23_UNION */
#define SOC_PCTRL_PERI_CTRL23_ADDR(base)                      ((base) + 0x0054UL)

/* Register description: 通用外设控制寄存器24。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL24_UNION */
#define SOC_PCTRL_PERI_CTRL24_ADDR(base)                      ((base) + 0x0058UL)

/* Register description: 通用外设控制寄存器25。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL25_UNION */
#define SOC_PCTRL_PERI_CTRL25_ADDR(base)                      ((base) + 0x005CUL)

/* Register description: 通用外设控制寄存器26。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL26_UNION */
#define SOC_PCTRL_PERI_CTRL26_ADDR(base)                      ((base) + 0x0060UL)

/* Register description: 通用外设控制寄存器27。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL27_UNION */
#define SOC_PCTRL_PERI_CTRL27_ADDR(base)                      ((base) + 0x0064UL)

/* Register description: 通用外设控制寄存器28。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL28_UNION */
#define SOC_PCTRL_PERI_CTRL28_ADDR(base)                      ((base) + 0x0068UL)

/* Register description: 通用外设控制寄存器29。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL29_UNION */
#define SOC_PCTRL_PERI_CTRL29_ADDR(base)                      ((base) + 0x006CUL)

/* Register description: 通用外设控制寄存器30。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL30_UNION */
#define SOC_PCTRL_PERI_CTRL30_ADDR(base)                      ((base) + 0x0070UL)

/* Register description: 通用外设控制寄存器31。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL31_UNION */
#define SOC_PCTRL_PERI_CTRL31_ADDR(base)                      ((base) + 0x0074UL)

/* Register description: 通用外设控制寄存器32。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL32_UNION */
#define SOC_PCTRL_PERI_CTRL32_ADDR(base)                      ((base) + 0x0078UL)

/* Register description: 通用外设控制寄存器33。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL33_UNION */
#define SOC_PCTRL_PERI_CTRL33_ADDR(base)                      ((base) + 0x007CUL)

/* Register description: 通用外设控制寄存器67。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL67_UNION */
#define SOC_PCTRL_PERI_CTRL67_ADDR(base)                      ((base) + 0x0080UL)

/* Register description: 通用外设控制寄存器68。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL68_UNION */
#define SOC_PCTRL_PERI_CTRL68_ADDR(base)                      ((base) + 0x0084UL)

/* Register description: 通用外设控制寄存器69。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL69_UNION */
#define SOC_PCTRL_PERI_CTRL69_ADDR(base)                      ((base) + 0x0088UL)

/* Register description: 通用外设控制寄存器70。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL70_UNION */
#define SOC_PCTRL_PERI_CTRL70_ADDR(base)                      ((base) + 0x008CUL)

/* Register description: 通用外设控制寄存器71。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL71_UNION */
#define SOC_PCTRL_PERI_CTRL71_ADDR(base)                      ((base) + 0x0090UL)

/* Register description: 通用外设控制寄存器72。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL72_UNION */
#define SOC_PCTRL_PERI_CTRL72_ADDR(base)                      ((base) + 0x0094UL)

/* Register description: 通用外设控制寄存器73。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL73_UNION */
#define SOC_PCTRL_PERI_CTRL73_ADDR(base)                      ((base) + 0x0098UL)

/* Register description: 通用外设控制寄存器74。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL74_UNION */
#define SOC_PCTRL_PERI_CTRL74_ADDR(base)                      ((base) + 0x009CUL)

/* Register description: 通用外设控制寄存器88。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL88_UNION */
#define SOC_PCTRL_PERI_CTRL88_ADDR(base)                      ((base) + 0x00A0UL)

/* Register description: 通用外设控制寄存器89。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL89_UNION */
#define SOC_PCTRL_PERI_CTRL89_ADDR(base)                      ((base) + 0x00A4UL)

/* Register description: 通用外设控制寄存器90。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL90_UNION */
#define SOC_PCTRL_PERI_CTRL90_ADDR(base)                      ((base) + 0x00A8UL)

/* Register description: 通用外设控制寄存器91。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL91_UNION */
#define SOC_PCTRL_PERI_CTRL91_ADDR(base)                      ((base) + 0x00ACUL)

/* Register description: 通用外设控制寄存器92。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL92_UNION */
#define SOC_PCTRL_PERI_CTRL92_ADDR(base)                      ((base) + 0x00B0UL)

/* Register description: 通用外设控制寄存器93。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL93_UNION */
#define SOC_PCTRL_PERI_CTRL93_ADDR(base)                      ((base) + 0x00B4UL)

/* Register description: 通用外设控制寄存器95。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL95_UNION */
#define SOC_PCTRL_PERI_CTRL95_ADDR(base)                      ((base) + 0x00B8UL)

/* Register description: 通用外设控制寄存器96。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL96_UNION */
#define SOC_PCTRL_PERI_CTRL96_ADDR(base)                      ((base) + 0x00BCUL)

/* Register description: 通用外设控制寄存器97。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL97_UNION */
#define SOC_PCTRL_PERI_CTRL97_ADDR(base)                      ((base) + 0x00C0UL)

/* Register description: 通用外设控制寄存器108。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL108_UNION */
#define SOC_PCTRL_PERI_CTRL108_ADDR(base)                     ((base) + 0x00C4UL)

/* Register description: 通用外设控制寄存器109。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL109_UNION */
#define SOC_PCTRL_PERI_CTRL109_ADDR(base)                     ((base) + 0x00C8UL)

/* Register description: 通用外设控制寄存器110。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL110_UNION */
#define SOC_PCTRL_PERI_CTRL110_ADDR(base)                     ((base) + 0x00CCUL)

/* Register description: 通用外设控制寄存器111。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL111_UNION */
#define SOC_PCTRL_PERI_CTRL111_ADDR(base)                     ((base) + 0x00D0UL)

/* Register description: 通用外设控制寄存器112。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL112_UNION */
#define SOC_PCTRL_PERI_CTRL112_ADDR(base)                     ((base) + 0x00D4UL)

/* Register description: 通用外设控制寄存器113。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL113_UNION */
#define SOC_PCTRL_PERI_CTRL113_ADDR(base)                     ((base) + 0x00D8UL)

/* Register description: 通用外设控制寄存器114。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL114_UNION */
#define SOC_PCTRL_PERI_CTRL114_ADDR(base)                     ((base) + 0x00DCUL)

/* Register description: 通用外设控制寄存器115。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL115_UNION */
#define SOC_PCTRL_PERI_CTRL115_ADDR(base)                     ((base) + 0x00E0UL)

/* Register description: 通用外设控制寄存器116。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL116_UNION */
#define SOC_PCTRL_PERI_CTRL116_ADDR(base)                     ((base) + 0x00E4UL)

/* Register description: 通用外设控制寄存器117。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL117_UNION */
#define SOC_PCTRL_PERI_CTRL117_ADDR(base)                     ((base) + 0x00E8UL)

/* Register description: 通用外设控制寄存器118。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL118_UNION */
#define SOC_PCTRL_PERI_CTRL118_ADDR(base)                     ((base) + 0x00ECUL)

/* Register description: 通用外设控制寄存器119。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL119_UNION */
#define SOC_PCTRL_PERI_CTRL119_ADDR(base)                     ((base) + 0x00F0UL)

/* Register description: 通用外设控制寄存器120。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL120_UNION */
#define SOC_PCTRL_PERI_CTRL120_ADDR(base)                     ((base) + 0x00F4UL)

/* Register description: 通用外设控制寄存器121。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL121_UNION */
#define SOC_PCTRL_PERI_CTRL121_ADDR(base)                     ((base) + 0x00F8UL)

/* Register description: 通用外设控制寄存器122。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL122_UNION */
#define SOC_PCTRL_PERI_CTRL122_ADDR(base)                     ((base) + 0x00FCUL)

/* Register description: 通用外设控制寄存器123。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL123_UNION */
#define SOC_PCTRL_PERI_CTRL123_ADDR(base)                     ((base) + 0x0100UL)

/* Register description: 通用外设控制寄存器124。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL124_UNION */
#define SOC_PCTRL_PERI_CTRL124_ADDR(base)                     ((base) + 0x0104UL)

/* Register description: 通用外设控制寄存器125。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL125_UNION */
#define SOC_PCTRL_PERI_CTRL125_ADDR(base)                     ((base) + 0x0108UL)

/* Register description: 通用外设控制寄存器126。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL126_UNION */
#define SOC_PCTRL_PERI_CTRL126_ADDR(base)                     ((base) + 0x010CUL)

/* Register description: 通用外设控制寄存器127。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL127_UNION */
#define SOC_PCTRL_PERI_CTRL127_ADDR(base)                     ((base) + 0x0110UL)

/* Register description: 通用外设控制寄存器128。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL128_UNION */
#define SOC_PCTRL_PERI_CTRL128_ADDR(base)                     ((base) + 0x0114UL)

/* Register description: 通用外设控制寄存器129。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL129_UNION */
#define SOC_PCTRL_PERI_CTRL129_ADDR(base)                     ((base) + 0x0118UL)

/* Register description: 通用外设控制寄存器130。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL130_UNION */
#define SOC_PCTRL_PERI_CTRL130_ADDR(base)                     ((base) + 0x011CUL)

/* Register description: 通用外设控制寄存器131。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL131_UNION */
#define SOC_PCTRL_PERI_CTRL131_ADDR(base)                     ((base) + 0x0120UL)

/* Register description: 通用外设控制寄存器132。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL132_UNION */
#define SOC_PCTRL_PERI_CTRL132_ADDR(base)                     ((base) + 0x0124UL)

/* Register description: 通用外设控制寄存器133。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL133_UNION */
#define SOC_PCTRL_PERI_CTRL133_ADDR(base)                     ((base) + 0x0128UL)

/* Register description: 通用外设控制寄存器134。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL134_UNION */
#define SOC_PCTRL_PERI_CTRL134_ADDR(base)                     ((base) + 0x012CUL)

/* Register description: 通用外设控制寄存器135。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL135_UNION */
#define SOC_PCTRL_PERI_CTRL135_ADDR(base)                     ((base) + 0x0130UL)

/* Register description: 通用外设控制寄存器136。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL136_UNION */
#define SOC_PCTRL_PERI_CTRL136_ADDR(base)                     ((base) + 0x0134UL)

/* Register description: 通用外设控制寄存器137。
   Bit domain definition UNION:  SOC_PCTRL_mdm_tsp_nmi0_UNION */
#define SOC_PCTRL_mdm_tsp_nmi0_ADDR(base)                     ((base) + 0x0138UL)

/* Register description: 通用外设控制寄存器138。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL138_UNION */
#define SOC_PCTRL_PERI_CTRL138_ADDR(base)                     ((base) + 0x013CUL)

/* Register description: 通用外设控制寄存器139。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL139_UNION */
#define SOC_PCTRL_PERI_CTRL139_ADDR(base)                     ((base) + 0x0140UL)

/* Register description: 通用外设控制寄存器140。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL140_UNION */
#define SOC_PCTRL_PERI_CTRL140_ADDR(base)                     ((base) + 0x0144UL)

/* Register description: 通用外设控制寄存器141。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL141_UNION */
#define SOC_PCTRL_PERI_CTRL141_ADDR(base)                     ((base) + 0x0148UL)

/* Register description: 通用外设控制寄存器142。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL142_UNION */
#define SOC_PCTRL_PERI_CTRL142_ADDR(base)                     ((base) + 0x014CUL)

/* Register description: 通用外设控制寄存器143。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL143_UNION */
#define SOC_PCTRL_PERI_CTRL143_ADDR(base)                     ((base) + 0x0150UL)

/* Register description: 通用外设控制寄存器144。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL144_UNION */
#define SOC_PCTRL_PERI_CTRL144_ADDR(base)                     ((base) + 0x0154UL)

/* Register description: 通用外设控制寄存器145。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL145_UNION */
#define SOC_PCTRL_PERI_CTRL145_ADDR(base)                     ((base) + 0x0158UL)

/* Register description: 通用外设控制寄存器146。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL146_UNION */
#define SOC_PCTRL_PERI_CTRL146_ADDR(base)                     ((base) + 0x015CUL)

/* Register description: 通用外设控制寄存器147。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL147_UNION */
#define SOC_PCTRL_PERI_CTRL147_ADDR(base)                     ((base) + 0x0160UL)

/* Register description: 通用外设控制寄存器148。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL148_UNION */
#define SOC_PCTRL_PERI_CTRL148_ADDR(base)                     ((base) + 0x0164UL)

/* Register description: 通用外设控制寄存器149。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL149_UNION */
#define SOC_PCTRL_PERI_CTRL149_ADDR(base)                     ((base) + 0x0168UL)

/* Register description: 通用外设控制寄存器150。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL150_UNION */
#define SOC_PCTRL_PERI_CTRL150_ADDR(base)                     ((base) + 0x016CUL)

/* Register description: 通用外设控制寄存器151。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL151_UNION */
#define SOC_PCTRL_PERI_CTRL151_ADDR(base)                     ((base) + 0x0170UL)

/* Register description: 通用外设控制寄存器152。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL152_UNION */
#define SOC_PCTRL_PERI_CTRL152_ADDR(base)                     ((base) + 0x0174UL)

/* Register description: 通用外设控制寄存器153。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL153_UNION */
#define SOC_PCTRL_PERI_CTRL153_ADDR(base)                     ((base) + 0x0178UL)

/* Register description: 通用外设控制寄存器154。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL154_UNION */
#define SOC_PCTRL_PERI_CTRL154_ADDR(base)                     ((base) + 0x017CUL)

/* Register description: 通用外设控制寄存器155。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL155_UNION */
#define SOC_PCTRL_PERI_CTRL155_ADDR(base)                     ((base) + 0x0180UL)

/* Register description: 通用外设控制寄存器156。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL156_UNION */
#define SOC_PCTRL_PERI_CTRL156_ADDR(base)                     ((base) + 0x0184UL)

/* Register description: 通用外设控制寄存器157。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL157_UNION */
#define SOC_PCTRL_PERI_CTRL157_ADDR(base)                     ((base) + 0x0188UL)

/* Register description: 通用外设控制寄存器158。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL158_UNION */
#define SOC_PCTRL_PERI_CTRL158_ADDR(base)                     ((base) + 0x018CUL)

/* Register description: 通用外设控制寄存器159。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL159_UNION */
#define SOC_PCTRL_PERI_CTRL159_ADDR(base)                     ((base) + 0x0190UL)

/* Register description: 通用外设控制寄存器160。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL160_UNION */
#define SOC_PCTRL_PERI_CTRL160_ADDR(base)                     ((base) + 0x0194UL)

/* Register description: 通用外设控制寄存器161。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL161_UNION */
#define SOC_PCTRL_PERI_CTRL161_ADDR(base)                     ((base) + 0x0198UL)

/* Register description: 通用外设控制寄存器162。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL162_UNION */
#define SOC_PCTRL_PERI_CTRL162_ADDR(base)                     ((base) + 0x019CUL)

/* Register description: 通用外设控制寄存器163。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL163_UNION */
#define SOC_PCTRL_PERI_CTRL163_ADDR(base)                     ((base) + 0x01A0UL)

/* Register description: 通用外设控制寄存器164。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL164_UNION */
#define SOC_PCTRL_PERI_CTRL164_ADDR(base)                     ((base) + 0x01A4UL)

/* Register description: 通用外设控制寄存器165。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL165_UNION */
#define SOC_PCTRL_PERI_CTRL165_ADDR(base)                     ((base) + 0x01A8UL)

/* Register description: 通用外设控制寄存器166。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL166_UNION */
#define SOC_PCTRL_PERI_CTRL166_ADDR(base)                     ((base) + 0x01ACUL)

/* Register description: 通用外设控制寄存器167。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL167_UNION */
#define SOC_PCTRL_PERI_CTRL167_ADDR(base)                     ((base) + 0x01B0UL)

/* Register description: 通用外设状态寄存器0。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT0_UNION */
#define SOC_PCTRL_PERI_STAT0_ADDR(base)                       ((base) + 0x1000UL)

/* Register description: 通用外设状态寄存器1。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT1_UNION */
#define SOC_PCTRL_PERI_STAT1_ADDR(base)                       ((base) + 0x1004UL)

/* Register description: 通用外设状态寄存器2。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT2_UNION */
#define SOC_PCTRL_PERI_STAT2_ADDR(base)                       ((base) + 0x1008UL)

/* Register description: 通用外设状态寄存器3。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT3_UNION */
#define SOC_PCTRL_PERI_STAT3_ADDR(base)                       ((base) + 0x100CUL)

/* Register description: 通用外设状态寄存器4。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT4_UNION */
#define SOC_PCTRL_PERI_STAT4_ADDR(base)                       ((base) + 0x1010UL)

/* Register description: 通用外设状态寄存器5。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT5_UNION */
#define SOC_PCTRL_PERI_STAT5_ADDR(base)                       ((base) + 0x1014UL)

/* Register description: 通用外设状态寄存器6。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT6_UNION */
#define SOC_PCTRL_PERI_STAT6_ADDR(base)                       ((base) + 0x1018UL)

/* Register description: 通用外设状态寄存器7--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT7_UNION */
#define SOC_PCTRL_PERI_STAT7_ADDR(base)                       ((base) + 0x101CUL)

/* Register description: 通用外设状态寄存器8--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT8_UNION */
#define SOC_PCTRL_PERI_STAT8_ADDR(base)                       ((base) + 0x1020UL)

/* Register description: 通用外设状态寄存器9--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT9_UNION */
#define SOC_PCTRL_PERI_STAT9_ADDR(base)                       ((base) + 0x1024UL)

/* Register description: 通用外设状态寄存器10--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT10_UNION */
#define SOC_PCTRL_PERI_STAT10_ADDR(base)                      ((base) + 0x1028UL)

/* Register description: 通用外设状态寄存器11。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT11_UNION */
#define SOC_PCTRL_PERI_STAT11_ADDR(base)                      ((base) + 0x102CUL)

/* Register description: 通用外设状态寄存器12。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT12_UNION */
#define SOC_PCTRL_PERI_STAT12_ADDR(base)                      ((base) + 0x1030UL)

/* Register description: 通用外设状态寄存器13。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT13_UNION */
#define SOC_PCTRL_PERI_STAT13_ADDR(base)                      ((base) + 0x1034UL)

/* Register description: 通用外设状态寄存器14。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT14_UNION */
#define SOC_PCTRL_PERI_STAT14_ADDR(base)                      ((base) + 0x1038UL)

/* Register description: 通用外设状态寄存器15。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT15_UNION */
#define SOC_PCTRL_PERI_STAT15_ADDR(base)                      ((base) + 0x103CUL)

/* Register description: 通用外设状态寄存器16。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT16_UNION */
#define SOC_PCTRL_PERI_STAT16_ADDR(base)                      ((base) + 0x1040UL)

/* Register description: 通用外设状态寄存器17。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT17_UNION */
#define SOC_PCTRL_PERI_STAT17_ADDR(base)                      ((base) + 0x1044UL)

/* Register description: 通用外设状态寄存器18。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT18_UNION */
#define SOC_PCTRL_PERI_STAT18_ADDR(base)                      ((base) + 0x1048UL)

/* Register description: 通用外设状态寄存器19。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT19_UNION */
#define SOC_PCTRL_PERI_STAT19_ADDR(base)                      ((base) + 0x104CUL)

/* Register description: 通用外设状态寄存器29。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT29_UNION */
#define SOC_PCTRL_PERI_STAT29_ADDR(base)                      ((base) + 0x1050UL)

/* Register description: 通用外设状态寄存器30。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT30_UNION */
#define SOC_PCTRL_PERI_STAT30_ADDR(base)                      ((base) + 0x1054UL)

/* Register description: 通用外设状态寄存器31。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT31_UNION */
#define SOC_PCTRL_PERI_STAT31_ADDR(base)                      ((base) + 0x1058UL)

/* Register description: 通用外设状态寄存器32。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT32_UNION */
#define SOC_PCTRL_PERI_STAT32_ADDR(base)                      ((base) + 0x105CUL)

/* Register description: 通用外设状态寄存器33。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT33_UNION */
#define SOC_PCTRL_PERI_STAT33_ADDR(base)                      ((base) + 0x1060UL)

/* Register description: 通用外设状态寄存器34。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT34_UNION */
#define SOC_PCTRL_PERI_STAT34_ADDR(base)                      ((base) + 0x1064UL)

/* Register description: 通用外设状态寄存器35。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT35_UNION */
#define SOC_PCTRL_PERI_STAT35_ADDR(base)                      ((base) + 0x1068UL)

/* Register description: 通用外设状态寄存器36。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT36_UNION */
#define SOC_PCTRL_PERI_STAT36_ADDR(base)                      ((base) + 0x106CUL)

/* Register description: 通用外设状态寄存器37。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT37_UNION */
#define SOC_PCTRL_PERI_STAT37_ADDR(base)                      ((base) + 0x1070UL)

/* Register description: 通用外设状态寄存器38。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT38_UNION */
#define SOC_PCTRL_PERI_STAT38_ADDR(base)                      ((base) + 0x1074UL)

/* Register description: 通用外设状态寄存器39。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT39_UNION */
#define SOC_PCTRL_PERI_STAT39_ADDR(base)                      ((base) + 0x1078UL)

/* Register description: 通用外设状态寄存器40。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT40_UNION */
#define SOC_PCTRL_PERI_STAT40_ADDR(base)                      ((base) + 0x107CUL)

/* Register description: 通用外设状态寄存器41。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT41_UNION */
#define SOC_PCTRL_PERI_STAT41_ADDR(base)                      ((base) + 0x1080UL)

/* Register description: 通用外设状态寄存器42。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT42_UNION */
#define SOC_PCTRL_PERI_STAT42_ADDR(base)                      ((base) + 0x1084UL)

/* Register description: 通用外设状态寄存器43。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT43_UNION */
#define SOC_PCTRL_PERI_STAT43_ADDR(base)                      ((base) + 0x1088UL)

/* Register description: 通用外设状态寄存器44。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT44_UNION */
#define SOC_PCTRL_PERI_STAT44_ADDR(base)                      ((base) + 0x108CUL)

/* Register description: 通用外设状态寄存器45。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT45_UNION */
#define SOC_PCTRL_PERI_STAT45_ADDR(base)                      ((base) + 0x1090UL)

/* Register description: 通用外设状态寄存器46。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT46_UNION */
#define SOC_PCTRL_PERI_STAT46_ADDR(base)                      ((base) + 0x1094UL)

/* Register description: 通用外设状态寄存器47。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT47_UNION */
#define SOC_PCTRL_PERI_STAT47_ADDR(base)                      ((base) + 0x1098UL)

/* Register description: 通用外设状态寄存器48（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT48_UNION */
#define SOC_PCTRL_PERI_STAT48_ADDR(base)                      ((base) + 0x109CUL)

/* Register description: 通用外设状态寄存器49（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT49_UNION */
#define SOC_PCTRL_PERI_STAT49_ADDR(base)                      ((base) + 0x10A0UL)

/* Register description: 通用外设状态寄存器50（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT50_UNION */
#define SOC_PCTRL_PERI_STAT50_ADDR(base)                      ((base) + 0x10A4UL)

/* Register description: 通用外设状态寄存器51（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT51_UNION */
#define SOC_PCTRL_PERI_STAT51_ADDR(base)                      ((base) + 0x10A8UL)

/* Register description: 通用外设状态寄存器52（保留给SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT52_UNION */
#define SOC_PCTRL_PERI_STAT52_ADDR(base)                      ((base) + 0x10ACUL)

/* Register description: 通用外设状态寄存器53（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT53_UNION */
#define SOC_PCTRL_PERI_STAT53_ADDR(base)                      ((base) + 0x10B0UL)

/* Register description: 通用外设状态寄存器54（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT54_UNION */
#define SOC_PCTRL_PERI_STAT54_ADDR(base)                      ((base) + 0x10B4UL)

/* Register description: 通用外设状态寄存器55（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT55_UNION */
#define SOC_PCTRL_PERI_STAT55_ADDR(base)                      ((base) + 0x10B8UL)

/* Register description: 通用外设状态寄存器56（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT56_UNION */
#define SOC_PCTRL_PERI_STAT56_ADDR(base)                      ((base) + 0x10BCUL)

/* Register description: 通用外设状态寄存器57（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT57_UNION */
#define SOC_PCTRL_PERI_STAT57_ADDR(base)                      ((base) + 0x10C0UL)

/* Register description: 通用外设状态寄存器58（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT58_UNION */
#define SOC_PCTRL_PERI_STAT58_ADDR(base)                      ((base) + 0x10C4UL)

/* Register description: 通用外设状态寄存器59（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT59_UNION */
#define SOC_PCTRL_PERI_STAT59_ADDR(base)                      ((base) + 0x10C8UL)

/* Register description: 通用外设状态寄存器60（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT60_UNION */
#define SOC_PCTRL_PERI_STAT60_ADDR(base)                      ((base) + 0x10CCUL)

/* Register description: 通用外设状态寄存器61（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT61_UNION */
#define SOC_PCTRL_PERI_STAT61_ADDR(base)                      ((base) + 0x10D0UL)

/* Register description: 通用外设状态寄存器62（QIC nopending）
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT62_UNION */
#define SOC_PCTRL_PERI_STAT62_ADDR(base)                      ((base) + 0x10D4UL)

/* Register description: 通用外设状态寄存器63（QIC nopending）
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT63_UNION */
#define SOC_PCTRL_PERI_STAT63_ADDR(base)                      ((base) + 0x10D8UL)

/* Register description: 通用外设状态寄存器64（QIC nopending）
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT64_UNION */
#define SOC_PCTRL_PERI_STAT64_ADDR(base)                      ((base) + 0x10DCUL)

/* Register description: 通用外设状态寄存器66--保留给。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT66_UNION */
#define SOC_PCTRL_PERI_STAT66_ADDR(base)                      ((base) + 0x10E0UL)

/* Register description: 通用外设状态寄存器67--保留给。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT67_UNION */
#define SOC_PCTRL_PERI_STAT67_ADDR(base)                      ((base) + 0x10E4UL)

/* Register description: 通用外设状态寄存器68。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT68_UNION */
#define SOC_PCTRL_PERI_STAT68_ADDR(base)                      ((base) + 0x10E8UL)

/* Register description: 通用外设状态寄存器69。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT69_UNION */
#define SOC_PCTRL_PERI_STAT69_ADDR(base)                      ((base) + 0x10ECUL)

/* Register description: 通用外设状态寄存器70。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT70_UNION */
#define SOC_PCTRL_PERI_STAT70_ADDR(base)                      ((base) + 0x10F0UL)

/* Register description: 通用外设状态寄存器71。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT71_UNION */
#define SOC_PCTRL_PERI_STAT71_ADDR(base)                      ((base) + 0x10F4UL)

/* Register description: 通用外设状态寄存器72。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT72_UNION */
#define SOC_PCTRL_PERI_STAT72_ADDR(base)                      ((base) + 0x10F8UL)

/* Register description: 通用外设状态寄存器73。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT73_UNION */
#define SOC_PCTRL_PERI_STAT73_ADDR(base)                      ((base) + 0x10FCUL)

/* Register description: 通用外设状态寄存器74-mtcmos回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT74_UNION */
#define SOC_PCTRL_PERI_STAT74_ADDR(base)                      ((base) + 0x1100UL)

/* Register description: 通用外设状态寄存器75。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT75_UNION */
#define SOC_PCTRL_PERI_STAT75_ADDR(base)                      ((base) + 0x1104UL)

/* Register description: 通用外设状态寄存器76。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT76_UNION */
#define SOC_PCTRL_PERI_STAT76_ADDR(base)                      ((base) + 0x1108UL)

/* Register description: 通用外设状态寄存器77。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT77_UNION */
#define SOC_PCTRL_PERI_STAT77_ADDR(base)                      ((base) + 0x110CUL)

/* Register description: 通用外设状态寄存器78。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT78_UNION */
#define SOC_PCTRL_PERI_STAT78_ADDR(base)                      ((base) + 0x1110UL)

/* Register description: 通用外设状态寄存器79。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT79_UNION */
#define SOC_PCTRL_PERI_STAT79_ADDR(base)                      ((base) + 0x1114UL)

/* Register description: 通用外设状态寄存器80。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT80_UNION */
#define SOC_PCTRL_PERI_STAT80_ADDR(base)                      ((base) + 0x1118UL)

/* Register description: 通用外设状态寄存器81。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT81_UNION */
#define SOC_PCTRL_PERI_STAT81_ADDR(base)                      ((base) + 0x111CUL)

/* Register description: 通用外设状态寄存器82。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT82_UNION */
#define SOC_PCTRL_PERI_STAT82_ADDR(base)                      ((base) + 0x1120UL)

/* Register description: 通用外设状态寄存器83。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT83_UNION */
#define SOC_PCTRL_PERI_STAT83_ADDR(base)                      ((base) + 0x1124UL)

/* Register description: 通用外设状态寄存器84。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT84_UNION */
#define SOC_PCTRL_PERI_STAT84_ADDR(base)                      ((base) + 0x1128UL)

/* Register description: 通用外设状态寄存器85。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT85_UNION */
#define SOC_PCTRL_PERI_STAT85_ADDR(base)                      ((base) + 0x112CUL)

/* Register description: 通用外设状态寄存器86。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT86_UNION */
#define SOC_PCTRL_PERI_STAT86_ADDR(base)                      ((base) + 0x1130UL)

/* Register description: 通用外设状态寄存器87。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT87_UNION */
#define SOC_PCTRL_PERI_STAT87_ADDR(base)                      ((base) + 0x1134UL)

/* Register description: 通用外设状态寄存器88。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT88_UNION */
#define SOC_PCTRL_PERI_STAT88_ADDR(base)                      ((base) + 0x1138UL)

/* Register description: 通用外设状态寄存器89。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT89_UNION */
#define SOC_PCTRL_PERI_STAT89_ADDR(base)                      ((base) + 0x113CUL)

/* Register description: 通用外设状态寄存器90。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT90_UNION */
#define SOC_PCTRL_PERI_STAT90_ADDR(base)                      ((base) + 0x1140UL)

/* Register description: 通用外设状态寄存器91。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT91_UNION */
#define SOC_PCTRL_PERI_STAT91_ADDR(base)                      ((base) + 0x1144UL)

/* Register description: 通用外设状态寄存器92。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT92_UNION */
#define SOC_PCTRL_PERI_STAT92_ADDR(base)                      ((base) + 0x1148UL)

/* Register description: 通用外设状态寄存器93。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT93_UNION */
#define SOC_PCTRL_PERI_STAT93_ADDR(base)                      ((base) + 0x114CUL)

/* Register description: 通用外设状态寄存器94。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT94_UNION */
#define SOC_PCTRL_PERI_STAT94_ADDR(base)                      ((base) + 0x1150UL)

/* Register description: 通用外设状态寄存器95。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT95_UNION */
#define SOC_PCTRL_PERI_STAT95_ADDR(base)                      ((base) + 0x1154UL)

/* Register description: 通用外设状态寄存器96。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT96_UNION */
#define SOC_PCTRL_PERI_STAT96_ADDR(base)                      ((base) + 0x1158UL)

/* Register description: 通用外设状态寄存器97。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT97_UNION */
#define SOC_PCTRL_PERI_STAT97_ADDR(base)                      ((base) + 0x115CUL)

/* Register description: 通用外设状态寄存器98。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT98_UNION */
#define SOC_PCTRL_PERI_STAT98_ADDR(base)                      ((base) + 0x1160UL)

/* Register description: 通用外设状态寄存器99。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT99_UNION */
#define SOC_PCTRL_PERI_STAT99_ADDR(base)                      ((base) + 0x1164UL)

/* Register description: 通用外设状态寄存器100。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT100_UNION */
#define SOC_PCTRL_PERI_STAT100_ADDR(base)                     ((base) + 0x1168UL)

/* Register description: 通用外设状态寄存器101。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT101_UNION */
#define SOC_PCTRL_PERI_STAT101_ADDR(base)                     ((base) + 0x116CUL)

/* Register description: 通用外设状态寄存器102。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT102_UNION */
#define SOC_PCTRL_PERI_STAT102_ADDR(base)                     ((base) + 0x1170UL)

/* Register description: 通用外设状态寄存器103。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT103_UNION */
#define SOC_PCTRL_PERI_STAT103_ADDR(base)                     ((base) + 0x1174UL)

/* Register description: 通用外设状态寄存器104。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT104_UNION */
#define SOC_PCTRL_PERI_STAT104_ADDR(base)                     ((base) + 0x1178UL)

/* Register description: 通用外设状态寄存器105。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT105_UNION */
#define SOC_PCTRL_PERI_STAT105_ADDR(base)                     ((base) + 0x117CUL)

/* Register description: 通用外设状态寄存器106。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT106_UNION */
#define SOC_PCTRL_PERI_STAT106_ADDR(base)                     ((base) + 0x1180UL)

/* Register description: 通用外设状态寄存器107。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT107_UNION */
#define SOC_PCTRL_PERI_STAT107_ADDR(base)                     ((base) + 0x1184UL)

/* Register description: 通用外设状态寄存器108。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT108_UNION */
#define SOC_PCTRL_PERI_STAT108_ADDR(base)                     ((base) + 0x1188UL)

/* Register description: 通用外设状态寄存器109。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT109_UNION */
#define SOC_PCTRL_PERI_STAT109_ADDR(base)                     ((base) + 0x118CUL)

/* Register description: 通用外设状态寄存器110。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT110_UNION */
#define SOC_PCTRL_PERI_STAT110_ADDR(base)                     ((base) + 0x1190UL)

/* Register description: 通用外设状态寄存器111。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT111_UNION */
#define SOC_PCTRL_PERI_STAT111_ADDR(base)                     ((base) + 0x1194UL)

/* Register description: 通用外设状态寄存器112。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT112_UNION */
#define SOC_PCTRL_PERI_STAT112_ADDR(base)                     ((base) + 0x1198UL)

/* Register description: 通用外设状态寄存器113。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT113_UNION */
#define SOC_PCTRL_PERI_STAT113_ADDR(base)                     ((base) + 0x119CUL)

/* Register description: 通用外设状态寄存器114。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT114_UNION */
#define SOC_PCTRL_PERI_STAT114_ADDR(base)                     ((base) + 0x11A0UL)

/* Register description: 通用外设状态寄存器115。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT115_UNION */
#define SOC_PCTRL_PERI_STAT115_ADDR(base)                     ((base) + 0x11A4UL)

/* Register description: 通用外设状态寄存器116。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT116_UNION */
#define SOC_PCTRL_PERI_STAT116_ADDR(base)                     ((base) + 0x11A8UL)

/* Register description: 通用外设状态寄存器117。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT117_UNION */
#define SOC_PCTRL_PERI_STAT117_ADDR(base)                     ((base) + 0x11ACUL)

/* Register description: 通用外设状态寄存器118。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT118_UNION */
#define SOC_PCTRL_PERI_STAT118_ADDR(base)                     ((base) + 0x11B0UL)

/* Register description: 通用外设状态寄存器119。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT119_UNION */
#define SOC_PCTRL_PERI_STAT119_ADDR(base)                     ((base) + 0x11B4UL)

/* Register description: 通用外设状态寄存器120。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT120_UNION */
#define SOC_PCTRL_PERI_STAT120_ADDR(base)                     ((base) + 0x11B8UL)

/* Register description: 通用外设状态寄存器121。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT121_UNION */
#define SOC_PCTRL_PERI_STAT121_ADDR(base)                     ((base) + 0x11BCUL)

/* Register description: 通用外设状态寄存器122。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT122_UNION */
#define SOC_PCTRL_PERI_STAT122_ADDR(base)                     ((base) + 0x11C0UL)

/* Register description: 通用外设状态寄存器123。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT123_UNION */
#define SOC_PCTRL_PERI_STAT123_ADDR(base)                     ((base) + 0x11C4UL)

/* Register description: 通用外设状态寄存器124。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT124_UNION */
#define SOC_PCTRL_PERI_STAT124_ADDR(base)                     ((base) + 0x11C8UL)

/* Register description: 通用外设状态寄存器125。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT125_UNION */
#define SOC_PCTRL_PERI_STAT125_ADDR(base)                     ((base) + 0x11CCUL)

/* Register description: 通用外设状态寄存器126。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT126_UNION */
#define SOC_PCTRL_PERI_STAT126_ADDR(base)                     ((base) + 0x11D0UL)

/* Register description: 通用外设状态寄存器127。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT127_UNION */
#define SOC_PCTRL_PERI_STAT127_ADDR(base)                     ((base) + 0x11D4UL)

/* Register description: 通用外设状态寄存器128。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT128_UNION */
#define SOC_PCTRL_PERI_STAT128_ADDR(base)                     ((base) + 0x11D8UL)

/* Register description: 通用外设状态寄存器129-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT129_UNION */
#define SOC_PCTRL_PERI_STAT129_ADDR(base)                     ((base) + 0x11DCUL)

/* Register description: 通用外设状态寄存器130-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT130_UNION */
#define SOC_PCTRL_PERI_STAT130_ADDR(base)                     ((base) + 0x11E0UL)

/* Register description: 通用外设状态寄存器131-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT131_UNION */
#define SOC_PCTRL_PERI_STAT131_ADDR(base)                     ((base) + 0x11E4UL)

/* Register description: 通用外设状态寄存器132-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT132_UNION */
#define SOC_PCTRL_PERI_STAT132_ADDR(base)                     ((base) + 0x11E8UL)

/* Register description: 通用外设状态寄存器133-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT133_UNION */
#define SOC_PCTRL_PERI_STAT133_ADDR(base)                     ((base) + 0x11ECUL)

/* Register description: 通用外设状态寄存器134-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT134_UNION */
#define SOC_PCTRL_PERI_STAT134_ADDR(base)                     ((base) + 0x11F0UL)

/* Register description: 通用外设状态寄存器135-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT135_UNION */
#define SOC_PCTRL_PERI_STAT135_ADDR(base)                     ((base) + 0x11F4UL)

/* Register description: 通用外设状态寄存器136-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT136_UNION */
#define SOC_PCTRL_PERI_STAT136_ADDR(base)                     ((base) + 0x11F8UL)

/* Register description: 通用外设状态寄存器137-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT137_UNION */
#define SOC_PCTRL_PERI_STAT137_ADDR(base)                     ((base) + 0x11FCUL)

/* Register description: 通用外设状态寄存器138-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT138_UNION */
#define SOC_PCTRL_PERI_STAT138_ADDR(base)                     ((base) + 0x1200UL)

/* Register description: 通用外设状态寄存器139-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT139_UNION */
#define SOC_PCTRL_PERI_STAT139_ADDR(base)                     ((base) + 0x1204UL)

/* Register description: 通用外设状态寄存器140-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT140_UNION */
#define SOC_PCTRL_PERI_STAT140_ADDR(base)                     ((base) + 0x1208UL)

/* Register description: 通用外设状态寄存器141-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT141_UNION */
#define SOC_PCTRL_PERI_STAT141_ADDR(base)                     ((base) + 0x120CUL)

/* Register description: 通用外设状态寄存器142-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT142_UNION */
#define SOC_PCTRL_PERI_STAT142_ADDR(base)                     ((base) + 0x1210UL)

/* Register description: 通用外设状态寄存器143-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT143_UNION */
#define SOC_PCTRL_PERI_STAT143_ADDR(base)                     ((base) + 0x1214UL)

/* Register description: 通用外设状态寄存器144-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT144_UNION */
#define SOC_PCTRL_PERI_STAT144_ADDR(base)                     ((base) + 0x1218UL)

/* Register description: 通用外设状态寄存器145-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT145_UNION */
#define SOC_PCTRL_PERI_STAT145_ADDR(base)                     ((base) + 0x121CUL)

/* Register description: 通用外设状态寄存器146-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT146_UNION */
#define SOC_PCTRL_PERI_STAT146_ADDR(base)                     ((base) + 0x1220UL)

/* Register description: 通用外设状态寄存器147-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT147_UNION */
#define SOC_PCTRL_PERI_STAT147_ADDR(base)                     ((base) + 0x1224UL)

/* Register description: 通用外设状态寄存器148-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT148_UNION */
#define SOC_PCTRL_PERI_STAT148_ADDR(base)                     ((base) + 0x1228UL)

/* Register description: 通用外设状态寄存器149-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT149_UNION */
#define SOC_PCTRL_PERI_STAT149_ADDR(base)                     ((base) + 0x122CUL)

/* Register description: 通用外设状态寄存器150-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT150_UNION */
#define SOC_PCTRL_PERI_STAT150_ADDR(base)                     ((base) + 0x1230UL)

/* Register description: 通用外设状态寄存器151-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT151_UNION */
#define SOC_PCTRL_PERI_STAT151_ADDR(base)                     ((base) + 0x1234UL)

/* Register description: 通用外设状态寄存器152
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT152_UNION */
#define SOC_PCTRL_PERI_STAT152_ADDR(base)                     ((base) + 0x1238UL)

/* Register description: 通用外设状态寄存器153
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT153_UNION */
#define SOC_PCTRL_PERI_STAT153_ADDR(base)                     ((base) + 0x123CUL)

/* Register description: 通用外设状态寄存器154
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT154_UNION */
#define SOC_PCTRL_PERI_STAT154_ADDR(base)                     ((base) + 0x1240UL)

/* Register description: 通用外设状态寄存器155
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT155_UNION */
#define SOC_PCTRL_PERI_STAT155_ADDR(base)                     ((base) + 0x1244UL)

/* Register description: 通用外设状态寄存器156
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT156_UNION */
#define SOC_PCTRL_PERI_STAT156_ADDR(base)                     ((base) + 0x1248UL)

/* Register description: 通用外设状态寄存器157
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT157_UNION */
#define SOC_PCTRL_PERI_STAT157_ADDR(base)                     ((base) + 0x124CUL)

/* Register description: 通用外设状态寄存器158
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT158_UNION */
#define SOC_PCTRL_PERI_STAT158_ADDR(base)                     ((base) + 0x1250UL)

/* Register description: 通用外设状态寄存器159
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT159_UNION */
#define SOC_PCTRL_PERI_STAT159_ADDR(base)                     ((base) + 0x1254UL)

/* Register description: 通用外设控制寄存器0。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL0_UNION */
#define SOC_PCTRL_PERI_CTRL0_ADDR(base)                       ((base) + 0x1800UL)

/* Register description: 通用外设控制寄存器1。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL1_UNION */
#define SOC_PCTRL_PERI_CTRL1_ADDR(base)                       ((base) + 0x1804UL)

/* Register description: 通用外设控制寄存器2。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL2_UNION */
#define SOC_PCTRL_PERI_CTRL2_ADDR(base)                       ((base) + 0x1808UL)

/* Register description: 通用外设控制寄存器3。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL3_UNION */
#define SOC_PCTRL_PERI_CTRL3_ADDR(base)                       ((base) + 0x181CUL)

/* Register description: 通用外设控制寄存器102。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL102_UNION */
#define SOC_PCTRL_PERI_CTRL102_ADDR(base)                     ((base) + 0x1820UL)

/* Register description: 通用外设控制寄存器103。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL103_UNION */
#define SOC_PCTRL_PERI_CTRL103_ADDR(base)                     ((base) + 0x1824UL)

/* Register description: 通用外设控制寄存器105。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL105_UNION */
#define SOC_PCTRL_PERI_CTRL105_ADDR(base)                     ((base) + 0x1828UL)

/* Register description: 通用外设控制寄存器106。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL106_UNION */
#define SOC_PCTRL_PERI_CTRL106_ADDR(base)                     ((base) + 0x182CUL)

/* Register description: 通用外设控制寄存器107。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL107_UNION */
#define SOC_PCTRL_PERI_CTRL107_ADDR(base)                     ((base) + 0x1830UL)

/* Register description: 通用外设状态寄存器176(QIC nopending WC回读）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl176_wc_UNION */
#define SOC_PCTRL_peri_ctrl176_wc_ADDR(base)                  ((base) + 0x1834UL)

/* Register description: 通用外设状态寄存器177（QIC nopending WC回读）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl177_wc_UNION */
#define SOC_PCTRL_peri_ctrl177_wc_ADDR(base)                  ((base) + 0x1838UL)

/* Register description: 通用外设状态寄存器178（QIC nopending WC回读）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl178_wc_UNION */
#define SOC_PCTRL_peri_ctrl178_wc_ADDR(base)                  ((base) + 0x183CUL)

/* Register description: 通用外设状态寄存器179。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl179_wc_UNION */
#define SOC_PCTRL_peri_ctrl179_wc_ADDR(base)                  ((base) + 0x1840UL)

/* Register description: 通用外设状态寄存器180。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl180_wc_UNION */
#define SOC_PCTRL_peri_ctrl180_wc_ADDR(base)                  ((base) + 0x1844UL)

/* Register description: 通用外设状态寄存器181。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl181_wc_UNION */
#define SOC_PCTRL_peri_ctrl181_wc_ADDR(base)                  ((base) + 0x1848UL)

/* Register description: 通用外设状态寄存器182。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl182_wc_UNION */
#define SOC_PCTRL_peri_ctrl182_wc_ADDR(base)                  ((base) + 0x184CUL)

/* Register description: 通用外设状态寄存器183。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl183_wc_UNION */
#define SOC_PCTRL_peri_ctrl183_wc_ADDR(base)                  ((base) + 0x1850UL)

/* Register description: 通用外设状态寄存器184。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl184_wc_UNION */
#define SOC_PCTRL_peri_ctrl184_wc_ADDR(base)                  ((base) + 0x1854UL)

/* Register description: 通用外设状态寄存器185（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl185_wc_UNION */
#define SOC_PCTRL_peri_ctrl185_wc_ADDR(base)                  ((base) + 0x1858UL)

/* Register description: 通用外设状态寄存器186（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl186_wc_UNION */
#define SOC_PCTRL_peri_ctrl186_wc_ADDR(base)                  ((base) + 0x185CUL)

/* Register description: 通用外设状态寄存器187（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl187_wc_UNION */
#define SOC_PCTRL_peri_ctrl187_wc_ADDR(base)                  ((base) + 0x1860UL)

/* Register description: 通用外设状态寄存器188（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl188_wc_UNION */
#define SOC_PCTRL_peri_ctrl188_wc_ADDR(base)                  ((base) + 0x1864UL)

/* Register description: 通用外设状态寄存器189（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl189_wc_UNION */
#define SOC_PCTRL_peri_ctrl189_wc_ADDR(base)                  ((base) + 0x1868UL)

/* Register description: 通用外设状态寄存器190-qic_pwrd。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl190_qic_pwrd_UNION */
#define SOC_PCTRL_peri_ctrl190_qic_pwrd_ADDR(base)            ((base) + 0x186CUL)

/* Register description: 通用外设状态寄存器191-mtcmos。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl191_mtcmos_UNION */
#define SOC_PCTRL_peri_ctrl191_mtcmos_ADDR(base)              ((base) + 0x1870UL)

/* Register description: 通用外设状态寄存器192。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl192_wc_UNION */
#define SOC_PCTRL_peri_ctrl192_wc_ADDR(base)                  ((base) + 0x1874UL)

/* Register description: 通用外设状态寄存器193。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl193_wc_UNION */
#define SOC_PCTRL_peri_ctrl193_wc_ADDR(base)                  ((base) + 0x1878UL)

/* Register description: 通用外设状态寄存器194。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl194_wc_UNION */
#define SOC_PCTRL_peri_ctrl194_wc_ADDR(base)                  ((base) + 0x187CUL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl0_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl0_wc_ADDR(base)                 ((base) + 0x1880UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl1_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl1_wc_ADDR(base)                 ((base) + 0x1884UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl2_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl2_wc_ADDR(base)                 ((base) + 0x1888UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl3_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl3_wc_ADDR(base)                 ((base) + 0x188CUL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl4_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl4_wc_ADDR(base)                 ((base) + 0x1890UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl5_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl5_wc_ADDR(base)                 ((base) + 0x1894UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl6_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl6_wc_ADDR(base)                 ((base) + 0x1898UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl7_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl7_wc_ADDR(base)                 ((base) + 0x189CUL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl8_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl8_wc_ADDR(base)                 ((base) + 0x18A0UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl9_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl9_wc_ADDR(base)                 ((base) + 0x18A4UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl10_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl10_wc_ADDR(base)                ((base) + 0x18A8UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl11_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl11_wc_ADDR(base)                ((base) + 0x18ACUL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl12_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl12_wc_ADDR(base)                ((base) + 0x18B0UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl13_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl13_wc_ADDR(base)                ((base) + 0x18B4UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl14_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl14_wc_ADDR(base)                ((base) + 0x18B8UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl15_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl15_wc_ADDR(base)                ((base) + 0x18BCUL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl16_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl16_wc_ADDR(base)                ((base) + 0x18C0UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl17_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl17_wc_ADDR(base)                ((base) + 0x18C4UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl18_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl18_wc_ADDR(base)                ((base) + 0x18C8UL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl19_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl19_wc_ADDR(base)                ((base) + 0x18CCUL)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl20_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl20_wc_ADDR(base)                ((base) + 0x18D0UL)

/* Register description: 通用外设状态寄存器-QIC intr组合逻辑后锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl21_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl21_wc_ADDR(base)                ((base) + 0x18D4UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl22_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl22_wc_ADDR(base)                ((base) + 0x18D8UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl23_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl23_wc_ADDR(base)                ((base) + 0x18DCUL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl24_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl24_wc_ADDR(base)                ((base) + 0x18E0UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl25_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl25_wc_ADDR(base)                ((base) + 0x18E4UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl26_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl26_wc_ADDR(base)                ((base) + 0x18E8UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl27_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl27_wc_ADDR(base)                ((base) + 0x18ECUL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl28_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl28_wc_ADDR(base)                ((base) + 0x18F0UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl29_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl29_wc_ADDR(base)                ((base) + 0x18F4UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl30_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl30_wc_ADDR(base)                ((base) + 0x18F8UL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl31_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl31_wc_ADDR(base)                ((base) + 0x18FCUL)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl32_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl32_wc_ADDR(base)                ((base) + 0x1900UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl33_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl33_wc_ADDR(base)                ((base) + 0x1904UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl34_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl34_wc_ADDR(base)                ((base) + 0x1908UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl35_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl35_wc_ADDR(base)                ((base) + 0x190CUL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl36_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl36_wc_ADDR(base)                ((base) + 0x1910UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl37_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl37_wc_ADDR(base)                ((base) + 0x1914UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl38_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl38_wc_ADDR(base)                ((base) + 0x1918UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl39_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl39_wc_ADDR(base)                ((base) + 0x191CUL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl40_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl40_wc_ADDR(base)                ((base) + 0x1920UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl41_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl41_wc_ADDR(base)                ((base) + 0x1924UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl42_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl42_wc_ADDR(base)                ((base) + 0x1928UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl43_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl43_wc_ADDR(base)                ((base) + 0x192CUL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl44_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl44_wc_ADDR(base)                ((base) + 0x1930UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl45_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl45_wc_ADDR(base)                ((base) + 0x1934UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl46_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl46_wc_ADDR(base)                ((base) + 0x1938UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl47_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl47_wc_ADDR(base)                ((base) + 0x193CUL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl48_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl48_wc_ADDR(base)                ((base) + 0x1940UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl49_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl49_wc_ADDR(base)                ((base) + 0x1944UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl50_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl50_wc_ADDR(base)                ((base) + 0x1948UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl51_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl51_wc_ADDR(base)                ((base) + 0x194CUL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl52_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl52_wc_ADDR(base)                ((base) + 0x1950UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl53_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl53_wc_ADDR(base)                ((base) + 0x1954UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl54_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl54_wc_ADDR(base)                ((base) + 0x1958UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl55_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl55_wc_ADDR(base)                ((base) + 0x195CUL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl56_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl56_wc_ADDR(base)                ((base) + 0x1960UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl57_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl57_wc_ADDR(base)                ((base) + 0x1964UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl58_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl58_wc_ADDR(base)                ((base) + 0x1968UL)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl59_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl59_wc_ADDR(base)                ((base) + 0x196CUL)

/* Register description: 通用外设控制寄存器5_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL5_UNION */
#define SOC_PCTRL_PERI_CTRL5_ADDR(base)                       ((base) + 0x1C00UL)

/* Register description: 通用外设控制寄存器6_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL6_UNION */
#define SOC_PCTRL_PERI_CTRL6_ADDR(base)                       ((base) + 0x1C04UL)

/* Register description: 通用外设控制寄存器7_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL7_UNION */
#define SOC_PCTRL_PERI_CTRL7_ADDR(base)                       ((base) + 0x1C08UL)

/* Register description: 通用外设控制寄存器8_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL8_UNION */
#define SOC_PCTRL_PERI_CTRL8_ADDR(base)                       ((base) + 0x1C0CUL)

/* Register description: 通用外设控制寄存器9_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL9_UNION */
#define SOC_PCTRL_PERI_CTRL9_ADDR(base)                       ((base) + 0x1C10UL)

/* Register description: 通用外设控制寄存器10_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL10_UNION */
#define SOC_PCTRL_PERI_CTRL10_ADDR(base)                      ((base) + 0x1C14UL)

/* Register description: 通用外设控制寄存器11。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL11_UNION */
#define SOC_PCTRL_PERI_CTRL11_ADDR(base)                      ((base) + 0x1C18UL)

/* Register description: 通用外设控制寄存器34_安全区。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL34_UNION */
#define SOC_PCTRL_PERI_CTRL34_ADDR(base)                      ((base) + 0x1C1CUL)

/* Register description: 通用外设控制寄存器35。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL35_UNION */
#define SOC_PCTRL_PERI_CTRL35_ADDR(base)                      ((base) + 0x1C20UL)

/* Register description: 通用外设控制寄存器38。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL38_UNION */
#define SOC_PCTRL_PERI_CTRL38_ADDR(base)                      ((base) + 0x1C2CUL)

/* Register description: 通用外设控制寄存器39。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL39_UNION */
#define SOC_PCTRL_PERI_CTRL39_ADDR(base)                      ((base) + 0x1C30UL)

/* Register description: 通用外设控制寄存器40。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL40_UNION */
#define SOC_PCTRL_PERI_CTRL40_ADDR(base)                      ((base) + 0x1C34UL)

/* Register description: 通用外设控制寄存器41。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL41_UNION */
#define SOC_PCTRL_PERI_CTRL41_ADDR(base)                      ((base) + 0x1C38UL)

/* Register description: 通用外设控制寄存器42_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL42_UNION */
#define SOC_PCTRL_PERI_CTRL42_ADDR(base)                      ((base) + 0x1C3CUL)

/* Register description: 通用外设控制寄存器43_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL43_UNION */
#define SOC_PCTRL_PERI_CTRL43_ADDR(base)                      ((base) + 0x1C40UL)

/* Register description: 通用外设控制寄存器44。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL44_UNION */
#define SOC_PCTRL_PERI_CTRL44_ADDR(base)                      ((base) + 0x1C44UL)

/* Register description: 通用外设控制寄存器45_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL45_UNION */
#define SOC_PCTRL_PERI_CTRL45_ADDR(base)                      ((base) + 0x1C48UL)

/* Register description: 通用外设控制寄存器46_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL46_UNION */
#define SOC_PCTRL_PERI_CTRL46_ADDR(base)                      ((base) + 0x1C4CUL)

/* Register description: 通用外设控制寄存器47_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL47_UNION */
#define SOC_PCTRL_PERI_CTRL47_ADDR(base)                      ((base) + 0x1C50UL)

/* Register description: 通用外设控制寄存器48_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL48_UNION */
#define SOC_PCTRL_PERI_CTRL48_ADDR(base)                      ((base) + 0x1C54UL)

/* Register description: 通用外设控制寄存器49。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL49_UNION */
#define SOC_PCTRL_PERI_CTRL49_ADDR(base)                      ((base) + 0x1C58UL)

/* Register description: 通用外设控制寄存器50。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL50_UNION */
#define SOC_PCTRL_PERI_CTRL50_ADDR(base)                      ((base) + 0x1C5CUL)

/* Register description: 通用外设控制寄存器51。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL51_UNION */
#define SOC_PCTRL_PERI_CTRL51_ADDR(base)                      ((base) + 0x1C60UL)

/* Register description: 通用外设控制寄存器52。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL52_UNION */
#define SOC_PCTRL_PERI_CTRL52_ADDR(base)                      ((base) + 0x1C64UL)

/* Register description: 通用外设控制寄存器53。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL53_UNION */
#define SOC_PCTRL_PERI_CTRL53_ADDR(base)                      ((base) + 0x1C68UL)

/* Register description: 通用外设控制寄存器54。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL54_UNION */
#define SOC_PCTRL_PERI_CTRL54_ADDR(base)                      ((base) + 0x1C6CUL)

/* Register description: 通用外设控制寄存器55。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL55_UNION */
#define SOC_PCTRL_PERI_CTRL55_ADDR(base)                      ((base) + 0x1C70UL)

/* Register description: 通用外设控制寄存器56。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL56_UNION */
#define SOC_PCTRL_PERI_CTRL56_ADDR(base)                      ((base) + 0x1C74UL)

/* Register description: 通用外设控制寄存器57。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL57_UNION */
#define SOC_PCTRL_PERI_CTRL57_ADDR(base)                      ((base) + 0x1C78UL)

/* Register description: 通用外设控制寄存器58。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL58_UNION */
#define SOC_PCTRL_PERI_CTRL58_ADDR(base)                      ((base) + 0x1C7CUL)

/* Register description: 通用外设控制寄存器59。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL59_UNION */
#define SOC_PCTRL_PERI_CTRL59_ADDR(base)                      ((base) + 0x1C80UL)

/* Register description: 通用外设控制寄存器60。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL60_UNION */
#define SOC_PCTRL_PERI_CTRL60_ADDR(base)                      ((base) + 0x1C84UL)

/* Register description: 通用外设控制寄存器61。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL61_UNION */
#define SOC_PCTRL_PERI_CTRL61_ADDR(base)                      ((base) + 0x1C88UL)

/* Register description: 通用外设控制寄存器62_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL62_UNION */
#define SOC_PCTRL_PERI_CTRL62_ADDR(base)                      ((base) + 0x1C8CUL)

/* Register description: 通用外设控制寄存器63_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL63_UNION */
#define SOC_PCTRL_PERI_CTRL63_ADDR(base)                      ((base) + 0x1C90UL)

/* Register description: 通用外设控制寄存器64_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL64_UNION */
#define SOC_PCTRL_PERI_CTRL64_ADDR(base)                      ((base) + 0x1C94UL)

/* Register description: 通用外设控制寄存器65_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL65_UNION */
#define SOC_PCTRL_PERI_CTRL65_ADDR(base)                      ((base) + 0x1C98UL)

/* Register description: 通用外设控制寄存器66_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL66_UNION */
#define SOC_PCTRL_PERI_CTRL66_ADDR(base)                      ((base) + 0x1C9CUL)

/* Register description: 通用外设控制寄存器75_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL75_UNION */
#define SOC_PCTRL_PERI_CTRL75_ADDR(base)                      ((base) + 0x1CA0UL)

/* Register description: 通用外设控制寄存器76_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL76_UNION */
#define SOC_PCTRL_PERI_CTRL76_ADDR(base)                      ((base) + 0x1CA4UL)

/* Register description: 通用外设控制寄存器77_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL77_UNION */
#define SOC_PCTRL_PERI_CTRL77_ADDR(base)                      ((base) + 0x1CA8UL)

/* Register description: 通用外设控制寄存器78_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL78_UNION */
#define SOC_PCTRL_PERI_CTRL78_ADDR(base)                      ((base) + 0x1CACUL)

/* Register description: 通用外设控制寄存器79_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL79_UNION */
#define SOC_PCTRL_PERI_CTRL79_ADDR(base)                      ((base) + 0x1CB0UL)

/* Register description: 通用外设控制寄存器80_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL80_UNION */
#define SOC_PCTRL_PERI_CTRL80_ADDR(base)                      ((base) + 0x1CB4UL)

/* Register description: 通用外设控制寄存器81_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL81_UNION */
#define SOC_PCTRL_PERI_CTRL81_ADDR(base)                      ((base) + 0x1CB8UL)

/* Register description: 通用外设控制寄存器82_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL82_UNION */
#define SOC_PCTRL_PERI_CTRL82_ADDR(base)                      ((base) + 0x1CBCUL)

/* Register description: 通用外设控制寄存器83_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL83_UNION */
#define SOC_PCTRL_PERI_CTRL83_ADDR(base)                      ((base) + 0x1CC0UL)

/* Register description: 通用外设控制寄存器84_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL84_UNION */
#define SOC_PCTRL_PERI_CTRL84_ADDR(base)                      ((base) + 0x1CC4UL)

/* Register description: 通用外设控制寄存器85_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL85_UNION */
#define SOC_PCTRL_PERI_CTRL85_ADDR(base)                      ((base) + 0x1CC8UL)

/* Register description: 通用外设控制寄存器86_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL86_UNION */
#define SOC_PCTRL_PERI_CTRL86_ADDR(base)                      ((base) + 0x1CCCUL)

/* Register description: 通用外设控制寄存器87_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL87_UNION */
#define SOC_PCTRL_PERI_CTRL87_ADDR(base)                      ((base) + 0x1CD0UL)

/* Register description: 通用外设状态寄存器160_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT160_UNION */
#define SOC_PCTRL_PERI_STAT160_ADDR(base)                     ((base) + 0x1CD4UL)

/* Register description: 通用外设控制寄存器141_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL168_UNION */
#define SOC_PCTRL_PERI_CTRL168_ADDR(base)                     ((base) + 0x1CD8UL)

/* Register description: 通用外设控制寄存器142_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL169_UNION */
#define SOC_PCTRL_PERI_CTRL169_ADDR(base)                     ((base) + 0x1CDCUL)

/* Register description: 通用外设控制寄存器143_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL170_UNION */
#define SOC_PCTRL_PERI_CTRL170_ADDR(base)                     ((base) + 0x1CE0UL)

/* Register description: 通用外设控制寄存器144_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL171_UNION */
#define SOC_PCTRL_PERI_CTRL171_ADDR(base)                     ((base) + 0x1CE4UL)

/* Register description: 通用外设控制寄存器145_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL172_UNION */
#define SOC_PCTRL_PERI_CTRL172_ADDR(base)                     ((base) + 0x1CE8UL)

/* Register description: 通用外设控制寄存器146_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL173_UNION */
#define SOC_PCTRL_PERI_CTRL173_ADDR(base)                     ((base) + 0x1CECUL)

/* Register description: 通用外设控制寄存器147_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL174_UNION */
#define SOC_PCTRL_PERI_CTRL174_ADDR(base)                     ((base) + 0x1CF0UL)

/* Register description: 通用外设控制寄存器148_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL175_UNION */
#define SOC_PCTRL_PERI_CTRL175_ADDR(base)                     ((base) + 0x1CF8UL)

/* Register description: 通用外设控制寄存器149_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL176_UNION */
#define SOC_PCTRL_PERI_CTRL176_ADDR(base)                     ((base) + 0x1CFCUL)

/* Register description: 通用外设控制寄存器150_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL177_UNION */
#define SOC_PCTRL_PERI_CTRL177_ADDR(base)                     ((base) + 0x1D00UL)

/* Register description: 通用外设控制寄存器151_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL178_UNION */
#define SOC_PCTRL_PERI_CTRL178_ADDR(base)                     ((base) + 0x1D04UL)

/* Register description: 通用外设控制寄存器152_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL179_UNION */
#define SOC_PCTRL_PERI_CTRL179_ADDR(base)                     ((base) + 0x1D08UL)

/* Register description: 通用外设控制寄存器180_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL180_UNION */
#define SOC_PCTRL_PERI_CTRL180_ADDR(base)                     ((base) + 0x1D0CUL)

/* Register description: 通用外设控制寄存器181_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL181_UNION */
#define SOC_PCTRL_PERI_CTRL181_ADDR(base)                     ((base) + 0x1D10UL)

/* Register description: 通用外设控制寄存器182_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL182_UNION */
#define SOC_PCTRL_PERI_CTRL182_ADDR(base)                     ((base) + 0x1D14UL)

/* Register description: 通用外设控制寄存器183_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL183_UNION */
#define SOC_PCTRL_PERI_CTRL183_ADDR(base)                     ((base) + 0x1D18UL)

/* Register description: 通用外设控制寄存器184_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL184_UNION */
#define SOC_PCTRL_PERI_CTRL184_ADDR(base)                     ((base) + 0x1D1CUL)

/* Register description: 通用外设控制寄存器185_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL185_UNION */
#define SOC_PCTRL_PERI_CTRL185_ADDR(base)                     ((base) + 0x1D20UL)

/* Register description: 通用外设控制寄存器186_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL186_UNION */
#define SOC_PCTRL_PERI_CTRL186_ADDR(base)                     ((base) + 0x1D24UL)

/* Register description: 通用外设控制寄存器187_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL187_UNION */
#define SOC_PCTRL_PERI_CTRL187_ADDR(base)                     ((base) + 0x1D28UL)

/* Register description: 通用外设控制寄存器188_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL188_UNION */
#define SOC_PCTRL_PERI_CTRL188_ADDR(base)                     ((base) + 0x1D2CUL)

/* Register description: 通用外设控制寄存器189_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL189_UNION */
#define SOC_PCTRL_PERI_CTRL189_ADDR(base)                     ((base) + 0x1D30UL)

/* Register description: 通用外设控制寄存器190_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL190_UNION */
#define SOC_PCTRL_PERI_CTRL190_ADDR(base)                     ((base) + 0x1D34UL)

/* Register description: 通用外设控制寄存器191_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL191_UNION */
#define SOC_PCTRL_PERI_CTRL191_ADDR(base)                     ((base) + 0x1D38UL)

/* Register description: 通用外设控制寄存器192_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL192_UNION */
#define SOC_PCTRL_PERI_CTRL192_ADDR(base)                     ((base) + 0x1D3CUL)

/* Register description: 通用外设控制寄存器193_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL193_UNION */
#define SOC_PCTRL_PERI_CTRL193_ADDR(base)                     ((base) + 0x1D40UL)

/* Register description: 通用外设控制寄存器194_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL194_UNION */
#define SOC_PCTRL_PERI_CTRL194_ADDR(base)                     ((base) + 0x1D44UL)

/* Register description: 通用外设控制寄存器195_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL195_UNION */
#define SOC_PCTRL_PERI_CTRL195_ADDR(base)                     ((base) + 0x1D48UL)

/* Register description: 通用外设控制寄存器196_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL196_UNION */
#define SOC_PCTRL_PERI_CTRL196_ADDR(base)                     ((base) + 0x1D4CUL)

/* Register description: 通用外设控制寄存器197_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL197_UNION */
#define SOC_PCTRL_PERI_CTRL197_ADDR(base)                     ((base) + 0x1D50UL)

/* Register description: 通用外设控制寄存器198_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL198_UNION */
#define SOC_PCTRL_PERI_CTRL198_ADDR(base)                     ((base) + 0x1D54UL)

/* Register description: 通用外设控制寄存器199_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL199_UNION */
#define SOC_PCTRL_PERI_CTRL199_ADDR(base)                     ((base) + 0x1D58UL)

/* Register description: 通用外设控制寄存器200_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL200_UNION */
#define SOC_PCTRL_PERI_CTRL200_ADDR(base)                     ((base) + 0x1D5CUL)

/* Register description: 通用外设控制寄存器201_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL201_UNION */
#define SOC_PCTRL_PERI_CTRL201_ADDR(base)                     ((base) + 0x1D60UL)

/* Register description: 通用外设控制寄存器202_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL202_UNION */
#define SOC_PCTRL_PERI_CTRL202_ADDR(base)                     ((base) + 0x1D64UL)

/* Register description: 通用外设控制寄存器203_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL203_UNION */
#define SOC_PCTRL_PERI_CTRL203_ADDR(base)                     ((base) + 0x1D68UL)

/* Register description: 通用外设控制寄存器204_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL204_UNION */
#define SOC_PCTRL_PERI_CTRL204_ADDR(base)                     ((base) + 0x1D6CUL)

/* Register description: 通用外设控制寄存器205_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL205_UNION */
#define SOC_PCTRL_PERI_CTRL205_ADDR(base)                     ((base) + 0x1D70UL)

/* Register description: 通用外设控制寄存器206_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL206_UNION */
#define SOC_PCTRL_PERI_CTRL206_ADDR(base)                     ((base) + 0x1D74UL)

/* Register description: 通用外设控制寄存器207_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL207_UNION */
#define SOC_PCTRL_PERI_CTRL207_ADDR(base)                     ((base) + 0x1D78UL)

/* Register description: 通用外设控制寄存器208_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL208_UNION */
#define SOC_PCTRL_PERI_CTRL208_ADDR(base)                     ((base) + 0x1D7CUL)

/* Register description: 通用外设控制寄存器209_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL209_UNION */
#define SOC_PCTRL_PERI_CTRL209_ADDR(base)                     ((base) + 0x1D80UL)

/* Register description: 通用外设控制寄存器210_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL210_UNION */
#define SOC_PCTRL_PERI_CTRL210_ADDR(base)                     ((base) + 0x1D84UL)

/* Register description: 通用外设控制寄存器211_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL211_UNION */
#define SOC_PCTRL_PERI_CTRL211_ADDR(base)                     ((base) + 0x1D88UL)

/* Register description: 通用外设控制寄存器212_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL212_UNION */
#define SOC_PCTRL_PERI_CTRL212_ADDR(base)                     ((base) + 0x1D8CUL)

/* Register description: 通用外设控制寄存器213_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL213_UNION */
#define SOC_PCTRL_PERI_CTRL213_ADDR(base)                     ((base) + 0x1D90UL)

/* Register description: 通用外设控制寄存器214_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL214_UNION */
#define SOC_PCTRL_PERI_CTRL214_ADDR(base)                     ((base) + 0x1D94UL)

/* Register description: 通用外设控制寄存器215_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL215_UNION */
#define SOC_PCTRL_PERI_CTRL215_ADDR(base)                     ((base) + 0x1D98UL)

/* Register description: 通用外设控制寄存器216_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL216_UNION */
#define SOC_PCTRL_PERI_CTRL216_ADDR(base)                     ((base) + 0x1D9CUL)

/* Register description: 通用外设控制寄存器217_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL217_UNION */
#define SOC_PCTRL_PERI_CTRL217_ADDR(base)                     ((base) + 0x1DA0UL)

/* Register description: 通用外设控制寄存器218_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL218_UNION */
#define SOC_PCTRL_PERI_CTRL218_ADDR(base)                     ((base) + 0x1DA4UL)

/* Register description: 通用外设控制寄存器219_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL219_UNION */
#define SOC_PCTRL_PERI_CTRL219_ADDR(base)                     ((base) + 0x1DA8UL)

/* Register description: 通用外设控制寄存器220_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL220_UNION */
#define SOC_PCTRL_PERI_CTRL220_ADDR(base)                     ((base) + 0x1DACUL)

/* Register description: 通用外设控制寄存器221_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL221_UNION */
#define SOC_PCTRL_PERI_CTRL221_ADDR(base)                     ((base) + 0x1DB0UL)

/* Register description: 通用外设控制寄存器222_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL222_UNION */
#define SOC_PCTRL_PERI_CTRL222_ADDR(base)                     ((base) + 0x1DB4UL)

/* Register description: 通用外设控制寄存器223_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL223_UNION */
#define SOC_PCTRL_PERI_CTRL223_ADDR(base)                     ((base) + 0x1DB8UL)

/* Register description: 通用外设控制寄存器224_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL224_UNION */
#define SOC_PCTRL_PERI_CTRL224_ADDR(base)                     ((base) + 0x1DBCUL)

/* Register description: 通用外设控制寄存器225_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL225_UNION */
#define SOC_PCTRL_PERI_CTRL225_ADDR(base)                     ((base) + 0x1DC0UL)

/* Register description: 通用外设控制寄存器226_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL226_UNION */
#define SOC_PCTRL_PERI_CTRL226_ADDR(base)                     ((base) + 0x1DC4UL)

/* Register description: 通用外设控制寄存器227_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL227_UNION */
#define SOC_PCTRL_PERI_CTRL227_ADDR(base)                     ((base) + 0x1DC8UL)

/* Register description: 通用外设控制寄存器228_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL228_UNION */
#define SOC_PCTRL_PERI_CTRL228_ADDR(base)                     ((base) + 0x1DCCUL)

/* Register description: 通用外设控制寄存器229_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL229_UNION */
#define SOC_PCTRL_PERI_CTRL229_ADDR(base)                     ((base) + 0x1DD0UL)

/* Register description: 通用外设控制寄存器230_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL230_UNION */
#define SOC_PCTRL_PERI_CTRL230_ADDR(base)                     ((base) + 0x1DD4UL)

/* Register description: 通用外设控制寄存器231_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL231_UNION */
#define SOC_PCTRL_PERI_CTRL231_ADDR(base)                     ((base) + 0x1DD8UL)

/* Register description: 通用外设控制寄存器232_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL232_UNION */
#define SOC_PCTRL_PERI_CTRL232_ADDR(base)                     ((base) + 0x1DDCUL)

/* Register description: 通用外设控制寄存器233_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL233_UNION */
#define SOC_PCTRL_PERI_CTRL233_ADDR(base)                     ((base) + 0x1DE0UL)

/* Register description: 通用外设控制寄存器234_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL234_UNION */
#define SOC_PCTRL_PERI_CTRL234_ADDR(base)                     ((base) + 0x1DE4UL)

/* Register description: 通用外设控制寄存器235_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL235_UNION */
#define SOC_PCTRL_PERI_CTRL235_ADDR(base)                     ((base) + 0x1DE8UL)

/* Register description: 通用外设控制寄存器236_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL236_UNION */
#define SOC_PCTRL_PERI_CTRL236_ADDR(base)                     ((base) + 0x1DECUL)

/* Register description: 通用外设控制寄存器237_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL237_UNION */
#define SOC_PCTRL_PERI_CTRL237_ADDR(base)                     ((base) + 0x1DF0UL)

/* Register description: 通用外设控制寄存器238_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL238_UNION */
#define SOC_PCTRL_PERI_CTRL238_ADDR(base)                     ((base) + 0x1DF4UL)

/* Register description: 通用外设控制寄存器239_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL239_UNION */
#define SOC_PCTRL_PERI_CTRL239_ADDR(base)                     ((base) + 0x1DF8UL)

/* Register description: 通用外设控制寄存器240_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL240_UNION */
#define SOC_PCTRL_PERI_CTRL240_ADDR(base)                     ((base) + 0x1DFCUL)

/* Register description: 通用外设控制寄存器241_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL241_UNION */
#define SOC_PCTRL_PERI_CTRL241_ADDR(base)                     ((base) + 0x1E00UL)

/* Register description: 通用外设控制寄存器242_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL242_UNION */
#define SOC_PCTRL_PERI_CTRL242_ADDR(base)                     ((base) + 0x1E04UL)

/* Register description: 通用外设控制寄存器243_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL243_UNION */
#define SOC_PCTRL_PERI_CTRL243_ADDR(base)                     ((base) + 0x1E08UL)

/* Register description: 通用外设控制寄存器244_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL244_UNION */
#define SOC_PCTRL_PERI_CTRL244_ADDR(base)                     ((base) + 0x1E0CUL)

/* Register description: 通用外设控制寄存器245_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL245_UNION */
#define SOC_PCTRL_PERI_CTRL245_ADDR(base)                     ((base) + 0x1E10UL)

/* Register description: 通用外设控制寄存器246_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL246_UNION */
#define SOC_PCTRL_PERI_CTRL246_ADDR(base)                     ((base) + 0x1E14UL)

/* Register description: 通用外设控制寄存器247_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL247_UNION */
#define SOC_PCTRL_PERI_CTRL247_ADDR(base)                     ((base) + 0x1E18UL)

/* Register description: 通用外设控制寄存器248_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL248_UNION */
#define SOC_PCTRL_PERI_CTRL248_ADDR(base)                     ((base) + 0x1E1CUL)

/* Register description: 通用外设控制寄存器249_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL249_UNION */
#define SOC_PCTRL_PERI_CTRL249_ADDR(base)                     ((base) + 0x1E20UL)

/* Register description: 通用外设回读寄存器0_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT_SEC0_UNION */
#define SOC_PCTRL_PERI_STAT_SEC0_ADDR(base)                   ((base) + 0x1E24UL)


#else


/* Register description: G3D光栅控制寄存器。
   Bit domain definition UNION:  SOC_PCTRL_G3D_RASTER_UNION */
#define SOC_PCTRL_G3D_RASTER_ADDR(base)                       (base)

/* Register description: USB2 HOST控制寄存器0。
   Bit domain definition UNION:  SOC_PCTRL_USB2_HOST_CTRL0_UNION */
#define SOC_PCTRL_USB2_HOST_CTRL0_ADDR(base)                  ((base) + 0x0004)

/* Register description: USB2 HOST控制寄存器1。
   Bit domain definition UNION:  SOC_PCTRL_USB2_HOST_CTRL1_UNION */
#define SOC_PCTRL_USB2_HOST_CTRL1_ADDR(base)                  ((base) + 0x0008)

/* Register description: USB2 HOST控制寄存器2。
   Bit domain definition UNION:  SOC_PCTRL_USB2_HOST_CTRL2_UNION */
#define SOC_PCTRL_USB2_HOST_CTRL2_ADDR(base)                  ((base) + 0x000C)

/* Register description: 通用外设控制寄存器4。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL4_UNION */
#define SOC_PCTRL_PERI_CTRL4_ADDR(base)                       ((base) + 0x0010)

/* Register description: 通用外设控制寄存器98。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL98_UNION */
#define SOC_PCTRL_PERI_CTRL98_ADDR(base)                      ((base) + 0x0014)

/* Register description: 通用外设控制寄存器99。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL99_UNION */
#define SOC_PCTRL_PERI_CTRL99_ADDR(base)                      ((base) + 0x0018)

/* Register description: 通用外设控制寄存器100。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL100_UNION */
#define SOC_PCTRL_PERI_CTRL100_ADDR(base)                     ((base) + 0x001C)

/* Register description: 通用外设控制寄存器101。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL101_UNION */
#define SOC_PCTRL_PERI_CTRL101_ADDR(base)                     ((base) + 0x0020)

/* Register description: 通用外设控制寄存器104。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL104_UNION */
#define SOC_PCTRL_PERI_CTRL104_ADDR(base)                     ((base) + 0x0024)

/* Register description: 通用外设控制寄存器12。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL12_UNION */
#define SOC_PCTRL_PERI_CTRL12_ADDR(base)                      ((base) + 0x0028)

/* Register description: 通用外设控制寄存器13。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL13_UNION */
#define SOC_PCTRL_PERI_CTRL13_ADDR(base)                      ((base) + 0x002C)

/* Register description: 通用外设控制寄存器14。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL14_UNION */
#define SOC_PCTRL_PERI_CTRL14_ADDR(base)                      ((base) + 0x0030)

/* Register description: 通用外设控制寄存器15。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL15_UNION */
#define SOC_PCTRL_PERI_CTRL15_ADDR(base)                      ((base) + 0x0034)

/* Register description: 通用外设控制寄存器16。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL16_UNION */
#define SOC_PCTRL_PERI_CTRL16_ADDR(base)                      ((base) + 0x0038)

/* Register description: 通用外设控制寄存器17。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL17_UNION */
#define SOC_PCTRL_PERI_CTRL17_ADDR(base)                      ((base) + 0x003C)

/* Register description: 通用外设控制寄存器18。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL18_UNION */
#define SOC_PCTRL_PERI_CTRL18_ADDR(base)                      ((base) + 0x0040)

/* Register description: 通用外设控制寄存器19。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL19_UNION */
#define SOC_PCTRL_PERI_CTRL19_ADDR(base)                      ((base) + 0x0044)

/* Register description: 通用外设控制寄存器20。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL20_UNION */
#define SOC_PCTRL_PERI_CTRL20_ADDR(base)                      ((base) + 0x0048)

/* Register description: 通用外设控制寄存器21。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL21_UNION */
#define SOC_PCTRL_PERI_CTRL21_ADDR(base)                      ((base) + 0x004C)

/* Register description: 通用外设控制寄存器22。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL22_UNION */
#define SOC_PCTRL_PERI_CTRL22_ADDR(base)                      ((base) + 0x0050)

/* Register description: 通用外设控制寄存器23。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL23_UNION */
#define SOC_PCTRL_PERI_CTRL23_ADDR(base)                      ((base) + 0x0054)

/* Register description: 通用外设控制寄存器24。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL24_UNION */
#define SOC_PCTRL_PERI_CTRL24_ADDR(base)                      ((base) + 0x0058)

/* Register description: 通用外设控制寄存器25。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL25_UNION */
#define SOC_PCTRL_PERI_CTRL25_ADDR(base)                      ((base) + 0x005C)

/* Register description: 通用外设控制寄存器26。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL26_UNION */
#define SOC_PCTRL_PERI_CTRL26_ADDR(base)                      ((base) + 0x0060)

/* Register description: 通用外设控制寄存器27。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL27_UNION */
#define SOC_PCTRL_PERI_CTRL27_ADDR(base)                      ((base) + 0x0064)

/* Register description: 通用外设控制寄存器28。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL28_UNION */
#define SOC_PCTRL_PERI_CTRL28_ADDR(base)                      ((base) + 0x0068)

/* Register description: 通用外设控制寄存器29。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL29_UNION */
#define SOC_PCTRL_PERI_CTRL29_ADDR(base)                      ((base) + 0x006C)

/* Register description: 通用外设控制寄存器30。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL30_UNION */
#define SOC_PCTRL_PERI_CTRL30_ADDR(base)                      ((base) + 0x0070)

/* Register description: 通用外设控制寄存器31。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL31_UNION */
#define SOC_PCTRL_PERI_CTRL31_ADDR(base)                      ((base) + 0x0074)

/* Register description: 通用外设控制寄存器32。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL32_UNION */
#define SOC_PCTRL_PERI_CTRL32_ADDR(base)                      ((base) + 0x0078)

/* Register description: 通用外设控制寄存器33。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL33_UNION */
#define SOC_PCTRL_PERI_CTRL33_ADDR(base)                      ((base) + 0x007C)

/* Register description: 通用外设控制寄存器67。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL67_UNION */
#define SOC_PCTRL_PERI_CTRL67_ADDR(base)                      ((base) + 0x0080)

/* Register description: 通用外设控制寄存器68。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL68_UNION */
#define SOC_PCTRL_PERI_CTRL68_ADDR(base)                      ((base) + 0x0084)

/* Register description: 通用外设控制寄存器69。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL69_UNION */
#define SOC_PCTRL_PERI_CTRL69_ADDR(base)                      ((base) + 0x0088)

/* Register description: 通用外设控制寄存器70。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL70_UNION */
#define SOC_PCTRL_PERI_CTRL70_ADDR(base)                      ((base) + 0x008C)

/* Register description: 通用外设控制寄存器71。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL71_UNION */
#define SOC_PCTRL_PERI_CTRL71_ADDR(base)                      ((base) + 0x0090)

/* Register description: 通用外设控制寄存器72。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL72_UNION */
#define SOC_PCTRL_PERI_CTRL72_ADDR(base)                      ((base) + 0x0094)

/* Register description: 通用外设控制寄存器73。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL73_UNION */
#define SOC_PCTRL_PERI_CTRL73_ADDR(base)                      ((base) + 0x0098)

/* Register description: 通用外设控制寄存器74。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL74_UNION */
#define SOC_PCTRL_PERI_CTRL74_ADDR(base)                      ((base) + 0x009C)

/* Register description: 通用外设控制寄存器88。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL88_UNION */
#define SOC_PCTRL_PERI_CTRL88_ADDR(base)                      ((base) + 0x00A0)

/* Register description: 通用外设控制寄存器89。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL89_UNION */
#define SOC_PCTRL_PERI_CTRL89_ADDR(base)                      ((base) + 0x00A4)

/* Register description: 通用外设控制寄存器90。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL90_UNION */
#define SOC_PCTRL_PERI_CTRL90_ADDR(base)                      ((base) + 0x00A8)

/* Register description: 通用外设控制寄存器91。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL91_UNION */
#define SOC_PCTRL_PERI_CTRL91_ADDR(base)                      ((base) + 0x00AC)

/* Register description: 通用外设控制寄存器92。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL92_UNION */
#define SOC_PCTRL_PERI_CTRL92_ADDR(base)                      ((base) + 0x00B0)

/* Register description: 通用外设控制寄存器93。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL93_UNION */
#define SOC_PCTRL_PERI_CTRL93_ADDR(base)                      ((base) + 0x00B4)

/* Register description: 通用外设控制寄存器95。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL95_UNION */
#define SOC_PCTRL_PERI_CTRL95_ADDR(base)                      ((base) + 0x00B8)

/* Register description: 通用外设控制寄存器96。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL96_UNION */
#define SOC_PCTRL_PERI_CTRL96_ADDR(base)                      ((base) + 0x00BC)

/* Register description: 通用外设控制寄存器97。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL97_UNION */
#define SOC_PCTRL_PERI_CTRL97_ADDR(base)                      ((base) + 0x00C0)

/* Register description: 通用外设控制寄存器108。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL108_UNION */
#define SOC_PCTRL_PERI_CTRL108_ADDR(base)                     ((base) + 0x00C4)

/* Register description: 通用外设控制寄存器109。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL109_UNION */
#define SOC_PCTRL_PERI_CTRL109_ADDR(base)                     ((base) + 0x00C8)

/* Register description: 通用外设控制寄存器110。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL110_UNION */
#define SOC_PCTRL_PERI_CTRL110_ADDR(base)                     ((base) + 0x00CC)

/* Register description: 通用外设控制寄存器111。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL111_UNION */
#define SOC_PCTRL_PERI_CTRL111_ADDR(base)                     ((base) + 0x00D0)

/* Register description: 通用外设控制寄存器112。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL112_UNION */
#define SOC_PCTRL_PERI_CTRL112_ADDR(base)                     ((base) + 0x00D4)

/* Register description: 通用外设控制寄存器113。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL113_UNION */
#define SOC_PCTRL_PERI_CTRL113_ADDR(base)                     ((base) + 0x00D8)

/* Register description: 通用外设控制寄存器114。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL114_UNION */
#define SOC_PCTRL_PERI_CTRL114_ADDR(base)                     ((base) + 0x00DC)

/* Register description: 通用外设控制寄存器115。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL115_UNION */
#define SOC_PCTRL_PERI_CTRL115_ADDR(base)                     ((base) + 0x00E0)

/* Register description: 通用外设控制寄存器116。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL116_UNION */
#define SOC_PCTRL_PERI_CTRL116_ADDR(base)                     ((base) + 0x00E4)

/* Register description: 通用外设控制寄存器117。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL117_UNION */
#define SOC_PCTRL_PERI_CTRL117_ADDR(base)                     ((base) + 0x00E8)

/* Register description: 通用外设控制寄存器118。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL118_UNION */
#define SOC_PCTRL_PERI_CTRL118_ADDR(base)                     ((base) + 0x00EC)

/* Register description: 通用外设控制寄存器119。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL119_UNION */
#define SOC_PCTRL_PERI_CTRL119_ADDR(base)                     ((base) + 0x00F0)

/* Register description: 通用外设控制寄存器120。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL120_UNION */
#define SOC_PCTRL_PERI_CTRL120_ADDR(base)                     ((base) + 0x00F4)

/* Register description: 通用外设控制寄存器121。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL121_UNION */
#define SOC_PCTRL_PERI_CTRL121_ADDR(base)                     ((base) + 0x00F8)

/* Register description: 通用外设控制寄存器122。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL122_UNION */
#define SOC_PCTRL_PERI_CTRL122_ADDR(base)                     ((base) + 0x00FC)

/* Register description: 通用外设控制寄存器123。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL123_UNION */
#define SOC_PCTRL_PERI_CTRL123_ADDR(base)                     ((base) + 0x0100)

/* Register description: 通用外设控制寄存器124。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL124_UNION */
#define SOC_PCTRL_PERI_CTRL124_ADDR(base)                     ((base) + 0x0104)

/* Register description: 通用外设控制寄存器125。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL125_UNION */
#define SOC_PCTRL_PERI_CTRL125_ADDR(base)                     ((base) + 0x0108)

/* Register description: 通用外设控制寄存器126。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL126_UNION */
#define SOC_PCTRL_PERI_CTRL126_ADDR(base)                     ((base) + 0x010C)

/* Register description: 通用外设控制寄存器127。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL127_UNION */
#define SOC_PCTRL_PERI_CTRL127_ADDR(base)                     ((base) + 0x0110)

/* Register description: 通用外设控制寄存器128。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL128_UNION */
#define SOC_PCTRL_PERI_CTRL128_ADDR(base)                     ((base) + 0x0114)

/* Register description: 通用外设控制寄存器129。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL129_UNION */
#define SOC_PCTRL_PERI_CTRL129_ADDR(base)                     ((base) + 0x0118)

/* Register description: 通用外设控制寄存器130。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL130_UNION */
#define SOC_PCTRL_PERI_CTRL130_ADDR(base)                     ((base) + 0x011C)

/* Register description: 通用外设控制寄存器131。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL131_UNION */
#define SOC_PCTRL_PERI_CTRL131_ADDR(base)                     ((base) + 0x0120)

/* Register description: 通用外设控制寄存器132。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL132_UNION */
#define SOC_PCTRL_PERI_CTRL132_ADDR(base)                     ((base) + 0x0124)

/* Register description: 通用外设控制寄存器133。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL133_UNION */
#define SOC_PCTRL_PERI_CTRL133_ADDR(base)                     ((base) + 0x0128)

/* Register description: 通用外设控制寄存器134。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL134_UNION */
#define SOC_PCTRL_PERI_CTRL134_ADDR(base)                     ((base) + 0x012C)

/* Register description: 通用外设控制寄存器135。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL135_UNION */
#define SOC_PCTRL_PERI_CTRL135_ADDR(base)                     ((base) + 0x0130)

/* Register description: 通用外设控制寄存器136。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL136_UNION */
#define SOC_PCTRL_PERI_CTRL136_ADDR(base)                     ((base) + 0x0134)

/* Register description: 通用外设控制寄存器137。
   Bit domain definition UNION:  SOC_PCTRL_mdm_tsp_nmi0_UNION */
#define SOC_PCTRL_mdm_tsp_nmi0_ADDR(base)                     ((base) + 0x0138)

/* Register description: 通用外设控制寄存器138。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL138_UNION */
#define SOC_PCTRL_PERI_CTRL138_ADDR(base)                     ((base) + 0x013C)

/* Register description: 通用外设控制寄存器139。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL139_UNION */
#define SOC_PCTRL_PERI_CTRL139_ADDR(base)                     ((base) + 0x0140)

/* Register description: 通用外设控制寄存器140。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL140_UNION */
#define SOC_PCTRL_PERI_CTRL140_ADDR(base)                     ((base) + 0x0144)

/* Register description: 通用外设控制寄存器141。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL141_UNION */
#define SOC_PCTRL_PERI_CTRL141_ADDR(base)                     ((base) + 0x0148)

/* Register description: 通用外设控制寄存器142。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL142_UNION */
#define SOC_PCTRL_PERI_CTRL142_ADDR(base)                     ((base) + 0x014C)

/* Register description: 通用外设控制寄存器143。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL143_UNION */
#define SOC_PCTRL_PERI_CTRL143_ADDR(base)                     ((base) + 0x0150)

/* Register description: 通用外设控制寄存器144。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL144_UNION */
#define SOC_PCTRL_PERI_CTRL144_ADDR(base)                     ((base) + 0x0154)

/* Register description: 通用外设控制寄存器145。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL145_UNION */
#define SOC_PCTRL_PERI_CTRL145_ADDR(base)                     ((base) + 0x0158)

/* Register description: 通用外设控制寄存器146。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL146_UNION */
#define SOC_PCTRL_PERI_CTRL146_ADDR(base)                     ((base) + 0x015C)

/* Register description: 通用外设控制寄存器147。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL147_UNION */
#define SOC_PCTRL_PERI_CTRL147_ADDR(base)                     ((base) + 0x0160)

/* Register description: 通用外设控制寄存器148。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL148_UNION */
#define SOC_PCTRL_PERI_CTRL148_ADDR(base)                     ((base) + 0x0164)

/* Register description: 通用外设控制寄存器149。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL149_UNION */
#define SOC_PCTRL_PERI_CTRL149_ADDR(base)                     ((base) + 0x0168)

/* Register description: 通用外设控制寄存器150。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL150_UNION */
#define SOC_PCTRL_PERI_CTRL150_ADDR(base)                     ((base) + 0x016C)

/* Register description: 通用外设控制寄存器151。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL151_UNION */
#define SOC_PCTRL_PERI_CTRL151_ADDR(base)                     ((base) + 0x0170)

/* Register description: 通用外设控制寄存器152。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL152_UNION */
#define SOC_PCTRL_PERI_CTRL152_ADDR(base)                     ((base) + 0x0174)

/* Register description: 通用外设控制寄存器153。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL153_UNION */
#define SOC_PCTRL_PERI_CTRL153_ADDR(base)                     ((base) + 0x0178)

/* Register description: 通用外设控制寄存器154。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL154_UNION */
#define SOC_PCTRL_PERI_CTRL154_ADDR(base)                     ((base) + 0x017C)

/* Register description: 通用外设控制寄存器155。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL155_UNION */
#define SOC_PCTRL_PERI_CTRL155_ADDR(base)                     ((base) + 0x0180)

/* Register description: 通用外设控制寄存器156。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL156_UNION */
#define SOC_PCTRL_PERI_CTRL156_ADDR(base)                     ((base) + 0x0184)

/* Register description: 通用外设控制寄存器157。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL157_UNION */
#define SOC_PCTRL_PERI_CTRL157_ADDR(base)                     ((base) + 0x0188)

/* Register description: 通用外设控制寄存器158。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL158_UNION */
#define SOC_PCTRL_PERI_CTRL158_ADDR(base)                     ((base) + 0x018C)

/* Register description: 通用外设控制寄存器159。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL159_UNION */
#define SOC_PCTRL_PERI_CTRL159_ADDR(base)                     ((base) + 0x0190)

/* Register description: 通用外设控制寄存器160。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL160_UNION */
#define SOC_PCTRL_PERI_CTRL160_ADDR(base)                     ((base) + 0x0194)

/* Register description: 通用外设控制寄存器161。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL161_UNION */
#define SOC_PCTRL_PERI_CTRL161_ADDR(base)                     ((base) + 0x0198)

/* Register description: 通用外设控制寄存器162。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL162_UNION */
#define SOC_PCTRL_PERI_CTRL162_ADDR(base)                     ((base) + 0x019C)

/* Register description: 通用外设控制寄存器163。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL163_UNION */
#define SOC_PCTRL_PERI_CTRL163_ADDR(base)                     ((base) + 0x01A0)

/* Register description: 通用外设控制寄存器164。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL164_UNION */
#define SOC_PCTRL_PERI_CTRL164_ADDR(base)                     ((base) + 0x01A4)

/* Register description: 通用外设控制寄存器165。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL165_UNION */
#define SOC_PCTRL_PERI_CTRL165_ADDR(base)                     ((base) + 0x01A8)

/* Register description: 通用外设控制寄存器166。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL166_UNION */
#define SOC_PCTRL_PERI_CTRL166_ADDR(base)                     ((base) + 0x01AC)

/* Register description: 通用外设控制寄存器167。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL167_UNION */
#define SOC_PCTRL_PERI_CTRL167_ADDR(base)                     ((base) + 0x01B0)

/* Register description: 通用外设状态寄存器0。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT0_UNION */
#define SOC_PCTRL_PERI_STAT0_ADDR(base)                       ((base) + 0x1000)

/* Register description: 通用外设状态寄存器1。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT1_UNION */
#define SOC_PCTRL_PERI_STAT1_ADDR(base)                       ((base) + 0x1004)

/* Register description: 通用外设状态寄存器2。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT2_UNION */
#define SOC_PCTRL_PERI_STAT2_ADDR(base)                       ((base) + 0x1008)

/* Register description: 通用外设状态寄存器3。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT3_UNION */
#define SOC_PCTRL_PERI_STAT3_ADDR(base)                       ((base) + 0x100C)

/* Register description: 通用外设状态寄存器4。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT4_UNION */
#define SOC_PCTRL_PERI_STAT4_ADDR(base)                       ((base) + 0x1010)

/* Register description: 通用外设状态寄存器5。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT5_UNION */
#define SOC_PCTRL_PERI_STAT5_ADDR(base)                       ((base) + 0x1014)

/* Register description: 通用外设状态寄存器6。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT6_UNION */
#define SOC_PCTRL_PERI_STAT6_ADDR(base)                       ((base) + 0x1018)

/* Register description: 通用外设状态寄存器7--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT7_UNION */
#define SOC_PCTRL_PERI_STAT7_ADDR(base)                       ((base) + 0x101C)

/* Register description: 通用外设状态寄存器8--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT8_UNION */
#define SOC_PCTRL_PERI_STAT8_ADDR(base)                       ((base) + 0x1020)

/* Register description: 通用外设状态寄存器9--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT9_UNION */
#define SOC_PCTRL_PERI_STAT9_ADDR(base)                       ((base) + 0x1024)

/* Register description: 通用外设状态寄存器10--QIC busy回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT10_UNION */
#define SOC_PCTRL_PERI_STAT10_ADDR(base)                      ((base) + 0x1028)

/* Register description: 通用外设状态寄存器11。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT11_UNION */
#define SOC_PCTRL_PERI_STAT11_ADDR(base)                      ((base) + 0x102C)

/* Register description: 通用外设状态寄存器12。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT12_UNION */
#define SOC_PCTRL_PERI_STAT12_ADDR(base)                      ((base) + 0x1030)

/* Register description: 通用外设状态寄存器13。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT13_UNION */
#define SOC_PCTRL_PERI_STAT13_ADDR(base)                      ((base) + 0x1034)

/* Register description: 通用外设状态寄存器14。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT14_UNION */
#define SOC_PCTRL_PERI_STAT14_ADDR(base)                      ((base) + 0x1038)

/* Register description: 通用外设状态寄存器15。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT15_UNION */
#define SOC_PCTRL_PERI_STAT15_ADDR(base)                      ((base) + 0x103C)

/* Register description: 通用外设状态寄存器16。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT16_UNION */
#define SOC_PCTRL_PERI_STAT16_ADDR(base)                      ((base) + 0x1040)

/* Register description: 通用外设状态寄存器17。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT17_UNION */
#define SOC_PCTRL_PERI_STAT17_ADDR(base)                      ((base) + 0x1044)

/* Register description: 通用外设状态寄存器18。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT18_UNION */
#define SOC_PCTRL_PERI_STAT18_ADDR(base)                      ((base) + 0x1048)

/* Register description: 通用外设状态寄存器19。--QIC busy回读
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT19_UNION */
#define SOC_PCTRL_PERI_STAT19_ADDR(base)                      ((base) + 0x104C)

/* Register description: 通用外设状态寄存器29。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT29_UNION */
#define SOC_PCTRL_PERI_STAT29_ADDR(base)                      ((base) + 0x1050)

/* Register description: 通用外设状态寄存器30。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT30_UNION */
#define SOC_PCTRL_PERI_STAT30_ADDR(base)                      ((base) + 0x1054)

/* Register description: 通用外设状态寄存器31。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT31_UNION */
#define SOC_PCTRL_PERI_STAT31_ADDR(base)                      ((base) + 0x1058)

/* Register description: 通用外设状态寄存器32。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT32_UNION */
#define SOC_PCTRL_PERI_STAT32_ADDR(base)                      ((base) + 0x105C)

/* Register description: 通用外设状态寄存器33。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT33_UNION */
#define SOC_PCTRL_PERI_STAT33_ADDR(base)                      ((base) + 0x1060)

/* Register description: 通用外设状态寄存器34。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT34_UNION */
#define SOC_PCTRL_PERI_STAT34_ADDR(base)                      ((base) + 0x1064)

/* Register description: 通用外设状态寄存器35。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT35_UNION */
#define SOC_PCTRL_PERI_STAT35_ADDR(base)                      ((base) + 0x1068)

/* Register description: 通用外设状态寄存器36。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT36_UNION */
#define SOC_PCTRL_PERI_STAT36_ADDR(base)                      ((base) + 0x106C)

/* Register description: 通用外设状态寄存器37。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT37_UNION */
#define SOC_PCTRL_PERI_STAT37_ADDR(base)                      ((base) + 0x1070)

/* Register description: 通用外设状态寄存器38。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT38_UNION */
#define SOC_PCTRL_PERI_STAT38_ADDR(base)                      ((base) + 0x1074)

/* Register description: 通用外设状态寄存器39。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT39_UNION */
#define SOC_PCTRL_PERI_STAT39_ADDR(base)                      ((base) + 0x1078)

/* Register description: 通用外设状态寄存器40。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT40_UNION */
#define SOC_PCTRL_PERI_STAT40_ADDR(base)                      ((base) + 0x107C)

/* Register description: 通用外设状态寄存器41。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT41_UNION */
#define SOC_PCTRL_PERI_STAT41_ADDR(base)                      ((base) + 0x1080)

/* Register description: 通用外设状态寄存器42。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT42_UNION */
#define SOC_PCTRL_PERI_STAT42_ADDR(base)                      ((base) + 0x1084)

/* Register description: 通用外设状态寄存器43。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT43_UNION */
#define SOC_PCTRL_PERI_STAT43_ADDR(base)                      ((base) + 0x1088)

/* Register description: 通用外设状态寄存器44。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT44_UNION */
#define SOC_PCTRL_PERI_STAT44_ADDR(base)                      ((base) + 0x108C)

/* Register description: 通用外设状态寄存器45。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT45_UNION */
#define SOC_PCTRL_PERI_STAT45_ADDR(base)                      ((base) + 0x1090)

/* Register description: 通用外设状态寄存器46。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT46_UNION */
#define SOC_PCTRL_PERI_STAT46_ADDR(base)                      ((base) + 0x1094)

/* Register description: 通用外设状态寄存器47。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT47_UNION */
#define SOC_PCTRL_PERI_STAT47_ADDR(base)                      ((base) + 0x1098)

/* Register description: 通用外设状态寄存器48（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT48_UNION */
#define SOC_PCTRL_PERI_STAT48_ADDR(base)                      ((base) + 0x109C)

/* Register description: 通用外设状态寄存器49（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT49_UNION */
#define SOC_PCTRL_PERI_STAT49_ADDR(base)                      ((base) + 0x10A0)

/* Register description: 通用外设状态寄存器50（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT50_UNION */
#define SOC_PCTRL_PERI_STAT50_ADDR(base)                      ((base) + 0x10A4)

/* Register description: 通用外设状态寄存器51（SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT51_UNION */
#define SOC_PCTRL_PERI_STAT51_ADDR(base)                      ((base) + 0x10A8)

/* Register description: 通用外设状态寄存器52（保留给SD回读）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT52_UNION */
#define SOC_PCTRL_PERI_STAT52_ADDR(base)                      ((base) + 0x10AC)

/* Register description: 通用外设状态寄存器53（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT53_UNION */
#define SOC_PCTRL_PERI_STAT53_ADDR(base)                      ((base) + 0x10B0)

/* Register description: 通用外设状态寄存器54（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT54_UNION */
#define SOC_PCTRL_PERI_STAT54_ADDR(base)                      ((base) + 0x10B4)

/* Register description: 通用外设状态寄存器55（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT55_UNION */
#define SOC_PCTRL_PERI_STAT55_ADDR(base)                      ((base) + 0x10B8)

/* Register description: 通用外设状态寄存器56（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT56_UNION */
#define SOC_PCTRL_PERI_STAT56_ADDR(base)                      ((base) + 0x10BC)

/* Register description: 通用外设状态寄存器57（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT57_UNION */
#define SOC_PCTRL_PERI_STAT57_ADDR(base)                      ((base) + 0x10C0)

/* Register description: 通用外设状态寄存器58（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT58_UNION */
#define SOC_PCTRL_PERI_STAT58_ADDR(base)                      ((base) + 0x10C4)

/* Register description: 通用外设状态寄存器59（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT59_UNION */
#define SOC_PCTRL_PERI_STAT59_ADDR(base)                      ((base) + 0x10C8)

/* Register description: 通用外设状态寄存器60（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT60_UNION */
#define SOC_PCTRL_PERI_STAT60_ADDR(base)                      ((base) + 0x10CC)

/* Register description: 通用外设状态寄存器61（QIC nopending）。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT61_UNION */
#define SOC_PCTRL_PERI_STAT61_ADDR(base)                      ((base) + 0x10D0)

/* Register description: 通用外设状态寄存器62（QIC nopending）
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT62_UNION */
#define SOC_PCTRL_PERI_STAT62_ADDR(base)                      ((base) + 0x10D4)

/* Register description: 通用外设状态寄存器63（QIC nopending）
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT63_UNION */
#define SOC_PCTRL_PERI_STAT63_ADDR(base)                      ((base) + 0x10D8)

/* Register description: 通用外设状态寄存器64（QIC nopending）
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT64_UNION */
#define SOC_PCTRL_PERI_STAT64_ADDR(base)                      ((base) + 0x10DC)

/* Register description: 通用外设状态寄存器66--保留给。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT66_UNION */
#define SOC_PCTRL_PERI_STAT66_ADDR(base)                      ((base) + 0x10E0)

/* Register description: 通用外设状态寄存器67--保留给。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT67_UNION */
#define SOC_PCTRL_PERI_STAT67_ADDR(base)                      ((base) + 0x10E4)

/* Register description: 通用外设状态寄存器68。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT68_UNION */
#define SOC_PCTRL_PERI_STAT68_ADDR(base)                      ((base) + 0x10E8)

/* Register description: 通用外设状态寄存器69。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT69_UNION */
#define SOC_PCTRL_PERI_STAT69_ADDR(base)                      ((base) + 0x10EC)

/* Register description: 通用外设状态寄存器70。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT70_UNION */
#define SOC_PCTRL_PERI_STAT70_ADDR(base)                      ((base) + 0x10F0)

/* Register description: 通用外设状态寄存器71。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT71_UNION */
#define SOC_PCTRL_PERI_STAT71_ADDR(base)                      ((base) + 0x10F4)

/* Register description: 通用外设状态寄存器72。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT72_UNION */
#define SOC_PCTRL_PERI_STAT72_ADDR(base)                      ((base) + 0x10F8)

/* Register description: 通用外设状态寄存器73。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT73_UNION */
#define SOC_PCTRL_PERI_STAT73_ADDR(base)                      ((base) + 0x10FC)

/* Register description: 通用外设状态寄存器74-mtcmos回读。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT74_UNION */
#define SOC_PCTRL_PERI_STAT74_ADDR(base)                      ((base) + 0x1100)

/* Register description: 通用外设状态寄存器75。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT75_UNION */
#define SOC_PCTRL_PERI_STAT75_ADDR(base)                      ((base) + 0x1104)

/* Register description: 通用外设状态寄存器76。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT76_UNION */
#define SOC_PCTRL_PERI_STAT76_ADDR(base)                      ((base) + 0x1108)

/* Register description: 通用外设状态寄存器77。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT77_UNION */
#define SOC_PCTRL_PERI_STAT77_ADDR(base)                      ((base) + 0x110C)

/* Register description: 通用外设状态寄存器78。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT78_UNION */
#define SOC_PCTRL_PERI_STAT78_ADDR(base)                      ((base) + 0x1110)

/* Register description: 通用外设状态寄存器79。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT79_UNION */
#define SOC_PCTRL_PERI_STAT79_ADDR(base)                      ((base) + 0x1114)

/* Register description: 通用外设状态寄存器80。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT80_UNION */
#define SOC_PCTRL_PERI_STAT80_ADDR(base)                      ((base) + 0x1118)

/* Register description: 通用外设状态寄存器81。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT81_UNION */
#define SOC_PCTRL_PERI_STAT81_ADDR(base)                      ((base) + 0x111C)

/* Register description: 通用外设状态寄存器82。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT82_UNION */
#define SOC_PCTRL_PERI_STAT82_ADDR(base)                      ((base) + 0x1120)

/* Register description: 通用外设状态寄存器83。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT83_UNION */
#define SOC_PCTRL_PERI_STAT83_ADDR(base)                      ((base) + 0x1124)

/* Register description: 通用外设状态寄存器84。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT84_UNION */
#define SOC_PCTRL_PERI_STAT84_ADDR(base)                      ((base) + 0x1128)

/* Register description: 通用外设状态寄存器85。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT85_UNION */
#define SOC_PCTRL_PERI_STAT85_ADDR(base)                      ((base) + 0x112C)

/* Register description: 通用外设状态寄存器86。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT86_UNION */
#define SOC_PCTRL_PERI_STAT86_ADDR(base)                      ((base) + 0x1130)

/* Register description: 通用外设状态寄存器87。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT87_UNION */
#define SOC_PCTRL_PERI_STAT87_ADDR(base)                      ((base) + 0x1134)

/* Register description: 通用外设状态寄存器88。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT88_UNION */
#define SOC_PCTRL_PERI_STAT88_ADDR(base)                      ((base) + 0x1138)

/* Register description: 通用外设状态寄存器89。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT89_UNION */
#define SOC_PCTRL_PERI_STAT89_ADDR(base)                      ((base) + 0x113C)

/* Register description: 通用外设状态寄存器90。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT90_UNION */
#define SOC_PCTRL_PERI_STAT90_ADDR(base)                      ((base) + 0x1140)

/* Register description: 通用外设状态寄存器91。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT91_UNION */
#define SOC_PCTRL_PERI_STAT91_ADDR(base)                      ((base) + 0x1144)

/* Register description: 通用外设状态寄存器92。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT92_UNION */
#define SOC_PCTRL_PERI_STAT92_ADDR(base)                      ((base) + 0x1148)

/* Register description: 通用外设状态寄存器93。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT93_UNION */
#define SOC_PCTRL_PERI_STAT93_ADDR(base)                      ((base) + 0x114C)

/* Register description: 通用外设状态寄存器94。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT94_UNION */
#define SOC_PCTRL_PERI_STAT94_ADDR(base)                      ((base) + 0x1150)

/* Register description: 通用外设状态寄存器95。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT95_UNION */
#define SOC_PCTRL_PERI_STAT95_ADDR(base)                      ((base) + 0x1154)

/* Register description: 通用外设状态寄存器96。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT96_UNION */
#define SOC_PCTRL_PERI_STAT96_ADDR(base)                      ((base) + 0x1158)

/* Register description: 通用外设状态寄存器97。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT97_UNION */
#define SOC_PCTRL_PERI_STAT97_ADDR(base)                      ((base) + 0x115C)

/* Register description: 通用外设状态寄存器98。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT98_UNION */
#define SOC_PCTRL_PERI_STAT98_ADDR(base)                      ((base) + 0x1160)

/* Register description: 通用外设状态寄存器99。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT99_UNION */
#define SOC_PCTRL_PERI_STAT99_ADDR(base)                      ((base) + 0x1164)

/* Register description: 通用外设状态寄存器100。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT100_UNION */
#define SOC_PCTRL_PERI_STAT100_ADDR(base)                     ((base) + 0x1168)

/* Register description: 通用外设状态寄存器101。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT101_UNION */
#define SOC_PCTRL_PERI_STAT101_ADDR(base)                     ((base) + 0x116C)

/* Register description: 通用外设状态寄存器102。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT102_UNION */
#define SOC_PCTRL_PERI_STAT102_ADDR(base)                     ((base) + 0x1170)

/* Register description: 通用外设状态寄存器103。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT103_UNION */
#define SOC_PCTRL_PERI_STAT103_ADDR(base)                     ((base) + 0x1174)

/* Register description: 通用外设状态寄存器104。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT104_UNION */
#define SOC_PCTRL_PERI_STAT104_ADDR(base)                     ((base) + 0x1178)

/* Register description: 通用外设状态寄存器105。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT105_UNION */
#define SOC_PCTRL_PERI_STAT105_ADDR(base)                     ((base) + 0x117C)

/* Register description: 通用外设状态寄存器106。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT106_UNION */
#define SOC_PCTRL_PERI_STAT106_ADDR(base)                     ((base) + 0x1180)

/* Register description: 通用外设状态寄存器107。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT107_UNION */
#define SOC_PCTRL_PERI_STAT107_ADDR(base)                     ((base) + 0x1184)

/* Register description: 通用外设状态寄存器108。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT108_UNION */
#define SOC_PCTRL_PERI_STAT108_ADDR(base)                     ((base) + 0x1188)

/* Register description: 通用外设状态寄存器109。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT109_UNION */
#define SOC_PCTRL_PERI_STAT109_ADDR(base)                     ((base) + 0x118C)

/* Register description: 通用外设状态寄存器110。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT110_UNION */
#define SOC_PCTRL_PERI_STAT110_ADDR(base)                     ((base) + 0x1190)

/* Register description: 通用外设状态寄存器111。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT111_UNION */
#define SOC_PCTRL_PERI_STAT111_ADDR(base)                     ((base) + 0x1194)

/* Register description: 通用外设状态寄存器112。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT112_UNION */
#define SOC_PCTRL_PERI_STAT112_ADDR(base)                     ((base) + 0x1198)

/* Register description: 通用外设状态寄存器113。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT113_UNION */
#define SOC_PCTRL_PERI_STAT113_ADDR(base)                     ((base) + 0x119C)

/* Register description: 通用外设状态寄存器114。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT114_UNION */
#define SOC_PCTRL_PERI_STAT114_ADDR(base)                     ((base) + 0x11A0)

/* Register description: 通用外设状态寄存器115。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT115_UNION */
#define SOC_PCTRL_PERI_STAT115_ADDR(base)                     ((base) + 0x11A4)

/* Register description: 通用外设状态寄存器116。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT116_UNION */
#define SOC_PCTRL_PERI_STAT116_ADDR(base)                     ((base) + 0x11A8)

/* Register description: 通用外设状态寄存器117。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT117_UNION */
#define SOC_PCTRL_PERI_STAT117_ADDR(base)                     ((base) + 0x11AC)

/* Register description: 通用外设状态寄存器118。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT118_UNION */
#define SOC_PCTRL_PERI_STAT118_ADDR(base)                     ((base) + 0x11B0)

/* Register description: 通用外设状态寄存器119。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT119_UNION */
#define SOC_PCTRL_PERI_STAT119_ADDR(base)                     ((base) + 0x11B4)

/* Register description: 通用外设状态寄存器120。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT120_UNION */
#define SOC_PCTRL_PERI_STAT120_ADDR(base)                     ((base) + 0x11B8)

/* Register description: 通用外设状态寄存器121。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT121_UNION */
#define SOC_PCTRL_PERI_STAT121_ADDR(base)                     ((base) + 0x11BC)

/* Register description: 通用外设状态寄存器122。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT122_UNION */
#define SOC_PCTRL_PERI_STAT122_ADDR(base)                     ((base) + 0x11C0)

/* Register description: 通用外设状态寄存器123。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT123_UNION */
#define SOC_PCTRL_PERI_STAT123_ADDR(base)                     ((base) + 0x11C4)

/* Register description: 通用外设状态寄存器124。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT124_UNION */
#define SOC_PCTRL_PERI_STAT124_ADDR(base)                     ((base) + 0x11C8)

/* Register description: 通用外设状态寄存器125。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT125_UNION */
#define SOC_PCTRL_PERI_STAT125_ADDR(base)                     ((base) + 0x11CC)

/* Register description: 通用外设状态寄存器126。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT126_UNION */
#define SOC_PCTRL_PERI_STAT126_ADDR(base)                     ((base) + 0x11D0)

/* Register description: 通用外设状态寄存器127。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT127_UNION */
#define SOC_PCTRL_PERI_STAT127_ADDR(base)                     ((base) + 0x11D4)

/* Register description: 通用外设状态寄存器128。
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT128_UNION */
#define SOC_PCTRL_PERI_STAT128_ADDR(base)                     ((base) + 0x11D8)

/* Register description: 通用外设状态寄存器129-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT129_UNION */
#define SOC_PCTRL_PERI_STAT129_ADDR(base)                     ((base) + 0x11DC)

/* Register description: 通用外设状态寄存器130-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT130_UNION */
#define SOC_PCTRL_PERI_STAT130_ADDR(base)                     ((base) + 0x11E0)

/* Register description: 通用外设状态寄存器131-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT131_UNION */
#define SOC_PCTRL_PERI_STAT131_ADDR(base)                     ((base) + 0x11E4)

/* Register description: 通用外设状态寄存器132-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT132_UNION */
#define SOC_PCTRL_PERI_STAT132_ADDR(base)                     ((base) + 0x11E8)

/* Register description: 通用外设状态寄存器133-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT133_UNION */
#define SOC_PCTRL_PERI_STAT133_ADDR(base)                     ((base) + 0x11EC)

/* Register description: 通用外设状态寄存器134-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT134_UNION */
#define SOC_PCTRL_PERI_STAT134_ADDR(base)                     ((base) + 0x11F0)

/* Register description: 通用外设状态寄存器135-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT135_UNION */
#define SOC_PCTRL_PERI_STAT135_ADDR(base)                     ((base) + 0x11F4)

/* Register description: 通用外设状态寄存器136-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT136_UNION */
#define SOC_PCTRL_PERI_STAT136_ADDR(base)                     ((base) + 0x11F8)

/* Register description: 通用外设状态寄存器137-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT137_UNION */
#define SOC_PCTRL_PERI_STAT137_ADDR(base)                     ((base) + 0x11FC)

/* Register description: 通用外设状态寄存器138-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT138_UNION */
#define SOC_PCTRL_PERI_STAT138_ADDR(base)                     ((base) + 0x1200)

/* Register description: 通用外设状态寄存器139-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT139_UNION */
#define SOC_PCTRL_PERI_STAT139_ADDR(base)                     ((base) + 0x1204)

/* Register description: 通用外设状态寄存器140-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT140_UNION */
#define SOC_PCTRL_PERI_STAT140_ADDR(base)                     ((base) + 0x1208)

/* Register description: 通用外设状态寄存器141-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT141_UNION */
#define SOC_PCTRL_PERI_STAT141_ADDR(base)                     ((base) + 0x120C)

/* Register description: 通用外设状态寄存器142-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT142_UNION */
#define SOC_PCTRL_PERI_STAT142_ADDR(base)                     ((base) + 0x1210)

/* Register description: 通用外设状态寄存器143-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT143_UNION */
#define SOC_PCTRL_PERI_STAT143_ADDR(base)                     ((base) + 0x1214)

/* Register description: 通用外设状态寄存器144-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT144_UNION */
#define SOC_PCTRL_PERI_STAT144_ADDR(base)                     ((base) + 0x1218)

/* Register description: 通用外设状态寄存器145-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT145_UNION */
#define SOC_PCTRL_PERI_STAT145_ADDR(base)                     ((base) + 0x121C)

/* Register description: 通用外设状态寄存器146-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT146_UNION */
#define SOC_PCTRL_PERI_STAT146_ADDR(base)                     ((base) + 0x1220)

/* Register description: 通用外设状态寄存器147-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT147_UNION */
#define SOC_PCTRL_PERI_STAT147_ADDR(base)                     ((base) + 0x1224)

/* Register description: 通用外设状态寄存器148-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT148_UNION */
#define SOC_PCTRL_PERI_STAT148_ADDR(base)                     ((base) + 0x1228)

/* Register description: 通用外设状态寄存器149-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT149_UNION */
#define SOC_PCTRL_PERI_STAT149_ADDR(base)                     ((base) + 0x122C)

/* Register description: 通用外设状态寄存器150-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT150_UNION */
#define SOC_PCTRL_PERI_STAT150_ADDR(base)                     ((base) + 0x1230)

/* Register description: 通用外设状态寄存器151-QIC intr中断上报
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT151_UNION */
#define SOC_PCTRL_PERI_STAT151_ADDR(base)                     ((base) + 0x1234)

/* Register description: 通用外设状态寄存器152
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT152_UNION */
#define SOC_PCTRL_PERI_STAT152_ADDR(base)                     ((base) + 0x1238)

/* Register description: 通用外设状态寄存器153
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT153_UNION */
#define SOC_PCTRL_PERI_STAT153_ADDR(base)                     ((base) + 0x123C)

/* Register description: 通用外设状态寄存器154
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT154_UNION */
#define SOC_PCTRL_PERI_STAT154_ADDR(base)                     ((base) + 0x1240)

/* Register description: 通用外设状态寄存器155
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT155_UNION */
#define SOC_PCTRL_PERI_STAT155_ADDR(base)                     ((base) + 0x1244)

/* Register description: 通用外设状态寄存器156
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT156_UNION */
#define SOC_PCTRL_PERI_STAT156_ADDR(base)                     ((base) + 0x1248)

/* Register description: 通用外设状态寄存器157
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT157_UNION */
#define SOC_PCTRL_PERI_STAT157_ADDR(base)                     ((base) + 0x124C)

/* Register description: 通用外设状态寄存器158
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT158_UNION */
#define SOC_PCTRL_PERI_STAT158_ADDR(base)                     ((base) + 0x1250)

/* Register description: 通用外设状态寄存器159
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT159_UNION */
#define SOC_PCTRL_PERI_STAT159_ADDR(base)                     ((base) + 0x1254)

/* Register description: 通用外设控制寄存器0。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL0_UNION */
#define SOC_PCTRL_PERI_CTRL0_ADDR(base)                       ((base) + 0x1800)

/* Register description: 通用外设控制寄存器1。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL1_UNION */
#define SOC_PCTRL_PERI_CTRL1_ADDR(base)                       ((base) + 0x1804)

/* Register description: 通用外设控制寄存器2。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL2_UNION */
#define SOC_PCTRL_PERI_CTRL2_ADDR(base)                       ((base) + 0x1808)

/* Register description: 通用外设控制寄存器3。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL3_UNION */
#define SOC_PCTRL_PERI_CTRL3_ADDR(base)                       ((base) + 0x181C)

/* Register description: 通用外设控制寄存器102。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL102_UNION */
#define SOC_PCTRL_PERI_CTRL102_ADDR(base)                     ((base) + 0x1820)

/* Register description: 通用外设控制寄存器103。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL103_UNION */
#define SOC_PCTRL_PERI_CTRL103_ADDR(base)                     ((base) + 0x1824)

/* Register description: 通用外设控制寄存器105。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL105_UNION */
#define SOC_PCTRL_PERI_CTRL105_ADDR(base)                     ((base) + 0x1828)

/* Register description: 通用外设控制寄存器106。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL106_UNION */
#define SOC_PCTRL_PERI_CTRL106_ADDR(base)                     ((base) + 0x182C)

/* Register description: 通用外设控制寄存器107。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL107_UNION */
#define SOC_PCTRL_PERI_CTRL107_ADDR(base)                     ((base) + 0x1830)

/* Register description: 通用外设状态寄存器176(QIC nopending WC回读）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl176_wc_UNION */
#define SOC_PCTRL_peri_ctrl176_wc_ADDR(base)                  ((base) + 0x1834)

/* Register description: 通用外设状态寄存器177（QIC nopending WC回读）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl177_wc_UNION */
#define SOC_PCTRL_peri_ctrl177_wc_ADDR(base)                  ((base) + 0x1838)

/* Register description: 通用外设状态寄存器178（QIC nopending WC回读）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl178_wc_UNION */
#define SOC_PCTRL_peri_ctrl178_wc_ADDR(base)                  ((base) + 0x183C)

/* Register description: 通用外设状态寄存器179。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl179_wc_UNION */
#define SOC_PCTRL_peri_ctrl179_wc_ADDR(base)                  ((base) + 0x1840)

/* Register description: 通用外设状态寄存器180。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl180_wc_UNION */
#define SOC_PCTRL_peri_ctrl180_wc_ADDR(base)                  ((base) + 0x1844)

/* Register description: 通用外设状态寄存器181。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl181_wc_UNION */
#define SOC_PCTRL_peri_ctrl181_wc_ADDR(base)                  ((base) + 0x1848)

/* Register description: 通用外设状态寄存器182。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl182_wc_UNION */
#define SOC_PCTRL_peri_ctrl182_wc_ADDR(base)                  ((base) + 0x184C)

/* Register description: 通用外设状态寄存器183。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl183_wc_UNION */
#define SOC_PCTRL_peri_ctrl183_wc_ADDR(base)                  ((base) + 0x1850)

/* Register description: 通用外设状态寄存器184。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl184_wc_UNION */
#define SOC_PCTRL_peri_ctrl184_wc_ADDR(base)                  ((base) + 0x1854)

/* Register description: 通用外设状态寄存器185（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl185_wc_UNION */
#define SOC_PCTRL_peri_ctrl185_wc_ADDR(base)                  ((base) + 0x1858)

/* Register description: 通用外设状态寄存器186（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl186_wc_UNION */
#define SOC_PCTRL_peri_ctrl186_wc_ADDR(base)                  ((base) + 0x185C)

/* Register description: 通用外设状态寄存器187（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl187_wc_UNION */
#define SOC_PCTRL_peri_ctrl187_wc_ADDR(base)                  ((base) + 0x1860)

/* Register description: 通用外设状态寄存器188（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl188_wc_UNION */
#define SOC_PCTRL_peri_ctrl188_wc_ADDR(base)                  ((base) + 0x1864)

/* Register description: 通用外设状态寄存器189（QIC）。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl189_wc_UNION */
#define SOC_PCTRL_peri_ctrl189_wc_ADDR(base)                  ((base) + 0x1868)

/* Register description: 通用外设状态寄存器190-qic_pwrd。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl190_qic_pwrd_UNION */
#define SOC_PCTRL_peri_ctrl190_qic_pwrd_ADDR(base)            ((base) + 0x186C)

/* Register description: 通用外设状态寄存器191-mtcmos。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl191_mtcmos_UNION */
#define SOC_PCTRL_peri_ctrl191_mtcmos_ADDR(base)              ((base) + 0x1870)

/* Register description: 通用外设状态寄存器192。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl192_wc_UNION */
#define SOC_PCTRL_peri_ctrl192_wc_ADDR(base)                  ((base) + 0x1874)

/* Register description: 通用外设状态寄存器193。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl193_wc_UNION */
#define SOC_PCTRL_peri_ctrl193_wc_ADDR(base)                  ((base) + 0x1878)

/* Register description: 通用外设状态寄存器194。
   Bit domain definition UNION:  SOC_PCTRL_peri_ctrl194_wc_UNION */
#define SOC_PCTRL_peri_ctrl194_wc_ADDR(base)                  ((base) + 0x187C)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl0_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl0_wc_ADDR(base)                 ((base) + 0x1880)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl1_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl1_wc_ADDR(base)                 ((base) + 0x1884)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl2_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl2_wc_ADDR(base)                 ((base) + 0x1888)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl3_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl3_wc_ADDR(base)                 ((base) + 0x188C)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl4_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl4_wc_ADDR(base)                 ((base) + 0x1890)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl5_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl5_wc_ADDR(base)                 ((base) + 0x1894)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl6_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl6_wc_ADDR(base)                 ((base) + 0x1898)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl7_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl7_wc_ADDR(base)                 ((base) + 0x189C)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl8_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl8_wc_ADDR(base)                 ((base) + 0x18A0)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl9_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl9_wc_ADDR(base)                 ((base) + 0x18A4)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl10_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl10_wc_ADDR(base)                ((base) + 0x18A8)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl11_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl11_wc_ADDR(base)                ((base) + 0x18AC)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl12_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl12_wc_ADDR(base)                ((base) + 0x18B0)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl13_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl13_wc_ADDR(base)                ((base) + 0x18B4)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl14_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl14_wc_ADDR(base)                ((base) + 0x18B8)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl15_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl15_wc_ADDR(base)                ((base) + 0x18BC)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl16_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl16_wc_ADDR(base)                ((base) + 0x18C0)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl17_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl17_wc_ADDR(base)                ((base) + 0x18C4)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl18_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl18_wc_ADDR(base)                ((base) + 0x18C8)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl19_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl19_wc_ADDR(base)                ((base) + 0x18CC)

/* Register description: 通用外设状态寄存器-QIC intr MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl20_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl20_wc_ADDR(base)                ((base) + 0x18D0)

/* Register description: 通用外设状态寄存器-QIC intr组合逻辑后锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl21_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl21_wc_ADDR(base)                ((base) + 0x18D4)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl22_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl22_wc_ADDR(base)                ((base) + 0x18D8)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl23_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl23_wc_ADDR(base)                ((base) + 0x18DC)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl24_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl24_wc_ADDR(base)                ((base) + 0x18E0)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl25_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl25_wc_ADDR(base)                ((base) + 0x18E4)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl26_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl26_wc_ADDR(base)                ((base) + 0x18E8)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl27_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl27_wc_ADDR(base)                ((base) + 0x18EC)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl28_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl28_wc_ADDR(base)                ((base) + 0x18F0)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl29_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl29_wc_ADDR(base)                ((base) + 0x18F4)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl30_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl30_wc_ADDR(base)                ((base) + 0x18F8)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl31_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl31_wc_ADDR(base)                ((base) + 0x18FC)

/* Register description: 通用外设状态寄存器-QIC   busy MWC锁存
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl32_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl32_wc_ADDR(base)                ((base) + 0x1900)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl33_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl33_wc_ADDR(base)                ((base) + 0x1904)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl34_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl34_wc_ADDR(base)                ((base) + 0x1908)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl35_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl35_wc_ADDR(base)                ((base) + 0x190C)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl36_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl36_wc_ADDR(base)                ((base) + 0x1910)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl37_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl37_wc_ADDR(base)                ((base) + 0x1914)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl38_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl38_wc_ADDR(base)                ((base) + 0x1918)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl39_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl39_wc_ADDR(base)                ((base) + 0x191C)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl40_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl40_wc_ADDR(base)                ((base) + 0x1920)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl41_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl41_wc_ADDR(base)                ((base) + 0x1924)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl42_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl42_wc_ADDR(base)                ((base) + 0x1928)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl43_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl43_wc_ADDR(base)                ((base) + 0x192C)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl44_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl44_wc_ADDR(base)                ((base) + 0x1930)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl45_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl45_wc_ADDR(base)                ((base) + 0x1934)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl46_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl46_wc_ADDR(base)                ((base) + 0x1938)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl47_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl47_wc_ADDR(base)                ((base) + 0x193C)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl48_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl48_wc_ADDR(base)                ((base) + 0x1940)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl49_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl49_wc_ADDR(base)                ((base) + 0x1944)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl50_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl50_wc_ADDR(base)                ((base) + 0x1948)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl51_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl51_wc_ADDR(base)                ((base) + 0x194C)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl52_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl52_wc_ADDR(base)                ((base) + 0x1950)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl53_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl53_wc_ADDR(base)                ((base) + 0x1954)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl54_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl54_wc_ADDR(base)                ((base) + 0x1958)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl55_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl55_wc_ADDR(base)                ((base) + 0x195C)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl56_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl56_wc_ADDR(base)                ((base) + 0x1960)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl57_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl57_wc_ADDR(base)                ((base) + 0x1964)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl58_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl58_wc_ADDR(base)                ((base) + 0x1968)

/* Register description: 通用外设状态寄存器
   Bit domain definition UNION:  SOC_PCTRL_R2_peri_ctrl59_wc_UNION */
#define SOC_PCTRL_R2_peri_ctrl59_wc_ADDR(base)                ((base) + 0x196C)

/* Register description: 通用外设控制寄存器5_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL5_UNION */
#define SOC_PCTRL_PERI_CTRL5_ADDR(base)                       ((base) + 0x1C00)

/* Register description: 通用外设控制寄存器6_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL6_UNION */
#define SOC_PCTRL_PERI_CTRL6_ADDR(base)                       ((base) + 0x1C04)

/* Register description: 通用外设控制寄存器7_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL7_UNION */
#define SOC_PCTRL_PERI_CTRL7_ADDR(base)                       ((base) + 0x1C08)

/* Register description: 通用外设控制寄存器8_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL8_UNION */
#define SOC_PCTRL_PERI_CTRL8_ADDR(base)                       ((base) + 0x1C0C)

/* Register description: 通用外设控制寄存器9_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL9_UNION */
#define SOC_PCTRL_PERI_CTRL9_ADDR(base)                       ((base) + 0x1C10)

/* Register description: 通用外设控制寄存器10_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL10_UNION */
#define SOC_PCTRL_PERI_CTRL10_ADDR(base)                      ((base) + 0x1C14)

/* Register description: 通用外设控制寄存器11。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL11_UNION */
#define SOC_PCTRL_PERI_CTRL11_ADDR(base)                      ((base) + 0x1C18)

/* Register description: 通用外设控制寄存器34_安全区。
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL34_UNION */
#define SOC_PCTRL_PERI_CTRL34_ADDR(base)                      ((base) + 0x1C1C)

/* Register description: 通用外设控制寄存器35。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL35_UNION */
#define SOC_PCTRL_PERI_CTRL35_ADDR(base)                      ((base) + 0x1C20)

/* Register description: 通用外设控制寄存器38。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL38_UNION */
#define SOC_PCTRL_PERI_CTRL38_ADDR(base)                      ((base) + 0x1C2C)

/* Register description: 通用外设控制寄存器39。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL39_UNION */
#define SOC_PCTRL_PERI_CTRL39_ADDR(base)                      ((base) + 0x1C30)

/* Register description: 通用外设控制寄存器40。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL40_UNION */
#define SOC_PCTRL_PERI_CTRL40_ADDR(base)                      ((base) + 0x1C34)

/* Register description: 通用外设控制寄存器41。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL41_UNION */
#define SOC_PCTRL_PERI_CTRL41_ADDR(base)                      ((base) + 0x1C38)

/* Register description: 通用外设控制寄存器42_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL42_UNION */
#define SOC_PCTRL_PERI_CTRL42_ADDR(base)                      ((base) + 0x1C3C)

/* Register description: 通用外设控制寄存器43_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL43_UNION */
#define SOC_PCTRL_PERI_CTRL43_ADDR(base)                      ((base) + 0x1C40)

/* Register description: 通用外设控制寄存器44。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL44_UNION */
#define SOC_PCTRL_PERI_CTRL44_ADDR(base)                      ((base) + 0x1C44)

/* Register description: 通用外设控制寄存器45_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL45_UNION */
#define SOC_PCTRL_PERI_CTRL45_ADDR(base)                      ((base) + 0x1C48)

/* Register description: 通用外设控制寄存器46_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL46_UNION */
#define SOC_PCTRL_PERI_CTRL46_ADDR(base)                      ((base) + 0x1C4C)

/* Register description: 通用外设控制寄存器47_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL47_UNION */
#define SOC_PCTRL_PERI_CTRL47_ADDR(base)                      ((base) + 0x1C50)

/* Register description: 通用外设控制寄存器48_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL48_UNION */
#define SOC_PCTRL_PERI_CTRL48_ADDR(base)                      ((base) + 0x1C54)

/* Register description: 通用外设控制寄存器49。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL49_UNION */
#define SOC_PCTRL_PERI_CTRL49_ADDR(base)                      ((base) + 0x1C58)

/* Register description: 通用外设控制寄存器50。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL50_UNION */
#define SOC_PCTRL_PERI_CTRL50_ADDR(base)                      ((base) + 0x1C5C)

/* Register description: 通用外设控制寄存器51。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL51_UNION */
#define SOC_PCTRL_PERI_CTRL51_ADDR(base)                      ((base) + 0x1C60)

/* Register description: 通用外设控制寄存器52。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL52_UNION */
#define SOC_PCTRL_PERI_CTRL52_ADDR(base)                      ((base) + 0x1C64)

/* Register description: 通用外设控制寄存器53。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL53_UNION */
#define SOC_PCTRL_PERI_CTRL53_ADDR(base)                      ((base) + 0x1C68)

/* Register description: 通用外设控制寄存器54。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL54_UNION */
#define SOC_PCTRL_PERI_CTRL54_ADDR(base)                      ((base) + 0x1C6C)

/* Register description: 通用外设控制寄存器55。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL55_UNION */
#define SOC_PCTRL_PERI_CTRL55_ADDR(base)                      ((base) + 0x1C70)

/* Register description: 通用外设控制寄存器56。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL56_UNION */
#define SOC_PCTRL_PERI_CTRL56_ADDR(base)                      ((base) + 0x1C74)

/* Register description: 通用外设控制寄存器57。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL57_UNION */
#define SOC_PCTRL_PERI_CTRL57_ADDR(base)                      ((base) + 0x1C78)

/* Register description: 通用外设控制寄存器58。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL58_UNION */
#define SOC_PCTRL_PERI_CTRL58_ADDR(base)                      ((base) + 0x1C7C)

/* Register description: 通用外设控制寄存器59。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL59_UNION */
#define SOC_PCTRL_PERI_CTRL59_ADDR(base)                      ((base) + 0x1C80)

/* Register description: 通用外设控制寄存器60。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL60_UNION */
#define SOC_PCTRL_PERI_CTRL60_ADDR(base)                      ((base) + 0x1C84)

/* Register description: 通用外设控制寄存器61。_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL61_UNION */
#define SOC_PCTRL_PERI_CTRL61_ADDR(base)                      ((base) + 0x1C88)

/* Register description: 通用外设控制寄存器62_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL62_UNION */
#define SOC_PCTRL_PERI_CTRL62_ADDR(base)                      ((base) + 0x1C8C)

/* Register description: 通用外设控制寄存器63_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL63_UNION */
#define SOC_PCTRL_PERI_CTRL63_ADDR(base)                      ((base) + 0x1C90)

/* Register description: 通用外设控制寄存器64_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL64_UNION */
#define SOC_PCTRL_PERI_CTRL64_ADDR(base)                      ((base) + 0x1C94)

/* Register description: 通用外设控制寄存器65_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL65_UNION */
#define SOC_PCTRL_PERI_CTRL65_ADDR(base)                      ((base) + 0x1C98)

/* Register description: 通用外设控制寄存器66_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL66_UNION */
#define SOC_PCTRL_PERI_CTRL66_ADDR(base)                      ((base) + 0x1C9C)

/* Register description: 通用外设控制寄存器75_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL75_UNION */
#define SOC_PCTRL_PERI_CTRL75_ADDR(base)                      ((base) + 0x1CA0)

/* Register description: 通用外设控制寄存器76_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL76_UNION */
#define SOC_PCTRL_PERI_CTRL76_ADDR(base)                      ((base) + 0x1CA4)

/* Register description: 通用外设控制寄存器77_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL77_UNION */
#define SOC_PCTRL_PERI_CTRL77_ADDR(base)                      ((base) + 0x1CA8)

/* Register description: 通用外设控制寄存器78_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL78_UNION */
#define SOC_PCTRL_PERI_CTRL78_ADDR(base)                      ((base) + 0x1CAC)

/* Register description: 通用外设控制寄存器79_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL79_UNION */
#define SOC_PCTRL_PERI_CTRL79_ADDR(base)                      ((base) + 0x1CB0)

/* Register description: 通用外设控制寄存器80_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL80_UNION */
#define SOC_PCTRL_PERI_CTRL80_ADDR(base)                      ((base) + 0x1CB4)

/* Register description: 通用外设控制寄存器81_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL81_UNION */
#define SOC_PCTRL_PERI_CTRL81_ADDR(base)                      ((base) + 0x1CB8)

/* Register description: 通用外设控制寄存器82_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL82_UNION */
#define SOC_PCTRL_PERI_CTRL82_ADDR(base)                      ((base) + 0x1CBC)

/* Register description: 通用外设控制寄存器83_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL83_UNION */
#define SOC_PCTRL_PERI_CTRL83_ADDR(base)                      ((base) + 0x1CC0)

/* Register description: 通用外设控制寄存器84_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL84_UNION */
#define SOC_PCTRL_PERI_CTRL84_ADDR(base)                      ((base) + 0x1CC4)

/* Register description: 通用外设控制寄存器85_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL85_UNION */
#define SOC_PCTRL_PERI_CTRL85_ADDR(base)                      ((base) + 0x1CC8)

/* Register description: 通用外设控制寄存器86_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL86_UNION */
#define SOC_PCTRL_PERI_CTRL86_ADDR(base)                      ((base) + 0x1CCC)

/* Register description: 通用外设控制寄存器87_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL87_UNION */
#define SOC_PCTRL_PERI_CTRL87_ADDR(base)                      ((base) + 0x1CD0)

/* Register description: 通用外设状态寄存器160_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT160_UNION */
#define SOC_PCTRL_PERI_STAT160_ADDR(base)                     ((base) + 0x1CD4)

/* Register description: 通用外设控制寄存器141_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL168_UNION */
#define SOC_PCTRL_PERI_CTRL168_ADDR(base)                     ((base) + 0x1CD8)

/* Register description: 通用外设控制寄存器142_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL169_UNION */
#define SOC_PCTRL_PERI_CTRL169_ADDR(base)                     ((base) + 0x1CDC)

/* Register description: 通用外设控制寄存器143_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL170_UNION */
#define SOC_PCTRL_PERI_CTRL170_ADDR(base)                     ((base) + 0x1CE0)

/* Register description: 通用外设控制寄存器144_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL171_UNION */
#define SOC_PCTRL_PERI_CTRL171_ADDR(base)                     ((base) + 0x1CE4)

/* Register description: 通用外设控制寄存器145_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL172_UNION */
#define SOC_PCTRL_PERI_CTRL172_ADDR(base)                     ((base) + 0x1CE8)

/* Register description: 通用外设控制寄存器146_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL173_UNION */
#define SOC_PCTRL_PERI_CTRL173_ADDR(base)                     ((base) + 0x1CEC)

/* Register description: 通用外设控制寄存器147_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL174_UNION */
#define SOC_PCTRL_PERI_CTRL174_ADDR(base)                     ((base) + 0x1CF0)

/* Register description: 通用外设控制寄存器148_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL175_UNION */
#define SOC_PCTRL_PERI_CTRL175_ADDR(base)                     ((base) + 0x1CF8)

/* Register description: 通用外设控制寄存器149_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL176_UNION */
#define SOC_PCTRL_PERI_CTRL176_ADDR(base)                     ((base) + 0x1CFC)

/* Register description: 通用外设控制寄存器150_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL177_UNION */
#define SOC_PCTRL_PERI_CTRL177_ADDR(base)                     ((base) + 0x1D00)

/* Register description: 通用外设控制寄存器151_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL178_UNION */
#define SOC_PCTRL_PERI_CTRL178_ADDR(base)                     ((base) + 0x1D04)

/* Register description: 通用外设控制寄存器152_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL179_UNION */
#define SOC_PCTRL_PERI_CTRL179_ADDR(base)                     ((base) + 0x1D08)

/* Register description: 通用外设控制寄存器180_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL180_UNION */
#define SOC_PCTRL_PERI_CTRL180_ADDR(base)                     ((base) + 0x1D0C)

/* Register description: 通用外设控制寄存器181_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL181_UNION */
#define SOC_PCTRL_PERI_CTRL181_ADDR(base)                     ((base) + 0x1D10)

/* Register description: 通用外设控制寄存器182_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL182_UNION */
#define SOC_PCTRL_PERI_CTRL182_ADDR(base)                     ((base) + 0x1D14)

/* Register description: 通用外设控制寄存器183_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL183_UNION */
#define SOC_PCTRL_PERI_CTRL183_ADDR(base)                     ((base) + 0x1D18)

/* Register description: 通用外设控制寄存器184_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL184_UNION */
#define SOC_PCTRL_PERI_CTRL184_ADDR(base)                     ((base) + 0x1D1C)

/* Register description: 通用外设控制寄存器185_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL185_UNION */
#define SOC_PCTRL_PERI_CTRL185_ADDR(base)                     ((base) + 0x1D20)

/* Register description: 通用外设控制寄存器186_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL186_UNION */
#define SOC_PCTRL_PERI_CTRL186_ADDR(base)                     ((base) + 0x1D24)

/* Register description: 通用外设控制寄存器187_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL187_UNION */
#define SOC_PCTRL_PERI_CTRL187_ADDR(base)                     ((base) + 0x1D28)

/* Register description: 通用外设控制寄存器188_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL188_UNION */
#define SOC_PCTRL_PERI_CTRL188_ADDR(base)                     ((base) + 0x1D2C)

/* Register description: 通用外设控制寄存器189_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL189_UNION */
#define SOC_PCTRL_PERI_CTRL189_ADDR(base)                     ((base) + 0x1D30)

/* Register description: 通用外设控制寄存器190_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL190_UNION */
#define SOC_PCTRL_PERI_CTRL190_ADDR(base)                     ((base) + 0x1D34)

/* Register description: 通用外设控制寄存器191_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL191_UNION */
#define SOC_PCTRL_PERI_CTRL191_ADDR(base)                     ((base) + 0x1D38)

/* Register description: 通用外设控制寄存器192_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL192_UNION */
#define SOC_PCTRL_PERI_CTRL192_ADDR(base)                     ((base) + 0x1D3C)

/* Register description: 通用外设控制寄存器193_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL193_UNION */
#define SOC_PCTRL_PERI_CTRL193_ADDR(base)                     ((base) + 0x1D40)

/* Register description: 通用外设控制寄存器194_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL194_UNION */
#define SOC_PCTRL_PERI_CTRL194_ADDR(base)                     ((base) + 0x1D44)

/* Register description: 通用外设控制寄存器195_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL195_UNION */
#define SOC_PCTRL_PERI_CTRL195_ADDR(base)                     ((base) + 0x1D48)

/* Register description: 通用外设控制寄存器196_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL196_UNION */
#define SOC_PCTRL_PERI_CTRL196_ADDR(base)                     ((base) + 0x1D4C)

/* Register description: 通用外设控制寄存器197_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL197_UNION */
#define SOC_PCTRL_PERI_CTRL197_ADDR(base)                     ((base) + 0x1D50)

/* Register description: 通用外设控制寄存器198_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL198_UNION */
#define SOC_PCTRL_PERI_CTRL198_ADDR(base)                     ((base) + 0x1D54)

/* Register description: 通用外设控制寄存器199_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL199_UNION */
#define SOC_PCTRL_PERI_CTRL199_ADDR(base)                     ((base) + 0x1D58)

/* Register description: 通用外设控制寄存器200_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL200_UNION */
#define SOC_PCTRL_PERI_CTRL200_ADDR(base)                     ((base) + 0x1D5C)

/* Register description: 通用外设控制寄存器201_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL201_UNION */
#define SOC_PCTRL_PERI_CTRL201_ADDR(base)                     ((base) + 0x1D60)

/* Register description: 通用外设控制寄存器202_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL202_UNION */
#define SOC_PCTRL_PERI_CTRL202_ADDR(base)                     ((base) + 0x1D64)

/* Register description: 通用外设控制寄存器203_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL203_UNION */
#define SOC_PCTRL_PERI_CTRL203_ADDR(base)                     ((base) + 0x1D68)

/* Register description: 通用外设控制寄存器204_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL204_UNION */
#define SOC_PCTRL_PERI_CTRL204_ADDR(base)                     ((base) + 0x1D6C)

/* Register description: 通用外设控制寄存器205_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL205_UNION */
#define SOC_PCTRL_PERI_CTRL205_ADDR(base)                     ((base) + 0x1D70)

/* Register description: 通用外设控制寄存器206_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL206_UNION */
#define SOC_PCTRL_PERI_CTRL206_ADDR(base)                     ((base) + 0x1D74)

/* Register description: 通用外设控制寄存器207_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL207_UNION */
#define SOC_PCTRL_PERI_CTRL207_ADDR(base)                     ((base) + 0x1D78)

/* Register description: 通用外设控制寄存器208_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL208_UNION */
#define SOC_PCTRL_PERI_CTRL208_ADDR(base)                     ((base) + 0x1D7C)

/* Register description: 通用外设控制寄存器209_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL209_UNION */
#define SOC_PCTRL_PERI_CTRL209_ADDR(base)                     ((base) + 0x1D80)

/* Register description: 通用外设控制寄存器210_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL210_UNION */
#define SOC_PCTRL_PERI_CTRL210_ADDR(base)                     ((base) + 0x1D84)

/* Register description: 通用外设控制寄存器211_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL211_UNION */
#define SOC_PCTRL_PERI_CTRL211_ADDR(base)                     ((base) + 0x1D88)

/* Register description: 通用外设控制寄存器212_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL212_UNION */
#define SOC_PCTRL_PERI_CTRL212_ADDR(base)                     ((base) + 0x1D8C)

/* Register description: 通用外设控制寄存器213_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL213_UNION */
#define SOC_PCTRL_PERI_CTRL213_ADDR(base)                     ((base) + 0x1D90)

/* Register description: 通用外设控制寄存器214_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL214_UNION */
#define SOC_PCTRL_PERI_CTRL214_ADDR(base)                     ((base) + 0x1D94)

/* Register description: 通用外设控制寄存器215_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL215_UNION */
#define SOC_PCTRL_PERI_CTRL215_ADDR(base)                     ((base) + 0x1D98)

/* Register description: 通用外设控制寄存器216_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL216_UNION */
#define SOC_PCTRL_PERI_CTRL216_ADDR(base)                     ((base) + 0x1D9C)

/* Register description: 通用外设控制寄存器217_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL217_UNION */
#define SOC_PCTRL_PERI_CTRL217_ADDR(base)                     ((base) + 0x1DA0)

/* Register description: 通用外设控制寄存器218_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL218_UNION */
#define SOC_PCTRL_PERI_CTRL218_ADDR(base)                     ((base) + 0x1DA4)

/* Register description: 通用外设控制寄存器219_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL219_UNION */
#define SOC_PCTRL_PERI_CTRL219_ADDR(base)                     ((base) + 0x1DA8)

/* Register description: 通用外设控制寄存器220_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL220_UNION */
#define SOC_PCTRL_PERI_CTRL220_ADDR(base)                     ((base) + 0x1DAC)

/* Register description: 通用外设控制寄存器221_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL221_UNION */
#define SOC_PCTRL_PERI_CTRL221_ADDR(base)                     ((base) + 0x1DB0)

/* Register description: 通用外设控制寄存器222_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL222_UNION */
#define SOC_PCTRL_PERI_CTRL222_ADDR(base)                     ((base) + 0x1DB4)

/* Register description: 通用外设控制寄存器223_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL223_UNION */
#define SOC_PCTRL_PERI_CTRL223_ADDR(base)                     ((base) + 0x1DB8)

/* Register description: 通用外设控制寄存器224_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL224_UNION */
#define SOC_PCTRL_PERI_CTRL224_ADDR(base)                     ((base) + 0x1DBC)

/* Register description: 通用外设控制寄存器225_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL225_UNION */
#define SOC_PCTRL_PERI_CTRL225_ADDR(base)                     ((base) + 0x1DC0)

/* Register description: 通用外设控制寄存器226_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL226_UNION */
#define SOC_PCTRL_PERI_CTRL226_ADDR(base)                     ((base) + 0x1DC4)

/* Register description: 通用外设控制寄存器227_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL227_UNION */
#define SOC_PCTRL_PERI_CTRL227_ADDR(base)                     ((base) + 0x1DC8)

/* Register description: 通用外设控制寄存器228_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL228_UNION */
#define SOC_PCTRL_PERI_CTRL228_ADDR(base)                     ((base) + 0x1DCC)

/* Register description: 通用外设控制寄存器229_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL229_UNION */
#define SOC_PCTRL_PERI_CTRL229_ADDR(base)                     ((base) + 0x1DD0)

/* Register description: 通用外设控制寄存器230_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL230_UNION */
#define SOC_PCTRL_PERI_CTRL230_ADDR(base)                     ((base) + 0x1DD4)

/* Register description: 通用外设控制寄存器231_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL231_UNION */
#define SOC_PCTRL_PERI_CTRL231_ADDR(base)                     ((base) + 0x1DD8)

/* Register description: 通用外设控制寄存器232_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL232_UNION */
#define SOC_PCTRL_PERI_CTRL232_ADDR(base)                     ((base) + 0x1DDC)

/* Register description: 通用外设控制寄存器233_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL233_UNION */
#define SOC_PCTRL_PERI_CTRL233_ADDR(base)                     ((base) + 0x1DE0)

/* Register description: 通用外设控制寄存器234_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL234_UNION */
#define SOC_PCTRL_PERI_CTRL234_ADDR(base)                     ((base) + 0x1DE4)

/* Register description: 通用外设控制寄存器235_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL235_UNION */
#define SOC_PCTRL_PERI_CTRL235_ADDR(base)                     ((base) + 0x1DE8)

/* Register description: 通用外设控制寄存器236_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL236_UNION */
#define SOC_PCTRL_PERI_CTRL236_ADDR(base)                     ((base) + 0x1DEC)

/* Register description: 通用外设控制寄存器237_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL237_UNION */
#define SOC_PCTRL_PERI_CTRL237_ADDR(base)                     ((base) + 0x1DF0)

/* Register description: 通用外设控制寄存器238_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL238_UNION */
#define SOC_PCTRL_PERI_CTRL238_ADDR(base)                     ((base) + 0x1DF4)

/* Register description: 通用外设控制寄存器239_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL239_UNION */
#define SOC_PCTRL_PERI_CTRL239_ADDR(base)                     ((base) + 0x1DF8)

/* Register description: 通用外设控制寄存器240_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL240_UNION */
#define SOC_PCTRL_PERI_CTRL240_ADDR(base)                     ((base) + 0x1DFC)

/* Register description: 通用外设控制寄存器241_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL241_UNION */
#define SOC_PCTRL_PERI_CTRL241_ADDR(base)                     ((base) + 0x1E00)

/* Register description: 通用外设控制寄存器242_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL242_UNION */
#define SOC_PCTRL_PERI_CTRL242_ADDR(base)                     ((base) + 0x1E04)

/* Register description: 通用外设控制寄存器243_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL243_UNION */
#define SOC_PCTRL_PERI_CTRL243_ADDR(base)                     ((base) + 0x1E08)

/* Register description: 通用外设控制寄存器244_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL244_UNION */
#define SOC_PCTRL_PERI_CTRL244_ADDR(base)                     ((base) + 0x1E0C)

/* Register description: 通用外设控制寄存器245_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL245_UNION */
#define SOC_PCTRL_PERI_CTRL245_ADDR(base)                     ((base) + 0x1E10)

/* Register description: 通用外设控制寄存器246_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL246_UNION */
#define SOC_PCTRL_PERI_CTRL246_ADDR(base)                     ((base) + 0x1E14)

/* Register description: 通用外设控制寄存器247_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL247_UNION */
#define SOC_PCTRL_PERI_CTRL247_ADDR(base)                     ((base) + 0x1E18)

/* Register description: 通用外设控制寄存器248_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL248_UNION */
#define SOC_PCTRL_PERI_CTRL248_ADDR(base)                     ((base) + 0x1E1C)

/* Register description: 通用外设控制寄存器249_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_CTRL249_UNION */
#define SOC_PCTRL_PERI_CTRL249_ADDR(base)                     ((base) + 0x1E20)

/* Register description: 通用外设回读寄存器0_安全区
   Bit domain definition UNION:  SOC_PCTRL_PERI_STAT_SEC0_UNION */
#define SOC_PCTRL_PERI_STAT_SEC0_ADDR(base)                   ((base) + 0x1E24)


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
                     (1/1) reg_define
 ****************************************************************************/
/*****************************************************************************
 struct               : SOC_PCTRL_G3D_RASTER_UNION
 struct description   : G3D_RASTER Register structure definition
                        Address Offset:0x0000 Initial:0x0000 Width:32
 register description : G3D光栅控制寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 10; /* bit[0-9]  : 保留 */
        unsigned int  reserved_1: 1;  /* bit[10]   : 保留 */
        unsigned int  reserved_2: 1;  /* bit[11]   : 保留 */
        unsigned int  reserved_3: 1;  /* bit[12]   : 保留 */
        unsigned int  reserved_4: 4;  /* bit[13-16]: 保留 */
        unsigned int  reserved_5: 15; /* bit[17-31]: 保留。 */
    } reg;
} SOC_PCTRL_G3D_RASTER_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_USB2_HOST_CTRL0_UNION
 struct description   : USB2_HOST_CTRL0 Register structure definition
                        Address Offset:0x0004 Initial:0x00000000 Width:32
 register description : USB2 HOST控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_USB2_HOST_CTRL0_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_USB2_HOST_CTRL1_UNION
 struct description   : USB2_HOST_CTRL1 Register structure definition
                        Address Offset:0x0008 Initial:0x0000240C Width:32
 register description : USB2 HOST控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_USB2_HOST_CTRL1_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_USB2_HOST_CTRL2_UNION
 struct description   : USB2_HOST_CTRL2 Register structure definition
                        Address Offset:0x000C Initial:0x00000000 Width:32
 register description : USB2 HOST控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_USB2_HOST_CTRL2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL4_UNION
 struct description   : PERI_CTRL4 Register structure definition
                        Address Offset:0x0010 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_timeout_en               : 1;  /* bit[0]    : QIC timeout使能信号，1，打开，0 关闭。 */
        unsigned int  qic_timeout_en_tb_cfgdma2sys : 1;  /* bit[1]    :  */
        unsigned int  qic_timeout_en_ib_sys2cfgdma : 1;  /* bit[2]    :  */
        unsigned int  qic_timeout_en_tb_qice       : 1;  /* bit[3]    :  */
        unsigned int  qic_timeout_en_ib_ffts       : 1;  /* bit[4]    :  */
        unsigned int  qic_timeout_en_ib_mdmperi    : 1;  /* bit[5]    :  */
        unsigned int  qic_timeout_en_ib_mdm        : 1;  /* bit[6]    :  */
        unsigned int  qic_timeout_en_ib_npu        : 1;  /* bit[7]    :  */
        unsigned int  qic_timeout_en_ib_media2     : 1;  /* bit[8]    :  */
        unsigned int  qic_timeout_en_ib_media1     : 1;  /* bit[9]    :  */
        unsigned int  qic_timeout_en_ib_gpu        : 1;  /* bit[10]   :  */
        unsigned int  qic_timeout_en_ib_fcm        : 1;  /* bit[11]   :  */
        unsigned int  qic_timeout_en_ib_hsdt1      : 1;  /* bit[12]   :  */
        unsigned int  qic_timeout_en_ib_hsdt0      : 1;  /* bit[13]   :  */
        unsigned int  qic_timeout_en_ib_cfgdma     : 1;  /* bit[14]   :  */
        unsigned int  qic_timeout_en_ib_ao         : 1;  /* bit[15]   :  */
        unsigned int  peri_ctrl4_msk               : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                        与bit[15:0]命令位一一对应。
                                                                        0：屏蔽命令位，命令位写操作无效；
                                                                        1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL4_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_START                (0)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_END                  (0)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_tb_cfgdma2sys_START  (1)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_tb_cfgdma2sys_END    (1)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_sys2cfgdma_START  (2)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_sys2cfgdma_END    (2)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_tb_qice_START        (3)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_tb_qice_END          (3)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_ffts_START        (4)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_ffts_END          (4)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_mdmperi_START     (5)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_mdmperi_END       (5)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_mdm_START         (6)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_mdm_END           (6)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_npu_START         (7)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_npu_END           (7)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_media2_START      (8)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_media2_END        (8)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_media1_START      (9)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_media1_END        (9)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_gpu_START         (10)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_gpu_END           (10)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_fcm_START         (11)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_fcm_END           (11)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_hsdt1_START       (12)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_hsdt1_END         (12)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_hsdt0_START       (13)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_hsdt0_END         (13)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_cfgdma_START      (14)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_cfgdma_END        (14)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_ao_START          (15)
#define SOC_PCTRL_PERI_CTRL4_qic_timeout_en_ib_ao_END            (15)
#define SOC_PCTRL_PERI_CTRL4_peri_ctrl4_msk_START                (16)
#define SOC_PCTRL_PERI_CTRL4_peri_ctrl4_msk_END                  (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL98_UNION
 struct description   : PERI_CTRL98 Register structure definition
                        Address Offset:0x0014 Initial:0x00 Width:32
 register description : 通用外设控制寄存器98。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                     : 1;  /* bit[0]    : reserved */
        unsigned int  reserved_1                     : 1;  /* bit[1]    : reserved */
        unsigned int  reserved_2                     : 1;  /* bit[2]    : reserved */
        unsigned int  reserved_3                     : 1;  /* bit[3]    : reserved */
        unsigned int  reserved_4                     : 1;  /* bit[4]    : reserved */
        unsigned int  reserved_5                     : 1;  /* bit[5]    : reserved */
        unsigned int  reserved_6                     : 1;  /* bit[6]    : reserved */
        unsigned int  ddr_xctrl_spua_ctrl_slp        : 1;  /* bit[7]    : ddr_xctrl spua mem ctrl的slp位置 */
        unsigned int  bfmux_ddr_xctrl_spua_ctrl_dslp : 1;  /* bit[8]    : ddrx_ctrl spua mem ctrl的dslp位置 */
        unsigned int  ipf_spram_ctrl_slp             : 1;  /* bit[9]    : ipf spram mem ctrl的slp位置 */
        unsigned int  bfmux_ipf_spram_ctrl_dslp      : 1;  /* bit[10]   : ipf spram mem ctrl的dslp位置 */
        unsigned int  reserved_7                     : 1;  /* bit[11]   : reserved */
        unsigned int  reserved_8                     : 1;  /* bit[12]   : reserved */
        unsigned int  reserved_9                     : 1;  /* bit[13]   : reserved */
        unsigned int  reserved_10                    : 1;  /* bit[14]   : reserved */
        unsigned int  reserved_11                    : 1;  /* bit[15]   : reserved */
        unsigned int  reserved_12                    : 1;  /* bit[16]   : reserved */
        unsigned int  reserved_13                    : 1;  /* bit[17]   : reserved */
        unsigned int  socp_spram_ctrl_slp            : 1;  /* bit[18]   : socp spram mem ctrl的slp位置 */
        unsigned int  bfmux_socp_spram_ctrl_dslp     : 1;  /* bit[19]   : socp spram mem ctrl的dslp位置 */
        unsigned int  reserved_14                    : 1;  /* bit[20]   : reserved */
        unsigned int  reserved_15                    : 1;  /* bit[21]   : reserved */
        unsigned int  reserved_16                    : 1;  /* bit[22]   : reserved */
        unsigned int  reserved_17                    : 1;  /* bit[23]   : reserved */
        unsigned int  reserved_18                    : 8;  /* bit[24-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL98_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL98_ddr_xctrl_spua_ctrl_slp_START         (7)
#define SOC_PCTRL_PERI_CTRL98_ddr_xctrl_spua_ctrl_slp_END           (7)
#define SOC_PCTRL_PERI_CTRL98_bfmux_ddr_xctrl_spua_ctrl_dslp_START  (8)
#define SOC_PCTRL_PERI_CTRL98_bfmux_ddr_xctrl_spua_ctrl_dslp_END    (8)
#define SOC_PCTRL_PERI_CTRL98_ipf_spram_ctrl_slp_START              (9)
#define SOC_PCTRL_PERI_CTRL98_ipf_spram_ctrl_slp_END                (9)
#define SOC_PCTRL_PERI_CTRL98_bfmux_ipf_spram_ctrl_dslp_START       (10)
#define SOC_PCTRL_PERI_CTRL98_bfmux_ipf_spram_ctrl_dslp_END         (10)
#define SOC_PCTRL_PERI_CTRL98_socp_spram_ctrl_slp_START             (18)
#define SOC_PCTRL_PERI_CTRL98_socp_spram_ctrl_slp_END               (18)
#define SOC_PCTRL_PERI_CTRL98_bfmux_socp_spram_ctrl_dslp_START      (19)
#define SOC_PCTRL_PERI_CTRL98_bfmux_socp_spram_ctrl_dslp_END        (19)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL99_UNION
 struct description   : PERI_CTRL99 Register structure definition
                        Address Offset:0x0018 Initial:0x0 Width:32
 register description : 通用外设控制寄存器99。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipf_tprf_ctrl_slp              : 1;  /* bit[0]    : ipf tprf mem ctrl的ls位置， */
        unsigned int  bfmux_ipf_tprf_ctrl_dslp       : 1;  /* bit[1]    : ipf tprf mem ctrl的ds位置， */
        unsigned int  reserved_0                     : 1;  /* bit[2]    : 保留 */
        unsigned int  socp_tprf_ctrl_slp             : 1;  /* bit[3]    : socp tprf mem ctrl的ls位置， */
        unsigned int  bfmux_socp_tprf_ctrl_dslp      : 1;  /* bit[4]    : socp tprf mem ctrl的ds位置， */
        unsigned int  reserved_1                     : 1;  /* bit[5]    : 保留 */
        unsigned int  peri_edmac_tpl_ctrl_slp        : 1;  /* bit[6]    : peri_edmac tpl mem ctrl的ls位置， */
        unsigned int  bfmux_peri_edmac_tpl_ctrl_dslp : 1;  /* bit[7]    : peri_edmac tpl mem ctrl的ds位置， */
        unsigned int  reserved_2                     : 1;  /* bit[8]    : 保留 */
        unsigned int  spi_tpram_ctrl_slp             : 1;  /* bit[9]    : PERI区的spi memctrl信号的 tpram ctrl的ls位置 */
        unsigned int  bfmux_spi_tpram_ctrl_dslp      : 1;  /* bit[10]   : PERI区的spi memctrl信号的 tpram ctrl的ds位置 */
        unsigned int  reserved_3                     : 1;  /* bit[11]   :  */
        unsigned int  reserved_4                     : 1;  /* bit[12]   :  */
        unsigned int  spe_spram_ctrl_slp             : 1;  /* bit[13]   : spe SPRAM 的LS位置 */
        unsigned int  bfmux_spe_spram_ctrl_dslp      : 1;  /* bit[14]   : spe SPRAM 的DS位置 */
        unsigned int  spe_tpram_ctrl_slp             : 1;  /* bit[15]   : spe TPRAM 的LS位置 */
        unsigned int  bfmux_spe_tpram_ctrl_dslp      : 1;  /* bit[16]   : spe TPRAM 的DS位置 */
        unsigned int  maa_spram_ctrl_slp             : 1;  /* bit[17]   : MAA SPRAM 的LS位置 */
        unsigned int  bfmux_maa_spram_ctrl_dslp      : 1;  /* bit[18]   : MAA SPRAM 的DS位置 */
        unsigned int  maa_tpram_ctrl_slp             : 1;  /* bit[19]   : MAA TPRAM 的LS位置 */
        unsigned int  bfmux_maa_tpram_ctrl_dslp      : 1;  /* bit[20]   : MAA TPRAM 的DS位置 */
        unsigned int  reserved_5                     : 1;  /* bit[21]   :  */
        unsigned int  reserved_6                     : 1;  /* bit[22]   :  */
        unsigned int  reserved_7                     : 1;  /* bit[23]   :  */
        unsigned int  reserved_8                     : 1;  /* bit[24]   :  */
        unsigned int  eicc_spram_ctrl_slp            : 1;  /* bit[25]   : EICC SPRAM 的LS位置 */
        unsigned int  bfmux_eicc_spram_ctrl_dslp     : 1;  /* bit[26]   : EICC SPRAM 的DS位置 */
        unsigned int  eicc_tpram_ctrl_slp            : 1;  /* bit[27]   : EICC TPRAM 的LS位置 */
        unsigned int  bfmux_eicc_tpram_ctrl_dslp     : 1;  /* bit[28]   : EICC TPRAM 的DS位置 */
        unsigned int  reserved_9                     : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL99_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL99_ipf_tprf_ctrl_slp_START               (0)
#define SOC_PCTRL_PERI_CTRL99_ipf_tprf_ctrl_slp_END                 (0)
#define SOC_PCTRL_PERI_CTRL99_bfmux_ipf_tprf_ctrl_dslp_START        (1)
#define SOC_PCTRL_PERI_CTRL99_bfmux_ipf_tprf_ctrl_dslp_END          (1)
#define SOC_PCTRL_PERI_CTRL99_socp_tprf_ctrl_slp_START              (3)
#define SOC_PCTRL_PERI_CTRL99_socp_tprf_ctrl_slp_END                (3)
#define SOC_PCTRL_PERI_CTRL99_bfmux_socp_tprf_ctrl_dslp_START       (4)
#define SOC_PCTRL_PERI_CTRL99_bfmux_socp_tprf_ctrl_dslp_END         (4)
#define SOC_PCTRL_PERI_CTRL99_peri_edmac_tpl_ctrl_slp_START         (6)
#define SOC_PCTRL_PERI_CTRL99_peri_edmac_tpl_ctrl_slp_END           (6)
#define SOC_PCTRL_PERI_CTRL99_bfmux_peri_edmac_tpl_ctrl_dslp_START  (7)
#define SOC_PCTRL_PERI_CTRL99_bfmux_peri_edmac_tpl_ctrl_dslp_END    (7)
#define SOC_PCTRL_PERI_CTRL99_spi_tpram_ctrl_slp_START              (9)
#define SOC_PCTRL_PERI_CTRL99_spi_tpram_ctrl_slp_END                (9)
#define SOC_PCTRL_PERI_CTRL99_bfmux_spi_tpram_ctrl_dslp_START       (10)
#define SOC_PCTRL_PERI_CTRL99_bfmux_spi_tpram_ctrl_dslp_END         (10)
#define SOC_PCTRL_PERI_CTRL99_spe_spram_ctrl_slp_START              (13)
#define SOC_PCTRL_PERI_CTRL99_spe_spram_ctrl_slp_END                (13)
#define SOC_PCTRL_PERI_CTRL99_bfmux_spe_spram_ctrl_dslp_START       (14)
#define SOC_PCTRL_PERI_CTRL99_bfmux_spe_spram_ctrl_dslp_END         (14)
#define SOC_PCTRL_PERI_CTRL99_spe_tpram_ctrl_slp_START              (15)
#define SOC_PCTRL_PERI_CTRL99_spe_tpram_ctrl_slp_END                (15)
#define SOC_PCTRL_PERI_CTRL99_bfmux_spe_tpram_ctrl_dslp_START       (16)
#define SOC_PCTRL_PERI_CTRL99_bfmux_spe_tpram_ctrl_dslp_END         (16)
#define SOC_PCTRL_PERI_CTRL99_maa_spram_ctrl_slp_START              (17)
#define SOC_PCTRL_PERI_CTRL99_maa_spram_ctrl_slp_END                (17)
#define SOC_PCTRL_PERI_CTRL99_bfmux_maa_spram_ctrl_dslp_START       (18)
#define SOC_PCTRL_PERI_CTRL99_bfmux_maa_spram_ctrl_dslp_END         (18)
#define SOC_PCTRL_PERI_CTRL99_maa_tpram_ctrl_slp_START              (19)
#define SOC_PCTRL_PERI_CTRL99_maa_tpram_ctrl_slp_END                (19)
#define SOC_PCTRL_PERI_CTRL99_bfmux_maa_tpram_ctrl_dslp_START       (20)
#define SOC_PCTRL_PERI_CTRL99_bfmux_maa_tpram_ctrl_dslp_END         (20)
#define SOC_PCTRL_PERI_CTRL99_eicc_spram_ctrl_slp_START             (25)
#define SOC_PCTRL_PERI_CTRL99_eicc_spram_ctrl_slp_END               (25)
#define SOC_PCTRL_PERI_CTRL99_bfmux_eicc_spram_ctrl_dslp_START      (26)
#define SOC_PCTRL_PERI_CTRL99_bfmux_eicc_spram_ctrl_dslp_END        (26)
#define SOC_PCTRL_PERI_CTRL99_eicc_tpram_ctrl_slp_START             (27)
#define SOC_PCTRL_PERI_CTRL99_eicc_tpram_ctrl_slp_END               (27)
#define SOC_PCTRL_PERI_CTRL99_bfmux_eicc_tpram_ctrl_dslp_START      (28)
#define SOC_PCTRL_PERI_CTRL99_bfmux_eicc_tpram_ctrl_dslp_END        (28)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL100_UNION
 struct description   : PERI_CTRL100 Register structure definition
                        Address Offset:0x001C Initial:0x000000 Width:32
 register description : 通用外设控制寄存器100。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bootrom_rom_ctrl : 8;  /* bit[0-7] : bootrom rom ctrl
                                                           7:6 无意义，不使用
                                                           5：TEST 默认为0
                                                           4: TME 默认为0
                                                           3:0:RM 默认为0110 */
        unsigned int  reserved         : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL100_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_START  (0)
#define SOC_PCTRL_PERI_CTRL100_bootrom_rom_ctrl_END    (7)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL101_UNION
 struct description   : PERI_CTRL101 Register structure definition
                        Address Offset:0x0020 Initial:0x00 Width:32
 register description : 通用外设控制寄存器101。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0 : 3;  /* bit[0-2]  : 保留 */
        unsigned int  spua_tselr : 3;  /* bit[3-5]  : SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spua_tselw : 2;  /* bit[6-7]  : SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spua_test  : 3;  /* bit[8-10] : SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  rf_tselr   : 3;  /* bit[11-13]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  rf_tselw   : 2;  /* bit[14-15]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spua_tra   : 2;  /* bit[16-17]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spua_twa   : 2;  /* bit[18-19]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spua_twac  : 2;  /* bit[20-21]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spea_tra   : 2;  /* bit[22-23]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  spea_twac  : 2;  /* bit[24-25]: SPUA的控制信号，具体参考memory控制说明。 */
        unsigned int  reserved_1 : 6;  /* bit[26-31]: 保留给mem ctrl使用。 */
    } reg;
} SOC_PCTRL_PERI_CTRL101_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL101_spua_tselr_START  (3)
#define SOC_PCTRL_PERI_CTRL101_spua_tselr_END    (5)
#define SOC_PCTRL_PERI_CTRL101_spua_tselw_START  (6)
#define SOC_PCTRL_PERI_CTRL101_spua_tselw_END    (7)
#define SOC_PCTRL_PERI_CTRL101_spua_test_START   (8)
#define SOC_PCTRL_PERI_CTRL101_spua_test_END     (10)
#define SOC_PCTRL_PERI_CTRL101_rf_tselr_START    (11)
#define SOC_PCTRL_PERI_CTRL101_rf_tselr_END      (13)
#define SOC_PCTRL_PERI_CTRL101_rf_tselw_START    (14)
#define SOC_PCTRL_PERI_CTRL101_rf_tselw_END      (15)
#define SOC_PCTRL_PERI_CTRL101_spua_tra_START    (16)
#define SOC_PCTRL_PERI_CTRL101_spua_tra_END      (17)
#define SOC_PCTRL_PERI_CTRL101_spua_twa_START    (18)
#define SOC_PCTRL_PERI_CTRL101_spua_twa_END      (19)
#define SOC_PCTRL_PERI_CTRL101_spua_twac_START   (20)
#define SOC_PCTRL_PERI_CTRL101_spua_twac_END     (21)
#define SOC_PCTRL_PERI_CTRL101_spea_tra_START    (22)
#define SOC_PCTRL_PERI_CTRL101_spea_tra_END      (23)
#define SOC_PCTRL_PERI_CTRL101_spea_twac_START   (24)
#define SOC_PCTRL_PERI_CTRL101_spea_twac_END     (25)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL104_UNION
 struct description   : PERI_CTRL104 Register structure definition
                        Address Offset:0x0024 Initial:0x00000 Width:32
 register description : 通用外设控制寄存器104。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipf_spram_auto_clk_enable_mask  : 1;  /* bit[0]    : IPF 的 SPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  socp_spram_auto_clk_enable_mask : 1;  /* bit[1]    : SOCP 的 SPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  ipf_tpram_auto_clk_enable_mask  : 1;  /* bit[2]    : IPF 的 TPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  socp_tpram_auto_clk_enable_mask : 1;  /* bit[3]    : SOCP 的 TPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  reserved_0                      : 1;  /* bit[4]    :  */
        unsigned int  spe_spram_auto_clk_enable_mask  : 1;  /* bit[5]    : SPE 的 SPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  spe_tpram_auto_clk_enable_mask  : 1;  /* bit[6]    : SPE 的 TPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  maa_spram_auto_clk_enable_mask  : 1;  /* bit[7]    : MAA 的 SPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  maa_tpram_auto_clk_enable_mask  : 1;  /* bit[8]    : MAA 的 TPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  eicc_spram_auto_clk_enable_mask : 1;  /* bit[9]    : EICC 的 SPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  eicc_tpram_auto_clk_enable_mask : 1;  /* bit[10]   : EICC 的 TPRAM
                                                                           AP侧使用的modem提供的IP的memory，为其提供memory wrapper内部的ICG门控使能，默认为0：
                                                                           1'b0：ICG起作用；
                                                                           1'b1：ICG直通。 */
        unsigned int  reserved_1                      : 1;  /* bit[11]   :  */
        unsigned int  reserved_2                      : 1;  /* bit[12]   :  */
        unsigned int  reserved_3                      : 19; /* bit[13-31]: 保留给mem ctrl使用。 */
    } reg;
} SOC_PCTRL_PERI_CTRL104_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL104_ipf_spram_auto_clk_enable_mask_START   (0)
#define SOC_PCTRL_PERI_CTRL104_ipf_spram_auto_clk_enable_mask_END     (0)
#define SOC_PCTRL_PERI_CTRL104_socp_spram_auto_clk_enable_mask_START  (1)
#define SOC_PCTRL_PERI_CTRL104_socp_spram_auto_clk_enable_mask_END    (1)
#define SOC_PCTRL_PERI_CTRL104_ipf_tpram_auto_clk_enable_mask_START   (2)
#define SOC_PCTRL_PERI_CTRL104_ipf_tpram_auto_clk_enable_mask_END     (2)
#define SOC_PCTRL_PERI_CTRL104_socp_tpram_auto_clk_enable_mask_START  (3)
#define SOC_PCTRL_PERI_CTRL104_socp_tpram_auto_clk_enable_mask_END    (3)
#define SOC_PCTRL_PERI_CTRL104_spe_spram_auto_clk_enable_mask_START   (5)
#define SOC_PCTRL_PERI_CTRL104_spe_spram_auto_clk_enable_mask_END     (5)
#define SOC_PCTRL_PERI_CTRL104_spe_tpram_auto_clk_enable_mask_START   (6)
#define SOC_PCTRL_PERI_CTRL104_spe_tpram_auto_clk_enable_mask_END     (6)
#define SOC_PCTRL_PERI_CTRL104_maa_spram_auto_clk_enable_mask_START   (7)
#define SOC_PCTRL_PERI_CTRL104_maa_spram_auto_clk_enable_mask_END     (7)
#define SOC_PCTRL_PERI_CTRL104_maa_tpram_auto_clk_enable_mask_START   (8)
#define SOC_PCTRL_PERI_CTRL104_maa_tpram_auto_clk_enable_mask_END     (8)
#define SOC_PCTRL_PERI_CTRL104_eicc_spram_auto_clk_enable_mask_START  (9)
#define SOC_PCTRL_PERI_CTRL104_eicc_spram_auto_clk_enable_mask_END    (9)
#define SOC_PCTRL_PERI_CTRL104_eicc_tpram_auto_clk_enable_mask_START  (10)
#define SOC_PCTRL_PERI_CTRL104_eicc_tpram_auto_clk_enable_mask_END    (10)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL12_UNION
 struct description   : PERI_CTRL12 Register structure definition
                        Address Offset:0x0028 Initial:0x0 Width:32
 register description : 通用外设控制寄存器12。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0      : 3;  /* bit[0-2]  : 保留不使用。 */
        unsigned int  sphcsram_rtsel  : 3;  /* bit[3-5]  : sphcsram_rtsel SPS 具体参考memory控制说明。 */
        unsigned int  sphcsram_wtsel  : 2;  /* bit[6-7]  : sphcsram_wtsel SPS 具体参考memory控制说明。 */
        unsigned int  spsram_test     : 3;  /* bit[8-10] : spsram_test 具体参考memory控制说明。 */
        unsigned int  sphdsram_rtsel  : 3;  /* bit[11-13]: sphdsram_rtsel SPA 具体参考memory控制说明。 */
        unsigned int  sphdsram_wtsel  : 2;  /* bit[14-15]: sphdsram_wtsel SPA 具体参考memory控制说明。 */
        unsigned int  spsram_tra_peri : 2;  /* bit[16-17]: PERI电源域的SPRAM的TRA信号 具体参考memory控制说明。 */
        unsigned int  reserved_1      : 2;  /* bit[18-19]:  */
        unsigned int  reserved_2      : 2;  /* bit[20-21]:  */
        unsigned int  reserved_3      : 2;  /* bit[22-23]:  */
        unsigned int  reserved_4      : 2;  /* bit[24-25]:  */
        unsigned int  reserved_5      : 2;  /* bit[26-27]:  */
        unsigned int  reserved_6      : 3;  /* bit[28-30]:  */
        unsigned int  reserved_7      : 1;  /* bit[31]   : 保留不使用，但为了mem ctrl对应bit也禁止被其他信号占用。
                                                           SPS Spram的mem ctrl信号共26bit。 */
    } reg;
} SOC_PCTRL_PERI_CTRL12_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL12_sphcsram_rtsel_START   (3)
#define SOC_PCTRL_PERI_CTRL12_sphcsram_rtsel_END     (5)
#define SOC_PCTRL_PERI_CTRL12_sphcsram_wtsel_START   (6)
#define SOC_PCTRL_PERI_CTRL12_sphcsram_wtsel_END     (7)
#define SOC_PCTRL_PERI_CTRL12_spsram_test_START      (8)
#define SOC_PCTRL_PERI_CTRL12_spsram_test_END        (10)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_rtsel_START   (11)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_rtsel_END     (13)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_wtsel_START   (14)
#define SOC_PCTRL_PERI_CTRL12_sphdsram_wtsel_END     (15)
#define SOC_PCTRL_PERI_CTRL12_spsram_tra_peri_START  (16)
#define SOC_PCTRL_PERI_CTRL12_spsram_tra_peri_END    (17)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL13_UNION
 struct description   : PERI_CTRL13 Register structure definition
                        Address Offset:0x002C Initial:0x0 Width:32
 register description : 通用外设控制寄存器13。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0    : 4;  /* bit[0-3]  : 保留不使用。 */
        unsigned int  tprf_tselr    : 2;  /* bit[4-5]  : tprf_tselr 具体参考memory控制说明。 */
        unsigned int  tprf_tselw    : 2;  /* bit[6-7]  : tprf_tselw 具体参考memory控制说明。 */
        unsigned int  tprf_test     : 3;  /* bit[8-10] : tprf_test 具体参考memory控制说明。 */
        unsigned int  tprf_tra_peri : 2;  /* bit[11-12]: TPRAM的TRA信号 */
        unsigned int  reserved_1    : 9;  /* bit[13-21]: 保留 */
        unsigned int  tpl_tselr     : 2;  /* bit[22-23]: tpl_tselr 具体参考memory控制说明。 */
        unsigned int  tpl_tselw     : 2;  /* bit[24-25]: tpl_tselw 具体参考memory控制说明。 */
        unsigned int  tpl_test      : 3;  /* bit[26-28]: TPL类型寄存器的TEST信号 */
        unsigned int  tpl_tra_peri  : 2;  /* bit[29-30]: TPL类型寄存器的TRA信号 */
        unsigned int  reserved_2    : 1;  /* bit[31]   : 保留不使用，但为了mem ctrl对应bit也禁止被其他信号占用。
                                                         tprf的mem ctrl信号共16bit。 */
    } reg;
} SOC_PCTRL_PERI_CTRL13_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL13_tprf_tselr_START     (4)
#define SOC_PCTRL_PERI_CTRL13_tprf_tselr_END       (5)
#define SOC_PCTRL_PERI_CTRL13_tprf_tselw_START     (6)
#define SOC_PCTRL_PERI_CTRL13_tprf_tselw_END       (7)
#define SOC_PCTRL_PERI_CTRL13_tprf_test_START      (8)
#define SOC_PCTRL_PERI_CTRL13_tprf_test_END        (10)
#define SOC_PCTRL_PERI_CTRL13_tprf_tra_peri_START  (11)
#define SOC_PCTRL_PERI_CTRL13_tprf_tra_peri_END    (12)
#define SOC_PCTRL_PERI_CTRL13_tpl_tselr_START      (22)
#define SOC_PCTRL_PERI_CTRL13_tpl_tselr_END        (23)
#define SOC_PCTRL_PERI_CTRL13_tpl_tselw_START      (24)
#define SOC_PCTRL_PERI_CTRL13_tpl_tselw_END        (25)
#define SOC_PCTRL_PERI_CTRL13_tpl_test_START       (26)
#define SOC_PCTRL_PERI_CTRL13_tpl_test_END         (28)
#define SOC_PCTRL_PERI_CTRL13_tpl_tra_peri_START   (29)
#define SOC_PCTRL_PERI_CTRL13_tpl_tra_peri_END     (30)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL14_UNION
 struct description   : PERI_CTRL14 Register structure definition
                        Address Offset:0x0030 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器14。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  rom_dt   : 2;  /* bit[0-1]  : rom_dt 具体参考memory控制说明。 */
        unsigned int  rom_skp  : 2;  /* bit[2-3]  : rom_skp 具体参考memory控制说明。 */
        unsigned int  rom_ckle : 1;  /* bit[4]    : rom_ckle 具体参考memory控制说明。 */
        unsigned int  rom_ckhe : 1;  /* bit[5]    : rom_ckhe 具体参考memory控制说明。 */
        unsigned int  reserved_0: 11; /* bit[6-16] : 保留 */
        unsigned int  reserved_1: 1;  /* bit[17]   : 保留 */
        unsigned int  reserved_2: 14; /* bit[18-31]: 保留. */
    } reg;
} SOC_PCTRL_PERI_CTRL14_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL14_rom_dt_START    (0)
#define SOC_PCTRL_PERI_CTRL14_rom_dt_END      (1)
#define SOC_PCTRL_PERI_CTRL14_rom_skp_START   (2)
#define SOC_PCTRL_PERI_CTRL14_rom_skp_END     (3)
#define SOC_PCTRL_PERI_CTRL14_rom_ckle_START  (4)
#define SOC_PCTRL_PERI_CTRL14_rom_ckle_END    (4)
#define SOC_PCTRL_PERI_CTRL14_rom_ckhe_START  (5)
#define SOC_PCTRL_PERI_CTRL14_rom_ckhe_END    (5)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL15_UNION
 struct description   : PERI_CTRL15 Register structure definition
                        Address Offset:0x0034 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器15。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0     : 3;  /* bit[0-2]  : 保留 */
        unsigned int  bpram_tselr    : 3;  /* bit[3-5]  : BPRAM的调节读，默认011 */
        unsigned int  bpram_tselw    : 2;  /* bit[6-7]  : BPRAM的调节写，默认01 */
        unsigned int  bpram_test     : 3;  /* bit[8-10] : BPARM的测试pin，默认000 */
        unsigned int  bpram_tra_peri : 2;  /* bit[11-12]: PERI电源域的BPRAM的TRA信号 具体参考memory控制说明。 */
        unsigned int  reserved_1     : 1;  /* bit[13]   : 保留 */
        unsigned int  bpram_tselm    : 2;  /* bit[14-15]: BPRAM的selm信号，默认01 */
        unsigned int  reserved_2     : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL15_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL15_bpram_tselr_START     (3)
#define SOC_PCTRL_PERI_CTRL15_bpram_tselr_END       (5)
#define SOC_PCTRL_PERI_CTRL15_bpram_tselw_START     (6)
#define SOC_PCTRL_PERI_CTRL15_bpram_tselw_END       (7)
#define SOC_PCTRL_PERI_CTRL15_bpram_test_START      (8)
#define SOC_PCTRL_PERI_CTRL15_bpram_test_END        (10)
#define SOC_PCTRL_PERI_CTRL15_bpram_tra_peri_START  (11)
#define SOC_PCTRL_PERI_CTRL15_bpram_tra_peri_END    (12)
#define SOC_PCTRL_PERI_CTRL15_bpram_tselm_START     (14)
#define SOC_PCTRL_PERI_CTRL15_bpram_tselm_END       (15)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL16_UNION
 struct description   : PERI_CTRL16 Register structure definition
                        Address Offset:0x0038 Initial:0x000000 Width:32
 register description : 通用外设控制寄存器16。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spi1_cs_sel : 4;  /* bit[0-3] : SPI1片选输出选择控制。
                                                      0b0000: 无CS输出。
                                                      0b0001: CS0
                                                      0b0010: CS1
                                                      others：非法值
                                                      phoenixES，cs2/3已不需要，无效。 */
        unsigned int  spi6_cs_sel : 4;  /* bit[4-7] : SPI6片选输出选择控制。
                                                      0b0000: 无CS输出。
                                                      0b0001: CS0
                                                      0b0010: CS1
                                                      others：非法值
                                                      phoenixES，cs2/3已不需要，无效。 */
        unsigned int  reserved    : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL16_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL16_spi1_cs_sel_START  (0)
#define SOC_PCTRL_PERI_CTRL16_spi1_cs_sel_END    (3)
#define SOC_PCTRL_PERI_CTRL16_spi6_cs_sel_START  (4)
#define SOC_PCTRL_PERI_CTRL16_spi6_cs_sel_END    (7)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL17_UNION
 struct description   : PERI_CTRL17 Register structure definition
                        Address Offset:0x003C Initial:0x0000000 Width:32
 register description : 通用外设控制寄存器17。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ckg_byp_rs_dmca : 1;  /* bit[0]   : dmca ahb 自动时钟门控bypass信号 */
        unsigned int  ckg_byp_rs_dmcb : 1;  /* bit[1]   : dmcb ahb 自动时钟门控bypass信号 */
        unsigned int  ckg_byp_rs_dmcc : 1;  /* bit[2]   : dmcc ahb 自动时钟门控bypass信号 */
        unsigned int  ckg_byp_rs_dmcd : 1;  /* bit[3]   : dmcd ahb 自动时钟门控bypass信号 */
        unsigned int  reserved        : 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL17_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmca_START  (0)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmca_END    (0)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmcb_START  (1)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmcb_END    (1)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmcc_START  (2)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmcc_END    (2)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmcd_START  (3)
#define SOC_PCTRL_PERI_CTRL17_ckg_byp_rs_dmcd_END    (3)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL18_UNION
 struct description   : PERI_CTRL18 Register structure definition
                        Address Offset:0x0040 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器18。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pwr_dwn_req_cdphy0 : 1;  /* bit[0]   : MIPI TX PHY0下电配置 default:0X0
                                                             0 - 上电模式；
                                                             1 - 进入下电模式。 */
        unsigned int  pwr_dwn_req_cdphy1 : 1;  /* bit[1]   : MIPI TX PHY1下电配置 default:0X0
                                                             0 - 上电模式；
                                                             1 - 进入下电模式。 */
        unsigned int  reserved           : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL18_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_cdphy0_START  (0)
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_cdphy0_END    (0)
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_cdphy1_START  (1)
#define SOC_PCTRL_PERI_CTRL18_pwr_dwn_req_cdphy1_END    (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL19_UNION
 struct description   : PERI_CTRL19 Register structure definition
                        Address Offset:0x0044 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器19。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL19_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL20_UNION
 struct description   : PERI_CTRL20 Register structure definition
                        Address Offset:0x0048 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器20。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  uart0_1wire_en : 1;  /* bit[0]   : PERI UART0的1wire功能使能，1表示使能。 */
        unsigned int  reserved_0     : 1;  /* bit[1]   : 保留。 */
        unsigned int  reserved_1     : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL20_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL20_uart0_1wire_en_START  (0)
#define SOC_PCTRL_PERI_CTRL20_uart0_1wire_en_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL21_UNION
 struct description   : PERI_CTRL21 Register structure definition
                        Address Offset:0x004C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器21。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipc_ns_gt_clk_bypass      : 1;  /* bit[0]   : IPC NS内部自动门控bypass控制，为1时钟常开； */
        unsigned int  ipc_rtos_ns_gt_clk_bypass : 1;  /* bit[1]   : IPC_RTOS内部自动门控bypass控制，为1时钟常开 */
        unsigned int  reserved                  : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL21_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL21_ipc_ns_gt_clk_bypass_START       (0)
#define SOC_PCTRL_PERI_CTRL21_ipc_ns_gt_clk_bypass_END         (0)
#define SOC_PCTRL_PERI_CTRL21_ipc_rtos_ns_gt_clk_bypass_START  (1)
#define SOC_PCTRL_PERI_CTRL21_ipc_rtos_ns_gt_clk_bypass_END    (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL22_UNION
 struct description   : PERI_CTRL22 Register structure definition
                        Address Offset:0x0050 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器22。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dmss_bus_buff_en : 1;  /* bit[0]   : 此信号实际没有用，但是由于集成本阶段端口不动，需要保留。CharlotteES */
        unsigned int  dmc_bus_buff_en  : 1;  /* bit[1]   : LPMCU配置DMC通路，bufferable使能；
                                                           0：bufferable disable；
                                                           1：bufferable enable。 */
        unsigned int  reserved         : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL22_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL22_dmss_bus_buff_en_START  (0)
#define SOC_PCTRL_PERI_CTRL22_dmss_bus_buff_en_END    (0)
#define SOC_PCTRL_PERI_CTRL22_dmc_bus_buff_en_START   (1)
#define SOC_PCTRL_PERI_CTRL22_dmc_bus_buff_en_END     (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL23_UNION
 struct description   : PERI_CTRL23 Register structure definition
                        Address Offset:0x0054 Initial:0x0 Width:32
 register description : 通用外设控制寄存器23。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0             : 3;  /* bit[0-2]  : 保留。 */
        unsigned int  sc_noc_djtag_mst_pstrb : 4;  /* bit[3-6]  : bit[6:3]:djtag mst pstrb default:0xF 
                                                                  具体参考djtag调试相关说明。 */
        unsigned int  reserved_1             : 3;  /* bit[7-9]  : 保留 */
        unsigned int  reserved_2             : 1;  /* bit[10]   : 保留 */
        unsigned int  sc_dmac_ckgt_dis       : 1;  /* bit[11]   : dmac_ckgt_dis
                                                                  0：clk sc vote disable 1：clk sc vote enable。 */
        unsigned int  reserved_3             : 1;  /* bit[12]   :  */
        unsigned int  reserved_4             : 1;  /* bit[13]   : 保留。 */
        unsigned int  reserved_5             : 1;  /* bit[14]   : 保留。 */
        unsigned int  reserved_6             : 1;  /* bit[15]   : 保留。 */
        unsigned int  reserved_7             : 3;  /* bit[16-18]: 保留。 */
        unsigned int  reserved_8             : 1;  /* bit[19]   : 保留。 */
        unsigned int  reserved_9             : 3;  /* bit[20-22]:  */
        unsigned int  reserved_10            : 1;  /* bit[23]   :  */
        unsigned int  reserved_11            : 1;  /* bit[24]   :  */
        unsigned int  reserved_12            : 4;  /* bit[25-28]: 保留 */
        unsigned int  reserved_13            : 3;  /* bit[29-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL23_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL23_sc_noc_djtag_mst_pstrb_START  (3)
#define SOC_PCTRL_PERI_CTRL23_sc_noc_djtag_mst_pstrb_END    (6)
#define SOC_PCTRL_PERI_CTRL23_sc_dmac_ckgt_dis_START        (11)
#define SOC_PCTRL_PERI_CTRL23_sc_dmac_ckgt_dis_END          (11)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL24_UNION
 struct description   : PERI_CTRL24 Register structure definition
                        Address Offset:0x0058 Initial:0x000000 Width:32
 register description : 通用外设控制寄存器24。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  nopending_mux_sel1 : 5;  /* bit[0-4]  : 可维测性提升逻辑。
                                                              将原32bit（按照给pctrl回读的分组）的nopendingtrans信号根据实际需要做mux后选出所需的一组nopendingtrans信号，所使用的mux选择信号。
                                                              
                                                              配置效果如sel0 */
        unsigned int  nopending_mux_sel0 : 5;  /* bit[5-9]  : 可维测性提升逻辑。
                                                              将原32bit（按照给pctrl回读的分组）的nopendingtrans信号根据实际需要做mux后选出所需的一组nopendingtrans信号，所使用的mux选择信号。
                                                              
                                                              配置为一下值分别对应PCTRL的如下寄存器内的nopending信号排布。
                                                              0 PERI_STAT53
                                                              1 PERI_STAT54
                                                              2 PERI_STAT55
                                                              3 PERI_STAT56
                                                              4 PERI_STAT57
                                                              
                                                              配置为其他值，未接实际信号。 */
        unsigned int  reserved           : 22; /* bit[10-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL24_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL24_nopending_mux_sel1_START  (0)
#define SOC_PCTRL_PERI_CTRL24_nopending_mux_sel1_END    (4)
#define SOC_PCTRL_PERI_CTRL24_nopending_mux_sel0_START  (5)
#define SOC_PCTRL_PERI_CTRL24_nopending_mux_sel0_END    (9)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL25_UNION
 struct description   : PERI_CTRL25 Register structure definition
                        Address Offset:0x005C Initial:0x00 Width:32
 register description : 通用外设控制寄存器25。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  nopending_lpcm_mux_sel : 3;  /* bit[0-2]  : 可维测性提升逻辑。
                                                                  将原32bit（按照给pctrl回读的分组）的nopending lpcm信号根据实际需要做mux后选出所需的一组信号，所使用的mux选择信号。
                                                                  
                                                                  配置为：
                                                                  0：对应排布同PCTRL里寄存器PERI_STAT58。
                                                                  配置为其他值，未接实际信号。 */
        unsigned int  reserved_0             : 3;  /* bit[3-5]  :  */
        unsigned int  idlereq_mux_sel1       : 1;  /* bit[6]    : 可维测性提升逻辑。
                                                                  idlereq涉及上电&下电两种场景，因此要取反后做个mux。
                                                                  1'b0:取反前。
                                                                  1'b1:取反后。 */
        unsigned int  idleack_mux_sel1       : 1;  /* bit[7]    : 可维测性提升逻辑。
                                                                  idleack涉及上电&下电两种场景，因此要取反后做个mux。
                                                                  1'b0:取反前。
                                                                  1'b1:取反后。 */
        unsigned int  reserved_1             : 1;  /* bit[8]    : 保留 */
        unsigned int  idlereq_mux_sel0       : 3;  /* bit[9-11] : 可维测性提升逻辑。
                                                                  将原16bit（按照给pmc回读的分组）的idlereq信号根据实际需要做mux后选出所需的一组idlereq信号，所使用的mux选择信号。（CharlotteES实际没有使用） */
        unsigned int  idleack_mux_sel0       : 3;  /* bit[12-14]: 可维测性提升逻辑。
                                                                  选出一组16bit的idleack信号的mux选择信号。（CharlotteES实际没有使用） */
        unsigned int  busy_mux_sel           : 5;  /* bit[15-19]: 可维测性提升逻辑。
                                                                  选出一组32bit的busy信号的mux选择信号。
                                                                  配置为
                                                                  0 dfx_busy_ib_cfg_dma
                                                                  1 dfx_busy_tb_cfg_dma
                                                                  2 dfx_busy_cfg_req_cfg_dma
                                                                  3 dfx_busy_cfg_rsp_cfg_dma
                                                                  4 dfx_busy_ib_mdm_peri
                                                                  5 dfx_busy_tb_mdm_peri
                                                                  6 dfx_busy_cfg_req_mdm_peri
                                                                  7 dfx_busy_cfg_rsp_mdm_peri
                                                                  8 dfx_busy_ib_sys
                                                                  9 dfx_busy_tb_sys
                                                                  10 dfx_busy_cfg_req_sys
                                                                  11 dfx_busy_cfg_rsp_sys
                                                                  12 qic_peri_busy
                                                                  配置为其他值，未接实际信号。 */
        unsigned int  intr_qic_mux_sel       : 5;  /* bit[20-24]: 可维测性提升逻辑。
                                                                  选出一组32bit的intr信号的mux选择信号。
                                                                  默认选择grp31的汇总后中断信号，所以默认值为5'h1f.
                                                                  配置为:
                                                                  0 intr_errprob_cfg_dma
                                                                  1 intr_errprob_mdm_peri
                                                                  2 intr_errprob_sys
                                                                  3 intr_transprob_cfg_dma
                                                                  4 intr_transprob_mdm_peri
                                                                  5 intr_transprob_sys
                                                                  6 intr_latcy_cfg_dma
                                                                  7 intr_latcy_mdm_peri
                                                                  8 intr_latcy_sys
                                                                  9 intr_cfg_req_cfg_dma
                                                                  10 intr_cfg_rsp_cfg_dma
                                                                  11 intr_cfg_req_mdm_peri
                                                                  12 intr_cfg_rsp_mdm_peri
                                                                  13 intr_cfg_req_sys
                                                                  14 intr_cfg_rsp_sys
                                                                  15 intr_safety_cfg_dma
                                                                  16 intr_safety_err_cfg_dma
                                                                  17 intr_safety_mdm_peri
                                                                  18 intr_safety_err_mdm_peri
                                                                  19 intr_safety_sys
                                                                  20 intr_safety_err_sys
                                                                  21 intr_qic_bus_peri 
                                                                  31 {28‘b0， qic_peri_comb_intr_lpmcu_acpu，intr_qic_probe_modemperi，intr_qic_probe_sysbus， intr_qic_probe_cfg_dma}
                                                                  配置为其他值，未接实际信号。 */
        unsigned int  reserved_2             : 7;  /* bit[25-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL25_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL25_nopending_lpcm_mux_sel_START  (0)
#define SOC_PCTRL_PERI_CTRL25_nopending_lpcm_mux_sel_END    (2)
#define SOC_PCTRL_PERI_CTRL25_idlereq_mux_sel1_START        (6)
#define SOC_PCTRL_PERI_CTRL25_idlereq_mux_sel1_END          (6)
#define SOC_PCTRL_PERI_CTRL25_idleack_mux_sel1_START        (7)
#define SOC_PCTRL_PERI_CTRL25_idleack_mux_sel1_END          (7)
#define SOC_PCTRL_PERI_CTRL25_idlereq_mux_sel0_START        (9)
#define SOC_PCTRL_PERI_CTRL25_idlereq_mux_sel0_END          (11)
#define SOC_PCTRL_PERI_CTRL25_idleack_mux_sel0_START        (12)
#define SOC_PCTRL_PERI_CTRL25_idleack_mux_sel0_END          (14)
#define SOC_PCTRL_PERI_CTRL25_busy_mux_sel_START            (15)
#define SOC_PCTRL_PERI_CTRL25_busy_mux_sel_END              (19)
#define SOC_PCTRL_PERI_CTRL25_intr_qic_mux_sel_START        (20)
#define SOC_PCTRL_PERI_CTRL25_intr_qic_mux_sel_END          (24)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL26_UNION
 struct description   : PERI_CTRL26 Register structure definition
                        Address Offset:0x0060 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器26。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  idlereq_mask : 16; /* bit[0-15] : 可维测性提升逻辑
                                                        用于idlereq mask锁存使能信号的信号源，默认值为0（即mask） */
        unsigned int  idleack_mask : 16; /* bit[16-31]: 可维测性提升逻辑
                                                        用于idleack mask锁存使能信号的信号源，默认值为0（即mask） */
    } reg;
} SOC_PCTRL_PERI_CTRL26_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL26_idlereq_mask_START  (0)
#define SOC_PCTRL_PERI_CTRL26_idlereq_mask_END    (15)
#define SOC_PCTRL_PERI_CTRL26_idleack_mask_START  (16)
#define SOC_PCTRL_PERI_CTRL26_idleack_mask_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL27_UNION
 struct description   : PERI_CTRL27 Register structure definition
                        Address Offset:0x0064 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器27。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_pcie_ap_sys_sel : 1;  /* bit[0]   : intr_pcie_ap_sys_inta_gic_lpmcu pcie0/1信号的sel信号 */
        unsigned int  reserved             : 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL27_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL27_intr_pcie_ap_sys_sel_START  (0)
#define SOC_PCTRL_PERI_CTRL27_intr_pcie_ap_sys_sel_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL28_UNION
 struct description   : PERI_CTRL28 Register structure definition
                        Address Offset:0x0068 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器28。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  nopending_lpcm_mask : 32; /* bit[0-31]: 可维测性提升逻辑
                                                              用于nopending lpcm mask锁存使能信号的信号源，默认值为0（即mask） */
    } reg;
} SOC_PCTRL_PERI_CTRL28_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL28_nopending_lpcm_mask_START  (0)
#define SOC_PCTRL_PERI_CTRL28_nopending_lpcm_mask_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL29_UNION
 struct description   : PERI_CTRL29 Register structure definition
                        Address Offset:0x006C Initial:0x0000000F Width:32
 register description : 通用外设控制寄存器29。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_qic_mask : 32; /* bit[0-31]: 可维测性提升逻辑
                                                        用于errorprobe mask锁存使能信号的信号源，默认值为0（即mask）
                                                        
                                                        默认选择grp31的低4bit信号，所以默认值低4bit不mask。 */
    } reg;
} SOC_PCTRL_PERI_CTRL29_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL29_intr_qic_mask_START  (0)
#define SOC_PCTRL_PERI_CTRL29_intr_qic_mask_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL30_UNION
 struct description   : PERI_CTRL30 Register structure definition
                        Address Offset:0x0070 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器30。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  busy_mask : 32; /* bit[0-31]: 可维测性提升逻辑
                                                    用于busy mask锁存使能信号的信号源，默认值为0（即mask） */
    } reg;
} SOC_PCTRL_PERI_CTRL30_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL30_busy_mask_START  (0)
#define SOC_PCTRL_PERI_CTRL30_busy_mask_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL31_UNION
 struct description   : PERI_CTRL31 Register structure definition
                        Address Offset:0x0074 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器31。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  nopending_mask0 : 32; /* bit[0-31]: 可维测性提升逻辑
                                                          用于nopending mask锁存使能信号的信号源，默认值为0（即mask） */
    } reg;
} SOC_PCTRL_PERI_CTRL31_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL31_nopending_mask0_START  (0)
#define SOC_PCTRL_PERI_CTRL31_nopending_mask0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL32_UNION
 struct description   : PERI_CTRL32 Register structure definition
                        Address Offset:0x0078 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器32。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  nopending_mask1 : 32; /* bit[0-31]: 可维测性提升逻辑
                                                          用于nopending mask锁存使能信号的信号源，默认值为0（即mask） */
    } reg;
} SOC_PCTRL_PERI_CTRL32_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL32_nopending_mask1_START  (0)
#define SOC_PCTRL_PERI_CTRL32_nopending_mask1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL33_UNION
 struct description   : PERI_CTRL33 Register structure definition
                        Address Offset:0x007C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器33。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL33_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL67_UNION
 struct description   : PERI_CTRL67 Register structure definition
                        Address Offset:0x0080 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器67。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_timeout_en_tb_mdmperi2sys : 1;  /* bit[0]    :  */
        unsigned int  qic_timeout_en_ib_sys2mdmperi : 1;  /* bit[1]    :  */
        unsigned int  reserved                      : 14; /* bit[2-15] :  */
        unsigned int  peri_ctrl67_msk               : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                         与bit[15:0]命令位一一对应。
                                                                         0：屏蔽命令位，命令位写操作无效；
                                                                         1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL67_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL67_qic_timeout_en_tb_mdmperi2sys_START  (0)
#define SOC_PCTRL_PERI_CTRL67_qic_timeout_en_tb_mdmperi2sys_END    (0)
#define SOC_PCTRL_PERI_CTRL67_qic_timeout_en_ib_sys2mdmperi_START  (1)
#define SOC_PCTRL_PERI_CTRL67_qic_timeout_en_ib_sys2mdmperi_END    (1)
#define SOC_PCTRL_PERI_CTRL67_peri_ctrl67_msk_START                (16)
#define SOC_PCTRL_PERI_CTRL67_peri_ctrl67_msk_END                  (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL68_UNION
 struct description   : PERI_CTRL68 Register structure definition
                        Address Offset:0x0084 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器68。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL68_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL69_UNION
 struct description   : PERI_CTRL69 Register structure definition
                        Address Offset:0x0088 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器69。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL69_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL70_UNION
 struct description   : PERI_CTRL70 Register structure definition
                        Address Offset:0x008C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器70。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL70_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL71_UNION
 struct description   : PERI_CTRL71 Register structure definition
                        Address Offset:0x0090 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器71。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL71_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL72_UNION
 struct description   : PERI_CTRL72 Register structure definition
                        Address Offset:0x0094 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器72。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 10; /* bit[0-9]  :  */
        unsigned int  reserved_1: 1;  /* bit[10]   :  */
        unsigned int  reserved_2: 4;  /* bit[11-14]:  */
        unsigned int  reserved_3: 1;  /* bit[15]   :  */
        unsigned int  reserved_4: 16; /* bit[16-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL72_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL73_UNION
 struct description   : PERI_CTRL73 Register structure definition
                        Address Offset:0x0098 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器73。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_reserved_ctrl_0 : 32; /* bit[0-31]: PCTRL保留寄存器0 */
    } reg;
} SOC_PCTRL_PERI_CTRL73_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL73_pctrl_reserved_ctrl_0_START  (0)
#define SOC_PCTRL_PERI_CTRL73_pctrl_reserved_ctrl_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL74_UNION
 struct description   : PERI_CTRL74 Register structure definition
                        Address Offset:0x009C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器74。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_reserved_ctrl_1 : 32; /* bit[0-31]: PCTRL保留寄存器1 */
    } reg;
} SOC_PCTRL_PERI_CTRL74_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL74_pctrl_reserved_ctrl_1_START  (0)
#define SOC_PCTRL_PERI_CTRL74_pctrl_reserved_ctrl_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL88_UNION
 struct description   : PERI_CTRL88 Register structure definition
                        Address Offset:0x00A0 Initial:0x0000FFFF Width:32
 register description : 通用外设控制寄存器88。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_reserved_ctrl_2 : 32; /* bit[0-31]: PCTRL保留寄存器2 */
    } reg;
} SOC_PCTRL_PERI_CTRL88_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL88_pctrl_reserved_ctrl_2_START  (0)
#define SOC_PCTRL_PERI_CTRL88_pctrl_reserved_ctrl_2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL89_UNION
 struct description   : PERI_CTRL89 Register structure definition
                        Address Offset:0x00A4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器89。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_comb_qice_mask : 32; /* bit[0-31]: qice 中断mask，0，非屏蔽，1，屏蔽。 */
    } reg;
} SOC_PCTRL_PERI_CTRL89_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL89_intr_comb_qice_mask_START  (0)
#define SOC_PCTRL_PERI_CTRL89_intr_comb_qice_mask_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL90_UNION
 struct description   : PERI_CTRL90 Register structure definition
                        Address Offset:0x00A8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器90。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ap2mdm_ctrl : 32; /* bit[0-31]: ap2mdm_ctrl[31:0]，详见mdm_debug文档。 */
    } reg;
} SOC_PCTRL_PERI_CTRL90_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL90_ap2mdm_ctrl_START  (0)
#define SOC_PCTRL_PERI_CTRL90_ap2mdm_ctrl_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL91_UNION
 struct description   : PERI_CTRL91 Register structure definition
                        Address Offset:0x00AC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器91。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL91_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL92_UNION
 struct description   : PERI_CTRL92 Register structure definition
                        Address Offset:0x00B0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器92。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL92_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL93_UNION
 struct description   : PERI_CTRL93 Register structure definition
                        Address Offset:0x00B4 Initial:0x0000007C Width:32
 register description : 通用外设控制寄存器93。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL93_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL95_UNION
 struct description   : PERI_CTRL95 Register structure definition
                        Address Offset:0x00B8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器95。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留（time en不与其他仪器使用寄存器，不使用。） */
    } reg;
} SOC_PCTRL_PERI_CTRL95_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL96_UNION
 struct description   : PERI_CTRL96 Register structure definition
                        Address Offset:0x00BC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器96。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL96_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL97_UNION
 struct description   : PERI_CTRL97 Register structure definition
                        Address Offset:0x00C0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器97。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL97_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL108_UNION
 struct description   : PERI_CTRL108 Register structure definition
                        Address Offset:0x00C4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器108。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL108_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL109_UNION
 struct description   : PERI_CTRL109 Register structure definition
                        Address Offset:0x00C8 Initial:0x00004858 Width:32
 register description : 通用外设控制寄存器109。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL109_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL110_UNION
 struct description   : PERI_CTRL110 Register structure definition
                        Address Offset:0x00CC Initial:0x00004858 Width:32
 register description : 通用外设控制寄存器110。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL110_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL111_UNION
 struct description   : PERI_CTRL111 Register structure definition
                        Address Offset:0x00D0 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器111。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_zq_ch_sel_ca   : 2;  /* bit[0-1]  : DMC ZQ选择控制信号 */
        unsigned int  cfg_zq_ch_sel_db   : 2;  /* bit[2-3]  : DMC ZQ选择控制信号 */
        unsigned int  cfg_zq_ch_sel_rsv0 : 2;  /* bit[4-5]  : DMC ZQ选择控制信号，保留 */
        unsigned int  cfg_zq_ch_sel_rsv1 : 1;  /* bit[6]    : DMC ZQ选择控制信号，保留 */
        unsigned int  icfg_ch_seq_rsv0   : 1;  /* bit[7]    : DMC ZQ INTLV 选择控制信号，保留 */
        unsigned int  icfg_ch_seq_rsv1   : 2;  /* bit[8-9]  : DMC ZQ INTLV 选择控制信号，保留 */
        unsigned int  icfg_cha_seq       : 2;  /* bit[10-11]: DMC ZQ INTLV 选择控制信号 */
        unsigned int  icfg_chb_seq       : 2;  /* bit[12-13]:  */
        unsigned int  icfg_chc_seq       : 2;  /* bit[14-15]:  */
        unsigned int  icfg_chd_seq       : 2;  /* bit[16-17]:  */
        unsigned int  reserved           : 14; /* bit[18-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL111_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_ca_START    (0)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_ca_END      (1)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_db_START    (2)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_db_END      (3)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_rsv0_START  (4)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_rsv0_END    (5)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_rsv1_START  (6)
#define SOC_PCTRL_PERI_CTRL111_cfg_zq_ch_sel_rsv1_END    (6)
#define SOC_PCTRL_PERI_CTRL111_icfg_ch_seq_rsv0_START    (7)
#define SOC_PCTRL_PERI_CTRL111_icfg_ch_seq_rsv0_END      (7)
#define SOC_PCTRL_PERI_CTRL111_icfg_ch_seq_rsv1_START    (8)
#define SOC_PCTRL_PERI_CTRL111_icfg_ch_seq_rsv1_END      (9)
#define SOC_PCTRL_PERI_CTRL111_icfg_cha_seq_START        (10)
#define SOC_PCTRL_PERI_CTRL111_icfg_cha_seq_END          (11)
#define SOC_PCTRL_PERI_CTRL111_icfg_chb_seq_START        (12)
#define SOC_PCTRL_PERI_CTRL111_icfg_chb_seq_END          (13)
#define SOC_PCTRL_PERI_CTRL111_icfg_chc_seq_START        (14)
#define SOC_PCTRL_PERI_CTRL111_icfg_chc_seq_END          (15)
#define SOC_PCTRL_PERI_CTRL111_icfg_chd_seq_START        (16)
#define SOC_PCTRL_PERI_CTRL111_icfg_chd_seq_END          (17)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL112_UNION
 struct description   : PERI_CTRL112 Register structure definition
                        Address Offset:0x00D4 Initial:0x0 Width:32
 register description : 通用外设控制寄存器112。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hsdt1_ufs_mphy_parallel_test_en : 1;  /* bit[0]   : HSDT1 UFS PHY/UFS MPHY并行测试使能信号，1为使能 */
        unsigned int  mudp_mphy_parallel_test_en      : 1;  /* bit[1]   : MUDP PHY/UFS MPHY并行测试使能信号，1为使能 */
        unsigned int  reserved_0                      : 29; /* bit[2-30]: 保留 */
        unsigned int  reserved_1                      : 1;  /* bit[31]  : 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL112_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL112_hsdt1_ufs_mphy_parallel_test_en_START  (0)
#define SOC_PCTRL_PERI_CTRL112_hsdt1_ufs_mphy_parallel_test_en_END    (0)
#define SOC_PCTRL_PERI_CTRL112_mudp_mphy_parallel_test_en_START       (1)
#define SOC_PCTRL_PERI_CTRL112_mudp_mphy_parallel_test_en_END         (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL113_UNION
 struct description   : PERI_CTRL113 Register structure definition
                        Address Offset:0x00D8 Initial:0x0000000 Width:32
 register description : 通用外设控制寄存器113。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  perfstat_mon_clkgt : 1;  /* bit[0]   : PERFSTAT MON监控总线门控使能寄存器，门控全系统PERF MON中监控总线时钟aclk。(v120新增)
                                                             用于测试环境下的功耗测试场景。默认不门控。商用环境中此寄存器失效
                                                             0：门控关闭
                                                             1：门控打开 */
        unsigned int  reserved_0         : 1;  /* bit[1]   :  */
        unsigned int  reserved_1         : 1;  /* bit[2]   :  */
        unsigned int  reserved_2         : 1;  /* bit[3]   :  */
        unsigned int  reserved_3         : 1;  /* bit[4]   :  */
        unsigned int  reserved_4         : 1;  /* bit[5]   :  */
        unsigned int  reserved_5         : 1;  /* bit[6]   :  */
        unsigned int  reserved_6         : 25; /* bit[7-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL113_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL113_perfstat_mon_clkgt_START  (0)
#define SOC_PCTRL_PERI_CTRL113_perfstat_mon_clkgt_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL114_UNION
 struct description   : PERI_CTRL114 Register structure definition
                        Address Offset:0x00DC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器114。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL114_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL115_UNION
 struct description   : PERI_CTRL115 Register structure definition
                        Address Offset:0x00E0 Initial:0x0 Width:32
 register description : 通用外设控制寄存器115。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipf_aw_lmtr_saturation : 14; /* bit[0-13] : 14bit bandwidth contrler 控制信号，给IPF AXI W的带宽控制功能的带宽容限配置：
                                                                  配置值=带宽容限(B)/16。 */
        unsigned int  ipf_aw_lmtr_bandwidth  : 13; /* bit[14-26]: 13bit bandwidth contrler 控制信号，给IPF AXI W的带宽控制功能的允许平均带宽配置：
                                                                  配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。 */
        unsigned int  ipf_aw_lmtr_en         : 1;  /* bit[27]   : bandwidth contrler 控制信号，给IPF AXI W的CFC使能：
                                                                  0：关闭；
                                                                  1：使能 */
        unsigned int  reserved               : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL115_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_saturation_START  (0)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_saturation_END    (13)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_bandwidth_START   (14)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_bandwidth_END     (26)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_en_START          (27)
#define SOC_PCTRL_PERI_CTRL115_ipf_aw_lmtr_en_END            (27)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL116_UNION
 struct description   : PERI_CTRL116 Register structure definition
                        Address Offset:0x00E4 Initial:0x0 Width:32
 register description : 通用外设控制寄存器116。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipf_ar_lmtr_saturation : 14; /* bit[0-13] : 14bit bandwidth contrler 控制信号，给IPF AXI R的带宽控制功能的带宽容限配置：
                                                                  配置值=带宽容限(B)/16。 */
        unsigned int  ipf_ar_lmtr_bandwidth  : 13; /* bit[14-26]: 13bit bandwidth contrler 控制信号，给IPF AXI R的带宽控制功能的允许平均带宽配置：
                                                                  配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。 */
        unsigned int  ipf_ar_lmtr_en         : 1;  /* bit[27]   : bandwidth contrler 控制信号，给IPF AXI R的CFC使能：
                                                                  0：关闭；
                                                                  1：使能 */
        unsigned int  reserved               : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL116_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_saturation_START  (0)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_saturation_END    (13)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_bandwidth_START   (14)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_bandwidth_END     (26)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_en_START          (27)
#define SOC_PCTRL_PERI_CTRL116_ipf_ar_lmtr_en_END            (27)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL117_UNION
 struct description   : PERI_CTRL117 Register structure definition
                        Address Offset:0x00E8 Initial:0x0 Width:32
 register description : 通用外设控制寄存器117。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dmac_aw_lmtr_saturation : 14; /* bit[0-13] : 14bit bandwidth contrler 控制信号，给DMAC AXI W的带宽控制功能的带宽容限配置：
                                                                   配置值=带宽容限(B)/16。 */
        unsigned int  dmac_aw_lmtr_bandwidth  : 13; /* bit[14-26]: 13bit bandwidth contrler 控制信号，给DMAC AXI W的带宽控制功能的允许平均带宽配置：
                                                                   配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。 */
        unsigned int  dmac_aw_lmtr_en         : 1;  /* bit[27]   : bandwidth contrler 控制信号，给DMAC AXI W的CFC使能：
                                                                   0：关闭；
                                                                   1：使能 */
        unsigned int  reserved                : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL117_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_saturation_START  (0)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_saturation_END    (13)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_bandwidth_START   (14)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_bandwidth_END     (26)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_en_START          (27)
#define SOC_PCTRL_PERI_CTRL117_dmac_aw_lmtr_en_END            (27)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL118_UNION
 struct description   : PERI_CTRL118 Register structure definition
                        Address Offset:0x00EC Initial:0x0 Width:32
 register description : 通用外设控制寄存器118。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dmac_ar_lmtr_saturation : 14; /* bit[0-13] : 14bit bandwidth contrler 控制信号，给DMAC AXI R的带宽控制功能的带宽容限配置：
                                                                   配置值=带宽容限(B)/16。 */
        unsigned int  dmac_ar_lmtr_bandwidth  : 13; /* bit[14-26]: 13bit bandwidth contrler 控制信号，给DMAC AXI R的带宽控制功能的允许平均带宽配置：
                                                                   配置值=允许平均带宽(B/Hz)*256=允许带宽(MB/s)/工作频率（MHz）*256。 */
        unsigned int  dmac_ar_lmtr_en         : 1;  /* bit[27]   : bandwidth contrler 控制信号，给DMAC AXI R的CFC使能：
                                                                   0：关闭；
                                                                   1：使能 */
        unsigned int  reserved                : 4;  /* bit[28-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL118_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_saturation_START  (0)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_saturation_END    (13)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_bandwidth_START   (14)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_bandwidth_END     (26)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_en_START          (27)
#define SOC_PCTRL_PERI_CTRL118_dmac_ar_lmtr_en_END            (27)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL119_UNION
 struct description   : PERI_CTRL119 Register structure definition
                        Address Offset:0x00F0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器119。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi1_avs_en : 32; /* bit[0-31]: SPMI1 avs_en;
                                                       bit0,mdm avs */
    } reg;
} SOC_PCTRL_PERI_CTRL119_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL119_spmi1_avs_en_START  (0)
#define SOC_PCTRL_PERI_CTRL119_spmi1_avs_en_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL120_UNION
 struct description   : PERI_CTRL120 Register structure definition
                        Address Offset:0x00F4 Initial:0x000 Width:32
 register description : 通用外设控制寄存器120。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spmi1_avs_addr0 : 20; /* bit[0-19] : SPMI1 avs addr0 */
        unsigned int  reserved        : 12; /* bit[20-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL120_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL120_spmi1_avs_addr0_START  (0)
#define SOC_PCTRL_PERI_CTRL120_spmi1_avs_addr0_END    (19)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL121_UNION
 struct description   : PERI_CTRL121 Register structure definition
                        Address Offset:0x00F8 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器121。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  cfg_req_timeout    : 10; /* bit[0-9]  : timeout limit */
        unsigned int  cfg_timeout_bypass : 1;  /* bit[10]   : 0:not bypass; 1:bypass timer */
        unsigned int  cfg_sync_mask      : 4;  /* bit[11-14]: decides bist request which channel will be masked */
        unsigned int  cfg_timeout_clear  : 1;  /* bit[15]   : 0:not clear; 1:clear timeout */
        unsigned int  reserved           : 16; /* bit[16-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL121_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL121_cfg_req_timeout_START     (0)
#define SOC_PCTRL_PERI_CTRL121_cfg_req_timeout_END       (9)
#define SOC_PCTRL_PERI_CTRL121_cfg_timeout_bypass_START  (10)
#define SOC_PCTRL_PERI_CTRL121_cfg_timeout_bypass_END    (10)
#define SOC_PCTRL_PERI_CTRL121_cfg_sync_mask_START       (11)
#define SOC_PCTRL_PERI_CTRL121_cfg_sync_mask_END         (14)
#define SOC_PCTRL_PERI_CTRL121_cfg_timeout_clear_START   (15)
#define SOC_PCTRL_PERI_CTRL121_cfg_timeout_clear_END     (15)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL122_UNION
 struct description   : PERI_CTRL122 Register structure definition
                        Address Offset:0x00FC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器122。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved0 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL122_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL122_pctrl_modem_reserved0_START  (0)
#define SOC_PCTRL_PERI_CTRL122_pctrl_modem_reserved0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL123_UNION
 struct description   : PERI_CTRL123 Register structure definition
                        Address Offset:0x0100 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器123。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved1 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL123_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL123_pctrl_modem_reserved1_START  (0)
#define SOC_PCTRL_PERI_CTRL123_pctrl_modem_reserved1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL124_UNION
 struct description   : PERI_CTRL124 Register structure definition
                        Address Offset:0x0104 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器124。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved2 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL124_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL124_pctrl_modem_reserved2_START  (0)
#define SOC_PCTRL_PERI_CTRL124_pctrl_modem_reserved2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL125_UNION
 struct description   : PERI_CTRL125 Register structure definition
                        Address Offset:0x0108 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器125。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved3 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL125_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL125_pctrl_modem_reserved3_START  (0)
#define SOC_PCTRL_PERI_CTRL125_pctrl_modem_reserved3_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL126_UNION
 struct description   : PERI_CTRL126 Register structure definition
                        Address Offset:0x010C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器126。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved4 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL126_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL126_pctrl_modem_reserved4_START  (0)
#define SOC_PCTRL_PERI_CTRL126_pctrl_modem_reserved4_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL127_UNION
 struct description   : PERI_CTRL127 Register structure definition
                        Address Offset:0x0110 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器127。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved5 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL127_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL127_pctrl_modem_reserved5_START  (0)
#define SOC_PCTRL_PERI_CTRL127_pctrl_modem_reserved5_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL128_UNION
 struct description   : PERI_CTRL128 Register structure definition
                        Address Offset:0x0114 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器128。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved6 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL128_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL128_pctrl_modem_reserved6_START  (0)
#define SOC_PCTRL_PERI_CTRL128_pctrl_modem_reserved6_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL129_UNION
 struct description   : PERI_CTRL129 Register structure definition
                        Address Offset:0x0118 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器129。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved7 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL129_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL129_pctrl_modem_reserved7_START  (0)
#define SOC_PCTRL_PERI_CTRL129_pctrl_modem_reserved7_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL130_UNION
 struct description   : PERI_CTRL130 Register structure definition
                        Address Offset:0x011C Initial:0xAA Width:32
 register description : 通用外设控制寄存器130。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ib_perf_stat_icfg_arvcidx_device : 8;  /* bit[0-7]  :  */
        unsigned int  ib_perf_stat_icfg_awvcidx_device : 8;  /* bit[8-15] :  */
        unsigned int  ib_pwr_mon_icfg_arvcidx_device   : 8;  /* bit[16-23]:  */
        unsigned int  ib_pwr_mon_icfg_awvcidx_device   : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL130_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL130_ib_perf_stat_icfg_arvcidx_device_START  (0)
#define SOC_PCTRL_PERI_CTRL130_ib_perf_stat_icfg_arvcidx_device_END    (7)
#define SOC_PCTRL_PERI_CTRL130_ib_perf_stat_icfg_awvcidx_device_START  (8)
#define SOC_PCTRL_PERI_CTRL130_ib_perf_stat_icfg_awvcidx_device_END    (15)
#define SOC_PCTRL_PERI_CTRL130_ib_pwr_mon_icfg_arvcidx_device_START    (16)
#define SOC_PCTRL_PERI_CTRL130_ib_pwr_mon_icfg_arvcidx_device_END      (23)
#define SOC_PCTRL_PERI_CTRL130_ib_pwr_mon_icfg_awvcidx_device_START    (24)
#define SOC_PCTRL_PERI_CTRL130_ib_pwr_mon_icfg_awvcidx_device_END      (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL131_UNION
 struct description   : PERI_CTRL131 Register structure definition
                        Address Offset:0x0120 Initial:0xAA Width:32
 register description : 通用外设控制寄存器131。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ib_lpmcu_icfg_arvcidx_device    : 8;  /* bit[0-7]  :  */
        unsigned int  ib_djtag_icfg_arvcidx_device    : 8;  /* bit[8-15] :  */
        unsigned int  ib_dmac_mst_icfg_arvcidx_device : 8;  /* bit[16-23]:  */
        unsigned int  ib_dmac_mst_icfg_awvcidx_device : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL131_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL131_ib_lpmcu_icfg_arvcidx_device_START     (0)
#define SOC_PCTRL_PERI_CTRL131_ib_lpmcu_icfg_arvcidx_device_END       (7)
#define SOC_PCTRL_PERI_CTRL131_ib_djtag_icfg_arvcidx_device_START     (8)
#define SOC_PCTRL_PERI_CTRL131_ib_djtag_icfg_arvcidx_device_END       (15)
#define SOC_PCTRL_PERI_CTRL131_ib_dmac_mst_icfg_arvcidx_device_START  (16)
#define SOC_PCTRL_PERI_CTRL131_ib_dmac_mst_icfg_arvcidx_device_END    (23)
#define SOC_PCTRL_PERI_CTRL131_ib_dmac_mst_icfg_awvcidx_device_START  (24)
#define SOC_PCTRL_PERI_CTRL131_ib_dmac_mst_icfg_awvcidx_device_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL132_UNION
 struct description   : PERI_CTRL132 Register structure definition
                        Address Offset:0x0124 Initial:0x55 Width:32
 register description : 通用外设控制寄存器132。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ib_perf_stat_icfg_arvcidx_ddr : 8;  /* bit[0-7]  :  */
        unsigned int  ib_perf_stat_icfg_awvcidx_ddr : 8;  /* bit[8-15] :  */
        unsigned int  ib_pwr_mon_icfg_arvcidx_ddr   : 8;  /* bit[16-23]:  */
        unsigned int  ib_pwr_mon_icfg_awvcidx_ddr   : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL132_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL132_ib_perf_stat_icfg_arvcidx_ddr_START  (0)
#define SOC_PCTRL_PERI_CTRL132_ib_perf_stat_icfg_arvcidx_ddr_END    (7)
#define SOC_PCTRL_PERI_CTRL132_ib_perf_stat_icfg_awvcidx_ddr_START  (8)
#define SOC_PCTRL_PERI_CTRL132_ib_perf_stat_icfg_awvcidx_ddr_END    (15)
#define SOC_PCTRL_PERI_CTRL132_ib_pwr_mon_icfg_arvcidx_ddr_START    (16)
#define SOC_PCTRL_PERI_CTRL132_ib_pwr_mon_icfg_arvcidx_ddr_END      (23)
#define SOC_PCTRL_PERI_CTRL132_ib_pwr_mon_icfg_awvcidx_ddr_START    (24)
#define SOC_PCTRL_PERI_CTRL132_ib_pwr_mon_icfg_awvcidx_ddr_END      (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL133_UNION
 struct description   : PERI_CTRL133 Register structure definition
                        Address Offset:0x0128 Initial:0x55 Width:32
 register description : 通用外设控制寄存器133。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ib_lpmcu_icfg_arvcidx_ddr    : 8;  /* bit[0-7]  :  */
        unsigned int  ib_djtag_icfg_arvcidx_ddr    : 8;  /* bit[8-15] :  */
        unsigned int  ib_dmac_mst_icfg_arvcidx_ddr : 8;  /* bit[16-23]:  */
        unsigned int  ib_dmac_mst_icfg_awvcidx_ddr : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL133_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL133_ib_lpmcu_icfg_arvcidx_ddr_START     (0)
#define SOC_PCTRL_PERI_CTRL133_ib_lpmcu_icfg_arvcidx_ddr_END       (7)
#define SOC_PCTRL_PERI_CTRL133_ib_djtag_icfg_arvcidx_ddr_START     (8)
#define SOC_PCTRL_PERI_CTRL133_ib_djtag_icfg_arvcidx_ddr_END       (15)
#define SOC_PCTRL_PERI_CTRL133_ib_dmac_mst_icfg_arvcidx_ddr_START  (16)
#define SOC_PCTRL_PERI_CTRL133_ib_dmac_mst_icfg_arvcidx_ddr_END    (23)
#define SOC_PCTRL_PERI_CTRL133_ib_dmac_mst_icfg_awvcidx_ddr_START  (24)
#define SOC_PCTRL_PERI_CTRL133_ib_dmac_mst_icfg_awvcidx_ddr_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL134_UNION
 struct description   : PERI_CTRL134 Register structure definition
                        Address Offset:0x012C Initial:0xAA Width:32
 register description : 通用外设控制寄存器134。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ib_topcs_icfg_arvcidx_ddr    : 8;  /* bit[0-7]  :  */
        unsigned int  ib_topcs_icfg_awvcidx_ddr    : 8;  /* bit[8-15] :  */
        unsigned int  ib_topcs_icfg_arvcidx_device : 8;  /* bit[16-23]:  */
        unsigned int  ib_topcs_icfg_awvcidx_device : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL134_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_arvcidx_ddr_START     (0)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_arvcidx_ddr_END       (7)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_awvcidx_ddr_START     (8)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_awvcidx_ddr_END       (15)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_arvcidx_device_START  (16)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_arvcidx_device_END    (23)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_awvcidx_device_START  (24)
#define SOC_PCTRL_PERI_CTRL134_ib_topcs_icfg_awvcidx_device_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL135_UNION
 struct description   : PERI_CTRL135 Register structure definition
                        Address Offset:0x0130 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器135。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mdm_tsp_nmi0_en : 32; /* bit[0-31]: MODEM TSP中断使能寄存器0 */
    } reg;
} SOC_PCTRL_PERI_CTRL135_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL135_mdm_tsp_nmi0_en_START  (0)
#define SOC_PCTRL_PERI_CTRL135_mdm_tsp_nmi0_en_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL136_UNION
 struct description   : PERI_CTRL136 Register structure definition
                        Address Offset:0x0134 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器136。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mdm_tsp_nmi0_clr : 32; /* bit[0-31]: MODEM TSP中断清除寄存器0 */
    } reg;
} SOC_PCTRL_PERI_CTRL136_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL136_mdm_tsp_nmi0_clr_START  (0)
#define SOC_PCTRL_PERI_CTRL136_mdm_tsp_nmi0_clr_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_mdm_tsp_nmi0_UNION
 struct description   : mdm_tsp_nmi0 Register structure definition
                        Address Offset:0x0138 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器137。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mdm_tsp_nmi0 : 32; /* bit[0-31]: MODEM TSP中断寄存器0 */
    } reg;
} SOC_PCTRL_mdm_tsp_nmi0_UNION;
#endif
#define SOC_PCTRL_mdm_tsp_nmi0_mdm_tsp_nmi0_START  (0)
#define SOC_PCTRL_mdm_tsp_nmi0_mdm_tsp_nmi0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL138_UNION
 struct description   : PERI_CTRL138 Register structure definition
                        Address Offset:0x013C Initial:0x0000 Width:32
 register description : 通用外设控制寄存器138。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ispcpu2qice_gate_en             : 1;  /* bit[0]    :  */
        unsigned int  cpulc2qice_gate_en              : 1;  /* bit[1]    :  */
        unsigned int  cpuhc2qice_gate_en              : 1;  /* bit[2]    :  */
        unsigned int  dcdr2dmc3_ckg_en                : 1;  /* bit[3]    :  */
        unsigned int  dcdr2dmc2_ckg_en                : 1;  /* bit[4]    :  */
        unsigned int  dcdr2dmc1_ckg_en                : 1;  /* bit[5]    :  */
        unsigned int  dcdr2dmc0_ckg_en                : 1;  /* bit[6]    :  */
        unsigned int  icfg_ckg_rs_byp_npu2qice        : 1;  /* bit[7]    :  */
        unsigned int  icfg_ckg_rs_byp_m22qice         : 1;  /* bit[8]    :  */
        unsigned int  icfg_ckg_rs_byp_m12qice         : 1;  /* bit[9]    :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmca_drsp  : 1;  /* bit[10]   :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmca_crsp  : 1;  /* bit[11]   :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmca_rdata : 1;  /* bit[12]   :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmcd_wdata : 1;  /* bit[13]   :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmcc_wdata : 1;  /* bit[14]   :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmcb_wdata : 1;  /* bit[15]   :  */
        unsigned int  icfg_ckg_rs_byp_qice2dmca_wdata : 1;  /* bit[16]   :  */
        unsigned int  reserved                        : 15; /* bit[17-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL138_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL138_ispcpu2qice_gate_en_START              (0)
#define SOC_PCTRL_PERI_CTRL138_ispcpu2qice_gate_en_END                (0)
#define SOC_PCTRL_PERI_CTRL138_cpulc2qice_gate_en_START               (1)
#define SOC_PCTRL_PERI_CTRL138_cpulc2qice_gate_en_END                 (1)
#define SOC_PCTRL_PERI_CTRL138_cpuhc2qice_gate_en_START               (2)
#define SOC_PCTRL_PERI_CTRL138_cpuhc2qice_gate_en_END                 (2)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc3_ckg_en_START                 (3)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc3_ckg_en_END                   (3)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc2_ckg_en_START                 (4)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc2_ckg_en_END                   (4)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc1_ckg_en_START                 (5)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc1_ckg_en_END                   (5)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc0_ckg_en_START                 (6)
#define SOC_PCTRL_PERI_CTRL138_dcdr2dmc0_ckg_en_END                   (6)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_npu2qice_START         (7)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_npu2qice_END           (7)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_m22qice_START          (8)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_m22qice_END            (8)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_m12qice_START          (9)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_m12qice_END            (9)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_drsp_START   (10)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_drsp_END     (10)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_crsp_START   (11)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_crsp_END     (11)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_rdata_START  (12)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_rdata_END    (12)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmcd_wdata_START  (13)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmcd_wdata_END    (13)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmcc_wdata_START  (14)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmcc_wdata_END    (14)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmcb_wdata_START  (15)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmcb_wdata_END    (15)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_wdata_START  (16)
#define SOC_PCTRL_PERI_CTRL138_icfg_ckg_rs_byp_qice2dmca_wdata_END    (16)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL139_UNION
 struct description   : PERI_CTRL139 Register structure definition
                        Address Offset:0x0140 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器139。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pdm_nopending_sys2mdm_bypass : 1;  /* bit[0]   :  */
        unsigned int  reserved                     : 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL139_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL139_pdm_nopending_sys2mdm_bypass_START  (0)
#define SOC_PCTRL_PERI_CTRL139_pdm_nopending_sys2mdm_bypass_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL140_UNION
 struct description   : PERI_CTRL140 Register structure definition
                        Address Offset:0x0144 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器140。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL140_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL141_UNION
 struct description   : PERI_CTRL141 Register structure definition
                        Address Offset:0x0148 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器141。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL141_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL142_UNION
 struct description   : PERI_CTRL142 Register structure definition
                        Address Offset:0x014C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器142。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL142_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL143_UNION
 struct description   : PERI_CTRL143 Register structure definition
                        Address Offset:0x0150 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器143。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL143_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL144_UNION
 struct description   : PERI_CTRL144 Register structure definition
                        Address Offset:0x0154 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器144。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL144_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL145_UNION
 struct description   : PERI_CTRL145 Register structure definition
                        Address Offset:0x0158 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器145。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL145_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL146_UNION
 struct description   : PERI_CTRL146 Register structure definition
                        Address Offset:0x015C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器146。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL146_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL147_UNION
 struct description   : PERI_CTRL147 Register structure definition
                        Address Offset:0x0160 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器147。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL147_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL148_UNION
 struct description   : PERI_CTRL148 Register structure definition
                        Address Offset:0x0164 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器148。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL148_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL149_UNION
 struct description   : PERI_CTRL149 Register structure definition
                        Address Offset:0x0168 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器149。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL149_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL150_UNION
 struct description   : PERI_CTRL150 Register structure definition
                        Address Offset:0x016C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器150。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL150_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL151_UNION
 struct description   : PERI_CTRL151 Register structure definition
                        Address Offset:0x0170 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器151。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL151_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL152_UNION
 struct description   : PERI_CTRL152 Register structure definition
                        Address Offset:0x0174 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器152。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL152_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL153_UNION
 struct description   : PERI_CTRL153 Register structure definition
                        Address Offset:0x0178 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器153。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL153_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL154_UNION
 struct description   : PERI_CTRL154 Register structure definition
                        Address Offset:0x017C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器154。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL154_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL155_UNION
 struct description   : PERI_CTRL155 Register structure definition
                        Address Offset:0x0180 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器155。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL155_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL156_UNION
 struct description   : PERI_CTRL156 Register structure definition
                        Address Offset:0x0184 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器156。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL156_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL157_UNION
 struct description   : PERI_CTRL157 Register structure definition
                        Address Offset:0x0188 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器157。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL157_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL158_UNION
 struct description   : PERI_CTRL158 Register structure definition
                        Address Offset:0x018C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器158。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL158_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL159_UNION
 struct description   : PERI_CTRL159 Register structure definition
                        Address Offset:0x0190 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器159。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL159_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL160_UNION
 struct description   : PERI_CTRL160 Register structure definition
                        Address Offset:0x0194 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器160。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL160_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL161_UNION
 struct description   : PERI_CTRL161 Register structure definition
                        Address Offset:0x0198 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器161。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL161_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL162_UNION
 struct description   : PERI_CTRL162 Register structure definition
                        Address Offset:0x019C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器162。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL162_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL163_UNION
 struct description   : PERI_CTRL163 Register structure definition
                        Address Offset:0x01A0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器163。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL163_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL164_UNION
 struct description   : PERI_CTRL164 Register structure definition
                        Address Offset:0x01A4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器164。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL164_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL165_UNION
 struct description   : PERI_CTRL165 Register structure definition
                        Address Offset:0x01A8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器165。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL165_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL166_UNION
 struct description   : PERI_CTRL166 Register structure definition
                        Address Offset:0x01AC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器166。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL166_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL167_UNION
 struct description   : PERI_CTRL167 Register structure definition
                        Address Offset:0x01B0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器167。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL167_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT0_UNION
 struct description   : PERI_STAT0 Register structure definition
                        Address Offset:0x1000 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  maa_debug_data_l : 32; /* bit[0-31]: maa，内部状态信号.共64bit，低32bit */
    } reg;
} SOC_PCTRL_PERI_STAT0_UNION;
#endif
#define SOC_PCTRL_PERI_STAT0_maa_debug_data_l_START  (0)
#define SOC_PCTRL_PERI_STAT0_maa_debug_data_l_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT1_UNION
 struct description   : PERI_STAT1 Register structure definition
                        Address Offset:0x1004 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  maa_debug_data_h : 32; /* bit[0-31]: maa，内部状态信号.共64bit，高32bit */
    } reg;
} SOC_PCTRL_PERI_STAT1_UNION;
#endif
#define SOC_PCTRL_PERI_STAT1_maa_debug_data_h_START  (0)
#define SOC_PCTRL_PERI_STAT1_maa_debug_data_h_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT2_UNION
 struct description   : PERI_STAT2 Register structure definition
                        Address Offset:0x1008 Initial:0x00000 Width:32
 register description : 通用外设状态寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    : 总线POWERDOMAIN 握手ack信号 */
        unsigned int  reserved_1: 1;  /* bit[1]    :  */
        unsigned int  reserved_2: 1;  /* bit[2]    :  */
        unsigned int  reserved_3: 1;  /* bit[3]    :  */
        unsigned int  reserved_4: 1;  /* bit[4]    :  */
        unsigned int  reserved_5: 1;  /* bit[5]    :  */
        unsigned int  reserved_6: 1;  /* bit[6]    :  */
        unsigned int  reserved_7: 1;  /* bit[7]    :  */
        unsigned int  reserved_8: 1;  /* bit[8]    :  */
        unsigned int  reserved_9: 1;  /* bit[9]    :  */
        unsigned int  reserved_10: 1;  /* bit[10]   :  */
        unsigned int  reserved_11: 1;  /* bit[11]   :  */
        unsigned int  reserved_12: 1;  /* bit[12]   :  */
        unsigned int  reserved_13: 19; /* bit[13-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT2_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT3_UNION
 struct description   : PERI_STAT3 Register structure definition
                        Address Offset:0x100C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  idle_ddrxctrl_h2p     : 1;  /* bit[0]   : DDR XCTRL的h2p协议转换桥状态 */
        unsigned int  idle_ddrxctrl_minibus : 1;  /* bit[1]   : DDR XCTRL的minibus状态 */
        unsigned int  reserved              : 30; /* bit[2-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT3_UNION;
#endif
#define SOC_PCTRL_PERI_STAT3_idle_ddrxctrl_h2p_START      (0)
#define SOC_PCTRL_PERI_STAT3_idle_ddrxctrl_h2p_END        (0)
#define SOC_PCTRL_PERI_STAT3_idle_ddrxctrl_minibus_START  (1)
#define SOC_PCTRL_PERI_STAT3_idle_ddrxctrl_minibus_END    (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT4_UNION
 struct description   : PERI_STAT4 Register structure definition
                        Address Offset:0x1010 Initial:0x0 Width:32
 register description : 通用外设状态寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spe_idle                          : 1;  /* bit[0]    : SPE空闲指示。
                                                                             0 - busy
                                                                             1 - idle */
        unsigned int  ipf_idle                          : 1;  /* bit[1]    : IPF空闲指示。
                                                                             0 - busy
                                                                             1 - idle */
        unsigned int  reserved_0                        : 1;  /* bit[2]    :  */
        unsigned int  cssys_etr_x2x_ckg_idle            : 1;  /* bit[3]    : ETR接口X2X桥idle状态指示 */
        unsigned int  eicc_idle                         : 1;  /* bit[4]    : EICC空闲指示。
                                                                             0 - busy
                                                                             1 - idle */
        unsigned int  maa_idle                          : 1;  /* bit[5]    : MAA空闲指示。
                                                                             0 - busy
                                                                             1 - idle */
        unsigned int  socp_idle                         : 1;  /* bit[6]    : SOCP空闲指示。
                                                                             0 - busy
                                                                             1 - idle */
        unsigned int  reserved_1                        : 1;  /* bit[7]    :  */
        unsigned int  reserved_2                        : 2;  /* bit[8-9]  : 保留 */
        unsigned int  reserved_3                        : 8;  /* bit[10-17]: 保留 */
        unsigned int  intr1_modem_ipcm2hifi_s_eicc_stat : 1;  /* bit[18]   : modem给HiFi的中断信号的状态回读 */
        unsigned int  intr1_modem_ipcm2hifi_s_ipc_stat  : 1;  /* bit[19]   : modem给HiFi的中断信号的状态回读 */
        unsigned int  sc_mdm_reset_stat                 : 1;  /* bit[20]   : modem testpin具体参考modem管脚调试说明。 */
        unsigned int  pwr_dwn_ack_cdphy0                : 1;  /* bit[21]   : MIPI TX PHY0下电状态。 default:0X0
                                                                             0 - 处于上电模式；
                                                                             1 - 处于下电模式。 */
        unsigned int  pwr_dwn_ack_cdphy1                : 1;  /* bit[22]   : MIPI TX PHY1下电状态。 default:0X0
                                                                             0 - 处于上电模式；
                                                                             1 - 处于下电模式。 */
        unsigned int  reserved_4                        : 3;  /* bit[23-25]:  */
        unsigned int  reserved_5                        : 3;  /* bit[26-28]:  */
        unsigned int  reserved_6                        : 3;  /* bit[29-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT4_UNION;
#endif
#define SOC_PCTRL_PERI_STAT4_spe_idle_START                           (0)
#define SOC_PCTRL_PERI_STAT4_spe_idle_END                             (0)
#define SOC_PCTRL_PERI_STAT4_ipf_idle_START                           (1)
#define SOC_PCTRL_PERI_STAT4_ipf_idle_END                             (1)
#define SOC_PCTRL_PERI_STAT4_cssys_etr_x2x_ckg_idle_START             (3)
#define SOC_PCTRL_PERI_STAT4_cssys_etr_x2x_ckg_idle_END               (3)
#define SOC_PCTRL_PERI_STAT4_eicc_idle_START                          (4)
#define SOC_PCTRL_PERI_STAT4_eicc_idle_END                            (4)
#define SOC_PCTRL_PERI_STAT4_maa_idle_START                           (5)
#define SOC_PCTRL_PERI_STAT4_maa_idle_END                             (5)
#define SOC_PCTRL_PERI_STAT4_socp_idle_START                          (6)
#define SOC_PCTRL_PERI_STAT4_socp_idle_END                            (6)
#define SOC_PCTRL_PERI_STAT4_intr1_modem_ipcm2hifi_s_eicc_stat_START  (18)
#define SOC_PCTRL_PERI_STAT4_intr1_modem_ipcm2hifi_s_eicc_stat_END    (18)
#define SOC_PCTRL_PERI_STAT4_intr1_modem_ipcm2hifi_s_ipc_stat_START   (19)
#define SOC_PCTRL_PERI_STAT4_intr1_modem_ipcm2hifi_s_ipc_stat_END     (19)
#define SOC_PCTRL_PERI_STAT4_sc_mdm_reset_stat_START                  (20)
#define SOC_PCTRL_PERI_STAT4_sc_mdm_reset_stat_END                    (20)
#define SOC_PCTRL_PERI_STAT4_pwr_dwn_ack_cdphy0_START                 (21)
#define SOC_PCTRL_PERI_STAT4_pwr_dwn_ack_cdphy0_END                   (21)
#define SOC_PCTRL_PERI_STAT4_pwr_dwn_ack_cdphy1_START                 (22)
#define SOC_PCTRL_PERI_STAT4_pwr_dwn_ack_cdphy1_END                   (22)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT5_UNION
 struct description   : PERI_STAT5 Register structure definition
                        Address Offset:0x1014 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  sc_mdm2ap_status : 32; /* bit[0-31]: MODEM的状态上报。sc_mdm2ap_status[31:0] modem testpin具体参考modem管脚调试说明。 */
    } reg;
} SOC_PCTRL_PERI_STAT5_UNION;
#endif
#define SOC_PCTRL_PERI_STAT5_sc_mdm2ap_status_START  (0)
#define SOC_PCTRL_PERI_STAT5_sc_mdm2ap_status_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT6_UNION
 struct description   : PERI_STAT6 Register structure definition
                        Address Offset:0x1018 Initial:0x000000 Width:32
 register description : 通用外设状态寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  test_point_core : 10; /* bit[0-9]  :  */
        unsigned int  reserved        : 22; /* bit[10-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT6_UNION;
#endif
#define SOC_PCTRL_PERI_STAT6_test_point_core_START  (0)
#define SOC_PCTRL_PERI_STAT6_test_point_core_END    (9)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT7_UNION
 struct description   : PERI_STAT7 Register structure definition
                        Address Offset:0x101C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器7--QIC busy回读。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT7_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT8_UNION
 struct description   : PERI_STAT8 Register structure definition
                        Address Offset:0x1020 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器8--QIC busy回读。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT8_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT9_UNION
 struct description   : PERI_STAT9 Register structure definition
                        Address Offset:0x1024 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器9--QIC busy回读。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT9_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT10_UNION
 struct description   : PERI_STAT10 Register structure definition
                        Address Offset:0x1028 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器10--QIC busy回读。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT10_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT11_UNION
 struct description   : PERI_STAT11 Register structure definition
                        Address Offset:0x102C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器11。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT11_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT12_UNION
 struct description   : PERI_STAT12 Register structure definition
                        Address Offset:0x1030 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器12。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT12_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT13_UNION
 struct description   : PERI_STAT13 Register structure definition
                        Address Offset:0x1034 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器13。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT13_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT14_UNION
 struct description   : PERI_STAT14 Register structure definition
                        Address Offset:0x1038 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器14。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT14_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT15_UNION
 struct description   : PERI_STAT15 Register structure definition
                        Address Offset:0x103C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器15。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT15_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT16_UNION
 struct description   : PERI_STAT16 Register structure definition
                        Address Offset:0x1040 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器16。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT16_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT17_UNION
 struct description   : PERI_STAT17 Register structure definition
                        Address Offset:0x1044 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器17。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT17_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT18_UNION
 struct description   : PERI_STAT18 Register structure definition
                        Address Offset:0x1048 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器18。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT18_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT19_UNION
 struct description   : PERI_STAT19 Register structure definition
                        Address Offset:0x104C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器19。--QIC busy回读
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT19_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT29_UNION
 struct description   : PERI_STAT29 Register structure definition
                        Address Offset:0x1050 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器29。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipc_ns_gt_clk_status      : 1;  /* bit[0]   : IPC NS内部门控状态，为1指示时钟打开 */
        unsigned int  ipc_gt_clk_status         : 1;  /* bit[1]   : IPC内部门控状态，为1指示时钟打开 */
        unsigned int  ipc_rtos_ns_gt_clk_status : 1;  /* bit[2]   : IPC RTOS NS内部门控状态，为1指示时钟打开 */
        unsigned int  reserved                  : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT29_UNION;
#endif
#define SOC_PCTRL_PERI_STAT29_ipc_ns_gt_clk_status_START       (0)
#define SOC_PCTRL_PERI_STAT29_ipc_ns_gt_clk_status_END         (0)
#define SOC_PCTRL_PERI_STAT29_ipc_gt_clk_status_START          (1)
#define SOC_PCTRL_PERI_STAT29_ipc_gt_clk_status_END            (1)
#define SOC_PCTRL_PERI_STAT29_ipc_rtos_ns_gt_clk_status_START  (2)
#define SOC_PCTRL_PERI_STAT29_ipc_rtos_ns_gt_clk_status_END    (2)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT30_UNION
 struct description   : PERI_STAT30 Register structure definition
                        Address Offset:0x1054 Initial:0x000000 Width:32
 register description : 通用外设状态寄存器30。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pdc_pwrds_ack_sys2mdm  : 1;  /* bit[0]   : 低功耗应答信号
                                                                 0：退出低功耗应答
                                                                 1：进入低功耗应答，复位态 */
        unsigned int  pdc_pwrds_ack_mdm2sys  : 1;  /* bit[1]   :  */
        unsigned int  pdc_pwrdm_ack_sys2mdm  : 1;  /* bit[2]   :  */
        unsigned int  pdc_pwrdm_ack_mdm2sys  : 1;  /* bit[3]   :  */
        unsigned int  pdc_pwrds_ack_mdm2ddr  : 1;  /* bit[4]   :  */
        unsigned int  pdc_pwrds_ack_pdea2ddr : 1;  /* bit[5]   :  */
        unsigned int  pdc_pwrdm_ack_mdm2ddr  : 1;  /* bit[6]   :  */
        unsigned int  pdc_pwrdm_ack_pdea2ddr : 1;  /* bit[7]   : 低功耗应答信号
                                                                 0：退出低功耗应答
                                                                 1：进入低功耗应答，复位态 */
        unsigned int  reserved               : 24; /* bit[8-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT30_UNION;
#endif
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_sys2mdm_START   (0)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_sys2mdm_END     (0)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_mdm2sys_START   (1)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_mdm2sys_END     (1)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_sys2mdm_START   (2)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_sys2mdm_END     (2)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_mdm2sys_START   (3)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_mdm2sys_END     (3)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_mdm2ddr_START   (4)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_mdm2ddr_END     (4)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_pdea2ddr_START  (5)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrds_ack_pdea2ddr_END    (5)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_mdm2ddr_START   (6)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_mdm2ddr_END     (6)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_pdea2ddr_START  (7)
#define SOC_PCTRL_PERI_STAT30_pdc_pwrdm_ack_pdea2ddr_END    (7)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT31_UNION
 struct description   : PERI_STAT31 Register structure definition
                        Address Offset:0x1058 Initial:0x00 Width:32
 register description : 通用外设状态寄存器31。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pds_pw_stat_sys2mdm  : 3;  /* bit[0-2]  : 低功耗状态机状态指示信号 */
        unsigned int  pds_pw_stat_mdm2sys  : 3;  /* bit[3-5]  : 低功耗状态机状态指示信号 */
        unsigned int  pdm_pw_stat_sys2mdm  : 5;  /* bit[6-10] : 低功耗状态机状态指示信号 */
        unsigned int  pdm_pw_stat_mdm2sys  : 5;  /* bit[11-15]: 低功耗状态机状态指示信号 */
        unsigned int  pds_pw_stat_mdm2ddr  : 3;  /* bit[16-18]: 低功耗状态机状态指示信号 */
        unsigned int  pds_pw_stat_pdea2ddr : 3;  /* bit[19-21]: 低功耗状态机状态指示信号 */
        unsigned int  pdm_pw_stat_mdm2ddr  : 5;  /* bit[22-26]: 低功耗状态机状态指示信号 */
        unsigned int  pdm_pw_stat_pdea2ddr : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT31_UNION;
#endif
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_sys2mdm_START   (0)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_sys2mdm_END     (2)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_mdm2sys_START   (3)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_mdm2sys_END     (5)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_sys2mdm_START   (6)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_sys2mdm_END     (10)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_mdm2sys_START   (11)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_mdm2sys_END     (15)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_mdm2ddr_START   (16)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_mdm2ddr_END     (18)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_pdea2ddr_START  (19)
#define SOC_PCTRL_PERI_STAT31_pds_pw_stat_pdea2ddr_END    (21)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_mdm2ddr_START   (22)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_mdm2ddr_END     (26)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_pdea2ddr_START  (27)
#define SOC_PCTRL_PERI_STAT31_pdm_pw_stat_pdea2ddr_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT32_UNION
 struct description   : PERI_STAT32 Register structure definition
                        Address Offset:0x105C Initial:0x00 Width:32
 register description : 通用外设状态寄存器32。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pasensor_nbti_a_out : 9;  /* bit[0-8]  : nbti_a 读数结果 */
        unsigned int  reserved_0          : 7;  /* bit[9-15] : reserve */
        unsigned int  pasensor_nbti_o_out : 9;  /* bit[16-24]: nbti_o 读数结果 */
        unsigned int  reserved_1          : 7;  /* bit[25-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT32_UNION;
#endif
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_a_out_START  (0)
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_a_out_END    (8)
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_o_out_START  (16)
#define SOC_PCTRL_PERI_STAT32_pasensor_nbti_o_out_END    (24)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT33_UNION
 struct description   : PERI_STAT33 Register structure definition
                        Address Offset:0x1060 Initial:0x00 Width:32
 register description : 通用外设状态寄存器33。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pasensor_hci_a_out : 9;  /* bit[0-8]  : hci_a读数结果 */
        unsigned int  reserved_0         : 7;  /* bit[9-15] : reserve */
        unsigned int  pasensor_hci_o_out : 9;  /* bit[16-24]: hci_o读数结果 */
        unsigned int  pasensor_cfg_ready : 1;  /* bit[25]   : pasensor配置信号有效指示信号，高有效
                                                              1:PERI pasensor cfg is ready;
                                                              0:PERI pasensor cfg is not ready; */
        unsigned int  pasensor_valid     : 1;  /* bit[26]   : pasensor 读数有效指示信号，高有效 */
        unsigned int  reserved_1         : 5;  /* bit[27-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT33_UNION;
#endif
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_a_out_START  (0)
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_a_out_END    (8)
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_o_out_START  (16)
#define SOC_PCTRL_PERI_STAT33_pasensor_hci_o_out_END    (24)
#define SOC_PCTRL_PERI_STAT33_pasensor_cfg_ready_START  (25)
#define SOC_PCTRL_PERI_STAT33_pasensor_cfg_ready_END    (25)
#define SOC_PCTRL_PERI_STAT33_pasensor_valid_START      (26)
#define SOC_PCTRL_PERI_STAT33_pasensor_valid_END        (26)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT34_UNION
 struct description   : PERI_STAT34 Register structure definition
                        Address Offset:0x1064 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器34。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  req_timeout_flag : 1;  /* bit[0]   : 0:req not timeout; 1:req timeout */
        unsigned int  reserved         : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT34_UNION;
#endif
#define SOC_PCTRL_PERI_STAT34_req_timeout_flag_START  (0)
#define SOC_PCTRL_PERI_STAT34_req_timeout_flag_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT35_UNION
 struct description   : PERI_STAT35 Register structure definition
                        Address Offset:0x1068 Initial:0x0000000 Width:32
 register description : 通用外设状态寄存器35。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pdm_nopending_sys2mdm  : 1;  /* bit[0]   : no pending信号，表示当前模块没有存在命令需要执行 */
        unsigned int  pdm_nopending_mdm2sys  : 1;  /* bit[1]   : no pending信号，表示当前模块没有存在命令需要执行 */
        unsigned int  pdm_nopending_mdm2ddr  : 1;  /* bit[2]   : no pending信号，表示当前模块没有存在命令需要执行 */
        unsigned int  pdm_nopending_pdea2ddr : 1;  /* bit[3]   : no pending信号，表示当前模块没有存在命令需要执行 */
        unsigned int  reserved               : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT35_UNION;
#endif
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_sys2mdm_START   (0)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_sys2mdm_END     (0)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_mdm2sys_START   (1)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_mdm2sys_END     (1)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_mdm2ddr_START   (2)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_mdm2ddr_END     (2)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_pdea2ddr_START  (3)
#define SOC_PCTRL_PERI_STAT35_pdm_nopending_pdea2ddr_END    (3)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT36_UNION
 struct description   : PERI_STAT36 Register structure definition
                        Address Offset:0x106C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器36。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT36_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT37_UNION
 struct description   : PERI_STAT37 Register structure definition
                        Address Offset:0x1070 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器37。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT37_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT38_UNION
 struct description   : PERI_STAT38 Register structure definition
                        Address Offset:0x1074 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器38。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT38_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT39_UNION
 struct description   : PERI_STAT39 Register structure definition
                        Address Offset:0x1078 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器39。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT39_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT40_UNION
 struct description   : PERI_STAT40 Register structure definition
                        Address Offset:0x107C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器40。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT40_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT41_UNION
 struct description   : PERI_STAT41 Register structure definition
                        Address Offset:0x1080 Initial:0x0000 Width:32
 register description : 通用外设状态寄存器41。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  afmux_mem_sta_mdm_bbp_nonret_poff_ctrl_stat : 16; /* bit[0-15] : MODEM BBP的非retention 模式SD和DS回读信号，共15bit。详见mdm_debug文档。 */
        unsigned int  afmux_mem_sta_mdm_soc_nonret_poff_ctrl_stat : 16; /* bit[16-31]: MODEM soc的非retention 模式SD和DS回读信号，共15bit。详见mdm_debug文档。 */
    } reg;
} SOC_PCTRL_PERI_STAT41_UNION;
#endif
#define SOC_PCTRL_PERI_STAT41_afmux_mem_sta_mdm_bbp_nonret_poff_ctrl_stat_START  (0)
#define SOC_PCTRL_PERI_STAT41_afmux_mem_sta_mdm_bbp_nonret_poff_ctrl_stat_END    (15)
#define SOC_PCTRL_PERI_STAT41_afmux_mem_sta_mdm_soc_nonret_poff_ctrl_stat_START  (16)
#define SOC_PCTRL_PERI_STAT41_afmux_mem_sta_mdm_soc_nonret_poff_ctrl_stat_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT42_UNION
 struct description   : PERI_STAT42 Register structure definition
                        Address Offset:0x1084 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器42。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  afmux_mem_sta_modem_ret_poff_ctrl_stat : 32; /* bit[0-31]: MODEM 的retention 模式SD和DS回读信号，共32bit。详见mdm_debug文档。 */
    } reg;
} SOC_PCTRL_PERI_STAT42_UNION;
#endif
#define SOC_PCTRL_PERI_STAT42_afmux_mem_sta_modem_ret_poff_ctrl_stat_START  (0)
#define SOC_PCTRL_PERI_STAT42_afmux_mem_sta_modem_ret_poff_ctrl_stat_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT43_UNION
 struct description   : PERI_STAT43 Register structure definition
                        Address Offset:0x1088 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器43。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mdm_wdt_intr_merge_stat : 2;  /* bit[0-1] : modem内部wdt状态 */
        unsigned int  reserved                : 30; /* bit[2-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT43_UNION;
#endif
#define SOC_PCTRL_PERI_STAT43_mdm_wdt_intr_merge_stat_START  (0)
#define SOC_PCTRL_PERI_STAT43_mdm_wdt_intr_merge_stat_END    (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT44_UNION
 struct description   : PERI_STAT44 Register structure definition
                        Address Offset:0x108C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器44。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  mdmqdbs_pwrd_qdbs_ack    : 1;  /* bit[0]   : modem qdbs的qdbs上电请求 ack信号 */
        unsigned int  mdmqdbs_qdbs_qtp_err_int : 1;  /* bit[1]   : modem qdbs的qsbd qtp错误中断；  */
        unsigned int  mdmqdbs_qic_qdbs_busy    : 1;  /* bit[2]   : modem qdbs的qdbs_busy指示信号；  */
        unsigned int  reserved                 : 29; /* bit[3-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT44_UNION;
#endif
#define SOC_PCTRL_PERI_STAT44_mdmqdbs_pwrd_qdbs_ack_START     (0)
#define SOC_PCTRL_PERI_STAT44_mdmqdbs_pwrd_qdbs_ack_END       (0)
#define SOC_PCTRL_PERI_STAT44_mdmqdbs_qdbs_qtp_err_int_START  (1)
#define SOC_PCTRL_PERI_STAT44_mdmqdbs_qdbs_qtp_err_int_END    (1)
#define SOC_PCTRL_PERI_STAT44_mdmqdbs_qic_qdbs_busy_START     (2)
#define SOC_PCTRL_PERI_STAT44_mdmqdbs_qic_qdbs_busy_END       (2)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT45_UNION
 struct description   : PERI_STAT45 Register structure definition
                        Address Offset:0x1090 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器45。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_reserved_stat_0 : 32; /* bit[0-31]: PCTRL保留的状态寄存器0 */
    } reg;
} SOC_PCTRL_PERI_STAT45_UNION;
#endif
#define SOC_PCTRL_PERI_STAT45_pctrl_reserved_stat_0_START  (0)
#define SOC_PCTRL_PERI_STAT45_pctrl_reserved_stat_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT46_UNION
 struct description   : PERI_STAT46 Register structure definition
                        Address Offset:0x1094 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器46。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_reserved_stat_1 : 32; /* bit[0-31]: PCTRL保留的状态寄存器1 */
    } reg;
} SOC_PCTRL_PERI_STAT46_UNION;
#endif
#define SOC_PCTRL_PERI_STAT46_pctrl_reserved_stat_1_START  (0)
#define SOC_PCTRL_PERI_STAT46_pctrl_reserved_stat_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT47_UNION
 struct description   : PERI_STAT47 Register structure definition
                        Address Offset:0x1098 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器47。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_reserved_stat_2 : 32; /* bit[0-31]: PCTRL保留的状态寄存器2 */
    } reg;
} SOC_PCTRL_PERI_STAT47_UNION;
#endif
#define SOC_PCTRL_PERI_STAT47_pctrl_reserved_stat_2_START  (0)
#define SOC_PCTRL_PERI_STAT47_pctrl_reserved_stat_2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT48_UNION
 struct description   : PERI_STAT48 Register structure definition
                        Address Offset:0x109C Initial:0x0 Width:32
 register description : 通用外设状态寄存器48（SD回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                     : 1;  /* bit[0]    :  */
        unsigned int  afmux_ddr_xctrl_mem_sdo        : 1;  /* bit[1]    :  */
        unsigned int  afmux_ddr_xctrl_mem_dso        : 1;  /* bit[2]    :  */
        unsigned int  reserved_1                     : 1;  /* bit[3]    : 保留 */
        unsigned int  afmux_mem_sta_qic_peri_ram_sdo : 1;  /* bit[4]    : QIC_PERI的SD回读信号,qic_peri以前有个sdo信号给pctrl，现在不串联了，但端口仍然保留， */
        unsigned int  afmux_mem_sta_sd_peri          : 1;  /* bit[5]    : PERI区的SD回读信号 */
        unsigned int  reserved_2                     : 1;  /* bit[6]    : 保留 */
        unsigned int  reserved_3                     : 1;  /* bit[7]    :  */
        unsigned int  reserved_4                     : 1;  /* bit[8]    :  */
        unsigned int  afmux_retmem_sta_sd_dmc_pack   : 4;  /* bit[9-12] : 9-12分别是pack_a/b/c/d的SD回读信号 */
        unsigned int  afmux_retmem_sta_ds_dmc_pack   : 4;  /* bit[13-16]: 13-16分别是pack_a/b/c/d的DS回读信号 */
        unsigned int  reserved_5                     : 1;  /* bit[17]   : 保留 */
        unsigned int  reserved_6                     : 1;  /* bit[18]   : 保留 */
        unsigned int  reserved_7                     : 1;  /* bit[19]   : 保留。 */
        unsigned int  reserved_8                     : 1;  /* bit[20]   : 保留 */
        unsigned int  reserved_9                     : 1;  /* bit[21]   : 保留 */
        unsigned int  reserved_10                    : 1;  /* bit[22]   : 保留 */
        unsigned int  reserved_11                    : 1;  /* bit[23]   : 保留 */
        unsigned int  reserved_12                    : 1;  /* bit[24]   : 保留 */
        unsigned int  reserved_13                    : 1;  /* bit[25]   : 保留 */
        unsigned int  reserved_14                    : 1;  /* bit[26]   : 保留 */
        unsigned int  reserved_15                    : 1;  /* bit[27]   : 保留 */
        unsigned int  reserved_16                    : 1;  /* bit[28]   : 保留 */
        unsigned int  reserved_17                    : 1;  /* bit[29]   : 保留 */
        unsigned int  reserved_18                    : 1;  /* bit[30]   : 保留 */
        unsigned int  reserved_19                    : 1;  /* bit[31]   : 保留。 */
    } reg;
} SOC_PCTRL_PERI_STAT48_UNION;
#endif
#define SOC_PCTRL_PERI_STAT48_afmux_ddr_xctrl_mem_sdo_START         (1)
#define SOC_PCTRL_PERI_STAT48_afmux_ddr_xctrl_mem_sdo_END           (1)
#define SOC_PCTRL_PERI_STAT48_afmux_ddr_xctrl_mem_dso_START         (2)
#define SOC_PCTRL_PERI_STAT48_afmux_ddr_xctrl_mem_dso_END           (2)
#define SOC_PCTRL_PERI_STAT48_afmux_mem_sta_qic_peri_ram_sdo_START  (4)
#define SOC_PCTRL_PERI_STAT48_afmux_mem_sta_qic_peri_ram_sdo_END    (4)
#define SOC_PCTRL_PERI_STAT48_afmux_mem_sta_sd_peri_START           (5)
#define SOC_PCTRL_PERI_STAT48_afmux_mem_sta_sd_peri_END             (5)
#define SOC_PCTRL_PERI_STAT48_afmux_retmem_sta_sd_dmc_pack_START    (9)
#define SOC_PCTRL_PERI_STAT48_afmux_retmem_sta_sd_dmc_pack_END      (12)
#define SOC_PCTRL_PERI_STAT48_afmux_retmem_sta_ds_dmc_pack_START    (13)
#define SOC_PCTRL_PERI_STAT48_afmux_retmem_sta_ds_dmc_pack_END      (16)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT49_UNION
 struct description   : PERI_STAT49 Register structure definition
                        Address Offset:0x10A0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器49（SD回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT49_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT50_UNION
 struct description   : PERI_STAT50 Register structure definition
                        Address Offset:0x10A4 Initial:0x00000 Width:32
 register description : 通用外设状态寄存器50（SD回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  afmux_mem_stat_sd_dmc : 4;  /* bit[0-3]  : DDRPHY的SD回读信号，dmc四个harden的SDO端口名称：
                                                                 [3]:dmcd_sdo
                                                                 [2]:dmcc_sdo
                                                                 [1]:dmcb_sdo
                                                                 [0]:dmca_sdo */
        unsigned int  reserved_0            : 1;  /* bit[4]    : 保留 */
        unsigned int  reserved_1            : 1;  /* bit[5]    : 保留 */
        unsigned int  reserved_2            : 1;  /* bit[6]    : 保留 */
        unsigned int  reserved_3            : 1;  /* bit[7]    : 保留 */
        unsigned int  reserved_4            : 1;  /* bit[8]    : 保留 */
        unsigned int  reserved_5            : 1;  /* bit[9]    : 保留 */
        unsigned int  reserved_6            : 1;  /* bit[10]   : 保留 */
        unsigned int  reserved_7            : 1;  /* bit[11]   : 保留 */
        unsigned int  reserved_8            : 20; /* bit[12-31]: MODEM的SD和DS回读信号。
                                                                 详述请参考《MODEM子系统低功耗描述表》 */
    } reg;
} SOC_PCTRL_PERI_STAT50_UNION;
#endif
#define SOC_PCTRL_PERI_STAT50_afmux_mem_stat_sd_dmc_START  (0)
#define SOC_PCTRL_PERI_STAT50_afmux_mem_stat_sd_dmc_END    (3)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT51_UNION
 struct description   : PERI_STAT51 Register structure definition
                        Address Offset:0x10A8 Initial:0x000 Width:32
 register description : 通用外设状态寄存器51（SD回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  afmux_retmem_stat_sdo_qice  : 8;  /* bit[0-7]  : QICE的retmem的SD回读信号 */
        unsigned int  afmux_retmem_stat_dso_qice  : 8;  /* bit[8-15] : QICE的retmem的DS回读信号 */
        unsigned int  afmux_mem_stat_sdo_qice     : 1;  /* bit[16]   : QICE的SD回读信号 */
        unsigned int  afmux_mem_stat_sdo_ffts     : 1;  /* bit[17]   : ffts的SD回读信号 */
        unsigned int  afmux_mem_stat_sdo_exmbist0 : 1;  /* bit[18]   : exmbist0的SD回读信号 */
        unsigned int  afmux_mem_stat_sdo_exmbist1 : 1;  /* bit[19]   : exmbist1的SD回读信号 */
        unsigned int  reserved                    : 12; /* bit[20-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT51_UNION;
#endif
#define SOC_PCTRL_PERI_STAT51_afmux_retmem_stat_sdo_qice_START   (0)
#define SOC_PCTRL_PERI_STAT51_afmux_retmem_stat_sdo_qice_END     (7)
#define SOC_PCTRL_PERI_STAT51_afmux_retmem_stat_dso_qice_START   (8)
#define SOC_PCTRL_PERI_STAT51_afmux_retmem_stat_dso_qice_END     (15)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_qice_START      (16)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_qice_END        (16)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_ffts_START      (17)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_ffts_END        (17)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_exmbist0_START  (18)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_exmbist0_END    (18)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_exmbist1_START  (19)
#define SOC_PCTRL_PERI_STAT51_afmux_mem_stat_sdo_exmbist1_END    (19)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT52_UNION
 struct description   : PERI_STAT52 Register structure definition
                        Address Offset:0x10AC Initial:0x0 Width:32
 register description : 通用外设状态寄存器52（保留给SD回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  afmux_mem_stat_sd_sca : 6;  /* bit[0-5]  : system_cache a的SD回读信号 */
        unsigned int  afmux_mem_stat_sd_scb : 6;  /* bit[6-11] : system_cache b的SD回读信号 */
        unsigned int  afmux_mem_stat_sd_scc : 6;  /* bit[12-17]: system_cache c的SD回读信号 */
        unsigned int  afmux_mem_stat_sd_scd : 6;  /* bit[18-23]: system_cached 的SD回读信号 */
        unsigned int  reserved_0            : 7;  /* bit[24-30]: 保留 */
        unsigned int  reserved_1            : 1;  /* bit[31]   : 保留给SD回读，不做他用。 */
    } reg;
} SOC_PCTRL_PERI_STAT52_UNION;
#endif
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_sca_START  (0)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_sca_END    (5)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_scb_START  (6)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_scb_END    (11)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_scc_START  (12)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_scc_END    (17)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_scd_START  (18)
#define SOC_PCTRL_PERI_STAT52_afmux_mem_stat_sd_scd_END    (23)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT53_UNION
 struct description   : PERI_STAT53 Register structure definition
                        Address Offset:0x10B0 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器53（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_rd_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFG DMA QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT53_UNION;
#endif
#define SOC_PCTRL_PERI_STAT53_qic_rd_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT53_qic_rd_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT54_UNION
 struct description   : PERI_STAT54 Register structure definition
                        Address Offset:0x10B4 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器54（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_wr_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFG DMA QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT54_UNION;
#endif
#define SOC_PCTRL_PERI_STAT54_qic_wr_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT54_qic_wr_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT55_UNION
 struct description   : PERI_STAT55 Register structure definition
                        Address Offset:0x10B8 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器55（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_tb_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFG DMA QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT55_UNION;
#endif
#define SOC_PCTRL_PERI_STAT55_qic_tb_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT55_qic_tb_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT56_UNION
 struct description   : PERI_STAT56 Register structure definition
                        Address Offset:0x10BC Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器56（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdms_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFG DMA QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT56_UNION;
#endif
#define SOC_PCTRL_PERI_STAT56_qic_pdms_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT56_qic_pdms_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT57_UNION
 struct description   : PERI_STAT57 Register structure definition
                        Address Offset:0x10C0 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器57（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_rd_nopending_mdmperi_0 : 32; /* bit[0-31]: MODEM PERI QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT57_UNION;
#endif
#define SOC_PCTRL_PERI_STAT57_qic_rd_nopending_mdmperi_0_START  (0)
#define SOC_PCTRL_PERI_STAT57_qic_rd_nopending_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT58_UNION
 struct description   : PERI_STAT58 Register structure definition
                        Address Offset:0x10C4 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器58（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_wr_nopending_mdmperi_0 : 32; /* bit[0-31]: MODEM PERI QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT58_UNION;
#endif
#define SOC_PCTRL_PERI_STAT58_qic_wr_nopending_mdmperi_0_START  (0)
#define SOC_PCTRL_PERI_STAT58_qic_wr_nopending_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT59_UNION
 struct description   : PERI_STAT59 Register structure definition
                        Address Offset:0x10C8 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器59（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_tb_nopending_mdmperi_0 : 32; /* bit[0-31]: MODEM PERI QIC 的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT59_UNION;
#endif
#define SOC_PCTRL_PERI_STAT59_qic_tb_nopending_mdmperi_0_START  (0)
#define SOC_PCTRL_PERI_STAT59_qic_tb_nopending_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT60_UNION
 struct description   : PERI_STAT60 Register structure definition
                        Address Offset:0x10CC Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器60（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_rd_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT60_UNION;
#endif
#define SOC_PCTRL_PERI_STAT60_qic_rd_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT60_qic_rd_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT61_UNION
 struct description   : PERI_STAT61 Register structure definition
                        Address Offset:0x10D0 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器61（QIC nopending）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_wr_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT61_UNION;
#endif
#define SOC_PCTRL_PERI_STAT61_qic_wr_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT61_qic_wr_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT62_UNION
 struct description   : PERI_STAT62 Register structure definition
                        Address Offset:0x10D4 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器62（QIC nopending）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_tb_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT62_UNION;
#endif
#define SOC_PCTRL_PERI_STAT62_qic_tb_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT62_qic_tb_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT63_UNION
 struct description   : PERI_STAT63 Register structure definition
                        Address Offset:0x10D8 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器63（QIC nopending）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdms_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT63_UNION;
#endif
#define SOC_PCTRL_PERI_STAT63_qic_pdms_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT63_qic_pdms_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT64_UNION
 struct description   : PERI_STAT64 Register structure definition
                        Address Offset:0x10DC Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器64（QIC nopending）
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdms_nopending_center_crossbar_1 : 32; /* bit[0-31]: QIC CC的nopending */
    } reg;
} SOC_PCTRL_PERI_STAT64_UNION;
#endif
#define SOC_PCTRL_PERI_STAT64_qic_pdms_nopending_center_crossbar_1_START  (0)
#define SOC_PCTRL_PERI_STAT64_qic_pdms_nopending_center_crossbar_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT66_UNION
 struct description   : PERI_STAT66 Register structure definition
                        Address Offset:0x10E0 Initial:0x0 Width:32
 register description : 通用外设状态寄存器66--保留给。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0] :  */
        unsigned int  reserved_1: 1;  /* bit[1] :  */
        unsigned int  reserved_2: 1;  /* bit[2] :  */
        unsigned int  reserved_3: 1;  /* bit[3] :  */
        unsigned int  reserved_4: 1;  /* bit[4] :  */
        unsigned int  reserved_5: 1;  /* bit[5] :  */
        unsigned int  reserved_6: 1;  /* bit[6] :  */
        unsigned int  reserved_7: 1;  /* bit[7] :  */
        unsigned int  reserved_8: 1;  /* bit[8] :  */
        unsigned int  reserved_9: 1;  /* bit[9] :  */
        unsigned int  reserved_10: 1;  /* bit[10]:  */
        unsigned int  reserved_11: 1;  /* bit[11]:  */
        unsigned int  reserved_12: 1;  /* bit[12]:  */
        unsigned int  reserved_13: 1;  /* bit[13]:  */
        unsigned int  reserved_14: 1;  /* bit[14]:  */
        unsigned int  reserved_15: 1;  /* bit[15]:  */
        unsigned int  reserved_16: 1;  /* bit[16]:  */
        unsigned int  reserved_17: 1;  /* bit[17]:  */
        unsigned int  reserved_18: 1;  /* bit[18]:  */
        unsigned int  reserved_19: 1;  /* bit[19]:  */
        unsigned int  reserved_20: 1;  /* bit[20]:  */
        unsigned int  reserved_21: 1;  /* bit[21]:  */
        unsigned int  reserved_22: 1;  /* bit[22]:  */
        unsigned int  reserved_23: 1;  /* bit[23]:  */
        unsigned int  reserved_24: 1;  /* bit[24]:  */
        unsigned int  reserved_25: 1;  /* bit[25]:  */
        unsigned int  reserved_26: 1;  /* bit[26]:  */
        unsigned int  reserved_27: 1;  /* bit[27]:  */
        unsigned int  reserved_28: 1;  /* bit[28]:  */
        unsigned int  reserved_29: 1;  /* bit[29]:  */
        unsigned int  reserved_30: 1;  /* bit[30]:  */
        unsigned int  reserved_31: 1;  /* bit[31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT66_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT67_UNION
 struct description   : PERI_STAT67 Register structure definition
                        Address Offset:0x10E4 Initial:0xFFFFFF Width:32
 register description : 通用外设状态寄存器67--保留给。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 1;  /* bit[1]   :  */
        unsigned int  reserved_2: 1;  /* bit[2]   :  */
        unsigned int  reserved_3: 1;  /* bit[3]   :  */
        unsigned int  reserved_4: 1;  /* bit[4]   :  */
        unsigned int  reserved_5: 1;  /* bit[5]   :  */
        unsigned int  reserved_6: 1;  /* bit[6]   :  */
        unsigned int  reserved_7: 1;  /* bit[7]   :  */
        unsigned int  reserved_8: 24; /* bit[8-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT67_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT68_UNION
 struct description   : PERI_STAT68 Register structure definition
                        Address Offset:0x10E8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器68。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT68_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT69_UNION
 struct description   : PERI_STAT69 Register structure definition
                        Address Offset:0x10EC Initial:0x00000 Width:32
 register description : 通用外设状态寄存器69。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0                              : 1;  /* bit[0]    :  */
        unsigned int  qic_m1_comb_intr_lpmcu_acpu             : 1;  /* bit[1]    : qic回读 */
        unsigned int  qic_m2_comb_intr_lpmcu_acpu             : 1;  /* bit[2]    : qic回读 */
        unsigned int  qic_npu_comb_intr_lpmcu_acpu            : 1;  /* bit[3]    : qic回读 */
        unsigned int  intr_qice_qic_ns_to_iomcu_lpmcu_mdm_gic : 1;  /* bit[4]    : 回读 */
        unsigned int  intr_qic_probe_hsdt0_qic                : 1;  /* bit[5]    : qic回读 */
        unsigned int  intr_qic_probe_hsdt1_qic                : 1;  /* bit[6]    : qic回读 */
        unsigned int  intr_qic_probe_gpu_qic                  : 1;  /* bit[7]    : qic回读 */
        unsigned int  intr_qic_probe_cpu_qic                  : 1;  /* bit[8]    : qic回读 */
        unsigned int  intr_qic_probe_cfg_dma                  : 1;  /* bit[9]    : cfg dma qic所有中断相或后的中断 */
        unsigned int  intr_qic_probe_crossbar                 : 1;  /* bit[10]   : cc qic所有中断相或后的中断 */
        unsigned int  intr_qic_probe_modemperi                : 1;  /* bit[11]   : modemperi qic所有中断相或后的中断 */
        unsigned int  qic_ao_comb_intr_lpmcu_acpu             : 1;  /* bit[12]   : ao的组合qic信号回读 */
        unsigned int  intr_qic_probe_gpu_data_qic             : 1;  /* bit[13]   : qic回读 */
        unsigned int  qic_nputs_comb_intr_lpmcu_acpu          : 1;  /* bit[14]   :  */
        unsigned int  reserved_1                              : 17; /* bit[15-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT69_UNION;
#endif
#define SOC_PCTRL_PERI_STAT69_qic_m1_comb_intr_lpmcu_acpu_START              (1)
#define SOC_PCTRL_PERI_STAT69_qic_m1_comb_intr_lpmcu_acpu_END                (1)
#define SOC_PCTRL_PERI_STAT69_qic_m2_comb_intr_lpmcu_acpu_START              (2)
#define SOC_PCTRL_PERI_STAT69_qic_m2_comb_intr_lpmcu_acpu_END                (2)
#define SOC_PCTRL_PERI_STAT69_qic_npu_comb_intr_lpmcu_acpu_START             (3)
#define SOC_PCTRL_PERI_STAT69_qic_npu_comb_intr_lpmcu_acpu_END               (3)
#define SOC_PCTRL_PERI_STAT69_intr_qice_qic_ns_to_iomcu_lpmcu_mdm_gic_START  (4)
#define SOC_PCTRL_PERI_STAT69_intr_qice_qic_ns_to_iomcu_lpmcu_mdm_gic_END    (4)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_hsdt0_qic_START                 (5)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_hsdt0_qic_END                   (5)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_hsdt1_qic_START                 (6)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_hsdt1_qic_END                   (6)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_gpu_qic_START                   (7)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_gpu_qic_END                     (7)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_cpu_qic_START                   (8)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_cpu_qic_END                     (8)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_cfg_dma_START                   (9)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_cfg_dma_END                     (9)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_crossbar_START                  (10)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_crossbar_END                    (10)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_modemperi_START                 (11)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_modemperi_END                   (11)
#define SOC_PCTRL_PERI_STAT69_qic_ao_comb_intr_lpmcu_acpu_START              (12)
#define SOC_PCTRL_PERI_STAT69_qic_ao_comb_intr_lpmcu_acpu_END                (12)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_gpu_data_qic_START              (13)
#define SOC_PCTRL_PERI_STAT69_intr_qic_probe_gpu_data_qic_END                (13)
#define SOC_PCTRL_PERI_STAT69_qic_nputs_comb_intr_lpmcu_acpu_START           (14)
#define SOC_PCTRL_PERI_STAT69_qic_nputs_comb_intr_lpmcu_acpu_END             (14)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT70_UNION
 struct description   : PERI_STAT70 Register structure definition
                        Address Offset:0x10F0 Initial:0x0 Width:32
 register description : 通用外设状态寄存器70。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_ddrc0_err     : 1;  /* bit[0]    : 通道0的dmc的中断线； */
        unsigned int  intr_ddrc1_err     : 1;  /* bit[1]    : 通道1的dmc的中断线； */
        unsigned int  intr_ddrc2_err     : 1;  /* bit[2]    : 通道2的dmc的中断线； */
        unsigned int  intr_ddrc3_err     : 1;  /* bit[3]    : 通道3的dmc的中断线； */
        unsigned int  intr_ddrphy        : 4;  /* bit[4-7]  : 通道3~0的Pack的中断信号线； */
        unsigned int  intr_uce0_wdog     : 1;  /* bit[8]    : 通道0的uce内部的watch dog中断线； */
        unsigned int  intr_uce1_wdog     : 1;  /* bit[9]    : 通道1的uce内部的watch dog中断线； */
        unsigned int  intr_uce2_wdog     : 1;  /* bit[10]   : 通道2的uce内部的watch dog中断线； */
        unsigned int  intr_uce3_wdog     : 1;  /* bit[11]   : 通道3的uce内部的watch dog中断线； */
        unsigned int  intr_ddrc_fatal    : 4;  /* bit[12-15]: 通道0~3的fatal中断信号线； */
        unsigned int  intr_ddrca0_reset  : 1;  /* bit[16]   : 通道0的reset中断信号线； */
        unsigned int  intr_ddrca1_reset  : 1;  /* bit[17]   : 通道1的reset中断信号线； */
        unsigned int  intr_ddrca2_reset  : 1;  /* bit[18]   : 通道2的reset中断信号线； */
        unsigned int  intr_ddrca3_reset  : 1;  /* bit[19]   : 通道3的reset中断信号线； */
        unsigned int  intr_ddrc0_normal  : 1;  /* bit[20]   : 通道0的normal中断信号线； */
        unsigned int  intr_ddrc1_normal  : 1;  /* bit[21]   : 通道1的normal中断信号线； */
        unsigned int  intr_ddrc2_normal  : 1;  /* bit[22]   : 通道2的normal中断信号线； */
        unsigned int  intr_ddrc3_normal  : 1;  /* bit[23]   : 通道3的normal中断信号线； */
        unsigned int  intr_uce0_rtactive : 1;  /* bit[24]   : 通道0的uce_rtactive中断信号线； */
        unsigned int  intr_uce1_rtactive : 1;  /* bit[25]   : 通道1的uce_rtactive中断信号线； */
        unsigned int  intr_uce2_rtactive : 1;  /* bit[26]   : 通道2的uce_rtactive中断信号线； */
        unsigned int  intr_uce3_rtactive : 1;  /* bit[27]   : 通道3的uce_rtactive中断信号线； */
        unsigned int  reserved           : 4;  /* bit[28-31]: 置0 */
    } reg;
} SOC_PCTRL_PERI_STAT70_UNION;
#endif
#define SOC_PCTRL_PERI_STAT70_intr_ddrc0_err_START      (0)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc0_err_END        (0)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc1_err_START      (1)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc1_err_END        (1)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc2_err_START      (2)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc2_err_END        (2)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc3_err_START      (3)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc3_err_END        (3)
#define SOC_PCTRL_PERI_STAT70_intr_ddrphy_START         (4)
#define SOC_PCTRL_PERI_STAT70_intr_ddrphy_END           (7)
#define SOC_PCTRL_PERI_STAT70_intr_uce0_wdog_START      (8)
#define SOC_PCTRL_PERI_STAT70_intr_uce0_wdog_END        (8)
#define SOC_PCTRL_PERI_STAT70_intr_uce1_wdog_START      (9)
#define SOC_PCTRL_PERI_STAT70_intr_uce1_wdog_END        (9)
#define SOC_PCTRL_PERI_STAT70_intr_uce2_wdog_START      (10)
#define SOC_PCTRL_PERI_STAT70_intr_uce2_wdog_END        (10)
#define SOC_PCTRL_PERI_STAT70_intr_uce3_wdog_START      (11)
#define SOC_PCTRL_PERI_STAT70_intr_uce3_wdog_END        (11)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc_fatal_START     (12)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc_fatal_END       (15)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca0_reset_START   (16)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca0_reset_END     (16)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca1_reset_START   (17)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca1_reset_END     (17)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca2_reset_START   (18)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca2_reset_END     (18)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca3_reset_START   (19)
#define SOC_PCTRL_PERI_STAT70_intr_ddrca3_reset_END     (19)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc0_normal_START   (20)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc0_normal_END     (20)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc1_normal_START   (21)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc1_normal_END     (21)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc2_normal_START   (22)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc2_normal_END     (22)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc3_normal_START   (23)
#define SOC_PCTRL_PERI_STAT70_intr_ddrc3_normal_END     (23)
#define SOC_PCTRL_PERI_STAT70_intr_uce0_rtactive_START  (24)
#define SOC_PCTRL_PERI_STAT70_intr_uce0_rtactive_END    (24)
#define SOC_PCTRL_PERI_STAT70_intr_uce1_rtactive_START  (25)
#define SOC_PCTRL_PERI_STAT70_intr_uce1_rtactive_END    (25)
#define SOC_PCTRL_PERI_STAT70_intr_uce2_rtactive_START  (26)
#define SOC_PCTRL_PERI_STAT70_intr_uce2_rtactive_END    (26)
#define SOC_PCTRL_PERI_STAT70_intr_uce3_rtactive_START  (27)
#define SOC_PCTRL_PERI_STAT70_intr_uce3_rtactive_END    (27)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT71_UNION
 struct description   : PERI_STAT71 Register structure definition
                        Address Offset:0x10F4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器71。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  socp_hac_gif_axi_st_low : 32; /* bit[0-31]: SOCP的hac_gif debug信号
                                                                  {aw_os_cnt[4:0],
                                                                  ar_os_cnt[4:0],
                                                                  axi_w_st[1:0],
                                                                  axi_r_st,
                                                                  xm_awready,
                                                                  xm_wready,
                                                                  xm_bready,
                                                                  xm_arreadry,
                                                                  xm_rready,
                                                                  } */
    } reg;
} SOC_PCTRL_PERI_STAT71_UNION;
#endif
#define SOC_PCTRL_PERI_STAT71_socp_hac_gif_axi_st_low_START  (0)
#define SOC_PCTRL_PERI_STAT71_socp_hac_gif_axi_st_low_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT72_UNION
 struct description   : PERI_STAT72 Register structure definition
                        Address Offset:0x10F8 Initial:0x0000 Width:32
 register description : 通用外设状态寄存器72。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipf_hac_gif_axi_st : 18; /* bit[0-17] : IPF的hac_gif debug信号
                                                              {aw_os_cnt[4:0],
                                                              ar_os_cnt[4:0],
                                                              axi_w_st[1:0],
                                                              axi_r_st,
                                                              xm_awready,
                                                              xm_wready,
                                                              xm_bready,
                                                              xm_arreadry,
                                                              xm_rready,
                                                              } */
        unsigned int  reserved           : 14; /* bit[18-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT72_UNION;
#endif
#define SOC_PCTRL_PERI_STAT72_ipf_hac_gif_axi_st_START  (0)
#define SOC_PCTRL_PERI_STAT72_ipf_hac_gif_axi_st_END    (17)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT73_UNION
 struct description   : PERI_STAT73 Register structure definition
                        Address Offset:0x10FC Initial:0x0000 Width:32
 register description : 通用外设状态寄存器73。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  spe_hac_gif_axi_st : 18; /* bit[0-17] : SPE的hac_gif debug信号
                                                              {aw_os_cnt[4:0],
                                                              ar_os_cnt[4:0],
                                                              axi_w_st[1:0],
                                                              axi_r_st,
                                                              xm_awready,
                                                              xm_wready,
                                                              xm_bready,
                                                              xm_arreadry,
                                                              xm_rready,
                                                              } */
        unsigned int  reserved           : 14; /* bit[18-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT73_UNION;
#endif
#define SOC_PCTRL_PERI_STAT73_spe_hac_gif_axi_st_START  (0)
#define SOC_PCTRL_PERI_STAT73_spe_hac_gif_axi_st_END    (17)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT74_UNION
 struct description   : PERI_STAT74 Register structure definition
                        Address Offset:0x1100 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器74-mtcmos回读。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qice_gpu_mtcmos_ack : 1;  /* bit[0]   : qice的mtcmos回读信号 */
        unsigned int  reserved_0          : 1;  /* bit[1]   :  */
        unsigned int  reserved_1          : 1;  /* bit[2]   :  */
        unsigned int  reserved_2          : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT74_UNION;
#endif
#define SOC_PCTRL_PERI_STAT74_qice_gpu_mtcmos_ack_START  (0)
#define SOC_PCTRL_PERI_STAT74_qice_gpu_mtcmos_ack_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT75_UNION
 struct description   : PERI_STAT75 Register structure definition
                        Address Offset:0x1104 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器75。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  socp_hac_gif_axi_st_high : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT75_UNION;
#endif
#define SOC_PCTRL_PERI_STAT75_socp_hac_gif_axi_st_high_START  (0)
#define SOC_PCTRL_PERI_STAT75_socp_hac_gif_axi_st_high_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT76_UNION
 struct description   : PERI_STAT76 Register structure definition
                        Address Offset:0x1108 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器76。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  eicc_hac_gif_axi_st_low : 32; /* bit[0-31]: HAC_GIF0的AXI状态:
                                                                  bit17:13 写os;
                                                                  bit12:8 读os;
                                                                  bit7:6 写状态;
                                                                  bit5 读状态;
                                                                  bit4 awready;
                                                                  bit3 wready;
                                                                  bit2 bready;
                                                                  bit1 arready;
                                                                  bit0 rready; */
    } reg;
} SOC_PCTRL_PERI_STAT76_UNION;
#endif
#define SOC_PCTRL_PERI_STAT76_eicc_hac_gif_axi_st_low_START  (0)
#define SOC_PCTRL_PERI_STAT76_eicc_hac_gif_axi_st_low_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT77_UNION
 struct description   : PERI_STAT77 Register structure definition
                        Address Offset:0x110C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器77。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  eicc_hac_gif_axi_st_high : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT77_UNION;
#endif
#define SOC_PCTRL_PERI_STAT77_eicc_hac_gif_axi_st_high_START  (0)
#define SOC_PCTRL_PERI_STAT77_eicc_hac_gif_axi_st_high_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT78_UNION
 struct description   : PERI_STAT78 Register structure definition
                        Address Offset:0x1110 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器78。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT78_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT79_UNION
 struct description   : PERI_STAT79 Register structure definition
                        Address Offset:0x1114 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器79。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT79_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT80_UNION
 struct description   : PERI_STAT80 Register structure definition
                        Address Offset:0x1118 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器80。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT80_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT81_UNION
 struct description   : PERI_STAT81 Register structure definition
                        Address Offset:0x111C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器81。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT81_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT82_UNION
 struct description   : PERI_STAT82 Register structure definition
                        Address Offset:0x1120 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器82。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT82_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT83_UNION
 struct description   : PERI_STAT83 Register structure definition
                        Address Offset:0x1124 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器83。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT83_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT84_UNION
 struct description   : PERI_STAT84 Register structure definition
                        Address Offset:0x1128 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器84。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT84_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT85_UNION
 struct description   : PERI_STAT85 Register structure definition
                        Address Offset:0x112C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器85。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT85_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT86_UNION
 struct description   : PERI_STAT86 Register structure definition
                        Address Offset:0x1130 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器86。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT86_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT87_UNION
 struct description   : PERI_STAT87 Register structure definition
                        Address Offset:0x1134 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器87。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT87_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT88_UNION
 struct description   : PERI_STAT88 Register structure definition
                        Address Offset:0x1138 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器88。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT88_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT89_UNION
 struct description   : PERI_STAT89 Register structure definition
                        Address Offset:0x113C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器89。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT89_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT90_UNION
 struct description   : PERI_STAT90 Register structure definition
                        Address Offset:0x1140 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器90。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT90_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT91_UNION
 struct description   : PERI_STAT91 Register structure definition
                        Address Offset:0x1144 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器91。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT91_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT92_UNION
 struct description   : PERI_STAT92 Register structure definition
                        Address Offset:0x1148 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器92。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT92_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT93_UNION
 struct description   : PERI_STAT93 Register structure definition
                        Address Offset:0x114C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器93。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT93_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT94_UNION
 struct description   : PERI_STAT94 Register structure definition
                        Address Offset:0x1150 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器94。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tsp0_wakeup_irps_grp_stat : 32; /* bit[0-31]: tsp0_wakeup_irps_grp_stat[31:0]
                                                                    0-7bit 分别对应tsp0_wakeup_irps_stat_0~7按位或的结果。
                                                                    8-31bit 接0 */
    } reg;
} SOC_PCTRL_PERI_STAT94_UNION;
#endif
#define SOC_PCTRL_PERI_STAT94_tsp0_wakeup_irps_grp_stat_START  (0)
#define SOC_PCTRL_PERI_STAT94_tsp0_wakeup_irps_grp_stat_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT95_UNION
 struct description   : PERI_STAT95 Register structure definition
                        Address Offset:0x1154 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器95。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tsp0_wakeup_irps_stat_0 : 32; /* bit[0-31]: tsp0_wakeup_irps_stat[31：0] */
    } reg;
} SOC_PCTRL_PERI_STAT95_UNION;
#endif
#define SOC_PCTRL_PERI_STAT95_tsp0_wakeup_irps_stat_0_START  (0)
#define SOC_PCTRL_PERI_STAT95_tsp0_wakeup_irps_stat_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT96_UNION
 struct description   : PERI_STAT96 Register structure definition
                        Address Offset:0x1158 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器96。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tsp0_wakeup_irps_stat_1 : 32; /* bit[0-31]: tsp0_wakeup_irps_stat[63：32] */
    } reg;
} SOC_PCTRL_PERI_STAT96_UNION;
#endif
#define SOC_PCTRL_PERI_STAT96_tsp0_wakeup_irps_stat_1_START  (0)
#define SOC_PCTRL_PERI_STAT96_tsp0_wakeup_irps_stat_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT97_UNION
 struct description   : PERI_STAT97 Register structure definition
                        Address Offset:0x115C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器97。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT97_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT98_UNION
 struct description   : PERI_STAT98 Register structure definition
                        Address Offset:0x1160 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器98。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT98_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT99_UNION
 struct description   : PERI_STAT99 Register structure definition
                        Address Offset:0x1164 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器99。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT99_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT100_UNION
 struct description   : PERI_STAT100 Register structure definition
                        Address Offset:0x1168 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器100。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT100_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT101_UNION
 struct description   : PERI_STAT101 Register structure definition
                        Address Offset:0x116C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器101。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT101_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT102_UNION
 struct description   : PERI_STAT102 Register structure definition
                        Address Offset:0x1170 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器102。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT102_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT103_UNION
 struct description   : PERI_STAT103 Register structure definition
                        Address Offset:0x1174 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器103。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT103_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT104_UNION
 struct description   : PERI_STAT104 Register structure definition
                        Address Offset:0x1178 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器104。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT104_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT105_UNION
 struct description   : PERI_STAT105 Register structure definition
                        Address Offset:0x117C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器105。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT105_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT106_UNION
 struct description   : PERI_STAT106 Register structure definition
                        Address Offset:0x1180 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器106。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT106_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT107_UNION
 struct description   : PERI_STAT107 Register structure definition
                        Address Offset:0x1184 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器107。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT107_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT108_UNION
 struct description   : PERI_STAT108 Register structure definition
                        Address Offset:0x1188 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器108。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT108_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT109_UNION
 struct description   : PERI_STAT109 Register structure definition
                        Address Offset:0x118C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器109。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT109_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT110_UNION
 struct description   : PERI_STAT110 Register structure definition
                        Address Offset:0x1190 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器110。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT110_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT111_UNION
 struct description   : PERI_STAT111 Register structure definition
                        Address Offset:0x1194 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器111。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT111_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT112_UNION
 struct description   : PERI_STAT112 Register structure definition
                        Address Offset:0x1198 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器112。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT112_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT113_UNION
 struct description   : PERI_STAT113 Register structure definition
                        Address Offset:0x119C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器113。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT113_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT114_UNION
 struct description   : PERI_STAT114 Register structure definition
                        Address Offset:0x11A0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器114。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT114_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT115_UNION
 struct description   : PERI_STAT115 Register structure definition
                        Address Offset:0x11A4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器115。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT115_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT116_UNION
 struct description   : PERI_STAT116 Register structure definition
                        Address Offset:0x11A8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器116。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT116_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT117_UNION
 struct description   : PERI_STAT117 Register structure definition
                        Address Offset:0x11AC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器117。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT117_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT118_UNION
 struct description   : PERI_STAT118 Register structure definition
                        Address Offset:0x11B0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器118。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT118_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT119_UNION
 struct description   : PERI_STAT119 Register structure definition
                        Address Offset:0x11B4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器119。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT119_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT120_UNION
 struct description   : PERI_STAT120 Register structure definition
                        Address Offset:0x11B8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器120。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT120_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT121_UNION
 struct description   : PERI_STAT121 Register structure definition
                        Address Offset:0x11BC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器121。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT121_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT122_UNION
 struct description   : PERI_STAT122 Register structure definition
                        Address Offset:0x11C0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器122。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT122_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT123_UNION
 struct description   : PERI_STAT123 Register structure definition
                        Address Offset:0x11C4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器123。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT123_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT124_UNION
 struct description   : PERI_STAT124 Register structure definition
                        Address Offset:0x11C8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器124。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT124_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT125_UNION
 struct description   : PERI_STAT125 Register structure definition
                        Address Offset:0x11CC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器125。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT125_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT126_UNION
 struct description   : PERI_STAT126 Register structure definition
                        Address Offset:0x11D0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器126。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT126_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT127_UNION
 struct description   : PERI_STAT127 Register structure definition
                        Address Offset:0x11D4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器127。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tsp0_wakeup_intr_comb_stat : 1;  /* bit[0]   :  */
        unsigned int  tsp1_wakeup_intr_comb_stat : 1;  /* bit[1]   :  */
        unsigned int  tsp2_wakeup_intr_comb_stat : 1;  /* bit[2]   :  */
        unsigned int  reserved                   : 29; /* bit[3-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT127_UNION;
#endif
#define SOC_PCTRL_PERI_STAT127_tsp0_wakeup_intr_comb_stat_START  (0)
#define SOC_PCTRL_PERI_STAT127_tsp0_wakeup_intr_comb_stat_END    (0)
#define SOC_PCTRL_PERI_STAT127_tsp1_wakeup_intr_comb_stat_START  (1)
#define SOC_PCTRL_PERI_STAT127_tsp1_wakeup_intr_comb_stat_END    (1)
#define SOC_PCTRL_PERI_STAT127_tsp2_wakeup_intr_comb_stat_START  (2)
#define SOC_PCTRL_PERI_STAT127_tsp2_wakeup_intr_comb_stat_END    (2)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT128_UNION
 struct description   : PERI_STAT128 Register structure definition
                        Address Offset:0x11D8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器128。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT128_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT129_UNION
 struct description   : PERI_STAT129 Register structure definition
                        Address Offset:0x11DC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器129-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_pktc_cfg_dma_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT129_UNION;
#endif
#define SOC_PCTRL_PERI_STAT129_intr_pktc_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT129_intr_pktc_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT130_UNION
 struct description   : PERI_STAT130 Register structure definition
                        Address Offset:0x11E0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器130-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_em_cfg_dma_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT130_UNION;
#endif
#define SOC_PCTRL_PERI_STAT130_intr_em_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT130_intr_em_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT131_UNION
 struct description   : PERI_STAT131 Register structure definition
                        Address Offset:0x11E4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器131-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_em_mdmperi_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT131_UNION;
#endif
#define SOC_PCTRL_PERI_STAT131_intr_em_mdmperi_0_START  (0)
#define SOC_PCTRL_PERI_STAT131_intr_em_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT132_UNION
 struct description   : PERI_STAT132 Register structure definition
                        Address Offset:0x11E8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器132-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_pktc_center_crossbar_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT132_UNION;
#endif
#define SOC_PCTRL_PERI_STAT132_intr_pktc_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT132_intr_pktc_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT133_UNION
 struct description   : PERI_STAT133 Register structure definition
                        Address Offset:0x11EC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器133-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  intr_em_center_crossbar_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT133_UNION;
#endif
#define SOC_PCTRL_PERI_STAT133_intr_em_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT133_intr_em_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT134_UNION
 struct description   : PERI_STAT134 Register structure definition
                        Address Offset:0x11F0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器134-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_timeout_st_cfg_dma_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT134_UNION;
#endif
#define SOC_PCTRL_PERI_STAT134_qic_timeout_st_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT134_qic_timeout_st_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT135_UNION
 struct description   : PERI_STAT135 Register structure definition
                        Address Offset:0x11F4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器135-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_timeout_st_mdmperi_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT135_UNION;
#endif
#define SOC_PCTRL_PERI_STAT135_qic_timeout_st_mdmperi_0_START  (0)
#define SOC_PCTRL_PERI_STAT135_qic_timeout_st_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT136_UNION
 struct description   : PERI_STAT136 Register structure definition
                        Address Offset:0x11F8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器136-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_timeout_st_center_crossbar_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT136_UNION;
#endif
#define SOC_PCTRL_PERI_STAT136_qic_timeout_st_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT136_qic_timeout_st_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT137_UNION
 struct description   : PERI_STAT137 Register structure definition
                        Address Offset:0x11FC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器137-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_cfg_dma_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT137_UNION;
#endif
#define SOC_PCTRL_PERI_STAT137_qic_pdm_state_pdm_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT137_qic_pdm_state_pdm_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT138_UNION
 struct description   : PERI_STAT138 Register structure definition
                        Address Offset:0x1200 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器138-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT138_UNION;
#endif
#define SOC_PCTRL_PERI_STAT138_qic_pdm_state_pdm_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT138_qic_pdm_state_pdm_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT139_UNION
 struct description   : PERI_STAT139 Register structure definition
                        Address Offset:0x1204 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器139-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_1 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT139_UNION;
#endif
#define SOC_PCTRL_PERI_STAT139_qic_pdm_state_pdm_center_crossbar_1_START  (0)
#define SOC_PCTRL_PERI_STAT139_qic_pdm_state_pdm_center_crossbar_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT140_UNION
 struct description   : PERI_STAT140 Register structure definition
                        Address Offset:0x1208 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器140-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_2 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT140_UNION;
#endif
#define SOC_PCTRL_PERI_STAT140_qic_pdm_state_pdm_center_crossbar_2_START  (0)
#define SOC_PCTRL_PERI_STAT140_qic_pdm_state_pdm_center_crossbar_2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT141_UNION
 struct description   : PERI_STAT141 Register structure definition
                        Address Offset:0x120C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器141-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_3 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT141_UNION;
#endif
#define SOC_PCTRL_PERI_STAT141_qic_pdm_state_pdm_center_crossbar_3_START  (0)
#define SOC_PCTRL_PERI_STAT141_qic_pdm_state_pdm_center_crossbar_3_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT142_UNION
 struct description   : PERI_STAT142 Register structure definition
                        Address Offset:0x1210 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器142-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_4 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT142_UNION;
#endif
#define SOC_PCTRL_PERI_STAT142_qic_pdm_state_pdm_center_crossbar_4_START  (0)
#define SOC_PCTRL_PERI_STAT142_qic_pdm_state_pdm_center_crossbar_4_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT143_UNION
 struct description   : PERI_STAT143 Register structure definition
                        Address Offset:0x1214 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器143-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_5 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT143_UNION;
#endif
#define SOC_PCTRL_PERI_STAT143_qic_pdm_state_pdm_center_crossbar_5_START  (0)
#define SOC_PCTRL_PERI_STAT143_qic_pdm_state_pdm_center_crossbar_5_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT144_UNION
 struct description   : PERI_STAT144 Register structure definition
                        Address Offset:0x1218 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器144-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pdm_state_pdm_center_crossbar_6 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT144_UNION;
#endif
#define SOC_PCTRL_PERI_STAT144_qic_pdm_state_pdm_center_crossbar_6_START  (0)
#define SOC_PCTRL_PERI_STAT144_qic_pdm_state_pdm_center_crossbar_6_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT145_UNION
 struct description   : PERI_STAT145 Register structure definition
                        Address Offset:0x121C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器145-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pds_state_pds_cfg_dma_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT145_UNION;
#endif
#define SOC_PCTRL_PERI_STAT145_qic_pds_state_pds_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT145_qic_pds_state_pds_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT146_UNION
 struct description   : PERI_STAT146 Register structure definition
                        Address Offset:0x1220 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器146-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pds_state_pds_center_crossbar_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT146_UNION;
#endif
#define SOC_PCTRL_PERI_STAT146_qic_pds_state_pds_center_crossbar_0_START  (0)
#define SOC_PCTRL_PERI_STAT146_qic_pds_state_pds_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT147_UNION
 struct description   : PERI_STAT147 Register structure definition
                        Address Offset:0x1224 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器147-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pds_state_pds_center_crossbar_1 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT147_UNION;
#endif
#define SOC_PCTRL_PERI_STAT147_qic_pds_state_pds_center_crossbar_1_START  (0)
#define SOC_PCTRL_PERI_STAT147_qic_pds_state_pds_center_crossbar_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT148_UNION
 struct description   : PERI_STAT148 Register structure definition
                        Address Offset:0x1228 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器148-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pds_state_pds_center_crossbar_2 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT148_UNION;
#endif
#define SOC_PCTRL_PERI_STAT148_qic_pds_state_pds_center_crossbar_2_START  (0)
#define SOC_PCTRL_PERI_STAT148_qic_pds_state_pds_center_crossbar_2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT149_UNION
 struct description   : PERI_STAT149 Register structure definition
                        Address Offset:0x122C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器149-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qic_pds_state_pds_center_crossbar_3 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT149_UNION;
#endif
#define SOC_PCTRL_PERI_STAT149_qic_pds_state_pds_center_crossbar_3_START  (0)
#define SOC_PCTRL_PERI_STAT149_qic_pds_state_pds_center_crossbar_3_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT150_UNION
 struct description   : PERI_STAT150 Register structure definition
                        Address Offset:0x1230 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器150-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dfx_busy_ca_cfg_dma_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT150_UNION;
#endif
#define SOC_PCTRL_PERI_STAT150_dfx_busy_ca_cfg_dma_0_START  (0)
#define SOC_PCTRL_PERI_STAT150_dfx_busy_ca_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT151_UNION
 struct description   : PERI_STAT151 Register structure definition
                        Address Offset:0x1234 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器151-QIC intr中断上报
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  dfx_busy_ca_center_mdmperi_0 : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT151_UNION;
#endif
#define SOC_PCTRL_PERI_STAT151_dfx_busy_ca_center_mdmperi_0_START  (0)
#define SOC_PCTRL_PERI_STAT151_dfx_busy_ca_center_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT152_UNION
 struct description   : PERI_STAT152 Register structure definition
                        Address Offset:0x1238 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器152
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT152_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT153_UNION
 struct description   : PERI_STAT153 Register structure definition
                        Address Offset:0x123C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器153
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT153_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT154_UNION
 struct description   : PERI_STAT154 Register structure definition
                        Address Offset:0x1240 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器154
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT154_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT155_UNION
 struct description   : PERI_STAT155 Register structure definition
                        Address Offset:0x1244 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器155
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT155_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT156_UNION
 struct description   : PERI_STAT156 Register structure definition
                        Address Offset:0x1248 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器156
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT156_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT157_UNION
 struct description   : PERI_STAT157 Register structure definition
                        Address Offset:0x124C Initial:0x00 Width:32
 register description : 通用外设状态寄存器157
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  uce_prog_stat_a : 8;  /* bit[0-7]  :  */
        unsigned int  uce_prog_stat_b : 8;  /* bit[8-15] :  */
        unsigned int  uce_prog_stat_c : 8;  /* bit[16-23]:  */
        unsigned int  uce_prog_stat_d : 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT157_UNION;
#endif
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_a_START  (0)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_a_END    (7)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_b_START  (8)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_b_END    (15)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_c_START  (16)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_c_END    (23)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_d_START  (24)
#define SOC_PCTRL_PERI_STAT157_uce_prog_stat_d_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT158_UNION
 struct description   : PERI_STAT158 Register structure definition
                        Address Offset:0x1250 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器158
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT158_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT159_UNION
 struct description   : PERI_STAT159 Register structure definition
                        Address Offset:0x1254 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器159
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT159_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL0_UNION
 struct description   : PERI_CTRL0 Register structure definition
                        Address Offset:0x1800 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0           : 1;  /* bit[0]    :  */
        unsigned int  reserved_1           : 1;  /* bit[1]    :  */
        unsigned int  reserved_2           : 1;  /* bit[2]    :  */
        unsigned int  reserved_3           : 1;  /* bit[3]    :  */
        unsigned int  reserved_4           : 1;  /* bit[4]    :  */
        unsigned int  reserved_5           : 1;  /* bit[5]    :  */
        unsigned int  qtpe_icfg_ckg_rs_byp : 1;  /* bit[6]    : QICE与QIC MEDIA1/2之间插入QTPE RS的+O242ckg bypass信号 */
        unsigned int  cssys_etr_x2x_ckg_en : 1;  /* bit[7]    : ETR接口X2X桥自动门控使能控制（静态配置） */
        unsigned int  reserved_6           : 1;  /* bit[8]    : SYSQIC控制防挂死模块的bypass 默认0 */
        unsigned int  h2p_ckg_en_qicecfg   : 1;  /* bit[9]    : LPMCU AHB转APB的minibus的自动时钟门控使能信号 */
        unsigned int  h2h_ckg_en_dmc       : 1;  /* bit[10]   : LPMCU AHB转APB的minibus的自动时钟门控使能信号 */
        unsigned int  reserved_7           : 1;  /* bit[11]   : 一个4to1_minibus_ckg_en信号，默认值0，
                                                                0：bypass时钟自动门控，即时钟常开；
                                                                1：打开自动门控， */
        unsigned int  reserved_8           : 4;  /* bit[12-15]: bit[15:0]：reserved
                                                                注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  PERI_CTRL0_msk       : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                与bit[15:0]命令位一一对应。
                                                                0：屏蔽命令位，命令位写操作无效；
                                                                1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL0_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL0_qtpe_icfg_ckg_rs_byp_START  (6)
#define SOC_PCTRL_PERI_CTRL0_qtpe_icfg_ckg_rs_byp_END    (6)
#define SOC_PCTRL_PERI_CTRL0_cssys_etr_x2x_ckg_en_START  (7)
#define SOC_PCTRL_PERI_CTRL0_cssys_etr_x2x_ckg_en_END    (7)
#define SOC_PCTRL_PERI_CTRL0_h2p_ckg_en_qicecfg_START    (9)
#define SOC_PCTRL_PERI_CTRL0_h2p_ckg_en_qicecfg_END      (9)
#define SOC_PCTRL_PERI_CTRL0_h2h_ckg_en_dmc_START        (10)
#define SOC_PCTRL_PERI_CTRL0_h2h_ckg_en_dmc_END          (10)
#define SOC_PCTRL_PERI_CTRL0_PERI_CTRL0_msk_START        (16)
#define SOC_PCTRL_PERI_CTRL0_PERI_CTRL0_msk_END          (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL1_UNION
 struct description   : PERI_CTRL1 Register structure definition
                        Address Offset:0x1804 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  test_point_sel_for_core : 6;  /* bit[0-5]  : bit[5:0] :test_point_sel
                                                                   测试相关，从可选的测试数据中选择一组输出。给core sig misc使用
                                                                   详见《Hi36B0 V100 ES Test Point合并分组表.xlsm》 */
        unsigned int  test_point_sel_for_peri : 4;  /* bit[6-9]  : 给PERI区的testpoint做mux的信号，接在PERI SIG MISC接。 */
        unsigned int  reserved                : 6;  /* bit[10-15]: 保留 */
        unsigned int  peri_ctrl1_msk          : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                   与bit[15:0]命令位一一对应。
                                                                   0：屏蔽命令位，命令位写操作无效；
                                                                   1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL1_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL1_test_point_sel_for_core_START  (0)
#define SOC_PCTRL_PERI_CTRL1_test_point_sel_for_core_END    (5)
#define SOC_PCTRL_PERI_CTRL1_test_point_sel_for_peri_START  (6)
#define SOC_PCTRL_PERI_CTRL1_test_point_sel_for_peri_END    (9)
#define SOC_PCTRL_PERI_CTRL1_peri_ctrl1_msk_START           (16)
#define SOC_PCTRL_PERI_CTRL1_peri_ctrl1_msk_END             (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL2_UNION
 struct description   : PERI_CTRL2 Register structure definition
                        Address Offset:0x1808 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bfmux_mem_ctrl_sd_dmc_pack : 4;  /* bit[0-3]  : DMC的SD控制信号。 默认为1 */
        unsigned int  bfmux_dmc_d_ret_mem_ds_in  : 1;  /* bit[4]    : DMC的DS控制信号 */
        unsigned int  bfmux_dmc_c_ret_mem_ds_in  : 1;  /* bit[5]    : DMC的DS控制信号 */
        unsigned int  bfmux_dmc_b_ret_mem_ds_in  : 1;  /* bit[6]    : DMC的DS控制信号 */
        unsigned int  bfmux_dmc_a_ret_mem_ds_in  : 1;  /* bit[7]    : DMC的DS控制信号 */
        unsigned int  reserved_0                 : 1;  /* bit[8]    : 保留 */
        unsigned int  reserved_1                 : 5;  /* bit[9-13] : 保留 */
        unsigned int  reserved_2                 : 2;  /* bit[14-15]: 保留 */
        unsigned int  peri_ctrl2_msk             : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                      与bit[15:0]命令位一一对应。
                                                                      0：屏蔽命令位，命令位写操作无效；
                                                                      1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL2_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL2_bfmux_mem_ctrl_sd_dmc_pack_START  (0)
#define SOC_PCTRL_PERI_CTRL2_bfmux_mem_ctrl_sd_dmc_pack_END    (3)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_d_ret_mem_ds_in_START   (4)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_d_ret_mem_ds_in_END     (4)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_c_ret_mem_ds_in_START   (5)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_c_ret_mem_ds_in_END     (5)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_b_ret_mem_ds_in_START   (6)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_b_ret_mem_ds_in_END     (6)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_a_ret_mem_ds_in_START   (7)
#define SOC_PCTRL_PERI_CTRL2_bfmux_dmc_a_ret_mem_ds_in_END     (7)
#define SOC_PCTRL_PERI_CTRL2_peri_ctrl2_msk_START              (16)
#define SOC_PCTRL_PERI_CTRL2_peri_ctrl2_msk_END                (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL3_UNION
 struct description   : PERI_CTRL3 Register structure definition
                        Address Offset:0x181C Initial:0x0000 Width:32
 register description : 通用外设控制寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bfmux_pctrl_mem_ctrl_sd_qice    : 1;  /* bit[0]    : QICE的SD信号，非ret模式的 */
        unsigned int  bfmux_pctrl_mem_ctrl_sd_ffts    : 1;  /* bit[1]    : FFTS的SD信号，非ret模式的 */
        unsigned int  bfmux_mem_ctrl_sd_exmb0         : 1;  /* bit[2]    : bit[14]：pctrl_mem_ctrl_sd_exmb 默认为1，注意：只有在对应的mask位同时写1时，写操作才有效。
                                                                           此信号会在PERI SIG MISC里做DFT MUX后再以注释的名字送出，下同 */
        unsigned int  bfmux_mem_ctrl_sd_exmb1         : 1;  /* bit[3]    : bit[15]：保留 默认为0，注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  bfmux_mem_ctrl_sd_hts           : 1;  /* bit[4]    : HTS的SD信号，非ret模式的 */
        unsigned int  bfmux_mem_ctrl_sd_bootroom      : 1;  /* bit[5]    : bootrom的SD信号，非ret模式的 */
        unsigned int  reserved_0                      : 1;  /* bit[6]    : mudp的SD信号，非ret模式的 */
        unsigned int  reserved_1                      : 1;  /* bit[7]    :  */
        unsigned int  bfmux_pctrl_retmem_ctrl_ds_qice : 8;  /* bit[8-15] : QICE的DS信号，ret模式 */
        unsigned int  peri_ctrl3_msk                  : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                           与bit[15:0]命令位一一对应。
                                                                           0：屏蔽命令位，命令位写操作无效；
                                                                           1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL3_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL3_bfmux_pctrl_mem_ctrl_sd_qice_START     (0)
#define SOC_PCTRL_PERI_CTRL3_bfmux_pctrl_mem_ctrl_sd_qice_END       (0)
#define SOC_PCTRL_PERI_CTRL3_bfmux_pctrl_mem_ctrl_sd_ffts_START     (1)
#define SOC_PCTRL_PERI_CTRL3_bfmux_pctrl_mem_ctrl_sd_ffts_END       (1)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_exmb0_START          (2)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_exmb0_END            (2)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_exmb1_START          (3)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_exmb1_END            (3)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_hts_START            (4)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_hts_END              (4)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_bootroom_START       (5)
#define SOC_PCTRL_PERI_CTRL3_bfmux_mem_ctrl_sd_bootroom_END         (5)
#define SOC_PCTRL_PERI_CTRL3_bfmux_pctrl_retmem_ctrl_ds_qice_START  (8)
#define SOC_PCTRL_PERI_CTRL3_bfmux_pctrl_retmem_ctrl_ds_qice_END    (15)
#define SOC_PCTRL_PERI_CTRL3_peri_ctrl3_msk_START                   (16)
#define SOC_PCTRL_PERI_CTRL3_peri_ctrl3_msk_END                     (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL102_UNION
 struct description   : PERI_CTRL102 Register structure definition
                        Address Offset:0x1820 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器102。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bfmux_mem_ctrl_sd_peri : 1;  /* bit[0]    : bit[0]：pctrl_mem_ctrl_sd_peri
                                                                  Peri底(top_coresight/peri_edmac/spi1/spi4/modem_peri/SC_CFG_CMO_TOP) mem SD
                                                                  默认为0
                                                                  注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  bfmux_mem_ctrl_sd_dsi2 : 1;  /* bit[1]    : DSI的shut down信号，dsi对应的3个mem shut down，请单独实现 */
        unsigned int  bfmux_mem_ctrl_sd_dsi1 : 1;  /* bit[2]    : DSI的shut down信号，dsi对应的3个mem shut down，请单独实现 */
        unsigned int  bfmux_mem_ctrl_sd_dsi0 : 1;  /* bit[3]    : DSI的shut down信号，dsi对应的3个mem shut down，请单独实现
                                                                  默认为0 */
        unsigned int  bfmux_mem_ctrl_sd_sca  : 6;  /* bit[4-9]  : SCA的SD信号 */
        unsigned int  bfmux_mem_ctrl_sd_scb  : 6;  /* bit[10-15]: SCB的SD信号 */
        unsigned int  peri_ctrl102_msk       : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                  与bit[15:0]命令位一一对应。
                                                                  0：屏蔽命令位，命令位写操作无效；
                                                                  1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL102_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_peri_START  (0)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_peri_END    (0)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_dsi2_START  (1)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_dsi2_END    (1)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_dsi1_START  (2)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_dsi1_END    (2)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_dsi0_START  (3)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_dsi0_END    (3)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_sca_START   (4)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_sca_END     (9)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_scb_START   (10)
#define SOC_PCTRL_PERI_CTRL102_bfmux_mem_ctrl_sd_scb_END     (15)
#define SOC_PCTRL_PERI_CTRL102_peri_ctrl102_msk_START        (16)
#define SOC_PCTRL_PERI_CTRL102_peri_ctrl102_msk_END          (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL103_UNION
 struct description   : PERI_CTRL103 Register structure definition
                        Address Offset:0x1824 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器103。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  bfmux_rom_ctrl_sd_dmc_pack_a : 1;  /* bit[0]    : bit[8]：pctrl_rom_ctrl_sd_dmc_pack_a 默认为0，注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  bfmux_rom_ctrl_sd_dmc_pack_b : 1;  /* bit[1]    : bit[9]：pctrl_rom_ctrl_sd_dmc_pack_b 默认为0，注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  bfmux_rom_ctrl_sd_dmc_pack_c : 1;  /* bit[2]    : bit[10]：pctrl_rom_ctrl_sd_dmc_pack_c 默认为0，注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  bfmux_rom_ctrl_sd_dmc_pack_d : 1;  /* bit[3]    : bit[11]：pctrl_rom_ctrl_sd_dmc_pack_d 默认为0，注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  bfmux_mem_ctrl_sd_scc        : 6;  /* bit[4-9]  : SCC的SD信号 */
        unsigned int  bfmux_mem_ctrl_sd_scd        : 6;  /* bit[10-15]: SCD的SD信号 */
        unsigned int  peri_ctrl103_msk             : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                        与bit[15:0]命令位一一对应。
                                                                        0：屏蔽命令位，命令位写操作无效；
                                                                        1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL103_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_a_START  (0)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_a_END    (0)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_b_START  (1)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_b_END    (1)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_c_START  (2)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_c_END    (2)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_d_START  (3)
#define SOC_PCTRL_PERI_CTRL103_bfmux_rom_ctrl_sd_dmc_pack_d_END    (3)
#define SOC_PCTRL_PERI_CTRL103_bfmux_mem_ctrl_sd_scc_START         (4)
#define SOC_PCTRL_PERI_CTRL103_bfmux_mem_ctrl_sd_scc_END           (9)
#define SOC_PCTRL_PERI_CTRL103_bfmux_mem_ctrl_sd_scd_START         (10)
#define SOC_PCTRL_PERI_CTRL103_bfmux_mem_ctrl_sd_scd_END           (15)
#define SOC_PCTRL_PERI_CTRL103_peri_ctrl103_msk_START              (16)
#define SOC_PCTRL_PERI_CTRL103_peri_ctrl103_msk_END                (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL105_UNION
 struct description   : PERI_CTRL105 Register structure definition
                        Address Offset:0x1828 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器105。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pasensor_cfg_valid : 1;  /* bit[0]    : valid上升沿锁存cmd和msg配置参数；
                                                              注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  pasensor_cmd       : 2;  /* bit[1-2]  : 2'b00：idle 模式
                                                              2'b01: 使能single read读数，并输出读数结果
                                                              2'b10: 输出Vmin读数
                                                              2'b11: 进入auto read模式
                                                              
                                                              注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  pasensor_en        : 1;  /* bit[3]    : 1：pasensor内部数字逻辑时钟使能
                                                              0：pasensor内部数字逻辑时钟关闭
                                                              注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  pasensor_ctrl      : 6;  /* bit[4-9]  : [9]: 1 表示meas使能，0表示meas不使能；
                                                              [8]：时钟选择，0选择pasensor_clk，1选择core_clk；
                                                              [7]: 1表示模拟stress pasensor clk门控使能，0表示不使能；
                                                              [6]: 1表示模拟stress core clk门控使能，0表示不使能；
                                                              [5]: 1表示使用DC老化模式；0表示n管和p管均处于关断模式；
                                                              [4]:选择stress模式，1表示选择clk类型作为stress；0表示选择bit5作为stress控制信号；
                                                              注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  reserved           : 6;  /* bit[10-15]: 保留 */
        unsigned int  peri_ctrl105_msk   : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                              与bit[15:0]命令位一一对应。
                                                              0：屏蔽命令位，命令位写操作无效；
                                                              1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL105_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL105_pasensor_cfg_valid_START  (0)
#define SOC_PCTRL_PERI_CTRL105_pasensor_cfg_valid_END    (0)
#define SOC_PCTRL_PERI_CTRL105_pasensor_cmd_START        (1)
#define SOC_PCTRL_PERI_CTRL105_pasensor_cmd_END          (2)
#define SOC_PCTRL_PERI_CTRL105_pasensor_en_START         (3)
#define SOC_PCTRL_PERI_CTRL105_pasensor_en_END           (3)
#define SOC_PCTRL_PERI_CTRL105_pasensor_ctrl_START       (4)
#define SOC_PCTRL_PERI_CTRL105_pasensor_ctrl_END         (9)
#define SOC_PCTRL_PERI_CTRL105_peri_ctrl105_msk_START    (16)
#define SOC_PCTRL_PERI_CTRL105_peri_ctrl105_msk_END      (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL106_UNION
 struct description   : PERI_CTRL106 Register structure definition
                        Address Offset:0x182C Initial:0x0000 Width:32
 register description : 通用外设控制寄存器106。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pasensor_msg     : 16; /* bit[0-15] : [2:0]自动读数模式连续读数次数；
                                                            [4:3]自动读数的最大周期（启动读数的间隔）；
                                                            [7:5]meas使能到启动第一次读数的latency；
                                                            [10:8]内部DIV分频比 （支持2~8的偶数分频，实际分频比为配置值加1）
                                                            [15:11] reserved
                                                            注意：只有在对应的mask位同时写1时，写操作才有效。 */
        unsigned int  peri_ctrl106_msk : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                            与bit[15:0]命令位一一对应。
                                                            0：屏蔽命令位，命令位写操作无效；
                                                            1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL106_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL106_pasensor_msg_START      (0)
#define SOC_PCTRL_PERI_CTRL106_pasensor_msg_END        (15)
#define SOC_PCTRL_PERI_CTRL106_peri_ctrl106_msk_START  (16)
#define SOC_PCTRL_PERI_CTRL106_peri_ctrl106_msk_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL107_UNION
 struct description   : PERI_CTRL107 Register structure definition
                        Address Offset:0x1830 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器107。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]    :  */
        unsigned int  reserved_1: 1;  /* bit[1]    :  */
        unsigned int  reserved_2: 1;  /* bit[2]    :  */
        unsigned int  reserved_3: 1;  /* bit[3]    :  */
        unsigned int  reserved_4: 1;  /* bit[4]    :  */
        unsigned int  reserved_5: 11; /* bit[5-15] : 保留。 */
        unsigned int  reserved_6: 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                     与bit[15:0]命令位一一对应。
                                                     0：屏蔽命令位，命令位写操作无效；
                                                     1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL107_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl176_wc_UNION
 struct description   : peri_ctrl176_wc Register structure definition
                        Address Offset:0x1834 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器176(QIC nopending WC回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_rd_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFGBUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl176_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl176_wc_wc_qic_rd_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_peri_ctrl176_wc_wc_qic_rd_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl177_wc_UNION
 struct description   : peri_ctrl177_wc Register structure definition
                        Address Offset:0x1838 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器177（QIC nopending WC回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_wr_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFGBUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl177_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl177_wc_wc_qic_wr_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_peri_ctrl177_wc_wc_qic_wr_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl178_wc_UNION
 struct description   : peri_ctrl178_wc Register structure definition
                        Address Offset:0x183C Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器178（QIC nopending WC回读）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_tb_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFGBUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl178_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl178_wc_wc_qic_tb_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_peri_ctrl178_wc_wc_qic_tb_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl179_wc_UNION
 struct description   : peri_ctrl179_wc Register structure definition
                        Address Offset:0x1840 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器179。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdms_nopending_cfg_dma_0 : 32; /* bit[0-31]: CFGBUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl179_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl179_wc_wc_qic_pdms_nopending_cfg_dma_0_START  (0)
#define SOC_PCTRL_peri_ctrl179_wc_wc_qic_pdms_nopending_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl180_wc_UNION
 struct description   : peri_ctrl180_wc Register structure definition
                        Address Offset:0x1844 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器180。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_rd_nopending_mdmperi_0 : 32; /* bit[0-31]: MODEM PERI BUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl180_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl180_wc_wc_qic_rd_nopending_mdmperi_0_START  (0)
#define SOC_PCTRL_peri_ctrl180_wc_wc_qic_rd_nopending_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl181_wc_UNION
 struct description   : peri_ctrl181_wc Register structure definition
                        Address Offset:0x1848 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器181。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_wr_nopending_mdmperi_0 : 32; /* bit[0-31]: MODEM PERI BUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl181_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl181_wc_wc_qic_wr_nopending_mdmperi_0_START  (0)
#define SOC_PCTRL_peri_ctrl181_wc_wc_qic_wr_nopending_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl182_wc_UNION
 struct description   : peri_ctrl182_wc Register structure definition
                        Address Offset:0x184C Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器182。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_tb_nopending_mdmperi_0 : 32; /* bit[0-31]: MODEM PERI BUS的nopending WC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl182_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl182_wc_wc_qic_tb_nopending_mdmperi_0_START  (0)
#define SOC_PCTRL_peri_ctrl182_wc_wc_qic_tb_nopending_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl183_wc_UNION
 struct description   : peri_ctrl183_wc Register structure definition
                        Address Offset:0x1850 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器183。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_rd_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的关键信号MWC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl183_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl183_wc_wc_qic_rd_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_peri_ctrl183_wc_wc_qic_rd_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl184_wc_UNION
 struct description   : peri_ctrl184_wc Register structure definition
                        Address Offset:0x1854 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器184。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_wr_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的关键信号MWC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl184_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl184_wc_wc_qic_wr_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_peri_ctrl184_wc_wc_qic_wr_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl185_wc_UNION
 struct description   : peri_ctrl185_wc Register structure definition
                        Address Offset:0x1858 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器185（QIC）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_tb_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的关键信号MWC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl185_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl185_wc_wc_qic_tb_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_peri_ctrl185_wc_wc_qic_tb_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl186_wc_UNION
 struct description   : peri_ctrl186_wc Register structure definition
                        Address Offset:0x185C Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器186（QIC）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdms_nopending_center_crossbar_0 : 32; /* bit[0-31]: QIC CC的关键信号MWC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl186_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl186_wc_wc_qic_pdms_nopending_center_crossbar_0_START  (0)
#define SOC_PCTRL_peri_ctrl186_wc_wc_qic_pdms_nopending_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl187_wc_UNION
 struct description   : peri_ctrl187_wc Register structure definition
                        Address Offset:0x1860 Initial:0xFFFFFFFF Width:32
 register description : 通用外设状态寄存器187（QIC）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdms_nopending_center_crossbar_1 : 32; /* bit[0-31]: QIC CC的关键信号MWC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl187_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl187_wc_wc_qic_pdms_nopending_center_crossbar_1_START  (0)
#define SOC_PCTRL_peri_ctrl187_wc_wc_qic_pdms_nopending_center_crossbar_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl188_wc_UNION
 struct description   : peri_ctrl188_wc Register structure definition
                        Address Offset:0x1864 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器188（QIC）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_sc_pmc_qic_power_idle_ack0 : 32; /* bit[0-31]: 低功耗信号MWC回读 */
    } reg;
} SOC_PCTRL_peri_ctrl188_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl188_wc_wc_sc_pmc_qic_power_idle_ack0_START  (0)
#define SOC_PCTRL_peri_ctrl188_wc_wc_sc_pmc_qic_power_idle_ack0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl189_wc_UNION
 struct description   : peri_ctrl189_wc Register structure definition
                        Address Offset:0x1868 Initial:0x0000 Width:32
 register description : 通用外设状态寄存器189（QIC）。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_sc_pmc_qic_power_idle_req0 : 16; /* bit[0-15] : 低功耗信号MWC回读 */
        unsigned int  reserved                      : 16; /* bit[16-31]:  */
    } reg;
} SOC_PCTRL_peri_ctrl189_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl189_wc_wc_sc_pmc_qic_power_idle_req0_START  (0)
#define SOC_PCTRL_peri_ctrl189_wc_wc_sc_pmc_qic_power_idle_req0_END    (15)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl190_qic_pwrd_UNION
 struct description   : peri_ctrl190_qic_pwrd Register structure definition
                        Address Offset:0x186C Initial:0x0000 Width:32
 register description : 通用外设状态寄存器190-qic_pwrd。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0        : 1;  /* bit[0]    : 总线POWERDOMIAN 握手请求信号 */
        unsigned int  reserved_1        : 1;  /* bit[1]    :  */
        unsigned int  reserved_2        : 1;  /* bit[2]    :  */
        unsigned int  reserved_3        : 1;  /* bit[3]    :  */
        unsigned int  reserved_4        : 1;  /* bit[4]    :  */
        unsigned int  reserved_5        : 1;  /* bit[5]    :  */
        unsigned int  reserved_6        : 1;  /* bit[6]    :  */
        unsigned int  reserved_7        : 1;  /* bit[7]    :  */
        unsigned int  reserved_8        : 1;  /* bit[8]    :  */
        unsigned int  reserved_9        : 1;  /* bit[9]    :  */
        unsigned int  reserved_10       : 1;  /* bit[10]   :  */
        unsigned int  reserved_11       : 1;  /* bit[11]   :  */
        unsigned int  reserved_12       : 1;  /* bit[12]   :  */
        unsigned int  reserved_13       : 3;  /* bit[13-15]:  */
        unsigned int  peri_ctrl190_mask : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                             与bit[15:0]命令位一一对应。
                                                             0：屏蔽命令位，命令位写操作无效；
                                                             1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_peri_ctrl190_qic_pwrd_UNION;
#endif
#define SOC_PCTRL_peri_ctrl190_qic_pwrd_peri_ctrl190_mask_START  (16)
#define SOC_PCTRL_peri_ctrl190_qic_pwrd_peri_ctrl190_mask_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl191_mtcmos_UNION
 struct description   : peri_ctrl191_mtcmos Register structure definition
                        Address Offset:0x1870 Initial:0x0000 Width:32
 register description : 通用外设状态寄存器191-mtcmos。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  qice_gpu_mtcmos_en : 1;  /* bit[0]    : QICE的mtcmos信号，默认为1 */
        unsigned int  reserved_0         : 1;  /* bit[1]    :  */
        unsigned int  reserved_1         : 1;  /* bit[2]    :  */
        unsigned int  reserved_2         : 13; /* bit[3-15] :  */
        unsigned int  peri_ctrl191_mask  : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                              与bit[15:0]命令位一一对应。
                                                              0：屏蔽命令位，命令位写操作无效；
                                                              1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_peri_ctrl191_mtcmos_UNION;
#endif
#define SOC_PCTRL_peri_ctrl191_mtcmos_qice_gpu_mtcmos_en_START  (0)
#define SOC_PCTRL_peri_ctrl191_mtcmos_qice_gpu_mtcmos_en_END    (0)
#define SOC_PCTRL_peri_ctrl191_mtcmos_peri_ctrl191_mask_START   (16)
#define SOC_PCTRL_peri_ctrl191_mtcmos_peri_ctrl191_mask_END     (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl192_wc_UNION
 struct description   : peri_ctrl192_wc Register structure definition
                        Address Offset:0x1874 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器192。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_noc_stat0_topctrl : 32; /* bit[0-31]: noc_stat0_topctrl维测用寄存器
                                                               31 st_clk_ddrsys_noc
                                                               30 st_clk_ddrsys_ao
                                                               29 sc_gt_clk_ddrc_sw
                                                               28:26 sw_ack_clk_ddrc_sw
                                                               25:24 sw_ack_clk_ddrc_fll_sw
                                                               23:20 div_ddrc_fll
                                                               19 sc_gt_clk_ddrc_fll
                                                               18:17 sw_ack_clk_ddrc_sys_sw
                                                               16:15 div_clk_sys_ddr
                                                               14 sc_gt_clk_ddrc_sys
                                                               13:9 div_ddrc_pll
                                                               8 sc_gt_clk_ddrc_pll
                                                               7:4 sw_ack_clk_ddrc_pll_sw
                                                               3 st_clk_hsdtbus
                                                               2:0 reserved */
    } reg;
} SOC_PCTRL_peri_ctrl192_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl192_wc_wc_noc_stat0_topctrl_START  (0)
#define SOC_PCTRL_peri_ctrl192_wc_wc_noc_stat0_topctrl_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl193_wc_UNION
 struct description   : peri_ctrl193_wc Register structure definition
                        Address Offset:0x1878 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器193。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_noc_stat1_topctrl : 32; /* bit[0-31]: noc_stat1_topctrl维测用寄存器
                                                               31 st_clk_sysbus
                                                               30:28 sw_ack_clk_sysbus_sw
                                                               27:26 sw_ack_clk_sysbus_fll_sw
                                                               25 sc_gt_clk_sysbus_fll
                                                               24:23 div_clk_sys_sysbus
                                                               22 sc_gt_clk_sysbus_sys
                                                               21:17 div_sysbus_pll
                                                               16:11 sysbus_div_auto_cfg
                                                               10 sc_gt_clk_sysbus_pll
                                                               9 st_clk_noc_mdm_cfg_peri
                                                               8 st_clk_sysbus_l
                                                               7 st_clk_cfgbus
                                                               6 st_clk_lpmcu2cfgbus
                                                               5 st_clk_dma2sysbus
                                                               4 st_clk_sysbus_r
                                                               3 st_clk_dmabus
                                                               2 sc_gt_clk_dmabus
                                                               1 st_clk_noc_spe_asyncbrg
                                                               0 st_clk_hsdt1bus */
    } reg;
} SOC_PCTRL_peri_ctrl193_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl193_wc_wc_noc_stat1_topctrl_START  (0)
#define SOC_PCTRL_peri_ctrl193_wc_wc_noc_stat1_topctrl_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_peri_ctrl194_wc_UNION
 struct description   : peri_ctrl194_wc Register structure definition
                        Address Offset:0x187C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器194。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_noc_stat2_topctrl : 32; /* bit[0-31]: noc_stat2_topctrl维测用寄存器
                                                               31 sc_gt_clk_hsdt1bus
                                                               30 sw_ack_clk_hsdt1bus_sw
                                                               29 st_clk_hsdt1_subsys
                                                               28 st_clk_sysbus2hsdt1
                                                               27 st_clk_cfgbus_media2
                                                               26 st_clk_cfgbus_media
                                                               25 st_clk_sysbus2npu
                                                               24 st_clk_noc_npubus_cfg
                                                               23 st_clk_noc_npubus
                                                               22 st_clk_fcm_nic_m
                                                               21 st_clk_cpu_gic
                                                               20 st_clk_cpu_pctrl
                                                               19 st_clk_gpu_noc
                                                               18 ip_rst_noc_dmabus
                                                               17 ip_rst_cfgbus
                                                               16 ip_rst_noc_hsdt1bus
                                                               15 ip_rst_sysbus
                                                               14 ip_rst_media2_qic_cfg_peri
                                                               13 ip_rst_media1_qic_cfg_peri
                                                               12 ip_rst_npu_qic_cfg_peri
                                                               11 ip_rst_cpu_gic
                                                               10 ip_rst_gpu_noc
                                                               9 ip_rst_gpu_crg
                                                               8 ip_rst_npu_subsys
                                                               7 st_clk_noc_cfg2fcm
                                                               7:0 reserved */
    } reg;
} SOC_PCTRL_peri_ctrl194_wc_UNION;
#endif
#define SOC_PCTRL_peri_ctrl194_wc_wc_noc_stat2_topctrl_START  (0)
#define SOC_PCTRL_peri_ctrl194_wc_wc_noc_stat2_topctrl_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl0_wc_UNION
 struct description   : R2_peri_ctrl0_wc Register structure definition
                        Address Offset:0x1880 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_intr_pktc_cfg_dma_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl0_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl0_wc_wc_intr_pktc_cfg_dma_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl0_wc_wc_intr_pktc_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl1_wc_UNION
 struct description   : R2_peri_ctrl1_wc Register structure definition
                        Address Offset:0x1884 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_intr_em_cfg_dma_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl1_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl1_wc_wc_intr_em_cfg_dma_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl1_wc_wc_intr_em_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl2_wc_UNION
 struct description   : R2_peri_ctrl2_wc Register structure definition
                        Address Offset:0x1888 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_intr_em_mdmperi_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl2_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl2_wc_wc_intr_em_mdmperi_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl2_wc_wc_intr_em_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl3_wc_UNION
 struct description   : R2_peri_ctrl3_wc Register structure definition
                        Address Offset:0x188C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_intr_pktc_center_crossbar_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl3_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl3_wc_wc_intr_pktc_center_crossbar_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl3_wc_wc_intr_pktc_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl4_wc_UNION
 struct description   : R2_peri_ctrl4_wc Register structure definition
                        Address Offset:0x1890 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_intr_em_center_crossbar_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl4_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl4_wc_wc_intr_em_center_crossbar_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl4_wc_wc_intr_em_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl5_wc_UNION
 struct description   : R2_peri_ctrl5_wc Register structure definition
                        Address Offset:0x1894 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_timeout_st_cfg_dma_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl5_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl5_wc_wc_qic_timeout_st_cfg_dma_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl5_wc_wc_qic_timeout_st_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl6_wc_UNION
 struct description   : R2_peri_ctrl6_wc Register structure definition
                        Address Offset:0x1898 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_timeout_st_mdmperi_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl6_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl6_wc_wc_qic_timeout_st_mdmperi_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl6_wc_wc_qic_timeout_st_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl7_wc_UNION
 struct description   : R2_peri_ctrl7_wc Register structure definition
                        Address Offset:0x189C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_timeout_st_center_crossbar_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl7_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl7_wc_wc_qic_timeout_st_center_crossbar_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl7_wc_wc_qic_timeout_st_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl8_wc_UNION
 struct description   : R2_peri_ctrl8_wc Register structure definition
                        Address Offset:0x18A0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_cfg_dma_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl8_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl8_wc_wc_qic_pdm_state_pdm_cfg_dma_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl8_wc_wc_qic_pdm_state_pdm_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl9_wc_UNION
 struct description   : R2_peri_ctrl9_wc Register structure definition
                        Address Offset:0x18A4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl9_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl9_wc_wc_qic_pdm_state_pdm_center_crossbar_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl9_wc_wc_qic_pdm_state_pdm_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl10_wc_UNION
 struct description   : R2_peri_ctrl10_wc Register structure definition
                        Address Offset:0x18A8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_1 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl10_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl10_wc_wc_qic_pdm_state_pdm_center_crossbar_1_START  (0)
#define SOC_PCTRL_R2_peri_ctrl10_wc_wc_qic_pdm_state_pdm_center_crossbar_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl11_wc_UNION
 struct description   : R2_peri_ctrl11_wc Register structure definition
                        Address Offset:0x18AC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_2 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl11_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl11_wc_wc_qic_pdm_state_pdm_center_crossbar_2_START  (0)
#define SOC_PCTRL_R2_peri_ctrl11_wc_wc_qic_pdm_state_pdm_center_crossbar_2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl12_wc_UNION
 struct description   : R2_peri_ctrl12_wc Register structure definition
                        Address Offset:0x18B0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_3 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl12_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl12_wc_wc_qic_pdm_state_pdm_center_crossbar_3_START  (0)
#define SOC_PCTRL_R2_peri_ctrl12_wc_wc_qic_pdm_state_pdm_center_crossbar_3_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl13_wc_UNION
 struct description   : R2_peri_ctrl13_wc Register structure definition
                        Address Offset:0x18B4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_4 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl13_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl13_wc_wc_qic_pdm_state_pdm_center_crossbar_4_START  (0)
#define SOC_PCTRL_R2_peri_ctrl13_wc_wc_qic_pdm_state_pdm_center_crossbar_4_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl14_wc_UNION
 struct description   : R2_peri_ctrl14_wc Register structure definition
                        Address Offset:0x18B8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_5 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl14_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl14_wc_wc_qic_pdm_state_pdm_center_crossbar_5_START  (0)
#define SOC_PCTRL_R2_peri_ctrl14_wc_wc_qic_pdm_state_pdm_center_crossbar_5_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl15_wc_UNION
 struct description   : R2_peri_ctrl15_wc Register structure definition
                        Address Offset:0x18BC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pdm_state_pdm_center_crossbar_6 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl15_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl15_wc_wc_qic_pdm_state_pdm_center_crossbar_6_START  (0)
#define SOC_PCTRL_R2_peri_ctrl15_wc_wc_qic_pdm_state_pdm_center_crossbar_6_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl16_wc_UNION
 struct description   : R2_peri_ctrl16_wc Register structure definition
                        Address Offset:0x18C0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pds_state_pds_cfg_dma_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl16_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl16_wc_wc_qic_pds_state_pds_cfg_dma_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl16_wc_wc_qic_pds_state_pds_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl17_wc_UNION
 struct description   : R2_peri_ctrl17_wc Register structure definition
                        Address Offset:0x18C4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pds_state_pds_center_crossbar_0 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl17_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl17_wc_wc_qic_pds_state_pds_center_crossbar_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl17_wc_wc_qic_pds_state_pds_center_crossbar_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl18_wc_UNION
 struct description   : R2_peri_ctrl18_wc Register structure definition
                        Address Offset:0x18C8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pds_state_pds_center_crossbar_1 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl18_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl18_wc_wc_qic_pds_state_pds_center_crossbar_1_START  (0)
#define SOC_PCTRL_R2_peri_ctrl18_wc_wc_qic_pds_state_pds_center_crossbar_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl19_wc_UNION
 struct description   : R2_peri_ctrl19_wc Register structure definition
                        Address Offset:0x18CC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pds_state_pds_center_crossbar_2 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl19_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl19_wc_wc_qic_pds_state_pds_center_crossbar_2_START  (0)
#define SOC_PCTRL_R2_peri_ctrl19_wc_wc_qic_pds_state_pds_center_crossbar_2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl20_wc_UNION
 struct description   : R2_peri_ctrl20_wc Register structure definition
                        Address Offset:0x18D0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_qic_pds_state_pds_center_crossbar_3 : 32; /* bit[0-31]: QIC INTR中断的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl20_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl20_wc_wc_qic_pds_state_pds_center_crossbar_3_START  (0)
#define SOC_PCTRL_R2_peri_ctrl20_wc_wc_qic_pds_state_pds_center_crossbar_3_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl21_wc_UNION
 struct description   : R2_peri_ctrl21_wc Register structure definition
                        Address Offset:0x18D4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC intr组合逻辑后锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_intr_qic_probe_cfg_dma   : 1;  /* bit[0]   : cfg dma qic所有中断相或后的中断的wc锁存 */
        unsigned int  wc_intr_qic_probe_crossbar  : 1;  /* bit[1]   : sysbus qic所有中断相或后的中断的wc锁存 */
        unsigned int  wc_intr_qic_probe_modemperi : 1;  /* bit[2]   : modemperi qic所有中断相或后的中断的wc锁存 */
        unsigned int  reserved                    : 29; /* bit[3-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl21_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl21_wc_wc_intr_qic_probe_cfg_dma_START    (0)
#define SOC_PCTRL_R2_peri_ctrl21_wc_wc_intr_qic_probe_cfg_dma_END      (0)
#define SOC_PCTRL_R2_peri_ctrl21_wc_wc_intr_qic_probe_crossbar_START   (1)
#define SOC_PCTRL_R2_peri_ctrl21_wc_wc_intr_qic_probe_crossbar_END     (1)
#define SOC_PCTRL_R2_peri_ctrl21_wc_wc_intr_qic_probe_modemperi_START  (2)
#define SOC_PCTRL_R2_peri_ctrl21_wc_wc_intr_qic_probe_modemperi_END    (2)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl22_wc_UNION
 struct description   : R2_peri_ctrl22_wc Register structure definition
                        Address Offset:0x18D8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_dfx_busy_ca_cfg_dma_0 : 32; /* bit[0-31]: QIC busy的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl22_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl22_wc_wc_dfx_busy_ca_cfg_dma_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl22_wc_wc_dfx_busy_ca_cfg_dma_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl23_wc_UNION
 struct description   : R2_peri_ctrl23_wc Register structure definition
                        Address Offset:0x18DC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  wc_dfx_busy_ca_center_mdmperi_0 : 32; /* bit[0-31]: QIC busy的MWC锁存，维测提升方案 */
    } reg;
} SOC_PCTRL_R2_peri_ctrl23_wc_UNION;
#endif
#define SOC_PCTRL_R2_peri_ctrl23_wc_wc_dfx_busy_ca_center_mdmperi_0_START  (0)
#define SOC_PCTRL_R2_peri_ctrl23_wc_wc_dfx_busy_ca_center_mdmperi_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl24_wc_UNION
 struct description   : R2_peri_ctrl24_wc Register structure definition
                        Address Offset:0x18E0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl24_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl25_wc_UNION
 struct description   : R2_peri_ctrl25_wc Register structure definition
                        Address Offset:0x18E4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl25_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl26_wc_UNION
 struct description   : R2_peri_ctrl26_wc Register structure definition
                        Address Offset:0x18E8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl26_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl27_wc_UNION
 struct description   : R2_peri_ctrl27_wc Register structure definition
                        Address Offset:0x18EC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl27_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl28_wc_UNION
 struct description   : R2_peri_ctrl28_wc Register structure definition
                        Address Offset:0x18F0 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl28_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl29_wc_UNION
 struct description   : R2_peri_ctrl29_wc Register structure definition
                        Address Offset:0x18F4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl29_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl30_wc_UNION
 struct description   : R2_peri_ctrl30_wc Register structure definition
                        Address Offset:0x18F8 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl30_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl31_wc_UNION
 struct description   : R2_peri_ctrl31_wc Register structure definition
                        Address Offset:0x18FC Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl31_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl32_wc_UNION
 struct description   : R2_peri_ctrl32_wc Register structure definition
                        Address Offset:0x1900 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器-QIC   busy MWC锁存
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl32_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl33_wc_UNION
 struct description   : R2_peri_ctrl33_wc Register structure definition
                        Address Offset:0x1904 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl33_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl34_wc_UNION
 struct description   : R2_peri_ctrl34_wc Register structure definition
                        Address Offset:0x1908 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl34_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl35_wc_UNION
 struct description   : R2_peri_ctrl35_wc Register structure definition
                        Address Offset:0x190C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl35_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl36_wc_UNION
 struct description   : R2_peri_ctrl36_wc Register structure definition
                        Address Offset:0x1910 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl36_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl37_wc_UNION
 struct description   : R2_peri_ctrl37_wc Register structure definition
                        Address Offset:0x1914 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl37_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl38_wc_UNION
 struct description   : R2_peri_ctrl38_wc Register structure definition
                        Address Offset:0x1918 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl38_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl39_wc_UNION
 struct description   : R2_peri_ctrl39_wc Register structure definition
                        Address Offset:0x191C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl39_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl40_wc_UNION
 struct description   : R2_peri_ctrl40_wc Register structure definition
                        Address Offset:0x1920 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl40_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl41_wc_UNION
 struct description   : R2_peri_ctrl41_wc Register structure definition
                        Address Offset:0x1924 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl41_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl42_wc_UNION
 struct description   : R2_peri_ctrl42_wc Register structure definition
                        Address Offset:0x1928 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl42_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl43_wc_UNION
 struct description   : R2_peri_ctrl43_wc Register structure definition
                        Address Offset:0x192C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl43_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl44_wc_UNION
 struct description   : R2_peri_ctrl44_wc Register structure definition
                        Address Offset:0x1930 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl44_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl45_wc_UNION
 struct description   : R2_peri_ctrl45_wc Register structure definition
                        Address Offset:0x1934 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl45_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl46_wc_UNION
 struct description   : R2_peri_ctrl46_wc Register structure definition
                        Address Offset:0x1938 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl46_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl47_wc_UNION
 struct description   : R2_peri_ctrl47_wc Register structure definition
                        Address Offset:0x193C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl47_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl48_wc_UNION
 struct description   : R2_peri_ctrl48_wc Register structure definition
                        Address Offset:0x1940 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl48_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl49_wc_UNION
 struct description   : R2_peri_ctrl49_wc Register structure definition
                        Address Offset:0x1944 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl49_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl50_wc_UNION
 struct description   : R2_peri_ctrl50_wc Register structure definition
                        Address Offset:0x1948 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl50_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl51_wc_UNION
 struct description   : R2_peri_ctrl51_wc Register structure definition
                        Address Offset:0x194C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl51_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl52_wc_UNION
 struct description   : R2_peri_ctrl52_wc Register structure definition
                        Address Offset:0x1950 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl52_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl53_wc_UNION
 struct description   : R2_peri_ctrl53_wc Register structure definition
                        Address Offset:0x1954 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl53_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl54_wc_UNION
 struct description   : R2_peri_ctrl54_wc Register structure definition
                        Address Offset:0x1958 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl54_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl55_wc_UNION
 struct description   : R2_peri_ctrl55_wc Register structure definition
                        Address Offset:0x195C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl55_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl56_wc_UNION
 struct description   : R2_peri_ctrl56_wc Register structure definition
                        Address Offset:0x1960 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl56_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl57_wc_UNION
 struct description   : R2_peri_ctrl57_wc Register structure definition
                        Address Offset:0x1964 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl57_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl58_wc_UNION
 struct description   : R2_peri_ctrl58_wc Register structure definition
                        Address Offset:0x1968 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl58_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_R2_peri_ctrl59_wc_UNION
 struct description   : R2_peri_ctrl59_wc Register structure definition
                        Address Offset:0x196C Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_R2_peri_ctrl59_wc_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL5_UNION
 struct description   : PERI_CTRL5 Register structure definition
                        Address Offset:0x1C00 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器5_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  soft_lock_dis : 1;  /* bit[0]   : djtag_router的disable拦截信号。
                                                        本SC_LOCK_REG类型寄存器被锁定（置1）后，除非复位，无法解锁。 */
        unsigned int  reserved      : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL5_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL5_soft_lock_dis_START  (0)
#define SOC_PCTRL_PERI_CTRL5_soft_lock_dis_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL6_UNION
 struct description   : PERI_CTRL6 Register structure definition
                        Address Offset:0x1C04 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器6_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ckg_en_ddrxctrl_h2p     : 1;  /* bit[0]    : ddrxctrl的h2p内部自动时钟门控 */
        unsigned int  ckg_en_ddrxctrl_minibus : 1;  /* bit[1]    : ddrxctrl的minibus内部自动时钟门控 */
        unsigned int  reserved                : 14; /* bit[2-15] :  */
        unsigned int  peri_ctrl6_msk          : 16; /* bit[16-31]: 通用外设控制mask掩码。
                                                                   与bit[15:0]命令位一一对应。
                                                                   0：屏蔽命令位，命令位写操作无效；
                                                                   1：使能命令位，命令位写操作有效。 */
    } reg;
} SOC_PCTRL_PERI_CTRL6_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL6_ckg_en_ddrxctrl_h2p_START      (0)
#define SOC_PCTRL_PERI_CTRL6_ckg_en_ddrxctrl_h2p_END        (0)
#define SOC_PCTRL_PERI_CTRL6_ckg_en_ddrxctrl_minibus_START  (1)
#define SOC_PCTRL_PERI_CTRL6_ckg_en_ddrxctrl_minibus_END    (1)
#define SOC_PCTRL_PERI_CTRL6_peri_ctrl6_msk_START           (16)
#define SOC_PCTRL_PERI_CTRL6_peri_ctrl6_msk_END             (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL7_UNION
 struct description   : PERI_CTRL7 Register structure definition
                        Address Offset:0x1C08 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器7_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ipc_gt_clk_bypass : 1;  /* bit[0]   : IPC内部自动门控bypass控制，为1时钟常开
                                                            安全属性 */
        unsigned int  reserved          : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL7_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL7_ipc_gt_clk_bypass_START  (0)
#define SOC_PCTRL_PERI_CTRL7_ipc_gt_clk_bypass_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL8_UNION
 struct description   : PERI_CTRL8 Register structure definition
                        Address Offset:0x1C0C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器8_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  counter_unify_en : 1;  /* bit[0]   : 0：维测使用原始tsgen产生计数值；
                                                           1：维测使用芯片归一化syscounter计数值 */
        unsigned int  reserved         : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL8_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL8_counter_unify_en_START  (0)
#define SOC_PCTRL_PERI_CTRL8_counter_unify_en_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL9_UNION
 struct description   : PERI_CTRL9 Register structure definition
                        Address Offset:0x1C10 Initial:0x0000000 Width:32
 register description : 通用外设控制寄存器9_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 1;  /* bit[1]   :  */
        unsigned int  reserved_2: 1;  /* bit[2]   :  */
        unsigned int  reserved_3: 1;  /* bit[3]   :  */
        unsigned int  reserved_4: 28; /* bit[4-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL9_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL10_UNION
 struct description   : PERI_CTRL10 Register structure definition
                        Address Offset:0x1C14 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器10_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL10_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL11_UNION
 struct description   : PERI_CTRL11 Register structure definition
                        Address Offset:0x1C18 Initial:0x0000 Width:32
 register description : 通用外设控制寄存器11。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pdc_pwrds_req_sys2mdm  : 1;  /* bit[0]    : sys2mdm pwrd pds 请求
                                                                  低功耗请求信号
                                                                  0：退出低功耗请求
                                                                  1：进入低功耗请求，复位态 */
        unsigned int  pdc_pwrds_req_mdm2sys  : 1;  /* bit[1]    : mdm2sys pwrd pds 请求 */
        unsigned int  pdc_pwrdm_req_sys2mdm  : 1;  /* bit[2]    : sys2mdm pwrd pdm 请求 */
        unsigned int  pdc_pwrdm_req_mdm2sys  : 1;  /* bit[3]    : mdm2sys pwrd pdm 请求 */
        unsigned int  pdc_pwrds_req_mdm2ddr  : 1;  /* bit[4]    : mdm2ddr pwrd pds 请求 */
        unsigned int  pdc_pwrds_req_pdea2ddr : 1;  /* bit[5]    : pdea2ddr pwrd pds 请求 */
        unsigned int  pdc_pwrdm_req_mdm2ddr  : 1;  /* bit[6]    : mdm2ddr pwrd pdm 请求 */
        unsigned int  pdc_pwrdm_req_pdea2ddr : 1;  /* bit[7]    : pdea2sys pwrd pdm 请求 */
        unsigned int  reserved               : 8;  /* bit[8-15] :  */
        unsigned int  peri_ctrl11_msk        : 16; /* bit[16-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL11_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_sys2mdm_START   (0)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_sys2mdm_END     (0)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_mdm2sys_START   (1)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_mdm2sys_END     (1)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_sys2mdm_START   (2)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_sys2mdm_END     (2)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_mdm2sys_START   (3)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_mdm2sys_END     (3)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_mdm2ddr_START   (4)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_mdm2ddr_END     (4)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_pdea2ddr_START  (5)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrds_req_pdea2ddr_END    (5)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_mdm2ddr_START   (6)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_mdm2ddr_END     (6)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_pdea2ddr_START  (7)
#define SOC_PCTRL_PERI_CTRL11_pdc_pwrdm_req_pdea2ddr_END    (7)
#define SOC_PCTRL_PERI_CTRL11_peri_ctrl11_msk_START         (16)
#define SOC_PCTRL_PERI_CTRL11_peri_ctrl11_msk_END           (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL34_UNION
 struct description   : PERI_CTRL34 Register structure definition
                        Address Offset:0x1C1C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器34_安全区。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  hard_volt_low  : 1;  /* bit[0]   : 默认态为0，代表常压；
                                                         实际使用场景外设区降压，软件M3可以配置该寄存器，配置为1则表示降压。
                                                         与hard_vold_high共同使用：
                                                         2'b01：低压
                                                         2'b00：常压
                                                         2'b11:和2'b10：高压
                                                         
                                                         
                                                         给MEDIA用 */
        unsigned int  hard_volt_high : 1;  /* bit[1]   : 默认态为0，代表常压；
                                                         实际使用场景外设区降压，软件M3可以配置该寄存器，配置为1则表示高压。
                                                         与hard_vold_low共同使用：
                                                         2‘b01：低压
                                                         2'b00：常压
                                                         2'b11:和2'b10：高压
                                                         
                                                         给MEDIA用 */
        unsigned int  reserved       : 30; /* bit[2-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL34_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL34_hard_volt_low_START   (0)
#define SOC_PCTRL_PERI_CTRL34_hard_volt_low_END     (0)
#define SOC_PCTRL_PERI_CTRL34_hard_volt_high_START  (1)
#define SOC_PCTRL_PERI_CTRL34_hard_volt_high_END    (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL35_UNION
 struct description   : PERI_CTRL35 Register structure definition
                        Address Offset:0x1C20 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器35。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL35_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL38_UNION
 struct description   : PERI_CTRL38 Register structure definition
                        Address Offset:0x1C2C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器38。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL38_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL39_UNION
 struct description   : PERI_CTRL39 Register structure definition
                        Address Offset:0x1C30 Initial:0x0 Width:32
 register description : 通用外设控制寄存器39。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  uid_check_bypass : 1;  /* bit[0]    : HTS uid 寄存器开关，1bit clok reg，本SC_LOCK_REG类型寄存器被锁定（置1）后，除非复位，无法解锁。 */
        unsigned int  reserved_0       : 5;  /* bit[1-5]  : 保留 */
        unsigned int  reserved_1       : 6;  /* bit[6-11] : 保留 */
        unsigned int  reserved_2       : 6;  /* bit[12-17]: 保留 */
        unsigned int  reserved_3       : 6;  /* bit[18-23]: 保留 */
        unsigned int  reserved_4       : 6;  /* bit[24-29]: 保留。 */
        unsigned int  reserved_5       : 2;  /* bit[30-31]: 保留。 */
    } reg;
} SOC_PCTRL_PERI_CTRL39_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL39_uid_check_bypass_START  (0)
#define SOC_PCTRL_PERI_CTRL39_uid_check_bypass_END    (0)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL40_UNION
 struct description   : PERI_CTRL40 Register structure definition
                        Address Offset:0x1C34 Initial:0x000000 Width:32
 register description : 通用外设控制寄存器40。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 4;  /* bit[0-3] :  */
        unsigned int  reserved_1: 4;  /* bit[4-7] :  */
        unsigned int  reserved_2: 24; /* bit[8-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL40_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL41_UNION
 struct description   : PERI_CTRL41 Register structure definition
                        Address Offset:0x1C38 Initial:0x0000000 Width:32
 register description : 通用外设控制寄存器41。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  psw_test_mux   : 4;  /* bit[0-3] : PSW 模拟testmux为8路模拟输入选1路输出的选择和使能信号：
                                                         bit[3]： 使能控制。
                                                         bit[2:0]：用于从8路输入中选通1路输入。
                                                          其中取3'd0~7分别对应psw_test_in0 ~ psw_test_in7 */
        unsigned int  ocldo_test_mux : 3;  /* bit[4-6] : OCLDO 模拟testmux为4路模拟输入选1路输出。
                                                         bit[6]： 为使能控制
                                                         bit[5:4]：用于从4路输入中选通1路输入
                                                          其中4'd0~3分别对应ocldo_test_in0 ~ ocldo_test_in3 */
        unsigned int  reserved       : 25; /* bit[7-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL41_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL41_psw_test_mux_START    (0)
#define SOC_PCTRL_PERI_CTRL41_psw_test_mux_END      (3)
#define SOC_PCTRL_PERI_CTRL41_ocldo_test_mux_START  (4)
#define SOC_PCTRL_PERI_CTRL41_ocldo_test_mux_END    (6)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL42_UNION
 struct description   : PERI_CTRL42 Register structure definition
                        Address Offset:0x1C3C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器42_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL42_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL43_UNION
 struct description   : PERI_CTRL43 Register structure definition
                        Address Offset:0x1C40 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器43_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL43_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL44_UNION
 struct description   : PERI_CTRL44 Register structure definition
                        Address Offset:0x1C44 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器44。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   : modem访问8段AP寄存器空间安全访问控制start起始地址,底软只有安全世界可配，使能配置lock寄存器的bit，则寄存器锁定，不能再配 */
        unsigned int  reserved_1: 31; /* bit[1-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL44_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL45_UNION
 struct description   : PERI_CTRL45 Register structure definition
                        Address Offset:0x1C48 Initial:0x00 Width:32
 register description : 通用外设控制寄存器45_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] : modem访问8段AP寄存器空间安全访问控制start起始地址,底软只有安全世界可配，使能配置lock寄存器的bit，则寄存器锁定，不能再配 */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL45_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL46_UNION
 struct description   : PERI_CTRL46 Register structure definition
                        Address Offset:0x1C4C Initial:0x0000000 Width:32
 register description : 通用外设控制寄存器46_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddr_slp_req_external_a : 1;  /* bit[0]   : DDR SR PLLDIS流程开始PLL时钟指示 */
        unsigned int  ddr_slp_req_external_b : 1;  /* bit[1]   : DDR SR PLLDIS流程开始PLL时钟指示 */
        unsigned int  ddr_slp_req_external_c : 1;  /* bit[2]   : DDR SR PLLDIS流程开始PLL时钟指示 */
        unsigned int  ddr_slp_req_external_d : 1;  /* bit[3]   : DDR SR PLLDIS流程开始PLL时钟指示 */
        unsigned int  reserved               : 28; /* bit[4-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL46_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_a_START  (0)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_a_END    (0)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_b_START  (1)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_b_END    (1)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_c_START  (2)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_c_END    (2)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_d_START  (3)
#define SOC_PCTRL_PERI_CTRL46_ddr_slp_req_external_d_END    (3)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL47_UNION
 struct description   : PERI_CTRL47 Register structure definition
                        Address Offset:0x1C50 Initial:0x00 Width:32
 register description : 通用外设控制寄存器47_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL47_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL48_UNION
 struct description   : PERI_CTRL48 Register structure definition
                        Address Offset:0x1C54 Initial:0x00 Width:32
 register description : 通用外设控制寄存器48_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL48_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL49_UNION
 struct description   : PERI_CTRL49 Register structure definition
                        Address Offset:0x1C58 Initial:0x00 Width:32
 register description : 通用外设控制寄存器49。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL49_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL50_UNION
 struct description   : PERI_CTRL50 Register structure definition
                        Address Offset:0x1C5C Initial:0x00 Width:32
 register description : 通用外设控制寄存器50。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL50_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL51_UNION
 struct description   : PERI_CTRL51 Register structure definition
                        Address Offset:0x1C60 Initial:0x00 Width:32
 register description : 通用外设控制寄存器51。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL51_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL52_UNION
 struct description   : PERI_CTRL52 Register structure definition
                        Address Offset:0x1C64 Initial:0x00 Width:32
 register description : 通用外设控制寄存器52。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL52_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL53_UNION
 struct description   : PERI_CTRL53 Register structure definition
                        Address Offset:0x1C68 Initial:0x00 Width:32
 register description : 通用外设控制寄存器53。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL53_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL54_UNION
 struct description   : PERI_CTRL54 Register structure definition
                        Address Offset:0x1C6C Initial:0x00 Width:32
 register description : 通用外设控制寄存器54。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL54_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL55_UNION
 struct description   : PERI_CTRL55 Register structure definition
                        Address Offset:0x1C70 Initial:0x00 Width:32
 register description : 通用外设控制寄存器55。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL55_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL56_UNION
 struct description   : PERI_CTRL56 Register structure definition
                        Address Offset:0x1C74 Initial:0x00 Width:32
 register description : 通用外设控制寄存器56。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL56_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL57_UNION
 struct description   : PERI_CTRL57 Register structure definition
                        Address Offset:0x1C78 Initial:0x00 Width:32
 register description : 通用外设控制寄存器57。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL57_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL58_UNION
 struct description   : PERI_CTRL58 Register structure definition
                        Address Offset:0x1C7C Initial:0x00 Width:32
 register description : 通用外设控制寄存器58。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL58_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL59_UNION
 struct description   : PERI_CTRL59 Register structure definition
                        Address Offset:0x1C80 Initial:0x00 Width:32
 register description : 通用外设控制寄存器59。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL59_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL60_UNION
 struct description   : PERI_CTRL60 Register structure definition
                        Address Offset:0x1C84 Initial:0x00 Width:32
 register description : 通用外设控制寄存器60。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 24; /* bit[0-23] :  */
        unsigned int  reserved_1: 8;  /* bit[24-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL60_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL61_UNION
 struct description   : PERI_CTRL61 Register structure definition
                        Address Offset:0x1C88 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器61。_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  lock : 32; /* bit[0-31]: 锁存寄存器，表示该bit对应的位被锁定。
                                               释义：
                                               该bit为0：未被锁定。
                                               该bit为1：被锁定，对应的安全配置寄存器不能被写，也不能被清零。
                                               本SC_LOCK1_REG类型寄存器被锁定（置1）后，除非复位，无法解锁。
                                               对应：
                                               [0-16]:对mdm access的region addr寄存器配置的锁。 */
    } reg;
} SOC_PCTRL_PERI_CTRL61_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL61_lock_START  (0)
#define SOC_PCTRL_PERI_CTRL61_lock_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL62_UNION
 struct description   : PERI_CTRL62 Register structure definition
                        Address Offset:0x1C8C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器62_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tsp0_wakeup_irps_en_0 : 32; /* bit[0-31]: tsp0_wakeup_irps_en[31:0] */
    } reg;
} SOC_PCTRL_PERI_CTRL62_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL62_tsp0_wakeup_irps_en_0_START  (0)
#define SOC_PCTRL_PERI_CTRL62_tsp0_wakeup_irps_en_0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL63_UNION
 struct description   : PERI_CTRL63 Register structure definition
                        Address Offset:0x1C90 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器63_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  tsp0_wakeup_irps_en_1 : 32; /* bit[0-31]: tsp0_wakeup_irps_en[63:32] */
    } reg;
} SOC_PCTRL_PERI_CTRL63_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL63_tsp0_wakeup_irps_en_1_START  (0)
#define SOC_PCTRL_PERI_CTRL63_tsp0_wakeup_irps_en_1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL64_UNION
 struct description   : PERI_CTRL64 Register structure definition
                        Address Offset:0x1C94 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器64_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL64_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL65_UNION
 struct description   : PERI_CTRL65 Register structure definition
                        Address Offset:0x1C98 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器65_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL65_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL66_UNION
 struct description   : PERI_CTRL66 Register structure definition
                        Address Offset:0x1C9C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器66_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL66_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL75_UNION
 struct description   : PERI_CTRL75 Register structure definition
                        Address Offset:0x1CA0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器75_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL75_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL76_UNION
 struct description   : PERI_CTRL76 Register structure definition
                        Address Offset:0x1CA4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器76_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL76_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL77_UNION
 struct description   : PERI_CTRL77 Register structure definition
                        Address Offset:0x1CA8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器77_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved8 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL77_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL77_pctrl_modem_reserved8_START  (0)
#define SOC_PCTRL_PERI_CTRL77_pctrl_modem_reserved8_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL78_UNION
 struct description   : PERI_CTRL78 Register structure definition
                        Address Offset:0x1CAC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器78_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved9 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL78_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL78_pctrl_modem_reserved9_START  (0)
#define SOC_PCTRL_PERI_CTRL78_pctrl_modem_reserved9_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL79_UNION
 struct description   : PERI_CTRL79 Register structure definition
                        Address Offset:0x1CB0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器79_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved10 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL79_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL79_pctrl_modem_reserved10_START  (0)
#define SOC_PCTRL_PERI_CTRL79_pctrl_modem_reserved10_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL80_UNION
 struct description   : PERI_CTRL80 Register structure definition
                        Address Offset:0x1CB4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器80_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved11 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL80_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL80_pctrl_modem_reserved11_START  (0)
#define SOC_PCTRL_PERI_CTRL80_pctrl_modem_reserved11_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL81_UNION
 struct description   : PERI_CTRL81 Register structure definition
                        Address Offset:0x1CB8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器81_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved12 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL81_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL81_pctrl_modem_reserved12_START  (0)
#define SOC_PCTRL_PERI_CTRL81_pctrl_modem_reserved12_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL82_UNION
 struct description   : PERI_CTRL82 Register structure definition
                        Address Offset:0x1CBC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器82_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved13 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL82_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL82_pctrl_modem_reserved13_START  (0)
#define SOC_PCTRL_PERI_CTRL82_pctrl_modem_reserved13_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL83_UNION
 struct description   : PERI_CTRL83 Register structure definition
                        Address Offset:0x1CC0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器83_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved14 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL83_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL83_pctrl_modem_reserved14_START  (0)
#define SOC_PCTRL_PERI_CTRL83_pctrl_modem_reserved14_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL84_UNION
 struct description   : PERI_CTRL84 Register structure definition
                        Address Offset:0x1CC4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器84_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  pctrl_modem_reserved15 : 32; /* bit[0-31]: charlotte ES上保留给modem的寄存器，给底软使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL84_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL84_pctrl_modem_reserved15_START  (0)
#define SOC_PCTRL_PERI_CTRL84_pctrl_modem_reserved15_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL85_UNION
 struct description   : PERI_CTRL85 Register structure definition
                        Address Offset:0x1CC8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器85_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL85_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL86_UNION
 struct description   : PERI_CTRL86 Register structure definition
                        Address Offset:0x1CCC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器86_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL86_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL87_UNION
 struct description   : PERI_CTRL87 Register structure definition
                        Address Offset:0x1CD0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器87_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL87_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT160_UNION
 struct description   : PERI_STAT160 Register structure definition
                        Address Offset:0x1CD4 Initial:0x00000000 Width:32
 register description : 通用外设状态寄存器160_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0: 1;  /* bit[0]   :  */
        unsigned int  reserved_1: 31; /* bit[1-31]:  */
    } reg;
} SOC_PCTRL_PERI_STAT160_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL168_UNION
 struct description   : PERI_CTRL168 Register structure definition
                        Address Offset:0x1CD8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器141_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL168_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL169_UNION
 struct description   : PERI_CTRL169 Register structure definition
                        Address Offset:0x1CDC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器142_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL169_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL170_UNION
 struct description   : PERI_CTRL170 Register structure definition
                        Address Offset:0x1CE0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器143_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL170_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL171_UNION
 struct description   : PERI_CTRL171 Register structure definition
                        Address Offset:0x1CE4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器144_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL171_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL172_UNION
 struct description   : PERI_CTRL172 Register structure definition
                        Address Offset:0x1CE8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器145_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL172_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL173_UNION
 struct description   : PERI_CTRL173 Register structure definition
                        Address Offset:0x1CEC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器146_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL173_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL174_UNION
 struct description   : PERI_CTRL174 Register structure definition
                        Address Offset:0x1CF0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器147_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL174_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL175_UNION
 struct description   : PERI_CTRL175 Register structure definition
                        Address Offset:0x1CF8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器148_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL175_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL176_UNION
 struct description   : PERI_CTRL176 Register structure definition
                        Address Offset:0x1CFC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器149_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL176_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL177_UNION
 struct description   : PERI_CTRL177 Register structure definition
                        Address Offset:0x1D00 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器150_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL177_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL178_UNION
 struct description   : PERI_CTRL178 Register structure definition
                        Address Offset:0x1D04 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器151_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL178_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL179_UNION
 struct description   : PERI_CTRL179 Register structure definition
                        Address Offset:0x1D08 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器152_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL179_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL180_UNION
 struct description   : PERI_CTRL180 Register structure definition
                        Address Offset:0x1D0C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器180_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL180_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL181_UNION
 struct description   : PERI_CTRL181 Register structure definition
                        Address Offset:0x1D10 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器181_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL181_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL182_UNION
 struct description   : PERI_CTRL182 Register structure definition
                        Address Offset:0x1D14 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器182_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL182_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL183_UNION
 struct description   : PERI_CTRL183 Register structure definition
                        Address Offset:0x1D18 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器183_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL183_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL184_UNION
 struct description   : PERI_CTRL184 Register structure definition
                        Address Offset:0x1D1C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器184_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL184_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL185_UNION
 struct description   : PERI_CTRL185 Register structure definition
                        Address Offset:0x1D20 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器185_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL185_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL186_UNION
 struct description   : PERI_CTRL186 Register structure definition
                        Address Offset:0x1D24 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器186_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL186_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL187_UNION
 struct description   : PERI_CTRL187 Register structure definition
                        Address Offset:0x1D28 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器187_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL187_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL188_UNION
 struct description   : PERI_CTRL188 Register structure definition
                        Address Offset:0x1D2C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器188_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL188_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL189_UNION
 struct description   : PERI_CTRL189 Register structure definition
                        Address Offset:0x1D30 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器189_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL189_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL190_UNION
 struct description   : PERI_CTRL190 Register structure definition
                        Address Offset:0x1D34 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器190_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL190_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL191_UNION
 struct description   : PERI_CTRL191 Register structure definition
                        Address Offset:0x1D38 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器191_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL191_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL192_UNION
 struct description   : PERI_CTRL192 Register structure definition
                        Address Offset:0x1D3C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器192_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL192_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL193_UNION
 struct description   : PERI_CTRL193 Register structure definition
                        Address Offset:0x1D40 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器193_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL193_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL194_UNION
 struct description   : PERI_CTRL194 Register structure definition
                        Address Offset:0x1D44 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器194_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL194_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL195_UNION
 struct description   : PERI_CTRL195 Register structure definition
                        Address Offset:0x1D48 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器195_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL195_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL196_UNION
 struct description   : PERI_CTRL196 Register structure definition
                        Address Offset:0x1D4C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器196_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL196_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL197_UNION
 struct description   : PERI_CTRL197 Register structure definition
                        Address Offset:0x1D50 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器197_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL197_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL198_UNION
 struct description   : PERI_CTRL198 Register structure definition
                        Address Offset:0x1D54 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器198_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL198_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL199_UNION
 struct description   : PERI_CTRL199 Register structure definition
                        Address Offset:0x1D58 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器199_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL199_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL200_UNION
 struct description   : PERI_CTRL200 Register structure definition
                        Address Offset:0x1D5C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器200_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL200_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL201_UNION
 struct description   : PERI_CTRL201 Register structure definition
                        Address Offset:0x1D60 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器201_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL201_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL202_UNION
 struct description   : PERI_CTRL202 Register structure definition
                        Address Offset:0x1D64 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器202_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL202_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL203_UNION
 struct description   : PERI_CTRL203 Register structure definition
                        Address Offset:0x1D68 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器203_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL203_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL204_UNION
 struct description   : PERI_CTRL204 Register structure definition
                        Address Offset:0x1D6C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器204_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved_0   : 1;  /* bit[0]   :  */
        unsigned int  bootram_auth : 1;  /* bit[1]   : lpmcu子系统外部mst访问lpram权限控制，默认有权限访问. */
        unsigned int  reserved_1   : 30; /* bit[2-31]:  */
    } reg;
} SOC_PCTRL_PERI_CTRL204_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL204_bootram_auth_START  (1)
#define SOC_PCTRL_PERI_CTRL204_bootram_auth_END    (1)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL205_UNION
 struct description   : PERI_CTRL205 Register structure definition
                        Address Offset:0x1D70 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器205_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read0 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL205_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL205_software_secure_write_read0_START  (0)
#define SOC_PCTRL_PERI_CTRL205_software_secure_write_read0_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL206_UNION
 struct description   : PERI_CTRL206 Register structure definition
                        Address Offset:0x1D74 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器206_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read1 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL206_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL206_software_secure_write_read1_START  (0)
#define SOC_PCTRL_PERI_CTRL206_software_secure_write_read1_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL207_UNION
 struct description   : PERI_CTRL207 Register structure definition
                        Address Offset:0x1D78 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器207_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read2 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL207_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL207_software_secure_write_read2_START  (0)
#define SOC_PCTRL_PERI_CTRL207_software_secure_write_read2_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL208_UNION
 struct description   : PERI_CTRL208 Register structure definition
                        Address Offset:0x1D7C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器208_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read3 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL208_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL208_software_secure_write_read3_START  (0)
#define SOC_PCTRL_PERI_CTRL208_software_secure_write_read3_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL209_UNION
 struct description   : PERI_CTRL209 Register structure definition
                        Address Offset:0x1D80 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器209_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read4 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL209_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL209_software_secure_write_read4_START  (0)
#define SOC_PCTRL_PERI_CTRL209_software_secure_write_read4_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL210_UNION
 struct description   : PERI_CTRL210 Register structure definition
                        Address Offset:0x1D84 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器210_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read5 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL210_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL210_software_secure_write_read5_START  (0)
#define SOC_PCTRL_PERI_CTRL210_software_secure_write_read5_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL211_UNION
 struct description   : PERI_CTRL211 Register structure definition
                        Address Offset:0x1D88 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器211_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read6 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL211_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL211_software_secure_write_read6_START  (0)
#define SOC_PCTRL_PERI_CTRL211_software_secure_write_read6_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL212_UNION
 struct description   : PERI_CTRL212 Register structure definition
                        Address Offset:0x1D8C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器212_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  software_secure_write_read7 : 32; /* bit[0-31]: 给软件安全相关密钥交换相关功能，配置和读取使用。
                                                                      只在PCTRL内部存在，不伸出IP，以免被使用 */
    } reg;
} SOC_PCTRL_PERI_CTRL212_UNION;
#endif
#define SOC_PCTRL_PERI_CTRL212_software_secure_write_read7_START  (0)
#define SOC_PCTRL_PERI_CTRL212_software_secure_write_read7_END    (31)


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL213_UNION
 struct description   : PERI_CTRL213 Register structure definition
                        Address Offset:0x1D90 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器213_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL213_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL214_UNION
 struct description   : PERI_CTRL214 Register structure definition
                        Address Offset:0x1D94 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器214_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL214_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL215_UNION
 struct description   : PERI_CTRL215 Register structure definition
                        Address Offset:0x1D98 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器215_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL215_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL216_UNION
 struct description   : PERI_CTRL216 Register structure definition
                        Address Offset:0x1D9C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器216_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL216_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL217_UNION
 struct description   : PERI_CTRL217 Register structure definition
                        Address Offset:0x1DA0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器217_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL217_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL218_UNION
 struct description   : PERI_CTRL218 Register structure definition
                        Address Offset:0x1DA4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器218_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL218_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL219_UNION
 struct description   : PERI_CTRL219 Register structure definition
                        Address Offset:0x1DA8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器219_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL219_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL220_UNION
 struct description   : PERI_CTRL220 Register structure definition
                        Address Offset:0x1DAC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器220_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL220_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL221_UNION
 struct description   : PERI_CTRL221 Register structure definition
                        Address Offset:0x1DB0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器221_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL221_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL222_UNION
 struct description   : PERI_CTRL222 Register structure definition
                        Address Offset:0x1DB4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器222_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL222_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL223_UNION
 struct description   : PERI_CTRL223 Register structure definition
                        Address Offset:0x1DB8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器223_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL223_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL224_UNION
 struct description   : PERI_CTRL224 Register structure definition
                        Address Offset:0x1DBC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器224_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL224_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL225_UNION
 struct description   : PERI_CTRL225 Register structure definition
                        Address Offset:0x1DC0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器225_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL225_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL226_UNION
 struct description   : PERI_CTRL226 Register structure definition
                        Address Offset:0x1DC4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器226_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL226_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL227_UNION
 struct description   : PERI_CTRL227 Register structure definition
                        Address Offset:0x1DC8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器227_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL227_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL228_UNION
 struct description   : PERI_CTRL228 Register structure definition
                        Address Offset:0x1DCC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器228_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL228_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL229_UNION
 struct description   : PERI_CTRL229 Register structure definition
                        Address Offset:0x1DD0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器229_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL229_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL230_UNION
 struct description   : PERI_CTRL230 Register structure definition
                        Address Offset:0x1DD4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器230_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL230_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL231_UNION
 struct description   : PERI_CTRL231 Register structure definition
                        Address Offset:0x1DD8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器231_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL231_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL232_UNION
 struct description   : PERI_CTRL232 Register structure definition
                        Address Offset:0x1DDC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器232_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL232_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL233_UNION
 struct description   : PERI_CTRL233 Register structure definition
                        Address Offset:0x1DE0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器233_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL233_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL234_UNION
 struct description   : PERI_CTRL234 Register structure definition
                        Address Offset:0x1DE4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器234_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL234_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL235_UNION
 struct description   : PERI_CTRL235 Register structure definition
                        Address Offset:0x1DE8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器235_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL235_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL236_UNION
 struct description   : PERI_CTRL236 Register structure definition
                        Address Offset:0x1DEC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器236_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL236_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL237_UNION
 struct description   : PERI_CTRL237 Register structure definition
                        Address Offset:0x1DF0 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器237_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL237_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL238_UNION
 struct description   : PERI_CTRL238 Register structure definition
                        Address Offset:0x1DF4 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器238_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL238_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL239_UNION
 struct description   : PERI_CTRL239 Register structure definition
                        Address Offset:0x1DF8 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器239_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL239_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL240_UNION
 struct description   : PERI_CTRL240 Register structure definition
                        Address Offset:0x1DFC Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器240_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL240_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL241_UNION
 struct description   : PERI_CTRL241 Register structure definition
                        Address Offset:0x1E00 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器241_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL241_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL242_UNION
 struct description   : PERI_CTRL242 Register structure definition
                        Address Offset:0x1E04 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器242_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL242_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL243_UNION
 struct description   : PERI_CTRL243 Register structure definition
                        Address Offset:0x1E08 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器243_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL243_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL244_UNION
 struct description   : PERI_CTRL244 Register structure definition
                        Address Offset:0x1E0C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器244_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL244_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL245_UNION
 struct description   : PERI_CTRL245 Register structure definition
                        Address Offset:0x1E10 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器245_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL245_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL246_UNION
 struct description   : PERI_CTRL246 Register structure definition
                        Address Offset:0x1E14 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器246_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL246_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL247_UNION
 struct description   : PERI_CTRL247 Register structure definition
                        Address Offset:0x1E18 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器247_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL247_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL248_UNION
 struct description   : PERI_CTRL248 Register structure definition
                        Address Offset:0x1E1C Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器248_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL248_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_CTRL249_UNION
 struct description   : PERI_CTRL249 Register structure definition
                        Address Offset:0x1E20 Initial:0x00000000 Width:32
 register description : 通用外设控制寄存器249_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  reserved : 32; /* bit[0-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_CTRL249_UNION;
#endif


/*****************************************************************************
 struct               : SOC_PCTRL_PERI_STAT_SEC0_UNION
 struct description   : PERI_STAT_SEC0 Register structure definition
                        Address Offset:0x1E24 Initial:0x0000000 Width:32
 register description : 通用外设回读寄存器0_安全区
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  ddr_slp_ack_external_a : 1;  /* bit[0]   :  */
        unsigned int  ddr_slp_ack_external_b : 1;  /* bit[1]   :  */
        unsigned int  ddr_slp_ack_external_c : 1;  /* bit[2]   :  */
        unsigned int  ddr_slp_ack_external_d : 1;  /* bit[3]   :  */
        unsigned int  reserved               : 28; /* bit[4-31]: 保留 */
    } reg;
} SOC_PCTRL_PERI_STAT_SEC0_UNION;
#endif
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_a_START  (0)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_a_END    (0)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_b_START  (1)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_b_END    (1)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_c_START  (2)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_c_END    (2)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_d_START  (3)
#define SOC_PCTRL_PERI_STAT_SEC0_ddr_slp_ack_external_d_END    (3)






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

#endif /* end of soc_pctrl_interface.h */
