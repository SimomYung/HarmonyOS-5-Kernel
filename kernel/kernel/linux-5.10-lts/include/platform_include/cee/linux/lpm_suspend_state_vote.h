/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description : information about lpm suspend
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __LPM_SUSPEND_STATE_VOTE_H__
#define __LPM_SUSPEND_STATE_VOTE_H__

/*
 * driver can vote when suspend
 * ID_USB: USB derver
 * ID_MUX: max num
 */
enum {
	ID_USB = 0,
	ID_MUX,
};
#ifdef CONFIG_SR_STAT_VOTE_IN_KERNEL
s32 lpm_suspend_state_vote(u32 modid, u32 val);
#else
static inline s32 lpm_suspend_state_vote(u32 modid, u32 val) { return 0; }
#endif
#endif
