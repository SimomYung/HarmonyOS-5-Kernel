/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_PAL_H__
#define __LIBLINUX_PAL_H__

#include <linux/types.h>

#ifdef CONFIG_LIBLINUX
#ifndef __ASSEMBLY__

/* liblinux exported symbols */
extern void liblinux_vendor_hook_init(void);
extern void liblinux_kernel_init(const char *cmdline);
extern void liblinux_do_initcalls(void);
extern void liblinux_smp_init(void);
extern int  liblinux_setup_fdt(void *dt_virt);
extern void liblinux_enter(int pid, ...);
extern void liblinux_exit(void);
extern int  liblinux_thread_setup(void);
extern void *liblinux_mm_prepare_map(unsigned long long pa, unsigned long size);
extern void liblinux_mm_prepare_unmap(void *va, unsigned long size);

struct liblinux_suspend_core_ops {
	/* callbacks for suspend mem */
	int (*suspend)(void);
	void (*resume)(void);
	/* callbacks for suspend disk */
	int (*freeze)(void);
	void (*thaw)(void);
};

extern int liblinux_freeze_wqs(void);
extern void liblinux_thaw_wqs(void);
extern int liblinux_suspend(void);
extern int liblinux_suspend_prepare(int force);
extern void liblinux_resume(void);
extern void liblinux_resume_finish(void);
extern int liblinux_suspend_late(void);
extern void liblinux_resume_early(void);
extern int liblinux_add_cpu(unsigned int cpu);
extern int liblinux_remove_cpu(unsigned int cpu);
extern int liblinux_suspend_core_ops_register(struct liblinux_suspend_core_ops *ops);

#ifdef CONFIG_LIBLINUX_HAVE_ULSR
extern void liblinux_ulsr_enter(void);
extern void liblinux_ulsr_leave(void);
#endif

extern void liblinux_time_sync(void);
extern void liblinux_time_sync_early(void);
extern void liblinux_current_thread_fill(void);
extern void liblinux_preempt_bypass(bool bypass);
extern void liblinux_profile_task_exit(unsigned long pid, unsigned long uid);

extern int liblinux_ext_hvgr_mmu_cbit_config(void);
extern int liblinux_ext_hvgr_mmu_irq_mask(unsigned int mask);
extern int liblinux_ext_hvgr_mmu_irq_disable(void);
extern int liblinux_ext_hvgr_mmu_irq_enable(void);
extern int liblinux_ext_hvgr_mmu_clear_fault(unsigned int no, unsigned int fault_type);
extern int liblinux_ext_hvgr_mmu_unmask_fault(unsigned int no, unsigned int fault_type);
extern int liblinux_ext_hvgr_mmu_hal_unlock(unsigned int asid);
extern int liblinux_ext_hvgr_mmu_hal_enable(unsigned long long pgd, unsigned int asid);
extern int liblinux_ext_hvgr_mmu_hal_disable(unsigned int asid);
extern int liblinux_ext_hvgr_mmu_hal_flush_pt(unsigned int asid, unsigned long long gva,
	unsigned int pages, unsigned int level);
extern long liblinux_pal_ksys_symlink(const char *oldname, const char *newname);
extern int liblinux_ext_hvgr_mmu_hal_flush_l2_caches(void);
extern int liblinux_ext_hvgr_mmu_tbl_map(void *para);
extern int liblinux_ext_hvgr_mmu_tbl_unmap(void *para);
extern int liblinux_ext_hvgr_mmu_tbl_free_pgd(void *para);
extern int liblinux_ext_hvgr_mmu_fault_clear_as(uint32_t asid, uint64_t gva,
	uint32_t pages, uint32_t as_bits);

int liblinux_hibernate_prepare(void);
void liblinux_post_hibernate(void);
int liblinux_hibernate_restore_prepare(void);
void liblinux_hibernate_post_restore(void);
int liblinux_hibernate(int event);
void liblinux_hibernate_resume(int event);

#ifdef CONFIG_LIBLINUX_NO_HZ
extern int liblinux_nohz_idle_enter(void);
extern void liblinux_nohz_idle_exit(void);
#endif

struct liblinux_tcb_time {
	unsigned long long total;
	unsigned long long runtime;
	unsigned long long ready;
};

int liblinux_pal_get_task_sched_time(struct liblinux_tcb_time *time);
int liblinux_pal_suspend_cnt_read(int *cnt);
bool liblinux_is_suspend_success(void);

unsigned int liblinux_pal_get_log_usertype(void);

int liblinux_pal_get_cpu_total_inst(uint64_t *cpu_inst, uint32_t cpu_nr);

int liblinux_pal_get_max_capacity_cpu(int event, unsigned int *cpuid);

bool liblinux_pal_get_freq_privileged_user(void);

/* devfs related */
extern  int liblinux_pal_register_chrdev_region(unsigned int major,
		unsigned int baseminor, int minorct,
		const char *name, unsigned int *major_out);

