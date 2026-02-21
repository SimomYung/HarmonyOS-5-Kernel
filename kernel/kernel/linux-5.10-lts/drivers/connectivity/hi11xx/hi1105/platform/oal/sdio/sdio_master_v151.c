/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 *
 * Description:sdio master ip driver, v151
 * Author: @CompanyNameTag
 */

#define HISI_LOG_TAG               "[SDIO_V151]"
#include "oal_util.h"
#include "sdio/oal_sdio_host_if.h"
#include "sdio_master_v151.h"

/* v151 sdio function1 registers */
#define HISDIO_REG_FUNC1_FIFO                0x00  /* Read Write FIFO */
#define HISDIO_REG_FUNC1_INT_STATUS          0x08  /* interrupt mask and clear reg */
#define HISDIO_REG_FUNC1_INT_ENABLE          0x09  /* interrupt */
#define HISDIO_REG_FUNC1_XFER_COUNT          0x20  /* notify number of bytes to be read */
#define HISDIO_REG_FUNC1_WRITE_MSG           0x28  /* write msg to device */
#define HISDIO_REG_FUNC1_MSG_FROM_DEV        0x2c  /* notify Host that device has got the msg */
#define HISDIO_REG_FUNC1_MSG_HIGH_FROM_DEV   0x2f  /* notify master, slave had got the msg, master rev the msg ack */

#define HISDIO_EXTEND_BASE_ADDR 0x30
#define HISDIO_FUNC1_EXTEND_REG_BASE 0x3c

int32_t sdio_master_ip_init_v151(struct oal_sdio *sdio)
{
    struct msdio_func1_addr *f1_addr = &sdio->reg_addr.func1_addr;

    /* function1 reg address */
    f1_addr->fifo = HISDIO_REG_FUNC1_FIFO;
    f1_addr->int_status = HISDIO_REG_FUNC1_INT_STATUS;
    f1_addr->int_en = HISDIO_REG_FUNC1_INT_ENABLE;
    f1_addr->slave_msg_with_ack = HISDIO_REG_FUNC1_MSG_HIGH_FROM_DEV;
    f1_addr->extend_sdio_func = HISDIO_EXTEND_BASE_ADDR;      /* 扩展寄存器，前12字节功能定制 */
    f1_addr->extend_sdio_comm = HISDIO_FUNC1_EXTEND_REG_BASE; /* 扩展寄存器，后68字节RW存储 */
    f1_addr->xfercount = HISDIO_REG_FUNC1_XFER_COUNT;
    f1_addr->host_msg = HISDIO_REG_FUNC1_WRITE_MSG;
    f1_addr->slave_msg = HISDIO_REG_FUNC1_MSG_FROM_DEV;
    oal_print_mpxx_log(MPXX_LOG_INFO, "sdio master v151 ip init");
    return OAL_SUCC;
}