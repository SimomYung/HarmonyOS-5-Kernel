/*
 * ufshcd_debug.c
 *
 * basic interface for hufs
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "ufshcd_debug.h"
#include "ufshcd.h"
#include "ufshcd_extend.h"
#include <securec.h>

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
static char ufs_product_name[UFS_PRODUCT_NAME_LEN + 1];
#ifdef CONFIG_PLATFORM_DIEID
static u8 *ufs_hixxxx_dieid;
static int is_fsr_read_failed;
#endif
#endif

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
/*
 * specified scsi device for lun, used by read buffer to get request queue
 */
#define UFS_FSR_LUN_NUM   2
static struct scsi_device *ufsfsr_sdev;
static struct ufs_hba *ufsfsr_hba;
void ufsfsr_set_sdev(struct scsi_device *dev, struct ufs_hba *hba)
{
	if (dev->lun == UFS_FSR_LUN_NUM) {
		ufsfsr_sdev = dev;
		ufsfsr_hba = shost_priv(dev->host);
#ifdef CONFIG_PLATFORM_DIEID
		ufshcd_ufs_set_dieid(hba, &hba->dev_info);
#endif
	}
}
#endif

const char *hufs_uic_link_state_to_string(
			enum uic_link_state state)
{
	switch (state) {
	case UIC_LINK_OFF_STATE:
		return "OFF";
	case UIC_LINK_ACTIVE_STATE:
		return "ACTIVE";
	case UIC_LINK_HIBERN8_STATE:
		return "HIBERN8";
	default:
		return "UNKNOWN";
	}
}

const char *hufs_dev_pwr_mode_to_string(
			enum ufs_dev_pwr_mode state)
{
	switch (state) {
	case UFS_ACTIVE_PWR_MODE:
		return "ACTIVE";
	case UFS_SLEEP_PWR_MODE:
		return "SLEEP";
	case UFS_POWERDOWN_PWR_MODE:
		return "POWERDOWN";
	default:
		return "UNKNOWN";
	}
}

ssize_t config_desc_store(struct device *dev,
				 struct device_attribute *attr, const char *buf,
				 size_t count)
{
	u32 value = 0;
	u32 origin_attr = 0;
	int ret;
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);

	ret = kstrtouint(buf, 0, &value);
	if (ret || value != 1) {
		dev_err(hba->dev, "%s invalid value ret %d value %d \n",
			__func__, ret, value);
		return -EINVAL;
	}
	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				      QUERY_ATTR_IDN_CONF_DESC_LOCK, 0, 0,
				      &origin_attr);
	if (ret) {
		dev_err(hba->dev, "%s read config desc lock failed \n",
			__func__);
		return -EBUSY;
	}
	dev_err(hba->dev, "%s config desc is %d\n", __func__, origin_attr);
	if (origin_attr == 1) {
		dev_err(hba->dev, "%s: config desc lock already locked \n",
			__func__);
		return -EPERM;
	}
	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_WRITE_ATTR,
				      QUERY_ATTR_IDN_CONF_DESC_LOCK, 0, 0,
				      &value);
	if (ret) {
		dev_err(hba->dev, "%s enable config desc lock failed \n",
			__func__);
		return -EBUSY;
	}
	return count;
}

ssize_t config_desc_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	u32 value = 0;
	int ret;

	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				      QUERY_ATTR_IDN_CONF_DESC_LOCK, 0, 0,
				      &value);
	if (ret) {
		dev_err(hba->dev, "%s read config desc lock failed \n",
			__func__);
		return -EBUSY;
	}
	return snprintf(buf, sizeof(u32), "%d\n", value);
}

ssize_t unique_number_show(struct device *dev,
					 struct device_attribute *attr,
					 char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	int curr_len;

	curr_len = snprintf(
	    buf, PAGE_SIZE,
	    "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%04x%04x\n",
	    hba->unique_number.serial_number[0],
	    hba->unique_number.serial_number[1],
	    hba->unique_number.serial_number[2],
	    hba->unique_number.serial_number[3],
	    hba->unique_number.serial_number[4],
	    hba->unique_number.serial_number[5],
	    hba->unique_number.serial_number[6],
	    hba->unique_number.serial_number[7],
	    hba->unique_number.serial_number[8],
	    hba->unique_number.serial_number[9],
	    hba->unique_number.serial_number[10],
	    hba->unique_number.serial_number[11],
	    hba->unique_number.manufacturer_date,
	    hba->unique_number.manufacturer_id);

	return curr_len;
}

ssize_t man_id_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	int curr_len;

	curr_len = snprintf(buf, PAGE_SIZE, "%04x\n", hba->manufacturer_id);

	return curr_len;
}

ssize_t shared_alloc_units_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	return snprintf(buf, PAGE_SIZE, "0x%x\n", hba->wb_shared_alloc_units);
}

ssize_t spec_version_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	return snprintf(buf, PAGE_SIZE, "%x\n", hba->ufs_device_spec_version);
}