extern void liblinux_pal_unregister_chrdev_region(unsigned int major,
		unsigned int baseminor, int minorct);

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

#ifdef CONFIG_HM_HKIP_SEHARMONY_PROT
void *liblinux_pal_sel_seharmony_pool_init(void);
void *liblinux_pal_sel_seharmony_prot_pool_init(void);
#endif

#ifdef CONFIG_HM_HKIP_PRMEM
void *liblinux_pal_prmem_malloc(void *pool, size_t size, int flags);
void *liblinux_pal_prmem_calloc(void *pool, size_t n, size_t size, int flags);
char *liblinux_pal_prmem_strdup(void *pool, const char *s, int flags);
void liblinux_pal_prmem_protect_pool(void *pool);
void liblinux_pal_prmem_sec_region_protect(void);
#endif

extern uint64_t liblinux_pal_get_current_signals(void);
extern uint64_t liblinux_pal_get_pending_signals(void);

/* IRQ related */
int liblinux_pal_local_irq_save(void);
void liblinux_pal_local_irq_enable(void);
void liblinux_pal_local_irq_disable(void);
void liblinux_pal_local_irq_restore(int disabled);
int liblinux_pal_local_save_flags(void);
int liblinux_pal_irq_set_affinity_hint(unsigned long linux_irq, unsigned long mask);

int liblinux_register_ipi_handler(int irq, int (*handler)(int, void *), unsigned int cpu);
int liblinux_unregister_ipi(int irq, unsigned int cpu);
int liblinux_pal_smp_call_cpu(int irq, unsigned int cpu);

enum ipi_msg_type {
	IPI_RESCHEDULE,
	IPI_CALL_FUNC,
	IPI_CPU_STOP,
	IPI_TIMER,
	IPI_IRQ_WORK,
	IPI_SECURE_RPMB,
	IPI_MNTN_INFORM,
	IPI_CPU_CRASH_STOP,
	IPI_WAKEUP,
	IPI_HHEE_INFORM,
	/*
	* A custom SGI ipinr which is different from Linux
	* which should be the same as that defined by the microkernel.
	* For details, see IPI_TYPE_USER_CALL_FUNC in kernel/include/hmkernel/interrupt/ipi.h
	*/
	IPI_USER_CALL_FUNC = 10,
	NR_IPI
};

/* vmalloc */
extern void *liblinux_pal_vmalloc_range_ex(unsigned long size, unsigned long align,
					unsigned long start, unsigned long end,
					unsigned long prot, unsigned long pal_vm_flags, const void *caller);
extern void liblinux_pal_vfree(const void *addr);
extern unsigned long liblinux_pal_vmalloc_nr_pages(void);
int liblinux_va_to_pa(const void *vaddr, unsigned long long *pa);
extern int liblinux_pal_remap_vmalloc_range(unsigned long uaddr, unsigned long kaddr,
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
		      unsigned int order, unsigned long *rvaddr, int recaim);
	void *(*alloc)(const struct liblinux_page_allocator *_allocator,
		       unsigned int order, unsigned long long *phys);
	void (*free)(const struct liblinux_page_allocator *_allocator,
		     unsigned long vaddr, unsigned int order);
	int (*shrink)(const struct liblinux_page_allocator *_allocator,
		     unsigned long vaddr, unsigned long long paddr, unsigned long size);
	int (*extend_ex)(const struct liblinux_page_allocator *_allocator,
			 unsigned int order, unsigned long *rvaddr, int recaim);
	int (*extend_exact)(const struct liblinux_page_allocator *_allocator,
			    unsigned long size, unsigned long *rvaddr, int recaim);
	void *(*alloc_exact)(const struct liblinux_page_allocator *_allocator,
			     unsigned long size, unsigned long long *phys);
	void (*free_exact)(const struct liblinux_page_allocator *_allocator,
			   unsigned long vaddr, unsigned long size);
	int (*extend_alloc)(const struct liblinux_page_allocator *_allocator,
			    unsigned int extend_order, unsigned int alloc_order,
			    unsigned long *rvaddr, int flags);
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

/*
 * Liblinux pal vmemmap initialization.
 */
struct liblinux_vmemmap_info {
	/* highmem info */
	unsigned long long page_addr;
	unsigned long long page_size;
	/* vmemmap region info */
	unsigned long long vmemmap_start;
	unsigned long long vmemmap_size;
	unsigned int flags;
};

#define LIBLINUX_VMEMMAP_POPULATE_ALL		0x01U
#define LIBLINUX_VMEMAP_HIGHMEM			0x02U
#define LIBLINUX_VMEMMAP_POPULATE_IOFAST	0x04U

