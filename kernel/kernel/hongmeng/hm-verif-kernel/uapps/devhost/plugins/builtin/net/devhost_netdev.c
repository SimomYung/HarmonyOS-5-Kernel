/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Net device support
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 31 12:18:25 2020
 */

#include "devhost_netdev.h"

#include <devhost_api.h>
#include <hmasm/page.h>
#include <sys/mman.h>
#include <hmnet/hmnet_devices.h>
#include <libhmsrv_io/io_net_sring.h>
#include <libcrt/hmcrt.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libsysif/devhost/server.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_sem.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_thread.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmshmalloc/shm_allocator.h>
#include <libhwsecurec/securec.h>

#include "types.h"
#include "devhost.h"
#include <devhost/log.h>

static int g_shm_id;
static uintptr_t g_devhost_cell_pool_va;
static unsigned long long g_devhost_cell_pool_pa;

static raw_atomic_int_t g_dh_rx_uring_inited = RAW_ATOMIC_VAL_INIT(0);
static struct io_uring *g_rx_ring = NULL;
static bool g_rx_thread_running = false;
static raw_thread_t g_rx_thread = NULL;

#define NET_CELL_POOL_UNIT_NUM (NET_CELL_POOL_LEN / NET_CELL_POOL_UNIT_SIZE)

#define HI_FEMAC_RESERVE_PBUF_CNT       64

struct netdev_statistic_devhost {
	unsigned int rx_count;
	unsigned int tx_count;
	unsigned int alloc_count;
	unsigned int free_count;
};
static struct netdev_statistic_devhost g_devhost_stat = {0, 0, 0, 0};
static raw_atomic_int_t g_devhost_pool_is_alloc = RAW_ATOMIC_VAL_INIT(0);

void devhost_netstat_inc_tx(void)
{
	g_devhost_stat.tx_count++;
}

void devhost_netstat_get(struct netdev_status *status)
{
	if (status == NULL) {
		return;
	}

	status->total_rx    = g_devhost_stat.rx_count;
	status->total_tx    = g_devhost_stat.tx_count;
	status->total_alloc = g_devhost_stat.alloc_count;
	status->total_free  = g_devhost_stat.free_count;
}

PUBLIC_SYMBOL
int devhost_native_net_rx(unsigned int pool_offset, int shm_len,
			  int rx_len, int offset, int ifindex)
{
	int err = E_HM_OK;
	unsigned int num;
	struct uring_net_sqe sqe;
	g_devhost_stat.rx_count++;

	num = devhost_dma_get_free_num();
	if (num < HI_FEMAC_RESERVE_PBUF_CNT) {
		dh_debug("free unit num is not enough, ifindex=%d, num=%u\n",
			ifindex, num);
		return E_HM_NOMEM;
	}

	if (g_rx_ring == NULL) {
		err = devhost_netcall_nic_rx(pool_offset, shm_len, rx_len, offset, ifindex);
		if (err < 0) {
			dh_error("devhost_netcall_nic_rx failed, pool_offset=0x%x\n",
				 pool_offset);
			return err;
		}
	} else {
		io_uring_prepare_net_recv(&sqe, pool_offset, offset, rx_len, ifindex);
		err = io_uring_submit_netsqe(g_rx_ring, &sqe);
		if (err < 0) {
			dh_error("submit net rx sqe failed\n");
			return err;
		}
	}

	return 0;
}

PUBLIC_SYMBOL
int devhost_native_net_rx_ex(int num, void *data, int data_size)
{
	int err = E_HM_OK;
	unsigned int free_num;

	g_devhost_stat.rx_count += (unsigned int)num;

	free_num = devhost_dma_get_free_num();
	if (free_num < HI_FEMAC_RESERVE_PBUF_CNT) {
		dh_info("free unit num is not enough, free_num=%u\n", free_num);
		return E_HM_NOMEM;
	}

	err = devhost_netcall_nic_rx_ex(num, data, data_size);
	if (err < 0) {
		dh_error("devhost_netcall_nic_rx failed\n");
		return err;
	}

	return 0;
}

PUBLIC_SYMBOL
unsigned int pool_va2offset(uintptr_t va)
{
	return (unsigned int)(va - g_devhost_cell_pool_va);
}

PUBLIC_SYMBOL
uintptr_t pool_offset2va(unsigned int offset)
{
	return g_devhost_cell_pool_va + offset;
}

