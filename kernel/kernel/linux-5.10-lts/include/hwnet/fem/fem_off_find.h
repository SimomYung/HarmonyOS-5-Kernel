/*
 * fem_off_find.h
 *
 * fem header
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
 *
 */

#ifndef FEM_OFF_FIND_H
#define FEM_OFF_FIND_H

#include <linux/device.h>

#define DTS_PROP_PMU_FEM_SWITCH1_REGULATOR "huawei,fem_switch1"
#define DTS_PROP_PMU_FEM_SWITCH1_VOLTAGE "huawei,fem_switch1_voltage"
#define DTS_PROP_PMU_FEM_SWITCH2_REGULATOR "huawei,fem_switch2"
#define DTS_PROP_PMU_FEM_SWITCH2_VOLTAGE "huawei,fem_switch2_voltage"
#define FEM_SWITCH1_VOLTAGE_MIN 1650000
#define FEM_SWITCH1_VOLTAGE_TYP 2850000
#define FEM_SWITCH1_VOLTAGE_MAX 3300000
#define FEM_SWITCH2_VOLTAGE_MIN 1650000
#define FEM_SWITCH2_VOLTAGE_TYP 1800000
#define FEM_SWITCH2_VOLTAGE_MAX 1950000


struct dc_characteristics {
	char *id;
	uint32_t const_vout_min;
	uint32_t const_vout_typ;
    uint32_t const_vout_max;
};
#endif