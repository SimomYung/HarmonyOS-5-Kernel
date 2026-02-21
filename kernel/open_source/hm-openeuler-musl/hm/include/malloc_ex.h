/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022 ~ 2022. All rights reserved.
 * Description: Atfork hook interfaces for malloc
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 22 15:50:08 2022
 */

#ifndef HM_INCLUDE_MALLOC_EX_H
#define HM_INCLUDE_MALLOC_EX_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tag_topn_mem_use_info {
	uint16_t handle;
	uint16_t reserved[3]; // This is reserved variable. 3 is caculated number to reserved.
	size_t use_size;
} mem_handle_use_info;

/* Memory DFX interface in release versions such as malloc_ex. */
void *malloc_ex(uint16_t handle, const char *filename, uint16_t line, size_t size);
void *calloc_ex(uint16_t handle, const char *filename, uint16_t line, size_t n, size_t size);
void *realloc_ex(uint16_t handle, const char *filename, uint16_t line, void *old_mem, size_t new_size);
void free_ex(uint16_t handle, void *mem);
size_t malloc_usable_size_ex(uint16_t handle, void *mem);

/* APIs related to handle management, the handle obtained here can be used as the input parameter of malloc_ex.*/
uint16_t mem_handle_get(const char *component_name);
const char *handle_name_get(uint16_t handle);

/* Interface for Obtaining Memory Statistics in Release Versions */
/* (0,      *,     *,    *) show the help info. */
/* (1,      *, count, size) obtains the number and total size of memory in current process. */
/* (2,      *, count, size) obtains the number and total size of memory by malloc posix funcs. */
/* (3,      *, count, size) obtains the number and total size of memory by malloc_ex handle funcs. */
/* (4,      *,     *,    *) show the detail info of memory by malloc_ex handle funcs. */
/* (5, handle, count, size) obtains the number and size of memory in specified handle. */
uint32_t meminfo_get(uint16_t type, uint16_t handle, size_t *count, size_t *size);

/* Log management interfaces. Users can take over the information output of the meminfo_get interface. */
typedef void (*malloc_ex_log_func)(int32_t level, const char *fmt, ...);
void malloc_ex_log_func_reg(malloc_ex_log_func func);

uint32_t mem_ref_get_ex(void *addr, uint32_t *ref_count);
uint32_t mem_ref_inc_ex(void *addr);
uint32_t mem_user_size_get_ex(uint16_t handle, size_t *size);
uint32_t mem_topn_size_get_ex(uint32_t *handle_num, mem_handle_use_info handle_info[]);
void *memalign_ex(uint16_t handle, size_t align, size_t size);
uint16_t mem_handle_get_by_addr(void *mem);

#ifdef __cplusplus
}
#endif

#endif /* HM_INCLUDE_MALLOC_EX_H */
