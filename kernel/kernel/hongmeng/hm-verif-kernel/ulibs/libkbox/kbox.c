/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Kbox lib interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 20 16:57:13 2019
 */

#include <libkbox/device.h>
#include <libkbox/kbox.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/atomic.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>
#include <hmkernel/mm/pgtable.h>
#include <stdbool.h>
#include <stdarg.h>
#include <vfs.h>

#ifdef CONFIG_ENABLE_KASAN
#define INNER
int actv_open_region(const char *region_name, uintptr_t *pvaddr, size_t *psize);
int actv_close_region(const char *region_name, uintptr_t vaddr, size_t size);
#else
#define INNER static
#endif

struct kbox_region {
	int id;
	size_t size;
	size_t data_size;
	int refcnt;
	char name[KBOX_REGION_NAME_SIZE];
	struct kbox_device_region *dev;
};

static struct raw_mutex g_mutex = RAW_MUTEX_INITIALIZER_RECURSIVE;
static struct kbox_region *g_regions[KBOX_REGION_COUNT_MAX] = {
	[0 ... KBOX_REGION_COUNT_MAX - 1U] = NULL,
};

static struct kbox_region *region_of_id(int id)
{
	struct kbox_region *region = NULL;

	if (id >= 0 && (unsigned int)id < KBOX_REGION_COUNT_MAX) {
		region = g_regions[id];
	}
	return region;
}

static struct kbox_region *acquire_by_region_id_nolock(int id)
{
	struct kbox_region *region = NULL;

	region = region_of_id(id);
	if (likely(region != NULL)) {
		/*
		 * the region should be opened before, so its refcnt is not
		 * expected to be 0.
		 */
		if (region->refcnt != 0) {
			region->refcnt++;
		} else {
			region = NULL;
		}
	}

	return region;
}

static struct kbox_region *acquire_by_region_id(int id)
{
	struct kbox_region *region = NULL;

	raw_mutex_lock(&g_mutex);
	region = acquire_by_region_id_nolock(id);
	raw_mutex_unlock(&g_mutex);

	return region;
}

static struct kbox_region *region_of_name(const char *name)
{
	unsigned int i;
	struct kbox_region *region = NULL;

	for (i = 0U; i < KBOX_REGION_COUNT_MAX; i++) {
		if (g_regions[i] != NULL &&
		    strcmp(g_regions[i]->name, name) == 0) {
			region = g_regions[i];
			break;
		}
	}
	return region;
}

static struct kbox_region *acquire_by_region_name(const char *name)
{
	struct kbox_region *region = NULL;

	raw_mutex_lock(&g_mutex);
	region = region_of_name(name);
	if (likely(region != NULL)) {
		if (region->refcnt == 0) {
			region = NULL;
		} else {
			region->refcnt++;
		}
	}
	raw_mutex_unlock(&g_mutex);
	return region;
}

static int do_add_region(struct kbox_region *region)
{
	unsigned int i;
	int err = E_HM_INVAL;

	for (i = 0U; i < KBOX_REGION_COUNT_MAX; i++) {
		if (g_regions[i] == NULL) {
			region->id = (int)i;
			g_regions[i] = region;
			hm_debug("add region <%s, %d, %#x, %#x>\n",
				 region->name, region->id,
				 (unsigned int)region->size,
				 (unsigned int)region->data_size);
			err = E_HM_OK;
			break;
		}
	}
	return err;
}

static struct kbox_region *add_region(const char *region_name,
				      struct kbox_device_region *dev,
				      size_t region_size)
{
	int err;
	struct kbox_region *region = NULL;

	region = (struct kbox_region *)malloc(sizeof(struct kbox_region));
	if (region != NULL) {
		err = strcpy_s(region->name, KBOX_REGION_NAME_SIZE,
			       region_name);
		if (err == 0) {
			region->size = region_size;
			region->data_size = region_size -
				sizeof(struct kbox_device_region);
			region->dev = dev;
			region->refcnt = 0;
			err = do_add_region(region);
		} else {
			hm_warn("strcpy_s failed: %d\n", err);
			err = E_HM_RESFAULT;
		}
		if (err != E_HM_OK) {
			free(region);
			region = NULL;
		}
	} else {
		hm_warn("failed to malloc region structure\n");
	}
	return region;
}

