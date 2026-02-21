/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define the output struct of the trace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 28 03:42:59 2024
 */
#undef __field
#define __field(_type, _item)

#undef __array
#define __array(_type, _item, _len)

#undef __dynamic_array
#define __dynamic_array(_type, _item, len)

#undef __string
#define __string(item, src)

#undef __get_dynamic_array
#define __get_dynamic_array(item) ((void *)((char *)__entry + (__entry->__data_loc_##item & 0xffff)))

/* You must use this function to get dynamic_array! */
#undef __get_dynamic_array_len
#define __get_dynamic_array_len(item) ((void *)((char *)__entry + ((__entry->__data_loc_##item >> 16) & 0xffff)))

#undef __get_str
#define __get_str(field) ((char *)__get_dynamic_array(field))
