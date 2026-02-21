/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to proc satelite related ctrl.
 */

#ifndef __RF_SATELLITE_CTRL_H__
#define __RF_SATELLITE_CTRL_H__
#define HKADC_DATA_GRADE0 0
#define HKADC_DATA_GRADE1 100
#define HKADC_DATA_GRADE2 300
#define HKADC_DATA_GRADE3 500
#define HKADC_DATA_GRADE4 700
#define HKADC_DATA_GRADE5 900
#define HKADC_DATA_GRADE6 1100
#define HKADC_DATA_GRADE7 1300
#define HKADC_DATA_GRADE8 1500
#define HKADC_DATA_GRADE9 1700
#define HKADC_DATA_GRADE10 1800
#define BOARDID_VALUE0 0x0
#define BOARDID_VALUE1 0x1
#define BOARDID_VALUE2 0x2
#define BOARDID_VALUE3 0x3
#define BOARDID_VALUE4 0x4
#define BOARDID_VALUE5 0x5
#define BOARDID_VALUE6 0x6
#define BOARDID_VALUE7 0x7
#define BOARDID_VALUE8 0x8
#define BOARDID_VALUE9 0x9
#define BOARDID_UNKNOWN 0xF


#include "rf_plat_driver.h"

int32_t rsmc_on_proc(struct ps_core_s *cd);
int32_t rsmc_off_proc(struct ps_core_s *cd);
int32_t tts_on_proc(struct ps_core_s *cd);
int32_t tts_off_proc(struct ps_core_s *cd);
int32_t rf_sateliete_parse_dts_config(struct device_node *dev_node,
	struct ps_core_s *cd);
int32_t rf_satelite_config_init(struct ps_core_s *cd);

extern int32_t lpm_adc_get_value(int adc_channel);
int32_t get_hkadc_value(unsigned int cmd, int channel);
int32_t get_rf_boardid(struct ps_core_s *cd);

#ifdef CONFIG_SCSI_UFS_RSMC
extern void ufshcd_auto_hibern8_open(void);
extern void ufshcd_auto_hibern8_close(void);
#endif

#endif