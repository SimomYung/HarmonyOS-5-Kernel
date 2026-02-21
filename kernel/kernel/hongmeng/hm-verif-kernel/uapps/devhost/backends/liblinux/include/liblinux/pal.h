/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: liblinux PAL prototypes
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 3 15:48:16 2021
 */
#ifndef __LIBLINUX_PAL_H__
#define __LIBLINUX_PAL_H__
#include <libhmseharmony/services.h>
#ifndef __ASSEMBLY__

#include <sys/types.h>
#include <sys/socket.h>

#define ARCH_HAS_DMA_GET_REQUIRED_MASK

struct task_security_struct {
	unsigned int osid;
	unsigned int sid;
	unsigned int exec_sid;
	unsigned int create_sid;
	unsigned int keycreate_sid;
	unsigned int sockcreate_sid;
};

/* liblinux exported symbols */
extern void liblinux_kernel_init(void);
extern void liblinux_do_initcalls(void);
extern int  liblinux_setup_fdt(void *dt_virt);
extern void liblinux_enter(int pid, ...);
extern void liblinux_enter_basic(void);
extern void liblinux_exit(void);
extern int  liblinux_thread_setup(void);
extern void *liblinux_mm_prepare_map(unsigned long long pa, unsigned long size);
extern void liblinux_mm_prepare_unmap(void *va, unsigned long size);
extern int liblinux_suspend_prepare(int force);
extern void liblinux_freeze_proc(void);
extern void liblinux_thaw_proc(void);
extern int liblinux_freeze_wqs(void);
extern void liblinux_thaw_wqs(void);
extern int liblinux_suspend(void);
extern void liblinux_resume(void);
extern void liblinux_resume_finish(void);
extern int liblinux_suspend_late(void);
extern void liblinux_resume_early(void);

#ifdef CONFIG_LIBLINUX_HAVE_ULSR
extern void liblinux_ulsr_enter(void);
extern void liblinux_ulsr_leave(void);
#endif

extern void liblinux_time_sync(void);
extern struct wakeup_source *liblinux_acquire_deleted_ws(void);

int liblinux_hibernate_prepare(void);
void liblinux_post_hibernate(void);
int liblinux_hibernate_restore_prepare(void);
void liblinux_hibernate_post_restore(void);
int liblinux_hibernate(int event);
void liblinux_hibernate_resume(int event);

extern void liblinux_preempt_bypass(bool bypass);

struct liblinux_tcb_time {
	unsigned long long total;
	unsigned long long runtime;
	unsigned long long ready;
};

int liblinux_pal_get_task_sched_time(struct liblinux_tcb_time *time);
unsigned long liblinux_pal_get_time_us(void);
int liblinux_pal_suspend_cnt_read(int *cnt);
int liblinux_pal_set_pm_crypt_info(uint32_t key_index, bool clear);

unsigned int liblinux_pal_get_log_usertype(void);

int liblinux_pal_get_cpu_total_inst(uint64_t *cpu_inst, uint32_t cpu_nr);

int liblinux_pal_get_max_capacity_cpu(int in_suspend, unsigned int *cpuid);

bool liblinux_pal_get_freq_privileged_user(void);

rref_t liblinux_pal_pm_class_rref_acquire(void);
int liblinux_pal_pm_class_init(void);
void liblinux_pal_pm_class_exit(void);

#ifdef CONFIG_HISI_THERMAL_CPUISO
extern int liblinux_pal_set_cpu_isolate(unsigned int cluster, unsigned int cpu, unsigned int isolated);
#endif
typedef int(*nl_transmit_handler_t)(const void *msg,
				   unsigned int len, int proto, unsigned int dst);
/* obsolete */
typedef int(*nl_genl_reg_handler_t)(const char *name, const void *fam_info, unsigned long info_len,
				    unsigned int *fam_id_inout, unsigned int *mcgroup_offset_out);
/* obsolete */
typedef int(*nl_genl_unreg_handler_t)(unsigned int fam_id);

#define GEN_NETLINK 1
#define RT_NETLINK 2
#define CUST_NETLINK 3
struct liblinux_nl_info {
	int type;
	union {
		struct {
			const char *name;
			const void *fam_info;
			unsigned long info_len;
			unsigned int fam_id;		/* out */
			unsigned int mcgroup_offset;	/* out */
		} genl;
		struct {
			const char *name;
		}rtnl;
		struct {
			unsigned int unit;
			unsigned int nr_grp;
		} cust_nl;
	};
};
typedef int(*nl_reg_handler_t)(struct liblinux_nl_info *nl_info);
typedef int(*nl_unreg_handler_t)(struct liblinux_nl_info *nl_info);
struct netlink_pal_handler {
	nl_transmit_handler_t unicast_hdr;
	nl_transmit_handler_t multicast_hdr;
	/* obsolete */
	nl_genl_reg_handler_t genl_reg_hdr;
	/* obsolete */
	nl_genl_unreg_handler_t genl_unreg_hdr;
	nl_reg_handler_t nl_reg_hdr;
	nl_unreg_handler_t nl_unreg_hdr;
};
extern int liblinux_pal_netlink_init(struct netlink_pal_handler *hdr);

/* linker script symbols */
extern char __head_start[];

/* devfs related */
extern  int liblinux_pal_register_chrdev_region(unsigned int major,
		unsigned int baseminor, int minorct,
		const char *name, unsigned int *major_out);

extern void liblinux_pal_unregister_chrdev_region(unsigned int major,
		unsigned int baseminor, int minorct);

extern  int liblinux_pal_register_blkdev_region(unsigned int major,
		const char *name, unsigned int *major_out);
extern void liblinux_pal_unregister_blkdev_region(unsigned int major, const char *name);

void *liblinux_pal_alloc_dma_pool(unsigned int *offset, unsigned long long *paddr);
void *liblinux_pal_alloc_dma_pool_ex(unsigned int *offset,
				     unsigned long long *paddr,
				     unsigned int size);
int liblinux_pal_free_dma_pool(unsigned int offset);
/*
 * CAUTION:
 *   The `dma_alloc` PAL return non-coherent (a.k.a cacheable) dma memory.
 *   It will be removed or replaced by a non-coherent version PAL when all
 *   the dependencies have been removed.
 */
extern void *liblinux_pal_dma_alloc(unsigned int size, unsigned long long *paddr);
extern void *liblinux_pal_dma_alloc_coherent(unsigned int size,
					     unsigned long long *paddr);
extern int liblinux_pal_dma_free(const void *vaddr, unsigned int size);
int
liblinux_pal_native_net_rx(unsigned int pool_offset, int shm_len,
			   int rx_len, int offset, int ifindex);
/*
 * Note: After shared ring buffer ready,
 * we should to replace this function to the notification mechanism based on
 * shared ring buffer in future.
 */
