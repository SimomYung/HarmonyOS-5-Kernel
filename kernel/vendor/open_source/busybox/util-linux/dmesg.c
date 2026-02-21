/* vi: set sw=4 ts=4: */
/*
 *
 * dmesg - display/control kernel ring buffer.
 *
 * Copyright 2006 Rob Landley <rob@landley.net>
 * Copyright 2006 Bernhard Reutner-Fischer <rep.nop@aon.at>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */
//config:config DMESG
//config:	bool "dmesg (3.7 kb)"
//config:	default y
//config:	help
//config:	dmesg is used to examine or control the kernel ring buffer. When the
//config:	Linux kernel prints messages to the system log, they are stored in
//config:	the kernel ring buffer. You can use dmesg to print the kernel's ring
//config:	buffer, clear the kernel ring buffer, change the size of the kernel
//config:	ring buffer, and change the priority level at which kernel messages
//config:	are also logged to the system console. Enable this option if you
//config:	wish to enable the 'dmesg' utility.
//config:
//config:config FEATURE_DMESG_PRETTY
//config:	bool "Pretty output"
//config:	default y
//config:	depends on DMESG
//config:	help
//config:	If you wish to scrub the syslog level from the output, say 'Y' here.
//config:	The syslog level is a string prefixed to every line with the form
//config:	"<#>".
//config:
//config:	With this option you will see:
//config:		# dmesg
//config:		Linux version 2.6.17.4 .....
//config:		BIOS-provided physical RAM map:
//config:		 BIOS-e820: 0000000000000000 - 000000000009f000 (usable)
//config:
//config:	Without this option you will see:
//config:		# dmesg
//config:		<5>Linux version 2.6.17.4 .....
//config:		<6>BIOS-provided physical RAM map:
//config:		<6> BIOS-e820: 0000000000000000 - 000000000009f000 (usable)
//config:
//config:config FIX_DMESG_PRETTY_OUTPUT
//config:	bool "solve problem in dmesg_pretty"
//config:	default n
//config:	depends on DMESG
//config:	help
//config:	Add busybox bugfix patches to fix bug in dmesg_pretty.

//applet:IF_DMESG(APPLET(dmesg, BB_DIR_BIN, BB_SUID_DROP))

//kbuild:lib-$(CONFIG_DMESG) += dmesg.o

//usage:#define dmesg_trivial_usage
//usage:       "[-cr] [-n LEVEL] [-s SIZE]"
//usage:#define dmesg_full_usage "\n\n"
//usage:       "Print or control the kernel ring buffer\n"
//usage:     "\n	-c		Clear ring buffer after printing"
//usage:     "\n	-n LEVEL	Set console logging level"
//usage:     "\n	-s SIZE		Buffer size"
//usage:     "\n	-r		Print raw message buffer"

#include <sys/klog.h>
#include "libbb.h"

#ifdef __hongmeng
#include <libhmlog/hmlog.h>

/* dmesg is a dedicated log tool for Linux,
 * Linux log has 8 levels, while hm log only has 5 levels,
 * try to maintain the behavior of dmesg
 * as consistent as possible with the Linux manual
 */
static int transform_hm_level(int level)
{
	int ret;
	switch (level) {
	case 0: /* KERN_EMERG */
	case 1: /* KERN_ALERT */
	case 2: /* KERN_CRIT */
		ret = HMLOG_PANIC;
		break;
	case 3: /* KERN_ERR */
		ret = HMLOG_ERROR;
		break;
	case 4: /* KERN_WARNING */
		ret = HMLOG_WARN;
		break;
	case 5: /* KERN_NOTICE */
	case 6: /* KERN_INFO */
		ret = HMLOG_INFO;
		break;
	case 7: /* KERN_DEBUG */
		ret = HMLOG_DEBUG;
		break;
	default:
		ret = level;
	}

	return ret;
}

