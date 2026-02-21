/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Naming conversion in spincall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 06:04:11 2025
 */

#ifndef HMKERNEL_CTRLMEM_SPINCALLENTRY_NAMING_H
#define HMKERNEL_CTRLMEM_SPINCALLENTRY_NAMING_H

#define SPINCALLENTRY_SYMNAME(funcname)        __spincallentry_##funcname
#define SPINCALLENTRY_CFNNAME(funcname)        __spincallentry_cfunc_##funcname
#define SPINCALLENTRY_BDYNAME(funcname)        __spincallentry_funcbody_##funcname
#define SPINCALLENTRY_MIDNAME(funcname, layer) __spincallentry_mid_##layer##_##funcname

#endif