ssize_t ufshcd_rpm_lvl_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	int ret;
	int curr_len = 0;
	u8 lvl;

	if (hba->rpm_lvl >= UFS_PM_LVL_MAX)
		return 0;

	ret = snprintf(buf, PAGE_SIZE,
			"\nCurrent Runtime PM level [%d] => dev_state [%s] link_state [%s]\n",
			hba->rpm_lvl,
			hufs_dev_pwr_mode_to_string(
				ufs_pm_lvl_states[hba->rpm_lvl].dev_state),
			hufs_uic_link_state_to_string(
				ufs_pm_lvl_states[hba->rpm_lvl].link_state));
	if (ret <= 0)
		return -1;
	curr_len += ret;

	ret = snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
			"\nAll available Runtime PM levels info:\n");
	if (ret <= 0)
		return -1;
	curr_len += ret;

	for (lvl = UFS_PM_LVL_0; lvl < UFS_PM_LVL_MAX; lvl++) {
		ret = snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
				"\tRuntime PM level [%d] => dev_state [%s] link_state [%s]\n",
				lvl,
				hufs_dev_pwr_mode_to_string(
					ufs_pm_lvl_states[lvl].dev_state),
				hufs_uic_link_state_to_string(
					ufs_pm_lvl_states[lvl].link_state));
		if (ret <= 0)
			return -1;
		curr_len += ret;
	}

	return curr_len;
}

ssize_t ufshcd_pm_lvl_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count,
		bool rpm)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	unsigned long flags, value;

	if (kstrtoul(buf, 0, &value))
		return -EINVAL;

	if (value >= UFS_PM_LVL_MAX)
		return -EINVAL;

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (rpm)
		hba->rpm_lvl = value;
	else
		hba->spm_lvl = value;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return count;
}

ssize_t ufshcd_rpm_lvl_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	return ufshcd_pm_lvl_store(dev, attr, buf, count, true);
}

void ufshcd_add_rpm_lvl_sysfs_nodes(struct ufs_hba *hba)
{
	hba->rpm_lvl_attr.show = ufshcd_rpm_lvl_show;
	hba->rpm_lvl_attr.store = ufshcd_rpm_lvl_store;
	sysfs_attr_init(&hba->rpm_lvl_attr.attr);
	hba->rpm_lvl_attr.attr.name = "rpm_lvl";
	hba->rpm_lvl_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->rpm_lvl_attr))
		dev_err(hba->dev, "Failed to create sysfs for rpm_lvl\n");
}

ssize_t ufshcd_spm_lvl_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	int curr_len;
	u8 lvl;

	if (hba->spm_lvl >= UFS_PM_LVL_MAX)
		return 0;

	curr_len = snprintf(buf, PAGE_SIZE,
			    "\nCurrent System PM level [%d] => dev_state [%s] link_state [%s]\n",
			    hba->spm_lvl,
			    hufs_dev_pwr_mode_to_string(
				ufs_pm_lvl_states[hba->spm_lvl].dev_state),
			    hufs_uic_link_state_to_string(
				ufs_pm_lvl_states[hba->spm_lvl].link_state));

	curr_len += snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
			     "\nAll available System PM levels info:\n");
	for (lvl = UFS_PM_LVL_0; lvl < UFS_PM_LVL_MAX; lvl++)
		curr_len += snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
				     "\tSystem PM level [%u] => dev_state [%s] link_state [%s]\n",
				    lvl,
				    hufs_dev_pwr_mode_to_string(
					ufs_pm_lvl_states[lvl].dev_state),
				    hufs_uic_link_state_to_string(
					ufs_pm_lvl_states[lvl].link_state));

	return curr_len;
}

ssize_t ufshcd_spm_lvl_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	return ufshcd_pm_lvl_store(dev, attr, buf, count, false);
}

void ufshcd_add_spm_lvl_sysfs_nodes(struct ufs_hba *hba)
{
	hba->spm_lvl_attr.show = ufshcd_spm_lvl_show;
	hba->spm_lvl_attr.store = ufshcd_spm_lvl_store;
	sysfs_attr_init(&hba->spm_lvl_attr.attr);
	hba->spm_lvl_attr.attr.name = "spm_lvl";
	hba->spm_lvl_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->spm_lvl_attr))
		dev_err(hba->dev, "Failed to create sysfs for spm_lvl\n");
}

void ufshcd_complete_time(struct ufs_hba *hba, struct ufshcd_lrb *lrbp,
		uint8_t opcode, ktime_t complete_time)
{
	s64 cost_time;

	lrbp->compl_time_stamp = complete_time;
	cost_time = ktime_ms_delta(lrbp->compl_time_stamp,
				   lrbp->issue_time_stamp);

#ifdef CONFIG_RPMB_UFS
	if (lrbp->lun == UFS_UPIU_RPMB_WLUN)
		g_rpmb_ufs_end_time = dfx_getcurtime();
#endif

	/* if request costs 2000ms, print it */
	if (cost_time >= 2000)
		dev_info(hba->dev,
			"opcode 0x%x, tag: %d, :cost %lldms, issue %lld, complete %lld\n",
			opcode, lrbp->task_tag, cost_time, ktime_to_us(lrbp->issue_time_stamp),
			ktime_to_us(lrbp->compl_time_stamp));
}

void ufshcd_print_uic_err_hist(struct ufs_hba *hba,
		struct ufs_uic_err_reg_hist *err_hist, char *err_name)
{
	unsigned int i;

