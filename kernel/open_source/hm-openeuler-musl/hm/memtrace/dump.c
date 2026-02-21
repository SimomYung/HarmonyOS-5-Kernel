/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Implement for interfaces of backtrace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 30 09:56:14 2020
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MEMTRACE_ARM

#include <hmsyscall.h>
#include <hmpthread.h>
#include <pthread.h>
#include <unistd.h>

#define MEM_HM_RESOURCEBUF_LENGTH 1024      /* buffer length used to read file content */
#define MEM_HM_READ_SIZE 11
#define ADDR_SIZE_MODEL_32 4
#define STACKFRAME_OFFSET_APCS_IP (2 * ADDR_SIZE_MODEL_32)
#define STACKFRAME_OFFSET_APCS_FP (3 * ADDR_SIZE_MODEL_32)

/* structure of each line in the maps info */
typedef struct {
    unsigned long beginAddr;            /* start addr of segment */
    unsigned long endAddr;              /* end addr of segment */
    unsigned long excurs;
    uint32_t mainEquip;             /* main device No. */
    uint32_t childEquip;            /* child device No. */
    uint32_t node;                  /* the file inode number of the current executable */
    char readAble;                  /* is readable */
    char writeAble;                 /* is writeable */
    char excuteAble;                /* is executable */
    char private;                   /* is private */
    char *notes;                    /* the content of the current line */
} MemHMMapsInfo;

struct stack_frame_s {
	uintptr_t start;
	uintptr_t end;
	pid_t tid;
	bool init;
};

enum frame_type {
	/* stackframe compliant with apcs, which store pc/lr/ip/fp at frame start */
	ARM_STACKFRAME_APCS = 0,

	/* stackframe that store lr/fp at frame start */
	ARM_STACKFRAME_COMMON1 = 1,

	/* stackframe that store fp at frame start */
	ARM_STACKFRAME_COMMON2 = 2,
	ARM_STACKFRAME_UNKNOWN = 3,
};

static struct stack_frame_s g_main_stack = {
	.start = 0UL,
	.end = 0UL,
	.init = false,
	.tid = 0,
};

static inline uintptr_t fp_of_current(void)
{
	uintptr_t fp = 0UL;
	asm("mov %0, fp" : "=r" (fp) :);
	return fp;
}

static int acquire_stack_end(uintptr_t *stack_end)
{
	FILE *fp = NULL;
	pid_t pid;
	int ret = 0;
	int err = -1;
	MemHMMapsInfo mapsInfo = {0};
	char buffer[MEM_HM_RESOURCEBUF_LENGTH] = {0};
	char notes[MEM_HM_RESOURCEBUF_LENGTH] = {0};

	pid = (uint32_t)getpid();
	(void)snprintf(buffer, MEM_HM_RESOURCEBUF_LENGTH, "/proc/%u/maps", pid);
	fp = fopen(buffer, "r");
	if (fp != NULL) {
		while (fgets(buffer, MEM_HM_RESOURCEBUF_LENGTH, fp) != NULL) {
			(void)memset(notes, 0, MEM_HM_RESOURCEBUF_LENGTH - 1);
			mapsInfo.notes = notes;
			ret = sscanf(buffer, "%lx-%lx %c%c%c%c %lx %x:%x %d %1023s", &mapsInfo.beginAddr,
				     &mapsInfo.endAddr, &mapsInfo.readAble, &mapsInfo.writeAble,
				     &mapsInfo.excuteAble, &mapsInfo.private, &mapsInfo.excurs,
				     &mapsInfo.mainEquip, &mapsInfo.childEquip, &mapsInfo.node, mapsInfo.notes);
			if ((ret == MEM_HM_READ_SIZE) && (mapsInfo.readAble == 'r') && (mapsInfo.writeAble == 'w') &&
			    (mapsInfo.notes != NULL) && (strcmp("[stack]", mapsInfo.notes) == 0)) {
				*stack_end = (uintptr_t)mapsInfo.endAddr;
				err = 0;
			}
		}
	}
	(void)fclose(fp);

	return err;
}

int dump_init(void)
{
	int err = 0;
	uintptr_t stack_end = 0UL;

	if (!g_main_stack.init) {
		g_main_stack.tid = pthread_tid(pthread_self());
		err = acquire_stack_end(&stack_end);
		if (err == 0) {
			g_main_stack.end = stack_end;
			g_main_stack.init = true;
		}
	}

	return err;
}

static inline bool is_in_frame(const struct stack_frame_s *frame, uintptr_t addr)
{
	return ((addr >= frame->start) && (addr <= frame->end));
}

static bool is_frame_apcs(const struct stack_frame_s *frame, uintptr_t fp)
{
	bool is_apcs = false;
	uintptr_t pc, ip;

	ip = *(uintptr_t *)(fp - STACKFRAME_OFFSET_APCS_IP);
	pc = *(uintptr_t *)(fp - STACKFRAME_OFFSET_APCS_FP);

	if (is_in_frame(frame, ip) && ip > fp &&
	    (pc == 0UL || (is_in_frame(frame, pc) && pc >= ip))) {
		is_apcs = true;
	}
	return is_apcs;
}

