/*
 * Linux kernel modules for MT380X FlightSense TOF sensor
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef MT380X_API_H
#define MT380X_API_H
#include "mt380x.h"

mt380x_error mt380x_power_on(mt380x_dev dev);
mt380x_error mt380x_power_off(mt380x_dev dev);
mt380x_error mt380x_set_period(mt380x_dev dev, uint32_t period);
mt380x_error mt380x_single_measure(mt380x_dev dev);
mt380x_error mt380x_begin_continuous_measure(mt380x_dev dev);
mt380x_error mt380x_end_continuous_measure(mt380x_dev dev);
mt380x_error mt380x_get_measure_datas(mt380x_dev dev);
mt380x_error mt380x_get_and_clear_interrupt_state(mt380x_dev dev, uint8_t *stat);
mt380x_error mt380x_init(mt380x_dev dev);
mt380x_error mt380x_begin_crosstalk_calibration(mt380x_dev dev);
mt380x_error mt380x_get_crosstalk_params(mt380x_dev dev);
mt380x_error mt380x_config_crosstalk_params(mt380x_dev dev);
mt380x_error mt380x_begin_offset_calibration(mt380x_dev dev, uint32_t param);
mt380x_error mt380x_config_offset_params(mt380x_dev dev);
mt380x_error mt380x_read_chip_version(mt380x_dev dev);
void mt380x_read_fw_version(mt380x_dev dev);

#endif