static void usage(void)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, " dmesg [options]\n\n");
	fprintf(stderr, "Display or control the kernel ring buffer.\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "%-14sClear buffer after kernel messages printed.\n", " -c");
	fprintf(stderr, "%-14sSet level of kernel messages printed to console. LEVEL "
			"values 0~7.\n", " -n LEVEL");
	fprintf(stderr, "%-14sRelationship between LEVEL and hmlog level described"
			" as follows:\n", "");
	fprintf(stderr, "%-16sLEVEL   HM_Level\n", "");
	fprintf(stderr, "%-16s%-5s   %s\n", "", "0~2",
			hmlog_name_of_level(HMLOG_PANIC));
	fprintf(stderr, "%-16s%-5s   %s\n", "", "3",
			hmlog_name_of_level(HMLOG_ERROR));
	fprintf(stderr, "%-16s%-5s   %s\n", "", "4",
			hmlog_name_of_level(HMLOG_WARN));
	fprintf(stderr, "%-16s%-5s   %s\n", "", "5~6",
			hmlog_name_of_level(HMLOG_INFO));
	fprintf(stderr, "%-16s%-5s   %s\n", "", "7",
			hmlog_name_of_level(HMLOG_DEBUG));
	fprintf(stderr, "%-14sAlloc a buffer to query kernel messages. Relationship"
			" between SIZE \n", " -s SIZE");
	fprintf(stderr, "%-14sand actual buffer size described as follows:\n",
			"");
	fprintf(stderr, "%-16sCases         Actual_Buffer_Size\n", "");
	fprintf(stderr, "%-16sSIZE < 16KB   16KB\n", "");
	fprintf(stderr, "%-16sSIZE > 16MB   16MB\n", "");
	fprintf(stderr, "%-16sOtherwise     SIZE\n", "");
}
#endif

int dmesg_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int dmesg_main(int argc UNUSED_PARAM, char **argv)
{
	int len, level;
	char *buf;
	unsigned opts;
	enum {
		OPT_c = 1 << 0,
		OPT_s = 1 << 1,
		OPT_n = 1 << 2,
#ifdef __hongmeng
		OPT_r = 1 << 3,
		OPT_h = 1 << 4
#else
		OPT_r = 1 << 3
#endif
	};

#ifdef __hongmeng
	opts = getopt32(argv, "cs:+n:+rh", &len, &level);
	if (opts & OPT_h) {
		usage();
		return EXIT_SUCCESS;
	}
#else
	opts = getopt32(argv, "cs:+n:+r", &len, &level);
#endif
	if (opts & OPT_n) {
#ifdef __hongmeng
		/* range of log level value: 0~7 */
		if ((level >= 0) && (level <= 7))
			level = transform_hm_level(level);
#endif
		if (klogctl(8, NULL, (long) level))
			bb_simple_perror_msg_and_die("klogctl");
		return EXIT_SUCCESS;
	}

	if (!(opts & OPT_s))
		len = klogctl(10, NULL, 0); /* read ring buffer size */
	if (len < 16*1024)
		len = 16*1024;
	if (len > 16*1024*1024)
		len = 16*1024*1024;

	buf = xmalloc(len);
	len = klogctl(3 + (opts & OPT_c), buf, len); /* read ring buffer */
	if (len < 0)
		bb_simple_perror_msg_and_die("klogctl");
	if (len == 0)
		return EXIT_SUCCESS;


	if (ENABLE_FEATURE_DMESG_PRETTY && !(opts & OPT_r)) {
		int last = '\n';
		int in = 0;

		/* Skip <[0-9]+> at the start of lines */
		while (1) {
			if (last == '\n' && buf[in] == '<') {
				while (buf[in++] != '>' && in < len)
					;
#if ENABLE_FIX_DMESG_PRETTY_OUTPUT
				last = buf[in];
#endif
			} else {
				last = buf[in++];
				putchar(last);
			}
			if (in >= len)
				break;
		}
		/* Make sure we end with a newline */
		if (last != '\n')
			bb_putchar('\n');
	} else {
		full_write(STDOUT_FILENO, buf, len);
		if (buf[len-1] != '\n')
			bb_putchar('\n');
	}

	if (ENABLE_FEATURE_CLEAN_UP) free(buf);

	return EXIT_SUCCESS;
}
