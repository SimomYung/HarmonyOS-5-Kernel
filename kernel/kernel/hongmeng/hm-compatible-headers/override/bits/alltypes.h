/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 07 15:04:16 2022
 */
#include <muslorigin/bits/alltypes.h>

/*
 * To adapt to GLIBC,
 * for sonme headers of GLIBC include the following header,
 * and these headers in musl include bits/alltypes.h,
 * so let bits/alltypes.h includes the following header.
 */
#include <bits/types.h>