int liblinux_pal_vmemmap_init(struct liblinux_vmemmap_info *info);
int liblinux_pal_vmemmap_is_populated(const void *start, unsigned long size);
int liblinux_pal_vmemmap_dump(char *info, unsigned long size);

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
extern int liblinux_pal_page_alloc_populate(const void *start, unsigned long size);
extern int liblinux_pal_page_alloc_unpopulate(const void *start, unsigned long size);
extern unsigned long long liblinux_pal_current_mm(void);

/* GFP related */
#define LIBLINUX_PAL_GFP_DMA			0x01U
#define LIBLINUX_PAL_GFP_DETACH			0x02U
#define LIBLINUX_PAL_GFP_ZERO			0x10U
#define LIBLINUX_PAL_GFP_PGTBL			0x20U
#define LIBLINUX_PAL_GFP_PGD			0x40U
#define LIBLINUX_PAL_GFP_NORETRY		0x10000U
#define LIBLINUX_PAL_GFP_NO_RESVMEM		0x20000U
#define LIBLINUX_PAL_GFP_NOEXTRA		0x8000000U
#define LIBLINUX_PAL_GFP_KSWAPD_SELF 0x40000U

/* for dma-buf */
#define LIBLINUX_PAL_GFP_NODOWNGRADE		0x4000000U

#ifdef CONFIG_LOCKDEP
#define LIBLINUX_PAL_GFP_RESERVE		0x2000000u
#else
#define LIBLINUX_PAL_GFP_RESERVE		0x1000000u
#endif

/* cache related */
#define LIBLINUX_PAL_CACHE_FLUSH_RANGE     0U
#define LIBLINUX_PAL_DCACHE_FLUSH_RANGE    1U
#define LIBLINUX_PAL_ICACHE_FLUSH_RANGE    2U
#define LIBLINUX_PAL_DCACHE_CLEAN_RANGE    3U
#define LIBLINUX_PAL_DCACHE_INVAL_RANGE    4U
#define LIBLINUX_PAL_CACHE_FLUSH_ALL		5U
#define LIBLINUX_PAL_DCACHE_FLUSH_ALL		6U
#define LIBLINUX_PAL_ICACHE_FLUSH_ALL		7U
#define LIBLINUX_PAL_DCACHE_CLEAN_ALL		8U

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
struct pfn_range {
	unsigned long long start;
	unsigned long long end;
};
int liblinux_pal_remap_pa_range_batch(unsigned long addr, const struct pfn_range *pfn_range_array,
				      unsigned int cnt, unsigned long prot, unsigned int flags);
