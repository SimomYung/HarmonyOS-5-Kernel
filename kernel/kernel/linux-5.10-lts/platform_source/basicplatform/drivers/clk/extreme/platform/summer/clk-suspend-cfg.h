#ifndef __CLK_SUSPEND_CFG_H
#define __CLK_SUSPEND_CFG_H

#define MASK_ALL	0xFFFF0000
#define array_size(arr) (sizeof(arr) / sizeof((arr)[0]))

/* AO REG offset */
#define SCCLKDIV0             0x250
#define SCCLKDIV3             0x25C
#define SCCLKDIV4             0x260
#define SCCLKDIV5             0x264
#define SCCLKDIV6             0x268
#define SCCLKDIV7             0x26C
#define SCCLKDIV9             0x274
#define SCCLKDIV10            0x280
#define SCCLKDIV11            0x284
#define SCCLKDIV12            0x288
#define SCCLKDIV14            0x290
#define SCCLKDIV15            0x294
#define SCCLKDIV16            0x298
#define SCCLKDIV19            0x2A4
#define SCCLKDIV20            0x2A8
#define SCCLKDIV21            0x2AC

/* PERI REG offset */
#define PPLL6CTRL0            0x830
#define PPLL6CTRL1            0x834
#define CLKDIV0               0x0A8
#define CLKDIV1               0x0AC
#define CLKDIV2               0x0B0
#define CLKDIV3               0x0B4
#define CLKDIV4               0x0B8
#define CLKDIV5               0x0BC
#define CLKDIV6               0x0C0
#define CLKDIV7               0x0C4
#define CLKDIV10              0x0D0
#define CLKDIV11              0x0D4
#define CLKDIV12              0x0D8
#define CLKDIV13              0x0DC
#define CLKDIV14              0x0E0
#define CLKDIV16              0x0E8
#define CLKDIV18              0x0F0
#define CLKDIV19              0x0F4
#define CLKDIV20              0x0F8
#define CLKDIV21              0x0FC
#define CLKDIV22              0x100
#define CLKDIV24              0x108
#define CLKDIV25              0x10C
#define CLKDIV26              0x700
#define CLKDIV27              0x704
#define CLKDIV29              0x70C
#define CLKDIV30              0x710
#define CLKDIV32              0x718


int plat_clk_hibernate_sr_init(void);

#endif /* __CLK_SUSPEND_CFG_H */