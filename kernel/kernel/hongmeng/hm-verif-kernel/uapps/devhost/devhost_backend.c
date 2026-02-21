/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost backend APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#include "devhost_backend.h"

#include <linux/version.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libmem/types.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libpreempt/preempt.h>

#include <devhost/backend.h>

#include "devhost.h"
#include <devhost/log.h>
#include "utils/utils.h"
#include <sys/membarrier.h>

struct dh_backend_type {
	struct dh_backend_ctor ctor;
	void *sohandle;
};

struct dh_shm_info {
	int32_t id;
	size_t len;
	void *vaddr;
};

static void *__dh_ctor_alloc(size_t size, void **handle)
{
	int err;
	struct dh_shm_info *shm_info = NULL;
	uint32_t prot = MPROT_READ | MPROT_WRITE;
	uint64_t oflags = SHM_O_CREAT | SHM_O_POPULATE | SHM_O_EXCL | SHM_O_RDWR;
	uint32_t flags = MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED | MFLAG_MAP_POPULATE;

	if ((size == 0) || (handle == NULL)) {
		return NULL;
	}

	shm_info = malloc(sizeof(*shm_info));
	if (shm_info == NULL) {
		return NULL;
	}
	mem_zero_s(*shm_info);

	err = hm_ashm_open_auth(0, 0U, size, oflags, &shm_info->len);
	if (err < 0) {
		dh_error("shm open failed, err=%s\n", hmstrerror(err));
		free(shm_info);
		return NULL;
	}
	shm_info->id = err;

	shm_info->vaddr = hm_mem_mmap_posix(NULL, shm_info->len, prot,
					    flags, shm_info->id, 0, &err);
	if (err < 0) {
		dh_error("failed to map shm, err=%s\n", hmstrerror(err));
		(void)hm_ashm_close(shm_info->id);
		(void)hm_ashm_unlink(shm_info->id);
		free(shm_info);
		return NULL;
	}

	*handle = (void *)shm_info;

	return shm_info->vaddr;
}

static int __dh_ctor_query(void *handle, void *ranges, unsigned int ranges_num)
{
	int err;
	unsigned int i;
	unsigned int page_cnt;
	unsigned long long *paddrs = NULL;
	struct dh_shm_info *shm_info = NULL;
	struct mem_raw_ranges *_ranges = NULL;

	if (handle == NULL || ranges == NULL || ranges_num == 0) {
		return E_HM_INVAL;
	}

	shm_info = (struct dh_shm_info *)handle;
	BUG_ON(shm_info->vaddr == NULL);
	BUG_ON(shm_info->len == 0);

	page_cnt = (unsigned int)(shm_info->len >> PAGE_SHIFT);
	if (ranges_num < page_cnt) {
		return E_HM_INVAL;
	}

	paddrs = malloc(sizeof(unsigned long long) * page_cnt);
	if (paddrs == NULL) {
		return E_HM_NOMEM;
	}

	err = hm_mem_query_shm_paddr(shm_info->id, paddrs,
				     sizeof(unsigned long long) * page_cnt);
	if (err < 0) {
		dh_error("query shm info failed, err=%s\n", hmstrerror(err));
		free(paddrs);
		return err;
	}

	_ranges = (struct mem_raw_ranges *)ranges;
	for (i = 0; i < page_cnt; i++) {
		_ranges[i].start = paddrs[i] >> PAGE_SHIFT;
		_ranges[i].end = _ranges[i].start + 1;
	}
	free(paddrs);

	return (int)page_cnt;
}

static void __dh_ctor_free(void *handle)
{
	struct dh_shm_info *shm_info = NULL;

	if (handle == NULL) {
		return;
	}

	shm_info = (struct dh_shm_info *)handle;
	BUG_ON(shm_info->vaddr == NULL);
	(void)hm_mem_munmap(shm_info->vaddr, shm_info->len);
	(void)hm_ashm_close(shm_info->id);
	(void)hm_ashm_unlink(shm_info->id);
	free(shm_info);
}

static inline void *__dh_ctor_alloc_page(unsigned int order)
{
	return hm_mem_mmap(0, PAGE_SIZE * (1UL << order),
			   PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MFLAG_MAP_ACTV,
			   0, 0);
}
static inline void __dh_ctor_free_page(void *ptr, unsigned int order)
{
	int ret = E_HM_OK;
	ret = hm_mem_munmap(ptr, PAGE_SIZE * (1UL << order));
	if (ret != E_HM_OK) {
		dh_warn("mem unmap failed, err=%s\n", hmstrerror(ret));
	}
}

