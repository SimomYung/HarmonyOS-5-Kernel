/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_thread
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019 -0400
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAW_THREAD_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAW_THREAD_H

#include <libhmsync/thread.h>
#include <hmkernel/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAW_THREAD_INHERIT_SCHED 0
#define RAW_THREAD_EXPLICIT_SCHED 1

#define RAW_THREAD_DETACH_MODE 0x1

struct raw_sched_param {
	unsigned int priority;
};

/* Don't export raw thread attr detailed outside */
typedef struct {
	unsigned int inherit;
	unsigned int policy;
	unsigned int prio;
	unsigned int mode;
	uint64_t flags;
	unsigned long stacksize;
	unsigned long guardsize;
	unsigned long stackaddr;
} raw_thread_attr_t;

int raw_thread_attr_init(raw_thread_attr_t *attr);
int raw_thread_attr_destroy(const raw_thread_attr_t *attr);

int raw_thread_attr_setstack(raw_thread_attr_t *attr, const void* stack, size_t size);
int raw_thread_attr_setstacksize(raw_thread_attr_t *attr, size_t size);
int raw_thread_attr_setschedparam(raw_thread_attr_t *attr, const struct raw_sched_param *param);
int raw_thread_attr_getschedparam(const raw_thread_attr_t *attr, struct raw_sched_param *param);
int raw_thread_attr_setguardsize(raw_thread_attr_t *attr, size_t size);
int raw_thread_attr_getinheritsched(const raw_thread_attr_t *attr, int *inherit);
int raw_thread_attr_setinheritsched(raw_thread_attr_t *attr, int inherit);
int raw_thread_attr_getschedpolicy(const raw_thread_attr_t *attr, int *policy);
int raw_thread_attr_setschedpolicy(raw_thread_attr_t *attr, int policy);
int raw_thread_attr_setflags(raw_thread_attr_t *attr, __u64 flags);
int raw_thread_attr_getmode(const raw_thread_attr_t *attr, unsigned int *mode);
int raw_thread_attr_setmode(raw_thread_attr_t *attr, unsigned int mode);

/* Don't export raw_thread detailed outside */
struct raw_thread;
typedef struct raw_thread* raw_thread_t;
int raw_thread_create(raw_thread_t *thread,
		      const raw_thread_attr_t *raw_attr,
		      void *(*func)(void *args),
		      void *args);

int __deprecated raw_thread_terminate(const struct raw_thread *thread);
void raw_thread_exit(void *exitcode);
int raw_thread_join(raw_thread_t thread, void **exitcode);

raw_thread_t raw_thread_self(void);
cref_t raw_thread_cref_of(raw_thread_t thread);
pid_t raw_thread_tid_of(raw_thread_t thread);
unsigned long raw_thread_stack_size_of(raw_thread_t thread);
unsigned long raw_thread_stack_base_of(raw_thread_t thread);
unsigned long raw_thread_actv_stack_size_of(raw_thread_t thread);

int raw_tls_init(void);
size_t raw_thread_tls_size(void);
void *raw_thread_private_of(struct raw_thread *thread);
void raw_thread_set_private(struct raw_thread *thread, void *priv);
struct raw_thread *raw_tls_base_to_thread(unsigned long tls);

typedef unsigned int raw_thread_key_t;
int raw_thread_key_create(raw_thread_key_t *k, void(*dtor)(void *));
int raw_thread_key_delete(raw_thread_key_t k);
int raw_thread_setspecific(raw_thread_key_t k, const void *x);
int raw_thread_target_setspecific(struct raw_thread *thread, raw_thread_key_t k, const void *x);
void *raw_thread_getspecific(raw_thread_key_t k);
void raw_thread_specific_init(unsigned int *tls_offset);

void raw_thread_entry_modifier_init(void);

#ifdef __cplusplus
}
#endif
#endif