struct netdev_rx_cb {
	unsigned int cell_pool_offset;
	int rx_len;
	int offset;
	int linux_devno;
};
#define MAX_RX_CB_NUM 16
int liblinux_pal_native_net_rx_ex(int num, void *data, int data_size);
int liblinux_pal_netdev_alloc_ifindex(int ifindex);
int liblinux_pal_is_compat_client(void);

int liblinux_pal_socket_create(int family, int type, int protocol);
int liblinux_pal_socket_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t liblinux_pal_socket_sendmsg(int sockfd, const struct msghdr *msg, int flags);
int liblinux_pal_socket_close(int sockfd);

/* IRQ related */
int liblinux_pal_local_irq_save(void);
void liblinux_pal_local_irq_enable(void);
void liblinux_pal_local_irq_disable(void);
void liblinux_pal_local_irq_restore(int disabled);
int liblinux_pal_local_save_flags(void);
int liblinux_pal_irq_set_affinity_hint(unsigned long linux_irq, unsigned long mask);
int liblinux_pal_smp_call_cpu(int irq, unsigned int cpu);

/* vmalloc */
struct vmap_node;
extern void *liblinux_pal_vmalloc_range(unsigned long size, unsigned long align,
		unsigned long start, unsigned long end,
		unsigned long prot, const void *caller);
extern void *liblinux_pal_vmalloc_range_ex(unsigned long size, unsigned long align,
		unsigned long start, unsigned long end,
		unsigned long prot, unsigned long pal_vm_flags, const void *caller);
extern void   liblinux_pal_vfree(const void *addr);
extern unsigned long liblinux_pal_vmalloc_nr_pages(void);
extern int liblinux_pal_vmalloc_nr_pages_trylock(unsigned long *total_nr_pages);
extern int liblinux_pal_vmalloc_pmem_stat(struct vmap_node *vnode, unsigned char *vec,
										  unsigned long *used_size, unsigned int *nr_pages_out);
extern int liblinux_pal_remap_vmalloc_range(unsigned long long vs_key, unsigned long uaddr, unsigned long kaddr,
	unsigned long size, unsigned long prot, unsigned int flags);
extern int liblinux_pal_check_vmalloc_size(void* kaddr, unsigned long end_index);

/* alloc/free for task struct to avoid recursive in thread_setup */
extern void *liblinux_pal_malloc(unsigned long size);
extern void *liblinux_pal_calloc(size_t nmemb, size_t size);
extern void liblinux_pal_free(void *addr);

/* page alloc */
struct liblinux_page_allocator {
	void (*destroy)(const struct liblinux_page_allocator *_allocator);
	int (*extend)(const struct liblinux_page_allocator *_allocator,
		      unsigned int order, unsigned long *rvaddr, int flags);
	void *(*alloc)(const struct liblinux_page_allocator *_allocator,
		       unsigned int order, unsigned long long *phys);
	void (*free)(const struct liblinux_page_allocator *_allocator,
		     unsigned long addr, unsigned int order);
	int (*shrink)(const struct liblinux_page_allocator *_allocator,
		      unsigned long vaddr, unsigned long long paddr, unsigned long size);

	/* extend_ex */
	int (*extend_ex)(const struct liblinux_page_allocator *_allocator,
			 unsigned int order, unsigned long *rvaddr, int recaim);
	/* for optimised implementation of alloc_pages_exact */
	int (*extend_exact)(const struct liblinux_page_allocator *_allocator,
			    unsigned long size, unsigned long *rvaddr, int flags);
	void *(*alloc_exact)(const struct liblinux_page_allocator *_allocator,
			     unsigned long size, unsigned long long *phys);
	void (*free_exact)(const struct liblinux_page_allocator *_allocator,
			   unsigned long addr, unsigned long size);

	/* extend and alloc */
	int (*extend_alloc)(const struct liblinux_page_allocator *_allocator,
			    unsigned int extend_order, unsigned int alloc_order, unsigned long *rvaddr, int flags);
	int (*extend_alloc_exact)(const struct liblinux_page_allocator *_allocator,
				  unsigned long extend_size, unsigned long alloc_size,
				  unsigned long *rvaddr, int flags);
	int (*should_reclaim)(void);
};

struct liblinux_pool_base {
	unsigned long kmap_virt_start;
	unsigned long kmap_virt_end;
	unsigned long long phys_offset;
	unsigned int page_sizeorder;
};

struct liblinux_ram_info {
	unsigned long long memstart;
	unsigned long long memend;
	unsigned long long total_ram;
};

/*
 * Liblinux pal page alloc initialization.
 *
 * @param pool_base		[I] base info of ldk kmap
 *
 * @param ram_info		[O] ram info if operation succeeds
 *
 * @return OK if operation succeeds.
 */
extern int liblinux_pal_page_alloc_init(struct liblinux_pool_base *pool_base,
					struct liblinux_ram_info *ram_info);

#define LIBLINUX_VMEMMAP_POPULATE_ALL		0x01U
#define LIBLINUX_VMEMAP_HIGHMEM			0x02U
#define LIBLINUX_VMEMMAP_POPULATE_IOFAST	0x04U

struct liblinux_vmmemmap_info {
	/* highmem info */
	unsigned long long page_addr;
	unsigned long long page_size;
	unsigned long long vmemmap_start;
	unsigned long long vmemmap_size;
	unsigned int flags;
};

int liblinux_pal_vmemmap_init(struct liblinux_vmmemmap_info *info);
int liblinux_pal_vmemmap_is_exist(const void *start, unsigned long size);
int liblinux_pal_vmemmap_is_populated(const void *start, unsigned long size);
int liblinux_pal_vmemmap_dump(char *info, unsigned long size);

struct liblinux_kmap_info {
	/* kmap info */
	unsigned long kmap_virt_start;
	unsigned long kmap_virt_end;
	unsigned long long phys_offset;
	unsigned int page_sizeorder;
};

extern int liblinux_pal_page_alloc_init_ex(struct liblinux_ram_info *ram_info,
					   struct liblinux_page_allocator **allocator,
					   unsigned int min_alloc_shift,
					   const struct liblinux_kmap_info *info);

/*
 * Liblinux pal page allocator get.
 *
 * @param allocator_name	[I] name of allocator
 *
 * @return page allocator if operation succeeds.
 */
struct liblinux_page_allocator *liblinux_pal_page_allocator_get(char *allocator_name);
extern int liblinux_pal_reserve_range_name(void *start, unsigned long size,
					   int prot, const char *name);
