// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2010  Red Hat, Inc.
 */



#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include "tst_test.h"
#include "tst_safe_stdio.h"

#define KB 1024

static void verify_mlock(void)
{
	long from, to;
	long first = -1, last = -1;
	char b[KB];
	FILE *fp;

	fp = SAFE_FOPEN("/proc/self/maps", "r");
	while (!feof(fp)) {
		if (!fgets(b, KB - 1, fp))
			break;
		b[strlen(b) - 1] = '\0';
		if (sscanf(b, "%lx-%lx", &from, &to) != 2) {
			tst_brk(TBROK, "parse %s start and end address failed",
					b);
			continue;
		}

		/* Record the initial stack size. */
		if (strstr(b, "[stack]") != NULL)
			first = (to - from) / KB;

		tst_res(TINFO, "mlock [%lx,%lx]", from, to);
		if (mlock((const void *)from, to - from) == -1)
			tst_res(TINFO | TERRNO, "mlock failed");

		tst_res(TINFO, "munlock [%lx,%lx]", from, to);
		if (munlock((void *)from, to - from) == -1)
			tst_res(TINFO | TERRNO, "munlock failed");

		/* Record the final stack size. */
		if (strstr(b, "[stack]") != NULL)
			last = (to - from) / KB;
	}
	SAFE_FCLOSE(fp);

	tst_res(TINFO, "starting stack size is %ld", first);
	tst_res(TINFO, "final stack size is %ld", last);
	if (last < first)
		tst_res(TFAIL, "stack size is decreased.");
	else
		tst_res(TPASS, "stack size is not decreased.");
}

static struct tst_test test = {
	.test_all = verify_mlock,
};
