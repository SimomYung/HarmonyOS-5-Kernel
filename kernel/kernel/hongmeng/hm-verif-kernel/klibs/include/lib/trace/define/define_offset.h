/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define the struct to record offset
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 28 03:42:59 2024
 */

/*
 * This is the second file you need to include if you want to add a new tracepoint to a
 * new system service. This header file will define the struct which serving for dynamic_array
 * of the tracepoint. If the event exists dynamic_array, this struct will record the offset of
 * the dynamic_array,  or it will be an empty struct.
 */
#undef __field
#define __field(type, item)

#undef __array
#define __array(type, item, len)

#undef __dynamic_array
#define __dynamic_array(type, item, len) __u32 item;

#undef __string
#define __string(item, src) __dynamic_array(char, item, -1);