	for (i = 0; i < UIC_ERR_REG_HIST_LENGTH; i++) {
		if (err_hist->reg[i] == 0)
			continue;
		dev_err(hba->dev, "%s[%u] = 0x%x at %llu us\n", err_name, i,
			err_hist->reg[i], ktime_to_us(err_hist->tstamp[i]));
	}
}

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
static int ufshcd_query_fsr_info(struct ufs_hba *hba,
				u8 desc_id,
				int desc_index,
				u8 *param_read_buf,
				u32 param_size)
{
	int ret;
	struct ufs_query_vcmd cmd = { 0 };

#ifdef CONFIG_DFA_MCQ
	if (is_dfa_enabled(hba))
		return -EPERM;
#endif

	cmd.buf_len = param_size;
	/* allocate memory to hold full descriptor */
	cmd.desc_buf = kmalloc(cmd.buf_len, GFP_KERNEL);
	if (!cmd.desc_buf)
		return -ENOMEM;
	if (memset_s(cmd.desc_buf, cmd.buf_len, 0, cmd.buf_len))
		return -ENOMEM;

	cmd.opcode = UPIU_QUERY_OPCODE_READ_HI1861_FSR;
	cmd.idn = desc_id;
	cmd.index = desc_index;
	cmd.query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;

	ret = ufshcd_query_vcmd_retry(hba, &cmd);
	if (ret) {
		dev_err(hba->dev, "%s: Failed reading FSR. desc_id %d "
				  "buff_len %d ret %d",
			__func__, desc_id, cmd.buf_len, ret);

		goto out;
	}
	if (memcpy_s(param_read_buf, param_size, cmd.desc_buf, cmd.buf_len))
		goto out;
out:
	kfree(cmd.desc_buf);

	return ret;
}

static int ufshcd_read_thor950_fsr_info(struct ufs_hba *hba, u8 *buffer, u16 buffer_len)
 {
	int ret;
	struct ufs_read_buffer_vcmd vcmd = { 0 };
	struct scsi_device *dev = ufsfsr_sdev;

	if (!dev) {
		dev_err(hba->dev, "%s: scsi_device is null\n", __func__);
		return -EINVAL;
	}
	vcmd.opcode = READ_BUFFER;
	vcmd.buffer_id = VCMD_READ_BUFFER_ID;
	vcmd.buffer_len = buffer_len;
	vcmd.retries = VCMD_REQ_RETRIES;
	vcmd.buffer = buffer;

	ret = ufshcd_scsi_read_buffer_vcmd(dev, &vcmd);
	if (ret)
		dev_err(hba->dev, "%s: ufshcd_read_buffer_vcmd_retry fail %d\n", __func__, ret);
	return ret;
 }

static bool is_query_fsr_device(struct ufs_hba *hba)
 {
	uint32_t i;
	char support_device[][MAX_MODEL_LEN + 1] = {{"THR92"}, {"THR93"}, {"THR61"}};

	for (i = 0; i < sizeof(support_device) / sizeof(support_device[0]); i++) {
		if (strncmp(hba->model, support_device[i], strlen(support_device[i])) == 0) {
			return true;
		}
	}
	return false;
 }

static int ufshcd_read_fsr_info(struct ufs_hba *hba,
				u8 desc_id,
				int desc_index,
				u8 *param_read_buf,
				u32 param_size)
{
	if (!is_query_fsr_device(hba)) {
		return ufshcd_read_thor950_fsr_info(hba, param_read_buf, (u16)param_size);
	}
	return ufshcd_query_fsr_info(hba, desc_id, desc_index, param_read_buf, param_size);;
}

int ufshcd_read_fsr(struct ufs_hba *hba, u8 *buf, u32 size)
{
	if (UFS_VENDOR_VELA61 != hba->manufacturer_id)
		return -EINVAL;
	if (!buf)
		return -ENOMEM;
	if (!size)
		return -EINVAL;

#ifdef CONFIG_SCSI_UFS_UNISTORE
	if (ufshcd_rw_buffer_is_enabled(hba))
		return ufshcd_get_fsr_by_read_buffer(hba, buf, (u16)size);
#endif
	return ufshcd_read_fsr_info(hba, 0, 0, buf, size);
}
#endif

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
/**
* ufshcd_get_fsr_command -scsi layer get fsr func
* @hba: Pointer to adapter instance
* @buf: the buf pointer to fsr info
* @size:the buf size, just like 4k byte
*/
int ufshcd_get_fsr_command(struct ufs_hba *hba, u8 *buf,
				u32 size)
{
	if (!hba) {
		pr_err( "%s shost_priv host failed\n", __func__);
		return -1;
	}
	return ufshcd_read_fsr(hba, buf, size);
}
EXPORT_SYMBOL(ufshcd_get_fsr_command);

static ssize_t ufshcd_fsr_info_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	/* allocate memory to hold full descriptor */
	u32 fsr_buf_len = HI1861_FSR_INFO_SIZE;
	u8 *fbuf = NULL;
	int len = 0;
	int i = 0;
	int ret = 0;

	if (UFS_VENDOR_VELA61 != hba->manufacturer_id)
		return 0;
	if (strncmp(hba->model, UFS_PRODUCT_NAME_THOR950_DFA, UFS_PRODUCT_NAME_THOR950_LEN) == 0) {
		fsr_buf_len = THR950_DFA_FSR_INFO_SIZE;
	}
	fbuf = kmalloc(fsr_buf_len, GFP_KERNEL);
	if (!fbuf)
		return -ENOMEM;

	ret = memset_s(fbuf, fsr_buf_len, 0, fsr_buf_len);
	if (ret) {
		kfree(fbuf);
		return ret;
	}
	ret = ufshcd_read_fsr(hba, fbuf, fsr_buf_len);
	if (ret) {
		kfree(fbuf);
		dev_err(hba->dev, "[%s]READ FSR FAILED\n", __func__);
		return ret;
	}
	/* lint -save -e661 -e662 */
	for (i = 0; i < fsr_buf_len; i = i + 16)
		len += snprintf(buf + len, PAGE_SIZE - len ,
	"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		*(fbuf + i + 0), *(fbuf + i + 1), *(fbuf + i + 2), *(fbuf + i + 3),
		*(fbuf + i + 4), *(fbuf + i + 5), *(fbuf + i + 6), *(fbuf + i + 7),
		*(fbuf + i + 8), *(fbuf + i + 9), *(fbuf + i + 10), *(fbuf + i + 11),
		*(fbuf + i + 12), *(fbuf + i + 13), *(fbuf + i + 14), *(fbuf + i + 15));
	/* lint -restore */
	kfree(fbuf);
	/* todo: if len >= 4KB, its caller dev_attr_show will returned bad count. */
	return len;
}
#endif

