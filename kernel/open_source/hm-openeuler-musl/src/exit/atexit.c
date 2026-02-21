#include <stdlib.h>
#include <stdint.h>
#include "libc.h"
#include "lock.h"
#include "fork_impl.h"

/* Ensure that at least 32 atexit handlers can be registered without malloc */
#define COUNT 32

enum {
    EF_AT,
    EF_ON
};

static struct fl
{
	struct fl *next;
	void (*f[COUNT])(void *);
	void *a[COUNT];
    void (*f_on[COUNT])(int, void *);
    void *f_arg[COUNT];  // 2th paramter of on_exit
    int flavor[COUNT];  // 1 - on_exit, 0 - atexit
} builtin, *head;

static int slot;
static volatile int lock[1];
volatile int *const __atexit_lockptr = lock;

void __funcs_on_exit(int status)
{
	LOCK(lock);
	for (; head; head=head->next, slot=COUNT) while(slot-->0) {
		if (head->flavor[slot] == EF_ON) {
			void *f_arg;
			void (*onfunc)(int, void *);
			onfunc = head->f_on[slot];
			f_arg = head->f_arg[slot];
			UNLOCK(lock);
			onfunc(status, f_arg);
			LOCK(lock);
		} else {
			void (*func)(void *), *arg;
			func = head->f[slot];
			arg = head->a[slot];
			UNLOCK(lock);
			func(arg);
			LOCK(lock);
			(void)status;
		}
	}
}

void __cxa_finalize(void *dso)
{
}

int __cxa_atexit(void (*func)(void *), void *arg, void *dso)
{
	LOCK(lock);

	/* Defer initialization of head so it can be in BSS */
	if (!head) head = &builtin;

	/* If the current function list is full, add a new one */
	if (slot==COUNT) {
		struct fl *new_fl = calloc(sizeof(struct fl), 1);
		if (!new_fl) {
			UNLOCK(lock);
			return -1;
		}
		new_fl->next = head;
		head = new_fl;
		slot = 0;
	}

	/* Append function to the list. */
	head->f[slot] = func;
	head->a[slot] = arg;
	slot++;

	UNLOCK(lock);
	return 0;
}

static void call(void *p)
{
	((void (*)(void))(uintptr_t)p)();
}

int atexit(void (*func)(void))
{
	return __cxa_atexit(call, (void *)(uintptr_t)func, 0);
}

int on_exit(void (*func)(int, void*), void* arg)
{
    LOCK(lock);

    if (!head) head = &builtin;

    if (slot == COUNT) {
        struct fl *new_fl = calloc(sizeof(struct fl), 1);
        if (!new_fl) {
            UNLOCK(lock);
            return -1;
        }
        new_fl->next = head;
        head = new_fl;
        slot = 0;
    }

    head->f_on[slot] = func;
    head->f_arg[slot] = arg;
    head->flavor[slot] = EF_ON;
    slot++;

    UNLOCK(lock);
    return 0;
}
