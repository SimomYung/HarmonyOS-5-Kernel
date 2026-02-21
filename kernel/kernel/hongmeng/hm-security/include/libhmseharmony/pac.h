/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: seharmony head file for PAC
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef LIBHMSEHARMONY_PAC_H
#define LIBHMSEHARMONY_PAC_H
 
#ifdef PAC_DFI_PTR
#define __sec_seharmony_ptr ___pac_ptr
#else
#define __sec_seharmony_ptr
#endif
#ifdef PAC_DFI_DATA
#define __sec_seharmony_data ___pac_data
#else
#define __sec_seharmony_data
#endif
#ifdef PAC_DFI_SEHARMONY
 
#define ___sec_seharmony_cntx __sec_seharmony_data
#define ___sec_seharmony_ptr  __sec_seharmony_ptr
 
#else
 
#define ___sec_seharmony_cntx
#define ___sec_seharmony_ptr
 
#endif

#ifdef PAC_CFI_FUNC_TABLE_SEHARMONY
#define ___sec_seharmony_func_table_ptr ___pac_func_table_ptr
#else
#define ___sec_seharmony_func_table_ptr
#endif

#endif
