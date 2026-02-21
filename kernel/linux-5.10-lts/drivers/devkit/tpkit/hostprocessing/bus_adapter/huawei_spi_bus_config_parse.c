/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "huawei_spi_bus_config_parse.h"
#include "huawei_spi_bus_platform.h"

#ifdef CONFIG_HUAWEI_THP_QCOM
void thp_qcom_parse_spi_config(struct device_node *spi_cfg_node,
	struct thp_core_data *cd)
{
	unsigned int value = 0;
	int rc;

	rc = of_property_read_u32(spi_cfg_node, "spi_cs_clk_delay", &value);
	if (!rc) {
		cd->qcom_spi_config.spi_cs_clk_delay = value;
		thp_log_info("%s:spi_cs_clk_delay configed %u",
			__func__, value);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "spi_inter_words_delay", &value);
	if (!rc) {
		cd->qcom_spi_config.spi_inter_words_delay = value;
		thp_log_info("%s:spi_inter_words_delay configed %u\n",
			__func__, value);
	}
}
#endif

void thp_of_property_read_spi_value(struct device_node *spi_cfg_node,
struct thp_core_data *cd,struct thp_spi_config *spi_config)
{
	int rc;
	unsigned int value;
	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "spi-max-frequency", &value);
	if (!rc) {
		spi_config->max_speed_hz = value;
		thp_log_info("%s:spi-max-frequency configed %d\n",
				__func__, value);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "spi-bus-id", &value);
	if (!rc) {
		spi_config->bus_id = (u8)value;
		thp_log_info("%s:spi-bus-id configed %d\n", __func__, value);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "spi-mode", &value);
	if (!rc) {
		spi_config->mode = value;
		thp_log_info("%s:spi-mode configed %d\n", __func__, value);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "bits-per-word", &value);
	if (!rc) {
		spi_config->bits_per_word = value;
		thp_log_info("%s:bits-per-word configed %d\n", __func__, value);
	}
}

void thp_of_property_read_pl022_comm_value(struct device_node *spi_cfg_node,
	struct thp_core_data *cd, struct pl022_config_chip *pl022_spi_config)
{
	int rc;
	unsigned int value;

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,interface", &value);
	if (!rc) {
		pl022_spi_config->iface = value;
		thp_log_info("%s: pl022,interface parsed\n", __func__);
	}
	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,com-mode", &value);
	if (!rc) {
		pl022_spi_config->com_mode = value;
		thp_log_info("%s:com_mode parsed\n", __func__);
	}
	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,rx-level-trig", &value);
	if (!rc) {
		pl022_spi_config->rx_lev_trig = value;
		thp_log_info("%s:rx-level-trig parsed\n", __func__);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,tx-level-trig", &value);
	if (!rc) {
		pl022_spi_config->tx_lev_trig = value;
		thp_log_info("%s:tx-level-trig parsed\n", __func__);
	}
}

void thp_of_property_read_pl022_value(struct device_node *spi_cfg_node,
	struct thp_core_data *cd, struct pl022_config_chip *pl022_spi_config)
{
	int rc;
	unsigned int value;

	thp_of_property_read_pl022_comm_value(spi_cfg_node, cd, pl022_spi_config);

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,ctrl-len", &value);
	if (!rc) {
		pl022_spi_config->ctrl_len = value;
		thp_log_info("%s:ctrl-len parsed\n", __func__);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,wait-state", &value);
	if (!rc) {
		pl022_spi_config->wait_state = value;
		thp_log_info("%s:wait-state parsed\n", __func__);
	}

	value = 0;
	rc = of_property_read_u32(spi_cfg_node, "pl022,duplex", &value);
	if (!rc) {
		pl022_spi_config->duplex = value;
		thp_log_info("%s:duplex parsed\n", __func__);
	}
}

#ifdef CONFIG_HUAWEI_THP_MTK
void thp_mtk_assignment(struct thp_core_data *cd,
	struct thp_spi_config *spi_config,struct pl022_config_chip *pl022_spi_config)
{
	/* tx ordering, 1-msb first send; 0-lsb first end */
	cd->mtk_spi_config.rx_mlsb = 1;
	/* rx ordering, 1-msb first send; 0-lsb first end */
	cd->mtk_spi_config.tx_mlsb = 1;
	/* control cs polarity, 0-active low; 1-active high */
	cd->mtk_spi_config.cs_pol = 0;
	/* control sample edge, 0-positive edge; 1-negative edge */
	cd->mtk_spi_config.sample_sel = 0;
	/* cs setup time, 0-default time */
	cd->mtk_spi_config.cs_setuptime = 0;	
}
#endif

void thp_set_default_value(struct thp_core_data *cd)
{
	if (cd->panel_id == MAIN_TOUCH_PANEL)
		cd->spi_config.pl022_spi_config.cs_control = thp_spi_cs_set;
	else if (cd->panel_id == SUB_TOUCH_PANEL)
		cd->spi_config.pl022_spi_config.cs_control = thp_spi_cs_set_sub_panel;
	cd->spi_config.pl022_spi_config.hierarchy = SSP_MASTER;

	if (!cd->spi_config.max_speed_hz)
		cd->spi_config.max_speed_hz = THP_SPI_SPEED_DEFAULT;
	if (!cd->spi_config.mode)
		cd->spi_config.mode = SPI_MODE_0;
	if (!cd->spi_config.bits_per_word)
	/* spi_config.bits_per_word default value is 8 */
		cd->spi_config.bits_per_word = 8;
}

void thp_set_sdev_value(struct thp_spi_config *spi_config, struct thp_core_data *cd)
{
	cd->sdev->mode = spi_config->mode;
	cd->sdev->max_speed_hz = spi_config->max_speed_hz;
	cd->sdev->bits_per_word = spi_config->bits_per_word;
#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
	cd->sdev->controller_data = &spi_config->pl022_spi_config;
#endif
#ifdef CONFIG_HUAWEI_THP_MTK
	cd->sdev->controller_data = (void *)&(cd->mtk_spi_config);
#endif
#ifdef CONFIG_HUAWEI_THP_QCOM
	cd->sdev->controller_data = (void *)&(cd->qcom_spi_config);
#endif
}

int thp_parse_spi_config(struct device_node *spi_cfg_node,
	struct thp_core_data *cd)
{
	struct thp_spi_config *spi_config = NULL;
	struct pl022_config_chip *pl022_spi_config = NULL;

	if (!spi_cfg_node || !cd) {
		thp_log_info("%s: input null\n", __func__);
		return -ENODEV;
	}

	spi_config = &cd->spi_config;
	pl022_spi_config = &cd->spi_config.pl022_spi_config;

	thp_of_property_read_spi_value(spi_cfg_node, cd, spi_config);
	thp_of_property_read_pl022_value(spi_cfg_node, cd, pl022_spi_config);
#ifdef CONFIG_HUAWEI_THP_QCOM
	thp_qcom_parse_spi_config(spi_cfg_node, cd);
#endif
	thp_set_default_value(cd);
#ifdef CONFIG_HUAWEI_THP_MTK
	thp_mtk_assignment(cd, spi_config, pl022_spi_config);
#endif
	thp_set_sdev_value(spi_config, cd);
	return 0;
}
EXPORT_SYMBOL(thp_parse_spi_config);