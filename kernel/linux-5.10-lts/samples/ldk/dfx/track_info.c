/* SPDX-License-Identifier: GPL-2.0-only */
#include <liblinux/pal.h>
#include <hongmeng/errno.h>
#include <libtrack_info/track_info.h>
#include <hmkernel/ctrlmem/sysconf.h>

const char *liblinux_track_info_file_name_of(__u8 type)
{
	return hm_track_info_file_name_of(type);
}

void *liblinux_track_info_slot_header_of(void *track_info_ptr, size_t size)
{
	return hm_track_info_slot_header_of(track_info_ptr, size);
}

__u8 liblinux_track_info_record_type_nr_of(void *slot_header_ptr)
{
	struct track_info_slot_header_s *slot_header;

	slot_header = (struct track_info_slot_header_s *)slot_header_ptr;

	return slot_header->types;
}

void *liblinux_track_info_region_header_of(void *slot_header_ptr, unsigned int type)
{
	struct track_info_slot_header_s *slot_header;

	slot_header = (struct track_info_slot_header_s *)slot_header_ptr;

	return hm_track_info_region_header_of(slot_header, type);
}

int liblinux_track_info_record_for_each(void *region_header_ptr,
					hm_track_info_iter_func_t func, uintptr_t arg)
{
	struct track_info_region_header_s *region_header;
	int err = 0;

	region_header = (struct track_info_region_header_s *)region_header_ptr;
	err = hm_track_info_record_for_each(region_header, func, arg);

	return -hmerrno2posix(err);
}

int liblinux_read_track_info_memory_config(uint64_t *addr, uint32_t *size)
{
       return hm_reserved_memory_read_config(__CTRLMEM_SYSCONF_RESERV_MEM_TRACKINFO, addr, size);
}