int liblinux_pal_remap_pa_range_batch_sp(unsigned long addr, const struct pfn_range *pfn_range_array,
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
#define LIBLINUX_PAL_REMAP_PBHA			(0xFU << LIBLINUX_PAL_REMAP_PBHA_SHIFT) // bit[8:5]
#define LIBLINUX_PAL_REMAP_SH_SHIFT		9
#define LIBLINUX_PAL_REMAP_SH			(0x3U << LIBLINUX_PAL_REMAP_SH_SHIFT) // bit[10:9]

#define LIBLINUX_PAL_VM_TO_USER			(1U << 31)

extern int liblinux_pal_iofast_page_info_init(unsigned long pg_struct_va_start, size_t pg_struct_size,
					      unsigned int pg_slab_shift);
extern void *liblinux_pal_vm_prepare(unsigned long vaddr, unsigned long size,
				     unsigned long prot, unsigned int flags);
extern int liblinux_pal_vm_mmap(unsigned long paddr, unsigned long vaddr,
				unsigned long size, unsigned long prot,
				unsigned int flags);
extern int liblinux_pal_vm_unpopulate(void *addr, unsigned long size);
extern int liblinux_pal_vm_mmap_batch(void *pa_array,
				      unsigned int pa_array_size,
				      unsigned long vaddr,
				      unsigned long prot,
				      unsigned int flags);
extern int liblinux_pal_vm_unmap(const void *addr);

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
/* `pid` is root pid_ns val */
extern int liblinux_pal_usermap_munmap_sp(unsigned long start, size_t len, int pid);
extern void *liblinux_pal_find_vma(unsigned long addr);
extern void *liblinux_pal_find_vma_by_mm(unsigned long addr, unsigned long long vspace_key);

extern int liblinux_pal_vm_zap(unsigned long addr, unsigned long len, unsigned int flags);
/* `pid` is root pid_ns val */
extern int liblinux_pal_vm_zap_sp(unsigned long addr, unsigned long len, int pid);
extern int liblinux_pal_vm_zap_ptes(unsigned long addr, unsigned long len, unsigned long long mm);
extern int liblinux_pal_copy_from_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_to_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_in_user(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_from_caller(void *dst, const void *src, unsigned long n);
extern int liblinux_pal_copy_to_caller(void *dst, const void *src, unsigned long n);

extern int liblinux_pal_set_pageattr(const void *addr, int numpages,
				     int set_prot, int clear_prot);
extern int liblinux_pal_set_pageattr_ex(const void *addr, int numpages,
					int set_prot, int clear_prot);

extern void liblinux_pal_change_secpage_range(unsigned long long phys_addr, unsigned long vaddr,
					      unsigned long size, unsigned int flags);

/* kernel module support */
int liblinux_pal_module_alloc_init(const void *base, unsigned long size);
int liblinux_pal_module_alloc_prepare(const void *addr, unsigned long size);
int liblinux_pal_module_alloc_release(const void *addr, unsigned long *psize_out);
int liblinux_pal_module_alloc_prepare_hkip(const void *addr, unsigned long size);

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

struct liblinux_wchan_info {
	char *name;
	unsigned int name_len;
	unsigned long offset;
	unsigned long size;
};

/* thread related */
typedef struct {
	unsigned long long handle;
	int tid;
	unsigned long long tcb_cref;
} liblinux_pal_thread_handle_t;

unsigned long long liblinux_pal_thread_actv_cref(void);
#ifdef CONFIG_LIBLINUX_THREAD_BLOCK
unsigned long long liblinux_pal_thread_sched_cref(void);
int liblinux_pal_thread_block(volatile int *futex, int val,
				unsigned long long timeout, /* ns */
				unsigned long flags,
				const struct liblinux_wchan_info *wchan_info);
int liblinux_pal_thread_unblock(unsigned long long tcb_cref, volatile int *futex, int sync);
#endif

int liblinux_pal_thread_create(void *(*fun)(void *), void *arg,
			       liblinux_pal_thread_handle_t *handle);
int liblinux_pal_thread_create_ex(void *(*fun)(void *), void *arg,
				  liblinux_pal_thread_handle_t *handle);
struct task_struct;
int liblinux_pal_thread_create_task_init(void *(*func)(void *), void *arg, liblinux_pal_thread_handle_t *handle,
					 struct task_struct *task,
					 void (*task_init)(struct task_struct *task,
							   liblinux_pal_thread_handle_t *handle));

void liblinux_pal_thread_exit(void) ;
void liblinux_pal_thread_exit_ex(void *exitcode);
int liblinux_pal_thread_join_ex(const liblinux_pal_thread_handle_t *handle,
				void **exitcode);
unsigned long liblinux_pal_thread_stack_info(const liblinux_pal_thread_handle_t *handle, unsigned long *base);
int liblinux_pal_thread_set_my_data(const void *data);
void *liblinux_pal_thread_get_my_data(void);
void liblinux_pal_thread_init(void);
int liblinux_pal_thread_init_ex(void (*dtor)(void *));
void liblinux_pal_thread_specific_init(unsigned int *offset, unsigned int *key);
void liblinux_pal_thread_yield(void);
int liblinux_pal_thread_setname(const liblinux_pal_thread_handle_t *handle,
				const char *thread_name);
extern int liblinux_pal_thread_getname(char* thread_name);
int liblinux_pal_get_current_exec_path(char *buf, size_t len);
int liblinux_pal_get_thread_name(pid_t tid, char *comm, int len);
int liblinux_pal_sched_getaffinity(int tid, unsigned long *mask);
int liblinux_pal_sched_setaffinity(int tid, unsigned long mask);
int liblinux_pal_thread_setaffinity(const liblinux_pal_thread_handle_t *handle,
				    unsigned long mask);
int liblinux_pal_thread_getaffinity(const liblinux_pal_thread_handle_t *handle,
				    unsigned long *mask);
int liblinux_pal_thread_setscheduler(const liblinux_pal_thread_handle_t *handle,
				     int prio, int policy);
int liblinux_pal_thread_set_vip_prio(const liblinux_pal_thread_handle_t *handle,
				     int prio);
int liblinux_pal_is_thread_ctx(void);
void liblinux_pal_thread_dump(pid_t tid);
int liblinux_pal_set_user_nice(int tid, int nice);
int liblinux_pal_getpriority_by_tid(int tid);
int liblinux_pal_query_main_thread(int tid, int *main_thread_tid, char *main_thread_name, unsigned int name_len);
void liblinux_pal_process_signal_with_pid(int pid, int sig);
#ifdef CONFIG_LIBLINUX_PERI_DMA
#define PERI_DMA_WRITE_INFO_MAX 8
struct peri_dma_write_info {
	unsigned int val[PERI_DMA_WRITE_INFO_MAX];
	unsigned int offset[PERI_DMA_WRITE_INFO_MAX];
	unsigned int count;
};
void liblinux_pal_peri_dma_write(struct peri_dma_write_info* info);
#endif
void liblinux_pal_thread_set_power_group(const liblinux_pal_thread_handle_t *handle);
int liblinux_pal_proc_lock_pid(int pid);
int liblinux_pal_proc_unlock_pid(int pid);
void liblinux_pal_thread_set_task_boost(const liblinux_pal_thread_handle_t *handle, int boost);
int liblinux_pal_thread_set_freezable(void);
int liblinux_pal_thread_try_to_freeze(void);
bool liblinux_pal_thread_freezing(void);
void liblinux_pal_thread_freezer_count(bool count, bool fwd);
void liblinux_pal_thread_set_pin_preempted(void);

void liblinux_pal_set_iowait(void);
void liblinux_pal_clr_iowait(void);
pid_t liblinux_pal_thread_vtid_to_root_tid(pid_t tid);

/* cpuhp related */
int liblinux_cpu_up(unsigned int cpu);
int liblinux_cpu_down(unsigned int cpu);

unsigned int liblinux_pal_processor_id(void);
int liblinux_pal_cpu_config(unsigned long *online_mask,
			    unsigned long *cpu_map,
			    unsigned long cpu_nr,
			    int preempt_flag);
extern int liblinux_pal_cpu_config_ex(unsigned long *online_mask, unsigned long size,
				      int preempt_flag);
int liblinux_pal_cpu_topology(unsigned int *topo, unsigned int size);

/* sched-level for cpu hotplug */
#define THREAD_SCHED_LEVEL_MAX 2
#define PROCESS_SCHED_LEVEL_MAX 2
void liblinux_pal_process_set_slv(unsigned int cpu, unsigned int slv);
void liblinux_pal_thread_set_slv(const liblinux_pal_thread_handle_t *handle, unsigned int slv);

/* lock related */
#define LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE	1
#define LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT	2
#define LIBLINUX_PAL_FUTEX_LOCK_IN_LDK		4
#define LIBLINUX_PAL_FUTEX_IOWAIT		8
#define LIBLINUX_PAL_FUTEX_MAX_TIMEOUT		~0ULL

int liblinux_pal_futex_timedwait(volatile int *futex, int val,
				 unsigned long long timeout, /* ns */
				 unsigned long flags);
int liblinux_pal_futex_timedwait_ex(volatile int *futex, int val,
				    unsigned long long timeout, /* ns */
				    unsigned long flags,
				    const struct liblinux_wchan_info *wchan_info);
int liblinux_pal_futex_wake(volatile int *futex);
int liblinux_pal_futex_wake_ex(volatile int *futex, int sync);

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
extern int liblinux_pal_kptr_restrict(void);

/*
 * used to adapt fget for anon fd, should be removed when all files use
 * hm fd in LDK
 */
enum liblinux_fd_type {
	DEFAULT_FD = 0,
	ANON_FD,
	TRANSFS_FD,
	NET_FD,
	MAX_FD, /* always be the last one */
};
void *liblinux_pal_fget_ex(enum liblinux_fd_type fd_type, int fd, void (*fget)(void*));
void liblinux_pal_fput_ex(enum liblinux_fd_type fd_type, void *file);
void *liblinux_pal_gdentry(const char *pathname);
void liblinux_pal_pdentry(void *sb, void *dentry);

/* preempt related */
void *liblinux_pal_preempt_init(void);
void liblinux_pal_preempt_resched(void);
void liblinux_pal_preempt_enable(void);
void liblinux_pal_preempt_disable(void);

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
int liblinux_pal_signal_isignored(int pid, int sig);
int liblinux_pal_current_pgrp_isorphaned(void);
void liblinux_pal_sys_shutdown(void);
void liblinux_pal_sys_snapshot(void);

/* mem stat dfx api */
typedef void (*liblinux_mem_hook_t)(int ev, void *addr, unsigned long size,
				    const char *slab_cache_name);

int liblinux_dfx_vmalloc_hook(liblinux_mem_hook_t hook);
int liblinux_pal_mincore(const void *addr, unsigned long size, unsigned char *vec);

/* capabilities */
#define LIBLINUX_CAP_OPT_NONE 0
#define LIBLINUX_CAP_OPT_NOAUDIT 1
#define LIBLINUX_CAP_OPT_INSETID 2
int liblinux_pal_capable(int cap, unsigned int opts);

int liblinux_pal_get_unused_fd_flags(unsigned flags);
void liblinux_pal_fd_install(unsigned int fd, void *file);
struct dh_fd_install_context {
	const char *file_name;
	unsigned int buffer_len;
	unsigned int f_flags;
};
void liblinux_pal_fd_install_ex(unsigned int fd, void *file, struct dh_fd_install_context *ctx);
int liblinux_pal_alloc_unused_fd(unsigned int flags, void *file, struct dh_fd_install_context *ctx);
void liblinux_pal_put_unused_fd(unsigned int fd);

struct stat;
struct statfs;
struct iovec;

int liblinux_pal_vfs_fstat(int fd, struct stat *stat);
long liblinux_pal_vfs_read(int fd, void *buf, unsigned int nbyte);
int liblinux_pal_ksys_access(const char *pathname, int mode);
int liblinux_pal_vfs_open(const char *filename, unsigned int flag, unsigned int mode);
int liblinux_pal_ksys_open(const char *filename, unsigned int flag, unsigned int mode);
int liblinux_pal_ksys_openat(int dfd, const char *filename, unsigned int flag, unsigned int mode);
int liblinux_pal_ksys_lseek(int fd, long long offset, int whence, long long *pos);
long liblinux_pal_ksys_write(int fd, const void *buf, unsigned int nbyte);
long liblinux_pal_ksys_pwrite(int fd, long long pos, const void *buf, unsigned int nbyte);
long liblinux_pal_ksys_pwritev(int fd, const struct iovec *iov, int iovcnt, long offset);
long liblinux_pal_ksys_read(int fd, void *buf, unsigned int nbyte);
long liblinux_pal_ksys_pread(int fd, long long pos, void *buf, unsigned int nbyte);
long liblinux_pal_ksys_preadv(int fd, const struct iovec *iov, int iovcnt, long offset);
int liblinux_pal_ksys_fsync(int fd);
int liblinux_pal_ksys_sync(void);
int liblinux_pal_vfs_close(int fd);
int liblinux_pal_ksys_close(int fd);
int liblinux_pal_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int liblinux_pal_ksys_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int liblinux_pal_ksys_fstat(int fd, struct stat *stat);
int liblinux_pal_ksys_fstat_nocheck(int fd, struct stat *stat);
int liblinux_pal_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags);
int liblinux_pal_ksys_rename(const char *oldpath, const char *newpath);
int liblinux_pal_ksys_readlink(const char *path, char *buf, unsigned int bufsize);
int liblinux_pal_ksys_readlinkat(int dfd, const char *path, char *buf, unsigned int bufsize);
int liblinux_pal_ksys_readdir(unsigned int fd, char *buf, unsigned int bufsize);
int liblinux_pal_ksys_mkdir(const char *pathname, unsigned int mode);
int liblinux_pal_ksys_rmdir(const char *pathname);
int liblinux_pal_ksys_fchown(int fd, unsigned int uid, unsigned int gid);
int liblinux_pal_ksys_unlink(const char *filename);
int liblinux_pal_ksys_statfs(const char *pathname, struct statfs *statfs);
int liblinux_vfs_open(const char *filename, unsigned int flag, unsigned int mode);
int liblinux_vfs_fstat(int fd, struct stat *stat);
long liblinux_vfs_read(int fd, void *buf, unsigned int nbyte);
int liblinux_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz);
int liblinux_vfs_close(int fd);
int liblinux_ksys_fstat_nocheck(int fd, struct stat *stat);
int liblinux_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags);


