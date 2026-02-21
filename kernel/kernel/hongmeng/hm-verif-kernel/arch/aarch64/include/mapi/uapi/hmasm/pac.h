/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: header about PAC
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 23 18:03:02 2024
 */
#ifndef UAPI_A64_ASM_PAC_H
#define UAPI_A64_ASM_PAC_H
 
#ifdef __ASSEMBLY__
 
#ifdef CONFIG_PAC
 
.macro pac_bwcfi_sign
#ifdef CONFIG_PAC_BWCFI_IB_KEY
	pacib x30, sp
#else
	pacia x30, sp
#endif
.endm
 
.macro pac_bwcfi_retaut
#ifdef CONFIG_PAC_BWCFI_IB_KEY
	retab
#else
	retaa
#endif
.endm
 
#else /* CONFIG_PAC */
 
.macro pac_bwcfi_sign
.endm
.macro pac_bwcfi_retaut
.endm
 
#endif /* CONFIG_PAC */
 
#endif /* __ASSEMBLY__ */
#endif
