/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Implement codes for memory trace
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 27 18:41:17 2020
 */
#include "../src/himalloc/mem_hm.h"
#include "memtrace.h"
#include <mem/dump.h>

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <dlfcn.h>

#define SYM_UNKNOWN           "UNKNOWN"
#define STR_STATE_FREE        "FREE"
#define STR_STATE_USED        "USED"
#define ELF_NAME_MAX          1024
#define DEFAULT_TRACE_MAX_CNT 10000
#define LOG_BUF_MAX           256
#define SYM_NAME_MAX          128
#define DUMP_TRACE_SIG        SIGUSR2
#define DUMP_TYPE_USED        0
#define DUMP_TYPE_ALL         1
#define DEFAULT_DUMP_TYPE     DUMP_TYPE_ALL
#define STRTOL_BASE           10
#define MAX_BT_DEPTH 16

static bool mtrace_at_fork = false;

/*
 * One trace element for a memory unit, which hints
 * a memory segments allocated by malloc or realloc.
 */
struct mtrace_s {
	bool is_free;
	void *ptr;
	size_t size;
	int trace_len;
	uintptr_t traces[MAX_BT_DEPTH];
};

/*
 * A pool for all trace elements for one process.
 */
struct mtrace_pool_s {
	pthread_mutex_t mutex;
	struct mtrace_s *buf;
	int free_pos;
	bool exist;
	bool ready;
	int cnt;
};

/*
 * Signal handle thread
 */
struct signal_thread_s {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	pthread_t thread;
	int signo;
	bool dead;
};

static char g_elf_file[ELF_NAME_MAX];
static struct mtrace_pool_s g_pool;
static struct signal_thread_s g_sig_thread;

__attribute__((format(printf, 1, 2)))
static void mtrace_log(const char *fmt, ...)
{
	char buf[LOG_BUF_MAX];
	va_list ap;
	int cnt;

	va_start(ap, fmt);
	cnt = vsnprintf(buf, LOG_BUF_MAX, fmt, ap);
	va_end(ap);
	if (cnt > 0) {
		(void)write(STDOUT_FILENO, buf, (size_t)(uint32_t)cnt);
	}
}

static inline int mtrace_atoi(const char *str)
{
	long int ret;
	ret = strtol(str, NULL, STRTOL_BASE);
	if (ret > INT_MAX) {
		ret = INT_MAX;
	} else if (ret < INT_MIN) {
		ret = INT_MIN;
	}
	return (int)ret;
}

static inline void lock_mtrace(void)
{
	(void)pthread_mutex_lock(&g_pool.mutex);
}

static inline void unlock_mtrace(void)
{
	(void)pthread_mutex_unlock(&g_pool.mutex);
}

static inline bool is_mtrace_ready(void)
{
	return g_pool.ready;
}

static inline bool is_pool_full(void)
{
	return g_pool.free_pos >= g_pool.cnt;
}

static inline bool is_pool_exist(void)
{
	return g_pool.exist;
}

static inline void enable_mtrace(void)
{
	g_pool.ready = true;
}

static inline void disable_mtrace(void)
{
	g_pool.ready = false;
}

#ifndef HIMALLOC_MEMTRACE
/* print backtraces for call stack */
static void print_stack(const uintptr_t *btraces, int depth)
{
	char buf[SYM_NAME_MAX] = { 0 };
	char *sym = NULL;
	uintptr_t pc;
	int i;

	for (i = 0; i < depth; i++) {
		pc = btraces[i];
		sym = addr2sym(pc, buf, SYM_NAME_MAX);
		if (sym == NULL) {
			mtrace_log("%2d %p in %s\n", i, (void *)pc, SYM_UNKNOWN);
		} else {
			mtrace_log("%2d %p in %s\n", i, (void *)pc, sym);
		}
	}
}

static inline char *str_of_state(bool is_free)
{
	return (is_free ? (char *)STR_STATE_FREE :
		(char *)STR_STATE_USED);
}

