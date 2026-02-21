/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define the function to calculate the data size
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 28 03:42:59 2024
 */
#undef __field
#define __field(type, item)

#undef __array
#define __array(type, item, len)

#undef __dynamic_array
#define __dynamic_array(type, item, len) __entry->__data_loc_##item = data_offsets.item;

#undef __string
#define __string(item, src) __dynamic_array(char, item, -1)

#undef __get_dynamic_array
#define __get_dynamic_array(item) ((void *)((char *)__entry + (__entry->__data_loc_##item & 0xffff)))
/* You must use this function to get dynamic_array! */
#undef __get_dynamic_array_len
#define __get_dynamic_array_len(item) ((__entry->__data_loc_##item >> 16) & 0xffff)
#undef __get_str
#define __get_str(field) ((char *)__get_dynamic_array(field))

#undef __assign_str
#ifdef __KERNEL__
#define __assign_str(dst, src)						\
	if (likely(entry_data_alloc)) {\
		(void)strncpy(__get_str(dst), src != NULL ? src : "(null)",\
			      __get_dynamic_array_len(dst));\
	}
#else
#define __assign_str(dst, src)						\
	if (likely(entry_data_alloc)) {\
		(void)strncpy_s(__get_str(dst), __get_dynamic_array_len(dst),	\
				src != NULL ? src : "(null)", __get_dynamic_array_len(dst) - 1);\
	}
#endif
