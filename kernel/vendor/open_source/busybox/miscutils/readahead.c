/* vi: set sw=4 ts=4: */
/*
 * readahead implementation for busybox
 *
 * Preloads the given files in RAM, to reduce access time.
 * Does this by calling the readahead(2) system call.
 *
 * Copyright (C) 2006  Michael Opdenacker <michael@free-electrons.com>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */
//config:config READAHEAD
//config:	bool "readahead (1.5 kb)"
//config:	default y
//config:	depends on LFS
//config:	help
//config:	Preload the files listed on the command line into RAM cache so that
//config:	subsequent reads on these files will not block on disk I/O.
//config:
//config:	This applet just calls the readahead(2) system call on each file.
//config:	It is mainly useful in system startup scripts to preload files
//config:	or executables before they are used. When used at the right time
//config:	(in particular when a CPU bound process is running) it can
//config:	significantly speed up system startup.
//config:
//config:	As readahead(2) blocks until each file has been read, it is best to
//config:	run this applet as a background job.

//applet:IF_READAHEAD(APPLET(readahead, BB_DIR_USR_SBIN, BB_SUID_DROP))

//kbuild:lib-$(CONFIG_READAHEAD) += readahead.o
//usage:#if ENABLE_FEATURE_OFFSET_READAHEAD
//usage:#define readahead_trivial_usage
//usage:       "[FILE]...\n"
//usage:       "[FILE] -r OFFSET,LEN"
//usage:#define readahead_full_usage "\n\n"
//usage:       "Preload FILEs to RAM\n"
//usage:       "The command format cannot be mixed, for example, '[FILE1] [FILE2] -r OFFSET,LEN'.\n"
//usage:       "-r		Specifies a range of the file for preloading."
//usage:#else
//usage:#define readahead_trivial_usage
//usage:       "[FILE]..."
//usage:#define readahead_full_usage "\n\n"
//usage:       "Preload FILEs to RAM"
//usage:#endif

#include "libbb.h"

#if ENABLE_FEATURE_OFFSET_READAHEAD
static int readahead_offest(char *file, char *range);
#endif

int readahead_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
#if ENABLE_FEATURE_OFFSET_READAHEAD
int readahead_main(int argc , char **argv)
#else
int readahead_main(int argc UNUSED_PARAM, char **argv)
#endif
{
	int retval = EXIT_SUCCESS;

	if (!argv[1]) {
		bb_show_usage();
	}

#if ENABLE_FEATURE_OFFSET_READAHEAD
	const int OFFSET_OPT = (1 << 0);
	char *range;
    char **argp = argv;
	char *file = *++argp;
	getopt32(argv, "r:", &range);

	if (option_mask32 & OFFSET_OPT) {
		if (range == NULL) {
			bb_show_usage();
		}
		if (readahead_offest(file, range) != 0) {
			retval = EXIT_FAILURE;
			return retval;
		}
		return retval;
	}
#endif

    while (*++argv) {
		int fd = open_or_warn(*argv, O_RDONLY);
		if (fd >= 0) {
			off_t len;
			int r;

			/* fdlength was reported to be unreliable - use seek */
#if ENABLE_FEATURE_NOFORK_READAHEAD
			len = lseek(fd, 0, SEEK_END);
			off_t ret = lseek(fd, 0, SEEK_SET);
			if ((len == (off_t)-1) || (ret == (off_t)-1)) {
				bb_perror_msg("lseek in readahead dies\n");
				close(fd);
				retval = EXIT_FAILURE;
				return retval;
			}
#else
			len = xlseek(fd, 0, SEEK_END);
			xlseek(fd, 0, SEEK_SET);
#endif

			r = readahead(fd, 0, len);
			close(fd);
			if (r >= 0)
				continue;
		}
		retval = EXIT_FAILURE;
	}
	return retval;
}

#if ENABLE_FEATURE_OFFSET_READAHEAD
static int readahead_offest(char *file, char *range)
{
	int retval = EXIT_SUCCESS;
	char *token = strtok(range, ",");
    int i = 0;
    char *arg_array[2] = {0};
    while (token != NULL) {
        if (i >= 2) {
            bb_show_usage();
        }
        arg_array[i++] = token;
        token = strtok(NULL, ",");
    }

	int fd = open_or_warn(file, O_RDONLY);
	if (fd < 0) {
		retval = EXIT_FAILURE;
		return retval;
	}

	off_t offset, len;
	/* If offset is not given, read the whole file */
	if (arg_array[0] != '\0') {
		offset = (off_t) atoi(arg_array[0]);
	} else {
        offset = 0;
	}

#if ENABLE_FEATURE_NOFORK_READAHEAD
	off_t file_len = lseek(fd, 0, SEEK_END);
	off_t ret = lseek(fd, 0, SEEK_SET);
	if ((file_len == (off_t)-1) || (ret == (off_t)-1)) {
		bb_perror_msg("lseek in readahead dies\n");
		close(fd);
		retval = EXIT_FAILURE;
		return retval;
	}
#else
	off_t file_len = xlseek(fd, 0, SEEK_END);
	xlseek(fd, 0, SEEK_SET);
#endif

	/* If the length is not given or exceeds the file length, read to the file length */
	if (arg_array[1] != '\0') {
		len = (off_t) atoi(arg_array[1]);
		if (len > file_len) {
			len = file_len;
		}
	} else {
		len = file_len;
	}

	int r = readahead(fd, offset, len);
	close(fd);
	if (r < 0)
		retval = EXIT_FAILURE;

	return retval;
}
#endif