#ifdef CONFIG_DEVHOST_NET_SPARSE_POOL
PUBLIC_SYMBOL
unsigned long long pool_offset2pa(unsigned int offset)
{
	unsigned int page_offset = offset % PAGE_SIZE;
	unsigned int page_idx = offset / PAGE_SIZE;

	return *((unsigned long long *)(uintptr_t)g_devhost_cell_pool_pa + page_idx) + page_offset;
}

static int prepare_va2pa_mem(void)
{
	int err = E_HM_OK;
	unsigned long long *paddr = NULL;
	unsigned long size = (NET_CELL_POOL_LEN / PAGE_SIZE) * sizeof(unsigned long long);

	/* to store shm_id va<>pa mappings */
	paddr = (unsigned long long *)malloc(size);
	if (paddr == NULL) {
		dh_error("malloc va<>pa memory failed\n");
		return E_HM_NOMEM;
	}

	mem_zero_b(paddr, size);
	err = hm_mem_query_shm_paddr(g_shm_id, paddr, size);
	if ((err < 0) || ((unsigned long)err != size / sizeof(unsigned long long))) {
		free(paddr);
		dh_error("hm_mem_query_shm_paddr failed, g_shm_id=%d, err=%d\n",
			 g_shm_id, err);
		return err;
	}
	g_devhost_cell_pool_pa = (unsigned long long)(uintptr_t)paddr;

	return E_HM_OK;
}

static int __devhost_shm_mem_alloc_sparse(size_t size, int *ret_id)
{
	int shm_id = 0;
	uint64_t oflags = SHM_O_CREAT | SHM_O_EXCL |
			  SHM_O_POPULATE | SHM_O_RDWR;

	shm_id = hm_ashm_open_auth(0, 0U, size, oflags, NULL);
	if (shm_id < 0) {
		dh_error("hm_mem_shm_open failed, err=%s\n", hmstrerror(shm_id));
		return shm_id;
	}

	if (ret_id != NULL) {
		*ret_id = shm_id;
	}

	return E_HM_OK;
}

static void *devhost_shm_mem_alloc_sparse(void *addr, size_t size,
					  int *rshmid)
{
	int err = E_HM_OK;
	int shm_id = 0;
	off_t offset = 0;
	void *rvaddr = NULL;
	uint32_t flags =  MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS |
			  MFLAG_MAP_POPULATE;

	err = __devhost_shm_mem_alloc_sparse(size, &shm_id);
	if (err < 0) {
		return MAP_FAILED;
	}

	rvaddr = hm_mem_mmap(addr, size, PROT_READ | PROT_WRITE, flags, shm_id, offset);
	if (rvaddr == MAP_FAILED) {
		dh_error("shm map failed\n");
		(void)__devhost_shm_mem_destroy(shm_id);
		return MAP_FAILED;
	}

	if (rshmid != NULL) {
		*rshmid = shm_id;
	}

	return rvaddr;
}

int prepare_cell_pool_once(int *shm_id)
{
	int err = E_HM_OK;
	void *addr = NULL;
	struct share_mem_ctrl_info *p = NULL;

	if (raw_atomic_int_cas(&g_devhost_pool_is_alloc, 0, 1) != 0) {
		if (shm_id) {
			*shm_id = g_shm_id;
		}
		return E_HM_OK;
	}

	addr = devhost_shm_mem_alloc_sparse(NULL, NET_CELL_POOL_LEN, &g_shm_id);
	if (addr == MAP_FAILED) {
		dh_error("devhost_shm_mem_alloc failed\n");
		(void)raw_atomic_int_set(&g_devhost_pool_is_alloc, 0);
		return E_HM_NOMEM;
	}

	p = shmalloc_init(addr, NET_CELL_POOL_LEN, NET_CELL_POOL_UNIT_SIZE);
	if (p == NULL) {
		dh_error("shmalloc_init failed\n");
		goto free_shm_mem;
	}

	err = prepare_va2pa_mem();
	if (err < 0) {
		goto free_shm_mem;
	}

	g_devhost_cell_pool_va = (uintptr_t)addr;
	if (shm_id) {
		*shm_id = g_shm_id;
	}

	dh_info("pool_size=0x%x, unit_size=0x%x, shm_id=%d\n",
		NET_CELL_POOL_LEN, NET_CELL_POOL_UNIT_SIZE, g_shm_id);

	return E_HM_OK;

free_shm_mem:
	(void)raw_atomic_int_set(&g_devhost_pool_is_alloc, 0);

	err = hm_mem_munmap(addr, NET_CELL_POOL_LEN);
	if (err != E_HM_OK) {
		dh_warn("hm_mem_munmap failed, rc=%s\n", hmstrerror(err));
	}
	err = hm_ashm_close(g_shm_id);
	if (err != E_HM_OK) {
		dh_warn("hm_ashm_close failed, rc=%s\n", hmstrerror(err));
	}
	err = hm_ashm_unlink(g_shm_id);
	if (err != E_HM_OK) {
		dh_warn("hm_ashm_unlink failed, rc=%s\n", hmstrerror(err));
	}

	return E_HM_INVAL;
}