void hufs_parse_dev_desc(struct ufs_hba *hba,
				    struct ufs_dev_info *dev_info,
				    const u8 *desc_buf)
{
	dev_info->spec_version =
		get_unaligned_be16(&desc_buf[DEVICE_DESC_PARAM_SPEC_VER]);
	dev_info->wmanufacturer_date =
		get_unaligned_be16(&desc_buf[DEVICE_DESC_PARAM_MANF_DATE]);
	dev_info->serial_num_index = desc_buf[DEVICE_DESC_PARAM_SN];
	if (hba->desc_size[QUERY_DESC_IDN_DEVICE] >= DEVICE_DESC_PARAM_FEATURE)
		dev_info->vendor_feature = get_unaligned_be32(
			&desc_buf[DEVICE_DESC_PARAM_FEATURE]);

	hba->manufacturer_id = dev_info->wmanufacturerid;
	hba->manufacturer_date = dev_info->wmanufacturer_date;
	hba->ufs_device_spec_version = dev_info->spec_version;

	dev_err(hba->dev, "ufs spec version: 0x%x\n",
		hba->ufs_device_spec_version);

	if (hba->ufs_device_spec_version >= UFS_DEVICE_SPEC_3_1 ||
		hba->ufs_device_spec_version == UFS_DEVICE_SPEC_2_2) {
		hba->wb_type = desc_buf[DEVICE_DESC_PARAM_WB_TYPE];
		hba->wb_shared_alloc_units = get_unaligned_be32(
			desc_buf + DEVICE_DESC_PARAM_WB_SHARED_ALLOC_UNITS);
		dev_err(hba->dev, "write shared alloc units = 0x%x\n",
			hba->wb_shared_alloc_units);

		hba->d_ext_ufs_feature_sup = get_unaligned_be32(
			desc_buf + DEVICE_DESC_PARAM_EXT_UFS_FEATURE_SUP);
		dev_err(hba->dev, "d_ext_ufs_feature_sup = 0x%x\n",
			hba->d_ext_ufs_feature_sup);
	}
}

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
/**
* ufshcd_init_fsr_sys -init fsr attr node
* @hba: Pointer to adapter instance
* the node only to be read
*/
void ufshcd_init_fsr_sys(struct ufs_hba *hba)
{
	hba->ufs_fsr.fsr_attr.show = ufshcd_fsr_info_show;
	sysfs_attr_init(&hba->ufs_fsr.fsr_attr.attr);
	hba->ufs_fsr.fsr_attr.attr.name = "ufs_fsr";
	hba->ufs_fsr.fsr_attr.attr.mode = S_IRUSR | S_IRGRP;
	if (device_create_file(hba->dev, &hba->ufs_fsr.fsr_attr))
		dev_err(hba->dev, "Failed to create sysfs for ufs fsrs\n");
}

#ifdef CONFIG_PLATFORM_DIEID

#define CHIP_VER_OFFSET_950            12
#define MAX_DIE_NUM_950                32
#define FLASH_DIE_ID_OFFSET_950        1960
#define CONTROLLER_DIEID_OFFSET_950    1640
#define CRACK_NOW_OFFSET_950           1706
#define CRACK_EVER_OFFSET_950          1707
#define AR_UID_OFFSET_950          	   1800
#define ST_UID_MAP_OFFSET_950          1896

static void ufshcd_fill_950_dieid_buff(u8 *fbuf)
{
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + CHIP_VER_OFFSET_950, UFS_NAND_CHIP_VER_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET memcpy_s fail\n");
		return;
	}
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + CONTROLLER_DIEID_OFFSET_950, UFS_CONTROLLER_DIEID_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET memcpy_s fail\n");
		return;
	}
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + FLASH_DIE_ID_OFFSET_950, UFS_FLASH_DIE_ID_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET memcpy_s fail\n");
		return;
	}
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DCIP_CRACK_NOW_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + CRACK_NOW_OFFSET_950, UFS_DCIP_CRACK_NOW_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DCIP_CRACK_NOW_OFFSET memcpy_s fail\n");
		return;
	}
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DCIP_CRACK_EVER_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + CRACK_EVER_OFFSET_950, UFS_DCIP_CRACK_EVER_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DCIP_CRACK_EVER_OFFSET memcpy_s fail\n");
		return;
	}
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DIEID_AR_UID_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + AR_UID_OFFSET_950, UFS_AR_UID_TABLE_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DIEID_AR_UID_OFFSET memcpy_s fail\n");
		return;
	}
	if (memcpy_s(ufs_hixxxx_dieid + UFS_DIEID_ST_UID_MAP_OFFSET, THR950_DFA_FSR_INFO_SIZE,
		fbuf + ST_UID_MAP_OFFSET_950, UFS_ST_UID_MAP_SIZE) != EOK) {
		pr_err("ufs_hixxxx_dieid + UFS_DIEID_ST_UID_MAP_OFFSET memcpy_s fail\n");
		return;
	}
}

