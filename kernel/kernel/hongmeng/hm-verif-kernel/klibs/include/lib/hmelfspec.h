/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: Hongmeng specific elf related definitions
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 20 04:12:13 2022
 */

#ifndef KLIBS_HMELFSPEC_H
#define KLIBS_HMELFSPEC_H

/* OS-specific program segment types */
/* referenced from elf.h of linux source code */
/* hm-specific PT types should be placed in this range */
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6fffffff

/* segment of linear map */
#define PT_HM_LINEAR	0x60000001

/* segment of uapps */
#define PT_HM_UAPPS	0x60000002

/* segment of rootfs */
#define PT_HM_ROOTFS	0x60000003

/* segment of aslr info */
#define PT_HM_ASLR	0x60000004

/* segment of pie offset info */
#define PT_HM_PIE	0x60000005

/* mapping of ctrlmem */
#define PT_HM_CTRLMEM	0x60000006

/* mapping of pgstr table */
#define PT_HM_PGSTRTBL	0x60000007

/* segment of initrd */
#define PT_HM_INITRD	0x60000008

#endif