extern int  liblinux_pal_register_blkdev_region(unsigned int major,
					        const char *name, unsigned int *major_out);
extern void liblinux_pal_unregister_blkdev_region(unsigned int major, const char *name);

/* only for skb data */
void *liblinux_pal_alloc_dma_pool_ex(unsigned int *offset, unsigned long long *paddr, unsigned int size);
int liblinux_pal_free_dma_pool(unsigned int offset);
int liblinux_pal_native_net_rx(unsigned int pool_offset, int shm_len,
			       int rx_len, int offset, int ifindex);
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
/* request firmware related */
int liblinux_pal_fw_acquire_contents(const char *filename, void *data,
				     unsigned int size, unsigned int *rsize /* out */);

/* liblinux shrinker */
struct liblinux_pal_sub_shrinker {
	/* return `nr_pages` that can be reclaimed */
	unsigned long (*query)(void* priv);
	/* return `nr_pages` that actual reclaimed */
	unsigned long (*shrink)(void* priv, int free_nr);
	void *priv;
};

void liblinux_pal_sub_shrinker_register(const struct liblinux_page_allocator *_allocator,
					struct liblinux_pal_sub_shrinker shrinker);
void liblinux_pal_sub_shrinker_unregister(const struct liblinux_page_allocator *_allocator,
					  struct liblinux_pal_sub_shrinker shrinker);

