/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony head file for network interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_EXPORT_NETWORK_H
#define HMSEL_EXPORT_NETWORK_H

#include <libhmseharmony/services.h>

hmsel_class_t socket_type_to_sclass(int domain, int type, int protocol);

#endif
