/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Cpio
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 19 02:05:08 2018
 */
#ifndef KLIBS_CPIO_H
#define KLIBS_CPIO_H

/* Magic identifiers for the CPIO entry format */
#define CPIO_HEADER_MAGIC "070701"
#define CPIO_HEADER_MAGIC_LEN 6U

/* File name for EOF of CPIO archive */
#define CPIO_FOOTER_MAGIC "TRAILER!!!"
#define CPIO_FOOTER_MAGIC_LEN 10U

#define CPIO_ALIGNMENT 4UL

struct cpio_header {
	char c_magic[6];	/* Magic header '070701' for files */
	char c_ino[8];		/* Inode number */
	char c_mode[8];		/* File type and mode */
	char c_uid[8];		/* User ID of owner */
	char c_gid[8];		/* Group ID of owner */
	char c_nlink[8];	/* Number of hard links */
	char c_mtime[8];	/* Time of last modification */
	char c_filesize[8];	/* Total size of file, in bytes */
	char c_devmajor[8];	/* Major device number */
	char c_devminor[8];	/* Minor device number */
	char c_rdevmajor[8];	/* Major device ID */
	char c_rdevminor[8];	/* Minor device ID */
	char c_namesize[8];	/* Length of filename, in bytes */
	char c_check[8];	/* Checksum */
};

struct cpio_fileinfo_s {
	const char *filename;
	unsigned long filesize;
	void *filedata;
};

/*
 * Parse the header of the given CPIO entry
 *
 * @archive: The location of the CPIO archive
 * @cpioend: The end location of the CPIO archive
 * @fileinfo: The struct to store CPIO file information
 * @nextfile: Pointer to next CPIO entry
 *
 * Return 0 if success, -1 if the header is not valid, 1 if EOF.
 */
int cpio_parse_entry(struct cpio_header *archive, void *cpioend,
		     struct cpio_fileinfo_s *fileinfo,
		     struct cpio_header **nextfile);
unsigned long hex_to_ulong(const char *str, unsigned int len);
#endif /* _LIB_CPIO_H_ */
