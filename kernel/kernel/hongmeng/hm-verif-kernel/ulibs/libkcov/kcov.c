/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: Implement common kcov functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 28 21:13:17 2023
 */

#include <ctype.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libkcov/kcov.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <hmkernel/miscdata.h>
#include <hongmeng/fast_syscall.h>
#include <libstrict/strict.h>
#include <libsysif/utils_common.h>
#include <libsysif/sysmgr/api.h>
#include <libsec/service_management.h>

#ifdef CONFIG_KCOV

#define KCOV_PC_LEN	57
#define COMPOSE_PC_DATA(pc, sidx)	(((pc) & ((1ULL << KCOV_PC_LEN) - 1U)) | ((unsigned long)(sidx) << KCOV_PC_LEN))
#define KCOV_WORDS_PER_CMP 4
#define KCOV_CMP_BITS0 0U
#define KCOV_CMP_BITS1 1U
#define KCOV_CMP_BITS2 2U
#define KCOV_CMP_BITS3 3U
#define BITS_PER_BYTE 8U

struct kcov_thread_data_s {
	struct kcov_service_data service_data;
};

/* start from 1, 0 indicates kcov disabled */
static struct kcov_thread_data_s g_kcov_thread_datas[MAX_KCOV_THREAD_COUNT + 1];

static bool g_init_finished = false;

static __thread bool g_kcov_disabled = false;

void kcov_init_finished(void)
{
}

void kcov_enable(void)
{
	g_init_finished = true;
}

bool kcov_is_enabled(void)
{
	return g_init_finished;
}

static int map_shm(struct kcov_service_data *service_data, __u32 kcov_idx)
{
	int err = E_HM_OK;
	__u32 cnode_idx = hm_ucap_self_cnode_idx();

	err = actvxactcall_hmcall_kcov_map_shm(false, true, cnode_idx, kcov_idx, ptr_to_type(service_data, long long));
	if (err != E_HM_OK) {
		hm_warn("kcov map shm failed: %s\n", hmstrerror(err));
	}

	return err;
}

static void acquire_kcov_service_data(unsigned int state, struct kcov_service_data **service_data)
{
	int err = E_HM_OK;
	__u32 kcov_idx_state = 0U;
	__u32 kcov_idx = 0U;
	__u32 kcov_state = __KCOV_STATE_INVALID;

	kcov_idx_state = hm_actv_local_thread_kcov_idx_state();
	kcov_idx = __KCOV_IDX_OF(kcov_idx_state);
	kcov_state = __KCOV_STATE_OF(kcov_idx_state);
	if (kcov_idx != __KCOV_IDX_DISABLED && kcov_state == state) {
		if (g_kcov_thread_datas[kcov_idx].service_data.manage_data_vaddr == 0U) {
			err = map_shm(&g_kcov_thread_datas[kcov_idx].service_data, kcov_idx);
			if (err != E_HM_OK) {
				g_kcov_thread_datas[kcov_idx].service_data.manage_data_vaddr = 0U;
				g_kcov_thread_datas[kcov_idx].service_data.data_vaddr = 0U;
				g_kcov_thread_datas[kcov_idx].service_data.sidx = -1;
			}
		}
		if (g_kcov_thread_datas[kcov_idx].service_data.manage_data_vaddr != 0U) {
			*service_data = &g_kcov_thread_datas[kcov_idx].service_data;
		}
	}
}

static void record_pc(struct kcov_service_data *service_data, unsigned long pc)
{
	int sidx = service_data->sidx;
	struct kcov_manager_data_s *manager_data =
		ptr_to_type(service_data->manage_data_vaddr, struct kcov_manager_data_s *);
	struct kcov_data_s *data = ptr_to_type(service_data->data_vaddr, struct kcov_data_s *);
	unsigned long size = manager_data->size;
	unsigned long pos = data->data[0] + 1;
	__u64 pc_data = COMPOSE_PC_DATA(pc, sidx);

	if (pos < size) {
		data->data[pos] = pc_data;
		data->data[0] = pos;
	}
}

void __sanitizer_cov_trace_pc(void)
{
	struct kcov_service_data *service_data = NULL;

	if (g_init_finished && !g_kcov_disabled) {
		g_kcov_disabled = true;
		acquire_kcov_service_data(__KCOV_STATE_TRACE_PC, &service_data);
		if (service_data != NULL) {
			record_pc(service_data, (__u64)__builtin_return_address(0));
		}
		g_kcov_disabled = false;
	}
}