extern void *liblinux_pal_reserve_size_name(unsigned long size, int prot, int flags, const char *name);
extern void liblinux_pal_populate_size_per_page(void *rvaddr);
extern int liblinux_pal_page_alloc_extend(unsigned long start, unsigned long end,
					  unsigned long size, unsigned long *rvaddr, unsigned long long *rpaddr,
					  int flags);
extern int liblinux_pal_page_alloc_populate(const void *start, unsigned long size);
extern int liblinux_pal_page_alloc_unpopulate(const void *start, unsigned long size);
extern unsigned long long liblinux_pal_current_mm(void);
extern int liblinux_pal_vm_zap_ptes(unsigned long addr, unsigned long len, unsigned long long mm);

/* GFP related */
#define LIBLINUX_PAL_GFP_DMA		0x01U
#define LIBLINUX_PAL_GFP_DETACH		0x02U
#define LIBLINUX_PAL_GFP_ZERO		0x10U
#define LIBLINUX_PAL_GFP_PGTBL          0x20U /* alloc gpu smmu pt pages */
#define LIBLINUX_PAL_GFP_PGD            0x40U /* alloc gpu smmu pgd pages */
#define LIBLINUX_PAL_GFP_NORETRY	0x10000U
#define LIBLINUX_PAL_GFP_NO_RESVMEM	0x20000U
#define LIBLINUX_PAL_GFP_DIRECT_RECLAIM	0x400U
#define LIBLINUX_PAL_GFP_NOFAIL	        0x8000U
#define LIBLINUX_PAL_GFP_KSWAPD_SELF 0x40000U
#define LIBLINUX_PAL_GFP_MAYSLOW     0x100000U
#define LIBLINUX_PAL_GFP_NOEXTRA	0x8000000U

/* for ion */
#define LIBLINUX_PAL_GFP_NODOWNGRADE    0x4000000U

/* cache related */
/*
 * cmd : mixture of flush flags and flush cmd, the upper four bits indicate
 * the flush flag, and the lower 28 bits indicate the flush cmd.
 * |31..28|27.........0|
 * | flag |     cmd    |
 */
/* flush cmd */
#define LIBLINUX_PAL_CACHE_FLUSH_RANGE		0U
#define LIBLINUX_PAL_DCACHE_FLUSH_RANGE		1U
#define LIBLINUX_PAL_ICACHE_FLUSH_RANGE		2U
#define LIBLINUX_PAL_DCACHE_CLEAN_RANGE		3U
#define LIBLINUX_PAL_DCACHE_INVAL_RANGE		4U
#define LIBLINUX_PAL_CACHE_FLUSH_ALL		5U
#define LIBLINUX_PAL_DCACHE_FLUSH_ALL		6U
#define LIBLINUX_PAL_ICACHE_FLUSH_ALL		7U
#define LIBLINUX_PAL_DCACHE_CLEAN_ALL		8U
#define LIBLINUX_PAL_FLUSH_CMD_MASK		0xFFFFFFF

/* flush flag */
#define LIBLINUX_PAL_FLUSH_USER_ADDR		0x40000000U
extern void liblinux_pal_flush_cache_range(unsigned int cmd,
					   unsigned long start, unsigned long end);
extern void liblinux_pal_flush_pgtbl_cache(unsigned int pid, unsigned long vstart, unsigned long vend);

/* mm related */
extern void *liblinux_pal_ioremap(unsigned long phys_addr,
				  unsigned long size, unsigned long prot, const void *caller);
extern void *liblinux_pal_ioremap_ex(unsigned long phys_addr, unsigned long size,
				     unsigned long prot, unsigned int flags, const void *caller);
extern
int liblinux_pal_remap_pfn_range(unsigned long addr, unsigned long pfn, unsigned long size,
				 unsigned long prot, unsigned int flags);
extern
int liblinux_pal_remap_pa_range_batch(unsigned long addr, const void *pa_array, unsigned int pa_array_size,
				      unsigned long prot, unsigned int flags);
int liblinux_pal_remap_pa_range_batch_sp(unsigned long addr, const void *pfn_range_array,
					 unsigned int cnt, unsigned long prot, unsigned int flags, int pid, unsigned long long vr_key);
int liblinux_pal_remap_pa_range_batch_mm(unsigned long addr, const void *pa_array,
					 unsigned int pa_array_size, unsigned long prot,
					 unsigned int flags, unsigned long long vs_key);
int liblinux_pal_mm_ama_ddr_intr(unsigned int flow_type, unsigned int ch);
#define LIBLINUX_PAL_REMAP_NONE			0
#define LIBLINUX_PAL_REMAP_CACHE		0x1
#define LIBLINUX_PAL_REMAP_NORMAL_NC		0x2
#define LIBLINUX_PAL_REMAP_DEVICE		0x4
#define LIBLINUX_PAL_REMAP_DMA_BUF		0x8
#define LIBLINUX_PAL_VM_USERMAP			0x10U
#define LIBLINUX_PAL_REMAP_PBHA_SHIFT	5
#define LIBLINUX_PAL_REMAP_PBHA	(0xFU << LIBLINUX_PAL_REMAP_PBHA_SHIFT) // bit[8:5]
#define LIBLINUX_PAL_REMAP_SH_SHIFT	9
#define LIBLINUX_PAL_REMAP_SH		(0x3U << LIBLINUX_PAL_REMAP_SH_SHIFT) // bit[10:9]
#define LIBLINUX_PAL_VM_TO_USER			(1U << 31)

extern int liblinux_pal_iofast_page_info_init(unsigned long pg_struct_va_start, size_t pg_struct_size,
					      unsigned int pg_slab_shift);
extern void *liblinux_pal_vm_prepare(unsigned long vaddr, unsigned long size,
				     unsigned long prot, unsigned int flags);
extern int liblinux_pal_vm_mmap(unsigned long paddr, unsigned long vaddr,
				unsigned long size, unsigned long prot,
				unsigned int flags);
extern int liblinux_pal_vm_unpopulate(void *addr, unsigned long size);
extern int liblinux_pal_vm_mmap_batch(const void *pa_array,
				      unsigned int pa_array_size,
				      unsigned long vaddr,
				      unsigned long prot,
				      unsigned int flags);
extern int liblinux_pal_vm_unmap(const void *addr);
extern int liblinux_pal_vm_zap(unsigned long addr, unsigned long len, unsigned int flags);
/* `pid` is root pid_ns val */

/* same with `struct vm_unmapped_area_info` */
struct pal_vm_info {
	unsigned long flags;
	unsigned long length;
	unsigned long low_limit;
	unsigned long high_limit;
	unsigned long align_mask;
	unsigned long align_offset;
};
#define VM_UNMAPPED_AREA_TOPDOWN 1
#define VM_UNMAPPED_AREA_XPM 2
/* extended flag: same with sysmgr */
#define VM_PERSISTED_MAP	0x80000000
extern unsigned long liblinux_pal_usermap_alloc_va(const struct pal_vm_info *info);
extern unsigned long liblinux_pal_usermap_prepare(unsigned long addr, unsigned long len,
						  unsigned long prot, unsigned long flags,
						  void **priv);
