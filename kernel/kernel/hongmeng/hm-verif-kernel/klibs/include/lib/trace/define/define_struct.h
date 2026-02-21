/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define the main struct of the trace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 28 03:42:59 2024
 */
/*
 * This is the first file you need to include if you want to add a new tracepoint to
 * a new system service. This header file will define the key struct of the tracepoint.
 */
#undef __field
#define __field(type, item) type item;

#undef __array
#define __array(type, item, len) type item[len];

#undef __dynamic_array
#define __dynamic_array(type, item, len) __u32 __data_loc_##item;

#undef __string
#define __string(item, src) __dynamic_array(char, item, -1);
