/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to start the rf platform driver.
 */

#ifndef __RF_PLAT_DRIVER_H__
#define __RF_PLAT_DRIVER_H__

#include <linux/platform_device.h>
#include <linux/tty.h>

#define DTS_RF_COMP_NAME "huawei_rf"

#define GPIO_LOW 0
#define GPIO_HIGH 1
#define GPIO_DIRECTION_INPUT 0
#define GPIO_DIRECTION_OUTPUT 1

struct ps_device {
	char *dev_node_name;
	struct platform_device *pm_pdev;
	struct ps_core_s *ps_core;
};

struct ps_core_s {
	void *ps_plat;
	struct platform_device *pm_pdev;
	struct ps_device *hlct_dev;
	struct device *dev;
	struct device_node *dev_node;
	bool rsmc_ufs_lp_mode_support; //UFS 低功耗模式开关 for ICL

	int32_t sate_rsmc_gpio_mipi; // GPIO_321
	int32_t sate_rsmc_gps_spdt; // GPIO_321
	int32_t sate_rsmc_i2c_index; // index = 0
	int32_t sate_rsmc_i2c_on_volt;
	int32_t sate_rsmc_i2c_off_volt;
	int32_t sate_rsmc_pwr_ldo_vol; // LDO53
	int32_t sate_tts_i2c_pwr_sw; // GPIO 5 in iceland,GPIO 324 in Pi
	int32_t sate_tts_i2c_index; // index = 2
	int32_t sate_tts_i2c_on_volt; // 4600mV
	int32_t sate_tts_i2c_off_volt; // 3400mV
	int32_t sate_tts_pwr_ldo_vol; // LDO53
	int32_t rf_adc_channel_1;
	int32_t rf_adc_channel_2;
	int32_t sate_uefi_oled_en;
	int32_t sate_rsmc_bbst_gpio_en;
	int32_t sate_rsmc_wifi_fem_en;
	int32_t sate_rsmc_pwr_en;
	int32_t sate_tts_rsmc_sw;
	int32_t sate_tts_rf_bp_bst_en;
};

int32_t hw_rf_dev_init(void);
void hw_rf_dev_exit(void);
int32_t rf_init_gpio(int32_t gpio_id, char *gpio_name, int32_t status, int32_t gpio_direction);
int32_t rf_release_ioctl_proc(unsigned int cmd, int data);
int32_t rf_fac_ioctl_proc(unsigned int cmd, int data);


#endif