extern void liblinux_pal_usermap_finish(const void *priv, int success);
extern int liblinux_pal_usermap_munmap(unsigned long addr, unsigned long len);
extern void *liblinux_pal_find_vma(unsigned long addr);
extern void *liblinux_pal_find_vma_by_mm(unsigned long addr, unsigned long long vspace_key);

extern int liblinux_pal_copy_from_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_to_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_in_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_strcpy_from_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_from_caller(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_to_caller(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_msghdr_from_user(void *dst, void *src);
extern int liblinux_pal_copy_msghdr_to_user(void *dst, void *src, void *uaddr, void *ucmsg, int ulen);

extern int liblinux_pal_rpc_invoke(uint64_t rpc_key, int cmd, void *rd, size_t rd_size,
				   void *wr, size_t wr_size, bool is_xact);
extern int liblinux_pal_proxy_udk_invoke(int cmd, void *rd, size_t rd_size,
					 void *wr, size_t wr_size);
extern int liblinux_proxy_udk_invoke_xact(int cmd, void *rd, size_t rd_size,
					  void *wr, size_t wr_size);
extern int liblinux_pal_set_pageattr(const void *addr, int numpages,
				     int set_prot, int clear_prot);

extern int liblinux_pal_set_pageattr_ex(const void *addr, int numpages,
					int set_prot, int clear_prot);
/* kernel module support */
int liblinux_pal_module_alloc_init(const void *base, unsigned long size);
#ifdef CONFIG_HKIP
int liblinux_pal_module_alloc_prepare(void *addr, unsigned long size);
int liblinux_pal_module_alloc_release(void *addr, unsigned long *psize_out);
#else
int liblinux_pal_module_alloc_release(const void *addr, unsigned long *psize_out);
int liblinux_pal_module_alloc_prepare(const void *addr, unsigned long size);
#endif
int liblinux_pal_module_alloc_prepare_hkip(void *addr, unsigned long size);

extern int liblinux_pal_request_module(const char *modname);

struct liblinux_syminfo {
	void *__start___ksymtab;
	void *__stop___ksymtab;
	void *__start___ksymtab_gpl;
	void *__stop___ksymtab_gpl;
	void *__start___ksymtab_gpl_future;
	void *__stop___ksymtab_gpl_future;
	void *__start___kcrctab;
	void *__start___kcrctab_gpl;
	void *__start___kcrctab_gpl_future;
};
extern int liblinux_init_symbol_module(const char *modname,
		const struct liblinux_syminfo *si);

struct liblinux_mod_syminfo {
	const void *symtab;
	unsigned int symtab_len;
	const char *strtab;
	unsigned int strtab_len;
	const void *module_core;
	unsigned int core_size;
};
extern int liblinux_pal_fill_mod_symbol(const char *name, const struct liblinux_mod_syminfo *si);
extern int liblinux_pal_drop_ko_symbol(const void *module_core);

/* thread related */
typedef struct {
	unsigned long long handle;
	int tid;
	unsigned long long tcb_cref;
} liblinux_pal_thread_handle_t;

int liblinux_pal_thread_create(void *(*fun)(void *), void *arg,
			       liblinux_pal_thread_handle_t *handle);
int   liblinux_pal_thread_create_ex(void *(*fun)(void *), void *arg,
				    liblinux_pal_thread_handle_t *handle);
/* create thread inited */
struct task_struct;
int liblinux_pal_thread_create_task_init(void *(*func)(void *), void *arg,
					 liblinux_pal_thread_handle_t *handle,
					 struct task_struct *task,
					 void (*task_init)(struct task_struct *task,
							   liblinux_pal_thread_handle_t *handle));

void  liblinux_pal_thread_exit(void) ;
void  liblinux_pal_thread_exit_ex(void *exitcode);
int   liblinux_pal_thread_join_ex(const liblinux_pal_thread_handle_t *handle,
				  void **exitcode);
unsigned long liblinux_pal_thread_stack_info(const liblinux_pal_thread_handle_t *handle, unsigned long *base);
unsigned long long liblinux_pal_thread_actv_cref(void);
unsigned long long liblinux_pal_thread_sched_cref(void);
int   liblinux_pal_thread_set_my_data(const void *data);
void *liblinux_pal_thread_get_my_data(void);
int liblinux_pal_thread_set_freezable(void);
int liblinux_pal_thread_try_to_freeze(void);
bool liblinux_pal_thread_freezing(void);
void liblinux_pal_thread_freezer_count(bool count, bool fwd);
void  liblinux_pal_thread_init(void);
int liblinux_pal_thread_init_ex(void (*dtor)(void *));
void liblinux_pal_thread_specific_init(unsigned int *offset, unsigned int *key);
void liblinux_pal_thread_yield(void);
int liblinux_pal_getpriority_by_tid(int tid);
int liblinux_pal_sched_setaffinity(int tid, unsigned long mask);
int liblinux_pal_sched_getaffinity(int tid, unsigned long *mask);
int liblinux_pal_thread_setname(const liblinux_pal_thread_handle_t *handle,
				const char *thread_name);
int liblinux_pal_thread_setaffinity(const liblinux_pal_thread_handle_t *handle,
				    unsigned long mask);
int liblinux_pal_thread_getaffinity(const liblinux_pal_thread_handle_t *handle,
				    unsigned long *mask);
int liblinux_pal_thread_setscheduler(const liblinux_pal_thread_handle_t *handle,
				     int prio, int policy);
int liblinux_pal_is_thread_ctx(void);
int liblinux_pal_set_user_nice(int tid, int nice);
void liblinux_pal_thread_dump(pid_t tid);
int liblinux_pal_thread_inactive(const liblinux_pal_thread_handle_t *handle);
int liblinux_pal_query_main_thread(int tid, int *main_thread_tid, char *main_thread_name, unsigned int name_len);
int liblinux_pal_thread_set_vip_prio(const liblinux_pal_thread_handle_t *handle,
				     int prio);
int liblinux_pal_thread_set_vip_prio_by_tid(int tid, int prio);
void liblinux_pal_thread_set_power_group(const liblinux_pal_thread_handle_t *handle);
void liblinux_pal_thread_set_task_boost(const liblinux_pal_thread_handle_t *handle, int boost);
void liblinux_pal_thread_set_pin_preempted(void);
int liblinux_pal_proc_lock_pid(int pid);
int liblinux_pal_proc_unlock_pid(int pid);

unsigned int liblinux_pal_processor_id(void);
int liblinux_pal_cpu_config(unsigned long *online_mask,
			     unsigned long *cpu_map,
			     unsigned long cpu_nr,
			     int preempt_flag);
int liblinux_pal_cpu_config_ex(unsigned long *online_mask, unsigned long size,
			       int preempt_flag);
int liblinux_pal_cpu_topology(unsigned int *topo, unsigned int size);

void liblinux_pal_set_iowait(void);
void liblinux_pal_clr_iowait(void);

void liblinux_pal_thread_get_priority(int *rt_prio, int *nice);
int liblinux_pal_get_thread_name(pid_t tid, char *comm, int len);
int liblinux_pal_get_current_exec_path(char *buf, size_t len);

/* lock related */
#define LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE	1
#define LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT	2
#define LIBLINUX_PAL_FUTEX_LOCK_IN_LDK		4
#define LIBLINUX_PAL_FUTEX_IOWAIT		8
#define LIBLINUX_PAL_FUTEX_KILLABLE		16
#define LIBLINUX_PAL_FUTEX_MAX_TIMEOUT		~0ULL

struct liblinux_wchan_info {
	char *name;
	unsigned int name_len;
	unsigned long offset;
	unsigned long size;
};

int liblinux_pal_futex_timedwait(volatile int *futex, int val,
				 unsigned long long timeout, /* ns */
				 unsigned long flags);
int liblinux_pal_futex_timedwait_ex(volatile int *futex, int val,
				    unsigned long long timeout, /* ns */
				    unsigned long flags,
				    const struct liblinux_wchan_info *wchan_info);
int liblinux_pal_futex_wake(volatile int *futex);
int liblinux_pal_futex_wake_ex(volatile int *futex, int sync);
#ifdef CONFIG_USER_FUTEX
int liblinux_pal_thread_block(volatile int *futex, int val,
				 unsigned long long timeout, /* ns */
				 unsigned long flags,
				 const struct liblinux_wchan_info *wchan_info);
int liblinux_pal_thread_unblock(unsigned long long tcb_cref, volatile int *futex, int sync);
#endif /* CONFIG_USER_FUTEX */

#ifdef CONFIG_BIG_MODEL_MONITOR
int liblinux_pal_mpam_event_ctrl(void *event, unsigned int size);
int liblinux_pal_mpam_event_bind(unsigned long long cref);
int liblinux_pal_mpam_event_unbind(unsigned long long cref);
int liblinux_pal_mpam_event_wait(void);
int liblinux_pal_mpam_event_read(void *buf, unsigned long buf_size);
#endif

/* This data structure and initializer should be aligned to
 * libhmsync/raw_mutex.h
 */
struct liblinux_pal_mutex_t {
	struct {
		union {
			unsigned long id;
			unsigned long long align_id; /* ensure align in 32 and 64 data model */
		};
		union {
			const void *pc;
			unsigned long long align_pc; /* ensure align in 32 and 64 data model */
		};
	} owner;
	unsigned int lock;
	unsigned int waiters;
	unsigned long __rsvd[4];        /* padding for pthread */
};

#define LIBLINUX_MUTEX_INITIALIZER {.owner = {{.align_id = 0ULL}, {.align_pc = 0ULL}}, .lock = 0U, .waiters = 0U}

void liblinux_pal_mutex_init(struct liblinux_pal_mutex_t *mutex);
void liblinux_pal_mutex_destroy(struct liblinux_pal_mutex_t *mutex);
void liblinux_pal_mutex_lock(struct liblinux_pal_mutex_t *mutex);
int  liblinux_pal_mutex_trylock(struct liblinux_pal_mutex_t *mutex);
void liblinux_pal_mutex_unlock(struct liblinux_pal_mutex_t *mutex);
int liblinux_pal_mutex_is_locked(struct liblinux_pal_mutex_t *mutex);

/* This data structure should be aligned to
 * libhmsync/raw_thread_cond.h
 */
struct liblinux_pal_cond_t {
	int seq;
	int waiters;
	int lock;

	void *head;
	void *tail;

	unsigned int attr;
	unsigned long __rsvd[4];        /* padding for pthread */
};

struct liblinux_pal_cond_attr_t {
	unsigned int attr;
};

int liblinux_pal_cond_init(struct liblinux_pal_cond_t *cond,
			   const struct liblinux_pal_cond_attr_t *attr);
int liblinux_pal_cond_destroy(struct liblinux_pal_cond_t *cond);
int liblinux_pal_cond_wait(struct liblinux_pal_cond_t *cond, struct liblinux_pal_mutex_t *mutex);
int liblinux_pal_cond_signal(struct liblinux_pal_cond_t *cond);
int liblinux_pal_cond_broadcast(struct liblinux_pal_cond_t *cond);

/* This data structure should be aligned to
 * libhmsync/raw_sem.h
 */
struct liblinux_pal_sem_t {
	volatile int value;
	volatile unsigned int waiters;
	unsigned long __rsvd[4];        /* padding for pthread */
};
struct liblinux_pal_timespec_t {
	long ts_sec;
	long ts_nsec;
};

#define LIBLINUX_SEM_INITIALIZER(n) {.value = n, .waiters = 0}

int liblinux_pal_sem_init(struct liblinux_pal_sem_t *sem, unsigned int value);
int liblinux_pal_sem_trywait(struct liblinux_pal_sem_t *sem);
int liblinux_pal_sem_wait(struct liblinux_pal_sem_t *sem);
int liblinux_pal_sem_timedwait(struct liblinux_pal_sem_t *sem,
			       const struct liblinux_pal_timespec_t *ts);
int liblinux_pal_sem_post(struct liblinux_pal_sem_t *sem);
int liblinux_pal_sem_getvalue(struct liblinux_pal_sem_t *sem);

enum linux_log_level {
	LIBLINUX_LOGLEVEL_EMERG,
	LIBLINUX_LOGLEVEL_ALERT,
	LIBLINUX_LOGLEVEL_CRIT,
	LIBLINUX_LOGLEVEL_ERR,
	LIBLINUX_LOGLEVEL_WARNING,
	LIBLINUX_LOGLEVEL_NOTICE,
	LIBLINUX_LOGLEVEL_INFO,
	LIBLINUX_LOGLEVEL_DEBUG,
	LIBLINUX_LOGLEVEL_CONT = -3, /* KERN_CONT */
};
void liblinux_pal_log_store(int level, const char *txt, int txt_len);
int liblinux_pal_kptr_restrict(void);
bool liblinux_pal_kptr_show_value(void);

/* procfs related */
struct liblinux_pal_kernfs_ops {
	int (*read)(unsigned long long pos, unsigned long ubuf, unsigned long size,
		    void *ctx, unsigned long *rsize /* out */);
	int (*write)(unsigned long long pos, unsigned long ubuf, unsigned long size,
		     void *ctx, unsigned long *wsize /* out */);
	int (*poll)(int (*callback)(void *ctx, int pollable), int id, int pollable);
	void *(*open)(void *pathname, unsigned int flags, unsigned int mode);
	void (*close)(void *filep);
	void (*flush)(void);
};

int liblinux_pal_procfs_register_ops(struct liblinux_pal_kernfs_ops *kernfs_ops);
int liblinux_pal_procfs_create_file(const char *name,
				    struct liblinux_pal_kernfs_ops *ops,
				    void *ctx, unsigned int mode);
int liblinux_pal_procfs_create_file_bypath(const char *name, unsigned int mode);
int liblinux_pal_procfs_create_dir(const char *name, unsigned int mode);
int liblinux_pal_procfs_create_link(const char *name, unsigned int mode,
				    char *linkdata);
int liblinux_pal_procfs_delete(const char *name);
int liblinux_pal_procfs_delete_bypath(const char *name);
/* request firmware related */
int liblinux_pal_fw_acquire_contents(const char *filename, void *data,
				     unsigned int size, unsigned int *rsize /* out */);

/* wakeup source related */
int liblinux_pal_ws_activate(void);
int liblinux_pal_ws_deactivate(void);

/* fs related */
enum liblinux_fd_type {
	DEFAULT_FD = 0,
	ANON_FD,
	TRANSFS_FD,
	NET_FD,
	MAX_FD, /* always be the last one */
};

enum liblinux_fs_type {
	LIBLINUX_SYSFS = 1,
	MAX_FS_TYPE, /* always be the last one */
};
void *liblinux_pal_fget(int fd);
void *liblinux_pal_transfs_fget(int fd);
void liblinux_pal_transfs_fput(void *file);
void *liblinux_pal_gdentry(const char *pathname);
void liblinux_pal_pdentry(void *sb, void *dentry);
void *liblinux_pal_fget_ex(enum liblinux_fd_type fd_type, int fd, void (*fget)(void*));
void liblinux_pal_fput_ex(enum liblinux_fd_type fd_type, void *file);
int liblinux_pal_get_unused_fd_flags(unsigned flags);
void liblinux_pal_fd_install(unsigned int fd, void *file);
void *liblinux_pal_pidfd_anon_fget(int pid, int fd, void (*fget)(void *));
int liblinux_pal_ipermission(const char *pathname, int mask);
struct dh_fd_install_context {
	const char *file_name;
	unsigned int buffer_len;
	unsigned int f_flags;
};
void liblinux_pal_fd_install_ex(unsigned int fd, void *file, struct dh_fd_install_context *ctx);
int liblinux_pal_alloc_unused_fd(unsigned int flags, void *file, struct dh_fd_install_context *ctx);
void liblinux_pal_put_unused_fd(int fd);
int liblinux_pal_require_frange(int type, int *min, int *max);

struct stat;
struct statfs;
struct iovec;
int liblinux_pal_ksys_access(const char *pathname, int mode);
int liblinux_pal_vfs_open(const char *filename, unsigned int flag, mode_t mode);
ssize_t liblinux_pal_ksys_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte);
ssize_t liblinux_pal_ksys_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t liblinux_pal_ksys_pread(int fd, int64_t pos, void *buf, size_t nbyte);
ssize_t liblinux_pal_ksys_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
int liblinux_pal_vfs_close(int fd);
int liblinux_pal_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int liblinux_pal_vfs_fstat(int fd, struct stat *stat);
int liblinux_pal_ksys_fstat(int fd, struct stat *stat);
int liblinux_pal_ksys_fstat_nocheck(int fd, struct stat *stat);
int liblinux_pal_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags);
int liblinux_pal_ksys_statfs(const char *pathname, struct statfs *statfs);
int liblinux_pal_ksys_rename(const char *oldpath, const char *newpath);
int liblinux_pal_ksys_readlinkat(int dfd, const char *path, char *buf, size_t bufsize);
int liblinux_pal_ksys_mkdir(const char *pathname, mode_t mode);
int liblinux_pal_ksys_rmdir(const char *pathname);
int liblinux_pal_ksys_unlink(const char *pathname);
int liblinux_pal_ksys_fchown(int fd, uid_t uid, gid_t gid);
long liblinux_pal_ksys_symlink(const char *oldname, const char *newname);

