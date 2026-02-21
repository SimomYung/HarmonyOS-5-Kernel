/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: kconfig helper macros
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 23 15:54:18 2022
 */
#ifndef KLIBS_KCONFIG_H
#define KLIBS_KCONFIG_H

#define __KCONFIG_COMMA_1	,
#define __kconfig_take_arg1(ignored, val, ...)	val
#define ___kconfig_is_defined(comma_or_junk)	__kconfig_take_arg1(comma_or_junk 1, 0)
#define __kconfig_is_defined(val)	___kconfig_is_defined(__KCONFIG_COMMA_##val)
/*
 * KCONFIG_IS_DEFINED(CONFIG_XXX) evaluates to true only if CONFIG_XXX is set to 'y'.
 *
 * if CONFIG_XXX is set to 'y', autoconf will generate #define CONFIG_XXX 1,
 * then (comma_or_junk 1, 0) will be expand to (, 1, 0), otherwise
 * (__KCONFIG_COMMA_CONFIG_XXX 1, 0).
 *
 * __kconfig_take_arg1 take the arg after the first comma.
 */
#define KCONFIG_IS_DEFINED(config)	(__kconfig_is_defined(config) == 1)

#endif