static int actv_create_region(const char *region_name, size_t size)
{
	int err;
	struct kbox_region_name name;

	err = strcpy_s(name.value, KBOX_REGION_NAME_SIZE, region_name);
	if (err == 0) {
		err = actvcall_hmcall_kbox_create_region(name, size);
		if (err != E_HM_OK) {
			hm_warn("actv call failed: %s\n", hmstrerror(err));
		}
	} else {
		hm_warn("strcpy_s failed: %d\n", err);
		err = E_HM_RESFAULT;
	}
	return err;
}

INNER int actv_open_region(const char *region_name, uintptr_t *pvaddr,
			   size_t *psize)
{
	int err;
	struct kbox_region_name name;
	struct __actvret_hmcall_kbox_open_region ret;

	err = strcpy_s(name.value, KBOX_REGION_NAME_SIZE, region_name);
	if (err == 0) {
		mem_zero_s(ret);
		err = actvcall_hmcall_kbox_open_region(name, &ret);
		if (err == E_HM_AGAIN) {
			hm_info("region %s opened twice\n", region_name);
		} else if (err != E_HM_OK) {
			hm_info("unable to open region %s: %s\n", region_name, hmstrerror(err));
		} else if (kbox_check_region_size((size_t)ret.size) != E_HM_OK) {
			hm_warn("unexpected region size: %llu\n", ret.size);
			(void)actvcall_hmcall_kbox_close_region(name, ret.vaddr,
								ret.size);
			err = E_HM_RESFAULT;
		} else {
			*pvaddr = (uintptr_t)ret.vaddr;
			*psize = (size_t)ret.size;
		}
	} else {
		hm_warn("strcpy_s failed: %d\n", err);
		err = E_HM_RESFAULT;
	}
	return err;
}

INNER int actv_close_region(const char *region_name, uintptr_t vaddr,
			    size_t size)
{
	int err;
	struct kbox_region_name name;

	err = strcpy_s(name.value, KBOX_REGION_NAME_SIZE, region_name);
	if (err == 0) {
		err = actvcall_hmcall_kbox_close_region(name, (unsigned long long)vaddr,
							(unsigned long long)size);
	} else {
		hm_warn("strcpy_s failed: %d when processing %s\n", err, region_name);
		err = E_HM_RESFAULT;
	}

	return err;
}

static void remove_region(struct kbox_region *region)
{
	(void)actv_close_region(region->name, (uintptr_t)region->dev,
				region->size);
	g_regions[region->id] = NULL;
	free(region);
}

static int do_kbox_open(const char *region_name, struct kbox_region **pregion)
{
	int err;
	unsigned int i;
	struct kbox_region *region = NULL;
	uintptr_t vaddr = 0UL;
	size_t size = 0UL;

	err = E_HM_TOOMANY;
	for (i = 0U; i < KBOX_REGION_COUNT_MAX; i++) {
		if (g_regions[i] == NULL) {
			err = E_HM_OK;
			break;
		}
	}
	if (err == E_HM_OK) {
		err = actv_open_region(region_name, &vaddr, &size);
	} else {
		hm_warn("no free region slot left\n");
	}
	if (err == E_HM_OK) {
		region = add_region(region_name,
				    (struct kbox_device_region *)vaddr,
				    size);
		if (region == NULL) {
			hm_warn("IMPOSSIBLE: no free region slot\n");
			(void)actv_close_region(region_name, vaddr, size);
			err = E_HM_NOOBJ;
		}
	} else if (err == E_HM_AGAIN) {
		hm_warn("IMPOSSIBLE: the server thinks we opened region %s twice\n",
			region_name);
		err = E_HM_RESFAULT;
	} else if (err == E_HM_NOOBJ) {
		hm_info("try to open unknown kbox region: %s\n", region_name);
	} else {
		hm_warn("failed to call actv open: %s\n", hmstrerror(err));
	}
	*pregion = region;
	return err;
}

int kbox_create(const char *region_name, size_t region_size)
{
	int err;

	err = kbox_check_region_size(region_size);
	if (err == E_HM_OK) {
		err = kbox_check_region_name(region_name);
	}
	if (err == E_HM_OK) {
		err = actv_create_region(region_name, region_size);
	}
	return err;
}

int kbox_query(struct kbox_info *info)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_kbox_query_device dret;

	if (info == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = actvcall_hmcall_kbox_query_device(&dret);
		if (err != E_HM_OK) {
			hm_warn("failed to query kbox device, err=%s\n", hmstrerror(err));
		} else {
			info->paddr = dret.paddr;
			info->total_size = (size_t)dret.total_size;
		}
	}

	return err;
}

