#include <features.h>
#include "libc.h"

#define START "_start"
#include "crt_arch.h"

int main();
weak void _init();
weak void _fini();
int __libc_start_main(int (*)(), int, char **,
	void (*)(), void(*)(), void(*)());

extern void hmcrt_prehook(char **envp);
extern void hmcrt_posthook(void);

void _start_c(long *p)
{
	/*
	 * mem layout:
	 *
	 *                  ...
	 * higher addr   ---------
	 *              | AT_NULL |
	 *              |   ...   |
	 *              | auxv[1] |
	 *              | auxv[0] |
	 *               ---------
	 *              |  NULL   |
	 *              |   ...   |
	 *              | envp[1] |
	 *              | envp[0] |
	 *               ---------
	 *              |  NULL   |
	 *              |   ...   |
	 *              | argv[1] |
	 *              | argv[0] |
	 *               ---------
	 *              |  argc   |
	 * lower addr    ---------    <- p
	 */
	int argc = p[0];
	char **argv = (void *)(p+1);

	hmcrt_prehook(argv + argc + 1);
	__libc_start_main(main, argc, argv, _init, _fini, hmcrt_posthook);
}
