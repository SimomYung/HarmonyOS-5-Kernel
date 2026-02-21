/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: security service communication
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 10:10:16 2023
 */
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <libsec/service_communication.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>

#define HM_SECS_DATA_NEED_UNLINK_BIT	1u
#define HM_SECS_DATA_NEED_UNLINK	(1u << HM_SECS_DATA_NEED_UNLINK_BIT)

static int _hm_secs_data_make(struct hm_secs_data *data,
			     const void *buf, size_t size)
{
	int ret = E_HM_OK;
	int shm_id = 0;
	void *addr = MAP_FAILED;

	shm_id = hm_ashm_open_auth(0, 0U, size, SHM_O_RDWR | SHM_O_CREAT, NULL);
	if (shm_id < 0) {
		ret = shm_id;
	}
	if (ret == E_HM_OK) {
		addr = hm_mem_mmap(NULL, size, PROT_READ | PROT_WRITE,
				   MAP_SHARED | MAP_ANON, shm_id, 0);
		if (addr == MAP_FAILED) {
			ret = E_HM_NOMEM;
		}
	}
	if (ret == E_HM_OK && buf != NULL) {
		ret = memcpy_s(addr, size, buf, size);
		if (ret != EOK) {
			ret = E_HM_FAULT;
		}
	}
	if (ret == E_HM_OK) {
		mem_zero_s(data->desc);
		data->addr = addr;
		data->size = size;
		data->shm_desc = shm_id;
		data->flags = HM_SECS_DATA_NEED_UNLINK;
	} else {
		if (addr != MAP_FAILED) {
			(void)hm_mem_munmap(addr, size);
		}
		if (shm_id >= 0) {
			(void)hm_ashm_close(shm_id);
			(void)hm_ashm_unlink(data->shm_desc);
		}
	}
	return ret;
}

int hm_secs_data_make(struct hm_secs_data *data,
		      const void *buf, size_t size)
{
	int ret = E_HM_OK;

	if (data == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		if (size == 0) {
			mem_zero_s(*data);
		} else {
			ret = _hm_secs_data_make(data, buf, size);
		}
	}
	return ret;
}

int hm_secs_data_sendto(struct hm_secs_data *data, xref_t ref)
{
	int ret = E_HM_OK;
	uint64_t key = 0;

	if (data == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK && data->size != 0) {
		ret = hm_ashm_grant(data->shm_desc, SHM_GRANT_RDONLY, ref, &key);
	}
	if (ret == E_HM_OK) {
		data->desc.shm_key = key;
		data->desc.size = data->size;
	}
	return ret;
}

static int _hm_secs_data_recvfrom(struct hm_secs_data *data,
				  const struct hm_secs_data_desc *desc,
				  uint32_t cidx)
{
	int ret = E_HM_OK;
	int shm_id = -1;
	void *addr = NULL;
	size_t size = 0;

	shm_id = hm_ashm_open_auth(desc->shm_key, cidx, 0, SHM_O_RDONLY, &size);
	if (shm_id < 0) {
		ret = shm_id;
	}
	if (ret == E_HM_OK) {
		if (size < desc->size) {
			ret = E_HM_RANGE;
		}
	}
	if (ret == E_HM_OK) {
		addr = hm_mem_mmap(NULL, size, PROT_READ, MAP_SHARED | MAP_ANON, shm_id, 0);
		if (addr == MAP_FAILED) {
			ret = E_HM_NOMEM;
		}
	}
	if (ret == E_HM_OK) {
		data->desc = *desc;
		data->addr = addr;
		data->size = (size_t)desc->size;
		data->flags = 0u;
		data->shm_desc = shm_id;
	} else {
		if (shm_id > 0) {
			(void)hm_ashm_close(shm_id);
		}
	}
	return ret;
}

int hm_secs_data_recvfrom(struct hm_secs_data *data,
			  const struct hm_secs_data_desc *desc,
			  uint32_t cidx)
{
	int ret = E_HM_OK;

	if (data == NULL || desc == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		if (desc->shm_key == 0) {
			mem_zero_s(*data);
		} else {
			ret = _hm_secs_data_recvfrom(data, desc, cidx);
		}
	}

	return ret;
}

void hm_secs_data_destroy(struct hm_secs_data *data)
{
	if (data != NULL && data->size != 0u) {
		(void)hm_mem_munmap(data->addr, data->size);
		(void)hm_ashm_close(data->shm_desc);
		if (____IS(data->flags & HM_SECS_DATA_NEED_UNLINK)) {
			(void)hm_ashm_unlink(data->shm_desc);
		}
		mem_zero_s(*data);
	}
}