int kbox_open(const char *region_name)
{
	int err;
	int id = -1;
	struct kbox_region *region = NULL;

	err = kbox_check_region_name(region_name);
	if (err == E_HM_OK) {
		raw_mutex_lock(&g_mutex);
		region = region_of_name(region_name);
		if (region == NULL) {
			err = do_kbox_open(region_name, &region);
		}
		if (err == E_HM_OK && region != NULL) {
			id = region->id;
			region->refcnt++;
			hm_debug("open region: %s, id: %d, refcnt: %d\n",
				 region->name, region->id, region->refcnt);
		}
		raw_mutex_unlock(&g_mutex);
	}

	return err == E_HM_OK ? id : err;
}

void kbox_close(int region_id)
{
	struct kbox_region *region = NULL;

	raw_mutex_lock(&g_mutex);
	region = region_of_id(region_id);
	if (region == NULL) {
		hm_warn("invalid region id: %d\n", region_id);
	} else if (region->refcnt <= 0) {
		hm_warn("unexpected refcnt: %d\n", region->refcnt);
	} else {
		hm_debug("close region: %s, id: %d, refcnt: %d\n",
			 region->name, region_id, region->refcnt);
		region->refcnt--;
		if (region->refcnt == 0) {
			remove_region(region);
		}
	}
	raw_mutex_unlock(&g_mutex);
}

static void release_region_nolock(struct kbox_region *region)
{
	if (region->refcnt > 0) {
		region->refcnt--;
		if (region->refcnt == 0) {
			remove_region(region);
		}
	}
}

static void release_region(struct kbox_region *region)
{
	raw_mutex_lock(&g_mutex);
	release_region_nolock(region);
	raw_mutex_unlock(&g_mutex);
}


static inline size_t min(size_t x, size_t y)
{
	return x < y ? x : y;
}

static ssize_t copy_write_data(const struct kbox_region *region, size_t offset,
			       const void *buf, size_t size)
{
	int err;
	size_t len = 0UL;
	size_t left = size;
	void *dst = NULL;
	const void *src = buf;

	while (left != 0UL) {
		src = ptr_add(src, len);
		dst = ptr_add(region->dev->data, offset);
		len = min(region->data_size - offset, left);
		err = memcpy_s(dst, len, src, len);
		if (err != 0) {
			break;
		}
		left -= len;
		offset = (offset + len) % region->data_size;
	}
	return (ssize_t)(unsigned int)(size - left);
}

/*
 * This function is only called when seq overflows.
 *
 * Consider the following special scenario:
 *    KBOX_SEQ_MASK is (1U << 30) - 1,
 *    (1) clr_seq = 0x8, seq = 0x3fff,fffb
 *    (2) writing 64 bytes will cause seq to overflow to 0x5f
 *    (3) use "seq - clr_seq" to calculate the length to be read, that will get
 *        the wrong value 0x57
 *
 * To solve the above problem, when seq overflows and seq > clr_seq, clr_seq
 * must be set to:
 *    clr_seq = kbox_seq_minus(seq, region->data_size);
 */
static void seq_ovf_reset_clr_seq(struct kbox_region *region, uint32_t seq)
{
	raw_atomic_int_t *p_cbits = ptr_cast(&region->dev->cbits, raw_atomic_int_t);
	raw_atomic_int_t *p_clr_seq = ptr_cast(&region->dev->clr_seq, raw_atomic_int_t);
	uint32_t cbits;
	uint32_t clr_seq;
	uint32_t new;

	do {
		clr_seq = (uint32_t)raw_atomic_int_read(p_clr_seq);
		if (clr_seq > seq) {
			break;
		}

		/*
		 * If erase is concurrently executed at this time, empty may be true.
		 * In this case, give up trying to modify clr_seq.
		 */
		cbits = (uint32_t)raw_atomic_int_read(p_cbits);
		if (__KBOX_REGION_CBITS_DECODE_EMPTY(cbits) != 0U) {
			break;
		}

		new = (uint32_t)kbox_seq_minus(seq, region->data_size);
	} while ((uint32_t)raw_atomic_int_cas(p_clr_seq,
					(int)clr_seq, (int)new) != clr_seq);
}