void ufshcd_ufs_set_dieid(struct ufs_hba *hba, struct ufs_dev_info *dev_info)
{
	/* allocate memory to hold full descriptor */
	u32 fsr_buf_len = HI1861_FSR_INFO_SIZE;
	u8 *fbuf = NULL;
	int ret = 0;

	if (hba->manufacturer_id != UFS_VENDOR_VELA61)
		return;

#ifdef CONFIG_SCSI_UFS_UNISTORE
	if (hba->host && hba->host->unistore_enable)
		return;
#endif
	if (ufs_hixxxx_dieid == NULL)
		ufs_hixxxx_dieid = kmalloc(UFS_DIEID_TOTAL_SIZE, GFP_KERNEL);
	if (!ufs_hixxxx_dieid)
		return;

	memset(ufs_hixxxx_dieid, 0, UFS_DIEID_TOTAL_SIZE);

	if (strncmp(hba->model, UFS_PRODUCT_NAME_THOR950_DFA, UFS_PRODUCT_NAME_THOR950_LEN) == 0) {
		fsr_buf_len = THR950_DFA_FSR_INFO_SIZE;
	}
	fbuf = kmalloc(fsr_buf_len, GFP_KERNEL);
	if (!fbuf) {
		kfree(ufs_hixxxx_dieid);
		ufs_hixxxx_dieid = NULL;
		return;
	}

	ret = memset_s(fbuf, fsr_buf_len, 0, fsr_buf_len);
	if (ret) {
		pr_err("%s memset_s failed\n", __func__);
		goto out;
	}
	ret = ufshcd_read_fsr_info(hba, 0, 0, fbuf, fsr_buf_len);
	if (ret) {
		is_fsr_read_failed = 1;
		dev_err(hba->dev, "[%s]READ FSR FAILED\n", __func__);
		goto out;
	}

	/* get ufs product name */
	ret = snprintf(ufs_product_name, UFS_PRODUCT_NAME_LEN, dev_info->model);
	if (ret <= 0) {
		dev_err(hba->dev, "[%s]copy ufs product name fail\n", __func__);
		goto out;
	}
	ret = strncmp(UFS_HIXXXX_PRODUCT_NAME, dev_info->model, HIXXXX_PROD_LEN);
	if (ret != 0) {
		/* 950 ver */
		if (strncmp(hba->model, UFS_PRODUCT_NAME_THOR950_DFA, UFS_PRODUCT_NAME_THOR950_LEN) == 0) {
			pr_err("enter ufshcd_fill_950_dieid_buff\n");
			ufshcd_fill_950_dieid_buff(fbuf);
			goto out;
		}
		/* after hi1861 ver. */
		memcpy(ufs_hixxxx_dieid, fbuf + 16, UFS_DIEID_NUM_SIZE_THOR920);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET,
			fbuf + 36, UFS_NAND_CHIP_VER_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET,
			fbuf + 256, UFS_CONTROLLER_DIEID_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET,
			fbuf + 448, UFS_FLASH_DIE_ID_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DCIP_CRACK_NOW_OFFSET,
			fbuf + 440, UFS_DCIP_CRACK_NOW_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DCIP_CRACK_EVER_OFFSET,
			fbuf + 441, UFS_DCIP_CRACK_EVER_SIZE);
	} else {
		/* hi1861 ver. */
		memcpy(ufs_hixxxx_dieid, fbuf + 12, UFS_DIEID_NUM_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET,
			fbuf + 28, UFS_NAND_CHIP_VER_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET,
			fbuf + 1692, UFS_CONTROLLER_DIEID_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET,
			fbuf + 1900, UFS_FLASH_DIE_ID_SIZE);
	}
out:
	kfree(fbuf);
}

int hufs_get_flash_dieid(
	char *dieid, u32 offset, u32 dieid_num, u8 vendor_id, u32 *flash_id)
{
	int len = 0;
	int i = 0;
	int j = 0;
	int flag = 0;
	int ret = 0;

	dieid += offset;
	/*
	 * T vendor flash id, the length is 32B.As is required,
	 * the output flash ids need to formatted in hex with appropriate prefix
	 * eg:\r\nDIEID_UFS_FLASH_B:0x00CD...\r\n
	 *    \r\nDIEID_UFS_FLASH_C:0xAC3D...\r\n
	 */
	/*lint -save -e574 -e679 */
	if (offset > UFS_DIEID_BUFFER_SIZE)
		return -EINVAL;

	if (((strncmp(ufs_product_name, UFS_PRODUCT_NAME_THOR920,
		      UFS_PRODUCT_NAME_LEN) == 0) ||
	     (strncmp(ufs_product_name, UFS_PRODUCT_NAME_SS6100,
		      UFS_PRODUCT_NAME_LEN) == 0)) &&
	    (vendor_id == UFS_FLASH_VENDOR_M)) {
		for (i = 0; i < dieid_num; i++) {
			if (dieid_num == UFS_FLASH_FOUR_DIE &&
			    (i == UFS_FLASH_TWO_DIE ||
			     i == UFS_FLASH_THREE_DIE)) {
				i += 2;
				flag = 1;
			}

			ret = snprintf(
				dieid + len,
				UFS_DIEID_BUFFER_SIZE - len - offset,
				"\r\nDIEID_UFS_FLASH_%c:0x%08X%08X%08X%08X00000000000000000000000000000000\r\n",
				'B' + j++, *(flash_id + i * 4),
				*(flash_id + i * 4 + 1),
				*(flash_id + i * 4 + 2),
				*(flash_id + i * 4 + 3));
			if (ret <= 0)
				return -2;
			len += ret;

			if (flag) {
				flag = 0;
				i -= 2;
			}
		}
	} else if ((vendor_id == UFS_FLASH_VENDOR_T) ||
		   (vendor_id == UFS_FLASH_VENDOR_H) ||
		   (vendor_id == UFS_FLASH_VENDOR_M) ||
		   (vendor_id == UFS_FLASH_VENDOR_Y)) {
		if (strncmp(ufs_product_name, UFS_PRODUCT_NAME_THOR950_DFA, UFS_PRODUCT_NAME_THOR950_LEN) == 0) {
		    return 0;
		}
		for (i = 0; i < dieid_num; i++) {
			ret = snprintf(
				dieid + len,
				UFS_DIEID_BUFFER_SIZE - len - offset,
				"\r\nDIEID_UFS_FLASH_%c:0x%08X%08X%08X%08X\r\n",
				'B' + i, *(flash_id + i * 4),
				*(flash_id + i * 4 + 1),
				*(flash_id + i * 4 + 2),
				*(flash_id + i * 4 + 3));
			if (ret <= 0)
				return -2;
			len += ret;
		}
	} else {
		return -2;
	}

	return 0;
}