unsigned long liblinux_pal_query_pgd(unsigned int pid);

/* liblinux kprobe */
struct kprobe;
int liblinux_set_current_kprobe(struct kprobe *p);
void liblinux_reset_current_kprobe(void);
struct kprobe *liblinux_kprobe_running(void);
struct kprobe_ctlblk *liblinux_get_kprobe_ctlblk(void);

/* file_guard */
int liblinux_pal_sec_fileguard_cfg(void *ctx, size_t len, size_t *ret);

/* tzdriver */
int liblinux_pal_get_processname_by_tid(__u32 calling_pid, char *path, uint32_t path_len);

/* cma */
int liblinux_pal_cma_area_init(unsigned long paddr, unsigned long vaddr, unsigned long size,
			       unsigned int cma_id, const char *name);
int liblinux_pal_cma_alloc(unsigned int cma_id, unsigned long size, unsigned int align_order,
			   unsigned long *paddr);
int liblinux_pal_cma_release(unsigned int cma_id, unsigned long vaddr,
			     unsigned long paddr, unsigned long size);

/* memory swap */
int liblinux_pal_mem_swap_space_init(int swap_id, unsigned long total_size, unsigned int ext_size);
int liblinux_pal_mem_swap_alloc_extentid(int swap_id, int *ext_id);
int liblinux_pal_mem_swap_in_range(int swap_id, unsigned long *pa_array, int *ext_ids, int num);
int liblinux_pal_mem_swap_out_range(int swap_id, unsigned long *pa_array, int *ext_ids, int num);
void liblinux_pal_mem_swap_free_extentid(int swap_id, int ext_id);

