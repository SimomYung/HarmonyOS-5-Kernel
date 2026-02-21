/*
 * Copyright(C) 2023 Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __RAS_BASE__
#define __RAS_BASE__

#include <linux/proc_fs.h>
#include <linux/mount.h>
#include <linux/version.h>
#include "securec.h"
struct RasVector;
int rasvector_create(struct RasVector **self);
int rasvector_append(struct RasVector *vec, const char *str);
int rasvector_substr(struct RasVector *vec, int idx, int len, char *savedbuf);
int rasvector_reset(struct RasVector *vec);
int rasvector_length(struct RasVector *vec);
int rasvector_destroy(struct RasVector *vec);
void rasvector_display(struct RasVector *vec);

struct RasProc;
#if KERNEL_VERSION(3, 10, 0) > LINUX_VERSION_CODE
int rasproc_create(struct RasProc **self, const char *name,
		   read_proc_t *pread, write_proc_t *pwrite, void *data);
#endif
int rasproc_read(struct RasProc *self, void (*update) (struct RasProc *),
		 char *buf, char **bl, off_t offset, int len, int *eof);
int rasproc_append(struct RasProc *self, const char *str);
int rasproc_destroy(struct RasProc *self);
void *rasproc_data(struct RasProc *self);

struct RasMap;
typedef int (rasmap_less_t)(void *data, void *k1, void *k2);
typedef int (rasmap_each_t)(void *data, void *key, void *val);
typedef void (rasmap_free_t)(void *data, void *key, void *val);
int rasmap_size(struct RasMap *rm);
int rasmap_each(struct RasMap *rm, rasmap_each_t *each, void *data);
int rasmap_add(struct RasMap *rm, void *key, void *val);
void *rasmap_get(struct RasMap *rm, void *key);
void rasmap_del(struct RasMap *rm, void *key);
void rasmap_clear(struct RasMap *rm);
void rasmap_destroy(struct RasMap *rm);
int rasmap_create(struct RasMap **rm, rasmap_less_t *less,
		  rasmap_free_t *freeKV, void *data);

int ras_read(const char *filename, int (*line) (void *, const char *), void *data);

/* oct: 012345670 ; dec: 1234567890 ; hex: 0x1234567890abcdef */
int ras_atoll(long long *ll, const char *a, long len, long *ppos);
int ras_equal(const char *exp, const char *act);
const char *ras_skipword(const char *bff, unsigned long len);
const char *ras_skipnull(const char *bff, unsigned long len);
int ras_args(const char *args, unsigned long len,
	     int (*process)(void *dt, int argc, char *args[]), void *dt);
long long ras_div(long long n, long long base);

char *ras_trim(char *s, int *n);
int ras_check(void);
int ras_isspace(char c);
int ras_malloc(void **m, long len);
void ras_free(const void *p);
#define RASTICK (3 * 1000 / HZ)
void ras_sleep(unsigned long long ms);

unsigned long long ras_ns(void);
unsigned long long ras_ms(void);
void ras_delay(unsigned long long ms);

int ras_debugset(int level); /* set level, return old level */
#define ras_info(msg) __ras_info(__FILE__, __func__, __LINE__, msg)
#define ras_warn(msg) __ras_warn(__FILE__, __func__, __LINE__, msg)
#define ras_fail(msg) __ras_fail(__FILE__, __func__, __LINE__, msg)
#define ras_retn(ret) do { \
	return __ras_retn(__FILE__, __func__, __LINE__, ret); \
} while (0)
int __ras_info(const char *file, const char *func, int line, const char *msg);
int __ras_warn(const char *file, const char *func, int line, const char *msg);
int __ras_fail(const char *file, const char *func, int line, const char *msg);
int __ras_retn(const char *file, const char *func, int line, int ret);

#define ras_retn_if(con, ret) do { \
	if (con) \
		ras_retn(ret); \
} while (0)
#define ras_retn_iferr(ret) do { \
	int ____ret = (ret); \
	if (____ret) { \
		ras_retn(____ret); \
	} \
} while (0)

#define ras_retn_iferr_then(ret, op) do { \
	int ____ret = (ret); \
	if (____ret) { \
		(op); \
		ras_retn(____ret); \
	} \
} while (0)

#define ras_setllu_retn(rase, valu, flag, fld, arg) do { \
	int len = strlen(#fld"="); \
	if (strncmp(#fld"=", arg, len) == 0) { \
		long long val = 0; \
		char *cval = arg + len; \
		ras_retn_if(*cval == 0, (-EINVAL)); \
		ras_retn_iferr(ras_atoll(&val, cval, strlen(cval), 0)); \
		rase->flag.fld = 1; \
		rase->valu.fld = val; \
		return 0; \
	} \
} while (0)

#define ras_setstr_retn(rase, valu, flag, fld, arg) do { \
	int len = strlen(#fld"="); \
	if (strncmp(#fld"=", arg, len) == 0) { \
		char *cval = arg + len; \
		ras_retn_if(*cval == 0, (-EINVAL)); \
		rase->flag.fld[0] = 1; \
		(void)strncpy_s((rase)->valu.fld, sizeof((rase)->valu.fld), cval, sizeof((rase)->valu.fld) - 1); \
		return 0; \
	} \
} while (0)

int rasfile_pathname(struct vfsmount *mnt, struct dentry *den, int (*op)(void *, const char *), void *data);
#define rasbase_set(ins, fld, arg) do { \
	unsigned long long val = 0; \
	int len = strlen(#fld"="); \
	if (strncmp(#fld"=", arg, len) == 0) { \
		char *cval = arg + len; \
		ras_retn_if(*cval == 0, (-EINVAL)); \
		ras_retn_iferr(ras_atoll(&val, cval, strlen(cval), 0)); \
		ins->fld = val; \
		return 0; \
	} \
} while (0)
#define rasbase_set_func(ins, fld, arg, func) do { \
	int len = strlen(#fld"="); \
	if (strncmp(#fld"=", arg, len) == 0) { \
		char *cval = arg + len; \
		ras_retn_if(*cval == 0, (-EINVAL)); \
		ins->fld = func(cval); \
		return 0; \
	} \
} while (0)
#define rasbase_cset(ins, fld, arg) do { \
	int len = strlen(#fld"="); \
	if (strncmp(#fld"=", arg, len) == 0) { \
		char *cval = arg + len; \
		ras_retn_if(*cval == 0, (-EINVAL)); \
		(void)snprintf_s((ins)->fld, sizeof((ins)->fld), sizeof((ins)->fld) - 1, cval); \
		return 0; \
	} \
} while (0)
#endif
