/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Kbox lib interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 20 16:58:03 2019
 */

#ifndef ULIBS_LIBKBOX_KBOX_H
#define ULIBS_LIBKBOX_KBOX_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

struct kbox_info {
	unsigned long long paddr;
	size_t total_size;
};

/*
 * Description
 *     create a kbox region
 *
 * Return Value
 * E_HM_OK       success, the region is created
 * E_HM_NOSYS    kbox is not enabled
 * E_HM_RANGE    @region_size is out of range
 * E_HM_ALIGN    @region_size is not aligned correctly
 * E_HM_INVAL    @region_name is NULL
 * E_HM_NOMSG    @region_name is empty
 * E_HM_EMSGSIZE @region_name is too long
 * E_HM_BADMSG   @region_name contains illegal characters
 * E_HM_OBJEXIST there is a region with same name, and different size
 * E_HM_TOOMANY  there are too many regions in the kbox
 * E_HM_NOSPC    no space to create new region
 * E_HM_RESFAULT internal error
 */
int kbox_create(const char *region_name, size_t region_size);

/*
 * Description
 *     query kbox information
 *
 * Return Value
 * E_HM_OK       success
 * E_HM_INVAL    @info is NULL
 * E_HM_PERM     ability check failed
 * E_HM_RESFAULT internal error
 */
int kbox_query(struct kbox_info *info);

/*
 * Description
 *      open a kbox region
 *      1) if the kbox region is not opened before:
 *         a) map the kbox region to the process adress space.
 *         b) allocate a new region id, which is a uniq non-negative integer,
 *            to this region.
 *         c) return the region id.
 *      2) if the kbox region is opened already
 *         a) increase the region's refence count
 *         b) return the region id
 *
 * Return Value
 * >= 0          the region id
 * E_HM_NOSYS    kbox is not enabled
 * E_HM_INVAL    @region_name is NULL
 * E_HM_NOMSG    @region_name is empty
 * E_HM_EMSGSIZE @region_name is too long
 * E_HM_BADMSG   @region_name contains illegal characters
 * E_HM_NOOBJ    no region with name @region_name found
 * E_HM_TOOMANY  the opened kbox region count is more than 64
 * E_HM_RESFAULT internal error
 */
int kbox_open(const char *region_name);

/*
 * Description
 *      close a kbox region
 *      1) if there is no region opened with @region_id, nothing happens
 *      2) if the reference count to the region specified by @region_id is
 *         bigger than 1, just decrease the reference count
 *      3) if the reference count to the region specified by @region_id is
 *         1, unmap the kbox region from the the process address space
 *
 * Return Value
 * none
 */
void kbox_close(int region_id);

/*
 * Description
 *      write data to a kbox region
 *
 * Return Value
 * >= 0          the size of data has been written to the region
 * E_HM_INVAL    the @region_id is negative, or there is no kbox region opened
 *               with @region_id, or @buf is NULL
 *
 * Note
 *      kbox_write() writes at most 1024 bytes, if the @size is more than 1024,
 *      only the first 1024 bytes is written.
 */
ssize_t kbox_write(int region_id, const void *buf, size_t size);

/*
 * Description
 *     Duplicate last written bytes of data specified by dup_size to the same region
 *     specifed by region_id. When dup_size is greater than the remain size(total - dump_size)
 *     of the region, and full_dup is speified as true, total bytes of dup_size of data
 *     will be copied and overwrite to the existing data, otherwise, only part of dup_size
 *     of data is copied.
 *
 * Return Value
 * >= 0        Duplicated bytes of data
 * E_HM_INVAL  The specified dup_size is greater than the total size of the region
 */
ssize_t kbox_dup(int region_id, size_t dup_size, bool full_dup);

/*
 * Description
 *      write data to a kbox region with no lock, only used in sysmgr snapshot
 */
ssize_t kbox_write_nolock(int region_id, const void *buf, size_t size);

/*
 * Description
 *      this is a fprintf like function, it generates a formatted message
 *      according to @fmt and write the message to kbox region specified by
 *      @region_id
 *
 * Note
 *      if error occurs in message formatting, the kbox_printf writes a message
 *      with content "data truncated in kbox_printf\n" to the region
 */
extern void __attribute__((format(printf, 2, 3)))
kbox_printf(int region_id, const char *fmt, ...);

/*
 * Description
 *      read the whole kbox region content to buffer @buf
 *
 * Return Value
 * >= 0          the bytes of data read
 * E_HM_INVAL    the @region_id is negative, or there is no kbox region opened
 *               with @region_id, or @buf is NULL
 * E_HM_FBIG     buffer size @size is smaller than the region data size
 * E_HM_RESFAULT the kbox region state is not correct
 *
 * Note
 *      kbox_read reads the whole region buffer at once, make sure to allocate
 *      a big enough buffer before calling this function. The region size can
 *      be read with kbox_region_size() function.
 */
ssize_t kbox_read(int region_id, char *buf, size_t size);


/*
 * Description
 *      read and clear the whole kbox region
 *
 * Return Value
 *       same as kbox_read
 *
 * Note
 *      similar to SYSLOG_ACTION_READ_CLEAR, kbox_read_clear does not really
 *      clear the content in the kbox region. Rather, it updates a variable
 *      in the kbox header that records where the last read ended.
 */
ssize_t kbox_read_clear(int region_id, char *buf, size_t size);

/*
 * Description
 *      erase the region content
 *
 * Return Value
 * E_HM_OK       success
 * E_HM_INVAL    the @region_id is negative or there is no kbox region opened
 *               with this id
 */
int kbox_erase(int region_id);

/*
 * Description
 *      get the kbox region size
 *
 * Return Value
 * > 0           the region size
 * E_HM_INVAL    the @region_id is negative or there is no kbox region opened
 *               with this id
 */
ssize_t kbox_region_size(int region_id);

/*
 * Description
 *      return string describing error number
 *
 * Return Value
 *      the error string
 */

ssize_t kbox_read_raw(const char *region_name, char *buf, size_t size, bool clear_data);
int kbox_erase_raw(const char *region_name);
const char *kbox_err_msg(int err);

#endif
