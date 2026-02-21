/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for route operation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#ifndef __UX_VISP_ROUTE_H__
#define __UX_VISP_ROUTE_H__

int visp_get_route_info_procfs(char **msg, int *msg_len, int type);
int visp_get_route_info_nl(void **buf, size_t *len);

#endif
