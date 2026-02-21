 /*
  * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
  * Description: definition for brk no.
  * Author: Huawei OS Kernel Lab
  * Create: Fri Dec 27 16:19:10 2024
  */
#ifndef AARCH64_UAPI_ASM_DEBUG_H
#define AARCH64_UAPI_ASM_DEBUG_H

#define BRK_KDP_CHECKFAIL	0xdd00
#define BRK_PANIC		0xdead
#define BRK_PANIC_TRAP_PRE	0xbeae
#define BRK_PANIC_TRAP		0xbeaf

#define __BRK_STR(x)		#x
#define __ASM_IMM(x)		"#" __BRK_STR(x)

#define ASM_IMM(x)		__ASM_IMM(x)
#endif
