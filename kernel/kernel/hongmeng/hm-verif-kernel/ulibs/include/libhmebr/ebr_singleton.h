/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declarations for ebr singleton
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 29 13:54:30 2020
 */
#ifndef ULIBS_LIBHMEBR_EBR_SINGLETON_H
#define ULIBS_LIBHMEBR_EBR_SINGLETON_H

#include <libhmebr/ebr.h>

/* Basic APIs for ebr singleton */
int ebr_mono_read_lock(void);
int ebr_mono_read_unlock(void);
int synchronize_ebr_mono(void);
int call_ebr_mono(struct ebr_entry *entry, void (*cb_func)(struct ebr_entry *));

#endif