static void record_cmp_data(__u64 type, __u64 arg1, __u64 arg2, __u64 ip)
{
	int sidx;
	unsigned long count;
	unsigned long start_index;
	unsigned long end_index;
	struct kcov_service_data *service_data = NULL;
	struct kcov_manager_data_s *manager_data = NULL;
	struct kcov_data_s *data = NULL;

	g_kcov_disabled = true;
	acquire_kcov_service_data(__KCOV_STATE_TRACE_CMP, &service_data);
	if (service_data != NULL) {
		sidx = service_data->sidx;
		manager_data = ptr_to_type(service_data->manage_data_vaddr, struct kcov_manager_data_s *);
		data = ptr_to_type(service_data->data_vaddr, struct kcov_data_s *);
		count = data->data[0];
		start_index = 1 + count * KCOV_WORDS_PER_CMP;
		end_index = start_index + KCOV_WORDS_PER_CMP;
		if (end_index <= manager_data->size) {
			data->data[start_index++] = type;
			data->data[start_index++] = arg1;
			data->data[start_index++] = arg2;
			data->data[start_index] = COMPOSE_PC_DATA(ip, sidx);
			data->data[0] = count + 1;
		}
	}
	g_kcov_disabled = false;
}

static void record_trace_cmp(__u64 type, __u64 arg1, __u64 arg2, __u64 ip)
{
	if (g_init_finished && !g_kcov_disabled) {
		record_cmp_data(type, arg1, arg2, ip);
	}
}

static void record_trace_switch(__u64 val, __u64 *cases)
{
	int err = E_HM_OK;
	__u64 i;
	__u64 index = 0U;
	__u64 count = cases[index++];
	__u64 size = cases[index++];
	__u64 type = KCOV_CMP_CONST;

	if (size % BITS_PER_BYTE != 0U) {
		err = E_HM_INVAL;
	} else {
		size /= BITS_PER_BYTE;
	}

	if (err == E_HM_OK) {
		switch (size) {
		case sizeof(__u8):
			type |= KCOV_CMP_SIZE(KCOV_CMP_BITS0);
			break;
		case sizeof(__u16):
			type |= KCOV_CMP_SIZE(KCOV_CMP_BITS1);
			break;
		case sizeof(__u32):
			type |= KCOV_CMP_SIZE(KCOV_CMP_BITS2);
			break;
		case sizeof(__u64):
			type |= KCOV_CMP_SIZE(KCOV_CMP_BITS3);
			break;
		default:
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		for (i = 0U; i < count; i++) {
			record_cmp_data(type, cases[i + index], val, (__u64)__builtin_return_address(0));
		}
	}
}

void __sanitizer_cov_trace_cmp1(__u8 arg1, __u8 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS0), arg1, arg2, (__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_cmp2(__u16 arg1, __u16 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS1), arg1, arg2, (__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_cmp4(__u32 arg1, __u32 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS2), arg1, arg2, (__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_cmp8(__u64 arg1, __u64 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS3), arg1, arg2, (__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_const_cmp1(__u8 arg1, __u8 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS0) | KCOV_CMP_CONST, arg1, arg2,
			(__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_const_cmp2(__u16 arg1, __u16 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS1) | KCOV_CMP_CONST, arg1, arg2,
			(__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_const_cmp4(__u32 arg1, __u32 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS2) | KCOV_CMP_CONST, arg1, arg2,
			(__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_const_cmp8(__u64 arg1, __u64 arg2)
{
	record_trace_cmp(KCOV_CMP_SIZE(KCOV_CMP_BITS3) | KCOV_CMP_CONST, arg1, arg2,
			(__u64)__builtin_return_address(0));
}

void __sanitizer_cov_trace_switch(__u64 val, __u64 *cases)
{
	if (g_init_finished && !g_kcov_disabled) {
		record_trace_switch(val, cases);
	}
}
#else
void kcov_init_finished(void)
{
}
void __sanitizer_cov_trace_pc(void)
{
}
void __sanitizer_cov_trace_cmp1(__u8 arg1, __u8 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_cmp2(__u16 arg1, __u16 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_cmp4(__u32 arg1, __u32 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_cmp8(__u64 arg1, __u64 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_const_cmp1(__u8 arg1, __u8 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_const_cmp2(__u16 arg1, __u16 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_const_cmp4(__u32 arg1, __u32 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_const_cmp8(__u64 arg1, __u64 arg2)
{
	UNUSED(arg1, arg2);
}
void __sanitizer_cov_trace_switch(__u64 val, __u64 *cases)
{
	UNUSED(val, cases);
}
#endif