int liblinux_pal_ksys_open(const char *filename, unsigned int flag, mode_t mode);
int liblinux_pal_ksys_openat(int dfd, const char *filename, unsigned int flag, mode_t mode);
int liblinux_pal_ksys_close(int fd);
int liblinux_pal_ksys_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int liblinux_pal_ksys_lseek(int fd, int64_t offset, int whence, int64_t *pos);
ssize_t liblinux_pal_ksys_write(int fd, const void *buf, size_t nbyte);
ssize_t liblinux_pal_ksys_read(int fd, void *buf, size_t nbyte);
ssize_t liblinux_pal_vfs_read(int fd, void *buf, size_t nbyte);
ssize_t liblinux_pal_ksys_readdir(int fd, void *buf, size_t nbyte);
int liblinux_pal_ksys_fsync(int fd);
int liblinux_pal_ksys_sync(void);

/* sysfs related */
int liblinux_pal_sysfs_register_ops(void *ops);
int liblinux_pal_sysfs_register_ops_ex(void *ops);

int liblinux_pal_sysfs_create_dir(const char *name, unsigned int mode);

int liblinux_pal_sysfs_create_file(const char *name, unsigned int mode,
				   const struct liblinux_pal_kernfs_ops *fops,
				   void *ctx);

int liblinux_pal_sysfs_create_link(const char *name, unsigned int mode,
				   const char *linkdata);

