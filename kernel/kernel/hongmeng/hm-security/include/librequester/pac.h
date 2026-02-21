/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: ******
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 11 15:58:27 2025
 */

#ifndef ULIBS_LIBREQUESTER_PAC_H
#define ULIBS_LIBREQUESTER_PAC_H

#ifdef PAC_DFI_PTR
#define __sec_requester_ptr ___pac_ptr
#else
#define __sec_requester_ptr
#endif
#ifdef PAC_DFI_DATA
#define __sec_requester_data ___pac_data
#else
#define __sec_requester_data
#endif
#ifdef PAC_DFI_SYSMGR

#define ___sec_requester_cntx __sec_requester_data
#define ___sec_requester_ptr  __sec_requester_ptr

#else

#define ___sec_requester_cntx
#define ___sec_requester_ptr

#endif

#ifdef PAC_CFI_FUNC_TABLE_SYSMGR
#define ___sec_requester_func_table_ptr ___pac_func_table_ptr
#else
#define ___sec_requester_func_table_ptr
#endif

#endif
