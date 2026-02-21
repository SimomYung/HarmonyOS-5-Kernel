/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022.
 * Description: Interface of modules management utils
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 18 11:33:15 2022
 */
#ifndef __KMOD_H__
#define __KMOD_H__

#define KMOD_OK		0
#define KMOD_DONE	1
#define KMOD_NONE	2

int dmctl_handle_kmod_alias(int argc, char *argv[], int *ret);

#endif
