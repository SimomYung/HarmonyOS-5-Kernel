/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ebr singleton
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 29 13:55:00 2020
 */
#include <libhmebr/ebr_singleton.h>

/* Define an ebr singleton for process */
static struct epoch_set es_singleton = EBR_INIT(es_singleton);

/* Marks the beginning of a critical section */
int ebr_mono_read_lock(void)
{
	return ebr_read_lock(&es_singleton);
}

/* Marks the end of a critical section */
int ebr_mono_read_unlock(void)
{
	return ebr_read_unlock(&es_singleton);
}

int synchronize_ebr_mono(void)
{
	return synchronize_ebr(&es_singleton);
}

int call_ebr_mono(struct ebr_entry *entry, void (*cb_func)(struct ebr_entry *ebr_entry))
{
	return call_ebr(&es_singleton, entry, cb_func);
}
