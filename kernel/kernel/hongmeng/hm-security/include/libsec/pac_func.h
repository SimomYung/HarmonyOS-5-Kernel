/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PAC inline function
 */

#ifndef HM_PACFUNC_H
#define HM_PACFUNC_H

#ifndef __ASSEMBLY__
#include <hmasm/types.h>
#if defined(__aarch64__) && defined(PAC_DFI_SHARED)

#define ENABLE_PAC_HARD_CODE
#define PAC_SHIFT_BITS 32U
#define BYTES_PER_U64 8U
#define BITS_PER_BYTE 8U

static inline __u64 hm_pacdb(__u64 data, __u64 context)
{
	__u64 res = data;
	asm volatile("pacdb  %0, %1" : "+r"(res) : "r"(context) :);
	return res >> PAC_SHIFT_BITS;
}

static inline __u64 hm_pacga(__u64 data, __u64 context)
{
	__u64 res;
	asm volatile(
		"pacga  %0, %1, %2"
		: "=r"(res)
		: "r" (data), "r"(context)
		:
	);
	return res >> PAC_SHIFT_BITS;
}

static inline void _hm_pac_record_fault(__u64 data, __u64 context, __u64 mem_pac, __u64 calc_pac)
{
	asm volatile("mov x0, #1\n\t"
		     "mov w1, %w0\n\t"
		     "mov w2, %w1\n\t"
		     "mov w3, %w2\n\t"
		     "mov w4, %w3\n\t"
		     "ldr x0, [x0]"
		     :
		     :"r"(mem_pac), "r"(calc_pac), "r"(data), "r"(context)
		     :);
}

static inline void hm_pacdb_check(__u64 data, __u64 context, __u64 pac)
{
	__u64 pac1 = hm_pacdb(data, context);
	if (pac != pac1) {
		hm_error("[PAC SHARED DFI] pacdb check fail pac %llx pac1 %llx context %llx data %llx\n",
			pac, pac1, context, data);
		_hm_pac_record_fault(data, context, pac, pac1);
	}
}

static inline void hm_pacga_check(__u64 data, __u64 context, __u64 pac)
{
	__u64 pac1 = hm_pacga(data, context);
	if (pac != pac1) {
		hm_error("[PAC SHARED DFI] pacga check fail pac %llx pac1 %llx context %llx data %llx\n",
			pac, pac1, context, data);
		_hm_pac_record_fault(data, context, pac, pac1);
	}
}

static inline void hm_pac_mem_dump(void *base, __u64 len)
{
	int i;
	__u64 tmp = 0;
	unsigned char *data = (unsigned char *)base;
 
	for (i = 0; i < len / BYTES_PER_U64; i++, data += BYTES_PER_U64) {
		hm_warn("PAC dump data[%d] = 0x%llx\n", i, *((__u64 *)data));
	}
 
	if (len % BYTES_PER_U64) {
		tmp = 0;
		for (i = 0; i < len % BITS_PER_BYTE; i++, data++) {
			tmp = tmp | *data;
			tmp <<= BITS_PER_BYTE;
		}
		hm_warn("PAC dump data = 0x%llx\n", tmp);
	}
}

static inline __u64 hm_pacga_mem(void *base, __u64 len, __u64 modifier)
{
	int i;
	__u64 tmp = 0;
	unsigned char *data = (unsigned char *)base;
	__u64 res = modifier;

	for (i = 0; i < len / BYTES_PER_U64; i++, data += BYTES_PER_U64) {
		res = hm_pacga(*(__u64 *)data, res);
	}

	if (len % BYTES_PER_U64) {
		tmp = 0;
		for (i = 0; i < len % BITS_PER_BYTE; i++, data++) {
			tmp = tmp | *data;
			tmp <<= BITS_PER_BYTE;
		}
		res = hm_pacga(tmp, res);
	}
	return res;
}

static inline void hm_pacga_mem_check(void *base, __u64 len, __u64 modifier, __u64 pac, __u32 BID)
{
	__u64 pac1 = hm_pacga_mem(base, len, modifier);
	if (pac != pac1) {
		hm_error("[PAC SHARED DFI] pacga mem base %llx mod %llx pac %llx pac1 %llx BID %x\n",
			base, modifier, pac, pac1, BID);
		hm_pac_mem_dump(base, len);
		_hm_pac_record_fault(base, modifier, pac, pac1);
	}
}

static inline __u64 hm_pacdb_mem(void *base, __u64 len, __u64 modifier)
{
	int i;
	__u64 tmp = 0;
	unsigned char *data = (unsigned char *)base;
	__u64 res = modifier;

	for (i = 0; i < len / BYTES_PER_U64; i++, data += BYTES_PER_U64) {
		res = hm_pacdb(*(__u64 *)data, res);
	}

	if (len % BYTES_PER_U64) {
		tmp = 0;
		for (i = 0; i < len % BYTES_PER_U64; i++, data++) {
			tmp = tmp | *data;
			tmp <<= BITS_PER_BYTE;
		}
		res = hm_pacdb(tmp, res);
	}
	return res;
}

static inline void hm_pacdb_mem_check(void *base, __u64 len, __u64 modifier, __u64 pac, __u32 BID)
{
	__u64 pac1 = hm_pacdb_mem(base, len, modifier);
	if (pac != pac1) {
		hm_error("[PAC SHARED DFI] pacdb mem base %llx mod %llx pac %llx pac1 %llx BID %x\n",
			 base, modifier, pac, pac1, BID);
		hm_pac_mem_dump(base, len);
		_hm_pac_record_fault(base, modifier, pac, pac1);
	}
}

#else

static inline __u64 hm_pacdb(__u64 data, __u64 context)
{
	UNUSED(context);
	return data;
}

static inline __u64 hm_pacga(__u64 data, __u64 context)
{
	UNUSED(context);
	return data;
}

static inline void _hm_pac_record_fault(__u64 data, __u64 context, __u64 mem_pac, __u64 calc_pac)
{
	UNUSED(data);
	UNUSED(context);
	UNUSED(mem_pac);
	UNUSED(calc_pac);
}

static inline void hm_pacdb_check(__u64 data, __u64 context, __u64 pac)
{
	UNUSED(data, context, pac);
}

static inline void hm_pacga_check(__u64 data, __u64 context, __u64 pac)
{
	UNUSED(data, context, pac);
}

static inline __u64 hm_pacga_mem(void *base, __u64 len, __u64 modifier)
{
	UNUSED(base, len, modifier);
	return 0;
}

static inline void hm_pacga_mem_check(void *base, __u64 len, __u64 modifier, __u64 pac, __u32 BID)
{
	UNUSED(base, len, modifier, pac, BID);
}

static inline __u64 hm_pacdb_mem(void *base, __u64 len, __u64 modifier)
{
	UNUSED(base, len, modifier);
	return 0;
}

static inline void hm_pacdb_mem_check(void *base, __u64 len, __u64 modifier, __u64 pac, __u32 BID)
{
	UNUSED(base, len, modifier, pac, BID);
}
#endif  // __aarch64 && PAC_DFI_SHARED
#endif  // __ASSEMBLY__
#endif  // HM_PACFUNC_H

