/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Implementation of ulibs hook
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 08 19:14:29 2018
 */
#include <lib/perfra/ksus.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <libsysif/sysmgr/api.h>
#include <libvsyscall/hmvsyscall.h>
#include <libhwsecurec/securec.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_crypt/crypt_server.h>
#include <libcrt/hmcrt.h>
#include <libhmelf/elf.h>
#include <libhmsync/raw_thread.h>
#include <libstrict/overflow.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libself_rel/relocate.h>
#include <libhmsrv_sys/hm_auxv.h>
#include <libhmactv/actv.h>
#include <libhmsrv_key/keymgr.h>
#include <libpreempt/preempt.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmtrace_ng/hmtrace.h>
#include <libkcov/kcov.h>

#include <elf.h>
#include <vfs.h>
#include <hmdsosym.h>
#include <malloc_hook.h>
#include <memtrace.h>
#ifdef CONFIG_VNOTIFY_DRIVER_EXTENSION
#include <libvpipe/vpipe.h>
#endif

void hmcrt_prehook(char **envp);
void hmcrt_posthook(void);

#if __SIZEOF_LONG__ == 4
#define PHDR_OFFSET	0x34U
#else
#define PHDR_OFFSET	0x40U
#endif

__attribute__((weak)) struct dso_sym_info *dso_syminfo = NULL;

static void mprotect_relro_segment(uintptr_t load_start)
{
	int err;
	uintptr_t relro_addr_start;
	size_t relro_memsz;

	err = elf_phdr_info(load_start, PT_GNU_RELRO, &relro_addr_start,
			    &relro_memsz, NULL, NULL);
	if (err == E_HM_OK && relro_memsz != 0U) {
		/*
		 * Here is to determine whether the type of elf is DYN by whether there is
		 * a dynamic segment, if it is DYN type relro_addr_start needs to add load_start.
		 */
		err = elf_phdr_info(load_start, PT_DYNAMIC, NULL, NULL, NULL, NULL);
		if (err == E_HM_OK) {
			relro_addr_start += load_start;
		}
		err = actvxactcall_hmcall_mem_mprotect(false, true, (void *)ALIGN_DOWN(relro_addr_start, PAGE_SIZE),
						       ALIGN_UP(relro_memsz, PAGE_SIZE), MPROT_READ);
		if (err != E_HM_OK) {
			hm_panic("mprotect failed\n");
		}
	}
}

static void __setup_sysinfo_from_auxv(size_t *auxv)
{
	size_t i;
	size_t *current_aux = hm_crt_current_aux();

	for (i = 0U; auxv[i] != 0U; i += STEP_SIZE) {
#ifdef CONFIG_SPIF
		if (auxv[i] == (size_t)AT_SPIF_KSUS) {
			auxv[i + 1U] = ptr_to_ulong(get_ksus_desc());
		}
#endif
		if (auxv[i] == (size_t)AT_SYS_CNODE_IDX) {
			hmsrv_set_sysmgr_cnode_idx((__u32)auxv[i + 1U]);
		}

		if (auxv[i] == (size_t)AT_TRACE_SHM_ADDR) {
			hm_trace_shm_set_addr((uintptr_t)auxv[i + 1U]);
		}

		if (auxv[i] == (size_t)AT_TRACE_SNAPSHOT_SHM_ADDR) {
			hm_trace_shm_set_snapshot_addr((uintptr_t)auxv[i + 1U]);
		}

		if (auxv[i] == (size_t)AT_TRACE_SAVED_TASKINFO_SHM_ADDR) {
			hm_trace_shm_set_saved_taskinfo_addr((uintptr_t)auxv[i + 1U]);
		}

		if (auxv[i] == (size_t)AT_APP_CNODE_IDX) {
			hm_crt_set_cnode_idx((uint32_t)auxv[i + 1U]);
		}

		if (auxv[i] == (size_t)AT_MAIN_TCB_INNER_IDX) {
			hm_crt_set_tcb_inner_idx((uint32_t)auxv[i + 1U]);
		}

		if (auxv[i] < AUX_CNT) {
			current_aux[auxv[i]] = auxv[i + 1U];
		} else if (auxv[i] == (size_t)AT_DSOINFO) {
			dso_syminfo = (struct dso_sym_info *)auxv[i + 1U];
		} else if ((auxv[i] != (size_t)AT_SYS_CNODE_IDX)
			   && (auxv[i] != (size_t)AT_LIBCMAPLEN)
			   && (auxv[i] != (size_t)AT_SPIF_KSUS)
			   && (auxv[i] != (size_t)AT_TRACE_SHM_ADDR)
			   && (auxv[i] != (size_t)AT_APP_CNODE_IDX)
			   && (auxv[i] != (size_t)AT_MAIN_TCB_INNER_IDX)
			   && (auxv[i] != (size_t)AT_TRACE_SNAPSHOT_SHM_ADDR)
			   && (auxv[i] != (size_t)AT_TRACE_SAVED_TASKINFO_SHM_ADDR)) {
			hm_panic("Got unexpected auxv entry %zu\n", auxv[i]);
		} else {
			/* do nothing */
		}
	}
}
#ifdef PAC_FWCFI_SYSSERVICE
void libc_start_preinit(void);
#endif

void hmcrt_prehook(char **envp)
{
	size_t i;
	size_t *auxv = NULL;
	uintptr_t load_start = 0UL;
	uintptr_t phdr_addr = 0UL;

	/* compute base address of auxv */
	for (i = 0U; envp[i] != NULL; i++) {
		;
	} /* nop */
	auxv = (size_t *)(uintptr_t)(envp + i + 1U);

	for (i = 0u; auxv[i] != 0U; i += STEP_SIZE) {
		if (auxv[i] == (size_t)AT_BASE) {
			load_start = auxv[i + 1U];
			continue;
		}
		if (auxv[i] == (size_t)AT_PHDR) {
			phdr_addr = auxv[i + 1U];
		} else {
			/* do nothing */
		}
	}
	/*
	 * This is for init/vfs, whom are not started by hmld.so, we need to
	 * do self relocate if they are compiled with pie, note that global
	 * variables are not allowed to be used before self relocation.
	 *
	 * The process started by hmld.so, AT_BASE stores the address of
	 * hmld.so or libc.so.  Here we judge how uapp is lunched by checking
	 * whether AT_BASE stores the address of uapp.
	 *
	 * Should also check whether it's fs and has self-relocated.
	 */
	if (phdr_addr > load_start &&
	    phdr_addr - load_start == PHDR_OFFSET &&
	    get_fs_relocated() == 0) {
		selfreloc_relocate(load_start);
#ifdef PAC_FWCFI_SYSSERVICE
		libc_start_preinit();
#endif
		mprotect_relro_segment(load_start);
	}

	__setup_sysinfo_from_auxv(auxv);
}

void hmcrt_posthook(void)
{
	int rc;
	/* setup raw thread tls */
	rc = raw_tls_init();
	BUG_ON(rc != 0);

#ifdef CONFIG_PAC
	/* should invoked before any thread create. */
	raw_thread_entry_modifier_init();
#endif

	actv_threadpool_init();
#ifdef CONFIG_KCOV
	kcov_init_finished();
#endif
}