struct dh_backend_type g_default_backend __read_mostly = {
	.ctor = {
		.ctx_setup = NULL,
		.syscall_enter = NULL,
		.syscall_enter_nosec = NULL,
		.syscall_enter_nomm = NULL,
		.syscall_enter_basic = NULL,
		.syscall_exit = NULL,
		.alloc = __dh_ctor_alloc,
		.query = __dh_ctor_query,
		.free = __dh_ctor_free,
		.alloc_page = __dh_ctor_alloc_page,
		.free_page = __dh_ctor_free_page,
		.pre_map = NULL,
		.pre_unmap = NULL,
		.handle_page_fault = NULL,
		.handle_mm_rdlock = NULL,
		.handle_mm_rdunlock = NULL,
		.handle_mm_release = NULL,
		.umap_ops = NULL,
		.vhangup = NULL,
	},
	.sohandle = NULL,
};

int backend_ctx_setup(void)
{
	int ret = E_HM_OK;
	if (g_default_backend.ctor.ctx_setup != NULL) {
		ret = posix2hmerrno(-(*g_default_backend.ctor.ctx_setup)());
		if (ret != E_HM_OK) {
			hm_error("dh backend ctx failed %s\n", hmstrerror(ret));
		}
	} else {
		hm_error("not init backend ctx\n");
	}
	return ret;
}

void backend_syscall_enter(enum devhost_syscall_enter_type type)
{
	switch (type) {
	case SYSCALL_ENTER_FULL_INFO:
		if (g_default_backend.ctor.syscall_enter != NULL) {
			(*g_default_backend.ctor.syscall_enter)();
		}
		break;

	case SYSCALL_ENTER_NO_SEC:
		if (g_default_backend.ctor.syscall_enter_nosec != NULL) {
			(*g_default_backend.ctor.syscall_enter_nosec)();
		}
		break;

	case SYSCALL_ENTER_NO_MM:
		if (g_default_backend.ctor.syscall_enter_nomm != NULL) {
			(*g_default_backend.ctor.syscall_enter_nomm)();
		}
		break;

	case SYSCALL_ENTER_BASIC:
		if (g_default_backend.ctor.syscall_enter_basic != NULL) {
			(*g_default_backend.ctor.syscall_enter_basic)();
		}
		break;

	default:
		break;
	}
}

void backend_basic_syscall_enter(void)
{
	backend_syscall_enter(SYSCALL_ENTER_BASIC);
}

void backend_syscall_exit(void)
{
	if (g_default_backend.ctor.syscall_exit != NULL) {
		(*g_default_backend.ctor.syscall_exit)();
	}
}

void *devhost_backend_alloc(size_t size, void **handle)
{
	void *vaddr = NULL;
	if (g_default_backend.ctor.alloc != NULL) {
		vaddr = g_default_backend.ctor.alloc(size, handle);
	}
	return vaddr;
}

int devhost_backend_query(void *handle, struct mem_raw_ranges *ranges,
			  unsigned int ranges_num)
{
	int ret = E_HM_INVAL;
	if (g_default_backend.ctor.query != NULL) {
		ret = g_default_backend.ctor.query(handle, ranges, ranges_num);
	}
	return ret;
}

void devhost_backend_free(void *handle)
{
	if (g_default_backend.ctor.free != NULL) {
		g_default_backend.ctor.free(handle);
	}
}

void *devhost_backend_alloc_page(unsigned int order)
{
	void *ptr = NULL;
	if (g_default_backend.ctor.alloc_page != NULL) {
		ptr = g_default_backend.ctor.alloc_page(order);
	}
	return ptr;
}

void devhost_backend_free_page(void *ptr, unsigned int order)
{
	if (g_default_backend.ctor.free_page != NULL) {
		g_default_backend.ctor.free_page(ptr, order);
	}
}

void *devhost_backend_prepare_map(unsigned long long pa, unsigned long size)
{
	void *va = NULL;
	if (g_default_backend.ctor.pre_map != NULL) {
		va = g_default_backend.ctor.pre_map(pa, size);
		if (va == NULL) {
			va = MAP_FAILED;
		}
	}
	return va;
}
void devhost_backend_prepare_unmap(void *va, unsigned long size)
{
	if (g_default_backend.ctor.pre_unmap != NULL) {
		g_default_backend.ctor.pre_unmap(va, size);
	}
}

int devhost_backend_handle_page_fault(void *va, unsigned long long vr_key, unsigned long long addr, unsigned int flags)
{
	int ret = -ENOSYS;

	if (g_default_backend.ctor.handle_page_fault != NULL) {
		ret = g_default_backend.ctor.handle_page_fault(va, vr_key, addr, flags, NULL);
	}

	return ret;
}

int devhost_backend_handle_mm_rdlock(void)
{
	int ret = -ENOSYS;

	if (g_default_backend.ctor.handle_mm_rdlock != NULL) {
		ret = g_default_backend.ctor.handle_mm_rdlock();
	}

	return ret;
}

void devhost_backend_handle_mm_rdunlock(void)
{
	if (g_default_backend.ctor.handle_mm_rdunlock != NULL) {
		g_default_backend.ctor.handle_mm_rdunlock();
	}
}