static inline void print_mtrace(const struct mtrace_s *t)
{
	mtrace_log("addr: %p, size: 0x%zx, state: %s\n",
		   t->ptr, t->size, str_of_state(t->is_free));
	mtrace_log("call stack:\n");
	print_stack(t->traces, t->trace_len);
	mtrace_log("\n");
}
#endif /* ifndef HIMALLOC_MEMTRACE */

void mtrace_record_malloc(void *p, size_t size)
{
	struct mtrace_s *t = NULL;

	if (!mtrace_at_fork) {
		lock_mtrace();
		if (is_mtrace_ready() && !is_pool_full()) {
			t = &g_pool.buf[g_pool.free_pos];
			t->trace_len = dump_stack(t->traces, MAX_BT_DEPTH);
			t->ptr = p;
			t->size = size;
			t->is_free = false;
			g_pool.free_pos++;
		}
		unlock_mtrace();
	}
}

static void search_and_record_free(const void *p)
{
	struct mtrace_s *t = NULL;
	int i;

	for (i = 0; i < g_pool.free_pos; i++) {
		t = &g_pool.buf[i];
		if ((t->ptr == p) && !t->is_free) {
			t->is_free = true;
			break;
		}
	}
}

void mtrace_record_free(const void *p)
{
	if (!mtrace_at_fork) {
		lock_mtrace();
		if (is_mtrace_ready()) {
			search_and_record_free(p);
		}
		unlock_mtrace();
	}
}

static size_t search_and_get_size(const void *p)
{
	struct mtrace_s *t = NULL;
	size_t size = 0UL;
	int i;

	for (i = 0; i < g_pool.free_pos; i++) {
		t = &g_pool.buf[i];
		if ((t->ptr == p) && !t->is_free) {
			size = t->size;
			break;
		}
	}

	return size;
}

size_t mtrace_get_obj_size(const void *p)
{
	size_t size = 0UL;

	if (!mtrace_at_fork) {
		lock_mtrace();
		if (is_mtrace_ready()) {
			size = search_and_get_size(p);
		}
		unlock_mtrace();
	}

	return size;
}

#ifndef HIMALLOC_MEMTRACE
static void read_dump_info(int *type, int *cnt)
{
	char *env = NULL;
	int t, c;

	t = DEFAULT_DUMP_TYPE;
	env = getenv("MEMTRACE_DUMP_TYPE");
	if (env != NULL) {
		t = mtrace_atoi(env);
		if (t != DUMP_TYPE_USED) {
			t = DEFAULT_DUMP_TYPE;
		}
	}
	c = g_pool.free_pos;
	env = getenv("MEMTRACE_DUMP_CNT");
	if (env != NULL) {
		c = mtrace_atoi(env);
		if (c <= 0) {
			c = g_pool.free_pos;
		}
	}
	*type = t;
	*cnt = c;
}

static void search_and_dump_trace(int type, int cnt)
{
	struct mtrace_s *t = NULL;
	int num = 0;
	int i;

	mtrace_log("============================================\n");
	if (type == DUMP_TYPE_USED) {
		mtrace_log("memory traces for used memory:\n");
	} else {
		mtrace_log("memory traces for used/free memory:\n");
	}
	for (i = 0; i < g_pool.free_pos && num < cnt; i++) {
		t = &g_pool.buf[i];
		if ((type == DUMP_TYPE_USED) && t->is_free) {
			continue;
		}
		mtrace_log("[%d]", ++num);
		print_mtrace(t);
	}
	mtrace_log("============================================\n");
}

static void dump_trace(int type, int cnt)
{
	if (!mtrace_at_fork) {
		lock_mtrace();
		if (is_mtrace_ready()) {
			disable_mtrace();
			search_and_dump_trace(type, cnt);
			enable_mtrace();
		}
		unlock_mtrace();
	}
}
#endif /* ifndef HIMALLOC_MEMTRACE */

