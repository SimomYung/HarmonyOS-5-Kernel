/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat factory test function
 * Create : 2024/3/12
 */
#ifndef __SEPLAT_FACTORY_H_
#define __SEPLAT_FACTORY_H_

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/types.h>
#include <dsm/dsm_pub.h>

#define SEPLAT_GET_DIEID_PADD_LEN     6  /* ":0x" "\r\n" '\0' */
#define SEPLAT_FT_DIEID_SIZE_MAX      32

#define SEPLAT_BSP_RET_OK 0x5A5A

#define SEPLAR_FT_FLASHECC_xBIT_THRESHOLD   1
#define SEPLAR_FT_FLASHECC_1BIT_THRESHOLD   36

ssize_t seplat_flash_ecc_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t seplat_flash_ecc_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

void seplat_ft_remove_files(struct device *pdevice);
int32_t seplat_ft_create_files(struct device *pdevice);

int seplat_ft_get_dieid(char *dieid, u32 len);

#endif /* __SEPLAT_FACTORY_H_ */
