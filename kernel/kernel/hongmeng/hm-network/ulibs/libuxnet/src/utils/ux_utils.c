/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interface for network utils
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 10:43:52 2019
 */

#include <arpa/inet.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libmem/utils.h>
#include <limits.h>

#include "ux_utils.h"
#include "ux_netlog.h"

#define MAC_ADDR_SIZE	6

void ux_eth_random_addr(unsigned char *addr, size_t addr_size)
{
	struct timespec ts = {0};
	int i;
	int ret = 0;

	if (addr_size < MAC_ADDR_SIZE) {
		net_error(HMNET_CORE, "addr size too short!\n");
		return;
	}

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	if (ret != E_HM_OK) {
		net_debug(HMNET_CORE, "clock get time error\n");
		return;
	}
	srandom((unsigned int)ts.tv_nsec + (unsigned int)ts.tv_sec * NSEC_PER_SEC);

	for (i = 0; i < MAC_ADDR_SIZE; i++) {
		addr[i] = (unsigned char)random();
	}
	addr[0] &= 0xfe; /* clear multicast bit */
	addr[0] |= 0x02; /* set local assignment bit (IEEE802) */
}

int ux_sem_init(struct raw_sem *sem, unsigned int count)
{
	return raw_sem_init(sem, count);
}

void ux_sem_signal(struct raw_sem *sem)
{
	(void)raw_sem_post(sem);
}

int ux_sem_cond_waiti(struct raw_sem *sem, time_t timeout)
{
	struct timespec rtime1 = {0};
	struct timespec rtime2 = {0};
	struct timespec ts = {0};
	int ret;

	/* when timeout opt < 0 and setsockopt func set sk->rcvtimeo=0, it means DONTWAIT */
	if (timeout == 0) {
		return E_HM_AGAIN;
	}

	/* when timeout opt = 0 and setsockopt func set sk->rcvtimeo=LONG_MAX, it means BLOCK */
	if (timeout == LONG_MAX) {
		ret = raw_sem_wait(sem);
		/* should restart if socket operation interrupted without a timeout */
		if (ret == E_HM_INTR) {
			return E_HM_RESTARTSYS;
		}
		return 0;
	}

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &rtime1);
	if (ret != 0) {
		return E_HM_INVAL;
	}

	ts.tv_sec = rtime1.tv_sec + timeout / MSEC_PER_SEC;
	ts.tv_nsec = rtime1.tv_nsec + (timeout % MSEC_PER_SEC) * NSEC_PER_MSEC;
	if (ts.tv_nsec >= NSEC_PER_SEC) {
		ts.tv_sec++;
		ts.tv_nsec -= NSEC_PER_SEC;
	}

	ret = raw_sem_timedwait(sem, &ts);
	if (ret == E_HM_INTR) {
		/* Do not restart if socket operation has a timeout */
		return E_HM_INTR;
	} else if (ret == E_HM_TIMEDOUT) {
		ret = raw_sem_post(sem);
		if (ret != 0) {
			return ret;
		}
		return E_HM_TIMEDOUT;
	}

	/* Calculate for how long we waited for the cond. */
	ret = hm_clock_gettime(CLOCK_MONOTONIC, &rtime2);
	if (ret != 0) {
		return E_HM_INVAL;
	}
	ts.tv_sec = rtime2.tv_sec - rtime1.tv_sec;
	ts.tv_nsec = rtime2.tv_nsec - rtime1.tv_nsec;
	if (ts.tv_nsec < 0) {
		ts.tv_sec--;
		ts.tv_nsec += NSEC_PER_SEC;
	}
	return (int)(ts.tv_sec * MSEC_PER_SEC + ts.tv_nsec / NSEC_PER_MSEC);
}

int ux_acquire_shm_anon(uint64_t shm_key, uint32_t cnode,
			void **rvaddr, size_t *rlen, int *ret_id)
{
	size_t shm_len = 0;
	void *addr = NULL;
	int shm_id;
	int rc;

	shm_id = hm_ashm_open_auth(shm_key, cnode, 0, SHM_O_RDWR, &shm_len);
	if (shm_id < 0) {
		rc = shm_id;
		net_warn(HMNET_CORE, "failed to open shmem, rc=%d, shm_id=%"PRIu64"\n",
			 shm_id, shm_key);
	} else {
		addr = hm_mem_mmap_posix(NULL, shm_len, MPROT_READ | MPROT_WRITE,
					 MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
					 shm_id, 0, &rc);
		if (rc < 0) {
			net_warn(HMNET_CORE, "failed to mmap shmem, rc=%d, shm_id=%d\n", rc, shm_id);
			if (hm_ashm_close(shm_id) != 0) {
				net_warn(HMNET_CORE, "failed to close shmem, shm_id=%d\n", shm_id);
			}
		}
	}

	if (rc == E_HM_OK) {
		if (rvaddr != NULL) {
			*rvaddr = addr;
		}
		if (rlen != NULL) {
			*rlen = shm_len;
		}
		if (ret_id != NULL) {
			*ret_id = shm_id;
		}
	}
	return rc;
}

