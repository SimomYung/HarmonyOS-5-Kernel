/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Definition for hongmeng native object api
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 04 18:10:19 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_STAT_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_STAT_H
#include <sys/stat.h>

/*
 * Native objects include path, mqueue and named shm. The flag of mode
 * supported as below:
 *   - S_ISYS_SRVC flag suggests that objects only can be accessed by
 *     system service.
 *
 * This will be used with mode of RWX form 0000 to 0777 which is defined
 * in libc file sys/stat.h. But MISRA rule 7.1 suggests octal constants
 * shall not be used, so we use hex here.
 */
#define S_ISYS_SRVC 0x10000

#endif