static ssize_t do_write(struct kbox_region *region, const void *buf,
			size_t size)
{
	uint32_t empty;
	uint32_t seq;
	uint32_t old;
	uint32_t new;
	size_t wr_seq;
	raw_atomic_int_t *addr = ptr_cast(&region->dev->cbits, raw_atomic_int_t);

	do {
		old = (uint32_t)raw_atomic_int_read(addr);
		kbox_cbits_split(old, &empty, &seq);
		seq = (uint32_t)kbox_seq_add(seq, size);
		/* first round write full, set empty to 0 */
		if (unlikely(empty != 0U) && seq >= region->data_size) {
			empty = 0U;
		}
		/* allocate space */
		kbox_cbits_merge(&new, empty, seq);
	} while ((uint32_t)raw_atomic_int_cas(addr, (int)old, (int)new) != old);

	wr_seq = kbox_seq_minus(seq, size);
	if (unlikely(wr_seq > seq)) {
		/*
		 * seq overflowed, need to handle clr_seq carefully,
		 * see seq_ovf_reset_clr_seq() for details
		 */
		seq_ovf_reset_clr_seq(region, seq);
	}

	return copy_write_data(region, wr_seq % region->data_size, buf, size);
}

static ssize_t __kbox_write(struct kbox_region *region, const void *buf, size_t size)
{
	ssize_t wsize = E_HM_INVAL;

	/*
	 * kbox maybe used as a backend for hmlog, so we can't call
	 * hmlog to print log in kbox_write.
	 */
	if (unlikely(size > KBOX_REGION_WRITE_SIZE)) {
		size = KBOX_REGION_WRITE_SIZE;
	}
	if (likely(size > 0UL)) {
		wsize = do_write(region, buf, size);
	} else {
		wsize = 0;
	}

	return wsize;
}

ssize_t kbox_write_nolock(int region_id, const void *buf, size_t size)
{
	struct kbox_region *region = NULL;
	ssize_t wsize = E_HM_INVAL;

	if (buf != NULL) {
		region = acquire_by_region_id_nolock(region_id);
		if (region != NULL) {
			wsize = __kbox_write(region, buf, size);
			release_region_nolock(region);
		}
	}
	return wsize;
}

ssize_t kbox_write(int region_id, const void *buf, size_t size)
{
	struct kbox_region *region = NULL;
	ssize_t wsize = E_HM_INVAL;

	if (buf != NULL) {
		region = acquire_by_region_id(region_id);
		if (region != NULL) {
			wsize = __kbox_write(region, buf, size);
			release_region(region);
		}
	}
	return wsize;
}

static ssize_t do_kbox_dup(struct kbox_region *region, size_t dup_size)
{
	int err = E_HM_OK;
	size_t start_offset;
	size_t left = dup_size;
	ssize_t total_copied = 0;
	uint32_t empty;
	uint32_t wr_seq;

	raw_atomic_int_t *addr = ptr_cast(&region->dev->cbits, raw_atomic_int_t);
	kbox_cbits_split((uint32_t)raw_atomic_int_read(addr), &empty, &wr_seq);
	start_offset = (region->data_size + wr_seq - left) % region->data_size;
	while (left > 0) {
		size_t copy_size = min(left, region->data_size - start_offset);
		ssize_t wsize = do_write(region, ptr_add(region->dev->data, start_offset), copy_size);
		if (wsize <= 0) {
			if (wsize < 0) {
				err = (int)wsize;
			}
			break;
		}
		left -= (size_t)wsize;
		total_copied += wsize;
		start_offset = ((start_offset + wsize) % region->data_size);
	}
	if (err < 0) {
		total_copied = (ssize_t)err;
	}

	return total_copied;
}

ssize_t kbox_dup(int region_id, size_t dup_size, bool full_dup)
{
	ssize_t err = E_HM_OK;
	struct kbox_region *region = NULL;
	size_t target_size = dup_size;

	region = acquire_by_region_id(region_id);
	if (region == NULL || dup_size >= region->data_size) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK && dup_size > 0) {
		size_t left = region->data_size - target_size;
		if (!full_dup && left < target_size) {
			target_size = left;
		}
		err = do_kbox_dup(region, target_size);
	}

	return err;
}

