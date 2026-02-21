/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Implementation records the page_fault status.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sept 7 14:00:00 2024
 */
#ifndef SYSMGR_MEM_PAGE_FAULT_MARK_H
#define SYSMGR_MEM_PAGE_FAULT_MARK_H

#ifdef CONFIG_PAGE_FAULT_MARK
void page_fault_mark_set(void);
void page_fault_mark_clear(void);
bool is_page_fault_marked(void);
#else
static inline void page_fault_mark_set(void)
{
}

static inline void page_fault_mark_clear(void)
{
}

static inline bool is_page_fault_marked(void)
{
	return false;
}
#endif

#endif /* SYSMGR_MEM_PAGE_FAULT_MARK_H */