int liblinux_pal_sysfs_delete(const char *name);
int liblinux_pal_sysfs_notify(const void *ctx, const char *name);
int liblinux_pal_sysfs_rename_link(const char *old_name, const char *new_name);

/* sysfs namespace related */
int liblinux_pal_sysfs_register_ops_ex_ns(void *ops);

int liblinux_pal_sysfs_create_file_ex(const char *name, unsigned int mode, unsigned long long size,
				   const struct liblinux_pal_kernfs_ops *fops,
				   void *ctx, unsigned int nsid);

int liblinux_pal_sysfs_create_dir_ns(const char *name, unsigned int mode, unsigned int nsid);

int liblinux_pal_sysfs_create_link_ns(const char *name, unsigned int mode,
									  const char *linkdata, unsigned int nsid);

int liblinux_pal_sysfs_delete_ns(const char *name, unsigned int nsid);

int liblinux_pal_sysfs_rename_link_ns(const char *old_name, const char *new_name,
									  unsigned int old_nsid, unsigned int new_nsid);

int liblinux_pal_sysfs_create_data_batch(void *ldk_entry, int cnt);

/* preempt related */
void *liblinux_pal_preempt_init(void);
void liblinux_pal_preempt_resched(void);
void liblinux_pal_preempt_enable(void);
void liblinux_pal_preempt_disable(void);
void liblinux_pal_process_set_slv(unsigned int cpu, unsigned int slv);
void liblinux_pal_thread_set_slv(const liblinux_pal_thread_handle_t *handle, unsigned int slv);