#define KBOX_PRINTF_TRUNCATED_STR "data truncated in kbox_printf\n"
#define KBOX_PRINTF_BUF_SIZE 1024UL
void __attribute__((format(printf, 2, 3)))
kbox_printf(int region_id, const char *fmt, ...)
{
	int len;
	va_list args;
	char buf[KBOX_PRINTF_BUF_SIZE];

	va_start(args, fmt);
	len = vsnprintf_s(buf, KBOX_PRINTF_BUF_SIZE, KBOX_PRINTF_BUF_SIZE - 1,
			  fmt, args);
	va_end(args);
	if (len < 0) {
		(void)kbox_write(region_id, KBOX_PRINTF_TRUNCATED_STR,
				 sizeof(KBOX_PRINTF_TRUNCATED_STR));
	} else {
		(void)kbox_write(region_id, buf, (size_t)(unsigned int)len);
	}
}

static ssize_t copy_read_data(const struct kbox_region *region, size_t offset,
			      void *buf, size_t size)
{
	int err;
	size_t len = 0UL;
	size_t left = size;
	void *dst = buf;
	const void *src = NULL;

	while (left != 0UL) {
		dst = ptr_add(dst, len);
		src = ptr_add(region->dev->data, offset);
		len = min(region->data_size - offset, left);
		err = memcpy_s(dst, len, src, len);
		if (err != 0) {
			hm_warn("kbox: memcpy_s failed: err=%d, region=%s\n",
				err, region->name);
			break;
		}
		/* len must be larger than 0, since offset is impossible equal
		 * to region->data_size as a result of % */
		left -= len;
		offset = (offset + len) % region->data_size;
	}
	return (ssize_t)(unsigned int)(size - left);
}

static ssize_t calc_read_clear_len(size_t data_size, uint32_t clr_seq,
				   uint32_t empty, uint32_t nseq)
{
	ssize_t rsize = 0;

	/*
	 * When kbox is empty, the kbox header must be:
	 *     data_size > nseq >= clr_seq
	 */
	if (empty != 0U) {
		if ((clr_seq > nseq) || (nseq >= data_size)) {
			hm_warn("kbox: invalid header: empty=%u, nseq=%u, clr_seq=%u\n",
				empty, nseq, clr_seq);
			rsize = E_HM_RESFAULT;
		}
	}

	if (rsize == 0) {
		size_t seq_diff = kbox_seq_minus(nseq, clr_seq);
		if (seq_diff > data_size) {
			/* whole kbox region has been overwritten, ignore clr_seq */
			rsize = (ssize_t)(unsigned int)data_size;
		} else {
			rsize = (ssize_t)(unsigned int)seq_diff;
		}
	}

	return rsize;
}

static ssize_t do_read_clear(struct kbox_region *region, char *buf)
{
	ssize_t rsize = 0;
	raw_atomic_int_t *p_cbits = ptr_cast(&region->dev->cbits, raw_atomic_int_t);
	raw_atomic_int_t *p_clr_seq = ptr_cast(&region->dev->clr_seq, raw_atomic_int_t);
	uint32_t cbits;
	uint32_t clr_seq;
	uint32_t empty, nseq;

	do {
		size_t offset;

		cbits = (uint32_t)raw_atomic_int_read(p_cbits);
		clr_seq = (uint32_t)raw_atomic_int_read(p_clr_seq);

		kbox_cbits_split(cbits, &empty, &nseq);

		rsize = calc_read_clear_len(region->data_size, clr_seq, empty, nseq);
		if (rsize <= 0) {
			break;
		}

		if ((size_t)rsize < region->data_size) {
			offset = (size_t)clr_seq;
		} else {
			offset = kbox_seq_minus(nseq, region->data_size);
		}

		rsize = copy_read_data(region, offset % region->data_size, buf, (size_t)rsize);
	} while (((uint32_t)raw_atomic_int_read(p_cbits) != cbits) ||
		 ((uint32_t)raw_atomic_int_cas(p_clr_seq,
					(int)clr_seq, (int)nseq) != clr_seq));

	return rsize;
}

static ssize_t do_read(struct kbox_region *region, char *buf)
{
	uint32_t empty;
	uint32_t nseq; /* next sequence in the region */
	uint32_t old;
	ssize_t rsize; /* read size */
	raw_atomic_int_t *addr = ptr_cast(&region->dev->cbits, raw_atomic_int_t);

	do {
		old = (uint32_t)raw_atomic_int_read(addr);
		kbox_cbits_split(old, &empty, &nseq);
		if (likely(empty == 0U)) {
			size_t lseq = kbox_seq_minus(nseq, region->data_size);
			rsize = copy_read_data(region, lseq % region->data_size,
					       buf, region->data_size);
		} else if (likely(nseq < region->data_size)) {
			rsize = copy_read_data(region, 0UL, buf, nseq);
		} else {
			hm_warn("kbox: impossible empty flag and sequence: "
				"empty=%u, nseq=%u\n", empty, nseq);
			rsize = E_HM_RESFAULT;
		}
	} while ((uint32_t)raw_atomic_int_read(addr) != old);

	return rsize;
}

