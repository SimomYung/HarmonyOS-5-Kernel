/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure hash table share memory head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:50:14 2019
 */
#ifndef ULIBS_LIBSHTABLE_SHM_H
#define ULIBS_LIBSHTABLE_SHM_H

#include <stddef.h>

struct sht_shm;
struct shtable;

int sht_shm_open(const struct shtable *sht, struct sht_shm *shm,
		 const char *name, size_t element_size);
void sht_shm_close(const struct shtable *sht, struct sht_shm *shm);

#endif /* ifndef ULIBS_LIBSHTABLE_SHM_H */