#ifdef HIMALLOC_MEMTRACE
#ifdef CONFIG_DEBUG_BUILD
static void do_memleak_detect(void)
{
	int rc;

	lock_mtrace();
	rc = MemHMLeakScan();
	if (rc != 0) {
		mtrace_log("Himalloc memleak scan fail: %d\n", rc);
	}
	MemHMLeakInfoClean();
	unlock_mtrace();
}
#else
static void do_memleak_detect(void)
{
	mtrace_log("Himalloc only supports memory leak detection under debug build.\n");
}
#endif /* CONFIG_DEBUG_BUILD */
#else
static void do_memleak_detect(void)
{
	int type, cnt;

	read_dump_info(&type, &cnt);
	dump_trace(type, cnt);
}
#endif /* HIMALLOC_MEMTRACE */

static void *signal_thread_func(void *arg)
{
	sigset_t mask;
	int rc;

	rc = sigfillset(&mask);
	if (rc >= 0) {
		rc = pthread_sigmask(SIG_SETMASK, &mask, NULL);
		if (rc < 0) {
			mtrace_log("pthread_sigmask failed: %d\n", rc);
		}
	} else {
		mtrace_log("sigfillset failed: %d\n", rc);
	}
	while ((rc >= 0) && !g_sig_thread.dead) {
		(void)pthread_mutex_lock(&g_sig_thread.mutex);
		(void)pthread_cond_wait(&g_sig_thread.cond, &g_sig_thread.mutex);
		(void)pthread_mutex_unlock(&g_sig_thread.mutex);
		if (g_sig_thread.dead) {
			break;
		}
		do_memleak_detect();
	}

	return NULL;
}

static int signal_thread_init(int signo)
{
	int rc;

	g_sig_thread.signo = signo;
	g_sig_thread.dead = false;
	rc = pthread_mutex_init(&g_sig_thread.mutex, NULL);
	if (rc >= 0) {
		rc = pthread_cond_init(&g_sig_thread.cond, NULL);
		if (rc < 0) {
			(void)pthread_mutex_destroy(&g_sig_thread.mutex);
		}
	}
	if (rc >= 0) {
		rc = pthread_create(&g_sig_thread.thread, NULL,
				    signal_thread_func, NULL);
		if (rc < 0) {
			(void)pthread_cond_destroy(&g_sig_thread.cond);
			(void)pthread_mutex_destroy(&g_sig_thread.mutex);
		}
	}

	return rc;
}

static void sig_handler(int signo)
{
	if (signo == g_sig_thread.signo) {
		(void)pthread_mutex_lock(&g_sig_thread.mutex);
		(void)pthread_cond_signal(&g_sig_thread.cond);
		(void)pthread_mutex_unlock(&g_sig_thread.mutex);
	}
}

static int signal_register(void)
{
	struct sigaction sigact;
	char *sig = NULL;
	int signo;
	int rc;

	/* register signal */
	signo = DUMP_TRACE_SIG;
	sig = getenv("MEMTRACE_SIGNO");
	if (sig != NULL) {
		signo = mtrace_atoi(sig);
	}
	if ((signo <= 0) || signo > SIGRTMAX) {
		signo = DUMP_TRACE_SIG;
	}
	sigact.sa_handler = sig_handler;
	sigact.sa_flags = 0;
	rc = sigemptyset(&sigact.sa_mask);
	if (rc >= 0) {
		rc = sigaction(signo, &sigact, NULL);
	}
	if (rc >= 0) {
		/* init signal thread */
		rc = signal_thread_init(signo);
	}

	return rc;
}

static void signal_deregister(void)
{
	g_sig_thread.dead = true;
	(void)pthread_cond_destroy(&g_sig_thread.cond);
	(void)pthread_mutex_destroy(&g_sig_thread.mutex);
}