ssize_t kbox_read_raw(const char *region_name, char *buf, size_t size, bool clear_data)
{
	ssize_t rsize = E_HM_INVAL; /* read size */
	struct kbox_region *region = NULL;

	if (buf != NULL) {
		region = acquire_by_region_name(region_name);
	}
	if (region != NULL) {
		if (size >= region->data_size) {
			rsize = clear_data ? do_read_clear(region, buf)
					   : do_read(region, buf);
		} else {
			rsize = E_HM_FBIG;
		}
		release_region(region);
	}
	return rsize;
}

#define DATA_SIZE	50
ssize_t kbox_read(int region_id, char *buf, size_t size)
{
	int err = E_HM_OK;
	int fd = -1;
	ssize_t len = 0;
	struct kbox_region *region = NULL;
	char kbox_file[SYSFS_KBOX_FILE_LEN];

	if (buf == NULL) {
		err = E_HM_NOOBJ;
	}
	if (err == E_HM_OK) {
		region = acquire_by_region_id(region_id);
		if (region == NULL) {
			err = E_HM_INVAL;
		} else {
			if (snprintf_s(kbox_file, SYSFS_KBOX_FILE_LEN,
				       SYSFS_KBOX_FILE_LEN - 1U,
				       SYS_KBOX_PATH"%s", region->name) < 0) {
				err = E_HM_NOBUFS;
			}
		}
	}
	if (err == E_HM_OK) {
		fd = vfs_open(kbox_file, O_RDONLY, 0);
		if (fd >= 0) {
			len = vfs_read(fd, buf, size);
			if (len < 0) {
				err = (int)len;
				hm_warn("libkbox kbox_read read '%s' failed: %s\n",
					kbox_file, hmstrerror(err));
			}
			(void)vfs_close(fd);
		} else {
			err = fd;
			hm_warn("libkbox kbox_read open '%s' failed: %s\n",
				kbox_file, hmstrerror(err));
		}
	}
	if (region != NULL) {
		release_region(region);
	}

	return len > 0 ? len : (ssize_t)err;
}

ssize_t kbox_read_clear(int region_id, char *buf, size_t size)
{
	int err = E_HM_OK;
	int fd = -1;
	ssize_t len = 0;
	struct kbox_region *region = NULL;
	char kbox_file_clear[SYSFS_KBOX_CLEAR_FILE_LEN];

	if (buf == NULL) {
		err = E_HM_NOOBJ;
	}
	if (err == E_HM_OK) {
		region = acquire_by_region_id(region_id);
		if (region == NULL) {
			err = E_HM_INVAL;
		} else {
			if (snprintf_s(kbox_file_clear, SYSFS_KBOX_CLEAR_FILE_LEN,
				       SYSFS_KBOX_CLEAR_FILE_LEN - 1U,
				       SYS_KBOX_PATH"%s_clear", region->name) < 0) {
				err = E_HM_NOBUFS;
			}
		}
	}
	if (err == E_HM_OK) {
		fd = vfs_open(kbox_file_clear, O_RDONLY, 0);
		if (fd >= 0) {
			len = vfs_read(fd, buf, size);
			if (len < 0) {
				err = (int)len;
				hm_warn("libkbox kbox_read_clear read '%s' failed: %s\n",
					kbox_file_clear, hmstrerror(err));
			}
			(void)vfs_close(fd);
		} else {
			err = fd;
			hm_warn("libkbox kbox_read_clear open '%s' failed: %s\n",
				kbox_file_clear, hmstrerror(err));
		}
	}
	if (region != NULL) {
		release_region(region);
	}

	return len > 0 ? len : (ssize_t)err;
}

