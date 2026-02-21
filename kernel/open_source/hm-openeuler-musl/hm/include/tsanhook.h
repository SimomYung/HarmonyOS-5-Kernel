/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: tsan hook function header
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 28 15:11:46 2022
 */
#ifndef TSAN_HOOK_H
#define TSAN_HOOK_H

#define define_tsan_loadstore_ptr(size)				\
	void (*__tsan_read##size)(unsigned long addr, unsigned long pc);		\
	void (*__tsan_unaligned_read##size)(unsigned long addr, unsigned long pc);	\
	void (*__tsan_read##size##_pc)(unsigned long addr, unsigned long pc);		\
	void (*__tsan_write##size)(unsigned long addr, unsigned long pc);		\
	void (*__tsan_unaligned_write##size)(unsigned long addr, unsigned long pc);	\
	void (*__tsan_write##size##_pc)(unsigned long addr, unsigned long pc)


struct tsan_callback {
	define_tsan_loadstore_ptr(1);
	define_tsan_loadstore_ptr(2);
	define_tsan_loadstore_ptr(4);
	define_tsan_loadstore_ptr(8);
	define_tsan_loadstore_ptr(16);
	void (*__tsan_read_range)(unsigned long addr, unsigned long size);
	void (*__tsan_write_range)(unsigned long addr, unsigned long size);
	void (*__tsan_func_entry)(unsigned long pc);
	void (*__tsan_func_exit)(void);
	void (*__tsan_init)(void);
};


void set_tsan_callback(const struct tsan_callback *new_ac);

#endif
