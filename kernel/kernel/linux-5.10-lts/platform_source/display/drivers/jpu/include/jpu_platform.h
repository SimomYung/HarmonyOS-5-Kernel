/*
 * jpegdec paltform
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

#ifndef JPU_PLATFORM_H
#define JPU_PLATFORM_H

#include "jpu/jpu_base_address.h"
#include "soc_iomcu_baseaddr_interface.h"

#ifdef CONFIG_JPU_JPEGD
#include "soc_media2_ctrl_interface.h"
#include "soc_media2_crg_interface.h"
#include "soc_smmuv3_tbu_interface.h"
#include "soc_smmuv3_tcu_interface.h"
#include "soc_isp_nmanager_sec_adpt_interface.h"
#include <platform_hiiom7.h>
#endif

#endif /* JPGDEC_PLATFORM_H */