static void do_erase(struct kbox_region *region)
{
	uint32_t old;
	uint32_t new;
	uint32_t clr_seq;
	raw_atomic_int_t *p_cbits = ptr_cast(&region->dev->cbits, raw_atomic_int_t);
	raw_atomic_int_t *p_clr_seq = ptr_cast(&region->dev->clr_seq, raw_atomic_int_t);

	do {
		old = (uint32_t)raw_atomic_int_read(p_cbits);
		/* set sequence to 0, and clean first round full flag */
		kbox_cbits_merge(&new, 1U, 0U);
	} while ((uint32_t)raw_atomic_int_cas(p_cbits, (int)old, (int)new) != old);

	do {
		clr_seq = (uint32_t)raw_atomic_int_read(p_clr_seq);
	} while ((uint32_t)raw_atomic_int_cas(p_clr_seq, (int)clr_seq, 0) != clr_seq);
}

int kbox_erase_raw(const char *region_name)
{
	int err = E_HM_INVAL;
	struct kbox_region *region = NULL;
	region = acquire_by_region_name(region_name);
	if (region != NULL) {
		do_erase(region);
		err = E_HM_OK;
		release_region(region);
	}
	return err;
}

int kbox_erase(int region_id)
{
	int err = E_HM_OK;
	int fd = -1;
	size_t size = 1;
	char data[DATA_SIZE] = {0};
	struct kbox_region *region = NULL;
	char kbox_file[SYSFS_KBOX_FILE_LEN];

	region = acquire_by_region_id(region_id);
	if (region == NULL) {
		err = E_HM_INVAL;
	} else {
		if (snprintf_s(kbox_file, SYSFS_KBOX_FILE_LEN,
			       SYSFS_KBOX_FILE_LEN - 1U,
			       SYS_KBOX_PATH"%s", region->name) < 0) {
			err = E_HM_NOBUFS;
		}
	}
	if (err == E_HM_OK) {
		fd = vfs_open(kbox_file, O_WRONLY | O_TRUNC, 0);
		if (fd >= 0) {
			(void)vfs_write(fd, data, size);
			(void)vfs_close(fd);
		} else {
			err = fd;
			hm_warn("libkbox kbox_erase open '%s' failed: %s\n",
				kbox_file, hmstrerror(err));
		}
	}
	if (region != NULL) {
		release_region(region);
	}

	return err;
}

ssize_t kbox_region_size(int region_id)
{
	ssize_t size = E_HM_INVAL;
	struct kbox_region *region = NULL;

	raw_mutex_lock(&g_mutex);
	region = region_of_id(region_id);
	if (region != NULL) {
		size = (ssize_t)region->size;
	}
	raw_mutex_unlock(&g_mutex);
	return size;
}

struct err_msg_s {
	int err;
	const char *msg;
};

const char *kbox_err_msg(int err)
{
	static const struct err_msg_s err_msgs[] = {
		{ ERR_AS_INT(E_HM_OK), "success" },
		{ ERR_AS_INT(E_HM_NOSYS), "kbox is not enabled" },
		{ ERR_AS_INT(E_HM_RANGE), "region size is not in the range of [4KB, 1GB]" },
		{ ERR_AS_INT(E_HM_ALIGN), "region size is not aligned to 4K" },
		{ ERR_AS_INT(E_HM_INVAL), "invalid argument" },
		{ ERR_AS_INT(E_HM_NOMSG), "region name is empty" },
		{ ERR_AS_INT(E_HM_MSGSIZE), "the max length of region name is 31" },
		{ ERR_AS_INT(E_HM_BADMSG), "only letters, digit, '_' and '-' are allowed" },
		{ ERR_AS_INT(E_HM_OBJEXIST), "a region with the same name already exists" },
		{ ERR_AS_INT(E_HM_NOOBJ), "no region with the name exists" },
		{ ERR_AS_INT(E_HM_TOOMANY), "the max region count is 64" },
		{ ERR_AS_INT(E_HM_NOSPC), "no free space" },
		{ ERR_AS_INT(E_HM_FBIG), "read buffer is smaller than region size" },
		{ ERR_AS_INT(E_HM_BUSY), "one or more dynamic regions are not closed" },
		{ ERR_AS_INT(E_HM_RESFAULT), "internal error" },
		{ ERR_AS_INT(E_HM_PERM), "permission check failed" },
		{ ERR_AS_INT(E_HM_ACCES), "vfs access failed" },
	};

	const char *msg = "unknown";
	for (unsigned int i = 0U; i < sizeof(err_msgs) / sizeof(err_msgs[0]); ++i) {
		if (err == err_msgs[i].err) {
			msg = err_msgs[i].msg;
			break;
		}
	}
	return msg;
}
