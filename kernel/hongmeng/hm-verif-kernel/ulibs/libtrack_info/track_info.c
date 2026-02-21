/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: track_info parse function
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan  8 03:49:59 PM 2024
 */
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>
#include <libtrack_info/track_info.h>

#define TRACK_INFO_MAX_DATA_LEN	128U

#ifdef CONFIG_TRACK_INFO
static const char *track_info_filenames[] = {
#define TRACK_INFO_FILE_NAME(...) __VA_ARGS__
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, args, strs, assigns, parse, file_name, ...)	\
	[RECORD_##RECORD_TYPE] = file_name,
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
#undef TRACK_INFO_FILE_NAME
};

const char *hm_track_info_file_name_of(__u8 record_type)
{
	const char *filename = NULL;

	if (record_type < RECORD_TYPE_NR) {
		filename = track_info_filenames[record_type];
	}

	return filename;
}

/* generate track info parse function */
#define NSEC_PER_SEC 1000000000
#define TRACK_INFO_PARSE(...) __VA_ARGS__
#define TRACK_PARSE_GEN(record_type, fmt, ...)								\
static int parse_##record_type##_record(struct record_common_s *record_common, unsigned int cpu_id,	\
					char *buf, size_t size)						\
{													\
	struct record_type##_record_s *record = (struct record_type##_record_s *)record_common;		\
	return sprintf_s(buf, size, "[%03u] " fmt, cpu_id, ##__VA_ARGS__);				\
}
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, args, strs, assigns, parse, ...)	\
	TRACK_PARSE_GEN(record_type, parse)
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO

typedef int (*track_info_parse_fn_s)(struct record_common_s *record_common,
				     unsigned int cpu_id, char *buf, size_t size);
static const track_info_parse_fn_s track_info_parse_fns[RECORD_TYPE_NR] = {
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, ...)	\
	[RECORD_##RECORD_TYPE] = parse_##record_type##_record,
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
};

static bool is_track_info_header_invalid(struct track_info_header_s *info_header, size_t size)
{
	bool invalid = true;

	if (is_track_info_header_csum_invalid(info_header)) {
		hm_info("track_info: track_info_header csum doesn't match\n");
	} else if (info_header->magic != TRACK_INFO_HEADER_MAGIC) {
		hm_info("track_info: magic 0x%x/0x%x doesn't match\n",
			info_header->magic, TRACK_INFO_HEADER_MAGIC);
	} else if (info_header->size != size) {
		hm_info("track_info: size 0x%x/0x%zx doesn't match\n",
			info_header->size, size);
	} else if (info_header->curr_slot >= NR_SLOTS) {
		hm_info("track_info: curr_slot %u is invalid\n",
			info_header->curr_slot);
	} else {
		invalid = false;
	}

	return invalid;
}

static struct track_info_slot_header_s *
track_info_slot_header_of(struct track_info_header_s *info_header)
{
	unsigned int slot_id = info_header->curr_slot == SLOT_ID_A
			       ? SLOT_ID_B : SLOT_ID_A;

	return ptr_cast(ptr_add(info_header->data, info_header->slot_offset[slot_id]),
			struct track_info_slot_header_s);
}

static bool is_track_info_slot_header_invalid(struct track_info_slot_header_s *slot_header, size_t size)
{
	bool invalid = true;

	if (is_track_info_slot_header_csum_invalid(slot_header)) {
		hm_info("track_info: track_info_slot_header csum doesn't match\n");
	} else if (slot_header->magic != TRACK_INFO_SLOT_HEADER_MAGIC) {
		hm_info("track_info: magic 0x%x/0x%x doesn't match\n",
			slot_header->magic, TRACK_INFO_SLOT_HEADER_MAGIC);
	} else if (slot_header->size != size) {
		hm_info("track_info: size 0x%x/0x%zx doesn't match\n",
			slot_header->size, size);
	} else {
		invalid = false;
	}

	return invalid;
}

struct track_info_slot_header_s *
hm_track_info_slot_header_of(void *track_info_ptr, size_t size)
{
	struct track_info_slot_header_s *slot_header = NULL;
	size_t track_info_size = size - sizeof(struct track_info_header_s);
	if (track_info_ptr != NULL) {
		struct track_info_header_s *info_header = ptr_cast(track_info_ptr,
								struct track_info_header_s);
		if (!is_track_info_header_invalid(info_header, track_info_size)) {
			slot_header = track_info_slot_header_of(info_header);
			size_t track_info_slot_size = info_header->slot_size -
						sizeof(struct track_info_slot_header_s);
			if (is_track_info_slot_header_invalid(slot_header, track_info_slot_size)) {
				slot_header = NULL;
			}
		}
	}

	return slot_header;
}

static struct track_info_region_header_s *
track_info_region_header_of(struct track_info_slot_header_s *slot_header, unsigned int type)
{
	return ptr_cast(ptr_add(slot_header->data, slot_header->region_offset[type]),
			struct track_info_region_header_s);
}

static bool is_track_info_region_header_invalid(struct track_info_region_header_s *region_header,
						unsigned int type, size_t size)
{
	bool invalid = true;

	if (is_track_info_region_header_csum_invalid(region_header)) {
		hm_info("track_info: track_info_region_header csum doesn't match\n");
	} else if (region_header->magic != TRACK_INFO_REGION_HEADER_MAGIC) {
		hm_info("track_info: magic 0x%x/0x%x doesn't match\n",
			region_header->magic, TRACK_INFO_REGION_HEADER_MAGIC);
	} else if (region_header->size != size) {
		hm_info("track_info: size 0x%x/0x%zx doesn't match\n",
			region_header->size, size);
	} else if (region_header->type != type) {
		hm_info("track_info: type %u/%u doesn't match\n",
			region_header->type, type);
	} else if (region_header->type >= RECORD_TYPE_NR) {
		hm_info("track_info: type %u is invalid\n", region_header->type);
	} else {
		invalid = false;
	}

	return invalid;
}

