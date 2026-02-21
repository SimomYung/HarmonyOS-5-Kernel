// SPDX-License-Identifier: GPL-2.0
#include "libdh_linux.h"

#include <hmkernel/cpuset.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <libpreempt/preempt.h>
#include <libhwsecurec/securec.h>
#include <libhmpm/shutdown.h>
#include <libhmsrv_sys/hm_mem.h>

#include <devhost/interrupt.h>
#include <devhost/log.h>
#include <devhost/security.h>
#include <devhost/memory.h>
#include <libmem/utils.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/helper.h>
#include <sys/mman.h>

#include <vfs.h>

#include "ldk_stat.h"

void current_thread_name_fill(char *comm, unsigned int size)
{
	if (likely(HMCAP_THREAD_NAME_SIZE <= size)) {
		memcpy(comm, hm_actv_local_thread_name_of(), HMCAP_THREAD_NAME_SIZE);
	} else {
		memcpy(comm, hm_actv_local_thread_name_of(), size);
	}
	comm[size - 1] = '\0';
}

void liblinux_fill_thread_info(int *pid, int *tgid, int *vtgid, unsigned long long *vspace_key,
			       char *comm, unsigned int size)
{
	struct rb_cnode_node *ret = NULL;

	*pid = hm_actv_local_thread_tid_of();
	*tgid = hm_actv_local_thread_pid_of();
	*vtgid = hm_actv_local_thread_vpid_of();
	*vspace_key = hm_actv_local_thread_vskey_of();
	if (likely(HMCAP_THREAD_NAME_SIZE <= size)) {
		memcpy(comm, hm_actv_local_thread_name_of(), HMCAP_THREAD_NAME_SIZE);
	} else {
		memcpy(comm, hm_actv_local_thread_name_of(), size);
	}
	comm[size - 1] = '\0';
}

static unsigned int liblinux_boot_cpu_id;
static unsigned long saved_boot_affinity;
static void save_boot_affinity(unsigned int bind_cpu)
{
	__cpuset_t cpuset = __cpuset_one(bind_cpu);
	int ret;

	ret = liblinux_pal_thread_getaffinity(NULL, &saved_boot_affinity);
	BUG_ON(ret < 0);

	ret = liblinux_pal_thread_setaffinity(NULL, (unsigned long)cpuset.__bits);
	BUG_ON(ret < 0);
}

static void restore_boot_affinity(void)
{
	int ret;

	ret = liblinux_pal_thread_setaffinity(NULL, saved_boot_affinity);
	BUG_ON(ret < 0);
}

/* liblinux post-init hook */
static int liblinux_postinit(void *arg)
{
	UNUSED(arg);

	/* do initcalls */
	save_boot_affinity(liblinux_boot_cpu_id);
	liblinux_do_initcalls();
	restore_boot_affinity();

	return E_HM_OK;
}

static struct lnxbase_ctx *g_ctx = NULL;

int dh_post_dev_populate_call(void)
{
	return lnxbase_root_notify(g_ctx);
}

void liblinux_load_extra_modules(void)
{
	int ret = lnxbase_load_extra_modules(g_ctx);
	if (ret) {
		dh_panic("lnxbase_load_extra_modules failed %d\n", ret);
	}
}

static void liblinux_ldk_stat_register(void)
{
	void *pstat = liblinux_get_ldk_stat_common();
	size_t size = liblinux_get_ldk_stat_size();

	if (liblinux_is_ldk_stat_registered() == false) {
		int rc = hm_mem_devhost_stat_register(pstat, size);
		if (rc == E_HM_OK)
			liblinux_set_ldk_stat_registered();
	}
}

#ifdef __clang__
#define __no_stack_protector	__attribute__((no_stack_protector))
#else
#define __no_stack_protector	__attribute__((optimize("-fno-stack-protector")))
#endif

static int __dh_pid = 0;
int liblinux_process_pid(void)
{
	return __dh_pid;
}

#ifdef PAC_FWCFI_SYSSERVICE
#define PAC_FECFI_SIGN(_addr, _type) \
	asm volatile ("pacia %0, %1 \n": "=r" (_addr): "r"(__builtin_get_modifier_bytype(_type)), "0"(_addr))
#define PAC_FECFI_XPAC(_addr) \
({ \
	void *__addr = (_addr); \
\
	asm volatile ("xpaci %0\n" : "+r" (__addr)); \
	(typeof (_addr))__addr; \
})
#define PAC_FECFI_XPAC_SIGN(_addr, _type) do { \
        _addr = PAC_FECFI_XPAC(_addr); \
        if ((_addr) != NULL) { \
                PAC_FECFI_SIGN((_addr), _type); \
        } \
} while (0)
#endif