/* sd */
int liblinux_pal_sd_ida_alloc(void);
void liblinux_pal_sd_ida_free(int sd_index);

void liblinux_pal_fpu_save(void);
void liblinux_pal_fpu_restore(void);

extern void liblinux_pal_sync_wakeup_trad(void *filp_node, unsigned int revents);
extern void liblinux_pal_sync_wakeup(void *kobj_ctx, unsigned int revents);
extern void *liblinux_pal_sync_open(void *kobj_ctx);
extern void liblinux_pal_sync_close(void *kobj_ctx);

int liblinux_pal_get_user_pages(unsigned long vaddr, size_t len, unsigned long long *ret_pa_array,
				size_t array_size);
int liblinux_pal_put_user_pages(unsigned long vaddr, size_t len, unsigned long long *ret_pa_array,
				size_t array_size);
int liblinux_pal_charge_mlock(unsigned long mlock_size);
int liblinux_pal_uncharge_mlock(unsigned long mlock_size);
void liblinux_pal_mem_enable_rsv_mem(unsigned long pfn_start, unsigned long pfn_end);

int liblinux_read_klog_config(unsigned long long *paddr, unsigned int *size);
int liblinux_read_kbox_config(unsigned long long *paddr, unsigned long *size);
int liblinux_erase_kbox_region(const char *name);
int liblinux_iter_last_klog(unsigned long max_read_len,
		int (*dump_func)(const char *log, unsigned int len, uintptr_t arg), uintptr_t arg);
int liblinux_iter_current_klog(unsigned long max_read_len,
		int (*dump_func)(const char *log, unsigned int len, uintptr_t arg), uintptr_t arg);
int liblinux_iter_on_klog_data(const char *buffer, size_t buflen,
		int (*dump_func)(const char *log, unsigned int len, uintptr_t arg), uintptr_t arg);
int liblinux_read_klog_data(char *buffer, size_t buflen, ssize_t offset, size_t *ret_read_len);
int liblinux_decompress_kbox_snapshot(const char *out_path);

int liblinux_pal_sysevent_write(const char *buf, size_t buf_len);
int liblinux_vmemmap_pg_populate(void *vaddr, unsigned long size);
int liblinux_vmemmap_pg_unpopulate(void *vaddr, unsigned long size);
int liblinux_reboot(const char *cmd, bool is_abnormal);
void liblinux_stop_feed_harddog(unsigned int timeout_sec);
void liblinux_stop_feed_highdog(unsigned int timeout_sec);
#ifdef CONFIG_LIBLINUX_CDC
void liblinux_shutdown(void);
#endif
void liblinux_save_hm_log(char *pbuf, unsigned int buf_size);
bool liblinux_get_hm_log_buffer(char *pbuf, unsigned int buf_size, size_t *retlen);
bool liblinux_is_lastklog_valid(const char *klog_buff, size_t len, unsigned int *ret_header_len);
#ifdef CONFIG_LOG_ENCRYPT
int liblinux_get_elog_metadata(int (*callback)(const char* buf, size_t buf_size));
#endif

/* sec */
int liblinux_vfs_lock_in(unsigned int user_id);
int liblinux_vfs_unlock_in(unsigned int user_id, unsigned int file,
	unsigned char *iv, unsigned int iv_len);

char *liblinux_pal_realpath(const char *path, char *buf, unsigned long len);
void liblinux_dump_hm_track_info(const char *dst_dir_str, const char *bin_file_name, bool dump_active);
int liblinux_hm_track_info_set(bool pause);

/* ldk_stat */
void liblinux_set_buddy_dma_free_pages(void);
void liblinux_mod_dmaheap_free_pages(long val);
long liblinux_get_system_free_pages(void);
unsigned long liblinux_get_sys_avail_buf_wmark_min(void);
unsigned long liblinux_get_sys_avail_buf_wmark_mid(void);
unsigned long liblinux_get_sys_avail_buf_wmark_high(void);
unsigned long liblinux_get_sys_curr_avail_buf(void);

/* liblinux_pal_proxy */
#define PAL_PROXY_MAJOR_CMD_OF(x)		(((x) >> 16) & 0xFFFF)
#define PAL_PROXY_MINOR_CMD_OF(x)		(x & 0xFFFF)
#define PAL_PROXY_MAJOR(x)			((((x) & 0xFFFF) << 16))
#define PAL_PROXY_COMBINE_CMD(x, y)		((PAL_PROXY_MAJOR(x)) | ((y) & 0xFFFF))

