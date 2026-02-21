/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: stub api
 */
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>

int getcontext(ucontext_t *ucp)
{
	(void)ucp;
	return -1;
}

void makecontext(ucontext_t *ucp, void (*func)(void), int argc, ...)
{
	(void)ucp;
	(void)func;
	(void)argc;
	return;
}

int setcontext(const ucontext_t *ucp)
{
	(void)ucp;
	return -1;
}

int pthread_sigqueue(pthread_t thread, int sig, const union sigval value)
{
	(void)thread;
	(void)sig;
	(void)value;
	return -1;
}

/* permissive-cross-dso with info. of caller */
void hcfi_slowpath_permissive(const char *loc_file, unsigned int loc_line, const char *current_func, void *ptr)
{
	(void)loc_file;
	(void)loc_line;
	(void)current_func;
	(void)ptr;
	return;
}

/*
 * permissive-cross-dso to print log anyway.
 * note that we cannot modify GLRO(dl_debug_mask) directly.
 * print MAX_OUTPUT_INFO_TIMES unique logs, this function can be ignored
 * if 'LD_DEBUG=cross-dso' can be sent to the main executable.
 */
void hcfi_slowpath_permissive_always_print(
	const char *loc_file, unsigned int loc_line, const char *current_func, void *ptr)
{
	(void)loc_file;
	(void)loc_line;
	(void)current_func;
	(void)ptr;
	return;
}

/* In our scheme, pass ptr as callee address */
void hcfi_slowpath_enforcing(void *ptr)
{
	(void)ptr;
	return;
}