int __no_stack_protector __attribute__((__section__(".ref.text")))
dh_backend_init(struct dh_backend_ctor *ctor, const char *backend_cmdline)
{
	int ret;

	__dh_pid = hm_getpid();
	hm_set_fork_not_allowed();
	dh_info("initializing linux subsystem ...\n");

	__fdt_pointer = lnxbase_acquire_bootfdt(NULL);
	if (__fdt_pointer == NULL) {
		return E_HM_INVAL;
	}

	/*
	 * Ensure that ctx_setup assigned before the earliest interrupt
	 * is registered which may support irq-actv.
	 */
	ctor->ctx_setup = liblinux_thread_setup;

	/* Ensure that no sched migration occurs during kernel init & initcalls */
	liblinux_boot_cpu_id = liblinux_pal_processor_id();
	dh_info("booting from cpu %d\n", liblinux_boot_cpu_id);
	save_boot_affinity(liblinux_boot_cpu_id);
	liblinux_kernel_init(backend_cmdline);
	restore_boot_affinity();

	ret = lnxbase_helper_simple_init(ctor, liblinux_postinit,
					 (void *)devhost_get_backend_args(),
					 &g_ctx);
	if (ret < 0) {
		return ret;
	}

	/*
	 * override default syscall_enter_nosec in lnxbase helper,
	 * delay the initialization of current->cred to the first use
	 */
	ctor->syscall_enter_nosec = liblinux_enter_nodac;
	ctor->syscall_enter_nomm = liblinux_enter_nomm;

	/*
	 * override default alloc_page in lnxbase helper,
	 * use `GFP_KERNEL | __GFP_NOWARN |  __GFP_ZERO` as gfp flags
	 */
	ctor->handle_page_fault = dh_ctor_handle_fault;
#ifdef PAC_FWCFI_SYSSERVICE
	PAC_FECFI_XPAC_SIGN(ctor->handle_page_fault,
				int(*)(void *, unsigned long long, unsigned long long, unsigned int, void *));
#endif
	ctor->handle_mm_rdlock = dh_ctor_mm_rdlock;
	ctor->handle_mm_rdunlock = dh_ctor_mm_rdunlock;
	ctor->alloc_page = dh_backend_alloc_page;
	ctor->handle_mm_release = __liblinux_mm_release;

	ctor->vhangup = liblinux_vhangup;

	lnxbase_enable_device_autoprobe(g_ctx);

	lnxbase_enable_bootfdt(g_ctx);

	lnxbase_enable_skip_first_timesync(g_ctx);

	ret = lnxbase_enable_post_dev_populate(g_ctx, dh_extralib_init);
	if (ret < 0) {
		lnxbase_destroy(g_ctx);
		g_ctx = NULL;
		return ret;
	}

#ifdef LNXBASE_HELPER_ENABLE_STOP_TICK
	lnxbase_enable_stop_tick(liblinux_try_stop_tick);
#endif

	/* register dh ldk_stat to sysmgr */
	liblinux_ldk_stat_register();
	return 0;
}

void emergency_restart(void)
{
	int ret;

	ret = hm_reboot(lnxbase_backend_args_find("reboot_reason"));
	if (ret < 0) {
		dh_panic("hm_reboot failed err: %s\n", hmstrerror(ret));
	}
}

int liblinux_kasan_populate_shadow(const void *addr, size_t len)
{
	return mlock(PAGE_ALIGN_DOWN(addr), PAGE_ALIGN_UP(len));
}

int liblinux_populate_bss(const void *start, const void *stop)
{
	unsigned long len = ptr_to_ulong(stop) - ptr_to_ulong(start);
	return mlock(PAGE_ALIGN_DOWN(start), PAGE_ALIGN_UP(len));
}

void __liblinux_assign_vma_info(void *dh_vma, void *ldk_vma)
{
	struct devhost_vma_info *vma_info = (struct devhost_vma_info *)dh_vma;
	if (vma_info == NULL) {
		return;
	}
	vma_info->priv = ldk_vma;
}

#ifdef CONFIG_LTO
static unsigned int liblinux_thread_offset;
static unsigned int liblinux_thread_key;
__attribute__((const)) unsigned int get_liblinux_thread_offset(void){
    return liblinux_thread_offset;
}
__attribute__((const)) unsigned int get_liblinux_thread_key(void){
    return liblinux_thread_key;
}
#else
const unsigned int liblinux_thread_offset __attribute__((__section__(".data..ro_after_init")));
const unsigned int liblinux_thread_key __attribute__((__section__(".data..ro_after_init")));
#endif

void liblinux_raw_thread_init(void)
{
	liblinux_pal_thread_specific_init(&liblinux_thread_offset, &liblinux_thread_key);
}

int liblinux_register_ipi_handler(int irq, int (*handler)(int, void *), unsigned int cpu)
{
	return devhost_register_ipi_handler(irq, handler, cpu);
}

int liblinux_unregister_ipi(int irq, unsigned int cpu)
{
	return devhost_unregister_ipi(irq, cpu);
}

int liblinux_vmemmap_pg_populate(void *vaddr, unsigned long size)
{
	int ret = hm_mem_madvise((const void *)vaddr, size, MADV_FLAG_WILLNEED);
	if (ret < 0) {
		dh_error("vmemmap populate failed ret=%s\n", hmstrerror(ret));
		return -EINVAL;
	}

	return 0;
}

int liblinux_vmemmap_pg_unpopulate(void *vaddr, unsigned long size)
{
	int ret = hm_mem_madvise((const void *)vaddr, size, MADV_FLAG_DONTNEED);
	if (ret < 0) {
		dh_error("vmemmap unpopulate failed ret=%s\n", hmstrerror(ret));
		return -EINVAL;
	}

	return 0;
}

int liblinux_va_to_pa(const void *vaddr, unsigned long long *pa)
{
	int ret = sysfast_pgtable_va_to_pa(vaddr, pa);
	return ret;
}
