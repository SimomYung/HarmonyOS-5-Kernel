/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: String
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifndef KLIBS_STRING_H
#define KLIBS_STRING_H

#include <hmkernel/types.h>
#include <hmkernel/panic.h>

size_t strlen(const char *str);
size_t strnlen(const char *s, size_t maxlen);
int strncmp(const char *src1, const char *src2, size_t len);

int memcmp(const void *vl, const void *vr, size_t n);
void *memset(void *dst, int c, size_t len);
void *memcpy(void *dst, const void *src, size_t len);
char *strncpy(char *dst, const char *src, size_t len);
void bzero(void *dst, size_t sz);

#ifdef __HOST_LLT__
int memcpy_s(void *dst, size_t dstMax, const void *src, size_t len);
int memset_s(void *dest, size_t destsz, int ch, size_t count);
int strncpy_s(char *dest, size_t destsz, const char *src, size_t count);
#endif
int strcmp(const char *s1, const char *s2);

void *raw_movemem(void *to, const void *from, size_t len);
void *raw_setmem(void *dst, int c, size_t len);
void *raw_cpymem(void *dst, const void *src, size_t len);
char *raw_strncpy(char *dst, const char *src, size_t len);

/*
 * copy:	destsz = count
 * copy_to:	destsz = sizeof(dest)
 * copy_from:	count  = sizeof(src)
*/
#define mem_assign_s(dest, src) do {(dest) = (src);} while (__false)
#define mem_assign_a(dest, src)					\
do {								\
	BUG_ON(sizeof(dest) != sizeof(src));			\
	unsigned long ___i;					\
	unsigned long ___num = sizeof(dest) / sizeof(dest[0]);	\
	for (___i = 0; ___i < ___num; ___i++) {			\
		(dest[___i]) = (src[___i]);			\
	}							\
} while (__false)

#define mem_cpy_p_to_p(destp, dest_len, srcp, src_len)		\
do {								\
	size_t ____src_len = (src_len);				\
	BUG_ON((dest_len) < (____src_len));			\
	(void)memcpy((void *)(destp), (const void *)(srcp), (____src_len));	\
} while (__false)

#define mem_zero_s(data) bzero((void*)(&(data)), sizeof(data))
#define mem_zero_a(array) bzero((void*)(&(array[0])), sizeof(array))

#define mem_zero_p(mem, dest_len, src_len)			\
do {								\
	BUG_ON((dest_len) < (src_len)); 			\
	bzero((void*)(mem), (src_len));				\
} while (__false)

#define mem_zero_b(mem, len)					\
do {								\
	bzero((void*)(mem), (len));				\
} while (__false)

#define str_cpy_p_to_p(destp, dest_len, srcp, src_len)		\
do {								\
	BUG_ON((dest_len) <= ((typeof(dest_len))(0)));		\
	BUG_ON((dest_len) < (src_len)); 			\
	(void)strncpy((char *)(uptr_t)(destp),			\
		(const char *)(uptr_t)(srcp),			\
		(src_len));					\
	((char *)(uptr_t)(destp))[(dest_len) - 1] = '\0';	\
} while (__false)

#define str_cpy_p_to_a(desta, srcp)				\
do {								\
	BUG_ON(sizeof(desta) <= 0);				\
	(void)strncpy((char *)(&((desta)[0])),			\
		(srcp), sizeof((desta)));			\
	((char *)(&((desta)[0])))[sizeof(desta)-1] = '\0';	\
} while (__false)

#define str_cpy_a_to_a(desta, srca)				\
do {								\
	BUG_ON(sizeof(desta) < sizeof(srca));			\
	(void)strncpy((char *)(&((desta)[0])),			\
		(const char *)(&((srca)[0])),			\
		sizeof((srca)));				\
	((char *)(&((desta)[0])))[sizeof(desta) - 1] = '\0';	\
} while (__false)

static inline void
cpymem_aligned(void *_dst, void *_src, unsigned long len)
{
	u32 *dst = ptr_from_void(_dst, u32);
	u32 *src = ptr_from_void(_src, u32);

	while (len != 0u) {
		*(dst++) = *(src++);
		len -= sizeof(u32);
	}
}

#define NOFAIL(s) do {                         \
	int ___err;                             \
	___err = (s);                           \
	BUG_ON(___err != 0);                    \
} while (__false)

#endif
