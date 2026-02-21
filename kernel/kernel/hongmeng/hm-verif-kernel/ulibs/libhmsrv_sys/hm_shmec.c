/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: shmec function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 7 19:47:41 2022
 */
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <hmasm/page.h>
#include <libhmlog/hmlog.h>
#include <libmem/utils.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_shmec.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_timer.h>

#ifdef CONFIG_SHMEC
#define DEFAULT_RECV_BUFFER_SIZE	(CONFIG_SHMEC_RECV_BUFFER_PAGE_NUM * PAGE_SIZE)
#define SHMEC_RB_BLOCKS			(2U)
#define __EC_EP_SHMEC			__ec_ep_type_to_flags(__EC_EP_TYPE_SHMEC)
#define SHMEC_MIN_SEND_EVENT		(8U)
#define SHEMC_RECV_TIMEOUT		300
#define SHMEC_RECV_TRY_TIMES		2

struct shmec_ep {
	cref_t ep_cref;
	void *buf;
	uint32_t len;
	int futex;
	struct rbuffer_s rb;
};

int hm_shmec_attach(const struct shmec_ep *endpoint, const char *chn, int flag, mode_t mode)
{
	int ret = E_HM_INVAL;

	if ((endpoint != NULL) && (chn != NULL) && (strlen(chn) < EC_CHN_NAME_MAX_LEN)) {
		ret = actvcall_hmcall_ec_attach_chn(endpoint->ep_cref, chn, strlen(chn), flag, mode);
		if (ret != E_HM_OK) {
			hm_warn("attach %s channel failed:%s, flag:%d, mode:%u\n", chn, hmstrerror(ret), flag, (uint32_t)mode);
		}
	}

	return ret;
}

int hm_shmec_detach(const struct shmec_ep *endpoint)
{
	int ret = E_HM_INVAL;

	if (endpoint != NULL) {
		ret = actvcall_hmcall_ec_detach_chn(endpoint->ep_cref);
		if (ret != E_HM_OK) {
			hm_warn("detach channel failed, err:%s\n", hmstrerror(ret));
		}
	}

	return ret;
}

int hm_shmec_send_event(const struct shmec_ep *endpoint, const void *buf, size_t size)
{
	int ret = E_HM_INVAL;

	/*
	 * Currently, the shmec only supports event size more than 8 bytes
	 * due to the rbuffer restriction.
	 */
	if ((endpoint != NULL) && (buf != NULL) && (size >= SHMEC_MIN_SEND_EVENT)) {
		ret = syscap_EcEpSendEvent(endpoint->ep_cref, buf, size);
		if (ret != E_HM_OK) {
			hm_warn("send event failed:%s, size:%u\n", hmstrerror(ret), (uint32_t)size);
		}
	}

	return ret;
}

static int recv_data_in_rb(struct shmec_ep *endpoint, void *buf, size_t size)
{
	int i;
	int ret = 0;
	/*
	 * If writer writes to next block, the first read will get
	 * nothing and move to the next block. So we should try again.
	 */
	for (i = 0; i < SHMEC_RECV_TRY_TIMES; i++) {
		ret = (int)rb_try_read_entry(&(endpoint->rb), buf, size, NULL);
		if (ret > 0) {
			break;
		}
	}

	return ret;
}

static int recv_event(struct shmec_ep *endpoint, void *buf, size_t size, bool need_wait)
{
	int ret = 0;
	int err;
	struct timespec timeout = {
		.tv_sec = SHEMC_RECV_TIMEOUT,
		.tv_nsec = 0,
	};

	do {
		ret = recv_data_in_rb(endpoint, buf, size);
		if (ret > 0) {
			break;
		}

		if (need_wait) {
			err = hm_futex_wait_interruptible(&(endpoint->futex), 0, &timeout, 0);
			if (err == E_HM_INTR) {
				break;
			}
			endpoint->futex = 0;
		}
	} while (need_wait);

	if (ret == 0) {
		ret = E_HM_FAULT;
	}

	return ret;
}

int hm_shmec_try_recv_event(struct shmec_ep *endpoint, void *buf, size_t size)
{
	int ret = E_HM_INVAL;

	if (endpoint != NULL) {
		ret = recv_event(endpoint, buf, size, false);
	}

	return ret;
}

int hm_shmec_recv_event(struct shmec_ep *endpoint, void *buf, size_t size)
{
	int ret = E_HM_INVAL;

	if (endpoint != NULL) {
		ret = recv_event(endpoint, buf, size, true);
	}

	return ret;
}