struct tty_info {
	int tty_sid;
	unsigned int tty_dev_no;
	char *tty_name;
	unsigned int f_mode;
};
int liblinux_pal_set_control_terminal(struct tty_info *info, int preempt);
int liblinux_pal_get_control_terminal_devno(unsigned int *devno);
int liblinux_pal_get_sid(void);
int liblinux_pal_get_sid_pgid(int *sid, int *pgid);
int liblinux_pal_get_pgid(void);
int liblinux_pal_set_pgrp(int tty_sid, int pgrp);
int liblinux_pal_get_pgrp(int tty_sid);
void liblinux_pal_process_signal(int sid, int sig);
void liblinux_pal_process_signal_with_pid(int pid, int sig);
#define PERI_DMA_WRITE_INFO_MAX 8
struct peri_dma_write_info {
	unsigned int val[PERI_DMA_WRITE_INFO_MAX];
	unsigned int offset[PERI_DMA_WRITE_INFO_MAX];
	unsigned int count;
};
void liblinux_pal_peri_dma_write(struct peri_dma_write_info* info);
int liblinux_pal_signal_isignored(int pid, int sig);
int liblinux_pal_current_pgrp_isorphaned(void);
void liblinux_pal_sys_shutdown(void);
void liblinux_pal_sys_snapshot(void);

/* kasan */
extern void liblinux_pal_kasan_poison(const void *address,
				      unsigned long size, unsigned char value);
extern void liblinux_pal_kasan_check(unsigned long addr, unsigned long size,
				     unsigned int is_write, unsigned long ret_addr);
extern void liblinux_pal_kasan_report(unsigned long addr, unsigned long size,
				      unsigned int is_write, unsigned long ret_addr);

/* mem stat dfx api */
enum liblinux_dfx_mem_event {
	LIBLINUX_DFX_MEM_MIN,
	LIBLINUX_DFX_SLAB_ALLOC,
	LIBLINUX_DFX_SLAB_FREE,
	LIBLINUX_DFX_PAGE_ALLOC,
	LIBLINUX_DFX_PAGE_FREE,
	LIBLINUX_DFX_VMALLOC_ALLOC,
	LIBLINUX_DFX_VMALLOC_FREE,
	LIBLINUX_DFX_MEM_MAX,
};
typedef void (*liblinux_mem_hook_t)(int ev, void *addr, unsigned long size,
				    const void *data);
int liblinux_dfx_vmalloc_hook(liblinux_mem_hook_t hook);
int liblinux_pal_mincore(const void *addr, unsigned long size, unsigned char *vec);

/* capabilities */
int liblinux_pal_in_group_p(unsigned int gid);
int liblinux_pal_in_egroup_p(unsigned int gid);
#define LIBLINUX_CAP_OPT_NONE 0
#define LIBLINUX_CAP_OPT_NOAUDIT 1
#define LIBLINUX_CAP_OPT_INSETID 2
int liblinux_pal_capable(int cap, unsigned int opts);

/* seharmony */
struct liblinux_pal_sel_sid_context {
	unsigned int ssid;
	unsigned int tsid;
	unsigned short tclass;
};

int liblinux_pal_sel_avc_check(struct liblinux_pal_sel_sid_context *ccon, unsigned int requested);
int liblinux_pal_sel_transition_sid(struct liblinux_pal_sel_sid_context *ccon, const char *objname, unsigned int *sidp);
int liblinux_pal_sel_policycap_extsockclass(void);
int liblinux_pal_sel_initialized(void);
int liblinux_pal_sel_allow_unknown(void);
int liblinux_pal_sel_load_policy(const char *buffer, unsigned long size);
void liblinux_pal_sel_init_ctx(struct task_security_struct *sel_cred);
int liblinux_pal_sel_update_enforce(int value);
void *liblinux_pal_sel_seharmony_pool_init(void);
void *liblinux_pal_sel_seharmony_prot_pool_init(void);
void liblinux_pal_sel_verify_tampering(void *p, const char *msg);

/* prmem */
void *liblinux_pal_prmem_malloc(void *pool, size_t size, int flags);
void *liblinux_pal_prmem_calloc(void *pool, size_t n, size_t size, int flags);
char *liblinux_pal_prmem_strdup(void *pool, const char *s, int flags);
void liblinux_pal_prmem_protect_pool(void *pool);
void liblinux_pal_prmem_sec_region_protect(void);

/* signal */
extern uint64_t liblinux_pal_get_current_signals(void);
extern uint64_t liblinux_pal_get_pending_signals(void);

/* kernel livepatch */
typedef int (*klp_stacktrace_t)(unsigned long, unsigned long, unsigned long, void *);
int liblinux_pal_klp_for_each_thread(klp_stacktrace_t trace_callback, void *args);

struct liblinux_pal_rwlock_t {
	unsigned int lock;
	unsigned int waiters;
	unsigned long prev;
	unsigned long next;
	unsigned int owner_lock;
	struct {
		unsigned int wrhold	: 1;
		unsigned int shared 	: 1;
		unsigned int xdep    	: 1;
		unsigned int xdep_trace	: 1;
		unsigned int reserved 	: 28;
	} attr;
	/* For liblinux it is sufficient to ensure that the struct is >= then the HM
	   definition, so to simplify things we just unconditionally define this
	   member in liblinux. liblinux code never directly accesses the struct
	   members if the lock was created by HM. */
	unsigned int lock_id;
};

void liblinux_pal_rwlock_init(struct liblinux_pal_rwlock_t *lock);
int  liblinux_pal_rwlock_tryrdlock(struct liblinux_pal_rwlock_t *lock);
void liblinux_pal_rwlock_rdlock(struct liblinux_pal_rwlock_t *lock);
int  liblinux_pal_rwlock_trywrlock(struct liblinux_pal_rwlock_t *lock);
void liblinux_pal_rwlock_wrlock(struct liblinux_pal_rwlock_t *lock);
void liblinux_pal_rwlock_unlock(struct liblinux_pal_rwlock_t *lock);
void liblinux_pal_rwlock_destroy(struct liblinux_pal_rwlock_t *lock);

extern int liblinux_tick_stopped(void);