int ux_release_shm_anon(int32_t shm_id, void *vaddr, size_t len)
{
	int rc;

	rc = hm_mem_munmap(vaddr, len);
	if (rc < 0) {
		net_warn(HMNET_CORE, "failed to unmap shmem, rc=%d\n", rc);
	} else {
		rc = hm_ashm_close(shm_id);
		if (rc < 0) {
			net_warn(HMNET_CORE, "failed to close shmem, rc=%d\n", rc);
		}
	}

	return rc;
}

#define PREFIX_MASK		0x80000000
uint8_t mask_to_prelen(uint32_t mask)
{
	uint8_t prelen = 0;
	uint32_t pre_msk = PREFIX_MASK;

	uint32_t msk = htonl(mask);
	while (pre_msk != 0U) {
		if ((msk & pre_msk) != 0U) {
			prelen++;
		}
		pre_msk >>= 1;
	}
	return prelen;
}

/* return mask with host endian */
uint32_t prelen_to_mask(uint8_t prefix_len)
{
	uint32_t mask = 0;
	uint8_t prefix_len_r = prefix_len;

	if (prefix_len_r > PREFIX_MAX_LEN) {
		prefix_len_r = PREFIX_MAX_LEN;
	}

	uint8_t bits = PREFIX_MAX_LEN - prefix_len_r;
	while (bits--) {
		mask = (mask << 1) + 1;
	}
	mask = ~mask;

	return mask;
}

void ux_wq_head_init(struct ux_wq_head *waitqueue)
{
	if (waitqueue != NULL) {
		raw_mutex_init(&waitqueue->lock);
		dlist_init(&waitqueue->wq);
	}
}

void ux_wq_prepare_wait(struct ux_wq_head *waitqueue, struct ux_wq_entry *waiter)
{
	if (waiter != NULL && waitqueue != NULL) {
		mem_zero_s(*waiter);
		dlist_init(&waiter->node);
		waiter->wait = 0;

		raw_mutex_lock(&waitqueue->lock);
		dlist_insert(&waitqueue->wq, &waiter->node);
		raw_mutex_unlock(&waitqueue->lock);
	}
}

static int calc_timeout_tock(time_t timeout, __u64 *timeo)
{
	int err;
	struct timespec ts = {0};

	ts.tv_sec = timeout / MSEC_PER_SEC;
	ts.tv_nsec = (timeout % MSEC_PER_SEC) * NSEC_PER_MSEC;
	err = timespec_to_values(&ts, timeo);
	if (err != 0) {
		net_error(HMNET_CORE, "ux time transfer failed\n");
	}

	return err;
}

int ux_wq_do_wait(struct ux_wq_entry* waiter, time_t timeout)
{
	int ret;
	__u64 timeo;
	if (waiter == NULL) {
		return E_HM_NOMEM;
	}

	/* when timeout opt = 0 and setsockopt func set sk->rcvtimeo=LONG_MAX, it means BLOCK */
	if (timeout == LONG_MAX) {
		timeo = (__u64)-1;
	} else {
		ret = calc_timeout_tock(timeout, &timeo);
		if (ret < 0) {
			return ret;
		}
	}

	ret = hm_futex_timedwait(ptr_to_type(&waiter->wait, int *), 0, &timeo, NULL, FUTEX_TYPE_PRIVATE);
	if (ret == E_HM_AGAIN) {
		ret = 0;
	}
	if (ret == E_HM_HMSIGINTR) {
		if (timeout == LONG_MAX) {
			/* should restart if socket operation interrupted without a timeout */
			ret = E_HM_RESTARTSYS;
		} else {
			/* Do not restart if socket operation has a timeout */
			ret = E_HM_INTR;
		}
	}
	if (ret == E_HM_TIMEDOUT) {
		ret = E_HM_AGAIN;
	}
	return ret;
}

void ux_wq_finish_wait(struct ux_wq_head *waitqueue, struct ux_wq_entry* waiter)
{
	if (waiter != NULL && waitqueue != NULL) {
		raw_mutex_lock(&waitqueue->lock);
		dlist_delete(&waiter->node);
		raw_mutex_unlock(&waitqueue->lock);
	}
}

void __ux_wq_wakeup(struct ux_wq_head *waitqueue, bool all, bool sync)
{
	unsigned int futex_flags = FUTEX_TYPE_PRIVATE;
	struct ux_wq_entry *next = NULL;
	struct ux_wq_entry *waiter = NULL;

	BUG_ON(waitqueue == NULL);

	if (sync) {
		futex_flags |= FUTEX_FLAG_YIELD_SOON;
	}

	raw_mutex_lock(&waitqueue->lock);
	dlist_for_each_entry_safe(waiter, next, &waitqueue->wq, struct ux_wq_entry, node) {
		if (waiter->wait != 0) {
			continue;
		}
		(void)raw_atomic_int_cas((raw_atomic_int_t *)(void *)(&waiter->wait), 0, 1);
		(void)hm_futex_wake(&(waiter->wait), 1, futex_flags);
		if (!all) {
			break;
		}
	}
	raw_mutex_unlock(&waitqueue->lock);
	return;
}
