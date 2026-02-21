/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to start vcom between ril and kernel
 */

#ifndef _VCOM_H_
#define _VCOM_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// RF factory version ioctl cmd
#define RF_IOCTL_MAGIC_FAC 'F'
#define RSMC_START_CMD_FAC _IO(RF_IOCTL_MAGIC_FAC, 0)
#define RSMC_STOP_CMD_FAC _IO(RF_IOCTL_MAGIC_FAC, 1)
#define TTS_START_CMD_FAC _IO(RF_IOCTL_MAGIC_FAC, 2)
#define TTS_STOP_CMD_FAC _IO(RF_IOCTL_MAGIC_FAC, 3)
#define GET_HKADC_VALUE_FAC  _IOW(RF_IOCTL_MAGIC_FAC, 4, int)
#define GET_RF_BOARD_ID      _IO(RF_IOCTL_MAGIC_FAC, 5)

// RF release version ioctl cmd
#define RF_IOCTL_MAGIC_RELEASE 'R'
#define RSMC_START_CMD_RELEASE _IO(RF_IOCTL_MAGIC_RELEASE, 0)
#define RSMC_STOP_CMD_RELEASE _IO(RF_IOCTL_MAGIC_RELEASE, 1)
#define TTS_START_CMD_RELEASE _IO(RF_IOCTL_MAGIC_RELEASE, 2)
#define TTS_STOP_CMD_RELEASE _IO(RF_IOCTL_MAGIC_RELEASE, 3)

int hw_rfvcom_init(void);
void hw_rfvcom_exit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
