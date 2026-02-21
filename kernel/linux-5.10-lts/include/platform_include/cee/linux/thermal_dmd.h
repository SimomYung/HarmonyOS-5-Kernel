/* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved. */
#ifndef __THERMAL_DMD_H_
#define __THERMAL_DMD_H_

#include <linux/types.h>
#include <linux/of.h>

#define BUFFER_MAX_LENGTH	1024

int parse_tempinfo_from_rdr(const void *bbox_base, size_t size, char *out_buf, size_t buf_size);
void parse_temp_dmd_parameter(const struct device_node *np);

#endif
