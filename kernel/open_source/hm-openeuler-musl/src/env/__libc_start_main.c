#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "syscall.h"
#include "atomic.h"
#include "libc.h"
#include "stdio_impl.h"
#include "pthread.h"
#include "pthread_impl.h"
#include "syslog_impl.h"

#ifndef _DEFAULT_MALLOC
#include <stdlib.h>
__attribute__((__visibility__("hidden"))) bool g_heapmgr_enable_mcheck = false;
__attribute__((__visibility__("hidden"))) bool g_himalloc_enable_mcheck = false;
#endif

static void dummy(void) {}
weak_alias(dummy, _init);
weak_alias(dummy, __reclaim_hmldso);

extern weak hidden void (*const __init_array_start)(void), (*const __init_array_end)(void);
extern weak hidden void (*const __preinit_array_start)(void), (*const __preinit_array_end)(void);

static void dummy1(void *p) {}
weak_alias(dummy1, __init_ssp);

#define AUX_CNT 38

#ifdef __GNUC__
__attribute__((__noinline__))
#endif
void __init_libc(char **envp, char *pn)
{
	size_t i, *auxv, aux[AUX_CNT] = { 0 };
	__environ = envp;
	for (i=0; envp[i]; i++);
	libc.auxv = auxv = (void *)(envp+i+1);
	for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
	__hwcap = aux[AT_HWCAP];
	if (aux[AT_SYSINFO]) __sysinfo = aux[AT_SYSINFO];
	libc.page_size = aux[AT_PAGESZ];

	if (!pn) pn = (void*)aux[AT_EXECFN];
	if (!pn) pn = "";
	__progname = __progname_full = pn;
	for (i=0; pn[i]; i++) if (pn[i]=='/') __progname = pn+i+1;

	__init_tls(aux);
	__init_ssp((void *)aux[AT_RANDOM]);
	__reclaim_hmldso();

	if (aux[AT_UID]==aux[AT_EUID] && aux[AT_GID]==aux[AT_EGID]
		&& !aux[AT_SECURE]) return;

	/* Note: some codes are deleted because they are not support now.
	 * When we support, we need to add back, see musl.
	 */

	libc.secure = 1;
}

static void libc_start_init(void)
{
	/* For dynamically linked executable, the preinit array is
	 * executed by ldso. And the preinit array should be executed
	 * before _init. */
	uintptr_t pa = (uintptr_t)&__preinit_array_start;
	for (; pa<(uintptr_t)&__preinit_array_end; pa+=sizeof(void(*)()))
		(*(void (**)(void))pa)();

	_init();
	uintptr_t a = (uintptr_t)&__init_array_start;
	for (; a<(uintptr_t)&__init_array_end; a+=sizeof(void(*)()))
		(*(void (**)(void))a)();
}

weak_alias(libc_start_init, __libc_start_init);

void child_clean_ctor(void)
{

}

weak_alias(child_clean_ctor, __child_clean_ctor);

typedef int lsm2_fn(int (*)(int,char **,char **), int, char **, void (*)());
static lsm2_fn libc_start_main_stage2;

int __libc_start_main(int (*main)(int,char **,char **), int argc, char **argv,
	void (*init)(), void(*fini)(), void(*posthook)())
{
	char **envp = argv+argc+1;

	/* External linkage, and explicit noinline attribute if available,
	 * are used to prevent the stack frame used during init from
	 * persisting for the entire process lifetime. */
	__init_libc(envp, argv[0]);

	/* Barrier against hoisting application code or anything using ssp
	 * or thread pointer prior to its initialization above. */
	lsm2_fn *stage2 = libc_start_main_stage2;
	__asm__ ( "" : "+r"(stage2) : : "memory" );
	return stage2(main, argc, argv, posthook);
}

static void __child_unlock_stdio(void)
{
	__unlockfile(stdin);
	__unlockfile(stdout);
	__unlockfile(stderr);
}

static int libc_start_main_stage2(int (*main)(int,char **,char **), int argc, char **argv,
				  void (*posthook)())
{
	char **envp = argv+argc+1;

	/*
	 * NOTE: These codes are ugly, we'll explain why we wrote it this way.
	 *
	 * It's all because we have raw_thread in our underlying OS, and we
	 * still want to use libc functions such as printf in raw_thread.
	 *
	 * Libc use internal variables threads_minus_1 and threaded to optimize
	 * lock operation, so lock can be avoided in signal thread scenario,
	 * these variables are changed in pthread_create. However raw_thread
	 * APIs are not in libc and we can't call any libc callbacks in
	 * raw_thread_create for safe dependence consideration, so we can't
	 * change these variables when we actually created some 'threads'.
	 * Then the subsequent libc calls will get race issue because libc
	 * still think it's in signal thread.
	 *
	 * Another way to fix this is to remove these lock optimizations in
	 * libc, but it needs too much changes in musl code, we choose the
	 * simpler way that only need changes here. The ugly side is that it
	 * completely messes up the semantics of these variables, and we
	 * lost these lock optimizations in signal thread scenario.
	 *
	 * The lost of lock optimization is acceptable because it's simply
	 * some atomic instructions and it won't affect any benchmarks.
	 */
	libc.threads_minus_1 = 1;
	libc.threaded = 1;
	/*
	 * libc.need_locks is used to determine whether to handle internal lock
	 * concurrency problem, but it would be set as 1 in pthread_create only
	 * when libc.threads_minus_1 is 0.
	 * So libc.need_locks should be set as 1 to follow modification for
	 * libc.threads_minus_1 here.
	 */
	libc.need_locks = 1;

	posthook();

	pthread_atfork(NULL, NULL, __child_unlock_stdio);
	pthread_atfork(NULL, NULL, __child_clean_ctor);
	/* start to call constructors from elf/so, must be called after posthook,
	 * as posthook will init files, or construtors maybe fail. */
	__libc_start_init();
	/*
	 * Reinit tid_addr for silk support.
	 * Return value (tid) does not change for main thread.
	 */
	(void)__syscall(SYS_set_tid_address, &__thread_list_lock);


#ifndef _DEFAULT_MALLOC
	char *data = getenv("MALLOC_CHECK_");
	if (data != NULL) {
		/* 10 means decimal */
		long n = strtol(data, (char **)NULL, 10);
		if (n > 0L) {
			g_heapmgr_enable_mcheck = true;
			g_himalloc_enable_mcheck = true;
		}
	}
#endif

	/* Pass control to the application */
	exit(main(argc, argv, envp));
	return 0;
}