int hufs_get_950_ar_uid(char *dieid, u32 offset, u32 *ar_uid) {
	int i;
    int ar_uid_total_len = 0;
    int ar_uid_len = 0;

    for (i = 0; i < UFS_AR_UID_TABLE_SIZE / UFS_AR_UID_STEP_SIZE; i++) {
        ar_uid_len = snprintf_s(dieid + ar_uid_total_len, UFS_DIEID_BUFFER_SIZE - ar_uid_total_len - offset,
			UFS_DIEID_BUFFER_SIZE - ar_uid_total_len - offset - 1,
			i == 0 ? "\r\nDIEID_UFS_AR_UID:0x%08X%08X%08X%08X" : "%08X%08X%08X%08X",
			*(ar_uid + i * UFS_AR_UID_STEP_NUN),
			*(ar_uid + i * UFS_AR_UID_STEP_NUN + UFS_DIEID_ARRAY_IDX_1),
			*(ar_uid + i * UFS_AR_UID_STEP_NUN + UFS_DIEID_ARRAY_IDX_2),
			*(ar_uid + i * UFS_AR_UID_STEP_NUN + UFS_DIEID_ARRAY_IDX_3));
        if (ar_uid_len <= 0)
            return UFS_DIEID_SNPRINTF_ERR;
        ar_uid_total_len += ar_uid_len;
    }
	ar_uid_len = snprintf_s(dieid + ar_uid_total_len, UFS_DIEID_BUFFER_SIZE - ar_uid_total_len - offset,
		UFS_DIEID_BUFFER_SIZE - ar_uid_total_len - offset - 1, "\r\n");
    if (ar_uid_len <= 0)
        return UFS_DIEID_SNPRINTF_ERR;
	ar_uid_total_len += ar_uid_len;
    return ar_uid_total_len;
}

int hufs_get_950_st_uid_map(char *dieid, u32 offset, u16 *st_uid_map) {
	int i;
    int st_uid_total_len = 0;
    int st_uid_len = 0;

	for (i = 0; i < UFS_ST_UID_MAP_SIZE / UFS_ST_UID_STEP_SIZE; i++) {
        st_uid_len = snprintf_s(dieid + st_uid_total_len, UFS_DIEID_BUFFER_SIZE - st_uid_total_len - offset,
			UFS_DIEID_BUFFER_SIZE - st_uid_total_len - offset - 1,
			i == 0 ? "\r\nDIEID_UFS_ST_UID_MAP:0x%04X%04X" : "%04X%04X",
            *(st_uid_map + i * UFS_ST_UID_STEP_NUN),
			*(st_uid_map + i * UFS_ST_UID_STEP_NUN + UFS_DIEID_ARRAY_IDX_1));
		if (st_uid_len <= 0)
			return UFS_DIEID_SNPRINTF_ERR;
		st_uid_total_len += st_uid_len;
    }
	st_uid_len = snprintf_s(dieid + st_uid_total_len, UFS_DIEID_BUFFER_SIZE - st_uid_total_len - offset,
		UFS_DIEID_BUFFER_SIZE - st_uid_total_len - offset - 1, "\r\n");
    if (st_uid_len <= 0)
        return UFS_DIEID_SNPRINTF_ERR;
	st_uid_total_len += st_uid_len;
    return st_uid_total_len;
}

int hufs_get_950_flash_dieid(char *dieid, u32 offset, u32 *flash_id) {
	int i;
    int flash_dieid_total_len = 0;
    int flash_dieid_len = 0;

	for (i = 0; i < UFS_FLASH_DIE_ID_SIZE / UFS_FLASH_DIEID_STEP_SIZE; i++) {
		flash_dieid_len = snprintf_s(dieid + flash_dieid_total_len,
			UFS_DIEID_BUFFER_SIZE - flash_dieid_total_len - offset,
			UFS_DIEID_BUFFER_SIZE - flash_dieid_total_len - offset - 1,
			i == 0 ? "\r\nDIEID_UFS_FLASH:0x%08X%08X%08X%08X" : "%08X%08X%08X%08X",
			*(flash_id + i * UFS_FLASH_DIEID_STEP_NUN),
			*(flash_id + i * UFS_FLASH_DIEID_STEP_NUN + UFS_DIEID_ARRAY_IDX_1),
			*(flash_id + i * UFS_FLASH_DIEID_STEP_NUN + UFS_DIEID_ARRAY_IDX_2),
			*(flash_id + i * UFS_FLASH_DIEID_STEP_NUN + UFS_DIEID_ARRAY_IDX_3));
		if (flash_dieid_len <= 0)
			return UFS_DIEID_SNPRINTF_ERR;
		flash_dieid_total_len += flash_dieid_len;
    }
	flash_dieid_len = snprintf_s(dieid + flash_dieid_total_len,
		UFS_DIEID_BUFFER_SIZE - flash_dieid_total_len - offset,
		UFS_DIEID_BUFFER_SIZE - flash_dieid_total_len - offset - 1, "\r\n");
    if (flash_dieid_len <= 0)
        return UFS_DIEID_SNPRINTF_ERR;
	flash_dieid_total_len += flash_dieid_len;
    return flash_dieid_total_len;
}

