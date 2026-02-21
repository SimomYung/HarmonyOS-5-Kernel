/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Inner interface of hkids
 * Author: Huawei OS Kernel Lab
 * Create: Thur Jan 04 18:30:00 2024
 */
#ifndef INTAPI_HKIDS_H
#define INTAPI_HKIDS_H

#include <stdbool.h>
#include <intapi/mem/map.h>

int hkids_init(void);
void hkids_exit(void);

int hkids_module_enable(unsigned int module_id);
int hkids_module_disable(unsigned int module_id);
bool hkids_module_is_enabled(unsigned int module_id);
int hkids_module_exec_cmd(unsigned int module_id, unsigned int command_id,
		void *command_args, unsigned int args_size);

int hkids_ringbuffer_poll(void);
int hkids_ringbuffer_mmap(uint32_t cnode_idx, uint32_t offset, uint32_t len, uint32_t prot, uintptr_t *vaddr_p);
int hkids_ringbuffer_check_mmap(uint32_t offset, uint32_t len, uint32_t prot);

int hkids_palloc(int nr_meta_pages, int nr_data_pages, int *ret_nr_pages,
		void ***ret_pages, struct mem_raw_ranges **ret_vmap_info);
void hkids_pfrees(void **pages, int nr_pages);

#endif
