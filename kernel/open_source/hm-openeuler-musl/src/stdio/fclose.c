#include "stdio_impl.h"
#include <stdlib.h>

static void dummy(FILE *f) { }
weak_alias(dummy, __unlist_locked_file);

int fclose(FILE *f)
{
	int r;
	FILE **head = __ofl_lock();

	FLOCK(f);
	r = fflush(f);
	r |= f->close(f);
	FUNLOCK(f);

	if (f->flags & F_PERM) {
		__ofl_unlock();
		return r;
	}

	__unlist_locked_file(f);

	if (f->prev) f->prev->next = f->next;
	if (f->next) f->next->prev = f->prev;
	if (*head == f) *head = f->next;
	__ofl_unlock();

	free(f->getln_buf);
	free(f);

	return r;
}