int hufs_get_950_dieid(char *dieid, u32 offset, u32 *ar_uid, u16 *st_uid_map, u32 *flash_id)
{
	int len = 0;
	int ar_uid_total_len = 0;
	int st_uid_total_len = 0;
	int flash_dieid_total_len = 0;

	if (offset > UFS_DIEID_BUFFER_SIZE)
		return -EINVAL;
	if (strncmp(ufs_product_name, UFS_PRODUCT_NAME_THOR950_DFA, UFS_PRODUCT_NAME_THOR950_LEN) != 0) {
		return 0;
	}
	dieid += offset;

    ar_uid_total_len = hufs_get_950_ar_uid(dieid, offset, ar_uid);
    if (ar_uid_total_len < 0)
        return ar_uid_total_len;
    len += ar_uid_total_len;

    st_uid_total_len = hufs_get_950_st_uid_map(dieid + len, offset + len, st_uid_map);
    if (st_uid_total_len < 0)
        return st_uid_total_len;
    len += st_uid_total_len;

    flash_dieid_total_len = hufs_get_950_flash_dieid(dieid + len, offset + len, flash_id);
    if (flash_dieid_total_len < 0)
        return flash_dieid_total_len;

	return 0;
}
#endif
#endif

#ifdef CONFIG_PLATFORM_DIEID
int hufs_get_dieid(char *dieid, unsigned int len)
{
#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
	int length = 0;
	int ret = 0;
	u32 dieid_num = 0;
	u8 vendor_id = 0;
	u32 *controller_id = NULL;
	u32 *flash_id = NULL;
	u32 *ar_uid = NULL;
    u16 *st_uid_map = NULL;
	u8 die_crack_now = 0;
	u8 die_crack_ever = 0;
	char buf[UFS_DIEID_BUFFER_SIZE] = {0};

	if (dieid == NULL || ufs_hixxxx_dieid == NULL)
		return -2;
	if (is_fsr_read_failed)
		return -1;

	dieid_num = *(u32 *)ufs_hixxxx_dieid;
	vendor_id = *(u8 *)(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET);
	controller_id = (u32 *)(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET);
	flash_id = (u32 *)(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET);
	ar_uid = (u32 *)(ufs_hixxxx_dieid + UFS_DIEID_AR_UID_OFFSET);
	st_uid_map = (u16 *)(ufs_hixxxx_dieid + UFS_DIEID_ST_UID_MAP_OFFSET);
	die_crack_now = *(u8 *)(ufs_hixxxx_dieid + UFS_DCIP_CRACK_NOW_OFFSET);
	die_crack_ever = *(u8 *)(ufs_hixxxx_dieid + UFS_DCIP_CRACK_EVER_OFFSET);

	ret = snprintf(buf, UFS_DIEID_BUFFER_SIZE,
			"\r\nDIEID_UFS_CONTROLLER_A:0x%08X%08X%08X%08X%08X%08X%08X%08X\r\n",
			*controller_id, *(controller_id + 1),         /* 1: array index */
			*(controller_id + 2), *(controller_id + 3),   /* 2, 3: array index */
			*(controller_id + 4), *(controller_id + 5),   /* 4, 5: array index */
			*(controller_id + 6), *(controller_id + 7));  /* 6, 7: array index */
	if (ret <= 0)
		return -2;
	length += ret;

	ret = hufs_get_flash_dieid(
		buf, length, dieid_num, vendor_id, flash_id);
	if (ret != 0)
		return ret;
	length = strlen(buf);

	ret = hufs_get_950_dieid(buf, length, ar_uid, st_uid_map, flash_id);
	if (ret != 0)
		return ret;
	length = strlen(buf);

	ret = snprintf(buf + length, UFS_DIEID_BUFFER_SIZE - length,
		"\r\nCRACK_NOW:0x%08X\r\n\r\nCRACK_EVER:0x%08X\r\n",
		die_crack_now, die_crack_ever);
	if (ret <= 0)
		return -2;

	if (len >= strlen(buf))
		strncat(dieid, buf, strlen(buf));
	else
		return strlen(buf);

	return 0;
#else
	return -1;
#endif
}
#endif