static int create_mtrace_pool(int cnt)
{
	int rc;

	rc = pthread_mutex_init(&g_pool.mutex, NULL);
	if (rc >= 0) {
		size_t size = (size_t)(uint32_t)cnt;
		g_pool.buf = (struct mtrace_s *)malloc(size * sizeof(struct mtrace_s));
		if (g_pool.buf == NULL) {
			rc = -1;
		}
	}
	if (rc >= 0) {
		g_pool.exist = true;
		g_pool.free_pos = 0;
		g_pool.cnt = cnt;
	}

	return rc;
}

static void destroy_mtrace_pool(void)
{
	free(g_pool.buf);
	g_pool.buf = NULL;
	g_pool.exist = false;
	(void)pthread_mutex_destroy(&g_pool.mutex);
}

static int backtrace_env_create(void)
{
	int rc;

	rc = dump_init();
	if (rc >= 0) {
		rc = load_symbol(g_elf_file);
		if (rc < 0) {
			mtrace_log("failed to load symbol: %d\n", rc);
		}
	} else {
		mtrace_log("dump_init failed: %d\n", rc);
	}

	return rc;
}

static void backtrace_env_destroy(void)
{
	int rc;
	rc = unload_symbol();
	if (rc < 0) {
		mtrace_log("unload symbol failed: %d\n", rc);
	}
}

static int record_cnt_of_mtrace(void)
{
	char *env = NULL;
	int cnt = 0;

	env = getenv("MEMTRACE_MAX_CNT");
	if (env != NULL) {
		cnt = mtrace_atoi(env);
	}
	if (cnt <= 0) {
		cnt = DEFAULT_TRACE_MAX_CNT;
	}

	return cnt;
}

void mtrace_fork_prehook(void)
{
	mtrace_at_fork = true;
	(void)pthread_mutex_lock(&g_pool.mutex);
	(void)pthread_mutex_lock(&g_sig_thread.mutex);
}

void mtrace_fork_posthook(void)
{
	(void)pthread_mutex_unlock(&g_sig_thread.mutex);
	(void)pthread_mutex_unlock(&g_pool.mutex);
	mtrace_at_fork = false;
}

static void global_dump_init(void)
{
	__dump_stack_func = (dump_stack_func_t)dlsym(RTLD_DEFAULT, "dump_stack");
}

__attribute__((constructor))
static int mtrace_init(void)
{
	int cnt = 0;
	int rc;

	rc = (int)readlink("/proc/self/exe", g_elf_file, ELF_NAME_MAX);
	if (rc < 0) {
		mtrace_log("get elf name failed: %d\n", rc);
	}
	if (rc >= 0) {
		cnt = record_cnt_of_mtrace();
		rc = create_mtrace_pool(cnt);
		if (rc < 0) {
			mtrace_log("create_mtrace_pool failed: %d\n", rc);
		}
	}
	if (rc == 0) {
		rc = backtrace_env_create();
		if (rc < 0) {
			destroy_mtrace_pool();
			mtrace_log("backtrace_env_create failed: %d\n", rc);
		}
	}
	if (rc == 0) {
		rc = signal_register();
		if (rc < 0) {
			backtrace_env_destroy();
			destroy_mtrace_pool();
			mtrace_log("signal_register failed: %d\n", rc);
		}
	}
	if (rc == 0) {
		global_dump_init();
		enable_mtrace();
	}

	return rc;
}

#ifdef HIMALLOC_MEMTRACE
__attribute__((destructor))
static void mtrace_deinit(void)
{
	if (is_pool_exist()) {
		disable_mtrace();
		signal_deregister();
		backtrace_env_destroy();
		destroy_mtrace_pool();
	}
}
#else
__attribute__((destructor))
static void mtrace_deinit(void)
{
	if (is_pool_exist()) {
		do_memleak_detect();
		disable_mtrace();
		signal_deregister();
		backtrace_env_destroy();
		destroy_mtrace_pool();
	}
}
#endif /* HIMALLOC_MEMTRACE */
