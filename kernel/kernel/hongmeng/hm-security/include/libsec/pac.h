/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header about PAC
 * Author: Huawei OS Kernel Lab
 */

#ifndef ULIBS_LIBSEC_PAC_H
#define ULIBS_LIBSEC_PAC_H

#ifdef PAC_DFI_PTR
#define __sec_libsec_ptr ___pac_ptr
#else
#define __sec_libsec_ptr
#endif
#ifdef PAC_DFI_DATA
#define __sec_libsec_data ___pac_data
#else
#define __sec_libsec_data
#endif
#ifdef PAC_DFI_SYSMGR
/*
 * Data types in libsec that require integrity protection:
 * ___sec_libsec_ptr: pointer to sensitive info,
 * ___sec_libsec_cntx: context related to permissions and access control.
 */
#define ___sec_libsec_ptr __sec_libsec_ptr
#define ___sec_libsec_cntx __sec_libsec_data
#else
#define ___sec_libsec_ptr
#define ___sec_libsec_cntx
#endif

#ifdef PAC_CFI_FUNC_TABLE_SYSMGR
#define ___sec_libsec_func_table_ptr ___pac_func_table_ptr
#else
#define ___sec_libsec_func_table_ptr
#endif

#endif
