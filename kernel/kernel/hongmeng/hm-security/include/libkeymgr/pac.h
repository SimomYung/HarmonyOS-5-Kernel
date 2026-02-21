/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: keymgr head file for PAC
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 29 18:03:46 2024
 */
#ifndef LIBKEYMGR_PAC_H
#define LIBKEYMGR_PAC_H

#ifdef PAC_DFI_PTR
#define __sec_keymgr_ptr ___pac_ptr
#else
#define __sec_keymgr_ptr
#endif
#ifdef PAC_DFI_DATA
#define __sec_keymgr_data ___pac_data
#else
#define __sec_keymgr_data
#endif
#ifdef PAC_DFI_SYSMGR

#define ___sec_keymgr_cntx __sec_keymgr_data
#define ___sec_keymgr_ptr  __sec_keymgr_ptr

#else

#define ___sec_keymgr_cntx
#define ___sec_keymgr_ptr

#endif

#ifdef PAC_CFI_FUNC_TABLE_SYSMGR
#define ___sec_keymgr_func_table_ptr ___pac_func_table_ptr
#else
#define ___sec_keymgr_func_table_ptr
#endif

#endif