#ifdef CONFIG_SCSI_UFS_VELA61_VCMD
void ufshcd_fsr_dump_handler(struct work_struct *work)
{
	u32 fsr_buf_len = HI1861_FSR_INFO_SIZE;
	struct ufs_hba *hba;
	u8 *fbuf = NULL;
	int i = 0;
	int ret = 0;

	hba = container_of(work, struct ufs_hba, fsr_work);

	if (UFS_VENDOR_VELA61 != hba->manufacturer_id)
		return;
	/* allocate memory to hold full descriptor */
	if (strncmp(hba->model, UFS_PRODUCT_NAME_THOR950_DFA, UFS_PRODUCT_NAME_THOR950_LEN) == 0) {
		fsr_buf_len = THR950_DFA_FSR_INFO_SIZE;
	}
	fbuf = kmalloc(fsr_buf_len, GFP_KERNEL);
	if (!fbuf)
		return;
	ret = memset_s(fbuf, fsr_buf_len, 0, fsr_buf_len);
	if (ret) {
		kfree(fbuf);
		pr_err("%s memset_s failed\n", __func__);
		return;
	}
	ret = ufshcd_read_fsr(hba, fbuf, fsr_buf_len);
	if (ret) {
		kfree(fbuf);
		dev_err(hba->dev, "[%s]READ FSR FAILED\n", __func__);
		return;
	}
#ifdef CONFIG_DFX_DEBUG_FS
	dev_err(hba->dev, "===============UFS HI1861 FSR INFO===============\n");
#endif
	/*lint -save -e661 -e662*/
	for (i = 0 ; i < fsr_buf_len; i = i + 16)
		dev_err(hba->dev, "0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		*(fbuf + i + 0), *(fbuf + i + 1), *(fbuf + i + 2), *(fbuf + i + 3),
		*(fbuf + i + 4), *(fbuf + i + 5), *(fbuf + i + 6), *(fbuf + i + 7),
		*(fbuf + i + 8), *(fbuf + i + 9), *(fbuf + i + 10), *(fbuf + i + 11),
		*(fbuf + i + 12), *(fbuf + i + 13), *(fbuf + i + 14), *(fbuf + i + 15));
	/*lint -restore*/
	kfree(fbuf);
}
#endif

static ssize_t ufshcd_ufs_temp_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int len = 0;
	struct ufs_hba *hba = dev_get_drvdata(dev);
	hba->ufs_temp.temp = get_soc_temp() / 1000;
	if (hba->ufs_temp.temp <= 0) {
		dsm_ufs_update_error_info(hba, DSM_UFS_TEMP_LOW_ERR);
		schedule_ufs_dsm_work(hba);
	}
	dev_info(hba->dev, "UFS temp is %d\n", hba->ufs_temp.temp);
	len = snprintf(buf, PAGE_SIZE, "%d\n", hba->ufs_temp.temp);
	return len;
}

void ufshcd_init_ufs_temp_sys(struct ufs_hba *hba)
{
	hba->ufs_temp.temp_attr.show = ufshcd_ufs_temp_show;
	sysfs_attr_init(&hba->ufs_temp.temp_attr.attr);
	hba->ufs_temp.temp_attr.attr.name = "ufs_temp";
	hba->ufs_temp.temp_attr.attr.mode = S_IRUSR | S_IRGRP;
	if (device_create_file(hba->dev, &hba->ufs_temp.temp_attr))
		dev_err(hba->dev, "Failed to create sysfs for ufs_temp\n");
}

void ufshcd_print_host_regs(struct ufs_hba *hba)
{
	if (hba->vops && hba->vops->dbg_hci_dump)
		hba->vops->dbg_hci_dump(hba);
	if (ufshcd_is_hufs_hc(hba) && hba->vops &&
	    hba->vops->dbg_hufs_dme_dump)
		hba->vops->dbg_hufs_dme_dump(hba);
#ifdef CONFIG_HUFS_HC
	if (hba->vops && hba->vops->dbg_uic_dump)
		hba->vops->dbg_uic_dump(hba);
#endif
	/*
	 * hex_dump reads its data without the readl macro. This might
	 * cause inconsistency issues on some platform, as the printed
	 * values may be from cache and not the most recent value.
	 * To know whether you are looking at an un-cached version verify
	 * that IORESOURCE_MEM flag is on when xxx_get_resource() is invoked
	 * during platform/pci probe function.
	 */
	ufshcd_dump_regs(hba, 0, UFSHCI_REG_SPACE_SIZE, "host_regs: ");
	dev_err(hba->dev, "hba->ufs_version = 0x%x, hba->capabilities = 0x%x\n",
		hba->ufs_version, hba->capabilities);
	dev_err(hba->dev,
		"hba->outstanding_reqs = 0x%x, hba->outstanding_tasks = 0x%x\n",
		(u32)hba->outstanding_reqs, (u32)hba->outstanding_tasks);
#ifdef CONFIG_UFS_4_0_HC
	dev_err(hba->dev, "hba->mcq_inflt_reqs = %d\n", atomic_read(&hba->mcq_inflt_reqs));
	for (u32 i = 0; i < MCQ_OUTSTANDING_ITEMS; i++)
		dev_err(hba->dev, "hba->mcq_outstanding_reqs: idx[%d]-[%lld]\n", i, hba->mcq_outstanding_reqs[i]);
#endif
	dev_err(hba->dev,
		"last_hibern8_exit_tstamp at %lld us, hibern8_exit_cnt = %d\n",
		ktime_to_us(hba->ufs_stats.last_hibern8_exit_tstamp),
		hba->ufs_stats.hibern8_exit_cnt);

	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.pa_err, "pa_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.dl_err, "dl_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.nl_err, "nl_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.tl_err, "tl_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.dme_err, "dme_err");

	ufshcd_print_clk_freqs(hba);

	if (hba->vops && hba->vops->dbg_register_dump)
		hba->vops->dbg_register_dump(hba);
}

void ufshcd_print_gic(struct ufs_hba *hba)
{
	if (hba->vops && hba->vops->dbg_gic_dump)
		hba->vops->dbg_gic_dump(hba);
}
