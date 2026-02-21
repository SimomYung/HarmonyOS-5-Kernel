/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Bunch data serialization and deserialization
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 30 17:20:23 2019
 */
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <libbunch/bunch.h>
#include <libsecure/bounds_check.h>
#include <lib/utils.h>
#include <libstrict/strict.h>

void *bunch_payload_of(const struct bunch *bunch)
{
	void *ret = NULL;

	if (bunch != NULL) {
		ret = bunch->payload;
	}

	return ret;
}

size_t bunch_size_of(const struct bunch *bunch)
{
	size_t ret = (size_t)0U;

	if (bunch != NULL) {
		ret = bunch->size;
	}
	return ret;
}

int bunch_payload_install(struct bunch *bunch, void *payload, size_t size)
{
	int err = E_HM_OK;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		bunch->payload = payload;
		bunch->size = size;
		bunch->position = 0U;
		bunch->status = BUNCH_STATUS_OK;
	}
	return err;
}

int bunch_create(struct bunch *bunch, size_t size)
{
	int err = E_HM_OK;
	void *payload = NULL;
	size_t real_size = size;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		real_size = (size == 0U) || (size > BUNCH_SIZE_MAX) ?
			BUNCH_DEF_SIZE : size;
		payload = malloc(real_size);
		if (payload == NULL) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		mem_zero_b(payload, real_size);
		mem_zero_s(*bunch);
		bunch->payload = payload;
		bunch->size = real_size;
		bunch->position = 0;
		bunch->status = BUNCH_STATUS_OK;
	}

	return err;
}

void bunch_destroy(struct bunch *bunch)
{
	int err = E_HM_OK;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (bunch->payload != NULL) {
			free(bunch->payload);
		}
		mem_zero_s(*bunch);
	}
}

static int write_data(struct bunch *bunch, const void *data, size_t size)
{
	int err = E_HM_OK;
	size_t position = bunch->position;
	size_t new_pos = position + size;

	/* check both position and new_size to avoid overflow */
	if (new_pos < position || new_pos >= bunch->size) {
		bunch->status = BUNCH_STATUS_WOF;
		err = E_HM_OVERFLOW;
	}

	if (err == E_HM_OK) {
		NOFAIL(memcpy_s(ptr_add(bunch->payload, position), size, data, size));
		bunch->position = new_pos;
	}

	return err;
}

void bunch_write_byte(struct bunch *bunch, uint8_t val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(uint8_t));
	}
}

void bunch_write_int32(struct bunch *bunch, int32_t val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(int32_t));
	}
}

void bunch_write_uint32(struct bunch *bunch, uint32_t val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(uint32_t));
	}
}

void bunch_write_int64(struct bunch *bunch, int64_t val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(int64_t));
	}
}

void bunch_write_uint64(struct bunch *bunch, uint64_t val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(uint64_t));
	}
}

void bunch_write_uintptr(struct bunch *bunch, uintptr_t val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(uintptr_t));
	}
}

void bunch_write_bool(struct bunch *bunch, bool val)
{
	if (bunch != NULL) {
		(void)write_data(bunch, (const void *)&val, sizeof(bool));
	}
}

void bunch_write_string(struct bunch *bunch, const char *s)
{
	uint32_t len;
	int err = E_HM_OK;

	if (bunch == NULL || s == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		len = (uint32_t)strlen(s) + 1U;
		err = write_data(bunch, (const void *)&len, sizeof(uint32_t));
		if (err == E_HM_OK) {
			(void)write_data(bunch, (const void *)s, (size_t)len);
		}
	}
}

static int read_data(struct bunch *bunch, void *data, size_t size)
{
	size_t position = bunch->position;
	size_t new_pos = position + size;
	int err = E_HM_OK;

	if (new_pos < position || new_pos >= bunch->size) {
		bunch->status = BUNCH_STATUS_ROF;
		err = E_HM_OVERFLOW;
	}

	if (err == E_HM_OK) {
		NOFAIL(memcpy_s(data, size, ptr_add(bunch->payload, position), size));
		bunch->position = new_pos;
	}

	return err;
}

uint8_t bunch_read_byte(struct bunch *bunch)
{
	uint8_t val = 0;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(uint8_t));
	}

	return val;
}

int32_t bunch_read_int32(struct bunch *bunch)
{
	int32_t val = 0;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(int32_t));
	}

	return val;
}

uint32_t bunch_read_uint32(struct bunch *bunch)
{
	uint32_t val = 0;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(uint32_t));
	}

	return val;
}

int64_t bunch_read_int64(struct bunch *bunch)
{
	int64_t val = 0;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(int64_t));
	}

	return val;
}

uint64_t bunch_read_uint64(struct bunch *bunch)
{
	uint64_t val = 0;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(uint64_t));
	}

	return val;
}

uintptr_t bunch_read_uintptr(struct bunch *bunch)
{
	uintptr_t val = 0;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(uintptr_t));
	}

	return val;
}

bool bunch_read_bool(struct bunch *bunch)
{
	bool val = false;

	if (bunch != NULL) {
		(void)read_data(bunch, (void *)&val, sizeof(bool));
	}

	return val;
}

