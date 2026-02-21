/* SPDX-License-Identifier: GPL-2.0 */
/*
 * lcd_kit_hybrid_aod.h
 *
 * head file for aod control
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
#ifndef LCD_KIT_HYBRID_AOD_H
#define LCD_KIT_HYBRID_AOD_H

#include "lcd_kit_hybrid_control.h"

#define ALPM_STATE_IN  1
#define ALPM_STATE_OUT 0

/*
 * go to alpm high backlight mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_on_high_light(struct display_hybrid_ctrl *display_ctrl);

/*
 * go to alpm low backlight mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_on_low_light(struct display_hybrid_ctrl *display_ctrl);

/*
 * go to alpm middle backlight mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_on_middle_light(struct display_hybrid_ctrl *display_ctrl);

/*
 * go to alpm hbm backlight mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_on_hbm_light(struct display_hybrid_ctrl *display_ctrl);

/*
 * go to alpm no backlight mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_on_no_light(struct display_hybrid_ctrl *display_ctrl);

/*
 * change to send frames mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_send_frame(struct display_hybrid_ctrl *display_ctrl);

/*
 * change to no-send frames mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_stop_send_frame(struct display_hybrid_ctrl *display_ctrl);

/*
 * exit aod mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_exit(struct display_hybrid_ctrl *display_ctrl);

/*
 * enter aod doze mode
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_doze(struct display_hybrid_ctrl *display_ctrl);

/*
 * enter aod doze_suspend mode, this mode is same as: doze + disable TE
 * @param display_ctrl the display control
 * @return >= 0 for success, otherwise fail
 */
int alpm_doze_suspend(struct display_hybrid_ctrl *display_ctrl);

/*
 * get AOD state
 * @param display_ctrl the display control
 * @return 1 for ap in aod mode & ap hold control, otherwise 0
 */
int alpm_get_state(void);

/*
 * set AOD state without read register
 * @param flag: 0-Not AOD, 1-AOD
 * @return NA
 */
void alpm_set_state_flag(int flag);

/*
 * set AOD state
 * @param display_ctrl the display control
 * @return NA
 */
void alpm_set_state(struct display_hybrid_ctrl *display_ctrl);

/*
 * enter deep sleep mode
 * @param display_ctrl the display control
 * @return = 0 for success, otherwise fail
 */
int lcd_kit_enter_deep_sleep(struct display_hybrid_ctrl *display_ctrl);

#endif
