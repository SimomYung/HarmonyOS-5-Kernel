/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define the function to calculate the data size
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 28 03:42:59 2024
 */
/*
* This the third file you need to include if you want to add a new tracepoint to
* a new system service. This header file will define the function that can help you get
* information of dynamic_array, including its address and size.
* If the event doesn't exist dynamic_array, the function  will be an empty function.
*/
#undef __field
#define __field(type, item)

#undef __array
#define __array(type, item, len)

#undef __dynamic_array
#define __dynamic_array(type, item, len) 					\
	__item_length = (unsigned int)(len) * (unsigned int)sizeof(type); 					\
	__data_offsets->item = (unsigned int)(__data_size + offset_of(typeof(*entry), __data));	\
	__data_offsets->item |= __item_length << 16;				\
	__data_size += (int)__item_length;

#undef __string
#define __string(item, src) __dynamic_array(char, item,				\
					    strlen((src) ? (const char *)(src) : "(null)") + 1)
