#ifndef __UFS_FPGA_SPI_H__
#define __UFS_FPGA_SPI_H__

#include "ufshcd.h"

#define spi_bpw  8

struct ufs_fpga_spi_info {
    struct spi_device *spi;
    void __iomem *xctrl_reg_base;
    u32 xctrl_spi_offset;
    u32 xctrl_spi1_cs_sel;
    u32 xctrl_spi1_cs_val;
};


int ufs_fpga_spi_read(u32 addr);
int ufs_fpga_spi_write(u32 value, u32 addr);
int hufs_fpga_spi_driver_probe(struct ufs_hba *hba);

#endif