/* liblinux shrink */
struct liblinux_pal_sub_shrinker {
	unsigned long (*query)(void *priv);
	unsigned long (*shrink)(void *priv, int free_nr);
	void *priv;
};

void liblinux_pal_sub_shrinker_register(const struct liblinux_page_allocator *_allocator,
					struct liblinux_pal_sub_shrinker shrinker);
void liblinux_pal_sub_shrinker_unregister(const struct liblinux_page_allocator *_allocator,
					struct liblinux_pal_sub_shrinker shrinker);

unsigned long liblinux_pal_query_pgd(unsigned int pid);

void liblinux_pal_fpu_save(void);
void liblinux_pal_fpu_restore(void);

int liblinux_pal_get_processname_by_tid(__u32 calling_tid, char *path, uint32_t path_len);

/* cma */
int liblinux_pal_cma_area_init(unsigned long paddr, unsigned long vaddr, unsigned long size,
			       unsigned int cma_id, const char *name);
int liblinux_pal_cma_alloc(unsigned int cma_id, unsigned long size, unsigned int align_order,
			   unsigned long *paddr);
int liblinux_pal_cma_release(unsigned int cma_id, unsigned long vaddr,
			     unsigned long paddr, unsigned long size);

/* dmabuf swap */
int liblinux_pal_mem_swap_space_init(int swap_id, unsigned long total_size, unsigned int ext_size);
int liblinux_pal_mem_swap_alloc_extentid(int swap_id, int *ext_id);
int liblinux_pal_mem_swap_in_range(int swap_id, unsigned long *pa_array, int *ext_ids, int num);
int liblinux_pal_mem_swap_out_range(int swap_id, unsigned long *pa_array, int *ext_ids, int num);
void liblinux_pal_mem_swap_free_extentid(int swap_id, int ext_id);

/* process */
int liblinux_pal_process_dormancy_check(void);

/* sd */
int liblinux_pal_sd_ida_alloc(void);
void liblinux_pal_sd_ida_free(int sd_index);

extern void liblinux_pal_sync_wakeup_trad(void *filp_node, unsigned int revents);
extern void liblinux_pal_sync_wakeup(void *kobj_ctx, unsigned int revents);
extern void *liblinux_pal_sync_open(void *kobj_ctx);
extern void liblinux_pal_sync_close(void *kobj_ctx);

/* liblinux mm */
int liblinux_pal_get_user_pages(unsigned long vaddr, size_t len, unsigned long long *ret_pa_array,
				size_t array_size);
int liblinux_pal_put_user_pages(unsigned long vaddr, size_t len, unsigned long long *ret_pa_array,
				size_t array_size);
int liblinux_pal_charge_mlock(unsigned long mlock_size);
int liblinux_pal_uncharge_mlock(unsigned long mlock_size);
void liblinux_pal_mem_enable_rsv_mem(unsigned long pfn_start, unsigned long pfn_end);

void liblinux_pal_change_secpage_range(unsigned long long phys_addr, unsigned long vaddr,
				       unsigned long size, unsigned int flags);

/* cpa heap */
#define LIBLINUX_PAL_CPA_PREPARE_CAN_DROP (1U << 0)
#define LIBLINUX_PAL_CPA_PREPARE_CAN_KILL (1U << 1)
#define LIBLINUX_PAL_CPA_PREPARE_CAN_RECLAIM (1U << 2)
#define LIBLINUX_PAL_CPA_PREPARE_CAN_COMPAT (1U << 3)

#define LIBLINUX_PAL_CPA_PREPARE_KILL_ZRAM (1U << 4)
#define LIBLINUX_PAL_CPA_PREPARE_KILL_FREEMEM 0

#define LIBLINUX_PAL_CPA_PREPARE_COMPAT_MODEL_0 0
#define LIBLINUX_PAL_CPA_PREPARE_COMPAT_MODEL_1 (1U << 5)

#define LIBLINUX_PAL_CPA_PREPARE_MODE_WORK 0
#define LIBLINUX_PAL_CPA_PREPARE_MODE_ALLOC (1U << 6)
#define LIBLINUX_PAL_CPA_PREPARE_MODE_REMAIN (1U << 7)

struct liblinux_pal_cpa_prepare_memory_args {
	unsigned int flags;
	unsigned long water_mark_page;
	unsigned long heap_size;
	unsigned long allocated_size;

	unsigned long alloc_size;
	unsigned int order;
};
void liblinux_pal_cpa_prepare_memory(struct liblinux_pal_cpa_prepare_memory_args *args);
void liblinux_pal_hvc_call(unsigned long *in_regs, unsigned long *out_regs);

/* liblinux cpufreq */
enum constraint_reason {
	CONSTRAINT_SYSFS,
	CONSTRAINT_DRG,
	CONSTRAINT_DRIVER,
	CONSTRAINT_THERMAL,
	CONSTRAINT_REASON_NR,
};
unsigned int liblinux_pal_cpufreq_quick_get(unsigned int cpuid);
unsigned int liblinux_pal_cpufreq_quick_get_max(unsigned int cpuid);
unsigned int liblinux_pal_cpufreq_quick_get_hw_max(unsigned int cpuid);
unsigned long long liblinux_pal_get_cpu_idle_time(unsigned int cpuid, unsigned long long *wall, int io_busy);
int liblinux_pal_set_max_cpufreq(unsigned int cpuid, unsigned int target_freq, enum constraint_reason reason);

/* rtc */
struct liblinux_pal_rtc_ops {
	int (*read_time)(unsigned long *sec);
	int (*set_time)(unsigned long sec);
	int (*read_alarm)(unsigned long *sec, unsigned int *pending,
			  unsigned int *enable);
	int (*set_alarm)(unsigned long sec, unsigned int enable);
	int (*irq_enable)(unsigned int enable);
};
int liblinux_pal_rtc_ops_init(struct liblinux_pal_rtc_ops *ops);
typedef int (*set_power_on_alarm_t)(long, bool);
int liblinux_pal_register_rtc_ops(set_power_on_alarm_t set_power_on_alarm);

/* random */
int liblinux_pal_get_random_bytes(void *buf, int nbytes);
int liblinux_pal_get_random_u32(uint32_t *value);
int liblinux_pal_get_random_u64(uint64_t *value);

/* extfrag index */
int liblinux_pal_mm_extfrag_index_read(void *buf, size_t len);

#ifdef CONFIG_HISI_THERMAL_INTELLIGENT_FREQ_CTRL
int liblinux_pal_cooling_freq_limit(uint32_t cooling_step);
#endif

#define LIBLINUX_SYSFS_MAX 0x00FFFFFFU /* Maximum number of files supported by ldk */

#endif /* !__ASSEMBLY__ */
#endif /* __LIBLINUX_PAL_H__ */
