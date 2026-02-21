/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: bbox_diaginfo_lpmcu_msg_map.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */
#ifndef __LPMCU_DIAGINFO_MAP
#define __LPMCU_DIAGINFO_MAP(x,y,z)
#endif
#include 	<bbox_diaginfo_id_def.h>

// X: fault id, Y:message parameter size, Z: string stored into diaginfo
__LPMCU_DIAGINFO_MAP(LPM3_DDR_FAIl, 3, "PLL lock fail:")
__LPMCU_DIAGINFO_MAP(ACPU_LIT_AVS_VAL, 6, "CPU_LIT_AVS:")
__LPMCU_DIAGINFO_MAP(ACPU_MID_AVS_VAL, 6, "CPU_MID_AVS:")
__LPMCU_DIAGINFO_MAP(ACPU_BIG_AVS_VAL, 6, "CPU_BIG_AVS:")
__LPMCU_DIAGINFO_MAP(LPM3_PERI_PA_SENSOR, 5, "PERI_PASENSOR:")
__LPMCU_DIAGINFO_MAP(DMD_PERI_AVS_VAL, 6, "PERI_AVS:")
__LPMCU_DIAGINFO_MAP(LPM3_DDR_PA_SENSOR, 6, "PASENSOR:")
__LPMCU_DIAGINFO_MAP(LPM3_DDR_TRAINING, 6, "TRAINING:")
__LPMCU_DIAGINFO_MAP(LPM3_DDR_AVS_VAL, 6, "AVSVAL:")
__LPMCU_DIAGINFO_MAP(LOWPOWER_GPU_AVS_VAL, 6, "GPU_AVS:")
__LPMCU_DIAGINFO_MAP(LOWPOWER_GPU_PA_SENSOR, 6, "GPU_PASENSOR:")
__LPMCU_DIAGINFO_MAP(MSPC_DIAG_INFO, 6, "CRYPTO_CORE:")