static bool is_stackframe_common1(const struct stack_frame_s *frame, uintptr_t fp)
{
	bool is_common = false;
	uintptr_t pc;

	pc = *(uintptr_t *)(fp - ADDR_SIZE_MODEL_32);
	if (pc == 0 || (is_in_frame(frame, pc) && pc > fp)) {
		is_common = true;
	}
	return is_common;
}

static bool is_stackframe_common2(const struct stack_frame_s *frame, uintptr_t fp)
{
	bool is_common = false;
	uintptr_t pc;

	pc = *(uintptr_t *)fp;
	if (is_in_frame(frame, pc) && pc > fp) {
		is_common = true;
	}
	return is_common;
}

static enum frame_type guess_stackframe_type(const struct stack_frame_s *frame, uintptr_t fp)
{
	enum frame_type tp;

	if (fp == 0UL) {
		tp = ARM_STACKFRAME_UNKNOWN;
	} else if (is_frame_apcs(frame, fp)) {
		tp = ARM_STACKFRAME_APCS;
	} else if (is_stackframe_common2(frame, fp)) {
		/*
		 * STACKFRAME_COMMON2 is more special than STACKFRAME_COMMON1,
		 * so we should first check if it is type of STACKFRAME_COMMON2
		 */
		tp = ARM_STACKFRAME_COMMON2;
	} else if (is_stackframe_common1(frame, fp)) {
		tp = ARM_STACKFRAME_COMMON1;
	} else {
		tp = ARM_STACKFRAME_UNKNOWN;
	}

	return tp;
}

static uintptr_t __dump_stack(const struct stack_frame_s *frame, uintptr_t fp)
{
	enum frame_type type;
	uintptr_t rc = 0UL;

	if ((fp >= frame->start) && (fp < frame->end)) {
		type = guess_stackframe_type(frame, fp);
		switch (type) {
		case ARM_STACKFRAME_APCS:
			rc = *(uintptr_t *)(fp - STACKFRAME_OFFSET_APCS_FP);
			break;
		case ARM_STACKFRAME_COMMON1:
			rc = *(uintptr_t *)(fp - ADDR_SIZE_MODEL_32);
			break;
		case ARM_STACKFRAME_COMMON2:
			rc = *(uintptr_t *)(fp);
			break;
		default:
			rc = 0UL;
			break;
		}
	}

	return rc;
}

int dump_stack(uintptr_t *buf, int max)
{
	struct stack_frame_s frame = {0};
	int depth = 0;
	int err = 0;
	uintptr_t fp;

	if ((buf == NULL) || !g_main_stack.init) {
		err = -1;
	}
	if (err == 0) {
		fp = fp_of_current();
		if (fp == 0UL) {
			err = -1;
		}
	}
	if (err == 0) {
		frame.tid = pthread_tid(pthread_self());
		if (frame.tid == g_main_stack.tid) {
			frame.end = g_main_stack.end;
		} else {
			frame.end = (uintptr_t)pthread_stack(pthread_self());
		}
		frame.start = fp;

		/* Skip stack of current function */
		fp = __dump_stack(&frame, fp);
		if (fp == 0UL) {
			err = -1;
		}
	}
	if (err == 0) {
		for (depth = 0; depth < max; depth++) {
			fp = __dump_stack(&frame, fp);
			if (fp == 0UL) {
				break;
			}
			buf[depth] = *(uintptr_t *)fp;
		}
	}

	return depth;
}
#else /* MEMTRACE_ARM */

#define get_sp(sp)	asm("mov %0, sp" : "=r" (sp) :)

static inline uintptr_t sp_of_current(void)
{
	uintptr_t sp = 0UL;
	get_sp(sp);
	return sp;
}

int dump_init(void)
{
	return 0;
}

int dump_stack(uintptr_t *buf, int max)
{
	uintptr_t fp = 0UL;
	int depth = 0;

	if (buf != NULL) {
		fp = sp_of_current();
	}
	if (fp != 0UL) {
		for (depth = 0; depth < max; depth++) {
			buf[depth] = *((uintptr_t *)(fp + sizeof(uintptr_t)));
			fp = *(uintptr_t *)fp;
			if (fp == 0UL) {
				break;
			}
		}
	}

	return depth;
}
#endif /* MEMTRACE_ARM */

/*
 * Functions for symbol conversion for memtrace
 */
#ifdef COMPILE_SHARE

#include <dlfcn.h>

int load_symbol(const char *name)
{
	return 0;
}

int unload_symbol(void)
{
	return 0;
}

char *addr2sym(uintptr_t addr, char *sym, int max)
{
	Dl_info info;
	int ret;

	ret = dladdr((void *)addr, &info);
	if (ret != 0 && info.dli_sname != NULL && max >= 0) {
		sym = strncpy(sym, info.dli_sname, (size_t)(unsigned int)max);
	} else {
		sym = NULL;
	}
	return sym;
}

#else

#include <unistd.h>
/*
 * Only print symbols in shared library
 */

int load_symbol(const char *name)
{
	return 0;
}

int unload_symbol(void)
{
	return 0;
}

char *addr2sym(uintptr_t addr, char *sym, int max)
{
	return NULL;
}
#endif /* COMPILE_SHARE */