/* for udk call ldk */
int liblinux_pal_proxy_register(unsigned int cmd, int (*func)(unsigned int cmd, void *rd,
						     size_t rd_size, void *wr,
						     size_t wr_size));
int liblinux_pal_proxy_handler(unsigned int cmd, void *rd, size_t rd_size,
			       void *wr, size_t wr_size);

/* for ldk call udk */
int liblinux_pal_rpc_invoke(uint64_t rpc_key, int cmd, void *rd, size_t rd_size,
			    void *wr, size_t wr_size, bool is_xact);
int liblinux_pal_proxy_udk_invoke(int cmd, void *rd, size_t rd_size,
				 void *wr, size_t wr_size);
int liblinux_proxy_udk_invoke_xact(int cmd, void *rd, size_t rd_size,
				   void *wr, size_t wr_size);

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

/* hibernation */
int liblinux_set_pm_crypt_info(uint32_t key_index, bool clear);

struct liblinux_pal_cpa_prepare_memory_args {
	unsigned int flags;
	unsigned long water_mark_page;
	unsigned long heap_size;
	unsigned long allocated_size;

	unsigned long alloc_size;
	unsigned int order;
};
void liblinux_cpa_prepare_memory_for_work(struct liblinux_pal_cpa_prepare_memory_args *args);
void liblinux_cpa_prepare_memory_for_alloc(struct liblinux_pal_cpa_prepare_memory_args *args);
void liblinux_cpa_prepare_memory_for_remaining(struct liblinux_pal_cpa_prepare_memory_args *args);
void liblinux_pal_cpa_prepare_memory(struct liblinux_pal_cpa_prepare_memory_args *args);

#ifdef CONFIG_NO_HZ_IDLE
void liblinux_inform_suspend(void);
void liblinux_inform_resume(void);
#else
static inline void liblinux_inform_suspend(void)
{
	return;
}

static inline void liblinux_inform_resume(void)
{
	return;
}
#endif
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
#ifdef CONFIG_LIBLINUX_HVC_CALL
void liblinux_pal_hvc_call(unsigned long *in_regs, unsigned long *out_regs);
#endif /* CONFIG_LIBLINUX_HVC_CALL */

enum liblinux_iofast_page_type {
	LIBLINUX_IOFAST_PAGE_BUDDY, /* ldk buddy pages */
	LIBLINUX_IOFAST_PAGE_DMA_BUF, /* dma_buf pages */
	LIBLINUX_IOFAST_PAGE_INIT, /* mem init used pages */
	LIBLINUX_IOFAST_PAGE_PGTBL, /* smmu level 2-3 page table */
	LIBLINUX_IOFAST_PAGE_PGD, /* smmu base level page table */
	LIBLINUX_IOFAST_PAGE_FW_CACHE, /* firmare cache pages */
	LIBLINUX_IOFAST_PAGE_TYPE_MAX,
};

/* iofast */
void *liblinux_iofast_alloc_pages(unsigned gfp_mask, unsigned int order,
				  unsigned long length, void **page_array,
				  unsigned int *page_num, enum liblinux_iofast_page_type page_type);
void *liblinux_iofast_alloc_page(unsigned gfp_mask, unsigned int order, enum liblinux_iofast_page_type page_type);
void liblinux_iofast_free_pages(void *page, unsigned int order, enum liblinux_iofast_page_type page_type);
void liblinux_iofast_stat_inc(enum liblinux_iofast_page_type page_type, unsigned long nr_pages);
void liblinux_iofast_stat_dec(enum liblinux_iofast_page_type page_type, unsigned long nr_pages);
long liblinux_iofast_stat_get(enum liblinux_iofast_page_type page_type);
void liblinux_iofast_stat_move(enum liblinux_iofast_page_type dst, enum liblinux_iofast_page_type src, unsigned long nr_pages);

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

#ifdef CONFIG_LDK_THERMAL_INTELLIGENT
int liblinux_pal_cooling_freq_limit(unsigned int freq_limit);
#endif

struct device;
typedef struct file *(*devfs_open_handler_t)(struct device *dev, unsigned int flags);
#ifdef CONFIG_UNIX98_PTYS
#define liblinux_register_devfs_open_handler liblinux_register_devfs_open_handler
void liblinux_register_devfs_open_handler(devfs_open_handler_t fn);
#endif

#define LIBLINUX_SYSFS_MAX 0x00FFFFFFU /* Maximum number of files supported by ldk */

int liblinux_read_lastklog_memory_config(uint64_t *addr, uint32_t *size);

#endif	/* !__ASSEMBLY__ */
#endif	/* CONFIG_LIBLINUX */
#endif	/* __LIBLINUX_PAL_H__ */
