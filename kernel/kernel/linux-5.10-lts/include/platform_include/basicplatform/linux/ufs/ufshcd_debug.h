/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: ufs debug header file
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __UFSHCD_DEBUG_H__
#define __UFSHCD_DEBUG_H__
#include "ufshcd.h"

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
#define UFS_HIXXXX_PRODUCT_NAME                 "SS6100GBCV100"
#define HIXXXX_PROD_LEN                         (13)
#define UFS_PRODUCT_NAME_THOR920                "THR920"
#define UFS_PRODUCT_NAME_THOR925                "THR925"
#define UFS_PRODUCT_NAME_THOR950_DFA            "THR950GFCV10"
#define UFS_PRODUCT_NAME_SS6100                 "SS6100"
#define UFS_PRODUCT_NAME_LEN                    (16)
#define UFS_PRODUCT_NAME_THOR950_LEN            (12)
#define VCMD_READ_BUFFER_ID                     (0x22)
#define FSR_READ_BUFFER_SIZE                    (8192)
#endif

ssize_t config_desc_store(struct device *dev,
				 struct device_attribute *attr, const char *buf,
				 size_t count);
ssize_t config_desc_show(struct device *dev,
				struct device_attribute *attr, char *buf);
ssize_t unique_number_show(struct device *dev,
					 struct device_attribute *attr,
					 char *buf);
ssize_t man_id_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t shared_alloc_units_show(struct device *dev,
				       struct device_attribute *attr, char *buf);
ssize_t spec_version_show(struct device *dev, struct device_attribute *attr, char *buf);

void ufshcd_complete_time(struct ufs_hba *hba, struct ufshcd_lrb *lrbp,
				 uint8_t opcode, ktime_t complete_time);
void ufshcd_print_host_regs(struct ufs_hba *hba);
void ufshcd_print_gic(struct ufs_hba *hba);
void ufshcd_init_ufs_temp_sys(struct ufs_hba *hba);
void ufshcd_fsr_dump_handler(struct work_struct *work);
void ufshcd_ufs_set_dieid(struct ufs_hba *hba, struct ufs_dev_info *dev_info);
void ufshcd_init_fsr_sys(struct ufs_hba *hba);
void hufs_parse_dev_desc(struct ufs_hba *hba,
				    struct ufs_dev_info *dev_info,
				    const u8 *desc_buf);
void ufshcd_add_rpm_lvl_sysfs_nodes(struct ufs_hba *hba);
void ufshcd_add_spm_lvl_sysfs_nodes(struct ufs_hba *hba);
int ufshcd_read_fsr(struct ufs_hba *hba, u8 *buf, u32 size);

static inline void ufshcd_add_sysfs_nodes(struct ufs_hba *hba)
{
	ufshcd_add_rpm_lvl_sysfs_nodes(hba);
	ufshcd_add_spm_lvl_sysfs_nodes(hba);
}

static inline void ufshcd_remove_sysfs_nodes(struct ufs_hba *hba)
{
	device_remove_file(hba->dev, &hba->rpm_lvl_attr);
	device_remove_file(hba->dev, &hba->spm_lvl_attr);
}

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
void ufsfsr_set_sdev(struct scsi_device *dev, struct ufs_hba *hba);
#endif

#endif