void devhost_backend_vhangup(void)
{
	if (g_default_backend.ctor.vhangup != NULL) {
		g_default_backend.ctor.vhangup();
	}
}

int devhost_backend_umap_munmap(void *data, int vs_is_releasing)
{
	int ret;
	struct devhost_umap_ops *ops = g_default_backend.ctor.umap_ops;

	if (ops == NULL || ops->munmap == NULL) {
		/* it is normal for devhost umap ops to be null */
		return 0;
	}

	ret = ops->munmap(data, vs_is_releasing);
	return ret;
}

int devhost_backend_mm_release(unsigned long long vspace_key)
{
	if (g_default_backend.ctor.handle_mm_release != NULL) {
		return g_default_backend.ctor.handle_mm_release(vspace_key);
	}
	return -ENOSYS;
}

int hm_user_preempt_disable(void)
{
	if (g_default_backend.ctor.preempt_disable != NULL) {
		return g_default_backend.ctor.preempt_disable();
	}
	return preempt_process_disable();
}

int hm_user_preempt_enable(void)
{
	if (g_default_backend.ctor.preempt_enable != NULL) {
		return g_default_backend.ctor.preempt_enable();
	}
	return preempt_process_enable();
}

int devhost_backend_register_umap_ops(struct devhost_umap_ops *ops)
{
	if (g_default_backend.ctor.umap_ops != NULL) {
		return -EEXIST;
	}

	g_default_backend.ctor.umap_ops = ops;

	return 0;
}

#ifdef CONFIG_DLOPEN_WITH_HUGE
#define PROC_SYS_VM_NR_HUGEPAGES "/proc/sys/vm/nr_hugepages"
#define ENABLE_HUGE_PAGE "50"
#define DISABLE_HUGE_PAGE "0"

static int ctrl_huge_pages(const char *str)
{
	int ret = E_HM_OK;
	int fd = -1;
	ssize_t rc = 0;

	fd = open(PROC_SYS_VM_NR_HUGEPAGES, O_WRONLY);
	if (fd < 0) {
		hm_error("failed to open nr_hugepages, error=%s\n", strerror(errno));
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		rc = write(fd, str, strlen(str) + 1);
		if (rc < 0) {
			hm_error("failed to write nr_hugepages, error=%s\n", strerror(errno));
			ret = E_HM_INVAL;
		}
		close(fd);
	}

	return ret;
}

static void *dlopen_with_huge_page(const char *backend)
{
	int ret = E_HM_OK;
	void *sohandle = NULL;

	ret = ctrl_huge_pages(ENABLE_HUGE_PAGE);
	if (ret != E_HM_OK) {
		hm_error("failed to enable huge pages\n");
	}

	if (ret == E_HM_OK) {
		sohandle = dlopen(backend, RTLD_LAZY | RTLD_GLOBAL | RTLD_HUGEPAGE);
		if (sohandle == NULL) {
			hm_error("failed to dlopen with RTLD_HUGEPAGE file=%s, err=%s\n", backend, dlerror());
		}
		ret = ctrl_huge_pages(DISABLE_HUGE_PAGE);
		if (ret != E_HM_OK) {
			hm_error("failed to disable huge pages\n");
		}
	}

	return sohandle;
}
#endif

int init_backend_dllib(const char* backend)
{
	void *sohandle = NULL;
	dh_backend_init_fn_t backend_init = NULL;
	int ret = E_HM_OK;

	ret = membarrier(MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED, 0);
	if (ret != E_HM_OK) {
		return ret;
	}

#ifdef CONFIG_DLOPEN_WITH_HUGE
	/* load backend dso with RTLD_GLOBAL for UDK */
	if (dh_is_root()) {
		/* dlopen with huge page for root devhost only */
		sohandle = dlopen_with_huge_page(backend);
	}
	if (sohandle == NULL) {
		/* fallback to dlopen without RTLD_HUGEPAGE */
		sohandle = dlopen(backend, RTLD_LAZY | RTLD_GLOBAL);
	}
#else
	/* load backend dso with RTLD_GLOBAL for UDK */
	sohandle = dlopen(backend, RTLD_LAZY | RTLD_GLOBAL);
#endif
	if (sohandle == NULL) {
		dh_error("%s\n", dlerror());
		return E_HM_INVAL;
	}

	backend_init = dlsym(sohandle, DH_BACKEND_INIT_SYM);
	if (backend_init == NULL) {
		dh_error("load symbol failed, symbol=%s, err=%s\n",
			 DH_BACKEND_INIT_SYM, dlerror());
		(void)dlclose(sohandle);
		return E_HM_INVAL;
	}

	ret = (*backend_init)(&g_default_backend.ctor, getenv("BACKEND_CMDLINE"));
	if (ret < 0) {
		dh_error("dh_backend_init() failed, err=%s\n", hmstrerror(ret));
		(void)dlclose(sohandle);
		return E_HM_INVAL;
	}

	g_default_backend.sohandle = sohandle;

	return ret;
}
