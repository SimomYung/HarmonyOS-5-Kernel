/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: oal_ktime.h header file
 * Author: @CompanyNameTag
 */

#ifndef OAL_KTIME_H
#define OAL_KTIME_H
#include "oal_types.h"
#include "oal_util.h"

OAL_STATIC OAL_INLINE uint64_t oal_ktime_get_us(ktime_t start_time)
{
    uint64_t total_time;
    ktime_t  end_time, trans_time;

    end_time = ktime_get();

    trans_time = ktime_sub(end_time, start_time);
    total_time = (uint64_t)ktime_to_us(trans_time);

    return total_time;
}

OAL_STATIC OAL_INLINE uint64_t oal_ktime_get_ms(ktime_t start_time)
{
    uint64_t total_time;
    ktime_t  end_time, trans_time;

    end_time = ktime_get();

    trans_time = ktime_sub(end_time, start_time);
    total_time = (uint64_t)ktime_to_ms(trans_time);

    return total_time;
}

#endif /* end of oal_ktime.h */
