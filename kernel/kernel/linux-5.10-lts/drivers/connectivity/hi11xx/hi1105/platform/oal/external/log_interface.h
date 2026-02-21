/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:lpcpu_feature.h header file
 * Author: @CompanyNameTag
 */

#ifndef LOG_INTERFACE_H
#define LOG_INTERFACE_H

#if (defined(CONFIG_LOG_EXCEPTION) || defined(CONFIG_LOG_USERTYPE)) && \
    !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#include <log/log_usertype.h>
#endif
#include "plat_debug.h"
#include "board.h"

static inline int32_t extern_get_logusertype_flag(void)
{
#if (defined(CONFIG_LOG_EXCEPTION) || defined(CONFIG_LOG_USERTYPE)) && \
    !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
    if (get_logusertype_flag() == BETA_USER) {
        /* beta user */
        ps_print_dbg("beta user\n");
        return MPXX_VTYPE_RELEASE_DEBUG;
    } else {
        /* release(user & non beta user) */
        ps_print_dbg("release user\n");
        return MPXX_VTYPE_RELEASE;
    }
#elif (defined(_PRE_PRODUCT_ID_E5))
#if defined(MBB_BUILD_DEBUG)
    /* kernel module or no beta user function */
    ps_print_dbg("default user\n");
    return MPXX_VTYPE_DEBUG;
#else
    /* release(user & non beta user) */
    ps_print_dbg("release user\n");
    return MPXX_VTYPE_RELEASE;
#endif
#else
    /* kernel module or no beta user function */
    ps_print_dbg("default user\n");
    return MPXX_VTYPE_DEBUG;
#endif
}

#endif // __LOG_INTERFACE_H__