struct track_info_region_header_s *
hm_track_info_region_header_of(struct track_info_slot_header_s *slot_header, unsigned int type)
{
	struct track_info_region_header_s *region_header = NULL;

	if (slot_header != NULL && type < slot_header->types) {
		region_header = track_info_region_header_of(slot_header, type);
		size_t track_info_region_size = slot_header->region_size[type] -
						sizeof(struct track_info_region_header_s);
		if (is_track_info_region_header_invalid(region_header, type,
							track_info_region_size)) {
			region_header = NULL;
		}
	}

	return region_header;
}

static struct track_info_rb_header_s *
track_info_first_rb_header_of(struct track_info_region_header_s *region_header)
{
	return ptr_cast(ptr_add(region_header->data, region_header->rb_offset),
			struct track_info_rb_header_s);
}

static struct track_info_rb_header_s *
track_info_next_rb_header_of(struct track_info_rb_header_s *rb_header)
{
	return ptr_cast(ptr_add(rb_header->data, rb_header->size),
			struct track_info_rb_header_s);
}

static bool is_track_info_rb_header_invalid(struct track_info_rb_header_s *rb_header,
					    unsigned int cpu_id)
{
	bool invalid = true;

	if (is_track_info_rb_header_csum_invalid(rb_header)) {
		hm_info("track_info: track_info_rb_header csum doesn't match\n");
	} else if (rb_header->magic != TRACK_INFO_RB_HEADER_MAGIC) {
		hm_info("track_info: magic 0x%x/0x%x doesn't match\n",
			rb_header->magic, TRACK_INFO_RB_HEADER_MAGIC);
	} else if (rb_header->cpu_id != cpu_id) {
		hm_info("track_info: cpu_id %u/%u doesn't match\n",
			rb_header->cpu_id, cpu_id);
	} else if (rb_header->w_index >= rb_header->max_count) {
		hm_info("track_info: w_index %u exceed max_count %u\n",
			rb_header->w_index, rb_header->max_count);
	} else if (rb_header->r_index >= rb_header->max_count) {
		hm_info("track_info: r_index %u exceed max_count %u\n",
			rb_header->r_index, rb_header->max_count);
	} else if (rb_header->count > rb_header->max_count) {
		hm_info("track_info: count %u exceed max_count %u\n",
			rb_header->count, rb_header->max_count);
	} else {
		invalid = false;
	}

	return invalid;
}

static void *track_info_rb_read(struct track_info_rb_header_s *rb_header)
{
	void *entry = NULL;

	if (rb_header->count >= rb_header->max_count) {
		rb_header->r_index = rb_header->w_index;
	}

	if (rb_header->r_index >= rb_header->max_count) {
		rb_header->r_index = 0;
	}

	entry = ptr_add(rb_header->data,
			rb_header->offset + rb_header->r_index * rb_header->field_size);

	rb_header->count--;
	rb_header->r_index++;

	return entry;
}

static int track_info_record_iter(struct track_info_rb_header_s *rb_header,
				  unsigned int type,
				  hm_track_info_iter_func_t func, uintptr_t arg)
{
	struct record_common_s *record_common = NULL;
	char buf[TRACK_INFO_MAX_DATA_LEN] = {};
	void *entry = NULL;
	int err = E_HM_OK;
	int ret = 0;

	while (err == E_HM_OK) {
		if (rb_header->count == 0) {
			break;
		}

		entry = track_info_rb_read(rb_header);
		record_common = ptr_cast(entry, struct record_common_s);
		int len = track_info_parse_fns[type](record_common, rb_header->cpu_id,
						     buf, sizeof(buf));
		if (len <= 0) {
			err = E_HM_INVAL;
			break;
		}

		ret = func(buf, (unsigned int)len, arg);
		if (ret < 0) {
			err = E_HM_INVAL;
			break;
		}
	}

	return err;
}

int hm_track_info_record_for_each(struct track_info_region_header_s *region_header,
				  hm_track_info_iter_func_t func, uintptr_t arg)
{
	struct track_info_rb_header_s *rb_header = NULL;
	int err = E_HM_OK;
	if (region_header == NULL) {
		err = E_HM_INVAL;
	} else {
		rb_header = track_info_first_rb_header_of(region_header);
		for (unsigned int cpu_id = 0; cpu_id < region_header->cpus; cpu_id++) {
			if (is_track_info_rb_header_invalid(rb_header, cpu_id)) {
				err = E_HM_INVAL;
				break;
			}

			err = track_info_record_iter(rb_header, region_header->type,
				func, arg);
			if (err != E_HM_OK) {
				break;
			}

			rb_header = track_info_next_rb_header_of(rb_header);
		}
	}

	return err;
}
#else
const char *hm_track_info_file_name_of(__u8 record_type)
{
	UNUSED(record_type);
	return NULL;
}

struct track_info_slot_header_s *
hm_track_info_slot_header_of(void *track_info_ptr, size_t size)
{
	UNUSED(track_info_ptr, size);
	return NULL;
}

struct track_info_region_header_s *
hm_track_info_region_header_of(struct track_info_slot_header_s *slot_header, unsigned int type)
{
	UNUSED(slot_header, type);
	return NULL;
}

int hm_track_info_record_for_each(struct track_info_region_header_s *region_header,
				  hm_track_info_iter_func_t func, uintptr_t arg)
{
	UNUSED(region_header, func, arg);
	return E_HM_NOSYS;
}
#endif /* CONFIG_TRACK_INFO */