#else
PUBLIC_SYMBOL
unsigned long long pool_offset2pa(unsigned int offset)
{
	return g_devhost_cell_pool_pa + offset;
}

int prepare_cell_pool_once(int *shm_id)
{
	void *backend_addr = NULL;
	struct share_mem_ctrl_info *p = NULL;

	if (raw_atomic_int_cas(&g_devhost_pool_is_alloc, 0, 1) != 0) {
		if (shm_id) {
			*shm_id = g_shm_id;
		}
		return E_HM_OK;
	}

	backend_addr = devhost_backend_mem_alloc(NET_CELL_POOL_LEN, MPROT_READ | MPROT_WRITE,
						 &g_devhost_cell_pool_pa, &g_shm_id);
	if (backend_addr == MAP_FAILED) {
		dh_error("devhost_backend_mem_alloc failed\n");
		return E_HM_NOMEM;
	}

	p = shmalloc_init(backend_addr, NET_CELL_POOL_LEN, NET_CELL_POOL_UNIT_SIZE);
	if (p == NULL) {
		dh_error("shmalloc_init failed\n");
		devhost_backend_mem_free(g_shm_id, backend_addr, NET_CELL_POOL_LEN);
		return E_HM_INVAL;
	}

	g_devhost_cell_pool_va = (uintptr_t)backend_addr;
	if (shm_id) {
		*shm_id = g_shm_id;
	}

	dh_info("pool_size=0x%x, unit_size=0x%x, shm_id=%d\n",
		NET_CELL_POOL_LEN, NET_CELL_POOL_UNIT_SIZE, g_shm_id);

	return E_HM_OK;
}
#endif

void prepare_cell_pool_cleanup(void)
{
	int err = E_HM_OK;

	(void)raw_atomic_int_set(&g_devhost_pool_is_alloc, 0);

	err = hm_mem_munmap((void *)(uintptr_t)g_devhost_cell_pool_va,
			NET_CELL_POOL_LEN);
	if (err != E_HM_OK) {
		dh_warn("cleanup: hm_mem_munmap failed, rc=%s\n", hmstrerror(err));
	}
	err = hm_ashm_close(g_shm_id);
	if (err != E_HM_OK) {
		dh_warn("cleanup: hm_ashm_close failed, rc=%s\n", hmstrerror(err));
	}
	err = hm_ashm_unlink(g_shm_id);
	if (err != E_HM_OK) {
		dh_warn("cleanup hm_ashm_unlink failed, rc=%s\n", hmstrerror(err));
	}
}

PUBLIC_SYMBOL
void *devhost_dma_pool_alloc(unsigned long long *paddr,
			     unsigned int *offset)
{
	int err = E_HM_OK;
	uintptr_t va;

	if ((paddr == NULL) || (offset == NULL)) {
		return NULL;
	}

	err = prepare_cell_pool_once(NULL);
	if (err != 0) {
		dh_error("prepare_cell_pool_once failed, err=%s\n", hmstrerror(err));
		return NULL;
	}

	err = shmalloc_unit_alloc((struct share_mem_ctrl_info *)(uintptr_t)g_devhost_cell_pool_va, &va);
	if (err == E_HM_NOMEM) {
		return NULL;
	}

	if (err < 0) {
		dh_error("shmalloc_unit_alloc failed, err=%s, "
			 "pool_size=0x%x, unit_size=0x%x\n",
			 hmstrerror(err), NET_CELL_POOL_LEN,
			 NET_CELL_POOL_UNIT_SIZE);
		return NULL;
	}

	g_devhost_stat.alloc_count++;
	*offset = pool_va2offset(va);
	*paddr = pool_offset2pa(*offset);

	return (void *)va;
}