static int mmap_recv_buf(void **buf, uint32_t len)
{
	int ret = E_HM_OK;

	*buf = hm_mem_mmap(NULL, len, PROT_READ | PROT_WRITE,
				  MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE | MFLAG_NOCOW | MAP_LOCKED, 0, 0);
	if (*buf == MAP_FAILED) {
		hm_warn("mmap buf failed, size:%u\n", len);
		ret = E_HM_NOMEM;
	}

	return ret;
}

static struct shmec_ep *shmec_new_endpoint(uint64_t flags, uint32_t len)
{
	int ret = E_HM_OK;
	void *buf = NULL;
	struct shmec_ep *endpoint = NULL;
	struct __ec_ep_args args = { 0 };
	struct __actvret_hmcall_ec_create_ep create_ret;

	if ((flags & (__EC_EP_SRC | __EC_EP_DST)) == 0) {
		hm_debug("flags is invalid: %lu\n", flags);
		ret = E_HM_INVAL;
	} else {
		endpoint = hm_mem_mmap(NULL, sizeof(struct shmec_ep), PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE | MFLAG_NOCOW | MAP_LOCKED, 0, 0);
		if (endpoint == MAP_FAILED) {
			hm_warn("mmap shmec_ep failed\n");
			ret = E_HM_NOMEM;
		}
	}

	/* Only dst endpoint need prepare receive event buffer. */
	if (ret == E_HM_OK && (flags & __EC_EP_DST)) {
		len = PAGE_ALIGN_UP(len);
		ret = mmap_recv_buf(&buf, len);
		if (ret == E_HM_OK) {
			ret = rb_init(&(endpoint->rb), buf, len, SHMEC_RB_BLOCKS, RB_RDONLY);
			if (ret != E_HM_OK) {
				hm_warn("init ring buffer failed\n");
				ret = E_HM_FAULT;
			}
		}
	}

	if (ret == E_HM_OK) {
		args.flags = __EC_EP_SHMEC | (flags & (__EC_EP_SRC | __EC_EP_DST));
		args.shmec_args.buf = (__uptr_t)ptr_to_ulong(buf);
		args.shmec_args.len = len;
		args.shmec_args.futex = &(endpoint->futex);

		ret = actvcall_hmcall_ec_create_ep(&args, &create_ret);
		if (ret != E_HM_OK) {
			hm_warn("create endpoint failed, err:%s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		endpoint->ep_cref = create_ret.ep_cref;
		endpoint->buf = buf;
		endpoint->len = len;
	} else {
		if (buf != NULL) {
			(void)hm_mem_munmap(buf, len);
		}

		if (endpoint != NULL) {
			(void)hm_mem_munmap(endpoint, sizeof(struct shmec_ep));
			endpoint = NULL;
		}
	}

	return endpoint;
}

struct shmec_ep *hm_shmec_create_endpoint(uint64_t flags)
{
	return shmec_new_endpoint(flags, DEFAULT_RECV_BUFFER_SIZE);
}

int hm_shmec_delete_endpoint(struct shmec_ep *endpoint)
{
	int ret;

	if (endpoint != NULL) {
		ret = actvcall_hmcall_ec_destroy_ep(endpoint->ep_cref);
		if (ret != E_HM_OK) {
			hm_warn("destroy endpoint failed, err:%s\n", hmstrerror(ret));
		}

		if (endpoint->buf != NULL) {
			ret = hm_mem_munmap(endpoint->buf, endpoint->len);
			if (ret != E_HM_OK) {
				hm_warn("mem unmap failed, err=%s\n", hmstrerror(ret));
			}
		}

		(void)hm_mem_munmap(endpoint, sizeof(struct shmec_ep));
	}

	return E_HM_OK;
}

#else
int hm_shmec_attach(const struct shmec_ep *endpoint, const char *chn, int flag, mode_t mode)
{
	UNUSED(endpoint, chn, flag, mode);
	return E_HM_NOSYS;
}

int hm_shmec_detach(const struct shmec_ep *endpoint)
{
	UNUSED(endpoint);
	return E_HM_NOSYS;
}

int hm_shmec_send_event(const struct shmec_ep *endpoint, const void *buf, size_t size)
{
	UNUSED(endpoint, buf, size);
	return E_HM_NOSYS;
}

int hm_shmec_recv_event(struct shmec_ep *endpoint, void *buf, size_t size)
{
	UNUSED(endpoint, buf, size);
	return E_HM_NOSYS;
}

int hm_shmec_try_recv_event(struct shmec_ep *endpoint, void *buf, size_t size)
{
	UNUSED(endpoint, buf, size);
	return E_HM_NOSYS;
}

struct shmec_ep *hm_shmec_create_endpoint(uint64_t flags)
{
	UNUSED(flags);
	return NULL;
}

int hm_shmec_delete_endpoint(struct shmec_ep *endpoint)
{
	UNUSED(endpoint);
	return E_HM_NOSYS;
}
#endif
