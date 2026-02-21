/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Hmcrt
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 21 09:00:17 2019
 */
#ifndef ULIBS_LIBCRT_HMCRT_H
#define ULIBS_LIBCRT_HMCRT_H

/* defined in libcrt/hook.c */
#include <libhmsrv_sys/hmsrv_sys.h>

#define AUX_CNT 38U

extern struct dso_sym_info *dso_syminfo;

extern size_t *hm_crt_current_aux(void);
extern void hm_setup_vdso_aux(uintptr_t base);
extern uint32_t hm_crt_cnode_idx_of(void);
extern uint32_t hm_crt_tcb_inner_idx_of(void);
extern void hm_crt_set_cnode_idx(uint32_t cnode);
extern void hm_crt_set_tcb_inner_idx(uint32_t inner);
#endif