PUBLIC_SYMBOL
void *devhost_dma_pool_alloc_ex(unsigned long long *paddr,
				unsigned int *offset, unsigned int size)
{
	if (size > NET_CELL_POOL_UNIT_SIZE) {
		dh_warn("requested skb size [%u] is too big\n", size);
		return NULL;
	}
	return devhost_dma_pool_alloc(paddr, offset);
}

PUBLIC_SYMBOL
int devhost_dma_pool_free(unsigned int offset)
{
	int err = E_HM_OK;

	err = shmalloc_unit_free((struct share_mem_ctrl_info *)(uintptr_t)g_devhost_cell_pool_va, offset);
	if (err < 0) {
		dh_error("shmalloc_unit_free failed, err=%s\n", hmstrerror(err));
		return -1;
	}

	g_devhost_stat.free_count++;

	return 0;
}

unsigned int devhost_dma_get_free_num(void)
{
	struct share_mem_ctrl_info *pctrl = NULL;
	pctrl = (struct share_mem_ctrl_info *)(uintptr_t)g_devhost_cell_pool_va;

	return shmalloc_get_unit_free_num(pctrl);
}
static void *__handler_rx_ack(void *arg)
{
	struct uring_cqe cqe;
	uint32_t pool_offset;
	struct io_uring *rx_uring = (struct io_uring *)arg;
	char thread_name[HMCAP_THREAD_NAME_SIZE];
	int ret = E_HM_OK;

	ret = snprintf_s(thread_name, HMCAP_THREAD_NAME_SIZE,
			 HMCAP_THREAD_NAME_SIZE - 1,
			 "dhrx-%d", hm_getpid());
	if (ret < 0) {
		return (void *)(intptr_t)ret;
	}

	ret = hm_thread_setname(0, thread_name, NULL);
	if (ret < 0) {
		dh_error("set net tx thread name failed, ret=%s\n",
			 hmstrerror(ret));
		return (void *)(intptr_t)ret;
	}

	while (g_rx_thread_running) {
		ret = io_uring_wait_netcqe(rx_uring, &cqe);
		if (ret < 0) {
			if (ret == E_HM_NOMSG) {
				g_rx_thread_running = false;
				break;
			}
			continue;
		}
		pool_offset = (uint32_t)cqe.user_data;
		if (cqe.result < 0) {
			(void)devhost_dma_pool_free(pool_offset);
		}
	}
	return NULL;
}

static int __rxcomplete_thread_setup(struct io_uring *uring)
{
	int ret = E_HM_OK;

	g_rx_thread_running = true;
	ret = raw_thread_create(&g_rx_thread, NULL, __handler_rx_ack, uring);
	if (ret != E_HM_OK) {
		g_rx_thread_running = false;
		dh_error("create handler_netdev_tx fail\n");
	}
	return ret;
}

struct io_uring *prepare_rx_uring_once(void)
{
	int ret;

	if (raw_atomic_int_cas(&g_dh_rx_uring_inited, 0, 1) != 0) {
		return g_rx_ring;
	}

	g_rx_ring = io_uring_create(NET_CELL_POOL_UNIT_NUM, sizeof(struct uring_net_data), 0);
	if (g_rx_ring == NULL) {
		(void)raw_atomic_int_set(&g_dh_rx_uring_inited, 0);
		return NULL;
	}

	ret = __rxcomplete_thread_setup(g_rx_ring);
	if (ret != E_HM_OK) {
		io_uring_destroy(g_rx_ring);
		g_rx_ring = NULL;
		(void)raw_atomic_int_set(&g_dh_rx_uring_inited, 0);
	}

	return g_rx_ring;
}

void prepare_rx_uring_cleanup(void)
{
	io_uring_stop(g_rx_ring);
	(void)raw_thread_join(g_rx_thread, NULL);
	g_rx_thread_running = false;
	g_rx_thread = NULL;

	io_uring_destroy(g_rx_ring);
	g_rx_ring = NULL;
	(void)raw_atomic_int_set(&g_dh_rx_uring_inited, 0);
}