const char *bunch_read_string(struct bunch *bunch)
{
	const char *s = NULL;
	int err = E_HM_OK;
	uint32_t size = 0U;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = read_data(bunch, (void *)&size, sizeof(uint32_t));
	}

	if (err == E_HM_OK) {
		if (SIZE_MAX - bunch->position < (size_t)size || bunch->position + (size_t)size > bunch->size ||
		    bunch->size < (size_t)size) {
			hm_warn("bunch read string failed: overflow\n");
			bunch->status |= BUNCH_STATUS_POF;
			err = E_HM_OVERFLOW;
		}
	}

	if (err == E_HM_OK) {
		bunch->position = array_index_fixup(bunch->position, (bunch->size - (size_t)size));
		s = (const char *)ptr_add(bunch->payload, bunch->position);
		bunch->position += (size_t)size;
	}

	return s;
}

static void override_data(struct bunch *bunch, size_t position,
			  const void *data, size_t size, bool blob)
{
	size_t current_pos;
	int err = E_HM_OK;

	current_pos = bunch->position;
	if (current_pos < position) {
		bunch->status |= BUNCH_STATUS_POF;
	} else {
		bunch->position = position;
		if (blob) {
			int32_t data_size = (int32_t)(ssize_t)(size);
			err = write_data(bunch, (const void *)&data_size, sizeof(int32_t));
		}

		if (err == E_HM_OK) {
			err = write_data(bunch, data, size);
		}

		if (err == E_HM_OK) {
			bunch->position = current_pos;
		}
	}
}

void bunch_override_byte(struct bunch *bunch, size_t position, uint8_t val)
{
	if (bunch != NULL) {
		override_data(bunch, position, (const void *)&val,
			      sizeof(uint8_t), false);
	}
}

void bunch_override_int32(struct bunch *bunch, size_t position, int32_t val)
{
	if (bunch != NULL) {
		override_data(bunch, position, (const void *)&val,
			      sizeof(int32_t), false);
	}
}

void bunch_override_uint32(struct bunch *bunch, size_t position, uint32_t val)
{
	if (bunch != NULL) {
		override_data(bunch, position, (const void *)&val,
			      sizeof(uint32_t), false);
	}
}

void bunch_override_int64(struct bunch *bunch, size_t position, int64_t val)
{
	if (bunch != NULL) {
		override_data(bunch, position, (const void *)&val,
			      sizeof(int64_t), false);
	}
}

void bunch_override_bool(struct bunch *bunch, size_t position, bool val)
{
	if (bunch != NULL) {
		override_data(bunch, position, (const void *)&val,
			      sizeof(bool), false);
	}
}

void bunch_override_string(struct bunch *bunch, size_t position,
			   const char *s)
{
	size_t len;

	if (bunch != NULL && s != NULL) {
		len = strlen(s) + 1U;
		override_data(bunch, position, (const void *)s, len, true);
	}
}

void bunch_override_add_int32(struct bunch *bunch, size_t position,
			      int32_t val)
{
	int err = E_HM_OK;
	size_t current_pos;
	int32_t *data = NULL;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		current_pos = bunch->position;
		if (current_pos < position) {
			hm_warn("override position greater than current\n");
			bunch->status |= BUNCH_STATUS_POF;
		} else {
			position = array_index_fixup(position, current_pos);
			data = (int32_t *)ptr_add(bunch->payload, position);
			*data += val;
			bunch->position = current_pos;
		}
	}
}

void bunch_override_add_int64(struct bunch *bunch, size_t position,
			      int64_t val)
{
	int err = E_HM_OK;
	size_t curr_pos;
	int64_t *data = NULL;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		curr_pos = bunch->position;
		if (curr_pos < position) {
			hm_warn("override position greater than current\n");
			bunch->status |= BUNCH_STATUS_POF;
		} else {
			position = array_index_fixup(position, curr_pos);
			data = (int64_t *)ptr_add(bunch->payload, position);
			*data += val;
			bunch->position = curr_pos;
		}
	}
}

void bunch_seek(struct bunch *bunch, size_t position)
{
	int err = E_HM_OK;

	if (err == E_HM_OK && position >= bunch->size) {
		err = E_HM_NOMEM;
	}

	if (err == E_HM_OK) {
		bunch->position = position;
	}
}

size_t bunch_get_position(const struct bunch *bunch)
{
	size_t pos;

	if (bunch == NULL) {
		pos = 0UL;
	} else {
		pos = bunch->position;
	}

	return pos;
}

size_t bunch_get_capacity(const struct bunch *bunch)
{
	size_t capacity;

	if (bunch == NULL) {
		capacity = 0UL;
	} else {
		capacity = bunch->size;
	}

	return capacity;
}

void bunch_set_private_data(struct bunch *bunch, void *private_data)
{
	if (bunch != NULL) {
		bunch->private_data = private_data;
	}
}

void *bunch_get_private_data(struct bunch *bunch)
{
	void *data = NULL;

	if (bunch != NULL) {
		data = bunch->private_data;
	}

	return data;
}

void bunch_duplicate(struct bunch *bunch, const struct bunch *origin)
{
	int err = E_HM_OK;

	if (bunch == NULL || origin == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_assign_s(*bunch, *origin);
	}
}

bool bunch_is_empty(const struct bunch *bunch)
{
	return (bunch == NULL) || (bunch->size == 0UL);
}

bool bunch_is_healthy(const struct bunch *bunch)
{
	bool rc = false;

	if (bunch == NULL) {
		rc = false;
	} else {
		rc = (bunch->status == BUNCH_STATUS_OK);
	}

	return rc;
}

unsigned int bunch_get_status(const struct bunch *bunch)
{
	unsigned int rc;

	if (bunch == NULL) {
		rc = BUNCH_STATUS_INVALID;
	} else {
		rc = bunch->status;
	}

	return rc;
}
