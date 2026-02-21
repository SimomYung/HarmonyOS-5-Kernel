/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBDH_LINUX_DFX_TRACK_INFO_H__
#define __LIBDH_LINUX_DFX_TRACK_INFO_H__

typedef int (*hm_track_info_iter_func_t)(const char *buf, unsigned int len, uintptr_t arg);
const char *liblinux_track_info_file_name_of(__u8 type);
void *liblinux_track_info_slot_header_of(void *track_info_ptr, size_t size, bool dump_active);
__u8 liblinux_track_info_record_type_nr_of(void *slot_header);
void *liblinux_track_info_region_header_of(void *slot_header, unsigned int type);
int liblinux_track_info_record_for_each(void *region_header,
					hm_track_info_iter_func_t func, uintptr_t arg);
int liblinux_read_track_info_memory_config(uint64_t *addr, uint32_t *size);

#endif /* __LIBDH_LINUX_DFX_TRACK_INFO_H__ */
